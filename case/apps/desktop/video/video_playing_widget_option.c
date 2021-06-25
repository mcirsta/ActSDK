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
 * \file     video_playing_widget_option.c
 * \brief    
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2009/05/15
*******************************************************************************/

#include "include_psp.h"
#include "include_case.h"

/* include file of this application, 本应用的头文件 */
#include "include_video.h"


/* extern function, 函数声明 */
static void _menuview_callback(menuview_t *menuview, menuview_sig_e signal);


/* define variable, 变量定义 */
static option_entry_t s_option_entry  = NULL;
static menuview_cb_t s_widget_option_cb = NULL;



#if 0
#endif
static void _menuview_callback(menuview_t *menuview, menuview_sig_e signal)
{
}

#if 0
bool video_playing_widget_option_load_style( menuview_t *menuview )
{
    if ( NULL == menuview )
    {
        return false;
    }
    
    return menuview_load_style( menuview, menuview->type );
}

bool video_playing_widget_option_paint( menuview_t *menuview )
{
    return menuview_paint( menuview );
}

bool video_playing_widget_option_proc_gui( menuview_t *menuview, gui_wm_message_t *pmsg )
{
    return menuview_proc_gui( menuview, pmsg );
}

bool video_playing_widget_option_exit( menuview_t *menuview,  option_entry_t entry )
{
    //save last menu entry
    s_option_entry = entry;

    //callback
    if ( NULL == s_widget_option_cb )
    {
        return false;
    }
    if ( NULL == menuview )
    {
        return false;
    }
    
    (s_widget_option_cb) ( menuview, WIDGET_SIG_MENUVIEW_QUIT );
    
    return true;
}
#endif

bool video_playing_widget_option_create( menuview_cb_t callback )
{
    //create menuview
    video_vars.menuview = menuview_create( MENUVIEW_OPTION, _menuview_callback );
    if( video_vars.menuview == NULL )
    {
        print_err("menuview create fail!"); 
        return false;
    }
    
    if ( s_option_entry != NULL )
    {
        (s_option_entry)( video_vars.menuview );
    }
    else
    {
        video_playing_option_enter( video_vars.menuview, FUSION_FORWARD );
    }
    
    //save callback
    s_widget_option_cb = callback;

    return true;
}

bool video_playing_widget_option_delete( menuview_t *menuview )
{
    video_playing_option_playmode_exit();
    video_playing_option_info_exit();
    video_playing_option_exit();
    
    if( menuview == NULL )
    {
        print_warning("menuview is NULL"); 
        return false;
    }
    //delete menuview
    menuview_delete( menuview );
    
    return true;
}

