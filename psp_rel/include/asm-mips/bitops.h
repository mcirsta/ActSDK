/********************************************************************************
 *                              USDK130
 *                            Module: BITOPS
 *                 Copyright(c) 2001-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-6-12 9:42     1.0             build this file
 ********************************************************************************/
/*!
 * \file     bitops.h
 * \brief    位操作相关内嵌函数实现
 * \author   open source
 * \version 1.0
 * \date  2008/6/12
 *******************************************************************************/
/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (c) 1994 - 1997, 1999, 2000  Ralf Baechle (ralf@gnu.org)
 * Copyright (c) 1999, 2000  Silicon Graphics, Inc.
 */
#ifndef __ASM_BITOPS_H__
#define __ASM_BITOPS_H__

#include <asm-mips/interrupt.h>

#define SZLONG_LOG 5
#define SZLONG_MASK 31UL

/*
 * Only disable interrupt for kernel mode stuff to keep usermode stuff
 * that dares to use kernel include files alive.
 */

#define __bi_flags			unsigned long flags
#define __bi_local_irq_save(x)		local_irq_save(x)
#define __bi_local_irq_restore(x)	local_irq_restore(x)

/*
 * set_bit - Atomically set a bit in memory
 * @nr: the bit to set
 * @addr: the address to start counting from
 *
 * This function is atomic and may not be reordered.  See __set_bit()
 * if you do not require the atomic guarantees.
 * Note that @nr may be almost arbitrarily large; this function is not
 * restricted to acting on a single-word quantity.
 */
static inline void set_bit(unsigned long nr, volatile unsigned long *addr)
{
    volatile unsigned long *a = addr;
    unsigned long mask;
    __bi_flags;

    a += nr >> SZLONG_LOG;
    mask = 1UL << (nr & SZLONG_MASK);
    __bi_local_irq_save(flags);
    *a |= mask;
    __bi_local_irq_restore(flags);
}

/*
 * __set_bit - Set a bit in memory
 * @nr: the bit to set
 * @addr: the address to start counting from
 *
 * Unlike set_bit(), this function is non-atomic and may be reordered.
 * If it's called on the same region of memory simultaneously, the effect
 * may be that only one operation succeeds.
 */
static inline void __set_bit(unsigned long nr, volatile unsigned long * addr)
{
    unsigned long * m = ((unsigned long *) addr) + (nr >> SZLONG_LOG);

    *m |= 1UL << (nr & SZLONG_MASK);
}

/*
 * clear_bit - Clears a bit in memory
 * @nr: Bit to clear
 * @addr: Address to start counting from
 *
 * clear_bit() is atomic and may not be reordered.  However, it does
 * not contain a memory barrier, so if it is used for locking purposes,
 * you should call smp_mb__before_clear_bit() and/or smp_mb__after_clear_bit()
 * in order to ensure changes are visible on other processors.
 */
static inline void clear_bit(unsigned long nr, volatile unsigned long *addr)
{
    volatile unsigned long *a = addr;
    unsigned long mask;
    __bi_flags;

    a += nr >> SZLONG_LOG;
    mask = 1UL << (nr & SZLONG_MASK);
    __bi_local_irq_save(flags);
    *a &= ~mask;
    __bi_local_irq_restore(flags);

}

/*
 * __clear_bit - Clears a bit in memory
 * @nr: Bit to clear
 * @addr: Address to start counting from
 *
 * Unlike clear_bit(), this function is non-atomic and may be reordered.
 * If it's called on the same region of memory simultaneously, the effect
 * may be that only one operation succeeds.
 */
static inline void __clear_bit(unsigned long nr, volatile unsigned long * addr)
{
    unsigned long * m = ((unsigned long *) addr) + (nr >> SZLONG_LOG);

    *m &= ~(1UL << (nr & SZLONG_MASK));
}

/*
 * change_bit - Toggle a bit in memory
 * @nr: Bit to change
 * @addr: Address to start counting from
 *
 * change_bit() is atomic and may not be reordered.
 * Note that @nr may be almost arbitrarily large; this function is not
 * restricted to acting on a single-word quantity.
 */
static inline void change_bit(unsigned long nr, volatile unsigned long *addr)
{
    volatile unsigned long *a = addr;
    unsigned long mask;
    __bi_flags;

    a += nr >> SZLONG_LOG;
    mask = 1UL << (nr & SZLONG_MASK);
    __bi_local_irq_save(flags);
    *a ^= mask;
    __bi_local_irq_restore(flags);
}

/*
 * __change_bit - Toggle a bit in memory
 * @nr: the bit to change
 * @addr: the address to start counting from
 *
 * Unlike change_bit(), this function is non-atomic and may be reordered.
 * If it's called on the same region of memory simultaneously, the effect
 * may be that only one operation succeeds.
 */
