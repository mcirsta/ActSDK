/*******************************************************************************
 *                              USDK(1100)
 *                            Module: camera_dv
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *        2009-5-26 16:27   1.0             build this file
 *******************************************************************************/
/*!
 * \file     camera_dv_ui.c
 * \brief    概述
 * \author
 * \par      GENERAL DESCRIPTION:
 *               dv预览界面ui图标显示
 * \par      EXTERNALIZED FUNCTIONS:
 *               用到外面的模块
 * \version 1.0
 * \date  2009-5-26
 *******************************************************************************/
#include "camera_dv_ui.h"
#include "camera_scene_dv.h"
#include "camera_dv_control.h"
#include "camera_define.h"

char g_dv_resolution_buf[ARRAY_LENGTH_RESOLUTION] = "4672X3504";
char g_use_free_number_buf[ARRAY_LENGTH_TIME] = "99:59:59/99:59:59 ";//100:59:59

static bool _dv_millisecond2time(rtc_time_t* time, unsigned int millisecond);

#define RESOLUTION_ICON_BUFFER_LENGTH sizeof"4672X3504"()/sizeof(char)


void camera_dv_ui_free_space_to_buf(void)
{
    //只是借用时间的结构体
    rtc_time_t freetime, usetime;
    _dv_millisecond2time(&freetime, g_camera_scene_dv->dv_config.cfg_freeinfo.info);
    _dv_millisecond2time(&usetime, g_camera_scene_dv->dv_config.dv_status.time);
    snprintf(g_use_free_number_buf,  sizeof(g_use_free_number_buf), "%02d:%02d:%02d/%02d:%02d:%02d", \
        usetime.hour, usetime.minute, usetime.second, freetime.hour, freetime.minute, freetime.second);
}

mmm_res_constant_t camera_dv_ui_resolution_to_buf(int value)
{
    mmm_res_constant_t res_constant[] =
    { MMM_RES640X480, MMM_RES320X240, MMM_RES224X176, MMM_RES176X144, };
    int offset, max_scale;
    //int x_zoom = 30;

    camera_dv_get_cur_sensor_items(&offset, &max_scale);
    assert(value <= max_scale);
    /*! value + offset都是从0开始 */
    switch (res_constant[value + offset])
    {
        case MMM_RES640X480:
        snprintf(g_dv_resolution_buf, sizeof(g_dv_resolution_buf), "640X480");
        break;
        case MMM_RES320X240:
        snprintf(g_dv_resolution_buf, sizeof(g_dv_resolution_buf), "320X240");
        break;
        case MMM_RES224X176:
        snprintf(g_dv_resolution_buf, sizeof(g_dv_resolution_buf), "224X176");
        break;
        case MMM_RES176X144:
        snprintf(g_dv_resolution_buf, sizeof(g_dv_resolution_buf), "176X144");
        break;
        default:
        snprintf(g_dv_resolution_buf, sizeof(g_dv_resolution_buf), "320X240");
        print_err();

        break;
    }

    return (mmm_res_constant_t) res_constant[value + offset];
}
mmm_video_bitrate_level_t camera_dv_ui_quality_to_bitrate(int value)
{
    mmm_video_bitrate_level_t ret;
    assert((value >= MMM_VIDDEO_CBR_LEVEL0) && (value <= MMM_VIDDEO_VBR));

    switch (value)
    {
        case 0:
        ret = MMM_VIDDEO_CBR_LEVEL3;
        break;
        case 1:
        ret = MMM_VIDDEO_CBR_LEVEL2;
        break;
        case 2:
        ret = MMM_VIDDEO_CBR_LEVEL1;
        break;
        case 3:
        ret = MMM_VIDDEO_VBR;
        break;
        default:
        ret = MMM_VIDDEO_VBR;
        break;
    }
    return ret;
}
void camera_dv_ui_draw_slider_bg(int hdc, ui_pic_t *ui_pic)
{
    bool changed;
    changed = (bool) (((ui_pic->saved_show) != (ui_pic->show)) ? true : false);

    if (1 == ui_pic->show)
    {
        if (true == changed)
        {
            sty_draw_picture_res(hdc, ui_pic->pic_res);
        }
    }
    else if (0 == ui_pic->show)
    {
        print_warning("set bg hide is invalid\n\n");
    }
    else if (2 == ui_pic->show)
    {
        gui_dc_clear_rect(hdc, ui_pic->pic_res->x, ui_pic->pic_res->y, ui_pic->pic_res->x + ui_pic->pic_res->width,
                ui_pic->pic_res->y + ui_pic->pic_res->height);
        print_warning("set bg hide is invalid\n\n");
    }
    else
    {
    }

    ui_pic->saved_show = ui_pic->show;
}

