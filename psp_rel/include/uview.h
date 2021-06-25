/*******************************************************************************
 *                              USDK213F
 *                            Module: UVIEW
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2009-7-26 15:14     1.0             build this file
 *******************************************************************************/
/*!
 * \file     uview.h
 * \brief    uview
 * \author   hmwei
 * \version 1.0
 * \date  2007/5/4
 *******************************************************************************/
#ifndef __UVIEW_H__
#define __UVIEW_H__

#define MACRO_SCH_INT 0x101

#ifndef _ASSEMBLER_

/*!
 *  \brief
 *  uview的数据/事件类型
 *  \li 0x0~0x4f : 应用程序的事件类型
 *  \li 0x80~0xff : 内核模块的事件类型
 *  \li 0x100~ : 内核模块的其它事件类型
 */
enum uview_type
{
    /*! 应用程序数据类型*/
    PROCESS_CREATE = 0x0,
    MUTEX_CREATE = 0x10,
    MUTEX_DELETE,
    MUTEX_LOCK,
    MUTEX_UNLOCK,
    SEM_CREATE = 0x20,
    SEM_DELETE,
    SEM_WAIT,
    SEM_TIMEDWAIT,
    SEM_POST,
    COND_INIT = 0x30,
    COND_DELETE,
    COND_WAIT,
    COND_TIMEDWAIT,
    COND_BRODCAST,
    COND_SIGNAL,
    MQ_CREATE = 0x40,
    MQ_DELETE,
    MQ_WAIT,
    MQ_TIMEDWAIT,
    MQ_SEND,
    MQ_TIMEDSEND,
    /*! 内核模块数据类型*/
    T_CREATE = 0x80,
    T_DELETE,
    T_PRIO_CHANGE,
    T_RESUME,
    T_SUSPEND,
    T_SCHED,
    S_CREATE = 0x90,
    S_DELETE,
    S_PEND,
    S_POST,
    Q_CREATE = 0xa0,
    Q_DELETE,
    Q_PEND,
    Q_POST,
    F_CREATE = 0xb0,
    F_DELETE,
    F_PEND,
    F_POST,
    /*! 内核模块的其它事件类型*/
    PM_CHANGE = 0x100,
    SCH_INT,
    SCH_TIMEOUT,
    SCH_UNLOCK,
    SCH_PTHEAD_ACTIVE
};

/*!
 * \brief
 *      线程基本信息数据结构
 */
struct thread_base_info
{
    /*! 进程id*/
    unsigned short pid;
    /*! 线程id*/
    unsigned short tid;
    /*! 当前任务优先级*/
    unsigned char prio;
};

/*!
 * \brief
 *      线程扩展信息数据结构
 */
struct thread_extra_info
{
    /*! 栈顶*/
    unsigned int stk_ptr;
    /*! 栈长度*/
    unsigned int stk_size;
    /*! 线程所在进程的内存占用长度*/
    unsigned int memory_size;
};

/*!
 * \brief
 *      线程信息数据结构
 */
struct thread_info
{
    /*! 线程基本信息*/
    struct thread_base_info base;
    /*! 线程扩展信息*/
    struct thread_extra_info extra;
};

/*!
 * \brief
 *      线程调度原因数据结构
 */
struct thread_sched_cause
{
    /*! 调度原因*/
    unsigned int cause;
    /*! 调度事件*/
    void *event;
};

/*!
 * \brief
 *      PM模式调频信息数据结构
 */
struct pm_info
{
    /*! cpu频率值*/
    unsigned int cpuclk;
    /*! vdd值*/
    unsigned int vdd;
    /*! vcc值*/
    unsigned int vcc;
};

/*!
 * \brief
 *      发送给usdkview的数据的结构定义
 */
struct usdkview_date
{
    /*! 数据类型*/
    unsigned int type;
    /*! 时间（上电后绝对时间ms）*/
    unsigned int time;
    /*! 时间（上电后绝对tick数）*/
    unsigned long long tick;
    /*! 数据包总长度*/
    unsigned int len;
};

/*!
 * \brief
 *      进程（主线程）创建数据结构
 */
struct process_create_info
{
    /*! 进程名*/
    char name[16];
    /*! 进程的主线程基本信息*/
    struct thread_info main_thread;
};

/*!
 * \brief
 *      线程删除数据结构
 */
struct thread_del_info
{
    /*! 线程基本信息*/
    struct thread_base_info thread;
};

/*!
 * \brief
 *      线程优先级改变数据结构
 */
struct thread_change_info
{
    /*! 当前线程信息*/
    struct thread_info cur;
    /*! 新线程信息*/
    struct thread_info new;
};

/*!
 * \brief
 *      线程调度数据结构
 */
struct thread_sw_info
{
    /*! 当前线程信息*/
    struct thread_info cur;
    /*! 新线程信息*/
    struct thread_info new;
    /*! 调度原因*/
    struct thread_sched_cause cause;
};

/*!
 * \brief
 *      event信息数据结构
 */
struct event_info
{
    /*! 基于ucos_event扩展的pthread_event指针*/
    void *pthread_event;
    /*! ucos_event指针*/
    void *ucos_event;
};

/*!
 * \brief
 *      PM模式调频信息数据结构
 */
struct pm_change_info
{
    /*! 新频率信息*/
    struct pm_info new;
};

/*
 1.启动usdkview时，先将当前信息组包发送给pc
 2.实时模式：buf满时，发送
 3.缓冲模式：停止时，发送
 */

#endif /*_ASSEMBLER_*/
#endif /*__UVIEW_H__*/
