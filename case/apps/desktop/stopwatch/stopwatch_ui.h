/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * History:
 *      <author>    <time>           <version >             <desc>
 *      dupengcheng    2009-04-01          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     stopwatch_ui.h
 * \brief
 * \author   dupengcheng
 *
 * \version 1.0
 * \date  2009/04/30
 *******************************************************************************/

#ifndef __STOPWATCH_UI_H__
#define __STOPWATCH_UI_H__

#include "stopwatch_scene.h"
#include "stopwatch_ctl.h"

extern bool timeTostr(watchvalue_t *time, char *pstr);
extern bool stopwatch_draw_bg(scene_stopwatch_t *my_scene);
extern bool stopwatch_draw_frame(scene_stopwatch_t *my_scene, int fb_id, int frame);
extern bool when_key_next_draw(scene_stopwatch_t* my_scene, int old_lable);
extern bool draw_watch1_idx(scene_stopwatch_t *my_scene);
extern bool draw_cur_watch_time(scene_stopwatch_t *my_scene);
extern bool update_watch_head(scene_stopwatch_t *my_scene);
extern unsigned int onTimer(void *para);

//////////////////////////////////////////
extern void paint_necessary_when_rotate(scene_stopwatch_t *my_scene);
extern void get_watch_string_rect(int idx, gui_rect_t *rect);
extern void get_watch_idx_string_rect(int idx, gui_rect_t *rect);
extern bool draw_watch_time(int watch_idx, int position, int color);
extern bool draw_watch_idx(int watch_idx, int position, int color);
extern bool wipe_watch_string_area_to_high_color(scene_stopwatch_t *my_scene, int idx);
extern bool timer_draw_cur_watch_time(scene_stopwatch_t *my_scene);
extern void paint_necessary_when_rotate(scene_stopwatch_t *my_scene);

#endif   /* __STOPWATCH_UI_H__ */
