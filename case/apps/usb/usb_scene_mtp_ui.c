/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : usb_scene_mtp_ui.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    zhaowenjie    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     usb_scene_mtp_ui.c
* \brief    mtp scene ui
* \author   zhaowenjie
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#include "usb.h"
#include "usb_scene_mtp_ui.h"

static int s_hdc;

static scene_t* s_scene_resource = NULL;
static picregion_resource_t* s_status_frame = NULL;

/* MTP状态图片数据缓存 */
static gui_bitmap_info_t **s_bmp_status_frame = NULL;
static gui_bitmap_info_t *s_bmp_bg = NULL;

static scene_t* load_style_resource( rotation_e angle );
static void free_style_resource(void);

void set_mtp_hdc( int hdc );
void paint_mtp_initial_ui(void);
void paint_mtp_status_frame( mtp_status_e status );


static scene_mtp_ui_t s_scene_mtp_ui_api = 
{
    load_style_resource,
    free_style_resource,

    set_mtp_hdc,
    paint_mtp_initial_ui,
    paint_mtp_status_frame
};

void set_mtp_hdc( int hdc )
{
    s_hdc = hdc;
}

static scene_t* load_style_resource( rotation_e angle )
{
    int i;
    picregion_frame_t pic_frame;
    sty_rect_t rect;

    if ( s_scene_resource != NULL )
    {
        free_style_resource();
    }

    if ( ( angle == ROTATION_0 ) ||  ( angle == ROTATION_180 ) )
    {
        s_scene_resource = g_style_api->load_scene( g_usb_main->get_style(), MTP_SIDELONG_SCENE );
    }
    else
    {
        s_scene_resource = g_style_api->load_scene( g_usb_main->get_style(), MTP_UPRIGHT_SCENE );
    }

    if( s_scene_resource == NULL)
    {
        print_err(" usb_scene_udisk_ui load scene failure "); 
        return NULL;
    }
    else
    {
        /* 设置应用style场景资源 */
        sty_set_app_scene( s_scene_resource );

        s_status_frame = ( picregion_resource_t* )g_style_api->get_scene_child( s_scene_resource, REG_STATUS_FRAME );
        if ( s_status_frame == NULL )
        {
            print_err(" usb_scene_udisk_ui load s_status_frame failure ");
            return NULL;
        }
    }

    /* 提取图片资源数据 */
    s_bmp_status_frame = (gui_bitmap_info_t **)malloc(s_status_frame->frames * sizeof(gui_bitmap_info_t *));
    if(NULL == s_bmp_status_frame)
    {
        print_err("malloc failure!");
        return NULL;
    }

    memset(s_bmp_status_frame, 0, s_status_frame->frames * sizeof(gui_bitmap_info_t *));
    for(i = 0; i < s_status_frame->frames; i++)
    {
        get_picregion_frame(s_status_frame, (unsigned short)i, &pic_frame);
        s_bmp_status_frame[i] = sty_create_bitmap(pic_frame.pic_id);
        if(NULL == s_bmp_status_frame[i])
        {
            print_err("sty_create_bitmap failure!");
            return NULL;
        }
    }

    rect.x = s_status_frame->x;
    rect.y = s_status_frame->y;
    rect.width = s_status_frame->width;
    rect.height = s_status_frame->height;
    s_bmp_bg = sty_create_sub_bitmap( g_usb_main->get_background_picture()->pic_id, &rect );

    return s_scene_resource;
}

void paint_mtp_initial_ui(void)
{    
    sty_draw_picture_res( s_hdc, g_usb_main->get_background_picture() );

    paint_mtp_status_frame( MTP_STATUS_IDLE );
}

void paint_mtp_status_frame( mtp_status_e status )
{
    sty_rect_t rect;
    rect.x = s_status_frame->x;
    rect.y = s_status_frame->y;
    rect.width = s_status_frame->width;
    rect.height = s_status_frame->height;

#if 1
    sty_draw_bitmap(s_hdc, s_bmp_bg, rect.x, rect.y);
    sty_draw_bitmap(s_hdc, s_bmp_status_frame[status], rect.x, rect.y);
#else
    sty_draw_sub_picture_id( s_hdc, (int)(g_usb_main->get_background_picture()->pic_id),
                            rect.x, rect.y, &rect );
    sty_draw_picregion_res( s_hdc, s_status_frame, status );
#endif
}

static void free_style_resource(void)
{
    int i;

    for(i = 0; i < s_status_frame->frames; i++)
    {
        if(NULL != s_bmp_status_frame[i])
        {
            sty_delete_bitmap(s_bmp_status_frame[i]);
        }
    }
    sty_delete_bitmap(s_bmp_bg);
    free(s_bmp_status_frame);

    g_style_api->free_scene( s_scene_resource );
    s_scene_resource = NULL;
}



scene_mtp_ui_t* create_scene_mtp_ui(void)
{
    return &s_scene_mtp_ui_api;
}

