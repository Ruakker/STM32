#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Motor.h"
#include "Serial.h"


int main(void) {
    OLED_Init();
    Serial_Init();
    Motor_Init();

    OLED_ShowString(1, 1, "Direction: ");
    OLED_ShowString(3, 1, "Speed: ");
    
    Motor_SetSpeed(100);
    
    while (1) {
    }
}
