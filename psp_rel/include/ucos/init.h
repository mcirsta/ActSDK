/********************************************************************************
 *                              USDK(1100)
 *                            Module: INIT
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-8-30 16:45     1.0             build this file
 ********************************************************************************/
/*!
 * \file     init.h
 * \brief    module_init/_exit∫Í∂®“Â
 * \author   hmwei
 * \version 1.0
 * \date  2008/8/30
 *******************************************************************************/

#ifndef __INIT_H__
#define __INIT_H__

#define __init	__attribute__ ((__section__ (".init")))
#define __exit	__attribute__ ((__section__ (".fini")))

typedef int (*initcall_t)(void); /* module init function type. */
typedef void (*exitcall_t)(void); /* module exit function type. */

/* Each module must use one module_init(), or one no_module_init */
#define module_init(initfn)					\
	static inline initcall_t __inittest(void)		\
	{ return initfn; }					\
	int init_module(void) __attribute__((alias(#initfn)));

/* This is only required if you want to be unloadable. */
#define module_exit(exitfn)					\
	static inline exitcall_t __exittest(void)		\
	{ return exitfn; }					\
	void cleanup_module(void) __attribute__((alias(#exitfn)));

#endif  /* __INIT_H__ */
