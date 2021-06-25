/********************************************************************************
 *                              USDK(1100)
 *                            Module: BUG
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-8-30 16:45     1.0             build this file
 ********************************************************************************/
/*!
 * \file     bug.h
 * \brief    bug鐩稿叧瀹忓畾涔�
 * \author   open sourc
 * \version 1.0
 * \date  2008/8/30
 *******************************************************************************/
#ifndef __ASM_GENERIC_BUG_H__
#define __ASM_GENERIC_BUG_H__

#include <ucos/compiler.h>
#include <asm-mips/config.h>
#include <ucos/kernel.h>

#ifdef CONFIG_BUG
#ifndef HAVE_ARCH_BUG
#define BUG() do { \
	printk("kernel BUG at %s:%d!\n", __FILE__, __LINE__); \
} while (0)
#endif

#ifndef HAVE_ARCH_BUG_ON
#define BUG_ON(condition) do { \
	if (unlikely((condition)!=0) != 0) { \
		printk("kernel BUG at %s:%d!\n", __FILE__, __LINE__); \
	} \
} while (0)
#endif

#ifndef HAVE_ARCH_WARN_ON
#define WARN_ON(condition) do { \
	if (unlikely((condition)!=0) != 0) { \
		printk("Badness in %s at %s:%d\n", __FUNCTION__, __FILE__, __LINE__); \
	} \
} while (0)
#endif

#else /* !CONFIG_BUG */
#ifndef HAVE_ARCH_BUG
#define BUG()
#endif

#ifndef HAVE_ARCH_BUG_ON
#define BUG_ON(condition) do { if (condition) ; } while(0)
#endif

#ifndef HAVE_ARCH_WARN_ON
#define WARN_ON(condition) do { if (condition) ; } while(0)
#endif
#endif

#endif/*__ASM_GENERIC_BUG_H__*/
