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
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //ʹ��GPIOAʱ��
	
	//USART1_TX   PA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9

	//USART1_RX	  PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA10
}

//UART1���ж����ȼ���ʼ��
void UART1_NVIC_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		//����1�����ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��VIC�Ĵ���
}

//UART1_RX��DMA��ʼ��
void UART1_DMA_RX_Init(void)
{
	DMA_InitTypeDef DMA_UART1_RX;
	
	//��Ӧ��DMA����
	DMA_DeInit(DMA1_Channel5);   //��DMA��ͨ��5�Ĵ�������Ϊȱʡֵ  ����1��Ӧ����DMAͨ��5
	DMA_UART1_RX.DMA_PeripheralBaseAddr = (u32)&USART1->DR;  //DMA����ADC����ַ
	DMA_UART1_RX.DMA_MemoryBaseAddr = (u32)DMA_Rece_Buf;  //DMA�ڴ����ַ
	DMA_UART1_RX.DMA_DIR = DMA_DIR_PeripheralSRC;  //���ݴ��䷽�򣬴������ȡ���͵��ڴ�
	DMA_UART1_RX.DMA_BufferSize = DMA_Rec_Len;  //DMAͨ����DMA����Ĵ�С
	DMA_UART1_RX.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
	DMA_UART1_RX.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
	DMA_UART1_RX.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
	DMA_UART1_RX.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ
	DMA_UART1_RX.DMA_Mode = DMA_Mode_Normal;  //��������������ģʽ
	DMA_UART1_RX.DMA_Priority = DMA_Priority_Medium; //DMAͨ�� xӵ�������ȼ� 
	DMA_UART1_RX.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel5, &DMA_UART1_RX);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���
		
	DMA_Cmd(DMA1_Channel5, ENABLE);  //��ʽ����DMA����
}

//UART1_TX��DMA��ʼ��
void UART1_DMA_TX_Init(void)
{
	DMA_InitTypeDef DMA_UART1_TX;
	
	//��Ӧ��DMA����
	DMA_DeInit(DMA1_Channel4);   //��DMA��ͨ��5�Ĵ�������Ϊȱʡֵ  ����1��Ӧ����DMAͨ��5
	DMA_UART1_TX.DMA_PeripheralBaseAddr = (u32)&USART1->DR;  //DMA����ADC����ַ
	DMA_UART1_TX.DMA_MemoryBaseAddr = (u32)DMA_Tx_Buf;  //DMA�ڴ����ַ
	DMA_UART1_TX.DMA_DIR = DMA_DIR_PeripheralDST;  //���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
	DMA_UART1_TX.DMA_BufferSize = DMA_Tx_Len;  //DMAͨ����DMA����Ĵ�С
	DMA_UART1_TX.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
	DMA_UART1_TX.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
	DMA_UART1_TX.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
	DMA_UART1_TX.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ
	DMA_UART1_TX.DMA_Mode = DMA_Mode_Normal;  //��������������ģʽ
	DMA_UART1_TX.DMA_Priority = DMA_Priority_Medium; //DMAͨ�� xӵ�������ȼ� 
	DMA_UART1_TX.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel4, &DMA_UART1_TX);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���
		
	DMA_Cmd(DMA1_Channel4, ENABLE);  //��ʽ����DMA����
}

//��ʼ��IO ����1 
//bound:������
void uart_init(u32 bound)
{
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//ʹ��USART1ʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����
	
	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound; //���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No; //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
	UART1_GPIO_Init(); //UART1��GPIO��ʼ��
	UART1_NVIC_Init(); //UART1���ж����ȼ���ʼ��
	
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE); //���������ж�
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE); //ʹ�ܴ���1��DMA����
	USART_Cmd(USART1, ENABLE); //ʹ�ܴ��� 
	
	UART1_DMA_RX_Init(); //UART1_RX��EDMA���ܳ�ʼ��
	set_gnss(3);
}

//����ʹ��UART1_RX��DMA����
void UART1_RX_DMA_Enable(void)
{ 
	USART_DMACmd(USART1, USART_DMAReq_Rx, DISABLE); //��ֹͣ����1��DMA����
	DMA_Cmd(DMA1_Channel5, DISABLE ); //��ֹͣDMA����ͣ���� 
 	DMA_SetCurrDataCounter(DMA1_Channel5, DMA_Rec_Len); //DMAͨ����DMA����Ĵ�С
 	DMA_Cmd(DMA1_Channel5, ENABLE); //ʹ��USART1 TX DMA1 ��ָʾ��ͨ�� 
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE); //ʹ�ܴ���1��DMA����
}	

//����ʹ��UART1_TX��DMA����
void UART1_TX_DMA_Enable(void)
{ 
	USART_DMACmd(USART1, USART_DMAReq_Tx, DISABLE); //��ֹͣ����1��DMA����
	DMA_Cmd(DMA1_Channel4, DISABLE ); //��ֹͣDMA����ͣ���� 
 	DMA_SetCurrDataCounter(DMA1_Channel4, DMA_Tx_Len); //DMAͨ����DMA����Ĵ�С
 	DMA_Cmd(DMA1_Channel4, ENABLE); //ʹ��USART1 TX DMA1 ��ָʾ��ͨ�� 
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE); //ʹ�ܴ���1��DMA����
}	

//����len���ֽ�.
//buf:�������׵�ַ
//len:���͵��ֽ���
void Usart1_Send(u8 *buf,u8 len)
{
	u8 t;
  	for(t=0;t<len;t++)		//ѭ����������
	{		   
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	  
		USART_SendData(USART1,buf[t]);
	}	 
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
}
u8 usart1_rx_flag;
char gsa_data[100]={0};
void USART1_IRQHandler(void)                	//����1�жϷ������
{
	//����1�����ж�
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)  
	{	
		/* 1.�������1�����жϱ�־  (�ȶ�USART_SR��Ȼ���USART_DR)*/
		USART_ClearITPendingBit(USART1, USART_IT_IDLE); //��USART_SR
		USART_ReceiveData(USART1); 											//Ȼ���USART_DR
		
		/* 2.��ȡDMA */
		Usart1_Rec_Cnt = DMA_Rec_Len - DMA_GetCurrDataCounter(DMA1_Channel5); //���ո������ڽ��ջ������ܴ�С���Ѿ����յĸ���
		str2str_TX((char *)DMA_Tx_Buf,(char *)DMA_Rece_Buf);
		usart1_rx_flag=1;
		u16 num=0;
		while(DMA_Rece_Buf[num]!='\0'&&DMA_Rece_Buf[num]!='\r'&&DMA_Rece_Buf[num]!='\n'){
			gsa_data[num]=DMA_Rece_Buf[num];
			num++;
		}
//		/* 3.�������ݽ����������� */
//		memset(DMA_Tx_Buf,'\0',sizeof(DMA_Tx_Buf));				//�����DMA_Tx_Buf
//		memcpy(DMA_Tx_Buf, DMA_Rece_Buf, Usart1_Rec_Cnt); //������ת��ͨ������1��DMA��ʽ���ͳ�ȥ����

		/* 4.�����µ�һ��DMA���� */
		UART1_RX_DMA_Enable(); //����ʹ��DMA���ȴ���һ�εĽ���
		memset(DMA_Rece_Buf,'\0',sizeof(DMA_Rece_Buf));//���DMA_Rece_Buf
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

