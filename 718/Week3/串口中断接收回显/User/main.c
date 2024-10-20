#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Serial.h"

void USART1_IRQHandler(void) {
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) {
        uint8_t ReceivedByte = Serial_ReceiveByte();
        OLED_ShowHexNum(1, 15, ReceivedByte, 2);
        Serial_SendByte(ReceivedByte);
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}

int main() {
    OLED_Init();
    Serial_Init();
    
    OLED_ShowString(1, 1, "ReceivedByte: ");
    
    while (1) {
    }
}
