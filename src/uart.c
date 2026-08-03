#include "uart.h"

// we use PA9 - TX, PA10 - RX
// set the clock to 1 and set the pin function to alternste function
// which is 

void initclocks()
{
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
}

void configPinMode()
{
  GPIOA->MODER |= ((2 << 18) | (2 << 20)); // set PA9 and PA10 AF
  
}

// set the alternate function to uart or usart1 which is af7
// each size of the peripherals is 4 bits
// 0 -> 0 - 7
// 1 -> 8 - 15
void setAF()
{
  GPIOA->AFR[1] |= ((7 << 4) | (7 << 8));
}
 
void configUart1Pins()
{
  configPinMode();

  setAF();
  
}

void transmitUart(uint8_t data)
{
  while (!(USART1->ISR & USART_ISR_TXE));

  // transmit data register
  USART1->TDR = data;
}

void transmitStrUart(char * str)
{
  while (*str)
    {
      transmitUart(*str++);
    }
}

void recieveUart()
{
  uint8_t rxData = 0;

    // IF THERE IS DATA IN THE RECEIVE DATA REGISTER
    // USART_ISR_RXNE EXPANDS TO (1 << 5)
    if(USART1->ISR & USART_ISR_RXNE)
    {
        // READ DATA FROM THE REGISTER
        rxData = USART1->RDR;
    }

    return rxData;
}
