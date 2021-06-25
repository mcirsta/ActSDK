/********************************************************************************
 *                              USDK1100
 *                            Module: SYSTEM
 *                 Copyright(c) 2001-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2009-2-10 15:38     1.0             build this file
 ********************************************************************************/
/*!
 * \file     system.h
 * \brief    À≥–Ú∫Õ∆¡’œ
 * \author   open source
 * \version 1.0
 * \date  2009/2/10
 *******************************************************************************/
#ifndef __ASM_SYSTEM_H__
#define __ASM_SYSTEM_H__

#include <asm-mips/config.h>
#include <asm-mips/addrspace.h>
/*
 * read_barrier_depends - Flush all pending reads that subsequents reads
 * depend on.
 *
 * No data-dependent reads from memory-like regions are ever reordered
 * over this barrier.  All reads preceding this primitive are guaranteed
 * to access memory (but not necessarily other CPUs' caches) before any
 * reads following this primitive that depend on the data return by
 * any of the preceding reads.  This primitive is much lighter weight than
 * rmb() on most CPUs, and is never heavier weight than is
 * rmb().
 *
 * These ordering constraints are respected by both the local CPU
 * and the compiler.
 *
 * Ordering is not guaranteed by anything other than these primitives,
 * not even by data dependencies.  See the documentation for
 * memory_barrier() for examples and URLs to more information.
 *
 * For example, the following code would force ordering (the initial
 * value of "a" is zero, "b" is one, and "p" is "&a"):
 *
 * <programlisting>
 *	CPU 0				CPU 1
 *
 *	b = 2;
 *	memory_barrier();
 *	p = &b;				q = p;
 *					read_barrier_depends();
 *					d = *q;
 * </programlisting>
 *
 * because the read of "*q" depends on the read of "p" and these
 * two reads are separated by a read_barrier_depends().  However,
 * the following code, with the same initial values for "a" and "b":
 *
 * <programlisting>
 *	CPU 0				CPU 1
 *
 *	a = 2;
 *	memory_barrier();
 *	b = 3;				y = b;
 *					read_barrier_depends();
 *					x = a;
 * </programlisting>
 *
 * does not enforce ordering, since there is no data dependency between
 * the read of "a" and the read of "b".  Therefore, on some CPUs, such
 * as Alpha, "y" could be set to 3 and "x" to 0.  Use rmb()
 * in cases like this where there are no data dependencies.
 */

#define read_barrier_depends()	do { } while(0)

#ifdef CONFIG_CPU_HAS_SYNC
#define __sync()				\
	__asm__ __volatile__(			\
		".set	push\n\t"		\
		".set	noreorder\n\t"		\
		".set	mips2\n\t"		\
		"sync\n\t"			\
		".set	pop"			\
		: /* no output */		\
		: /* no input */		\
		: "memory")
#else
#define __sync()	do { } while(0)
#endif

#define __fast_iob()				\
	__asm__ __volatile__(			\
		".set	push\n\t"		\
		".set	noreorder\n\t"		\
		"lw	$0,%0\n\t"		\
		"nop\n\t"			\
		".set	pop"			\
		: /* no output */		\
		: "m" (*(int *)KSEG1)		\
		: "memory")

#define fast_wmb()	__sync()
#define fast_rmb()	__sync()
#define fast_mb()	__sync()
#define fast_iob()				\
	do {					\
		__sync();			\
		__fast_iob();			\
	} while (0)

#ifdef CONFIG_CPU_HAS_WB

#include <asm-mips/wbflush.h>

#define wmb()		fast_wmb()
#define rmb()		fast_rmb()
#define mb()		wbflush()
#define iob()		wbflush()

#else /* !CONFIG_CPU_HAS_WB */

#define wmb()		fast_wmb()
#define rmb()		fast_rmb()
#define mb()		fast_mb()
#define iob()		fast_iob()

#endif /* !CONFIG_CPU_HAS_WB */

#define set_mb(var, value) \
do { var = value; mb(); } while (0)

#define set_wmb(var, value) \
do { var = value; wmb(); } while (0)

#endif /* __ASM_SYSTEM_H__ */
