// File: uart.h
// Author: Oladayo Oyedeji
#include "stm32f4xx.h"

void initclocks();
void configPinMode();
void setAF(void);
void configUart1Pins();
void configUart1();
void initUART();
void transmitUart(uint8_t data);
void transmitStrUart(char * str);
void recieveUart();

void uart_init(void);
void uart_print(const char* str);
void uart_println(const char* str);
void uart_print_num(int32_t num);  // print integers
