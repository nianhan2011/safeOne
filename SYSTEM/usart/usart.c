#include "sys.h"
#include "usart.h"	

#include "string.h"
#include <stdarg.h>
#include "stdlib.h"
#include "nb_model.h"
u8  DMA_Rece_Buf[DMA_Rec_Len];	  
u16  Usart1_Rec_Cnt=0;            
u8  DMA_Tx_Buf[DMA_Tx_Len];	 
u16  Usart1_Tx_Cnt=0;          
void UART1_TX_DMA_Enable(void);
void UART1_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能GPIOA时钟
	
	//USART1_TX   PA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9

	//USART1_RX	  PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA10
}

//UART1的中断优先级初始化
void UART1_NVIC_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		//串口1接收中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化VIC寄存器
}

//UART1_RX的DMA初始化
void UART1_DMA_RX_Init(void)
{
	DMA_InitTypeDef DMA_UART1_RX;
	
	//相应的DMA配置
	DMA_DeInit(DMA1_Channel5);   //将DMA的通道5寄存器重设为缺省值  串口1对应的是DMA通道5
	DMA_UART1_RX.DMA_PeripheralBaseAddr = (u32)&USART1->DR;  //DMA外设ADC基地址
	DMA_UART1_RX.DMA_MemoryBaseAddr = (u32)DMA_Rece_Buf;  //DMA内存基地址
	DMA_UART1_RX.DMA_DIR = DMA_DIR_PeripheralSRC;  //数据传输方向，从外设读取发送到内存
	DMA_UART1_RX.DMA_BufferSize = DMA_Rec_Len;  //DMA通道的DMA缓存的大小
	DMA_UART1_RX.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
	DMA_UART1_RX.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
	DMA_UART1_RX.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
	DMA_UART1_RX.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
	DMA_UART1_RX.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
	DMA_UART1_RX.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
	DMA_UART1_RX.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA1_Channel5, &DMA_UART1_RX);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
		
	DMA_Cmd(DMA1_Channel5, ENABLE);  //正式驱动DMA传输
}

//UART1_TX的DMA初始化
void UART1_DMA_TX_Init(void)
{
	DMA_InitTypeDef DMA_UART1_TX;
	
	//相应的DMA配置
	DMA_DeInit(DMA1_Channel4);   //将DMA的通道5寄存器重设为缺省值  串口1对应的是DMA通道5
	DMA_UART1_TX.DMA_PeripheralBaseAddr = (u32)&USART1->DR;  //DMA外设ADC基地址
	DMA_UART1_TX.DMA_MemoryBaseAddr = (u32)DMA_Tx_Buf;  //DMA内存基地址
	DMA_UART1_TX.DMA_DIR = DMA_DIR_PeripheralDST;  //数据传输方向，从内存读取发送到外设
	DMA_UART1_TX.DMA_BufferSize = DMA_Tx_Len;  //DMA通道的DMA缓存的大小
	DMA_UART1_TX.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
	DMA_UART1_TX.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
	DMA_UART1_TX.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
	DMA_UART1_TX.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
	DMA_UART1_TX.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
	DMA_UART1_TX.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
	DMA_UART1_TX.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA1_Channel4, &DMA_UART1_TX);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
		
	DMA_Cmd(DMA1_Channel4, ENABLE);  //正式驱动DMA传输
}

//初始化IO 串口1 
//bound:波特率
void uart_init(u32 bound)
{
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//使能USART1时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输
	
	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound; //串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No; //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
	UART1_GPIO_Init(); //UART1的GPIO初始化
	UART1_NVIC_Init(); //UART1的中断优先级初始化
	
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE); //开启空闲中断
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE); //使能串口1的DMA接收
	USART_Cmd(USART1, ENABLE); //使能串口 
	
	UART1_DMA_RX_Init(); //UART1_RX的EDMA功能初始化
	set_gnss(3);
}

