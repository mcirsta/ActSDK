/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : usb_scene_charge.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    zhaowenjie    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     usb_scene_charge.h
* \brief    charge scene
* \author   zhaowenjie
*               
* \version 1.0
* \date  2009/06/04
*******************************************************************************/


#ifndef __USB_SCENE_CHARGE_H__
#define __USB_SCENE_CHARGE_H__

#include "include_case.h"


typedef struct
{
    void (*const enter_scene_charge)(void);

    void (*const set_need_charge_ui)( bool need );
    
}scene_charge_t;

scene_charge_t* create_scene_charge(void);

#endif /* __USB_SCENE_CHARGE_H__ */

