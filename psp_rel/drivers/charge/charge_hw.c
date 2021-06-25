/********************************************************************************
 *                        USDK130
 *                      the charge driver
 *
 *                (c) Copyright 2002, Actions Co,Ld.
 *                        All Right Reserved
 *
 * File:  charge_info.c
 * By  : 
 * Version: 1> v1.00     first version     2006-9-25 8:28
 * modified by  jianggaoping               2007-10-1 10:00
 *******************************************************************************
 */
 
  /*!
* \file  charge_info.c
* \brief  
* \author 
* \par GENERAL DESCRIPTION:
*          This file inclue fuction will be called by other application or driver
*          
* \par DECLARATIONS:
*       (c) Copyright 2002-2004, Actions Co,Ld. All Right Reserved.
*
*  \version 1.1
*  \date  2007/10/1
*/

#include "charge_inner.h"


void charge_start(void)
{
    OS_INT_SAVE_AREA
    unsigned int tmp;
  
    OS_ENTER_CRITICAL();
    tmp = act_readl((unsigned int) PMU_CHG);
    tmp = tmp | (0x1 << 31);
    act_writel(tmp, (unsigned int) PMU_CHG); /* 开启充电电路 */
    OS_EXIT_CRITICAL();
}

void charge_stop(void)
{
    OS_INT_SAVE_AREA
    unsigned int tmp;
  
    OS_ENTER_CRITICAL();
    tmp = act_readl((unsigned int) PMU_CHG);
    tmp = tmp & ~(0x1 << 31);
    act_writel(tmp, (unsigned int) PMU_CHG); /* 关闭充电电路 */
    OS_EXIT_CRITICAL();
}

unsigned char charge_set_charging_current(unsigned int level)
{
    OS_INT_SAVE_AREA
    unsigned int tmp;
  
    if (level > 7)
    {
        return 0;
    }
    OS_ENTER_CRITICAL();
    tmp = act_readl((unsigned int) PMU_CHG);
    tmp = (tmp & (~(0x7 << 28))) | (level << 28);
    act_writel(tmp, (unsigned int) PMU_CHG);
    OS_EXIT_CRITICAL();
    return 1;
}

unsigned char charge_get_charging_current(void)
{
    OS_INT_SAVE_AREA
    unsigned int tmp;
  
    OS_ENTER_CRITICAL();
    tmp = act_readl((unsigned int) PMU_CHG);
    tmp = tmp & (0x7 << 28);
    tmp = tmp >> 28;
    OS_EXIT_CRITICAL();
  
    return (unsigned char) tmp;
}

unsigned char charge_get_battery_voltage(void)
{
    OS_INT_SAVE_AREA
    unsigned int tmp;
  
    OS_ENTER_CRITICAL();
    tmp = act_readl((unsigned int) PMU_LRADC);
    tmp = (tmp >> 16) & 0x3f;
    OS_EXIT_CRITICAL();
    return (unsigned char) tmp;
}

unsigned char charge_is_charger_stable(void) /*充电电路状态是否稳定，稳定返回1，不稳定返回0*/
{
    OS_INT_SAVE_AREA
    unsigned int tmp;
  
    OS_ENTER_CRITICAL();
    tmp = act_readl((unsigned int) PMU_CHG);
    tmp = tmp & (0x3 << 25);
    OS_EXIT_CRITICAL();
    if (tmp!=0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

unsigned char charge_is_charging(void) /*充电电路是否在充电，是返回1，否则返回0*/
{
    OS_INT_SAVE_AREA
    unsigned int tmp;
  
    OS_ENTER_CRITICAL();
    tmp = act_readl((unsigned int) PMU_CHG);
    tmp = tmp & (0x1 << 27);
    OS_EXIT_CRITICAL();
    if (tmp!=0)
    {
        return 1;
    }
    else
    {
        return 0;
    }

}

void charge_disable_trickle(void)
{
    OS_INT_SAVE_AREA
    unsigned int tmp;
  
    OS_ENTER_CRITICAL();
    tmp = act_readl((unsigned int) PMU_CHG);
    tmp = tmp & ~(0xf << 20);
    tmp = tmp | (0x3 << 22);
    act_writel(tmp, (unsigned int) PMU_CHG);
    OS_EXIT_CRITICAL();

}

void charge_enable_trickle(void)
{
    OS_INT_SAVE_AREA
    unsigned int tmp;
  
    OS_ENTER_CRITICAL();
    tmp = act_readl((unsigned int) PMU_CHG);
    tmp = tmp & ~(0xf << 20);
    act_writel(tmp, (unsigned int) PMU_CHG);
    OS_EXIT_CRITICAL();
}
/*
 unsigned char charge_is_bat_on(void)
 {
 unsigned char tmp;

 tmp = charge_get_battery_voltage();
 if (tmp >= 0x1d)
 return 1;
 else
 {
 charge_disable_trickle();
 charge_set_charging_current(CHARGE_CURRENT_50);
 charge_start();
 S_OSTimeDly(50);
 tmp = charge_get_battery_voltage();
 if (tmp < 0x30)
 return 1;
 else
 return 0;
 }
 charge_stop();
 charge_enable_trickle();
 }
 */
unsigned char charge_detect_dc5v(void) /*检测DC5V是否有供电，是返回1，否返回0*/
{
    OS_INT_SAVE_AREA
    unsigned int tmp;
  
    OS_ENTER_CRITICAL();
    tmp = act_readl((unsigned int) PMU_LRADC);
    tmp = (tmp >> 31) & 0x01;
    OS_EXIT_CRITICAL();
    return (unsigned char) tmp;
}

/*获取SPEAKER状态，打开返回1，关闭返回0*/
unsigned int charge_is_speaker_on(void)
{
    OS_INT_SAVE_AREA
    unsigned int tmp;
  
    OS_ENTER_CRITICAL();
  
    act_writel(act_readl((unsigned int) SPEAKER_GPIO_INEN)
        | (0x1 << SPEAKER_GPIO_NUM), (unsigned int) SPEAKER_GPIO_INEN);
    tmp = act_readl((unsigned int) SPEAKER_GPIO_DATA);
    tmp = (tmp >> SPEAKER_GPIO_NUM) & 0x01;
  
    OS_EXIT_CRITICAL();
    return (unsigned char) tmp;
}
