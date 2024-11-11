#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x.h"
void Motor_Init(void);
void Motor_SetLeftSpeed(uint16_t Speed);
void Motor_SetRightSpeed(uint16_t Speed);
void Motor_LeftForward(void);
void Motor_RightForward(void);
void Motor_LeftBackward(void);
void Motor_RightBackward(void);
void Motor_LeftStop(void);
void Motor_RightStop(void);
void Motor_SetSpeed(uint16_t Speed);
void Motor_Forward(void);
void Motor_Backward(void);
void Motor_Stop(void);
void Motor_LeftRotate(void);
void Motor_RightRotate(void);

#endif
