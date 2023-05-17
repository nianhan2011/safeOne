#include "ble.h"
#include <stdarg.h>
#include "delay.h"
#include "bsp_key.h"
#include "bsp_GeneralTim.h" 
#include "common.h"
#include "bsp_usart5.h"
#include "delay.h"
#include "bsp_key.h"
#include "bsp_GeneralTim.h" 
#include "common.h"
#include "string.h"
#include "nb_model.h"
#include "bsp_nb.H"
#include "flash.h"
void UART3_Config(void)
{
	GPIO_InitTypeDef  NBGPIO_InitStructure;
	USART_InitTypeDef NBUSART_InitStructure;
	NVIC_InitTypeDef  NBNVIC_InitStructure;
	
	BLEUSART_GPIO_APBxClock_FUN(BLEUSART_GPIO_CLK, ENABLE);
	BLEUSART_APBxClock_FUN(BLEUSART_CLK, ENABLE);
	
	NBGPIO_InitStructure.GPIO_Pin = BLEUSART_TX_PIN;
	NBGPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	NBGPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BLEUSART_TX_PORT, &NBGPIO_InitStructure);
	
	NBGPIO_InitStructure.GPIO_Pin = BLEUSART_RX_PIN;
	NBGPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(BLEUSART_RX_PORT, &NBGPIO_InitStructure);
	
	USART_DeInit(BLEUSARTx);
	NBUSART_InitStructure.USART_BaudRate = BLEUSART_BAUD_RATE;
	NBUSART_InitStructure.USART_WordLength = USART_WordLength_8b;
	NBUSART_InitStructure.USART_StopBits = USART_StopBits_1;
	NBUSART_InitStructure.USART_Parity = USART_Parity_No ;
	NBUSART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	NBUSART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(BLEUSARTx, &NBUSART_InitStructure);
	
	USART_ClearITPendingBit(BLEUSARTx,USART_IT_RXNE);
	USART_ITConfig(BLEUSARTx, USART_IT_RXNE, ENABLE);
	
  //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NBNVIC_InitStructure.NVIC_IRQChannel = BLEUSART_IRQ;
  NBNVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NBNVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NBNVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NBNVIC_InitStructure);	
	USART_Cmd(BLEUSARTx, ENABLE);	
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
void Usart3_SendStr_length( USART_TypeDef * pUSARTx, uint8_t *str,uint32_t strlen )
{
	unsigned int k=0;
    do 
    {
        Usart_SendByte( pUSARTx, *(str + k) );
        k++;
    } while(k < strlen);
}

/*****************  发送字符串 **********************/
void Usart3_SendString( USART_TypeDef * pUSARTx, uint8_t *str)
{
	unsigned int k=0;
    do 
    {
        Usart_SendByte( pUSARTx, *(str + k) );
        k++;
    } while(*(str + k)!='\0');
}
u8 ble_connect_flag;
u8 ble_connect_flag2[10];
u8 ble_tx_flag;
u8 ble_write_flag;
u8 ble_ready_flag;
extern unsigned char NB_IMEI_SEARCH_FLAG;
void ble_init(){
	UART3_Config();


//	Usart3_SendString(BLEUSARTx,( uint8_t *)"AT+BLEINIT=0\r\n");
//	delay_ms(1000);
//	Usart3_SendString(BLEUSARTx,( uint8_t *)"AT+BLEINIT=1\r\n");
//	delay_ms(1000);
//	//Usart3_SendString(BLEUSARTx,( uint8_t *)"AT+BLESCAN=1,3\r\n");
//	Usart3_SendString(BLEUSARTx,( uint8_t *)"AT+BLECONN=0,\"BA:03:54:18:64:5F\"\r\n");
//	delay_ms(1000);
//	delay_ms(1000);
//	Usart3_SendString(BLEUSARTx,( uint8_t *)"AT+BLEGATTCPRIMSRV=0\r\n");
//	delay_ms(1000);
//	delay_ms(1000);
////		Usart3_SendString(BLEUSARTx,( uint8_t *)"AT+BLEGATTCCHAR=0,6\r\n");
//	delay_ms(500);
//	Usart3_SendString(BLEUSARTx,( uint8_t *)"AT+BLEGATTCWR=0,6,3,1,1\r\n");
//	delay_ms(1000);
//	delay_ms(1000);
//	delay_ms(800);
//	Usart3_SendString(BLEUSARTx,( uint8_t *)"1");
//	delay_ms(800);
	

}
char get_connect_flag(u8 *flag, u8 num)
{
	int i = 0;
	for(i = 0; i < num; i++)
	{
		if(flag[i] == 0)
		{
			return i;
		}
	}
	return i;
}

