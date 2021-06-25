/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : usb_scene_charge_ui.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    zhaowenjie    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     usb_scene_charge_ui.h
* \brief    charge scene ui
* \author   zhaowenjie
*               
* \version 1.0
* \date  2009/06/01
*******************************************************************************/


#ifndef __USB_SCENE_CHARGE_UI_H__
#define __USB_SCENE_CHARGE_UI_H__

#include "include_case.h"


typedef struct
{
    scene_t* (*const load_style_resource )( rotation_e angle );
    void (*const free_style_resource)(void);

    void (*const set_charge_hdc)( int hdc );
    void (*const paint_charge_initial_ui)(void);
    void (*const update_battery_status)( charge_status_t status );
        
}scene_charge_ui_t;

scene_charge_ui_t* create_scene_charge_ui(void);

#endif /* __USB_SCENE_CHARGE_UI_H__ */

