// File: i2c.h
// Author: Oladayo Oyedeji
void i2c_init(void);
void i2c_write(uint8_t addr, uint8_t reg, uint8_t data);
uint8_t i2c_read(uint8_t addr, uint8_t reg);
void i2c_read_multi(uint8_t addr, uint8_t reg, uint8_t* buf, uint8_t len);
