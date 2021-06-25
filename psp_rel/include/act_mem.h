/********************************************************************************
 *                              usdk1100
 *                            Module: act_decoder.c
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       huixu    2008-12-01 15:00     1.0             build this file
 ********************************************************************************/

/*!
 * \file     act_mem.h
 * \brief    中间件内存分配接口
 * \author   候景昆 徐会
 * \version 1.0
 * \date  2008/12/01
 *******************************************************************************/

#ifndef __ACT_MEM_H__
#define __ACT_MEM_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*! 分配内存类型的标志, 用bit 0来表示是否是连续空间或是非连续空间*/
#define MEM_DISCRETE    0x00000000
#define MEM_CONTINUOUS  0x00000001
/*! 内存类型标识的mask*/
#define MEM_TYPE_MASK   0x00000001

/*! 内存cache方式的标识, 用bit 1和2来标识*/
/*! uncache*/
#define UNCACHE_MEM         0x00000000
/*! write through, no write allocate*/
#define WRITE_THROUGH_MEM   0x00000002
/*! write back, write allocate*/
#define WRITE_BACK_MEM      0x00000004
/*! write through, write allocate*/
#define WRITE_THROUGH_ALLOC 0x00000006
/*! mask for cache type */
#define MEM_CACHE_TYPE_MASK 0x00000006

typedef enum
{
    ACT_MEM_EX_IGNORE = 0,
    ACT_MEM_EX_LIMIT,
    ACT_MEM_EX_PREALLOC,
    ACT_MEM_EX_POOL,
} act_mem_ex_param_t;

extern int act_mem_initialize(unsigned int uncache_size,
        unsigned int wThrough_size, unsigned int wBack_size,
        unsigned int wThroughAlloc_size, unsigned int total_size,
        unsigned int block_size, act_mem_ex_param_t extra_param);
extern int act_mem_dispose(void);
extern void * act_mem_allocate(unsigned int size, unsigned int mem_flag);
extern int act_mem_free(char * pBuffer);
extern void * act_get_phyaddr(char * pBuffer);
extern unsigned int act_get_max_free();
extern unsigned int act_mem_set_total_size(unsigned int total_size);
extern void* act_mem_realloc(void *mem_address, unsigned int new_size);

#ifdef __cplusplus
}
#endif

#endif
