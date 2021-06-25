/********************************************************************************
 *                              NOYA
 *                            Module: libc_general
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       wurui     2008-09-12 10:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     semaphore.h
 * \brief    posix C库中系统相关的信号量定义
 * \author   wurui
 * \version 1.0
 * \date  2008/09/12
 *******************************************************************************/

#ifndef __SYS_SEMAPHORE_H__
#define __SYS_SEMAPHORE_H__

#include <libc/sys/types.h>
#include <libc/sys/debug.h>

/*!
 *  \brief
 *     信号量创建失败返回值
 */
#define SEM_FAILED	((sem_t *) -1)

/*!
 *  \brief
 *     命名信号量赋初值标志
 */
#define PTHREAD_SEM_NAMING_MAGIC    "SEMN"

/*!
 *  \brief
 *     无名信号量赋初值标志
 */
#define PTHREAD_SEM_WITHOUT_MAGIC   "SEMW"

/*!
 *  \brief
 *     信号量赋初值判断标志
 */
#define PTHREAD_SEM_MAGIC           "SEM"

/* 无名信号量全局初值宏 */
#define SEMAPHOR_INITIALIZER \
{ \
    OS_EVENT_TYPE_SEM, /*OSEventType*/ \
    0x00, /*OSEventGrp*/ \
    0, /*OSEventCnt*/ \
    NULL, /*OSEventPtr*/ \
    OS_EVENT_TBL, \
    OS_EVENT_NAME, \
    0, \
}

/*!
 *  \brief
 *     信号量内部结构:信号量命名空间结构定义
 */
typedef struct
{
    /*! 信号量结构有效标志 */
    char magic[4];
    /*! 信号量引用深度 */
    int ref;
    /*! 进程间引用属性 */
    int pshared;
    /*! 无名信号量 */
    os_event_t semaphor_thread;
    /*! 有名信号量 */
    os_event_t *semaphor;
    /*! 若信号量已经删除，保存信号量命名空间地址 */
    void *ulink_node;
#if LIBC_PIPE_DEBUG_ON > 0
void *debug_sapce_node;
#endif
} sem_ns_t;

/*!
 *  \brief
 *     信号量描述符结构
 */
typedef struct
{
    /*! 信号量命名空间结构 */
    sem_ns_t *sem_ns;
} sem_t;

#endif /* #ifndef __SYS_SEMAPHORE_H__ */

