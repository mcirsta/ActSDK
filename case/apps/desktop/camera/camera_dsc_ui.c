/*******************************************************************************
 *                              USDK(1100)
 *                            Module: camera_dsc
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *        2009-5-26 16:27   1.0             build this file
 *******************************************************************************/
/*!
 * \file     camera_dsc_ui.c
 * \brief    概述
 * \author
 * \par      GENERAL DESCRIPTION:
 *               dsc预览界面ui图标显示
 * \par      EXTERNALIZED FUNCTIONS:
 *               用到外面的模块
 * \version 1.0
 * \date  2009-5-26
 *******************************************************************************/
#include "camera_dsc_ui.h"
#include "camera_scene_dsc.h"
#include "camera_dsc_control.h"

char g_dsc_resolution_buf[ARRAY_LENGTH_RESOLUTION] = "4672X3504";
char g_free_number_buf[ARRAY_LENGTH_TIME] = "1234567890";//100:59:59

int g_date_stamp_h;

#define RESOLUTION_ICON_BUFFER_LENGTH sizeof"4672X3504"()/sizeof(char)


void camera_dsc_ui_free_space_to_buf(void)
{
    snprintf(g_free_number_buf, sizeof(g_free_number_buf), "%d", g_camera_scene_dsc->dsc_config.cfg_freeinfo.info);

}

mmm_res_constant_t camera_dsc_ui_resolution_to_buf(int value)
{
    mmm_res_constant_t res_constant[] =
    { 
        MMM_RES4672X3504, MMM_RES4416X3312, MMM_RES3840X2880, MMM_RES3264X2448, MMM_RES2816X2112, MMM_RES2560X1920,
        MMM_RES2400X1800, MMM_RES2048X1536, MMM_RES1600X1200, MMM_RES1280X1024,/*MMM_RES1280X960,*/
        MMM_RES1024X768, MMM_RES640X480, 
    };
    int offset, max_scale;
    int x_zoom = 30;
#if 0
#if (CMOS_SENSOR == 310)
    {
        MMM_RES4672X3504,MMM_RES4416X3312,MMM_RES3840X2880,MMM_RES3264X2448,MMM_RES2816X2112,MMM_RES2560X1920,MMM_RES2400X1800,MMM_RES2048X1536,
    };
    // 2M
#elif (CMOS_SENSOR == 200)
    {
        MMM_RES3840X2880,MMM_RES3264X2448,MMM_RES2816X2112,MMM_RES2560X1920,MMM_RES2400X1800,MMM_RES2048X1536,MMM_RES1600X1200,
    };
    // 1.3M
#elif (CMOS_SENSOR == 130)
    {   MMM_RES2560X1920,MMM_RES2400X1800,MMM_RES2048X1536,MMM_RES1600X1200,MMM_RES1280X960,
    };
    // .3M
#elif (CMOS_SENSOR == 30)
    {
        MMM_RES2048X1536,MMM_RES1600X1200,MMM_RES1280X960,MMM_RES1024X768,MMM_RES640X480,
    };
#endif
#endif
    /*! 保存的第一个分辨率和支持分辨率的项数 */
    camera_dsc_get_cur_sensor_items(&offset, &max_scale);

    assert(value <= max_scale);
    /*! value + offset都是从0开始 */
    switch (res_constant[value + offset])
    {
        case MMM_RES4672X3504:
        snprintf(g_dsc_resolution_buf, sizeof(g_dsc_resolution_buf), "4672X3504");
        g_date_stamp_h = 3504 / x_zoom;
        break;
        case MMM_RES4416X3312:
        snprintf(g_dsc_resolution_buf, sizeof(g_dsc_resolution_buf), "4416X3312");
        g_date_stamp_h = 3312 / x_zoom;
        break;
        case MMM_RES3840X2880:
        snprintf(g_dsc_resolution_buf, sizeof(g_dsc_resolution_buf), "3840X2880");
        g_date_stamp_h = 2880 / x_zoom;
        break;
        case MMM_RES3264X2448:
        snprintf(g_dsc_resolution_buf, sizeof(g_dsc_resolution_buf), "3264X2448");
        g_date_stamp_h = 2448 / x_zoom;
        break;
        case MMM_RES2816X2112:
        snprintf(g_dsc_resolution_buf, sizeof(g_dsc_resolution_buf), "2816X2112");
        g_date_stamp_h = 2112 / x_zoom;
        break;
        case MMM_RES2560X1920:
        snprintf(g_dsc_resolution_buf, sizeof(g_dsc_resolution_buf), "2560X1920");
        g_date_stamp_h = 1920 / x_zoom;
        break;
        case MMM_RES2400X1800:
        //1800实际为1808
        snprintf(g_dsc_resolution_buf, sizeof(g_dsc_resolution_buf), "2400X1808");
        g_date_stamp_h = 1808 / x_zoom;
        break;
        case MMM_RES2048X1536:
        snprintf(g_dsc_resolution_buf, sizeof(g_dsc_resolution_buf), "2048X1536");
        g_date_stamp_h = 1536 / x_zoom;
        break;
        case MMM_RES1600X1200:
        snprintf(g_dsc_resolution_buf, sizeof(g_dsc_resolution_buf), "1600X1200");
        g_date_stamp_h = 1200 / x_zoom;
        break;
        /*
         case MMM_RES1280X960:
         snprintf(g_dsc_resolution_buf, sizeof(g_dsc_resolution_buf), "1280X960");
         g_date_stamp_h = 960 / x_zoom;
         break;
         */
        case MMM_RES1280X1024:
        snprintf(g_dsc_resolution_buf, sizeof(g_dsc_resolution_buf), "1280X1024");
        g_date_stamp_h = 960 / x_zoom;
        break;
        case MMM_RES1024X768:
        snprintf(g_dsc_resolution_buf, sizeof(g_dsc_resolution_buf), "1024X768");
        g_date_stamp_h = 768 / x_zoom;
        break;
        case MMM_RES640X480:
        snprintf(g_dsc_resolution_buf, sizeof(g_dsc_resolution_buf), "640X480");
        g_date_stamp_h = 480 / x_zoom;
        break;
        default:
        snprintf(g_dsc_resolution_buf, sizeof(g_dsc_resolution_buf), "640X480");
        break;
    }
    print_info("g_dsc_resolution_buf=%s\n",g_dsc_resolution_buf);
    return (mmm_res_constant_t) res_constant[value + offset];
    //如果错了default就不会返回正确的值
}

