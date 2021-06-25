/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_playing_option_level.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_playing_option_level.c
* \brief    music_playing_option_level widget
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#include "music_playing_option_level.h"

/* 退出主菜单 */
bool music_playing_option_level_exit( menuview_t *menuview );

/* 初始化菜单数据 */
menu_t* music_playing_option_level_init( void );

/* 删除菜单数据 */
int music_playing_option_level_deinit( void );

/* listmenu entry */
static int music_playing_option_level_process( menuview_t *menuview, menuview_sig_e signal );

/* menu list data define */
static menu_t *s_option_level = NULL;
static int s_option_level_offset = 0;
static int s_option_level_current = 0;

/* menu items data define */
static style_menuitem_t s_option_level_items[] =
{    
    // name, has_child, callback
    { STR_LEVEL1,   false,   (item_cb_t)music_playing_option_level_process, 0, 0, 0, 0, 0 },
    { STR_LEVEL2,   false,   (item_cb_t)music_playing_option_level_process, 0, 0, 0, 0, 0 },
    { STR_LEVEL3,   false,   (item_cb_t)music_playing_option_level_process, 0, 0, 0, 0, 0 },
    { STR_LEVEL4,   false,   (item_cb_t)music_playing_option_level_process, 0, 0, 0, 0, 0 },
    { STR_LEVEL5,   false,   (item_cb_t)music_playing_option_level_process, 0, 0, 0, 0, 0 },
};

/* menu list max items */
#define MENU_OPTION_LEVEL_MAX_ITEMS  ( sizeof(s_option_level_items)/sizeof(style_menuitem_t) )

/* 进入主菜单 */
bool music_playing_option_level_enter( menuview_t *menuview )
{
    menu_t *cur_menu;

    cur_menu = music_playing_option_level_init();
    menuview_set_menu( menuview, cur_menu );
    
    if ( menuview->type != MENUVIEW_OPTION )
    {
        menuview_load_style( menuview, MENUVIEW_OPTION );
    }
    menuview_paint( menuview );

    return true;
}

/* 退出主菜单 */
bool music_playing_option_level_exit( menuview_t *menuview )
{
    //delete main menu
    music_playing_option_level_deinit();

    //reset index
    s_option_level_offset = 0;
    s_option_level_current = 0;

    //return
    music_playing_option_enter( menuview );    

    return true;
}

menu_t* music_playing_option_level_init( void )
{
    menuitem_t item;
    unsigned int num;
    
    //initial menu    
    if( s_option_level == NULL )
    {
        //create menu
        s_option_level = menu_create();

        //load style
        menu_load_style( LEVEL_MENU_RESGROUP, s_option_level_items, MENU_OPTION_LEVEL_MAX_ITEMS );
        
        //init items
        memset( &item, 0, sizeof(menuitem_t) );
        
        for ( num=0; num<MENU_OPTION_LEVEL_MAX_ITEMS; num++ )
        {
            item.text_id = s_option_level_items[num].str_id;
            item.has_child = s_option_level_items[num].has_child;
            item.callback = s_option_level_items[num].callback;
            
            if ( (int)num == ( g_scene_playing->id3_info->rating - 1 ) )
            {
                item.unsel_icon_id = (int)g_scene_playing->pic_tick->pic_id;
                item.sel_icon_id = (int)g_scene_playing->pic_tick->pic_id;
            }
            else
            {
                item.unsel_icon_id = 0;
                item.sel_icon_id = 0;
            }
            menu_append_item( s_option_level, &item );
        }

        //init index
        menu_set_offset( s_option_level, s_option_level_offset );
        menu_set_cur_index( s_option_level, s_option_level_current );

    }

    return s_option_level;
}

int music_playing_option_level_deinit( void )
{    
    //delete menu    
    if( s_option_level != NULL )
    {
        //save index
        s_option_level_offset = s_option_level->offset;
        s_option_level_current = s_option_level->current;
        
        //create menu
        menu_delete( s_option_level );
        s_option_level = NULL;
    }

    return true;
}

static int music_playing_option_level_process( menuview_t *menuview, menuview_sig_e signal )
{        
    menu_t *cur_menu = menuview->menu;
    menuitem_t *cur_item = menu_get_cur_item( cur_menu );  
    bool bret;

    //check item
    if ( cur_item == NULL )
    {
        return false;
    }

    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        //change rating
        bret = plistclass_change_rating( g_scene_playing->id3_info->fullpath, cur_menu->current + 1 ); 
        if ( bret == false )
        {
            break;
        }

        if( cur_menu->current != ( g_scene_playing->id3_info->rating - 1 ) )
        { 
            //set new tick
            cur_item->unsel_icon_id = (int)g_scene_playing->pic_tick->pic_id;
            cur_item->sel_icon_id = (int)g_scene_playing->pic_tick->pic_id;

            //clear old tick
            if ( ( g_scene_playing->id3_info->rating - 1 ) >= 0 )
            {
                cur_item = menu_get_item( cur_menu, g_scene_playing->id3_info->rating - 1 );
                if ( cur_item != NULL )
                {
                    cur_item->unsel_icon_id = 0;
                    cur_item->sel_icon_id = 0;
                }
            }
            
            //repaint
            menuview_paint( menuview );                
        } 
        
        //update rating
        g_scene_playing->id3_info->rating = cur_menu->current + 1;
        
        //paint file info
        playing_paint_file_info( g_scene_playing );
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_playing_option_level_exit( menuview );
        break;
         
    default:
        break;
    }    

    return true;
}



