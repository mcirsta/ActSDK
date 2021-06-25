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
 * \file     video_list_widget_menu.c
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
static void _menuview_callback(menuview_t *menuview, menuview_sig_e signal);


/* define variable, 变量定义 */
static menu_entry_t s_menu_entry  = NULL;
static menuview_cb_t s_widget_menu_cb = NULL;



#if 0
#endif
//shaofeng 作用??
static void _menuview_callback(menuview_t *menuview, menuview_sig_e signal)
{
}

//shaofeng 与menuview_load_style完全重复, 不需要封一层
bool video_list_widget_menu_load_style( menuview_t *menuview )
{
    if ( NULL == menuview )
    {
        return false;
    }
    
    return menuview_load_style( menuview, menuview->type );
}

//shaofeng 与menuview_paint完全重复, 不需要封一层
bool video_list_widget_menu_paint( menuview_t *menuview )
{
    return menuview_paint( menuview );
}

//保存退出menu时的断点入口函数:entry
bool video_list_widget_menu_exit( menuview_t *menuview,  menu_entry_t entry )
{
    //save last menu entry
    s_menu_entry = entry;

    //callback
    if ( NULL == s_widget_menu_cb )
    {
        return false;
    }
    if ( NULL == menuview )
    {
        return false;
    }
    
    (s_widget_menu_cb) ( menuview, WIDGET_SIG_MENUVIEW_QUIT );
    
    return true;
}

bool video_list_widget_menu_create( menuview_cb_t callback )
{
    //create menuview
    video_vars.menuview = menuview_create( MENUVIEW_MENULIST_TEXT, _menuview_callback );
    if( video_vars.menuview == NULL )
    {
        print_err("menuview create fail!"); 
        return false;
    }
    
    if ( s_menu_entry != NULL )
    {
        (s_menu_entry)( video_vars.menuview, FUSION_BACKWARD );
    }
    else
    {
        video_list_mainmenu_enter( video_vars.menuview, FUSION_FORWARD );
    }
    
    //save callback
    s_widget_menu_cb = callback;

    return true;
}

bool video_list_widget_menu_delete( menuview_t *menuview )
{
    video_list_taglist_exit();
    video_list_mainmenu_exit();
    
    if( menuview == NULL )
    {
        print_warning("menuview is NULL"); 
        return false;
    }
    //delete menuview
    menuview_delete( menuview );
    
    return true;
}

