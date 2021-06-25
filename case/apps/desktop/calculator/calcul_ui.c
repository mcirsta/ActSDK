/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : stopcalcul_main.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *     dupengcheng    2009-04-28          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     calclul_ui.c
 * \brief    calculator UI 绘图函数的集合
 * \author   dupengcheng
 *
 * \version 1.0
 * \date  2009/04/14
 *******************************************************************************/
#include "calcul_scene.h"
#include "calcul_ui.h"


/*!\cond CALCULATOR*/
/******************************************************************************/
/*!
 * \par  Description: 显示输入数字 或 计算结果
 * \param[in]   my_scene: 场景结构体
 * \param[in]   str:  要显示的字符串
 * \param[out]  none
 * \return       the result
 * \retval
 * \ingroup      CALCUL_FUNC
 * \par          exmaple code
 * \code
 char pstr = "12345.6789";
 calcul_draw_str(my_scene, pstr);
 * \endcode
 *******************************************************************************/
void calcul_draw_str(scene_calcul_t *my_scene, char *str)
{
    int oldcolor;
    gui_rect_t rect;
    sty_rect_t s_rect;

//    if (str == NULL)
//    {
//        print_err(" string is null ");
//    }

    /* 用来遮蔽的图片的 sty_rect_t */
    s_rect.x = my_scene->str->x;
    s_rect.y = my_scene->str->y;
    s_rect.width = my_scene->str->width;
    s_rect.height = my_scene->str->height;
    sty_draw_sub_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->keyboard_pic->pic_id, 
                            my_scene->str->x, my_scene->str->y, &s_rect);

    /* gui_dc 关于字符串的显示设置 */
    oldcolor = gui_dc_get_color(my_scene->scene_common.handle_gui_dc);
    gui_dc_set_color(my_scene->scene_common.handle_gui_dc, GUI_COLOR_WHITE);
    gui_dc_set_text_mode(my_scene->scene_common.handle_gui_dc, GUI_DRAW_MODE_TRANS);
    gui_dc_set_font_size(my_scene->scene_common.handle_gui_dc, 20);
    gui_dc_set_pen_size(my_scene->scene_common.handle_gui_dc, 2);
    if (str[0] > 57)
    {
        gui_dc_set_font_size(my_scene->scene_common.handle_gui_dc, 15);
        gui_dc_set_pen_size(my_scene->scene_common.handle_gui_dc, 1);        	
    }

    /* gui_dc_display_string_in_rect() 中的画图区域 gui_rect_t */
    rect.x0 = my_scene->str->x;
    rect.y0 = my_scene->str->y + 2;
    rect.x1 = (my_scene->str->x + my_scene->str->width) - 1;
    rect.y1 = (my_scene->str->y + my_scene->str->height) - 1;
    gui_dc_display_string_in_rect(my_scene->scene_common.handle_gui_dc, str, &rect, GUI_ALIGN_RIGHT, GUI_MBCS_ENC);
    gui_dc_set_color(my_scene->scene_common.handle_gui_dc, oldcolor);
}

/*
 wipe key when it is not current key, (not focus key).
 */
