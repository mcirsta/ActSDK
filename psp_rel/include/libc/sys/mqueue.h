/********************************************************************************
 *                                    NOYA
 *                            Module: libc_general
 *                 Copyright(c) 2001-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>           <version >             <desc>
 *        wurui     2008-09-12 10:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     mqueue.h
 * \brief    posix C����ϵͳ��ص���Ϣ���ж���
 * \author wurui
 * \par �������:
 * ����errno
 * \par EXTERNALIZED FUNCTIONS:
 *     Copyright(c) 2001-2008 Actions Semiconductor, All Rights Reserved.
 *
 *  \version 1.0
 *  \date  2008/09/12
 *******************************************************************************/

#ifndef __SYS_MQUEUE_H__
#define __SYS_MQUEUE_H__

#include <libc/sys/types.h>

/*!
 *  \brief
 *     ��Ϣ���г�ʼ���ṹ
 */
struct mq_attr
{
    /*! ��Ϣ������������: 0--����������(��Ϣ������ʱ����������,��Ϣ���п�ʱ����������)
     O_NONBLOCK--������������,�����ش���*/
    long mq_flags;
    /*! ��Ϣ���г�ʼ��ʱʹ��,������Ϣ���������Ϣ����Ŀ; 2��Ϣ�������Ի�ȡʱʹ��*/
    long mq_maxmsg;
    /*! ��Ϣ���г�ʼ��ʱʹ��,����ÿһ����Ϣ�������ݳ���; 2��Ϣ�������Ի�ȡʱʹ��*/
    long mq_msgsize;
    /*! ��Ϣ�����Ի�ȡʱʹ��,��ȡ��Ϣ���е�ǰ��Ϣλ��*/
    long mq_curmsgs;
};

/*!
 *  \brief
 *     ��Ϣ�����ļ�������
 */
typedef int mqd_t;

#endif /*#ifndef __SYS_MQUEUE_H__*/
