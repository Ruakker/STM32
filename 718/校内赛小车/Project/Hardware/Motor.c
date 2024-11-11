#include "stm32f10x.h"
#include "PWM.h"
#include "Motor.h"
/*
ENA: T4C1 PD12
IN1: PG3
IN2: PG4
ENB: T4C2 PD13
IN3: PG5
IN4: PG6
*/
void Motor_Init(void) {
    PWM_Init();

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOG, &GPIO_InitStructure);
    
    Motor_SetSpeed(100);
    Motor_Stop();
}

// Speed: 0 ~ 100
void Motor_SetLeftSpeed(uint16_t Speed) {
    PWM_SetCompare1(Speed * 200 - 1);
}

// Speed: 0 ~ 100
void Motor_SetRightSpeed(uint16_t Speed) {
    PWM_SetCompare2(Speed * 200 - 1);
}

// void Motor_SpeedUp(uint16_t dSpeed) {
//     uint16_t Speed = 
//     PWM_SetCompare();
// }

void Motor_LeftForward(void) {
    GPIO_ResetBits(GPIOG, GPIO_Pin_3);
    GPIO_SetBits(GPIOG, GPIO_Pin_4);
}

void Motor_RightForward(void) {
    GPIO_ResetBits(GPIOG, GPIO_Pin_5);
    GPIO_SetBits(GPIOG, GPIO_Pin_6);
}

void Motor_LeftBackward(void) {
    GPIO_SetBits(GPIOG, GPIO_Pin_3);
    GPIO_ResetBits(GPIOG, GPIO_Pin_4);
}

void Motor_RightBackward(void) {
    GPIO_SetBits(GPIOG, GPIO_Pin_5);
    GPIO_ResetBits(GPIOG, GPIO_Pin_6);
}

void Motor_LeftStop(void) {
    GPIO_ResetBits(GPIOG, GPIO_Pin_3);
    GPIO_ResetBits(GPIOG, GPIO_Pin_4);
}

void Motor_RightStop(void) {
    GPIO_ResetBits(GPIOG, GPIO_Pin_5);
    GPIO_ResetBits(GPIOG, GPIO_Pin_6);
}

void Motor_SetSpeed(uint16_t Speed) {
    Motor_SetLeftSpeed(Speed);
    Motor_SetRightSpeed(Speed);
}

void Motor_Forward(void) {
    Motor_LeftForward();
    Motor_RightForward();
}

void Motor_Backward(void) {
    Motor_LeftBackward();
    Motor_RightBackward();
}

void Motor_Stop(void) {
    Motor_LeftStop();
    Motor_RightStop();
}

void Motor_LeftRotate(void) {
    Motor_LeftBackward();
    Motor_RightForward();
}

void Motor_RightRotate(void) {
    Motor_LeftForward();
    Motor_RightBackward();
}
