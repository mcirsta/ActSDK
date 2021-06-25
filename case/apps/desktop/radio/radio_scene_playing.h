/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : radio_scene_playing.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason    2009-04-23          v1.0               build this file
 ********************************************************************************/
/*!
* \file     radio_scene_playing.h
* \brief    radio application definitions
* \author   cheason
*
* \version 1.0
* \date  2009/02/25
*******************************************************************************/


#ifndef __RADIO_SCENE_PLAYING_H__
#define __RADIO_SCENE_PLAYING_H__

#include "include_case.h"
#include "radio_app.h"
#include "radio_playing_widget_option.h"
#include "radio_utils.h"

#define MAX_PATH_NAME 1024
/*
#define SYS_PATH "/mnt/disk0/system"
#define SYS_FM_PATH "/mnt/disk0/system/FM"
#define CHANNEL_LIST_FILE "/mnt/disk0/system/FM/default.ini"
*/
const char *sys_path;
const char *sys_fm_path;
char channel_list_file[MAX_PATH_NAME];
char userchannel_path[PATH_MAX];

#define FILE_HEAD_LENTH 4
#define FILE_FREQ_LENTH 6
#define FREQ_MIN_US 87500
#define FREQ_MAX_US 108000
#define FREQ_MIN_JP 76000
#define FREQ_MAX_JP 90000



/* 播放界面 状态*/
typedef enum
{
    STATE_QUIT = -1,
    STATE_PLAYING,
    STATE_OPTION,
    STATE_VOLUMEBAR,

}scene_state_e;

/* 搜索状态  */
typedef enum
{
    NORMAL_PLAY = 0,        /* 正常播放 */
    HALF_AUTO_SEARCH_ADD,   /* 前向半自动搜索(遇到台就停) */
    HALF_AUTO_SEARCH_MINUS, /* 后向半自动搜索(遇到台就停) */
    AUTO_SEARCH,            /* 全自动搜索，搜索当前波段所有电台（最多保存20个）*/

}search_state_e;

/*engine need info for auto seeking*/
typedef struct
{
    unsigned int min_freq;
    unsigned int max_freq;
    unsigned int step;
    search_state_e mode;
}en_seek_info_t;

/*result of auto seek*/
typedef struct{
    unsigned int channel_num;
    unsigned int channel_freq[MAX_CHANNEL_NUM];
}eg_seek_info_t;

/* 播放界面ui资源 */
typedef struct
{
    resgroup_resource_t     *resgroup_playing;  /* 播放界面资源组 */

    picture_resource_t      *pic_foreground; /*前景图*/
    picture_resource_t      *pic_stereo;        /* 立体声图标 */
    picture_resource_t      *pic_num_hundreds;  /* 当前频率百位数图标 */
    picture_resource_t      *pic_num_tens;      /* 当前频率十位数图标 */
    picture_resource_t      *pic_num_units;     /* 当前频率个位数图标 */
    picture_resource_t      *pic_num_dot;       /* 当前频率小数点图标 */
    picture_resource_t      *pic_num_tenth;     /* 当前频率十分位数图标 */
    string_resource_t       *str_num_centi;     /* 当前频率百十分位数 */
    string_resource_t       *str_mhz;           /* 频率单位(MHz) */
    picregion_resource_t    *picreg_signal;     /* 信号强度图标(共5个) */

    string_resource_t       *str_country;       /* 当前频道所属国家 */
    string_resource_t       *str_channel;       /* 当前频道名称 */

    picture_resource_t      *pic_freq_bg;       /* 频率指示牌背景图 */
    string_resource_t       *str_freq_start;    /* 频率指示的最小频率 */
    string_resource_t       *str_freq_end;      /* 频率指示的最大频率 */
    picture_resource_t      *pic_freq_notation[181]; /* 当前频率指示图标 */

}res_playing_t;

