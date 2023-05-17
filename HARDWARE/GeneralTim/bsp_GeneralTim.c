#include "bsp_GeneralTim.h" 
#include "nb_model.h"
#include "bsp_nb.h"
#include "sys.h"
#include "rl_control.h"
#include "delay.h"
#include "iwdg.h"
#include "common.h"
#include "ble.h"
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
#define AD_N 100
u8 ad_num;
u8 ad_done_flag;

extern u8 powerdown_flag;
extern u16 ADC_Convert[3];
extern float voltage_value[3];
//Ӧ��ʾ����TIM2_PWM_Init(99,18);
void TIM2_PWM_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//ʹ�ܶ�ʱ��2ʱ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ�ӣ������ȿ�����������ʱ��
	
	//���ø�����Ϊ�����������,���TIM2 CH2��PWM���岨��	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //����������� ע��
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO
	
	//��ʼ��TIM2
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM2 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2 ������ ����ΪPWMģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	//����
	TIM_OCInitStructure.TIM_Pulse = 75;	   //��������ֵ�������������������ֵʱ����ƽ��������//86(84~90) //91(90~92)   //60(5~86) //60(10~86)
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը� ����ʱ������ֵС��pulseʱΪ�ߵ�ƽ
	
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM2 OC1 ʹ��ͨ��1
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIM2��CCR2�ϵ�Ԥװ�ؼĴ���
 
//  TIM_OC2Init(TIM4, &TIM_OCInitStructure);	 //ʹ��ͨ��2
//  TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
//	
//  TIM_OC3Init(TIM4, &TIM_OCInitStructure);	 //ʹ��ͨ��3
//  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
//  
//  TIM_OC4Init(TIM4, &TIM_OCInitStructure);	 //ʹ��ͨ��3
//  TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
 
	TIM_ARRPreloadConfig(TIM2, ENABLE);			 // ʹ��TIM2���ؼĴ���ARR ������
  
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIM2
}


//int main(void)
//{
//	......
//	TIM2_PWM_Init(99,18);//��PA0����38KHz������
//	//�����������������Ҫ��������ռ�ձȣ�Ҳ���Բ����ϣ�
//	TIM_SetCompare1(TIM2,50);		 
//	.......
//}

void Timerx_Init()
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = 10; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ��������5000Ϊ500ms
    TIM_TimeBaseStructure.TIM_Prescaler =(720-1); //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
     
    TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
        TIM4, //TIM2
        TIM_IT_Update  |  //TIM �ж�Դ
        TIM_IT_Trigger,   //TIM �����ж�Դ 
        ENABLE  //ʹ��
        );
     
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

    TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx����
                             
}
void Timer1_Init()
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = 100; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ��������5000Ϊ500ms
    TIM_TimeBaseStructure.TIM_Prescaler =(72-1); //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);//���жϱ�־λ
     
    TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
        TIM1, //TIM1
        TIM_IT_Update  |  //TIM �ж�Դ
        TIM_IT_Trigger,   //TIM �����ж�Դ 
        ENABLE  //ʹ��
        );
     
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM3�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

    TIM_Cmd(TIM1, ENABLE);  //ʹ��TIMx����
                             
}
static void GENERAL_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM_IRQ ;	
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1;	 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void GENERAL_TIM_Mode_Config(void)
{
	  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;		
		// ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
    GENERAL_TIM_APBxClock_FUN(GENERAL_TIM_CLK, ENABLE);	
		// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
    TIM_TimeBaseStructure.TIM_Period=GENERAL_TIM_Period;
	  // ʱ��Ԥ��Ƶ��
    TIM_TimeBaseStructure.TIM_Prescaler= GENERAL_TIM_Prescaler;	
		// ʱ�ӷ�Ƶ���� ��û�õ����ù�
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
		// ����������ģʽ������Ϊ���ϼ���
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 		
		// �ظ���������ֵ��û�õ����ù�
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	  // ��ʼ����ʱ��
    TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);
		// ����������жϱ�־λ
    TIM_ClearFlag(GENERAL_TIM, TIM_FLAG_Update);
		// �����������ж�
    TIM_ITConfig(GENERAL_TIM,TIM_IT_Update,ENABLE);
		// ʹ�ܼ�����
    TIM_Cmd(GENERAL_TIM, ENABLE);
}

