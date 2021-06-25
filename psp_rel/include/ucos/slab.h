/********************************************************************************
 *                              USDK(1100)
 *                            Module: SLAB
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-8-30 16:44     1.0             build this file
 ********************************************************************************/
/*!
 * \file     slab.h
 * \brief    kmalloc函数实现和kfree函数声明
 * \author   hmwei
 * \version 1.0
 * \date  2008/8/30
 *******************************************************************************/
#ifndef __SLAB_H__
#define __SLAB_H__

#include <ucos/types.h>
#include <ucos/gfp.h>

#ifndef _ASSEMBLER_

extern void *kmalloc(size_t size, gfp_t flags);
extern void kfree(const void *);

#endif /*_ASSEMBLER_*/
#endif /*__SLAB_H__*/
