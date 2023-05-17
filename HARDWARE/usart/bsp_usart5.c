#include "bsp_usart5.h"
#include <stdarg.h>
#include "delay.h"
#include "bsp_key.h"
#include "bsp_GeneralTim.h" 
#include "common.h"
#include "bsp_adc.h"
/*
语音模块使用JR6001，需要有宏定义JR6001
*/

/*****************************JQ8400改为JQ8900,程序沿用**************************************/

#ifdef STM32F10X_HD

uint8_t play_flag=0;
uint8_t nozitai_flag=0;
#define             macUSART_BAUD_RATE                       9600

#define             macUSARTx                                UART4
#define             macUSART_APBxClock_FUN                   RCC_APB1PeriphClockCmd
#define             macUSART_CLK                             RCC_APB1Periph_UART4
#define             macUSART_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             macUSART_GPIO_CLK                        (RCC_APB2Periph_GPIOC)     
#define             macUSART_TX_PORT                         GPIOC   
#define             macUSART_TX_PIN                          GPIO_Pin_10
#define             macUSART_RX_PORT                         GPIOC
#define             macUSART_RX_PIN                          GPIO_Pin_11
#define             macUSART_IRQ                             UART4_IRQn
#define             macUSART_INT_FUN                         UART4_IRQHandler
void UARTx_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* config USART4 clock */
	macUSART_APBxClock_FUN(macUSART_CLK, ENABLE);
	macUSART_GPIO_APBxClock_FUN(macUSART_GPIO_CLK, ENABLE);

	/* USART4 GPIO config */
   /* Configure USART4 Tx (PC.10) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = macUSART_TX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(macUSART_TX_PORT, &GPIO_InitStructure);
	    
  /* Configure USART4 Rx (PC.11) as input floating */
  GPIO_InitStructure.GPIO_Pin = macUSART_RX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(macUSART_RX_PORT, &GPIO_InitStructure);
	  
	/* USART4 mode config */
	USART_InitStructure.USART_BaudRate = macUSART_BAUD_RATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
	
	USART_Init(macUSARTx, &USART_InitStructure); 
	
	USART_Cmd(macUSARTx, ENABLE);
	USART_ClearFlag(macUSARTx, USART_FLAG_TC);
	extern u8 vol_0[5];
	Usart_SendStr_length(macUSARTx, vol_0,sizeof(vol_0));
	delay_ms(100);
}

/*****************  发送一个字符 **********************/
static void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch )
{
	/* 发送一个字节数据到USART1 */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送完毕 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}
/*****************  指定长度的发送字符串 **********************/
void Usart_SendStr_length( USART_TypeDef * pUSARTx, uint8_t *str,uint32_t strlen )
{
	unsigned int k=0;
    do 
    {
        Usart_SendByte( pUSARTx, *(str + k) );
        k++;
    } while(k < strlen);
}

