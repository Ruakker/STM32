#include "stm32f10x.h"                  // Device header
#include "Key.h"
#include "LED.h"

void EXTI0_IRQHandler(void) {  // 按下 Key1 亮灯
    // if (EXTI_GetITStatus(EXTI_Line0) == SET)
    LED1_ON();
    EXTI_ClearITPendingBit(EXTI_Line0);
}


void EXTI1_IRQHandler(void) {  // 按下 Key2 灭灯
    // if (EXTI_GetITStatus(EXTI_Line1) == SET)
    LED1_OFF();
    EXTI_ClearITPendingBit(EXTI_Line1);
}


int main(void) {
    Key1_Init();
    Key2_Init();
    LED1_Init();
    
    while (1) {
        
    }
}
