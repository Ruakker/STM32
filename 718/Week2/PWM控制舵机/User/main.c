#include "Servo.h"
#include "Timer.h"
#include "stm32f10x.h"  // Device heade

uint16_t ServoAngle = 0;

void TIM2_IRQHandler(void) {
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) {
        ServoAngle += 30;
        if (ServoAngle > 180) ServoAngle = 0;
        Servo_SetAngle(ServoAngle);
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}

int main(void) {
    Servo_Init();
    Timer_Init();
    while (1) {
    }
}