struct BLEMAC ble_mac;
u8 char_id;
u8 char_id2;
u8 init_flag2[10]={0};
u8 notify_cmd[15]="+NOTIFY:0,6,3,";
u8 notify_cmd1[15]="+NOTIFY:0,6,2,";
u8 ble_time=1;
u8 ble_time_flag=1;
u16 xinlv_cnt=0,xinlv_flag=0;
u8 init_flag=0,nb_xinlv_tx_flag=0;
BLE_IO_INFO bleioinfo;
u8 ble_ap_flag=0;
u8 ble_mac_rx_flag=0;
u8 mac_client_id;
u8 debug_ble_flag=0;
extern u8 busy_flag;
void ble_connect(){
	static u8 busy_flag_old = 0;
	if(busy_flag_old == 1 && busy_flag == 0)
	{
		ble_ap_flag = 0;
	}
	busy_flag_old = busy_flag;
	if(busy_flag)
		return;
	extern u16 ble_time_cnt;
	if(NB_IMEI_SEARCH_FLAG&&ble_ap_flag==0){
		delay_ms(100);
		Usart3_SendString(BLEUSARTx,( uint8_t *)"AT+CWMODE=3\r\n");
		delay_ms(100);
		char command[50]={0},ap_name[20]={0};
		strcpy(ap_name,nb_imei_number);
		strcat(command,"AT+CWSAP=\"");
		strcat(command,ap_name);
		strcat(command,"\",\"1234567890\",5,4,10,0\r\n");
		Usart3_SendString(BLEUSARTx,( uint8_t *)command);
		delay_ms(100);

		Usart3_SendString(BLEUSARTx,( uint8_t *)"AT+CIPMUX=1\r\n");
		delay_ms(100);

		Usart3_SendString(BLEUSARTx,( uint8_t *)"AT+CWDHCP=1,2\r\n");
		delay_ms(100);
		ble_ap_flag=1;
		Usart3_SendString(BLEUSARTx,( uint8_t *)"AT+CWDHCPS=1,3,\"192.168.4.2\",\"192.168.4.255\"\r\n");
		delay_ms(100);
		Usart3_SendString(BLEUSARTx,( uint8_t *)"AT+CIPSERVER=1,8899\r\n");
		delay_ms(100);
		ble_time_cnt = 0;
		ble_time_flag = 0;
		
	}
	if(ble_ready_flag==1){
		delay_ms(100);
		char command[50]={0},*message="ready";
		strcat(command,"AT+CIPSEND=");
		strncat(command,(const char *)&mac_client_id,1);
		strcat(command,",");
		char str_temp[10]={0};
		sprintf(str_temp,"%d",strlen(message));
		strcat(command,str_temp);
		strcat(command,"\r\n");
		ble_ready_flag=0;
		Usart3_SendString(BLEUSARTx,( uint8_t *)command);
		delay_ms(300);
		Usart3_SendString(BLEUSARTx,( uint8_t *)message);
		delay_ms(300);
		ble_time_cnt = 0;
		ble_time_flag = 0;
	}
	if(ble_mac_rx_flag == 1){ //mac_client_id
		delay_ms(100);
		char command[50]={0},*message="绑定成功";
		strcat(command,"AT+CIPSEND=");
		strncat(command,(const char *)&mac_client_id,1);
		strcat(command,",");
		char str_temp[10]={0};
		sprintf(str_temp,"%d",strlen(message));
		strcat(command,str_temp);
		strcat(command,"\r\n");
		ble_mac_rx_flag=0;
		Usart3_SendString(BLEUSARTx,( uint8_t *)command);
		delay_ms(300);
		Usart3_SendString(BLEUSARTx,( uint8_t *)message);
		delay_ms(300);
		ble_time_cnt = 0;
		ble_time_flag = 0;
	}
	else if(ble_mac_rx_flag > 1 && ble_mac_rx_flag <= 4)
	{
		delay_ms(100);//AT+CWQIF//AT+CIPCLOSE=
//		char command[50]={0},*message="OK";
//		strcat(command,"AT+CIPSEND=");
//		strncat(command,(const char *)&mac_client_id,1);
//		strcat(command,",");
//		char str_temp[10]={0};
//		sprintf(str_temp,"%d",strlen(message));
//		strcat(command,str_temp);
//		strcat(command,"\r\n");
//		ble_mac_rx_flag=0;
//		Usart3_SendString(BLEUSARTx,( uint8_t *)command);
//		delay_ms(300);
//		Usart3_SendString(BLEUSARTx,( uint8_t *)message);
//		delay_ms(300);	
		char *message0="AT+CIPCLOSE=5\r\n", *message="AT+CWQIF\r\n";
		Usart3_SendString(BLEUSARTx,( uint8_t *)message0);
		delay_ms(100);			
		Usart3_SendString(BLEUSARTx,( uint8_t *)message);
		ble_mac_rx_flag=0; 
		delay_ms(100);
		Usart3_SendString(BLEUSARTx,( uint8_t *)"AT+CIPSERVER=0,1\r\n");
		delay_ms(100);
		Usart3_SendString(BLEUSARTx,( uint8_t *)"AT+CIPSERVER=1,8899\r\n");
		delay_ms(100);		
		ble_time_cnt = 0;
		ble_time_flag = 0;
	}
	static u16 cnt = 0;
	if(ble_time_flag&&ble_time == 11)
		cnt++;
	int num = get_connect_flag(ble_connect_flag2, 3);
	int init2 = get_connect_flag(init_flag2, 3);
	init2 = 3;//取消蓝牙连接卡扣
	if(init_flag==1)
		cnt = 0;
	else if(init2 >= 3)
	{
		cnt = 11;
	}
	if(ble_mac.kakou_mac[init2][19] == 0 || ble_mac.kakou_mac[init2][0] == 0xff)
		cnt = 11;
	if(!debug_ble_flag&&(num < 3||init2<3)&&NB_TCP_CONNECT_FLAG&&(cnt%20 <= 10)){
		uint8_t cmd_temp[40];
		
		ble_mac.shouhuan_mac[0]='\"';
		ble_mac.shouhuan_mac[18]='\"';
		ble_mac.shouhuan_mac[19]='\r';
		ble_mac.shouhuan_mac[20]='\n';
		ble_mac.shouhuan_mac[21]='\0';

		ble_mac.kakou_mac[init2][0]='\"';
		ble_mac.kakou_mac[init2][18]='\"';
		ble_mac.kakou_mac[init2][19]='\r';
		ble_mac.kakou_mac[init2][20]='\n';
		ble_mac.kakou_mac[init2][21]='\0';
		if(ble_time_flag){
			switch(ble_time){
				case 1:	//Usart3_SendString(BLEUSARTx,( uint8_t *)"AT+BLEINIT=0\r\n");
						break;
				case 2: Usart3_SendString(BLEUSARTx,( uint8_t *)"AT+BLEINIT=1\r\n");
						break;
				case 3:
						if(!ble_connect_flag2[init2]&&init2 < 3)
						{
							char ble_id = init2 + 1 + '0';
							memset(cmd_temp,0,40);
							strcat((char *)cmd_temp,"AT+BLESCAN=1,3,1,");
							strcat((char *)cmd_temp,(const char *)ble_mac.kakou_mac[init2]);
							Usart3_SendString(BLEUSARTx,cmd_temp);
							Usart3_SendString(BLEUSARTx,cmd_temp);
							Usart3_SendString(BLEUSARTx,cmd_temp);
							Usart3_SendString(BLEUSARTx,cmd_temp);
						}
						break;
				case 4: 
						if(!ble_connect_flag2[init2]&&init2 < 3)
						{
							char ble_id = init2 + 1 + '0';
							memset(cmd_temp,0,40);
							strcat((char *)cmd_temp,"AT+BLECONN=");//AT+BLEDISCONN=0
							strncat((char *)cmd_temp,(const char *)&ble_id, 1);//AT+BLEDISCONN=0
							strcat((char *)cmd_temp,",");//AT+BLEDISCONN=0
							strcat((char *)cmd_temp,(const char *)ble_mac.kakou_mac[init2]);
							Usart3_SendString(BLEUSARTx,cmd_temp);
							Usart3_SendString(BLEUSARTx,cmd_temp);
							Usart3_SendString(BLEUSARTx,cmd_temp);
							Usart3_SendString(BLEUSARTx,cmd_temp);
						}
						break;
				case 5: 
					{
						if(ble_connect_flag2[init2]){
							char ble_id = init2 + 1 + '0';
							memset(cmd_temp,0,40);
							strcat((char *)cmd_temp,(const char *)"AT+BLEGATTCPRIMSRV=");//AT+BLEDISCONN=0
							strncat((char *)cmd_temp,(const char *)&ble_id, 1);//AT+BLEDISCONN=0
							strcat((char *)cmd_temp,(const char *)"\r\n");//AT+BLEDISCONN=0
							Usart3_SendString(BLEUSARTx, cmd_temp);
							Usart3_SendString(BLEUSARTx, cmd_temp);
							Usart3_SendString(BLEUSARTx, cmd_temp);
							Usart3_SendString(BLEUSARTx, cmd_temp);
						}else
							ble_time = 9;
					}
						break;
				case 6: if((ble_connect_flag2[init2])&&char_id2>'0'){
							char ble_id = init2 + 1 + '0';
							memset(cmd_temp,0,40);//AT+BLEGATTCCHAR=0,3
							strcat((char *)cmd_temp,"AT+BLEGATTCWR=");
							strncat((char *)cmd_temp,(const char *)&ble_id, 1);//AT+BLEDISCONN=0
							strcat((char *)cmd_temp,",");
							strncat((char *)cmd_temp,(const char *)&char_id2,1);
							strcat((char *)cmd_temp,",1,1,2\r\n");

							Usart3_SendString(BLEUSARTx,cmd_temp);	
						}else
							ble_time = 9;
						break;
				case 7: if((ble_connect_flag2[init2])&&char_id2>'0'){
							//Usart3_SendString(BLEUSARTx,( uint8_t *)"1");	
					Usart_SendByte(BLEUSARTx, 0x01);
					Usart_SendByte(BLEUSARTx, 0x00);
						}else
							ble_time = 9;
						break;	
				case 9: 
//						if(init_flag2[init2]==0)
//						{
//							char ble_id = init2 + 1 + '0';
//							memset(cmd_temp,0,40);
//							strcat((char *)cmd_temp,"AT+BLEDISCONN=");//
//							strncat((char *)cmd_temp,(const char *)&ble_id, 1);//AT+BLEDISCONN=0
//							strcat((char *)cmd_temp,"\r\n");//
//							Usart3_SendString(BLEUSARTx,cmd_temp);

//						}
						break;
				default:break;
			
			}
			ble_time_flag = 0;
		}
	}

	if(!debug_ble_flag&&(!ble_connect_flag||init_flag==0)&&NB_TCP_CONNECT_FLAG&&(cnt%20 > 10)
		&&ble_mac.shouhuan_mac[1]!=0&&ble_mac.shouhuan_mac[1]!=0xff){
		uint8_t cmd_temp[40];
		ble_mac.shouhuan_mac[0]='\"';
		ble_mac.shouhuan_mac[18]='\"';
		ble_mac.shouhuan_mac[19]='\r';
		ble_mac.shouhuan_mac[20]='\n';
		ble_mac.shouhuan_mac[21]='\0';
		static u16 shouhuan_con_flag = 0;
		if(ble_time_flag){
			if(shouhuan_con_flag >=1 && shouhuan_con_flag <=10)
			{
				shouhuan_con_flag++;
				ble_time = 11;
			}else if(shouhuan_con_flag>10)
			{
				shouhuan_con_flag = 0;
			}
#if(0)
			switch(ble_time){
				case 1:	//Usart3_SendString(BLEUSARTx,( uint8_t *)"AT+BLEINIT=0\r\n");
						break;
				case 2: Usart3_SendString(BLEUSARTx,( uint8_t *)"AT+BLEINIT=1\r\n");
						break;
				case 3:	
					
						if(!ble_connect_flag)
						{
							memset(cmd_temp,0,40);
							strcat((char *)cmd_temp,"AT+BLESCAN=1,3,1,");
							strcat((char *)cmd_temp,(const char *)ble_mac.shouhuan_mac);
							Usart3_SendString(BLEUSARTx,cmd_temp);
						}
						break;
				case 4: 
						if(!ble_connect_flag)
						{
							memset(cmd_temp,0,40);
							strcat((char *)cmd_temp,"AT+BLECONN=0,");
							strcat((char *)cmd_temp,(const char *)ble_mac.shouhuan_mac);
							Usart3_SendString(BLEUSARTx,cmd_temp);
						}
						break;
				case 5: Usart3_SendString(BLEUSARTx,( uint8_t *)"AT+BLEGATTCPRIMSRV=0\r\n");
						break;
				case 6: if(char_id>'0'){
							memset(cmd_temp,0,40);
							strcat((char *)cmd_temp,"AT+BLEGATTCWR=0,");
							strncat((char *)cmd_temp,(const char *)&char_id,1);
							strcat((char *)cmd_temp,",3,1,1\r\n");
							Usart3_SendString(BLEUSARTx,cmd_temp);	
						}
						break;
				case 7: if(char_id>'0'){
							Usart3_SendString(BLEUSARTx,( uint8_t *)"1");	
							init_flag = 1;
						}
						
						break;	
				case 9:
//					bleioinfo.read_pos=0;
//						bleioinfo.write_pos=0;
						break;
				default:
					shouhuan_con_flag++;
					break;
			
			}
#else
			switch(ble_time){
				case 1:	init_flag=0;//Usart3_SendString(BLEUSARTx,( uint8_t *)"AT+BLEINIT=0\r\n");
						break;
				case 2: Usart3_SendString(BLEUSARTx,( uint8_t *)"AT+BLEINIT=1\r\n");
						break;
				case 3:	
						
						if(!ble_connect_flag)
						{memset(cmd_temp,0,40);
						strcat((char *)cmd_temp,"AT+BLESCAN=1,3,1,");
						strcat((char *)cmd_temp,(const char *)ble_mac.shouhuan_mac);
						}
						Usart3_SendString(BLEUSARTx,cmd_temp);
//						Usart3_SendString(BLEUSARTx,cmd_temp);
//						Usart3_SendString(BLEUSARTx,cmd_temp);
//						Usart3_SendString(BLEUSARTx,cmd_temp);
						break;
				case 4: 
						if(!ble_connect_flag)
						{
						memset(cmd_temp,0,40);
						strcat((char *)cmd_temp,"AT+BLECONN=0,");
						strcat((char *)cmd_temp,(const char *)ble_mac.shouhuan_mac);
						Usart3_SendString(BLEUSARTx,cmd_temp);
//						Usart3_SendString(BLEUSARTx,cmd_temp);
//						Usart3_SendString(BLEUSARTx,cmd_temp);
//						Usart3_SendString(BLEUSARTx,cmd_temp);
						}
						break;
				case 5: 
						if(ble_connect_flag)
						{
							Usart3_SendString(BLEUSARTx,( uint8_t *)"AT+BLEGATTCPRIMSRV=0\r\n");
//							Usart3_SendString(BLEUSARTx,( uint8_t *)"AT+BLEGATTCPRIMSRV=0\r\n");
//							Usart3_SendString(BLEUSARTx,( uint8_t *)"AT+BLEGATTCPRIMSRV=0\r\n");
//							Usart3_SendString(BLEUSARTx,( uint8_t *)"AT+BLEGATTCPRIMSRV=0\r\n");
						}
						break;
				case 6: 

						if(ble_connect_flag&&char_id>'0'){
							memset(cmd_temp,0,40);
							strcat((char *)cmd_temp,"AT+BLEGATTCCHAR=0,");
							strncat((char *)cmd_temp,(const char *)&char_id,1);
							strcat((char *)cmd_temp,"\r\n");
							Usart3_SendString(BLEUSARTx,cmd_temp);	
//							Usart3_SendString(BLEUSARTx,cmd_temp);	
//							Usart3_SendString(BLEUSARTx,cmd_temp);	
//							Usart3_SendString(BLEUSARTx,cmd_temp);	
							
							memset(cmd_temp,0,40);
							strcat((char *)cmd_temp,"AT+BLEGATTCWR=0,");
							strncat((char *)cmd_temp,(const char *)&char_id,1);
							strcat((char *)cmd_temp,",1,,4\r\n");
							Usart3_SendString(BLEUSARTx,cmd_temp);	
						}else
						{
							ble_time = 10;
						}
						break;
				case 7: 

						if(ble_connect_flag&&char_id>'0'){
							
							memset(cmd_temp,0,40);
							strcat((char *)cmd_temp,"AT+BLEGATTCWR=0,");
							strncat((char *)cmd_temp,(const char *)&char_id,1);
							strcat((char *)cmd_temp,",1,,4\r\n");
							Usart3_SendString(BLEUSARTx,cmd_temp);	
//							Usart3_SendString(BLEUSARTx,cmd_temp);	
//							Usart3_SendString(BLEUSARTx,cmd_temp);	
//							Usart3_SendString(BLEUSARTx,cmd_temp);	
						}else
						{
							ble_time = 10;
						}
						break;
				case 8: if(ble_connect_flag&&char_id>'0'){
							u8 zhendong[10]={0xab,0x04,0x09,0x90};
							Usart3_SendStr_length(BLEUSARTx,( uint8_t *)zhendong,4);	
						}else
						{
							ble_time = 10;
						}
						break;	
				case 9:
						if(ble_connect_flag&&char_id>'0'){
//							memset(cmd_temp,0,40);
//							strcat((char *)cmd_temp,"AT+BLEGATTCCHAR=0,");
//							strncat((char *)cmd_temp,(const char *)&char_id,1);
//							strcat((char *)cmd_temp,"\r\n");
//							Usart3_SendString(BLEUSARTx,cmd_temp);	
							
							memset(cmd_temp,0,40);
							strcat((char *)cmd_temp,"AT+BLEGATTCWR=0,");
							strncat((char *)cmd_temp,(const char *)&char_id,1);
							strcat((char *)cmd_temp,",2,1,1\r\n");
							Usart3_SendString(BLEUSARTx,cmd_temp);	
						}else
						{
							ble_time = 10;
						}
					break;
				case 10:
						if(ble_connect_flag&&char_id>'0'){
							Usart3_SendString(BLEUSARTx,( uint8_t *)"1");	
							init_flag=1;
						}else
						{
							ble_time = 10;
						}
						
					break;
				default:
					shouhuan_con_flag++;
					break;
			
			}
#endif
			ble_time_flag=0;
		}

//		Usart3_SendString(BLEUSARTx,( uint8_t *)"AT+BLEINIT=0\r\n");
//		delay_ms(1000);
//		Usart3_SendString(BLEUSARTx,( uint8_t *)"AT+BLEINIT=1\r\n");
//		delay_ms(1000);
//		
//		memset(cmd_temp,0,40);
//		strcat((char *)cmd_temp,"AT+BLESCAN=1,3,1,");
//		strcat((char *)cmd_temp,(const char *)ble_mac);
//		Usart3_SendString(BLEUSARTx,cmd_temp);
//		delay_ms(1500);
//		memset(cmd_temp,0,40);
//		strcat((char *)cmd_temp,"AT+BLECONN=0,");
//		strcat((char *)cmd_temp,(const char *)ble_mac);
//		Usart3_SendString(BLEUSARTx,cmd_temp);
//		//Usart3_SendString(BLEUSARTx,( uint8_t *)"AT+BLECONN=0,\"BA:03:54:18:64:5F\"\r\n"); //6
//		//Usart3_SendString(BLEUSARTx,( uint8_t *)"AT+BLECONN=0,\"A4:C1:21:FA:82:AC\"\r\n");  //4
//		//Usart3_SendString(BLEUSARTx,( uint8_t *)"AT+BLECONN=0,\"BA:03:54:19:40:E7\"\r\n");//6 TODO: 扫描00c0	
//		delay_ms(1500);
//		Usart3_SendString(BLEUSARTx,( uint8_t *)"AT+BLEGATTCPRIMSRV=0\r\n");
//		delay_ms(1000);
//		delay_ms(1000);
////		Usart3_SendString(BLEUSARTx,( uint8_t *)"AT+BLEGATTCCHAR=0,6\r\n");
//		delay_ms(500);
//		if(char_id>'0'){
//			memset(cmd_temp,0,40);
//			strcat((char *)cmd_temp,"AT+BLEGATTCWR=0,");
//			strncat((char *)cmd_temp,(const char *)&char_id,1);
//			strcat((char *)cmd_temp,",3,1,1\r\n");
//			Usart3_SendString(BLEUSARTx,cmd_temp);	
//			delay_ms(1000);
//			delay_ms(1000);
//			delay_ms(800);
//			Usart3_SendString(BLEUSARTx,( uint8_t *)"1");			
//		}
		//Usart3_SendString(BLEUSARTx,( uint8_t *)"AT+BLEGATTCWR=0,4,3,1,1\r\n");
		//Usart3_SendString(BLEUSARTx,( uint8_t *)"AT+BLEGATTCWR=0,6,3,1,1\r\n");
	}
	
	if(!debug_ble_flag&&ble_connect_flag&&init_flag){
		
		if(xinlv_flag==1&&nb_xinlv_tx_flag==1){
			nb_xinlv_tx_flag=0;	
			uint8_t cmd_temp[40]={0};
			memset(cmd_temp,0,40);
			strcat((char *)cmd_temp,"AT+BLEGATTCWR=0,");
			strncat((char *)cmd_temp,(const char *)&char_id,1);
			strcat((char *)cmd_temp,",1,,5\r\n");
			Usart3_SendString(BLEUSARTx,cmd_temp);
			xinlv_flag=1;
			
		}
		if(xinlv_flag==2&&nb_xinlv_tx_flag==1){
			nb_xinlv_tx_flag=0;
			xinlv_flag=0;
			xinlv_flag=0;
			xinlv_cnt=0;
			u8 zhendong[10]={0xab,0x05,0x0f,0x01,0x4e};//ab050f014e
			Usart3_SendStr_length(BLEUSARTx,( uint8_t *)zhendong,5);	
		}
	}
	if(1==ble_tx_flag){
		ble_tx_flag=0;
		if(!ble_connect_flag){
			ble_dis_connect_calculate();
		}else{
			ble_connect_calculate();
		}
		NB_TCP_TX();
	
	}


}
/**********  ble接收函数  ********/
u8 data_tmp[BLE_MAX_SIZE];
u16 data_tmp_cnt;
static u8 data_rev_flag;
u8 bpm_value;

