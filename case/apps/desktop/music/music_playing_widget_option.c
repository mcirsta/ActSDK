/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_playing_widget_option.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_playing_widget_option.h
* \brief    music_playing_widget_option widget
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/
#include "music_playing_widget_option.h"

static opt_entry_t s_option_entry  = NULL;
static menuview_cb_t s_widget_option_cb = NULL;

static void _menuview_callback(menuview_t *menuview, menuview_sig_e signal)
{
}

menuview_t* music_playing_widget_option_create( menuview_cb_t callback )
{
    menuview_t *menuview;
    fusion_effect_t eff;
    
    //create list
    menuview = menuview_create( MENUVIEW_OPTION, _menuview_callback );
    if( menuview == NULL )
    {
        print_err("menuview create fail!"); 
        return NULL;
    }
    else
    {
        fusion_get_config(FUSION_TYPE_OPTION_RGB, FUSION_FORWARD, &eff);
        fusion_set_effect(&eff);
        
        //set first
        option_fusion_set_first_frame( menuview );
    
        //restore last menu
        if ( s_option_entry != NULL )
        {
            (s_option_entry)( menuview );
        }
        else
        {
            music_playing_option_enter( menuview );
        }
        
        //save callback
        s_widget_option_cb = callback;
        
        //set last and display
        option_fusion_set_last_frame( menuview );
        fusion_display(NULL);  
    }

    return menuview;
}

bool music_playing_widget_option_delete( menuview_t *menuview )
{
//    fusion_effect_t eff;
    
    if( menuview == NULL )
    {
        print_dbg("menuview delete fail!"); 
        return false;
    }
    else
    {
/*    
        //set first
        option_fusion_set_first_frame( menuview );
    
        //hide menuview
        menuview_hide( menuview );

        //set last and display
        option_fusion_set_last_frame( menuview );
        fusion_get_config(FUSION_TYPE_OPTION_RGB, FUSION_BACKWARD, &eff);
        fusion_display(&eff, NULL);         
*/
        //delete menuview
        menuview_delete( menuview );
        
        //delete menu
        //music_list_main_deinit();
        //music_list_allmusic_deinit();
    }
    
    return true;
}

bool music_playing_widget_option_load_style( menuview_t *menuview )
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

bool music_playing_widget_option_paint( menuview_t *menuview )
{
    return menuview_paint( menuview );
}

bool music_playing_widget_option_proc_gui( menuview_t *menuview, gui_wm_message_t *pmsg )
{
    return menuview_proc_gui( menuview, pmsg );
}

bool music_playing_widget_option_exit( menuview_t *menuview,  opt_entry_t entry )
{
    //save last menu entry
    s_option_entry = entry;

    //callback
    if ( s_widget_option_cb != NULL )
    {
        (s_widget_option_cb) ( menuview, WIDGET_SIG_MENUVIEW_QUIT );
        return true;
    }
    else
    {
        return false;
    }
}

