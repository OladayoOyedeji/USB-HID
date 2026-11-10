#include "uart.h"

void uart_init(struct uart * uart, unsigned long baud)
{
    uint8_t af = 7;
    uint16_t rx = 0, tx = 0;
    uint32_t pclk = 0;

    if (uart == USART1)
    {
        RCC->APB2ENR_b.USART1EN = 1;
        rx = PIN('A', 10);
        tx = PIN('A', 9);
        pclk = APB2_FREQUENCY;
        
    }
    else if (uart == UART2)
    {
        RCC->APB1ENR_b.UART2EN = 1;
        rx = PIN('A', 3);
        tx = PIN('A', 2);
        pclk = APB1_FREQUENCY;
    }
    
    else if (uart == UART3)
    {
        RCC->APB1ENR_b.UART3EN = 1;
        rx = PIN('B', 11);
        tx = PIN('B', 10);
        pclk = APB1_FREQUENCY;
    }
        
    else if (uart == UART4)
    {
        af = 8;
        RCC->APB1ENR_b.UART4EN = 1;
        rx = PIN('C', 11);
        tx = PIN('C', 10);
        pclk = APB1_FREQUENCY;
    }
        
    else if (uart == UART5)
    {
        af = 8;
        RCC->APB1ENR_b.UART5EN = 1;
        rx = PIN('C', 12);
        tx = PIN('D', 2);
        pclk = APB2_FREQUENCY;
    }
        
    else if (uart == USART6)
    {
        af = 8;
        RCC->APB2ENR_b.USART6EN = 1;
        rx = PIN('C', 7);
        tx = PIN('C', 6);
        pclk = APB2_FREQUENCY;
    }


    gpio_set_mode(rx, GPIO_MODE_AF);
    gpio_set_mode(tx, GPIO_MODE_AF);
    gpio_set_af(rx, af);
    gpio_set_af(tx, af);

    uart->CR1 = 0;

    uint32_t div = (pclk + (baud / 2)) / baud;
    uart->BRR = div;

    uart->CR1_b.RE = 1;
    uart->CR1_b.TE = 1;
    uart->CR1_b.UE = 1;
}

int uart_read_ready(struct uart * uart)
{
    return uart->SR_b.RXNE;
}

uint8_t uart_read_byte(struct uart * uart)
{
    return (uint8_t)(uart->DR & 255);
}

void uart_write_char(struct uart * uart, char ch)
{
    while (!uart->SR_b.TXE)
        (void)0;

    uart->DR = (uint32_t)ch;
}

void uart_write_buf(struct uart * uart, char * buf, size_t len)
{
    while (len-- > 0)
    {
        uart_write_char(uart, *(uint8_t *)buf++);
    }
}

