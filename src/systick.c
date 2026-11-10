#include "systick.h"

volatile uint32_t s_ticks = 0;

void systick_init(uint32_t ticks)
{
    SYSTICK->RVR = ticks - 1;
    SYSTICK->CVR = 0;
    SYSTICK->CSR = BIT(0) | BIT(1) | BIT(2);
}

void systick_handler(void)
{
    ++s_ticks;
}

void delay_ms(uint32_t ms)
{
    uint32_t old_ticks = s_ticks;
    while (s_ticks - old_ticks < ms)
    {
        (void)0;
    }
}
