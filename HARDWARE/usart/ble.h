#ifndef _BLE_H
#define _BLE_H
#include "common.h"
#include "stm32f10x.h"
#define             BLEUSART_BAUD_RATE                       115200

#define             BLEUSARTx                                USART3
#define             BLEUSART_APBxClock_FUN                   RCC_APB1PeriphClockCmd
#define             BLEUSART_CLK                             RCC_APB1Periph_USART3
#define             BLEUSART_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             BLEUSART_GPIO_CLK                        (RCC_APB2Periph_GPIOB)     
#define             BLEUSART_TX_PORT                         GPIOB   
#define             BLEUSART_TX_PIN                          GPIO_Pin_10
#define             BLEUSART_RX_PORT                         GPIOB
#define             BLEUSART_RX_PIN                          GPIO_Pin_11
#define             BLEUSART_IRQ                             USART3_IRQn
#define             BLEUSART_INT_FUN                         USART3_IRQHandler

struct BLEMAC{
	u8 shouhuan_mac[22];
	u8 kakou_mac[5][22];

};
#define BLE_MAX_SIZE 1000
typedef struct
{
	unsigned int write_pos;				//–¥
	unsigned int read_pos;				//∂¡
	unsigned char rev_status;
	char rev_buf[BLE_MAX_SIZE];		//Ω” ’ª∫¥Ê
} BLE_IO_INFO;
extern unsigned char bpm_value;
extern BLE_IO_INFO bleioinfo;
extern u8 ble_connect_flag;
extern u8 ble_time;
extern u8 ble_time_flag;
extern u8 debug_ble_flag;
void ble_init(void);
void UART3_Config(void);
unsigned char ble_solve(void);
void ble_connect(void);






















#endif

