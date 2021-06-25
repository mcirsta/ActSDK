/********************************************************************************
 *                              NOYA
 *                            Module: types.h
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       wurui     2008-09-12 10:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     types.h
 * \brief    posix C��ϵͳ��صĻ������Ͷ���
 * \author   wurui
 * \version 1.0
 * \date  2008/09/12
 *******************************************************************************/

#ifndef __POSIX_TYPES_H__
#define __POSIX_TYPES_H__

#include <syscalls.h>
#include <libc/sys/sched.h>
#include <libc/sys/limits.h>
#include <libc/sys/local_lim.h>
#include <libc/sys/reent.h>
#include <libc/sys/time.h>

/* ϵͳ�ź���ȫ�ָ���ֵ�� */
#if OS_EVENT_TBL_SIZE == 1
#define OS_EVENT_TBL \
{ \
    0, /*OSEventTbl[0]*/ \
}
#endif

#if OS_EVENT_TBL_SIZE == 2
#define OS_EVENT_TBL \
{ \
    0, /*OSEventTbl[0]*/ \
    0, /*OSEventTbl[1]*/ \
}
#endif

#if OS_EVENT_TBL_SIZE == 3
#define OS_EVENT_TBL \
{ \
    0, /*OSEventTbl[0]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
}
#endif

#if OS_EVENT_TBL_SIZE == 4
#define OS_EVENT_TBL \
{ \
    0, /*OSEventTbl[0]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
}
#endif

#if OS_EVENT_TBL_SIZE == 5
#define OS_EVENT_TBL \
{ \
    0, /*OSEventTbl[0]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
}
#endif

#if OS_EVENT_TBL_SIZE == 6
#define OS_EVENT_TBL \
{ \
    0, /*OSEventTbl[0]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
}
#endif

#if OS_EVENT_TBL_SIZE == 7
#define OS_EVENT_TBL \
{ \
    0, /*OSEventTbl[0]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
}
#endif

#if OS_EVENT_TBL_SIZE == 8
#define OS_EVENT_TBL \
{ \
    0, /*OSEventTbl[0]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
    0, /*OSEventTbl[1]*/ \
}
#endif

#if OS_EVENT_NAME_SIZE > 1
#define OS_EVENT_NAME "sem_thread"  /*OSEventName*/
#endif

/*!
 *  \brief
 *     �̷߳�������ö��ֵ
 */
typedef enum
{
    /*! �����߳� */
    PTHREAD_CREATE_DETACHED,
    /*! �Ƿ����߳� */
    PTHREAD_CREATE_JOINABLE,
} detach_attr_t;

/*!
 *  \brief
 *     �߳̾���������ö��ֵ
 */
typedef enum
{
    /*! ʵʱ�߳� */
    PTHREAD_SCOPE_SYSTEM,
    /*! ��ʵʱ�߳� */
    PTHREAD_SCOPE_PROCESS,
    MAX_SCOPE_NUM,
} scope_attr_t;

/*!
 *  \brief
 *     �̼̳߳�����ö��ֵ
 */
typedef enum
{
    /*! �̳д����ߵ��߳����� */
    PTHREAD_INHERIT_SCHED = 1,
    /*! ���߳�����ָ�� */
    PTHREAD_EXPLICIT_SCHED,
} inheritsched_attr_t;

/*!
 *  \brief
 *     ����������ö��ֵ����
 */
enum
{
    PTHREAD_MUTEX_NORMAL,
    PTHREAD_MUTEX_RECURSIVE,
    PTHREAD_MUTEX_ERRORCHECK,
    PTHREAD_MUTEX_INVALID,
    PTHREAD_MUTEX_DEFAULT = PTHREAD_MUTEX_NORMAL,
} pthread_mutex_type_t;

/*!
 *  \brief
 *     ���̼乲������ö�ٶ���
 */
enum
{
    /*! �����ڹ��� */
    PTHREAD_PROCESS_PRIVATE,
    /*! ���̼乲�� */
    PTHREAD_PROCESS_SHARED
} pthread_share_type_t;

/*!
 *  \brief
 *     pthread_once��������
 */
typedef int pthread_once_t;

/*!
 *  \brief
 *     �߳����Խṹ
 */
