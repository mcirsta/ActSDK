/********************************************************************************
 *                              USDK(1100)
 *                            Module: UACESS
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-8-30 16:42     1.0             build this file
 ********************************************************************************/
/*!
 * \file     uaccess.h
 * \brief    内核空间和用户空间的数据交互宏定义
 * \author   hmwei
 * \version 1.0
 * \date  2008/8/30
 *******************************************************************************/
/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1996, 1997, 1998, 1999, 2000, 03, 04 by Ralf Baechle
 * Copyright (C) 1999, 2000 Silicon Graphics, Inc.
 */
#ifndef __UACCESS_H__
#define __UACCESS_H__

//#include "asm-mips/config.h"
//#include "kernel.h"
//#include "errno.h"
//	#include "thread_info.h"
//	#include "uaccess.h"

/*!
 * \brief
 *      Write a simple value into user space
 * \li  x: Value to copy to user space
 * \li  ptr: Destination address, in user space
 * \note
 * Returns zero on success, or -EFAULT on error.
 */
#define put_user(x,ptr)	\
	memcpy((ptr),(&x),sizeof(*(ptr)))

/*!
 * \brief
 *      Get a simple variable from user space
 * \li  x: Variable to store result
 * \li  ptr: Source address, in user space
 * \note
 * Returns zero on success, or -EFAULT on error.
 */
#define get_user(x,ptr) \
	memcpy((&x),(ptr),sizeof(*(ptr)))

/*!
 * \brief
 *      Copy a block of data into user space
 * \li  to: Destination address, in user space
 * \li  from: Source address, in kernel space
 * \li  n: Number of bytes to copy
 * \note
 * Returns number of bytes that could not be copied.
 * On success, this will be zero.
 */
#define copy_to_user(to,from,n)						\
	memcpy((to),(from),n)

/*!
 * \brief
 *      Copy a block of data from user space
 * \li  to: Destination address, in kernel space
 * \li  from: Source address, in user space
 * \li  n: Number of bytes to copy
 * \note
 * Returns number of bytes that could not be copied.
 * On success, this will be zero.
 */
#define copy_from_user(to,from,n)					\
	memcpy((to),(from),n)

#endif /* __UACCESS_H__ */
