/***********************************************************************************************************************
Copyright 2008 - 2016 深圳市信盈达科技有限公司. All rights reserved.
文件名:        rtc.c
描述   :       实时时钟
作者   :       Jahol Fan
版本   :       V1.0
修改   :   
完成日期：     2016.9.19
************************************************************************************************************************/

/***********************************************************************************************************************
* INCLUDES
*/
#include "rtc.h"
#include "stm32f4xx.h"
/***********************************************************************************************************************
* CONSTANTS
*/


/***********************************************************************************************************************
* TYPEDEFS
*/
dateAndTime_t dateAndTime;

/***********************************************************************************************************************
* LOCAL VARIABLES
*/


/***********************************************************************************************************************
* LOCAL FUNCTIONS  DECLARE
*/
void RTC_getTime(u8 *hour,u8 *min,u8 *sec,u8 *ampm);
void RTC_getDate(u8 *year,u8 *month,u8 *date,u8 *week);
u8 RTC_setDate(u8 year,u8 month,u8 date,u8 week);
u8 RTC_setTime(u8 hour,u8 min,u8 sec,u8 ampm);
/***********************************************************************************************************************
* LOCAL FUNCTIONS  
*/



/***********************************************************************************************************************
* PUBLIC FUNCTIONS
*/
/**********************************************************************************************************
* 函数名：        RTC_waitSynchro
* 功能描述：      等待RSF同步
* 作者：          Jahol Fan  
* 参数说明：      val:要转换的十进制数
* 返回值说明：    返回0成功，1失败
* 修改记录：      2016.9.19
**********************************************************************************************************/
u8 RTC_waitSynchro(void)
{ 
  //关闭RTC寄存器写保护
  RTC->WPR=0xCA;
  RTC->WPR=0x53; 
  RTC->ISR &= ~(1<<5);		//清除RSF位
  
  while ((RTC->ISR&(1<<5)) == 0x00 )//等待影子寄存器同步
  {
    ;
  }
  
  RTC->WPR = 0xFF;	//激活RTC寄存器写保护  
  return 0;
}
/**********************************************************************************************************
* 函数名：        RTC_DEC2BCD
* 功能描述：      十进制转换为BCD码
* 作者：          Jahol Fan  
* 参数说明：      val:要转换的十进制数
* 返回值说明：    返回0成功，1失败
* 修改记录：      2016.9.19
**********************************************************************************************************/
u8 RTC_DEC2BCD(u8 val)
{
  u8 bcdhigh = 0; 
  while(val>=10)
  {
    bcdhigh++;
    val-=10;
  } 
  //	return ((val/10) << 4) | (val % 10)
  
  return ((u8)(bcdhigh<<4)|val);
}
/**********************************************************************************************************
* 函数名：        RTC_BCD2DEC
* 功能描述：      BCD码转换为十进制数据
* 作者：          Jahol Fan  
* 参数说明：      val:要转换的BCD码
* 返回值说明：    返回值:十进制数据
* 修改记录：      2016.9.19
**********************************************************************************************************/
u8 RTC_BCD2DEC(u8 val)
{
  u8 temp=0;
  temp=(val>>4)*10;
  return (temp+(val&0X0F));
}

/**********************************************************************************************************
* 函数名：        RTC_setDateAndTime
* 功能描述：      RTC设置日期和时间
* 作者：          Jahol Fan  
* 参数说明：      
* 返回值说明：    返回0成功，1失败
* 修改记录：      2016.9.19
**********************************************************************************************************/
u8 RTC_setDateAndTime(dateAndTime_t *dateAndTime)
{
  RTC_setTime(dateAndTime->hour,dateAndTime->minute,dateAndTime->second,dateAndTime->ampm);  
  RTC_setDate(dateAndTime->year,dateAndTime->month,dateAndTime->date,dateAndTime->week);
}



/**********************************************************************************************************
* 函数名：        RTC_setDate
* 功能描述：      RTC日期设置
* 作者：          Jahol Fan  
* 参数说明：    
*                 year,month,date:年(0~99),月(1~12),日(0~31)
*                 week:星期(1~7,0,非法!)
*                 返回值:0,成功
*                        1,进入初始化模式失败 
*
* 返回值说明：    
* 修改记录：      2016.9.22
**********************************************************************************************************/
u8 RTC_setDate(u8 year,u8 month,u8 date,u8 week)
{
	u32 temp=0;
 	//关闭RTC寄存器写保护
	RTC->WPR=0xCA;
	RTC->WPR=0x53; 
	if(RTC_initMode())return 1;//进入RTC初始化模式失败
	temp=(((u32)week&0X07)<<13)
		|((u32)RTC_DEC2BCD(year)<<16)
		|((u32)RTC_DEC2BCD(month)<<8)
		|(RTC_DEC2BCD(date)); 
	RTC->DR=temp;
	RTC->ISR&=~(1<<7);			//退出RTC初始化模式 
	return 0; 
}

