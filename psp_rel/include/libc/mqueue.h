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
 * \file     mqueue.h
 * \brief    posix消息队列相关定义
 * \author   wurui
 * \version 1.0
 * \date  2009/01/04 16:42
 *******************************************************************************/

#ifndef __MQUEUE_H__
#define __MQUEUE_H__

#include <libc/sys/mqueue.h>
#include <libc/sys/time.h>
#include <libc/sys/signal.h>

extern mqd_t mq_open(const char *name, int oflag, ...);
extern int mq_close(mqd_t mqdes);
extern int mq_unlink(const char *name);
extern int mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len,
        unsigned msg_prio);
extern int mq_timedsend(mqd_t mqdes, const char *msg_ptr, size_t msg_len,
        unsigned int msg_prio, const struct timespec *abs_timeout);
extern size_t mq_receive(mqd_t mqdes, char *msg_ptr, size_t msg_len,
        unsigned int *msg_prio);
extern size_t mq_timedreceive(mqd_t mqdes, char *msg_ptr, size_t msg_len,
        unsigned *msg_prio, const struct timespec *abs_timeout);
extern int mq_setattr(mqd_t mqdes, const struct mq_attr *mqstat,
        struct mq_attr *omqstat);
extern int mq_getattr(mqd_t mqdes, struct mq_attr *mqstat);

/* 消息队列mq_notify()函数的参数结构,usdk平台中未定义信号机制,本接口返回-1 */
extern int mq_notify(mqd_t mqdes, const struct sigevent *notification);

#endif /*#ifndef __MQUEUE_H__*/
