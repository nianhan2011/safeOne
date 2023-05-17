#include "bsp_nb.h"
#include <stdio.h>
#include "string.h"
#include "delay.h"
#include "nb_model.h"
#include "bsp_GENERALTim.h"
#include "common.h"
#include "ble.h"
#include "usart.h"
#ifdef ME3630
unsigned char NB_INIT_FLAG=0;
unsigned char NB_SOCKET_CLOSE_FLAG=0;
unsigned char NB_SOCKET_CREAT_FLAG=0;
unsigned char NB_IMEI_SEARCH_FLAG=0;
unsigned char NB_TCP_CONNECT_FLAG=0;
unsigned char NB_TCP_INIT_GPS_FLAG=0;
unsigned char NB_TCP_TX_FLAG=0;
uint32_t sszcsj_time_count=0;
uint32_t jszcsj_time_count=0;
uint8_t sszcsj_time_flag=0;
uint8_t ssbzw_flag=0;
uint8_t jsbzw_flag=0;
uint8_t sbjsbz_flag=0;
uint8_t skssys_flag=0;//延时标志
uint32_t skssys_time=0;//延时时间
uint8_t skssyswc_flag=0;//延时完成标志
uint32_t shangsuo_time=0;
uint32_t changanshijian=0;

uint8_t fly_mode_cnt = 0; // 飞行模式计数
uint8_t open_fly_mode = 0; // 飞行模式标记

// uint8_t close_fly_mode = 0; // 飞行模式标记

char nb_tx_chezaikt_shuju[1000];
char NB_TCP_CLOCK[1]={'a'};
char nb_imei_number[20]="866826031155956";
NB_IO_INFO nbioinfo;

void NBUSART_Config(void)
{
	GPIO_InitTypeDef  NBGPIO_InitStructure;
	USART_InitTypeDef NBUSART_InitStructure;
	NVIC_InitTypeDef  NBNVIC_InitStructure;
	
	NBDEBUG_USART_GPIO_APBxClkCmd(NBDEBUG_USART_GPIO_CLK, ENABLE);
	NBDEBUG_USART_APBxClkCmd(NBDEBUG_USART_CLK, ENABLE);
	
	NBGPIO_InitStructure.GPIO_Pin = NBDEBUG_USART_TX_GPIO_PIN;
	NBGPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	NBGPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(NBDEBUG_USART_TX_GPIO_PORT, &NBGPIO_InitStructure);
	
	NBGPIO_InitStructure.GPIO_Pin = NBDEBUG_USART_RX_GPIO_PIN;
	NBGPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(NBDEBUG_USART_RX_GPIO_PORT, &NBGPIO_InitStructure);
	
	USART_DeInit(USART2);
	NBUSART_InitStructure.USART_BaudRate = NBDEBUG_USART_BAUDRATE;
	NBUSART_InitStructure.USART_WordLength = USART_WordLength_8b;
	NBUSART_InitStructure.USART_StopBits = USART_StopBits_1;
	NBUSART_InitStructure.USART_Parity = USART_Parity_No ;
	NBUSART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	NBUSART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(NBDEBUG_USARTx, &NBUSART_InitStructure);
	
	USART_ClearITPendingBit(NBDEBUG_USARTx,USART_IT_RXNE);
	USART_ITConfig(NBDEBUG_USARTx, USART_IT_RXNE, ENABLE);
	
  //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NBNVIC_InitStructure.NVIC_IRQChannel = NBDEBUG_USART_IRQ;
  NBNVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NBNVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NBNVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NBNVIC_InitStructure);	
	USART_Cmd(NBDEBUG_USARTx, ENABLE);	
}

void NBUsart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	USART_SendData(pUSARTx,ch);
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/****************** 发送8位的数组 ************************/
void NBUsart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
  uint8_t i;
	for(i=0; i<num; i++)
  {
	    NBUsart_SendByte(pUSARTx,array[i]);	
  }
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

/*****************  发送字符串 **********************/
void NBUsart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      NBUsart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

