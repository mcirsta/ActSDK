/********************************************************************************
*                              USDK(1100)
*                             Module: app
*                 Copyright(c) 2003-2009 Actions Semiconductor,
*                            All Rights Reserved.
* File : tvin_record_widget_option.c
* History:
*      <author>    <time>           <version >             <desc>
*      Jerry Fu    2009-4-28             v1.0               build this file
********************************************************************************/
/*!
* \file     tvin_record_widget_option.c
* \brief    创建录像选择菜单的menuview; 退出并且销毁所创建的menuview
* \author   Jerry Fu
*
* \version 1.0
* \date  2009/04/28
*******************************************************************************/
#include "tvin.h"

static void _menuview_callback(menuview_t *menuview, menuview_sig_e signal)
{

}


bool tvin_widget_menu_load_style(menuview_t *mv)
{
    if ( mv != NULL )
    {
        return menuview_load_style( mv, mv->type );
    }
    else
    {
        return false;
    }
}

menuview_t* tvin_widget_option_create(void)
{
    menuview_t *menuview;
    tvin_scene_record_t  *prec = NULL;
    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return NULL;
    }
    
    menuview = menuview_create( MENUVIEW_OPTION, _menuview_callback);
    if( menuview == NULL )
    {
        print_err("menuview create fail!");
        return NULL;
    }
    else
    {       
        prec->pRecMView = menuview;        
        tvin_option_enter( menuview, FUSION_FORWARD );
    }    
    return menuview;
}

bool tvin_widget_option_delete( menuview_t *menuview )
{
    tvin_scene_record_t *prec;
    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return false;
    }
    
    if( menuview == NULL )
    {
        print_err("menuview delete fail!");
        return false;
    }
    else
    {        
        menuview_delete( menuview ); 
        prec->pRecMView = NULL;
        return true;
    }
}

bool tvin_widget_palntsc_list_proc_gui( menuview_t *menuview, gui_wm_message_t *pmsg )
{       
    return menuview_proc_gui( menuview, pmsg );     
}