void camera_dv_ui_draw_pic(int hdc, ui_pic_t *ui_pic)
{
    bool changed;
    changed = (bool) (((ui_pic->saved_show) != (ui_pic->show)) ? true : false);

    if (1 == ui_pic->show)
    {
        if (true == changed)
        {
            sty_draw_picture_res(hdc, ui_pic->pic_res);
        }
    }
    else if (0 == ui_pic->show)
    {
        print_warning("set bg hide is invalid\n\n");
    }
    else if (2 == ui_pic->show)
    {
        gui_dc_clear_rect(hdc, ui_pic->pic_res->x, ui_pic->pic_res->y, ui_pic->pic_res->x + ui_pic->pic_res->width,
                ui_pic->pic_res->y + ui_pic->pic_res->height);
        print_warning("set bg hide is invalid\n\n");
    }
    else
    {
    }

    ui_pic->saved_show = ui_pic->show;
}

void camera_dv_ui_draw_picregion(int hdc, ui_picregion_t *ui_picregion, int canvas_id)
{
    bool changed;
    picregion_resource_t* p = ui_picregion->picregion_res;
    changed = (bool) (((ui_picregion->saved_show != ui_picregion->show) || (ui_picregion->saved_frame
            != ui_picregion->frame)) ? true : false);

    print_dbg("show:%d, changed:%d, frame:%d, saved_frame:%d", ui_picregion->show,
            changed, ui_picregion->frame,ui_picregion->saved_frame);
    print_dbg("xywh:%d,%d,%d,%d", p->x, p->y, p->width, p->height);

    if (1 == ui_picregion->show)
    {
        if (true == changed)
        {
            //先画图片所在区域的背景, 否则前景的png图片会重叠在一起
            sty_rect_t srect;
            srect.x = p->x;
            srect.y = p->y;
            srect.width = p->width;
            srect.height = p->height;
            canvas_erase(canvas_id, &srect);
            sty_draw_picregion_res(hdc, p, (unsigned short) ui_picregion->frame);
        }
    }
    else if (0 == ui_picregion->show)
    {
        print_warning("set play_pause hide is invalid\n\n");
    }
    else
    {
    }

    ui_picregion->saved_show = ui_picregion->show;
    ui_picregion->saved_frame = ui_picregion->frame;
}

void camera_dv_ui_draw_picregion_status(int hdc, ui_picregion_t *ui_picregion, int canvas_id)
{
    bool changed;
    picregion_resource_t* p = ui_picregion->picregion_res;
    changed = (bool) (((ui_picregion->saved_show != ui_picregion->show) || (ui_picregion->saved_frame
            != ui_picregion->frame)) ? true : false);

    print_dbg("show:%d, changed:%d, frame:%d, saved_frame:%d", ui_picregion->show,
            changed, ui_picregion->frame,ui_picregion->saved_frame);
    print_dbg("xywh:%d,%d,%d,%d", p->x, p->y, p->width, p->height);

    if (1 == ui_picregion->show)
    {
        if (true == changed)
        {
            //先画图片所在区域的背景, 否则前景的png图片会重叠在一起
            sty_rect_t srect;
            srect.x = p->x;
            srect.y = p->y;
            srect.width = p->width;
            srect.height = p->height;
            canvas_erase(canvas_id, &srect);
            sty_draw_picregion_res(hdc, p, (unsigned short) ui_picregion->frame);
        }
    }
    else if (0 == ui_picregion->show)
    {
        print_warning("set play_pause hide is invalid\n\n");
    }
    else
    {
    }

    ui_picregion->saved_show = ui_picregion->show;
    ui_picregion->saved_frame = ui_picregion->frame;
}

#if (DV_ICON_STATUS2SDRAM == 1)
void camera_dv_ui_draw_sdram_status(int hdc, ui_picregion_t *ui_picregion, int canvas_id)
{
    bool changed;
    picregion_resource_t* p = ui_picregion->picregion_res;
    changed = (bool) (((ui_picregion->saved_show != ui_picregion->show) || (ui_picregion->saved_frame
            != ui_picregion->frame)) ? true : false);

    print_dbg("show:%d, changed:%d, frame:%d, saved_frame:%d", ui_picregion->show,
            changed, ui_picregion->frame,ui_picregion->saved_frame);
    print_dbg("xywh:%d,%d,%d,%d", p->x, p->y, p->width, p->height);

    if (1 == ui_picregion->show)
    {
        if (true == changed)
        {
            //先画图片所在区域的背景, 否则前景的png图片会重叠在一起
            sty_rect_t srect;
            srect.x = p->x;
            srect.y = p->y;
            srect.width = p->width;
            srect.height = p->height;
            canvas_erase(canvas_id, &srect);
            sty_draw_bitmap(hdc,sdram_buf_icon_info[(unsigned int)(ui_picregion->frame)], \
            sdram_buf_icon_frame[(unsigned int)(ui_picregion->frame)].x, \
            sdram_buf_icon_frame[(unsigned int)(ui_picregion->frame)].y);
        }
    }
    else if (0 == ui_picregion->show)
    {
        print_warning("set play_pause hide is invalid\n\n");
    }
    else
    {
    }

    ui_picregion->saved_show = ui_picregion->show;
    ui_picregion->saved_frame = ui_picregion->frame;
}
#endif /* DV_ICON_STATUS2SDRAM */


