#include "LED.h"
#include "Timer.h"
#include "stm32f10x.h"  // Device heade

void TIM2_IRQHandler(void) {
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) {
        if (GPIO_ReadOutputDataBit(GPIOG, GPIO_Pin_2) == 0)  // Toggle PG2
            GPIO_SetBits(GPIOG, GPIO_Pin_2);                 // Set PG2 to 1
        else
            GPIO_ResetBits(GPIOG, GPIO_Pin_2);  // Set PG2 to 0

        if (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_3) == 0)  // Read PG3
            LED1_ON();
        else
            LED1_OFF();
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}

void PG2_Init(void) {  // PG2 -> Output
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOG, &GPIO_InitStructure);
}

void PG3_Init(void) {  // PG3 -> Input
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOG, &GPIO_InitStructure);
}

int main(void) {
    PG2_Init();
    PG3_Init();
    LED1_Init();
    Timer_Init();
    while (1) {
    }
}
