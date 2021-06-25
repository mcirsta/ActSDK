/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : usb_scene_mtp.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    zhaowenjie    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     usb_scene_mtp.h
* \brief    udisk scene
* \author   zhaowenjie
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/


#ifndef __USB_SCENE_MTP_H__
#define __USB_SCENE_MTP_H__

#include "include_case.h"


typedef struct
{
    void (*const set_mtp_pid)( int p );
    void (*const set_mtp_vid)( int v );
    void (*const set_mtp_ext)( int e );
    void (*const set_mtp_dev_manufacturer)( char* string );
    void (*const set_mtp_dev_model)( char* string );
    void (*const set_mtp_dev_version)( char* string );
    void (*const set_mtp_dev_serial)( char* string );
    void (*const set_mtp_store_inner_desc)( char* string );
    void (*const set_mtp_store_portable_desc)( char* string );
    void (*const set_mtp_dev_ext_desc0)( char* string );
    void (*const set_mtp_dev_ext_desc1)( char* string );

    void (*const enter_scene_mtp )(void);

    bool (*const get_mtp_change )( void );
}scene_mtp_t;

scene_mtp_t* create_scene_mtp(void);

#endif /* __USB_SCENE_MTP_H__ */

