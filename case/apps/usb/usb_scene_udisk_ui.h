/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : usb_scene_udisk_ui.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    zhaowenjie    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     usb_scene_udisk_ui.h
* \brief    udisk scene ui
* \author   zhaowenjie
*               
* \version 1.0
* \date  2009/06/01
*******************************************************************************/


#ifndef __USB_SCENE_UDISK_UI_H__
#define __USB_SCENE_UDISK_UI_H__

#include "include_case.h"


typedef enum
{
    UDISK_STATUS_IDLE = 0,
    UDISK_STATUS_BUSY,

    UDISK_STATUS_UPGRADE
    
//	    UDISK_STATUS_READ,
//	    UDISK_STATUS_WRITE,

}udisk_status_e;

typedef struct
{
    scene_t* (*const load_style_resource )( rotation_e angle );
    void (*const free_style_resource)(void);

    void (*const set_udisk_hdc)( int hdc );
    void (*const paint_udisk_initial_ui)(void);
    void (*const paint_status_frame)( udisk_status_e status );
    void (*const paint_upgrade_strings)(void);
    
}scene_udisk_ui_t;

scene_udisk_ui_t* create_scene_udisk_ui(void);

#endif /* __USB_SCENE_UDISK_UI_H__ */

