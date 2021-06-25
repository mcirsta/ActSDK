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
 *  ioctl命令字 \n
 *  \li RTC_SET_DATE : 设置RTC日期
 *  \li RTC_SET_TIME : 设置RTC时间
 *  \li RTC_GET_DATE_TIME : 获取RTC日期和时间
 *  \li RTC_GET_DRM_DATE_TIME : 获取DRM日期和时间
 *  \li RTC_SET_DRM_DATE_TIME : 设置DRM日期和时间
 *  \li RTC_CHANGE_TO_STDSEC : 将日期时间转换成标准秒
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
 *      rtc日期数据结构定义
 */
typedef struct
{
    /*! 年*/
    int year;
    /*! 月*/
    int month;
    /*! 日*/
    int day;
    /*! 星期*/
    int wday;//星期
    /*! 类型，目前仅支持0表示公历*/
    char type;
} rtc_date_t;

/*!
 * \brief
 *      rtc时间数据结构定义
 */
typedef struct
{
    /*! 小时*/
    int hour;
    /*! 分*/
    int minute;
    /*! 秒*/
    int second;
} rtc_time_t;

/*!
 * \brief
 *      rtc日期时间数据结构定义
 */
typedef struct
{
    /*! 日期*/
    rtc_date_t date;
    /*! 时间*/
    rtc_time_t time;
    /*! 标准秒*/
    unsigned long stdsec;
} rtc_date_time_t;

#ifndef FUCNTION_SYMBOL //通过函数指针方式调用接口时，需增加FUCNTION_SYMBOL宏定义，否则编译会有重复定义错误
extern int rtc_set_time(rtc_time_t *time);
extern int rtc_set_date(rtc_date_t *date);
extern int rtc_get_dateandtime(rtc_time_t *time, rtc_date_t *date);
extern int rtc_get_drm_dateandtime(rtc_time_t *drmtime, rtc_date_t *drmdate);
extern int rtc_set_drm_dateandtime(rtc_time_t *drmtime, rtc_date_t *drmdate);
extern unsigned long rtc_change_to_stdsec(rtc_time_t *time, rtc_date_t *date);

#endif /*FUCNTION_SYMBOL*/

#endif /*_ASSEMBLER_*/

#endif  /* __RTC_H__ */
