/*
*******************************************************************************
*                       NOYA1100
*                (c) Copyright 2002-2008, Actions Co,Ld. 
*                        All Right Reserved 
* FileName: post_msg.h
* Description:
* History: 
*       <author>    <time>          <version >      <desc>
*       shaofeng    2009/03/11      1.0             建立该文件。
*******************************************************************************
*/ 

#ifndef __POST_MSG_H__
#define __POST_MSG_H__


#include "include_case.h"
#include "include_msg_server.h"



/************************ extern function, 函数声明 ***************************/
extern bool post_key_msg(mqd_t mq_fd, input_gui_msg_t * pmsg, int force);
extern bool post_broadcase_msg( msg_apps_type_e msg_type );


#endif /* __POST_MSG_H__ */

