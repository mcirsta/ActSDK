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
 * \file     calendar_play.c
 * \brief    calendar的实现和按键控制
 * \author   dupengcheng
 *
 * \version 1.0
 * \date  2009/04/14
 *******************************************************************************/

#include "calendar_scene.h"
#include "calendar_play.h"
#include "calendar_ui.h"
#include "date.h"




extern const short monthDays[]; /* this variable define in <date.c> */
extern scene_calendar_t* g_scene_calendar;

/*                 日历应用各文件的说明
 ******************************************************************
 *                  日历应用各文件的说明
 * getHoliday.c : 根据公历判断法定节日
 * date.c       : 完成年月日到date的转换
 * solar2lunar.c: 完成公历纪年到农历年，节气的转换
 *######[以上的几个文件和UI、系统平台无关，可以当作库，便于移植]####
 *
 * calendar_ui.c   : UI 绘图相关
 * calendar_play.c : 按键处理和对上面各个模块的组装，实现计算器功能.
 * calendar_main.c : 应用入口、退出
 * calendar_scene.c: calendar 场景相关函数
 ******************************************************************/

 /*
 ******************** 以下是内部静态数据区 ****************
 ** The following code used to
 ** difine static globle variables.
 **********************************************************
 */
int yearVar;
int monthVar;
int dayVar;

//当前选中的行;
int curRow;
int curCol;
//当前的年月日的焦点,默认日，年=2,月=1,日=0
int focusFlag = 0;
//重画屏幕的标志，0=不更新[默认]，1=更新 date table，2=更新 date bar 上的 UI
int paintFlag = 0;

//date表格的坐标
int dateTabX[7];
int dateTabY[6];

//1个月内date的字符串
const char *monChar[] =
{ "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", 
  "13", "14", "15", "16", "17", "18", "19", "20", "21",
  "22", "23", "24", "25", "26", "27", "28", "29", "30", "31" 
};

unsigned int jd; /* 保存当前日期的字符串 y-m-d */
static gui_rect_t ui_array[TOTALROWS][TOTALCOLS];

/****************************************************************/
static void _initCalendar(void);
static int _daysInMonth_Internal(int y, int m);
static int _isLeapYear(int y);
static void _onKeyPrevMonth(scene_calendar_t *my_scene);
static void _onKeyNextMonth(scene_calendar_t *my_scene);
static void _onKeyPrevYear(scene_calendar_t *my_scene);
static void _onKeyNextYear(scene_calendar_t *my_scene);
static void _onKeyPrevDay(scene_calendar_t *my_scene);
static void _onKeyNextDay(scene_calendar_t *my_scene);
static void _onKeyOk(scene_calendar_t *my_scene);



/*
 ****************** 以下是和初始化相关函数 ****************
 ** The following routines used to initialize calendar.
 **
 **********************************************************
 */

/*
 启动日历应用时,在场景资源初始化后,
 获取当前前日期,并在屏幕上显示出来.
 */
void calendar_load(scene_calendar_t *my_scene)
{
    _initCalendar();
    paintCalendarTable(my_scene);
    drawDateBar(my_scene);
    drawInfoBar(my_scene);
    drawCurDay(my_scene);
}

/*
 初始化日历的参数,获取当前日期
 */
static void _initCalendar(void)
{
    rtc_date_time_t date_time;
    int dev_fd;

    dev_fd = open(DEVICE_RTC, O_RDWR);
    if (dev_fd < 0)
    {
        //print_err("open %s failed!", DEVICE_RTC);
        close(dev_fd);
        return;
    }
    if (ioctl(dev_fd, RTC_GET_DATE_TIME, &date_time) != 0)
    {
        //print_err("get rtc date and time failed!");
        close(dev_fd);
        return;
    }
    close(dev_fd);

    yearVar = date_time.date.year;
    monthVar = date_time.date.month;
    dayVar = date_time.date.day;
    focusFlag = 0;
    paintFlag = 0;

    setCurYMD(yearVar, monthVar, 1, &jd);
}

/*
 *  得到界面中各个日期方格的坐标，用gui_rect_t表示
 *  竖屏、横屏坐标不同
 */
bool get_date_panes_coordinate(void)
{
    int count_row, count_col;
    int begin_x = g_scene_calendar->block_pic->x;
    int begin_y = g_scene_calendar->block_pic->y;
    int block_width = g_scene_calendar->block_pic->width;
    int block_higth = g_scene_calendar->block_pic->height;

    for (count_row = 0; count_row < TOTALROWS; count_row++)
    {
        dateTabY[count_row] = begin_y + (block_higth * count_row);
    }
    for (count_col = 0; count_col < TOTALCOLS; count_col++)
    {
        dateTabX[count_col] = begin_x + (block_width * count_col);
    }

    for (count_row = 0; count_row < TOTALROWS; count_row++)
    {
        for (count_col = 0; count_col < TOTALCOLS; count_col++)
        {
            ui_array[count_row][count_col].x0 = (short)(begin_x + (block_width * count_col));
            ui_array[count_row][count_col].y0 = (short)(begin_y + (block_higth * count_row));
            ui_array[count_row][count_col].x1 = (short)(ui_array[count_row][count_col].x0 + block_width);
            ui_array[count_row][count_col].y1 = (short)(ui_array[count_row][count_col].y0 + block_higth);
        }
    }
    return true;
}

