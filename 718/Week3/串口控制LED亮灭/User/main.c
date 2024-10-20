#include "stm32f10x.h"  // Device header
#include "LED.h"
#include "OLED.h"
#include "Serial.h"

#include <string.h>

void USART1_IRQHandler(void) {
    static uint8_t RxState = 0;
    static uint8_t RxLength = 0;
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) {
        uint8_t RxData = USART_ReceiveData(USART1);

        if (RxState == 0) {
            if (RxData == 'L') {
                RxState = 1;
                Serial_RxPacket[RxLength++] = RxData;
            }

        } else if (RxState == 1) {
            Serial_RxPacket[RxLength++] = RxData;
            if (RxLength == 5 && !strcmp((char *)Serial_RxPacket, "LEDON")) {
                LED1_ON(), RxState = 0, RxLength = 0, Serial_RxPacket[5] = 0;
                OLED_ShowString(1, 1, "LED1 ON ");
            }
            if (RxLength == 6 && !strcmp((char *)Serial_RxPacket, "LEDOFF")) {
                LED1_OFF(), RxState = 0, RxLength = 0, Serial_RxPacket[5] = 0;
                OLED_ShowString(1, 1, "LED1 OFF");
            }
        }

        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}

int main(void) {
    LED1_Init();
    OLED_Init();
    Serial_Init();

    LED1_OFF();
    OLED_ShowString(1, 1, "LED1 OFF");

    while (1) {
    }
}
