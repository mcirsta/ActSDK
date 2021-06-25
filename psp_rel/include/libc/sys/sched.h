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
 * \file     sched.h
 * \brief    posix �߳̿��̵߳�����ض���
 * \author wurui
 * \par �������:
 * ����errno
 * \par EXTERNALIZED FUNCTIONS:
 *     Copyright(c) 2001-2008 Actions Semiconductor, All Rights Reserved.
 *
 *  \version 1.0
 *  \date  2008/09/12
 *******************************************************************************/

#ifndef __SYS_SCHED_H__
#define __SYS_SCHED_H__

/*!
 *  \brief
 *     �̵߳�������ö��ֵ
 */
typedef enum
{
    SCHED_OTHER,
    SCHED_FIFO,
    SCHED_RR,
} sched_attr_t;

/*!
 *  \brief
 *     �̵߳��Ȳ���
 */
struct sched_param
{
    /*! �̵߳�posix���ȼ� */
    int sched_priority;
};

#endif /* #ifndef __SYS_SCHED_H__ */
