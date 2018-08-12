
#include "stm32f4xx.h"
#include "key.h"
#include "led.h"
#include "exti.h"
#include "uart.h"
#include "delay.h"
#include "timer.h"
#include "oled.h"
#include "spi.h"
#include "rtc.h"
#include "bmp.h"
#include "iic.h"
#include "sht20.h"
#include "hp6.h"
#include "mpu6050.h"
u8 page = 1;
void show_page(void);
void show_page1(void);
void show_page2(void);
void clock_page(void);
void pulse_page(void);
void bp_page(void);
void step_page(void);
void kal_km_page(void);
void time_page(void);
int main()//�������ṹ  
{
	delay_init(100);
	led_init();
	key_init();
	Spi1_init();
	Oled_init();
	IIC_init();
	SHT20_Init();
	
	Hp_6_init();       //��ʼ��HP6����Ѫѹ ģ��
  MPU_Init();
	RTC_init();
	
	OLED_clear();
	show_page();
	delay_ms(3000);
	
	while (1)
	{		
		switch(page)
		{
			
//			case 6:
//				OLED_clear();
//			  show_page1();
//			  break;
			case 1:
				OLED_clear();
			  time_page();
			  break;
			case 2:
			  OLED_clear();
				clock_page();
				break;
			case 3:
			  OLED_clear();
				pulse_page();
				break;
			case 4:
				OLED_clear();
				bp_page();
				break;
			case 5:
				OLED_clear();
				step_page();
				break;
			case 6:
				OLED_clear();
			  show_page2();
			  break;
		  default:
			break;				
		}
		
	}
}


void show_page(void)
{
//	while(1)
//	{
		OLED_drawBMP(0,1,54,8,(u8*)st);
		OLED_showCHinese(56,2,23);//��
		OLED_showCHinese(72,2,24);//��
		OLED_showCHinese(88,2,25);//��
		OLED_showCHinese(104,2,26);//ѧ
		OLED_showCHinese(56,5,27);//��
		OLED_showCHinese(72,5,28);//��
		OLED_showCHinese(88,5,29);//ѧ
		OLED_showCHinese(104,5,30);//Ժ
//		if(key_scan())
//		{
//			page = page +1;  //�л���page1
//			IIC_stop();
//			break;  //������ǰҳ��
//    }			
//	}
}

void show_page1(void)
{
	while(1)
	{
		OLED_drawBMP(0,1,54,8,(u8*)kyz);
		OLED_showCHinese(72,2,0);//��
		OLED_showCHinese(90,2,1);//��
		OLED_showCHinese(108,2,2);//��
		OLED_showCHinese(72,5,3);//��
		OLED_showCHinese(90,5,4);//��
		OLED_showCHinese(108,5,5);//��
		if(key_scan())
		{
			page = 1;  //�л���page1
			IIC_stop();
			break;  //������ǰҳ��
    }			
	}
}

void show_page2(void)
{
	while(1)
	{
		OLED_drawBMP(0,1,54,8,(u8*)IT);
		OLED_showString(54,0,"by:",12);
		OLED_showCHinese(76,0,31);//Ҷ
		OLED_showCHinese(94,0,32);//��
		OLED_showCHinese(112,0,33);//��
		OLED_showCHinese(76,2,34);//��
		OLED_showCHinese(94,2,35);//־
		OLED_showCHinese(112,2,36);//��
		OLED_showCHinese(76,4,37);//��
		OLED_showCHinese(94,4,38);//��
		OLED_showCHinese(112,4,39);//��
		OLED_showString(62,7,"16.12.25",12);
		delay_ms(5000);
		OLED_clear();
		OLED_showString(44,3,"Merry",16);
		OLED_showString(30,5,"Christmas",16);
		delay_ms(3000);
		OLED_clear();
		if(key_scan())
		{
			page = 1;  //�л���page1
			IIC_stop();
			break;  //������ǰҳ��
    }			
	}
}

