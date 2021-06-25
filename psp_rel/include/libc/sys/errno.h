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
 * \file     errno.h
 * \brief    posix C库中系统相关的errno定义
 * \author   wurui
 * \par 相关描述:
 * 声明errno
 * \par EXTERNALIZED FUNCTIONS:
 *     Copyright(c) 2001-2008 Actions Semiconductor, All Rights Reserved.
 *
 *  \version 1.0
 *  \date  2008/09/12
 *******************************************************************************/
#ifndef __SYS_ERRNO_H__
#define __SYS_ERRNO_H__

#include <libc/sys/reent.h>

#define errno (*__errno())
extern int *__errno(void);

#endif /* #ifndef __SYS_ERRNO_H__ */
