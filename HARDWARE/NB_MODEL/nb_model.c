#include "nb_model.h"
#include "bsp_nb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usart.h"	
#include "ble.h"
#include "common.h"
#include "flash.h"
#include "bsp_led.h"
#include "bsp_usart5.h"
#include "bsp_key.h"  

extern int key4_flag;
extern int key1_flag;

float wendu=101.1;

char str_nbimei_temp[20]={'\0'};
char fasongnbimei[60];

char str_jizhan_temp[20]={'\0'};
char fasongjizhan[60];

char fasongweidu[90];

char fasongjingdu[90];

char str_wendu_temp[10];
char fasongwendu[30];

char str_timersc_temp[10];
char fasongtimersc[30];

char str_bpm_temp[10];
char fasongbpm[30];

char str_dt_temp[10];
char fasongdt[30];

char str_peizhi_temp[10];
char fasongpeizhi[30];

uint32_t timersc_Value=0;


char str_do_temp[10];
char fasongdo[30];
float do_Value;
extern u8 bat_val;
char chezaikt_send_data[1024];
char jizhan_data[20];
char fasongkongge[5]={'2','0'};
char fasonghuiche[5]={'0','D','0','A'};
char fasongqingqiushangsuo[9]={'4','3','3','1','3','1','2','0'};
char fasongjiesuo[9]={'4','8','3','1','3','1','2','0'};
char fasongqingqiujiesuo[9]={'4','3','3','2','3','2','2','0'};
char fasongqingwucaozuo[9]={'3','0','3','0','3','0','2','0'};
char fasongshangsuozhuangtai[7]={'4','3','3','1','2','0'};
char fasongjiesuozhuangtai[7]={'4','F','3','2','2','0'};
char powerdownzhuangtai[9]={'4','F','4','6','2','0'};
char bleConnectZhuangtai[9]={'4','3','4','2','2','0'};
char bleDisConnectZhuangtai[9]={'4','4','4','2','2','0'};
char fasongSOS[9]={'5','3','4','F','5','3','2','0'};
char w11[9]={'5','7','3','1','3','1','2','0'};
char w10[9]={'5','7','3','1','3','0','2','0'};
char w21[9]={'5','7','3','2','3','1','2','0'};
char w20[9]={'5','7','3','2','3','0','2','0'};

char wg1[9]={'5','7','4','7','3','1','2','0'};

char wh1[9]={'5','7','4','8','3','1','2','0'};

extern u8 zhuiluo_flag ;//坠落判断标志


void strweidu_deal(void)
{
	int i,j;
	memset(fasongweidu,0,sizeof(fasongweidu));	
	for(i=0,j=18;j<28;i=i+2,j++)
	{
		if(('0'<=DMA_Rece_Buf[j])&&(DMA_Rece_Buf[j]<='9'))
		{
			fasongweidu[i]='3';
			fasongweidu[i+1]=DMA_Rece_Buf[j];
		}
		if('.'==DMA_Rece_Buf[j])
		{
			fasongweidu[i]='2';
			fasongweidu[i+1]='E';
		}
	}
			fasongweidu[i]='2';
			fasongweidu[i+1]='0';
}

void strjingdu_deal(void)
{
	int i,j;
	memset(fasongjingdu,0,sizeof(fasongjingdu));	
	for(i=0,j=31;j<42;i=i+2,j++)
	{
		if(('0'<=DMA_Rece_Buf[j])&&(DMA_Rece_Buf[j]<='9'))
		{
			fasongjingdu[i]='3';
			fasongjingdu[i+1]=DMA_Rece_Buf[j];
		}
		if('.'==DMA_Rece_Buf[j])
		{
			fasongjingdu[i]='2';
			fasongjingdu[i+1]='E';
		}
	}
			fasongjingdu[i]='2';
			fasongjingdu[i+1]='0';
}

