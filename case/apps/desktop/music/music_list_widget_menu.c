/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_list_widget_menu.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_list_widget_menu.h
* \brief    music_list_widget_menu widget
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/
#include "music_list_widget_menu.h"

static menu_entry_t s_menu_entry  = NULL;
static menuview_cb_t s_widget_menu_cb = NULL;

static void _menuview_callback(menuview_t *menuview, menuview_sig_e signal)
{
}

menuview_t* music_list_widget_menu_create( menuview_cb_t callback, fusion_enter_e fenter )
{
    menuview_t *menuview;
    
    //create list
    menuview = menuview_create( MENUVIEW_MENULIST_TEXT, _menuview_callback );
    if( menuview == NULL )
    {
        print_err("menuview create fail!"); 
        return NULL;
    }
    else
    {
        //prefetch setting
        menuview_set_prefetch_num( menuview, SIDE_SLIDE_NUM );
        
        //restore last menu
        if ( s_menu_entry != NULL )
        {
            (s_menu_entry)( menuview, fenter );
        }
        else
        {
            music_list_mainmenu_enter( menuview, fenter );
        }
        
        //save callback
        s_widget_menu_cb = callback;
    }

    return menuview;
}

bool music_list_widget_menu_delete( menuview_t *menuview )
{
    if( menuview == NULL )
    {
        print_dbg("menuview delete fail!"); 
        return false;
    }
    else
    {
        //delete menuview
        menuview_delete( menuview );

        //delete menu
//        music_list_main_deinit();
//        music_list_allmusic_deinit();
    }

    return true;
}

bool music_list_widget_menu_load_style( menuview_t *menuview )
{
    if ( menuview != NULL )
    {
        return menuview_load_style( menuview, menuview->type );
    }
    else
    {
        return false;
    }
}

bool music_list_widget_menu_paint( menuview_t *menuview )
{
    return menuview_paint( menuview );
}

bool music_list_widget_menu_proc_gui( menuview_t *menuview, gui_wm_message_t *pmsg )
{
    return menuview_proc_gui( menuview, pmsg );
}

bool music_list_widget_menu_exit( menuview_t *menuview,  menu_entry_t entry )
{
    //save last menu entry
    s_menu_entry = entry;

    //callback
    if ( s_widget_menu_cb != NULL )
    {
        (s_widget_menu_cb) ( menuview, WIDGET_SIG_MENUVIEW_QUIT );
        return true;
    }
    else
    {
        return false;
    }
}

