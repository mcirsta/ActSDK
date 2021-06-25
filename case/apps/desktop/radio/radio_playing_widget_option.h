/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : test_menuview_menu_ui.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    wangsheng    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     radio_playing_widget_option.h
* \brief    radio_playing widget
* \author   wangsheng
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#ifndef __RADIO_PLAYING_WIDGET_OPTION_H__
#define __RADIO_PLAYING_WIDGET_OPTION_H__


#include "include_case.h"


#include "radio_app.h"
#include "radio_scene_playing.h"

#include "radio_playing_option.h"
#include "radio_playing_option_level.h"
#include "radio_playing_option_band.h"
#include "radio_playing_option_path.h"
#include "radio_playing_option_channellist.h"
#include "radio_playing_option_userchannellist.h"
#include "radio_playing_option_userfilelist.h"

typedef bool (*option_entry_t)( menuview_t * );

menuview_t* radio_playing_widget_option_create( menuview_cb_t callback );
bool radio_playing_widget_option_delete( menuview_t *menuview );
bool radio_playing_widget_option_exit( menuview_t *menuview,  option_entry_t entry );

#endif