void strnbimei_deal(void)
{
	int i,j;
	memset(str_nbimei_temp,0,sizeof(str_nbimei_temp));	
	memset(fasongnbimei,0,sizeof(fasongnbimei));	
	strcat(str_nbimei_temp,nb_imei_number);
	strcat(str_nbimei_temp," ");
	for(i=0,j=0;j<sizeof(str_nbimei_temp)&&i<sizeof(fasongnbimei)&&str_nbimei_temp[j]!=0;i=i+2,j++)
	{
		if(('0'<=str_nbimei_temp[j])&&(str_nbimei_temp[j]<='9'))
		{
			fasongnbimei[i]='3';
			fasongnbimei[i+1]=str_nbimei_temp[j];
		}
		if(str_nbimei_temp[j]==' ')
		{
			fasongnbimei[i]='2';
			fasongnbimei[i+1]='0';
		}
	}
}
void strjizhan_deal(void)
{
	int i,j;
	memset(str_jizhan_temp,0,sizeof(str_jizhan_temp));	
	memset(fasongjizhan,0,sizeof(fasongjizhan));	
	strcat(str_jizhan_temp,jizhan_data);
	strcat(str_jizhan_temp," ");
	for(i=0,j=0;j<sizeof(str_jizhan_temp)&&i<sizeof(fasongjizhan)&&str_jizhan_temp[j]!=0;i=i+2,j++)
	{
		if(('0'<=str_jizhan_temp[j])&&(str_jizhan_temp[j]<='9'))
		{
			fasongjizhan[i]='3';
			fasongjizhan[i+1]=str_jizhan_temp[j];
		}
		if(('A'<=str_jizhan_temp[j])&&(str_jizhan_temp[j]<='F'))
		{
			fasongjizhan[i]='4';
			fasongjizhan[i+1]=str_jizhan_temp[j]-'A'+'1';
		}
		if(str_jizhan_temp[j]==' ')
		{
			fasongjizhan[i]='2';
			fasongjizhan[i+1]='0';
		}
		if(str_jizhan_temp[j]==',')
		{
			fasongjizhan[i]='2';
			fasongjizhan[i+1]='C';
		}
	}
}
void strwendu_deal(void)
{
	int i,j;
	memset(str_wendu_temp,0,sizeof(str_wendu_temp));
	memset(fasongwendu,0,sizeof(fasongwendu));	
	sprintf(str_wendu_temp, "%.1f", wendu);
	strcat(str_wendu_temp," ");	
	for(i=0,j=0;str_wendu_temp[j]!='\0';i=i+2,j++)
	{
		if(('0'<=str_wendu_temp[j])&&(str_wendu_temp[j]<='9'))
		{
			fasongwendu[i]='3';
			fasongwendu[i+1]=str_wendu_temp[j];
		}
		if(' '==str_wendu_temp[j])
		{
			fasongwendu[i]='2';
			fasongwendu[i+1]='0';
		}
		if('-'==str_wendu_temp[j])
		{
			fasongwendu[i]='2';
			fasongwendu[i+1]='D';
		}
		if('.'==str_wendu_temp[j])
		{
			fasongwendu[i]='2';
			fasongwendu[i+1]='E';
		}
	}
}

void strtimersc_deal(void)
{
	int i,j;
	memset(str_timersc_temp,0,sizeof(str_timersc_temp));
	memset(fasongtimersc,0,sizeof(fasongtimersc));	
	sprintf(str_timersc_temp, "%d", timersc_Value);
	strcat(str_timersc_temp," ");	
	for(i=0,j=0;str_timersc_temp[j]!='\0';i=i+2,j++)
	{
		if(('0'<=str_timersc_temp[j])&&(str_timersc_temp[j]<='9'))
		{
			fasongtimersc[i]='3';
			fasongtimersc[i+1]=str_timersc_temp[j];
		}
		if(' '==str_timersc_temp[j])
		{
			fasongtimersc[i]='2';
			fasongtimersc[i+1]='0';
		}
	}
}

void strdo_deal(void)
{
	int i,j;
	memset(str_do_temp,0,sizeof(str_do_temp));
	memset(fasongdo,0,sizeof(fasongdo));	
	sprintf(str_do_temp, "%.2f", do_Value);
	strcat(str_do_temp," ");	
	for(i=0,j=0;str_do_temp[j]!='\0';i=i+2,j++)
	{
		if(('0'<=str_do_temp[j])&&(str_do_temp[j]<='9'))
		{
			fasongdo[i]='3';
			fasongdo[i+1]=str_do_temp[j];
		}
		if(' '==str_do_temp[j])
		{
			fasongdo[i]='2';
			fasongdo[i+1]='0';
		}
		if('.'==str_do_temp[j])
		{
			fasongdo[i]='2';
			fasongdo[i+1]='E';
		}
	}
}

