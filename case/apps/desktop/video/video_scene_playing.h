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
 * \file     video_scene_playing.h
 * \brief    
 * \author   shaofeng
 *               
 * \version 1.0
 * \date  2009/04/01
*******************************************************************************/

#ifndef __VIDEO_SCENE_PLAYING_H__
#define __VIDEO_SCENE_PLAYING_H__

#include "include_case.h"
#include "include_psp.h"

 


typedef enum
{
    VIDEO_AUTO_PLAY = 0,
    VIDEO_FORCE_PLAY_NEXT,
    VIDEO_FORCE_PLAY_PREV,
}video_play_switch_mode_e;



typedef enum
{
    FFB_KEY_STATUS_OTHER = 0,
    FFB_KEY_STATUS_FF,
    FFB_KEY_STATUS_FB,
}ffb_key_status_e;


typedef enum
{
    VIDEO_PLAY = 0,
    VIDEO_PLAY_HL,
    VIDEO_PAUSE,
    VIDEO_PAUSE_HL,
    VIDEO_STOP,
    VIDEO_STOP_HL,
    VIDEO_PREV,
    VIDEO_PREV_HL,
    VIDEO_NEXT,
    VIDEO_NEXT_HL,
    VIDEO_FB,
    VIDEO_FB_HL,
    VIDEO_FF,
    VIDEO_FF_HL,
}video_status_frame_e;


/*!
*\brief
    保存的显示状态 结构
*/
typedef struct
{
    char pbar_bg;
    char pbar;
    
    char status_bg;
    char status;
    
    char time_left;
    char time_right;
}video_playing_saved_show_t;


/*!
*\brief
    显示状态 结构
*/
typedef struct
{
    char pbar_bg;
    char pbar;
    
    char status_bg;
    char status;
    char status_frame;
    
    char time_left;
    char time_right;
}video_playing_show_t;



typedef struct
{
    app_scene_t app_scene;
    /* 进入scene, 按键的有效状态. 只有收到down后才有效 */
    char is_key_valid;
    
    /* 从中间件获取的status */
    mmm_video_status_t status;
    /* 记录的按键状态, 用于区分从play到达stop, 还是从ffb到达stop, 用以分别做处理. */
    ffb_key_status_e ffb_key_status;
    /* ffb之前的player_status, 仅为playing or pause状态, 用于取消ffb后恢复到此状态 */
    mmm_player_status_t before_ffb_player_status;
    
    int timer_id;   //timer id for play timing
    unsigned int cur_time;   //current play time
    unsigned int total_time; //total time
    
    /*! 用于控制流控制显示状态, 需要用户改变, 以完成显示 */
    video_playing_show_t show;
    /*! 用于保存当前显示状态, 用户不需要关心 */
    video_playing_saved_show_t saved_show;
    
    /*! resource of show */
    picture_resource_t      *pic_pbar_bg;
    picture_resource_t      *pic_pbar;
    picture_resource_t      *pic_pbar_hl;
    resgroup_resource_t     *resgroup_pbar_highlight_rect;
    /*! pbar的长度和左端起始坐标, 都是相对于pbar背景的相对坐标 */
    int pbar_length;
    int pbar_xstart;
    
    picture_resource_t      *pic_status_bg;
    picregion_resource_t    *reg_status;

    picture_resource_t      *pic_select_status;
    
    string_resource_t       *str_time_left;
    string_resource_t       *str_time_right;
}video_scene_playing_t;



/************************ extern function, 函数声明 ***************************/
extern void video_scene_playing_shaked( video_scene_playing_t *scene_playing );

extern bool video_scene_playing_load_style( video_scene_playing_t *scene_playing );

/* scene entry */
extern scene_type_e video_scene_playing( video_scene_playing_t *scene_playing );
extern bool proc_displaymode_update( void );
extern void video_scene_playing_delete_file( void );



#endif  /* __VIDEO_SCENE_PLAYING_H__ */

