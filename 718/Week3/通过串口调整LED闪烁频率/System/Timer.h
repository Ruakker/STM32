#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f10x.h"

void Timer_Init(void);
void Timer_SetARR(uint16_t ARR);

#endif