void strbpm_deal(void)
{

}
extern int height;

void strdt_deal(void)
{
	int i,j,dt_tmp;
	memset(str_dt_temp,0,sizeof(str_dt_temp));
	memset(fasongdt,0,sizeof(fasongdt));	
	if(height < 0)
		dt_tmp = 0;
	else
		dt_tmp = height;
	sprintf(str_dt_temp, "%d", dt_tmp);
	strcat(str_dt_temp," ");	
	fasongdt[0]='5';
	fasongdt[1]='4';
	for(i=2,j=0;str_dt_temp[j]!='\0';i=i+2,j++)
	{
		if(('0'<=str_dt_temp[j])&&(str_dt_temp[j]<='9'))
		{
			fasongdt[i]='3';
			fasongdt[i+1]=str_dt_temp[j];
		}
		if(' '==fasongdt[j])
		{
			fasongdt[i]='2';
			fasongdt[i+1]='0';
		}
	}
}
extern u8 peizhi[10];
void strpeizhi_deal(void)
{
	if(key4_flag >= 7 && key4_flag <= 10)
	{
		int i,j, peizhi_tmp;
		u8 key_id = 0;
		memset(str_peizhi_temp,0,sizeof(str_peizhi_temp));
		memset(fasongpeizhi,0,sizeof(fasongpeizhi));	
		peizhi_tmp = peizhi[key4_flag - 7];
		key_id = key4_flag - 6;
		sprintf(str_peizhi_temp, "%d", key_id);
		sprintf(str_peizhi_temp+1, "%d", peizhi_tmp);
		strcat(str_peizhi_temp," ");	
		fasongpeizhi[0]='5';
		fasongpeizhi[1]='2';
		for(i=2,j=0;str_peizhi_temp[j]!='\0';i=i+2,j++)
		{
			if(('0'<=str_peizhi_temp[j])&&(str_peizhi_temp[j]<='9'))
			{
				fasongpeizhi[i]='3';
				fasongpeizhi[i+1]=str_peizhi_temp[j];
			}
			if(' '==fasongpeizhi[j])
			{
				fasongpeizhi[i]='2';
				fasongpeizhi[i+1]='0';
			}
		}
	}
}
void ble_connect_calculate(void)
{
	timersc_Value=bat_val;
	memset(chezaikt_send_data,0,sizeof(chezaikt_send_data));
	
	strweidu_deal();
	strjingdu_deal();
	strnbimei_deal();
	strwendu_deal();
	strtimersc_deal();
	strbpm_deal();
	strjizhan_deal();
	strcat(chezaikt_send_data,fasongnbimei);
	strcat(chezaikt_send_data,fasongwendu);	
	strcat(chezaikt_send_data,bleConnectZhuangtai); //发送关机
	strcat(chezaikt_send_data,fasongtimersc);
	
	if(key4_flag==0)
		strcat(chezaikt_send_data,fasongqingwucaozuo);
  if(key4_flag==1)
		strcat(chezaikt_send_data,fasongjiesuo);//H11，点击2次；O2解锁
	if(key4_flag==2)
		strcat(chezaikt_send_data,fasongqingqiujiesuo);	//C22，长按3S；请求解锁
	if(key4_flag==3)
		strcat(chezaikt_send_data,fasongqingqiushangsuo);	//C11，点击1次；请求上锁
	
//	strcat(chezaikt_send_data,"20");
	strcat(chezaikt_send_data,fasongweidu);
	strcat(chezaikt_send_data,fasongjingdu);
//	strcat(chezaikt_send_data,"20");		
	
	strcat(chezaikt_send_data,fasongjizhan);
	strcat(chezaikt_send_data,fasongbpm);
	strcat(chezaikt_send_data,fasonghuiche);
}
void ble_dis_connect_calculate(void)
{
	timersc_Value=bat_val;
	memset(chezaikt_send_data,0,sizeof(chezaikt_send_data));
	
  strweidu_deal();
  strjingdu_deal();
  strnbimei_deal();
	strwendu_deal();
	strtimersc_deal();
	strbpm_deal();
	strjizhan_deal();
	strcat(chezaikt_send_data,fasongnbimei);
	strcat(chezaikt_send_data,fasongwendu);	
	strcat(chezaikt_send_data,bleDisConnectZhuangtai); //发送关机
	strcat(chezaikt_send_data,fasongtimersc);
	
	if(key4_flag==0)
		strcat(chezaikt_send_data,fasongqingwucaozuo);
  if(key4_flag==1)
		strcat(chezaikt_send_data,fasongjiesuo);//H11，点击2次；O2解锁
	if(key4_flag==2)
		strcat(chezaikt_send_data,fasongqingqiujiesuo);	//C22，长按3S；请求解锁
	if(key4_flag==3)
		strcat(chezaikt_send_data,fasongqingqiushangsuo);	//C11，点击1次；请求上锁
//	
//	strcat(chezaikt_send_data,"20");
	strcat(chezaikt_send_data,fasongweidu);
	strcat(chezaikt_send_data,fasongjingdu);

//	strcat(chezaikt_send_data,"20");		
	
	strcat(chezaikt_send_data,fasongjizhan);
//	strcat(chezaikt_send_data,fasongbpm);

	strcat(chezaikt_send_data,fasonghuiche);
}
void powerdown_calculate(void)
{
	timersc_Value=bat_val;
	memset(chezaikt_send_data,0,sizeof(chezaikt_send_data));
	
	strweidu_deal();
	strjingdu_deal();
	strnbimei_deal();
	strwendu_deal();
	strtimersc_deal();
	strbpm_deal();
	strjizhan_deal();
	
	strcat(chezaikt_send_data,fasongnbimei);
	strcat(chezaikt_send_data,fasongwendu);	
	strcat(chezaikt_send_data,powerdownzhuangtai); //发送关机
	strcat(chezaikt_send_data,fasongtimersc);
	
	if(key4_flag==0)
		strcat(chezaikt_send_data,fasongqingwucaozuo);
  if(key4_flag==1)
		strcat(chezaikt_send_data,fasongjiesuo);//H11，点击2次；O2解锁
	if(key4_flag==2)
		strcat(chezaikt_send_data,fasongqingqiujiesuo);	//C22，长按3S；请求解锁
	if(key4_flag==3)
		strcat(chezaikt_send_data,fasongqingqiushangsuo);	//C11，点击1次；请求上锁
	
//	strcat(chezaikt_send_data,"20");
	strcat(chezaikt_send_data,fasongweidu);
	strcat(chezaikt_send_data,fasongjingdu);

//	strcat(chezaikt_send_data,"20");		
	
	strcat(chezaikt_send_data,fasongjizhan);
//	strcat(chezaikt_send_data,fasongbpm);

	strcat(chezaikt_send_data,fasonghuiche);
}

