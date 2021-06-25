/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Jerry Fu    2009-05-19          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     tvin_record_ui.h
* \brief    tvin record ui
* \author   Jerry Fu
*               
* \version 1.0
* \date  2009/05/19
*******************************************************************************/

#ifndef __TVIN_RECORD_UI_H__
#define __TVIN_RECORD_UI_H__

#include "tvin.h"

#define TVIN_DEFAULT_BLENDING_COLOR_KEY   (0)//0xffff00

#define TVIN_DEFAULT_BLENDING_ALPHA     (52)

#define TVIN_LETF_BLENDING_COLOR_KEY (0xff00ff)


void tvin_draw_headbar(tvin_scene_record_t *scene_record);
void tvin_draw_pbar_bg(tvin_scene_record_t *scene_record); 
void tvin_draw_pbar(tvin_scene_record_t *scene_record);
void tvin_draw_status(tvin_scene_record_t *scene_record);
void tvin_draw_status_bg(tvin_scene_record_t *scene_record);
void tvin_draw_recordtime(tvin_scene_record_t *scene_record);
void tvin_draw_remaintime(tvin_scene_record_t *scene_record);
bool tvin_create_bitmap(void);
bool tvin_delete_bitmap(void);

#endif  /* __VIDEO_UI_PLAYING_H__ */

