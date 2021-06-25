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
 * \file     semaphore.h
 * \brief    posix C����ϵͳ��ص��ź�������
 * \author   wurui
 * \version 1.0
 * \date  2008/09/12
 *******************************************************************************/

#ifndef __SYS_SEMAPHORE_H__
#define __SYS_SEMAPHORE_H__

#include <libc/sys/types.h>
#include <libc/sys/debug.h>

/*!
 *  \brief
 *     �ź�������ʧ�ܷ���ֵ
 */
#define SEM_FAILED	((sem_t *) -1)

/*!
 *  \brief
 *     �����ź�������ֵ��־
 */
#define PTHREAD_SEM_NAMING_MAGIC    "SEMN"

/*!
 *  \brief
 *     �����ź�������ֵ��־
 */
#define PTHREAD_SEM_WITHOUT_MAGIC   "SEMW"

/*!
 *  \brief
 *     �ź�������ֵ�жϱ�־
 */
#define PTHREAD_SEM_MAGIC           "SEM"

/* �����ź���ȫ�ֳ�ֵ�� */
#define SEMAPHOR_INITIALIZER \
{ \
    OS_EVENT_TYPE_SEM, /*OSEventType*/ \
    0x00, /*OSEventGrp*/ \
    0, /*OSEventCnt*/ \
    NULL, /*OSEventPtr*/ \
    OS_EVENT_TBL, \
    OS_EVENT_NAME, \
    0, \
}

/*!
 *  \brief
 *     �ź����ڲ��ṹ:�ź��������ռ�ṹ����
 */
typedef struct
{
    /*! �ź����ṹ��Ч��־ */
    char magic[4];
    /*! �ź���������� */
    int ref;
    /*! ���̼��������� */
    int pshared;
    /*! �����ź��� */
    os_event_t semaphor_thread;
    /*! �����ź��� */
    os_event_t *semaphor;
    /*! ���ź����Ѿ�ɾ���������ź��������ռ��ַ */
    void *ulink_node;
#if LIBC_PIPE_DEBUG_ON > 0
void *debug_sapce_node;
#endif
} sem_ns_t;

/*!
 *  \brief
 *     �ź����������ṹ
 */
typedef struct
{
    /*! �ź��������ռ�ṹ */
    sem_ns_t *sem_ns;
} sem_t;

#endif /* #ifndef __SYS_SEMAPHORE_H__ */

