#ifndef __USART5_H
#define	__USART5_H


#include "stm32f10x.h"
#include <stdio.h>

extern uint8_t nozitai_flag;


extern uint8_t play_flag;

void UARTx_Config(void);
void Usart_SendStr_length( USART_TypeDef * pUSARTx, uint8_t *str,uint32_t strlen );
void Usart_SendString( USART_TypeDef * pUSARTx, uint8_t *str);
void yyhy(void);
void yytx(void);
void yygb(void);
void yytxdealhs(void);

void shangsuo(void);
void jiesuo(void);
void sos(void);


void weixiangd(void);
//void pilaoyy(void);

#endif /* __USAR5_H */
