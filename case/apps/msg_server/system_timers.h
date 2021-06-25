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
 * 从配置文件读取各系统定时器的配置信息 
 */
extern void load_sys_timers_config(void);

/*!
 * \brief  
 * 启动或停止所有系统定时器 
 * \param start[in] true: 启动，false: 停止 
 */
extern void start_sys_timers(bool start);

/*!
 * \brief  
 * 启动或停止系统空闲时定时器(screen saver , auto standby 都属于系统空闲时定时器) 
 * \param start[in] true: 启动，false: 停止 
 */
extern void start_sys_idle_timers(bool start);

/*!
 * \brief  
 * 启动或停止指定系统定时器 
 * \param timer[in] 指定定时器枚举名 
 * \param start[in] true: 启动，false: 停止 
 */
extern void start_sys_timer(sys_timer_type_e timer, bool start);

/*!
 * \brief  
 * 设置指定系统定时器的定时时间 
 * \param timer[in] 指定定时器枚举名 
 * \param timeout[in]时间值，单位为秒
 */
extern void set_sys_timer(sys_timer_type_e timer, unsigned int timeout);

/*!
 * \brief  
 * 获取指定系统定时器的定时时间 
 * \param timer[in] 指定定时器枚举名 
 * \return 时间值，单位为秒
 */
extern inline unsigned int get_sys_timer_timeout(sys_timer_type_e timer);

/*!
 * \brief  
 *  进入屏保 
 * \return 成功返回true，失败返回false
 */
extern bool enter_screensaver(void);

/*!
 * \brief  
 *  退出屏保 
 * \return 成功返回true，失败返回false
 */
extern bool exit_screensaver(void);

 /*!
 * \brief  
 *  是否处于屏保中 
 * \return 处于屏保中返回true，否则返回false
 */
extern bool is_in_screensaver(void);

/*!
 * \brief
 * 检查定时关机定时器是否已经处于超时状态，
 * 如是，则启动关机流程。
 */
extern void check_shutdown_timer(void);


#endif /*#ifndef __SYSTEM_TIMERS_H__*/

