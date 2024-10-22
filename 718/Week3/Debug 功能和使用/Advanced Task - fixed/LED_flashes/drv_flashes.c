#include "drv_flashes.h"
#include "stm32f10x_it.h"
#include "limits.h"

/**
 * @brief        与 LED1 灯对应的 GPIO 端口初始化函数
 * @param        无
 * @retval       无
 * @author       718 Lab
 ***/
void LED_Flashes_Init(void)
{
  /* 定义一个 GPIO_InitTypeDef 类型的结构体 */
  GPIO_InitTypeDef GPIO_InitStructure;

  /* 开启 LED 相关的 GPIO 外设时钟 */
  RCC_APB2PeriphClockCmd(LED1_GPIO_CLK, ENABLE);
  /* 选择要控制的 GPIO 引脚 */
  GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN | LED2_GPIO_PIN;
  /* 设置引脚模式为通用推挽输出 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  /* 设置引脚速率为 50MHz */
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  /* 调用库函数，初始化 GPIO */
  GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);
}

/**
 * @brief        使得LED1亮
 * @param        无
 * @retval       无
 * @author       718 Lab
 ***/
void LED1_Flashes_ON(void)
{
  GPIO_ResetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
}

/**
 * @brief        使得LED1灭
 * @param        无
 * @retval       无
 * @author       718 Lab
 ***/
void LED1_Flashes_OFF(void)
{
  GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
}

/**
 * @brief        使得LED1亮
 * @param        无
 * @retval       无
 * @author       718 Lab
 ***/
void LED2_Flashes_ON(void)
{
  GPIO_ResetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);
}

/**
 * @brief        使得LED2灭
 * @param        无
 * @retval       无
 * @author       718 Lab
 ***/
void LED2_Flashes_OFF(void)
{
  GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);
}

/**
 * @brief        使得LED1状态反转
 * @param        无
 * @retval       无
 * @author       718 Lab
 ***/
void LED1_Flashes_TOGGLE(void)
{
  digitalToggle(LED1_GPIO_PORT, LED1_GPIO_PIN);
}

/**
 * @brief        使得LED1状态反转
 * @param        无
 * @retval       无
 * @author       718 Lab
 ***/
void LED2_Flashes_TOGGLE(void)
{
  digitalToggle(LED2_GPIO_PORT, LED2_GPIO_PIN);
}

/*--------------------分割线---------------------*/

static void (*tim_handler)(void) = NULL;
static __IO uint32_t time = 0;

/**
 * @brief        定时器中断初始化
 * @param        arr 自动重装载寄存器的值
 * @param        psc 时钟预分频数
 * @retval       无
 * @note         计数器频率为 freq = 72M/(psc+1) , 每隔 (arr+1)/freq 秒进入一次定时器中断
 * @author       718 Lab
 */
void TIM_Flashes_Init(uint16_t arr, uint16_t psc, void (*handler)())
{
  if (handler != NULL)
    tim_handler = handler; //给终端函数填入处理函数
  else
    while(1){
    };

  // 定时器TIM初始化
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  Flashes_TIM_APBxClock_FUN(Flashes_TIM_CLK, ENABLE);         // 开启定时器时钟, 即内部时钟 CK_INT=72M
  TIM_TimeBaseStructure.TIM_Period = arr;                     // 自动重装载寄存器的值，累计 TIM_Period + 1 个频率后产生一个更新或者中断
  TIM_TimeBaseStructure.TIM_Prescaler = psc;                  // 设置时钟预分频数
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     // 时钟分频因子（互补输出，死区才用得到）
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 计数器计数模式，选择向上计数
  TIM_TimeBaseInit(Flashes_TIM, &TIM_TimeBaseStructure);      // 初始化定时器
  TIM_ClearFlag(Flashes_TIM, TIM_FLAG_Update);                // 清除计数器中断标志位
  TIM_ITConfig(Flashes_TIM, TIM_IT_Update, ENABLE);           // 使能定时器中断,允许更新中断
  TIM_Cmd(Flashes_TIM, ENABLE);                               // 使能计数器

  // 中断优先级NVIC设置
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);           // 设置 NVIC 中断分组 2:2 位抢占优先级, 2 位响应优先级
  NVIC_InitStructure.NVIC_IRQChannel = Flashes_TIM_IRQ;     // 设置中断来源
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // 设置主优先级为 0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        // 设置抢占优先级为1
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // IRQ 通道被使能
  NVIC_Init(&NVIC_InitStructure);                           // 初始化 NVIC 寄存器
}

/**
 * @brief        返回时间刻
 * @param        无
 * @retval       无
 * @author       718 Lab
 */
uint32_t Get_Tick(void)
{
  return time;
}
/**
* @brief        清除时间刻，使得时间刻为零
 * @param        无
 * @retval       无
 * @author       718 Lab
 */
void Claer_Tick(void)
{
  time = 0;
}

/**
 * @brief        定时器中断服务函数
 * @param        无
 * @retval       无
 * @author       718 Lab
 */
void Flashes_TIM_IRQHandler(void)
{
  if (TIM_GetITStatus(Flashes_TIM, TIM_IT_Update)) // 检查 TIMx 更新中断发生与否
  {
    if (time != UINT32_MAX)
      time++;

    if (tim_handler != NULL) //防止错误
      tim_handler();         //调用处理函数
    else
      while(1){

      };

    TIM_ClearITPendingBit(Flashes_TIM, TIM_IT_Update); // 清除 TIMx 更新中断标志
  }
}

