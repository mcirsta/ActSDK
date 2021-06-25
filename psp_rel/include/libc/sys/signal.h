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
 * \file     signal.h
 * \brief    posix C库中系统相关的信号定义
 * \author   wurui
 * \version 1.0
 * \date  2008/09/12
 *******************************************************************************/

#ifndef __SYS_SIGNAL_H__
#define __SYS_SIGNAL_H__

/*!
 *  \brief
 *     消息队列mq_notify()函数的信号数据值,其值的类型根据信号类型而定,usdk平台中未定义信号机制
 */
union sigval
{
    /*! 信号类型*/
    int sigval_int;
    /*! 信号回调函数指针*/
    void * sigval_ptr;
};

/*!
 *  \brief
 *     消息队列mq_notify()函数的参数结构,usdk平台中未定义信号机制
 */
struct sigevent
{
    /*! 信号类型*/
    int sigev_notify;
    /*! 信号No.*/
    int sigev_signo;
    /*! 信号数据值*/
    union sigval sigev_value;
    /*! 信号回调函数指针*/
    void (* sigev_notify_function)(union sigval);
};

#endif /*#ifndef __SYS_SIGNAL_H__*/
