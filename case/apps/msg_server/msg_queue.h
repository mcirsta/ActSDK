/*
*******************************************************************************
*                       NOYA1100
*                (c) Copyright 2002-2008, Actions Co,Ld. 
*                        All Right Reserved 
* FileName: msg_queue.h
* Description:
* History: 
*       <author>    <time>          <version >      <desc>
*       shaofeng    2009/03/11      1.0             建立该文件。
*******************************************************************************
*/ 

#ifndef __MSG_QUEUE_H__
#define __MSG_QUEUE_H__


#include "include_case.h"
#include "include_msg_server.h"


/************************ extern variable, 变量声明 ***************************/
extern mqd_t mq_key_fd;


/************************ extern function, 函数声明 ***************************/
extern bool msg_queue_init(void);
extern bool msg_queue_deinit(void);



#endif /* __MSG_QUEUE_H__ */

