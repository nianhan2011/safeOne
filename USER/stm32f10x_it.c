/**
 ******************************************************************************
 * @file    GPIO/IOToggle/stm32f10x_it.c
 * @author  MCD Application Team
 * @version V3.5.0
 * @date    08-April-2011
 * @brief   Main Interrupt Service Routines.
 *          This file provides template for all exceptions handler and peripherals
 *          interrupt service routine.
 ******************************************************************************
 * @attention
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "delay.h"
#include "nb_model.h"
#include "bsp_nb.h"
#include "string.h"
#include "bsp_key.h"
#include "bsp_led.h"
#include "bsp_GeneralTim.h"
#include "bsp_usart5.h"
#include "rl_control.h"
#include "bsp_adc.h"
#include "ble.h"
#include "height.h"
extern uint32_t time_fasongjishu_count;
extern uint8_t shujufsflag;
extern int key1_flag;
extern int key4_flag;
extern int key4_count;
int count = 0;
int count_1 = 0;
extern int abc_timers_count;
uint32_t shuangjiliangcijishi = 0;
uint32_t shuangjiliangcijishi_1 = 0;

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
	/* Go to infinite loop when Hard Fault exception occurs */
	while (1)
	{
	}
}

void MemManage_Handler(void)
{
	/* Go to infinite loop when Memory Manage exception occurs */
	while (1)
	{
	}
}

void BusFault_Handler(void)
{
	/* Go to infinite loop when Bus Fault exception occurs */
	while (1)
	{
	}
}

void UsageFault_Handler(void)
{
	/* Go to infinite loop when Usage Fault exception occurs */
	while (1)
	{
	}
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
}

