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
 * \file     pthread.h
 * \brief    posix �߳̿���ϵͳ��ض���
 * \author wurui
 * \par �������:
 * ����errno
 * \par EXTERNALIZED FUNCTIONS:
 *     Copyright(c) 2001-2008 Actions Semiconductor, All Rights Reserved.
 *
 *  \version 1.0
 *  \date  2008/09/12
 *******************************************************************************/

#ifndef __SYS_PTHREAD_H__
#define __SYS_PTHREAD_H__

#include <libc/sys/types.h>

/*!
 *  \brief
 *     �û�ջʹ��״̬���ֵ
 */
#define PTHREAD_GUARD_MAGIC 0x67756964 /*guid*/

#define PTHREAD_MUTEX_ATTR_MAGIC 0x6d756174 /*muat*/

#define PTHREAD_COND_ATTR_MAGIC 0x636f6174 /*coat*/
/*!
 *  \brief
 *     pthread_once_t ��ֵ
 */
#define PTHREAD_ONCE_INIT 0

/*!
 *  \brief
 *     �̳߳�ʼ�����Ը���ֵ��־
 */
#define PTHREAD_ATTR_MAGIC "p_at"

/*!
 *  \brief
 *     ������������ֵ��־
 */
#define PTHREAD_COND_MAGIC "COND"

/* ������ȫ�ֳ�ֵ�� */
#define PTHREAD_MUTEX_INITIALIZER \
{ \
    PTHREAD_MUTEX_MAGIC, /*magic[4]*/ \
    { /*mutexdata*/ \
        { /*semaphor_thread*/ \
            OS_EVENT_TYPE_SEM, /*OSEventType*/ \
            0x00, /*OSEventGrp*/ \
            1, /*OSEventCnt*/ \
            NULL, /*OSEventPtr*/ \
            OS_EVENT_TBL, \
            OS_EVENT_NAME, \
            0, \
        }, \
        NULL, /*semaphor_process*/ \
        0, /*m_count*/ \
        -1, /*m_owner*/ \
    }, \
    { /*attr*/ \
        PTHREAD_MUTEX_ATTR_MAGIC, /*magic*/ \
        PTHREAD_PROCESS_PRIVATE,  /*pshared*/ \
        PTHREAD_MUTEX_DEFAULT, /*type*/ \
    }, \
}

/* ��������ȫ�ֳ�ֵ�� */
#define PTHREAD_COND_INITIALIZER \
{ \
    PTHREAD_COND_MAGIC, /*magic[4]*/ \
    { /*conddata*/ \
        { /*semaphor_thread*/ \
            OS_EVENT_TYPE_SEM, /*OSEventType*/ \
            0x00, /*OSEventGrp*/ \
            0, /*OSEventCnt*/ \
            NULL, /*OSEventPtr*/ \
            OS_EVENT_TBL, \
            OS_EVENT_NAME, \
            0, \
        }, \
        NULL, /*semaphor_process*/ \
        0, /*thread num*/ \
    }, \
    { /*attr*/ \
        PTHREAD_COND_ATTR_MAGIC,  /*magic*/ \
        PTHREAD_PROCESS_PRIVATE,  /*pshared*/ \
    }, \
}

#endif /* #ifndef __SYS_PTHREAD_H__ */
