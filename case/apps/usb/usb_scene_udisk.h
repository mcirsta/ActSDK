/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : usb_scene_udisk.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    zhaowenjie    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     usb_scene_udisk.h
* \brief    udisk scene
* \author   zhaowenjie
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/


#ifndef __USB_SCENE_UDISK_H__
#define __USB_SCENE_UDISK_H__

#include "include_case.h"


typedef struct
{
    void (*const set_pid)( int p );
    void (*const set_vid)( int v );
    void (*const set_release)( int r );
    void (*const set_vendor)( char* string );
    void (*const set_product)( char* string );
    void (*const set_serial)( char* string );

    void (*const enter_scene_udisk )(void);

    void (*const set_show_manufacturer_disk)(void);
    void (*const set_show_u_disk)(void);
    void (*const set_show_card_disk)(void);

    bool (*const get_disk_change)( void );
    bool (*const check_card_insert)(void);
}scene_udisk_t;

scene_udisk_t* create_scene_udisk(void);

#endif /* __USB_SCENE_UDISK_H__ */

