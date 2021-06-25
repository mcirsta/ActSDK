#ifndef __RECORDER_UI_RECORDING_H__
#define __RECORDER_UI_RECORDING_H__

#include "recorder_scene_recording.h"
#include "recorder_sty.h"

void recorder_draw_bg( scene_recording_t *param );
void recorder_draw_word_status( scene_recording_t *param );
void recorder_draw_filename( scene_recording_t *param );
void recorder_draw_pbbg( scene_recording_t *param );
void recorder_draw_pbhl_curtime( void *data );
void recorder_draw_totaltime( scene_recording_t *param );
void recorder_draw_color_status( scene_recording_t *param );
void recorder_draw_tape( scene_recording_t *param );
void recorder_draw_vol( void *data);
void recorder_draw_vol1( scene_recording_t *param );
void recorder_draw_vol2( scene_recording_t *param );
void recorder_draw_vol3( scene_recording_t *param );
void recorder_draw_vol4( scene_recording_t *param );
void recorder_draw_vol5( scene_recording_t *param );

#endif
