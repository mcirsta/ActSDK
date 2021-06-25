/********************************************************************************
 *                              USDK(1100)
 *                            Module: ERR
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-8-30 16:45     1.0             build this file
 ********************************************************************************/
/*!
 * \file     err.h
 * \brief    err号和指针的判断以及两者之间的转换
 * \athor   hmwei
 * \version 1.0
 * \date  2008/8/30
 *******************************************************************************/
#ifndef __ERR_H__
#define __ERR_H__

#include <ucos/compiler.h>
#include <asm-generic/errno.h>

#ifndef _ASSEMBLER_

/*
 * Kernel pointers have redundant information, so we can use a
 * scheme where we can return either an error code or a dentry
 * pointer with the same return value.
 *
 * This should be a per-architecture thing, to allow different
 * error and pointer decisions.
 */
#define IS_ERR_VALUE(x) unlikely((x) > (unsigned long)-20000L)

static inline void *ERR_PTR(long error)
{
    return (void *) error;
}

static inline long PTR_ERR(const void *ptr)
{
    return (long) ptr;
}

static inline long IS_ERR(const void *ptr)
{
    return IS_ERR_VALUE((unsigned long)ptr);
}

#endif /*_ASSEMBLER_*/
#endif /* __ERR_H__ */
