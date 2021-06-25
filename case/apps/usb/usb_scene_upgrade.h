/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : usb_scene_upgrade.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    zhaowenjie    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     usb_scene_upgrade.h
* \brief    upgrade scene
* \author   zhaowenjie
*               
* \version 1.0
* \date  2009/06/02
*******************************************************************************/


#ifndef __USB_SCENE_UPGRADE_H__
#define __USB_SCENE_UPGRADE_H__

#include "include_case.h"


typedef struct
{
    void (*const enter_scene_upgrade)(void);

    char* (*const get_upgrade_fw_file)(void);
}scene_upgrade_t;

scene_upgrade_t* create_scene_upgrade(void);

#endif /* __USB_SCENE_UPGRADE_H__ */
