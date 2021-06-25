/********************************************************************************
*                              这里填写项目名
*                            Module: FM Drvier
*                 Copyright(c) 2001-2007 Actions Semiconductor,
*                            All Rights Reserved. 
*
* History:         
*      <author>    <time>           <version >             <desc>
*       xkping     2008-07-03 09:00     1.0             build this file 
********************************************************************************/
/*!
 * \file   fm_config.h
 * \brief  describle the command to access the FM driver and the data struct used in the driver
 * \author xkping
 * \par GENERAL DESCRIPTION:
 *       In This File 
 *      Define some  macros ,This macro are used to compact fm driver developing 
 * \par EXTERNALIZED FUNCTIONS:
 *     Copyright(c) 2001-2007 Actions Semiconductor, All Rights Reserved.
 *
 *  \version 1.0
 *  \date  2008/7/3
 *******************************************************************************/
#ifndef __FM_CONFIG_H__
#define __FM_CONFIG_H__

/*!
 *    _ACT_LINUX_KERNEL_2_6_ ,running OS is linux \n   
 *    _ACT_UC_OS_ ,running OS is uc/os \n 
 */
//#define  _ACT_LINUX_KERNEL_2_6_ 
#define  _ACT_UC_OS_

/*!
*   I2C_ChAN_NUM \n
*   i2c bus num,two optional value:  1 or 2
*   0: stands for i2c1
*   1: stands for i2c2 
*/
#define I2C_ChAN_NUM  0
//#define I2C_PIN_MASK  (~(0x3<<4)) //GPIO_MFCTL1 bit4,5
//#define I2C_PIN_VALUE  0x00

/*!
 *    ANT_DETEC_TYPE_GPIO_A  use gpio A pin to detecte FM antenna  \n  
 *    ANT_DETEC_TYPE_GPIO_B  use gpio B pin to detecte FM antenna  \n
 *    ANT_DETEC_GPIO_NUM     GPIO NUM \n   
 */
#define ANT_DETEC_TYPE_GPIO_B
//#define ANT_DETEC_TYPE_GPIO_B
#define ANT_DETEC_GPIO_NUM  31

/*!
 *    FM_IFLOWLIMIT,  low(smal) IF counter value for FM IC judeging that if a freq is a station  \n  
 *    FM_IFHIGHLIMIT, high(big) IF counter value for FM IC judeging that if a freq is a station  \n  
 */
#define IFLOWLIMIT   0x31
#define IFHIGHLIMIT  0x3e 
#define IF_HAVE_NO_OSCILLATION   0x0

#endif

