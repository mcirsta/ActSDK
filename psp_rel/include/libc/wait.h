/********************************************************************************
 *                              USDK(1100)
 *                            Module: WAIT
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-8-30 16:42     1.0             build this file
 ********************************************************************************/
/*!
 * \file     wait.h
 * \brief    等待队列头文件
 * \author   linux
 * \version 1.0
 * \date  2008-8-30 16:42
 *******************************************************************************/
#ifndef _LINUX_WAIT_H
#define _LINUX_WAIT_H

#define WNOHANG		0x00000001
#define WUNTRACED	0x00000002
#define WSTOPPED	WUNTRACED
#define WEXITED		0x00000004
#define WCONTINUED	0x00000008
#define WNOWAIT		0x01000000	/* Don't reap, just poll status.  */

#define __WNOTHREAD	0x20000000	/* Don't wait on children of other threads in this group */
#define __WALL		0x40000000	/* Wait on all children, regardless of type */
#define __WCLONE	0x80000000	/* Wait only on non-SIGCHLD children */

/* First argument to waitid: */
#define P_ALL		0
#define P_PID		1
#define P_PGID		2

#include "asm-mips/config.h"
#include "ucos/list.h"
//#include "stddef.h"
#include "ucos/spinlock.h"
//#include "system.h"
//#include "current.h"
#include "sched.h"
#include <libc/sys/wait.h>

typedef struct __wait_queue wait_queue_t;
typedef int (*wait_queue_func_t)(wait_queue_t *wait, unsigned mode, int sync,
        void *key);
int default_wake_function(wait_queue_t *wait, unsigned mode, int sync,
        void *key);

struct __wait_queue
{
    unsigned int flags;
#define WQ_FLAG_EXCLUSIVE	0x01
    void *private;
    wait_queue_func_t func;
    struct list_head task_list;
};

struct wait_bit_key
{
    void *flags;
    int bit_nr;
};

struct wait_bit_queue
{
    struct wait_bit_key key;
    wait_queue_t wait;
};

/*
 *      等待队列头结构定义.
 */
struct __wait_queue_head
{
    /* spinlock锁*/
    spinlock_t lock;
    /* 等待队列的任务链表头结点*/
    struct list_head task_list;
};

/*!
 *  \brief
 *      wait_queue_head_t - struct __wait_queue_head
 */
typedef struct __wait_queue_head wait_queue_head_t;

/*
 * Macros for declaration and initialisaton of the datatypes
 */

#define __WAITQUEUE_INITIALIZER(name, tsk) {				\
	.private	= tsk,						\
	.func		= default_wake_function,			\
	.task_list	= { NULL, NULL } }

#define DECLARE_WAITQUEUE(name, tsk)					\
	wait_queue_t name = __WAITQUEUE_INITIALIZER(name, tsk)

#define __WAIT_QUEUE_HEAD_INITIALIZER(name) {				\
	.lock		= SPIN_LOCK_UNLOCKED,				\
	.task_list	= { &(name).task_list, &(name).task_list } }

#define DECLARE_WAIT_QUEUE_HEAD(name) \
	wait_queue_head_t name = __WAIT_QUEUE_HEAD_INITIALIZER(name)

#define __WAIT_BIT_KEY_INITIALIZER(word, bit)				\
	{ .flags = word, .bit_nr = bit, }

/*! \cond KMODULE_API*/
/******************************************************************************/
/*!
 * \par  Description:
 *	  初始化等待队列头节点
 * \param[in]    q  wait_queue_head_t类型指针
 * \return       null
 * \ingroup      module name
 * \par          exmaple code
 * \code
 wait_queue_head_t wait_queue_head;
 init_waitqueue_head(&wait_queue_head);
 * \endcode
 *******************************************************************************/
static inline void init_waitqueue_head(wait_queue_head_t *q)
{
    spin_lock_init(&q->lock);
    INIT_LIST_HEAD(&q->task_list);
}
/*! \endcond */

/*!
 * \brief
 *      wake up threads blocked on a waitqueue
 * \li  x: the waitqueue
 * \par          exmaple code
 * \code
 线程1：
 volatile int condition = 0;
 wait_queue_head_t wait_queue_head;
 init_waitqueue_head(&wait_queue_head);
 wait_event_interruptible(wait_queue_head, condition);

 线程2：
 wake_up_interruptible(&wait_queue_head);
 * \endcode
 */
#define wake_up_interruptible(x)	__wake_up(x, TASK_INTERRUPTIBLE, 1, NULL)

#define __wait_event_interruptible(wq, condition, ret)			\
do {									\
} while (0)

/*!
 * \brief
 *      sleep until a condition gets true
 * \li  wq: the waitqueue to wait on
 * \li  condition: a C expression for the event to wait for
 * \par          exmaple code
 * \code
 volatile int condition = 0;
 wait_queue_head_t wait_queue_head;
 init_waitqueue_head(&wait_queue_head);
 wait_event_interruptible(wait_queue_head, condition);
 * \endcode
 */
#define wait_event_interruptible(wq, condition)				\
({									\
	int __ret = 0;							\
	if (!(condition))						\
		__wait_event_interruptible(wq, condition, __ret);	\
	__ret;								\
})

#define __wait_event_interruptible_timeout(wq, condition, ret)		\
do {									\
} while (0)

#define wait_event_interruptible_timeout(wq, condition, timeout)	\
({									\
	long __ret = timeout;						\
	if (!(condition))						\
		__wait_event_interruptible_timeout(wq, condition, __ret); \
	__ret;								\
})

#endif
