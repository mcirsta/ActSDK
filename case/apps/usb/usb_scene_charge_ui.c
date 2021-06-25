/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : usb_scene_charge_ui.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *    zhaowenjie    2009-2-25          v1.0               build this file
 ********************************************************************************/
/*!
* \file     usb_scene_charge_ui.c
* \brief    charge scene ui
* \author   zhaowenjie
*
* \version 1.0
* \date  2009/06/03
*******************************************************************************/

#include "usb.h"
#include "usb_scene_charge_ui.h"

static int s_hdc;

static scene_t* s_scene_resource = NULL;
static picregion_resource_t* s_battery = NULL;

static unsigned short s_current_frame = -1;

/* 电池图片数据缓存 */
static gui_bitmap_info_t **s_bmp_battery = NULL;
static gui_bitmap_info_t *s_bmp_bg = NULL;

static scene_t* load_style_resource( rotation_e angle );
static void free_style_resource(void);

void set_charge_hdc( int hdc );
void paint_charge_initial_ui( void );
void update_battery_status( charge_status_t status );

static scene_charge_ui_t s_scene_charge_ui_api =
{
    load_style_resource,
    free_style_resource,

    set_charge_hdc,
    paint_charge_initial_ui,
    update_battery_status

};

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
        s_scene_resource = g_style_api->load_scene( g_usb_main->get_style(), CHARGE_SIDELONG_SCENE );
    }
    else
    {
        s_scene_resource = g_style_api->load_scene( g_usb_main->get_style(), CHARGE_UPRIGHT_SCENE );
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

        s_battery = ( picregion_resource_t* )g_style_api->get_scene_child( s_scene_resource, REG_BATTERY );
        if ( s_battery == NULL )
        {
            print_err(" usb_scene_charge_ui load s_battery failure ");
            return NULL;
        }
    }

    /* 提取图片资源数据 */
    s_bmp_battery = (gui_bitmap_info_t **)malloc(s_battery->frames * sizeof(gui_bitmap_info_t *));
    if(NULL == s_bmp_battery)
    {
        print_err("malloc failure!");
        return NULL;
    }

    memset(s_bmp_battery, 0, s_battery->frames * sizeof(gui_bitmap_info_t *));
    for(i = 0; i < s_battery->frames; i++)
    {
        get_picregion_frame(s_battery, (unsigned short)i, &pic_frame);
        s_bmp_battery[i] = sty_create_bitmap(pic_frame.pic_id);
        if(NULL == s_bmp_battery[i])
        {
            print_err("sty_create_bitmap failure!");
            return NULL;
        }
    }

    rect.x = s_battery->x;
    rect.y = s_battery->y;
    rect.width = s_battery->width;
    rect.height = s_battery->height;
    s_bmp_bg = sty_create_sub_bitmap( g_usb_main->get_background_picture()->pic_id, &rect );

    return s_scene_resource;
}

void set_charge_hdc( int hdc )
{
    s_hdc = hdc;
}

void paint_charge_initial_ui( void )
{
    sty_draw_picture_res( s_hdc, g_usb_main->get_background_picture() );
}

void update_battery_status( charge_status_t status )
{
    sty_rect_t rect;

    if( status == CHARGE_FULL )
    {
        s_current_frame = s_battery->frames - 1;
    }
    else
    {
        s_current_frame++;

        if ( s_current_frame >= s_battery->frames )
        {
            s_current_frame = 0;
        }
    }

    rect.x = s_battery->x;
    rect.y = s_battery->y;
    rect.width = s_battery->width;
    rect.height = s_battery->height;

#if 1
    sty_draw_bitmap(s_hdc, s_bmp_bg, rect.x, rect.y);
    sty_draw_bitmap(s_hdc, s_bmp_battery[s_current_frame], rect.x, rect.y);
#else
    /* 用背景覆盖原来的 */
    sty_draw_sub_picture_id( s_hdc, (g_usb_main->get_background_picture())->pic_id,
                            rect.x, rect.y, &rect );
    sty_draw_picregion_res( s_hdc, s_battery, s_current_frame );
#endif
}

static void free_style_resource(void)
{
    int i;

    for(i = 0; i < s_battery->frames; i++)
    {
        if(NULL != s_bmp_battery[i])
        {
            sty_delete_bitmap(s_bmp_battery[i]);
        }
    }
    sty_delete_bitmap(s_bmp_bg);
    free(s_bmp_battery);

    g_style_api->free_scene( s_scene_resource );
    s_scene_resource = NULL;
}


scene_charge_ui_t* create_scene_charge_ui(void)
{
    return &s_scene_charge_ui_api;
}
