#ifndef __KEY_H__
#define __KEY_H__

//ͷ�ļ��� ���������������������궨�塢�������Ͷ��塣

#include "stm32f4xx.h"

#define KEY ((GPIOA->IDR & 1<<0) && 1)  //PA0  ����Ч


void delay(u32 t);
void key_init();
u8 key_scan(void);


#endif
