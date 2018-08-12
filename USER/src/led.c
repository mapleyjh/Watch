#include "stm32f4xx.h"
#include "led.h"
void led_init(void)
{
	
	RCC->AHB1ENR |= 1<<0;			    //PA�˿�ʱ��ʹ�ܡ� ������
	
	GPIOA->MODER &= ~(3 << 14); 	//32bit,����
	GPIOA->MODER |= 1 << 14;    	//����PA7 IO�ڣ�Ϊͨ���������
	
	GPIOA->OTYPER &= ~(1 << 7);	  //����PA7Ϊ�������
	
	GPIOA->OSPEEDR &= ~(3 << 14);
	GPIOA->OSPEEDR |= 2 << 14;  	//����PA7������ٶ� 50MHZ
	
	
	GPIOA->ODR |= 1<<7;			      //PA7����ߵ�ƽ	
}