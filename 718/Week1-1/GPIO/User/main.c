#include "stm32f10x.h"
#include "Delay.h"

int main() {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    uint8_t GPIO_Pin_0_Status;

    while (1) {
        GPIO_ResetBits(GPIOB, GPIO_Pin_0);
        
        GPIO_Pin_0_Status = GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_0);
        GPIO_WriteBit(GPIOB, GPIO_Pin_1, GPIO_Pin_0_Status);
        
        Delay_ms(500);
        
        GPIO_SetBits(GPIOB, GPIO_Pin_0);
        
        GPIO_Pin_0_Status = GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_0);
        GPIO_WriteBit(GPIOB, GPIO_Pin_1, GPIO_Pin_0_Status);
        
        Delay_ms(500);
    }
}
