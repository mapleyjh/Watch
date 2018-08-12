/*******************************************************************************
Copyright 2008 - 2016 ��������ӯ��Ƽ����޹�˾. All rights reserved.
�ļ���:        sht20.c
����   :       ��ȡ��ʪ�ȴ�����sht20��ֵ
����   :       Jahol  Fan
�汾   :       V1.0
�޸�   :       none
������ڣ�     2016.8.25
*******************************************************************************/

/*******************************************************************************
 * INCLUDES
 */
#include "iic.h"  
#include "delay.h"
#include "sht20.h"
/*******************************************************************************
 * CONSTANTS
 */
#define SHT20ADDR       0x80         //SHT20д��ַ
#define ACK             0            //Ӧ���ź�
#define NACK            1            //��Ӧ���ź�
#define FAIL            1            //��ȡ��ʪ��ʧ�ܵķ���ֵ��
#define READ_TEMP_COMD  0xf3         //��ȡ�¶�����
#define READ_HUMI_COMD  0xf5         //��ȡʪ������
/*******************************************************************************
 * TYPEDEFS
 */


/*******************************************************************************
 * LOCAL VARIABLES
 */
 TempHumiValue_t temp_humi_Value;

/*******************************************************************************
 * LOCAL FUNCTIONS
 */
static void SHT20_setResolution(void);
 static void SHT20_setResolution(void);
 void SHT20_softReset(void);
/*******************************************************************************
 * PROFILE CALLBACKS
 */



/*******************************************************************************
 * PUBLIC FUNCTIONS
 */
  /*******************************************************************************
 * ��������SHT20_writeOneByte
 * ����������дһ���ֽ�
 * ���ߣ�Jahol Fan  
 * ����˵����  дһ���ֽڵ�����
 * ����ֵ˵�������ص���Ӧ���ź�
 * �޸ļ�¼��
*******************************************************************************/
static u8 SHT20_writeOneByte(u8 dataToWrite)
{
	u8 ack; 
	IIC_sendByte(dataToWrite);     //�����ֽ�	
	ack = IIC_waitAck(); 
	return ack;
}
 /*******************************************************************************
 * ��������SHT20_softReset
 * ����������SHT20�����λ���������е���
 * ���ߣ�Jahol Fan  
 * ����˵����  
 * ����ֵ˵����
 * �޸ļ�¼��
*******************************************************************************/
static void SHT20_softReset(void)                    
{
	IIC_start();                      //start I2C
	SHT20_writeOneByte(SHT20ADDR&0xfe);  //I2C address + write
	SHT20_writeOneByte(0xfe);                         //soft reset
	IIC_stop();                       //stop I2C
}
 /*******************************************************************************
 * ��������SHT20_Init
 * ������������ʼ��iic����
 * ���ߣ�Jahol Fan  
 * ����˵����  
 * ����ֵ˵�����ɹ����ز����Ľ����ʧ�ܷ��� 1
 * �޸ļ�¼��
*******************************************************************************/
 void SHT20_Init(void)
{
	SHT20_softReset();
}
 /*******************************************************************************
 * ��������SHT20_readOneByte
 * ������������һ���ֽ�
 * ���ߣ�Jahol Fan  
 * ����˵����  
 * ����ֵ˵����
 * �޸ļ�¼��
*******************************************************************************/
static u8 SHT20_readOneByte(u8 ack)
{
	u8 temp;
	temp=IIC_readByte(ack);	    
	return temp;
}


