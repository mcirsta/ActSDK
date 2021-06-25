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
* \file     usb_main.h
* \brief    
* \author   zhaowenjie
*               
* \version 1.0
* \date  2009/05/26
*******************************************************************************/

#ifndef __USB_MAIN_H__
#define __USB_MAIN_H__


#include "include_case.h"

/* include file of this application, 本应用的头文件 */
#include "usb.h"

typedef struct
{    
    char* (*const get_font_path)(void);
    void (*const set_current_scene)( scene_e scene );  

    style_t* (*const get_style)(void);
    picture_resource_t* (*const get_background_picture)(void);
    void (*const update_background_picture)(void);
    void (*const set_receive_quit_msg)( bool quit );

}usb_main_t;

usb_main_t* create_usb_main(void);

#endif  /* __USB_MAIN_H__ */

