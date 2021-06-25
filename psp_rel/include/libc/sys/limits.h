/********************************************************************************
 *                                    NOYA
 *                            Module: libc_general
 *                 Copyright(c) 2001-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>           <version >             <desc>
 *        wurui     2008-09-12 10:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     limits.h
 * \brief    posix C库中系统相关的限制信息
 * \author wurui
 * \par 相关描述:
 * 声明errno
 * \par EXTERNALIZED FUNCTIONS:
 *     Copyright(c) 2001-2008 Actions Semiconductor, All Rights Reserved.
 *
 *  \version 1.0
 *  \date  2008/09/12
 *******************************************************************************/

#ifndef __SYS_LIMITS_H__
#define __SYS_LIMITS_H__

#include <libc/sys/local_lim.h>

/*! 消息队列的优先级范围: 0 ~ MQ_PRIO_MAX-1 */
#define MQ_PRIO_MAX     10

/*! 信号量最大值 */
#define SEM_VALUE_MAX   255

/*! 路径名最大值 */
#define PATH_MAX        1024

/*! 文件名最大值 */
#define MAXNAMLEN       1024

/* 创建进程时允许的参数的数目 */
#define MAXARGS 10

/* atexit可以注册的最大退出处理函数数量 */
#define ATEXIT_MAX 10

#endif /* #ifndef __SYS_LIMITS_H__ */

