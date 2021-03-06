/*******************************************************************************
Copyright 2008 - 2016 深圳市信盈达电子有限公司. All rights reserved.
文件名:        spi.h
描述   :       
作者   :       Jahol Fan
版本   :       V1.0
修改   :   
完成日期：     2016.10.13
信盈达官网：http://www.edu118.com/
信盈达网校：http://www.edu118.cn/
Notice    :本程序只供学习使用，未经作者许可，不得用于其它任何用途。版权所有，盗版必究。
*******************************************************************************/
#ifndef SPI_H
#define SPI_H

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

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 *PUBLIC FUNCTIONS DECLARE
 */
void Spi1_init(void);
u8 Spi1_RevSendByte(u8 val);



/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* SPI_H */
