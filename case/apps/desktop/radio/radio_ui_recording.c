/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : radio_ui_recording.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      cheason    2009-04-23          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     radio_ui_recording.c
 * \brief    routine of radio scene recording ui
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/04/23
 *******************************************************************************/
#include "radio_ui_recording.h"

/*
 * 绘制背景
 * 参数：
 *   param: 场景结构体指针
 */
void fm_recorder_draw_bg( scene_recording_t *param )
{
    if(param->bg == need_paint)
    {
        sty_draw_bitmap(param->hdc, param->bg_bitmap_info, 0, 0);
    }
    else
        print_dbg("background status : %d\n", param->bg);
}
/*
 * 绘制录音状态
 * 参数：
 *   param: 场景结构体指针
 */
void fm_recorder_draw_word_status( scene_recording_t *param )
{
    picregion_frame_t picregion_frame;
    sty_rect_t rect;

    if(param->word_status == need_paint)
    {
        rect.x = param->fm_rec_word_status->x;
        rect.y = param->fm_rec_word_status->y;
        rect.width = param->fm_rec_word_status->width;
        rect.height = param->fm_rec_word_status->height;

        /*! draw background region */
        sty_draw_sub_bitmap(param->hdc, param->bg_bitmap_info, 
                                                    param->fm_rec_word_status->x, param->fm_rec_word_status->y, 
                                                    &rect);
        
        if(param->recorder_status == recording)
        {
            get_picregion_frame(param->fm_rec_word_status, RECORDING_FRAME, &picregion_frame);
        }
        if(param->recorder_status == pause)
        {
            get_picregion_frame(param->fm_rec_word_status, PAUSE_FRAME, &picregion_frame);
        }

        /*! draw word */
        sty_draw_picture_id(param->hdc, picregion_frame.pic_id, 
                                            param->fm_rec_word_status->x, param->fm_rec_word_status->y);
    }
    else
    {
        print_dbg("word status : %d\n", param->word_status);
    }
}

/*
 * 绘制文件名
 * 参数：
 *   param: 场景结构体指针
 */
void fm_recorder_draw_filename( scene_recording_t *param )
{
    if(param->filename == need_paint)
    {
        param->file_name_buf[14] = '.';
        param->file_name_buf[15] = 'm';
        param->file_name_buf[16] = 'p';
        param->file_name_buf[17] = '3';
        param->file_name_buf[18] = '\0';

        sty_draw_string_res_ext( param->hdc, param->fm_rec_filename, param->file_name_buf); 
    }
    else
    {
        print_dbg("filename status : %d \n", param->filename);
    }
}

/*
 * 绘制进度背景
 * 参数：
 *   param: 场景结构体指针
 */
void fm_recorder_draw_pbbg( scene_recording_t *param )
{
    if(param->pbbg == need_paint)
    {
        sty_draw_picture_res(param->hdc, param->fm_rec_pbbg);
    }
    else
    {
        print_dbg("process bar background status : %d\n", param->pbbg);
    }
}

/*
 * 绘制录音时间
 * 参数：
 *   param: 场景结构体指针
 */
void fm_recorder_draw_pbhl_curtime( void *data )
{
    int hour = 0;
    int min = 0;
    int second = 0;
    char buf[20];
    int result;
    int width;
    sty_rect_t rect;

    scene_recording_t *param = (scene_recording_t *)data;

    if( param->recorder_status == recording )
    {
        result = mmm_ae_cmd(param->ae_handle, MMM_GET_RECORDER_STATUS, (unsigned int)&param->recording_status);
        if(result < 0)
        {
            print_err("get recording status failed\n");
        }

        /*! draw rec time */
        hour = (int)(param->recording_status.time / 3600);
        min = (int)((param->recording_status.time % 3600) / 60);
        second = (int)((param->recording_status.time % 3600) % 60);

        rect.x = param->fm_rec_curtime->x;
        rect.y = param->fm_rec_curtime->y;
        rect.width = param->fm_rec_curtime->width;
        rect.height = param->fm_rec_curtime->height;
        
        sty_draw_sub_bitmap(param->hdc, param->bg_bitmap_info, 
                                                    param->fm_rec_curtime->x, param->fm_rec_curtime->y, 
                                                    &rect);
        sprintf(buf, "%02d:%02d:%02d", hour, min, second);
        sty_draw_string_res_ext( param->hdc, param->fm_rec_curtime, buf );

        /*! draw process bar (highlight) */
        width = (int)(param->fm_rec_pb->width) * (int)(param->recording_status.time) / param->total_time_info;
        
        rect.x = 0;
        rect.y = 0;
        rect.width = (short)width;
        rect.height = param->fm_rec_pb->height;
        sty_draw_sub_picture_id(param->hdc, param->fm_rec_pb->pic_id, 
                                                    param->fm_rec_pb->x, param->fm_rec_pb->y, 
                                                    &rect);
    }
    else
    {
        print_dbg("recorder status : %d \n", param->recorder_status);    
    }
}

/*
 * 显示总时间
 * 参数：
 *   param: 场景结构体指针
 */
