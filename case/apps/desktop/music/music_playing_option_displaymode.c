/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_playing_option_displaymode.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_playing_option_displaymode.c
* \brief    music_playing_option_displaymode widget
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#include "music_playing_option_displaymode.h"

/* 退出主菜单 */
bool music_playing_option_displaymode_exit( menuview_t *menuview );

/* 初始化菜单数据 */
menu_t* music_playing_option_displaymode_init( void );

/* 删除菜单数据 */
int music_playing_option_displaymode_deinit( void );

/* listmenu entry */
static int music_playing_option_displaymode_process( menuview_t *menuview, menuview_sig_e signal );

/* menu list data define */
static menu_t *s_option_displaymode = NULL;
static int s_option_displaymode_offset = 0;
static int s_option_displaymode_current = 0;

/* menu items data define */
static style_menuitem_t s_option_displaymode_items[] =
{    
    // name, has_child, callback
    { STR_SPECT,   false,  (item_cb_t)music_playing_option_displaymode_process, 0, 0, 0, 0, 0 },
    { STR_LYRIC,   false,   (item_cb_t)music_playing_option_displaymode_process, 0, 0, 0, 0, 0 },
    { STR_EFFECT1,   false,   (item_cb_t)music_playing_option_displaymode_process, 0, 0, 0, 0, 0 },
    { STR_EFFECT2,   false,   (item_cb_t)music_playing_option_displaymode_process, 0, 0, 0, 0, 0 },
//    { STR_EFFECT3,   false,   (item_cb_t)music_playing_option_displaymode_process, 0, 0, 0, 0, 0 },
};

/* menu list max items */
#define MENU_OPTION_DISPLAYMODE_MAX_ITEMS  ( sizeof(s_option_displaymode_items)/sizeof(style_menuitem_t) )

/* menu effect start */
#define MENU_OPTION_EFFECT_START    2

/* 进入主菜单 */
bool music_playing_option_displaymode_enter( menuview_t *menuview )
{
    menu_t *cur_menu;

    cur_menu = music_playing_option_displaymode_init();
    menuview_set_menu( menuview, cur_menu );
    
    if ( menuview->type != MENUVIEW_OPTION )
    {
        menuview_load_style( menuview, MENUVIEW_OPTION );
    }
    menuview_paint( menuview );

    return true;
}

/* 退出主菜单 */
bool music_playing_option_displaymode_exit( menuview_t *menuview )
{
    //delete main menu
    music_playing_option_displaymode_deinit();

    //reset index
    s_option_displaymode_offset = 0;
    s_option_displaymode_current = 0;

    //return
    music_playing_option_enter( menuview );
    
    return true;
}

menu_t* music_playing_option_displaymode_init( void )
{
    menuitem_t item;
    unsigned int num;
    mengine_config_t *config = &g_scene_playing->engine.config;   
    
    //initial menu    
    if( s_option_displaymode == NULL )
    {
        //create menu
        s_option_displaymode = menu_create();

        //load style
        menu_load_style( DISPMODE_MENU_RESGROUP, s_option_displaymode_items, MENU_OPTION_DISPLAYMODE_MAX_ITEMS );
        
        //init items
        memset( &item, 0, sizeof(menuitem_t) );
        
        for ( num=0; num<MENU_OPTION_DISPLAYMODE_MAX_ITEMS; num++ )
        {
            item.text_id = s_option_displaymode_items[num].str_id;
            item.has_child = s_option_displaymode_items[num].has_child;
            item.callback = s_option_displaymode_items[num].callback;
            
            if ( (display_mode_e)num == config->display_mode )
            {
                item.unsel_icon_id = (int)g_scene_playing->pic_tick->pic_id;
                item.sel_icon_id = (int)g_scene_playing->pic_tick->pic_id;
            }
            else
            {
                item.unsel_icon_id = 0;
                item.sel_icon_id = 0;
            }
            
            if ( ( num >= MENU_OPTION_EFFECT_START ) && ( config->display_effect != true ) )
            {
                continue;
            }
            
            menu_append_item( s_option_displaymode, &item );
        }

        //init index
        menu_set_offset( s_option_displaymode, s_option_displaymode_offset );
        menu_set_cur_index( s_option_displaymode, s_option_displaymode_current );

    }

    return s_option_displaymode;
}

int music_playing_option_displaymode_deinit( void )
{    
    //delete menu    
    if( s_option_displaymode != NULL )
    {
        //save index
        s_option_displaymode_offset = s_option_displaymode->offset;
        s_option_displaymode_current = s_option_displaymode->current;
        
        //create menu
        menu_delete( s_option_displaymode );
        s_option_displaymode = NULL;
    }

    return true;
}

static int music_playing_option_displaymode_process( menuview_t *menuview, menuview_sig_e signal )
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
        if( cur_menu->current != config->display_mode )
        { 
            //set new tick
            cur_item->unsel_icon_id = (int)g_scene_playing->pic_tick->pic_id;
            cur_item->sel_icon_id = (int)g_scene_playing->pic_tick->pic_id;

            //clear old tick
            cur_item = menu_get_item( cur_menu, config->display_mode );
            if ( cur_item != NULL )
            {
                cur_item->unsel_icon_id = 0;
                cur_item->sel_icon_id = 0;
            }
            
            //repaint
            menuview_paint( menuview );                
        } 

        //get display_mode      
        config->display_mode = cur_menu->current;
                
        //set usereq
        music_set_displaymode( config->display_mode );   
        
        //paint config info
        playing_paint_config( g_scene_playing );     
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_playing_option_displaymode_exit( menuview );
        break;
         
    default:
        break;
    }    

    return true;
}



