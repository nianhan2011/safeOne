#include "sys.h"
#include "bsp_GENERALTim.h"
#include "delay.h"
#include "string.h"
#include "stdio.h"
#include "nb_model.h"
#include "bsp_nb.h"
#include "bsp_key.h"
#include "bsp_led.h"
#include "bsp_usart5.h"
#include "bsp_adc.h"
#include "usart.h"
#include "rl_control.h"
#include "iwdg.h"
#include "ble.h"
#include "common.h"
#include "nb_model.h"
#include "flash.h"
#include "height.h"
#include "bsp_led.h"
uint32_t time_fasongjishu_count = 0;
uint8_t shujufsflag = 0;
uint8_t powerdown_flag = 0;
int key1_flag = 0; // 锁状态判断标志
int abc_timers_count = 0;
u8 didian_flag = 0; // 低电量标识位，1可以报警，0不能报警
u8 xintiao_flag = 0;
u8 zhuiluo_flag = 0;
u8 anjianjiaoling_flag = 0;

void lwzsd(void);
void dlzsd(void);
void dsfssj(void);
void yxsjtz(void);
void dianjishangsuo(void);
void sendBeforePowerdown(void);

int key2_flag = 0;
int key3_flag = 0;
int key4_flag = 0; // 人为操作判断标志
int key5_flag = 0;
int key4_count = 0;
int key3_count = 0;
int key3_count2 = 0;
int key6_flag = 0;
extern int count;
extern __IO uint16_t ADC_ConvertedValue;
float ADC_ConvertedValueLocal;
unsigned char icount;
RCC_ClocksTypeDef RCC_Clocks;

extern u8 peizhi[10];

extern u8 baojing_kakou_timer;

int main(void)
{
	// YY_OFF;
	RCC_GetClocksFreq(&RCC_Clocks);

	read_from_flash();

	Input_GPIO_Config();
	LED_GPIO_Config();
	SystemInit();
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	if (Key_Scan(KEY3_INT_GPIO_PORT, KEY3_INT_GPIO_PIN))
	{
		delay_ms(60);
		if (Key_Scan(KEY3_INT_GPIO_PORT, KEY3_INT_GPIO_PIN))
		{
			debug_nb_flag = 1;
		}
	}
	NBUSART_Config();
	powerADCInit();
	CONTROL_GPIO_Config();
	delay_ms(100);
	extern int get_adc0_val();
	int temp0 = 0;
	while (temp0 < 40)
	{
		if (get_adc0_val() == 0)
		{
			Control1_OFF;
			while (1)
				;
		}
		delay_ms(50);
		temp0++;
	}
	dlzsd();
	GENERAL_TIM_Init();
	UARTx_Config();
	UART5_Config();
	// set_height_freq(0);
	// set_height_zero();
	delay_ms(500);
	delay_ms(500);
	delay_ms(500);
	int i = 0;

	LED4_OFF;
	delay_ms(200);
	LED4_ON;
	// dlzsd();
//
//	if(Key_Scan(KEY2_INT_GPIO_PORT,KEY2_INT_GPIO_PIN)==0)
//	{
//		jsbzw_flag=1;
//		LED1_ON;
//		LED2_OFF;
//	}
#ifndef NO_VOICE
	yyhy();
#endif
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(200);
	yytxdealhs();
	skssyswc_flag = 1;
	uart_init(9600);
	// WatchDog_Init(4,625);
	zhuiluo_flag = 0;

	while (1)
	{
		usart5_deal();
		lwzsd();
		nb_chushihua_hanshu_zhongduan();
		dlzsd();

#ifndef NO_VOICE

		yytxdealhs();
#endif
		yxsjtz();
		anjiandeal();
		dianjishangsuo();
#ifdef ME3630
		NB_Solve();
#endif
		// sendBeforePowerdown();
	}
}

extern u16 ADC_Convert[3];
float voltage_value[3];
u8 lianwangok; // 用于电池标志