/**********************************************************************************************************
* 函数名：        RTC_setTime
* 功能描述：      RTC时间设置
* 作者：          Jahol Fan  
* 参数说明：      
*                  hour,min,sec:小时,分钟,秒钟 
*                  ampm:AM/PM,0=AM/24H,1=PM.
* 返回值说明：    
*                 返回值:0,成功
*                        1,进入初始化模式失败 
*
* 修改记录：      2016.9.22
**********************************************************************************************************/
u8 RTC_setTime(u8 hour,u8 min,u8 sec,u8 ampm)
{
	u32 temp=0;
	//关闭RTC寄存器写保护
	RTC->WPR=0xCA;
	RTC->WPR=0x53; 
	if(RTC_initMode())return 1;//进入RTC初始化模式失败
	temp=(((u32)ampm&0X01)<<22)
		|((u32)RTC_DEC2BCD(hour)<<16)
		|((u32)RTC_DEC2BCD(min)<<8)
		|(RTC_DEC2BCD(sec));
	RTC->TR=temp;
	RTC->ISR&=~(1<<7);			//退出RTC初始化模式 
	return 0; 
}


/**********************************************************************************************************
* 函数名：        RTC_getDateAndTime
* 功能描述：      获取RTC日期和时钟
* 作者：          Jahol Fan  
* 参数说明：      
* 返回值说明：    
* 修改记录：      2016.9.22
**********************************************************************************************************/
dateAndTime_t *RTC_getDateAndTime(void)
{
  u8 year,month,date,week,hour,min,sec,ampm;
  
  RTC_getDate(&year,&month,&date,&week);
  dateAndTime.year    = year;
  dateAndTime.month   = month;
  dateAndTime.date    = date;
  dateAndTime.week    = week; 
  
  RTC_getTime(&hour,&min,&sec,&ampm);
  dateAndTime.hour    = hour;
  dateAndTime.minute  = min;
  dateAndTime.second  = sec;
  dateAndTime.ampm    = ampm; 
  
  return &dateAndTime;
}

/**********************************************************************************************************
* 函数名：        RTC_getTime
* 功能描述：      获取RTC时间
* 作者：          Jahol Fan  
* 参数说明：      
*                  *hour,*min,*sec:小时,分钟,秒钟 
*                  *ampm:AM/PM,0=AM/24H,1=PM.
* 返回值说明：    
* 修改记录：      2016.9.22
**********************************************************************************************************/
void RTC_getTime(u8 *hour,u8 *min,u8 *sec,u8 *ampm)
{
	u32 temp=0;
 	while(RTC_waitSynchro());	//等待同步  	 
	temp=RTC->TR;
	*hour=RTC_BCD2DEC((temp>>16)&0X3F);
	*min=RTC_BCD2DEC((temp>>8)&0X7F);
	*sec=RTC_BCD2DEC(temp&0X7F);
	*ampm=temp>>22; 
}
/**********************************************************************************************************
* 函数名：        RTC_getDate
* 功能描述：      获取RTC日期
* 作者：          Jahol Fan  
* 参数说明：    
*			  *year,*mon,*date:年,月,日
*                 *week:星期
* 返回值说明：    
* 修改记录：      2016.9.22
**********************************************************************************************************/
void RTC_getDate(u8 *year,u8 *month,u8 *date,u8 *week)
{
	u32 temp=0;
 	while(RTC_waitSynchro());	//等待同步  	 
	temp=RTC->DR;
	*year=RTC_BCD2DEC((temp>>16)&0XFF);
	*month=RTC_BCD2DEC((temp>>8)&0X1F);
	*date=RTC_BCD2DEC(temp&0X3F);
	*week=(temp>>13)&0X07; 
}

