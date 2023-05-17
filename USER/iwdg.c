#include "stm32f10x.h"
#include "iwdg.h"
//看门狗初始化，参数：prer-分频，reld-计数器重装载值Tout=((4×2^prer) ×rlr) /40
//prer=4 reld=625 Tout=1000ms
void WatchDog_Init(u8 prer, u16 reld)
{
   IWDG->KR=0x5555; //允许访问PR和RLR寄存器
   IWDG->PR=prer;  //设置分频
   IWDG->RLR=reld; //设定计数器初值
   IWDG->KR=0xaaaa; //初次装初值
   IWDG->KR=0xcccc;  //启动看门狗定时器
}

//喂狗
void WatchDog_Feed(void)
{
 IWDG->KR=0xaaaa;
}
