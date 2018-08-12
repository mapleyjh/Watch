
#include "UART.h"

u8 uart_rx_buf[20];//���ڽ��ջ�������
u8 uart_rx_len = 0; //ָʾ����ǰ���ջ��������յ��˼������ݡ�   

void init_uart1()
{
	float DIV_Fraction;
	u16 DIV_Mantissa;
	u32 pro;

	RCC->AHB1ENR |= 1; //PAʱ��ʹ��
	
	//PA9   PA10
	GPIOA->MODER &= ~(3<<18);
	GPIOA->MODER |= (2<<18); //PA9 ���ù���  TX
	
	GPIOA->AFR[1] &= ~(0XF << 4);   	//�μ� ͼ18  �Ĵ�������
	GPIOA->AFR[1] |= 7 << 4;  		//PA9  uart1����    AF7
	
	GPIOA->MODER &= ~(3<<20);
	GPIOA->MODER |= (2<<20); //PA10 ���ù��� RX 
	
	GPIOA->AFR[1] &= ~(0XF << 8);
	GPIOA->AFR[1] |= 7 << 8;  //PA10  uart1����   AF7
	
	
	RCC->APB2ENR |= 1<<4; //ʹ��usart1 ʱ��
	
	DIV_Fraction = (float)100000000/9600/16; //���㲨����  
	DIV_Mantissa = (u16)DIV_Fraction;
	DIV_Fraction = DIV_Fraction - DIV_Mantissa;
	DIV_Mantissa = (DIV_Mantissa << 4) + DIV_Fraction * 16;
	
	USART1->BRR = DIV_Mantissa;//���ò����ʼĴ���
	
	USART1->CR1 |= 1<<2;//ʹ�ܽ���
	USART1->CR1 |= 1<<3;//ʹ�ܷ���
	USART1->CR1 |= 1<<13;//ʹ�ܴ���
	
	USART1->CR1 |= 1<<5;//����1 �����ж�ʹ�ܡ�  ����
	
	//��ռ���ȼ�2bit  �����ȼ�2bit
	NVIC_SetPriorityGrouping(5);//�����ж����ȼ�����
	pro = NVIC_EncodePriority(5, 3, 0);//��ռ���ȼ��ļ���3�� �������ȼ�����0��
	NVIC_SetPriority(USART1_IRQn, pro);//�������ȼ�
//	NVIC->IP[37] = 0xc0;
	
	NVIC_EnableIRQ(USART1_IRQn);//NVIC�ж�ͨ��ʹ�ܡ�
}


//�����һ���ֽں���
u8 rev_byte()
{
	while ( (USART1->SR & 1<<5) == 0 )
	{
		;//�ȴ� ���ջ������ǿա�
	}
	
	return USART1->DR; //�������ݡ�
}


//����һ���ֽں���
void send_byte(u8 val)
{
	while ( (USART1->SR & 1<<6) == 0 )
	{
		;//�ȴ��������
	}
	
	USART1->DR = val; //��������
}


void Uart1_sendData(u8 *dataBuf,u8 lenth)
{
  while (lenth--)
  {
    send_byte(*dataBuf);
    dataBuf++;
  }
}
void USART1_IRQHandler(void)
{
  static u8 cnt = 0;

  //�ж����������жϺ���������Ҫ����״̬�Ĵ������ж�
  if (USART1->SR & 1<<5)//���ջ������ǿ�
  {
    uart_rx_buf[cnt] = USART1->DR;//��ȡ�����յ�������
    cnt++;
    if(5 == cnt)  //���յ�5���ֽڵ����ݣ��ͽ�������Э�����
    {
      uart_rx_len = cnt;
      Uart1_sendData(uart_rx_buf,5);  //���յ�5���ֽھͻط�
      cnt = 0;
    }
  }

}








