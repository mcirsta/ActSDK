/********************************************************************************
 *                              USDK130
 *                            Module: TIME
 *                 Copyright(c) 2001-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       wurui     2008-09-12 10:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     time.h
 * \brief    posix C����ϵͳ��ص�ʱ�����Ͷ���
 * \author   wurui
 * \version 1.0
 * \date  2008/12/31
 *******************************************************************************/
#ifndef __SYS_TIME_H__
#define __SYS_TIME_H__

typedef unsigned int time_t;

/*!
 *  \brief
 *     ʱ��\����\����
 */
struct tm
{
    /*!   seconds   after   the   minute   -   [0,59]   */
    int tm_sec;
    /*!   minutes   after   the   hour   -   [0,59]   */
    int tm_min;
    /*!   hours   since   midnight   -   [0,23]   */
    int tm_hour;
    /*!   day   of   the   month   -   [1,31]   */
    int tm_mday;
    /*!   months   since   January   -   [0,11]   */
    int tm_mon;
    /*!   years   since   1900   */
    int tm_year;
    /*!   days   since   Sunday   -   [0,6]   */
    int tm_wday;
    /*!   days   since   January   1   -   [0,365]   */
    int tm_yday;
    /*!   daylight   savings   time   flag   */
    int tm_isdst;
};

/*!
 *  \brief
 *     ��׼ʱ��ṹ
 */
struct timespec
{
    /*! seconds */
    time_t tv_sec;
    /*! nanoseconds */
    long tv_nsec;
};

/*!
 *  \brief
 *     ʱ������
 */
typedef enum
{
    /*! ʵʱʱ�� */
    CLOCK_REALTIME = 1,
} clockid_t;

#endif /* __SYS_TIME_H__ */
