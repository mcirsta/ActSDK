/********************************************************************************
 *                              NOYA
 *                            Module: libc_general
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       wurui     2008-09-12 10:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     signal.h
 * \brief    posix C����ϵͳ��ص��źŶ���
 * \author   wurui
 * \version 1.0
 * \date  2008/09/12
 *******************************************************************************/

#ifndef __SYS_SIGNAL_H__
#define __SYS_SIGNAL_H__

/*!
 *  \brief
 *     ��Ϣ����mq_notify()�������ź�����ֵ,��ֵ�����͸����ź����Ͷ���,usdkƽ̨��δ�����źŻ���
 */
union sigval
{
    /*! �ź�����*/
    int sigval_int;
    /*! �źŻص�����ָ��*/
    void * sigval_ptr;
};

/*!
 *  \brief
 *     ��Ϣ����mq_notify()�����Ĳ����ṹ,usdkƽ̨��δ�����źŻ���
 */
struct sigevent
{
    /*! �ź�����*/
    int sigev_notify;
    /*! �ź�No.*/
    int sigev_signo;
    /*! �ź�����ֵ*/
    union sigval sigev_value;
    /*! �źŻص�����ָ��*/
    void (* sigev_notify_function)(union sigval);
};

#endif /*#ifndef __SYS_SIGNAL_H__*/
