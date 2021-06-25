/********************************************************************************
*                              USDK(1100)
*                             Module: app
*                 Copyright(c) 2003-2009 Actions Semiconductor,
*                            All Rights Reserved.
* File : tvin_palntsc_widget_list.c
* History:
*      <author>    <time>           <version >             <desc>
*      Jerry Fu    2009-4-28             v1.0               build this file
********************************************************************************/
/*!
* \file     tvin_palntsc_widget_list.c
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


menuview_t* tvin_widget_palntsc_list_create(void)
{
    menuview_t *menuview;
    menuview = menuview_create( MENUVIEW_MENULIST_ICON, _menuview_callback);
    if( menuview == NULL )
    {
        print_err("menuview create fail!");
        return NULL;
    }

    if(false == tvin_palntsc_list_enter(menuview))
    {
        return NULL;
    }
    return menuview;
}

bool tvin_widget_palntsc_list_delete( menuview_t *menuview )
{
    tvin_scene_palntsc_t *palntsc;
    palntsc = fetch_tvin_palntsc();
    if(NULL == palntsc)
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
        palntsc->menuview = NULL;
    }
    return true;
}

bool tvin_widget_option_proc_gui( menuview_t *menuview, gui_wm_message_t *pmsg )
{       
    return menuview_proc_gui( menuview, pmsg );     
}


