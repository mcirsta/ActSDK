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
 * \file     reent.h
 * \brief    posix 线程库中线程安全结构定义
 * \author wurui
 * \par 相关描述:
 * 声明errno
 * \par EXTERNALIZED FUNCTIONS:
 *     Copyright(c) 2001-2008 Actions Semiconductor, All Rights Reserved.
 *
 *  \version 1.0
 *  \date  2008/09/12
 *******************************************************************************/

#ifndef __SYS_REENT_H__
#define __SYS_REENT_H__

/* 错误号在线程内的副本, 以实现错误号的线程安全 */
struct _reent
{
    /*! local copy of errno */
    int _errno;
};

#endif /* #ifndef __SYS_REENT_H__ */
