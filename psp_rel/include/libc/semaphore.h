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
 * \file     semaphore.h
 * \brief    posix信号量相关定义及声明
 * \author   wurui
 * \version 1.0
 * \date  2009/01/04 16:42
 *******************************************************************************/

#ifndef __SEMAPHORE_H__
#define __SEMAPHORE_H__

#include <libc/sys/semaphore.h>

extern sem_t *sem_open(const char *name, int oflag, ...);
extern int sem_close(sem_t *sem);
extern int sem_unlink(const char *name);
extern int sem_init(sem_t *sem, int pshared, unsigned int value);
extern int sem_destroy(sem_t *sem);
extern int sem_getvalue(sem_t *sem, int *valp);
extern int sem_wait(sem_t *sem);
extern int sem_trywait(sem_t *sem);
extern int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);
extern int sem_post(sem_t *sem);

#endif /* #ifndef __SEMAPHORE_H__ */
