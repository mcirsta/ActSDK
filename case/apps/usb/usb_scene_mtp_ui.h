/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : usb_scene_mtp_ui.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    zhaowenjie    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     usb_scene_mtp_ui.h
* \brief    udisk scene ui
* \author   zhaowenjie
*               
* \version 1.0
* \date  2009/06/01
*******************************************************************************/


#ifndef __USB_SCENE_MTP_UI_H__
#define __USB_SCENE_MTP_UI_H__

#include "include_case.h"


typedef enum
{
    MTP_STATUS_IDLE = 0,
    MTP_STATUS_BUSY
    
//	    MTP_STATUS_DEAL_COMMAND,
//	    MTP_STATUS_RECEIVE_DATA,
//	    MTP_STATUS_SEND_DATA

}mtp_status_e;

typedef struct
{
    scene_t* (*const load_style_resource )( rotation_e angle );
    void (*const free_style_resource)(void);

    void (*const set_mtp_hdc)( int hdc );
    void (*const paint_mtp_initial_ui)(void);
    void (*const paint_mtp_status_frame)( mtp_status_e status );
    
}scene_mtp_ui_t;

scene_mtp_ui_t* create_scene_mtp_ui(void);

#endif /* __USB_SCENE_MTP_UI_H__ */

