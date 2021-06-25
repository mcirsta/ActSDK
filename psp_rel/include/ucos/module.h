/********************************************************************************
 *                              USDK(1100)
 *                            Module: MODULE
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-8-30 16:45     1.0             build this file
 ********************************************************************************/
/*!
 * \file     module.h
 * \brief    module管理头文件
 * \author   hmwei
 * \version 1.0
 * \date  2008/8/30
 *******************************************************************************/
#ifndef __MODULE_H__
#define __MODULE_H__

#include <ucos/list.h>
#include <ucos/moduleparam.h>
#include <ucos/compiler.h>

#ifndef _ASSEMBLER_

#define MODULE_NAME_LEN (64 - sizeof(unsigned long))

/* special ds for flash EXPORT_SYMBOL */
struct init_ret
{
    unsigned int len;
    unsigned int _fsym;
    unsigned int _esym;
    void *pm_cb;
};

/* used for EXPORT_SYMBOL */
struct kernel_symbol
{
    unsigned long value;
    const char *name;
};

#ifndef MODULE_SYMBOL_PREFIX
#define MODULE_SYMBOL_PREFIX ""
#endif

#define __CRC_SYMBOL(sym, sec)

/* For every exported symbol, place a struct in the __ksymtab section */
#define __EXPORT_SYMBOL(sym, sec)				\
	static const char __kstrtab_##sym[]			\
	__attribute__((section("__ksymtab_strings")))		\
	= #sym;                    	\
	static const struct kernel_symbol __ksymtab_##sym	\
	__attribute_used__					\
	__attribute__((section("__ksymtab" sec), unused))	\
	= { (unsigned long)&sym, __kstrtab_##sym };

/* export api to other module in kernel. */
#define EXPORT_SYMBOL(sym)					\
	__EXPORT_SYMBOL(sym, "")

#define EXPORT_SYMBOL_GPL(sym)					\
	__EXPORT_SYMBOL(sym, "_gpl")
enum module_state
{
    MODULE_STATE_LIVE,
    MODULE_STATE_COMING,
    MODULE_STATE_GOING,
};

struct module
{
    enum module_state state;

    /* Member of list of modules */
    struct list_head list;

    /* Unique handle for this module */
    char name[MODULE_NAME_LEN];

    /* Exported symbols */
    const struct kernel_symbol *syms;
    unsigned int num_syms;
    const unsigned long *crcs;

    /* GPL-only exported symbols. */
    const struct kernel_symbol *gpl_syms;
    unsigned int num_gpl_syms;
    const unsigned long *gpl_crcs;

    /* Startup function. */
    int (*init)(void);

    /* If this is non-NULL, vfree after init() returns */
    void *module_init;

    /* Here is the actual code + data, vfree'd on unload. */
    void *module_core;

    /* Here are the sizes of the init and core sections */
    unsigned long init_size, core_size;

    /* The size of the executable code in each section.  */
    unsigned long init_text_size, core_text_size;

    /* Am I unsafe to unload? */
    int unsafe;

    /* Am I GPL-compatible */
    int license_gplok;

    //	/* Reference counts */
    //	struct module_ref ref[NR_CPUS];
    //
    //	/* What modules depend on me? */
    //	struct list_head modules_which_use_me;
    //
    //	/* Who is waiting for us to be unloaded */
    //	struct task_struct *waiter;

    /* Destruction function. */
    void (*exit)(void);
    /* The command line arguments (may be mangled).  People like
     keeping pointers to this stuff */
    char *args;
};

#define THIS_MODULE ((struct module *)0)

/* These are either module local, or the kernel's dummy ones. */
extern int init_module(void);
extern void cleanup_module(void);

extern int __link_module(void *_mod);

/* BELOW HERE ALL THESE ARE OBSOLETE AND WILL VANISH */
#if 0
struct obsolete_modparm
{
    char name[64];
    char type[64-sizeof(void *)];
    void *addr;
};
#else
struct obsolete_modparm
{
    void *addr;
    int len;
//	void *addr;
};
#endif

static inline void MODULE_PARM_(void)
{
}

#if 0
/* DEPRECATED: Do not use. */
#define MODULE_PARM(var,type)						    \
struct obsolete_modparm __parm_##var __attribute__((section("__obsparm"))) = \
{ __stringify(var), type, &MODULE_PARM_ }; \
__MODULE_PARM_TYPE(var, type);
#else
#define MODULE_PARM(var,type)						    \
struct obsolete_modparm __parm_##var __attribute__((section("__obsparm"))) = \
{ &var, sizeof(var)};

#endif

#define __MODULE_STRING(x) __stringify(x)

#endif /*_ASSEMBLER_*/
#endif  /* __MODULE_H__ */
