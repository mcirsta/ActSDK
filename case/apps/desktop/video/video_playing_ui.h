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



/************************ MACRO defination, 宏定义 ***************************/
/* 范围: -1-0xFFFF(565颜色值). -1:不使用colorkey, 其他颜色值则透明处理, 如:0则与黑色进行透明处理 */
#define VIDEO_DEFAULT_BLENDING_COLOR_KEY    (0)

/* 范围: 0-255. 越大越透明, 前景越淡, video播放画面越清晰, 但目前只有5级 */
/* 0-51; 52-102; 103-153; 154-204; 205-255 */
#define VIDEO_DEFAULT_BLENDING_ALPHA    (52)



/************************ extern function, 函数声明 ***************************/
extern bool video_playing_time_to_str( unsigned int time, char *str );
extern bool video_playing_time_to_pbar_coordinate( unsigned int cur_time, unsigned int total_time, int *x_coordinate );

extern void video_playing_draw_pbar_bg(video_scene_playing_t *scene_playing, char show);
extern void video_playing_draw_pbar(video_scene_playing_t *scene_playing, char show, int x_coordinate);
extern void video_playing_draw_status_bg(video_scene_playing_t *scene_playing, char show);
extern void video_playing_draw_status(video_scene_playing_t *scene_playing, char show, video_status_frame_e frame);
extern void video_playing_draw_time_left(video_scene_playing_t *scene_playing, char show, unsigned int time);
extern void video_playing_draw_time_right(video_scene_playing_t *scene_playing, char show, unsigned int time);



#endif  /* __VIDEO_PLAYING_UI_H__ */

