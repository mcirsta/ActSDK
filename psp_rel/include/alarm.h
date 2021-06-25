/********************************************************************************
 *                        USDK130
 *                     	common sub
 *
 *                (c) Copyright 2002, Actions Co,Ld.
 *                        All Right Reserved
 *
 * File: alarm.h
 * By  : wurui
 * Version: 1> v1.00     first version     2009-05-29 8:28
 *******************************************************************************
 */
/*!
 * \file     alarm.h
 * \brief    ��ʱ�����豸������ͷ�ļ�
 * \author   wurui
 * \version 1.0
 * \date  2009/05/29
 *******************************************************************************/
#ifndef __ALARM_H__
#define __ALARM_H__

#include "kmod_calls.h"
#include "syscalls.h"

#define FUCNTION_SYMBOL
#include "rtc.h"

/*!
 *  \brief
 *  ����ģʽ
 *  \li ALARM_ONCE : ָ�����ں�ʱ��
 *  \li UPRIGHT_MODE : ָ�����ں�ʱ��
 */
typedef enum
{
    ALARM_ONCE,
    ALARM_WEEKLY
} alarm_type_t;

/*!
 * \brief
 *      alarm_info_t,��ʱ����Ϣ�ṹ .
 */
typedef struct
{
    /*! clock�����ͣ���ָ�����ڻ�ָ��ѭ������*/
    alarm_type_t type;

    /*! ��ʱʱ��*/
    rtc_time_t time;

    /*! ��ʱ����*/
    struct
    {
        /*! ��type==ALARM_WEEKLY����ʱ��Ч����ʱ����*/
        char wday[7];
        /*! ��type==ALARM_ONCE����ʱ��Ч����ʱ����*/
        rtc_date_t date;
    } special_date;
} alarm_info_t;

/*!
 * \brief
 *      alarm_new_in_t,�����¶�ʱ������������ṹ.
 */
typedef struct
{
    /*! ��ʱ���ĳ�ʼ����*/
    alarm_info_t alarm_clock;
    /*! Ҫ����һ·mode�Ͻ�����ʱ��*/
    int mode_no;
} alarm_new_in_t;

/*!
 * \brief
 *      alarm_new_out_t,�����¶�ʱ������������ṹ.
 */
typedef struct
{
    /*! �½����Ķ�ʱ�������к�*/
    int clock_no;
} alarm_new_out_t;

/*!
 * \brief
 *      alarm_new_param_t,�����¶�ʱ���Ĳ����ṹ.
 */
typedef struct
{
    /*! �����¶�ʱ������������ṹ*/
    alarm_new_in_t in;
    /*! �����¶�ʱ������������ṹ*/
    alarm_new_out_t out;
} alarm_new_param_t;

/*!
 * \brief
 *      alarm_query_in_t,��ʱ����Ϣ��ѯ����������ṹ;
 *      alarm_delete_param_t,��ʱ��ɾ���Ĳ����ṹ.
 */
typedef struct
{
    /*! ��ʱ��������ģʽ��*/
    int mode_no;
    /*! ��ʱ���Ķ�ʱ����*/
    int clock_no;
} alarm_query_in_t, alarm_delete_param_t;

/*!
 * \brief
 *      alarm_query_out_t,��ʱ����Ϣ��ѯ����������ṹ
 */
typedef struct
{
    /*! ��ʱ���Ķ�ʱ��Ϣ*/
    alarm_info_t alarm_clock;
    /*! ���ο����Ƿ�Ϊ�ö�ʱ�����ѵĽ��,0x01��ʾ����Ϊ�ö�ʱ���ĵ��ﵼ���Զ�����,0x00��ʾ����*/
    int wakeup;
} alarm_query_out_t;

/*!
 * \brief
 *      alarm_modify_param_t,��ʱ����Ϣ�޸ĵĲ����ṹ
 */
typedef struct
{
    /*! ��ʱ��������ģʽ��*/
    int mode_no;
    /*! ��ʱ���Ķ�ʱ����*/
    int clock_no;
    /*! ��ʱ��������Ϣ*/
    alarm_info_t alarm_info;
} alarm_modify_param_t;

/*!
 * \brief
 *      alarm_query_param_t,��ʱ����Ϣ��ѯ�Ĳ����ṹ
 */
typedef struct
{
    /*! ��ʱ����Ϣ��ѯ����������ṹ*/
    alarm_query_in_t in;
    /*! ��ʱ����Ϣ��ѯ����������ṹ*/
    alarm_query_out_t out;
} alarm_query_param_t;

/*!
 * \brief
 *      alarm_get_clock_param_t,����Ч��ʱ����������Ϣ��ȡ�ṹ
 */
typedef struct
{
    /*! ����Ч��ʱ��������ģʽ��*/
    int mode_no;
    /*! ����Ч��ʱ���Ķ�ʱ����*/
    int clock_no;
} alarm_queue_node_t, alarm_get_clock_param_t;

/*!
 *  \brief
 *  ��ʱ���豸��ioctl����������
 *  \li ALARM_NEW_CLOCK : ������ʱ��
 *  \li ALARM_MODIDY_CLOCK : �޸Ķ�ʱ������
 *  \li ALARM_DELETE_CLOCK : ɾ����ʱ��
 *  \li ALARM_GET_CLOCK : ��ȡ����Ч��ʱ������
 *  \li ALARM_QUERY_CLOCK : ��ѯ��ʱ������
 *  \li ALARM_RESET : ���޸�ϵͳʱ�����������ϵͳ��ʱ
 *  \li ALARM_DELETE_MODE : ɾ��ָ��ģʽ�µ����ж�ʱ
 *  \li ALARM_DELETE_ALL : ɾ�����ж�ʱ
 */
enum
{
    ALARM_NEW_CLOCK,
    ALARM_MODIDY_CLOCK,
    ALARM_DELETE_CLOCK,
    ALARM_GET_CLOCK,
    ALARM_QUERY_CLOCK,
    ALARM_RESET,
    ALARM_DELETE_MODE,
    ALARM_DELETE_ALL,
};

/* ��standby�����е��õľ�̬�⺯�������ڼ��ͱ��濪���Ƿ�Ϊ��ʱ��������
 ����ֵΪ1��ʾ���ο�����Ϊ��ʱ���������𣬷���ֵΪ0��Ϊ����ԭ��*/
int alarm_check_wakeup_reason(void);

#endif /*__ALARM_H__*/