/*
 * 绘制当前月份的日历表
 * 颜色由字符串资源(前景色、背景色)传进来：
 *      color_str : 前景色【当月周六】、背景色【当月周日】
 *      title_str : 前景色【当月周1--周5】、背景色【非当月】
 *      info_str  : 背景色【当前选中】 
 */
void paintCalendarTable(scene_calendar_t *my_scene)
{
    sty_rect_t rect;
    char str[48];
    unsigned char lanId;
    int days;
    int i;
    int color;
    int startCol = dayOfWeek(&jd) % 7;
    int startRow = 0;
    DateVarType scolar;
    DateVarType lunar;
    scolar.year = yearVar;
    scolar.month = monthVar;    
    
    
    //------- wipe old  date table -----
    rect.x = my_scene->bigarea_pic->x;
    rect.y = my_scene->bigarea_pic->y;
    rect.width = my_scene->bigarea_pic->width;
    rect.height = my_scene->bigarea_pic->height;       
    sty_draw_sub_bitmap(my_scene->scene_common.handle_gui_dc, my_scene->bg_bitmap_info, 
            my_scene->bigarea_pic->x, my_scene->bigarea_pic->y, &rect);
    
    sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, (int)my_scene->bigarea_pic->pic_id, 
            my_scene->bigarea_pic->x, my_scene->bigarea_pic->y);

    //------- draw new days in the table -----
    if (monthVar > 1)
    {
        days = _daysInMonth_Internal(yearVar, monthVar - 1); //上个月的总天数
    }
    else
    {
        if (yearVar > MIN_YEAR)
        {
            days = _daysInMonth_Internal(yearVar - 1, 12);
        }
        else
        {
            days = _daysInMonth_Internal(MAX_YEAR, 12);
        }
    }
    color = (int)g_scene_calendar->title_str->background;
    for (i = startCol - 1; i >= 0; i--) //画上一个月份
    {
        days--;
        drawStrAnyWhere(my_scene, (char*)monChar[days], &ui_array[0][i], 16, color, GUI_ALIGN_CENTER);
    }

    days = _daysInMonth_Internal(yearVar, monthVar); //本月的总天数
    lanId = (unsigned char)get_language_mode();
    for (i = 0; i < days; i++) //画这个月份
    {
        scolar.day = i + 1;
        if ((i + 1) == dayVar)
        {
            curRow = startRow;
            curCol = startCol;
        }
        getLunarString(str, &scolar, &lunar, (int)lanId);
        if (getHolidayStr(str, &scolar, &lunar, (int)lanId) == 1) //判断节日
        {
            color = GUI_COLOR_BLUE;
        }
        else if (startCol == 0) //判断周末[周日]
        {
            color = (int)g_scene_calendar->color_str->background;
        }
        else if (startCol == 6) //[周六]
        {
            color = (int)g_scene_calendar->color_str->foreground;
        }
        else
        {
            color = (int)g_scene_calendar->title_str->foreground;
        }
        drawStrAnyWhere(my_scene, (char*)monChar[i], &ui_array[startRow][startCol], 16, color, GUI_ALIGN_CENTER);
        startCol++;
        if (startCol == TOTALCOLS) //换行
        {
            startCol = 0;
            startRow++;
        }
    }

    i = 0;
    color = (int)g_scene_calendar->title_str->background;
    while (startRow < TOTALROWS) //画下个月份
    {
        drawStrAnyWhere(my_scene, (char*)monChar[i], &ui_array[startRow][startCol], 16, color, GUI_ALIGN_CENTER);
        i++;
        startCol++;
        if (startCol == TOTALCOLS) //换行
        {
            startCol = 0;
            startRow++;
        }
    }
}

/*
 *************** 以下是和按键，控制相关函数 ***************
 ** The following routines used to mamage key,
 ** and deal with control.
 **********************************************************
 */

/*
 *   按键管理
 *
 */
void calendar_key_mamage(scene_calendar_t *my_scene, key_event_t *kmsg)
{
    //unsigned int key = kmsg->val | kmsg->type;

    switch (kmsg->val | kmsg->type)
    {
        case KEY_OK | KEY_TYPE_DOWN:
        _onKeyOk(my_scene); //年月日的焦点切换
        break;

        case KEY_PREV | KEY_TYPE_DOWN:
        case KEY_PREV | KEY_TYPE_LONG:        
        case KEY_PREV | KEY_TYPE_HOLD:        
        switch (focusFlag)
        {
            case 0:
            _onKeyPrevDay(my_scene);
            break;
            case 1:
            _onKeyPrevMonth(my_scene);
            break;
            case 2:
            _onKeyPrevYear(my_scene);
            break;
            default:
            break;
        }
        break;

        case KEY_NEXT | KEY_TYPE_DOWN:
        case KEY_NEXT | KEY_TYPE_LONG:        
        case KEY_NEXT | KEY_TYPE_HOLD:
        switch (focusFlag)
        {
            case 0:
            _onKeyNextDay(my_scene);
            break;
            case 1:
            _onKeyNextMonth(my_scene);
            break;
            case 2:
            _onKeyNextYear(my_scene);
            break;
            default:
            break;
        }
        break;

        default:
        break;
    }
}