void time_page(void)
{
	TempHumiValue_t humi_temp_val ; 	//��ʪ��	
	  u32 uiBuf_w[40];//�¶ȱ���
	  u32 uiBuf_s[40];//ʪ�ȱ���
	u8 uiBuf[40];
  dateAndTime_t *dateAndTime; 		//ʱ��&����
	while (1)
	{		
		humi_temp_val = *SHT20_readTemAndHum();//��ȡ�¶�
		uiBuf_w[0]=(int)humi_temp_val.temperature;
		uiBuf_s[0]=(int)humi_temp_val.humidity;
		 OLED_showNum(0,6,*uiBuf_w,3,16);	
		 OLED_showNum(36,6,*uiBuf_s,3,16);	
		 OLED_drawBMP(24,6,40,8,(u8*)wddw);	
		 OLED_drawBMP(60,6,76,8,(u8*)sddw);

  /*******************<��ȡRTCʱ�����ڣ�����ʾ>*************************/	
  //Jahol Fan  ��ȡ������RTC
  dateAndTime = RTC_getDateAndTime();
		sprintf((char*)uiBuf,"%02d:%02d:%02d",dateAndTime->hour,dateAndTime->minute,dateAndTime->second); 	  
  OLED_showString(0,1,uiBuf,24);	
  sprintf((char*)uiBuf,"20%02d.%02d.%02d",dateAndTime->year,dateAndTime->month,dateAndTime->date); 
  OLED_showString(24+3,5,uiBuf,12);
  switch(dateAndTime->week)
  {
  case MONDAY:
    OLED_showString(10+8*10,6,"Mon",16);
    break;
  case TUESDAY:
    OLED_showString(10+8*10,6,"Tues",16);
    break;
  case WEDNESDAY:
    OLED_showString(10+8*10,6,"Wed",16);
    break;
  case THURSDAY:
    OLED_showString(10+8*10,6,"Thur",16);
    break;
  case FRIDAY:
    OLED_showString(10+8*10,6,"Fri",16);
    break;
  case SATURDAY:
    OLED_showString(10+8*10,6,"Sat",16);
    break;
  case SUNDAY:
    OLED_showString(10+8*10,6,"Sun",16);
    break;
  default:
    break;		
  }
   if(key_scan())
		{
			page = page +1;  //�л���page2
			IIC_stop();
			break;  //������ǰҳ��
    }		  
	}
}

void clock_page(void)
{
	TempHumiValue_t humi_temp_val ; 	//��ʪ��	
	  u32 uiBuf_w[40];//�¶ȱ���
	  u32 uiBuf_s[40];//ʪ�ȱ���
  while(1)
	{
		humi_temp_val = *SHT20_readTemAndHum();//��ȡ�¶�
		uiBuf_w[0]=(int)humi_temp_val.temperature;
		uiBuf_s[0]=(int)humi_temp_val.humidity;
		 OLED_showCHinese(57,1,6);//��
		 OLED_showCHinese(74,1,8);//��
		 OLED_showCHinese(57,5,7);//ʪ
		 OLED_showCHinese(74,5,8);//��
     OLED_showNum(89,1,*uiBuf_w,3,16);	
		 OLED_showNum(89,5,*uiBuf_s,3,16);	
		 OLED_drawBMP(112,1,128,3,(u8*)wddw);	
		 OLED_drawBMP(112,5,128,7,(u8*)sddw);
		 OLED_drawBMP(0,1,54,8,(u8*)wsd0);
		 delay_ms(100);
		 OLED_drawBMP(0,1,54,8,(u8*)wsd1);
		 delay_ms(100);
		 OLED_drawBMP(0,1,54,8,(u8*)wsd2);	
		 delay_ms(100);		
//     OLED_showString(120,1,"C",16);	
//		 OLED_showString(120,5,"%",16);
		if(key_scan())
		{
			page = page +1;  //�л���page3
			IIC_stop();
			break;  //������ǰҳ��
    }			
	}
}

void pulse_page(void)
{
	u8 uiBuf[40];
  u8 pulseBuf[24] = {0};
	HP_6_OpenRate();   //�����ʲ���

  while(1)
	
	{	
		
		OLED_showCHinese(64,1,9);//��
		OLED_showCHinese(78,1,10);//��
		OLED_showCHinese(92,1,13);//��
		OLED_showCHinese(106,1,14);//��
		OLED_showCHinese(70,3,11);//��
		OLED_showString(84,3,"/",16);
		OLED_showCHinese(92,3,12);//��
		
    HP_6_GetRateResult();
		HP_6_GetResultData(pulseBuf);
		
		sprintf((char*)uiBuf,"%03d",pulseBuf[7]); // //byte 7�����ʽ��
		OLED_showString(65,6,uiBuf,16); 	
		OLED_showString(90,6,"bpm",16); 
		OLED_drawBMP(0,1,54,8,(u8*)xltb);
		delay_ms(300);
		OLED_drawBMP(0,1,54,8,(u8*)xltb_small);
		delay_ms(300);
		delay_ms(100);
		if(key_scan())
		{
			page = page +1; //�л���page4
			HP_6_CloseRate();   //�ر����ʲ���
			IIC_stop();
			break;  //������ǰҳ��
    }	
	}
}


