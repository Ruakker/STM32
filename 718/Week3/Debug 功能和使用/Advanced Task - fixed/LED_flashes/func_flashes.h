#ifndef __FUNC_FLASHES__
#define __FUNC_FLASHES__

#include "drv_flashes.h"

#define TIM_Pre (72)
#define TIM_Frequecy 10000//Hz，为保证分辨率
#define LED1_Basic_Frequecy 2//Hz这个频率为LED1的闪烁频率，这个频率应人眼能够感受到，应该小于10Hz
#define LED2_Basic_Frequecy 50//Hz这个频率为LED2的呼吸灯的基础频率，这个频率人眼不能直接感受到，应该要大于10Hz
#define LED2_Breathing_Frequecy 0.2f//Hz这个频率为呼吸灯从亮到暗的频率
#define LED2_Basic_Count (TIM_Frequecy/LED2_Basic_Frequecy)//计算LED2基础周期需要定时器触发中断几次
#define LED2_count (LED2_Basic_Frequecy/LED2_Breathing_Frequecy)//计算需要几个LED2的基础周期

void Flashes_Init(void);

#endif