/*****************  发送一个16位数 **********************/
void NBUsart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	temp_h = (ch&0XFF00)>>8;
	temp_l = ch&0XFF;
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		USART_SendData(NBDEBUG_USARTx, (uint8_t) ch);
		while (USART_GetFlagStatus(NBDEBUG_USARTx, USART_FLAG_TXE) == RESET);		
		return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		while (USART_GetFlagStatus(NBDEBUG_USARTx, USART_FLAG_RXNE) == RESET);
		return (int)USART_ReceiveData(NBDEBUG_USARTx);
}

//NB模块程序发送配置说明
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void NB_IO_Send(unsigned char *str, unsigned short len)
{	
	for(; len>0; len--)
	{
		while(USART_GetFlagStatus(NBDEBUG_USARTx, USART_FLAG_TC) == RESET);		//等待发送完成
		USART_SendData(NBDEBUG_USARTx, *str++);									//发送数据
	}
}

void NB_IO_Clear_Cache(void)
{
	nbioinfo.write_pos=0;
	nbioinfo.read_pos=0;
	nbioinfo.rev_status=0;
	memset(nbioinfo.rev_buf,0,sizeof(nbioinfo.rev_buf));
}



u8 debug_nb_flag=0;
extern u8 usart1_rx_flag;
u8 nb_time=1;
u8 nb_time_flag=1;
u8 connect_on_cnt=0;
void NB_TCP_CONNECT_1(){
	if(NB_IMEI_SEARCH_FLAG&&
		!NB_TCP_CONNECT_FLAG){
		if(nb_time_flag){
			static u16 error_cnt;
			switch(nb_time){
				case 0:	
//						++error_cnt;
//						if(error_cnt>4)
//						{
//							error_cnt=0;
							nb_time++;
//						}
						nb_time_flag=0;
						break;
				case 1: printf("AT+IPR=9600\r\n");	
						//printf("AT+COPS?\r\n");
						//printf("AT+ZSNTE=0\r\n");
						nb_time_flag=0;
						nb_time++;
						break;
				case 2: printf("AT+ZCDS?\r\n");
						nb_time_flag=0;
						nb_time++;
						break;
				case 3:	printf("AT+ZIPCFG=cmnet,None,None\r\n");
						nb_time_flag=0;
						nb_time++;
						break;
				case 4: printf("AT+ZIPCALL=1\r\n");
						nb_time_flag=0;
						nb_time++;
						break;
				case 5: 
					//printf("AT+ZIPALIVE=1,1,300,75,9\r\n");
						nb_time_flag=0;
						nb_time++;
						break;
				case 6: 
						if(debug_nb_flag)
							printf("AT+ZIPOPEN=1,0,103.46.128.49,21988\r\n");
//						printf("AT+ZIPOPEN=1,0,103.46.128.21,21774\r\n");
						else
//					  printf("AT+ZIPOPEN=1,0,47.111.112.30,10087\r\n");
//					  printf("AT+ZIPOPEN=1,0,139.159.225.180,10087\r\n");//高工
//						printf("AT+ZIPOPEN=1,0,103.46.128.49,21988\r\n");//贝贝花生壳
//			      printf("AT+ZIPOPEN=1,0,103.46.128.44,50254\r\n");//吴昌原花生壳
//						printf("AT+ZIPOPEN=1,0,103.46.128.21,12657\r\n");//孙华龙花生壳
//						printf("AT+ZIPOPEN=1,0,115.236.153.174,20166\r\n");
            printf("AT+ZIPOPEN=1,0,www.jsald.cn,8068\r\n");//朱工
//						printf("AT+ZIPOPEN=1,0,124.70.218.101,8068\r\n");//朱工
						nb_time_flag=0;
						connect_on_cnt=0;
						break;			
				case 7: printf("AT+ZIPCALL=0\r\n");
						nb_time=0;
						break;		
				case 8: printf("AT+ZIPCALL=0\r\n");
						nb_time=0;
						break;
//				case 9:
//						printf("AT+CFUN=0\r\n");
//						open_fly_mode = 1;
//					    nb_time = 0;
//						break;
//				case 10:
//						printf("AT+CFUN=1\r\n");
//						nb_time = 0;
//						break;
				default:break;			
			}
		}
	}
	if(NB_TCP_CONNECT_FLAG&&connect_on_cnt<3){
		
		if(nb_time_flag){
			connect_on_cnt++;
			
			switch(connect_on_cnt){
				case 1: printf("AT+ZIPALIVE=1,1,300,75,9\r\n");
						break;
				case 2:	chezaikongtiao_calculate();
						NB_TCP_TX();
						break;
				case 3: chezaikongtiao_calculate();
						NB_TCP_TX();
						break;
				default: break;
			}
			nb_time_flag=0;
		}
	}

//	if(debug_nb_flag&&usart1_rx_flag){
//		usart1_rx_flag=0;
//		memset(chezaikt_send_data,0,sizeof(chezaikt_send_data));
//		strcat(chezaikt_send_data,(const char *)DMA_Tx_Buf);
//		NB_TCP_TX();
//	} //GPS数据
}
//入口参数：	cmd:要发送的命令  str：要校验的返回值	返回参数：	0：成功    1：失败
_Bool NB_DEVICE_SendCmd(char *cmd,char *str)
{
	char JY_CMD[30];
	unsigned char i=0;
	unsigned char flag=0;
	while(*str!=NULL)
	{
		JY_CMD[i++]=*str++;
	}
	NB_IO_Clear_Cache();
	NB_IO_Send((unsigned char*)cmd,strlen(cmd));
 //while(nbioinfo.rev_status==0)
	delay_ms(1000);	
	if(nbioinfo.rev_status==1)
	{
		if(str==NULL)
			flag=1;
		else
			if(strstr(nbioinfo.rev_buf,JY_CMD))
				flag=1;
			else
				flag=0;
	}
	else
		flag=0;
		
	if(flag==1)
		return 0;
	else
		return 1;
}	
void NB_SEARCH_IMEI(void)//3630
{
	unsigned char j,i;
	char nbsearchimei[12]={'A','T','+','C','G','S','N','\r','\n'};
	NB_IO_Clear_Cache();
	memset(nb_imei_number,0,sizeof(nb_imei_number));
	delay_ms(500);
	if(NB_DEVICE_SendCmd(nbsearchimei,"OK")==0)
	{
		for(j=0,i=0;j<strlen(nbioinfo.rev_buf);j++)
		{
			if((nbioinfo.rev_buf[j]>='0')&&(nbioinfo.rev_buf[j]<='9'))
			{
				nb_imei_number[i]=nbioinfo.rev_buf[j];
				i++;
			}
		}
		if((nb_imei_number[0]==0||nb_imei_number[1]=='0'||nb_imei_number[2]=='0'))
			NB_IMEI_SEARCH_FLAG=0;
		else
			NB_IMEI_SEARCH_FLAG=1;
	}
	else
	{
		delay_ms(500);
		delay_ms(500);
		delay_ms(500);
		if(strstr(nbioinfo.rev_buf,"OK"))
		{
		  for(j=0,i=0;j<strlen(nbioinfo.rev_buf);j++)
			{
				if((nbioinfo.rev_buf[j]>='0')&&(nbioinfo.rev_buf[j]<='9'))
				{
					nb_imei_number[i]=nbioinfo.rev_buf[j];
					i++;
				}
			}
		if((nb_imei_number[0]==0||nb_imei_number[1]=='0'||nb_imei_number[2]=='0'))
			NB_IMEI_SEARCH_FLAG=0;
		else
			NB_IMEI_SEARCH_FLAG=1;
		
	  }
	}	
}
extern char gsa_data[100];
extern u8 usart1_rx_flag;

