/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_playing_ui.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      cheason0   2009-06-14          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_playing_ui.c
 * \brief    ui operation of photo playing scene
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/06/14
 *******************************************************************************/
#include "photo.h"
#include "photo_scene_playing.h"
#include "photo_playing_ui.h"
#include "photo_playlist.h"

static void _fusion_cb(unsigned char *framebuffer, int width, int height);

static void _error_dialog_cb(dialog_t *dlg, widget_sig_dialog_e signal)
{ 
}

void draw_directbar(scene_playing_t *p, bool show)
{
    int hdc = gui_dc_get((unsigned short)p->canvas_directbar_id);

    if (true == show)
    {
        canvas_show(p->canvas_directbar_id);
        canvas_erase_all(p->canvas_directbar_id);
        sty_draw_picture_id(hdc, (int)p->show_pic_prev->pic_id,
                p->resg_directbar.pic_prev->x, p->resg_directbar.pic_prev->y);
        if(NULL != p->show_pic_rotate)
        {
            sty_draw_picture_id(hdc, (int)p->show_pic_rotate->pic_id,
                    p->resg_directbar.pic_r90->x, p->resg_directbar.pic_r90->y);
        }
        sty_draw_picture_id(hdc, (int)p->show_pic_next->pic_id,
                p->resg_directbar.pic_next->x, p->resg_directbar.pic_next->y);
    }
    else
    {
        canvas_hide(p->canvas_directbar_id);
    }
}

void draw_txtbar(scene_playing_t *p, bool show)
{
    int hdc = gui_dc_get((unsigned short)p->canvas_txtbar_id);
    char num_buf[32];

    if (true == show)
    {
        canvas_show(p->canvas_txtbar_id);
        canvas_erase_all(p->canvas_txtbar_id);
        sty_draw_picture_id(hdc, (int)p->resg_txtbar.pic_tbar->pic_id,
                p->resg_txtbar.pic_tbar->x, p->resg_txtbar.pic_tbar->y);

        if(true == p->slideshow)
        {
            sty_draw_picture_id(hdc, (int)p->resg_txtbar.pic_auto->pic_id,
                p->resg_txtbar.pic_auto->x, p->resg_txtbar.pic_auto->y);
        }

        sty_draw_string_res_ext(hdc, p->resg_txtbar.str_name, p->cur_file_info.file_name);

        sprintf(num_buf, "%d/%d", p->cur_file_index + 1, photo_playlist_get_num());
        sty_draw_string_res_ext(hdc, p->resg_txtbar.str_num, num_buf);
    }
    else
    {
        canvas_hide(p->canvas_txtbar_id);
    }
}

void draw_headbar(scene_playing_t *p, bool show)
{
    canvas_info_t info;
    
    if (NULL == p->hbar)
        return;

    if (true == show)
    {
        canvas_show(p->hbar->canvas_id);
        canvas_get_info(p->hbar->canvas_id, &info);
        gui_set_update_region(info.rect.x, info.rect.y, 
                info.rect.x + info.rect.width, 
                info.rect.y + info.rect.height);
    }
    else
    {
        canvas_hide(p->hbar->canvas_id);
    }
}

void draw_common_canvas(scene_playing_t *p)
{
    draw_directbar(p, p->user_opration_status);
    draw_txtbar(p, p->user_opration_status);
    draw_headbar(p, p->user_opration_status);
    if(NULL != p->option)
    {
        menuview_paint(p->option);
    }
}

/*!
 * \brief
 *   特效回调函数，当完成一帧的特效时，调用一次
 *   图标数据跟framefubber进行alpha blending
 * \param[in] framebuffer   framebuffer首地址
 * \param[in] width         framebuffer宽
 * \param[in] height        framebuffer高
 *
 */
static void _fusion_cb(unsigned char *framebuffer, int width, int height)
{
    draw_common_canvas(g_playing);
}

void draw_picture(scene_playing_t *p, fusion_mode_e mode)
{
    int x, y;
    int hdc = gui_dc_get((unsigned short)p->win_id);
    fusion_effect_t eff;

    print_dbg();
    if(DECODE_ERROR == p->cur_file_info.dec_status)
    {
        unsigned long errid;
        print_err("dec_errno:%d", p->cur_file_info.dec_errno);
        if(MMM_ERR_OPEN_FILE == p->cur_file_info.dec_errno)
        {
            errid = STR_FILE_OPEN_ERROR;
        }
        else if(MMM_ERR_FILE_NOT_SUPPORT == p->cur_file_info.dec_errno)
        {
            errid = STR_NOT_SUPPORT;
        }
        else
        {
            errid = STR_DECODE_ERROR;
        }
        
        if(p->option == NULL)   /* 只有option没弹出时才弹对话框 */
        {
            scene_playing_dialog(p, DIALOG_HEAD_FAIL, errid, _error_dialog_cb);
        }
        draw_common_canvas(p);
    }
    else if(DECODE_FINISHED == p->cur_file_info.dec_status)
    {
        eff.mode = mode;
        if(FUSION_ROTATE == mode)
        {
            eff.direct = TRANS_LEFT;
        }
        else
        {
            eff.direct = TRANS_RIGHT;
        }
        fusion_set_effect(&eff);

        /* 设置初始显示效果 */
        if(FUSION_NORMAL != mode)
        {
            fusion_set_first_frame(NULL);
        }
        gui_dc_clear(hdc);
        x = (int)((p->cur_file_info.dec_info.rect_width - p->cur_file_info.dec_info.img_width) >> 1);
        y = (int)((p->cur_file_info.dec_info.rect_height - p->cur_file_info.dec_info.img_height) >> 1);
        gui_dc_draw_bitmap_ext(hdc, p->cur_file_info.dec_info.buf, x, y,
                (int)p->cur_file_info.dec_info.img_width,
                (int)p->cur_file_info.dec_info.img_height,
                (int)p->cur_file_info.dec_info.bpp);

        /* 设置最终显示效果，显示渐进特效 */
        if(FUSION_NORMAL != mode)
        {
            fusion_set_last_frame(NULL);
            fusion_display(_fusion_cb);
        }
        else
        {
            draw_common_canvas(p);
        }
    }
    else
    {
    }
}