/**********************************************************************************************************
* 函数名：        RTC_initMode
* 功能描述：      RTC进入初始化模式
* 作者：          Jahol Fan  
* 参数说明：      
* 返回值说明：    返回0成功，1失败
* 修改记录：      2016.9.19
**********************************************************************************************************/
u8 RTC_initMode(void)
{ 

  if(RTC->ISR & (1<<6))//已经进入初始化模式。
  {
    return 0; 
  }
  
  RTC->ISR |= 1<<7;	//进入RTC初始化模式
  while ( (RTC->ISR & 1<<6) == 0x00 )
  {
    ;//等待进入RTC初始化模式成功 
  }
  
  return 0; 	//同步成功 

}
/**********************************************************************************************************
* 函数名：        RTC_init
* 功能描述：      RTC初始化
* 作者：          Jahol Fan  
* 参数说明：  
* 返回值说明：    返回0成功，1失败
* 修改记录：
**********************************************************************************************************/
u8 RTC_init(void)
{
	dateAndTime_t dateAndTime ={16,12,25,7,11,52,55,0};
  RCC->APB1ENR |= 1<<28;	//使能电源接口时钟
  PWR->CR |= 1<<8;				//后备区域访问使能(RTC+SRAM)  DBP置1
  
  RCC->BDCR |= 1<<0;			//LSE 开启  
  while ( (RCC->BDCR & 1<<1) == 0 )
  {
    ;//等待LSE就绪
  }
  RCC->BDCR &= ~(3<<8);
  RCC->BDCR |= 1<<8;			//选择LSE,作为RTC的时钟
  
  RCC->BDCR |= 1<<15;			//使能RTC时钟
  
  //关闭RTC寄存器写保护
  RTC->WPR = 0xCA;
  RTC->WPR = 0x53; 
  
  if (RTC_initMode())
  {
    return 1;//进入RTC初始化模式
  }
  
  RTC->PRER = 0XFF;				//RTC同步分频系数(0~7FFF)
  RTC->PRER |= 0X7F<<16;	//RTC异步分频系数(1~0X7F)
  
  RTC->CR &= ~(1<<6);			//RTC设置为,24小时格式
  
  RTC->ISR &= ~(1<<7);		//退出RTC初始化模式
  RTC->WPR = 0xFF;				//写入错误值，激活RTC寄存器写保护  
  
	RTC_setDateAndTime(&dateAndTime);  //设置日期和时间
  return 0;
}
/**********************************************************************************************************
* 函数名：        RTC_setAlarmA
* 功能描述：      往闹钟A数据寄存器里面写闹钟时间值，开启闹钟中断，设置闹钟时间(按星期闹铃,24小时制)
* 作者：          Jahol Fan  
* 参数说明：   
*                  week:星期几(1~7)
*                  hour,min,sec:小时,分钟,秒钟
* 返回值说明：    
* 修改记录：
**********************************************************************************************************/
void RTC_setAlarmA(u8 hour, u8 min, u8 sec, u8 week)
{
	//关闭RTC寄存器写保护
	RTC->WPR = 0xCA;
	RTC->WPR = 0x53; 
	
	RTC->CR &= ~(1<<8);	//关闭闹钟后，ALRAWF才会置1
	while ((RTC->ISR & 1) == 0)
	{
		;//等待 ALRAWF置1， 置1后，才允许写闹钟寄存器
	}
	
	RTC->ALRMAR = 0;
	RTC->ALRMAR |= 1 << 30;
	RTC->ALRMAR |= week << 24;
	RTC->ALRMAR |= RTC_DEC2BCD(hour) << 16;
	RTC->ALRMAR |= RTC_DEC2BCD(min) << 8;
	RTC->ALRMAR |= RTC_DEC2BCD(sec) << 0;
	
	RTC->CR |= (1<<8);	//开启闹钟A
	RTC->CR |= (1<<12);	//开启闹钟A中断  --- EXTI17
	
	RTC->WPR = 0xff;//激活写保护 
	
	EXTI->RTSR |= 1<<17;//EXTI_17上升沿触发
	EXTI->IMR |= 1<<17;// 开放EXTI_17中断请求
	NVIC_EnableIRQ(RTC_Alarm_IRQn);
}

/**********************************************************************************************************
* 函数名：        RTC_Alarm_IRQHandler
* 功能描述：      闹钟中断函数
* 作者：          Jahol Fan  
* 参数说明：     
* 返回值说明：    
* 修改记录：
**********************************************************************************************************/
void RTC_Alarm_IRQHandler()
{
	EXTI->PR = 1<<17;//写1，清除EXTI_17 挂起位
	
	if (RTC->ISR & 1<<8)//alarm_a
	{
		RTC->ISR &= ~(1<<8);
		
	}
	
	if (RTC->ISR & 1<<9)//alarm_b
	{
		//……
	}
}
dateAndTime_t *RTC_returnDateTimePointer(void)
{
  return &dateAndTime;
}

/***********************************************************************************************************************
***********************************************************************************************************************/