void nb_chushihua_hanshu_zhongduan(void)
{

	if(NB_IMEI_SEARCH_FLAG==0)
	{
		NB_IO_Clear_Cache();
		NB_SEARCH_IMEI();
		NB_IO_Clear_Cache();
	}
	NB_TCP_CONNECT_1();
//	if(usart1_rx_flag){
//		usart1_rx_flag=0;
//		str2str_TX(chezaikt_send_data,gsa_data);
//		NB_TCP_TX();
//		delay_ms(5);
//		if(gsa_data[53]>='0'&&gsa_data[57]<='9'){
//			
//			strncpy(height,gsa_data+53,5);
//			memset(chezaikt_send_data,0,sizeof(chezaikt_send_data));
//			char str_to_send[100]={0};
//			str2str_TX(chezaikt_send_data,height);
//			NB_TCP_TX();
//		}
//	}
}
extern u8 ble_connect_flag;
void NB_TCP_TX(void)//3630
{
	if(NB_TCP_CONNECT_FLAG)
		printf("AT+ZIPSEND=1,%s\r\n",chezaikt_send_data);
}
#else
#ifdef ME3616

unsigned char NB_INIT_FLAG=0;
unsigned char NB_SOCKET_CLOSE_FLAG=0;
unsigned char NB_SOCKET_CREAT_FLAG=0;
unsigned char NB_IMEI_SEARCH_FLAG=0;
unsigned char NB_TCP_CONNECT_FLAG=0;
unsigned char NB_TCP_TX_FLAG=0;
uint32_t sszcsj_time_count=0;
uint32_t jszcsj_time_count=0;
uint8_t sszcsj_time_flag=0;
uint8_t ssbzw_flag=0;
uint8_t jsbzw_flag=0;
uint8_t sbjsbz_flag=0;
uint8_t skssys_flag=0;//延时标志
uint32_t skssys_time=0;//延时时间
uint8_t skssyswc_flag=0;//延时完成标志
uint32_t shangsuo_time=0;
uint32_t changanshijian=0;

