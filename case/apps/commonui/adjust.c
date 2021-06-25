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
 * \brief    ����
 * \author
 * \par      GENERAL DESCRIPTION:
 *               adjustr�ؼ�ʵ��
 * \par      EXTERNALIZED FUNCTIONS:
 *               �õ������ģ��
 * \version 1.0
 * \date  2009-4-22
 *******************************************************************************/
#include "adjust_private.h"
#include "commonui_sty.h"

#define   MIN_YEAR     (2000)
#define   MAX_YEAR     (2037)

/* ƽ��ÿ�������������� */
int month_normal_days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

/* ����û��������������*/
int month_leapyear_days[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

/*!
 *  \par adjust �ؼ�˽�����ݽṹ
 *  \li key_status_t : adjust�ؼ�����ʹ�õİ�����״̬
 *  \li time_date_parameter_t : ����͵���ʱ��������ص�����
 *  \li pic_res : ָ����Դ����ͼƬ��Դ��ָ������
 *  \li picregion_res : ָ����Դ����ͼƬ����Դ��ָ��
 *  \li string_resource_t : ָ����Դ�����ַ�����Դ��ָ��
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
 *     �ж����������Ƿ�������
 * \param[in]    y   ��������
 * \return       the result
 * \retval       true    ���óɹ�
 * \retval       false   ����ʧ��
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
 *     �ж�����������Ƿ����ƶ��ķ�Χ֮�ڣ������
 *     ���ڷ�Χ֮�ڣ��Զ����������ݳ������ֵ��ʱ��
 *     ���ڳ���С�ģ���С����С��ʱ�򣬵������ģ�
 *     ����ڴ�֮�䣬����Ҫ�ı�����ֵ
 * \param[in]    p_pAdjust    Ҫ���ڵ�����
 * \param[in]    min       ���ݵ�����
 * \param[in]    max       ���ݵ�����
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
 *     ��ȡһ�����а������������ڶ��·�Ҫ���������껹��ƽ��
 * \param[in]    leapyear  �Ƿ������� TRUE�������꣬FALSE����������
 * \param[in]    month     ������·�
 * \return       the result
 * \retval       �������а���������
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
 *     ���ݴ�������ڣ��õ���ǰ���õ��·��а���������
 * \param[in]    pTimeDate ��ָ����ָ��Ľṹ���а�����ʱ���������Ϣ 
 * \param[in]    month     ������·�
 * \return       the result
 * \retval       �������а���������
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
 *     ��12Сʱ��ת����24Сʱ��
 * \param[in]    time_val  ��ָ����ָ��Ľṹ���а�����ʱ����Ϣ 
 * \param[in]    ap_flag   0��AM  1:PM
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
 *     ���ؼ��������ڵ�ʱ�򣬴����������ص�����
 *     �������ڵ���ȷ����֤
 * \ingroup common_adjust
 * \param[in]    pAdjust:  ָ��adjust�ؼ��ṹ���ָ��
 * \param[in]    ��ǰ������������(1��1)
 * \return       the result
 * \retval       �ɹ�����true, ʧ�ܷ���false
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
        case 0: /* ������ */
        /* ��ӻ��߼�һ */
        pDate->date_digit_info[0] += step;
        /* �ж���ķ�Χ�ǲ�����2000��2099�ķ�Χ֮�� */
        _qualify(&(pDate->date_digit_info[0]), MIN_YEAR, MAX_YEAR);
        /* ��ȡ��ǰ�趨�·ݵ�������� */
        max_day = _get_max_day(pDate);
        /* �жϵ�ǰ�趨�����Ƿ�Խ�磬���Խ�磬������Ӧ�ĵ���*/
        if ((pDate->date_digit_info[2]) > max_day)
        {
            pDate->date_digit_info[2] = max_day;
        }
        break;

        case 1: /* ������ */
        pDate->date_digit_info[1] += step;
        _qualify(&(pDate->date_digit_info[1]), 1, 12);

        /*�������趨���·ݣ��Ե�ǰ���ս����ж�*/
        max_day = _get_max_day(pDate);
        if ((pDate->date_digit_info[2]) > max_day)
        {
            pDate->date_digit_info[2] = max_day;
        }
        break;

        case 2: /* ������ */
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
 *     ���ؼ�����ʱ���ʱ�򣬴����ʱ����ص�����
 *     ��������ʱ�����ȷ����֤
 * \ingroup common_adjust
 * \param[in]    pAdjust:  ָ��adjust�ؼ��ṹ���ָ��
 * \param[in]    pmsg: ָ�򰴼���Ϣ�ṹ���ָ��
 * \return       the result
 * \retval       �ɹ�����true, ʧ�ܷ���false
 ************************************************************************************/
static bool _handle_time_data(adjust_t *pAdjust, int step)
{
    time_para_t *pTime;
    int cur_index;

    pTime = &(pAdjust->pAdjPrivate->adjust_time.time_para);
    cur_index = pTime->time_index;

    switch (cur_index)
    {
        case 0: /*����Сʱ */
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

        case 1: /*�������� */
        pTime->time_digit_info[1] += step;
        _qualify(&(pTime->time_digit_info[1]), 0, 59);
        break;

        case 2: /*����AM��PM��־*/
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
 *     ���ؼ����ڵ������֣������ı���Ϣ��ʱ��
 *     ���ݰ�����״̬������ʾ�����ݡ����Ҹ��ݾ������
 *     ���ûص�������ִ����Ӧ�Ĳ�����
 * \ingroup common_adjust
 * \param[in]    pAdjust:  ָ��adjust�ؼ��ṹ���ָ��
 * \param[in]    pmsg: ָ�򰴼���Ϣ�ṹ���ָ��
 * \return       the result
 * \retval       �ɹ�����true, ʧ�ܷ���false
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
            case KEY_TYPE_SHORT_UP:/*������̧���ʱ�򣬲Ž��е��ڿ��л�*/
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

        /*��"PREV"�����µ�ʱ�򣬿�ʼ������ */
        case KEY_PREV:
        switch (pmsg->type)
        {
            case KEY_TYPE_DOWN: /* ��PREV���µ�ʱ�򣬼�¼PREV����ǰ��״̬ */
            pAdjust->pAdjPrivate->adj_key_sta.key_down_status = KEY_PRESSED;
            pAdjust->pAdjPrivate->show_left_arrow = false;
            adjust_paint(pAdjust);
            break;

            /* ��PREV������hold״̬��ʱ���趨PREV��״̬Ϊ�����¡���
             �����趨�Ĳ������С�����������һֱ�������趨����Сֵ
             ���Ҹ��ݵ�ǰ���趨ֵ�����л�ͼ����
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

        /* NEXT�����мӲ���*/
        case KEY_NEXT:
        switch (pmsg->type)
        {
            case KEY_TYPE_DOWN: /* ��PREV���µ�ʱ�򣬼�¼NEXT����ǰ��״̬ */
            pAdjust->pAdjPrivate->adj_key_sta.key_up_status = KEY_PRESSED;
            pAdjust->pAdjPrivate->show_right_arrow = false;
            adjust_paint(pAdjust);
            break;
            /* ��NEXT������hold״̬��ʱ���趨NEXT��״̬Ϊ�����¡���
             �����趨�Ĳ������С��ӡ�������һֱ�ӵ����趨�����ֵ
             ���Ҹ��ݵ�ǰ���趨ֵ�����л�ͼ����
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
 *     ���ؼ����ڵ���ʱ���ʱ�򣬴�������Ϣ
 * \ingroup common_adjust
 * \param[in]    pAdjust:  ָ��adjust�ؼ��ṹ���ָ��
 * \param[in]    pmsg: ָ�򰴼���Ϣ�ṹ���ָ��
 * \return       the result
 * \retval       �ɹ�����true, ʧ�ܷ���false
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
            case KEY_TYPE_SHORT_UP:/*������̧���ʱ�򣬲Ž��е��ڿ��л�*/
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

        /* ������MENU����ʱ���л�ʱ����ڵ�����
         ���ڵķ����� ��AM(PM) - ���� - Сʱ
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

        /* ����Ҫ���ڵĶ������"��"����
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

        /* ����Ҫ���ڵĶ�����С���������*/
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
 *     ���ؼ����ڵ������ڵ�ʱ�򣬴�������Ϣ
 * \ingroup common_adjust
 * \param[in]    pAdjust:  ָ��adjust�ؼ��ṹ���ָ��
 * \param[in]    pmsg: ָ�򰴼���Ϣ�ṹ���ָ��
 * \return       the result
 * \retval       �ɹ�����true, ʧ�ܷ���false
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
            case KEY_TYPE_SHORT_UP: /*������̧���ʱ�򣬲Ž��е��ڿ��л�*/
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
        /* ������PREV����ʱ���л����ڶ���
         �л������� �� �� �� �� ��
         */
        /* ������MENU����ʱ���л�ʱ����ڵ�����
         ���ڵķ����� ��AM(PM) - ���� - Сʱ
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

        /* �Ե�ǰѡ�еĵ�������мӲ��� */
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

        /*�Ե�ǰѡ�еĵ�������м�����*/
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
 *     ���ؼ��ڡ�ON����OFF����״̬֮���л���ʱ��
 *     �԰�����Ϣ���д��� 
 * \ingroup common_adjust
 * \param[in]    pAdjust:  ָ��adjust�ؼ��ṹ���ָ��
 * \param[in]    pmsg: ָ�򰴼���Ϣ�ṹ���ָ��
 * \return       the result
 * \retval       �ɹ�����true, ʧ�ܷ���false
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
            case KEY_TYPE_SHORT_UP:/*������̧���ʱ�򣬲Ž��е��ڿ��л�*/
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

        /* ����PREV�����л���"ON"״̬*/
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
 *     ����adjust �ؼ��İ�����Ϣ
 * \ingroup common_adjust
 * \param[in]    pAdjust:  ָ��adjust�ؼ��ṹ���ָ��
 * \param[in]    pmsg: ָ�򰴼���Ϣ�ṹ���ָ��
 * \return       the result
 * \retval       �ɹ�����true, ʧ�ܷ���false
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
 *     ����adjust�ؼ�
 * \ingroup common_adjust
 * \param[in]    adjust_para:  ͨ���ýṹ����adjust�ؼ��������
 * \param[in]    adjust_style: adjust�ؼ��ķ��
 * \param[in]   callback: adjust�ؼ��Ļص�����
 * \return       the result
 * \retval       ����ָ��adjust�ؼ��Ľṹ��ָ�룬NULL:����ʧ��
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

        /*24Сʱ��*/
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
 *     ɾ��һ��adjust�ؼ�
 * \ingroup common_adjust
 * \param [in] pAdjust: adjust�ؼ����
 * \return  �ɹ�����true, ʧ�ܷ���false
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
 *     ���ؿؼ���Դ
 * \ingroup common_adjust
 * \param [in] pAdjust: adjust�ؼ����
 * \return  �ɹ�����true, ʧ�ܷ���false
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
 *     ����ؼ���Ϣ
 * \ingroup common_adjust
 * \param [in] pAdjust: adjust�ؼ����
 * \param [in] pmsg: ������Ϣ
 * \return  �ɹ�����true, ʧ�ܷ���false
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
 *     ��adjust�ؼ���ȡ��ǰ�ĵ���ֵ
 * \ingroup common_adjust
 * \param [out] cur_val: ��ȡ�ĵ���ֵ��������
 * \return  �ɹ�����true, ʧ�ܷ���false
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
 *     ��adjust�ؼ���ȡ��ǰ�������ݵ�����ֵ
 * \ingroup common_adjust
 * \param [out] cur_index: �õ�ַ��ŵ������ݵ�����ֵ
 * \return  �ɹ�����true, ʧ�ܷ���false
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
 *     ��adjust��ȡ����״̬
 * \ingroup common_adjust
 * \param [out] on_off_sta: �����ﷵ��adjust�ؼ������Ŀ���״̬
 * \return  �ɹ�����true, ʧ�ܷ���false
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
 *     ��ȡadjust�ؼ���ǰ���趨ֵ
 * \ingroup common_adjust
 * \param [out] set_time: ��ǰ������ʱ�䣬��adjust�ؼ��ϰ���"OK"�Ժ���Ч
 * \return  �ɹ�����true, ʧ�ܷ���false
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
 *     ��ȡadjust�ؼ���ǰ���趨ֵ
 * \ingroup common_adjust
 * \param [out] set_date: ��ǰ���������ڣ���adjust�ؼ��ϰ���"OK"�Ժ���Ч
 * \return  �ɹ�����true, ʧ�ܷ���false
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
 *     ��ȡadjust�ؼ���ǰ���趨ֵ
 * \ingroup common_adjust
 * \param [out] record_length: ��ǰ���õ�¼�񳤶ȣ���adjust�ؼ��ϰ���"OK"�Ժ���Ч
 * \return  �ɹ�����true, ʧ�ܷ���false
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

