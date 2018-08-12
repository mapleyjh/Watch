/***********************************************************************************************************************
Copyright 2008 - 2016 ��������ӯ��������޹�˾. All rights reserved.
�ļ���:        oled.c
����   :       oled����
����   :       Jahol Fan
�汾   :       V1.0
�޸�   :   
������ڣ�      2016.10.13
��ӯ�������http://www.edu118.com/
��ӯ����У��http://www.edu118.cn/
Notice    :������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;����Ȩ���У�����ؾ���
************************************************************************************************************************/

/***********************************************************************************************************************
 * INCLUDES
 */
#include "oled.h"
#include "spi.h"
#include "font.h"
#include "delay.h"
/***********************************************************************************************************************
 * CONSTANTS
 */


/***********************************************************************************************************************
 * TYPEDEFS
 */


/***********************************************************************************************************************
 * LOCAL VARIABLES
 */
 
/***********************************************************************************************************************
 * LOCAL FUNCTIONS  DECLARE
 */


/***********************************************************************************************************************
 * LOCAL FUNCTIONS  
 */



/***********************************************************************************************************************
 * PUBLIC FUNCTIONS
 */
/**********************************************************************************************************
* ������    ��   Oled_init
* ��������  ��   oled��ʼ��
* ����      ��   Jahol Fan  
* ����˵��  ��   none
* ����ֵ˵����   none
* �޸ļ�¼  �� 
* ����      :
**********************************************************************************************************/


//��ʼ��SSD1306					    
void Oled_init(void)
{ 	 
	OLED_pinInit();	
  OLED_deSelect();  //��ѡ��OLED
	OLED_reset();		
	
	OLED_writeByte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_writeByte(0x02,OLED_CMD);//---SET low column address
	OLED_writeByte(0x10,OLED_CMD);//---SET high column address
	OLED_writeByte(0x40,OLED_CMD);//--SET start line address  SET Mapping RAM Display Start Line (0x00~0x3F)
	OLED_writeByte(0x81,OLED_CMD);//--SET contrast control register
	OLED_writeByte(0xCF,OLED_CMD); // SET SEG Output Current Brightness
	OLED_writeByte(0xA1,OLED_CMD);//--SET SEG/Column Mapping     0xa0���ҷ��� 0xa1����
	OLED_writeByte(0xC8,OLED_CMD);//SET COM/Row Scan Direction   0xc0���·��� 0xc8����
	OLED_writeByte(0xA6,OLED_CMD);//--SET normal display
	OLED_writeByte(0xA8,OLED_CMD);//--SET multiplex ratio(1 to 64)
	OLED_writeByte(0x3f,OLED_CMD);//--1/64 duty
	OLED_writeByte(0xD3,OLED_CMD);//-SET display offSET	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_writeByte(0x00,OLED_CMD);//-not offSET
	OLED_writeByte(0xd5,OLED_CMD);//--SET display clock divide ratio/oscillator frequency
	OLED_writeByte(0x80,OLED_CMD);//--SET divide ratio, SET Clock as 100 Frames/Sec
	OLED_writeByte(0xD9,OLED_CMD);//--SET pre-charge period
	OLED_writeByte(0xF1,OLED_CMD);//SET Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_writeByte(0xDA,OLED_CMD);//--SET com pins hardware configuration
	OLED_writeByte(0x12,OLED_CMD);
	OLED_writeByte(0xDB,OLED_CMD);//--SET vcomh
	OLED_writeByte(0x40,OLED_CMD);//SET VCOM Deselect Level
	OLED_writeByte(0x20,OLED_CMD);//-SET Page Addressing Mode (0x00/0x01/0x02)
	OLED_writeByte(0x02,OLED_CMD);//
	OLED_writeByte(0x8D,OLED_CMD);//--SET Charge Pump enable/disable
	OLED_writeByte(0x14,OLED_CMD);//--SET(0x10) disable
	OLED_writeByte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_writeByte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_writeByte(0xAF,OLED_CMD);//--turn on oled panel
	
	OLED_writeByte(0xAF,OLED_CMD); /*display ON*/ 
	OLED_clear();
	OLED_setPos(0,0); 	
}  
/**********************************************************************************************************
* ������    ��   OLED_Reset
* ��������  ��   oled��λ
* ����      ��   Jahol Fan  
* ����˵��  ��   none
* ����ֵ˵����   none
* �޸ļ�¼  �� 
* ����      :
**********************************************************************************************************/
u8 OLED_reset(void)
{
	OLED_RST_CLR();
	delay_ms(25);   //Jahol Fan ����ֵ����⣬��ʱ�ĳ��ˣ��Ͳ��ܼ���HP-6
	OLED_RST_SET();
	return 0;       //����0����ʾ��λ�ɹ�
}
/**********************************************************************************************************
* ������    ��   OLED_resetPinInit
* ��������  ��   oled��λ�ܽų�ʼ��
* ����      ��   Jahol Fan  
* ����˵��  ��   none
* ����ֵ˵����   none
* �޸ļ�¼  �� 
* ����      :
								OLED_RES  	PB13
								OLED_DC 		PA15
								OLED_CS  		PB7
**********************************************************************************************************/
u8 OLED_pinInit(void)
{
	RCC->AHB1ENR |= 3<<0;//PA  PB
	
	//OLED_RES  	PB13
	GPIOB->MODER &= ~(3<<26);
	GPIOB->MODER |= 1<<26;
	
	GPIOB->OTYPER &= ~(1<<13);
	GPIOB->OSPEEDR &= ~(3<<26);
	GPIOB->OSPEEDR |= 2<<26;
	
	//OLED_DC 		PA15
	GPIOA->MODER &= ~(3ul<<30);
	GPIOA->MODER |= 1ul<<30;
	
	GPIOA->OTYPER &= ~(1<<15);
	GPIOA->OSPEEDR &= ~(3ul<<30);
	GPIOA->OSPEEDR |= 2ul<<30;
	
	//OLED_CS  		PB7
	GPIOB->MODER &= ~(3<<14);
	GPIOB->MODER |= 1<<14;
	
	GPIOB->OTYPER &= ~(1<<7);
	GPIOB->OSPEEDR &= ~(3<<14);
	GPIOB->OSPEEDR |= 2<<14;	
	
	return 0;
}
/**********************************************************************************************************
* ������    ��   OLED_select
* ��������  ��   oledƬѡ
* ����      ��   Jahol Fan  
* ����˵��  ��   none
* ����ֵ˵����   none
* �޸ļ�¼  �� 
* ����      :
**********************************************************************************************************/
void OLED_select(void)
{
   OLED_CS_CLR();//����ʹ�ܶ�
}
/**********************************************************************************************************
* ������    ��   OLED_deSelect
* ��������  ��   oledȡ��Ƭѡ
* ����      ��   Jahol Fan  
* ����˵��  ��   none
* ����ֵ˵����   none
* �޸ļ�¼  �� 
* ����      :
**********************************************************************************************************/
void OLED_deSelect(void)
{  
	 OLED_CS_SET();//����ʹ�ܶ�
}