void calcul_wipe_key(scene_calcul_t *my_scene, int key)
{
    sty_rect_t s_rect;
    int scenex, sceney;

    if (key == 0)
    {
        return;
    }

    switch (key)
    {
        case 1: // backspace
        s_rect.x = my_scene->pic_backspace->x;
        s_rect.y = my_scene->pic_backspace->y;
        s_rect.width = my_scene->pic_backspace->width;
        s_rect.height = my_scene->pic_backspace->height;
        break;
        case 2: //ce
        s_rect.x = my_scene->pic_ce->x;
        s_rect.y = my_scene->pic_ce->y;
        s_rect.width = my_scene->pic_ce->width;
        s_rect.height = my_scene->pic_ce->height;
        break;
        case 3: //c
        s_rect.x = my_scene->pic_c->x;
        s_rect.y = my_scene->pic_c->y;
        s_rect.width = my_scene->pic_c->width;
        s_rect.height = my_scene->pic_c->height;
        break;
        case 4: //sqrt
        s_rect.x = my_scene->pic_sqrt->x;
        s_rect.y = my_scene->pic_sqrt->y;
        s_rect.width = my_scene->pic_sqrt->width;
        s_rect.height = my_scene->pic_sqrt->height;
        break;
        case 5: //%
        s_rect.x = my_scene->pic_cent->x;
        s_rect.y = my_scene->pic_cent->y;
        s_rect.width = my_scene->pic_cent->width;
        s_rect.height = my_scene->pic_cent->height;
        break;
        case 6: //1/x
        s_rect.x = my_scene->pic_daoshu->x;
        s_rect.y = my_scene->pic_daoshu->y;
        s_rect.width = my_scene->pic_daoshu->width;
        s_rect.height = my_scene->pic_daoshu->height;
        break;
        case 7: //+/-
        s_rect.x = my_scene->pic_sign->x;
        s_rect.y = my_scene->pic_sign->y;
        s_rect.width = my_scene->pic_sign->width;
        s_rect.height = my_scene->pic_sign->height;
        break;
        case 8: // '/'
        s_rect.x = my_scene->pic_div->x;
        s_rect.y = my_scene->pic_div->y;
        s_rect.width = my_scene->pic_div->width;
        s_rect.height = my_scene->pic_div->height;
        break;
        case 9: //mc
        s_rect.x = my_scene->pic_mc->x;
        s_rect.y = my_scene->pic_mc->y;
        s_rect.width = my_scene->pic_mc->width;
        s_rect.height = my_scene->pic_mc->height;
        break;
        case 10: //7
        s_rect.x = my_scene->pic_7->x;
        s_rect.y = my_scene->pic_7->y;
        s_rect.width = my_scene->pic_7->width;
        s_rect.height = my_scene->pic_7->height;
        break;
        case 11: //8
        s_rect.x = my_scene->pic_8->x;
        s_rect.y = my_scene->pic_8->y;
        s_rect.width = my_scene->pic_8->width;
        s_rect.height = my_scene->pic_8->height;
        break;
        case 12: //9
        s_rect.x = my_scene->pic_9->x;
        s_rect.y = my_scene->pic_9->y;
        s_rect.width = my_scene->pic_9->width;
        s_rect.height = my_scene->pic_9->height;
        break;
        case 13: //'*'
        s_rect.x = my_scene->pic_mul->x;
        s_rect.y = my_scene->pic_mul->y;
        s_rect.width = my_scene->pic_mul->width;
        s_rect.height = my_scene->pic_mul->height;
        break;
        case 14: // mr
        s_rect.x = my_scene->pic_mr->x;
        s_rect.y = my_scene->pic_mr->y;
        s_rect.width = my_scene->pic_mr->width;
        s_rect.height = my_scene->pic_mr->height;
        break;
        case 15: //4
        s_rect.x = my_scene->pic_4->x;
        s_rect.y = my_scene->pic_4->y;
        s_rect.width = my_scene->pic_4->width;
        s_rect.height = my_scene->pic_4->height;
        break;
        case 16: //5
        s_rect.x = my_scene->pic_5->x;
        s_rect.y = my_scene->pic_5->y;
        s_rect.width = my_scene->pic_5->width;
        s_rect.height = my_scene->pic_5->height;
        break;
        case 17: //6
        s_rect.x = my_scene->pic_6->x;
        s_rect.y = my_scene->pic_6->y;
        s_rect.width = my_scene->pic_6->width;
        s_rect.height = my_scene->pic_6->height;
        break;
        case 18: //'-'
        s_rect.x = my_scene->pic_sub->x;
        s_rect.y = my_scene->pic_sub->y;
        s_rect.width = my_scene->pic_sub->width;
        s_rect.height = my_scene->pic_sub->height;
        break;
        case 19: // m-
        s_rect.x = my_scene->pic_mminus->x;
        s_rect.y = my_scene->pic_mminus->y;
        s_rect.width = my_scene->pic_mminus->width;
        s_rect.height = my_scene->pic_mminus->height;
        break;
        case 20: //1
        s_rect.x = my_scene->pic_1->x;
        s_rect.y = my_scene->pic_1->y;
        s_rect.width = my_scene->pic_1->width;
        s_rect.height = my_scene->pic_1->height;
        break;
        case 21: //2
        s_rect.x = my_scene->pic_2->x;
        s_rect.y = my_scene->pic_2->y;
        s_rect.width = my_scene->pic_2->width;
        s_rect.height = my_scene->pic_2->height;
        break;
        case 22: //'3
        s_rect.x = my_scene->pic_3->x;
        s_rect.y = my_scene->pic_3->y;
        s_rect.width = my_scene->pic_3->width;
        s_rect.height = my_scene->pic_3->height;
        break;
        case 23: // +
        s_rect.x = my_scene->pic_add->x;
        s_rect.y = my_scene->pic_add->y;
        s_rect.width = my_scene->pic_add->width;
        s_rect.height = my_scene->pic_add->height;
        break;
        case 24: //m+
        s_rect.x = my_scene->pic_mplus->x;
        s_rect.y = my_scene->pic_mplus->y;
        s_rect.width = my_scene->pic_mplus->width;
        s_rect.height = my_scene->pic_mplus->height;
        break;
        case 25: //0
        s_rect.x = my_scene->pic_0->x;
        s_rect.y = my_scene->pic_0->y;
        s_rect.width = my_scene->pic_0->width;
        s_rect.height = my_scene->pic_0->height;
        break;
        case 26: // '.'
        s_rect.x = my_scene->pic_dot->x;
        s_rect.y = my_scene->pic_dot->y;
        s_rect.width = my_scene->pic_dot->width;
        s_rect.height = my_scene->pic_dot->height;
        break;
        case 27: // '='
        s_rect.x = my_scene->pic_equal->x;
        s_rect.y = my_scene->pic_equal->y;
        s_rect.width = my_scene->pic_equal->width;
        s_rect.height = my_scene->pic_equal->height;
        break;

        default:
        return;
    }
    scenex = s_rect.x;
    sceney = s_rect.y;
    //png图片半透明，会叠加，所以擦图时要先显示背景bmp图片
    sty_draw_sub_bitmap(my_scene->scene_common.handle_gui_dc, my_scene->bg_bitmap_info, scenex, sceney, &s_rect);
    //画非焦点时的图片
    sty_draw_sub_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->keyboard_pic->pic_id, 
                            scenex, sceney, &s_rect);
}
/*
 when the key catch focus, draw it
 */
