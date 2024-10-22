#ifndef __DRV_FLASHES__
#define __DRV_FLASHES__

#include "stm32f10x.h"
#include "stdlib.h"

#define TRUE (1)
#define FALSE (0)

/*--------------------分割线---------------------*/

// LED1
#define LED1_GPIO_PORT GPIOF
#define LED1_GPIO_CLK RCC_APB2Periph_GPIOF
#define LED1_GPIO_PIN GPIO_Pin_11

// LED2
#define LED2_GPIO_PORT GPIOF               /* GPIO端口 */
#define LED2_GPIO_CLK RCC_APB2Periph_GPIOF /* GPIO端口时钟 */
#define LED2_GPIO_PIN GPIO_Pin_12          /* 连接到SCL时钟线的GPIO */

#define digitalToggle(p, i) \
    {                       \
        p->ODR ^= i;        \
    } //输出反转状态

extern void LED1_Flashes_ON(void);
extern void LED1_Flashes_OFF(void);
extern void LED2_Flashes_ON(void);
extern void LED2_Flashes_OFF(void);
extern void LED1_Flashes_TOGGLE(void);
extern void LED2_Flashes_TOGGLE(void);
extern void LED_Flashes_Init(void);

/*--------------------分割线---------------------*/

#define Flashes_TIM TIM7
#define Flashes_TIM_APBxClock_FUN RCC_APB1PeriphClockCmd
#define Flashes_TIM_CLK RCC_APB1Periph_TIM7
#define Flashes_TIM_IRQ TIM7_IRQn
#define Flashes_TIM_IRQHandler TIM7_IRQHandler

extern uint32_t Get_Tick(void);
extern void Claer_Tick(void);
extern void TIM_Flashes_PreConfig(uint16_t Prescaler);
extern void TIM_Flashes_ARRConfig(uint16_t ARR);
extern void TIM_Flashes_Init(uint16_t arr, uint16_t psc, void (*handler)());

/*--------------------分割线---------------------*/

#define KEY_IRQHandle1  EXTI0_IRQHandler // KEY1
#define KEY_IRQn1       EXTI0_IRQn
#define KEY_IRQHandle2  EXTI15_10_IRQHandler // KEY2
#define KEY_IRQn2       EXTI15_10_IRQn

// KEY defination
//  KEY1
#define KEY1_GPIO_PORT GPIOA               /* GPIO 端口 */
#define KEY1_GPIO_CLK RCC_APB2Periph_GPIOA /* GPIO 端口时钟 */
#define KEY1_GPIO_PIN GPIO_Pin_0           /* 连接到 SCL 时钟线的 GPIO */

// KEY2
#define KEY2_GPIO_PORT GPIOC               /* GPIO 端口 */
#define KEY2_GPIO_CLK RCC_APB2Periph_GPIOC /* GPIO 端口时钟 */
#define KEY2_GPIO_PIN GPIO_Pin_13           /* 连接到SCL时钟线的 GPIO */

// KEY1 EXTI
#define KEY1_EXTI_PORT GPIO_PortSourceGPIOA
#define KEY1_EXTI_PIN GPIO_PinSource0
#define KEY1_EXTI_LINE EXTI_Line0

// KEY2 EXTI
#define KEY2_EXTI_PORT GPIO_PortSourceGPIOC
#define KEY2_EXTI_PIN GPIO_PinSource13
#define KEY2_EXTI_LINE EXTI_Line13

#define KEY1 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) // 读取KEY1
#define KEY2 GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) // 读取KEY2

extern void GPIO_EXIT_Init(void (*handler_key1)(), void (*handler_key2)());

#endif
