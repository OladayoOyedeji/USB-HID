#ifndef __I2C_H
#define __I2C_H

#include "gpio.h"

struct i2c
{
    union
    {
        volatile uint32_t CR1;   // control register 1
        struct
        {
            volatile uint32_t PE: 1;
            volatile uint32_t SMBUS: 1;
            volatile uint32_t :1;           // reserved
            volatile uint32_t SMBTYPE: 1;   // SMBus type
            volatile uint32_t ENARP: 1;     // ARP enable
            volatile uint32_t ENPEC: 1;     // PEC enable
            volatile uint32_t ENGCC: 1;     // General call enable
            volatile uint32_t NOSTRETCH: 1; // Clock stretching disable
            volatile uint32_t START: 1;     // Start generation
            volatile uint32_t STOP: 1;      // Stop generation
            volatile uint32_t ACK: 1;       // Acknowledge enable
            volatile uint32_t POS: 1;       // Acknowledge/PEC position
            volatile uint32_t PEC: 1;       // Packet error checking
            volatile uint32_t ALERT: 1;     // SMBus alert
            volatile uint32_t :1;           // reserved
            volatile uint32_t SWRST: 1;     // Software reset
            volatile uint32_t :16;          // 16bit padding
        } CR1_b;
    };

    union
    {
        volatile uint32_t CR2;   // control register 2
        struct
        {
            volatile uint32_t FREQ: 6;       // peripheral clock frequency
            volatile uint32_t :2;            // reserved
            volatile uint32_t ITERREN: 1;    // Error interrupt enable
            volatile uint32_t ITEVTEN: 1;    // event interrupt enable
            volatile uint32_t ITBUFEN: 1;    // Buffer interrupt enable
            volatile uint32_t DMAEN: 1;      // DMA request enable
            volatile uint32_t LAST: 1;       // DMA last transfer
            volatile uint32_t : 3;           // reserved
            volatile uint32_t :16;          // 16bit padding
        } CR2_b;
    };

    union
    {
        volatile uint32_t OAR1;  // own address register 1
        struct
        {
            volatile uint32_t ADD0: 1;        // interface address
            volatile uint32_t ADD: 9;        // interface address
            volatile uint32_t : 5;            // reserved
            volatile uint32_t ADDMODE: 1;     // Addressing mode
            volatile uint32_t :16;          // 16bit padding
        } OAR1_b;
    };

    union
    {
        volatile uint32_t OAR2;  // own address register 2
        struct
        {
            volatile uint32_t ENDUAL: 1;      // Dual addressing mode enable
            volatile uint32_t ADD2: 7;        // interface address
            volatile uint32_t : 8;            // reserved
            volatile uint32_t :16;          // 16bit padding
        } OAR2_b;
    };

    union
    {
        volatile uint32_t DR;    // data register
        struct
        {
            volatile uint32_t DR: 8;           // data register
            volatile uint32_t :8;              // reserved
            volatile uint32_t :16;          // 16bit padding
        } DR_b;
    };

    union
    {
        volatile uint32_t SR1;   // status register 1
        struct
        {
            volatile uint32_t SB: 1;            // Start bit
            volatile uint32_t ADDR: 1;          // Adress sent flag
            volatile uint32_t BTF: 1;           // Byte transfer finished
            volatile uint32_t ADD10: 1;         // 10 bit header sent
            volatile uint32_t STOPF: 1;         // stop detection
            volatile uint32_t :1;               // reserved
            volatile uint32_t RxNE: 1;          // Data register not empy (reciever)
            volatile uint32_t TxE: 1;           // Data register empty (transmitter)
            volatile uint32_t BERR: 1;          // Bus error
            volatile uint32_t ARLO: 1;          // Arbitration lost
            volatile uint32_t AF: 1;            // Acknowledge failure
            volatile uint32_t OVR: 1;           // Overrun/Underrun
            volatile uint32_t PECERR: 1;        // PEC Error in reception
            volatile uint32_t :1;               // reserved
            volatile uint32_t TIMEOUT: 1;       // Timeout
            volatile uint32_t SMBALERT: 1;      // SMBus alert
            volatile uint32_t :16;          // 16bit padding
        } SR1_b;
    };

    union
    {
        volatile uint32_t SR2;   // status register 2
        struct
        {
            volatile uint32_t MSL: 1;            // Controller / target mode
            volatile uint32_t BUSY: 1;           // Bus busy
            volatile uint32_t TRA: 1;            // Transmitter/reciever
            volatile uint32_t :1;                // reserved
            volatile uint32_t GENCALL: 1;        // general call address
            volatile uint32_t SMBDEFAULT: 1;
            volatile uint32_t SMBHOST: 1;
            volatile uint32_t DUALF: 1;
            volatile uint32_t PEC: 8;
            volatile uint32_t :16;          // 16bit padding
        } SR2_b;
    };

    union
    {
        volatile uint32_t CCR;   // clock control register
        struct
        {
            volatile uint32_t CCR: 12;
            volatile uint32_t : 2;
            volatile uint32_t DUTY: 1;
            volatile uint32_t F_S: 1;
            volatile uint32_t :16;          // 16bit padding
        } CCR_b;
    };

    union
    {
        volatile uint32_t TRISE; // TRISE register
        struct
        {
            volatile uint32_t TRISE: 6;
            volatile uint32_t :10;
            volatile uint32_t :16;          // 16bit padding
        } TRISE_b;
    };

    union
    {
        volatile uint32_t FTLR;  // FTLR register
        struct
        {
            volatile uint32_t DNF: 4;
            volatile uint32_t ANOFF: 1;
            volatile uint32_t :11;
            volatile uint32_t :16;          // 16bit padding
        };
    };
};

#define I2C1 ((struct i2c *) 0x40005400)
#define I2C2 ((struct i2c *) 0x40005800)
#define I2C3 ((struct i2c *) 0x40005c00)

#define WRITE 0
#define READ  1
#define I2C_TIMEOUT 1000000

/* #define WAIT(cond)                                 \ */
/*     do {                                           \ */
/*         uint32_t t_ = I2C_TIMEOUT;                 \ */
/*         while(!(cond))                             \ */
/*         {                                          \ */
/*             if (--t_ == 0)                         \ */
/*             {                                      \ */
/*                 i2c->CR1_b.POS = 0;                \ */
/*                 i2c->CR1_b.STOP = 1;               \ */
/*                 return -1;                         \ */
/*             }                                      \ */
/*         }                                          \ */
/*     }while (0)                                     \ */

#define WAIT(cond)                                                      \
    {                                                                   \
        uint32_t t_ = I2C_TIMEOUT;                                      \
        while(!(cond))                                                  \
        {                                                               \
            if (--t_ == 0)                                              \
            {                                                           \
                printf("I2C timeout: %s SR1=%04x SR2=%04x\n", #cond,    \
                       (unsigned)i2c->SR1, (unsigned)i2c->SR2);         \
                i2c->CR1_b.POS = 0;                                     \
                i2c->CR1_b.STOP = 1;                                    \
                return -1;                                              \
            }                                                           \
        }                                                               \
    }                                           \
    
#define WAIT2(a, b) WAIT((a) || (b))

// set gpio mode to alternate function
// set ahb1enr
void i2c_init(struct i2c * i2c);
int i2c_write_data(struct i2c * i2c, uint8_t dev_addr, uint8_t reg, uint8_t * buf, size_t length);
int i2c_read_data(struct i2c * i2c, uint8_t dev_addr, uint8_t reg, uint8_t * buf, size_t length);


#endif

