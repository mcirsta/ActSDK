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
 * \file     local_lim.h
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

#ifndef __SYS_LOCAL_LIM_H__
#define __SYS_LOCAL_LIM_H__

/*	若使用了系统虚拟栈技术，则不需要申请堆栈空间.
 由系统自动分配栈空间,并可以无限制增长.
 但忽略用户设定的栈空间*/
#define SYSTEM_VISUAL_STACK_SUPPORT 1

/* 若使用系统虚拟栈技术,则不允许用户自定义栈空间 */
#if SYSTEM_VISUAL_STACK_SUPPORT > 0
#define PTHREAD_STACK_MIN       0
#define PTHREAD_STACK_DEFAULT   0
#else
/* 用户自定义线程栈的最小栈空间,若小于此值,则设置不正确 */
#define PTHREAD_STACK_MIN       (1024*20)
#define PTHREAD_STACK_DEFAULT   (PTHREAD_STACK_MIN*2)
#endif

/*!
 *  \brief
 *     同一竞争域内的线程最大优先级
 */
#define MAX_PRIO 10

/*!
 *  \brief
 *     同一竞争域内的线程最小优先级
 */
#define MIN_PRIO 1

/*!
 *  \brief
 *     线程默认优先级
 */
#define COMMON_TASK ( (((MAX_PRIO-MIN_PRIO)+1)/2) + MIN_PRIO )

/*!
 *  \brief
 *     线程私有键限制
 */
#define PTHREAD_KEYS_MAX   10

/*!
 *  \brief
 *     线程私有键辗转开解析构函数限制
 */
#define PTHREAD_DESTRUCTORITERATIONS 10

#endif /*#ifndef __SYS_LOCAL_LIM_H__*/
