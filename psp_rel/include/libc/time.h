/********************************************************************************
 *                              USDK130
 *                            Module: TIME
 *                 Copyright(c) 2001-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       wurui     2008-09-12 10:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     time.h
 * \brief    posix标准时钟操作的头文件
 * \author   wurui
 * \version 1.0
 * \date  2008/12/31
 *******************************************************************************/
#ifndef __TIME_H__
#define __TIME_H__

#include <libc/stddef.h>
#include <libc/types.h>
#include <libc/sys/time.h>

int clock_gettime(clockid_t clock_id, struct timespec *tp);
int clock_settime(clockid_t clock_id, struct timespec *tp);
int nanosleep(const struct timespec *rqtp, struct timespec *rmtp);
size_t strftime(char *s, size_t maxsize, const char *format,
        const struct tm *tim_p);

#endif /* __TIME_H__ */

