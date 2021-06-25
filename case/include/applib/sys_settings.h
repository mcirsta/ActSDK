/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : sys_settings.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-3-24             v1.0             build this file 
 ********************************************************************************/
/*!
 * \file     sys_settings.h
 * \brief    
 * \author   Herb Hu 
 * \version 1.0
 * \date  2009/03/24
 *******************************************************************************/


#ifndef __SYS_SETTINGS_H__
#define __SYS_SETTINGS_H__

#include <libc.h>
#include <rtc.h>

/*! 
 * \defgroup   applib_system_settings    系统设置接口 
 */

typedef struct 
{
    /*! 屏保时间 单位: 秒 */
    unsigned int screensaver_time;
    /*! 省电关机时间 单位: 秒 */
    unsigned int auto_standby_time;
    /*! 睡眠时间 单位：秒 */
    unsigned int timed_shutdown_time;
    /*! 背光亮度等级  级别：0~5 */
    unsigned int lcd_backlight;
}system_settings_t;

/******************************************************************************/
/*!                    
 * \par  Description:
 *     是否禁止屏保
 * \ingroup applib_system_settings
 * \param[in] forbid -- true: 禁止, false: 不禁止
 * \return void      
 *******************************************************************************/
extern void sys_forbid_screensaver(bool forbid);

/******************************************************************************/
/*!                    
 * \par  Description:
 *    设置启动屏保时的空闲时间 
 * \ingroup applib_system_settings
 * \param[in] time -- 时间值，单位为秒
 * \return 成功返回 true, 失败返回false      
 *******************************************************************************/
extern bool sys_set_screensaver_idle_time(unsigned int time);

/******************************************************************************/
/*!                    
 * \par  Description:
 *     检查当前屏保是否被禁止(只要有一个进程禁止屏保，整个屏保即被禁止) 
 * \ingroup applib_system_settings
 * \return 如果被禁止返回true, 否则返回false      
 *******************************************************************************/
extern bool sys_is_screensaver_forbidden(void);

/******************************************************************************/
/*!                    
 * \par  Description:
 *    是否禁止省电关机
 * \ingroup applib_system_settings
 * \param[in] forbid -- true: 禁止，false: 不禁止
 * \return  void      
 *******************************************************************************/
extern void sys_forbid_auto_standby(bool forbid);

/******************************************************************************/
/*!                    
 * \par  Description:
 *    设置省电关机时的空闲时间 
 * \ingroup applib_system_settings
 * \param[in] time -- 时间值，单位为秒
 * \return 成功返回 true, 否则返回false      
 *******************************************************************************/
extern bool sys_set_auto_standby_idle_time(unsigned int time);

/******************************************************************************/
/*!                    
 * \par  Description:
 *    检查省电关机是否被禁止(只要有一个进程禁止待机，则省电关机机制被禁止) 
 * \ingroup applib_system_settings
 * \return 如果被禁止返回true, 否则返回false      
 *******************************************************************************/
extern bool sys_is_auto_standby_forbidden(void);

/******************************************************************************/
/*!                    
 * \par  Description:
 *    是否禁止定时关机
 * \ingroup applib_system_settings
 * \param[in] forbid -- true: 禁止，false: 不禁止
 * \return  void
 *******************************************************************************/
extern void sys_forbid_timed_shutdown(bool forbid);

/******************************************************************************/
/*!
 * \par  Description:
 *    设置定时关机的时间 
 * \ingroup applib_system_settings
 * \param[in] time -- 时间值，单位为秒
 * \return 成功返回 true, 否则返回false      
 *******************************************************************************/
extern bool sys_set_timed_shutdown_time(unsigned int time);

/******************************************************************************/
/*!                    
 * \par  Description:
 *    检查定时关机是否被禁止(只要有一个进程禁止待机，则省电关机机制被禁止)
 * \ingroup applib_system_settings
 * \return 如果被禁止返回true, 否则返回false
 *******************************************************************************/
extern bool sys_is_timed_shutdown_forbidden(void);

/******************************************************************************/
/*!
 * \par  Description:
 *    设置LCD背光亮度等级 
 * \ingroup applib_system_settings
 * \param[in] val -- 亮度等级：0~5 
 * \return 成功返回 true, 否则返回false      
 *******************************************************************************/
extern bool sys_set_lcd_backlight(unsigned int val);