//重新使能UART1_RX的DMA功能
void UART1_RX_DMA_Enable(void)
{ 
	USART_DMACmd(USART1, USART_DMAReq_Rx, DISABLE); //先停止串口1的DMA接收
	DMA_Cmd(DMA1_Channel5, DISABLE ); //先停止DMA，暂停接收 
 	DMA_SetCurrDataCounter(DMA1_Channel5, DMA_Rec_Len); //DMA通道的DMA缓存的大小
 	DMA_Cmd(DMA1_Channel5, ENABLE); //使能USART1 TX DMA1 所指示的通道 
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE); //使能串口1的DMA接收
}	

//重新使能UART1_TX的DMA功能
void UART1_TX_DMA_Enable(void)
{ 
	USART_DMACmd(USART1, USART_DMAReq_Tx, DISABLE); //先停止串口1的DMA发送
	DMA_Cmd(DMA1_Channel4, DISABLE ); //先停止DMA，暂停发送 
 	DMA_SetCurrDataCounter(DMA1_Channel4, DMA_Tx_Len); //DMA通道的DMA缓存的大小
 	DMA_Cmd(DMA1_Channel4, ENABLE); //使能USART1 TX DMA1 所指示的通道 
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE); //使能串口1的DMA发送
}	

//发送len个字节.
//buf:发送区首地址
//len:发送的字节数
void Usart1_Send(u8 *buf,u8 len)
{
	u8 t;
  	for(t=0;t<len;t++)		//循环发送数据
	{		   
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	  
		USART_SendData(USART1,buf[t]);
	}	 
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
}
u8 usart1_rx_flag;
char gsa_data[100]={0};
void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	//串口1空闲中断
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)  
	{	
		/* 1.清除串口1空闲中断标志  (先读USART_SR，然后读USART_DR)*/
		USART_ClearITPendingBit(USART1, USART_IT_IDLE); //读USART_SR
		USART_ReceiveData(USART1); 											//然后读USART_DR
		
		/* 2.读取DMA */
		Usart1_Rec_Cnt = DMA_Rec_Len - DMA_GetCurrDataCounter(DMA1_Channel5); //接收个数等于接收缓冲区总大小减已经接收的个数
		str2str_TX((char *)DMA_Tx_Buf,(char *)DMA_Rece_Buf);
		usart1_rx_flag=1;
		u16 num=0;
		while(DMA_Rece_Buf[num]!='\0'&&DMA_Rece_Buf[num]!='\r'&&DMA_Rece_Buf[num]!='\n'){
			gsa_data[num]=DMA_Rece_Buf[num];
			num++;
		}
//		/* 3.搬移数据进行其他处理 */
//		memset(DMA_Tx_Buf,'\0',sizeof(DMA_Tx_Buf));				//先清空DMA_Tx_Buf
//		memcpy(DMA_Tx_Buf, DMA_Rece_Buf, Usart1_Rec_Cnt); //将接收转移通过串口1的DMA方式发送出去测试

		/* 4.开启新的一次DMA接收 */
		UART1_RX_DMA_Enable(); //重新使能DMA，等待下一次的接收
		memset(DMA_Rece_Buf,'\0',sizeof(DMA_Rece_Buf));//清空DMA_Rece_Buf
  }
	
} 
u8 itoa(u8 i){
	if(i<=9){
		return i+'0';
	}else if(i<=0x0f){
		return i-0x0a+'A';
	}
	return 0;
}
/*
0x01: GPS
0x02: BDS
0x04: GLONASS
*/
void set_gnss(u8 value){
	char str_tmp[25]="$PCAS04,2*1B\r\n";
	str_tmp[8]=value+0x30;
	str_tmp[10]=0;
	str_tmp[11]=0;
	for(u8 i=1;i<9;i++){
		str_tmp[10]^=(str_tmp[i]>>4);
		str_tmp[11]^=(str_tmp[i]&0x0f);
	}
	str_tmp[10]=itoa(str_tmp[10]);
	str_tmp[11]=itoa(str_tmp[11]);
	Usart1_Send((u8 *)str_tmp,strlen(str_tmp));
}

