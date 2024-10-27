#include "stm32f10x.h"                  // Device header
#include "AD.h"
#include "OLED.h"
#include "Timer.h"
#include "Serial.h"

uint16_t LastTime, CurrentTime;
uint16_t AD1Value, AD2Value, AD3Value, AD4Value;

int main(void) {
    OLED_Init();

    OLED_ShowString(3, 1, "Time Used:    us");

    AD_Init();
    Serial_Init();
    Timer_Init();
    
    while (1) {
        LastTime = CurrentTime;
        AD1Value = AD_GetValue(ADC_Channel_12);
        AD2Value = AD_GetValue(ADC_Channel_13);
        AD3Value = AD_GetValue(ADC_Channel_14);
        AD4Value = AD_GetValue(ADC_Channel_15);
        CurrentTime = Timer_GetValue();
        OLED_ShowNum(1, 1, AD1Value, 4);
        OLED_ShowNum(1, 6, AD2Value, 4);
        OLED_ShowNum(2, 1, AD3Value, 4);
        OLED_ShowNum(2, 6, AD4Value, 4);
        OLED_ShowNum(3, 12, (CurrentTime - LastTime), 2);
        Serial_Printf("%d %d %d %d\r\n", AD1Value, AD2Value, AD3Value, AD4Value);
    }
}
