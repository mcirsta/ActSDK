/********************************************************************************
 *                              USDK(1100)
 *                            Module: STAT
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-8-30 16:44     1.0             build this file
 ********************************************************************************/
/*!
 * \file     stat.h
 * \brief    文件/文件系统状态相关的宏定义
 * \author   hmwei
 * \version 1.0
 * \date  2008-8-30 16:44
 *******************************************************************************/
#ifndef __STAT_H__
#define __STAT_H__

#include <ucos/stat.h>

extern int fstat(int fd, struct stat *buf);
extern int stat(const char *path, struct stat *buf);
extern int statfs(const char *path, struct statfs *buf);
extern int mkdir(const char *pathname, mode_t mode);

#define mknod sys_mknod

#endif	/*__STAT_H__*/
