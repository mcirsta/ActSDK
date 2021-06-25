/********************************************************************************
 *                              USDK1100
 *                            Module: WBFLUSH
 *                 Copyright(c) 2001-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2009-2-10 15:38     1.0             build this file
 ********************************************************************************/
/*!
 * \file     system.h
 * \brief    write buf
 * \author   open source
 * \version 1.0
 * \date  2009/2/10
 *******************************************************************************/
#ifndef __ASM_WBFLUSH_H__
#define __ASM_WBFLUSH_H__

#include <asm-mips/config.h>

#ifdef CONFIG_CPU_HAS_WB

extern void (*__wbflush)(void);
extern void wbflush_setup(void);

#define wbflush()			\
	do {				\
		__sync();		\
		__wbflush();		\
	} while (0)

#else /* !CONFIG_CPU_HAS_WB */

#define wbflush_setup() do { } while (0)

#define wbflush() fast_iob()

#endif /* !CONFIG_CPU_HAS_WB */

#endif /* __ASM_WBFLUSH_H__ */
