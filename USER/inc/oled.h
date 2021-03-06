/*******************************************************************************
Copyright 2008 - 2016 深圳市信盈达电子有限公司. All rights reserved.
文件名:       simpleBLEPeripheral.c
描述   : 
作者   :       Jahol Fan
版本   :       V1.0
修改   :   
完成日期：
信盈达官网：http://www.edu118.com/
信盈达网校：http://www.edu118.cn/
Notice    :本程序只供学习使用，未经作者许可，不得用于其它任何用途。版权所有，盗版必究。
*******************************************************************************/
#ifndef SIMPLEBLEPERIPHERAL_H
#define SIMPLEBLEPERIPHERAL_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "stm32f4xx.h"
/*********************************************************************
 * TYPEDEFS
 */
/*********************************************************************
*  EXTERNAL VARIABLES
*/

/*********************************************************************
 * CONSTANTS
 */
#define MAX_COLUMNC	128
	
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
	
#define OLED_CS_SET()  (GPIOB->BSRRL = 1<<7)  	//OLED_CS  PB7
#define OLED_CS_CLR()  (GPIOB->BSRRH = 1<<7)  
	
#define OLED_RST_SET()  (GPIOB->BSRRL = 1<<13)  //OLED_RES  PB13
#define OLED_RST_CLR()  (GPIOB->BSRRH = 1<<13)  //OLED_RES  PB13

#define OLED_DC_SET()  (GPIOA->BSRRL = 1<<15)  //OLED_DC  PA15
#define OLED_DC_CLR()  (GPIOA->BSRRH = 1<<15)  
/*********************************************************************
 * MACROS
 */

/*********************************************************************
 *PUBLIC FUNCTIONS DECLARE
 */
void Oled_init(void);
u8 OLED_reset(void);
u8 OLED_pinInit(void);
void OLED_select(void);
void OLED_deSelect(void);
void OLED_writeByte(u8 dat, u8 cmd);
void OLED_clear(void) ;
void OLED_setPos(unsigned char x, unsigned char y) ;
void OLED_displayOn(void);
void OLED_displayOff(void);
void OLED_showChar(u8 x,u8 y,u8 chr,u8 Char_Size);
void OLED_showString(u8 x,u8 y,u8 *chr,u8 Char_Size);
void OLED_showNum(u8 x,u8 y,u32 num,u8 len,u8 size2);
u32 OLED_pow(u8 m,u8 n);
void OLED_showCHinese(u8 x,u8 y,u8 no);
void OLED_drawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void OLED_eraseBMP(u8 x0, u8 y0,u8 x1, u8 y1, u8 BMP[]);
/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* SIMPLEBLEPERIPHERAL_H */