void calcul_draw_key(scene_calcul_t *my_scene, int key)
{
    if (key == 0)
    {
        return;
    }

    switch (key)
    {
        case 1: // backspace
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->pic_backspace->pic_id,
                my_scene->pic_backspace->x, my_scene->pic_backspace->y);
        break;
        case 2: //ce
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->pic_ce->pic_id, 
                my_scene->pic_ce->x, my_scene->pic_ce->y);
        break;
        case 3: //c
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->pic_c->pic_id, 
                my_scene->pic_c->x, my_scene->pic_c->y);
        break;
        case 4: //sqrt
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->pic_sqrt->pic_id, 
                my_scene->pic_sqrt->x, my_scene->pic_sqrt->y);
        break;
        case 5: //%
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->pic_cent->pic_id, 
                my_scene->pic_cent->x, my_scene->pic_cent->y);
        break;
        case 6: //1/x
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->pic_daoshu->pic_id,
                my_scene->pic_daoshu->x, my_scene->pic_daoshu->y);
        break;
        case 7: //+/-
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->pic_sign->pic_id, 
                my_scene->pic_sign->x, my_scene->pic_sign->y);
        break;
        case 8: // '/'
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->pic_div->pic_id, 
                my_scene->pic_div->x, my_scene->pic_div->y);
        break;
        case 9: //mc
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->pic_mc->pic_id, 
                my_scene->pic_mc->x, my_scene->pic_mc->y);
        break;
        case 10: //7
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->pic_7->pic_id, 
                my_scene->pic_7->x, my_scene->pic_7->y);
        break;
        case 11: //8
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->pic_8->pic_id, 
                my_scene->pic_8->x, my_scene->pic_8->y);
        break;
        case 12: //9
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->pic_9->pic_id, 
                my_scene->pic_9->x, my_scene->pic_9->y);
        break;
        case 13: //'*'
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->pic_mul->pic_id, 
                my_scene->pic_mul->x, my_scene->pic_mul->y);
        break;
        case 14: // mr
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->pic_mr->pic_id, 
                my_scene->pic_mr->x, my_scene->pic_mr->y);
        break;
        case 15: //4
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->pic_4->pic_id, 
                my_scene->pic_4->x, my_scene->pic_4->y);
        break;
        case 16: //5
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->pic_5->pic_id, 
                my_scene->pic_5->x, my_scene->pic_5->y);
        break;
        case 17: //6
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->pic_6->pic_id, 
                my_scene->pic_6->x, my_scene->pic_6->y);
        break;
        case 18: //'-'
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->pic_sub->pic_id, 
                my_scene->pic_sub->x, my_scene->pic_sub->y);
        break;
        case 19: // m-
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->pic_mminus->pic_id,
                my_scene->pic_mminus->x, my_scene->pic_mminus->y);
        break;
        case 20: //1
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->pic_1->pic_id, 
                my_scene->pic_1->x, my_scene->pic_1->y);
        break;
        case 21: //2
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->pic_2->pic_id, 
                my_scene->pic_2->x, my_scene->pic_2->y);
        break;
        case 22: //'3
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->pic_3->pic_id, 
                my_scene->pic_3->x, my_scene->pic_3->y);
        break;
        case 23: // +
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->pic_add->pic_id, 
                my_scene->pic_add->x, my_scene->pic_add->y);
        break;
        case 24: //m+
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->pic_mplus->pic_id, 
                my_scene->pic_mplus->x, my_scene->pic_mplus->y);
        break;
        case 25: //0
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->pic_0->pic_id, 
                my_scene->pic_0->x, my_scene->pic_0->y);
        break;
        case 26: // '.'
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->pic_dot->pic_id, 
                my_scene->pic_dot->x, my_scene->pic_dot->y);
        break;
        case 27: // '='
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->pic_equal->pic_id, 
                my_scene->pic_equal->x, my_scene->pic_equal->y);
        break;

        default:
        break;
    }
}

