/*******************************************************************************
 *                              NOYA1100
 *                            Module: camera_dsc
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 * History:
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-04-01          1.0             build this file
*******************************************************************************/
/*!
* \file     tvin_control.h
* \brief    tvin control operation
* \author   Jerry Fu
*
* \version 1.0
* \date  2009/04/28
*******************************************************************************/
#ifndef __TVIN_CONTROL_H__
#define __TVIN_CONTROL_H__

#include "tvin_config.h"

typedef  enum
{
    TVIN_BLENDING_HEADBAR,
    TVIN_BLENDING_BUTTON,
    TVIN_BLENDING_RECTIME,
    TVIN_BLENDING_REMAIN_TIME,
    TVIN_BLENDING_PBAR,
    TVIN_BLENDING_OPTION,
    TVIN_BLENDING_DIALOG,
    TVIN_BLENDING_ADJUST,
    TVIN_BLENDING_MSGBOX,
    TVIN_BLENDING_VOLUMEBAR,
}tvin_blending_area_e;



bool get_rtc_date_time(rtc_date_time_t *date_time);
bool tvin_mmm_init(void);
void tvin_mmm_Close(void);
bool tvin_install_videoin(void);

void tvin_unstall_videoin(void);
bool is_tvin_cable_insert(void);

bool  set_record_quality(void);
bool tvin_start_preview(void);
bool tvin_stop_preview(void);

bool  set_tvin_mode(int signal_mode);
int tvin_set_record_path(tvin_save_path_e  save_path);

void tvin_alarmin_proc(int alarm_clock);
bool tvin_start_record_file(int num);
void tvin_stop_record_file(void);
void tvin_return_check_recorder(void);
bool tvin_pause_record(void);
bool tvin_resume_record(void);

bool tvin_get_recorder_status(mmm_dv_status_t *dv_status);
bool tvin_get_disk_info(mmm_disk_info_t *disk_info);

bool  tvin_ui_blending_create(void);
bool  tvin_ui_blending_delete(void);

void tvin_blending_show_spec(tvin_blending_area_e index);
void tvin_blending_hide_spec(tvin_blending_area_e index);
void tvin_record_blending_ui_show(void);
void tvin_record_blending_ui_hide(void);


bool tvin_create_dialog(unsigned short str_id, dialog_head_e head);
bool tvin_delete_dialog(void);

bool tvin_create_headbar(void);
bool tvin_delete_headbar(void);

bool tvin_option_blending_create_show( void );
bool tvin_option_blending_delete( void );

bool tvin_adjust_blending_create(void);
bool tvin_adjust_blending_delete(void);

bool tvin_msgbox_blending_create_show(void);
bool tvin_msgbox_blending_delete(void);

bool tvin_create_volumebar(void);

#endif



