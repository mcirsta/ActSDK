/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_list_level.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_list_level.c
* \brief    music_list_level widget
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#include "music_list_level.h"

/* 退出主菜单 */
bool music_list_level_exit( menuview_t *menuview );

/* 初始化菜单数据 */
menu_t* music_list_level_init( void );

/* 删除菜单数据 */
int music_list_level_deinit( void );

/* listmenu entry */
static int music_list_level_process( menuview_t *menuview, menuview_sig_e signal );

/* menu list data define */
static menu_t *s_level = NULL;
static int s_level_offset = 0;
static int s_level_current = 0;

/* menu items data define */
static style_menuitem_t s_level_items[] =
{    
    // name, has_child, callback
    { STR_LEVEL1,   true,   (item_cb_t)music_list_level_process, 0, 0, 0, 0, 0 },
    { STR_LEVEL2,   true,   (item_cb_t)music_list_level_process, 0, 0, 0, 0, 0 },
    { STR_LEVEL3,   true,   (item_cb_t)music_list_level_process, 0, 0, 0, 0, 0 },
    { STR_LEVEL4,   true,   (item_cb_t)music_list_level_process, 0, 0, 0, 0, 0 },
    { STR_LEVEL5,   true,   (item_cb_t)music_list_level_process, 0, 0, 0, 0, 0 },
};

/* menu list max items */
#define MENU_LIST_LEVEL_MAX_ITEMS  ( sizeof(s_level_items)/sizeof(style_menuitem_t) )



/* 进入主菜单 */
bool music_list_level_enter( menuview_t *menuview, fusion_enter_e fenter )
{
    menu_t *cur_menu;
    fusion_effect_t eff;

    fusion_get_config(FUSION_TYPE_COMMON, fenter, &eff);
    fusion_set_effect(&eff);
    scene_fusion_set_first_frame(g_scene_list->scene);

    cur_menu = music_list_level_init();
    menuview_set_menu( menuview, cur_menu );
    
    if ( menuview->type != MENUVIEW_MENULIST_TEXT )
    {
        menuview_load_style( menuview, MENUVIEW_MENULIST_TEXT );
    }
    menuview_paint( menuview );

    scene_fusion_set_last_frame(g_scene_list->scene);
    fusion_display(NULL);

    return true;
}

/* 退出主菜单 */
bool music_list_level_exit( menuview_t *menuview )
{
    //delete main menu
    music_list_level_deinit();
    
    s_level_offset = 0;
    s_level_current = 0;

    //return
    plistclass_prev_level( g_plist_level );
    music_list_mainmenu_enter( menuview, FUSION_BACKWARD );    
    
    return true;
}

menu_t* music_list_level_init( void )
{
    menuitem_t item;
    unsigned int num;
    
    //initial menu    
    if( s_level == NULL )
    {
        //create menu
        s_level = menu_create();

        //load style
        menu_load_style( LEVEL_MENU_RESGROUP, s_level_items, MENU_LIST_LEVEL_MAX_ITEMS );
        
        //init items
        memset( &item, 0, sizeof(menuitem_t) );
        
        for ( num = 0; num < MENU_LIST_LEVEL_MAX_ITEMS; num ++ )
        {
            item.text_id = s_level_items[num].str_id;
            item.has_child = s_level_items[num].has_child;
            item.callback = s_level_items[num].callback;
            
            menu_append_item( s_level, &item );
        }

        //init current index
        menu_set_offset( s_level, s_level_offset );
        menu_set_cur_index( s_level, s_level_current );
    }

    return s_level;
}

int music_list_level_deinit( void )
{    
    //delete menu    
    if( s_level != NULL )
    {
        //save index
        s_level_offset = s_level->offset;
        s_level_current = s_level->current;
        
        //create menu
        menu_delete( s_level );
        s_level = NULL;
    }

    return true;
}

static int music_list_level_process( menuview_t *menuview, menuview_sig_e signal )
{        
    menu_t *cur_menu = menuview->menu;
    
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        //save selected content
        g_usrsel_content.rating = cur_menu->current + 1;
        plistclass_store_usrselcontent( RATING, &g_usrsel_content );
        
        //set level info
        plistclass_next_level( g_plist_level, LEVEL_TWO_LIST, LEVEL_THREE_FILELIST );
    
        //enter filelist
        music_list_level_deinit();
        music_list_filelist_enter( menuview, FUSION_FORWARD );
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_list_level_exit( menuview );
        break;
            
    default:
        break;
    }    

    return true;
}



