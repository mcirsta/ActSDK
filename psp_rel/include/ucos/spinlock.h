/********************************************************************************
 *                              USDK(1100)
 *                            Module: SPINLOCK
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-8-30 16:44     1.0             build this file
 ********************************************************************************/
/*!
 * \file     spinlock.h
 * \brief    spinlock头文件
 * \author   open source
 * \version 1.0
 * \date  2008/8/30
 *******************************************************************************/
#ifndef __SPINLOCK_H__
#define __SPINLOCK_H__

#ifndef _ASSEMBLER_

/*!
 * \brief
 *      spinlock结构定义.
 */
typedef struct
{
    /*! lock计数值*/
    volatile unsigned int lock;
} spinlock_t;

#define spin_lock_init(x)	do { (x)->lock = 0; } while(0)
#define spin_is_locked(x)	((x)->lock != 0)

#define SPIN_LOCK_UNLOCKED (spinlock_t) { 0 }

#define spin_unlock_wait(x)	({ do { barrier(); } while ((x)->lock); })

/*
 * Read-write spinlocks, allowing multiple readers but only one writer.
 *
 * NOTE! it is quite common to have readers in interrupts but no interrupt
 * writers. For those circumstances we can "mix" irq-safe locks - any writer
 * needs to get a irq-safe write-lock, but readers can get non-irqsafe
 * read-locks.
 */

typedef struct
{
    volatile unsigned int lock;
} rwlock_t;

#define RW_LOCK_UNLOCKED (rwlock_t) { 0 }

#define rwlock_init(x)  do { *(x) = RW_LOCK_UNLOCKED; } while(0)

#include <asm-mips/spinlock.h>

extern void _spin_lock(spinlock_t *lock);
extern void _spin_unlock(spinlock_t *lock);
extern void _spin_lock_irq(spinlock_t *lock);
extern void _spin_unlock_irq(spinlock_t *lock);
extern unsigned long _spin_lock_irqsave(spinlock_t *lock);
extern void _spin_unlock_irqrestore(spinlock_t *lock, unsigned long flags);

#define spin_lock(lock)			_spin_lock(lock)
#define spin_unlock(lock)			_spin_unlock(lock)

#define spin_lock_irq(lock)		_spin_lock_irq(lock)
#define spin_unlock_irq(lock)		_spin_unlock_irq(lock)

#define spin_lock_irqsave(lock, flags)	flags = _spin_lock_irqsave(lock)
#define spin_unlock_irqrestore(lock, flags)	_spin_unlock_irqrestore(lock, flags)

#endif /*_ASSEMBLER_*/
#endif /* __SPINLOCK_H__ */
