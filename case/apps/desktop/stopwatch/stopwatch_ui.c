/*******************************************************************************
 *                              NOYA1100
 *                            Module: video
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 * History:
 *      <author>    <time>           <version >             <desc>
 *     dupengcheng    2009-04-30          1.0             build this file
 *******************************************************************************/
/*!
 * \file     stopwatch_ui.c
 * \brief
 * \author   dupengcheng
 * \par      GENERAL DESCRIPTION:
 *
 * \par      EXTERNALIZED FUNCTIONS:
 *
 * \version 1.0
 * \date  2009/04/30
 *******************************************************************************/

#include "include_case.h"

/*本应用的头文件*/
#include "stopwatch_scene.h"
#include "stopwatch_ctl.h"
#include "stopwatch_ui.h"


struct timeval startTicks;

extern int status;
extern int timerEnabled;
extern int cur_label_idx;
extern watchvalue_t watchs[];
extern watchvalue_t curwatchvalue; //当前值
extern int num_x[];
extern scene_stopwatch_t* g_scene_stopwatch;
extern bool rotate_change_status;


static bool _updateUI(scene_stopwatch_t *my_scene);

//bool timeTostr(watchvalue_t *time, char *pstr);
//bool stopwatch_draw_bg(scene_stopwatch_t *my_scene);
//bool stopwatch_draw_frame(scene_stopwatch_t *my_scene, int fb_id, int frame);
//bool when_key_next_draw(scene_stopwatch_t* my_scene, int old_lable);
//bool draw_watch1_idx(scene_stopwatch_t *my_scene);
//bool draw_cur_watch_time(scene_stopwatch_t *my_scene);
//bool update_watch_head(scene_stopwatch_t *my_scene);
//unsigned int onTimer(void *para);
//////////////////////////////////////////
//void paint_necessary_when_rotate(scene_stopwatch_t *my_scene);
//void get_watch_string_rect(int idx, gui_rect_t *rect);
//void get_watch_idx_string_rect(int idx, gui_rect_t *rect);
//bool draw_watch_time(int watch_idx, int position, int color);
//bool draw_watch_idx(int watch_idx, int position, int color);
//bool wipe_watch_string_area_to_high_color(scene_stopwatch_t *my_scene, int idx);
//bool timer_draw_cur_watch_time(scene_stopwatch_t *my_scene);

//=============================================================================================================
/*
 ***得到某个watch的string的gui_rect_t区域
 */
void get_watch_string_rect(int idx, gui_rect_t *rect)
{
    scene_stopwatch_t *my_scene = g_scene_stopwatch;

    switch (idx)
    {
        case 0:
        rect->x0 = my_scene->watch1.str_res->x;
        rect->y0 = my_scene->watch1.str_res->y;
        rect->x1 = (my_scene->watch1.str_res->x + my_scene->watch1.str_res->width) - 1;
        rect->y1 = (my_scene->watch1.str_res->y + my_scene->watch1.str_res->height) - 1;
        break;

        case 1:
        rect->x0 = my_scene->watch2.str_res->x;
        rect->y0 = my_scene->watch2.str_res->y;
        rect->x1 = (my_scene->watch2.str_res->x + my_scene->watch2.str_res->width) - 1;
        rect->y1 = (my_scene->watch2.str_res->y + my_scene->watch2.str_res->height) - 1;
        break;

        case 2:
        rect->x0 = my_scene->watch3.str_res->x;
        rect->y0 = my_scene->watch3.str_res->y;
        rect->x1 = (my_scene->watch3.str_res->x + my_scene->watch3.str_res->width) - 1;
        rect->y1 = (my_scene->watch3.str_res->y + my_scene->watch3.str_res->height) - 1;
        break;

        case 3:
        rect->x0 = my_scene->watch4.str_res->x;
        rect->y0 = my_scene->watch4.str_res->y;
        rect->x1 = (my_scene->watch4.str_res->x + my_scene->watch4.str_res->width) - 1;
        rect->y1 = (my_scene->watch4.str_res->y + my_scene->watch4.str_res->height) - 1;
        break;

        case 4:
        rect->x0 = my_scene->watch5.str_res->x;
        rect->y0 = my_scene->watch5.str_res->y;
        rect->x1 = (my_scene->watch5.str_res->x + my_scene->watch5.str_res->width) - 1;
        rect->y1 = (my_scene->watch5.str_res->y + my_scene->watch5.str_res->height) - 1;
        break;

        case 5:
        rect->x0 = my_scene->watch6.str_res->x;
        rect->y0 = my_scene->watch6.str_res->y;
        rect->x1 = (my_scene->watch6.str_res->x + my_scene->watch6.str_res->width) - 1;
        rect->y1 = (my_scene->watch6.str_res->y + my_scene->watch6.str_res->height) - 1;
        break;

        default:
        break;
    }
}

