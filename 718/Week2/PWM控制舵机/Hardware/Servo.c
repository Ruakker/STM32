#include "PWM.h"
#include "stm32f10x.h"

void Servo_Init(void) { PWM_Init(); }

// Angle: 0 - 180
// CCR: 500 - 2500
void Servo_SetAngle(float Angle) { PWM_SetCompare2(Angle / 180 * 2000 + 500); }
