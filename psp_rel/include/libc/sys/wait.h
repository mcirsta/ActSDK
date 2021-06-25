/********************************************************************************
 *                                    NOYA
 *                            Module: libc_general
 *                 Copyright(c) 2001-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>           <version >             <desc>
 *        wurui     2008-09-12 10:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     wait.h
 * \brief    C库中有关于进程结束的头文件
 * \author wurui
 * \par EXTERNALIZED FUNCTIONS:
 *     Copyright(c) 2001-2008 Actions Semiconductor, All Rights Reserved.
 *
 *  \version 1.0
 *  \date  2008/09/12
 *******************************************************************************/

#ifndef _WAIT_H
#define _WAIT_H

#include <libc/sys/types.h>

/* A status looks like:
 <2 bytes info> <2 bytes code>

 <code> == 0, child has exited, info is the exit value
 <code> == 1..7e, child has exited, info is the signal number.
 <code> == 7f, child has stopped, info was the signal number.
 <code> == 80, there was a core dump.
 */

#define WIFEXITED(w)	(((w) & 0xff) == 0)
#define WIFSIGNALED(w)	(((w) & 0x7f) > 0 && (((w) & 0x7f) < 0x7f))
#define WIFSTOPPED(w)	(((w) & 0xff) == 0x7f)
#define WEXITSTATUS(w)	(((w) >> 8) & 0xff)
#define WTERMSIG(w)	    ((w) & 0x7f)
#define WSTOPSIG	    WEXITSTATUS

extern pid_t wait(int *);
extern pid_t waitpid(pid_t, int *, int);

/* Provide prototypes for most of the _<systemcall> names that are
 provided in newlib for some compilers.  */
extern pid_t _wait(int *);

#endif
