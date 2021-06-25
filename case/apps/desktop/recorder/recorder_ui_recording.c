#include "recorder_ui_recording.h"

#define ENERGY_LEVEL1   2
#define ENERGY_LEVEL2   8
#define ENERGY_LEVEL3   14
#define ENERGY_LEVEL4   20
#define ENERGY_LEVEL5   26

void recorder_draw_bg(scene_recording_t *param)
{
    if (param->bg == NEED_PAINT)
    {
        sty_draw_bitmap(param->hdc, param->bg_bitmap_info, 0, 0);
    }
    else
    {
        print_dbg("background status : %d\n", param->bg);
    }
}

void recorder_draw_word_status(scene_recording_t *param)
{
    picregion_frame_t picregion_frame;
    sty_rect_t rect;

    if (param->word_status == NEED_PAINT)
    {
        rect.x = param->rec_word_status->x;
        rect.y = param->rec_word_status->y;
        rect.width = param->rec_word_status->width;
        rect.height = param->rec_word_status->height;

        /*! draw background region */
        sty_draw_sub_bitmap(param->hdc, param->bg_bitmap_info, param->rec_word_status->x, param->rec_word_status->y,
                &rect);

        if (param->recorder_status == STANDBY)
        {
            get_picregion_frame(param->rec_word_status, STANDBY_FRAME, &picregion_frame);
        }
        if (param->recorder_status == RECORDING)
        {
            get_picregion_frame(param->rec_word_status, RECORDING_FRAME, &picregion_frame);
        }
        if (param->recorder_status == PAUSE)
        {
            get_picregion_frame(param->rec_word_status, PAUSE_FRAME, &picregion_frame);
        }

        /*! draw word */
        sty_draw_picture_id(param->hdc, picregion_frame.pic_id, param->rec_word_status->x, param->rec_word_status->y);
    }
    else
    {
        print_dbg("word status : %d\n", param->word_status);
    }
}

void recorder_draw_filename(scene_recording_t *param)
{
    sty_rect_t rect;

    if (param->filename == NEED_PAINT)
    {
        if (param->recorder_status == STANDBY)
        {
            /* clear menu flag */

            /* draw background region */
            rect.x = param->rec_filename->x;
            rect.y = param->rec_filename->y;
            rect.width = param->rec_filename->width;
            rect.height = param->rec_filename->height;
            sty_draw_sub_bitmap(param->hdc, param->bg_bitmap_info, param->rec_filename->x, param->rec_filename->y,
                    &rect);
        }
        else
        {
            /*! 根据option获得录音文件保存格式*/
            if (strcmp(param->format, "wav") == 0)
            {
                param->file_name_buf[14] = '.';
                param->file_name_buf[15] = 'w';
                param->file_name_buf[16] = 'a';
                param->file_name_buf[17] = 'v';
                param->file_name_buf[18] = '\0';
            }
            else if (strcmp(param->format, "mp3") == 0)
            {
                param->file_name_buf[14] = '.';
                param->file_name_buf[15] = 'm';
                param->file_name_buf[16] = 'p';
                param->file_name_buf[17] = '3';
                param->file_name_buf[18] = '\0';
            }
            else
            {
                param->file_name_buf[14] = '.';
                param->file_name_buf[15] = 'w';
                param->file_name_buf[16] = 'm';
                param->file_name_buf[17] = 'a';
                param->file_name_buf[18] = '\0';
            }
            sty_draw_string_res_ext(param->hdc, param->rec_filename, param->file_name_buf);
        }
    }
    else
    {
        print_dbg("filename status : %d \n", param->filename);
    }
}

void recorder_draw_pbbg(scene_recording_t *param)
{
    if (param->pbbg == NEED_PAINT)
    {
        sty_draw_picture_res(param->hdc, param->rec_pbbg);
    }
    else
    {
        print_dbg("process bar background status : %d\n", param->pbbg);
    }
}

