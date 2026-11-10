#ifndef UART_H
#define UART_H

#include "gpio.h"

struct uart
{
    union
    {
        volatile uint32_t SR;  // status register
        struct
        {
            volatile uint32_t PE: 1;
            volatile uint32_t FE: 1;
            volatile uint32_t NF: 1;
            volatile uint32_t ORE: 1;
            volatile uint32_t IDLE: 1;
            volatile uint32_t RXNE: 1;
            volatile uint32_t TC: 1;
            volatile uint32_t TXE: 1;
            volatile uint32_t LBD: 1;
            volatile uint32_t CTS: 1;
            volatile uint32_t :22;
        } SR_b;
    };

    union
    {
        volatile uint32_t DR;
        struct
        {
            volatile uint32_t DR: 9;
            volatile uint32_t RES: 23;
        } DR_b;
    };

    union
    {
        volatile uint32_t BRR; // baud rate
        struct
        {
            volatile uint32_t DIV_Fraction: 4;
            volatile uint32_t DIV_Mantissa: 12;
            volatile uint32_t : 16;
        } BRR_b;
    };

    union
    {
        volatile uint32_t CR1; // controls
        struct
        {
            volatile uint32_t SBK : 1;    // Send break
            volatile uint32_t RWU : 1;    // Receiver wakeup
            volatile uint32_t RE : 1;     // Receiver enable
            volatile uint32_t TE : 1;     // Transmitter enable
            volatile uint32_t IDLEIE : 1; // IDLE interrupt enable
            volatile uint32_t RXNEIE : 1; // RXNE interrupt enable
            volatile uint32_t TCIE : 1;   // Transmission complete interrupt enable
            volatile uint32_t TXEIE : 1;  // TXE interrupt enable
            volatile uint32_t PEIE : 1;   // PE interrupt enable
            volatile uint32_t PS : 1;     // Parity selection
            volatile uint32_t PCE : 1;    // Parity control enable
            volatile uint32_t WAKE : 1;   // Wakeup method
            volatile uint32_t M : 1;      // Word length
            volatile uint32_t UE : 1;     //  enable
            volatile uint32_t RES1 : 1;   // Reserved
            volatile uint32_t OVER8 : 1;  // Oversampling mode
            volatile uint32_t RES2 : 16;  // Reserved
        } CR1_b;
    };

    union
    {
        volatile uint32_t CR2;
        struct
        {
            volatile uint32_t ADD : 4;   // Address of the USART node
            volatile uint32_t : 1;       // Reserved
            volatile uint32_t LBDL : 1;  // LIN break detection length
            volatile uint32_t LBDIE : 1; // LIN break detection interrupt enable
            volatile uint32_t : 1;       // Reserved
            volatile uint32_t LBCL : 1;  // Last bit clock pulse
            volatile uint32_t CPHA : 1;  // Clock phase
            volatile uint32_t CPOL : 1;  // Clock polarity
            volatile uint32_t CLKEN : 1; // Clock enable
            volatile uint32_t STOP : 2;  // STOP bits
            volatile uint32_t LINEN : 1; // LIN mode enable
            volatile uint32_t : 17;      // Reserved
        } CR2_b;
    };

    union
    {
        volatile uint32_t CR3;
        struct
        {
            volatile uint32_t EIE : 1;    // Error interrupt enable
            volatile uint32_t IREN : 1;   // IrDA mode enable
            volatile uint32_t IRLP : 1;   // IrDA low-power
            volatile uint32_t HDSEL : 1;  // Half-duplex selection
            volatile uint32_t NACK : 1;   // Smartcard NACK enable
            volatile uint32_t SCEN : 1;   // Smartcard mode enable
            volatile uint32_t DMAR : 1;   // DMA enable receiver
            volatile uint32_t DMAT : 1;   // DMA enable transmitter
            volatile uint32_t RTSE : 1;   // RTS enable
            volatile uint32_t CTSE : 1;   // CTS enable
            volatile uint32_t CTSIE : 1;  // CTS interrupt enable
            volatile uint32_t ONEBIT : 1; // One bit sample method enable
            volatile uint32_t : 20;       // Reserved
        } CR3_b;
    };

    
    union {                     // Guard time and prescaler register
        volatile uint32_t GTPR; // Guard time and prescaler register (read/write)
        struct {
            volatile uint32_t PSC : 8; // Prescaler value
            volatile uint32_t GT : 8;  // Guard time value
            volatile uint32_t : 16;    // Reserved
        } GTPR_b;
    };
};

#define USART1 ((struct uart *) 0x40011000)
#define UART2  ((struct uart *) 0x40004400)
#define UART3  ((struct uart *) 0x40004800)
#define UART4  ((struct uart *) 0x40004c00)
#define UART5  ((struct uart *) 0x40005000)
#define USART6 ((struct uart *) 0x40011400)

void uart_init(struct uart * uart, unsigned long baud);
int uart_read_ready(struct uart * uart);
uint8_t uart_read_byte(struct uart * uart);
int uart_write_ready(struct uart * uart);
void uart_write_char(struct uart * uart, char c);
void uart_write_buf(struct uart * uart, char * buf, size_t len);

#endif

