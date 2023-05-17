#ifndef __RL_CONTROL_H
#define	__RL_CONTROL_H

#include "stm32f10x.h"

/* 定义控制IO的宏 */
#define Control1_OFF		   GPIO_ResetBits(GPIOB,GPIO_Pin_13)
#define Control1_ON			   GPIO_SetBits(GPIOB,GPIO_Pin_13)

#define IR_OFF		   	GPIO_SetBits(GPIOC,GPIO_Pin_1)
#define IR_ON			GPIO_ResetBits(GPIOC,GPIO_Pin_1)



#define MOTOR_ZZ		   	 GPIO_SetBits(GPIOB,GPIO_Pin_6);GPIO_ResetBits(GPIOB,GPIO_Pin_7)
#define MOTOR_FZ			   GPIO_ResetBits(GPIOB,GPIO_Pin_6);GPIO_SetBits(GPIOB,GPIO_Pin_7)
#define MOTOR_STOP		   GPIO_ResetBits(GPIOB,GPIO_Pin_6);GPIO_ResetBits(GPIOB,GPIO_Pin_7)


void CONTROL_GPIO_Config(void);

#endif /* __LED_H */
