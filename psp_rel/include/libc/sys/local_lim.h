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
 * \file     local_lim.h
 * \brief    posix C����ϵͳ��ص�������Ϣ
 * \author wurui
 * \par �������:
 * ����errno
 * \par EXTERNALIZED FUNCTIONS:
 *     Copyright(c) 2001-2008 Actions Semiconductor, All Rights Reserved.
 *
 *  \version 1.0
 *  \date  2008/09/12
 *******************************************************************************/

#ifndef __SYS_LOCAL_LIM_H__
#define __SYS_LOCAL_LIM_H__

/*	��ʹ����ϵͳ����ջ����������Ҫ�����ջ�ռ�.
 ��ϵͳ�Զ�����ջ�ռ�,����������������.
 �������û��趨��ջ�ռ�*/
#define SYSTEM_VISUAL_STACK_SUPPORT 1

/* ��ʹ��ϵͳ����ջ����,�������û��Զ���ջ�ռ� */
#if SYSTEM_VISUAL_STACK_SUPPORT > 0
#define PTHREAD_STACK_MIN       0
#define PTHREAD_STACK_DEFAULT   0
#else
/* �û��Զ����߳�ջ����Сջ�ռ�,��С�ڴ�ֵ,�����ò���ȷ */
#define PTHREAD_STACK_MIN       (1024*20)
#define PTHREAD_STACK_DEFAULT   (PTHREAD_STACK_MIN*2)
#endif

/*!
 *  \brief
 *     ͬһ�������ڵ��߳�������ȼ�
 */
#define MAX_PRIO 10

/*!
 *  \brief
 *     ͬһ�������ڵ��߳���С���ȼ�
 */
#define MIN_PRIO 1

/*!
 *  \brief
 *     �߳�Ĭ�����ȼ�
 */
#define COMMON_TASK ( (((MAX_PRIO-MIN_PRIO)+1)/2) + MIN_PRIO )

/*!
 *  \brief
 *     �߳�˽�м�����
 */
#define PTHREAD_KEYS_MAX   10

/*!
 *  \brief
 *     �߳�˽�м�շת����������������
 */
#define PTHREAD_DESTRUCTORITERATIONS 10

#endif /*#ifndef __SYS_LOCAL_LIM_H__*/