void NBDEBUG_USART_IRQHandler(void) // NB串口中断服务程序
{
	u8 err;
	if (USART_GetFlagStatus(NBDEBUG_USARTx, USART_FLAG_PE) != RESET)
	{
		USART_ReceiveData(NBDEBUG_USARTx);
		USART_ClearFlag(NBDEBUG_USARTx, USART_FLAG_PE);
	}
	if (USART_GetFlagStatus(NBDEBUG_USARTx, USART_FLAG_ORE) != RESET)
	{
		USART_ReceiveData(NBDEBUG_USARTx);
		USART_ClearFlag(NBDEBUG_USARTx, USART_FLAG_ORE);
	}
	if (USART_GetFlagStatus(NBDEBUG_USARTx, USART_FLAG_FE) != RESET)
	{
		USART_ReceiveData(NBDEBUG_USARTx);
		USART_ClearFlag(NBDEBUG_USARTx, USART_FLAG_FE);
	}
	if (USART_GetITStatus(NBDEBUG_USARTx, USART_IT_RXNE) != RESET)
	{
		USART_ClearFlag(NBDEBUG_USARTx, USART_FLAG_RXNE);
		USART_ClearITPendingBit(NBDEBUG_USARTx, USART_IT_RXNE);
		if (USART_GetFlagStatus(NBDEBUG_USARTx, USART_FLAG_NE | USART_FLAG_FE | USART_FLAG_PE))
			err = 1; // 检测到噪音、帧错误或校验错误
		else
			err = 0;
		if ((err == 0) && (nbioinfo.write_pos < NB_MAX_SIZE))
		{
			nbioinfo.rev_buf[nbioinfo.write_pos++] = USART_ReceiveData(NBDEBUG_USARTx);
			nbioinfo.rev_status = 1;
			if (nbioinfo.write_pos == NB_MAX_SIZE - 1)
			{
				nbioinfo.write_pos = 0;
			}
		}
		else
			USART_ReceiveData(NBDEBUG_USARTx);
	}
}
// void EXTI9_5_IRQHandler(void){
// if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET)
//	{//上锁
//		if(Key_Scan(KEY1_INT_GPIO_PORT,KEY1_INT_GPIO_PIN)==1)
//		{
//		 LED1_OFF;
//		 LED2_OFF;
//		 key1_flag=1;
////     chezaikongtiao_calculate();
////     timersc_Value=shangsuo_time;
////     NB_TCP_TX();
//		}
//		EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);
//	}
////////////////////////////////////////////////////////////
//	if(EXTI_GetITStatus(KEY2_INT_EXTI_LINE) != RESET)
//	{
//		if(Key_Scan(KEY2_INT_GPIO_PORT,KEY2_INT_GPIO_PIN)==1)
//		{
//      LED1_OFF;
//      LED2_OFF;
//      key1_flag=0;
//			sbjsbz_flag=1;
//			skssys_flag=1;
//			skssys_time=0;
//			skssyswc_flag=0;
////			chezaikongtiao_calculate();
////		  timersc_Value=shangsuo_time;
////		  NB_TCP_TX();
//		}
//		EXTI_ClearITPendingBit(KEY2_INT_EXTI_LINE);
//	}
//}
u16 time;
u16 IR_cnt;
u16 IR_flag;
u16 IR_flag_cnt;
void KEY1_IRQHandler(void)
{
	if (EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET)
	{ // 上锁
		if (Key_Scan(KEY1_INT_GPIO_PORT, KEY1_INT_GPIO_PIN) == 1 && ssbzw_flag == 1)
		{
			time = 0;
			key1_flag = 1;
			ssbzw_flag = 0;
			sszcsj_time_count = 0;
			sszcsj_time_flag = 1;
			extern u8 motor_flag;
			motor_flag = 0;
			LED1_OFF;
			LED2_OFF;
			//     chezaikongtiao_calculate();
			//     timersc_Value=shangsuo_time;
			//     NB_TCP_TX();
		}
		EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);
	}
	//////////////////////////////////////////////////////////
	if (EXTI_GetITStatus(KEY2_INT_EXTI_LINE) != RESET)
	{
		if (Key_Scan(KEY2_INT_GPIO_PORT, KEY2_INT_GPIO_PIN) == 1 && jsbzw_flag == 1)
		{
			time = 0;
			jsbzw_flag = 0;
			jszcsj_time_count = 0;
			extern u8 motor_flag;
			motor_flag = 0;
			LED1_OFF;
			LED2_OFF;
			key1_flag = 0;
			sbjsbz_flag = 1;
			skssys_flag = 1;
			skssys_time = 0;
			skssyswc_flag = 0;
			//			chezaikongtiao_calculate();
			//		  timersc_Value=shangsuo_time;
			//		  NB_TCP_TX();
		}
		EXTI_ClearITPendingBit(KEY2_INT_EXTI_LINE);
	}
}

