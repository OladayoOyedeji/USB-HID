#ifndef __MAIN_H
#define __MAIN_H

#include <stdint.h>
#include <stdbool.h>

enum
{
    APB1_PRE = 4,
    APB2_PRE = 2,
};

enum
{
    PLL_HSE = 8,
    PLL_M   = 4,
    PLL_N   = 168,
    PLL_P   = 2,
};

#define FLASH_LATENCY 5
#define SYS_FREQUENCY ((PLL_HSE * PLL_N / PLL_M / PLL_P) * 1000000)
#define APB2_FREQUENCY (SYS_FREQUENCY / APB2_PRE)
#define APB1_FREQUENCY (SYS_FREQUENCY / APB1_PRE)

#define PPRE_BITS(div) ((div) == 1 ? 0 : (31 - __builtin_clz(div)) + 3)
#include "f4x.h"

#endif