/*
 ***得到某个watch_idx的字符串的gui_rect_t区域
 */
void get_watch_idx_string_rect(int idx, gui_rect_t *rect)
{
    scene_stopwatch_t *my_scene = g_scene_stopwatch;

    switch (idx)
    {
        case 0:
        rect->x0 = my_scene->watch1.str_id_res->x;
        rect->y0 = my_scene->watch1.str_id_res->y;
        rect->x1 = (my_scene->watch1.str_id_res->x + my_scene->watch1.str_id_res->width) - 1;
        rect->y1 = (my_scene->watch1.str_id_res->y + my_scene->watch1.str_id_res->height) - 1;
        break;

        case 1:
        rect->x0 = my_scene->watch2.str_id_res->x;
        rect->y0 = my_scene->watch2.str_id_res->y;
        rect->x1 = (my_scene->watch2.str_id_res->x + my_scene->watch2.str_id_res->width) - 1;
        rect->y1 = (my_scene->watch2.str_id_res->y + my_scene->watch2.str_id_res->height) - 1;
        break;

        case 2:
        rect->x0 = my_scene->watch3.str_id_res->x;
        rect->y0 = my_scene->watch3.str_id_res->y;
        rect->x1 = (my_scene->watch3.str_id_res->x + my_scene->watch3.str_id_res->width) - 1;
        rect->y1 = (my_scene->watch3.str_id_res->y + my_scene->watch3.str_id_res->height) - 1;
        break;

        case 3:
        rect->x0 = my_scene->watch4.str_id_res->x;
        rect->y0 = my_scene->watch4.str_id_res->y;
        rect->x1 = (my_scene->watch4.str_id_res->x + my_scene->watch4.str_id_res->width) - 1;
        rect->y1 = (my_scene->watch4.str_id_res->y + my_scene->watch4.str_id_res->height) - 1;
        break;

        case 4:
        rect->x0 = my_scene->watch5.str_id_res->x;
        rect->y0 = my_scene->watch5.str_id_res->y;
        rect->x1 = (my_scene->watch5.str_id_res->x + my_scene->watch5.str_id_res->width) - 1;
        rect->y1 = (my_scene->watch5.str_id_res->y + my_scene->watch5.str_id_res->height) - 1;
        break;

        case 5:
        rect->x0 = my_scene->watch6.str_id_res->x;
        rect->y0 = my_scene->watch6.str_id_res->y;
        rect->x1 = (my_scene->watch6.str_id_res->x + my_scene->watch6.str_id_res->width) - 1;
        rect->y1 = (my_scene->watch6.str_id_res->y + my_scene->watch6.str_id_res->height) - 1;
        break;

        default:
        break;
    }
}

/*
 **	在指定的位置画某个 watch的时间
 */
bool draw_watch_time(int watch_idx, int position, int color)
{
    char str[60];
    int oldcolor;
    gui_rect_t rect;

    scene_stopwatch_t *my_scene = g_scene_stopwatch;

    timeTostr(&(watchs[watch_idx]), str);
    if (str == NULL)
    {
        print_err(" string is null ");
    }

    /* gui_dc 关于字符串的显示设置 */
    oldcolor = gui_dc_get_color(my_scene->scene_common.handle_gui_dc);
    gui_dc_set_color(my_scene->scene_common.handle_gui_dc, color);
    gui_dc_set_text_mode(my_scene->scene_common.handle_gui_dc, GUI_DRAW_MODE_TRANS);
    gui_dc_set_font_size(my_scene->scene_common.handle_gui_dc, 20);
    gui_dc_set_pen_size(my_scene->scene_common.handle_gui_dc, 2);

    get_watch_string_rect(position, &rect);

    gui_dc_display_string_in_rect(my_scene->scene_common.handle_gui_dc, str, &rect, GUI_ALIGN_LEFT, GUI_MBCS_ENC);
    gui_dc_set_color(my_scene->scene_common.handle_gui_dc, oldcolor);

    return true;
}

