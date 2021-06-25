/*******************************************************************************
 *                              NOYA1100
 *                            Module: video
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 * History:
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-05-15          1.0             build this file
*******************************************************************************/
/*!
 * \file     video_playing_option_playmode.c
 * \brief
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *
 * \par      EXTERNALIZED FUNCTIONS:
 *
 * \version 1.0
 * \date  2009/05/15
*******************************************************************************/


#include "include_case.h"

/* include file of this application, 本应用的头文件 */
#include "include_video.h"


/* extern function, 函数声明 */
static video_playmode_e _get_playmode_by_menu_offset( int playmode_index );
static int _get_menu_offset_by_playmode( video_playmode_e playmode );
static menu_t * _option_playmode_init( void );
static bool _option_playmode_deinit( void );
static int _proc_item_playmode_select( menuview_t *menuview, menuview_sig_e signal );


/* define variable, 变量定义 */
static menu_t *s_option_playmode = NULL;
//修改这里的顺序, 请一并修改_get_playmode_by_menu_offset, 和_get_menu_offset_by_playmode
const static video_menu_info_t s_option_playmode_info[] =
{
    { STR_SEQUENCE,     0, 0 },
    { STR_REPEAT_ONE,   0, 0 },
    { STR_REPEAT_ALL,   0, 0 },
    { STR_SHUFFLE,      0, 0 },
    { STR_SHUFFLE_REPEAT, 0, 0 },
};
/* menu items */
static menuitem_t s_option_playmode_items[] =
{
    { 0, 0, 0, false,  (item_cb_t)_proc_item_playmode_select, NULL, NULL, NULL, NULL },
    { 0, 0, 0, false,  (item_cb_t)_proc_item_playmode_select, NULL, NULL, NULL, NULL },
    { 0, 0, 0, false,  (item_cb_t)_proc_item_playmode_select, NULL, NULL, NULL, NULL },
    { 0, 0, 0, false,  (item_cb_t)_proc_item_playmode_select, NULL, NULL, NULL, NULL },
    { 0, 0, 0, false,  (item_cb_t)_proc_item_playmode_select, NULL, NULL, NULL, NULL },
};



#if 0
#endif
/* 进入主菜单 */
bool video_playing_option_playmode_enter( menuview_t *menuview )
{
    menu_t *cur_menu;

    cur_menu = _option_playmode_init();
    menuview_set_menu( menuview, cur_menu );

    menuview_load_style( menuview, MENUVIEW_OPTION );
    menuview_paint( menuview );

    return true;
}


/* 退出主菜单 */
bool video_playing_option_playmode_exit( void )
{
    //delete main menu
    _option_playmode_deinit();

    return true;
}


static video_playmode_e _get_playmode_by_menu_offset( int playmode_index )
{
/*
    video_playmode_e playmode;
    switch( playmode_index )
    {
    case 0:
        playmode = VIDEO_PLAYMOE_SEQUENCE;
        break;

    case 1:
        playmode = VIDEO_PLAYMOE_REPEAT_ONE;
        break;

    case 2:
        playmode = VIDEO_PLAYMOE_REPEAT_ALL;
        break;

    case 3:
        playmode = VIDEO_PLAYMOE_SHUFFLE;
        break;

    case 4:
        playmode = VIDEO_PLAYMOE_SHUFFLE_REPEAT;
        break;

    default:
        break;
    }
    return playmode;
*/
    return (video_playmode_e)playmode_index;
}


static int _get_menu_offset_by_playmode( video_playmode_e playmode )
{
/*
    int index;

    switch( playmode )
    {
    case VIDEO_PLAYMOE_SEQUENCE:
        index = 0;
        break;

    case VIDEO_PLAYMOE_REPEAT_ONE:
        index = 1;
        break;

    case VIDEO_PLAYMOE_REPEAT_ALL:
        index = 2;
        break;

    case VIDEO_PLAYMOE_SHUFFLE:
        index = 3;
        break;

    case VIDEO_PLAYMOE_SHUFFLE_REPEAT:
        index = 4;
        break;

    default:
        break;
    }

    return index;
*/
    return (int)playmode;
}


static menu_t * _option_playmode_init( void )
{
    menuitem_t menuitem;
    unsigned int i;
    int index_cur;

    //initial menu
    if( s_option_playmode == NULL )
    {
        //create menu
        s_option_playmode = menu_create();
        video_load_menuitem( OPTION_PLAYMODE_RESGROUP, s_option_playmode_info, \
                    sizeof(s_option_playmode_items)/sizeof(menuitem_t), s_option_playmode_items );

        //init items
        memset( &menuitem, 0, sizeof(menuitem_t) );
        for ( i=0; i<(sizeof(s_option_playmode_items)/sizeof(menuitem_t)); i++ )
        {
            menuitem.text_id   = s_option_playmode_items[i].text_id;
            menuitem.has_child = s_option_playmode_items[i].has_child;
            menuitem.callback  = s_option_playmode_items[i].callback;

            if(video_vars.play_param.playmode == (video_playmode_e)i)
            {
                menuitem.unsel_icon_id = video_vars.scene_playing.pic_select_status->pic_id;
                menuitem.sel_icon_id = video_vars.scene_playing.pic_select_status->pic_id;
            }
            else
            {
                menuitem.unsel_icon_id = 0;
                menuitem.sel_icon_id = 0;
            }

            menu_append_item( s_option_playmode, &menuitem );
        }

        //init current index
        index_cur = _get_menu_offset_by_playmode( video_vars.play_param.playmode );
        menu_set_cur_index( s_option_playmode, index_cur );
    }

    return s_option_playmode;
}


static bool _option_playmode_deinit( void )
{
    //delete menu
    if( NULL == s_option_playmode )
    {
        return false;
    }

    menu_delete( s_option_playmode );
    s_option_playmode = NULL;

    return true;
}


static int _proc_item_playmode_select( menuview_t *menuview, menuview_sig_e signal )
{
    video_playmode_e playmode;
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
        playmode = _get_playmode_by_menu_offset( index_cur );
        if(video_vars.play_param.playmode != playmode)
        {
            cur_item->unsel_icon_id = (int)video_vars.scene_playing.pic_select_status->pic_id;
            cur_item->sel_icon_id = (int)video_vars.scene_playing.pic_select_status->pic_id;

            cur_item = menu_get_item( cur_menu, video_vars.play_param.playmode );
            if ( cur_item != NULL )
            {
                cur_item->unsel_icon_id = 0;
                cur_item->sel_icon_id = 0;
            }

            video_set_play_mode( playmode );

        }
        video_playing_option_playmode_exit();
        video_playing_option_enter( menuview, FUSION_BACKWARD );
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        video_playing_option_playmode_exit();
        video_playing_option_enter( menuview, FUSION_BACKWARD );
        break;

    default:
        break;
    }
    return 1;
}

