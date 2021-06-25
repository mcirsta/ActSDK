/*
*******************************************************************************
*                       NOYA1100
*                (c) Copyright 2002-2008, Actions Co,Ld. 
*                        All Right Reserved 
* FileName: .h
* Description:
* History: 
*       <author>    <time>          <version >      <desc>
*       shaofeng    2009/03/11      1.0             �������ļ���
*******************************************************************************
*/ 

#ifndef __DRV_OP_KEY_H__
#define __DRV_OP_KEY_H__


#include "include_case.h"
#include "include_msg_server.h"



/************************ extern variable, �������� ***************************/
extern key_para_t  key_para;



/************************ extern function, �������� ***************************/
extern void key_para_init(void);
extern void key_device_open(void);
extern void key_device_close(void);

extern bool key_get_drv_keyval(unsigned int *key_val);
extern bool key_get_keylock_status(int *is_key_lock);

extern int key_get_no_jitter_keyval(int num, unsigned int *p_keyval);


#endif /* __DRV_OP_KEY_H__ */

