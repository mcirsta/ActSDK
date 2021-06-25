/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_list_recfile.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_list_recfile.c
* \brief    music_list_recfile widget
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#include "music_list_recfile.h"

/* 退出主菜单 */
bool music_list_recfile_exit( menuview_t *menuview );

/* 初始化菜单数据 */
menu_t* music_list_recfile_init( void );

/* 删除菜单数据 */
int music_list_recfile_deinit( void );

/* listmenu entry */
static int _proc_item_voicerec( menuview_t *menuview, menuview_sig_e signal );
static int _proc_item_fmrec( menuview_t *menuview, menuview_sig_e signal );
static int _proc_item_lineinrec( menuview_t *menuview, menuview_sig_e signal );

/* menu list data define */
static menu_t *s_recfile = NULL;
static int s_recfile_offset = 0;
static int s_recfile_current = 0;

/* menu items data define */
static style_menuitem_t s_recfile_items[] =
{    
    // name, has_child, callback
    { STR_VOICEREC,   true,   (item_cb_t)_proc_item_voicerec, 0, 0, 0, 0, 0 },
    { STR_LINEINREC,   true,   (item_cb_t)_proc_item_lineinrec, 0, 0, 0, 0, 0 },
    { STR_FMREC,   true,   (item_cb_t)_proc_item_fmrec, 0, 0, 0, 0, 0 },
};

/* menu list max items */
#define MENU_LIST_RECFILE_MAX_ITEMS  ( sizeof(s_recfile_items)/sizeof(style_menuitem_t) )



/* 进入主菜单 */
bool music_list_recfile_enter( menuview_t *menuview, fusion_enter_e fenter )
{
    menu_t *cur_menu;
    fusion_effect_t eff;

    if( fenter < FUSION_MAX )
    {
        fusion_get_config(FUSION_TYPE_COMMON, fenter, &eff);
        fusion_set_effect(&eff);
        scene_fusion_set_first_frame(g_scene_list->scene); 
    }  

    cur_menu = music_list_recfile_init();
    menuview_set_menu( menuview, cur_menu );
    
    if ( menuview->type != MENUVIEW_MENULIST_TEXT )
    {
        menuview_load_style( menuview, MENUVIEW_MENULIST_TEXT );
    }
    menuview_paint( menuview );

    if( fenter < FUSION_MAX )
    {
        scene_fusion_set_last_frame(g_scene_list->scene);
        fusion_display(NULL);
    }

    return true;
}

/* 退出主菜单 */
bool music_list_recfile_exit( menuview_t *menuview )
{
    //delete main menu
    music_list_recfile_deinit();

    s_recfile_offset = 0;
    s_recfile_current = 0;

    //return
    plistclass_prev_level( g_plist_level );
    music_list_mainmenu_enter( menuview, FUSION_BACKWARD );    
    
    return true;
}

menu_t* music_list_recfile_init( void )
{
    menuitem_t item;
    int num;
    
    //initial menu    
    if( s_recfile == NULL )
    {
        //create menu
        s_recfile = menu_create();

        //load style
        menu_load_style( REC_MENU_RESGROUP, s_recfile_items, MENU_LIST_RECFILE_MAX_ITEMS );
        
        //init items
        memset( &item, 0, sizeof(menuitem_t) );
        
        for ( num=0; num<(int)MENU_LIST_RECFILE_MAX_ITEMS; num++ )
        {
            item.text_id = s_recfile_items[num].str_id;
            item.has_child = s_recfile_items[num].has_child;
            item.callback = s_recfile_items[num].callback;
            
            menu_append_item( s_recfile, &item );
        }

        //init current index
        menu_set_offset( s_recfile, s_recfile_offset );
        menu_set_cur_index( s_recfile, s_recfile_current );
    }

    return s_recfile;
}

int music_list_recfile_deinit( void )
{    
    //delete menu    
    if( s_recfile != NULL )
    {
        //save index
        s_recfile_offset = s_recfile->offset;
        s_recfile_current = s_recfile->current;
        
        //create menu
        menu_delete( s_recfile );
        s_recfile = NULL;
    }

    return true;
}

static int _proc_item_voicerec( menuview_t *menuview, menuview_sig_e signal )
{
    const char * dir = NULL;
    
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        dir = sys_get_main_disk_dir(DIR_RECORDER_MIC);
        if ( dir != NULL )
        {
            //enter filelist
            music_list_recfile_deinit();
            strcpy( g_dirlist_path, dir );
            strcpy( g_dirlist_top, dir );
            music_list_dirlist_enter( menuview, FUSION_FORWARD );
        }
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_list_recfile_exit( menuview );
        break;
         
    default:
        break;
    }    
    
    return true;    
}

static int _proc_item_fmrec( menuview_t *menuview, menuview_sig_e signal )
{
    const char * dir = NULL;
    
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        dir = sys_get_main_disk_dir(DIR_RADIO_RECORD);
        if ( dir != NULL )
        {
            //enter filelist
            music_list_recfile_deinit();
            strcpy( g_dirlist_path, dir );
            strcpy( g_dirlist_top, dir );
            music_list_dirlist_enter( menuview, FUSION_FORWARD );
        }
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_list_recfile_exit( menuview );
        break;
         
    default:
        break;
    }    
    
    return true;    
}

static int _proc_item_lineinrec( menuview_t *menuview, menuview_sig_e signal )
{
    const char * dir = NULL;
    
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        dir = sys_get_main_disk_dir(DIR_RECORDER_LINEIN);
        if ( dir != NULL )
        {
            //enter filelist
            music_list_recfile_deinit();
            strcpy( g_dirlist_path, dir );
            strcpy( g_dirlist_top, dir );
            music_list_dirlist_enter( menuview, FUSION_FORWARD );
        }
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_list_recfile_exit( menuview );
        break;
         
    default:
        break;
    }    
    
    return true;    
}



