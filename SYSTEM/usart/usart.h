#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

#define DMA_Rec_Len 500
#define DMA_Tx_Len 1000  
extern u16  Usart1_Rec_Cnt;            
extern u8  DMA_Tx_Buf[DMA_Tx_Len];	 
extern u16  Usart1_Tx_Cnt;   
extern u8  DMA_Rece_Buf[DMA_Rec_Len];	

//如果想串口中断接收，请不要注释以下宏定义
void set_gnss(u8 value);
void uart_init(u32 bound);
void UART1_RX_DMA_Enable(void);


#endif