void camera_dv_ui_draw_string(int hdc, ui_string_t *ui_string, int canvas_id)
{
    bool changed;
    int tmp;
    string_resource_t* p = ui_string->str_res;

    tmp = strcmp(ui_string->saved_buf, ui_string->buf);
    changed = (bool) (((ui_string->saved_show != ui_string->show) || (tmp != 0)) ? true : false);
    /*print_dbg("show:%d, changed:%d, title:%s, savedtitle:%s", ui_string->show, ui_string->changed,\
    ui_string->buf, ui_string->saved_buf);*/

    if (1 == ui_string->show)
    {
        if (true == changed)
        {
            sty_rect_t srect;
            srect.x = p->x;
            srect.y = p->y;
            srect.width = p->width;
            srect.height = p->height;

            canvas_erase(canvas_id, &srect);
            sty_draw_string_res_ext(hdc, p, ui_string->buf);
        }
    }
    else if (0 == ui_string->show)
    {
        print_warning("set time hide is invalid\n\n");
    }
    else
    {
    }
    ui_string->saved_show = ui_string->show;
    memset(ui_string->saved_buf, 0x00, sizeof(ui_string->saved_buf));
    strncpy(ui_string->saved_buf, ui_string->buf, strlen(ui_string->saved_buf));
    print_dbg();
}
void camera_dv_change_icon(cam_icon_e icon, unsigned int frame)
{
    switch (icon)
    {
        case CAM_ICON_BRIGHTNESS:
        g_camera_scene_dv->dv_ui_icon.brightness.show = 1;
        g_camera_scene_dv->dv_ui_icon.brightness.frame = (char) frame + 3;
        break;

        case CAM_ICON_SPECIAL_EFFECT:
        g_camera_scene_dv->dv_ui_icon.special_effect.show = 1;
        g_camera_scene_dv->dv_ui_icon.special_effect.frame = (char) frame;
        break;

        case CAM_ICON_WHITE_BALANCE:
        g_camera_scene_dv->dv_ui_icon.white_balance.show = 1;
        g_camera_scene_dv->dv_ui_icon.white_balance.frame = (char) frame;
        break;

        case CAM_ICON_STORE_TYPE:
        g_camera_scene_dv->dv_ui_icon.store_type.show = 1;
        if(frame >0)
        {
            frame -= 1;
        }
        g_camera_scene_dv->dv_ui_icon.store_type.saved_frame = -1;
        //这里要强制更新，因为没卡的时候不会执行刷新，有时有卡也会无图标
        g_camera_scene_dv->dv_ui_icon.store_type.frame = (char) frame;

        g_camera_scene_dv->dv_ui_icon.use_free_number.show = 1;
        camera_dv_ui_free_space_to_buf();        
        break;

        case CAM_ICON_EXPOSURE_MODE:
        g_camera_scene_dv->dv_ui_icon.exposure_mode.show = 1;
        g_camera_scene_dv->dv_ui_icon.exposure_mode.frame = (char) frame;
        break;

        case CAM_ICON_REC_STATUS:
        g_camera_scene_dv->dv_ui_icon.rec_status.show = 1;
        g_camera_scene_dv->dv_ui_icon.rec_status.frame = (char) frame;
        break;

        case CAM_ICON_RECORD_QUALITY:
        g_camera_scene_dv->dv_ui_icon.record_quality.show = 1;
        g_camera_scene_dv->dv_ui_icon.record_quality.frame = (char) frame;

        g_camera_scene_dv->dv_ui_icon.use_free_number.show = 1;
        camera_dv_ui_free_space_to_buf();        
        break;

        case CAM_ICON_USE_FREE_NUMBER:
        case CAM_ICON_RESOLUTION:
        g_camera_scene_dv->dv_ui_icon.use_free_number.show = 1;
        camera_dv_ui_free_space_to_buf();
        break;

        default:
        break;
    }
   // g_camera_scene_dv->dv_ui_icon.focus.show = 1;
    camera_scene_dv_paint(g_camera_scene_dv);
}

/******************************************************************************/
/*!
 * \par  Description:
 *      将以ms为单位的时间转换成: "**:**:**"格式的字符串
 * \param[in]    int: 时间, 单位: ms
 * \param[out]   int *: 转换后的字符串格式:"**:**:**", 时:分:秒制
 * \retval
 * \ingroup      dv
 *******************************************************************************/
static bool _dv_millisecond2time(rtc_time_t* time, unsigned int millisecond)
{
    int tmp;
    time->second = 0;
    time->minute = 0;
    time->hour = 0;

    //计算秒
    time->second = (int)millisecond;

    if (time->second >= 60)
    {
        tmp = time->second;
        time->second %= 60;
        //计算分
        time->minute = tmp / 60;
        if (time->minute >= 60)
        {
            tmp = time->minute;
            time->minute %= 60;
            //计算小时
            time->hour = tmp / 60;
            assert(time->hour < 99);
        }
    }

    //    snprintf( str, sizeof(str), "%.2d:%.2d:%.2d", hour, minite, second );
    //    print_dbg("time:%d, time string:%s", time, str);
    return true;
}

