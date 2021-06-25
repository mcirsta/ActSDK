/*
*******************************************************************************
*                       NOYA1100
*                (c) Copyright 2002-2008, Actions Co,Ld. 
*                        All Right Reserved 
* FileName: process_msg.h
* Description:
* History: 
*       <author>    <time>          <version >      <desc>
*       shaofeng    2009/03/11      1.0             建立该文件。
*******************************************************************************
*/ 

#ifndef __PROCESS_MSG_H__
#define __PROCESS_MSG_H__


#include "include_case.h"
#include "include_msg_server.h"



/************************ extern function, 函数声明 ***************************/
extern bool process_key_event( unsigned int key_val_app, input_gui_msg_t * input_msg );
extern bool key_mapping_set_status( key_val_e key, bool enable );
extern bool key_mapping( input_gui_msg_t *input_msg );
extern bool key_shortcut( input_gui_msg_t *input_msg );


extern bool init_g_sensor_axes_trans_matrix(void);
extern bool process_gsensor_event( gsensor_data_t * gs_data);
extern rotation_e gsensor_get_rotation(void);


#endif /* __PROCESS_MSG_H__ */