typedef struct
{
    unsigned int  channel_num;      /* user channel num [max = 20]*/
    unsigned int  channel_freq[MAX_CHANNEL_NUM];    /* user channel freq */
    char channel_name[MAX_CHANNEL_NUM][PATH_MAX];   /* user channel name */

} channel_info_t;

channel_info_t channel_info;

/*band*/
typedef enum
{
    BAND_CHINA = 0,
    BAND_USA,
    BAND_JAPAN,
    BAND_EUROPE,
}option_band_item_e;


/* scene playing data */
typedef struct
{
    /* style resource */
    scene_t                 *scene;

    /* common scene */
    scene_t *common_scene;

    /* default ini文件句柄*/
    int fd; 

    /* fm驱动句柄*/
    int fm_fd;
    
    /*本地存储器状态*/
    bool local_disk_status;
    /*fsel句柄*/
    FSEL_HANDLE fp;

    int fontdes;

    picture_resource_t      *pic_bg;        /* 场景背景图 */
    gui_bitmap_info_t  * bg_bitmap_info;
    picture_resource_t      *pic_select;    /*option选中图标*/
    picture_resource_t      *pic_logo;      /* headbar上显示图标 */
    string_resource_t        *str_logo;     /* headbar上显示字符串*/
    picregion_resource_t    *picreg_num;    /* 数字图片组 */
    string_resource_t        *str_stoprec;  /* adjust上显示字符串*/
    string_resource_t        *str_fmterr;   /*文件格式不支持时dialog上显示字符串*/
    string_resource_t        *str_earinsert; /*耳机未插入时dialog上显示字符串*/
    string_resource_t        *str_linein; 
    string_resource_t        *str_nodisk;
    string_resource_t        *str_diskins;

    res_playing_t           res_playing;

    /* window resource */
    int                     win_id;

    /*fm录音信息*/
    int rec_quality;
    int rec_gain;
    int rec_path;

    /* 即时信息 */
    bool                    use_custum_file;    /* 是否使用用户文件，如“是”，则波段id无效 */
    char                    cur_file_path[PATH_MAX]; /* 用户文件路径 */
    int                     timer_id;           /* 定时器id */
    bool                    mute;               /* 是否静音 */
    int                     cur_intensity;      /* 即时信号强度 */
    bool                    cur_stereo;         /* 当前频道是否为立体声 */
    search_state_e          searching_flag;     /* 搜台标志，0-正常播放，1-半自动（遇到有台就停止）, 2-全自动 */
  
    bool                    auto_search_start;  /*标志已开始搜台，0未开始，1已开始搜台*/


    bool load_config_flag;/*读配置项后该标志置true，否则false*/
    bool parse_default_file_flag;/*正确解析default.ini文件，该标志置true，未解析或者未正确解析置false*/

    bool is_userchannel_mode;
    /* 需要保存到config里面的信息 */
    bool                    use_preset;             /* 是否使用预置频道 */
    int                     cur_preset_channel_id;  /* 预置频道id */
    int                     cur_band;               /* 当前波段  */
    unsigned int            cur_frequency;          /* 当前频率 */
    int                     cur_threthod;           /* 当前门槛值 */
   
    channel_info_t channel_list_info;

    /* 控件 */
    headbar_t               *headbar;
    volumebar_t             *volumebar;
    menuview_t              *option_menu;
    adjust_t                  *adjust;
    dialog_t                  *dialog;
    dialog_t                  *dialog_linein;
    msgbox_t               *msgbox;
    /*是否需要检测LINE_IN*/
    int line_in_check;

    app_scene_t             next_scene;

}radio_scene_playing_t;

extern radio_scene_playing_t* g_scene_playing;

/* scene entry */
int radio_scene_playing( fusion_enter_e fenter );
bool parse_ini_file(channel_info_t *channel_info, char *file_buf, unsigned int file_buf_lenth);
bool check_freq(option_band_item_e band, int freq);
bool save_default_file(radio_scene_playing_t *playing);


#endif  /* __RADIO_SCENE_PLAYING_H__ */

