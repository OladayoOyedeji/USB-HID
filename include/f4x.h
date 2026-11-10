#ifndef __F4X_H
#define __F4X_H 

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

// Bit manipulation macros
#define BIT(x) (1UL << (x))
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8)

#define _weak __attribute__((weak))

#include "scb.h"
#include "rcc.h"
#include "flash.h"
#include "sysclock.h"
#include "systick.h"
#include "gpio.h"
#include "uart.h"
#include "i2c.h"
#include "mpu6050.h"

#endif /* __F4X_H */
