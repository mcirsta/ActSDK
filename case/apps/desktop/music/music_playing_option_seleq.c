/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_playing_option_seleq.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_playing_option_seleq.c
* \brief    music_playing_option_seleq widget
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#include "music_playing_option_seleq.h"

/* 退出主菜单 */
bool music_playing_option_seleq_exit( menuview_t *menuview );

/* 初始化菜单数据 */
menu_t* music_playing_option_seleq_init( void );

/* 删除菜单数据 */
int music_playing_option_seleq_deinit( void );

/* listmenu entry */
static int music_playing_option_seleq_process( menuview_t *menuview, menuview_sig_e signal );

/* menu list data define */
static menu_t *s_option_seleq = NULL;
static int s_option_seleq_offset = 0;
static int s_option_seleq_current = 0;

/* menu items data define */
static style_menuitem_t s_option_seleq_items[] =
{    
    // name, has_child, callback
    { STR_NORMAL,   false,  (item_cb_t)music_playing_option_seleq_process, 0, 0, 0, 0, 0 },
    { STR_POP,   false,   (item_cb_t)music_playing_option_seleq_process, 0, 0, 0, 0, 0 },
    { STR_CLASSIC,   false,   (item_cb_t)music_playing_option_seleq_process, 0, 0, 0, 0, 0 },
    { STR_SOFT,   false,   (item_cb_t)music_playing_option_seleq_process, 0, 0, 0, 0, 0 },
    { STR_JAZZ,   false,   (item_cb_t)music_playing_option_seleq_process, 0, 0, 0, 0, 0 },
    { STR_ROCK,   false,   (item_cb_t)music_playing_option_seleq_process, 0, 0, 0, 0, 0 },
    { STR_DBB,   false,   (item_cb_t)music_playing_option_seleq_process, 0, 0, 0, 0, 0 },
    { STR_CUSTOM,   false,   (item_cb_t)music_playing_option_seleq_process, 0, 0, 0, 0, 0 },
    { STR_SRSWOW,   false,   (item_cb_t)music_playing_option_seleq_process, 0, 0, 0, 0, 0 },
    { STR_SRSWOWHD,   false,   (item_cb_t)music_playing_option_seleq_process, 0, 0, 0, 0, 0 },
    { STR_SRSUSER,   false,   (item_cb_t)music_playing_option_seleq_process, 0, 0, 0, 0, 0 },
};

/* menu list max items */
#define MENU_OPTION_SELEQ_MAX_ITEMS  ( sizeof(s_option_seleq_items)/sizeof(style_menuitem_t) )

/* eq value for save */
static eq_mode_e s_save_eq_mode;

/* 进入主菜单 */
bool music_playing_option_seleq_enter( menuview_t *menuview )
{
    menu_t *cur_menu;

    cur_menu = music_playing_option_seleq_init();
    menuview_set_menu( menuview, cur_menu );
    
    if ( menuview->type != MENUVIEW_OPTION )
    {
        menuview_load_style( menuview, MENUVIEW_OPTION );
    }
    menuview_paint( menuview );
    
    return true;
}

/* 退出主菜单 */
bool music_playing_option_seleq_exit( menuview_t *menuview )
{
    //delete main menu
    music_playing_option_seleq_deinit();

    //reset index
    s_option_seleq_offset = 0;
    s_option_seleq_current = 0;

    //return
    music_playing_option_enter( menuview );
    
    return true;
}

menu_t* music_playing_option_seleq_init( void )
{
    menuitem_t item;
    unsigned int num;
    mengine_eq_t *eq_val = &g_scene_playing->engine.config.eq_val;    
    
    //initial menu    
    if( s_option_seleq == NULL )
    {
        //create menu
        s_option_seleq = menu_create();

        //load style
        menu_load_style( SELEQ_MENU_RESGROUP, s_option_seleq_items, MENU_OPTION_SELEQ_MAX_ITEMS );

        //init items
        memset( &item, 0, sizeof(menuitem_t) );
        
        for ( num=0; num<MENU_OPTION_SELEQ_MAX_ITEMS; num++ )
        {
            item.text_id = s_option_seleq_items[num].str_id;
            item.has_child = s_option_seleq_items[num].has_child;
            item.callback = s_option_seleq_items[num].callback;
            
            if ( (eq_mode_e)num == eq_val->eq_mode )
            {
                item.unsel_icon_id = (int)g_scene_playing->pic_tick->pic_id;
                item.sel_icon_id = (int)g_scene_playing->pic_tick->pic_id;
            }
            else
            {
                item.unsel_icon_id = 0;
                item.sel_icon_id = 0;
            }
            menu_append_item( s_option_seleq, &item );
        }
        
        //init index
        menu_set_offset( s_option_seleq, s_option_seleq_offset );
        //menu_set_cur_index( s_option_seleq, s_option_seleq_current );
        menu_set_cur_index( s_option_seleq, (int)eq_val->eq_mode );
    }

    //save current eq
    s_save_eq_mode = eq_val->eq_mode;

    return s_option_seleq;
}

int music_playing_option_seleq_deinit( void )
{    
    mengine_eq_t *eq_val = &g_scene_playing->engine.config.eq_val;
    
    //delete menu    
    if( s_option_seleq != NULL )
    {
        //save index
        s_option_seleq_offset = s_option_seleq->offset;
        s_option_seleq_current = s_option_seleq->current;
        
        //create menu
        menu_delete( s_option_seleq );
        s_option_seleq = NULL;
    }
    
    //restore eq        
    eq_val->eq_mode = s_save_eq_mode;
    
    //restore usereq
    music_set_eq( eq_val );    

    return true;
}

static int music_playing_option_seleq_process( menuview_t *menuview, menuview_sig_e signal )
{        
    menu_t *cur_menu = menuview->menu;
    menuitem_t *cur_item = menu_get_cur_item( cur_menu );
    mengine_eq_t *eq_val = &g_scene_playing->engine.config.eq_val;   

    //check item
    if ( cur_item == NULL )
    {
        return false;
    }
    
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        if( cur_menu->current != (int)s_save_eq_mode )
        { 
            //set new tick
            cur_item->unsel_icon_id = (int)g_scene_playing->pic_tick->pic_id;
            cur_item->sel_icon_id = (int)g_scene_playing->pic_tick->pic_id;

            //clear old tick
            cur_item = menu_get_item( cur_menu, (int)s_save_eq_mode );
            if ( cur_item != NULL )
            {
                cur_item->unsel_icon_id = 0;
                cur_item->sel_icon_id = 0;
            }
                        
            //repaint
            menuview_paint( menuview );                
        }
        
        //set eq        
        eq_val->eq_mode = cur_menu->current;
        
        //save current eq
        s_save_eq_mode = eq_val->eq_mode;

        // return to parent menu
        music_playing_option_seleq_exit( menuview );
        
        //paint config info
        playing_paint_config( g_scene_playing );
        break;


    case WIDGET_SIG_MENUVIEW_UP:
    case WIDGET_SIG_MENUVIEW_DOWN:
        //get usereq        
        eq_val->eq_mode = cur_menu->current;

        //set usereq
        music_set_eq( eq_val );
        break;


    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_playing_option_seleq_exit( menuview );
        break;

         
    default:
        break;
    }    

    return true;
}



