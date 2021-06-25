/*******************************************************************************
 *                              USDK(1100)
 *                            Module: common_adjust
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version >             <desc>
 *       Jerry Fu    2009-4-22 13:11          1.0                 build this file
 *******************************************************************************/
/*!
 * \file     adjust.c
 * \brief    概述
 * \author
 * \par      GENERAL DESCRIPTION:
 *               adjustr控件实现
 * \par      EXTERNALIZED FUNCTIONS:
 *               用到外面的模块
 * \version 1.0
 * \date  2009-4-22
 *******************************************************************************/
#include "adjust_private.h"
#include "commonui_sty.h"

#define   MIN_YEAR     (2000)
#define   MAX_YEAR     (2037)

/* 平年每月所包含的天数 */
int month_normal_days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

/* 闰年没有所包含的天数*/
int month_leapyear_days[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

/*!
 *  \par adjust 控件私有数据结构
 *  \li key_status_t : adjust控件中所使用的按键的状态
 *  \li time_date_parameter_t : 保存和调整时间日期相关的数据
 *  \li pic_res : 指向资源组中图片资源的指针数组
 *  \li picregion_res : 指向资源组中图片区资源的指针
 *  \li string_resource_t : 指向资源组中字符串资源的指针
 */
struct adjust_private_s
{
    /* record key status  */
    key_status_t adj_key_sta;

    string_resource_t *str_adjust_title;
    int str_title_id;

    picregion_resource_t *picregion_adjust_bg;
    picregion_resource_t *picregion_left_arrow;
    picregion_resource_t *picregion_right_arrow;
    picture_resource_t *pic_disp_bg;

    adjust_private_textdigit_t text_digit;
    adjust_private_switch_t adjust_onoff;
    adjust_pritvate_time_t adjust_time;
    adjust_private_date_t adjust_date;

    bool show_bg;
    bool show_title;
    bool show_left_arrow;
    bool show_right_arrow;
    bool show_disp_bg;
};

adjust_t *cur_adjust;

static bool _is_leap_year(int y);
static void _qualify(int * p_data, int min, int max);
static int _get_days_in_month(bool leapyear, int month);
static int _get_max_day(date_para_t *pDate);
static void _time_12_to_24(rtc_time_t *time_val, int ap_flag);
static bool _handle_date_data(adjust_t *pAdjust, int step);
static bool _handle_time_data(adjust_t *pAdjust, int step);
static bool _key_handle_digit_text(adjust_t *pAdjust, key_event_t *pmsg);
static bool _key_handle_time(adjust_t *pAdjust, key_event_t *pmsg);
static bool _key_handle_date(adjust_t *pAdjust, key_event_t *pmsg);
static bool _key_handle_switch(adjust_t *pAdjust, key_event_t *pmsg);

/************************************************************************************/
/*!
 * \par  Description:
 *     判断输入的年份是否是闰年
 * \param[in]    y   输入的年份
 * \return       the result
 * \retval       true    设置成功
 * \retval       false   设置失败
 ************************************************************************************/
static bool _is_leap_year(int y)
{
    if ((y % 400) == 0)
    {
        return true;
    }

    if (((y % 4) == 0) && ((y % 100) != 0))
    {
        return true;
    }

    return false;
}

/************************************************************************************/
/*!
 * \par  Description:
 *     判断输入的数据是否在制定的范围之内，如果在
 *     不在范围之内，自动调整，数据超过最大值的时候
 *     调节成最小的，在小于最小的时候，调成最大的，
 *     如果在此之间，不需要改变它的值
 * \param[in]    p_pAdjust    要调节的数据
 * \param[in]    min       数据的下限
 * \param[in]    max       数据的上限
 * \return       the result
 ************************************************************************************/
static void _qualify(int * p_data, int min, int max)
{
    if (*p_data > max)
    {
        *p_data = min;
    }
    if (*p_data < min)
    {
        *p_data = max;
    }
}

/************************************************************************************/
/*!
 * \par  Description:
 *     获取一个月中包含的天数，在二月份要考虑是闰年还是平年
 * \param[in]    leapyear  是否是闰年 TRUE：是闰年，FALSE：不是闰年
 * \param[in]    month     输入的月份
 * \return       the result
 * \retval       返回月中包含的天数
 ************************************************************************************/
static int _get_days_in_month(bool leapyear, int month)
{
    int day;

    if (1 == leapyear)
    {
        day = month_leapyear_days[month - 1];
    }
    else
    {
        day = month_normal_days[month - 1];
    }
    return day;
}

/************************************************************************************/
/*!
 * \par  Description:
 *     根据传入的日期，得到当前设置的月份中包含的天数
 * \param[in]    pTimeDate 该指针所指向的结构体中包含了时间和日期信息 
 * \param[in]    month     输入的月份
 * \return       the result
 * \retval       返回月中包含的天数
 ************************************************************************************/
static int _get_max_day(date_para_t *pDate)
{
    int max_day;
    bool leap_year;

    leap_year = _is_leap_year(pDate->date_digit_info[0]);
    max_day = _get_days_in_month(leap_year, pDate->date_digit_info[1]);
    return max_day;
}

/************************************************************************************/
/*!
 * \par  Description:
 *     从12小时制转换成24小时制
 * \param[in]    time_val  该指针所指向的结构体中包含了时间信息 
 * \param[in]    ap_flag   0：AM  1:PM
 * \return       the result
 ************************************************************************************/
static void _time_12_to_24(rtc_time_t *time_val, int ap_flag)
{
    _qualify(&(time_val->hour), 1, 12);
    _qualify(&ap_flag, 0, 1);

    time_val->hour = (time_val->hour) + (ap_flag * 12);
    if ((time_val->hour) == 12)
    {
        time_val->hour = 0;
    }

    if ((time_val->hour) >= 24)
    {
        time_val->hour = 12;
    }
}

/************************************************************************************/
/*!
 * \par  Description:
 *     当控件调节日期的时候，处理和日期相关的数据
 *     包括日期的正确性验证
 * \ingroup common_adjust
 * \param[in]    pAdjust:  指向adjust控件结构体的指针
 * \param[in]    当前按键操作步长(1或－1)
 * \return       the result
 * \retval       成功返回true, 失败返回false
 ************************************************************************************/
static bool _handle_date_data(adjust_t *pAdjust, int step)
{
    date_para_t *pDate;
    int cur_index;
    int max_day;

    pDate = &(pAdjust->pAdjPrivate->adjust_date.date_para);
    cur_index = pDate->date_index;

    switch (cur_index)
    {
        case 0: /* 调整年 */
        /* 年加或者减一 */
        pDate->date_digit_info[0] += step;
        /* 判断年的范围是不是在2000－2099的范围之内 */
        _qualify(&(pDate->date_digit_info[0]), MIN_YEAR, MAX_YEAR);
        /* 读取当前设定月份的最大天数 */
        max_day = _get_max_day(pDate);
        /* 判断当前设定的日是否越界，如果越界，进行相应的调整*/
        if ((pDate->date_digit_info[2]) > max_day)
        {
            pDate->date_digit_info[2] = max_day;
        }
        break;

        case 1: /* 调整月 */
        pDate->date_digit_info[1] += step;
        _qualify(&(pDate->date_digit_info[1]), 1, 12);

        /*根据所设定的月份，对当前的日进行判断*/
        max_day = _get_max_day(pDate);
        if ((pDate->date_digit_info[2]) > max_day)
        {
            pDate->date_digit_info[2] = max_day;
        }
        break;

        case 2: /* 调整日 */
        max_day = _get_max_day(pDate);
        pDate->date_digit_info[2] += step;
        _qualify(&(pDate->date_digit_info[2]), 1, max_day);
        break;

        default:
        break;
    }

    return true;
}

/************************************************************************************/
/*!
 * \par  Description:
 *     当控件调节时间的时候，处理和时间相关的数据
 *     包括输入时间的正确性验证
 * \ingroup common_adjust
 * \param[in]    pAdjust:  指向adjust控件结构体的指针
 * \param[in]    pmsg: 指向按键消息结构体的指针
 * \return       the result
 * \retval       成功返回true, 失败返回false
 ************************************************************************************/
static bool _handle_time_data(adjust_t *pAdjust, int step)
{
    time_para_t *pTime;
    int cur_index;

    pTime = &(pAdjust->pAdjPrivate->adjust_time.time_para);
    cur_index = pTime->time_index;

    switch (cur_index)
    {
        case 0: /*调整小时 */
        pTime->time_digit_info[0] += step;
        if (pAdjust->adjust_style == ADJ_STYLE_TIME)
        {
            if (pAdjust->time_mode == TIME_MODE_12)
            {
                _qualify(&(pTime->time_digit_info[0]), 1, 12);
            }
            else
            {
                _qualify(&(pTime->time_digit_info[0]), 0, 23);
            }
        }
        else
        {
            _qualify(&(pTime->time_digit_info[0]), 0, 99);
        }
        break;

        case 1: /*调整分钟 */
        pTime->time_digit_info[1] += step;
        _qualify(&(pTime->time_digit_info[1]), 0, 59);
        break;

        case 2: /*调整AM，PM标志*/
        pTime->time_digit_info[2] += step;
        _qualify(&(pTime->time_digit_info[2]), TIME_AM, TIME_PM);
        break;

        default:
        break;
    }

    return true;
}

/************************************************************************************/
/*!
 * \par  Description:
 *     当控件调节的是数字，或者文本信息的时候，
 *     根据按键的状态调节显示的内容。并且根据具体情况
 *     调用回调函数来执行相应的操作。
 * \ingroup common_adjust
 * \param[in]    pAdjust:  指向adjust控件结构体的指针
 * \param[in]    pmsg: 指向按键消息结构体的指针
 * \return       the result
 * \retval       成功返回true, 失败返回false
 ************************************************************************************/
static bool _key_handle_digit_text(adjust_t *pAdjust, key_event_t *pmsg)
{
    int step;
    int max_val;
    int min_val;

    adjust_textdigit_init_t *pDigitPara;
    adjust_private_textdigit_t *pTextDigit;

    pDigitPara = &(pAdjust->adjust_digit);
    pTextDigit = (adjust_private_textdigit_t *) (&(pAdjust->pAdjPrivate->text_digit));

    step = pDigitPara->step;
    max_val = pDigitPara->max_val;
    min_val = pDigitPara->min_val;

    switch (pmsg->val)
    {
        case KEY_OK:
        switch (pmsg->type)
        {
            case KEY_TYPE_SHORT_UP:/*当按键抬起的时候，才进行调节框切换*/
            case KEY_TYPE_LONG_UP:
            case KEY_TYPE_HOLD_UP:
            pAdjust->callback(pAdjust, WIDGET_SIG_ADJUST_OK);
            break;

            default:
            break;
        }
        break;

        case KEY_RETURN:
        if ((pmsg->type == KEY_TYPE_SHORT_UP) || (pmsg->type == KEY_TYPE_LONG_UP) || (pmsg->type == KEY_TYPE_HOLD_UP))
        {
            pAdjust->callback(pAdjust, WIDGET_SIG_ADJUST_CANCEL);
        }
        break;

        /*当"PREV"键按下的时候，开始减操作 */
        case KEY_PREV:
        switch (pmsg->type)
        {
            case KEY_TYPE_DOWN: /* 当PREV按下的时候，记录PREV键当前的状态 */
            pAdjust->pAdjPrivate->adj_key_sta.key_down_status = KEY_PRESSED;
            pAdjust->pAdjPrivate->show_left_arrow = false;
            adjust_paint(pAdjust);
            break;

            /* 当PREV键处于hold状态的时候，设定PREV的状态为“按下”，
             按照设定的步长进行“减”计数。一直减到所设定的最小值
             并且根据当前的设定值，进行绘图操作
             */
            case KEY_TYPE_HOLD:
            case KEY_TYPE_HOLD_UP:
            case KEY_TYPE_SHORT_UP:
            case KEY_TYPE_LONG_UP:
            if (pmsg->type != KEY_TYPE_HOLD)
            {
                pAdjust->pAdjPrivate->adj_key_sta.key_down_status = KEY_RELEASED;
            }
            else
            {
                pAdjust->pAdjPrivate->adj_key_sta.key_down_status = KEY_PRESSED;
            }

            pDigitPara->cur_val -= step;
            if (pDigitPara->cur_val < min_val)
            {
                pDigitPara->cur_val = min_val;
            }
            pAdjust->pAdjPrivate->show_left_arrow = false;
            pAdjust->pAdjPrivate->show_disp_bg = false;
            pAdjust->pAdjPrivate->text_digit.show_text = false;

            adjust_paint(pAdjust);
            pAdjust->callback(pAdjust, WIDGET_SIG_ADJUST_DOWN);

            clear_key_msg_queue(-1, -1);

            break;

            default:
            break;
        }
        break;

        /* NEXT键进行加操作*/
        case KEY_NEXT:
        switch (pmsg->type)
        {
            case KEY_TYPE_DOWN: /* 当PREV按下的时候，记录NEXT键当前的状态 */
            pAdjust->pAdjPrivate->adj_key_sta.key_up_status = KEY_PRESSED;
            pAdjust->pAdjPrivate->show_right_arrow = false;
            adjust_paint(pAdjust);
            break;
            /* 当NEXT键处于hold状态的时候，设定NEXT的状态为“按下”，
             按照设定的步长进行“加”计数。一直加到所设定的最大值
             并且根据当前的设定值，进行绘图操作
             */
            case KEY_TYPE_HOLD:
            case KEY_TYPE_HOLD_UP:
            case KEY_TYPE_SHORT_UP:
            case KEY_TYPE_LONG_UP:
            if (pmsg->type != KEY_TYPE_HOLD)
            {
                pAdjust->pAdjPrivate->adj_key_sta.key_up_status = KEY_RELEASED;
            }
            else
            {
                pAdjust->pAdjPrivate->adj_key_sta.key_up_status = KEY_PRESSED;
            }
            pDigitPara->cur_val += step;
            if (pDigitPara->cur_val > max_val)
            {
                pDigitPara->cur_val = max_val;
            }
            pAdjust->pAdjPrivate->show_right_arrow = false;
            pAdjust->pAdjPrivate->show_disp_bg = false;
            pAdjust->pAdjPrivate->text_digit.show_text = false;

            adjust_paint(pAdjust);
            pAdjust->callback(pAdjust, WIDGET_SIG_ADJUST_UP);

            clear_key_msg_queue(-1, -1);

            break;

            default:
            break;
        }
        break;
        default:
        break;
    }

    return true;
}

/************************************************************************************/
/*!
 * \par  Description:
 *     当控件正在调节时间的时候，处理按键信息
 * \ingroup common_adjust
 * \param[in]    pAdjust:  指向adjust控件结构体的指针
 * \param[in]    pmsg: 指向按键消息结构体的指针
 * \return       the result
 * \retval       成功返回true, 失败返回false
 ************************************************************************************/
static bool _key_handle_time(adjust_t *pAdjust, key_event_t *pmsg)
{
    int hour;
    int minute;
    int ampm;

    time_para_t *pTime;
    pTime = &(pAdjust->pAdjPrivate->adjust_time.time_para);

    switch (pmsg->val)
    {
        case KEY_OK:
        switch (pmsg->type)
        {
            case KEY_TYPE_SHORT_UP:/*当按键抬起的时候，才进行调节框切换*/
            case KEY_TYPE_LONG_UP:
            case KEY_TYPE_HOLD_UP:
            hour = pTime->time_digit_info[0];
            minute = pTime->time_digit_info[1];
            ampm = pTime->time_digit_info[2];
            pAdjust->adjust_time.sys_time.hour = hour;
            pAdjust->adjust_time.sys_time.minute = minute;

            if ((pAdjust->adjust_style == ADJ_STYLE_TIME) && (pAdjust->time_mode == TIME_MODE_12))
            {
                _time_12_to_24(&(pAdjust->adjust_time.sys_time), ampm);
            }

            pAdjust->callback(pAdjust, WIDGET_SIG_ADJUST_OK);
            break;

            default:
            break;
        }
        break;

        case KEY_RETURN:
        if ((pmsg->type == KEY_TYPE_SHORT_UP) || (pmsg->type == KEY_TYPE_LONG_UP) || (pmsg->type == KEY_TYPE_SHORT_UP))
        {
            pAdjust->callback(pAdjust, WIDGET_SIG_ADJUST_CANCEL);
        }
        break;

        /* 当按下MENU键的时候，切换时间调节的内容
         调节的方向是 从AM(PM) - 分钟 - 小时
         */
        case KEY_MENU:
        switch (pmsg->type)
        {
            case KEY_TYPE_DOWN:
            pTime->time_index++;
            if ((pAdjust->adjust_style == ADJ_STYLE_TIME) && (pAdjust->time_mode == TIME_MODE_12))
            {
                _qualify(&(pTime->time_index), 0, TIME_ITEM_MAX - 1);
            }
            else
            {
                _qualify(&(pTime->time_index), 0, TIME_ITEM_MAX - 2);
            }
            adjust_paint(pAdjust);
            break;

            default:
            break;
        }
        break;

        /* 对所要调节的对象进行"加"操作
         */
        case KEY_PREV:
        switch (pmsg->type)
        {
            case KEY_TYPE_DOWN:
            break;

            case KEY_TYPE_HOLD:
            case KEY_TYPE_SHORT_UP:
            case KEY_TYPE_LONG_UP:
            case KEY_TYPE_HOLD_UP:
            _handle_time_data(pAdjust, -1);
            pAdjust->pAdjPrivate->show_bg = false;
            adjust_paint(pAdjust);
            clear_key_msg_queue(-1, -1);
            break;

            default:
            break;
        }
        break;

        /* 对所要调节的对象进行“减”操作*/
        case KEY_NEXT:
        switch (pmsg->type)
        {
            case KEY_TYPE_DOWN:
            break;

            case KEY_TYPE_HOLD:
            case KEY_TYPE_SHORT_UP:
            case KEY_TYPE_LONG_UP:
            case KEY_TYPE_HOLD_UP:
            _handle_time_data(pAdjust, 1);
            pAdjust->pAdjPrivate->show_bg = false;
            adjust_paint(pAdjust);
            clear_key_msg_queue(-1, -1);
            break;

            default:
            break;
        }
        break;

        default:
        break;
    }

    return true;
}

/************************************************************************************/
/*!
 * \par  Description:
 *     当控件正在调节日期的时候，处理按键信息
 * \ingroup common_adjust
 * \param[in]    pAdjust:  指向adjust控件结构体的指针
 * \param[in]    pmsg: 指向按键消息结构体的指针
 * \return       the result
 * \retval       成功返回true, 失败返回false
 ************************************************************************************/
static bool _key_handle_date(adjust_t *pAdjust, key_event_t *pmsg)
{
    int year;
    int month;
    int day;
    date_para_t *pDate;
    pDate = &(pAdjust->pAdjPrivate->adjust_date.date_para);

    switch (pmsg->val)
    {
        case KEY_OK:
        switch (pmsg->type)
        {
            case KEY_TYPE_SHORT_UP: /*当按键抬起的时候，才进行调节框切换*/
            case KEY_TYPE_LONG_UP:
            case KEY_TYPE_HOLD_UP:
            year = pDate->date_digit_info[0];
            month = pDate->date_digit_info[1];
            day = pDate->date_digit_info[2];
            pAdjust->adjust_date.sys_date.year = year;
            pAdjust->adjust_date.sys_date.month = month;
            pAdjust->adjust_date.sys_date.day = day;

            pAdjust->callback(pAdjust, WIDGET_SIG_ADJUST_OK);
            break;

            default:
            break;
        }
        break;

        case KEY_RETURN:
        if ((pmsg->type == KEY_TYPE_SHORT_UP) || (pmsg->type == KEY_TYPE_LONG_UP) || (pmsg->type == KEY_TYPE_HOLD_UP))
        {
            pAdjust->callback(pAdjust, WIDGET_SIG_ADJUST_CANCEL);
        }
        break;
        /* 当按下PREV键的时候，切换调节对象
         切换方向是 日 － 月 － 年
         */
        /* 当按下MENU键的时候，切换时间调节的内容
         调节的方向是 从AM(PM) - 分钟 - 小时
         */
        case KEY_MENU:
        switch (pmsg->type)
        {
            case KEY_TYPE_DOWN:
            pDate->date_index++;
            _qualify(&(pDate->date_index), 0, DATE_ITEM_MAX - 1);
            pAdjust->pAdjPrivate->show_bg = false;
            adjust_paint(pAdjust);
            break;

            default:
            break;
        }
        break;

        /* 对当前选中的调节项进行加操作 */
        case KEY_NEXT:
        switch (pmsg->type)
        {
            case KEY_TYPE_DOWN:
            break;

            case KEY_TYPE_HOLD:
            case KEY_TYPE_SHORT_UP:
            case KEY_TYPE_LONG_UP:
            case KEY_TYPE_HOLD_UP:
            _handle_date_data(pAdjust, 1);
            pAdjust->pAdjPrivate->show_bg = false;
            adjust_paint(pAdjust);
            clear_key_msg_queue(-1, -1);
            break;

            default:
            break;
        }
        break;

        /*对当前选中的调节项进行减操作*/
        case KEY_PREV:
        switch (pmsg->type)
        {
            case KEY_TYPE_DOWN:
            break;

            case KEY_TYPE_HOLD:
            case KEY_TYPE_SHORT_UP:
            case KEY_TYPE_LONG_UP:
            case KEY_TYPE_HOLD_UP:
            _handle_date_data(pAdjust, -1);
            pAdjust->pAdjPrivate->show_bg = false;
            adjust_paint(pAdjust);
            clear_key_msg_queue(-1, -1);
            break;

            default:
            break;
        }
        break;

        default:
        break;
    }

    return true;
}

/************************************************************************************/
/*!
 * \par  Description:
 *     当控件在“ON”和OFF两种状态之间切换的时候
 *     对按键信息进行处理 
 * \ingroup common_adjust
 * \param[in]    pAdjust:  指向adjust控件结构体的指针
 * \param[in]    pmsg: 指向按键消息结构体的指针
 * \return       the result
 * \retval       成功返回true, 失败返回false
 ************************************************************************************/
static bool _key_handle_switch(adjust_t *pAdjust, key_event_t *pmsg)
{
    adjust_switch_init_t *pSwitchPara;
    pSwitchPara = &(pAdjust->adjust_switch);
    switch (pmsg->val)
    {
        case KEY_OK: /* shift between two elements */
        switch (pmsg->type)
        {
            case KEY_TYPE_SHORT_UP:/*当按键抬起的时候，才进行调节框切换*/
            case KEY_TYPE_LONG_UP:
            case KEY_TYPE_HOLD_UP:
            pAdjust->callback(pAdjust, WIDGET_SIG_ADJUST_OK);
            break;

            default:
            break;
        }
        break;

        case KEY_RETURN:
        if ((pmsg->type == KEY_TYPE_SHORT_UP) || (pmsg->type == KEY_TYPE_LONG_UP) || (pmsg->type == KEY_TYPE_HOLD_UP))
        {
            pAdjust->callback(pAdjust, WIDGET_SIG_ADJUST_CANCEL);
        }
        break;

        /* 按下PREV键，切换到"ON"状态*/
        case KEY_PREV:
        switch (pmsg->type)
        {
            case KEY_TYPE_DOWN:
            pAdjust->pAdjPrivate->adj_key_sta.key_down_status = KEY_PRESSED;
            pAdjust->pAdjPrivate->show_left_arrow = false;
            adjust_paint(pAdjust);
            break;

            case KEY_TYPE_SHORT_UP:
            case KEY_TYPE_LONG_UP:
            case KEY_TYPE_HOLD_UP:
            pAdjust->pAdjPrivate->adj_key_sta.key_down_status = KEY_RELEASED;
            pSwitchPara->on_off_status = ADJUST_ON;
            pAdjust->pAdjPrivate->show_disp_bg = false;
            pAdjust->pAdjPrivate->show_left_arrow = false;
            pAdjust->pAdjPrivate->adjust_onoff.show_switchon_bg = false;
            pAdjust->pAdjPrivate->adjust_onoff.show_texton = false;
            pAdjust->pAdjPrivate->adjust_onoff.show_textoff = false;
            adjust_paint(pAdjust);
            pAdjust->callback(pAdjust, WIDGET_SIG_ADJUST_SWITCHON);
            break;

            default:
            break;
        }
        break;

        case KEY_NEXT:
        switch (pmsg->type)
        {
            case KEY_TYPE_DOWN:
            pAdjust->pAdjPrivate->adj_key_sta.key_up_status = KEY_PRESSED;
            pAdjust->pAdjPrivate->show_right_arrow = false;
            adjust_paint(pAdjust);
            break;

            case KEY_TYPE_SHORT_UP:
            case KEY_TYPE_LONG_UP:
            case KEY_TYPE_HOLD_UP:
            pAdjust->pAdjPrivate->adj_key_sta.key_up_status = KEY_RELEASED;
            pSwitchPara->on_off_status = ADJUST_OFF;
            pAdjust->pAdjPrivate->show_disp_bg = false;
            pAdjust->pAdjPrivate->show_right_arrow = false;
            pAdjust->pAdjPrivate->adjust_onoff.show_switchoff_bg = false;
            pAdjust->pAdjPrivate->adjust_onoff.show_texton = false;
            pAdjust->pAdjPrivate->adjust_onoff.show_textoff = false;

            adjust_paint(pAdjust);
            pAdjust->callback(pAdjust, WIDGET_SIG_ADJUST_SWITCHOFF);
            break;

            default:
            break;
        }
        break;

        default:
        break;
    }

    return true;
}

/************************************************************************************/
/*!
 * \par  Description:
 *     处理adjust 控件的按键消息
 * \ingroup common_adjust
 * \param[in]    pAdjust:  指向adjust控件结构体的指针
 * \param[in]    pmsg: 指向按键消息结构体的指针
 * \return       the result
 * \retval       成功返回true, 失败返回false
 ************************************************************************************/
bool adjust_key(adjust_t *pAdjust, key_event_t *pmsg)
{
    adjust_style_e element_type;
    element_type = pAdjust->adjust_style;
    if (element_type == ADJ_STYLE_TIME)
    {
        _key_handle_time(pAdjust, pmsg);
    }
    else if (element_type == ADJ_STYLE_DATE)
    {
        _key_handle_date(pAdjust, pmsg);
    }
    else if (element_type == ADJ_STYLE_SWITCH)
    {
        _key_handle_switch(pAdjust, pmsg);
    }
    else if (element_type == ADJ_STYLE_TEXTDIGIT)
    {
        _key_handle_digit_text(pAdjust, pmsg);
    }
    else// if (element_type == ADJ_STYLE_RECLEN)
    {
        _key_handle_time(pAdjust, pmsg);
    }
    return true;
}

/************************************************************************************/
/*!
 * \par  Description:
 *     创建adjust控件
 * \ingroup common_adjust
 * \param[in]    adjust_para:  通过该结构体向adjust控件传入参数
 * \param[in]    adjust_style: adjust控件的风格
 * \param[in]   callback: adjust控件的回调函数
 * \return       the result
 * \retval       返回指向adjust控件的结构体指针，NULL:创建失败
 ************************************************************************************/
adjust_t* adjust_create(void *adjust_para, adjust_style_e adjust_style, adjust_cb_t callback)
{
    bool result = true;
    int i;
    int hour,minute,year,month,day;
    int time_mode;

    rtc_time_t *pTime;
    rtc_date_t *pDate;
    adjust_switch_init_t *pOnOff;
    adjust_textdigit_init_t *pTD;
    char *p_str;

    sty_rect_t canvas_rect;
    adjust_t *pAdjust;

    if ((NULL == adjust_para) || (NULL == callback))
    {
        return NULL;
    }

    //malloc data
    pAdjust = (adjust_t *) malloc(sizeof(adjust_t));
    if (pAdjust == NULL)
    {
        print_err("malloc adjust_t failed\n");
        return NULL;
    }

    /* clear pAdjust */
    memset(pAdjust, 0, sizeof(adjust_t));
    print_dbg();

    /* malloc adjust private pAdjust area */
    pAdjust->pAdjPrivate = (struct adjust_private_s *) (malloc(sizeof(struct adjust_private_s)));
    if (NULL == pAdjust->pAdjPrivate)
    {
        print_err("malloc adjust private data error\n");
        goto adjust_err_handle;
    }

    memset(pAdjust->pAdjPrivate, 0, sizeof(struct adjust_private_s));

    /* set the style of adjust widget */
    pAdjust->adjust_style = adjust_style;

    pAdjust->callback = callback;
    pAdjust->canvas_id = -1;

    if (get_config(CFG_TIME_FORMAT, (char *) &time_mode, sizeof(time_mode)) < 0)
    {
        print_err("get_config(CFG_TIME_FORMAT) failed!");
    }

    if (12 == time_mode)
    {
        pAdjust->time_mode = TIME_MODE_12;
    }
    else
    {
        pAdjust->time_mode = TIME_MODE_24;
    }

    if (ADJ_STYLE_TIME == adjust_style)
    {
        memcpy(&(pAdjust->adjust_time), adjust_para, sizeof(adjust_time_init_t));
        pTime = &(pAdjust->adjust_time.sys_time);
        hour = pTime->hour;

        /*24小时制*/
        _qualify(&hour, 0, 23);

        if (TIME_MODE_12 == pAdjust->time_mode)
        {
            if (hour > 12)
            {
                hour -= 12;
                pAdjust->pAdjPrivate->adjust_time.time_para.time_digit_info[2] = TIME_PM;
            }
            else if (hour == 0)
            {
                hour = 12;
                pAdjust->pAdjPrivate->adjust_time.time_para.time_digit_info[2] = TIME_AM;
            }
            else if (hour == 12)
            {
                pAdjust->pAdjPrivate->adjust_time.time_para.time_digit_info[2] = TIME_PM;
            }
            else
            {
                pAdjust->pAdjPrivate->adjust_time.time_para.time_digit_info[2] = TIME_AM;
            }
        }

        pAdjust->pAdjPrivate->adjust_time.time_para.time_digit_info[0] = hour;
        minute = pTime->minute;
        _qualify(&minute, 0, 59);
        pAdjust->pAdjPrivate->adjust_time.time_para.time_digit_info[1] = minute;
        pAdjust->pAdjPrivate->adjust_time.time_para.time_index = 0;
        pAdjust->pAdjPrivate->str_title_id = pAdjust->adjust_time.str_title_id;
    }
    if (ADJ_STYLE_RECLEN == adjust_style)
    {
        memcpy(&(pAdjust->adjust_time), adjust_para, sizeof(adjust_time_init_t));
        pTime = &(pAdjust->adjust_time.sys_time);
        //if(NULL != pTime)
        //{
        pAdjust->pAdjPrivate->adjust_time.time_para.time_digit_info[0] = pTime->hour;
        pAdjust->pAdjPrivate->adjust_time.time_para.time_digit_info[1] = pTime->minute;
        //}
        pAdjust->pAdjPrivate->str_title_id = pAdjust->adjust_time.str_title_id;
    }
    if (ADJ_STYLE_DATE == adjust_style)
    {
        memcpy(&(pAdjust->adjust_date), adjust_para, sizeof(adjust_date_init_t));
        pDate = &(pAdjust->adjust_date.sys_date);
        year = pDate->year;
        _qualify(&year, MIN_YEAR, MAX_YEAR);

        pAdjust->pAdjPrivate->adjust_date.date_para.date_digit_info[0] = year;
        month = pDate->month;
        if (month == 0)
        {
            month = 1;
        }

        pAdjust->pAdjPrivate->adjust_date.date_para.date_digit_info[1] = month;
        day = pDate->day;
        if (day == 0)
        {
            day = 1;
        }
        pAdjust->pAdjPrivate->adjust_date.date_para.date_digit_info[2] = day;
        pAdjust->pAdjPrivate->str_title_id = pAdjust->adjust_date.str_title_id;
    }
    if (ADJ_STYLE_SWITCH == adjust_style)
    {
        memcpy(&(pAdjust->adjust_switch), adjust_para, sizeof(adjust_switch_init_t));
        pOnOff = &(pAdjust->adjust_switch);
        pAdjust->pAdjPrivate->str_title_id = pOnOff->str_title_id;
        if (ADJUST_ON == pOnOff->on_off_status)
        {
            pAdjust->pAdjPrivate->adjust_onoff.show_switchoff_bg = true;
        }
        else
        {
            pAdjust->pAdjPrivate->adjust_onoff.show_switchon_bg = true;
        }
    }
    if (ADJ_STYLE_TEXTDIGIT == adjust_style)
    {
        pTD = (adjust_textdigit_init_t *) adjust_para;
        memcpy(&(pAdjust->adjust_digit), pTD, sizeof(adjust_textdigit_init_t));
        if (pTD->str_disp[0] != NULL)
        {
            for (i = 0; i < MAX_DISP_STRING; i++)
            {
                p_str = pTD->str_disp[i];
                if (p_str == NULL)
                {
                    break;
                }
                pAdjust->adjust_digit.str_disp[i] = (char *) malloc(MAX_DISP_STRING);
                strcpy(pAdjust->adjust_digit.str_disp[i], p_str);
            }
        }
        pTD = &(pAdjust->adjust_digit);
        pAdjust->pAdjPrivate->str_title_id = pTD->str_title_id;
    }

    result = adjust_load_style(pAdjust);

    if (result == false)
    {
        print_dbg("adjust load style failed \n");
        goto adjust_err_handle;
    }

    //create canvas
    canvas_rect.x = pAdjust->adjust_resgroup->x;
    canvas_rect.y = pAdjust->adjust_resgroup->y;
    canvas_rect.width = pAdjust->adjust_resgroup->width;
    canvas_rect.height = pAdjust->adjust_resgroup->height;

    pAdjust->canvas_id = canvas_create(&canvas_rect, TRANSPARENT);
    if (-1 == pAdjust->canvas_id)
    {
        print_err("adjust create canvas error\n");
        goto adjust_err_handle;
    }

    adjust_paint(pAdjust);
    cur_adjust = pAdjust;
    return pAdjust;

    adjust_err_handle: free(pAdjust->pAdjPrivate);
    free(pAdjust);
    return NULL;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *     删除一个adjust控件
 * \ingroup common_adjust
 * \param [in] pAdjust: adjust控件句柄
 * \return  成功返回true, 失败返回false
 *******************************************************************************/
bool adjust_delete(adjust_t *pAdjust)
{
    int i;
    if (pAdjust != NULL)
    {
        //delete canvas
        if (pAdjust->canvas_id != 0)
        {
            canvas_delete(pAdjust->canvas_id);
            pAdjust->canvas_id = 0;
        }

        for (i = 0; i < MAX_DISP_STRING; i++)
        {
            if ((pAdjust->adjust_digit.str_disp[i]) != NULL)
            {
                free(pAdjust->adjust_digit.str_disp[i]);
            }
            else
            {
                break;
            }
            pAdjust->adjust_digit.str_disp[i] = NULL;
        }
        free(pAdjust->pAdjPrivate);
        free(pAdjust);
        pAdjust = NULL;
    }
    printf("adust_delete over\n");
    return true;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *     加载控件资源
 * \ingroup common_adjust
 * \param [in] pAdjust: adjust控件句柄
 * \return  成功返回true, 失败返回false
 *******************************************************************************/
bool adjust_load_style(adjust_t *pAdjust)
{
    scene_t* scene; //!< commonui scene  
    resgroup_resource_t *temp;
    sty_rect_t rect;

    //get root resource
    print_dbg("begin adjust_load_style\n");

    pAdjust->pAdjPrivate->show_bg = false;
    pAdjust->pAdjPrivate->show_title = false;
    pAdjust->pAdjPrivate->show_left_arrow = false;
    pAdjust->pAdjPrivate->show_right_arrow = false;
    pAdjust->pAdjPrivate->show_disp_bg = false;

    pAdjust->pAdjPrivate->text_digit.show_text = false;

    if (pAdjust->adjust_switch.on_off_status == ADJUST_ON)
    {
        pAdjust->pAdjPrivate->adjust_onoff.show_switchon_bg = false;
    }
    else
    {
        pAdjust->pAdjPrivate->adjust_onoff.show_switchoff_bg = false;
    }

    pAdjust->pAdjPrivate->adjust_onoff.show_texton = false;
    pAdjust->pAdjPrivate->adjust_onoff.show_textoff = false;

    pAdjust->pAdjPrivate->adjust_time.show_hour = false;
    pAdjust->pAdjPrivate->adjust_time.show_min = false;
    pAdjust->pAdjPrivate->adjust_time.show_ampm = false;
    pAdjust->pAdjPrivate->adjust_time.show_time_sep = false;

    pAdjust->pAdjPrivate->adjust_date.show_left_arrow = false;
    pAdjust->pAdjPrivate->adjust_date.show_right_arrow = false;
    pAdjust->pAdjPrivate->adjust_date.show_year = false;
    pAdjust->pAdjPrivate->adjust_date.show_month = false;
    pAdjust->pAdjPrivate->adjust_date.show_day = false;
    pAdjust->pAdjPrivate->adjust_date.show_date_sep = false;
    pAdjust->pAdjPrivate->adjust_date.show_date_sep1 = false;

    scene = sty_get_common_scene();
    if (scene == NULL)
    {
        printf("sty_get_common_scene failed\n");
        return false;
    }

    temp = (resgroup_resource_t *) get_scene_child(scene, ADJUST_RESGROUP);

    if (pAdjust->adjust_style == ADJ_STYLE_TEXTDIGIT)
    {
        pAdjust->adjust_resgroup = (resgroup_resource_t *) get_resgroup_child(temp, ADJUST_TEXT_DIGIT);

        pAdjust->pAdjPrivate->picregion_adjust_bg = (picregion_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, PICREGION_BG);

        pAdjust->pAdjPrivate->str_adjust_title = (string_resource_t *) get_resgroup_child(pAdjust->adjust_resgroup,
                STR_TITLE);

        pAdjust->pAdjPrivate->picregion_left_arrow = (picregion_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, PICREGION_LARROW);

        pAdjust->pAdjPrivate->picregion_right_arrow = (picregion_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, PICREGION_RARROW);

        pAdjust->pAdjPrivate->pic_disp_bg = (picture_resource_t *) get_resgroup_child(pAdjust->adjust_resgroup,
                PIC_DISP_BG);

        pAdjust->pAdjPrivate->text_digit.str_adjust_text = (string_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, STR_DISP_TEXT);

    }
    else if (pAdjust->adjust_style == ADJ_STYLE_SWITCH)
    {
        pAdjust->adjust_resgroup = (resgroup_resource_t *) get_resgroup_child(temp, ADJUST_SWITCH);

        pAdjust->pAdjPrivate->picregion_adjust_bg = (picregion_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, PICREGION_BG);

        pAdjust->pAdjPrivate->str_adjust_title = (string_resource_t *) get_resgroup_child(pAdjust->adjust_resgroup,
                STR_TITLE);

        pAdjust->pAdjPrivate->picregion_left_arrow = (picregion_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, PICREGION_LARROW);

        pAdjust->pAdjPrivate->picregion_right_arrow = (picregion_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, PICREGION_RARROW);

        pAdjust->pAdjPrivate->pic_disp_bg = (picture_resource_t *) get_resgroup_child(pAdjust->adjust_resgroup,
                PIC_DISP_BG);

        pAdjust->pAdjPrivate->adjust_onoff.pic_switch_on_bg = (picture_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, PIC_SWITCH_ON_BG);

        pAdjust->pAdjPrivate->adjust_onoff.pic_switch_off_bg = (picture_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, PIC_SWITCH_OFF_BG);

        pAdjust->pAdjPrivate->adjust_onoff.str_text_on = (string_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, STR_ON);

        pAdjust->pAdjPrivate->adjust_onoff.str_text_off = (string_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, STR_OFF);

    }
    else if (pAdjust->adjust_style == ADJ_STYLE_TIME)
    {
        if (pAdjust->time_mode == TIME_MODE_12)
        {
            pAdjust->adjust_resgroup = (resgroup_resource_t *) get_resgroup_child(temp, ADJUST_TIME_12);
        }
        else
        {
            pAdjust->adjust_resgroup = (resgroup_resource_t *) get_resgroup_child(temp, ADJUST_TIME_24);
        }

        pAdjust->pAdjPrivate->picregion_adjust_bg = (picregion_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, PICREGION_BG);

        pAdjust->pAdjPrivate->str_adjust_title = (string_resource_t *) get_resgroup_child(pAdjust->adjust_resgroup,
                STR_TIME_TITLE);

        if ((pAdjust->pAdjPrivate->str_title_id == 0) && (pAdjust->pAdjPrivate->str_adjust_title != NULL))
        {
            pAdjust->pAdjPrivate->str_title_id = pAdjust->pAdjPrivate->str_adjust_title->str_id;
        }
        pAdjust->pAdjPrivate->picregion_left_arrow = (picregion_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, PICREGION_LARROW);

        pAdjust->pAdjPrivate->picregion_right_arrow = (picregion_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, PICREGION_RARROW);

        pAdjust->pAdjPrivate->adjust_time.picregion_ampm = (picregion_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, PICREGION_AMPM);

        pAdjust->pAdjPrivate->adjust_time.picregion_hour_high = (picregion_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, PICREGION_HOUR_HIGH);

        pAdjust->pAdjPrivate->adjust_time.picregion_hour_low = (picregion_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, PICREGION_HOUR_LOW);

        pAdjust->pAdjPrivate->adjust_time.picregion_min_high = (picregion_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, PICREGION_MIN_HIGH);

        pAdjust->pAdjPrivate->adjust_time.picregion_min_low = (picregion_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, PICREGION_MIN_LOW);

        pAdjust->pAdjPrivate->adjust_time.pic_time_sep = (picture_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, PIC_TIME_SEP);
    }
    else if (pAdjust->adjust_style == ADJ_STYLE_DATE)
    {
        pAdjust->adjust_resgroup = (resgroup_resource_t *) get_resgroup_child(temp, ADJUST_DATE);

        pAdjust->pAdjPrivate->picregion_adjust_bg = (picregion_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, PICREGION_BG);

        pAdjust->pAdjPrivate->str_adjust_title = (string_resource_t *) get_resgroup_child(pAdjust->adjust_resgroup,
                STR_DATE_TITLE);

        if ((pAdjust->pAdjPrivate->str_title_id == 0) && (pAdjust->pAdjPrivate->str_adjust_title != NULL))
        {
            pAdjust->pAdjPrivate->str_title_id = pAdjust->pAdjPrivate->str_adjust_title->str_id;
        }

        pAdjust->pAdjPrivate->picregion_left_arrow = (picregion_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, PICREGION_LARROW);

        pAdjust->pAdjPrivate->picregion_right_arrow = (picregion_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, PICREGION_RARROW);

        pAdjust->pAdjPrivate->adjust_date.picregion_year4 = (picregion_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, PICREGION_YEAR4);

        pAdjust->pAdjPrivate->adjust_date.picregion_year3 = (picregion_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, PICREGION_YEAR3);

        pAdjust->pAdjPrivate->adjust_date.picregion_year2 = (picregion_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, PICREGION_YEAR2);

        pAdjust->pAdjPrivate->adjust_date.picregion_year1 = (picregion_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, PICREGION_YEAR1);

        pAdjust->pAdjPrivate->adjust_date.picregion_month_high = (picregion_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, PICREGION_MONTH_HIGH);

        pAdjust->pAdjPrivate->adjust_date.picregion_month_low = (picregion_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, PICREGION_MONTH_LOW);

        pAdjust->pAdjPrivate->adjust_date.picregion_day_high = (picregion_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, PICREGION_DAY_HIGH);

        pAdjust->pAdjPrivate->adjust_date.picregion_day_low = (picregion_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, PICREGION_DAY_LOW);

        pAdjust->pAdjPrivate->adjust_date.pic_date_sep = (picture_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, PIC_DATE_SEP);

        pAdjust->pAdjPrivate->adjust_date.pic_date_sep1 = (picture_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, PIC_DATE_SEP1);

    }
    else// if (pAdjust->adjust_style == ADJ_STYLE_RECLEN)
    {
        pAdjust->adjust_resgroup = (resgroup_resource_t *) get_resgroup_child(temp, ADJUST_RECORD_LENGTH);

        pAdjust->pAdjPrivate->picregion_adjust_bg = (picregion_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, PICREGION_BG);

        pAdjust->pAdjPrivate->str_adjust_title = (string_resource_t *) get_resgroup_child(pAdjust->adjust_resgroup,
                STR_RECORD_TITLE);

        if ((pAdjust->pAdjPrivate->str_title_id == 0) && (pAdjust->pAdjPrivate->str_adjust_title != NULL))
        {
            pAdjust->pAdjPrivate->str_title_id = pAdjust->pAdjPrivate->str_adjust_title->str_id;
        }

        pAdjust->pAdjPrivate->picregion_left_arrow = (picregion_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, PICREGION_LARROW);

        pAdjust->pAdjPrivate->picregion_right_arrow = (picregion_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, PICREGION_RARROW);

        pAdjust->pAdjPrivate->adjust_time.picregion_hour_high = (picregion_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, PICREGION_HOUR_HIGH);

        pAdjust->pAdjPrivate->adjust_time.picregion_hour_low = (picregion_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, PICREGION_HOUR_LOW);

        pAdjust->pAdjPrivate->adjust_time.picregion_min_high = (picregion_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, PICREGION_MIN_HIGH);

        pAdjust->pAdjPrivate->adjust_time.picregion_min_low = (picregion_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, PICREGION_MIN_LOW);

        pAdjust->pAdjPrivate->adjust_time.pic_time_sep = (picture_resource_t *) get_resgroup_child(
                pAdjust->adjust_resgroup, PIC_TIME_SEP);
    }

    //canvas resize    
    if ((pAdjust->canvas_id != -1) && (pAdjust->adjust_resgroup != NULL))
    {
        rect.x = pAdjust->adjust_resgroup->absolute_x;
        rect.y = pAdjust->adjust_resgroup->absolute_y;
        rect.width = pAdjust->adjust_resgroup->width;
        rect.height = pAdjust->adjust_resgroup->height;

        //set canvas region
        canvas_set_region(pAdjust->canvas_id, &rect);
        canvas_erase_all(pAdjust->canvas_id);
    }

    return true;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *     处理控件消息
 * \ingroup common_adjust
 * \param [in] pAdjust: adjust控件句柄
 * \param [in] pmsg: 按键消息
 * \return  成功返回true, 失败返回false
 *******************************************************************************/
bool adjust_proc_gui(adjust_t *pAdjust, gui_wm_message_t *pmsg)
{
    switch (pmsg->msgid)
    {
        case WM_MSG_PAINT:
        break;

        case WM_MSG_KEY:
        adjust_key(pAdjust, (key_event_t *) (pmsg->data.p));
        break;

        case WM_MSG_TOUCH:
        break;

        default:
        break;
    }
    return true;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *     从adjust控件读取当前的调整值
 * \ingroup common_adjust
 * \param [out] cur_val: 读取的调整值放在这里
 * \return  成功返回true, 失败返回false
 *******************************************************************************/
bool adjust_get_current_val(int *cur_val)
{
    if (NULL != cur_adjust)
    {
        *cur_val = cur_adjust->adjust_digit.cur_val;
        return true;
    }
    else
    {
        return false;
    }
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *     从adjust控件读取当前设置内容的索引值
 * \ingroup common_adjust
 * \param [out] cur_index: 该地址存放调节内容的索引值
 * \return  成功返回true, 失败返回false
 *******************************************************************************/
bool adjust_get_current_index(int *cur_index)
{
    if (NULL != cur_adjust)
    {
        *cur_index = cur_adjust->adjust_digit.cur_val;
        return true;
    }
    else
    {
        return false;
    }
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *     从adjust读取开关状态
 * \ingroup common_adjust
 * \param [out] on_off_sta: 从这里返回adjust控件调整的开关状态
 * \return  成功返回true, 失败返回false
 *******************************************************************************/
bool adjust_get_switch_status(adj_switch_mode_e *on_off_sta)
{
    if (NULL != cur_adjust)
    {
        *on_off_sta = cur_adjust->adjust_switch.on_off_status;
        return true;
    }
    else
    {
        return false;
    }
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *     读取adjust控件当前的设定值
 * \ingroup common_adjust
 * \param [out] set_time: 当前调整的时间，在adjust控件上按下"OK"以后生效
 * \return  成功返回true, 失败返回false
 *******************************************************************************/
bool adjust_get_setting_time(rtc_time_t *set_time)
{
    if (NULL != cur_adjust)
    {
        memcpy(set_time, &(cur_adjust->adjust_time.sys_time), sizeof(rtc_time_t));
        return true;
    }
    else
    {
        return false;
    }
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *     读取adjust控件当前的设定值
 * \ingroup common_adjust
 * \param [out] set_date: 当前调整的日期，在adjust控件上按下"OK"以后生效
 * \return  成功返回true, 失败返回false
 *******************************************************************************/
bool adjust_get_setting_date(rtc_date_t *set_date)
{
    if (NULL != cur_adjust)
    {
        memcpy(set_date, &(cur_adjust->adjust_date.sys_date), sizeof(rtc_date_t));
        return true;
    }
    else
    {
        return false;
    }
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *     读取adjust控件当前的设定值
 * \ingroup common_adjust
 * \param [out] record_length: 当前设置的录像长度，在adjust控件上按下"OK"以后生效
 * \return  成功返回true, 失败返回false
 *******************************************************************************/
bool adjust_get_recordtime(rtc_time_t *record_length)
{
    if (NULL != cur_adjust)
    {
        memcpy(record_length, &(cur_adjust->adjust_time.sys_time), sizeof(rtc_time_t));
        return true;
    }
    else
    {
        return false;
    }
}

