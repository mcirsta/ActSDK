/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      zhaowenjie    2009-05-26          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     usb_app.h
* \brief    
* \author   zhaowenjie
*               
* \version 1.0
* \date  2009/05/26
*******************************************************************************/

#ifndef __USB_GLOBAL_H__
#define __USB_GLOBAL_H__


#include "include_case.h"
#include "style.h"
#include "usb_sty.h"

//#define DEBUG_UI

#define USB_APP "usb.app"

#define WRONG_DEVICE_FD -1

/*!
* scene type define
*/
typedef enum
{

    SCENE_EXIT = 0,
    
    SCENE_UDISK,
    SCENE_UMTP,
    SCENE_UCHARGE,
    SCENE_UPGRADE,

}scene_e;

#define NULL_TIMER -1

#define INT_LENGTH 4

#define PLAYLIST_DB "actplaylist.db"

#define UDC_DRIVER "udc.ko"
#define MSC_DRIVER "msc.ko"

#define AMTP_DRIVER "amtp.ko"
#define TUNNEL_DRIVER "tunnel.ko"
#define HOTPLUG_DRIVER "hotplug.ko"

#include "usb_main.h"
#include "usb_scene_udisk.h"
#include "usb_scene_mtp.h"
#include "usb_scene_charge.h"
#include "usb_scene_upgrade.h"

style_api_t* g_style_api;

usb_main_t* g_usb_main;
scene_udisk_t* g_scene_udisk;
scene_mtp_t* g_scene_mtp;
scene_charge_t* g_scene_charge;
scene_upgrade_t* g_scene_upgrade;

#endif  /* __USB_GLOBAL_H__ */

