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
 * \brief    calendar��ʵ�ֺͰ�������
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

/*                 ����Ӧ�ø��ļ���˵��
 ******************************************************************
 *                  ����Ӧ�ø��ļ���˵��
 * getHoliday.c : ���ݹ����жϷ�������
 * date.c       : ��������յ�date��ת��
 * solar2lunar.c: ��ɹ������굽ũ���꣬������ת��
 *######[���ϵļ����ļ���UI��ϵͳƽ̨�޹أ����Ե����⣬������ֲ]####
 *
 * calendar_ui.c   : UI ��ͼ���
 * calendar_play.c : ��������Ͷ��������ģ�����װ��ʵ�ּ���������.
 * calendar_main.c : Ӧ����ڡ��˳�
 * calendar_scene.c: calendar ������غ���
 ******************************************************************/

 /*
 ******************** �������ڲ���̬������ ****************
 ** The following code used to
 ** difine static globle variables.
 **********************************************************
 */
int yearVar;
int monthVar;
int dayVar;

//��ǰѡ�е���;
int curRow;
int curCol;
//��ǰ�������յĽ���,Ĭ���գ���=2,��=1,��=0
int focusFlag = 0;
//�ػ���Ļ�ı�־��0=������[Ĭ��]��1=���� date table��2=���� date bar �ϵ� UI
int paintFlag = 0;

//date��������
int dateTabX[7];
int dateTabY[6];

//1������date���ַ���
const char *monChar[] =
{ "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", 
  "13", "14", "15", "16", "17", "18", "19", "20", "21",
  "22", "23", "24", "25", "26", "27", "28", "29", "30", "31" 
};

unsigned int jd; /* ���浱ǰ���ڵ��ַ��� y-m-d */
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
 ****************** �����Ǻͳ�ʼ����غ��� ****************
 ** The following routines used to initialize calendar.
 **
 **********************************************************
 */

/*
 ��������Ӧ��ʱ,�ڳ�����Դ��ʼ����,
 ��ȡ��ǰǰ����,������Ļ����ʾ����.
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
 ��ʼ�������Ĳ���,��ȡ��ǰ����
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
 *  �õ������и������ڷ�������꣬��gui_rect_t��ʾ
 *  �������������겻ͬ
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
 * ���Ƶ�ǰ�·ݵ�������
 * ��ɫ���ַ�����Դ(ǰ��ɫ������ɫ)��������
 *      color_str : ǰ��ɫ������������������ɫ���������ա�
 *      title_str : ǰ��ɫ��������1--��5��������ɫ���ǵ��¡�
 *      info_str  : ����ɫ����ǰѡ�С� 
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
        days = _daysInMonth_Internal(yearVar, monthVar - 1); //�ϸ��µ�������
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
    for (i = startCol - 1; i >= 0; i--) //����һ���·�
    {
        days--;
        drawStrAnyWhere(my_scene, (char*)monChar[days], &ui_array[0][i], 16, color, GUI_ALIGN_CENTER);
    }

    days = _daysInMonth_Internal(yearVar, monthVar); //���µ�������
    lanId = (unsigned char)get_language_mode();
    for (i = 0; i < days; i++) //������·�
    {
        scolar.day = i + 1;
        if ((i + 1) == dayVar)
        {
            curRow = startRow;
            curCol = startCol;
        }
        getLunarString(str, &scolar, &lunar, (int)lanId);
        if (getHolidayStr(str, &scolar, &lunar, (int)lanId) == 1) //�жϽ���
        {
            color = GUI_COLOR_BLUE;
        }
        else if (startCol == 0) //�ж���ĩ[����]
        {
            color = (int)g_scene_calendar->color_str->background;
        }
        else if (startCol == 6) //[����]
        {
            color = (int)g_scene_calendar->color_str->foreground;
        }
        else
        {
            color = (int)g_scene_calendar->title_str->foreground;
        }
        drawStrAnyWhere(my_scene, (char*)monChar[i], &ui_array[startRow][startCol], 16, color, GUI_ALIGN_CENTER);
        startCol++;
        if (startCol == TOTALCOLS) //����
        {
            startCol = 0;
            startRow++;
        }
    }

    i = 0;
    color = (int)g_scene_calendar->title_str->background;
    while (startRow < TOTALROWS) //���¸��·�
    {
        drawStrAnyWhere(my_scene, (char*)monChar[i], &ui_array[startRow][startCol], 16, color, GUI_ALIGN_CENTER);
        i++;
        startCol++;
        if (startCol == TOTALCOLS) //����
        {
            startCol = 0;
            startRow++;
        }
    }
}

/*
 *************** �����ǺͰ�����������غ��� ***************
 ** The following routines used to mamage key,
 ** and deal with control.
 **********************************************************
 */

/*
 *   ��������
 *
 */
void calendar_key_mamage(scene_calendar_t *my_scene, key_event_t *kmsg)
{
    //unsigned int key = kmsg->val | kmsg->type;

    switch (kmsg->val | kmsg->type)
    {
        case KEY_OK | KEY_TYPE_DOWN:
        _onKeyOk(my_scene); //�����յĽ����л�
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
 *    press key_ok: �����յĽ����л���
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
 *      �л���ǰһ��
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
 *      �л�����һ��
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
 *      ��һ���µ�������
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

    //֪ͨ�����ػ��������
    rePaintWindowArea(my_scene);
}

/*
 *       ��һ���µ�������
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
 *       ��һ��ı��µ�������
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
 *       ��һ��ı��µ�������
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
 *    �жϱ����ֶ�����
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
 ��Ϊ [����ģ��]�ж����Եı�� �� 213f ��һ�£�
 ��������ת��
 */
int get_language_mode(void)
{
    if (sys_get_cur_language_id() == MLANG_ZH_CN)    
    {
        return 2; //��������
    }
    else if (sys_get_cur_language_id() == MLANG_ZH_TW)    
    {
        return 3; //��������
    }
    else
    {
        return 1; //default
    }
}