void recorder_draw_pbhl_curtime(void *data)
{
    unsigned int hour = 0;
    unsigned int min = 0;
    unsigned int second = 0;
    char buf[20];
    int result;
    unsigned int width;
    sty_rect_t rect;

    scene_recording_t *param = (scene_recording_t *) data;

    /*! recorder standby时，curtime为00:00:00, pb高亮不显示*/
    if (param->recorder_status == STANDBY)
    {
        /*! draw rec time*/
        rect.x = param->rec_curtime->x;
        rect.y = param->rec_curtime->y;
        rect.width = param->rec_curtime->width;
        rect.height = param->rec_curtime->height;

        sty_draw_sub_bitmap(param->hdc, param->bg_bitmap_info, param->rec_curtime->x, param->rec_curtime->y, &rect);
        sprintf(buf, "%02d:%02d:%02d", hour, min, second);
        sty_draw_string_res_ext(param->hdc, param->rec_curtime, buf);

        /*! draw process bar background*/
        sty_draw_picture_res(param->hdc, param->rec_pbbg);
    }
    else if (param->recorder_status == RECORDING)
    {
        result = mmm_ae_cmd(param->ae_handle, MMM_GET_RECORDER_STATUS, (unsigned int) &param->recording_status);
        if (result < 0)
        {
            print_warning("get recording status failed\n");
        }

        /*! draw rec time */
        hour = param->recording_status.time / 3600;
        min = (param->recording_status.time % 3600) / 60;
        second = (param->recording_status.time % 3600) % 60;
//        print_info("hour : %02d, min : %02d, second : %02d", hour, min, second);
        rect.x = param->rec_curtime->x;
        rect.y = param->rec_curtime->y;
        rect.width = param->rec_curtime->width;
        rect.height = param->rec_curtime->height;

        sty_draw_sub_bitmap(param->hdc, param->bg_bitmap_info, param->rec_curtime->x, param->rec_curtime->y, &rect);
        sprintf(buf, "%02d:%02d:%02d", hour, min, second);
        sty_draw_string_res_ext(param->hdc, param->rec_curtime, buf);

        /*! draw process bar (highlight) */
        if (0 == param->total_time_info)
        {
            print_err("total_time_info is 0\n");
            return;
        }
        else
        {
            width = ((unsigned int)(param->rec_pb->width) * param->recording_status.time )/ param->total_time_info;
        }

        rect.x = 0;
        rect.y = 0;
        rect.width = (short) width;
        rect.height = param->rec_pb->height;
        sty_draw_sub_picture_id(param->hdc, (int)param->rec_pb->pic_id, param->rec_pb->x, param->rec_pb->y, &rect);
    }
    else
    {
        print_dbg("recorder status : %d \n", param->recorder_status);
    }
}

void recorder_draw_totaltime(scene_recording_t *param)
{
    int result;
    unsigned int hour;
    unsigned int min;
    unsigned int second;
    char buf[20];
    sty_rect_t rect;

    if (param->totaltime == NEED_PAINT)
    {
        /*! draw background region */
        rect.x = param->rec_totaltime->x;
        rect.y = param->rec_totaltime->y;
        rect.width = param->rec_totaltime->width;
        rect.height = param->rec_totaltime->height;
        sty_draw_sub_bitmap(param->hdc, param->bg_bitmap_info, param->rec_totaltime->x, param->rec_totaltime->y, &rect);

        /*! draw total time */
        result = mmm_ae_cmd(param->ae_handle, MMM_GET_DISK_INFO, (unsigned int) &param->disk_info);
        if (result < 0)
        {
            print_warning("get disk info failed\n");
        }
        param->total_time_info = param->disk_info.info;
        hour = param->disk_info.info / 3600;
        min = (param->disk_info.info % 3600) / 60;
        second = (param->disk_info.info % 3600) % 60;
        print_info("hour : %02d, min : %02d, second : %02d", hour, min, second);

        sprintf(buf, "%02d:%02d:%02d", hour, min, second);
        sty_draw_string_res_ext(param->hdc, param->rec_totaltime, buf);
    }
    else
    {
        print_dbg("total time status : %d", param->totaltime);
    }
}

