#include "bsp_led.h"   

void LED_GPIO_Config(void)
{		
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO| LED1_GPIO_CLK | LED2_GPIO_CLK | LED3_GPIO_CLK | LED4_GPIO_CLK | LED5_GPIO_CLK | LED6_GPIO_CLK |LED7_GPIO_CLK, ENABLE);
	
	
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
		GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);	
	
		GPIO_InitStructure.GPIO_Pin = LED2_GPIO_PIN;
		GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     
		GPIO_InitStructure.GPIO_Pin = LED3_GPIO_PIN;
		GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = LED4_GPIO_PIN;
		GPIO_Init(LED4_GPIO_PORT, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = LED5_GPIO_PIN;
		GPIO_Init(LED5_GPIO_PORT, &GPIO_InitStructure);
			
		GPIO_InitStructure.GPIO_Pin = LED6_GPIO_PIN;
		GPIO_Init(LED6_GPIO_PORT, &GPIO_InitStructure); 

		GPIO_InitStructure.GPIO_Pin = LED7_GPIO_PIN;
		GPIO_Init(LED7_GPIO_PORT, &GPIO_InitStructure); 
		LED1_OFF;
		LED2_OFF;	
		LED3_ON;
		GPIO_SetBits(LED4_GPIO_PORT, LED4_GPIO_PIN);
		GPIO_SetBits(LED5_GPIO_PORT, LED5_GPIO_PIN);
		GPIO_SetBits(LED6_GPIO_PORT, LED6_GPIO_PIN);	
		GPIO_ResetBits(LED7_GPIO_PORT, LED7_GPIO_PIN);	
}

void assert_failed(uint8_t* file, uint32_t line)
{
	LED1_ON;
//	shangsuo_flag=0;
}
/*********************************************END OF FILE**********************/
