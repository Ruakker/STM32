#include "Waterfall.h"
#include "stm32f10x.h"  // Device header

int main(void) {
    Waterfall_Init();
    LED_Waterfall(led1 | led3 | led5);

    while (1) {
    }
}
