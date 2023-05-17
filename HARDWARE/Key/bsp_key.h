#ifndef __KEY_H
#define	__KEY_H

#include "stm32f10x.h"

#define KEY1_INT_GPIO_PORT         GPIOC
#define KEY1_INT_GPIO_CLK          (RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO)
#define KEY1_INT_GPIO_PIN          GPIO_Pin_7
#define KEY1_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOC
#define KEY1_INT_EXTI_PINSOURCE    GPIO_PinSource7
#define KEY1_INT_EXTI_LINE         EXTI_Line7
#define KEY1_INT_EXTI_IRQ          EXTI9_5_IRQn

#define KEY1_IRQHandler            EXTI9_5_IRQHandler

#define KEY2_INT_GPIO_PORT         GPIOC
#define KEY2_INT_GPIO_CLK          (RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO)
#define KEY2_INT_GPIO_PIN          GPIO_Pin_6
#define KEY2_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOC
#define KEY2_INT_EXTI_PINSOURCE    GPIO_PinSource6
#define KEY2_INT_EXTI_LINE         EXTI_Line6
#define KEY2_INT_EXTI_IRQ          EXTI9_5_IRQn

#define KEY2_IRQHandler            EXTI9_5_IRQHandler

#define KEY3_INT_GPIO_PORT         GPIOB
#define KEY3_INT_GPIO_CLK          (RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO)
#define KEY3_INT_GPIO_PIN          GPIO_Pin_14
#define KEY3_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOB
#define KEY3_INT_EXTI_PINSOURCE    GPIO_PinSource14
#define KEY3_INT_EXTI_LINE         EXTI_Line14
#define KEY3_INT_EXTI_IRQ          EXTI15_10_IRQn

#define KEY3_IRQHandler            EXTI15_10_IRQHandler

#define KEY4_INT_GPIO_PORT         GPIOB
#define KEY4_INT_GPIO_CLK          RCC_APB2Periph_GPIOB
#define KEY4_INT_GPIO_PIN          GPIO_Pin_15
#define KEY4_INT_EXTI_LINE         EXTI_Line15


#define KEY5_INT_GPIO_PORT         GPIOA
#define KEY5_INT_GPIO_CLK          (RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)
#define KEY5_INT_GPIO_PIN          GPIO_Pin_12
#define KEY5_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOA
#define KEY5_INT_EXTI_PINSOURCE    GPIO_PinSource12
#define KEY5_INT_EXTI_LINE         EXTI_Line12
#define KEY5_INT_EXTI_IRQ          EXTI15_10_IRQn

#define KEY5_IRQHandler            EXTI15_10_IRQHandler

#define KEY6_INT_GPIO_PORT         GPIOA
#define KEY6_INT_GPIO_CLK          (RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)
#define KEY6_INT_GPIO_PIN          GPIO_Pin_4
#define KEY6_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOA
#define KEY6_INT_EXTI_PINSOURCE    GPIO_PinSource4
#define KEY6_INT_EXTI_LINE         EXTI_Line4
#define KEY6_INT_EXTI_IRQ          EXTI4_IRQn

#define KEY6_IRQHandler            EXTI4_IRQHandler

#define KEYD0_INT_GPIO_PORT         GPIOA
#define KEYD0_INT_GPIO_PIN          GPIO_Pin_12

#define KEYD1_INT_GPIO_PORT         GPIOA
#define KEYD1_INT_GPIO_PIN          GPIO_Pin_15

#define KEY_ON	1
#define KEY_OFF	0

void Key_GPIO_Config(void);
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);
void Input_GPIO_Config(void);
#endif