/*
 **	在指定的位置画某个 watch的index
 */
bool draw_watch_idx(int watch_idx, int position, int color)
{
    char str[2];
    int oldcolor;
    gui_rect_t rect;

    scene_stopwatch_t *my_scene = g_scene_stopwatch;

    /* gui_dc 关于字符串的显示设置 */
    oldcolor = gui_dc_get_color(my_scene->scene_common.handle_gui_dc);
    gui_dc_set_color(my_scene->scene_common.handle_gui_dc, color);//GUI_COLOR_BLACK
    gui_dc_set_text_mode(my_scene->scene_common.handle_gui_dc, GUI_DRAW_MODE_TRANS);
    gui_dc_set_font_size(my_scene->scene_common.handle_gui_dc, 20);
    gui_dc_set_pen_size(my_scene->scene_common.handle_gui_dc, 2);

    get_watch_idx_string_rect(position, &rect);
    str[0] = (char)(watch_idx + 48); //ascii[0]=48
    str[1] = '\0';

    gui_dc_display_string_in_rect(my_scene->scene_common.handle_gui_dc, str, &rect, GUI_ALIGN_LEFT, GUI_MBCS_ENC);
    gui_dc_set_color(my_scene->scene_common.handle_gui_dc, oldcolor);

    return true;
}

/*
 ** rotate 时候画一些必要的图片,
 ** 包括: 背景、数字图片后面的bar、':'、'.'和 watch[i]的bar
 */
void paint_necessary_when_rotate(scene_stopwatch_t *my_scene)
{
    /* draw static main window bg */
    stopwatch_draw_bg(my_scene);
    //数字图片后面的bar
    sty_draw_picture_res(my_scene->scene_common.handle_gui_dc, my_scene->show_area);

    /* watch[i]的bar */
    stopwatch_draw_frame(my_scene, 0, 0);
    stopwatch_draw_frame(my_scene, 1, 0);
    stopwatch_draw_frame(my_scene, 2, 0);
    stopwatch_draw_frame(my_scene, 3, 0);
    if (is_upright_disp() == true)
    {
        stopwatch_draw_frame(my_scene, 4, 0);
        stopwatch_draw_frame(my_scene, 5, 0);
    }

    // ':' and '.'
    sty_draw_picture_res(my_scene->scene_common.handle_gui_dc, my_scene->dot2_pic);
    sty_draw_picture_res(my_scene->scene_common.handle_gui_dc, my_scene->dot2_2_pic);
    sty_draw_picture_res(my_scene->scene_common.handle_gui_dc, my_scene->dot1_pic);
}

//=============================================================================================================
/******************************************************************************/
/*!
 * \par  Description:
 *        取得时间的字符串
 * \param[in]    watchvalue_t *: watch结构体指针
 * \param[in]    char *: 存放结构的串
 * \retval       void
 * \ingroup      stopwatch
 *******************************************************************************/
bool timeTostr(watchvalue_t *time, char *pstr)
{
    int hour;
    int minute;
    int second;
    int hundredsecond;

    hour = time->hour;
    minute = time->minutes;
    second = time->seconds;
    hundredsecond = time->hundredSeconds;
    if (hundredsecond > 0)
    {
        hundredsecond /= 10;
    }

    sprintf(pstr, "%02d : %02d : %02d.%02d", hour, minute, second, hundredsecond);
    return true;
}

/******************************************************************************/
/*!
 * \par  Description:*      draw back ground pictrue
 * \param[in]    video_scene_play_t *: 控件指针
 * \retval       void
 * \ingroup      stopwatch
 *******************************************************************************/
