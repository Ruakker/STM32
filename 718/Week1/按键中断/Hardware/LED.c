#include "stm32f10x.h"                  // Device header

void LED1_Init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  // 开漏输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void LED1_ON(void) {
    GPIO_ResetBits(GPIOB, GPIO_Pin_0);
}

void LED1_OFF(void) {
    GPIO_SetBits(GPIOB, GPIO_Pin_0);
}
