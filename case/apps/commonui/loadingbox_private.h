/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : loadingbox_private.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     loadingbox_private.h
* \brief    loadingbox widget private interface
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#ifndef __LOADINGBOX_PRIVATE_H__
#define __LOADINGBOX_PRIVATE_H__


#include "include_case.h"


/* define animation interval */
#define ANIMATION_INTERVAL 200

/* define loadingbox private data */
typedef struct loadingbox_private_s
{
    /*! »æÍ¼DC */
    int hdc;

    /* animation timer */
    int timer_id;
    int current;
    int total;
    
    /* resource for painting */
    resgroup_resource_t *resgroup;
    picture_resource_t *pic_bg;
    picregion_resource_t *reg_icon;
    string_resource_t *str_name;
    
    gui_bitmap_info_t *bmp_bg;
    
}loadingbox_private_t;


#endif

