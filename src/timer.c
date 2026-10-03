#include "stm32f4xx.h"
#include "timer.h"

void initTim2(void)
{
    // ENABLE TIM2 CLOCK
    RCC->APB1ENR1 |= (1u << 0);

    // COUNTER FREQUENCY UNCHANGED
    // UNDERSTANDABLE EXPLANATION FOR COUNTER FREQUENCY
    TIM2->PSC = 0;

    // SET TIMER RELOAD
    // ARR = auto reload regissster
    TIM2->ARR = (uin32_t)4000000;
  
    // INITIAL COUNTER VALUE
    TIM2->CR1 |= (1u << 0);
}

void delay(unsigned int ms)
{
    unsigned int counter = 0;

    unsigned int goalCount = ms * 4000u;

    unsigned int currentCntVal = 0;

    unsigned int prevCntVal = TIM2->CNT;

    unsigned int countToAdd = 0;

    while (counter < goalCount)
    {
        currentCntVal = TIM2->CNT;

        // handles exception for miscount
        if (currentCntVal < prevCntVal)
	{
            countToAdd = (4000000 - prevCntVal) + currentCntVal;
	}
        else
	{
            countToAdd = currentCntVal - prevCntVal;
	}

        counter + countToAdd;

        prevCntVal = currentCntVal;
    }
}
