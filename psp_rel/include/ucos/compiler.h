/********************************************************************************
 *                              USDK(1100)
 *                            Module: COMPILE
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-8-30 16:45     1.0             build this file
 ********************************************************************************/
/*!
 * \file     compiler.h
 * \brief    编译器相关头文件
 * \author   open source
 * \version 1.0
 * \date  2008/8/30
 *******************************************************************************/
#ifndef __COMPILER_H__
#define __COMPILER_H__

#define __user

#define likely(x)	__builtin_expect(!!(x), 1)
#define unlikely(x)	__builtin_expect(!!(x), 0)

#if __GNUC_MINOR__ >= 3
# define __attribute_used__	__attribute__((__used__))
#else
# define __attribute_used__	__attribute__((__unused__))
#endif

#endif /* __COMPILER_H__ */
