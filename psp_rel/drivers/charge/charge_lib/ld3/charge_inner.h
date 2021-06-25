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

/*�������ʱ�䣬3600*0.5S=30���ӣ���������30���ӵ�ص�ѹ�����ܴﵽ3.0V������Ϊ�õ��ʧЧ*/
#define CHARGE_TRICKLE_LIMIT     3600				
/*���ʧ������ʱ�䣬1200*0.5S=10���ӣ�������������ڳ��ʧ��״̬����ÿ10���ӻ����³��Գ��1��*/
#define CHARGE_RETRY_TIME 1200	


#define CHARGE_VMAX_BY_MODIFY
#ifdef CHARGE_VMAX_BY_MODIFY
#define CHARGE_VOLTAGE_MAX get_max_charge_voltage()
#else
#define CHARGE_VOLTAGE_MAX LI420		 			/*������ѹ*/
#endif

#define CHARGE_CURRENT_MAX CHARGE_CURRENT_300		/*��������*/
#define CHARGE_CURRENT_USB CHARGE_CURRENT_150		/*UDISK,UMTP��ʹ�õĳ�����*/
#define CHARGE_CURRENT_VIDEO CHARGE_CURRENT_100		/*VEDIOӦ����ʹ�õĳ�����*/
#define CHARGE_ADDED_TIME 10						/*�ӳ�ʱ�䣬��λ�Ƿ���*/


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
/*���ó�������volt��������ѹ��current����������,time��Ϊ�˷�ֹ������
�ڼ�⵽��ص�ѹ�ﵽ������ѹ��ӳ��ʱ�䣬��λ��min*/

unsigned int getBattery(bat_value_mode_t mode);/*����ֵ��bit16---���״̬��0���޳�磻1�����*/
                                  /*bit0~3:0~a,��ѹʮ�ֱȡ�*/ 
								  /*�������Ϊ"BAT_MODE_REALʱ,���������ʵ����*/
unsigned char setPMUCharge(unsigned char type);

/*�������Ϊ"BAT_MODE_SHOWʱ,������Ƿ���ʵ�������Ǿ�������ĵ�����������ʾ����ͼ��*/

unsigned char get_max_charge_voltage(void);
//void  set_charge_time_limit(charge_time_limit_t val);


void charge_start(void); /* ��������· */
void charge_stop(void); /* �رճ���· */
unsigned char charge_set_charging_current(unsigned int level); /*���õ�ǰ������*/
unsigned char charge_get_charging_current(void); /*��ȡ��ǰ������*/

unsigned char charge_is_charger_stable(void); /*����·״̬�Ƿ��ȶ����ȶ�����1�����ȶ�����0*/
unsigned char charge_is_charging(void); /*����·�Ƿ����ڳ�磬�Ƿ���1���񷵻�0*/

void charge_disable_trickle(void); /*���������15S�ȴ�*/
void charge_enable_trickle(void); /*���������15S�ȴ�*/

//unsigned char charge_is_bat_on(void); /*�Ƿ��е�أ��з���1���޷���0*/
unsigned char charge_detect_dc5v(void); /*���DC5V�Ƿ��й��磬�Ƿ���1���񷵻�0*/

unsigned int charge_is_speaker_on(void); /*��ȡSPEAKER״̬���򿪷���1���رշ���0*/
void state_machine(void);

#endif  /* _CHARGE_INNER_H_ */

