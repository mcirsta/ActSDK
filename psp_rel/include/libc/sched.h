/********************************************************************************
 *                              USDK(1100)
 *                            Module: SCHED
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-8-30 16:45     1.0             build this file
 ********************************************************************************/
/*!
 * \file     sched.h
 * \brief    任务状态定义
 * \author   linux
 * \version 1.0
 * \date  2008-8-30 16:45
 *******************************************************************************/
/*
 * Task state bitmask. NOTE! These bits are also
 * encoded in fs/proc/array.c: get_task_state().
 *
 * We have two separate sets of flags: task->state
 * is about runnability, while task->exit_state are
 * about the task exiting. Confusing, but this way
 * modifying one set can't modify the other one by
 * mistake.
 */
#ifndef __SCHED_H__
#define __SCHED_H__
#include <libc/sys/sched.h>

#define TASK_RUNNING		0
#define TASK_INTERRUPTIBLE	1
#define TASK_UNINTERRUPTIBLE	2
#define TASK_STOPPED		4
#define TASK_TRACED		8
/* in tsk->exit_state */
#define EXIT_ZOMBIE		16
#define EXIT_DEAD		32
/* in tsk->state again */
#define TASK_NONINTERACTIVE	64

int sched_get_priority_min(int policy);
int sched_get_priority_max(int policy);
int sched_yield(void);
#endif

