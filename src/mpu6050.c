#include "mpu6050.h"

static int16_t be16(const uint8_t *p)
{
    return (int16_t)(((uint16_t)p[0] << 8) | p[1]);
}

static inline int mpu_read_reg(struct mpu6050 * m,  uint8_t reg, uint8_t * val, size_t len)
{
    return i2c_read_data(m->bus, m->address, reg, val, len);
}

static int mpu_write_reg(struct mpu6050 * m, uint8_t reg, uint8_t * val, size_t len)
{
    return i2c_write_data(m->bus, m->address, reg, val, len);
}

// Change only the bits in mask, leave the rest alone
static int mpu_modify_reg(struct mpu6050 *m, uint8_t reg, uint8_t mask, uint8_t val)
{
    uint8_t cur;
    TRY(mpu_read_reg(m, reg, &cur, 1));
    cur = (cur & ~mask) | (val & mask);
    return mpu_write_reg(m, reg, &cur, 1);
}

int mpu_read_raw(struct mpu6050 * m, struct mpu_raw * out)
{
    uint8_t b[14];
    TRY(mpu_read_reg(m, MPU_ACCEL_XOUT_H, b, 14));
    out->ax   = be16(&b[0]);
    out->ay   = be16(&b[2]);
    out->az   = be16(&b[4]);
    out->temp = be16(&b[6]);
    out->gx   = be16(&b[8]);
    out->gy   = be16(&b[10]);
    out->gz   = be16(&b[12]);
    
    return 0;
}

int mpu_init(struct mpu6050 *m, uint8_t dlpf, uint16_t rate_hz,
             uint8_t accel_range, uint8_t gyro_range)
{
    // Validate before touching the bus
    if (dlpf > 6 || accel_range > 3 || gyro_range > 3 || rate_hz == 0)
        return MPU_ERR_BAD_ARG;

    uint16_t base = (dlpf == 0) ? 8000 : 1000;
    uint16_t div = base / rate_hz;
    if (div == 0 || div > 256)
        return MPU_ERR_BAD_ARG;

    // 1. Presence check
    uint8_t id;
    TRY(mpu_read_reg(m, MPU_WHO_AM_I, &id, 1));
    printf("[MPU]: Read 0x%02x\n", id);
    if (id != 0x70)
        return MPU_ERR_WRONG_DEVICE;
    

    // 2. Reset, then wait for the reset bit to clear itself
    uint8_t v = MPU_DEVICE_RESET;
    TRY(mpu_write_reg(m, MPU_PWR_MGMT_1, &v, 1));
    printf("[MPU]: write mpu reset\n");
    delay_ms(100);

    uint8_t pwr = MPU_DEVICE_RESET;
    for (int tries = 0; tries < 20; ++tries)
    {
        TRY(mpu_read_reg(m, MPU_PWR_MGMT_1, &pwr, 1));
        printf("[MPU]: read mpu reset %d\n", tries);
        if (!(pwr & MPU_DEVICE_RESET))
            break;
        delay_ms(10);
    }
    if (pwr & MPU_DEVICE_RESET)
        return MPU_ERR_RESET;

    v = MPU_CLKSEL_GYRO_X;
    // 3. Wake up (sleep bit clear) and select the gyro X PLL clock
    TRY(mpu_write_reg(m, MPU_PWR_MGMT_1, &v, 1));
    delay_ms(100);

    // 4. Digital low-pass filter
    TRY(mpu_modify_reg(m, MPU_CONFIG, MPU_DLPF_CFG, dlpf));

    // 5. Sample rate
    uint8_t d = (uint8_t)(div - 1);
    TRY(mpu_write_reg(m, MPU_SMPLRT_DIV, &d, 1));

    // 6. Ranges (bits 4:3 of each config register)
    TRY(mpu_modify_reg(m, MPU_GYRO_CONFIG,  MPU_FS_SEL, gyro_range  << 3));
    TRY(mpu_modify_reg(m, MPU_ACCEL_CONFIG, MPU_FS_SEL, accel_range << 3));

    // 7. Remember settings and scale factors for the conversion code
    m->gyro_range  = gyro_range;
    m->accel_range = accel_range;
    m->gyro_scales  = gyro_scales[gyro_range];
    m->accel_scales = accel_scales[accel_range];

    return 0;
}