/*****************  发送字符串 **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, uint8_t *str)
{
	unsigned int k=0;
    do 
    {
        Usart_SendByte( pUSARTx, *(str + k) );
        k++;
    } while(*(str + k)!='\0');
}
u8 vol[5]={0xAA, 0x13, 0x01, 0x1E, 0xDC};
u8 vol_0[5]={0xAA, 0x13, 0x01, 0x01, 0xBf};
u8 mode_single_stop[6]={0xAA, 0x18, 0x01, 0x02, 0xC5};
u8 set_14[6]={0xAA, 0x07, 0x02, 0x00, 0x0e, 0xC1};
u8 set_13[6]={0xAA, 0x07, 0x02, 0x00, 0x0d, 0xC0};
u8 set_12[6]={0xAA, 0x07, 0x02, 0x00, 0x0c, 0xBF};
u8 set_11[6]={0xAA, 0x07, 0x02, 0x00, 0x0b, 0xBe};
u8 set_10[6]={0xAA, 0x07, 0x02, 0x00, 0x0A, 0xBD};
u8 set_09[6]={0xAA, 0x07, 0x02, 0x00, 0x09, 0xBC};
u8 set_08[6]={0xAA, 0x07, 0x02, 0x00, 0x08, 0xBB};
u8 set_07[6]={0xAA, 0x07, 0x02, 0x00, 0x07, 0xBA};
u8 set_06[6]={0xAA, 0x07, 0x02, 0x00, 0x06, 0xB9};
u8 set_05[6]={0xAA, 0x07, 0x02, 0x00, 0x05, 0xB8};
u8 set_04[6]={0xAA, 0x07, 0x02, 0x00, 0x04, 0xB7};
u8 set_03[6]={0xAA, 0x07, 0x02, 0x00, 0x03, 0xB6};
u8 set_02[6]={0xAA, 0x07, 0x02, 0x00, 0x02, 0xB5};
u8 mode_single_tune_circulation[5]={0xAA, 0x18, 0x01, 0x01, 0xC4};
u8 stop[4]={0xAA, 0x04, 0x00, 0xAE};
u8 set_01[6]={0xAA, 0x07, 0x02, 0x00, 0x01, 0xB4};
u8 yy_h_flag = 0;
u8 yy_val = 0;
void yyhy(void)
{
#if defined(JR6001)
	Usart_SendString(macUSARTx, "AF:30");
	delay_ms(100);
	Usart_SendString(macUSARTx, "B4:02");
	delay_ms(100);	
	Usart_SendString(macUSARTx, "A7:00002");
	delay_ms(100);
#elif defined(JQ8400)
	Usart_SendStr_length(macUSARTx, vol,sizeof(vol));yy_val = 30;
	delay_ms(100);
	Usart_SendStr_length(macUSARTx, mode_single_stop,sizeof(mode_single_stop));
	delay_ms(100);	
	Usart_SendStr_length(macUSARTx,set_02,sizeof(set_02));
	yy_h_flag = 1;
	delay_ms(100);
#endif	
}
void yylw(void)
{
#if defined(JQ8400)
	Usart_SendStr_length(macUSARTx, vol,sizeof(vol));yy_val = 30;
	delay_ms(100);
	Usart_SendStr_length(macUSARTx, mode_single_stop,sizeof(mode_single_stop));
	delay_ms(100);
	Usart_SendStr_length(macUSARTx, set_03,sizeof(set_03));
	yy_h_flag = 1;
	delay_ms(100);
	play_flag=0;
#endif
}
void yygd_init(void)
{
#if defined(JQ8400)
	Usart_SendStr_length(macUSARTx, vol,sizeof(vol));yy_val = 30;
	delay_ms(100);
	Usart_SendStr_length(macUSARTx, mode_single_stop,sizeof(mode_single_stop));
	delay_ms(100);
	Usart_SendStr_length(macUSARTx, set_04,sizeof(set_04));
	delay_ms(100);
	play_flag=0;
	yy_h_flag = 1;
#endif
}
void yygd_gj(void)
{
#if defined(JQ8400)
	Usart_SendStr_length(macUSARTx, vol,sizeof(vol));yy_val = 30;
	delay_ms(100);
	Usart_SendStr_length(macUSARTx, mode_single_stop,sizeof(mode_single_stop));
	delay_ms(100);
	Usart_SendStr_length(macUSARTx, set_05,sizeof(set_05));
	delay_ms(100);
	yy_h_flag = 1;
#endif
}
void yytx(void)
{
#if defined(JR6001)
	Usart_SendString(macUSARTx, "B4:01");
	delay_ms(100);
	Usart_SendString(macUSARTx, "A7:00001");
	delay_ms(100);
#elif defined(JQ8400)
	Usart_SendStr_length(macUSARTx, vol,sizeof(vol));yy_val = 10;
	delay_ms(100);
	Usart_SendStr_length(macUSARTx, mode_single_tune_circulation,sizeof(mode_single_tune_circulation));
	delay_ms(100);
	Usart_SendStr_length(macUSARTx, set_01,sizeof(set_01));
	delay_ms(100);
	yy_h_flag = 1;
#endif
}
void yydidianliang(void)
{
#if defined(JQ8400)
	Usart_SendStr_length(macUSARTx, vol,sizeof(vol));yy_val = 15;
	delay_ms(100);
	Usart_SendStr_length(macUSARTx, mode_single_stop,sizeof(mode_single_stop));
	delay_ms(100);
	Usart_SendStr_length(macUSARTx, set_09,sizeof(set_09));
	delay_ms(300);
	yy_h_flag = 1;
#endif
}
void yypilao(void)
{
#if defined(JQ8400)
	Usart_SendStr_length(macUSARTx, vol,sizeof(vol));yy_val = 30;
	delay_ms(100);
	Usart_SendStr_length(macUSARTx, mode_single_stop,sizeof(mode_single_stop));
	delay_ms(100);
	Usart_SendStr_length(macUSARTx, set_10,sizeof(set_10));
	delay_ms(100);
	yy_h_flag = 1;
#endif
}
void yyjj_jiesuo(void)
{
#if defined(JQ8400)
	Usart_SendStr_length(macUSARTx, vol,sizeof(vol));yy_val = 30;
	delay_ms(100);
	Usart_SendStr_length(macUSARTx, mode_single_stop,sizeof(mode_single_stop));
	delay_ms(100);
	Usart_SendStr_length(macUSARTx, set_11,sizeof(set_11));
	delay_ms(100);
	yy_h_flag = 1;
#endif
}
/************新增3个语音模块*****************/
void shangsuo(void)
{
#if defined(JQ8400)
	Usart_SendStr_length(macUSARTx, vol,sizeof(vol));yy_val = 30;
	delay_ms(100);
	Usart_SendStr_length(macUSARTx, mode_single_stop,sizeof(mode_single_stop));
	delay_ms(100);
	Usart_SendStr_length(macUSARTx, set_06,sizeof(set_06));
	delay_ms(100);
	yy_h_flag = 1;
#endif
}
void jiesuo(void)
{
#if defined(JQ8400)
	Usart_SendStr_length(macUSARTx, vol,sizeof(vol));yy_val = 30;
	delay_ms(100);
	Usart_SendStr_length(macUSARTx, mode_single_stop,sizeof(mode_single_stop));
	delay_ms(100);
	Usart_SendStr_length(macUSARTx, set_07,sizeof(set_07));
	delay_ms(100);
	yy_h_flag = 1;
#endif
}
void sos(void)
{
#if defined(JQ8400)
	Usart_SendStr_length(macUSARTx, vol,sizeof(vol));yy_val = 30;
	delay_ms(100);
	Usart_SendStr_length(macUSARTx, mode_single_stop,sizeof(mode_single_stop));
	delay_ms(100);
	Usart_SendStr_length(macUSARTx, set_08,sizeof(set_08));
	delay_ms(100);
	yy_h_flag = 1;
#endif
}

