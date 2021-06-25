/********************************************************************************
 *                              USDK(1100)
 *                            Module: INTERRUPT
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-8-30 16:45     1.0             build this file
 ********************************************************************************/
/*!
 * \file     interrupt.h
 * \brief    开关中断头文件
 * \author   open source
 * \version 1.0
 * \date  2008/8/30
 *******************************************************************************/
/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1994, 95, 96, 97, 98, 99, 2003 by Ralf Baechle
 * Copyright (C) 1996 by Paul M. Antoine
 * Copyright (C) 1999 Silicon Graphics
 * Copyright (C) 2000 MIPS Technologies, Inc.
 */
#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include <ucos/types.h>
#include <asm-mips/atomic.h>
#include <asm-mips/bitops.h>
#include <asm-mips/interrupt.h>

#ifndef _ASSEMBLER_

enum
{
    HI_SOFTIRQ = 0,
    TIMER_SOFTIRQ,
    NET_TX_SOFTIRQ,
    NET_RX_SOFTIRQ,
    SCSI_SOFTIRQ,
    TASKLET_SOFTIRQ
};

struct softirq_action
{
    void (*action)(struct softirq_action *);
    void *data;
};

extern __u32 softirq_pending;

#define local_softirq_pending() softirq_pending
#define set_softirq_pending(x) (local_softirq_pending() = (x))
#define or_softirq_pending(x)  (local_softirq_pending() |= (x))

extern void do_softirq(void);
extern void open_softirq(int nr, void(*action)(struct softirq_action*),
        void *data);
#define __raise_softirq_irqoff(nr) do { or_softirq_pending(1UL << (nr)); } while (0)
extern void softirq_init(void);
extern void raise_softirq(unsigned int nr);

/* Tasklets --- multithreaded analogue of BHs.

 Main feature differing them of generic softirqs: tasklet
 is running only on one CPU simultaneously.

 Main feature differing them of BHs: different tasklets
 may be run simultaneously on different CPUs.

 Properties:
 * If tasklet_schedule() is called, then tasklet is guaranteed
 to be executed on some cpu at least once after this.
 * If the tasklet is already scheduled, but its excecution is still not
 started, it will be executed only once.
 * If this tasklet is already running on another CPU (or schedule is called
 from tasklet itself), it is rescheduled for later.
 * Tasklet is strictly serialized wrt itself, but not
 wrt another tasklets. If client needs some intertask synchronization,
 he makes it with spinlocks.
 */

struct tasklet_struct
{
    struct tasklet_struct *next;
    unsigned long state;
    atomic_t count;
    void (*func)(unsigned long);
    unsigned long data;
};

#define DECLARE_TASKLET(name, func, data) \
struct tasklet_struct name = { NULL, 0, ATOMIC_INIT(0), func, data }

#define DECLARE_TASKLET_DISABLED(name, func, data) \
struct tasklet_struct name = { NULL, 0, ATOMIC_INIT(1), func, data }

enum
{
    TASKLET_STATE_SCHED, /* Tasklet is scheduled for execution */
    TASKLET_STATE_RUN
/* Tasklet is running (SMP only) */
};

#define tasklet_trylock(t) 1
#define tasklet_unlock_wait(t) do { } while (0)
#define tasklet_unlock(t) do { } while (0)

extern void __tasklet_schedule(struct tasklet_struct *t);

static inline void tasklet_schedule(struct tasklet_struct *t)
{
    if (!test_and_set_bit((unsigned long) TASKLET_STATE_SCHED, &t->state))
        __tasklet_schedule(t);
}

extern void __tasklet_hi_schedule(struct tasklet_struct *t);

static inline void tasklet_hi_schedule(struct tasklet_struct *t)
{
    if (!test_and_set_bit(TASKLET_STATE_SCHED, &t->state))
        __tasklet_hi_schedule(t);
}

static inline void tasklet_disable_nosync(struct tasklet_struct *t)
{
    atomic_inc(&t->count);
}

static inline void tasklet_disable(struct tasklet_struct *t)
{
    tasklet_disable_nosync(t);
    tasklet_unlock_wait(t);
}

static inline void tasklet_enable(struct tasklet_struct *t)
{
    atomic_dec(&t->count);
}

extern void tasklet_kill(struct tasklet_struct *t);
extern void tasklet_init(struct tasklet_struct *t, void(*func)(unsigned long),
        unsigned long data);

#endif /*_ASSEMBLER_*/
#endif /* __INTERRUPT_H__ */
