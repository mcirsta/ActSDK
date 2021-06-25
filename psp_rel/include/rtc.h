/*********************************************************************************
 *                                       USDK 1100
 *                            Module: RTC
 *                (c) Copyright 2003 - 2008, Actions Co,Ld.
 *                        All Right Reserved
 *
 * History:
 *      <author>      <time>       <version >    <desc>
 *       hmwei        2008/09/11         1.0         build this file
 ********************************************************************************/
/*!
 * \file   rtc.h
 * \brief
 *      Some data structure defines and function declaration.
 * \author hmwei
 * \par GENERAL DESCRIPTION:
 * \par EXTERNALIZED FUNCTIONS:
 *       null
 *
 *  Copyright(c) 2008-2012 Actions Semiconductor, All Rights Reserved.
 *
 * \version 1.0
 * \date  2008/09/11
 *******************************************************************************/

#ifndef __RTC_H__
#define __RTC_H__

#ifndef _ASSEMBLER_

/*!
 *  ioctl������ \n
 *  \li RTC_SET_DATE : ����RTC����
 *  \li RTC_SET_TIME : ����RTCʱ��
 *  \li RTC_GET_DATE_TIME : ��ȡRTC���ں�ʱ��
 *  \li RTC_GET_DRM_DATE_TIME : ��ȡDRM���ں�ʱ��
 *  \li RTC_SET_DRM_DATE_TIME : ����DRM���ں�ʱ��
 *  \li RTC_CHANGE_TO_STDSEC : ������ʱ��ת���ɱ�׼��
 *
 */
enum
{
    RTC_SET_DATE,
    RTC_SET_TIME,
    RTC_GET_DATE_TIME,
    RTC_GET_DRM_DATE_TIME,
    RTC_SET_DRM_DATE_TIME,
    RTC_CHANGE_TO_STDSEC
};

/*!
 * \brief
 *      rtc�������ݽṹ����
 */
typedef struct
{
    /*! ��*/
    int year;
    /*! ��*/
    int month;
    /*! ��*/
    int day;
    /*! ����*/
    int wday;//����
    /*! ���ͣ�Ŀǰ��֧��0��ʾ����*/
    char type;
} rtc_date_t;

/*!
 * \brief
 *      rtcʱ�����ݽṹ����
 */
typedef struct
{
    /*! Сʱ*/
    int hour;
    /*! ��*/
    int minute;
    /*! ��*/
    int second;
} rtc_time_t;

/*!
 * \brief
 *      rtc����ʱ�����ݽṹ����
 */
typedef struct
{
    /*! ����*/
    rtc_date_t date;
    /*! ʱ��*/
    rtc_time_t time;
    /*! ��׼��*/
    unsigned long stdsec;
} rtc_date_time_t;

#ifndef FUCNTION_SYMBOL //ͨ������ָ�뷽ʽ���ýӿ�ʱ��������FUCNTION_SYMBOL�궨�壬�����������ظ��������
extern int rtc_set_time(rtc_time_t *time);
extern int rtc_set_date(rtc_date_t *date);
extern int rtc_get_dateandtime(rtc_time_t *time, rtc_date_t *date);
extern int rtc_get_drm_dateandtime(rtc_time_t *drmtime, rtc_date_t *drmdate);
extern int rtc_set_drm_dateandtime(rtc_time_t *drmtime, rtc_date_t *drmdate);
extern unsigned long rtc_change_to_stdsec(rtc_time_t *time, rtc_date_t *date);

#endif /*FUCNTION_SYMBOL*/

#endif /*_ASSEMBLER_*/

#endif  /* __RTC_H__ */
