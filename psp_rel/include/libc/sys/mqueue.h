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
 * \file     mqueue.h
 * \brief    posix C库中系统相关的消息队列定义
 * \author wurui
 * \par 相关描述:
 * 声明errno
 * \par EXTERNALIZED FUNCTIONS:
 *     Copyright(c) 2001-2008 Actions Semiconductor, All Rights Reserved.
 *
 *  \version 1.0
 *  \date  2008/09/12
 *******************************************************************************/

#ifndef __SYS_MQUEUE_H__
#define __SYS_MQUEUE_H__

#include <libc/sys/types.h>

/*!
 *  \brief
 *     消息队列初始化结构
 */
struct mq_attr
{
    /*! 消息队列阻塞属性: 0--阻塞调用者(消息队列满时阻塞发送者,消息队列空时阻塞接收者)
     O_NONBLOCK--不阻塞调用者,但返回错误*/
    long mq_flags;
    /*! 消息队列初始化时使用,定义消息队列最大消息的数目; 2消息队列属性获取时使用*/
    long mq_maxmsg;
    /*! 消息队列初始化时使用,定义每一条消息最大的数据长度; 2消息队列属性获取时使用*/
    long mq_msgsize;
    /*! 消息队属性获取时使用,获取消息队列当前消息位置*/
    long mq_curmsgs;
};

/*!
 *  \brief
 *     消息队列文件描述符
 */
typedef int mqd_t;

#endif /*#ifndef __SYS_MQUEUE_H__*/
