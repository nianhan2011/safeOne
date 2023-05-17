#include "stm32f103x.h"


void pwm_init(){
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE); 

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP; //�����������
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	TIM_TimeBaseInitStructure.TIM_Period = 100;//�����Զ���װ�ؼĴ������ڵ�ֵ

	TIM_TimeBaseInitStructure.TIM_Prescaler = 3000;//����������ΪTIMxʱ��Ƶ��Ԥ��Ƶֵ
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, & TIM_TimeBaseInitStructure);

	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//PWM���ʹ��
	//TIM_OCInitStructure.TIM_OutputState=TIM_OutputNState_Disable; //PWM���ʹ��
	TIM_OCInitStructure.TIM_Pulse=10;
	//TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	// TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Set;
	//TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCNIdleState_Reset;
	TIM_OC3Init(TIM1,&TIM_OCInitStructure);


	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=10;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Set;
	TIM_OC4Init(TIM1,&TIM_OCInitStructure);

	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);

	TIM_Cmd(TIM1,ENABLE);//

	//��������Ǹ߼���ʱ�����У����PWM�����

	TIM_CtrlPWMOutputs(TIM1, ENABLE); //pwm���

}