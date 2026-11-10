#ifndef MPU6050_H
#define MPU6050_H

#define MPU_WHO_AM_I       0x75
#define MPU_PWR_MGMT_1     0x6b
#define MPU_SMPLRT_DIV     0x19
#define MPU_CONFIG         0x1a
#define MPU_GYRO_CONFIG    0x1b
#define MPU_ACCEL_CONFIG   0x1c

#define MPU_ADDRESS        0x68
#define MPU_ERR_WRONG_DEVICE  -10
#define MPU_ERR_BAD_ARG       -11
#define MPU_ERR_RESET         -12

#include "i2c.h"
#include "systick.h"

struct mpu6050
{
    struct i2c * bus;
    uint8_t address;
    uint8_t gyro_range;
    uint8_t accel_range;
    float gyro_scales;
    float accel_scales;
};

/* struct pwr_mgmt */
/* { */
/*     union */
/*     { */
/*         struct */
/*         { */
/*             volatile uint8_t CLKSEL: 2;    // clockselect */
/*             volatile uint8_t TEMP_DIS: 1;  // temp disables */
/*             volatile uint8_t :1;           // reserved */
/*             volatile uint8_t CYCLE: 1;     // cycle */
/*             volatile uint8_t SLEEP: 1;     // sleep */
/*             volatile uint8_t DEVICE_RESET: 1; // device reset */
/*         }; */
/*     }; */
/* }; */

/* #define CLKSEL  */
#define MPU_TEMP_DISABLE (1 << 3)
#define MPU_CYCLE        (1 << 5)
#define MPU_SLEEP        (1 << 6)
#define MPU_DEVICE_RESET (1 << 7)
#define MPU_CLKSEL_GYRO_X 0x01
#define MPU_DLPF_CFG      0x07
#define MPU_FS_SEL        0x18
#define MPU_ZG_ST         0x60

#define MPU_ACCEL_XOUT_H  0x3b      // start of the 14-byte sample block
#define TRY(expr) do { int r_ = (expr); if (r_ != 0) return r_; } while (0)
static const float gyro_scales[4]  = { 131.0f, 65.5f, 32.8f, 16.4f };
static const float accel_scales[4] = { 16384.0f, 8192.0f, 4096.0f, 2048.0f };


struct mpu_raw
{
    int16_t ax, ay, az;
    int16_t temp;
    int16_t gx, gy, gz;
};

// Reads one sample (single 14-byte burst). Returns 0 or a negative error.
int mpu_read_raw(struct mpu6050 *m, struct mpu_raw *out);

/* static int mpu_read_reg(struct mpu6050 * m, uint8_t reg, uint8_t * val, size_t len); */

/* static int mpu_write_reg(struct mpu6050 * m, uint8_t reg, uint8_t * val); */

/* // Change only the bits in mask, leave the rest alone */
/* static int mpu_modify_reg(struct mpu6050 *m, uint8_t reg, uint8_t mask, uint8_t val); */

int mpu_init(struct mpu6050 * m, uint8_t dlpf, uint16_t rate_hz, uint8_t accel_range, uint8_t gyro_range);

#endif
