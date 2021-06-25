/********************************************************************************
 *                              USDK(1100)
 *                            Module: TYPES
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-8-30 16:42     1.0             build this file
 ********************************************************************************/
/*!
 * \file     types.h
 * \brief    数据类型定义
 * \author   hmwei
 * \version 1.0
 * \date  2008-8-30 16:42
 *******************************************************************************/

#ifndef   __TYPES_H__
#define   __TYPES_H__

#ifndef NULL
#define NULL (void *)0
#endif

#ifndef TRUE
#define TRUE   1
#endif

#ifndef FALSE
#define FALSE   0
#endif

#ifndef _ASSEMBLER_

//typedef int bool;
typedef enum
{
    false = 0,
    true = 1
} bool;

typedef unsigned short umode_t;

typedef unsigned int uint;

typedef unsigned int size_t;
typedef int ssize_t;

typedef unsigned int mode_t;

typedef long long off_t;
typedef long long loff_t;

typedef unsigned long vfs_time_t;

typedef unsigned long useconds_t;/* Microseconds */

typedef int pid_t;

typedef unsigned gfp_t;

typedef long fpos_t;

#endif /*_ASSEMBLER_*/

#endif /* __TYPES_H__ */