void BLE_IO_Clear_Cache(void)
{
	data_tmp_cnt=0;
	memset(data_tmp,0,sizeof(data_tmp));
}
void BLEUSART_INT_FUN(){
	if (USART_GetFlagStatus(BLEUSARTx, USART_FLAG_PE) != RESET)
       {
          USART_ReceiveData(BLEUSARTx);
         USART_ClearFlag(BLEUSARTx, USART_FLAG_PE);
       }
        
       if (USART_GetFlagStatus(BLEUSARTx, USART_FLAG_ORE) != RESET)
       {
           USART_ReceiveData(BLEUSARTx);
         USART_ClearFlag(BLEUSARTx, USART_FLAG_ORE);
       }
        
        if (USART_GetFlagStatus(BLEUSARTx, USART_FLAG_FE) != RESET)
       {
           USART_ReceiveData(BLEUSARTx);
          USART_ClearFlag(BLEUSARTx, USART_FLAG_FE);
       }
        
        if(USART_GetITStatus(BLEUSARTx, USART_IT_RXNE) != RESET)
        {   
            USART_ClearFlag(BLEUSARTx, USART_FLAG_RXNE);
            USART_ClearITPendingBit(BLEUSARTx, USART_IT_RXNE);
            bleioinfo.rev_buf[bleioinfo.write_pos++]=USART_ReceiveData(BLEUSARTx);
			bleioinfo.rev_status=1;
			if(bleioinfo.write_pos==(BLE_MAX_SIZE-1)){
				bleioinfo.write_pos=0;
			}
        }

//	if(USART_GetITStatus(BLEUSARTx,USART_IT_RXNE)!=RESET)
//	{
//		u8 err;
//		USART_ClearFlag(BLEUSARTx, USART_FLAG_RXNE); 
//		USART_ClearITPendingBit(BLEUSARTx,USART_IT_RXNE);
//		if(USART_GetFlagStatus(BLEUSARTx,USART_FLAG_NE|USART_FLAG_FE|USART_FLAG_PE))
//			err=1;//检测到噪音、帧错误或校验错误
//		else 
//			err=0;
//		if((err==0)&&(bleioinfo.write_pos<2047))
//		{
//			bleioinfo.rev_buf[bleioinfo.write_pos++]=USART_ReceiveData(BLEUSARTx);
//			bleioinfo.rev_status=1;
//			if(bleioinfo.write_pos==(BLE_MAX_SIZE-1)){
//				bleioinfo.write_pos=0;
//			}
//		}
//		else
//			USART_ReceiveData(BLEUSARTx);
//		
//	}	
}
extern u8 baojing_kakou_flag[10];
u8 ble_solve(void){
	if(bleioinfo.read_pos!=bleioinfo.write_pos){
		data_tmp[data_tmp_cnt++]=bleioinfo.rev_buf[bleioinfo.read_pos++];
		if(data_tmp_cnt==BLE_MAX_SIZE){
			data_tmp_cnt=0;
		}
		if(bleioinfo.read_pos==BLE_MAX_SIZE-1){
			bleioinfo.read_pos=0;
		}
		if(data_tmp_cnt==1 &&((data_tmp[data_tmp_cnt-1]==0x0a||data_tmp[data_tmp_cnt-1]==0x0d))){
			data_tmp_cnt=0;
			data_rev_flag=0;

		}
		if(data_tmp_cnt>2 &&(data_tmp[data_tmp_cnt-1]==0x0a||data_tmp[data_tmp_cnt-1]==0x0d)){
			data_tmp_cnt=0;
			data_rev_flag=1;
		}
	}
	if(data_rev_flag){
		if(strstr((const char*)data_tmp,(const char*)notify_cmd)){
		//if(strstr((const char*)data_tmp,"+NOTIFY:0,4,3,")){
			if(data_tmp[23]>30){
				bpm_value=data_tmp[23]; //心率值
				ble_tx_flag=1;
			}else if(data_tmp[24]>30){
				bpm_value=data_tmp[24]; //心率值
				ble_tx_flag=1;
			} else 	if(data_tmp[27]>30){
				bpm_value=data_tmp[27]; //心率值
				ble_tx_flag=1;
			}

			BLE_IO_Clear_Cache();
			data_rev_flag=0;
			return 0;
		}
		if(strstr((const char*)data_tmp,(const char*)"+NOTIFY:0,6,2")){
			char *p = strstr((const char*)data_tmp,"+NOTIFY:0,6,2");
		//if(strstr((const char*)data_tmp,"+NOTIFY:0,4,3,")){
			if(*(p+strlen("+NOTIFY:0,6,2")+8)>30){
				bpm_value=*(p+strlen("+NOTIFY:0,6,2")+8); //心率值
				ble_tx_flag=1;
			}

			BLE_IO_Clear_Cache();
			data_rev_flag=0;
			return 0;
		}
		//
		for(int i = 0; i < 4; i++)
		{
			if(ble_connect_flag2[i]&&strstr((const char*)data_tmp,"IN")&&strstr((const char*)data_tmp,"+NOTIFY:")){
				char *p = strstr((const char*)data_tmp,"IN");
				char *p1 = strstr((const char*)data_tmp,"+NOTIFY:");
				int num = *(p1+8)-'0'-1;
			
				if(num >= 0 && num <= 4)
				{
						init_flag2[num] = 1;
						baojing_kakou_flag[num] = p[2];
				}
				BLE_IO_Clear_Cache();
				data_rev_flag=0;
				return 0;		
			}
		}
		if(strstr((const char*)data_tmp,(const char*)"+NOTIFY:0,4,2")){
		//if(strstr((const char*)data_tmp,"+NOTIFY:0,4,3,")){
			if(data_tmp[21]>55&&data_tmp[21]<105){
				bpm_value=data_tmp[21]; //心率值
				ble_tx_flag=1;
			}else if(data_tmp[22]>55&&data_tmp[22]<105){
				bpm_value=data_tmp[22]; //心率值
				ble_tx_flag=1;
			} else 	if(data_tmp[27]>55&&data_tmp[27]<105){
				bpm_value=data_tmp[27]; //心率值
				ble_tx_flag=1;
			}

			BLE_IO_Clear_Cache();
			data_rev_flag=0;
			return 0;
		}
		if(strstr((const char*)data_tmp,"0xCC00")){
			char_id=data_tmp[19];
			notify_cmd[10]=char_id;
			BLE_IO_Clear_Cache();
			data_rev_flag=0;
			return 0;		
		}
		if(strstr((const char*)data_tmp,"0x00FF")){
			char_id2=data_tmp[19];
			BLE_IO_Clear_Cache();
			data_rev_flag=0;
			return 0;		
		}
		if(strstr((const char*)data_tmp,"2D00")){
//			char_id=data_tmp[19];
//			notify_cmd[10]=char_id;
			BLE_IO_Clear_Cache();
			data_rev_flag=0;
			return 0;		
		}
		if(strstr((const char*)data_tmp,"2D01")){
//			char_id=data_tmp[19];
//			notify_cmd[10]=char_id;
			BLE_IO_Clear_Cache();
			data_rev_flag=0;
			return 0;		
		}
		if(strstr((const char*)data_tmp,"18D0")){
			char_id=data_tmp[19];
			notify_cmd[10]=char_id;
			BLE_IO_Clear_Cache();
			data_rev_flag=0;
			return 0;		
		}
		if(strstr((const char*)data_tmp,"OK")){
			BLE_IO_Clear_Cache();
			data_rev_flag=0;
			return 0;
		}
		if(strstr((const char*)data_tmp,">")){
			BLE_IO_Clear_Cache();
			data_rev_flag=0;
//			if(ble_connect_flag==0)
//				ble_tx_flag=1;
//			ble_connect_flag=1;
			return 0;
		}
		//+BLECONN:
		if(strstr((const char*)data_tmp,"+BLECONN:")){

			if(ble_connect_flag==0)
				ble_tx_flag=1;
			char *p;
			p=strstr((const char *)data_tmp,"+BLECONN:");
			if(p&&(*(p+11)!='-')){
				if(*(p+9) == '0')
				{
					ble_connect_flag=1;
					init_flag=0;
				}else
				{
					int num = *(p+9)-'0'-1;
					if(num >= 0 && num <= 4)
						ble_connect_flag2[num]=1;
				}
			}else
			{
				if(*(p+9) == '0')
				{
					ble_connect_flag=0;
					init_flag=0;
				}else
				{
					int num = *(p+9)-'0'-1;
					if(num >= 0 && num <= 4)
						ble_connect_flag2[num]=0;
					init_flag2[num]=0;
				}
			
			}
			BLE_IO_Clear_Cache();
			data_rev_flag=0;
//			ble_connect_flag=1;
			return 0;
		}		
		if(strstr((const char*)data_tmp,",CONNECT")){
			
			
			mac_client_id=data_tmp[0];
			ble_ready_flag=1;
			BLE_IO_Clear_Cache();
			data_rev_flag=0;
			return 1;		
		}
		if(strstr((const char*)data_tmp,"+IPD")){
			char *p;
			if((p=strstr((const char *)data_tmp,"BLEMAC:"))){
				mac_client_id=data_tmp[5];
				//u8 data_temp[25]={0};
				strncpy((char *)ble_mac.shouhuan_mac +1,(const char *)p+7,17);
				ble_connect_flag=0;
				//memcpy(ble_mac+ 2,data_temp,17);
				ble_mac_rx_flag=1;
				write_to_flash();
			}
			else if((p=strstr((const char *)data_tmp,"BLEMAC1:"))){
				mac_client_id=data_tmp[5];
				//u8 data_temp[25]={0};
				strncpy((char *)&ble_mac.kakou_mac[0][1],(const char *)p+8,17);
				ble_connect_flag2[0]=0;
				//memcpy(ble_mac+ 2,data_temp,17);
				ble_mac_rx_flag=1;
				write_to_flash();
			}
			else if((p=strstr((const char *)data_tmp,"BLEMAC2:"))){
				mac_client_id=data_tmp[5];
				//u8 data_temp[25]={0};
				strncpy((char *)&ble_mac.kakou_mac[1][1],(const char *)p+8,17);
				ble_connect_flag2[1]=0;
				//memcpy(ble_mac+ 2,data_temp,17);
				ble_mac_rx_flag=1;
				write_to_flash();
			}
			else if((p=strstr((const char *)data_tmp,"BLEMAC3:"))){
				mac_client_id=data_tmp[5];
				//u8 data_temp[25]={0};
				strncpy((char *)&ble_mac.kakou_mac[2][1],(const char *)p+8,17);
				ble_connect_flag2[2]=0;
				//memcpy(ble_mac+ 2,data_temp,17);
				ble_mac_rx_flag=1;
				write_to_flash();
			}else if((p=strstr((const char *)data_tmp,"KKDATA:"))){
				mac_client_id=data_tmp[5];
				int num_KK = 0;
				for(num_KK = 0; num_KK < 3; num_KK++)
				{
					int ans = strncmp((char *)&ble_mac.kakou_mac[num_KK][1],(const char *)p+7,17);
					if(ans == 0)
					{
						break;
					}
				}
				if(num_KK < 3)
				{
					baojing_kakou_flag[num_KK] = *(p+24);
					ble_mac_rx_flag=num_KK+2;
				}
				
			}
			BLE_IO_Clear_Cache();
			data_rev_flag=0;
			return 1;		
		}
		if(strstr((const char*)data_tmp,"+BLEDISCONN:")){

			if(ble_connect_flag==1)
				ble_tx_flag=1;
						char *p;
			if((p=strstr((const char *)data_tmp,"+BLEDISCONN:"))){
				if(*(p+12) == '0')
				{
					ble_connect_flag=0;
					init_flag=0;
				}else
				{
					int num = *(p+12) -  '0' - 1;
					if(num >= 0 && num <= 4){
						ble_connect_flag2[num]=0;
						init_flag2[num]=0;
					}
				}
			}
			BLE_IO_Clear_Cache();
			data_rev_flag=0;
			return 1;		
		}
		if(strstr((const char*)data_tmp,"busy")){
					busy_flag = 1;
					ble_connect_flag=0;
					init_flag=0;
					init_flag2[0]=0;
			BLE_IO_Clear_Cache();
			data_rev_flag=0;
			return 1;		
		}
		BLE_IO_Clear_Cache();
		data_rev_flag=0;
	}
		
	return 0;	


}
