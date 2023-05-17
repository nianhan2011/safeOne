#ifndef __LED_H
#define	__LED_H

#include "stm32f10x.h"

// R-��ɫ
#define LED1_GPIO_PORT    	GPIOB			             
#define LED1_GPIO_CLK 	    RCC_APB2Periph_GPIOB		
#define LED1_GPIO_PIN		GPIO_Pin_3			

// G-��ɫ
#define LED2_GPIO_PORT    	GPIOB			      
#define LED2_GPIO_CLK 	    RCC_APB2Periph_GPIOB		
#define LED2_GPIO_PIN		GPIO_Pin_4			 

// B-��ɫ
#define LED3_GPIO_PORT    	GPIOC			            
#define LED3_GPIO_CLK 	    RCC_APB2Periph_GPIOC		
#define LED3_GPIO_PIN		GPIO_Pin_8		

#define LED4_GPIO_PORT    	GPIOC			            
#define LED4_GPIO_CLK 	    RCC_APB2Periph_GPIOC		
#define LED4_GPIO_PIN		GPIO_Pin_9	


// ����ָʾ��
#define LED5_GPIO_PORT    	GPIOA			            
#define LED5_GPIO_CLK 	    RCC_APB2Periph_GPIOA		
#define LED5_GPIO_PIN		GPIO_Pin_8		

#define LED6_GPIO_PORT    	GPIOA			            
#define LED6_GPIO_CLK 	    RCC_APB2Periph_GPIOA		
#define LED6_GPIO_PIN		GPIO_Pin_11	

//����ָʾ��
#define LED7_GPIO_PORT    	GPIOA			            
#define LED7_GPIO_CLK 	    RCC_APB2Periph_GPIOA		
#define LED7_GPIO_PIN		GPIO_Pin_1	

#define ON  0
#define OFF 1

/* ʹ�ñ�׼�Ĺ̼������IO*/
#define LED1(a)	if (a)	\
					GPIO_SetBits(LED1_GPIO_PORT,LED1_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED1_GPIO_PORT,LED1_GPIO_PIN)

#define LED2(a)	if (a)	\
					GPIO_SetBits(LED2_GPIO_PORT,LED2_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED2_GPIO_PORT,LED2_GPIO_PIN)

#define LED3(a)	if (a)	\
					GPIO_SetBits(LED3_GPIO_PORT,LED3_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED3_GPIO_PORT,LED3_GPIO_PIN)


/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //���Ϊ�ߵ�ƽ		
#define digitalLo(p,i)		 {p->BRR=i;}	 //����͵�ƽ
#define digitalToggle(p,i) {p->ODR ^=i;} //�����ת״̬


/* �������IO�ĺ� */
#define LED2_TOGGLE		 digitalToggle(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED2_OFF		   digitalLo(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED2_ON			   digitalHi(LED1_GPIO_PORT,LED1_GPIO_PIN)

#define LED1_TOGGLE		 digitalToggle(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED1_OFF		   digitalLo(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED1_ON			   digitalHi(LED2_GPIO_PORT,LED2_GPIO_PIN)

#define LED3_TOGGLE		 digitalToggle(LED3_GPIO_PORT,LED3_GPIO_PIN)
#define LED3_OFF		   digitalHi(LED3_GPIO_PORT,LED3_GPIO_PIN)
#define LED3_ON			   digitalLo(LED3_GPIO_PORT,LED3_GPIO_PIN)

#define LED4_TOGGLE		 digitalToggle(LED4_GPIO_PORT,LED4_GPIO_PIN)
#define LED4_OFF		   digitalHi(LED4_GPIO_PORT,LED4_GPIO_PIN)
#define LED4_ON			   digitalLo(LED4_GPIO_PORT,LED4_GPIO_PIN)

#define LED5_TOGGLE		 digitalToggle(LED5_GPIO_PORT,LED5_GPIO_PIN)
#define LED5_OFF		   digitalHi(LED5_GPIO_PORT,LED5_GPIO_PIN)
#define LED5_ON			   digitalLo(LED5_GPIO_PORT,LED5_GPIO_PIN)

#define LED6_TOGGLE		 digitalToggle(LED6_GPIO_PORT,LED6_GPIO_PIN)
#define LED6_OFF		   digitalHi(LED6_GPIO_PORT,LED6_GPIO_PIN)
#define LED6_ON			   digitalLo(LED6_GPIO_PORT,LED6_GPIO_PIN)

#define LED7_TOGGLE		 digitalToggle(LED7_GPIO_PORT,LED7_GPIO_PIN)
#define LED7_OFF		   digitalLo(LED7_GPIO_PORT,LED7_GPIO_PIN)
#define LED7_ON			   digitalHi(LED7_GPIO_PORT,LED7_GPIO_PIN)

#define YY_TOGGLE		 digitalToggle(GPIOC,GPIO_Pin_14)
#define YY_OFF		   digitalLo(GPIOC,GPIO_Pin_14)
#define YY_ON			   digitalHi(GPIOC,GPIO_Pin_14)
void LED_GPIO_Config(void);

#endif /* __LED_H */
