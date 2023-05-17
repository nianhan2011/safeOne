#include "stm32f10x.h"
#include "iwdg.h"
//���Ź���ʼ����������prer-��Ƶ��reld-��������װ��ֵTout=((4��2^prer) ��rlr) /40
//prer=4 reld=625 Tout=1000ms
void WatchDog_Init(u8 prer, u16 reld)
{
   IWDG->KR=0x5555; //�������PR��RLR�Ĵ���
   IWDG->PR=prer;  //���÷�Ƶ
   IWDG->RLR=reld; //�趨��������ֵ
   IWDG->KR=0xaaaa; //����װ��ֵ
   IWDG->KR=0xcccc;  //�������Ź���ʱ��
}

//ι��
void WatchDog_Feed(void)
{
 IWDG->KR=0xaaaa;
}