void weixiangd(void)//危险高度语音
{
#if defined(JQ8400)
	Usart_SendStr_length(macUSARTx, vol,sizeof(vol));yy_val = 30;
	delay_ms(100);
	Usart_SendStr_length(macUSARTx, mode_single_stop,sizeof(mode_single_stop));
	delay_ms(100);
	Usart_SendStr_length(macUSARTx, set_12,sizeof(set_12));
	delay_ms(100);
	yy_h_flag = 1;
#endif
}
//void pilaoyy(void)//疲劳语音
//{
//#if defined(JQ8400)
//	Usart_SendStr_length(macUSARTx, vol,sizeof(vol));yy_val = 5;
//	delay_ms(100);
//	Usart_SendStr_length(macUSARTx, mode_single_tune_circulation,sizeof(mode_single_tune_circulation));
//	delay_ms(100);
//	Usart_SendStr_length(macUSARTx, set_13,sizeof(set_13));
//	delay_ms(300);
//	yy_h_flag = 1;
//#endif
//}

// void waist_buckle_warn(void)//yaokou warn
// {
// #if defined(JQ8400)
// 	Usart_SendStr_length(macUSARTx, vol,sizeof(vol));yy_val = 30;
// 	delay_ms(100);
// 	Usart_SendStr_length(macUSARTx, mode_single_stop,sizeof(mode_single_stop));
// 	delay_ms(100);
// 	Usart_SendStr_length(macUSARTx, set_14,sizeof(set_14));
// 	delay_ms(100);
// 	yy_h_flag = 1;
// #endif
// }
void yygb(void)
{
#if defined(JR6001)
	Usart_SendString(macUSARTx, "A4");
	delay_ms(100);
#elif defined(JQ8400)
	Usart_SendStr_length(macUSARTx, vol,sizeof(vol));yy_val = 30;
	delay_ms(100);
	Usart_SendStr_length(macUSARTx, stop,sizeof(stop));
	delay_ms(100);
	//yy_h_flag = 1;
#endif
}
extern u8 NB_TCP_CONNECT_FLAG;
extern u8 pilao_flag;
extern u8 didian_flag;
u8 set0_flag = 0;
u8 play_flag1 = 0;
void yytxdealhs(void)
{
	if(yy_h_flag == 0 && yy_val != 5)
	{
			pilao_flag = 0;
			//yy_val = 0;
			//Usart_SendStr_length(macUSARTx, vol_0,sizeof(vol_0));
			//delay_ms(100);
	}
	if(yy_h_flag == 0 && yy_val != 15)
	{
			didian_flag=0x00;
			//yy_val = 0;
			//Usart_SendStr_length(macUSARTx, vol_0,sizeof(vol_0));
			//delay_ms(100);
	}
	if(yy_h_flag == 0 && yy_val != 10)
	{
			play_flag=0x00;
			//yy_val = 0;
			//Usart_SendStr_length(macUSARTx, vol_0,sizeof(vol_0));
			//delay_ms(100);
	}
	extern u8 peizhi[10];
	if(peizhi[0] == 1)
	{
		if(NB_TCP_CONNECT_FLAG&&(!ad_check())&&(play_flag==0x00))
		{
			if((!ad_check())&&(play_flag==0x00))
			{
				if(yy_h_flag == 0 && nozitai_flag==0)
				{
					yytx();
					play_flag1 = 1;
					play_flag=0x01;
				}
			}
		}
		if(NB_TCP_CONNECT_FLAG&&(ad_check())&&(play_flag==0x01))
		{
			if((ad_check())&&(play_flag==0x01))
			{
				if(yy_h_flag == 0)
				{
					yygb();
					play_flag=0x00;	
				}
				
			}	
		}
	}
	else
	{
		if((play_flag ==0x01)||set0_flag == 1)
		{
			if((play_flag==0x01)|| set0_flag == 1)
			{
				if(yy_h_flag == 0)
				{
					yygb();
					play_flag=0x00;	
					set0_flag = 0;
				}
				
			}	
		}
	}
	static u8 yygj_flag = 0;
	extern u8 powerdown_anjian;
	extern void yygd_gj(void);
	if(powerdown_anjian&&yygj_flag == 0)
	{
		extern u8 guanji_yy_flag;
		guanji_yy_flag = 1;
		yygj_flag = 1;
		yygd_gj();
	}
}
#endif
#ifdef STM32F10X_MD
#define             macUSART_BAUD_RATE                       9600