static inline void __change_bit(unsigned long nr, volatile unsigned long * addr)
{
    unsigned long * m = ((unsigned long *) addr) + (nr >> SZLONG_LOG);

    *m ^= 1UL << (nr & SZLONG_MASK);
}

/*
 * test_and_set_bit - Set a bit and return its old value
 * @nr: Bit to set
 * @addr: Address to count from
 *
 * This operation is atomic and cannot be reordered.
 * It also implies a memory barrier.
 */
static inline int test_and_set_bit(unsigned long nr,
        volatile unsigned long *addr)
{
    volatile unsigned long *a = addr;
    unsigned long mask;
    int retval;
    __bi_flags;

    a += nr >> SZLONG_LOG;
    mask = 1UL << (nr & SZLONG_MASK);
    __bi_local_irq_save(flags);
    retval = (mask & *a) != 0;
    *a |= mask;
    __bi_local_irq_restore(flags);

    return retval;
}

/*
 * __test_and_set_bit - Set a bit and return its old value
 * @nr: Bit to set
 * @addr: Address to count from
 *
 * This operation is non-atomic and can be reordered.
 * If two examples of this operation race, one can appear to succeed
 * but actually fail.  You must protect multiple accesses with a lock.
 */
static inline int __test_and_set_bit(unsigned long nr,
        volatile unsigned long *addr)
{
    volatile unsigned long *a = addr;
    unsigned long mask;
    int retval;

    a += nr >> SZLONG_LOG;
    mask = 1UL << (nr & SZLONG_MASK);
    retval = (mask & *a) != 0;
    *a |= mask;

    return retval;
}

/*
 * test_and_clear_bit - Clear a bit and return its old value
 * @nr: Bit to clear
 * @addr: Address to count from
 *
 * This operation is atomic and cannot be reordered.
 * It also implies a memory barrier.
 */
static inline int test_and_clear_bit(unsigned long nr,
        volatile unsigned long *addr)
{
    volatile unsigned long *a = addr;
    unsigned long mask;
    int retval;
    __bi_flags;

    a += nr >> SZLONG_LOG;
    mask = 1UL << (nr & SZLONG_MASK);
    __bi_local_irq_save(flags);
    retval = (mask & *a) != 0;
    *a &= ~mask;
    __bi_local_irq_restore(flags);

    return retval;
}

/*
 * __test_and_clear_bit - Clear a bit and return its old value
 * @nr: Bit to clear
 * @addr: Address to count from
 *
 * This operation is non-atomic and can be reordered.
 * If two examples of this operation race, one can appear to succeed
 * but actually fail.  You must protect multiple accesses with a lock.
 */
static inline int __test_and_clear_bit(unsigned long nr,
        volatile unsigned long * addr)
{
    volatile unsigned long *a = addr;
    unsigned long mask;
    int retval;

    a += (nr >> SZLONG_LOG);
    mask = 1UL << (nr & SZLONG_MASK);
    retval = ((mask & *a) != 0);
    *a &= ~mask;

    return retval;
}

/*
 * test_and_change_bit - Change a bit and return its old value
 * @nr: Bit to change
 * @addr: Address to count from
 *
 * This operation is atomic and cannot be reordered.
 * It also implies a memory barrier.
 */
static inline int test_and_change_bit(unsigned long nr,
        volatile unsigned long *addr)
{
    volatile unsigned long *a = addr;
    unsigned long mask, retval;
    __bi_flags;

    a += nr >> SZLONG_LOG;
    mask = 1UL << (nr & SZLONG_MASK);
    __bi_local_irq_save(flags);
    retval = (mask & *a) != 0;
    *a ^= mask;
    __bi_local_irq_restore(flags);

    return retval;
}

/*
 * __test_and_change_bit - Change a bit and return its old value
 * @nr: Bit to change
 * @addr: Address to count from
 *
 * This operation is non-atomic and can be reordered.
 * If two examples of this operation race, one can appear to succeed
 * but actually fail.  You must protect multiple accesses with a lock.
 */
static inline int __test_and_change_bit(unsigned long nr,
        volatile unsigned long *addr)
{
    volatile unsigned long *a = addr;
    unsigned long mask;
    int retval;

    a += (nr >> SZLONG_LOG);
    mask = 1UL << (nr & SZLONG_MASK);
    retval = ((mask & *a) != 0);
    *a ^= mask;

    return retval;
}

#undef __bi_flags
#undef __bi_local_irq_save
#undef __bi_local_irq_restore

/*
 * test_bit - Determine whether a bit is set
 * @nr: bit number to test
 * @addr: Address to start counting from
 */
static inline int test_bit(unsigned long nr, const volatile unsigned long *addr)
{
    return 1UL & (addr[nr >> SZLONG_LOG] >> (nr & SZLONG_MASK));
}

#endif /* __ASM_BITOPS_H__ */