char nb_tx_chezaikt_shuju[1000];
char NB_TCP_CLOCK[1]={'a'};
char nb_imei_number[20]="866826031156087";
NB_IO_INFO nbioinfo;

void NBUSART_Config(void)
{
	GPIO_InitTypeDef  NBGPIO_InitStructure;
	USART_InitTypeDef NBUSART_InitStructure;
	NVIC_InitTypeDef  NBNVIC_InitStructure;
	
	NBDEBUG_USART_GPIO_APBxClkCmd(NBDEBUG_USART_GPIO_CLK, ENABLE);
	NBDEBUG_USART_APBxClkCmd(NBDEBUG_USART_CLK, ENABLE);
	
	NBGPIO_InitStructure.GPIO_Pin = NBDEBUG_USART_TX_GPIO_PIN;
	NBGPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	NBGPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(NBDEBUG_USART_TX_GPIO_PORT, &NBGPIO_InitStructure);
	
	NBGPIO_InitStructure.GPIO_Pin = NBDEBUG_USART_RX_GPIO_PIN;
	NBGPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(NBDEBUG_USART_RX_GPIO_PORT, &NBGPIO_InitStructure);
	
	USART_DeInit(USART2);
	NBUSART_InitStructure.USART_BaudRate = NBDEBUG_USART_BAUDRATE;
	NBUSART_InitStructure.USART_WordLength = USART_WordLength_8b;
	NBUSART_InitStructure.USART_StopBits = USART_StopBits_1;
	NBUSART_InitStructure.USART_Parity = USART_Parity_No ;
	NBUSART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	NBUSART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(NBDEBUG_USARTx, &NBUSART_InitStructure);
	
	USART_ClearITPendingBit(NBDEBUG_USARTx,USART_IT_RXNE);
	USART_ITConfig(NBDEBUG_USARTx, USART_IT_RXNE, ENABLE);
	
  //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NBNVIC_InitStructure.NVIC_IRQChannel = NBDEBUG_USART_IRQ;
  NBNVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NBNVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NBNVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NBNVIC_InitStructure);	
	USART_Cmd(NBDEBUG_USARTx, ENABLE);	
}