#define             macUSARTx                                USART3
#define             macUSART_APBxClock_FUN                   RCC_APB1PeriphClockCmd
#define             macUSART_CLK                             RCC_APB1Periph_USART3
#define             macUSART_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             macUSART_GPIO_CLK                        (RCC_APB2Periph_GPIOC)     
#define             macUSART_TX_PORT                         GPIOC   
#define             macUSART_TX_PIN                          GPIO_Pin_10
#define             macUSART_RX_PORT                         GPIOC
#define             macUSART_RX_PIN                          GPIO_Pin_11
#define             macUSART_IRQ                             UART3_IRQn
#define             macUSART_INT_FUN                         UART3_IRQHandler
uint8_t play_flag=0;

void UARTx_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_PartialRemap_USART3,ENABLE);
	/* config USART4 clock */
	macUSART_APBxClock_FUN(macUSART_CLK, ENABLE);
	macUSART_GPIO_APBxClock_FUN(macUSART_GPIO_CLK, ENABLE);

	/* USART4 GPIO config */
   /* Configure USART4 Tx (PC.10) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = macUSART_TX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(macUSART_TX_PORT, &GPIO_InitStructure);
	    
  /* Configure USART4 Rx (PC.11) as input floating */
  GPIO_InitStructure.GPIO_Pin = macUSART_RX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(macUSART_RX_PORT, &GPIO_InitStructure);
	  
	/* USART4 mode config */
	USART_InitStructure.USART_BaudRate = macUSART_BAUD_RATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
	
	USART_Init(macUSARTx, &USART_InitStructure); 
	
	USART_Cmd(macUSARTx, ENABLE);
	USART_ClearFlag(macUSARTx, USART_FLAG_TC);
}

