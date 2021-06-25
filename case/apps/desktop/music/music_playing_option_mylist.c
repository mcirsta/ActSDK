/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_playing_option_mylist.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_playing_option_mylist.c
* \brief    music_playing_option_mylist widget
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#include "music_playing_option_mylist.h"

/* 退出主菜单 */
bool music_playing_option_mylist_exit( menuview_t *menuview );

/* 初始化菜单数据 */
menu_t* music_playing_option_mylist_init( void );

/* 删除菜单数据 */
int music_playing_option_mylist_deinit( void );

/* listmenu entry */
static int _proc_items( menuview_t *menuview, menuview_sig_e signal );

/* menu list max items */
#define MYLIST_MAX_ITEMS  5

/* const strings */
static char *str_myplaylist[MYLIST_MAX_ITEMS] =
{
    "My Playlist 1",
    "My Playlist 2",
    "My Playlist 3",
    "My Playlist 4",
    "My Playlist 5",
};

/* menu list data define */
static menu_t *s_mylist = NULL;
static int s_mylist_offset = 0;
static int s_mylist_current = 0;


/* 进入主菜单 */
bool music_playing_option_mylist_enter( menuview_t *menuview )
{
    menu_t *cur_menu;

    cur_menu = music_playing_option_mylist_init();
    menuview_set_menu( menuview, cur_menu );
    
    if ( menuview->type != MENUVIEW_OPTION )
    {
        menuview_load_style( menuview, MENUVIEW_OPTION );
    }
    menuview_paint( menuview );

    return true;
}

/* 退出主菜单 */
bool music_playing_option_mylist_exit( menuview_t *menuview )
{
    //delete main menu
    music_playing_option_mylist_deinit();

    s_mylist_offset = 0;
    s_mylist_current = 0;

    //return
    music_playing_option_enter( menuview );    
    
    return true;
}

menu_t* music_playing_option_mylist_init( void )
{
    menuitem_t item;
    int num;
    
    //initial menu    
    if( s_mylist == NULL )
    {
        //create menu
        s_mylist = menu_create();

        //init items
        memset( &item, 0, sizeof(menuitem_t) );
        
        for ( num=0; num<MYLIST_MAX_ITEMS; num++ )
        {
            item.text = str_myplaylist[num];
            item.has_child = false;
            item.callback = (item_cb_t)_proc_items;
            
            menu_append_item( s_mylist, &item );
        }

        //init current index
        menu_set_offset( s_mylist, s_mylist_offset );
        menu_set_cur_index( s_mylist, s_mylist_current );
    }

    return s_mylist;
}

int music_playing_option_mylist_deinit( void )
{    
    //delete menu    
    if( s_mylist != NULL )
    {
        //save index
        s_mylist_offset = s_mylist->offset;
        s_mylist_current = s_mylist->current;
        
        //create menu
        menu_delete( s_mylist );
        s_mylist = NULL;
    }

    return true;
}

static int _proc_items( menuview_t *menuview, menuview_sig_e signal )
{        
    bool bret;
    dialog_init_t dialog_init;
    menu_t *cur_menu = menuview->menu;
    
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        //add to mylist
        bret = plistclass_add_item_to_mylist( cur_menu->current + 1, g_scene_playing->id3_info );
        if ( bret == false )
        {
            //模态提示框
            if (g_scene_playing->str_add_fail != NULL)
            {
                dialog_init.head = DIALOG_HEAD_WARN;
                dialog_init.str_id = g_scene_playing->str_add_fail->str_id;
                dialog_modal( &dialog_init );
            }
        }
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_playing_option_mylist_exit( menuview );
        break;
         
    default:
        break;
    }    

    return true;
}



