#include "func_flashes.h"

#define min(a, b) (((a) > (b)) ? (b) : (a))

static uint8_t led1_open_flag = 1;
static uint8_t led2_open_flag = 1;

/**
 * @brief        返回一个三角折线函数的值，三角函数由cyclecount，height决定
 * @param        cycle_count，表示三角函数的底
 * @param        count，表示现在进行到周期的多少
 * @param        height，表示三角形的高
 * @retval       无
 * @author       718 Lab
 ***/
static uint32_t delta_function(uint32_t cycle_count, uint32_t count, uint32_t height)
{
    cycle_count /= 2; //半个周期
    if (count > cycle_count)//如果大于半个周期进行处理
        count = 2 * cycle_count - count;
    return (count * height) / cycle_count;//返回count占height的比例，再乘以height
}

/**
 * @brief        key1处理函数
 * @param        无
 * @retval       无
 * @author       718 Lab
 ***/
static void key1_handler()
{
    led1_open_flag = !led1_open_flag;//反转标志位
}

/**
 * @brief        key2处理函数
 * @param        无
 * @retval       无
 * @author       718 Lab
 ***/
static void key2_handler()
{
    led2_open_flag = !led2_open_flag;
}

/**
 * @brief        定时器处理函数
 * @param        无
 * @retval       无
 * @author       718 Lab
 ***/
static void tick_handler()
{
    uint32_t time = Get_Tick(); //获取时间刻
    if (led1_open_flag)
    {
        if (time % (TIM_Frequecy / (LED1_Basic_Frequecy * 2)) == 0)
            LED1_Flashes_TOGGLE(); //直接用LED1_Basic_Frequecy*2的频率切换LED即可
    }
    else
    {
        LED1_Flashes_OFF();
    }

    if (led2_open_flag) //呼吸灯控制代码
    {
        uint32_t count = time / LED2_Basic_Count; //这是基本周期的第几个周期
        uint32_t extral_time = time % LED2_Basic_Count;
        //这可以表示在LED2_Basic_Frequecy频率下，这个周期过了多久
        if (extral_time == delta_function(LED2_count, count, LED2_Basic_Count))
            //可变化占空比的获取，如果碰到可变占空比的边沿就关灯
            LED2_Flashes_OFF();
        else if (extral_time == 0)//else是为了边界情况
            LED2_Flashes_ON();
    }
    else
    {
        LED2_Flashes_OFF();
    }
    if (time >= TIM_Frequecy / min(LED1_Basic_Frequecy, LED2_Breathing_Frequecy)) //选取最小的频率清空time
        Claer_Tick();                                                             //防止时间刻溢出
}

/**
 * @brief        闪烁和呼吸灯初始化函数
 * @param        无
 * @retval       无
 * @author       718 Lab
 ***/
void Flashes_Init(void)
{
    RCC_ClocksTypeDef RCC_Clockstructure;
    RCC_GetClocksFreq(&RCC_Clockstructure);
    LED_Flashes_Init();
    GPIO_EXIT_Init(key1_handler, key2_handler);//填充中断函数
    TIM_Flashes_Init(TIM_Pre - 1, (RCC_Clockstructure.SYSCLK_Frequency / TIM_Pre / TIM_Frequecy) - 1, tick_handler);
    //填充中断函数，确定中断周期
}
