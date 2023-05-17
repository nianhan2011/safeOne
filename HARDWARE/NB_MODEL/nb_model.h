#ifndef __NB_MODEL_H
#define __NB_MODEL_H
#include "sys.h"

extern char chezaikt_send_data[1024];

extern float ph_Value;
extern float do_Value;
extern uint32_t timersc_Value;
extern u8 gd_jiesuo_key;
extern u8 init_0_flag ;
extern u8 anjianjiaoling_flag ;
extern  int dt_old_tmp;
extern int dt_tmp;
extern u8 init_0_ok;
extern u8 shangsuo_flag;
extern u8 shangsuo_delay ;

void strnbimei_deal(void);
void strwendu_deal(void);
void strdo_deal(void);
void strtimersc_deal(void);
void anjiandeal(void);
void strweidu_deal(void);
void strjingdu_deal(void);

void chezaikongtiao_calculate(void);
void powerdown_calculate(void);
void ble_dis_connect_calculate(void);
void ble_connect_calculate(void);
//void dsfssj(void);
void str2str_TX(char *dest,const char *str);
void warn_flag_calculate(u8 tmp);
int16_t NB_Solve(void);
#endif
