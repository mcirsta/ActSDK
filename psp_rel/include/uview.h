/*******************************************************************************
 *                              USDK213F
 *                            Module: UVIEW
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2009-7-26 15:14     1.0             build this file
 *******************************************************************************/
/*!
 * \file     uview.h
 * \brief    uview
 * \author   hmwei
 * \version 1.0
 * \date  2007/5/4
 *******************************************************************************/
#ifndef __UVIEW_H__
#define __UVIEW_H__

#define MACRO_SCH_INT 0x101

#ifndef _ASSEMBLER_

/*!
 *  \brief
 *  uview������/�¼�����
 *  \li 0x0~0x4f : Ӧ�ó�����¼�����
 *  \li 0x80~0xff : �ں�ģ����¼�����
 *  \li 0x100~ : �ں�ģ��������¼�����
 */
enum uview_type
{
    /*! Ӧ�ó�����������*/
    PROCESS_CREATE = 0x0,
    MUTEX_CREATE = 0x10,
    MUTEX_DELETE,
    MUTEX_LOCK,
    MUTEX_UNLOCK,
    SEM_CREATE = 0x20,
    SEM_DELETE,
    SEM_WAIT,
    SEM_TIMEDWAIT,
    SEM_POST,
    COND_INIT = 0x30,
    COND_DELETE,
    COND_WAIT,
    COND_TIMEDWAIT,
    COND_BRODCAST,
    COND_SIGNAL,
    MQ_CREATE = 0x40,
    MQ_DELETE,
    MQ_WAIT,
    MQ_TIMEDWAIT,
    MQ_SEND,
    MQ_TIMEDSEND,
    /*! �ں�ģ����������*/
    T_CREATE = 0x80,
    T_DELETE,
    T_PRIO_CHANGE,
    T_RESUME,
    T_SUSPEND,
    T_SCHED,
    S_CREATE = 0x90,
    S_DELETE,
    S_PEND,
    S_POST,
    Q_CREATE = 0xa0,
    Q_DELETE,
    Q_PEND,
    Q_POST,
    F_CREATE = 0xb0,
    F_DELETE,
    F_PEND,
    F_POST,
    /*! �ں�ģ��������¼�����*/
    PM_CHANGE = 0x100,
    SCH_INT,
    SCH_TIMEOUT,
    SCH_UNLOCK,
    SCH_PTHEAD_ACTIVE
};

/*!
 * \brief
 *      �̻߳�����Ϣ���ݽṹ
 */
struct thread_base_info
{
    /*! ����id*/
    unsigned short pid;
    /*! �߳�id*/
    unsigned short tid;
    /*! ��ǰ�������ȼ�*/
    unsigned char prio;
};

/*!
 * \brief
 *      �߳���չ��Ϣ���ݽṹ
 */
struct thread_extra_info
{
    /*! ջ��*/
    unsigned int stk_ptr;
    /*! ջ����*/
    unsigned int stk_size;
    /*! �߳����ڽ��̵��ڴ�ռ�ó���*/
    unsigned int memory_size;
};

/*!
 * \brief
 *      �߳���Ϣ���ݽṹ
 */
struct thread_info
{
    /*! �̻߳�����Ϣ*/
    struct thread_base_info base;
    /*! �߳���չ��Ϣ*/
    struct thread_extra_info extra;
};

/*!
 * \brief
 *      �̵߳���ԭ�����ݽṹ
 */
struct thread_sched_cause
{
    /*! ����ԭ��*/
    unsigned int cause;
    /*! �����¼�*/
    void *event;
};

/*!
 * \brief
 *      PMģʽ��Ƶ��Ϣ���ݽṹ
 */
struct pm_info
{
    /*! cpuƵ��ֵ*/
    unsigned int cpuclk;
    /*! vddֵ*/
    unsigned int vdd;
    /*! vccֵ*/
    unsigned int vcc;
};

/*!
 * \brief
 *      ���͸�usdkview�����ݵĽṹ����
 */
struct usdkview_date
{
    /*! ��������*/
    unsigned int type;
    /*! ʱ�䣨�ϵ�����ʱ��ms��*/
    unsigned int time;
    /*! ʱ�䣨�ϵ�����tick����*/
    unsigned long long tick;
    /*! ���ݰ��ܳ���*/
    unsigned int len;
};

/*!
 * \brief
 *      ���̣����̣߳��������ݽṹ
 */
struct process_create_info
{
    /*! ������*/
    char name[16];
    /*! ���̵����̻߳�����Ϣ*/
    struct thread_info main_thread;
};

/*!
 * \brief
 *      �߳�ɾ�����ݽṹ
 */
struct thread_del_info
{
    /*! �̻߳�����Ϣ*/
    struct thread_base_info thread;
};

/*!
 * \brief
 *      �߳����ȼ��ı����ݽṹ
 */
struct thread_change_info
{
    /*! ��ǰ�߳���Ϣ*/
    struct thread_info cur;
    /*! ���߳���Ϣ*/
    struct thread_info new;
};

/*!
 * \brief
 *      �̵߳������ݽṹ
 */
struct thread_sw_info
{
    /*! ��ǰ�߳���Ϣ*/
    struct thread_info cur;
    /*! ���߳���Ϣ*/
    struct thread_info new;
    /*! ����ԭ��*/
    struct thread_sched_cause cause;
};

/*!
 * \brief
 *      event��Ϣ���ݽṹ
 */
struct event_info
{
    /*! ����ucos_event��չ��pthread_eventָ��*/
    void *pthread_event;
    /*! ucos_eventָ��*/
    void *ucos_event;
};

/*!
 * \brief
 *      PMģʽ��Ƶ��Ϣ���ݽṹ
 */
struct pm_change_info
{
    /*! ��Ƶ����Ϣ*/
    struct pm_info new;
};

/*
 1.����usdkviewʱ���Ƚ���ǰ��Ϣ������͸�pc
 2.ʵʱģʽ��buf��ʱ������
 3.����ģʽ��ֹͣʱ������
 */

#endif /*_ASSEMBLER_*/
#endif /*__UVIEW_H__*/
