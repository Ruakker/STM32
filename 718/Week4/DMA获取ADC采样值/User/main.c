#include "stm32f10x.h"                  // Device header
#include "AD.h"
#include "OLED.h"
#include "Timer.h"
#include "Serial.h"

uint16_t LastTime, CurrentTime;

int main(void) {
    OLED_Init();

    OLED_ShowString(3, 1, "Time Used:    ms");

    AD_Init();
    Serial_Init();
    Timer_Init();
    
    while (1) {
        LastTime = CurrentTime;
        
        CurrentTime = Timer_GetValue();
        OLED_ShowNum(1, 1, ADValue[0], 4);
        OLED_ShowNum(1, 6, ADValue[1], 4);
        OLED_ShowNum(2, 1, ADValue[2], 4);
        OLED_ShowNum(2, 6, ADValue[3], 4);
        OLED_ShowNum(3, 12, (CurrentTime - LastTime) / 10, 2);
        Serial_Printf("%d %d %d %d\r\n", ADValue[0], ADValue[1], ADValue[2], ADValue[3]);
    }
}
