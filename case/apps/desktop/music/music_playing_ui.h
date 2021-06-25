/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_playing_ui.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_playing_ui.c
* \brief    music_playing_ui widget
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/
    
#ifndef __PLAYING_UI_H__
#define __PLAYING_UI_H__

//#include "music.h"

/* music_playing external interface */
bool playing_paint_bg( scene_play_t *scene_play );
bool playing_paint_file_info( scene_play_t *scene_play );
bool playing_paint_config( scene_play_t *scene_play );
bool playing_paint_effect( scene_play_t *scene_play );
bool playing_paint_progress( scene_play_t *scene_play );
bool playing_paint_button( scene_play_t *scene_play );
bool playing_paint_album( scene_play_t *scene_play );

#endif

