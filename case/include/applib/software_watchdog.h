/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : software_watchdog.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-3-24             v1.0             build this file 
 ********************************************************************************/
/*!
 * \file     software_watchdog.h
 * \brief    
 * \author   Herb Hu 
 * \version 1.0
 * \date  2009/03/24
 *******************************************************************************/



#ifndef __SOFTWARE_WATCHDOG_H__
#define __SOFTWARE_WATCHDOG_H__

/*! 
 * \defgroup   applib_software_watchdog   软看门狗接口 
 */

/******************************************************************************/
/*!                    
 * \par  Description:
 *      检测当前是否禁止看门狗。
 * \ingroup applib_software_watchdog
 * \return 如果被禁止返回true, 否则返回false
 *******************************************************************************/
extern bool  sys_is_soft_watchdog_forbid(void);

/******************************************************************************/
/*!
 * \par  Description:
 *      是否禁止看门狗
 * \ingroup applib_software_watchdog
 * \param[in] forbid -- true: 禁止，false: 不禁止
 * \return  void
 *******************************************************************************/
extern void sys_forbid_soft_watchdog(bool forbid);

/******************************************************************************/
/*!
 * \par  Description:
 *      设置软看门狗超时时间 
 * \ingroup applib_software_watchdog
 * \param[in] timeout -- 超时时间，单位为毫秒，如果设为0，则关闭软看门狗 
 * \return void
 *******************************************************************************/
extern void set_soft_watchdog_timeout(unsigned int timeout);

/******************************************************************************/
/*!                    
 * \par  Description:
 *      检查所用应用进程的software watchdog, 如果发现有超时的，kill 该应用进程
 * \ingroup applib_software_watchdog
 * \return void
 * \note 该接口仅供 manager 应用调用
 *******************************************************************************/
extern void check_soft_watchdogs(void);

#endif /*#ifndef __SOFTWARE_WATCHDOG_H__*/
