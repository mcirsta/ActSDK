/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : radio_playing_option_channellist.h.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason    2009-04-23          v1.0               build this file
 ********************************************************************************/
/*!
* \file     radio_playing_option_channellist.h.h
* \brief    FM播放界面场景下的option菜单频道列表相关操作
* \author   cheason
*
* \version 1.0
* \date  2009/02/25
*******************************************************************************/


#ifndef __RADIO_OPTION_CHANNELLIST_H__
#define __RADIO_OPTION_CHANNELLIST_H__

#include "radio_app.h"
#include "radio_scene_playing.h"

/* menu data init */
menu_t* radio_playing_option_channellist_init (bool save);
int radio_playing_option_channellist_deinit (void);

bool radio_playing_option_channellist_enter( menuview_t *menuview, bool save);
bool radio_playing_option_channellist_exit( menuview_t *menuview );

#endif  /* #ifndef __RADIO_OPTION_CHANNELLIST_H__ */