void camera_dsc_ui_draw_slider_bg(int hdc, ui_pic_t *ui_pic)
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

/******************************************************************************/
/*!
 * \par  Description:
 *      将当前播放时间,总时间 转换成进度条的x坐标值
 * \param[in]    int: 当前播放时间, 单位: ms
 * \param[in]    int: 总时间, 单位: ms
 * \param[out]   int *: 计算后的高亮条最右侧 x坐标
 * \retval
 * \ingroup      video
 *******************************************************************************/
bool camera_dsc_ui_zoom_level_to_coordinate(int cur_zoom_level, int total_zoom_level, int *y_coordinate)
{
    static int s_saved_cur_zoom_level = -1;
    static int s_saved_total_zoom_level = -1;
    /* 高亮条最右侧的x坐标. 传给pbar */
    int y;
    int h0 = g_camera_scene_dsc->dsc_ui_icon.foci_slider.pic_h_res->height;
    int y0 = g_camera_scene_dsc->dsc_ui_icon.foci_slider.pic_h_res->y;

    print_info("cur=%d,total=%d,savecur=%d,savetotol=%d\n", cur_zoom_level, total_zoom_level, s_saved_cur_zoom_level,
            s_saved_total_zoom_level);
    if ((NULL == y_coordinate) || (cur_zoom_level < 0) || (total_zoom_level <= 0)
            || (cur_zoom_level > total_zoom_level))
    {
        print_err("parameter err. cur_time:%d, total_time:%d", cur_zoom_level, total_zoom_level);
        return false;
    }

    //没有变化, 直接返回. 只有变化才继续计算

    s_saved_cur_zoom_level = cur_zoom_level;
    s_saved_total_zoom_level = total_zoom_level;

    //计算高亮条右端坐标, 并考虑到高亮条左侧缩进的坐标
    y = (h0 * (total_zoom_level - cur_zoom_level)) / total_zoom_level;
    y += y0;

    *y_coordinate = y;
    print_info("after cal. ***********y=%d\n", y);

    return true;
}

void camera_dsc_ui_draw_slider(int hdc, ui_slider_t *ui_slider, int y)
{
    //初始为高亮条左侧缩进的坐标

    bool changed;
    picture_resource_t* p = ui_slider->pic_h_res;
    changed = (bool) (((ui_slider->saved_show != ui_slider->show) || (ui_slider->saved_y != y)) ? true : false);
    print_info("show:%d, changed:%d, frame:%d, saved_frame:%d", \
    ui_slider->show, changed, ui_slider->y, ui_slider->saved_y);
    print_info("xywh:%d,%d,%d,%d", p->x, p->y, p->width, p->height);

    if (y < 0)
    {
        print_err("x:%d, param err", y);
        return;
    }

    if (1 == ui_slider->show)
    {
        if (true == changed)
        {
            sty_rect_t srect;
            int canvas_id = g_camera_scene_dsc->blending.slider_section.canvas_id;
            sty_draw_picture_res(hdc, ui_slider->pic_bg_res);
            sty_draw_string_res_ext(hdc, ui_slider->max_str_res, "3");
            sty_draw_string_res_ext(hdc, ui_slider->min_str_res, "0");
            srect.x = p->x;
            srect.y = p->y;
            srect.width = p->width;
            srect.height = p->height;
            canvas_erase(canvas_id, &srect);
            srect.x = ui_slider->pic_h_res->x - ui_slider->pic_bg_res->x;
            srect.y = ui_slider->pic_h_res->y - ui_slider->pic_bg_res->y;
            srect.width = ui_slider->pic_h_res->width;
            srect.height = ui_slider->pic_h_res->height;
            print_info("BBBBBBBBBBBBBBshow bg x=%d,y=%d,w=%d,h=%d\n", srect.x, srect.y, srect.width, srect.height);
            sty_draw_sub_picture_id(hdc, (int) ui_slider->pic_bg_res->pic_id, p->x, p->y, &srect);

            srect.x = p->x - p->x;
            srect.width = p->width;
            srect.y = (short) y - p->y; //绝对win y
            assert(srect.y >= 0);
            srect.height = (((p->y) + (p->height)) - ((short) (y)));
            print_info("HHHHHHHHHHHHshow hl x=%d,y=%d,w=%d,h=%d\n", srect.x, srect.y, srect.width, srect.height);
            sty_draw_sub_picture_id(hdc, (int) ui_slider->pic_h_res->pic_id, p->x, y, &srect);
        }
    }
    else if (0 == ui_slider->show)
    {
        print_warning("set  hide is invalid\n\n");
    }
    else
    {
    }
    ui_slider->saved_show = ui_slider->show;
    ui_slider->saved_y = ui_slider->y;
}