void recorder_draw_color_status(scene_recording_t *param)
{
    picregion_frame_t picregion_frame;
    sty_rect_t rect;

    if (param->color_status == NEED_PAINT)
    {
        rect.x = param->rec_color_status->x;
        rect.y = param->rec_color_status->y;
        rect.width = param->rec_color_status->width;
        rect.height = param->rec_color_status->height;
        sty_draw_sub_bitmap(param->hdc, param->bg_bitmap_info, param->rec_color_status->x, param->rec_color_status->y,
                &rect);
        if (param->recorder_status == STANDBY)
        {
            get_picregion_frame(param->rec_color_status, STANDBY_FRAME, &picregion_frame);
        }
        if (param->recorder_status == RECORDING)
        {
            get_picregion_frame(param->rec_color_status, RECORDING_FRAME, &picregion_frame);
        }
        if (param->recorder_status == PAUSE)
        {
            get_picregion_frame(param->rec_color_status, PAUSE_FRAME, &picregion_frame);
        }
        sty_draw_picture_id(param->hdc, picregion_frame.pic_id, param->rec_color_status->x, param->rec_color_status->y);
    }
    else
    {
        print_dbg("color status : %d\n", param->color_status);
    }
}

void recorder_draw_tape(scene_recording_t *param)
{
    if (param->tape == NEED_PAINT)
    {
        sty_draw_picture_res(param->hdc, param->rec_tape);
    }
    else
    {
        print_dbg("tape status : %d\n", param->tape);
    }
}

void recorder_draw_vol1(scene_recording_t *param)
{
    if ((param->recording_status.energy_level >= ENERGY_LEVEL1) && (param->recorder_status == RECORDING))         
    {
        sty_draw_picture_res(param->hdc, param->rec_vol1);
    }
}

void recorder_draw_vol2(scene_recording_t *param)
{
    if ((param->recording_status.energy_level >= ENERGY_LEVEL2) && (param->recorder_status == RECORDING))
    {
        sty_draw_picture_res(param->hdc, param->rec_vol2);
    }
}

void recorder_draw_vol3(scene_recording_t *param)
{
    if ((param->recording_status.energy_level >= ENERGY_LEVEL3) && (param->recorder_status == RECORDING))
    {
        sty_draw_picture_res(param->hdc, param->rec_vol3);
    }
}

void recorder_draw_vol4(scene_recording_t *param)
{
    if ((param->recording_status.energy_level >= ENERGY_LEVEL4) && (param->recorder_status == RECORDING))
    {
        sty_draw_picture_res(param->hdc, param->rec_vol4);
    }
}

void recorder_draw_vol5(scene_recording_t *param)
{
    if ((param->recording_status.energy_level >= ENERGY_LEVEL5) && (param->recorder_status == RECORDING))
    {
        sty_draw_picture_res(param->hdc, param->rec_vol5);
    }
}

void recorder_draw_vol(void *data)
{
    int result;

    scene_recording_t * param = (scene_recording_t *) data;

    /*! draw vol energe background */
    sty_draw_picture_res(param->hdc, param->rec_vol_bg);
    /*在录音的时候显示能量条*/
    if ( param->recorder_status == RECORDING )
    {
        result = mmm_ae_cmd(param->ae_handle, MMM_GET_RECORDER_STATUS, (unsigned int) &param->recording_status);
        if (result < 0)
        {
            print_warning("get recording status failed\n");
        }
        print_dbg("volume energe level : %d ", param->recording_status.energy_level);

        recorder_draw_vol1(param);
        recorder_draw_vol2(param);
        recorder_draw_vol3(param);
        recorder_draw_vol4(param);
        recorder_draw_vol5(param);
    }
    else
    {
        print_dbg("recorder status : %d \n", param->recorder_status);
    }
}
