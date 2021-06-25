/*!\cond LIBC*/
/********************************************************************************
 *                              usdk 1100
 *                            Module:libc_fs_dir
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *      husanxi    2009-1-19 15:24     1.0             build this file
 ********************************************************************************/
/*!
 * \file     mman.h
 * \brief    本文件包含了mmap和munmap等函数原型。
 * \author   husanxi
 * \version 1.0
 * \date  2009/01/19
 *******************************************************************************/
#ifndef __MMAN_H__
#define __MMAN_H__
#define MAP_FAILED (void*)-1
#include "syscalls.h"
extern void *mmap(void *addr, size_t len, int prot, int flags, int fildes,
        off_t off);
extern int munmap(void *addr, size_t len);
#endif
/*!\endcond*/