/*****************  发送一个字符 **********************/
static void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch )
{
	/* 发送一个字节数据到USART1 */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送完毕 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}
/*****************  指定长度的发送字符串 **********************/
void Usart_SendStr_length( USART_TypeDef * pUSARTx, uint8_t *str,uint32_t strlen )
{
	unsigned int k=0;
    do 
    {
        Usart_SendByte( pUSARTx, *(str + k) );
        k++;
    } while(k < strlen);
}

/*****************  发送字符串 **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, uint8_t *str)
{
	unsigned int k=0;
    do 
    {
        Usart_SendByte( pUSARTx, *(str + k) );
        k++;
    } while(*(str + k)!='\0');
}
u8 vol[5]={0xAA, 0x13, 0x01, 0x1E, 0xDC};
u8 mode_single_stop[6]={0xAA, 0x18, 0x01, 0x02, 0xC5};
u8 set_02[6]={0xAA, 0x07, 0x02, 0x00, 0x02, 0xB5};
u8 mode_single_tune_circulation[5]={0xAA, 0x18, 0x01, 0x01, 0xC4};
u8 stop[4]={0xAA, 0x04, 0x00, 0xAE};
u8 set_01[6]={0xAA, 0x07, 0x02, 0x00, 0x01, 0xB4};
void yyhy(void)
{
#if defined(JR6001)
	Usart_SendString(macUSARTx, "AF:30");
	delay_ms(100);
	Usart_SendString(macUSARTx, "B4:02");
	delay_ms(100);	
	Usart_SendString(macUSARTx, "A7:00002");
	delay_ms(100);
#elif defined(JQ8400)
	Usart_SendStr_length(macUSARTx, vol,sizeof(vol));yy_val = 30;
	delay_ms(100);
	Usart_SendStr_length(macUSARTx, mode_single_stop,sizeof(mode_single_stop));
	delay_ms(100);	
	Usart_SendStr_length(macUSARTx,set_02,sizeof(set_02));
	delay_ms(100);
#endif	
}

void yytx(void)
{
#if defined(JR6001)
	Usart_SendString(macUSARTx, "B4:01");
	delay_ms(100);
	Usart_SendString(macUSARTx, "A7:00001");
	delay_ms(100);
#elif defined(JQ8400)
	Usart_SendStr_length(macUSARTx, mode_single_tune_circulation,sizeof(mode_single_tune_circulation));
	delay_ms(100);
	Usart_SendStr_length(macUSARTx, set_01,sizeof(set_01));
	delay_ms(100);
#endif
}

void yygb(void)
{
#if defined(JR6001)
	Usart_SendString(macUSARTx, "A4");
	delay_ms(100);
#elif defined(JQ8400)
	Usart_SendStr_length(macUSARTx, stop,sizeof(stop));
	delay_ms(100);
#endif
}

void yytxdealhs(void)
{
	if((!ad_check())&&(play_flag==0x00))
	{
		delay_ms(100);
		if((!ad_check())&&(play_flag==0x00))
		{
			yytx();
		  play_flag=0x01;
		}
	}
	if((ad_check())&&(play_flag==0x01))
	{
		delay_ms(100);
		if((ad_check())&&(play_flag==0x01))
	  {
		  yygb();
	  	play_flag=0x00;	
	  }
	}
}
#endif
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
