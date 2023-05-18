#include "bsp_key.h"
#include "delay.h"
static void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	NVIC_InitStructure.NVIC_IRQChannel = KEY1_INT_EXTI_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = KEY2_INT_EXTI_IRQ;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = KEY3_INT_EXTI_IRQ;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = KEY5_INT_EXTI_IRQ;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = KEY6_INT_EXTI_IRQ;
	NVIC_Init(&NVIC_InitStructure);
}
void Input_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE); // ʹ��ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // ���ó����룬Ĭ�ϸ���
	GPIO_Init(GPIOC, &GPIO_InitStructure);		  // ��ʼ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // ���ó����룬Ĭ�ϸ���
	GPIO_Init(GPIOB, &GPIO_InitStructure);		  // ��ʼ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_12 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // ���ó����룬Ĭ�ϸ���
	GPIO_Init(GPIOA, &GPIO_InitStructure);		  // ��ʼ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // ���ó����룬Ĭ�ϸ���
	GPIO_Init(GPIOC, &GPIO_InitStructure);		  // ��ʼ��

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource6);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource7);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource12);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource15);
	EXTI_InitTypeDef EXTI_InitStructure;																						// �����ʼ���ṹ��
	EXTI_InitStructure.EXTI_Line = EXTI_Line1 | EXTI_Line4 | EXTI_Line6 | EXTI_Line7 | EXTI_Line12 | EXTI_Line14 | EXTI_Line15; // �ж��ߵı�� ȡֵ��ΧΪEXTI_Line0~EXTI_Line15
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;																			// �ж�ģʽ����ѡֵΪ�ж� EXTI_Mode_Interrupt ���¼� EXTI_Mode_Event��
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;																		// ������ʽ���������½��ش��� EXTI_Trigger_Falling�������ش��� EXTI_Trigger_Rising�����������ƽ�������غ��½��أ�����EXTI_Trigger_Rising_Falling
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); // ���ݽṹ����Ϣ���г�ʼ��

	NVIC_InitTypeDef NVIC_InitStructure;						 // ����ṹ��
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			 // ʹ���ⲿ�ж����ڵ�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; // ��ռ���ȼ� 2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;		 // �����ȼ� 2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 // ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);								 // ���ݽṹ����Ϣ�������ȼ���ʼ��

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			 // ʹ���ⲿ�ж����ڵ�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; // ��ռ���ȼ� 2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;		 // �����ȼ� 2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 // ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);								 // ���ݽṹ����Ϣ�������ȼ���ʼ��

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			 // ʹ���ⲿ�ж����ڵ�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; // ��ռ���ȼ� 2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x05;		 // �����ȼ� 2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 // ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);								 // ���ݽṹ����Ϣ�������ȼ���ʼ��

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;		 // ʹ���ⲿ�ж����ڵ�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; // ��ռ���ȼ� 2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x07;		 // �����ȼ� 2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 // ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);								 // ���ݽṹ����Ϣ�������ȼ���ʼ��
}
void Key_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(KEY1_INT_GPIO_CLK | KEY2_INT_GPIO_CLK | KEY3_INT_GPIO_CLK | KEY4_INT_GPIO_CLK | KEY5_INT_GPIO_CLK | KEY6_INT_GPIO_CLK, ENABLE);
	NVIC_Configuration();

	GPIO_InitStructure.GPIO_Pin = KEY1_INT_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(KEY1_INT_GPIO_PORT, &GPIO_InitStructure);

	// GPIO_EXTILineConfig(KEY1_INT_EXTI_PORTSOURCE, KEY1_INT_EXTI_PINSOURCE);
	EXTI_InitStructure.EXTI_Line = KEY1_INT_EXTI_LINE;

	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	GPIO_InitStructure.GPIO_Pin = KEY2_INT_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(KEY2_INT_GPIO_PORT, &GPIO_InitStructure);

	// GPIO_EXTILineConfig(KEY2_INT_EXTI_PORTSOURCE, KEY2_INT_EXTI_PINSOURCE);
	EXTI_InitStructure.EXTI_Line = KEY2_INT_EXTI_LINE;

	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	GPIO_InitStructure.GPIO_Pin = KEY3_INT_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(KEY3_INT_GPIO_PORT, &GPIO_InitStructure);

	// GPIO_EXTILineConfig(KEY3_INT_EXTI_PORTSOURCE, KEY3_INT_EXTI_PINSOURCE);
	EXTI_InitStructure.EXTI_Line = KEY3_INT_EXTI_LINE;

	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	GPIO_InitStructure.GPIO_Pin = KEY4_INT_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(KEY4_INT_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = KEY5_INT_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(KEY5_INT_GPIO_PORT, &GPIO_InitStructure);

	// GPIO_EXTILineConfig(KEY5_INT_EXTI_PORTSOURCE, KEY5_INT_EXTI_PINSOURCE);
	EXTI_InitStructure.EXTI_Line = KEY5_INT_EXTI_LINE;

	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	GPIO_InitStructure.GPIO_Pin = KEY6_INT_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(KEY6_INT_GPIO_PORT, &GPIO_InitStructure);

	// GPIO_EXTILineConfig(KEY6_INT_EXTI_PORTSOURCE, KEY6_INT_EXTI_PINSOURCE);
	EXTI_InitStructure.EXTI_Line = KEY6_INT_EXTI_LINE;

	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

uint8_t Key_Scan(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	if (GPIO_Pin == KEYD0_INT_GPIO_PIN || KEYD0_INT_GPIO_PIN == KEYD1_INT_GPIO_PIN)
	{
		if (GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == KEY_OFF)
		{
			delay_ms(20);
			if (GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == KEY_OFF)
			{
				return KEY_ON;
			}
		}
		else
			return KEY_OFF;
	}
	else
	{
		if (GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == KEY_OFF)
		{

			return KEY_ON;
		}
		else
			return KEY_OFF;
		/* code */
	}
}
