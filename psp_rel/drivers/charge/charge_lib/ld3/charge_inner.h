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
  * modified by  cccw for 1100               2008-10-17 12:42
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
#include "kmod_calls.h"
#include "charge.h"

#ifndef  _CHARGE_INNER_H_
#define _CHARGE_INNER_H_

#define CHARGE_ADAPTER_SUPPORT

#define SPEAKER_GPIO_INEN GPIO_AINEN
#define SPEAKER_GPIO_DATA GPIO_ADAT
#define SPEAKER_GPIO_NUM 26

/*最长涓流充电时间，3600*0.5S=30分钟，即如果充电30分钟电池电压还不能达到3.0V，则认为该电池失效*/
#define CHARGE_TRICKLE_LIMIT     3600				
/*充电失败重试时间，1200*0.5S=10分钟，即若充电器处于充电失败状态，则每10分钟会重新尝试充电1次*/
#define CHARGE_RETRY_TIME 1200	


#define CHARGE_VMAX_BY_MODIFY
#ifdef CHARGE_VMAX_BY_MODIFY
#define CHARGE_VOLTAGE_MAX get_max_charge_voltage()
#else
#define CHARGE_VOLTAGE_MAX LI420		 			/*最大充电电压*/
#endif

#define CHARGE_CURRENT_MAX CHARGE_CURRENT_300		/*最大充电电流*/
#define CHARGE_CURRENT_USB CHARGE_CURRENT_150		/*UDISK,UMTP里使用的充电电流*/
#define CHARGE_CURRENT_VIDEO CHARGE_CURRENT_100		/*VEDIO应用里使用的充电电流*/
#define CHARGE_ADDED_TIME 10						/*加充时间，单位是分钟*/


typedef struct
{
    unsigned int corepll; 
    unsigned int back_light_level;
    unsigned int speaker_on_off;
}charge_power_attribute_t;


typedef enum 
{
    BAT_MODE_REAL = 0x00,
	BAT_MODE_SHOW = 0x01,
	BAT_MODE_SHOW_LOGIC 
}bat_value_mode_t;

extern charge_status_t charge_status;

void do_vbus_on(void);
void act_get_battery(void );
unsigned char act_set_pmu_charge(unsigned char type);


unsigned char act_set_charge_current(unsigned char level);
int act_set_max_param(charge_param_t	 *para_pt);

int charge_get_charge_status(charge_status_t * status_pt);

void charge_check(void);
void act_charge(void);
int act_get_max_param(charge_param_t	 *para_pt);
//static int charge_init(void);
//static void * null_func(void);
extern struct timer_list battery_detect_timer;
extern unsigned long *p_jiffies;
int lookup_bat_table(int index);

extern unsigned int bat_power ;
extern unsigned int bat_show ;

void charge_dc5v_check(void);

unsigned char charge_get_max_charge_voltage(void);

unsigned char charge_is_dc5v_on(void);
int charge_dev_init(void);
int charge_dev_exit(void);

unsigned char charge_get_battery_voltage(void);

extern void charge_action(void);
extern charge_status_t get_charge_status(void);

extern unsigned char is_DC5V_on(void);
extern unsigned char is_speaker_on(void);

extern unsigned char setMaxChargeParam(unsigned int volt, unsigned int current,unsigned char time);
/*设置充电参数，volt是最大充电电压，current是最大充电电流,time是为了防止虚满，
在检测到电池电压达到最大充电电压后加充的时间，单位是min*/

unsigned int getBattery(bat_value_mode_t mode);/*返回值：bit16---充电状态：0－无充电；1－充电*/
                                  /*bit0~3:0~a,电压十分比。*/ 
								  /*输入参数为"BAT_MODE_REAL时,输出的是真实电量*/
unsigned char setPMUCharge(unsigned char type);

/*输入参数为"BAT_MODE_SHOW时,输出的是非真实电量，是经过处理的电量，用于显示电量图标*/

unsigned char get_max_charge_voltage(void);
//void  set_charge_time_limit(charge_time_limit_t val);


void charge_start(void); /* 开启充电电路 */
void charge_stop(void); /* 关闭充电电路 */
unsigned char charge_set_charging_current(unsigned int level); /*设置当前充电电流*/
unsigned char charge_get_charging_current(void); /*获取当前充电电流*/

unsigned char charge_is_charger_stable(void); /*充电电路状态是否稳定，稳定返回1，不稳定返回0*/
unsigned char charge_is_charging(void); /*充电电路是否正在充电，是返回1，否返回0*/

void charge_disable_trickle(void); /*屏蔽涓流和15S等待*/
void charge_enable_trickle(void); /*开启涓流和15S等待*/

//unsigned char charge_is_bat_on(void); /*是否有电池，有返回1，无返回0*/
unsigned char charge_detect_dc5v(void); /*检测DC5V是否有供电，是返回1，否返回0*/

unsigned int charge_is_speaker_on(void); /*获取SPEAKER状态，打开返回1，关闭返回0*/
void state_machine(void);

#endif  /* _CHARGE_INNER_H_ */

