#include "stm32f10x.h"  // Device header

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "LED.h"
#include "OLED.h"
#include "Serial.h"

float FREQ;
uint8_t STATUS;

void OLED_Update(uint8_t Status, float Freq) {
    OLED_Clear();
    if (Status) {
        OLED_ShowString(1, 1, "LED1 ON");
    } else {
        OLED_ShowString(1, 1, "LED1 OFF");
    }
    OLED_ShowString(2, 1, "Freq: ");
    char FreqStr[5];
    sprintf(FreqStr, "%.1fs", Freq);
    OLED_ShowString(2, 7, FreqStr);
}

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
            if (!strcmp((char *)Serial_RxPacket, "LEDON")) {
                LED1_ON(), RxState = 0, RxLength = 0, STATUS = 1;
                memset(Serial_RxPacket, 0, sizeof(Serial_RxPacket));
                OLED_Update(STATUS, FREQ);
            }
            if (!strcmp((char *)Serial_RxPacket, "LEDOFF")) {
                LED1_OFF(), RxState = 0, RxLength = 0, STATUS = 0;
                memset(Serial_RxPacket, 0, sizeof(Serial_RxPacket));
                OLED_Update(STATUS, FREQ);
            }
            if (!strcmp((char *)Serial_RxPacket, "LEDFREQ")) {
                RxState = 2, RxLength = 0;
                memset(Serial_RxPacket, 0, sizeof(Serial_RxPacket));
            }
        } else if (RxState == 2) {
            if (RxData == 's') {
                FREQ = atof((char *)Serial_RxPacket);
                LED1_SetFreq(FREQ);
                OLED_Update(STATUS, FREQ);
                RxState = 0, RxLength = 0;
                memset(Serial_RxPacket, 0, sizeof(Serial_RxPacket));
            } else
                Serial_RxPacket[RxLength++] = RxData;
        }

        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}

int main(void) {
    LED1_Init();
    OLED_Init();
    Serial_Init();
    FREQ = 0.1, STATUS = 0;

    LED1_OFF();
    OLED_Update(STATUS, FREQ);

    while (1) {
    }
}