u8 jj_js_flag = 0;
void KEY3_IRQHandler(void)
{

	if (EXTI_GetITStatus(KEY3_INT_EXTI_LINE) != RESET)
	{
		EXTI_ClearITPendingBit(KEY3_INT_EXTI_LINE);
		// delay_ms(50);
		//		if(Key_Scan(KEY2_INT_GPIO_PORT,KEY2_INT_GPIO_PIN)==1)
		//		{
		//		  LED1_OFF;
		//		  LED2_OFF;
		//		}
		//		if(Key_Scan(KEY2_INT_GPIO_PORT,KEY2_INT_GPIO_PIN)==0)
		//		{

		//	  }
	}
	if (EXTI_GetITStatus(KEY4_INT_EXTI_LINE) != RESET)
	{
		EXTI_ClearITPendingBit(KEY4_INT_EXTI_LINE);

		//		  if(Key_Scan(KEY4_INT_GPIO_PORT,KEY4_INT_GPIO_PIN)==1) // 按压两次K2解锁
		//		  {
		//		    count_1++;
		//		  }
		//		  if(count_1==2)
		//      {
		//		    key4_flag=4;
		//	      count_1=0;
		//	    }
	}
	///////////////////////////////////////////////////
	if (EXTI_GetITStatus(KEY5_INT_EXTI_LINE) != RESET) // 传感器上锁
	{
		EXTI_ClearITPendingBit(KEY5_INT_EXTI_LINE);
		IR_cnt++;
		//		if(Key_Scan(KEY1_INT_GPIO_PORT,KEY1_INT_GPIO_PIN)==1)
		//		{
		//			LED1_OFF;
		//			LED2_OFF;
		//		}
		/*
		限位开关1无信号 2有信号 延时完成标志位为1 姿态和红外有信号 上锁
		*/
		if ((Key_Scan(KEY1_INT_GPIO_PORT, KEY1_INT_GPIO_PIN) == 0) && (skssyswc_flag == 1))
		{
#ifndef ONLY_REMOTE_CONTROL
			if ((ad_check()) && IR_flag)
			{
				//	delay_ms(20);
				LED1_OFF;
				LED2_ON;
				//				sszcsj_time_count=0;
				ssbzw_flag = 1;
				skssyswc_flag = 0;
				shangsuo_flag = 1;
			}
#endif
		}
	}
	////////////////////////////////////////////////////////////////
}
void EXTI1_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line1) != RESET) // 传感器上锁
	{
		EXTI_ClearITPendingBit(EXTI_Line1);
		IR_cnt++;
		if ((Key_Scan(KEY1_INT_GPIO_PORT, KEY1_INT_GPIO_PIN) == 0) && (skssyswc_flag == 1))
		{
#ifndef ONLY_REMOTE_CONTROL
			if ((ad_check()) && IR_flag)
			{
				//	delay_ms(20);
				LED1_OFF;
				LED2_ON;
				//				sszcsj_time_count=0;
				ssbzw_flag = 1;
				skssyswc_flag = 0;
				shangsuo_flag = 1;
			}
#endif
		}
	}
}
void KEY6_IRQHandler(void) // 传感器上锁
{
	if (EXTI_GetITStatus(KEY6_INT_EXTI_LINE) != RESET)
	{
		//		if(Key_Scan(KEY1_INT_GPIO_PORT,KEY1_INT_GPIO_PIN)==1)
		//		{
		//		  LED1_OFF;
		//		  LED2_OFF;
		//		}
		/*
		限位开关1无信号 2有信号 延时完成标志位为1 姿态和红外有信号 上锁
		*/
#ifndef ONLY_REMOTE_CONTROL
		if ((Key_Scan(KEY1_INT_GPIO_PORT, KEY1_INT_GPIO_PIN) == 0) && (skssyswc_flag == 1))
		{
			if ((ad_check()) && IR_flag)
			{
				// delay_ms(20);
				LED1_OFF;
				LED2_ON;
				//				sszcsj_time_count=0;
				skssyswc_flag = 0;
				ssbzw_flag = 1;
				shangsuo_flag = 1;
			}
		}
#endif
		EXTI_ClearITPendingBit(KEY6_INT_EXTI_LINE);
	}
}
extern u8 nb_time_flag;
extern u8 nb_time;
extern u16 xinlv_cnt, xinlv_flag;
extern u8 init_flag, nb_xinlv_tx_flag;

u8 baojing_zitai_flag = 0;
u8 baojing_kakou_flag = 2;
u8 baojing_kakou_total_flag;

u8 baojing_kakou_timer;
u16 baojing_kakou_time_cnt;

u8 key4_edge = 0;
u8 key3_edge = 0;
extern u8 dt_cal_flag;
extern u32 dt_cal_cnt;
u8 init_0_flag = 0;

extern int usart5_rx_flag;
extern int usart5_rx_done;
extern int usart5_rx_done_cnt;
u8 key21_flag = 0;
u8 busy_flag = 0;
u16 busy_cnt = 0;

extern u8 zhuiluo_flag;
extern u8 peizhi[10];
u8 gaoduyuyin_flag;
u16 zhuiluo_buf = 0;

u16 gd_jiesuo_cnt = 0;
extern int height;
extern u16 time_10min_buf;

u8 pilao_flag = 0;
// u8 didianliang_flag = 0;
// u8 bat_lowflag = 0 ;

extern uint8_t fly_mode_cnt;  // 飞行模式计数
extern uint8_t open_fly_mode; // 飞行模式标记
extern u8 nb_time;