/**********************************************************************************************************
* ������    ��   OLED_writeByte
* ��������  ��   oled�ֽ�д����SSH1106д��һ���ֽڡ�
* ����      ��   Jahol Fan  
* ����˵��  ��   dat:Ҫд�������/����
*                cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
* ����ֵ˵����   none
* �޸ļ�¼  �� 
* ����      :
**********************************************************************************************************/
void OLED_writeByte(u8 dat, u8 cmd)
{	
	cmd ? OLED_DC_SET() : OLED_DC_CLR();	
		
	OLED_select();		 
	
	Spi1_RevSendByte(dat);
	
	OLED_deSelect();
} 
/**********************************************************************************************************
* ������    ��   OLED_clear
* ��������  ��   ��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	
* ����      ��   Jahol Fan  
* ����˵��  ��   none
* ����ֵ˵����   none
* �޸ļ�¼  �� 
* ����      :
**********************************************************************************************************/
void OLED_clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_writeByte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_writeByte (0x02,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_writeByte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0; n<128; n++)
		{
			OLED_writeByte(0,OLED_DATA); 
		}
	} //������ʾ
}
/**********************************************************************************************************
* ������    ��   OLED_setPos
* ��������  ��   ��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	
* ����      ��   Jahol Fan  
* ����˵��  ��   none
* ����ֵ˵����   none
* �޸ļ�¼  �� 
* ����      :
**********************************************************************************************************/
void OLED_setPos(unsigned char x, unsigned char y) 
{ 
	x += 2;//SSH1106 Ҫ�����
	
	OLED_writeByte(0xb0+y,OLED_CMD);
	OLED_writeByte(((x&0xf0)>>4)|0x10, OLED_CMD);
	OLED_writeByte((x&0x0f)|0x01,OLED_CMD); 
}  
/**********************************************************************************************************
* ������    ��   OLED_displayOn
* ��������  ��   ����OLED��ʾ  
* ����      ��   Jahol Fan  
* ����˵��  ��   none
* ����ֵ˵����   none
* �޸ļ�¼  �� 
* ����      :
**********************************************************************************************************/   
void OLED_displayOn(void)
{
	OLED_writeByte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_writeByte(0X14,OLED_CMD);  //DCDC ON
	OLED_writeByte(0XAF,OLED_CMD);  //DISPLAY ON
}
/**********************************************************************************************************
* ������    ��   OLED_displayOff
* ��������  ��   �ر�OLED��ʾ   
* ����      ��   Jahol Fan  
* ����˵��  ��   none
* ����ֵ˵����   none
* �޸ļ�¼  �� 
* ����      :
**********************************************************************************************************/   
void OLED_displayOff(void)
{
	OLED_writeByte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_writeByte(0X10,OLED_CMD);  //DCDC OFF
	OLED_writeByte(0XAE,OLED_CMD);  //DISPLAY OFF
}	

/*******************************************************************************
* ��������OLED_showChar
* ������������ָ��λ����ʾһ���ַ�,���������ַ�
* ���ߣ�Jahol Fan	
* ����˵����  
*			��ָ��λ����ʾһ���ַ�,���������ַ�
*			x:0~127
*			y:0~63
*			mode:0,������ʾ;1,������ʾ				 
*			size:ѡ������ 24/16/12
* ����ֵ˵����none
* �޸ļ�¼��
*******************************************************************************/