void camera_dsc_ui_draw_pic(int hdc, ui_pic_t *ui_pic)
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

void camera_dsc_ui_draw_picregion(int hdc, ui_picregion_t *ui_picregion, int canvas_id)
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

void camera_dsc_ui_draw_string(int hdc, ui_string_t *ui_string, int canvas_id)
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
    strncpy(ui_string->saved_buf, ui_string->buf,strlen(ui_string->saved_buf));
    //TODO:check strncpy use
    /*
    strncpy 的标准用法为：（手工写上 \0）
    strncpy(path, src, sizeof(path) - 1);
    path[sizeof(path) - 1] = '\0';
    len = strlen(path);
    */
    print_dbg();
}

void camera_dsc_change_icon(cam_icon_e icon, unsigned int frame)
{
    switch (icon)
    {
        case CAM_ICON_SELFTIME:
        g_camera_scene_dsc->dsc_ui_icon.self_time.show = 1;
        g_camera_scene_dsc->dsc_ui_icon.self_time.frame = (char) frame;
        break;
        case CAM_ICON_BRIGHTNESS:
        g_camera_scene_dsc->dsc_ui_icon.brightness.show = 1;
        g_camera_scene_dsc->dsc_ui_icon.brightness.frame = (char) frame + 3;
        break;
        case CAM_ICON_SPECIAL_EFFECT:
        g_camera_scene_dsc->dsc_ui_icon.special_effect.show = 1;
        g_camera_scene_dsc->dsc_ui_icon.special_effect.frame = (char) frame;
        break;
        case CAM_ICON_WHITE_BALANCE:
        g_camera_scene_dsc->dsc_ui_icon.white_balance.show = 1;
        g_camera_scene_dsc->dsc_ui_icon.white_balance.frame = (char) frame;
        break;
        case CAM_ICON_FLASH_LAMP:
        g_camera_scene_dsc->dsc_ui_icon.flash_lamp.show = 1;
        g_camera_scene_dsc->dsc_ui_icon.flash_lamp.frame = (char) frame;
        break;
        case CAM_ICON_SHUTTER_SOUND:
        g_camera_scene_dsc->dsc_ui_icon.shutter_sound.show = 1;
        g_camera_scene_dsc->dsc_ui_icon.shutter_sound.frame = (char) frame;
        break;
        case CAM_ICON_STORE_TYPE:
        g_camera_scene_dsc->dsc_ui_icon.store_type.show = 1;
        if (frame > 0)
        {
            frame -= 1;
        }
        g_camera_scene_dsc->dsc_ui_icon.store_type.saved_frame = -1;
        //这里要强制更新，因为没卡的时候不会执行刷新，有时有卡也会无图标
        g_camera_scene_dsc->dsc_ui_icon.store_type.frame = (char) frame;
        
        /*! 改变了存储路径后需要重新获取剩余值 */        
        g_camera_scene_dsc->dsc_ui_icon.free_space_number.show = 1;
        snprintf(g_free_number_buf, sizeof(g_free_number_buf), "%d",\
         (g_camera_scene_dsc->dsc_config.cfg_freeinfo.info));        
        break;
        case CAM_ICON_RESOLUTION:
        g_camera_scene_dsc->dsc_ui_icon.resolution.show = 1;
        camera_dsc_ui_resolution_to_buf((int)frame);

        g_camera_scene_dsc->dsc_ui_icon.free_space_number.show = 1;
        snprintf(g_free_number_buf, sizeof(g_free_number_buf), "%d",\
         (g_camera_scene_dsc->dsc_config.cfg_freeinfo.info));
        break;
        case CAM_ICON_FREE_NUMBER:
        g_camera_scene_dsc->dsc_ui_icon.free_space_number.show = 1;
        snprintf(g_free_number_buf, sizeof(g_free_number_buf), "%d", frame);
        break;
        case CAM_ICON_FOCI_SLIDER:
        g_camera_scene_dsc->dsc_ui_icon.foci_slider.show = 1;
        //in paint juge the param
        break;

        default:
        break;
    }
    g_camera_scene_dsc->dsc_ui_icon.focus.show = 1;
    camera_scene_dsc_paint(g_camera_scene_dsc);
}

