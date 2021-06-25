/********************************************************************************
 *                              NOYA
 *                            Module: types.h
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       wurui     2008-09-12 10:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     types.h
 * \brief    posix C库系统相关的基本类型定义
 * \author   wurui
 * \version 1.0
 * \date  2008/09/12
 *******************************************************************************/

#ifndef __POSIX_TYPES_H__
#define __POSIX_TYPES_H__

#include <syscalls.h>
#include <libc/sys/sched.h>
#include <libc/sys/limits.h>
#include <libc/sys/local_lim.h>
#include <libc/sys/reent.h>
#include <libc/sys/time.h>

/* 系统信号量全局赋初值宏 */
#if OS_EVENT_TBL_SIZE == 1
#define OS_EVENT_TBL \
{ \
    0, /*OSEventTbl[0]*/ \
}
#endif

#if OS_EVENT_TBL_SIZE == 2
#define OS_EVENT_TBL \
{ \
    0, /*OSEventTbl[0]*/ \
    0, /*OSEventTbl[1]*/ \
}
#endif

#if OS_EVENT_TBL_SIZE == 3
#define OS_EVENT_TBL \
{ \
    0, /*OSEventTbl[0]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
}
#endif

#if OS_EVENT_TBL_SIZE == 4
#define OS_EVENT_TBL \
{ \
    0, /*OSEventTbl[0]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
}
#endif

#if OS_EVENT_TBL_SIZE == 5
#define OS_EVENT_TBL \
{ \
    0, /*OSEventTbl[0]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
}
#endif

#if OS_EVENT_TBL_SIZE == 6
#define OS_EVENT_TBL \
{ \
    0, /*OSEventTbl[0]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
}
#endif

#if OS_EVENT_TBL_SIZE == 7
#define OS_EVENT_TBL \
{ \
    0, /*OSEventTbl[0]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
}
#endif

#if OS_EVENT_TBL_SIZE == 8
#define OS_EVENT_TBL \
{ \
    0, /*OSEventTbl[0]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
}
#endif

#if OS_EVENT_NAME_SIZE > 1
#define OS_EVENT_NAME "sem_thread"  /*OSEventName*/
#endif

/*!
 *  \brief
 *     线程分离属性枚举值
 */
typedef enum
{
    /*! 分离线程 */
    PTHREAD_CREATE_DETACHED,
    /*! 非分离线程 */
    PTHREAD_CREATE_JOINABLE,
} detach_attr_t;

/*!
 *  \brief
 *     线程竞争域属性枚举值
 */
typedef enum
{
    /*! 实时线程 */
    PTHREAD_SCOPE_SYSTEM,
    /*! 非实时线程 */
    PTHREAD_SCOPE_PROCESS,
    MAX_SCOPE_NUM,
} scope_attr_t;

/*!
 *  \brief
 *     线程继承属性枚举值
 */
typedef enum
{
    /*! 继承创建者的线程属性 */
    PTHREAD_INHERIT_SCHED = 1,
    /*! 由线程属性指定 */
    PTHREAD_EXPLICIT_SCHED,
} inheritsched_attr_t;

/*!
 *  \brief
 *     互斥量类型枚举值定义
 */
enum
{
    PTHREAD_MUTEX_NORMAL,
    PTHREAD_MUTEX_RECURSIVE,
    PTHREAD_MUTEX_ERRORCHECK,
    PTHREAD_MUTEX_INVALID,
    PTHREAD_MUTEX_DEFAULT = PTHREAD_MUTEX_NORMAL,
} pthread_mutex_type_t;

/*!
 *  \brief
 *     进程间共享属性枚举定义
 */
enum
{
    /*! 进程内共享 */
    PTHREAD_PROCESS_PRIVATE,
    /*! 进程间共享 */
    PTHREAD_PROCESS_SHARED
} pthread_share_type_t;

/*!
 *  \brief
 *     pthread_once参数定义
 */
typedef int pthread_once_t;

/*!
 *  \brief
 *     线程属性结构
 */
typedef struct
{
    /*! 初始化有效标志 */
    char magic[4];
    /*! 线程栈首地址 */
    void *stackaddr;
    /*! 线程栈大小 */
    int stacksize;
    /*! 堆栈保护区大小 */
    unsigned int guardsize;
    /*! 线程调度策略的获取模式,由父线程继承或是由属性指定 */
    int inheritsched;
    /*! 线程的调度竞争范围 */
    int contentionscope;
    /*! 调度参数 */
    struct sched_param schedparam;
    /*! 线程的调度策略 */
    int schedpolicy;
    /*! 分离属性 */
    detach_attr_t detachstate;
} pthread_attr_t;

/*!
 *  \brief
 *     互斥量初始化属性
 */
typedef struct
{
    unsigned int magic;
    /*! PTHREAD_PROCESS_SHARED,PTHREAD_PROCESS_PRIVATE */
    int pshared;
    /*! PTHREAD_MUTEX_NORMAL,PTHREAD_MUTEX_ERRORCHECK,PTHREAD_MUTEX_RECURSIVE,PTHREAD_MUTEX_DEFAULT*/
    int type;
} pthread_mutexattr_t;

/*!
 *  \brief
 *     互斥量内部结构: 用于系统相关的数据及句柄
 */
typedef struct
{
    /*! pshared为PTHREAD_PROCESS_PRIVATE有效,用于进程内共享*/
    os_event_t semaphor_thread;
    /*! pshared为PTHREAD_PROCESS_SHARED有效,用于进程间共享*/
    os_event_t *semaphor_process;
    /*! Depth of recursive locking */
    int m_count;
    /*! Owner thread (if recursive or errcheck) */
    unsigned int m_owner;
} pthread_mutexdata_t;

/*!
 *  \brief
 *     判断互斥量是否已经初始化的标志
 */
#define PTHREAD_MUTEX_MAGIC "MUTE"

/*!
 *  \brief
 *     互斥量句柄结构
 */
typedef struct
{
    /*! 用于判断当前条件互斥量是否已经被赋初值 */
    char magic[4];
    /*! 互斥量内部结构: 用于系统相关的数据及句柄 */
    pthread_mutexdata_t mutexdata;
    /*! 互斥量属性 */
    pthread_mutexattr_t attr;
} pthread_mutex_t;

/*!
 *  \brief
 *     条件变量属性结构
 */
typedef struct
{
    unsigned int magic;
    /*! 进程间共享属性,PTHREAD_PROCESS_SHARED--可用于进程间共享,PTHREAD_PROCESS_PRIVATE--只限于进程内使用*/
    int pshared;
} pthread_condattr_t;

/*!
 *  \brief
 *     条件变量内部结构定义:系统相关数据结构,用于系统调用时使用
 */
typedef struct
{
    /*! 当PTHREAD_PROCESS_PRIVATE时有效,系统信号结构保存在进程空间中*/
    os_event_t semaphor_thread;
    /*! 当PTHREAD_PROCESS_SHARED时有效,系统信号结构保存在系统中*/
    os_event_t *semaphor_process;
    /*! 等待在条件变量上的线程数*/
    int wait_num;
} pthread_conddata_t;

/*!
 *  \brief
 *     条件变量数据结构
 */
typedef struct
{
    /*! 用于判断当前条件变量是否已经被赋初值 */
    char magic[4];
    /*! 条件变量系统相关数据结构 */
    pthread_conddata_t conddata;
    /*! 条件变量属性结构 */
    pthread_condattr_t attr;
} pthread_cond_t;

/*
 *     线程号内部结构
 */
typedef union
{
    struct
    {
        unsigned short pid;
        unsigned short tid;
    } struct_id;
    unsigned int int_id;
} _thread_id_t;

/*!
 *  \brief
 *     线程号结构定义
 */
typedef _thread_id_t pthread_t; /* 线程ID号 */

/*!
 *  \brief
 *     线程描述符
 */
typedef struct _pthread_descr_struct *pthread_descr;

/*!
 *  \brief
 *     线程私有键结构
 */
typedef unsigned int pthread_key_t;

#endif  /*#ifndef __POSIX_TYPES_H__*/