bool stopwatch_draw_bg(scene_stopwatch_t *my_scene)
{
    //sty_draw_picture_res( my_scene->scene_common.handle_gui_dc, my_scene->bg_pic);
    sty_draw_bitmap(my_scene->scene_common.handle_gui_dc, my_scene->bg_bitmap_info, 0, 0);
    return true;
}

/******************************************************************************/
/*!
 * \par  Description: 也可以当中擦除某个 watch的计时 来使用
 *       画watch的图片组的其中一帧，每组有两帧：黑色或黄色
 * \param[in]    my_scene *: scene_stopwatch_t指针
 * \param[int]   fb_id: scene成员，即需要draw的frame的编号
 * \param[in]    int: 显示的帧号. 0[],1[高亮黄色]
 * \retval       void
 * \ingroup      stopwatch
 *******************************************************************************/
bool stopwatch_draw_frame(scene_stopwatch_t *my_scene, int fb_id, int frame)
{
    picregion_frame_t picregion_frame;
    sty_rect_t s_rect;
    int x, y;
    
    //png图片半透明，会叠加，所以擦图时要先显示背景bmp图片
    if ( frame == 0)
    {
        x = my_scene->watch1.pic_res->x;
        s_rect.x = (short)x;
        s_rect.width = my_scene->watch1.pic_res->width;
        s_rect.height = my_scene->watch1.pic_res->height;
    
        switch (fb_id)
        {  
            case 0:
            y = my_scene->watch1.pic_res->y; 
            s_rect.y = (short)y;        
            break;
            case 1:
            y = my_scene->watch2.pic_res->y; 
            s_rect.y = (short)y;        
            break;
            case 2:
            y = my_scene->watch3.pic_res->y; 
            s_rect.y = (short)y;        
            break;
            case 3:
            y = my_scene->watch4.pic_res->y; 
            s_rect.y = (short)y;        
            break;
            case 4:
            y = my_scene->watch5.pic_res->y; 
            s_rect.y = (short)y;        
            break;
            case 5:
            y = my_scene->watch6.pic_res->y; 
            s_rect.y = (short)y;        
            break;
            default:
            break;                            
        }  
        sty_draw_sub_bitmap(my_scene->scene_common.handle_gui_dc, my_scene->bg_bitmap_info, x, y, &s_rect);
        sty_draw_sub_picture_id(my_scene->scene_common.handle_gui_dc, 
                             (int)my_scene->show_area->pic_id, x, y, &s_rect);  
    }
                             

    switch (fb_id)
    {
        case 0:
        get_picregion_frame((picregion_resource_t*)my_scene->watch1.pic_res, 
                            (unsigned short)frame, &picregion_frame);
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)picregion_frame.pic_id, 
                            my_scene->watch1.pic_res->x, my_scene->watch1.pic_res->y);
        break;
        case 1:
        get_picregion_frame((picregion_resource_t*)my_scene->watch2.pic_res, 
                            (unsigned short)frame, &picregion_frame);
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)picregion_frame.pic_id, 
                            my_scene->watch2.pic_res->x, my_scene->watch2.pic_res->y);
        break;
        case 2:
        get_picregion_frame((picregion_resource_t*)my_scene->watch3.pic_res, 
                            (unsigned short)frame, &picregion_frame);
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)picregion_frame.pic_id, 
                            my_scene->watch3.pic_res->x, my_scene->watch3.pic_res->y);
        break;
        case 3:
        get_picregion_frame((picregion_resource_t*)my_scene->watch4.pic_res, 
                            (unsigned short)frame, &picregion_frame);
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)picregion_frame.pic_id, 
                            my_scene->watch4.pic_res->x, my_scene->watch4.pic_res->y);
        break;
        case 4:
        get_picregion_frame((picregion_resource_t*)my_scene->watch5.pic_res, 
                            (unsigned short)frame, &picregion_frame);
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)picregion_frame.pic_id, 
                            my_scene->watch5.pic_res->x, my_scene->watch5.pic_res->y);
        break;
        case 5:
        get_picregion_frame((picregion_resource_t*)my_scene->watch6.pic_res, 
                            (unsigned short)frame, &picregion_frame);
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)picregion_frame.pic_id, 
                            my_scene->watch6.pic_res->x, my_scene->watch6.pic_res->y);
        break;

        default:
        break;
    }
    return true;
}