void GENERAL_TIM_Init(void)
{
	GENERAL_TIM_NVIC_Config();
	GENERAL_TIM_Mode_Config();	
//	Timerx_Init();
//	Timer1_Init();
}
uint16_t value_buf[AD_N];
uint16_t filter(){
       
        uint16_t i,j,temp;
        for(j=0;j<AD_N-1;j++){
                for(i=0;i<AD_N-j-1;i++){
                        if(value_buf[i]>value_buf[i+1]){
                                temp =value_buf[i];
                                value_buf[i]= value_buf[i+1];
                                value_buf[i+1]= temp;
                        }
                }
        }
        return value_buf[(AD_N-1)/2];
}
extern uint8_t powerdown_flag;
u8 powerdown_anjian = 0;
u8 guanji_yy_flag = 0;

int get_adc0_val()
{
	voltage_value[0]=3.3*((float)ADC_Convert[0]/4096);
	if(powerdown_flag==0){
		if(voltage_value[0]<0.3){
			
			return 0;
		}else{
			return 1;
		}
	}
}
void sendBeforePowerdown(){
	static u8 power_key_flag = 0;
	static u16 power_cnt = 0, power_cnt1 = 0;

	static u8 powerdown_flag1 = 0;
	voltage_value[0]=3.3*((float)ADC_Convert[0]/4096);
	if(powerdown_flag==0){
		if(voltage_value[0]<0.3){
			
			power_key_flag = (power_key_flag<<1)&0xfe;
		}else{
			if(powerdown_flag1 > 1)
				power_cnt++;
			power_key_flag = (power_key_flag<<1)|0x01;
		}
		if((power_key_flag)==0x0f)
		{
			
			powerdown_flag1 ++;
			if(powerdown_flag1 ==3)
				powerdown_flag1 =2;
		}
		if(powerdown_flag1 > 1){
			if((power_key_flag&0xc3)==0xc3)
			{
				if(power_cnt == 250 && powerdown_anjian == 0){
					powerdown_anjian = 1;
					power_cnt1 = 0;
					power_cnt = 0;
				}
			}if((power_key_flag&0xc3)==0xc0)
			{
				if(powerdown_anjian == 0)
				{
					power_cnt = 0;
				}	
			}
			if(powerdown_anjian){//&&guanji_yy_flag){
				power_cnt1++;
			}
			if(power_cnt1 >= 180)
			{
				/*send POWERDOWN message */
				powerdown_flag=1;
			}
		}
	}	
}
void TIM4_IRQHandler(void)   //TIM4�ж�
{
		
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
    {
				TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
//				WatchDog_Feed();
				if(ad_done_flag==0&&ad_num<AD_N){
                value_buf[ad_num]=ADC_Convert[1];
								ad_num++;
				}
				if(ad_num==AD_N){
					ad_done_flag=1;
					ad_num=0;
				}
        sendBeforePowerdown();
#ifdef ME3630
		NB_Solve();
#endif
    }
}

void TIM1_UP_IRQHandler(void)   //TIM1�ж� ����TIM2����PWM��
{
		
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
    {
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		
		static u16 ir_flag1=0,ir_cnt1=0,ir_flag2=0,ir_cnt2=0;
		static u16 ir_1=10,ir_2=10;
		if(ir_flag2==50){
			TIM_Cmd(TIM2,DISABLE);
			ir_cnt2++;
			if(ir_cnt2==200){
				ir_flag2=0;
				ir_cnt2=0;
			}
		}else{
			if(ir_flag1==0){
				ir_cnt1++;
				if(ir_cnt1==2){
					ir_1++;
					ir_cnt1=0;
					ir_flag1=1;
					TIM_Cmd(TIM2,DISABLE);
				}
			}else{
				ir_cnt1++;
				if(ir_cnt1==10){
					ir_2++;
					ir_cnt1=0;
					ir_flag1=0;
					TIM_Cmd(TIM2,ENABLE);
					ir_flag2++;
					
				}
			}
		
		}

		
    }
}
