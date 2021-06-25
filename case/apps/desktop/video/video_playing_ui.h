/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-04-01          v1.0               build this file 
********************************************************************************/
/*!
 * \file     video_playing_ui.h
 * \brief    
 * \author   shaofeng
 *               
 * \version 1.0
 * \date  2009/04/01
*******************************************************************************/

#ifndef __VIDEO_PLAYING_UI_H__
#define __VIDEO_PLAYING_UI_H__



/************************ MACRO defination, �궨�� ***************************/
/* ��Χ: -1-0xFFFF(565��ɫֵ). -1:��ʹ��colorkey, ������ɫֵ��͸������, ��:0�����ɫ����͸������ */
#define VIDEO_DEFAULT_BLENDING_COLOR_KEY    (0)

/* ��Χ: 0-255. Խ��Խ͸��, ǰ��Խ��, video���Ż���Խ����, ��Ŀǰֻ��5�� */
/* 0-51; 52-102; 103-153; 154-204; 205-255 */
#define VIDEO_DEFAULT_BLENDING_ALPHA    (52)



/************************ extern function, �������� ***************************/
extern bool video_playing_time_to_str( unsigned int time, char *str );
extern bool video_playing_time_to_pbar_coordinate( unsigned int cur_time, unsigned int total_time, int *x_coordinate );

extern void video_playing_draw_pbar_bg(video_scene_playing_t *scene_playing, char show);
extern void video_playing_draw_pbar(video_scene_playing_t *scene_playing, char show, int x_coordinate);
extern void video_playing_draw_status_bg(video_scene_playing_t *scene_playing, char show);
extern void video_playing_draw_status(video_scene_playing_t *scene_playing, char show, video_status_frame_e frame);
extern void video_playing_draw_time_left(video_scene_playing_t *scene_playing, char show, unsigned int time);
extern void video_playing_draw_time_right(video_scene_playing_t *scene_playing, char show, unsigned int time);



#endif  /* __VIDEO_PLAYING_UI_H__ */

