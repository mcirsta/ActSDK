/*!\cond LIBC*/
/********************************************************************************
 *                              usdk130-1017
 *                            Module:libc_fs_dir
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *      husanxi    2008-09-27 15:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     ioctl.h
 * \brief    本文件包含了ioctl的函数原型。
 * \author   husanxi
 * \version 1.0
 * \date  2009/01/17
 *******************************************************************************/
#ifndef __IOCTL_H__
#define __IOCTL_H__
#include "syscalls.h"

#pragma __PRQA_IGNORE_START__
extern int ioctl(int fildes, int request, int arg);
#pragma __PRQA_IGNORE_END__

#endif
/*!\endcond*/

