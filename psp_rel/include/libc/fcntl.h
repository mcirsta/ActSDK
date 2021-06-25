/********************************************************************************
 *                              USDK(1100)
 *                            Module: FCNTL
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-8-30 16:45     1.0             build this file
 ********************************************************************************/
/*!
 * \file     fcntl.h
 * \brief    文件控制操作头文件
 * \author   hmwei
 * \version 1.0
 * \date  2008-8-30 16:45
 *******************************************************************************/
#ifndef __FCNTL_H__
#define __FCNTL_H__

#include "ucos/err.h"
#include "ucos/fcntl.h"
#include "syscalls.h"
#include "errno.h"

extern int lockf(int fd, int function, off_t size);
extern int creat(const char *pathname, mode_t mode);
extern int fcntl(int fd, int cmd, long arg);

#endif /* __FCNTL_H__ */
