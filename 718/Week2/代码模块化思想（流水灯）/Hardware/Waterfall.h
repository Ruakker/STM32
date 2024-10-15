#ifndef __WATERFALL_H
#define __WATERFALL_H
#include "stm32f10x.h"

typedef enum {
    led1 = (0x01 << 0),
    led2 = (0x01 << 1),
    led3 = (0x01 << 2),
    led4 = (0x01 << 3),
    led5 = (0x01 << 4),
    led6 = (0x01 << 5)
} leds_t;
void Waterfall_Init(void);
void LED_Waterfall(uint8_t _leds);

#endif
