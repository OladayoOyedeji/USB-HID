#include "i2c.h"

// we are using PB6 - SCL, PB7 - SDA

void initclocks()
{
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
  RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
}

void configPinMode()
{
  GPIOB->MODER |= ((2 << 12) | (2 << 14));

  GPIOB->OTYPER |= (GPIO_OTYPER_OT6 | GPIO_OTYPER_OT7);   // open-drain
  GPIOB->PUPDR  |= ((1 << 12) | (1 << 14));               // pull-up on pins 6,7
  
}

void setAF()
{
  GPIOB->AFR[0] |= ((4 << (4 * 6)) | (4 << (4 * 7)));
}

void i2c_init()
{
  I2C->CR1 |= I2C_CR1_SWRST; // software reset
  I2C->CR1 &= ~I2C_CR1_SWRST;

  I2C1->CR1 = 16;

  I2C1->CCR = 80;
  I2C1->TRISE = 17;

  I2C1->CR1 |= I2C_CR1_PE;
}

static int i2c_wait_flag(volatile uint32_t * reg, uint32_t flag)
{
  uint32_t timeout = I2C_TIMEOUT;
  while (!(*reg & flag))
    {
      if (--timeout == 0) return -1;
    }

  return 0;
}

static int i2c_start()
{
  I2C1->CR1 |= I2C_CR1_START;
  return i2c_wait_flag((volatile uint32_t *)&I2C1->SR1, I2C_SR1_SB);
}

static int i2c_send_addr(uint8_t addr7, uint8_t rw)
{
  I2C1->DR = (addr7 << 1) | rw;
  if (i2c_wait_flag((volatile uint32_t *)&I2C1->SR1, I2C_SR1_ADDR) != 0)
    return -1;
  I2C1->SR1;
  I2C1->SR2;
  return 0;
}

static void i2c_stop(void)
{
  I2C1->CR1 |= I2C_CR1_STOP;
}

int i2c_transmit(uint8_t slave_addr, uint8_t * data, uint16_t len)
{
  if (i2c_start() != 0) return -1;
  if (i2c_send_addr(slave_addr, 0) != 0)
    {
      i2c_stop();
      return -1;
    }

  for (uint16_t i = 0; i < len; i++)
    {
      if (i2c_send_byte(data[i]) != 0)
	{

	  i2c_stop();
	  return -1;
	}
    }
  
  if (i2c_wait_flag((volatile uint32_t *)&I2C1->SR1, I2C_SR1_BTF) != 0)
    {
      i2c_stop();
      return -1;
    }

  i2c_stop();
  return 0;
}