/*******************************************************************************
 *                              UDFK213F
 *                            Module: DL
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-06-12 9:42     1.0             build this file
 *******************************************************************************/
/*!
 * \file     dlfcn.h
 * \brief    共享库相关数据结构和函数接口声明
 * \author   hmwei
 * \version 1.0
 * \date  2007/5/4
 *******************************************************************************/
#ifndef __OS_DLFCN_H__
#define __OS_DLFCN_H__

#include <ucos/list.h>
#include <ucos/compiler.h>
//#include <ucos/page.h>

#define MAX_DLL_BUCKETS 10

/* Collect various system dependent definitions and declarations.  */
/* The MODE argument to `dlopen' contains one of the following: */
#define RTLD_LAZY   0x0001  /* Lazy function call binding.  */
#define RTLD_NOW    0x0002  /* Immediate function call binding.  */

#ifndef _ASSEMBLER_

struct dll_symbol
{
    unsigned long value;
    const char *name;
};
/* export api to exe or other dl. */
#define DL_EXPORT_SYMBOL(sym)                   \
    static const char __dlstrtab_##sym[]            \
    __attribute__((section(".dlstr")))      \
    = #sym;                     \
    static const struct dll_symbol __dlsymtab_##sym \
    __attribute_used__                  \
    __attribute__((section(".dlsym"), unused))  \
    = { (unsigned long)&sym, __dlstrtab_##sym };

/*!
 * \brief
 *      进程加载的共享库链表头
 */
struct dll_process
{
    /*! 进程号*/
    int pid;
    /*! 链表头节点*/
    struct list_head list;
};

/*!
 * \brief
 *      单个共享库在多个进程中加载后的共享信息数据结构
 */
struct dll_share_struct
{
    /*! 管理共享库导出符号的哈希表首址*/
    void *hash;
    /*! 共享库的符号区首地址*/
    const struct dll_symbol *syms;
    /*! 共享库的符号数*/
    unsigned int nchains;
    /*! 共享库的bucket数*/
    unsigned int nbuckets;
    /*! 校验和（未使用）*/
    const unsigned long *crcs;
    /*! 初始化函数指针*/
    void (*_init)(void);
    /*! 退出函数指针*/
    void (*_fini)(void);
    /*! 共享库在所有进程内加载的次数*/
    int count;
};

/*!
 * \brief
 *      单个共享库在单个进程内的实体数据结构
 */
struct dll
{
    /* 共享库的名称链表头指针 */
    struct dll_name *dll_name_info;
    /* 共享库的进程链表头指针 */
    struct dll_process *dll_pid_info;
    /*  共享库的名称链表节点*/
    struct list_head process_list;
    /* 共享库的进程链表节点 */
    struct list_head name_list;
    /* 共享库的共享信息结构指针 */
    struct dll_share_struct *dll_share_info;
    /* 单个进程内加载该共享库的次数 */
    int count;
    /* 首次加载标志 */
    unsigned long rtld_flags;
};

extern void *dlopen(const char *file, int mode);
extern int dlclose(void *handle);
extern void *dlsym(void *handle, const char *name);
extern char *dlerror(void);

extern int find_index_by_name(const char *name);
extern struct dll_share_struct *get_dll_share_info(char *name);
extern int __link_dll(struct dll *_dl, char *name, int pid);
extern void insert_dll_symbol(struct dll_symbol *dl, unsigned int index,
        struct dll *dll);
extern void free_dll_by_pid(int pid);
#endif /*_ASSEMBLER_*/

#endif  /* __OS_DLFCN_H__ */
