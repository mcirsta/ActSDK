/********************************************************************************/
/*!
 * \file     radio_ui_recording.h
 * \brief    routine of radio scene recording ui
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/04/23
 *******************************************************************************/

#ifndef __RADIO_UI_RECORDING_H__
#define __RADIO_UI_RECORDING_H__
#include "radio_scene_recording.h"

void fm_recorder_draw_bg( scene_recording_t *param );
void fm_recorder_draw_word_status( scene_recording_t *param );
void fm_recorder_draw_filename( scene_recording_t *param );
void fm_recorder_draw_pbbg( scene_recording_t *param );
void fm_recorder_draw_pbhl_curtime( void *data );
void fm_recorder_draw_totaltime( scene_recording_t *param );
void fm_recorder_draw_color_status( scene_recording_t *param );
void fm_recorder_draw_tape( scene_recording_t *param );
void fm_recorder_draw_vol( void *data);
void fm_recorder_draw_vol1( scene_recording_t *param );
void fm_recorder_draw_vol2( scene_recording_t *param );
void fm_recorder_draw_vol3( scene_recording_t *param );
void fm_recorder_draw_vol4( scene_recording_t *param );
void fm_recorder_draw_vol5( scene_recording_t *param );
#endif
