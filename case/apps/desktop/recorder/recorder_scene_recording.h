#ifndef __SCENE_RECORDING_H__
#define __SCENE_RECORDING_H__

#include "include_case.h"


#include "recorder_recording_widget_option.h"

#define AE_PATH  "/mnt/disk0/"
#define FILE_NAME "test"
#define SCENE_RECORDING_NEXT 0
#define RESERVE_TIME 2
#define MIC      0
#define LINE_IN  1

/*存储路径枚举*/
typedef enum
{
    INTERNAL_MEMORY = 0,
    CARD_MEMORY,
    EXTERNAL_MEMORY,
    INVALID_MEMORY,
}recorder_save_path_e;

typedef enum
{
    STANDBY_FRAME = 0,
    RECORDING_FRAME,
    PAUSE_FRAME,
}recorder_word_status_frame_e;

typedef enum
{
    GRAY_FRAME = 0,
    GREEN_FRAME,
    RED_FRAME,
}recorder_color_status_frame_e;

typedef enum
{
    NOT_NEED_PAINT,
    NEED_PAINT,
}recorder_res_status_e;

/*录音状态枚举*/
typedef enum
{
    STANDBY,
    RECORDING,
    PAUSE,
}recorder_status_e;

typedef struct scene_recording_s
{
    int win_id;
    int exit_type;
    int hdc;

    int vol_energe_timer_id;
    int pb_curtime_timer_id;
    int disk_info_timer_id;

    void *ae_handle;
    void *fptr;
    char file_name_buf[61];

    unsigned int total_time_info;
    int ok_down_flag;

    int fontdes;

    /*当前主盘*/
    hotplugin_dev_type_e main_disk;

    /*录音支持格式*/
    char *support_format;
    /*LINE_IN支持*/
    int line_in_support;

    /*录音相关参数*/
    char *format;
    int quality;
    int mode;
    int gain;
    recorder_save_path_e path;

    /* recording scene */
    scene_t * scene;

    /* common scene */
    scene_t *common_scene;

    recorder_status_e recorder_status;

    /* resource status */
    recorder_res_status_e bg;
    recorder_res_status_e word_status;
    recorder_res_status_e filename;
    recorder_res_status_e pbbg;
    recorder_res_status_e totaltime;
    recorder_res_status_e color_status;
    recorder_res_status_e tape;

    /* menu key flag in pause status */

    /* resource for painting */
    picture_resource_t *rec_bg;
    gui_bitmap_info_t  * bg_bitmap_info;
    picregion_resource_t *rec_word_status;
    string_resource_t *rec_filename;
    picture_resource_t *rec_pbbg;
    picture_resource_t *rec_pb;
    string_resource_t *rec_curtime;
    string_resource_t *rec_totaltime;
    picregion_resource_t *rec_color_status;
    picture_resource_t *rec_tape;
    picture_resource_t *rec_vol_bg;
    picture_resource_t *rec_vol1;
    picture_resource_t *rec_vol2;
    picture_resource_t *rec_vol3;
    picture_resource_t *rec_vol4;
    picture_resource_t *rec_vol5;

    picture_resource_t *rec_select;

    resgroup_resource_t *rec_opt_main;
    string_resource_t *rec_fmt;
    string_resource_t *rec_quality;
    string_resource_t *rec_gain;
    string_resource_t *rec_mode;
    string_resource_t *rec_savepath;

    resgroup_resource_t *rec_opt_fmt;
    string_resource_t *rec_wav;
    string_resource_t *rec_mp3;
    string_resource_t *rec_wma;

    resgroup_resource_t *rec_opt_quality;
    string_resource_t *rec_low;
    string_resource_t *rec_medium;
    string_resource_t *rec_high;

    resgroup_resource_t *rec_opt_mode;
    string_resource_t *rec_mic;
    string_resource_t *rec_linein;

    resgroup_resource_t *rec_opt_savepath;
    string_resource_t *rec_internal;
    string_resource_t *rec_card;
    string_resource_t *rec_external;

    resgroup_resource_t *rec_hbar;
    picture_resource_t *rec_hbar_pic;
    string_resource_t *rec_hbar_str;

    resgroup_resource_t *rec_dialog;
    string_resource_t *rec_dialog_stoprec;
    string_resource_t *rec_dialog_diskinfo;
    string_resource_t *rec_dialog_savefile;
    string_resource_t *rec_dialog_lowbat;
    string_resource_t *rec_dialog_diskins;
    string_resource_t *rec_dialog_nodisk;

    resgroup_resource_t *rec_adjust;
    string_resource_t *rec_adjust_str;

    /*! record mmm */
    mmm_audio_param_t   param;
    mmm_disk_info_t disk_info;
    mmm_record_status_t recording_status;

    /*! common ui */
    volumebar_t *vbar;
    headbar_t *headbar;
    menuview_t *option;
    dialog_t *dialog;
    dialog_t *dialog_query;
    adjust_t *adjust;
    msgbox_t *msgbox;
}scene_recording_t;

extern scene_recording_t *g_scene_recording;

/* scene type define */
typedef enum
{
    SCENE_EXIT = 0, /*exit state*/
    SCENE_RECORDING,
} app_scene_t;

typedef struct app_s
{
    /* system resource */
    void *applib_lib;
    void *commonui_lib;
    void *fusion_lib;
    void *gui_lib;
    void *sty_lib;
    void *config_lib;

    /* scene resource */
    scene_recording_t scene_record_data;

    /* other data */
    app_scene_t next_scene; /*场景退出后进入的下一场景*/

    /*style */
    style_t* recorder_style;
    style_t* common_style;

} app_t;

extern app_t app_data;

int recorder_scene_recording( scene_recording_t* data);
void mount_dsp_enc(void);
void mount_dsp_dec(void);
int  set_path(void);
bool set_param(void);
bool set_format(void);
int set_mmm(void);
bool start_monitor(int mode);
bool stop_monitor(int mode);
#endif


