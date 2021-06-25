/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : app_timer.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-3-24             v1.0             build this file 
 ********************************************************************************/
/*!
 * \file     app_timer.h
 * \brief    应用级定时器接口 
 * \author   Herb Hu 
 * \version 1.0
 * \date  2009/03/24
 *******************************************************************************/

#ifndef __APP_TIMER_H__
#define __APP_TIMER_H__

#include <libc.h>

/*! 
 * \defgroup   applib_app_timer    应用级定时器
 */

/*!
 * \brief
 *  定时器触发时的回调函数类型定义
 */
typedef int (* timer_proc)(void * param);

/******************************************************************************/
/*!
 * \par  Description:
 *     设置一个定时器
 * \ingroup applib_app_timer
 * \param[in] timeout -- 定时器超时时间，单位为毫秒
 * \param[in] func_proc -- 定时器触发时的回调函数
 * \param[in] param -- 传递给回调函数的参数 
 * \return 成功返回该定时器的id, 失败返回-1  
 * \note 超时时间不可以设置为0
 *******************************************************************************/
extern int set_timer(unsigned int timeout, timer_proc func_proc, void * param);

/******************************************************************************/
/*!
 * \par  Description:
 *     设置一个单触发定时器
 * \ingroup applib_app_timer
 * \param[in] timeout -- 定时器超时时间，单位为毫秒
 * \param[in] func_proc -- 定时器触发时的回调函数
 * \param[in] param -- 传递给回调函数的参数 
 * \return 成功返回该定时器的id, 失败返回-1  
 * \note 超时时间不可以设置为0 
 *******************************************************************************/
extern int set_single_shot_timer(unsigned int timeout, timer_proc func_proc, void * param);

/******************************************************************************/
/*!
 * \par  Description:
 *     修改一个定时器的定时时间
 * \ingroup applib_app_timer
 * \param[in] timer_id -- 定时器id
 * \param[in] timeout -- 新的超时时间 
 * \return 成功返回true, 失败返回false  
 *******************************************************************************/
extern bool modify_timer(int timer_id, unsigned int timeout);

/******************************************************************************/
/*!
 * \par  Description:
 *     停止一个定时器 
 * \ingroup applib_app_timer
 * \param[in] timer_id -- 定时器id
 * \return 成功返回true, 失败返回false  
 *******************************************************************************/
extern bool stop_timer(int timer_id);

/******************************************************************************/
/*!
 * \par  Description:
 *     重新启动一个定时器
 * \ingroup applib_app_timer
 * \param[in] timer_id -- 定时器id
 * \return 成功返回true, 失败返回false  
 *******************************************************************************/
extern bool restart_timer(int timer_id);

/******************************************************************************/
/*!
 * \par  Description:
 *     杀死指定定时器
 * \ingroup applib_app_timer
 * \param[in] timer_id -- 定时器id
 * \return 成功返回true, 失败返回false  
 *******************************************************************************/
extern bool kill_timer(int timer_id);

/******************************************************************************/
/*!
 * \par  Description:
 *     杀死所有应用程序自行设置的定时器 
 * \return void
 * \ingroup applib_app_timer
 *******************************************************************************/
extern void kill_timers(void);




/******************************************************************************/
/*!
 * \par  Description:
 *     设置一个applib内部使用的定时器 
 * \ingroup applib_app_timer
 * \param [in] timeout   定时器超时时间，单位为毫秒
 * \param [in] func_proc 定时器触发时的回调函数
 * \param [in] param     传递给回调函数的参数 
 * \return 成功返回该定时器的id，失败返回-1 
 * \note 仅供applib库内部使用
 *******************************************************************************/
extern int set_inner_timer(unsigned int timeout, timer_proc func_proc, void * param);

/******************************************************************************/
/*!
 * \par  Description:
 *     设置一个applib内部使用的单触发定时器
 * \ingroup applib_app_timer
 * \param [in] timeout   定时器超时时间，单位为毫秒
 * \param [in] func_proc 定时器触发时的回调函数
 * \param [in] param     传递给回调函数的参数 
 * \return 成功返回该定时器的id, 失败返回-1  
 * \note 仅供applib库内部使用
 *******************************************************************************/
extern int set_inner_single_shot_timer(unsigned int timeout, timer_proc func_proc, void * param);

/******************************************************************************/
/*!
 * \par  Description:
 *     修改applib内部定时器定时时间
 * \ingroup applib_app_timer
 * \param [in] timer_id 定时器id
 * \param [in] timeout 新的超时时间 
 * \return 成功返回true, 失败返回false  
 * \note 仅供applib内部使用
 *******************************************************************************/
extern bool modify_inner_timer(int timer_id, unsigned int timeout);

/******************************************************************************/
/*!
 * \par  Description:
 *     停止一个applib内部定时器 
 * \ingroup applib_app_timer
 * \param [in] timer_id 定时器id
 * \return 成功返回true, 失败返回false  
 * \note 仅供applib内部使用
 *******************************************************************************/
extern bool stop_inner_timer(int timer_id);

/******************************************************************************/
/*!
 * \par  Description:
 *     重新启动一个applib内部定时器
 * \ingroup applib_app_timer
 * \param [in] timer_id 定时器id
 * \return 成功返回true, 失败返回false  
 * \note 仅供applib内部使用
 *******************************************************************************/
extern bool restart_inner_timer(int timer_id);

/******************************************************************************/
/*!
 * \par  Description:
 *      杀死applib内部使用的指定定时器 
 * \ingroup applib_app_timer
 * \param [in] timer_id 定时器id
 * \return 成功返回true, 失败返回false  
 * \note 仅供applib内部使用
 *******************************************************************************/
extern bool kill_inner_timer(int timer_id);

/******************************************************************************/
/*!
 * \par  Description:
 *     杀死所有applib内部使用的定时器
 * \ingroup applib_app_timer
 * \note 仅供applib内部使用
 *******************************************************************************/
extern void kill_inner_timers(void);

/******************************************************************************/
/*!
 * \par  Description:
 *     删除所有应用程序自行设置的定时器 
 * \ingroup applib_app_timer
 * \note 仅供applib内部使用
 *******************************************************************************/
extern void delete_timers(void);

/******************************************************************************/
/*!
 * \par  Description:
 *     删除所有applib内部使用的定时器
 * \ingroup applib_app_timer
 * \note 仅供applib内部使用
 *******************************************************************************/
extern void delete_inner_timers(void);

/******************************************************************************/
/*!
 * \par  Description:
 *    查询是否有超时的定时器, 如果找到就调用其处理函数
 * \ingroup applib_app_timer
 * \note 仅供applib内部使用
 *******************************************************************************/
extern void handle_timers(void);

/******************************************************************************/
/*!
 * \par  Description:
 *    Get current tick time in millisecond;
 * \ingroup applib_app_timer
 *******************************************************************************/
extern unsigned int get_tick_time_ms(void);

#endif /*#ifndef __APP_TIMER_H__*/
