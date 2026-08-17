#include "system_init.h"

void system_init(void);    // configure PLL, run at 100MHz
void initclocks()
{
  // FOR I2C with MPU_6050
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
  RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

  // FOR UART for DEBUGGING
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
}
void configurePinMode()
{
  // FOR I2C with MPU_6050
  GPIOB->MODER |= ((2 << 12) | (2 << 14));

  GPIOB->OTYPER |= (GPIO_OTYPER_OT6 | GPIO_OTYPER_OT7);   // open-drain
  GPIOB->PUPDR  |= ((1 << 12) | (1 << 14));               // pull-up on pins 6,7

  // FOR UART for DEBUGGING
  GPIOA->MODER |= ((2 << 18) | (2 << 20)); // set PA9 and PA10 AF
}

void setAF()
{
  // FOR I2C with MPU_6050
  GPIOB->AFR[0] |= ((4 << (4 * 6)) | (4 << (4 * 7)));

  // FOR UART for DEBUGGING
  GPIOA->AFR[1] |= ((7 << 4) | (7 << 8));
}
void systick_init(void);   // 1ms tick counter
void delay_ms(uint32_t ms);
uint32_t get_tick(void);   // returns ms since boot
