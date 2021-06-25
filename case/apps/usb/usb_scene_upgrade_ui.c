/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : usb_scene_upgrade_ui.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    zhaowenjie    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     usb_scene_upgrade_ui.c
* \brief    upgrade scene ui
* \author   zhaowenjie
*               
* \version 1.0
* \date  2009/06/03
*******************************************************************************/

#include "usb.h"
#include "usb_scene_upgrade_ui.h"

#define UPGRADE_TIMER_INTERVAL 100

static int s_hdc;

static scene_t* s_scene_resource = NULL;

static picregion_resource_t* s_wait_picregion = NULL;
static string_resource_t* s_upgrade_string = NULL;
static picture_resource_t* s_bar_picture = NULL;


gui_bitmap_info_t* s_background = NULL;
gui_bitmap_info_t* s_height_light = NULL;
gui_bitmap_info_t* s_wait_frames[ 10 ];

static unsigned short s_percent = 0;
static unsigned short s_frame = -1;

static int s_timer = NULL_TIMER;
static unsigned int s_timer_interval = UPGRADE_TIMER_INTERVAL;


static scene_t* load_style_resource(void);
static void free_style_resource(void);

void paint_upgrade_initial_ui( int hdc );
void update_process_bar( unsigned short percent );

static int _proc_timer( void * param );


static scene_upgrade_ui_t s_scene_upgrade_ui_api = 
{
    load_style_resource,
    free_style_resource,

    paint_upgrade_initial_ui,
    update_process_bar

};


static scene_t* load_style_resource(void)
{
    rotation_e angle;

    typedef struct pic_id_s
    {
        short pic_id;
        
    }__attribute__ ((packed))
    pic_id_t;
    
    picture_resource_t* bar_hl_picture = NULL;

    angle = sty_get_view_rotation();
    if ( ( angle == ROTATION_0 ) ||  ( angle == ROTATION_180 ) )
    {
        s_scene_resource = g_style_api->load_scene( g_usb_main->get_style(), UPGRADE_SIDELONG_SCENE );
    }
    else
    {
        s_scene_resource = g_style_api->load_scene( g_usb_main->get_style(), UPGRADE_UPRIGHT_SCENE );
    }

//	    s_scene_resource = g_style_api->load_scene( g_usb_main->get_style(), UPGRADE_SCENE );
    if( s_scene_resource == NULL)
    {
        print_err(" usb_scene_udisk_ui load scene failure "); 
        return NULL;
    }
    else
    {        
        /* 设置应用style场景资源 */
        sty_set_app_scene( s_scene_resource );

        s_wait_picregion = ( picregion_resource_t* )g_style_api->get_scene_child( s_scene_resource, REG_WAIT );
        s_upgrade_string = ( string_resource_t* )g_style_api->get_scene_child( s_scene_resource, STR_UPGRADE );
        s_bar_picture = ( picture_resource_t* )g_style_api->get_scene_child( s_scene_resource, PIC_BAR );
        bar_hl_picture = ( picture_resource_t* )g_style_api->get_scene_child( s_scene_resource, PIC_BAR_HL );
        if( ( s_bar_picture == NULL ) || ( bar_hl_picture == NULL ) )
        {
            print_err(" upgrade scene load picture error "); 
            return false;
        }

        s_height_light = sty_create_bitmap( bar_hl_picture->pic_id );
        
        if ( s_wait_picregion != NULL )
        {
            pic_id_t* pid_p;
            unsigned short* ids;
            unsigned short i;
            
            sty_rect_t rect;

            ids = ( unsigned short* )( (int)s_wait_picregion + (int)s_wait_picregion->pic_offset );
            for ( i = 0; i < s_wait_picregion->frames; i++ )
            {
                pid_p = ( pic_id_t* )( ids + i );
            
                s_wait_frames[ i ] = sty_create_bitmap( pid_p->pic_id );
            }

            rect.x = s_wait_picregion->x;
            rect.y = s_wait_picregion->y;
            rect.width = s_wait_picregion->width;
            rect.height = s_wait_picregion->height;
            s_background = sty_create_sub_bitmap( g_usb_main->get_background_picture()->pic_id, &rect );
        }
    }

    return s_scene_resource;
}

void paint_upgrade_initial_ui( int hdc )
{
    s_hdc = hdc;
    
    sty_draw_picture_res( hdc, g_usb_main->get_background_picture() );
    sty_draw_picture_res( hdc, s_bar_picture );

    if ( s_upgrade_string != NULL )
    {
        sty_draw_string_res( hdc, s_upgrade_string );
    }

    s_timer = set_timer( s_timer_interval, _proc_timer, NULL );
}

static int _proc_timer( void * param )
{
    int x;
    int y;
    
    if ( s_wait_picregion != NULL )
    {
        /* 循环所有的帧 */
        s_frame++;
        if ( s_frame >= s_wait_picregion->frames )
        {
            s_frame = 0;
        }
    
        x = s_wait_picregion->x;
        y = s_wait_picregion->y;

        sty_draw_bitmap( s_hdc, s_background, x, y );
        sty_draw_bitmap( s_hdc, s_wait_frames[ s_frame ], x, y );        
    }

    
    
    return 0;
}

void update_process_bar( unsigned short percent )
{
    short distance;
    int i;
    int x;
    
    distance = (short)( percent - s_percent );
    if ( distance <= 0 )
    {
        return;
    }

    for ( i = 0; i < distance; i++ )
    {
        x = s_bar_picture->x + s_percent + i;

        sty_draw_bitmap( s_hdc, s_height_light, x, ( s_bar_picture->y + 1 ) );
    }

    s_percent = percent;
}


static void free_style_resource(void)
{
    int i;
    
    if ( s_timer != NULL_TIMER )
    {
        kill_timer( s_timer );
        s_timer = NULL_TIMER;
    }

    if ( s_background != NULL )
    {
        sty_delete_bitmap( s_background );
        s_background = NULL;
    }

    if ( s_height_light != NULL )
    {
        sty_delete_bitmap( s_height_light );
        s_height_light = NULL;
    }

    if ( s_wait_picregion != NULL )
    {
        for ( i = 0; i < s_wait_picregion->frames; i++ )
        {
            sty_delete_bitmap( s_wait_frames[ i ] );
        }
    }

    if ( s_scene_resource != NULL )
    {
        g_style_api->free_scene( s_scene_resource );
        s_scene_resource = NULL;
    }
    
}



scene_upgrade_ui_t* create_scene_upgrade_ui(void)
{
    return &s_scene_upgrade_ui_api;
}

