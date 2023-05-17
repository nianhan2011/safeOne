#ifndef BSP_NB_H
#define BSP_NB_H 	

#include "sys.h"
//定义通                                                                                                                                                                                                                                                                                                                信模块
#define  NB_MAX_SIZE		200
#define  NB_REV_SIZE		200

#define  NBDEBUG_USARTx                   USART2
#define  NBDEBUG_USART_CLK                RCC_APB1Periph_USART2
#define  NBDEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  NBDEBUG_USART_BAUDRATE           9600

#define  NBDEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  NBDEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  NBDEBUG_USART_TX_GPIO_PORT       GPIOA   
#define  NBDEBUG_USART_TX_GPIO_PIN        GPIO_Pin_2
#define  NBDEBUG_USART_RX_GPIO_PORT       GPIOA
#define  NBDEBUG_USART_RX_GPIO_PIN        GPIO_Pin_3

#define  NBDEBUG_USART_IRQ                USART2_IRQn
#define  NBDEBUG_USART_IRQHandler         USART2_IRQHandler

typedef struct
{
	unsigned int write_pos;				//写
	unsigned int read_pos;				//读
	unsigned char rev_status;
					 char rev_buf[NB_MAX_SIZE];		//接收缓存
} NB_IO_INFO;

extern NB_IO_INFO nbioinfo;
extern unsigned char NB_INIT_FLAG;
extern unsigned char NB_SOCKET_CLOSE_FLAG;
extern unsigned char NB_SOCKET_CREAT_FLAG;
extern unsigned char NB_TCP_CONNECT_FLAG;
extern unsigned char NB_TCP_TX_FLAG;
extern char NB_TCP_CLOCK[1];
extern char nb_imei_number[20];
extern char nb_tx_chezaikt_shuju[1000];
extern uint32_t sszcsj_time_count;
extern uint32_t jszcsj_time_count;
extern uint8_t sbjsbz_flag;
extern uint8_t skssys_flag;
extern uint32_t skssys_time;
extern uint8_t skssyswc_flag;
extern uint8_t sszcsj_time_flag;
extern uint8_t ssbzw_flag;
extern uint8_t jsbzw_flag;
extern uint32_t shangsuo_time;
extern uint32_t changanshijian;

extern u8 debug_nb_flag;
void NBUSART_Config(void);
void NBUsart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void NBUsart_SendString( USART_TypeDef * pUSARTx, char *str);
void NBUsart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);
void NBUsart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num);

void NB_IO_Send(unsigned char *str, unsigned short len);
void NB_IO_Clear_Cache(void);
_Bool NB_DEVICE_SendCmd(char *cmd,char *str);
void NB_MODULE_INIT(void);
void NB_SEARCH_IMEI(void);
void NB_TCP_REST(void);
void NB_TCP_SOCKET_CLOSE(void);
void NB_TCP_SOCKET_CREAT(void);
void NB_TCP_CONNECT(void);
void NB_TCP_CONNECT_1(void);
void NB_TCP_TX(void);
void nb_chushihua_hanshu_zhongduan(void);
void NB_SET_BOODLATE(void);
void NB_URC_READ(void);
#endif


