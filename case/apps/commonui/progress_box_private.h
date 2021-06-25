/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : progress_box_private.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    herb hu      2009-07-27          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     progress_box_private.h
* \brief    progress_box widget private interface
* \author   herb hu 
*               
* \version 1.0
* \date  2009/07/27
*******************************************************************************/

#ifndef __PROGRESS_BOX_PRIVATE_H__
#define __PROGRESS_BOX_PRIVATE_H__

#include "include_case.h"
#include "commonui_sty.h"

/* define animation interval */
#define ANIMATION_INTERVAL 200

/* define progress_box private data */
typedef struct progress_box_private_s
{
    /*! »æÍ¼DC */
    int hdc;

    int current_frame;
    int total_frame;

    /* animation timer */
    int timer_id;
    
    /* resource for painting */
    resgroup_resource_t *resgroup;
    picture_resource_t *pic_bg;
    gui_bitmap_info_t *bmp_bg;
    
    picregion_resource_t *frame_wait;
    string_resource_t * text_str;

    picture_resource_t *pic_pbar_bg;
    gui_bitmap_info_t *bmp_pbar_bg;

    picture_resource_t *pic_pbar_hl;
    gui_bitmap_info_t *bmp_pbar_hl;
    
    progress_box_init_t init_data;
    
}progress_box_private_t;

bool progress_box_draw_bg( progress_box_t *progress_box );
bool progress_box_draw_wait_animation( progress_box_t *progress_box );
bool progress_box_draw_text( progress_box_t *progress_box );
bool progress_box_draw_progress_bar( progress_box_t *progress_box );

#endif

