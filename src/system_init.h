#ifndef SYSTEM_INIT_H
#define SYSTEM_INIT_H

#include "stm32f4xx.h"

void system_init(void);    // configure PLL, run at 100MHz
void systick_init(void);   // 1ms tick counter
void delay_ms(uint32_t ms);
uint32_t get_tick(void);   // returns ms since boot
