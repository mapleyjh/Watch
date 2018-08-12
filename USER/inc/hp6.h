/***********************************************************************************************************************
Copyright 2008 - 2016 ��������ӯ��������޹�˾. All rights reserved.
�ļ���:        hp6.h
����   :       ����Ѫѹģ�������ļ�
����   :       Jahol Fan
�汾   :       V1.0
�޸�   :   
������ڣ�     2016.10.13
Notice    :������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
��ӯ�������http://www.edu118.com/
��ӯ����У��http://www.edu118.cn/
��Ȩ���У�����ؾ���
************************************************************************************************************************/


// IIC ����ʹ��100K��7bite��ַ��0x66

#ifndef HP_6_PROTOCOL_H
#define HP_6_PROTOCOL_H
#include "stm32f4xx.h"
#define HP_6_FAILURE			0x00
#define HP_6_SUCCESS			0x01
void Hp_6_init(void);
void Hp_6_PowerON(void);
void Hp_6_PowerOFF(void);
uint16_t HP_6_GetResultData(uint8_t *data);

uint8_t HP_6_GetRateResult(void);
uint8_t HP_6_OpenRate(void);
uint8_t HP_6_CloseRate(void);

uint8_t HP_6_GetBpResult(void);
uint8_t HP_6_OpenBp(void);
uint8_t HP_6_CloseBp(void);

uint8_t HP_6_GetADC(uint8_t num, uint8_t id);

uint8_t HP_6_PowerSaving(void);

uint8_t HP_6_VersionInfo(void);

#define Hp_6_PowerON()           GPIOB->ODR &= ~(1<<15)			  //PB15����͵�ƽ
#define Hp_6_PowerOFF()      		 GPIOB->ODR |=  (1<<15)			  //PB15����ߵ�ƽ


#endif