void  warn_flag_calculate(u8 tmp)
{
	timersc_Value=bat_val;
	memset(chezaikt_send_data,0,sizeof(chezaikt_send_data));
	
	strweidu_deal();
	strjingdu_deal();
	strnbimei_deal();
	strwendu_deal();
	strtimersc_deal();
	strjizhan_deal();
	strcat(chezaikt_send_data,fasongnbimei);
	strcat(chezaikt_send_data,fasongwendu);	
	if(key1_flag==0)
		strcat(chezaikt_send_data,fasongjiesuozhuangtai);
	if(key1_flag==1)
		strcat(chezaikt_send_data,fasongshangsuozhuangtai);
	strcat(chezaikt_send_data,fasongtimersc);
	extern u8 baojing_zitai_flag;
	extern u8 baojing_kakou_total_flag;

	extern u8 baojing_kakou_flag;
	if(tmp == 0)
	{
	if(baojing_zitai_flag==0)//姿态报警
		strcat(chezaikt_send_data,w10);
  else if(baojing_zitai_flag==1)
		strcat(chezaikt_send_data,w11);
	}
	else 
	{
		if(tmp<5 && tmp>= 2){
			w21[3] = tmp + '0';
			w20[3] = tmp + '0';
			if(baojing_kakou_flag==1)//卡扣报警
				strcat(chezaikt_send_data,w21);
		   
			else if (baojing_kakou_flag==0)
				strcat(chezaikt_send_data,w20);	
		}
	}
	strcat(chezaikt_send_data,fasongweidu);
	strcat(chezaikt_send_data,fasongjingdu);
	strcat(chezaikt_send_data,fasongjizhan);
	strcat(chezaikt_send_data,fasonghuiche);
}
void chezaikongtiao_calculate(void)
{  
	timersc_Value=bat_val;
	memset(chezaikt_send_data,0,sizeof(chezaikt_send_data));
	
	strweidu_deal();
	strjingdu_deal();
	strnbimei_deal();
	strwendu_deal();
	strtimersc_deal();
	strbpm_deal();
	strjizhan_deal();
	strdt_deal();
	strpeizhi_deal();
	strcat(chezaikt_send_data,fasongnbimei);
	strcat(chezaikt_send_data,fasongwendu);
	extern u8 anjianjiaoling_flag;
	
	if(key1_flag==0)
		strcat(chezaikt_send_data,fasongjiesuozhuangtai);
	if(key1_flag==1)
		strcat(chezaikt_send_data,fasongshangsuozhuangtai);
	
	strcat(chezaikt_send_data,fasongtimersc);
	
	if(key4_flag==0)
		strcat(chezaikt_send_data,fasongqingwucaozuo);
  if(key4_flag==1)
		strcat(chezaikt_send_data,fasongjiesuo);//H11，点击2次；O2解锁
	if(key4_flag==2)
		strcat(chezaikt_send_data,fasongqingqiujiesuo);	//C22，长按3S；请求解锁
	if(key4_flag==3)
		strcat(chezaikt_send_data,fasongqingqiushangsuo);	//C11，点击1次；请求上锁
	if(key4_flag==4)
		strcat(chezaikt_send_data,fasongSOS);	//SOS, 双击B15发送报警
	if(key4_flag==5 || key4_flag == 21)
	 {
		if(anjianjiaoling_flag == 1)
	   {
			strcat(chezaikt_send_data,fasongdt);	//高度发送，T0
	    strcat(chezaikt_send_data,"20");
		 }
	 }		 
	if(key4_flag >= 7 && key4_flag <= 10)
	{
		strcat(chezaikt_send_data,fasongpeizhi);	//配置回复P11P21P31P41回复R11R21R31R41
		strcat(chezaikt_send_data,"20");
	}
	if(key4_flag == 20)
	{
		strcat(chezaikt_send_data,wg1);	//高度坠落WG1
	}
	if(key4_flag == 30)
	{
		strcat(chezaikt_send_data,wh1);	//疲劳状态
		
	}
	
//	strcat(chezaikt_send_data,"20");		
	strcat(chezaikt_send_data,fasongweidu);
	strcat(chezaikt_send_data,fasongjingdu);
	
	strcat(chezaikt_send_data,"20");		
	
	strcat(chezaikt_send_data,fasongjizhan);
//	strcat(chezaikt_send_data,fasongbpm);
	strcat(chezaikt_send_data,fasonghuiche);
}
extern void set_mpu6050_init();

