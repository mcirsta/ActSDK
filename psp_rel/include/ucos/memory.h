/********************************************************************************
 *                                    USDK213F
 *                                   Module: Kernel
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       fanqh     2008-06-17 15:53     1.0             build this file
 ********************************************************************************/
/*!
 * \file   memory.h
 * \brief  对case层开放的内存分配头文件
 * \author fanqh
 * \par GENERAL DESCRIPTION:
 *       这里对文件进行描述
 * \par EXTERNALIZED FUNCTIONS:
 *       这里描述调用到外面的模块
 *  \version 1.0
 *  \date  2008/6/17
 *******************************************************************************/

#ifndef __MEMORY_H__
#define __MEMORY_H__
#include <ucos/types.h>

#ifndef _ASSEMBLER_
enum cache_prio
{
    WRITE_BACK_WRITE_ALLOC,
    WRITE_THROUGHT_WRITE_ALLOC,
    WRITE_THROUGHT_NO_WRITE_ALLOC,
    NO_CACHE
};

extern void *malloc(unsigned int size);
extern void free(void * ptr);
extern void *malloc_dma(unsigned int size);
extern void free_dma(void * ptr);
extern void *realloc(void *mem_address, unsigned int newsize);
extern void *get_memory(unsigned int num_bytes, unsigned int cache_flag);
extern unsigned int os_mem_query(void);
extern void print_mem(void);

extern void *inner_kmalloc(unsigned int size, unsigned int calladdr);
extern void inner_kfree(void * ptr, unsigned int calladdr);

#endif
#endif/*__MEMORY_H__*/
