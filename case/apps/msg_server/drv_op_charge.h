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

#ifndef __DRV_OP_CHARGE_H__
#define __DRV_OP_CHARGE_H__


#include "include_case.h"
#include "include_msg_server.h"


/************************ extern function, 函数声明 ***************************/
extern void charge_para_init( void );
extern void charge_device_open( void );
extern void charge_device_close( void );

extern bool charge_get_battery_voltage( charge_voltage_t * vol );
extern bool charge_get_battery_voltage_show( charge_voltage_t *vol );
extern bool charge_get_adapter( bool *is_on );
extern bool charge_set_param( charge_param_t *param );
extern bool charge_set_start( int start );
extern bool charge_get_status( charge_status_t *status );
extern bool charge_set_process( void );


#endif /* __DRV_OP_CHARGE_H__ */