/*
 ** 擦去某个watch在屏幕上显示的string,擦去后显示高亮色
 ** index:哪个watch
 */
bool wipe_watch_string_area_to_high_color(scene_stopwatch_t *my_scene, int idx)
{
    sty_rect_t s_rect;
    int x_offset;
    int begin_x=0, begin_y=0; //显示区域左上角在屏幕上中的绝对坐标
    picregion_frame_t picregion_frame;

    x_offset = 30;

    s_rect.x = (short)x_offset;
    s_rect.y = 0;
    s_rect.width = (short)(my_scene->cover_pic->width - x_offset);
    s_rect.height = my_scene->cover_pic->height;

    get_picregion_frame((picregion_resource_t*)my_scene->watch1.pic_res, 0, &picregion_frame);
    begin_x = my_scene->watch1.pic_res->x + x_offset;

    switch (idx)
    {
        case 0:
        begin_y = my_scene->watch1.pic_res->y;
        break;
        case 1:
        begin_y = my_scene->watch2.pic_res->y;
        break;
        case 2:
        begin_y = my_scene->watch3.pic_res->y;
        break;
        case 3:
        begin_y = my_scene->watch4.pic_res->y;
        break;
        case 4:
        begin_y = my_scene->watch5.pic_res->y;
        break;
        case 5:
        begin_y = my_scene->watch6.pic_res->y;
        break;

        default:
        break;
    }
    sty_draw_sub_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->cover_pic->pic_id, 
                            begin_x, begin_y, &s_rect);
    return true;
}

/******************************************************************************/
/*!
 * \par  Description:
 *       当按下 key_next 键时的 绘图
 * \param[in]    scene_stopwatch_t *: 场景结构指针
 * \param[in]    int: 按键之前的焦点watch的索引，
 * \retval       void
 * \ingroup      stopwatch
 *******************************************************************************/
bool when_key_next_draw(scene_stopwatch_t* my_scene, int old_lable)
{
    int color;
    int focus_color;

    color = (int)g_scene_stopwatch->watch1.str_res->background;
    focus_color = (int)g_scene_stopwatch->watch1.str_res->foreground;

    if (is_upright_disp() == true) //竖屏
    {
        /*当前watch高亮*/
        draw_cur_watch_time(g_scene_stopwatch);

        //擦去old watch的字符和编号
        stopwatch_draw_frame(g_scene_stopwatch, old_lable, 0);
        //显示old watch编号
        draw_watch_idx(old_lable, old_lable, color);
        //显示old watch计时字符串
        draw_watch_time(old_lable, old_lable, color);
    }
    else //横屏
    {
        if (cur_label_idx > 3)
        {
            stopwatch_draw_frame(g_scene_stopwatch, 0, 0);
            draw_watch_idx(cur_label_idx - 3, 0, color);
            draw_watch_time(cur_label_idx - 3, 0, color);

            stopwatch_draw_frame(g_scene_stopwatch, 1, 0);
            draw_watch_idx(cur_label_idx - 2, 1, color);
            draw_watch_time(cur_label_idx - 2, 1, color);

            stopwatch_draw_frame(g_scene_stopwatch, 2, 0);
            draw_watch_idx(old_lable, 2, color);
            draw_watch_time(old_lable, 2, color);

            stopwatch_draw_frame(g_scene_stopwatch, 3, 1); //当前 焦点 高亮
            draw_watch_idx(cur_label_idx, 3, focus_color);
            draw_watch_time(cur_label_idx, 3, focus_color);
        }
        else if (cur_label_idx == 0)
        {
            stopwatch_draw_frame(g_scene_stopwatch, 0, 1);//当前 焦点 高亮
            draw_watch_idx(0, 0, focus_color);
            draw_watch_time(0, 0, focus_color);

            if(watchs[1].hundredSeconds > 0)
            {
                stopwatch_draw_frame(g_scene_stopwatch, 1, 0);
                draw_watch_idx(1, 1, color);
                draw_watch_time(1, 1, color);
            }
            
            if(watchs[2].hundredSeconds > 0)
            {
                stopwatch_draw_frame(g_scene_stopwatch, 2, 0);
                draw_watch_idx(2, 2, color);
                draw_watch_time(2, 2, color);
            }
            
            if(watchs[3].hundredSeconds > 0)
            {
                stopwatch_draw_frame(g_scene_stopwatch, 3, 0);
                draw_watch_idx(3, 3, color);
                draw_watch_time(3, 3, color);
            }
        }
        else //1,2,3
        {
            /*当前watch高亮*/
            draw_cur_watch_time(g_scene_stopwatch);

            //擦去old watch的字符和编号
            stopwatch_draw_frame(g_scene_stopwatch, old_lable, 0);
            draw_watch_idx(old_lable, old_lable, color);
            draw_watch_time(old_lable, old_lable, color);
        }
    }

    return true;
}