/*
 *    press key_ok: 年月日的焦点切换，
 *    focusFlag: 2=year; 1=month; 0=day default
 */
static void _onKeyOk(scene_calendar_t *my_scene)
{
    focusFlag++;
    if (focusFlag < 0)
    {
        focusFlag = 2;
    }
    if (focusFlag > 2)
    {
        focusFlag = 0;
    }
    drawDateBar(my_scene);
}

/*
 *      切换到前一天
 */
static void _onKeyPrevDay(scene_calendar_t *my_scene)
{
    dayVar--;
    if (dayVar < 1)
    {
        monthVar--;
        if (monthVar < 1)
        {
            monthVar = 12;
            yearVar--;
            if (yearVar < MIN_YEAR)
            {
                yearVar = MAX_YEAR;
            }
        }
        dayVar = _daysInMonth_Internal(yearVar, monthVar);
    }

    rePaintWindowArea(my_scene);
}

/*
 *      切换到后一天
 */
static void _onKeyNextDay(scene_calendar_t *my_scene)
{
    dayVar++;
    if (dayVar > _daysInMonth_Internal(yearVar, monthVar))
    {
        dayVar = 1;
        monthVar++;
        if (monthVar > 12)
        {
            monthVar = 1;
            yearVar++;
            if (yearVar > MAX_YEAR)
            {
                yearVar = MIN_YEAR;
            }
        }
    }

    rePaintWindowArea(my_scene);
}

/*
 *      上一个月的日历表
 */
static void _onKeyPrevMonth(scene_calendar_t *my_scene)
{
    monthVar--;
    if (monthVar < 1)
    {
        monthVar = 12;
        yearVar--;
        if (yearVar < MIN_YEAR)
        {
            yearVar = MAX_YEAR;
        }
    }

    if (dayVar > _daysInMonth_Internal(yearVar, monthVar))
    {
        dayVar = _daysInMonth_Internal(yearVar, monthVar);
    }

    //通知窗口重绘真个区域
    rePaintWindowArea(my_scene);
}

/*
 *       下一个月的日历表
 */
static void _onKeyNextMonth(scene_calendar_t *my_scene)
{
    monthVar++;
    if (monthVar > 12)
    {
        monthVar = 1;
        yearVar++;
        if (yearVar > MAX_YEAR)
        {
            yearVar = MIN_YEAR;
        }
    }
    if (dayVar > _daysInMonth_Internal(yearVar, monthVar))
    {
        dayVar = _daysInMonth_Internal(yearVar, monthVar);
    }

    rePaintWindowArea(my_scene);
}

/*
 *       上一年的本月的日历表
 */
static void _onKeyPrevYear(scene_calendar_t *my_scene)
{
    yearVar--;
    if (yearVar < MIN_YEAR)
    {
        yearVar = MAX_YEAR;
    }
    if (dayVar > _daysInMonth_Internal(yearVar, monthVar))
    {
        dayVar = _daysInMonth_Internal(yearVar, monthVar);
    }

    rePaintWindowArea(my_scene);
}

/*
 *       下一年的本月的日历表
 */
static void _onKeyNextYear(scene_calendar_t *my_scene)
{
    yearVar++;
    if (yearVar > MAX_YEAR)
    {
        yearVar = MIN_YEAR;
    }
    if (dayVar > _daysInMonth_Internal(yearVar, monthVar))
    {
        dayVar = _daysInMonth_Internal(yearVar, monthVar);
    }

    rePaintWindowArea(my_scene);
}

/*
 *    判断本月又多少天
 */
static int _isLeapYear(int y)
{
    return (((y % 4) == 0) && ((y % 100) != 0)) || ((y % 400) == 0);
}

static int _daysInMonth_Internal(int y, int m)
{
    if ((m == 2) && (_isLeapYear(y) ))
    {
        return 29;
    }
    else
    {
        return monthDays[m];
    }
}

/*
 因为 [日历模块]中对语言的编号 和 213f 不一致，
 所以作个转换
 */
int get_language_mode(void)
{
    if (sys_get_cur_language_id() == MLANG_ZH_CN)    
    {
        return 2; //简体中文
    }
    else if (sys_get_cur_language_id() == MLANG_ZH_TW)    
    {
        return 3; //繁体中文
    }
    else
    {
        return 1; //default
    }
}
