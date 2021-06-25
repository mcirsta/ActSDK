/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_playing_ui.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *      cheason0   2009-06-14          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_playing_ui.h
 * \brief    ui operation of photo playing scene
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/06/14
 *******************************************************************************/
#ifndef __PHOTO_PLAYING_UI_H__
#define __PHOTO_PLAYING_UI_H__

#include "photo.h"
#include "photo_scene_playing.h"

void draw_directbar(scene_playing_t *p, bool show);
void draw_txtbar(scene_playing_t *p, bool show);
void draw_headbar(scene_playing_t *p, bool show);
void draw_common_canvas(scene_playing_t *p);
void draw_picture(scene_playing_t *p, fusion_mode_e mode);

#endif