/******************************************************************************/
/*!
 * \par  Description:
 *       特殊用途，当进入应用后，第一次计时，watch1的索引数字应该是黑色的
 * \param[in]    scene_stopwatch_t *: 场景结构指针
 * \retval       void
 * \ingroup      stopwatch
 *******************************************************************************/
bool draw_watch1_idx(scene_stopwatch_t *my_scene)
{
    int oldcolor;
    gui_rect_t rect;
    int color;
    int focus_color;

    color = (int)g_scene_stopwatch->watch1.str_res->background;
    focus_color = (int)g_scene_stopwatch->watch1.str_res->foreground;

    oldcolor = gui_dc_get_color(my_scene->scene_common.handle_gui_dc);
    gui_dc_set_color(my_scene->scene_common.handle_gui_dc, focus_color);
    gui_dc_set_text_mode(my_scene->scene_common.handle_gui_dc, GUI_DRAW_MODE_TRANS);
    gui_dc_set_font_size(my_scene->scene_common.handle_gui_dc, 20);
    gui_dc_set_pen_size(my_scene->scene_common.handle_gui_dc, 2);

    rect.x0 = my_scene->watch1.str_id_res->x;
    rect.y0 = my_scene->watch1.str_id_res->y;
    rect.x1 = (my_scene->watch1.str_id_res->x + my_scene->watch1.str_id_res->width) - 1;
    rect.y1 = (my_scene->watch1.str_id_res->y + my_scene->watch1.str_id_res->height) - 1;
    gui_dc_display_string_in_rect(my_scene->scene_common.handle_gui_dc, "0", &rect, GUI_ALIGN_CENTER, GUI_MBCS_ENC);

    gui_dc_set_color(my_scene->scene_common.handle_gui_dc, oldcolor);

    return true;
}

/******************************************************************************/
/*!
 * \par  Description:
 *        Timer（定时器） 中更新 当前 watch 的值
 * \param[in]    scene_stopwatch *: 场景结构体指针
 * \retval       void
 * \ingroup      stopwatch
 *******************************************************************************/
bool draw_cur_watch_time(scene_stopwatch_t *my_scene)
{
    int color;
    int focus_color;

    color = (int)g_scene_stopwatch->watch1.str_res->background;
    focus_color = (int)g_scene_stopwatch->watch1.str_res->foreground;

    if (is_upright_disp() == true) //竖屏
    {
        //擦去当前watch的字符和编号
        stopwatch_draw_frame(g_scene_stopwatch, cur_label_idx, 1);
        //显示编号
        draw_watch_idx(cur_label_idx, cur_label_idx, focus_color);
        //显示计时字符串
        draw_watch_time(cur_label_idx, cur_label_idx, focus_color);
    }
    else //横屏
    {
        if (cur_label_idx > 3)
        {
            //擦去当前watch的字符和编号
            stopwatch_draw_frame(g_scene_stopwatch, 3, 1);
            //显示编号
            draw_watch_idx(cur_label_idx, 3, focus_color);
            //显示计时字符串
            draw_watch_time(cur_label_idx, 3, focus_color);
        }
        else if (cur_label_idx == 0)
        {
            //擦去当前watch的字符和编号
            stopwatch_draw_frame(g_scene_stopwatch, 0, 1);
            //显示编号
            draw_watch_idx(cur_label_idx, 0, focus_color);
            //显示计时字符串
            draw_watch_time(cur_label_idx, 0, focus_color);
        }
        else
        {
            //擦去当前watch的字符和编号
            stopwatch_draw_frame(g_scene_stopwatch, cur_label_idx, 1);
            //显示编号
            draw_watch_idx(cur_label_idx, cur_label_idx, focus_color);
            //显示计时字符串
            draw_watch_time(cur_label_idx, cur_label_idx, focus_color);

        }
    }

    return true;
}

