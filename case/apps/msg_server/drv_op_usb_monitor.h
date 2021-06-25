/*
*******************************************************************************
*                       NOYA1100
*                (c) Copyright 2002-2008, Actions Co,Ld. 
*                        All Right Reserved 
* FileName: .h
* Description:
* History: 
*       <author>    <time>          <version >      <desc>
*       shaofeng    2009/03/11      1.0             建立该文件。
*******************************************************************************
*/ 

#ifndef __DRV_OP_USB_MONITOR_H__
#define __DRV_OP_USB_MONITOR_H__


#include "include_case.h"
#include "include_msg_server.h"



/************************ extern variable, 变量声明 ***************************/
extern void usb_monitor_para_init( void );
extern void usb_monitor_device_open( void );
extern void usb_monitor_device_close( void );

extern bool usb_get_connect_type( USB_CONNECTOR_T *usb_connect_type );



#endif /* __DRV_OP_USB_MONITOR_H__ */

