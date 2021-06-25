/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_control.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_control.h
* \brief    music control operation
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#ifndef   __MUSIC_CONTROL_H
#define   __MUSIC_CONTROL_H

#include "music.h"


/************************************************************************
* 
*                      添加宏定义
*           
*************************************************************************/
bool music_open_engine( bool waiting );
bool music_close_engine( void );

bool music_get_engine_info(mengine_t *engine);
bool music_get_filepath(char *filepath, int max_len);

/************************************************************************
* Function:         music_set_eq
* Description:      设置EQ 
* para1：           eq 从该地址开始存放要设置EQ的信息
* Return：          bool   true:  成功   false: 失败
*************************************************************************/
bool music_set_eq(mengine_eq_t *eq_val);

bool music_set_playmode(play_mode_e playmode);
bool music_set_displaymode(display_mode_e displaymode);
bool music_set_fade(bool fade);

/************************************************************************
* Function:         music_set_playspeed
* Description:      调整播放速度，值范围[-8，8] 
* para1：           play_speed  播放速度
* Return：          bool   true:  成功   false: 失败
*************************************************************************/
bool music_set_playspeed(int playspeed);

bool music_set_ab_auto(bool auto_play);

/************************************************************************
* Function:         music_set_ab_count
* Description:      设置AB循环次数，值范围[0，+∞] 
* para1：           count AB循环播放次数
* Return：          bool   true:  成功   false: 失败
*************************************************************************/
bool music_set_ab_count(int count);

/************************************************************************
* Function:         music_set_ab_time
* Description:      设置从B点回到A点的等待时间
* para1：           wait_time 从B点返回到A点的等待时间
* Return：          bool   true:  成功   false: 失败
*************************************************************************/
bool music_set_ab_time(int time);

bool music_start_ab(void);
bool music_clear_ab(void);
bool music_set_ab_next(void);
bool music_set_ab_prev(void);

/************************************************************************
* Function:      music_play
* Description:   开始播放音乐
* Return：       bool   true: 成功   false:失败
*************************************************************************/
bool music_play(void);

bool music_stop(void);
bool music_pause(void);
bool music_resume(void);

/************************************************************************
* Function:         music_seek_play
* Description:      seek播放,传递时间以ms为单位 
* para1：           seek_position  从该时间开始播放
* Return：          bool   true:  成功   false: 失败
*************************************************************************/
bool music_seek(int ms);

/************************************************************************
* Function:         music_fast_forward
* Description:      快进，传递参数为快进
*                   的速度等级，范围为[0，4]
*
* para1：           none
* Return：          bool   true:  成功   false: 失败
*************************************************************************/
bool music_fast_forward(void);

/************************************************************************
* Function:         music_fast_backward
* Description:      快退，传递参数为快进
*                   的速度等级，范围为[0，4]
*
* para1：           none
* Return：          bool   true:  成功   false: 失败
*************************************************************************/
bool music_fast_backward(void);

bool music_cancel_ffb(void);

/************************************************************************
* Function:         music_set_ffb_step
* Description:      设置快近快退速度等级，范围为[0，4] 
* para1：           step   快进快退的速度等级
* Return：          bool   true:  成功   false: 失败
*************************************************************************/
bool music_set_ffb_step(int speed);

/************************************************************************
* Function:          music_set_ffb_time
* Description:       设置在快近快退过程中，
*                    每一快进或快退之间音乐
*                    播放的时间长度
*                          
* para1：           time  播放的时间长度
* Return：          bool   true:  成功   false: 失败
*************************************************************************/
bool music_set_ffb_time(int time);

/************************************************************************
* Function:           music_set_filepath
* Description:        选择一个等待播放的音乐文件
* para1：             pFile 指向要播放的文件的路径
* Return：            true or false
*************************************************************************/ 
bool music_set_filepath(char *filepath);
bool music_set_fileindex(int fileindex);
bool music_play_next(void);
bool music_play_prev(void);

bool  music_make_bookmark(eg_audio_tag_t *audio_tag);

bool music_set_last_file( void );

bool music_delete_file(char * fullpath);


/* album image decode */
bool album_open_engine( void );
bool album_close_engine( void );
bool album_get_status( mmm_image_status_t *status );
bool album_get_image_info( mmm_img_info_t *img_info );
bool album_check_file( plist_detail_info_t *id3_info );
bool album_start_decode( plist_detail_info_t *id3_info, gui_bitmap_info_t *bmp, void (*callback)() );
bool album_abort_decode( void );

/* music pthread operation */
bool music_create_thread( void *proc, void *param );

/* keymap control */
bool music_keymap_control( key_val_e key_val, bool is_map );

/* fusion control */
void scene_fusion_set_first_frame( scene_t *scene );
void scene_fusion_set_last_frame( scene_t *scene );
void option_fusion_set_first_frame( menuview_t *mv );
void option_fusion_set_last_frame( menuview_t *mv );

bool music_get_nowplaying( void );
bool music_set_nowplaying( bool nowplaying );

#endif

