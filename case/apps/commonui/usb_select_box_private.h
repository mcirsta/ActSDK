/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : usb_select_box_private.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    herb hu      2009-07-14          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     usb_select_box_private.h
* \brief    usb select box widget private interface
* \author   herb hu 
*               
* \version 1.0
* \date  2009/07/14
*******************************************************************************/

#ifndef __USB_SELECT_BOX_PRIVATE_H__
#define __USB_SELECT_BOX_PRIVATE_H__

#include <include_case.h>
#include "commonui_sty.h"

/* define usb select box private data */
typedef struct usb_select_box_private_s
{
    /*! »æÍ¼DC */
    int hdc;

    int current_item;

    /* resource for painting */
    resgroup_resource_t *resgroup;
    picture_resource_t *pic_bg;
    picture_resource_t *pic_icon;
    picture_resource_t *pic_sel_frame;
    string_resource_t *str_highlight;
    string_resource_t *str_items[3];

    string_resource_t *str_mtp;
    string_resource_t *str_data_trans;
    string_resource_t *str_charge;
    string_resource_t *str_charge_play;

    int max_items;
    
}usb_select_box_private_t;

extern bool usb_select_box_draw_bg( usb_select_box_t *loadingbox );
extern bool usb_select_box_draw_items( usb_select_box_t *loadingbox );

#endif

