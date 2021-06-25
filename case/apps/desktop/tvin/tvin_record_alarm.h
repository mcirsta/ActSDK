/********************************************************************************
 *                              USDK(1100)
 *                             Module: tvin 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : tvin_record_alarm.h
 * History:         
 *      <author>       <time>        <version >            <desc>
 *      Jerry Fu      2009-4-27         v1.0             build this file 
 ********************************************************************************/
/*!
 * \file       tvin_record_alarm.h
 * \brief      controls of tvin record alarm
 * \author   Jerry Fu 
 *               
 * \version 1.0
 * \date  2009/04/28
 *******************************************************************************/

/*! 
 * \defgroup   case_tvin    tvin ģ������ݽṹ�ͽӿں���
 */

#ifndef   __TVIN_RECORD_ALARM_H__
#define   __TVIN_RECORD_ALARM_H__

#include "tvin_config.h"

#define TVIN_MODE_NO    0x01    //TVINӦ����ռ�õ�mode��
#define ALARM_MODE_NO   0x02    //����Ӧ����ռ�õ�mode��


bool tvin_create_alarm_clock(alarm_new_param_t *alarm_new_param);
bool tvin_get_alarm_clock(alarm_get_clock_param_t *alarm_get_clock_param);
bool tvin_query_alarm_clock(alarm_query_param_t *alarm_query_param);
bool tvin_modify_alarm_clock(alarm_modify_param_t *alarm_modify_param);
bool tvin_delete_alarm_clock(alarm_delete_param_t *alarm_delete_param);

#endif



