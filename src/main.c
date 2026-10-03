// File: main.c
// Author: Oladayo Oyedeji

#include "i2c.h"
#include "uart.h"
#include <stdio.h>
#include <stdint.h>

void delay_ms(volatile uint32_t ms) {
    for(uint32_t i = 0; i < ms; i++) {
        SysTick->LOAD = 16000 - 1;
        SysTick->VAL  = 0;
        SysTick->CTRL = 0x05;
        while(!(SysTick->CTRL & (1 << 16)));
    }
}


int main(void) {
    clock_init();
    gpio_init();
    i2c_init();
    tusb_init();        // TinyUSB's own init — sets up the USB peripheral

    while (1) {
        tud_task();      // pump USB state machine — always first

        static uint32_t last = 0;
        if (millis() - last >= 1) {           // 1kHz
            last = millis();
            i2c_write_read(slave_address, 0x3B, raw, 14);
            int16_t gyro_data[3] = { gyro_x, gyro_y, gyro_z };
            if (tud_hid_ready()) {
                tud_hid_report(0, gyro_data, sizeof(gyro_data));
            }
        }
    }
}
