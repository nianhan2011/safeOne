#include "height.h"
#include "string.h"
#include <stdlib.h>
#include "stm32f10x.h"
#include "delay.h"
int presure;
 int height;
//float height;
//int presure;

#define             heightUSART_BAUD_RATE                       115200

#define             heightUSARTx                                UART5
#define             heightUSART_APBxClock_FUN                   RCC_APB1PeriphClockCmd
#define             heightUSART_CLK                             RCC_APB1Periph_UART5
#define             heightUSART_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             heightUSART_GPIO_CLK                        (RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOC)     
#define             heightUSART_TX_PORT                         GPIOC   
#define             heightUSART_TX_PIN                          GPIO_Pin_12
#define             heightUSART_RX_PORT                         GPIOD
#define             heightUSART_RX_PIN                          GPIO_Pin_2
#define             heightUSART_IRQ                             UART5_IRQn
#define             heightUSART_INT_FUN                         UART5_IRQHandler
void UART5_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NBNVIC_InitStructure;
	
	/* config USART5 clock */
	heightUSART_APBxClock_FUN(heightUSART_CLK, ENABLE);
	heightUSART_GPIO_APBxClock_FUN(heightUSART_GPIO_CLK, ENABLE);

	/* USART5 GPIO config */
   /* Configure USART5 Tx (PC.10) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = heightUSART_TX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(heightUSART_TX_PORT, &GPIO_InitStructure);
	    
  /* Configure USART5 Rx (PC.11) as input floating */
  GPIO_InitStructure.GPIO_Pin = heightUSART_RX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(heightUSART_RX_PORT, &GPIO_InitStructure);
	  
	USART_DeInit(heightUSARTx);
	/* USART5 mode config */
	USART_InitStructure.USART_BaudRate = heightUSART_BAUD_RATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
	USART_Init(heightUSARTx, &USART_InitStructure); 
	
	USART_ClearITPendingBit(heightUSARTx,USART_IT_RXNE);
	USART_ITConfig(heightUSARTx, USART_IT_RXNE, ENABLE);
	
  //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NBNVIC_InitStructure.NVIC_IRQChannel = heightUSART_IRQ;
  NBNVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NBNVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NBNVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NBNVIC_InitStructure);	
	
	USART_Cmd(heightUSARTx, ENABLE);
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
static void Usart_SendStr_length( USART_TypeDef * pUSARTx, uint8_t *str,uint32_t strlen )
{
	unsigned int k=0;
    do 
    {
        Usart_SendByte( pUSARTx, *(str + k) );
        k++;
    } while(k < strlen);
}

/*****************  发送字符串 **********************/
static void Usart_SendString( USART_TypeDef * pUSARTx, uint8_t *str)
{
	unsigned int k=0;
    do 
    {
        Usart_SendByte( pUSARTx, *(str + k) );
        k++;
    } while(*(str + k)!='\0');
}
u8 cmd1[30] = "https://www.wit-motion.cn/\n";
u8 cmd2[20] = {0xFF, 0xAA, 0x09, 0x00, 0x00, 0x0a};
u8 cmd3[20] = {0xFF, 0xAA, 0x0d, 0x03, 0x00, 0x0a};
void set_0_freq(u8 freq)
{
	Usart_SendStr_length(heightUSARTx, cmd1, 29);
	delay_ms(100);
	Usart_SendStr_length(heightUSARTx, cmd2, 5);
	delay_ms(100);
}

void set_height_zero()
{
	delay_ms(1000);
	Usart_SendStr_length(heightUSARTx, cmd1, 27);
	delay_ms(100);
	Usart_SendStr_length(heightUSARTx, cmd3, 6);
	height = 0;
}


int usart5_rx_flag = 0;
int usart5_rx_done = 0;
int usart5_rx_done_cnt = 0;
char usart5_rx_buf[100] = {0};
u32 usart5_rx_cnt = 0;
void heightUSART_INT_FUN(){
	if (USART_GetFlagStatus(heightUSARTx, USART_FLAG_PE) != RESET)
       {
          USART_ReceiveData(heightUSARTx);
         USART_ClearFlag(heightUSARTx, USART_FLAG_PE);
       }
        
       if (USART_GetFlagStatus(heightUSARTx, USART_FLAG_ORE) != RESET)
       {
           USART_ReceiveData(heightUSARTx);
         USART_ClearFlag(heightUSARTx, USART_FLAG_ORE);
       }
        
        if (USART_GetFlagStatus(heightUSARTx, USART_FLAG_FE) != RESET)
       {
           USART_ReceiveData(heightUSARTx);
          USART_ClearFlag(heightUSARTx, USART_FLAG_FE);
       }
        
        if(USART_GetITStatus(heightUSARTx, USART_IT_RXNE) != RESET)
        {   
            USART_ClearFlag(heightUSARTx, USART_FLAG_RXNE);
            USART_ClearITPendingBit(heightUSARTx, USART_IT_RXNE);
            usart5_rx_buf[usart5_rx_cnt++]=USART_ReceiveData(heightUSARTx);
						usart5_rx_done=1;
						usart5_rx_done_cnt = 0;
						if(usart5_rx_cnt==(100-1)){
							usart5_rx_cnt=0;
						}
        }
}

void usart5_deal()
{
	if(usart5_rx_flag)
	{
		char datatmp[200] = {0};
		if(usart5_rx_cnt>200)
			usart5_rx_cnt = 180;
		memcpy(datatmp, usart5_rx_buf, usart5_rx_cnt);
		memset(usart5_rx_buf,0, sizeof(usart5_rx_buf));
		usart5_rx_flag = 0;
		usart5_rx_done_cnt = 0;
		usart5_rx_cnt = 0;
		float htmp = 0;
		char *p = strstr(datatmp, "Pressure = ");
		if(p)
		{
			presure = atoi(p+11);
		}
		p = NULL;
		p = strstr(datatmp, "Height = ");
		if(p)
		{
		  htmp = atof(p+9);
			height = htmp*10;
		}
	}
}