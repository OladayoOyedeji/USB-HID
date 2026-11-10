#include "i2c.h"

void i2c_init(struct i2c * i2c)
{
    uint16_t sda = 0;
    uint16_t scl = 0;
    uint8_t scl_af = 4, sda_af = 4;
    if (i2c == I2C1)
    {
        RCC->APB1ENR_b.I2C1EN = 1;
        sda = PIN('B', 9);
        scl = PIN('B', 8);
    }
    else if (i2c == I2C2)
    {
        RCC->APB1ENR_b.I2C2EN = 1;
        scl = PIN('B', 10);
        sda = PIN('B', 3); sda_af = 9;
    }
    else if (i2c == I2C3)
    {
        RCC->APB1ENR_b.I2C3EN = 1;
        scl = PIN('A', 8);
        sda = PIN('C', 9);
    
    }
    

    gpio_set_mode(sda, GPIO_MODE_AF);
    gpio_set_mode(scl, GPIO_MODE_AF);
    gpio_set_af(sda, sda_af);
    gpio_set_af(scl, scl_af);
    gpio_set_open_drain(scl);
    gpio_set_open_drain(sda);
    gpio_set_pull_up(sda);
    gpio_set_pull_up(scl);

    i2c->CR1_b.PE = 0;
    i2c->CR1_b.SWRST = 1;
    i2c->CR1_b.SWRST = 0;

    // Timing (assumes APB1 = 16 MHz, standard mode 100 kHz)
    i2c->CR2_b.FREQ = APB1_FREQUENCY / 1000000;;       // APB1 clock in MHz
    i2c->CCR_b.CCR = APB1_FREQUENCY / (2 * 100000);        // 16 MHz / (2 * 100 kHz)
    i2c->TRISE_b.TRISE = (APB1_FREQUENCY / 1000000) + 1;    // (1000 ns / 62.5 ns) + 1

    i2c->CR1_b.ACK = 1;
    i2c->CR1_b.PE = 1;
    
}

// (bus, dev_addr, reg, buffer, length)
inline int i2c_read_data(struct i2c * i2c, uint8_t dev_addr, uint8_t reg, uint8_t * buf, size_t length)
{
    // wait until bus is not busy
    WAIT(!(i2c->SR2_b.BUSY));

    // set the slave register pointer
    // enable ACK
    i2c->CR1_b.ACK = 1;
    // send start
    i2c->CR1_b.START = 1;

    // wait for start sent
    WAIT(i2c->SR1_b.SB);

    // put address in the data register
    i2c->DR_b.DR = (dev_addr << 1) | WRITE;
    WAIT2(i2c->SR1_b.ADDR, i2c->SR1_b.AF);

    /* printf("written address into data register\n"); */
    /* printf("dev_addr=0x%02x sent=0x%02x SR1=%04x\n", */
    /*        dev_addr, (uint8_t)((dev_addr << 1) | WRITE), (unsigned)i2c->SR1); */
    if (i2c->SR1_b.AF)
    {
        i2c->SR1_b.AF = 0;
        i2c->CR1_b.STOP = 1;
        return -2;
    }
    (void)i2c->SR1; (void)i2c->SR2;

    WAIT(i2c->SR1_b.TxE);
    i2c->DR_b.DR = reg;
    WAIT(i2c->SR1_b.BTF);
    
    // phase 2:
    if (length == 2)
        i2c->CR1_b.POS = 1;
    
    i2c->CR1_b.START = 1;
    WAIT(i2c->SR1_b.SB);

    i2c->DR_b.DR = (dev_addr << 1) | READ;
    
    WAIT2(i2c->SR1_b.ADDR, i2c->SR1_b.AF);
    (void)i2c->SR1; (void)i2c->SR2;

    /* printf("read address into data register\n"); */
    if (i2c->SR1_b.AF)
    {
        i2c->SR1_b.AF = 0;
        i2c->CR1_b.POS = 0;
        i2c->CR1_b.STOP = 1;
        return -2;
    }
    
    switch (length)
    {
        case 1:
        {
            i2c->CR1_b.ACK = 0;
            (void)i2c->SR1; (void)i2c->SR2;
            i2c->CR1_b.STOP = 1;
            WAIT(i2c->SR1_b.RxNE);
            buf[0] = i2c->DR_b.DR;
            break;
        }

        case 2:
        {
            (void)i2c->SR1; (void)i2c->SR2;
            i2c->CR1_b.ACK = 0;
            WAIT(i2c->SR1_b.BTF);
            i2c->CR1_b.STOP = 1;
            buf[0] = i2c->DR_b.DR;
            buf[1] = i2c->DR_b.DR;
            i2c->CR1_b.POS = 0;
            break;
        }
        default:
        {
            (void)i2c->SR1; (void)i2c->SR2;
            /* i2c->SR1_b.ADDR = 0; */
            size_t i = 0;
            while (length - i > 3)
            {
                WAIT(i2c->SR1_b.RxNE);
                buf[i++] = i2c->DR_b.DR;
            }
            
            WAIT(i2c->SR1_b.BTF);
            i2c->CR1_b.ACK = 0;
            buf[i++] = i2c->DR_b.DR;
            WAIT(i2c->SR1_b.BTF);
            i2c->CR1_b.STOP = 1;
            buf[i++] = i2c->DR_b.DR;
            buf[i++] = i2c->DR_b.DR;
        }
    }
    
    i2c->CR1_b.ACK = 1;
    return 0;
}

inline int i2c_write_data(struct i2c * i2c, uint8_t dev_addr, uint8_t reg, uint8_t * buf, size_t length)
{
    if (length == 0)
        return 1;

    // wait until bus is not busy
    WAIT(!(i2c->SR2_b.BUSY));
    /* printf("bus not busy\n"); */

    // phase 1
    // enable ACK
    i2c->CR1_b.ACK = 1;
    // send start
    i2c->CR1_b.START = 1;

    // wait for start sent
    WAIT(i2c->SR1_b.SB);
    /* printf("started\n"); */

    // put address in the data register
    i2c->DR_b.DR = (dev_addr << 1) | WRITE;
    WAIT2(i2c->SR1_b.ADDR, i2c->SR1_b.AF);
    /* printf("put address in data register\n"); */

    if (i2c->SR1_b.AF)
    {
        i2c->SR1_b.AF = 0;
        i2c->CR1_b.STOP = 1;
        return -2;
    }
    (void)i2c->SR1; (void)i2c->SR2;

    WAIT2(i2c->SR1_b.TxE, i2c->SR1_b.AF);
    /* printf("is register is not empty in \n"); */
    i2c->DR_b.DR = reg;

    if (i2c->SR1_b.AF)
    {
        i2c->SR1_b.AF = 0;
        i2c->CR1_b.STOP = 1;
        return -2;
    }

    for (size_t i = 0; i < length; ++i)
    {
        WAIT2(i2c->SR1_b.TxE, i2c->SR1_b.AF);
        if (i2c->SR1_b.AF)
        {
            i2c->SR1_b.AF = 0;
            i2c->CR1_b.STOP = 1;
            return -2;
        }
        i2c->DR_b.DR = buf[i];
    }

    WAIT2(i2c->SR1_b.BTF, i2c->SR1_b.AF);

    i2c->CR1_b.STOP = 1;
    return 0;
}