void NBUsart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	USART_SendData(pUSARTx,ch);
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/****************** 发送8位的数组 ************************/
void NBUsart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
  uint8_t i;
	for(i=0; i<num; i++)
  {
	    NBUsart_SendByte(pUSARTx,array[i]);	
  }
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

/*****************  发送字符串 **********************/
void NBUsart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      NBUsart_SendByte(pUSARTx,*(str+k));
      k++;
  } while(*(str + k)!='\0');
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

/*****************  发送一个16位数 **********************/
void NBUsart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	temp_h = (ch&0XFF00)>>8;
	temp_l = ch&0XFF;
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		USART_SendData(NBDEBUG_USARTx, (uint8_t) ch);
		while (USART_GetFlagStatus(NBDEBUG_USARTx, USART_FLAG_TXE) == RESET);		
		return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		while (USART_GetFlagStatus(NBDEBUG_USARTx, USART_FLAG_RXNE) == RESET);
		return (int)USART_ReceiveData(NBDEBUG_USARTx);
}

//NB模块程序发送配置说明
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void NB_IO_Send(unsigned char *str, unsigned short len)
{	
	for(; len>0; len--)
	{
		while(USART_GetFlagStatus(NBDEBUG_USARTx,USART_FLAG_TC) == RESET);		//等待发送完成
		USART_SendData(NBDEBUG_USARTx,*str++);									//发送数据
	}
}

void NB_IO_Clear_Cache(void)
{
	nbioinfo.write_pos=0;
	nbioinfo.rev_status=0;
	memset(nbioinfo.rev_buf,0,sizeof(nbioinfo.rev_buf));
}

//入口参数：	cmd:要发送的命令  str：要校验的返回值	返回参数：	0：成功    1：失败
_Bool NB_DEVICE_SendCmd(char *cmd,char *str)
{
	char JY_CMD[30];
	unsigned char i=0;
	unsigned char flag=0;
	while(*str!=NULL)
	{
		JY_CMD[i++]=*str++;
	}
	NB_IO_Clear_Cache();
	NB_IO_Send((unsigned char*)cmd,strlen(cmd));
 //while(nbioinfo.rev_status==0)
	delay_ms(1000);	
	if(nbioinfo.rev_status==1)
	{
		if(str==NULL)
			flag=1;
		else
			if(strstr(nbioinfo.rev_buf,JY_CMD))
				flag=1;
			else
				flag=0;
	}
	else
		flag=0;
		
	if(flag==1)
		return 0;
	else
		return 1;
}	

void NB_SEARCH_IMEI(void)//3616
{
	unsigned char j,i;
	char nbsearchimei[12]={'A','T','+','C','G','S','N','=','1','\r','\n'};
	NB_IO_Clear_Cache();
	memset(nb_imei_number,0,sizeof(nb_imei_number));
	delay_ms(500);
	if(NB_DEVICE_SendCmd(nbsearchimei,"OK")==0)
	{
		for(j=12,i=0;j<strlen(nbioinfo.rev_buf);j++)
		{
			if((nbioinfo.rev_buf[j]>='0')&&(nbioinfo.rev_buf[j]<='9'))
			{
				nb_imei_number[i]=nbioinfo.rev_buf[j];
				i++;
			}
		}
		if(i>=14)
			NB_IMEI_SEARCH_FLAG=0;
	}
	else
	{
		delay_ms(500);
		delay_ms(500);
		delay_ms(500);
		if(strstr(nbioinfo.rev_buf,"OK"))
		{
		  for(j=12,i=0;j<strlen(nbioinfo.rev_buf);j++)
			{
				if((nbioinfo.rev_buf[j]>='0')&&(nbioinfo.rev_buf[j]<='9'))
				{
					nb_imei_number[i]=nbioinfo.rev_buf[j];
					i++;
				}
			}
		if(i>=14)
			NB_IMEI_SEARCH_FLAG=0;
	  }
	}	
}

