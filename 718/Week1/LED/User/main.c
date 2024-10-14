#include "stm32f10x.h"

int main() {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  // 模式选择推挽输出（GPIO_Mode_Out_PP）或者开漏输出（GPIO_Mode_Out_OD），此处选择了开漏输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  // PB0 引脚对应 LED1
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_ResetBits(GPIOB, GPIO_Pin_0);  // 将 PB0 引脚设为 0，即低电平
    
    while (1) {
        
    }
}
