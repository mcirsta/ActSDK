/********************************************************************************
 *                              NOYA
 *                            Module: libc_general
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       wurui     2008-09-12 10:00     1.0             build this file
 ********************************************************************************/
/*!
 * \brief    posix实现接口类型定义
 * \author   wurui
 * \version 1.0
 * \date  2008/09/12
 *******************************************************************************/

#ifndef _SYS_FEATURES_H
#define _SYS_FEATURES_H

/* RTEMS adheres to POSIX -- 1003.1b with some features from annexes.  */

#define _POSIX_JOB_CONTROL     		        0
#define _POSIX_SAVED_IDS       		        0
#define _POSIX_VERSION			        199309L
#define _POSIX_ASYNCHRONOUS_IO		        0
#define _POSIX_FSYNC			            1
#define _POSIX_MAPPED_FILES		            1
#define _POSIX_MEMLOCK			            0
#define _POSIX_MEMLOCK_RANGE		        0
#define _POSIX_MEMORY_PROTECTION	        0
#define _POSIX_MESSAGE_PASSING		        1
#define _POSIX_PRIORITIZED_IO		        0
#define _POSIX_PRIORITY_SCHEDULING	        1
#define _POSIX_REALTIME_SIGNALS		        0
#define _POSIX_SEMAPHORES		            1
#define _POSIX_SHARED_MEMORY_OBJECTS	    1
#define _POSIX_SYNCHRONIZED_IO		        0
#define _POSIX_TIMERS			            1

/* In P1003.1b but defined by drafts at least as early as P1003.1c/D10  */
#define _POSIX_THREADS				        1
#define _POSIX_THREAD_ATTR_STACKADDR		1
#define _POSIX_THREAD_ATTR_STACKSIZE		1
#define _POSIX_THREAD_PRIORITY_SCHEDULING	1
#define _POSIX_THREAD_PRIO_INHERIT		    1
#define _POSIX_THREAD_PRIO_PROTECT		    1
#define _POSIX_THREAD_PROCESS_SHARED		1
#define _POSIX_THREAD_SAFE_FUNCTIONS		0

/* P1003.4b/D8 defines the constants below this comment. */
#define _POSIX_SPAWN				        0
#define _POSIX_TIMEOUTS				        0
#define _POSIX_CPUTIME				        0
#define _POSIX_THREAD_CPUTIME			    0
#define _POSIX_SPORADIC_SERVER			    0
#define _POSIX_THREAD_SPORADIC_SERVER		0
#define _POSIX_DEVICE_CONTROL			    0
#define _POSIX_DEVCTL_DIRECTION			    0
#define _POSIX_INTERRUPT_CONTROL		    0
#define _POSIX_ADVISORY_INFO			    0

#endif /* _SYS_FEATURES_H */