void lwzsd(void) // 联网指示灯函数
{
	static u8 lw_init_flag = 0;

	if (NB_TCP_CONNECT_FLAG == 1)
	{
		if (!lw_init_flag)
		{
			extern void yylw();
			lw_init_flag = 1;
			yylw();
			delay_ms(25000);
			didian_flag = 0; // 电量不足标识位
			lianwangok = 1;

			/*************联网后高度校准*************/
			// 10.24注释
			//			delay_ms(1000);
			//			key4_flag = 5;
			//			void set_height_zero();
			//			extern void yygd_init();
			//			set_height_zero();
			//			dt_old_tmp = 0;
			//			init_0_flag = 1;
			//			yygd_init();
			//			chezaikongtiao_calculate();
			//			NB_TCP_TX();
			//			key4_flag = 0;

			/*************20230116添加联机高度校零***************/
			//      delay_ms(1000);
			//			void set_height_zero();
			//			set_height_zero();
			//			dt_old_tmp = 0;
			//			zhuiluo_flag =0 ;
			//			init_0_flag = 1;
		}
		LED3_OFF;
	}
	else
	{
		LED3_ON; // 联网指示灯，报自检已完成（第2段语音）
				 //
	}
}
extern u8 ad_done_flag;
// extern u8 didianliang_flag;
// extern u8 bat_lowflag;
u8 bat_val = 0;
u16 bat_lowcnt = 0;
u8 motor_flag = 0;
u16 time_10min_buf;

static u8 lw_init_flag = 0;

void dlzsd(void) // 电量指示灯函数
{
	if (motor_flag == 0) // 电机停止时，进行电量检测
	{
		//		delay_ms(300);
		voltage_value[1] = 3.3 * ((float)ADC_Convert[1] / 4096);
		bat_val = (voltage_value[1] * 2 - 3.5) * 100 / (4.0 - 3.5); // 3.5V~4.0V电量检测，低于3.55V报警
		if (bat_val < 3)
			bat_val = 0;
		if (bat_val > 100)
			bat_val = 100;
		if (bat_val >= 0 && bat_val < 10 && didian_flag == 0)
		{
			bat_lowcnt++;
			if (bat_lowcnt == 3000) // 5s内一直低于20%
			{
				extern void yydidianliang(void);
				yydidianliang();
				didian_flag = 1;
				nozitai_flag = 1; // 取消姿态语音
			}
		}
		else
		{
			bat_lowcnt = 0;
			//			     didian_flag= 0 ;
		}

		if (time_10min_buf >= 30000) // 每5min报一次低电量语音
		{
			extern void yydidianliang(void);
			yydidianliang();
			time_10min_buf = 0;
		}
		if (bat_val >= 0 && bat_val < 10)
		{ // 电量低     2022.11.29改电量指示灯
			LED6_OFF;
			LED5_ON;
		}
		else
		{
			LED6_ON;
			LED5_OFF;
		}

		if (powerdown_flag || voltage_value[1] * 2 < 3.0)
		{
			// todo send POWERDOWN message
			powerdown_flag = 1;
			powerdown_calculate();
			NB_TCP_TX();
			delay_ms(1000);
			__set_PRIMASK(1);
			Control1_OFF;
			while (1)
				;
		}
		ad_done_flag = 0;
	}
}

void dsfssj(void) // 定时发送函数
{
	if (shujufsflag == 1)
	{

		chezaikongtiao_calculate();
		// delay_ms(100);
		//		TIM_ITConfig(GENERAL_TIM,TIM_IT_Update,DISABLE);
		NB_TCP_TX();
		//		TIM_ITConfig(GENERAL_TIM,TIM_IT_Update,ENABLE);
		time_fasongjishu_count = 0;
		shujufsflag = 0;
	}
}

void yxsjtz(void) // 上锁停止时间函数
{
	if (sszcsj_time_flag == 1)
	{
		sszcsj_time_flag = 0;
		sszcsj_time_count = 0;
		timersc_Value = shangsuo_time;
		//		if(play_flag==1)
		//		{
		//			TIM_ITConfig(GENERAL_TIM,TIM_IT_Update,DISABLE);
		//			yygb();
		//			TIM_ITConfig(GENERAL_TIM,TIM_IT_Update,ENABLE);
		//			play_flag=0;
		//		}
		//		TIM_ITConfig(GENERAL_TIM,TIM_IT_Update,DISABLE);
		key1_flag = 1;
		key4_flag = 0;
		timersc_Value = shangsuo_time;
		chezaikongtiao_calculate();
		NB_TCP_TX();
	}
	if (sbjsbz_flag == 1)
	{
		sbjsbz_flag = 0;
		key1_flag = 0;
		key4_flag = 1;
		timersc_Value = 0;
		chezaikongtiao_calculate();
		NB_TCP_TX();
	}
	if (xintiao_flag == 1) // 20230111每5分钟上报一次联网指令
	{
		xintiao_flag = 0;
		chezaikongtiao_calculate();
		NB_TCP_TX();
	}
}
extern u8 baojing_zitai_flag;
extern u8 baojing_kakou_flag;
extern u8 baojing_kakou_total_flag;
extern u16 baojing_kakou_time_cnt;

