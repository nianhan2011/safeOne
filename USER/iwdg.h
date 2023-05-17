#ifndef IWDG_H
#define IWDG_H
#include "stm32f10x.h"
void WatchDog_Init(u8, u16);//申明看门狗的初始化函数
void WatchDog_Feed(void);//申明喂狗的函数
#endif
