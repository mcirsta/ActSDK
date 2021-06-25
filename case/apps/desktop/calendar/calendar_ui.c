/*
 ********************************************************************************
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
 * \file     calendar_ui.c
 * \brief    calendar UI 绘图函数
 * \author   dupengcheng
 *
 * \version 1.0
 * \date  2009/04/14
 *******************************************************************************/
#include "calendar_scene.h"
#include "calendar_play.h"
#include "date.h"
#include "calendar_ui.h"


extern int yearVar;
extern int monthVar;
extern int dayVar;
extern int curRow;
extern int curCol;
extern int focusFlag;
extern int paintFlag;
extern unsigned int jd;
extern const int dateTabX[];
extern const int dateTabY[];
extern const char *monChar[];

static char lunar_buffer[60]; /* 显示date info的字符串 */



/*
 ******************* 以下是和画图相关函数 *****************
 ** The following routines used to draw information string
 ** and the pictures of solar or lunar date.
 **********************************************************
 */

/*
 在指定的位置，显示一个字符串，包括颜色，大小，对齐 等风格。
 */
void drawStrAnyWhere(scene_calendar_t *my_scene, char *str, gui_rect_t *rect, int size, int color, int align)
{
    gui_rect_t rt;
    int off_y,off_x;  //各种风格的日历表格大小不一，要确保日期显示在表格的中间
    
    off_x = ((int)my_scene->shield_pic->width - 20)/2;
    off_y = ((int)my_scene->shield_pic->height - 20)/2;

    if (str != NULL)
    {
        //rt.x0 = rect->x0;
        rt.x0 = rect->x0 + (short)off_x;
        //rt.y0 = rect->y0;// + 6; //因为在rect区域内字体显示偏靠顶部，所以手动下移
        rt.y0 = (rect->y0 + (short)off_y) - 1;

        //rt.x1 = rect->x1 - 1; 
        rt.x1 = (rect->x1 - (short)off_x) - 1;        
        //rt.y1 = rect->y1 - 1;
        rt.y1 = (rect->y1 - (short)off_y) - 1;

        //gui_dc_set_pen_size(my_scene->scene_common.handle_gui_dc, 1);
        gui_dc_set_text_mode(my_scene->scene_common.handle_gui_dc, GUI_DRAW_MODE_TRANS);
        gui_dc_set_font_size(my_scene->scene_common.handle_gui_dc, size);
        gui_dc_set_color(my_scene->scene_common.handle_gui_dc, color);

        gui_dc_display_string_in_rect(my_scene->scene_common.handle_gui_dc, str, &rt, align, GUI_MBCS_ENC);
    }
}

/*
 擦除指定区域图片，并填充背景图片
 (x,y) : 相对于屏幕的起始位置
 pic_id: 用来填充的背景图片的 id
 sty_rect_t *rect: 用来填充的图片是整张图片的局部区域(x0,y0,width,height)
 */
void wipe_pic(scene_calendar_t *my_scene, int x, int y, int pic_id, sty_rect_t *rect)
{
    sty_rect_t s_rect;

    s_rect.x = rect->x;
    s_rect.y = rect->y;
    s_rect.width = rect->width;
    s_rect.height = rect->height;
    sty_draw_sub_picture_id(my_scene->scene_common.handle_gui_dc, pic_id, x, y, &s_rect);
}

/*
 在指定的位置显示一幅黄色数字[0~9]图片, yellow number,yellow number
 */
void drawYellowNumPic(scene_calendar_t *my_scene, int number, int x, int y)
{
    picregion_frame_t picregion_frame;
    if ((number > 9) || (number < 0))
    {
        //print_err("number must be between 0 and 9!");
        return;
    }

    switch (number)
    {
        case 0:
        get_picregion_frame(my_scene->numArryY_pic, (unsigned short)number, &picregion_frame);
        break;
        case 1:
        get_picregion_frame(my_scene->numArryY_pic, (unsigned short)number, &picregion_frame);
        break;
        case 2:
        get_picregion_frame(my_scene->numArryY_pic, (unsigned short)number, &picregion_frame);
        break;
        case 3:
        get_picregion_frame(my_scene->numArryY_pic, (unsigned short)number, &picregion_frame);
        break;
        case 4:
        get_picregion_frame(my_scene->numArryY_pic, (unsigned short)number, &picregion_frame);
        break;
        case 5:
        get_picregion_frame(my_scene->numArryY_pic, (unsigned short)number, &picregion_frame);
        break;
        case 6:
        get_picregion_frame(my_scene->numArryY_pic, (unsigned short)number, &picregion_frame);
        break;
        case 7:
        get_picregion_frame(my_scene->numArryY_pic, (unsigned short)number, &picregion_frame);
        break;
        case 8:
        get_picregion_frame(my_scene->numArryY_pic, (unsigned short)number, &picregion_frame);
        break;
        case 9:
        get_picregion_frame(my_scene->numArryY_pic, (unsigned short)number, &picregion_frame);
        break;
        default:
        break;
    }
    sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)picregion_frame.pic_id, x, y);
}

/*
 在指定的位置显示一幅白色数字[0~9]图片, white number,white number
 */
