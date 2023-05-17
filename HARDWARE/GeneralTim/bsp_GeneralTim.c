#include "bsp_GeneralTim.h" 
#include "nb_model.h"
#include "bsp_nb.h"
#include "sys.h"
#include "rl_control.h"
#include "delay.h"
#include "iwdg.h"
#include "common.h"
#include "ble.h"
//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
#define AD_N 100
u8 ad_num;
u8 ad_done_flag;

extern u8 powerdown_flag;
extern u16 ADC_Convert[3];
extern float voltage_value[3];
//应用示例：TIM2_PWM_Init(99,18);
void TIM2_PWM_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//使能定时器2时钟
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟，必须先开启辅助功能时钟
	
	//设置该引脚为复用输出功能,输出TIM2 CH2的PWM脉冲波形	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出 注意
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
	
	//初始化TIM2
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM2 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2 ？？？ 配置为PWM模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	//新增
	TIM_OCInitStructure.TIM_Pulse = 75;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变//86(84~90) //91(90~92)   //60(5~86) //60(10~86)
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高 当定时器计数值小于pulse时为高电平
	
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM2 OC1 使能通道1
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM2在CCR2上的预装载寄存器
 
//  TIM_OC2Init(TIM4, &TIM_OCInitStructure);	 //使能通道2
//  TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
//	
//  TIM_OC3Init(TIM4, &TIM_OCInitStructure);	 //使能通道3
//  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
//  
//  TIM_OC4Init(TIM4, &TIM_OCInitStructure);	 //使能通道3
//  TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
 
	TIM_ARRPreloadConfig(TIM2, ENABLE);			 // 使能TIM2重载寄存器ARR ？？？
  
	TIM_Cmd(TIM2, ENABLE);  //使能TIM2
}


//int main(void)
//{
//	......
//	TIM2_PWM_Init(99,18);//在PA0产生38KHz波特率
//	//在主函数里面根据需要加上设置占空比，也可以不加上；
//	TIM_SetCompare1(TIM2,50);		 
//	.......
//}

void Timerx_Init()
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = 10; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值，计数到5000为500ms
    TIM_TimeBaseStructure.TIM_Prescaler =(720-1); //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
     
    TIM_ITConfig(  //使能或者失能指定的TIM中断
        TIM4, //TIM2
        TIM_IT_Update  |  //TIM 中断源
        TIM_IT_Trigger,   //TIM 触发中断源 
        ENABLE  //使能
        );
     
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

    TIM_Cmd(TIM4, ENABLE);  //使能TIMx外设
                             
}
void Timer1_Init()
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = 100; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值，计数到5000为500ms
    TIM_TimeBaseStructure.TIM_Prescaler =(72-1); //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);//清中断标志位
     
    TIM_ITConfig(  //使能或者失能指定的TIM中断
        TIM1, //TIM1
        TIM_IT_Update  |  //TIM 中断源
        TIM_IT_Trigger,   //TIM 触发中断源 
        ENABLE  //使能
        );
     
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM3中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

    TIM_Cmd(TIM1, ENABLE);  //使能TIMx外设
                             
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
		// 开启定时器时钟,即内部时钟CK_INT=72M
    GENERAL_TIM_APBxClock_FUN(GENERAL_TIM_CLK, ENABLE);	
		// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Period=GENERAL_TIM_Period;
	  // 时钟预分频数
    TIM_TimeBaseStructure.TIM_Prescaler= GENERAL_TIM_Prescaler;	
		// 时钟分频因子 ，没用到不用管
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
		// 计数器计数模式，设置为向上计数
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 		
		// 重复计数器的值，没用到不用管
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	  // 初始化定时器
    TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);
		// 清除计数器中断标志位
    TIM_ClearFlag(GENERAL_TIM, TIM_FLAG_Update);
		// 开启计数器中断
    TIM_ITConfig(GENERAL_TIM,TIM_IT_Update,ENABLE);
		// 使能计数器
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
void TIM4_IRQHandler(void)   //TIM4中断
{
		
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
    {
				TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
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

void TIM1_UP_IRQHandler(void)   //TIM1中断 控制TIM2生成PWM波
{
		
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
    {
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		
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
