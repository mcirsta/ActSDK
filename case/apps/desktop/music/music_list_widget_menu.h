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
* \brief    music_playing widget
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#ifndef __MUSIC_LIST_WIDGET_MENU_H__
#define __MUSIC_LIST_WIDGET_MENU_H__


#include "music.h"


typedef bool (*menu_entry_t)( menuview_t *, fusion_enter_e );

menuview_t* music_list_widget_menu_create( menuview_cb_t callback, fusion_enter_e fenter );
bool music_list_widget_menu_delete( menuview_t *menuview );
bool music_list_widget_menu_load_style( menuview_t *menuview );
bool music_list_widget_menu_paint( menuview_t *menuview );
bool music_list_widget_menu_proc_gui( menuview_t *menuview, gui_wm_message_t *pmsg );
bool music_list_widget_menu_exit( menuview_t *menuview,  menu_entry_t entry );


#endif
