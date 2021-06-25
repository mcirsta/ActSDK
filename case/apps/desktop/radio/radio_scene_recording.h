/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : radio_scene_recording.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *    wangsheng    2009-04-23          v1.0               build this file
 ********************************************************************************/
/*!
* \file     radio_scene_recording.h
* \brief    radio录音场景定义
* \author   wangsheng
*
* \version 1.0
* \date  2009/02/25
*******************************************************************************/
#ifndef __RADIO_SCENE_RECORDING_H__
#define __RADIO_SCENE_RECORDING_H__

#include "include_case.h"


#include "radio_app.h"

#define AE_PATH  "/mnt/disk0/"
#define SCENE_RECORDING_NEXT 0
#define RESERVE_TIME 2

typedef enum
{
    RECORDING_FRAME = 0,
    PAUSE_FRAME,
}recorder_word_status_frame_e;

typedef enum
{
    not_need_paint,
    need_paint,
}fm_recorder_res_status_e;

typedef enum
{
    recording,
    pause,
}fm_recorder_status_e;

typedef struct scene_recording_s
{
    int win_id;
    int hdc;

    int vol_energe_timer_id;
    int pb_curtime_timer_id;
    int disk_info_timer_id;

    void *ae_handle;
    void *fptr;
    char file_name_buf[61];

    int total_time_info;

    int fontdes;

    /* recording scene */
    scene_t * scene;
    
    /* common scene */
    scene_t *common_scene;

    int quality;
    int gain;
    int path;
    
    fm_recorder_status_e recorder_status;

    /* resource status */
    fm_recorder_res_status_e bg;
    fm_recorder_res_status_e word_status;
    fm_recorder_res_status_e filename;
    fm_recorder_res_status_e pbbg;
    fm_recorder_res_status_e totaltime;
    fm_recorder_res_status_e color_status;
    fm_recorder_res_status_e tape;
    
    /* resource for painting */
    picture_resource_t *fm_rec_bg;
    gui_bitmap_info_t  * bg_bitmap_info;
    picregion_resource_t *fm_rec_word_status;
    string_resource_t *fm_rec_filename;
    picture_resource_t *fm_rec_pbbg;
    picture_resource_t *fm_rec_pb;
    string_resource_t *fm_rec_curtime;
    string_resource_t *fm_rec_totaltime;
    picregion_resource_t *fm_rec_color_status;
    picture_resource_t *fm_rec_tape;
    picture_resource_t *fm_rec_vol_bg;
    picture_resource_t *fm_rec_vol1;
    picture_resource_t *fm_rec_vol2;
    picture_resource_t *fm_rec_vol3;
    picture_resource_t *fm_rec_vol4;
    picture_resource_t *fm_rec_vol5;

    resgroup_resource_t *fm_rec_hbar;
    picture_resource_t *fm_rec_hbar_pic;
    string_resource_t *fm_rec_hbar_str;

    resgroup_resource_t *fm_rec_dialog;
    string_resource_t *fm_rec_dialog_stoprec;
    string_resource_t *fm_rec_dialog_diskinfo;
    string_resource_t *rec_dialog_lowbat;/*低电*/
    string_resource_t *rec_dialog_savefile;
    string_resource_t *rec_dialog_nodisk;
    string_resource_t *rec_dialog_diskins;
    /*! record mmm */
    mmm_audio_param_t   param;
    mmm_disk_info_t disk_info;
    mmm_record_status_t recording_status;

    /*! common ui */
    volumebar_t *vbar;
    headbar_t *headbar;
    dialog_t *dialog;
    adjust_t *adjust;
    msgbox_t  *msgbox;

    app_scene_t             next_scene;
    
}scene_recording_t;

extern scene_recording_t* g_scene_recording;
extern radio_app_t app_data;

int radio_scene_recording( fusion_enter_e fenter );
#endif