// 大体和上个函数相同，只是在timer里不断显示当前时间
bool timer_draw_cur_watch_time(scene_stopwatch_t *my_scene)
{
    int color;
    int focus_color;

    color = (int)g_scene_stopwatch->watch1.str_res->background;
    focus_color = (int)g_scene_stopwatch->watch1.str_res->foreground;

    if (is_upright_disp() == true) //竖屏
    {
        //擦去当前watch的字符区域
        wipe_watch_string_area_to_high_color(g_scene_stopwatch, cur_label_idx);
        //显示计时字符串
        draw_watch_time(cur_label_idx, cur_label_idx, focus_color);
    }
    else //横屏
    {
        if (cur_label_idx > 3)
        {
            //擦去当前watch的字符区域
            wipe_watch_string_area_to_high_color(g_scene_stopwatch, 3);
            //显示计时字符串
            draw_watch_time(cur_label_idx, 3, focus_color);
        }
        else if (cur_label_idx == 0)
        {
            //擦去当前watch的字符区域
            wipe_watch_string_area_to_high_color(g_scene_stopwatch, 0);
            //显示计时字符串
            draw_watch_time(cur_label_idx, 0, focus_color);
        }
        else
        {
            //擦去当前watch的字符区域
            wipe_watch_string_area_to_high_color(g_scene_stopwatch, cur_label_idx);
            //显示计时字符串
            draw_watch_time(cur_label_idx, cur_label_idx, focus_color);
        }
    }

    return true;
}

/******************************************************************************/
/*!
 * \par  Description:
 *        Timer 中更新watch的图片区，headbar下面的那个区域
 * \param[in]    scene_stopwatch *: 场景结构体指针
 * \retval       void
 * \ingroup      stopwatch
 *******************************************************************************/
