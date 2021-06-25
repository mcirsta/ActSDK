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
 * \brief    �����������ݽṹ�ͺ����ӿ�����
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
 *      ���̼��صĹ��������ͷ
 */
struct dll_process
{
    /*! ���̺�*/
    int pid;
    /*! ����ͷ�ڵ�*/
    struct list_head list;
};

/*!
 * \brief
 *      ����������ڶ�������м��غ�Ĺ�����Ϣ���ݽṹ
 */
struct dll_share_struct
{
    /*! ������⵼�����ŵĹ�ϣ����ַ*/
    void *hash;
    /*! �����ķ������׵�ַ*/
    const struct dll_symbol *syms;
    /*! �����ķ�����*/
    unsigned int nchains;
    /*! ������bucket��*/
    unsigned int nbuckets;
    /*! У��ͣ�δʹ�ã�*/
    const unsigned long *crcs;
    /*! ��ʼ������ָ��*/
    void (*_init)(void);
    /*! �˳�����ָ��*/
    void (*_fini)(void);
    /*! ����������н����ڼ��صĴ���*/
    int count;
};

/*!
 * \brief
 *      ����������ڵ��������ڵ�ʵ�����ݽṹ
 */
struct dll
{
    /* ��������������ͷָ�� */
    struct dll_name *dll_name_info;
    /* �����Ľ�������ͷָ�� */
    struct dll_process *dll_pid_info;
    /*  ��������������ڵ�*/
    struct list_head process_list;
    /* �����Ľ�������ڵ� */
    struct list_head name_list;
    /* �����Ĺ�����Ϣ�ṹָ�� */
    struct dll_share_struct *dll_share_info;
    /* ���������ڼ��ظù����Ĵ��� */
    int count;
    /* �״μ��ر�־ */
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
