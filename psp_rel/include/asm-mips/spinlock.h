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
 * \brief    璇�-鍐欒嚜鏃嬮攣鍐呰仈鍑芥暟鐨勫疄鐜�
 * \author   open source
 * \version 1.0
 * \date  2008/8/30
 *******************************************************************************/
#ifndef __ASM_SPINLOCK_H__
#define __ASM_SPINLOCK_H__

#if 0
static inline void read_lock(rwlock_t *rw)
{
    unsigned int tmp;

    __asm__ __volatile__(
            ".set\tnoreorder\t\t\t# read_lock\n"
            "1:\tll\t%1, %2\n\t"
            "bltz\t%1, 1b\n\t"
            " addu\t%1, 1\n\t"
            "sc\t%1, %0\n\t"
            "beqz\t%1, 1b\n\t"
            " sync\n\t"
            ".set\treorder"
            : "=m" (rw->lock), "=&r" (tmp)
            : "m" (rw->lock)
            : "memory");
}

/* Note the use of sub, not subu which will make the kernel die with an
 overflow exception if we ever try to unlock an rwlock that is already
 unlocked or is being held by a writer.  */
static inline void read_unlock(rwlock_t *rw)
{
    unsigned int tmp;

    __asm__ __volatile__(
            ".set\tnoreorder\t\t\t# read_unlock\n"
            "1:\tll\t%1, %2\n\t"
            "sub\t%1, 1\n\t"
            "sc\t%1, %0\n\t"
            "beqz\t%1, 1b\n\t"
            " sync\n\t"
            ".set\treorder"
            : "=m" (rw->lock), "=&r" (tmp)
            : "m" (rw->lock)
            : "memory");
}

static inline void write_lock(rwlock_t *rw)
{
    unsigned int tmp;

    __asm__ __volatile__(
            ".set\tnoreorder\t\t\t# write_lock\n"
            "1:\tll\t%1, %2\n\t"
            "bnez\t%1, 1b\n\t"
            " lui\t%1, 0x8000\n\t"
            "sc\t%1, %0\n\t"
            "beqz\t%1, 1b\n\t"
            " sync\n\t"
            ".set\treorder"
            : "=m" (rw->lock), "=&r" (tmp)
            : "m" (rw->lock)
            : "memory");
}

static inline void write_unlock(rwlock_t *rw)
{
    __asm__ __volatile__(
            ".set\tnoreorder\t\t\t# write_unlock\n\t"
            "sync\n\t"
            "sw\t$0, %0\n\t"
            ".set\treorder"
            : "=m" (rw->lock)
            : "m" (rw->lock)
            : "memory");
}
#endif

#endif /* __ASM_SPINLOCK_H__ */