bool update_watch_head(scene_stopwatch_t *my_scene)
{
    picregion_frame_t picregion_frame;
    int msecond;
    static int oldhour = 0;
    static int oldminute = 0;
    static int oldsecond = 0;
    sty_rect_t s_rect;
    int num_y;

    /* 取得数字的Y坐标 */
    get_picregion_frame((picregion_resource_t*)my_scene->num_pic, 0, &picregion_frame);
    num_y = picregion_frame.y;

    s_rect.y = (short)(num_y - my_scene->show_area->y);
    s_rect.width = (short)((num_x[1] - num_x[0]) + my_scene->num_pic->width);
    s_rect.height = my_scene->num_pic->height;

    /*当切换横竖屏之后第一次需要重新画数字图片*/
    // draw hour
    if ((curwatchvalue.hour != oldhour) || (rotate_change_status == true))
    {
        s_rect.x = (short)(num_x[0] - my_scene->show_area->x);
        sty_draw_sub_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->show_area->pic_id, 
                                num_x[0], num_y, &s_rect);

        get_picregion_frame((picregion_resource_t*)my_scene->num_pic, (unsigned short)(curwatchvalue.hour / 10), 
                            &picregion_frame); //hour-2
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)picregion_frame.pic_id, num_x[0], num_y);
        get_picregion_frame((picregion_resource_t*)my_scene->num_pic, (unsigned short)(curwatchvalue.hour % 10), 
                            &picregion_frame); //hour-2
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)picregion_frame.pic_id, num_x[1], num_y);
    }
    // draw minute
    if ((curwatchvalue.minutes != oldminute) || (rotate_change_status == true))
    {
        s_rect.x = (short)(num_x[2] - my_scene->show_area->x);
        sty_draw_sub_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->show_area->pic_id, 
                                num_x[2], num_y, &s_rect);

        get_picregion_frame((picregion_resource_t*)my_scene->num_pic,(unsigned short)(curwatchvalue.minutes/10),
                            &picregion_frame); //minute-1
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc,(int)picregion_frame.pic_id, num_x[2], num_y);
        get_picregion_frame((picregion_resource_t*)my_scene->num_pic,(unsigned short)(curwatchvalue.minutes%10),
                            &picregion_frame); //minute-2
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)picregion_frame.pic_id, num_x[3], num_y);
    }
    // draw second
    if ((curwatchvalue.seconds != oldsecond) || (rotate_change_status == true))
    {
        s_rect.x = (short)(num_x[4] - my_scene->show_area->x);
        sty_draw_sub_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->show_area->pic_id, 
                                num_x[4], num_y, &s_rect);

        get_picregion_frame((picregion_resource_t*)my_scene->num_pic,(unsigned short)(curwatchvalue.seconds/10),
                            &picregion_frame);//second-1
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)picregion_frame.pic_id, num_x[4], num_y);
        get_picregion_frame((picregion_resource_t*)my_scene->num_pic,(unsigned short)(curwatchvalue.seconds%10),
                            &picregion_frame);//second-2
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)picregion_frame.pic_id, num_x[5], num_y);
    }

    // draw hundredSeconds
    s_rect.x = (short)(num_x[6] - my_scene->show_area->x);
    sty_draw_sub_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->show_area->pic_id, 
                            num_x[6], num_y, &s_rect);

    msecond = (curwatchvalue.hundredSeconds == 0) ? 0 : (curwatchvalue.hundredSeconds/10);
    get_picregion_frame((picregion_resource_t*)my_scene->num_pic, (unsigned short)(msecond / 10), 
                        &picregion_frame); //hundredSeconds-1
    sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)picregion_frame.pic_id, num_x[6], num_y);
    get_picregion_frame((picregion_resource_t*)my_scene->num_pic, (unsigned short)(msecond % 10), 
                        &picregion_frame); //hundredSeconds-2
    sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)picregion_frame.pic_id, num_x[7], num_y);

    oldhour = curwatchvalue.hour;
    oldminute = curwatchvalue.minutes;
    oldsecond = curwatchvalue.seconds;
    rotate_change_status = false;

    return true;
}

/******************************************************************************/
/*!
 * \par  Description:
 *        更新UI界面
 * \param[in]    scene_stopwatch *: 场景结构体指针
 * \retval       void
 * \ingroup      stopwatch
 *******************************************************************************/
static bool _updateUI(scene_stopwatch_t *my_scene)
{
    struct timeval tmpticks;
    int second;

    if (status == STATE_STOP)
    {
        return true;
    }
    if (status == STATE_RUNNING)
    {
        do_gettimeofday(&tmpticks);
        second = tmpticks.tv_sec - startTicks.tv_sec;

        startTicks = tmpticks;
        if (tmpticks.tv_usec > 0)
        {
            curwatchvalue.hundredSeconds = tmpticks.tv_usec / 1000;
        }
        second += (((curwatchvalue.hour * 3600) + (curwatchvalue.minutes * 60)) + curwatchvalue.seconds);
        curwatchvalue.hour = second / 3600;
        
        //如果大于60分钟，则计时不再变化
        if(curwatchvalue.hour >= 1)
        {         
            curwatchvalue.minutes = 0;
            curwatchvalue.seconds = 0;
            curwatchvalue.hundredSeconds = 0;	
        }
        else
        {
            curwatchvalue.minutes = ((second / 60) - (curwatchvalue.hour * 60));
            curwatchvalue.seconds = second % 60;
        }
        
        watchs[cur_label_idx] = curwatchvalue;

        //更新当前watch
        timer_draw_cur_watch_time(my_scene);

        //更新顶部时间
        update_watch_head(my_scene);
    }
    return true;
}

/******************************************************************************/
/*!
 * \par  Description:
 *        Timer定时器
 * \param[in]    void *para
 * \retval       void
 * \ingroup      stopwatch
 *******************************************************************************/
unsigned int onTimer(void *para)
{
    scene_stopwatch_t *my_scene = (scene_stopwatch_t *) para;

    if (timerEnabled == 0)
    {
        return 0;
    }
    if (curwatchvalue.hour > 0)
    {
        return 0;
    }
    _updateUI(my_scene);

    return 0;
}

