/********************************************************************************
 *                              USDK(1100)
 *                            Module: TYPES
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       wurui     2009-1-4 16:42     1.0             build this file
 ********************************************************************************/
/*!
 * \file     mman.h
 * \brief    posix共享内存相关定义
 * \author   wurui
 * \version 1.0
 * \date  2009/01/04 16:42
 *******************************************************************************/
#ifndef __MMAN_H__
#define __MMAN_H__

#include <libc/types.h>

#define MAP_FAILED (void*)-1

void *mmap(void *addr, size_t len, int prot, int flags, int fildes, off_t off);
int munmap(void *addr, size_t len);
int msync(void *addr, size_t len, int flags);
int shm_open(const char *name, int oflag, mode_t mode);
int shm_unlink(const char *name);

#endif /* __MMAN_H__ */

