#include "stm32f10x.h"  // Device header
// TIM4_CH1: PD12
// TIM4_CH2: PD13

void PWM_Init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);                             //使能AFIO复用时钟	//使能重映射IO时钟
    GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);                                    // TIM4 Remap
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOD, &GPIO_InitStructure);

    TIM_InternalClockConfig(TIM4);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 20000 - 1;  // ARR
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;  // PSC
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;  // CCR
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);
    TIM_OC2Init(TIM4, &TIM_OCInitStructure);

    TIM_Cmd(TIM4, ENABLE);
}

void PWM_SetCompare1(uint16_t Compare) {  // Modify CCR value
    TIM_SetCompare1(TIM4, Compare);
}

void PWM_SetCompare2(uint16_t Compare) {  // Modify CCR value
    TIM_SetCompare2(TIM4, Compare);
}
