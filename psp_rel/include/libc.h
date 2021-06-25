/********************************************************************************
 *                              USDK(1100)
 *                            Module: SLEEP
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       wurui     2008-8-30 16:44     1.0             build this file
 ********************************************************************************/
/*!
 * \file     libc.h
 * \brief    posix C库头文件汇总,供平台层调用
 * \author   wurui
 * \version 1.0
 * \date  2008-8-30 16:44
 *******************************************************************************/

#include "syscalls.h"
#include <libc/types.h>
#include <libc/stdarg.h>
#include <libc/stddef.h>
#include <libc/ctype.h>
#include <libc/time.h>
#include <libc/string.h>
#include <libc/errno.h>
#include <libc/process.h>
#include <libc/unistd.h>
#include <libc/wait.h>
#include <libc/fcntl.h>
#include <libc/sleep.h>
#include <libc/stdio.h>
#include <libc/stdlib.h>
#include <libc/pthread.h>
#include <libc/sched.h>
#include <libc/mqueue.h>
#include <libc/semaphore.h>
#include <libc/dirent.h>
#include <libc/stat.h>
#include <libc/mman.h>
#include <libc/sys/ioctl.h>
#include <assert.h>

#ifdef QAC_CHECK_PRIX
#define __attribute__(v)
#endif

