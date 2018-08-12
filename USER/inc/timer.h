#ifndef _TIMER_H
#define _TIMER_H
#include "stm32f4xx.h"
extern void TIM3_init(u16 arr,u16 psc);
extern void TIM2_initPWM(u32 arr,u32 psc);
#endif