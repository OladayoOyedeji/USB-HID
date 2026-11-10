#include "main.h"

uint16_t led1 = PIN('A', 5);

int main(void)
{
    sysclock_init();
    systick_init(SYS_FREQUENCY / 1000);

    RCC->AHB1ENR |= BIT(PINBANK(led1));
    gpio_set_mode(led1, GPIO_MODE_OUTPUT);
    uart_init(UART2, 2000000);
    
    printf("we are starting now");
    printf("hello");
    printf("\n\n\nSystem initialized.\n");
    printf("Core clock: %d Hz\n", SYS_FREQUENCY);
    printf("APB1 clock: %d Hz\n", APB1_FREQUENCY);
    printf("APB2 clock: %d Hz\n", APB2_FREQUENCY);

    gpio_set_mode(PIN('B',8), GPIO_MODE_INPUT);
    gpio_set_mode(PIN('B',9), GPIO_MODE_INPUT);
    printf("SCL=%d SDA=%d\n", gpio_read(PIN('B',8)), gpio_read(PIN('B',9)));
    i2c_init(I2C1);

    printf("after the init\n");
    struct gpio * gpio = GPIO(1);
    printf("MODER=%08lx AFRH=%08lx OTYPER=%04lx PUPDR=%08lx\n",
           gpio->MODER, gpio->AFRH, gpio->OTYPER, gpio->PUPDR);
    printf("CR1=%04x CR2=%04x CCR=%04x SR1=%04x SR2=%04x\n",
           (unsigned)I2C1->CR1, (unsigned)I2C1->CR2, (unsigned)I2C1->CCR,
           (unsigned)I2C1->SR1, (unsigned)I2C1->SR2);

    printf("AHB1ENR=%08lx APB1ENR=%08lx\n", RCC->AHB1ENR, RCC->APB1ENR);

    printf("GPIOB MODER=%08lx\n", *(volatile uint32_t *)0x40020400);
    printf("I2C1 CR1=%08lx CR2=%08lx CCR=%08lx TRISE=%08lx\n",
           *(volatile uint32_t *)0x40005400, *(volatile uint32_t *)0x40005404,
           *(volatile uint32_t *)0x4000541C, *(volatile uint32_t *)0x40005420);

    struct mpu6050 imu = {
        .bus = I2C1,
        .address = MPU_ADDRESS
    };

    int err = mpu_init(&imu, 3, 100, 0, 0);
    if (err != 0)
    {
        // Report the code before parking, so you know what failed
        printf("mpu_init failed: %d\r\n", err);
        while (true);
    }

    bool led_state = true;
    uint32_t next_blink = s_ticks + 500;

    while (true)
    {
        uint32_t now = s_ticks;

        if ((int32_t)(now - next_blink) >= 0)
        {
            next_blink += 500;
            gpio_write(led1, led_state);
            led_state = !led_state;

            struct mpu_raw s;
            err = mpu_read_raw(&imu, &s);

            if (err != 0)
                printf("read failed: %d\r\n", err);
            else
                printf("a: %d %d %d  t: %d  g: %d %d %d\r\n",
                       s.ax, s.ay, s.az, s.temp, s.gx, s.gy, s.gz);
        }
    }

    return 0;
}

/* int main(void) */
/* { */

/*     uint16_t led = PIN('A', 5); // Blue LED */

/*     sysclock_init(); // Settings in main.h: 16 MHz HSE - core clock at 168 MHz and enable apb1 at 42 MHz, apb2 at 84 MHz */

/*     systick_init(SYS_FREQUENCY / 1000); // 1ms SysTick */

/*     gpio_set_mode(led, GPIO_MODE_OUTPUT); // Set blue LED to output mode */

/*     uart_init(UART2, 2000000); // Initialize USART1 for debugging - 2000000 bps (2 Mbps) works nicely with 168 MHz core clock */

/*     printf("hello"); */
/*     printf("\n\n\nSystem initialized.\n"); */
/*     printf("Core clock: %d Hz\n", SYS_FREQUENCY); */
/*     printf("APB1 clock: %d Hz\n", APB1_FREQUENCY); */
/*     printf("APB2 clock: %d Hz\n", APB2_FREQUENCY); */

/*     bool led_state = true; */
/*     uint32_t now = 0, next_blink = 500, next_tick = 1000, loop_cnt = 0; */

/*     for (;;) { // Super loop */

/*         now = s_ticks; */

/*         if (now >= next_blink) { */
/*             gpio_write(led, led_state); // Toggle LED */
/*             led_state = !led_state; */
/*             next_blink = now + 500; // Schedule next toggle in 500 ms */
/*         } */

/*         if (now >= next_tick) { */
/*             printf("Tick: %lu (loop = %lu)\n", now / 1000, loop_cnt); */
/*             loop_cnt = 0; */
/*             next_tick = now + 1000; // Schedule next tick in 1000 ms */
/*         } */

/*         ++loop_cnt; // Just a counter to show how many times the loop runs between ticks */
/*     } */

/*     return 0; */
/* } */

int _write(int fd, char *ptr, int len) {
    int i = 0;

    /*
     * Write "len" of char from "ptr" to file id "fd"
     * Return number of char written.
     *
     * Only work for STDOUT, STDIN, and STDERR
     */
    if (fd > 2)
    {
        return -1;
    }
    while (*ptr && (i < len))
    {
        if (*ptr == '\n')
        {
            uart_write_char(UART2, '\r');
        }
        uart_write_char(UART2, *ptr);
        i++;
        ptr++;
    }
    return i;
}
