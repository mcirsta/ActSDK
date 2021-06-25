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
 * \file     sched.h
 * \brief    posix 线程库线程调度相关定义
 * \author wurui
 * \par 相关描述:
 * 声明errno
 * \par EXTERNALIZED FUNCTIONS:
 *     Copyright(c) 2001-2008 Actions Semiconductor, All Rights Reserved.
 *
 *  \version 1.0
 *  \date  2008/09/12
 *******************************************************************************/

#ifndef __SYS_SCHED_H__
#define __SYS_SCHED_H__

/*!
 *  \brief
 *     线程调度类型枚举值
 */
typedef enum
{
    SCHED_OTHER,
    SCHED_FIFO,
    SCHED_RR,
} sched_attr_t;

/*!
 *  \brief
 *     线程调度参数
 */
struct sched_param
{
    /*! 线程的posix优先级 */
    int sched_priority;
};

#endif /* #ifndef __SYS_SCHED_H__ */
