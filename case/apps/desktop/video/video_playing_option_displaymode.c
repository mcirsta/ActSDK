/*******************************************************************************
 *                              NOYA1100
 *                            Module: video
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 * History:
 *      <author>    <time>           <version >             <desc>
 *      dengjihai    2009-11-27          1.0             build this file
*******************************************************************************/
/*!
 * \file     video_playing_option_displaymode.c
 * \brief
 * \author   dengjihai
 * \par      GENERAL DESCRIPTION:
 *
 * \par      EXTERNALIZED FUNCTIONS:
 *
 * \version 1.0
 * \date  2009/11/27
*******************************************************************************/


#include "include_case.h"

/* include file of this application, 本应用的头文件 */
#include "include_video.h"


/* extern function, 函数声明 */
static video_displaymode_e _get_displaymode_by_menu_offset( int displaymode_index );
static int _get_menu_offset_by_displaymode( video_displaymode_e displaymode );
static menu_t * _option_displaymode_init( void );
static bool _option_displaymode_deinit( void );
static int _proc_item_displaymode_select( menuview_t *menuview, menuview_sig_e signal );


/* define variable, 变量定义 */
static menu_t *s_option_displaymode = NULL;
//修改这里的顺序, 请一并修改_get_displaymode_by_menu_offset, 和_get_menu_offset_by_displaymode
const static video_menu_info_t s_option_displaymode_info[] =
{
    { STR_FULL_SCREEN,  0, 0 },
    { STR_ORG_SCREEN,   0, 0 },
    { STR_EQU_SCREEN,   0, 0 },
};
/* menu items */
static menuitem_t s_option_displaymode_items[] =
{
    { 0, 0, 0, false,  (item_cb_t)_proc_item_displaymode_select, NULL, NULL, NULL, NULL },
    { 0, 0, 0, false,  (item_cb_t)_proc_item_displaymode_select, NULL, NULL, NULL, NULL },
    { 0, 0, 0, false,  (item_cb_t)_proc_item_displaymode_select, NULL, NULL, NULL, NULL },
};


#if 0
#endif
/* 进入主菜单 */
bool video_playing_option_displaymode_enter( menuview_t *menuview )
{
    menu_t *cur_menu;

    cur_menu = _option_displaymode_init();
    menuview_set_menu( menuview, cur_menu );

    menuview_load_style( menuview, MENUVIEW_OPTION );
    menuview_paint( menuview );

    return true;
}


/* 退出主菜单 */
bool video_playing_option_displaymode_exit( void )
{
    //delete main menu
    _option_displaymode_deinit();

    return true;
}


static video_displaymode_e _get_displaymode_by_menu_offset( int displaymode_index )
{
/*
    video_displaymode_e displaymode;
    switch( displaymode_index )
    {
    case 0:
        displaymode = VIDEO_DISPLAYMOE_FULL;
        break;

    case 1:
        displaymode = VIDEO_DISPLAYMOE_ORG;
        break;

    case 2:
        displaymode = VIDEO_DISPLAYMOE_EQU;
        break;

    default:
        break;
    }
    return displaymode;
*/
    return (video_displaymode_e)displaymode_index;
}


static int _get_menu_offset_by_displaymode( video_displaymode_e displaymode )
{
/*
    int index;

    switch( displaymode )
    {
    case VIDEO_DISPLAYMOE_FULL:
        index = 0;
        break;

    case VIDEO_DISPLAYMOE_ORG:
        index = 1;
        break;

    case VIDEO_DISPLAYMOE_EQU:
        index = 2;
        break;

    default:
        break;
    }

    return index;
*/
    return (int)displaymode;
}


static menu_t * _option_displaymode_init( void )
{
    menuitem_t menuitem;
    unsigned int i;
    int index_cur;

    //initial menu
    if( s_option_displaymode == NULL )
    {
        //create menu
        s_option_displaymode = menu_create();
        video_load_menuitem( OPTION_DISPLAY_MODE_RESGROUP, s_option_displaymode_info, \
                    sizeof(s_option_displaymode_items)/sizeof(menuitem_t), s_option_displaymode_items );

        //init items
        memset( &menuitem, 0, sizeof(menuitem_t) );
        for ( i=0; i<(sizeof(s_option_displaymode_items)/sizeof(menuitem_t)); i++ )
        {
            menuitem.text_id   = s_option_displaymode_items[i].text_id;
            menuitem.has_child = s_option_displaymode_items[i].has_child;
            menuitem.callback  = s_option_displaymode_items[i].callback;

            if(video_vars.play_param.displaymode == (video_displaymode_e)i)
            {
                menuitem.unsel_icon_id = video_vars.scene_playing.pic_select_status->pic_id;
                menuitem.sel_icon_id = video_vars.scene_playing.pic_select_status->pic_id;
            }
            else
            {
                menuitem.unsel_icon_id = 0;
                menuitem.sel_icon_id = 0;
            }

            menu_append_item( s_option_displaymode, &menuitem );
        }

        //init current index
        index_cur = _get_menu_offset_by_displaymode( video_vars.play_param.displaymode );
        menu_set_cur_index( s_option_displaymode, index_cur );
    }

    return s_option_displaymode;
}


static bool _option_displaymode_deinit( void )
{
    //delete menu
    if( NULL == s_option_displaymode )
    {
        return false;
    }

    menu_delete( s_option_displaymode );
    s_option_displaymode = NULL;

    return true;
}


static int _proc_item_displaymode_select( menuview_t *menuview, menuview_sig_e signal )
{
    video_displaymode_e displaymode;
    int index_cur = 0;
    menu_t *cur_menu = menuview->menu;
    menuitem_t *cur_item = menu_get_cur_item( cur_menu );

    if(NULL == cur_item)
    {
        return -1;
    }

    switch ( signal )
    {
    case WIDGET_SIG_MENUVIEW_OK:
        menuview_paint( menuview );
        index_cur = menuview->menu->global_offset + menuview->menu->current;
        displaymode = _get_displaymode_by_menu_offset( index_cur );
        if(video_vars.play_param.displaymode != displaymode)
        {
            cur_item->unsel_icon_id = (int)video_vars.scene_playing.pic_select_status->pic_id;
            cur_item->sel_icon_id = (int)video_vars.scene_playing.pic_select_status->pic_id;

            cur_item = menu_get_item( cur_menu, video_vars.play_param.displaymode );
            if ( cur_item != NULL )
            {
                cur_item->unsel_icon_id = 0;
                cur_item->sel_icon_id = 0;
            }

            video_set_display_mode( displaymode );

        }
        video_playing_option_displaymode_exit();
        video_playing_option_enter( menuview, FUSION_BACKWARD );
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        video_playing_option_displaymode_exit();
        video_playing_option_enter( menuview, FUSION_BACKWARD );
        break;

    default:
        break;
    }
    return 1;
}