void fm_recorder_draw_totaltime( scene_recording_t *param )
{
    int result;
    int hour;
    int min;
    int second;
    char buf[20];
    sty_rect_t rect;

    if(param->totaltime == need_paint)
    {
        /*! draw background region */
        rect.x = param->fm_rec_totaltime->x;
        rect.y = param->fm_rec_totaltime->y;
        rect.width = param->fm_rec_totaltime->width;
        rect.height = param->fm_rec_totaltime->height;
        sty_draw_sub_bitmap(param->hdc, param->bg_bitmap_info, 
                                                    param->fm_rec_totaltime->x, param->fm_rec_totaltime->y, 
                                                    &rect);
        
        /*! draw total time */    
        result = mmm_ae_cmd(param->ae_handle, MMM_GET_DISK_INFO , (unsigned int)&param->disk_info);
        if(result < 0)
        {
            print_err("get disk info failed\n");
        }
        param->total_time_info = (int)(param->disk_info.info);
        hour = (int)(param->disk_info.info / 3600);
        min = (int)((param->disk_info.info % 3600) / 60);
        second = (int)((param->disk_info.info % 3600) % 60);
        print_info("hour : %02d, min : %02d, second : %02d", hour, min, second);

        sprintf(buf, "%02d:%02d:%02d", hour, min, second);
        sty_draw_string_res_ext( param->hdc, param->fm_rec_totaltime, buf );    
    }
    else
    {
        print_dbg("total time status : %d",param->totaltime);
    }
}
/*
 * 绘制录音状态图片
 * 参数：
 *   param: 场景结构体指针
 */
void fm_recorder_draw_color_status( scene_recording_t *param )
{
    picregion_frame_t picregion_frame;
    sty_rect_t rect;

    if(param->color_status == need_paint)
    {
        rect.x = param->fm_rec_color_status->x;
        rect.y = param->fm_rec_color_status->y;
        rect.width = param->fm_rec_color_status->width;
        rect.height = param->fm_rec_color_status->height;
        sty_draw_sub_bitmap(param->hdc, param->bg_bitmap_info, 
                                                    param->fm_rec_color_status->x, param->fm_rec_color_status->y, 
                                                    &rect);
        if(param->recorder_status == recording)
        {
            get_picregion_frame(param->fm_rec_color_status, RECORDING_FRAME, &picregion_frame);    
        }
        if(param->recorder_status == pause)
        {
            get_picregion_frame(param->fm_rec_color_status, PAUSE_FRAME, &picregion_frame);    
        }
        sty_draw_picture_id(param->hdc, picregion_frame.pic_id, 
                                            param->fm_rec_color_status->x, param->fm_rec_color_status->y);
    }
    else
    {
        print_dbg("color status : %d\n", param->color_status);
    }
}

/*
 * 绘制录音机
 * 参数：
 *   param: 场景结构体指针
 */
void fm_recorder_draw_tape( scene_recording_t *param )
{
    if( param->tape == need_paint )
    {
        sty_draw_picture_res( param->hdc, param->fm_rec_tape );
    }
    else
    {
        print_dbg("tape status : %d\n", param->tape);
    }
}

/*
 * 绘制音量1
 * 参数：
 *   param: 场景结构体指针
 */
void fm_recorder_draw_vol1( scene_recording_t *param )
{
    sty_draw_picture_res( param->hdc, param->fm_rec_vol1 );
}
/*
 * 绘制音量
 * 参数：
 *   param: 场景结构体指针
 */
void fm_recorder_draw_vol2( scene_recording_t *param )
{
    if(param->recording_status.energy_level >= 8)
    {
        sty_draw_picture_res( param->hdc, param->fm_rec_vol2 );
    }
}
/*
 * 绘制音量
 * 参数：
 *   param: 场景结构体指针
 */
void fm_recorder_draw_vol3( scene_recording_t *param )
{
    if(param->recording_status.energy_level >= 14)
    {
        sty_draw_picture_res( param->hdc, param->fm_rec_vol3 );
    }
}
/*
 * 绘制音量
 * 参数：
 *   param: 场景结构体指针
 */
void fm_recorder_draw_vol4( scene_recording_t *param )
{
    if(param->recording_status.energy_level >= 20)
    {
        sty_draw_picture_res( param->hdc, param->fm_rec_vol4 );
    }
}
/*
 * 绘制音量
 * 参数：
 *   param: 场景结构体指针
 */
void fm_recorder_draw_vol5( scene_recording_t *param )
{
    if(param->recording_status.energy_level >= 26)
    {
        sty_draw_picture_res( param->hdc, param->fm_rec_vol5 );
    }
}
/*
 * 绘制音量
 * 参数：
 *   param: 场景结构体指针
 */
void fm_recorder_draw_vol( void *data)
{
    int result;

    scene_recording_t * param = (scene_recording_t *)data;

    /*! draw vol energe background */
    sty_draw_picture_res( param->hdc, param->fm_rec_vol_bg );

    if(param->recorder_status == recording)
    {
        result = mmm_ae_cmd(param->ae_handle, MMM_GET_RECORDER_STATUS, (unsigned int)&param->recording_status);
        if(result < 0)
        {
            print_err("get recording status failed\n");
        }
        print_dbg("volume energe level : %d ", param->recording_status.energy_level);
    
        fm_recorder_draw_vol1( param );
        fm_recorder_draw_vol2( param );
        fm_recorder_draw_vol3( param );
        fm_recorder_draw_vol4( param );
        fm_recorder_draw_vol5( param );
    }
    else
    {
        print_dbg("recorder status : %d \n", param->recorder_status);
    }
}