void bp_page(void)
{
	
	u8 uiBuf[40];
  u8 BpBuf[24] = {0};
	HP_6_OpenBp();   //��Ѫѹ����
  while(1)
	{
		OLED_drawBMP(0,1,54,8,(u8*)xytb);
		OLED_showCHinese(64,0,15);//Ѫ
		OLED_showCHinese(78,0,16);//ѹ
		OLED_showCHinese(92,0,13);//��
		OLED_showCHinese(106,0,14);//��
    HP_6_GetBpResult();                //��ȡѪѹ����״̬
	  HP_6_GetResultData(BpBuf);
   if(0 == BpBuf[7])                   //byte 7��Ѫѹ������״̬:0�����У�1������ɡ�2����ʧ��
		 OLED_showString(64,2,"testing",16);  
	 else if(1 == BpBuf[7])  
   {
		OLED_showString(64,2,"success",16);
		sprintf((char*)uiBuf,"%03d/%03d", BpBuf[10], BpBuf[11]);//byte 10��Ѫѹ��������ĸ�ѹ,byte 11��Ѫѹ��������ĵ�ѹ	
		OLED_showString(64,4,uiBuf,16); 
		OLED_showString(76,6,"mmHg",16);		 
	 }		 
	 else if(2 == BpBuf[7])                  
		 OLED_showString(64,2,"fail   ",16);
	 else 
		 //do nothing
		delay_ms(100);
		if(key_scan())
		{
			page = page +1; //�л���page5
			HP_6_CloseBp();   //�ر�Ѫѹ����
			IIC_stop();
			break;  //������ǰҳ��
    }	
	}
}
void step_page(void)
{
	u8 uiBuf[40];
	float accX,accY,accZ;                 //�ԡ���/��^2��Ϊ��λ�ļ��ٶ�
	short aacx,aacy,aacz;									//���ٶȴ�����ԭʼ����
  while(1)
	{
    MPU_Get_Accelerometer(&aacx,&aacy,&aacz);			  //�õ����ٶȴ���������
		//����������ת��Ϊ�ԡ���/��^2��Ϊ��λ�ļ��ٶ�
		accX = ((float)(int)aacx/16384) *10;
		accY = ((float)(int)aacy/16384) *10;
		accZ = ((float)(int)aacz/16384) *10; 
		
		OLED_showCHinese(4,2,17);//��
		OLED_showCHinese(20,2,18);//��
		OLED_showCHinese(36,2,19);//��
		OLED_showCHinese(4,4,20);//��
		OLED_showCHinese(20,4,21);//��
		OLED_showCHinese(36,4,22);//��
		
		if(accX>=0)
		{ 
			sprintf((char*)uiBuf,"X:%02d ",(char)accX); // ��������ʾ
			OLED_showString(60,1,uiBuf,16);				
		}
	  else
		{
			sprintf((char*)uiBuf,"X:-%02d",(char)(-accX)); // ��������ʾ
			OLED_showString(60,1,uiBuf,16);				
		}

		
		if(accY>=0)
		{
			sprintf((char*)uiBuf,"Y:%02d ",(char)accY); // ��������ʾ
			OLED_showString(60,3,uiBuf,16);				
		}
	  else
		{
			sprintf((char*)uiBuf,"Y:-%02d",(char)(-accY)); // ��������ʾ
			OLED_showString(60,3,uiBuf,16);				
		}
		
		
		if(accZ>=0)
		{
			sprintf((char*)uiBuf,"Z:%02d ",(char)accZ); // ��������ʾ
			OLED_showString(60,5,uiBuf,16);				
		}
	  else
		{
			sprintf((char*)uiBuf,"Z:-%02d",(char)(-accZ)); // ��������ʾ
			OLED_showString(60,5,uiBuf,16);				
		}	
		if(key_scan())
		{
			page = page +1; //�л���page6
			IIC_stop();
			break;  //������ǰҳ��
    }	
	}
}