void GENERAL_TIM_IRQHandler(void) // 定时器中断
{

	if (TIM_GetITStatus(GENERAL_TIM, TIM_IT_Update) != RESET)
	{

		TIM_ClearITPendingBit(GENERAL_TIM, TIM_FLAG_Update);
		static u16 nb_time_cnt = 0;
		static u16 time_200ms_buf = 0, time_2000ms_buf = 0;
		//		static u16 time_10min_buf  ;

		static u16 time_5min_buf = 0;
		//		static int time_10min_buf = 0 ;
		static int time_8hours_buf = 0;
		static int time_shangsuotimer_buf = 0;
		time_200ms_buf++;
		time_2000ms_buf++;
		static int gaoduyuyin_cnt = 0;
		//	zhuiluo_buf++;

		/***********10.25更新***************************/
		// extern u8 gd_jiesuo_key;//新增gd计时

		//		if (open_fly_mode == 1)
		//		{
		//			if (fly_mode_cnt == 1000)
		//			{
		//				fly_mode_cnt = 0;
		//				open_fly_mode = 0;
		//				nb_time = 10;
		//			}
		//			else
		//			{
		//				fly_mode_cnt++;
		//			}
		//		}

		// if (baojing_kakou_timer)
		// {
		// 	if (baojing_kakou_time_cnt == 800)
		// 	{
		// 		baojing_kakou_time_cnt = 0;

		// 	  extern void waist_buckle_warn();
		// 		waist_buckle_warn();
		// 	}
		// 	else
		// 	{
		// 		baojing_kakou_time_cnt++;
		// 	}
		// }
		// else
		// {
		// 	baojing_kakou_time_cnt = 0;
		// }

		if (zhuiluo_flag == 1)
		{
			zhuiluo_buf++;
			if (zhuiluo_buf == 250)
			{
				zhuiluo_buf = 0;
				zhuiluo_flag = 0;
			}
		}
		if (gd_jiesuo_key && anjianjiaoling_flag == 1 && peizhi[3] == 1)
		{
			gd_jiesuo_cnt++;
		}

		if (gd_jiesuo_cnt >= 1500) // 15miao
		{
			gd_jiesuo_cnt = 0;
			gd_jiesuo_key = 0;
			if (height >= 20)
			{
				key21_flag = 6;
				shangsuo_delay = 1;
				// LED1_OFF;
				// LED2_ON;
				// ssbzw_flag = 1;

				extern void weixiangd(); // 新增危险高度语音
				weixiangd();
				// key1_flag = 1; // 上锁状态
				// ssbzw_flag = 1;
				// chezaikongtiao_calculate();

				// NB_TCP_TX();
				// shangsuo_flag = 1;
			}
		}

		if (shangsuo_delay == 1)
		{
			time_shangsuotimer_buf++;
		}
		if (time_shangsuotimer_buf == 300) // 3s延时 YF修改成2秒
		{
			shangsuo_delay = 0;
			time_shangsuotimer_buf = 0;
			extern u8 motor_flag;
			motor_flag = 1;

			// if (gaoduyuyin_flag == 1 && peizhi[3] == 1)
			// {
			LED1_OFF;
			LED2_ON;
			key1_flag = 1; // 上锁状态
			ssbzw_flag = 1;
			chezaikongtiao_calculate();

			NB_TCP_TX();
			shangsuo_flag = 1;
			// 上锁

		}

		if (gaoduyuyin_flag == 1 && peizhi[3] == 0)
		{
			gaoduyuyin_cnt++;
		}
		if (gaoduyuyin_cnt >= 1000 && shangsuo_flag == 0) // 延迟十秒
		{
			extern void weixiangd(); // 新增危险高度语音
			weixiangd();
			gaoduyuyin_cnt = 0;
		}

		if (gaoduyuyin_flag == 1 && peizhi[3] == 1)
		{
			gaoduyuyin_flag = 0;
			// LED1_OFF;
			// LED2_ON; // 上锁
			ssbzw_flag = 1;
			shangsuo_flag = 1;
		}

		if (busy_flag)
		{
			busy_cnt++;
		}
		if (busy_cnt >= 100 * 60 * 5)
		{
			busy_flag = 0;
			busy_cnt = 0;
		}

		extern void dt_deal();
		extern u8 yy_h_flag;
		extern u8 didian_flag;
		extern u8 xintiao_flag;
		static u16 yy_h_cnt = 0;
		extern u8 yy_val;

		if (xintiao_flag == 0) // 20230111每5分钟上报一次联网指令
		{
			time_5min_buf++;
			if (time_5min_buf >= 30000)
			//		  if(time_5min_buf >= 100)
			{
				xintiao_flag = 1;
				time_5min_buf = 0;
			}
		}
		if (yy_h_flag)
		{
			yy_h_cnt++;
		}
		if (yy_val == 10)
		{
			if (yy_h_cnt > 100 * 2)
			{
				yy_h_cnt = 0;
				yy_h_flag = 0;
			}
		}
		if (yy_h_cnt > 100 * 4)
		{
			yy_h_cnt = 0;
			yy_h_flag = 0;
		}

		if (usart5_rx_done)
		{
			usart5_rx_done_cnt++;
		}

		if (usart5_rx_done_cnt == 5)
		{
			usart5_rx_done = 0;
			usart5_rx_done_cnt = 0;
			usart5_rx_flag = 1;
		}

		if (time_2000ms_buf == 100)
		{
			extern u8 peizhi[10];
			time_2000ms_buf = 0;
			if (peizhi[2] == 1)
			{
				key21_flag = 6;
			}
		}

		if (didian_flag == 1) // 20230110 电量语音报警5min一次
		{
			//		 if(time_10min_buf >= 30000)    //低电量报警  5min
			////			if(time_10min_buf >= 6000)
			//		  {
			//			  extern void yydidianliang(void);
			//			  yydidianliang();
			//				time_10min_buf = 0 ;
			//		  }
			//		 else
			//		  {
			time_10min_buf++;
			//		  }
		}
		if (time_8hours_buf >= 2880000) // 疲劳报警 8hours 2880000
		//		if(time_6hours_buf == 6000)
		{
			if (pilao_flag == 0)
			{
				key4_flag = 30;
				pilao_flag = 1;
				time_8hours_buf = 0;
			}
			pilao_flag = 0;
		}
		else
		{
			time_8hours_buf++;
		}

		if (time_200ms_buf == 100)
		{
			time_200ms_buf = 0;
			//			extern void dianjishangsuo(void);
			//			dianjishangsuo();
			if (baojing_zitai_flag || baojing_kakou_total_flag)
			{
				LED7_TOGGLE;
			}
			else
			{
				LED7_OFF;
			}
		}
		if (!nb_time_flag)
		{
			nb_time_cnt++;
		}
		if (nb_time_cnt == 100)
		{
			nb_time_cnt = 0;
			nb_time_flag = 1;
			// nb_time++;
			if (nb_time >= 8)
				nb_time = 1;
		}

		IR_flag_cnt++;
		if (IR_flag_cnt == 10)
		{
			if (IR_cnt > 3)
			{
				IR_flag = 1;
			}
			else
			{
				IR_flag = 0;
			}
			IR_cnt = 0;
			IR_flag_cnt = 0;
		}
		extern void sendBeforePowerdown();
		sendBeforePowerdown();

		if (Key_Scan(KEY3_INT_GPIO_PORT, KEY3_INT_GPIO_PIN) == 1)
		{
			key3_edge = (key3_edge << 1) | 0x01;
		}
		else
		{
			key3_edge = (key3_edge << 1) & 0xfe;
		}
		if ((key3_edge) == 0x0f)
		{
			count++;
			if (count == 2)
			{
				extern u8 peizhi[10];
				if (peizhi[1] == 1)
				{
					if (Key_Scan(KEY2_INT_GPIO_PORT, KEY2_INT_GPIO_PIN) == 0)
					{
						extern u8 motor_flag;
						motor_flag = 1;
						jj_js_flag = 1;
						LED1_ON;
						LED2_OFF;
						/*
						2020.5.7：修改控制电机
						*/
						//				MOTOR_FZ;
						key4_flag = 1;
						key1_flag = 0;
						jsbzw_flag = 1;
						shangsuo_flag = 0;
					}
				}
				count = 0;
			}
		}

		if (Key_Scan(KEY4_INT_GPIO_PORT, KEY4_INT_GPIO_PIN) == 1)
		{
			key4_edge = (key4_edge << 1) | 0x01;
		}
		else
		{
			key4_edge = (key4_edge << 1) & 0xfe;
		}
		if ((key4_edge) == 0x0f)
		{
			count_1++;
			if (count_1 == 2)
			{
				count_1 = 0;
				key4_flag = 4;
			}
		}
		////////////////////////////////////////////////////////解锁时间计算
		if (skssys_flag == 1)
			skssys_time++;
		if (skssys_time == 900)
		{
			skssys_flag = 0;
			skssyswc_flag = 1;
			skssys_time = 0;
			//	jj_js_flag =1;
		}
		///////////////////////////////////////////////////////长按时间
		if (Key_Scan(KEY4_INT_GPIO_PORT, KEY4_INT_GPIO_PIN) == 1)
			changanshijian++;
		if (changanshijian == 200)
		{
			key4_flag = 2;
			key1_flag = 1;
			changanshijian = 0;
			count_1 = 0;
			shuangjiliangcijishi = 0;
		}
		/////////////////////////////////////按两次按键间隔时间
		if (count == 1)
		{
			shuangjiliangcijishi++;
			if (shuangjiliangcijishi == 100 && (Key_Scan(KEY3_INT_GPIO_PORT, KEY3_INT_GPIO_PIN) == 0))
			{
				count = 0;
				key4_flag = 3;
				key1_flag = 0;
				shuangjiliangcijishi = 0;
			}
			if (shuangjiliangcijishi == 500 && (Key_Scan(KEY3_INT_GPIO_PORT, KEY3_INT_GPIO_PIN) == 1))
			{
				count = 0;
				extern u8 peizhi[10];
				if (peizhi[2] == 1)
				{
					key4_flag = 5;
				}
				shuangjiliangcijishi = 0;
			}
		}
		if (shuangjiliangcijishi > 500)
			shuangjiliangcijishi = 0;
		if (count_1 == 1)
			shuangjiliangcijishi_1++;
		if (shuangjiliangcijishi_1 == 100) // 未双击，执行单击
		{
			count_1 = 0;
			shuangjiliangcijishi_1 = 0;
		}
		/////////////////////////////////////最长运行时间
		if (ssbzw_flag == 1)
			sszcsj_time_count++;
		if (sszcsj_time_count == 200) // 240
		{
			key1_flag = 1;
			ssbzw_flag = 0;
			sszcsj_time_count = 0;
			sszcsj_time_flag = 1;
			extern u8 motor_flag;
			motor_flag = 0;
			LED1_OFF;
			LED2_OFF;
		}
		if (jsbzw_flag == 1)
			jszcsj_time_count++;
		if (time < jszcsj_time_count)
			time = jszcsj_time_count;
		if (time < sszcsj_time_count)
			time = sszcsj_time_count;
		if (jszcsj_time_count == 200) // yf 解锁时间改2S
		{
			jsbzw_flag = 0;
			jszcsj_time_count = 0;
			extern u8 motor_flag;
			motor_flag = 0;
			LED1_OFF;
			LED2_OFF;
			key1_flag = 0;
			sbjsbz_flag = 1;
			skssys_flag = 1;
			skssys_time = 0;
			skssyswc_flag = 0;
		}
		//////////////////////////////////////
		//		time_fasongjishu_count++;
		//		if(time_fasongjishu_count==1000)
		//		{
		//			time_fasongjishu_count=0;
		//			shujufsflag=1;
		//		}
		if (key1_flag == 1)
			abc_timers_count++;
		else
			abc_timers_count = 0;
		if (abc_timers_count == 100)
			shangsuo_time++;
		/////////////////////////////////////
	}
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