extern u8 init_0_flag;
extern u8 anjianjiaoling_flag ;
extern  int dt_old_tmp =0;

extern u8 init_0_ok = 0;
u8 gd_jiesuo_key = 0;
extern u8 shangsuo_flag = 0;
extern u8 shangsuo_delay =0;
void anjiandeal(void)//按键处理函数
{
	
	static u8 pilao_flag = 0;

	extern int height;
	int dt_tmp;
//	extern u8 init_0_flag ;
//	static  int dt_old_tmp;
	if(init_0_ok == 0 && init_0_flag == 1)
	{
		if(abs(height) < 10)
		{
			init_0_ok = 1;
			dt_old_tmp = height;
		}
	}

	if(key4_flag == 30)
	{
//			extern void pilaoyy();
//			pilaoyy();
			
			nozitai_flag=1;//取消姿态语音
			
			key4_flag=30;
			chezaikongtiao_calculate();
	//		TIM_ITConfig(GENERAL_TIM,TIM_IT_Update,DISABLE);
			NB_TCP_TX();
		key4_flag = 0;
	}
	if(key4_flag >= 7 && key4_flag <= 10)
	{
		timersc_Value=bat_val;
		chezaikongtiao_calculate();
//		TIM_ITConfig(GENERAL_TIM,TIM_IT_Update,DISABLE);
		NB_TCP_TX();
//		TIM_ITConfig(GENERAL_TIM,TIM_IT_Update,ENABLE);
		key4_flag=0;		

	}
	extern u8 key21_flag;
	extern u8 gaoduyuyin_flag;
	if(key21_flag == 6)//定时发送
	{
		
		 dt_tmp = height;
		if(init_0_ok == 1&&zhuiluo_flag==0&& (abs(dt_tmp - dt_old_tmp)) >= 100&& (abs(dt_tmp - dt_old_tmp)) <= 1000)   //wg1
		{
			extern u8 init_0_flag;
			extern u8 peizhi[10];
			dt_old_tmp = dt_tmp;
			key4_flag = 20;
			chezaikongtiao_calculate();
	//		TIM_ITConfig(GENERAL_TIM,TIM_IT_Update,DISABLE);
			NB_TCP_TX();
	//		TIM_ITConfig(GENERAL_TIM,TIM_IT_Update,ENABLE);
		}else if((abs(dt_old_tmp - dt_tmp)) >= 5)
		{
			if(init_0_ok)
			{
				extern u8 peizhi[10];//高度自动上锁
				
			//	if(peizhi[3]==1&&anjianjiaoling_flag==1)
				if(anjianjiaoling_flag==1)
				{
					if(init_0_ok == 1 && dt_old_tmp < 20 && dt_tmp >= 20  )
					{
						
//						ssbzw_flag=1;		
//		      if(jsbzw_flag==1)
//						{
						extern void weixiangd();		//新增危险高度语音
						weixiangd();	
						
					  gaoduyuyin_flag=1;   //延时高度语音
//					  }
//						else 
//							if(ssbzw_flag==1)
//						{
//							gaoduyuyin_flag=0;
//						}
					shangsuo_delay=1;//延时3s上锁
						
					//	LED1_OFF;
					//	LED2_ON;
						
				//		shangsuo_flag=1;
					}
					else if(init_0_ok == 1 && dt_old_tmp >= 20&& dt_tmp < 20)
					{
						extern u8 motor_flag;
						motor_flag = 1;
						LED1_ON;
						LED2_OFF; 	//锁扣解锁
						jsbzw_flag=1;			
						shangsuo_flag=0;
						gaoduyuyin_flag=0;
					}

				}
				dt_old_tmp = dt_tmp;
				key4_flag = 21;
				chezaikongtiao_calculate();

				NB_TCP_TX();
			}
		}
		key21_flag=0;		
		key4_flag = 0;
	}
	if(key4_flag == 5)//高度校准归零
	{
		void set_height_zero();
		extern void yygd_init();
		set_height_zero();
		dt_old_tmp = 0;
		init_0_flag = 1;
		yygd_init();
		chezaikongtiao_calculate();
//		TIM_ITConfig(GENERAL_TIM,TIM_IT_Update,DISABLE);
		NB_TCP_TX();
//		TIM_ITConfig(GENERAL_TIM,TIM_IT_Update,ENABLE);
		key4_flag=0;		
	  anjianjiaoling_flag = 1 ;//按键校0标志
		zhuiluo_flag=1;
	}
	if(key4_flag==4)
	{
		extern void sos();
		timersc_Value=bat_val;
		sos();
		chezaikongtiao_calculate();
//		TIM_ITConfig(GENERAL_TIM,TIM_IT_Update,DISABLE);
		NB_TCP_TX();
//		TIM_ITConfig(GENERAL_TIM,TIM_IT_Update,ENABLE);
		key4_flag=0;
	}
////////////////////////////////////
	if(key4_flag==3)
	{
	
		extern void jiesuo();
		timersc_Value=bat_val;
		jiesuo();
		chezaikongtiao_calculate();
//		TIM_ITConfig(GENERAL_TIM,TIM_IT_Update,DISABLE);
		NB_TCP_TX();
//		TIM_ITConfig(GENERAL_TIM,TIM_IT_Update,ENABLE);
		key4_flag=0;
//		shangsuo_flag=0;
	}
/////////////////////////////////////
	if(key4_flag==2)
	{
			extern void shangsuo();
		timersc_Value=bat_val;
		shangsuo();
		chezaikongtiao_calculate();
//		TIM_ITConfig(GENERAL_TIM,TIM_IT_Update,DISABLE);
		NB_TCP_TX();
//		TIM_ITConfig(GENERAL_TIM,TIM_IT_Update,ENABLE);
		key4_flag=0;
//		shangsuo_flag=0;
	}
////////////////////////////////////
	if(key4_flag==1)
	{
		extern u8 jj_js_flag;
		extern void yyjj_jiesuo(void);
		
		if(jj_js_flag)
		{
			jj_js_flag = 0;
			yyjj_jiesuo();
		}
		
		timersc_Value=bat_val;
		chezaikongtiao_calculate();
//		TIM_ITConfig(GENERAL_TIM,TIM_IT_Update,DISABLE);
		NB_TCP_TX();
//		TIM_ITConfig(GENERAL_TIM,TIM_IT_Update,ENABLE);
		key4_flag=0;
		
		gd_jiesuo_key=1;//置一后进入15s计时
//		shangsuo_flag=0;
	}
}
void str2str(char *dest,const char *str){
	for(u8 i=0,j=0;i<strlen(str);i++,j+=2){
		u8 temp1,temp2;
		if(str[j]>='0'&&str[j]<='9'){
			temp1=str[j]-'0';
		}else if(str[j]>='a'&&str[j]<='f'){
			temp1=str[j]-'a'+0X0A;
		}
		if(str[j+1]>='0'&&str[j+1]<='9'){
			temp2=str[j+1]-'0';
		}else if(str[j+1]>='a'&&str[j+1]<='f'){
			temp2=str[j+1]-'a'+0X0A;
		}
		dest[i]=temp1<<4|temp2;
	}
}
void str2str_TX(char *dest,const char *str){
	for(u16 i=0,j=0;i<Usart1_Rec_Cnt;i++,j+=2){
		u8 temp1,temp2;
		temp1=(str[i]&0xf0)>>4;
		temp2=(str[i]&0x0f);
		if(temp1>=0&&temp1<=9){
			dest[j]=temp1+'0';
		}else if(temp1>=0x0a&&temp1<=0x0f){
			dest[j]=temp1-0x0a+'a';
		}
		if(temp2>=0&&temp2<=9){
			dest[j+1]=temp2+'0';
		}else if(temp2>=0x0a&&temp2<=0x0f){
			dest[j+1]=temp2-0x0a+'a';
		}
	}
}
extern u8 ble_mac[44];
extern u8 ble_connect_flag;
u8 nb_data[NB_MAX_SIZE];
u16 nb_data_cnt;
static u8 data_rev_flag;

