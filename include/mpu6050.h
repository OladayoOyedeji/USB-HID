void mpu6050_init(void);
void mpu6050_read_gyro(int16_t * gx, int16_t * gy, int 16_t * gz);
void mpu6050_read_accel(int16_t* ax, int16 * ay, int16_t * az);
void mpu6050_get_temp();
