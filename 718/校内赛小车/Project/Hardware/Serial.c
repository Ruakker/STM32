/*
 * Bluetooth control
*/
#include <stdarg.h>
#include <stdio.h>

#include "stm32f10x.h"
#include "OLED.h"
#include "Motor.h"

// TX: PA9 RX: PA10

void Serial_Init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_HardwareFlowControl =
        USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART1, &USART_InitStructure);
    
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  // RXNE: Receive Data register not empty interrupt

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART1, ENABLE);
}

void Serial_SendByte(uint8_t Byte) {
    USART_SendData(USART1, Byte);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
        ;
}

void Serial_SendArray(uint8_t *Array, uint16_t Length) {
    for (uint16_t i = 0; i < Length; i++)
        Serial_SendByte(Array[i]);
}

void Serial_SendString(char *String) {
    while (*String)
        Serial_SendByte(*String++);
}

// Max length of NumberString is 10
void Serial_SendNumber(uint32_t Number) {
    char NumberString[10];
    sprintf(NumberString, "%d", Number);
    Serial_SendString(NumberString);
}

// Max length of String is 100
void Serial_Printf(char *format, ...) {
    char String[100];
    va_list args;
    va_start(args, format);
    vsprintf(String, format, args);
    va_end(args);
    Serial_SendString(String);
}

uint8_t Serial_ReceiveByte(void) {
    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
        ;
    return USART_ReceiveData(USART1);
}

void USART1_IRQHandler(void) {
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) {
        uint8_t ReceivedByte = Serial_ReceiveByte();
        switch (ReceivedByte) {
            case 'F':
                Motor_Forward();
                OLED_ShowString(2, 1, "        ");
                OLED_ShowString(2, 1, "Forward");
                break;
            case 'B':
                Motor_Backward();
                OLED_ShowString(2, 1, "        ");
                OLED_ShowString(2, 1, "Backward");
                break;
            case 'L':
                Motor_LeftRotate();
                OLED_ShowString(2, 1, "        ");
                OLED_ShowString(2, 1, "Left");
                break;
            case 'R':
                Motor_RightRotate();
                OLED_ShowString(2, 1, "        ");
                OLED_ShowString(2, 1, "Right");
                break;
            case 'S':
                Motor_Stop();
                OLED_ShowString(2, 1, "        ");
                OLED_ShowString(2, 1, "Stop");
                break;
        }
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}