void drawWhiteNumPic(scene_calendar_t *my_scene, int number, int x, int y)
{
    picregion_frame_t picregion_frame;
    if ((number > 9) || (number < 0))
    {
        //print_err("number must be between 0 and 9!");
        return;
    }

    switch (number)
    {
        case 0:
        get_picregion_frame(my_scene->numArryW_pic, (unsigned short)number, &picregion_frame);
        break;
        case 1:
        get_picregion_frame(my_scene->numArryW_pic, (unsigned short)number, &picregion_frame);
        break;
        case 2:
        get_picregion_frame(my_scene->numArryW_pic, (unsigned short)number, &picregion_frame);
        break;
        case 3:
        get_picregion_frame(my_scene->numArryW_pic, (unsigned short)number, &picregion_frame);
        break;
        case 4:
        get_picregion_frame(my_scene->numArryW_pic, (unsigned short)number, &picregion_frame);
        break;
        case 5:
        get_picregion_frame(my_scene->numArryW_pic, (unsigned short)number, &picregion_frame);
        break;
        case 6:
        get_picregion_frame(my_scene->numArryW_pic, (unsigned short)number, &picregion_frame);
        break;
        case 7:
        get_picregion_frame(my_scene->numArryW_pic, (unsigned short)number, &picregion_frame);
        break;
        case 8:
        get_picregion_frame(my_scene->numArryW_pic, (unsigned short)number, &picregion_frame);
        break;
        case 9:
        get_picregion_frame(my_scene->numArryW_pic, (unsigned short)number, &picregion_frame);
        break;
        default:
        break;
    }
    sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)picregion_frame.pic_id, x, y);
}

/*
 信息栏显示格式形如 2008-08-08 的图片组
 UI-Edit中坐标如 1985-06-27
 */
void drawDateBar(scene_calendar_t *my_scene)
{
    sty_rect_t rect;
    int datePicArrayX[8];
    int datePicArrayY;

    datePicArrayY = my_scene->position1_pic->y;
    datePicArrayX[0] = my_scene->position1_pic->x;
    datePicArrayX[1] = my_scene->position2_pic->x;
    datePicArrayX[2] = my_scene->position3_pic->x;
    datePicArrayX[3] = my_scene->position4_pic->x;
    datePicArrayX[4] = my_scene->position5_pic->x;
    datePicArrayX[5] = my_scene->position6_pic->x;
    datePicArrayX[6] = my_scene->position7_pic->x;
    datePicArrayX[7] = my_scene->position8_pic->x;

    //wipe year
    rect.width = (short)((datePicArrayX[3] - datePicArrayX[0]) + my_scene->numArryW_pic->width);
    rect.height = my_scene->numArryW_pic->height;  
      
    rect.x = (short)datePicArrayX[0];
    rect.y = (short)datePicArrayY;
    sty_draw_sub_bitmap(my_scene->scene_common.handle_gui_dc, my_scene->bg_bitmap_info, 
            datePicArrayX[0], datePicArrayY, &rect);  
            
    rect.x = (short)(datePicArrayX[0] - my_scene->area_pic->x);
    rect.y = (short)(datePicArrayY - my_scene->area_pic->y);                     
    sty_draw_sub_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->area_pic->pic_id, 
            datePicArrayX[0], datePicArrayY, &rect);

    //wipe month
    rect.width = (short)((datePicArrayX[5] - datePicArrayX[4]) + my_scene->numArryW_pic->width);
    rect.height = my_scene->numArryW_pic->height; 
      
    rect.x = (short)datePicArrayX[4];
    rect.y = (short)datePicArrayY;
    sty_draw_sub_bitmap(my_scene->scene_common.handle_gui_dc, my_scene->bg_bitmap_info, 
            datePicArrayX[4], datePicArrayY, &rect);  
            
    rect.x = (short)(datePicArrayX[4] - my_scene->area_pic->x);
    rect.y = (short)(datePicArrayY - my_scene->area_pic->y);              
    sty_draw_sub_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->area_pic->pic_id, 
            datePicArrayX[4], datePicArrayY, &rect);

    //wipe day
    rect.width = (short)((datePicArrayX[7] - datePicArrayX[6]) + my_scene->numArryW_pic->width);
    rect.height = my_scene->numArryW_pic->height;    
    
    rect.x = (short)datePicArrayX[6];
    rect.y = (short)datePicArrayY;
    sty_draw_sub_bitmap(my_scene->scene_common.handle_gui_dc, my_scene->bg_bitmap_info, 
            datePicArrayX[6], datePicArrayY, &rect);   
            
    rect.x = (short)(datePicArrayX[6] - my_scene->area_pic->x);
    rect.y = (short)(datePicArrayY - my_scene->area_pic->y);             
    sty_draw_sub_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->area_pic->pic_id, 
            datePicArrayX[6], datePicArrayY, &rect);

    // draw year
    if (focusFlag == 2)
    {
        drawYellowNumPic(my_scene, yearVar / 1000, datePicArrayX[0], datePicArrayY);
        drawYellowNumPic(my_scene, (yearVar / 100) % 10, datePicArrayX[1], datePicArrayY);
        drawYellowNumPic(my_scene, (yearVar / 10) % 10, datePicArrayX[2], datePicArrayY);
        drawYellowNumPic(my_scene, yearVar % 10, datePicArrayX[3], datePicArrayY);
    }
    else
    {
        drawWhiteNumPic(my_scene, yearVar / 1000, datePicArrayX[0], datePicArrayY);
        drawWhiteNumPic(my_scene, (yearVar / 100) % 10, datePicArrayX[1], datePicArrayY);
        drawWhiteNumPic(my_scene, (yearVar / 10) % 10, datePicArrayX[2], datePicArrayY);
        drawWhiteNumPic(my_scene, yearVar % 10, datePicArrayX[3], datePicArrayY);
    }
    // draw month
    if (focusFlag == 1)
    {
        drawYellowNumPic(my_scene, monthVar / 10, datePicArrayX[4], datePicArrayY);
        drawYellowNumPic(my_scene, monthVar % 10, datePicArrayX[5], datePicArrayY);
    }
    else
    {
        drawWhiteNumPic(my_scene, monthVar / 10, datePicArrayX[4], datePicArrayY);
        drawWhiteNumPic(my_scene, monthVar % 10, datePicArrayX[5], datePicArrayY);
    }
    // draw day
    if (focusFlag == 0)
    {
        drawYellowNumPic(my_scene, dayVar / 10, datePicArrayX[6], datePicArrayY);
        drawYellowNumPic(my_scene, dayVar % 10, datePicArrayX[7], datePicArrayY);
    }
    else
    {
        drawWhiteNumPic(my_scene, dayVar / 10, datePicArrayX[6], datePicArrayY);
        drawWhiteNumPic(my_scene, dayVar % 10, datePicArrayX[7], datePicArrayY);
    }
}

