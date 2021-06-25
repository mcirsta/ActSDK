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
 * \file     limits.h
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

#ifndef __SYS_LIMITS_H__
#define __SYS_LIMITS_H__

#include <libc/sys/local_lim.h>

/*! ��Ϣ���е����ȼ���Χ: 0 ~ MQ_PRIO_MAX-1 */
#define MQ_PRIO_MAX     10

/*! �ź������ֵ */
#define SEM_VALUE_MAX   255

/*! ·�������ֵ */
#define PATH_MAX        1024

/*! �ļ������ֵ */
#define MAXNAMLEN       1024

/* ��������ʱ����Ĳ�������Ŀ */
#define MAXARGS 10

/* atexit����ע�������˳����������� */
#define ATEXIT_MAX 10

#endif /* #ifndef __SYS_LIMITS_H__ */

