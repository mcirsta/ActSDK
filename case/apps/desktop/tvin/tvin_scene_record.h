/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : tvin_scene_record.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *      Jerry Fu    2009-4-28             v1.0               build this file
 ********************************************************************************/
/*!
* \file     tvin_scene_record.h
* \brief    tvin  record scene
* \author   Jerry Fu
*
* \version 1.0
* \date  2009/04/28
*******************************************************************************/
#ifndef      __SCENE_TVIN_RECORD_H__
#define      __SCENE_TVIN_RECORD_H__

#include "tvin_config.h"
#include "tvin_record_option.h"
#include "tvin_record_widget_option.h"
#include "tvin_record_option_singletime.h"
#include "tvin_record_option_repeattime.h"
#include "tvin_record_option_savepath.h"
#include "tvin_record_option_repeattime_selweekday.h"
#include "tvin_record_option_quality.h"
#include "tvin_control.h"
#include "tvin_record_alarm.h"

typedef  enum
{
    KEY_PRESSED,
    KEY_RELEASED,    
}tvin_key_status_e;


/*!
 *  \par 录像状态类型
 *  \li TVIN_RECORD_STOP : 停止状态
 *  \li TVIN_RECORD_STOPHL : 由停止切换到播放时按钮亮一下的过渡状态
 *  \li TVIN_RECORD_PLAY : 播放状态
 *  \li TVIN_RECORD_PLAYHL : 由播放切换到暂停时按钮亮一下的过渡状态
 *  \li TVIN_RECORD_PAUSE : 暂停状态
 *  \li TVIN_RECORD_PAUSEHL : 由暂停切换到播放时按钮亮一下的过渡状态
 */
typedef  enum
{    
    TVIN_RECORD_STOP,
    TVIN_RECORD_STOPHL,
    TVIN_RECORD_PLAY,
    TVIN_RECORD_PLAYHL,
    TVIN_RECORD_PAUSE,
    TVIN_RECORD_PAUSEHL,
}tvin_record_status_e;


typedef  struct
{
    int  begin_pos;
    int  end_pos;
    int  cur_pos;
    int  step;
}record_progress_t;


typedef  struct
{
    tvin_key_status_e  tvin_key_ok;
    tvin_key_status_e  tvin_key_return;
    tvin_key_status_e  tvin_key_menu;
    tvin_key_status_e  tvin_key_up;
    tvin_key_status_e  tvin_key_down;
}record_key_sta_t;


typedef struct
{
    int blending_handle[MAX_BLENDING_AREA];
    
    mmm_enc_blending_create_t hbar_region;
    bool  show_blending_hbar;
    
    int  button_canvas_id;
    mmm_enc_blending_create_t button_region;
    bool show_blending_button;

    int recordtime_canvas_id;
    mmm_enc_blending_create_t recordtime_region;
    bool show_blending_rectime;

    int remaintime_canvas_id;
    mmm_enc_blending_create_t remain_time_region;
    bool show_blending_remaintime;

    int pbar_canvas_id;
    mmm_enc_blending_create_t pbar_region;
    bool show_blending_pbar;

    mmm_enc_blending_create_t option_region;
    bool show_blending_option;

    mmm_enc_blending_create_t dialog_region;
    bool show_blending_dialog;

    mmm_enc_blending_create_t adjust_region;
    bool show_blending_adjust;

    mmm_enc_blending_create_t msgbox_region;
    bool show_blending_msgbox;

    mmm_enc_blending_create_t volumebar_region;
    bool show_blending_volumebar;
    
}tvin_blending_t;


typedef  struct
{
    int hide_ui_time;   
}tvin_time_relate_para_t;

typedef struct
{
    bool first_draw;
    bool tvin_install;
    int win_id;
    int hdc;    
    /*! scene initial flag */
    int inited;
    int timer_id;
    tvin_time_relate_para_t  time_relate;
    /* style resource */
    scene_t *scene;
    scene_t *common_scene;
    /*! tvin 的配置参数保存在该结构体中*/
    tvin_config_t  tvin_config;
    /*! tvin record 当前的状态 */
    tvin_record_status_e  tvin_record_sta;
    /*! 记录在tvin record 场景下按键的状态*/
    record_key_sta_t  tvin_key_info;

    /*! total record time */
    unsigned int total_time;
    unsigned int cur_record_time;
    unsigned int record_remain_time;
    /*! 设置的定时录像时长*/
    unsigned int alarm_len_time;

    msgbox_t *msgbox;
    /*! record界面下的menuview指针*/                                         
    menuview_t *pRecMView;
    
    /*! 在menu上弹出的adjust控件的结构体指针*/       
    adjust_t *adjust;
    dialog_t *dialog;
    volumebar_t *volume_bar;
                       
    /*! 字符串资源组的指针 */
    resgroup_resource_t     *group;

    /*! 指向headbar资源组的指针 */
    resgroup_resource_t     *headbar_resgroup;

    /*! 指示进度条高亮范围的资源组 */
    resgroup_resource_t  *pbar_hl_position_resgroup;

    /*! tvin headbar 上的图标 */
    picture_resource_t  *pic_hb_icon;

    /*! tvin headbar 上的title */
    string_resource_t *str_hb_title;

    /*! 指向录像操作按钮的图片资源的指针 */                                                     
    picture_resource_t  *pic_record_status_bg;

    /*! 指向录像状态图片区的指针 */
    picregion_resource_t    *picregion_record_status;
    
    /*! 指向进度条图片的指针 */    
    picture_resource_t  *pic_pbar_bg;

    /*! 指向进度条高亮图片的指针 */
    picture_resource_t  *pic_pbar_hl;

    /*! 指向擦掉高亮的图片的指针 */
    picture_resource_t  *pic_pbar;

    /*! 指向录制时间的字符串资源的指针  */
    string_resource_t  *str_record_time;

    /*! 指向剩余录制时间的字符串资源的指针  */
    string_resource_t  *str_remain_time; 
    
    /*! 指向磁盘空间满的字符串资源的指针 */
    string_resource_t  *str_disk_full;

    /*! 退出tvin record 场景以后要进入的场景 */
    scene_tvin_ret_e  next_scene;
          
    bool pbar_bg_is_show;
    bool record_status_is_show;
    bool record_statusbg_is_show;
    bool headbar_is_show;
    bool record_time_is_show;
    bool remain_time_is_show;    
   
    headbar_t *headbar;    
    tvin_blending_t blending;
    int fontdes;
    /*! 定时时间到，关机状态下唤醒标志*/
    int wake_up;
    int clock_number;

    gui_bitmap_info_t  *status_bitmap[6];
    gui_bitmap_info_t  *stabg_bitmap;
    gui_bitmap_info_t  *pbar_hl_bitmap;

    unsigned int saved_x;
    unsigned int xstart_pos;
    
}tvin_scene_record_t;


/* scene entry */

tvin_scene_record_t *fetch_tvin_record(void);
void tvin_scene_record_paint(tvin_scene_record_t *scene_record);
bool tvin_blending_deinit(void);
void tvin_scene_record_paint(tvin_scene_record_t *scene_record);

#endif