void NB_TCP_REST(void)//3616没有该指令
{
	char nbrset[10]={'A','T','+','Z','R','S','T','\r','\n'};
	NB_IO_Clear_Cache();
	delay_ms(500);
	NB_DEVICE_SendCmd(nbrset,"OK");
}

void NB_SET_BOODLATE(void)//3616
{
	char nbboundlate[13]={'A','T','+','I','P','R','=','9','6','0','0','\r','\n'};
	NB_IO_Clear_Cache();
	delay_ms(500);
	NB_DEVICE_SendCmd(nbboundlate,"OK");
}

void NB_TCP_SOCKET_CLOSE(void)//3616
{
	char nbstr1[13]={'A','T','+','E','S','O','C','L','=',' ','\r','\n'};
	char nbstr2[7]={"0123456"};
	char i;
	for(i=0;i<2;i++)
	{
		nbstr1[9]=nbstr2[i];
		NB_IO_Clear_Cache();
		delay_ms(500);
		NB_DEVICE_SendCmd(nbstr1,"OK");
	}
	NB_SOCKET_CLOSE_FLAG=1;
}

void NB_TCP_SOCKET_CREAT(void)//3616
{
	char nbstr1[30]={'A','T','+','E','S','O','C','=','1',',','1',',','1','\r','\n'};
	char i,j;
	for(i=0;i<2;i++)
	{
		NB_IO_Clear_Cache();
		delay_ms(500);
		if(NB_DEVICE_SendCmd(nbstr1,"OK")==0)
		{
			for(j=14;j<strlen(nbioinfo.rev_buf);j++)
			{
				if((nbioinfo.rev_buf[j]>='0')&&(nbioinfo.rev_buf[j]<='9'))
				{
					NB_TCP_CLOCK[0]=nbioinfo.rev_buf[j];
					NB_SOCKET_CREAT_FLAG=1;
					break;
				}
			}
			if((NB_TCP_CLOCK[0]>='0')&&(NB_TCP_CLOCK[0]<='9'))
				break;
		}
	}
	if((i>=2)&&(NB_TCP_CLOCK[0]=='a'))
		{
			NB_SOCKET_CLOSE_FLAG=0;
			NB_SOCKET_CREAT_FLAG=0;
		}
}
		
void NB_TCP_CONNECT(void)//3616
{
	char nbstr1[50]={'A','T','+','E','S','O','C','O','N','=',' ',',','2','8','5','3','0',',','\"','1','0','3','.','4','6','.','1','2','8','.','4','5','\"','\r','\n'};
//  char nbstr1[50]={'A','T','+','E','S','O','C','O','N','=',' ',',','5','6','9','4','9',',','\"','1','0','3','.','4','6','.','1','2','8','.','4','9','\"','\r','\n'};
  char nbstr2[50]={'A','T','+','E','S','O','C','O','N','=',' ',',','1','0','0','8','7',',','\"','4','7','.','1','1','1','.','1','1','2','.','3','0','\"','\r','\n'};
	/*
		0 连接服务器
		1 连接花生壳测试
		*/
	if(0)
		{
			nbstr1[10]=NB_TCP_CLOCK[0];
			NB_IO_Clear_Cache();
			delay_ms(200);
			if(NB_DEVICE_SendCmd(nbstr1,"OK")==0)
				NB_TCP_CONNECT_FLAG=1;
			else
				{
					delay_ms(500);
					delay_ms(500);
					delay_ms(500);
					if(strstr(nbioinfo.rev_buf,"OK"))
						NB_TCP_CONNECT_FLAG=1;
					else
					{
						NB_SOCKET_CLOSE_FLAG=0;
					  NB_SOCKET_CREAT_FLAG=0;
					  NB_TCP_CONNECT_FLAG=0;
					}
				}
		}
		else
			{
				nbstr2[10]=NB_TCP_CLOCK[0];
				NB_IO_Clear_Cache();
				delay_ms(200);
				if(NB_DEVICE_SendCmd(nbstr2,"OK")==0)
					NB_TCP_CONNECT_FLAG=1;
				else
					{
						delay_ms(500);
						delay_ms(500);
						delay_ms(500);
						if(strstr(nbioinfo.rev_buf,"OK"))
							NB_TCP_CONNECT_FLAG=1;
						else
						{
							NB_SOCKET_CLOSE_FLAG=0;
						  NB_SOCKET_CREAT_FLAG=0;
						  NB_TCP_CONNECT_FLAG=0;
						}
					}
			}
}
		
