/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : radio_ui_playing.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      cheason    2009-04-23          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     radio_ui_playing.h
 * \brief    routine of radio scene playing ui
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/04/23
 *******************************************************************************/

#ifndef __RADIO_UI_PLAYING_H__
#define __RADIO_UI_PLAYING_H__

#include "radio_app.h"
#include "radio_scene_playing.h"

void show_intensity_pic(radio_scene_playing_t *data, bool show_bg);
void show_stereo_pic(radio_scene_playing_t *data, bool show_bg);
void show_cur_freq(radio_scene_playing_t *data, bool show_bg);
void show_start_end_freq_str(radio_scene_playing_t *data, bool show_bg);
void scene_playing_style_init(radio_scene_playing_t *data);
void show_channel_name(radio_scene_playing_t *playing);

extern bool g_force_show_channel_name;


#endif /* __RADIO_UI_PLAYING_H__ */