typedef struct
{
    /*! ��ʼ����Ч��־ */
    char magic[4];
    /*! �߳�ջ�׵�ַ */
    void *stackaddr;
    /*! �߳�ջ��С */
    int stacksize;
    /*! ��ջ��������С */
    unsigned int guardsize;
    /*! �̵߳��Ȳ��ԵĻ�ȡģʽ,�ɸ��̼̳߳л���������ָ�� */
    int inheritsched;
    /*! �̵߳ĵ��Ⱦ�����Χ */
    int contentionscope;
    /*! ���Ȳ��� */
    struct sched_param schedparam;
    /*! �̵߳ĵ��Ȳ��� */
    int schedpolicy;
    /*! �������� */
    detach_attr_t detachstate;
} pthread_attr_t;

/*!
 *  \brief
 *     ��������ʼ������
 */
typedef struct
{
    unsigned int magic;
    /*! PTHREAD_PROCESS_SHARED,PTHREAD_PROCESS_PRIVATE */
    int pshared;
    /*! PTHREAD_MUTEX_NORMAL,PTHREAD_MUTEX_ERRORCHECK,PTHREAD_MUTEX_RECURSIVE,PTHREAD_MUTEX_DEFAULT*/
    int type;
} pthread_mutexattr_t;

/*!
 *  \brief
 *     �������ڲ��ṹ: ����ϵͳ��ص����ݼ����
 */
typedef struct
{
    /*! psharedΪPTHREAD_PROCESS_PRIVATE��Ч,���ڽ����ڹ���*/
    os_event_t semaphor_thread;
    /*! psharedΪPTHREAD_PROCESS_SHARED��Ч,���ڽ��̼乲��*/
    os_event_t *semaphor_process;
    /*! Depth of recursive locking */
    int m_count;
    /*! Owner thread (if recursive or errcheck) */
    unsigned int m_owner;
} pthread_mutexdata_t;

/*!
 *  \brief
 *     �жϻ������Ƿ��Ѿ���ʼ���ı�־
 */
#define PTHREAD_MUTEX_MAGIC "MUTE"

/*!
 *  \brief
 *     ����������ṹ
 */
typedef struct
{
    /*! �����жϵ�ǰ�����������Ƿ��Ѿ�������ֵ */
    char magic[4];
    /*! �������ڲ��ṹ: ����ϵͳ��ص����ݼ���� */
    pthread_mutexdata_t mutexdata;
    /*! ���������� */
    pthread_mutexattr_t attr;
} pthread_mutex_t;

/*!
 *  \brief
 *     �����������Խṹ
 */
typedef struct
{
    unsigned int magic;
    /*! ���̼乲������,PTHREAD_PROCESS_SHARED--�����ڽ��̼乲��,PTHREAD_PROCESS_PRIVATE--ֻ���ڽ�����ʹ��*/
    int pshared;
} pthread_condattr_t;

/*!
 *  \brief
 *     ���������ڲ��ṹ����:ϵͳ������ݽṹ,����ϵͳ����ʱʹ��
 */
typedef struct
{
    /*! ��PTHREAD_PROCESS_PRIVATEʱ��Ч,ϵͳ�źŽṹ�����ڽ��̿ռ���*/
    os_event_t semaphor_thread;
    /*! ��PTHREAD_PROCESS_SHAREDʱ��Ч,ϵͳ�źŽṹ������ϵͳ��*/
    os_event_t *semaphor_process;
    /*! �ȴ������������ϵ��߳���*/
    int wait_num;
} pthread_conddata_t;

/*!
 *  \brief
 *     �����������ݽṹ
 */
typedef struct
{
    /*! �����жϵ�ǰ���������Ƿ��Ѿ�������ֵ */
    char magic[4];
    /*! ��������ϵͳ������ݽṹ */
    pthread_conddata_t conddata;
    /*! �����������Խṹ */
    pthread_condattr_t attr;
} pthread_cond_t;

/*
 *     �̺߳��ڲ��ṹ
 */
typedef union
{
    struct
    {
        unsigned short pid;
        unsigned short tid;
    } struct_id;
    unsigned int int_id;
} _thread_id_t;

/*!
 *  \brief
 *     �̺߳Žṹ����
 */
typedef _thread_id_t pthread_t; /* �߳�ID�� */

/*!
 *  \brief
 *     �߳�������
 */
typedef struct _pthread_descr_struct *pthread_descr;

/*!
 *  \brief
 *     �߳�˽�м��ṹ
 */
typedef unsigned int pthread_key_t;

#endif  /*#ifndef __POSIX_TYPES_H__*/