/*--------------------分割线---------------------*/
static void (*exti1_handler)() = NULL, (*exti2_handler)() = NULL;

/**
 * @brief        按键中断的 GPIO 初始化函数，此函数无法直接调用
 * @param        无
 * @retval       无
 * @author       718 Lab
 ***/
static void Key_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(KEY2_GPIO_CLK, ENABLE);

  GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN; // KEY1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN; // KEY2
  GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);
}

/**
 * @brief        按键外部中断 EXTI 配置函数，此函数无法直接调用
 * @param        无
 * @retval       无
 * @author       718 Lab
 ***/
static void Key_EXIT_Init(void)
{
  EXTI_InitTypeDef EXIT_InitStruct;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // 打开AFIO的端口时钟（EXTI线的时钟）

  // GPIOC9	  中断线以及中断初始化配置 下降沿触发       // KEY1
  GPIO_EXTILineConfig(KEY1_EXTI_PORT, KEY1_EXTI_PIN);  // 选择中断线
  EXIT_InitStruct.EXTI_Line = KEY1_EXTI_LINE;          // 使能中断线
  EXIT_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;     // 产生中断（而不是事件）
  EXIT_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; // 下降沿触发
  EXIT_InitStruct.EXTI_LineCmd = ENABLE;               // 使能中断
  EXTI_Init(&EXIT_InitStruct);                         // 根据 EXTI_InitStruct 中指定的参数初始化外设 EXTI 寄存器

  // GPIOC8	  中断线以及中断初始化配置 下降沿触发       // KEY2
  GPIO_EXTILineConfig(KEY2_EXTI_PORT, KEY2_EXTI_PIN); // 选择中断线
  EXIT_InitStruct.EXTI_Line = KEY2_EXTI_LINE;         // 使能中断线
  EXTI_Init(&EXIT_InitStruct);                        // 根据 EXTI_InitStruct 中指定的参数初始化外设 EXTI 寄存器
}

/**
 * @brief        按键中断 NVIC 配置函数，此函数无法直接调用
 * @param        无
 * @retval       无
 * @author       718 Lab
 ***/
static void Key_NVIC_Init(void)
{
  NVIC_InitTypeDef NVIC_InitStruct;

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);           // 设置 NVIC 中断分组 2:2 位抢占优先级，2 位响应优先级
  NVIC_InitStruct.NVIC_IRQChannel = KEY_IRQn1;           // 选择 EXTI_Line19、7 所在的外部中断通道（5——9公用一个通道）
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x02; // 通道的抢占优先级 2
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;        // 通道的子优先级 1
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;              // 使能外部中断通道
  NVIC_Init(&NVIC_InitStruct);                              // 根据 NVIC_InitStruct 中指定的参数初始化外设 NVIC 寄存器
  
  NVIC_InitStruct.NVIC_IRQChannel = KEY_IRQn2;           // 选择 EXTI_Line19、7 所在的外部中断通道（5——9公用一个通道）
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x02; // 通道的抢占优先级 2
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x02;        // 通道的子优先级 1
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;              // 使能外部中断通道
  NVIC_Init(&NVIC_InitStruct);                              // 根据 NVIC_InitStruct 中指定的参数初始化外设 NVIC 寄存器
}

/**
 * @brief        按键中断初始化函数
 * @param        handler_exti1，中断函数1
 * @param        handler_exti2，中断函数2
 * @retval       无
 * @author       718 Lab
 ***/
void GPIO_EXIT_Init(void (*handler_exti1)(), void (*handler_exti2)())
{
  Key_GPIO_Init();
  Key_EXIT_Init();
  Key_NVIC_Init();
  if (handler_exti1 != NULL)
    exti1_handler = handler_exti1;//填充中断函数
  else
    while(1){
    };
  if (handler_exti2 != NULL)
    exti2_handler = handler_exti2;
  else
    while(1){
    };
}

/**
 * @brief        按键中断的中断服务函数
 * @param        无
 * @retval       无
 * @note         它的实际名称是 EXTI0_IRQHandler, 是 0 通道对应的中断服务程序, 底层已经将通道和程序指向好
 * @author       718 Lab
 ***/
void KEY_IRQHandle1(void)
{

  if (EXTI_GetFlagStatus(KEY1_EXTI_LINE))
  {
    if (KEY1 == 0)
    {
      if (exti1_handler != NULL)
        exti1_handler(); //调用函数
      else
        while(1){
        };
    }
    EXTI_ClearITPendingBit(KEY1_EXTI_LINE);
  }
}

/**
 * @brief        按键中断的中断服务函数
 * @param        无
 * @retval       无
 * @note         它的实际名称是 EXTI15_10_IRQHandler, 是15到10通道对应的中断服务程序, 底层已经将通道和程序指向好
 * @author       718 Lab
 ***/
void KEY_IRQHandle2(void)
{
  if (EXTI_GetFlagStatus(KEY2_EXTI_LINE))
  {
    if (KEY2 == 0)
    {
      if (exti2_handler != NULL)
        exti2_handler();
      else
        while(1){
        };
    }
    EXTI_ClearITPendingBit(KEY2_EXTI_LINE);
  }
}