void NB_data_Clear_Cache(void)
{
	nb_data_cnt=0;
	memset(nb_data,0,sizeof(nb_data));
}
extern u8 nb_time_flag;
extern u8 nb_time;
char rx_temp[30];
u8 connect_flag_temp;
extern u8 NB_TCP_CONNECT_FLAG;
extern u8 NB_TCP_INIT_GPS_FLAG;
u8 time_3630[10];
u8 date_3630[10];
u8 gps_3630[2][20];
int16_t NB_Solve(){
	if(nbioinfo.read_pos!=nbioinfo.write_pos){
		nb_data[nb_data_cnt++]=nbioinfo.rev_buf[nbioinfo.read_pos++];
		if(nb_data_cnt==(NB_MAX_SIZE-1)){
			nb_data_cnt=0;
		}
		if(nbioinfo.read_pos==(NB_MAX_SIZE-1)){
			nbioinfo.read_pos=0;
		}
		if(nb_data_cnt==1&&(nb_data[nb_data_cnt-1]==0x0d||nb_data[nb_data_cnt-1]==0x0a)){
			nb_data_cnt=0;
			data_rev_flag=0;		
		}
		if(nb_data_cnt>2&&(nb_data[nb_data_cnt-1]==0x0d||nb_data[nb_data_cnt-1]==0x0a)){
			nb_data_cnt=0;
			data_rev_flag=1;		
		}
	}
	if(data_rev_flag){
		if(strstr((const char *)nb_data,"+ZREADY")){
			//nb_time_flag=1;
		}
		if(strstr((const char *)nb_data,"OK")){
			//nb_time_flag=1;
		}
		if(nb_time==6&&strstr((const char *)nb_data,"ERROR")){
			nb_time=7;
			NB_data_Clear_Cache();
			data_rev_flag=0;
		}
		if(strstr((const char *)nb_data,"OK")){
//			nb_time_flag=1;
		}
		if(strstr((const char *)nb_data,"+CSQ:")){
			//nb_time_flag=1;
		}
		if(strstr((const char *)nb_data,"+CREG:")){
			//nb_time_flag=1;
		}
		if(strstr((const char *)nb_data,"+ZIPSTAT: 1,1")){
			NB_TCP_CONNECT_FLAG=1;
		}
		if(strstr((const char *)nb_data,"+ZIPSTAT: 1,0")){
			NB_TCP_CONNECT_FLAG=0;
			nb_time_flag=0;
			nb_time=1;
		}
		if(strstr((const char *)nb_data, "$GPRMC")){
			if(NB_TCP_INIT_GPS_FLAG ==0)
				NB_TCP_INIT_GPS_FLAG = 1;
				int i = 0, j = 0, k = 0;
				char argv[12][50]={{0}};
				for(i = 7; nb_data[i] != '\0'; i++)
				{
						if(nb_data[i] == ',')
						{
								j++;
								k = 0;
						}else
						{
								argv[j][k] = nb_data[i], 
								k++;
						}
				}
				if(strlen(argv[0]) > 0 && strstr(argv[1], "A"))
				{
					memset(time_3630, 0, sizeof(time_3630));
					memset(date_3630, 0, sizeof(date_3630));
					memset(gps_3630, 0, 40);
					strcpy((char *)time_3630, argv[0]);
					strcpy((char *)time_3630, argv[8]);
					strcpy((char *)gps_3630[0], argv[2]);
					strcpy((char *)gps_3630[1], argv[4]);
					
				}
		}
		if(strstr((const char *)nb_data,"+ZIPRECV")){
			memset(rx_temp,0,sizeof(rx_temp));
			strcpy(rx_temp,(const char *)nb_data+34);
		}
		if(strstr((const char *)nb_data,"+ZIPSTAT")){
			//u8 data_temp[25]={0};
			connect_flag_temp=nb_data[12]-'0';
		}
		if(strstr(rx_temp,"O33")) //O33 上锁
		{
		if(Key_Scan(KEY1_INT_GPIO_PORT,KEY1_INT_GPIO_PIN)==0)
		{
				extern u8 motor_flag;
				motor_flag = 1;
				LED1_OFF;
				LED2_ON;
			
        shangsuo_flag=1;
				ssbzw_flag=1;	
		}
			  


		}
		if(strstr(rx_temp,"S44")) //S44 解锁
		{		
			if(Key_Scan(KEY2_INT_GPIO_PORT,KEY2_INT_GPIO_PIN)==0)
			{
				extern u8 motor_flag;
				motor_flag = 1;
				LED1_ON;
				LED2_OFF; 	
				jsbzw_flag=1;
				shangsuo_flag=0;
      }
		}	
		if(strstr((const char *)nb_data,"+ZCDS")) //S44 解锁
		{
			u8 i=0,j=0,num_1,num_5;
			while(i<strlen((const char *)nb_data)){
				if(nb_data[i]==',')
					j++;
				i++;
				if(j==0)
					num_1=i;
				if(j==5){
					num_5=i;
					break;
				}
			}
			if(num_5-num_1-2 < 20)
				strncpy(jizhan_data,nb_data+num_1+1,(num_5-num_1-2));

		}	
		if(strstr(rx_temp,"P11")||strstr(rx_temp,"P10")
		||strstr(rx_temp,"P21")||strstr(rx_temp,"P20")
		||strstr(rx_temp,"P31")||strstr(rx_temp,"P30")
		||strstr(rx_temp,"P41")||strstr(rx_temp,"P40")
		)
		{
			char *s = rx_temp;
			char *p = NULL;
			for(u8 i = 0; i < 9; i++)
			{
				u8 temp[10] = {0};
				sprintf((char *)temp, "P%d", i+1);
				p = NULL;
				p = strstr(rx_temp, (const char *)temp);
				if(p)
				{
					u8 peizhi_value = p[2] - '0';
					extern u8 peizhi[10];
					if(peizhi_value == 0 || peizhi_value == 1)
					{
						peizhi[i] = peizhi_value;
						key4_flag = i + 7;
						if(i == 0 && peizhi[i] == 0)
						{
							extern u8 set0_flag;
							set0_flag = 1;
						}
					}
				}
			}
		}	
		extern void write_to_flash();
		write_to_flash();
		memset(rx_temp,0,sizeof(rx_temp));
		NB_data_Clear_Cache();
		data_rev_flag=0;
		//NB_URC_READ();   
		
	}
	return 0;
}	