/*
 在画屏幕日期表格中画当前天，即突出显示当前天
 */
void drawCurDay(scene_calendar_t *my_scene)
{
    gui_rect_t rect;
    int color;

    color = (int)my_scene->info_str->background;
    sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->shield_pic->pic_id, 
            dateTabX[curCol], dateTabY[curRow] - 1);

    rect.x0 = (short)dateTabX[curCol];
    rect.y0 = (short)dateTabY[curRow] - 1; //- 2
    rect.x1 = (short)((dateTabX[curCol] + my_scene->shield_pic->width));
    rect.y1 = (short)((dateTabY[curRow] + my_scene->shield_pic->height));
    drawStrAnyWhere(my_scene, (char*)monChar[dayVar - 1], &rect, 17, color, GUI_ALIGN_CENTER);
}

/*
 信息栏显示格式形如[2006]年[8]月[20]日[生肖][干支纪年][ 节气]
 flag为1表示需要擦除
 */
void drawInfoBar(scene_calendar_t *my_scene)
{
    char str[48];
    int lanId;
    //int oldcolor;
    gui_rect_t rect;
    sty_rect_t s_rect;
    DateVarType solar, lunar;
    solar.year = yearVar;
    solar.month = monthVar;
    solar.day = dayVar;

    /* gui_dc 关于字符串的显示设置 */
    gui_dc_set_color(my_scene->scene_common.handle_gui_dc, GUI_COLOR_WHITE);
    gui_dc_set_text_mode(my_scene->scene_common.handle_gui_dc, GUI_DRAW_MODE_TRANS);
    gui_dc_set_font_size(my_scene->scene_common.handle_gui_dc, 13);
    gui_dc_set_pen_size(my_scene->scene_common.handle_gui_dc, 1);

    /* get the info string */
    lanId = get_language_mode();
    getLunarString(lunar_buffer, &solar, &lunar, lanId);
    if (getHolidayStr(str, &solar, &lunar, lanId) == 1)
    {
        strcat(lunar_buffer, " ");
        strcat(lunar_buffer, str);
    }

    /* wipe the old info string */
    s_rect.x = my_scene->info_str->x - my_scene->area_pic->x;
    s_rect.y = my_scene->info_str->y - my_scene->area_pic->y;
    s_rect.width = my_scene->info_str->width;
    s_rect.height = my_scene->info_str->height;
    sty_draw_sub_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->area_pic->pic_id, 
            my_scene->info_str->x, my_scene->info_str->y, &s_rect);

    /* draw the new info string */
    rect.x0 = my_scene->info_str->x;
    rect.y0 = my_scene->info_str->y;// + 6;
    rect.x1 = (my_scene->info_str->x + my_scene->info_str->width) - 1;
    rect.y1 = (my_scene->info_str->y + my_scene->info_str->height) - 1;
    gui_dc_display_string_in_rect(my_scene->scene_common.handle_gui_dc, lunar_buffer, &rect, GUI_ALIGN_RIGHT,
            GUI_MBCS_ENC);
}

/*
 重画屏幕中所有有变化的区域
 */
void rePaintWindowArea(scene_calendar_t *my_scene)
{
    setCurYMD(yearVar, monthVar, 1, &jd);
    paintCalendarTable(my_scene);
    drawCurDay(my_scene);
    drawDateBar(my_scene);
    drawInfoBar(my_scene);
}