/******************************************************************************/
/*!
 * \par  Description:
 *    设置rtc日期和时间 
 * \ingroup applib_system_settings
 * \param[in] time -- 时间结构体指针, 为NULL时, 不会设置该项 
 * \param[in] date -- 日期结构体指针, 为NULL时, 不会设置该项 
 * \return 成功返回true, 失败返回false
 * \note 如果time和date中某个值为空指针NULL, 则不会设置该项
 *******************************************************************************/
extern bool sys_set_rtc_date_time(rtc_time_t *time, rtc_date_t *date);

/******************************************************************************/
/*!
 * \par  Description:
 *    获取rtc日期和时间 
 * \ingroup applib_system_settings
 * \param[out] time -- 时间结构体指针, 为NULL时, 不会获取该项 
 * \param[out] date -- 日期结构体指针, 为NULL时, 不会获取该项 
 * \return 成功返回true, 失败返回false
 * \note 如果time和date中某个值为空指针NULL，则不会获取该项
 *******************************************************************************/
extern bool sys_get_rtc_date_time(rtc_time_t *time, rtc_date_t *date);

/******************************************************************************/
/*!
 * \par  Description:
 *    设置drm rtc日期和时间 
 * \ingroup applib_system_settings
 * \param[in] drm_time -- 时间结构体指针, 为NULL时, 不会设置该项 
 * \param[in] drm_date -- 日期结构体指针, 为NULL时, 不会设置该项 
 * \return 成功返回true, 失败返回false
 * \note 如果drm_time和drm_date中某个值为空指针NULL，则不会设置该项
 *       一般应用无需调用
 *******************************************************************************/
extern bool sys_set_rtc_drm_date_time(rtc_time_t *drm_time, rtc_date_t *drm_date);

/******************************************************************************/
/*!
 * \par  Description:
 *   获取drm rtc日期和时间 
 * \ingroup applib_system_settings
 * \param[out] drm_time -- 时间结构体指针, 为NULL时, 不会获取该项 
 * \param[out] drm_date -- 日期结构体指针, 为NULL时, 不会获取该项 
 * \return 成功返回true, 失败返回false
 * \note 如果drm_time和drm_date中某个值为空指针NULL, 则不会获取该项
 *******************************************************************************/
extern bool sys_get_rtc_drm_date_time(rtc_time_t *drm_time, rtc_date_t *drm_date);

/******************************************************************************/
/*!
 * \par  Description:
 *    将日期时间转换成标准秒 
 * \ingroup applib_system_settings
 * \param[in] time -- 时间结构体指针, 不能为NULL指针
 * \param[in] date -- 日期结构体指针, 不能为NULL指针
 * \param[out] stdsec --  存放结果,  不能为NULL指针
 * \return 成功返回true, 失败返回false
 *******************************************************************************/
bool sys_rtc_change_to_stdsec(rtc_time_t *time, rtc_date_t *date, unsigned long * stdsec);

/******************************************************************************/
/*!
 * \par  Description:
 *    打开rtc设备文件
 * \ingroup applib_system_settings
 * \return 成功返回true, 失败返回false
 * \note applib库内部调用, 应用无需调用
 *******************************************************************************/
extern bool sys_open_rtc_dev(void);

/******************************************************************************/
/*!
 * \par  Description:
 *    关闭rtc设备文件
 * \ingroup applib_system_settings
 * \return 成功返回true, 失败返回false
 * \note applib库内部调用, 应用无需调用
 *******************************************************************************/
extern bool sys_close_rtc_dev(void);

/******************************************************************************/
/*!
 * \par  Description:
 *    设置按键音使能与否. 用于进入 退出audio应用, 关闭打开按键音功能 
 * \ingroup applib_system_settings
 * \param[in] flag_enable -- 1: 使能按键音，0：禁止按键音 
 * \return 成功返回true, 失败返回false
 *******************************************************************************/
extern bool sys_set_key_tone_enable( int flag_enable );

/******************************************************************************/
/*!
 * \par  Description:
 *    设置是否允许系统事件(USB插入，卡插入等)的检测 
 * \ingroup applib_system_settings
 * \param[in] on -- true : 允许，false：不允许 
 * \return 成功返回true, 失败返回false
 *******************************************************************************/
extern bool sys_enable_sys_evt_detect(bool on);

#endif /*#ifndef __SYS_SETTINGS_H__*/