void OLED_showChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
  u8 c=0,i=0;	
  c=chr-' ';//�õ�ƫ�ƺ��ֵ			
  if(x>MAX_COLUMNC-1)
  {
    x=0;
    y=y+2;  //���Ҫ�Ż�����������Ĳ�ͬ������
  }
  if(Char_Size == 24)
  {
    OLED_setPos(x,y);	
    for(i=0;i<16;i++)
      OLED_writeByte(F16X24[c*48+i],OLED_DATA);
    OLED_setPos(x,y+1);
    for(i=0;i<16;i++)
      OLED_writeByte(F16X24[c*48+i+16],OLED_DATA);
		OLED_setPos(x,y+2);
    for(i=0;i<16;i++)
      OLED_writeByte(F16X24[c*48+i+16*2],OLED_DATA);
  }
  else if(Char_Size ==16)
  {
    OLED_setPos(x,y);	
    for(i=0;i<8;i++)
      OLED_writeByte(F8X16[c*16+i],OLED_DATA); //һ���ַ���16���ֽ�������
    OLED_setPos(x,y+1); //y+1 ��ʾ���У�
    for(i=0;i<8;i++)
      OLED_writeByte(F8X16[c*16+i+8],OLED_DATA);
  }
	
  else 
  {	
    OLED_setPos(x,y);
    for(i=0;i<6;i++)
      OLED_writeByte(F6x8[c][i],OLED_DATA); 
  }
}


/*******************************************************************************
* ��������OLED_showString
* ������������ʾһ���ַ��Ŵ�
* ���ߣ�Jahol Fan	
* ����˵����  
* ����ֵ˵����none
* �޸ļ�¼��
*******************************************************************************/
void OLED_showString(u8 x,u8 y,u8 *chr,u8 Char_Size)
{
  u8 j=0;
  while (chr[j]!='\0')
  {		
    OLED_showChar(x,y,chr[j],Char_Size);
		if(24 == Char_Size)
		{
		  x+=16;
		}
		else
      x+=8;
    if(x>120){x=0;y+=2;}
    j++;
  }
}



/*******************************************************************************
* ��������OLED_showNum
* ������������ʾ2������
* ���ߣ�Jahol Fan	
* ����˵����  
*			x,y :�������	 
*		    en :���ֵ�λ��
*		    size:�����С
*			mode:ģʽ	0,���ģʽ;1,����ģʽ
*			num:��ֵ(0~4294967295);	
* ����ֵ˵����none
* �޸ļ�¼��
*******************************************************************************/		  
void OLED_showNum(u8 x,u8 y,u32 num,u8 len,u8 size2)
{         	
  u8 t,temp;
  u8 enshow=0;						   
  for(t=0;t<len;t++)
  {
    temp=(num/OLED_pow(10,len-t-1))%10;
    if(enshow==0&&t<(len-1))
    {
      if(temp==0)
      {
        OLED_showChar(x+(size2/2)*t,y,' ',size2);
        continue;
      }else enshow=1; 
    }
    OLED_showChar(x+(size2/2)*t,y,temp+'0',size2); 
  }
} 

/**********************************************************************************************************
* ������    ��   OLED_pow
* ��������  ��   m^n����
* ����      ��   Jahol Fan  
* ����˵��  ��   
* ����ֵ˵����   none
* �޸ļ�¼  �� 
* ����      :
**********************************************************************************************************/ 
u32 OLED_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}
/**********************************************************************************************************
* ������    ��   m^n����
* ��������  ��   OLED_showCHinese
* ����      ��   Jahol Fan  
* ����˵��  ��   
* ����ֵ˵����   none
* �޸ļ�¼  �� 
* ����      :
**********************************************************************************************************/ 
void OLED_showCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_setPos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_writeByte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_setPos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_writeByte(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}
/**********************************************************************************************************
* ������    ��   OLED_drawBMP
* ��������  ��   ������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7
* ����      ��   Jahol Fan  
* ����˵��  ��   
* ����ֵ˵����   none
* �޸ļ�¼  �� 
* ����      :
**********************************************************************************************************/ 
void OLED_drawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;

	for(y=y0;y<y1;y++)
	{
		OLED_setPos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_writeByte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 
/*******************************************************************************
* ��������  OLED_eraseBMP
* ������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7
* ���ߣ�    Jahol Fan	
* ����˵����  
* ����ֵ˵����none
* �޸ļ�¼��
*******************************************************************************/
void OLED_eraseBMP(u8 x0, u8 y0,u8 x1, u8 y1, u8 BMP[])
{ 	
  u8 x,y;

  for(y=y0;y<y1;y++)
  {
    OLED_setPos(x0,y);
    for(x=x0;x<x1;x++)
    {      
      OLED_writeByte(0x00,OLED_DATA);	    	
    }
  }
} 

/***********************************************************************************************************************
***********************************************************************************************************************/
