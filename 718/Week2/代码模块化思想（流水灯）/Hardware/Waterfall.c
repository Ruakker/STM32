#include "stm32f10x.h"  // Device header

uint8_t leds, cur, cnt = 1;

// TIM2: 1 sec
void Timer_Init(void) {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_InternalClockConfig(TIM2);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 10000 - 1;    // ARR = 10000
    TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1;  // PSC = 7200
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);
}

void TIM2_IRQHandler(void) {
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        
        if (!leds) return;

        // Turn off all LEDs
        GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_5 | GPIO_Pin_4);
        GPIO_SetBits(GPIOF, GPIO_Pin_11 | GPIO_Pin_12);

        if (cur == 0x00) cur = leds, cnt = 1;
        switch (cnt) {
            case 1:
                GPIO_ResetBits(GPIOB, GPIO_Pin_0);
                break;
            case 2:
                GPIO_ResetBits(GPIOB, GPIO_Pin_1);
                break;
            case 3:
                GPIO_ResetBits(GPIOB, GPIO_Pin_5);
                break;
            case 4:
                GPIO_ResetBits(GPIOB, GPIO_Pin_4);
                break;
            case 5:
                GPIO_ResetBits(GPIOF, GPIO_Pin_11);
                break;
            case 6:
                GPIO_ResetBits(GPIOF, GPIO_Pin_12);
                break;
        }
        cur >>= 1, cnt++;
        while (!(cur & 0x01)) {
            cur >>= 1, cnt++;
            if (cur == 0x00) cur = leds, cnt = 1;
        }
    }
}

// PB0, PB1, PB5, PB4, PF11, PF12
void Waterfall_Init(void) {
    Timer_Init();
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);

    // Remap to enable PB4
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

    // GPIOB
    GPIO_InitTypeDef GPIOB_InitStructure;
    GPIOB_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIOB_InitStructure.GPIO_Pin =
        GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_5 | GPIO_Pin_4;
    GPIOB_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOB, &GPIOB_InitStructure);

    // GPIOF
    GPIO_InitTypeDef GPIOF_InitStructure;
    GPIOF_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIOF_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
    GPIOF_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOF, &GPIOF_InitStructure);

    GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_5 | GPIO_Pin_4);
    GPIO_SetBits(GPIOF, GPIO_Pin_11 | GPIO_Pin_12);
}

void LED_Waterfall(uint8_t _leds) {
    leds = _leds;
    TIM_Cmd(TIM2, ENABLE);
}