void NB_TCP_TX(void)//3616
{
	int send_count;
	char nbstr1[12]="AT+ESOSEND=";
	char nbstr2[10];
	char nbstr3[1];
  if(NB_TCP_CONNECT_FLAG==1)
	{
		nbstr3[0]=NB_TCP_CLOCK[0];
		memset(nb_tx_chezaikt_shuju,0,sizeof(nb_tx_chezaikt_shuju));
		send_count=strlen(chezaikt_send_data)/2;
		sprintf(nbstr2,"%d",send_count);
		strcat(nb_tx_chezaikt_shuju,nbstr1);
		strcat(nb_tx_chezaikt_shuju,nbstr3);
		strcat(nb_tx_chezaikt_shuju,",");
		strcat(nb_tx_chezaikt_shuju,nbstr2);
		strcat(nb_tx_chezaikt_shuju,",");
		strcat(nb_tx_chezaikt_shuju,chezaikt_send_data);
		strcat(nb_tx_chezaikt_shuju,"\r\n");
		NB_IO_Clear_Cache();
		delay_ms(500);
		if(NB_DEVICE_SendCmd(nb_tx_chezaikt_shuju,"OK")==0)
			NB_TCP_TX_FLAG=1;
		else
			{
				delay_ms(500);
				if(strstr(nbioinfo.rev_buf,"OK"))
				  NB_TCP_TX_FLAG=1;
				else
					{
						delay_ms(500);
						if(strstr(nbioinfo.rev_buf,"OK"))
							NB_TCP_TX_FLAG=1;
						else
						{
							delay_ms(500);
						  if(strstr(nbioinfo.rev_buf,"OK"))
							 NB_TCP_TX_FLAG=1;
						  else
							{
								NB_SOCKET_CLOSE_FLAG=0;
								NB_SOCKET_CREAT_FLAG=0;
								NB_TCP_CONNECT_FLAG=0;
								NB_TCP_TX_FLAG=0;
							}
						}
					}
			}
	}
}

void nb_chushihua_hanshu_zhongduan(void)
{
	if(NB_SOCKET_CLOSE_FLAG==0)
	{
		//TIM_ITConfig(GENERAL_TIM,TIM_IT_Update,DISABLE);
		NB_IO_Clear_Cache();
		delay_ms(100);
		NB_SET_BOODLATE();
		delay_ms(100);
		NB_TCP_SOCKET_CLOSE();
		delay_ms(100);
		NB_IO_Clear_Cache();
		delay_ms(100);
		NB_SEARCH_IMEI();
		delay_ms(100);
	}
	if((NB_SOCKET_CREAT_FLAG==0)&&(NB_SOCKET_CLOSE_FLAG==1))
	{
		NB_IO_Clear_Cache();
		delay_ms(100);
		NB_TCP_SOCKET_CREAT();
		delay_ms(100);
	}
	if((NB_TCP_CONNECT_FLAG==0)&&(NB_SOCKET_CREAT_FLAG==1))
	{
		NB_IO_Clear_Cache();
		delay_ms(100);
		NB_TCP_CONNECT();
		delay_ms(500);
		chezaikongtiao_calculate();
		NB_TCP_TX();
		delay_ms(500);
		chezaikongtiao_calculate();
		NB_TCP_TX();
		//TIM_ITConfig(GENERAL_TIM,TIM_IT_Update,ENABLE);
	}
}

#endif

#endif