/*******************************************************************************
 * ��������SHT20_setResolution
 * ����������д�Ĵ���������֡��
 * ���ߣ�Jahol Fan  
 * ����˵����  
 * ����ֵ˵����
 * �޸ļ�¼��
*******************************************************************************/
static void SHT20_setResolution(void)                    
{
  IIC_start();                                                                        //Start I2C 
  if(SHT20_writeOneByte(SHT20ADDR&0xfe)==ACK)             //I2C address + write + ACK
  {
   if(SHT20_writeOneByte(0xe6)==ACK)                               //д�û��Ĵ���
    {  
     if(SHT20_writeOneByte(0x83)==ACK);    //���÷ֱ���   11bit RH% ����ʱ�䣺12ms(typ.) & 11bit T�� ����ʱ�䣺9ms(typ.) 
    }
  }
   IIC_stop();                                  //Stop I2C         
}
/*******************************************************************************
 * ��������SHT20_readTemOrHum
 * ������������ȡSHT20���¶Ȼ��ȡʪ�Ȳ���ֵ  
 * ���ߣ�Jahol Fan  
 * ����˵����  
 * ����ֵ˵�����ɹ����ز����Ľ����ʧ�ܷ��� 1
 * �޸ļ�¼��
*******************************************************************************/
float SHT20_readTemOrHum(u8 commod)
{
	float temp;                 //�¶�
	u8 ack=1;
	u8  MSB,LSB;              //�¶ȡ����ʪ�ȵļĴ�������
	float Humidity,Temperature; //��ʪ�ȵ�ת�����

	SHT20_setResolution();           //����֡��8bit,9bit,10 bit,11bit ,12bit,13bit,14bit
	IIC_start();                 //iic��ʼ�ź�
	ack = SHT20_writeOneByte(SHT20ADDR&0xfe);
	if( ack == ACK )                //д��ַ�����ȴ�ACK
	{        
		if(SHT20_writeOneByte(commod)==ACK)                      //д����
		{
		 do
			{
				delay_ms(6);                                      //��ʱ
				IIC_start();                                    //���Ϳ�ʼ�ź�
			}while(SHT20_writeOneByte(SHT20ADDR|0x01)==NACK);      //��Ӧ�������Σ����ڲ����У������Ӧ���������ǰѭ��

			MSB = SHT20_readOneByte(ACK);                          //��Data(MSB)����Ӧ��ACK
			LSB = SHT20_readOneByte(ACK);                          //��Data(LSB)����Ӧ��ACK
			SHT20_readOneByte(NACK);                               //��Checksum ������Ӧ��NACK 
			IIC_stop();                                         //Stop I2C
			LSB &= 0xfc;                                       //Data (LSB) �ĺ���λ�ڽ����������ǰǰ���á�0�� 
			temp = MSB*256 + LSB;                              //ʮ������ת��ʮ����

			if (commod==((u8)READ_HUMI_COMD))                         //����Ϊ��ȡʪ�ȵ�����
			{        
				/*-- calculate relative humidity [%RH] --*/ 
				Humidity =(temp*125)/65536-6;                   //��ʽ: RH%= -6 + 125 * SRH/2^16
				return Humidity;                                //����ֵ��humidity
			}                                                                                                  
			else if(commod==((u8)READ_TEMP_COMD))                     //����Ϊ��ȡ�¶ȵ�����
			{        
				 /*-- calculate temperature [��C] --*/
				Temperature = (temp*175.72f)/65536-46.85f;       //��ʽ:T= -46.85 + 175.72 * ST/2^16
				return Temperature;                             //����ֵ��temperature
			}
		 }
	}
  return FAIL;
}
/*******************************************************************************
 * ��������SHT20_readTemAndHum
 * ������������ȡSHT20���¶Ⱥ�ʪ�Ȳ���ֵ  
 * ���ߣ�Jahol Fan  
 * ����˵����  
 * ����ֵ˵�����ɹ����ز����Ľ����ʧ�ܷ��� 1
 * �޸ļ�¼��
*******************************************************************************/

TempHumiValue_t * SHT20_readTemAndHum(void)
{
	temp_humi_Value.humidity = SHT20_readTemOrHum(READ_HUMI_COMD);//��ȡʪ��
	temp_humi_Value.temperature = SHT20_readTemOrHum(READ_TEMP_COMD);//��ȡʪ��
	return &temp_humi_Value;
}



/*******************************************************************************
*******************************************************************************/
