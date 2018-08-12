#include "stm32f4xx.h"
#include "timer.h"
void TIM3_IRQHandler(void)
{	

	if(TIM3->SR&0X0001)//����ж�
	{
           GPIOA->ODR	^= 1 << 7; //ÿ��һ�ΰ�������ȡ��LED�Ƶ�״̬

	}				   
	TIM3->SR&=~(1<<0);//����жϱ�־λ		
}
//ͨ�ö�ʱ��3�жϳ�ʼ��,��ʱ����2/3/4/5��ʱ��Ϊ50M�⣬����Ϊ100M
void TIM3_init(u16 arr,u16 psc)
{
	u32 pro;
  RCC->APB1ENR|=1<<1; //TIM3ʱ��ʹ��	  
  TIM3->ARR=arr;		//�趨�������Զ���װֵ 
  TIM3->PSC=psc;		//Ԥ��Ƶ��	 
  TIM3->CNT = 0;        //Jahol Fan ���³�ʼ���ͻ����¼���
  TIM3->DIER|=1<<0;	//��������ж�	  
  TIM3->CR1|=0x01;	//ʹ�ܶ�ʱ��3
	//��ռ���ȼ�2bit  �����ȼ�2bit
	NVIC_SetPriorityGrouping(5);//�����ж����ȼ�����
	pro = NVIC_EncodePriority(5, 2, 0);//��ռ���ȼ��ļ���2�� �������ȼ�����0��
	NVIC_SetPriority(TIM3_IRQn, pro);//�������ȼ�

	
	NVIC_EnableIRQ(TIM3_IRQn);//NVIC�ж�ͨ��ʹ�ܡ�							 
}
void TIM2_initPWM(u32 arr,u32 psc)
{	
  RCC->APB1ENR |= 1<<0;   //TIM2ʱ��ʹ��  
  RCC->AHB1ENR|=1<<1;   	//ʹ��PORTBʱ��	
	
	GPIOB->MODER &= ~(3<<20);
	GPIOB->MODER |= (2<<20);	//PB10  ���ù���
	
	
	GPIOB->AFR[1] &= ~(0XF<<8);
	GPIOB->AFR[1] |= (0X1<<8);// AF1 ����
  
  TIM2->ARR=arr;			//�趨�������Զ���װֵ 
  TIM2->PSC=psc;			//Ԥ��Ƶ������Ƶ 
  
	TIM2->CCR3  = 250;  //PWM��ֵ��ƽ�仯ֵ  
	
	TIM2->CCMR2 &= ~(3<<0);// CH3  ���ģʽ  
	
	TIM2->CCMR2 &= ~(7<<4);  // CH3  PWM1 ģʽ  
	TIM2->CCMR2 |= 6<<4;  // CH3  PWM1 ģʽ     
	TIM2->CCMR2|=1<<3; 	//CH3 Ԥװ��ʹ��
	TIM2->CCER  |= 1<<8;      // CH3  ͨ��ʹ��
	TIM2->CCER  &=~(1<<9);   	//CH3  �ߵ�ƽ��Ч
	TIM2->CR1 |= 1;		//������ʹ��
  
} 