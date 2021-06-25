/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : system_timers.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-3-24             v1.0             build this file 
 ********************************************************************************/
/*!
 * \file     system_timers.h
 * \brief    
 * \author   Herb Hu 
 * \version 1.0
 * \date  2009/03/24
 *******************************************************************************/


#ifndef __SYSTEM_TIMERS_H__
#define __SYSTEM_TIMERS_H__


#include "include_case.h"
#include "include_msg_server.h"



typedef enum
{
    SYS_TIMER_TYPE_SCREENSAVER,
    SYS_TIMER_TYPE_STANDBY,
    SYS_TIMER_TYPE_SHUTDOWN
}sys_timer_type_e;


/*!
 * \brief  
 * �������ļ���ȡ��ϵͳ��ʱ����������Ϣ 
 */
extern void load_sys_timers_config(void);

/*!
 * \brief  
 * ������ֹͣ����ϵͳ��ʱ�� 
 * \param start[in] true: ������false: ֹͣ 
 */
extern void start_sys_timers(bool start);

/*!
 * \brief  
 * ������ֹͣϵͳ����ʱ��ʱ��(screen saver , auto standby ������ϵͳ����ʱ��ʱ��) 
 * \param start[in] true: ������false: ֹͣ 
 */
extern void start_sys_idle_timers(bool start);

/*!
 * \brief  
 * ������ָֹͣ��ϵͳ��ʱ�� 
 * \param timer[in] ָ����ʱ��ö���� 
 * \param start[in] true: ������false: ֹͣ 
 */
extern void start_sys_timer(sys_timer_type_e timer, bool start);

/*!
 * \brief  
 * ����ָ��ϵͳ��ʱ���Ķ�ʱʱ�� 
 * \param timer[in] ָ����ʱ��ö���� 
 * \param timeout[in]ʱ��ֵ����λΪ��
 */
extern void set_sys_timer(sys_timer_type_e timer, unsigned int timeout);

/*!
 * \brief  
 * ��ȡָ��ϵͳ��ʱ���Ķ�ʱʱ�� 
 * \param timer[in] ָ����ʱ��ö���� 
 * \return ʱ��ֵ����λΪ��
 */
extern inline unsigned int get_sys_timer_timeout(sys_timer_type_e timer);

/*!
 * \brief  
 *  �������� 
 * \return �ɹ�����true��ʧ�ܷ���false
 */
extern bool enter_screensaver(void);

/*!
 * \brief  
 *  �˳����� 
 * \return �ɹ�����true��ʧ�ܷ���false
 */
extern bool exit_screensaver(void);

 /*!
 * \brief  
 *  �Ƿ��������� 
 * \return ���������з���true�����򷵻�false
 */
extern bool is_in_screensaver(void);

/*!
 * \brief
 * ��鶨ʱ�ػ���ʱ���Ƿ��Ѿ����ڳ�ʱ״̬��
 * ���ǣ��������ػ����̡�
 */
extern void check_shutdown_timer(void);


#endif /*#ifndef __SYSTEM_TIMERS_H__*/

