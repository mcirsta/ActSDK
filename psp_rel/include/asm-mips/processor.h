/********************************************************************************
 *                              USDK(1100)
 *                            Module: PROCESS
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-8-30 16:45     1.0             build this file
 ********************************************************************************/
/*!
 * \file     processor.h
 * \brief    mips架构相关头文件
 * \author   linux
 * \version 1.0
 * \date  2008/8/30
 *******************************************************************************/
#ifndef __ASM_PROCESSOR_H__
#define __ASM_PROCESSOR_H__

#ifndef _ASSEMBLER_

#include <asm-mips/config.h>
#include <asm-mips/prefetch.h>

/*
 * Return_address is a replacement for __builtin_return_address(count)
 * which on certain architectures cannot reasonably be implemented in GCC
 * (MIPS, Alpha) or is unuseable with -fomit-frame-pointer (i386).
 * Note that __builtin_return_address(x>=1) is forbidden because GCC
 * aborts compilation on some CPUs.  It's simply not possible to unwind
 * some CPU's stackframes.
 *
 * __builtin_return_address works only for non-leaf functions.  We avoid the
 * overhead of a function call by forcing the compiler to save the return
 * address register on the stack.
 */
#define return_address() ({__asm__ __volatile__("":::"$31");__builtin_return_address(0);})

//	extern inline void prefetch(const void *addr)
static inline void prefetch(const void *addr)
{
    __asm__ __volatile__(
            "	.set	mips4		\n"
            "	pref	%0, (%1)	\n"
            "	.set	mips0		\n"
            :
            : "i" (Pref_Load), "r" (addr));
}
#endif
#endif /* __ASM_PROCESSOR_H__ */