/*
限位开关 1无信号 2有信号 解锁延时完成标志位为1
姿态传感器有信号 外红传感器有信号 上锁
*/
u8 flag[7];
extern u8 IR_flag;
void dianjishangsuo(void)
{
	flag[1] = Key_Scan(KEY1_INT_GPIO_PORT, KEY1_INT_GPIO_PIN);
	flag[2] = Key_Scan(KEY2_INT_GPIO_PORT, KEY2_INT_GPIO_PIN);

	flag[3] = Key_Scan(KEYD0_INT_GPIO_PORT, KEYD0_INT_GPIO_PIN);
	flag[4] = Key_Scan(KEYD1_INT_GPIO_PORT, KEYD1_INT_GPIO_PIN);

	flag[5] = Key_Scan(KEY5_INT_GPIO_PORT, KEY5_INT_GPIO_PIN);
	flag[6] = Key_Scan(KEY6_INT_GPIO_PORT, KEY6_INT_GPIO_PIN);

	if (flag[3] == 0 && flag[4] == 0)
		baojing_kakou_flag = 1;
	else if (flag[3] == 1 && flag[4] == 0)
		baojing_kakou_flag = 0;

#ifndef ONLY_REMOTE_CONTROL
	if ((Key_Scan(KEY1_INT_GPIO_PORT, KEY1_INT_GPIO_PIN) == 0) && (skssyswc_flag == 1))
	{
		if ((ad_check()) && IR_flag)
		{
			// delay_ms(20);
			//			LED1_OFF; //改2022/11/29限位开关
			//			LED2_ON;
			LED2_OFF;
			LED1_ON;
			skssyswc_flag = 0;
			ssbzw_flag = 1;
			shangsuo_flag = 0;
		}
	}
#endif
	static u8 old_baojing_zitai_flag = 0, old_baojing_kakou_flag = 2;
	if (peizhi[0] == 1)
	{
#if 0
		if(flag[6])
		{
			baojing_zitai_flag = 1;
		}
		else
		{
			baojing_zitai_flag = 0;
		}
#endif
		if (!ad_check())
		{
			baojing_zitai_flag = 1;
		}
		else
		{
			baojing_zitai_flag = 0;
		}
		if (old_baojing_zitai_flag != baojing_zitai_flag)
		{
			warn_flag_calculate(0);
			NB_TCP_TX();
			old_baojing_zitai_flag = baojing_zitai_flag;
		}
	}
	else if (peizhi[0] == 0)
	{
		baojing_zitai_flag = 0;
	}

	//	if(peizhi[0] == 1)//卡扣报警选择配置
	//	{
	//   	for(int i = 0; i < 3; i++)
	//	  {
	if ((old_baojing_kakou_flag != baojing_kakou_flag)) //[i]改[0]
	{
		//				if(baojing_kakou_flag[0] == 0 && baojing_kakou_flag[1] == 0  && baojing_kakou_flag[2] == 0)//三个卡扣
		if (baojing_kakou_flag == 0)
		{
			baojing_kakou_total_flag = 0;
			baojing_kakou_timer = 0;
		} // 1个卡扣

		else if (baojing_kakou_flag == 1)
		{
			baojing_kakou_total_flag = 1;
			baojing_kakou_time_cnt = 800;
//			extern void waist_buckle_warn(); 
//				waist_buckle_warn();
			baojing_kakou_timer = 1;
		}

		warn_flag_calculate(0 + 2);
		NB_TCP_TX();
		old_baojing_kakou_flag = baojing_kakou_flag;
	}
	//		}
	//	}
}
