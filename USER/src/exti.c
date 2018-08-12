#include "exti.h"
#include "stm32f4xx.h"
#include "delay.h"


void init_exti0()//PA0 <---> EXTI0
{
	RCC->AHB1ENR |= 1<<0;//ʹ�� PA�� ʱ��
	RCC->APB2ENR |= 1<<14;//ʹ�� SYSCFG ʱ�ӡ�
	
	SYSCFG->EXTICR[0] &= ~0XF;
	SYSCFG->EXTICR[0] |= 0X00;//0000  ���� PA0 --- EXTI0
	
	EXTI->RTSR |= 1<<0; //EXTI0 �����½��ش�����
	EXTI->IMR |= 1<<0;  //���� EXTI0 �ж�����
	
	NVIC_EnableIRQ(EXTI0_IRQn);// ʹ�� NVIC ��Ӧ���ж�ͨ����
}


void EXTI0_IRQHandler(void)
{
	EXTI->PR = 1<<0; //���LINE0�ϵ��жϱ�־λ ,EXTI �жϹ���λ��д1�����㡣
  delay_ms(10);//ȥ����
	if((GPIOA->IDR & 1<<0) && 1)
	{
	  GPIOA->ODR	^= 1 << 7; //ÿ��һ�ΰ�������ȡ��LED�Ƶ�״̬
	}
	
}
