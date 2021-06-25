/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : usb_scene_upgrade_ui.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    zhaowenjie    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     usb_scene_upgrade_ui.h
* \brief    udisk scene ui
* \author   zhaowenjie
*               
* \version 1.0
* \date  2009/06/01
*******************************************************************************/


#ifndef __USB_SCENE_UPGRADE_UI_H__
#define __USB_SCENE_UPGRADE_UI_H__

#include "include_case.h"


typedef struct
{
    scene_t* (*const load_style_resource )(void);
    void (*const free_style_resource)(void);

    void (*const paint_upgrade_initial_ui)( int hdc );
    void (*const update_process_bar)( unsigned short percent );
        
}scene_upgrade_ui_t;

scene_upgrade_ui_t* create_scene_upgrade_ui(void);

#endif /* __USB_SCENE_UPGRADE_UI_H__ */

