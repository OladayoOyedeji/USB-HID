// File: main.c
// Author: Oladayo Oyedeji

#include "i2c.h"
#include "uart.h"
#include "sysyem_init.h"
#include "tusb.h"
#include <stdint.h>

int main()
{
    system_init(); // clock config
    configure_pins(); // GPIO setup
    i2c_init();
    systick_init();
    tusb_init(); // sets up the USB peripheral

    const uint8_t slave_address = 0x68;

    // check sensor status
    uint8_t who_am_i = i2c_read(slave_address, 0x75);
    if (who_am_i != 0x68)
    {
        uart_print("MPU6050 not found!\r\n");
        while (1)
        {
            tud_task();
        }
    }

    uart_print("MPU6050 OK\r\n");

    //
    i2c_write(slave_address, 0x6B, 0x00);

    uint32_t last = 0;

    while (1)
    {
        tud_task(); // pump USB state machine

        if (get_tick() - last >= 1)
        {
            last = get_tick();

            uint8_t raw[14];
            i2c_read_multi(slave_address, 0x3B, raw, 14);

            int16_t gyro_x = (int16_t)((raw[8] << 8) | raw[9]);
            int16_t gyro_y = (int16_t)((raw[10] << 8) | raw[11]);
            int16_t gyro_z = (int16_t)((raw[12] << 8) | raw[13]);

            int16_t gyro_report[3] = {gyro_x, gyro_y, gyro_z};

            if (tud_hid_ready())
            {
                tud_hid_report(0, gyro_report, sizeof(gyro_report));
            }
        }
    }
}
