/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_playing_option_playmode.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_playing_option_playmode.c
* \brief    music_playing_option_playmode widget
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#include "music_playing_option_playmode.h"

/* 退出主菜单 */
bool music_playing_option_playmode_exit( menuview_t *menuview );

/* 初始化菜单数据 */
menu_t* music_playing_option_playmode_init( void );

/* 删除菜单数据 */
int music_playing_option_playmode_deinit( void );

/* listmenu entry */
static int music_playing_option_playmode_process( menuview_t *menuview, menuview_sig_e signal );

/* menu list data define */
static menu_t *s_option_playmode = NULL;
static int s_option_playmode_offset = 0;
static int s_option_playmode_current = 0;

/* menu items data define */
static style_menuitem_t s_option_playmode_items[] =
{    
    // name, has_child, callback
    { STR_SEQUENCE,   false,  (item_cb_t)music_playing_option_playmode_process, 0, 0, 0, 0, 0 },
    { STR_REPEATONE,   false,   (item_cb_t)music_playing_option_playmode_process, 0, 0, 0, 0, 0 },
    { STR_REPEATALL,   false,   (item_cb_t)music_playing_option_playmode_process, 0, 0, 0, 0, 0 },
    { STR_SHUFFLE,   false,   (item_cb_t)music_playing_option_playmode_process, 0, 0, 0, 0, 0 },
    { STR_SHUFFLEREP,   false,   (item_cb_t)music_playing_option_playmode_process, 0, 0, 0, 0, 0 },
    { STR_INTRO,   false,   (item_cb_t)music_playing_option_playmode_process, 0, 0, 0, 0, 0 },
};

/* menu list max items */
#define MENU_OPTION_PLAYMODE_MAX_ITEMS  ( sizeof(s_option_playmode_items)/sizeof(style_menuitem_t) )


/* 进入主菜单 */
bool music_playing_option_playmode_enter( menuview_t *menuview )
{
    menu_t *cur_menu;

    cur_menu = music_playing_option_playmode_init();
    menuview_set_menu( menuview, cur_menu );
    
    if ( menuview->type != MENUVIEW_OPTION )
    {
        menuview_load_style( menuview, MENUVIEW_OPTION );
    }
    menuview_paint( menuview );

    return true;
}

/* 退出主菜单 */
bool music_playing_option_playmode_exit( menuview_t *menuview )
{
    //delete main menu
    music_playing_option_playmode_deinit();

    //reset index
    s_option_playmode_offset = 0;
    s_option_playmode_current = 0;

    //return
    music_playing_option_enter( menuview );    

    return true;
}

menu_t* music_playing_option_playmode_init( void )
{
    menuitem_t item;
    unsigned int num;
    mengine_config_t *config = &g_scene_playing->engine.config;   
    
    //initial menu    
    if( s_option_playmode == NULL )
    {
        //create menu
        s_option_playmode = menu_create();

        //load style
        menu_load_style( PLAYMODE_MENU_RESGROUP, s_option_playmode_items, MENU_OPTION_PLAYMODE_MAX_ITEMS );
        
        //init items
        memset( &item, 0, sizeof(menuitem_t) );
        
        for ( num=0; num<MENU_OPTION_PLAYMODE_MAX_ITEMS; num++ )
        {
            item.text_id = s_option_playmode_items[num].str_id;
            item.has_child = s_option_playmode_items[num].has_child;
            item.callback = s_option_playmode_items[num].callback;
            
            if ( (play_mode_e)num == config->play_mode )
            {
                item.unsel_icon_id = (int)g_scene_playing->pic_tick->pic_id;
                item.sel_icon_id = (int)g_scene_playing->pic_tick->pic_id;
            }
            else
            {
                item.unsel_icon_id = 0;
                item.sel_icon_id = 0;
            }
            menu_append_item( s_option_playmode, &item );
        }

        //init index
        menu_set_offset( s_option_playmode, s_option_playmode_offset );
        //menu_set_cur_index( s_option_playmode, s_option_playmode_current );
        menu_set_cur_index( s_option_playmode, (int)config->play_mode );
    }

    return s_option_playmode;
}

int music_playing_option_playmode_deinit( void )
{    
    //delete menu    
    if( s_option_playmode != NULL )
    {
        //save index
        s_option_playmode_offset = s_option_playmode->offset;
        s_option_playmode_current = s_option_playmode->current;
        
        //create menu
        menu_delete( s_option_playmode );
        s_option_playmode = NULL;
    }

    return true;
}

static int music_playing_option_playmode_process( menuview_t *menuview, menuview_sig_e signal )
{        
    menu_t *cur_menu = menuview->menu;
    menuitem_t *cur_item = menu_get_cur_item( cur_menu );
    mengine_config_t *config = &g_scene_playing->engine.config;    
    
    //check item
    if ( cur_item == NULL )
    {
        return false;
    }

    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        if( cur_menu->current != config->play_mode )
        { 
            //set new tick
            cur_item->unsel_icon_id = (int)g_scene_playing->pic_tick->pic_id;
            cur_item->sel_icon_id = (int)g_scene_playing->pic_tick->pic_id;

            //clear old tick
            cur_item = menu_get_item( cur_menu, config->play_mode );
            if ( cur_item != NULL )
            {
                cur_item->unsel_icon_id = 0;
                cur_item->sel_icon_id = 0;
            }
                
            //repaint
            menuview_paint( menuview );                
        } 
        
        //get loop mode        
        config->play_mode = cur_menu->current;
                
        //set usereq
        music_set_playmode( config->play_mode );     
        
        //paint config info
        playing_paint_config( g_scene_playing );    
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_playing_option_playmode_exit( menuview );
        break;
         
    default:
        break;
    }    

    return true;
}



