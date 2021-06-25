/********************************************************************************
 *                        USDK130
 *                     watchdog driver
 *
 *                (c) Copyright 2002, Actions Co,Ld.
 *                        All Right Reserved
 *
 * File: watchdog.h
 * By  : wurui
 * Version: 1> v1.00     first version     2007-10-30 15:51
 *******************************************************************************
 */

/*!
 * \file  watchdog.h
 * \brief 任务运行状态监听驱动程序
 * \author wurui
 * \par GENERAL DESCRIPTION:
 *           每一个任务在创建后都需要不断地向监听程序报告运行状态, 驱动程序跟踪报告来断送是否有任务处于不正常的状态,
 *       若有则立刻重启系统.
 * \par DECLARATIONS:
 *       (c) Copyright 2002-2004, Actions Co,Ld. All Right Reserved.
 *
 *  \version 1.0
 *  \date  2007/10/30
 */

#ifndef __WDOG_H__
#define __WDOG_H__

#include "kmod_calls.h"

/*!
 *  查询间隔,单位: ms \n
 *  \ingroup     Drivers.WDM
 */
#define WDTASK_INTERVAL 500

/*!
 *  WDD 设备句柄 \n
 *  \li\b time_out   : WDD的超时设定
 *  \li\b count      : WDD的计时器
 *  \ingroup     Drivers.WDM
 */
typedef struct
{
    u32 time_out;
    u32 count;
    s32 index;
} WDD_handle_t;

/*!
 *  WDD 命令 \n
 *  \li\b WDD_CMD_SET_TIME   : 设置计时超时时间, 参数单位:ms
 *  \ingroup     Drivers.WDM
 */
typedef enum
{
    WDD_CMD_SET_TIME,
} WDD_cmd_t;

#endif  /*#ifndef __WDOG_H__*/
