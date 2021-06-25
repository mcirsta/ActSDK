/********************************************************************************
 *                               USDK(1100)
 *                            Module: WORKQUEUE
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-6-12 9:42     1.0             build this file
 ********************************************************************************/
/*!
 * \file     workqueue.h
 * \brief    linux工作队列的移植
 * \author   hmwei
 * \version 1.0
 * \date  2008/6/12
 *******************************************************************************/
#ifndef __WORKQUEUE_H__
#define __WORKQUEUE_H__

#include <asm-mips/bitops.h>
#include <ucos/list.h>
#include <ucos/timer.h>

#ifndef _ASSEMBLER_

struct workqueue_struct;

struct work_struct
{
    unsigned long pending;
    struct list_head entry;
    void (*func)(void *);
    void *data;
    void *wq_data;
    struct timer_list timer;
};

#define __WORK_INITIALIZER(n, f, d) {				\
	.entry	= { &(n).entry, &(n).entry },			\
	.func = (f),						\
	.data = (d),						\
	}

#define DECLARE_WORK(n, f, d)					\
	struct work_struct n = __WORK_INITIALIZER(n, f, d)

/*
 * initialize a work-struct's func and data pointers:
 */
#define PREPARE_WORK(_work, _func, _data)			\
	do {							\
		(_work)->func = _func;				\
		(_work)->data = _data;				\
	} while (0)

/*
 * initialize all of a work-struct:
 */
#define INIT_WORK(_work, _func, _data)				\
	do {							\
		INIT_LIST_HEAD(&(_work)->entry);		\
		(_work)->pending = 0;				\
		PREPARE_WORK((_work), (_func), (_data));	\
		init_timer(&(_work)->timer);			\
	} while (0)

extern struct workqueue_struct *__create_workqueue(const char *name,
        int singlethread);

#define create_workqueue(name) __create_workqueue((name), 0)
extern void destroy_workqueue(struct workqueue_struct *wq);

extern int queue_work(struct workqueue_struct *wq, struct work_struct *work);
extern int queue_delayed_work(struct workqueue_struct *wq,
        struct work_struct *work, unsigned long delay);
extern void flush_workqueue(struct workqueue_struct *wq);

extern int schedule_work(struct work_struct *work);
extern int schedule_delayed_work(struct work_struct *work, unsigned long delay);

//	extern int schedule_delayed_work_on(int cpu, struct work_struct *work, unsigned long delay);
extern void flush_scheduled_work(void);

extern void init_workqueues(void);
void cancel_rearming_delayed_work(struct work_struct *work);
void cancel_rearming_delayed_workqueue(struct workqueue_struct *,
        struct work_struct *);

/*
 * Kill off a pending schedule_delayed_work().  Note that the work callback
 * function may still be running on return from cancel_delayed_work().  Run
 * flush_scheduled_work() to wait on it.
 */
static inline int cancel_delayed_work(struct work_struct *work)
{
    int ret;

    ret = del_timer_sync(&work->timer);
    if (ret)
        clear_bit(0, &work->pending);
    return ret;
}
#endif /*_ASSEMBLER_*/
#endif/*__WORKQUEUE_H__*/
