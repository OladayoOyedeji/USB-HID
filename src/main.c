#include "i2c.h"

int main()
{
  initclocks();
  configPinMode();
  setAF();
  i2c_init();
  
  // address for mpu6050
  uint8_t slave_address = 0x68;
  uint8_t wake_cmd[2] = {0x6B, 0x00};

  // wake up
  i2c_transmit(slave_address, wake_cmd, 2);
  
  while (1)
    {
      uint8_t raw[14];
      i2c_write_read(slave_address, 0x3B, raw, 14); // read accel/gyro/temp

      int16_t accel_x = (raw[0] << 8) | raw[1];
      int16_t accel_y = (raw[2] << 8) | raw[3];
      int16_t accel_z = (raw[4] << 8) | raw[5];

      uart_println();
      uart_println();
      uart_println();
      // do something with the data — store, transmit over UART, etc.

      for (volatile int i = 0; i < 1000000; i++); // when you write your driver
    }
  return 0;
  
}