/*
 当键移动时，让当前位置的区域获得焦点
 重画当前和先前的2个区域
 */
void draw_key_when_move(scene_calcul_t *my_scene, int oldkey, int curkey)
{
    calcul_wipe_key(my_scene, oldkey);
    calcul_draw_key(my_scene, curkey);
}

/*
 当 memory 中保存数据时，画出 m flag标记
 */
void calcul_draw_mflag(scene_calcul_t *my_scene)
{
    int oldcolor;
    gui_rect_t rect;

    /* gui_dc 关于字符串的显示设置 */
    oldcolor = gui_dc_get_color(my_scene->scene_common.handle_gui_dc);
    gui_dc_set_color(my_scene->scene_common.handle_gui_dc, GUI_COLOR_WHITE);
    gui_dc_set_text_mode(my_scene->scene_common.handle_gui_dc, GUI_DRAW_MODE_TRANS);
    gui_dc_set_font_size(my_scene->scene_common.handle_gui_dc, 20);
    gui_dc_set_pen_size(my_scene->scene_common.handle_gui_dc, 2);

    rect.x0 = my_scene->pic_mem->x;
    rect.y0 = my_scene->pic_mem->y;
    rect.x1 = (rect.x0 + my_scene->pic_mem->width) - 1;
    rect.y1 = (my_scene->pic_mem->y + my_scene->pic_mem->height) - 1;
    gui_dc_display_string_in_rect(my_scene->scene_common.handle_gui_dc, "M", &rect, GUI_ALIGN_CENTER, GUI_MBCS_ENC);
    gui_dc_set_color(my_scene->scene_common.handle_gui_dc, oldcolor);
}

/*
 当 MC 时，擦除 m flag 标记
 */
void calcul_wipe_mflag(scene_calcul_t *my_scene)
{
    sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->pic_mem->pic_id, 
            my_scene->pic_mem->x, my_scene->pic_mem->y);
}

