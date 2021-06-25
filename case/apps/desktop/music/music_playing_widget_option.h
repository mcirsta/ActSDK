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
* \brief    music_playing widget
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#ifndef __MUSIC_PLAYING_WIDGET_OPTION_H__
#define __MUSIC_PLAYING_WIDGET_OPTION_H__


#include "music.h"

typedef bool (*opt_entry_t)( menuview_t * );

menuview_t* music_playing_widget_option_create( menuview_cb_t callback );
bool music_playing_widget_option_delete( menuview_t *menuview );
bool music_playing_widget_option_load_style( menuview_t *menuview );
bool music_playing_widget_option_paint( menuview_t *menuview );
bool music_playing_widget_option_proc_gui( menuview_t *menuview, gui_wm_message_t *pmsg );
bool music_playing_widget_option_exit( menuview_t *menuview,  opt_entry_t entry );


#endif
