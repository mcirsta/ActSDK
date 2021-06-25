/********************************************************************************
 *                              USDK(1100)
 *                            Module: UTIME
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-8-30 16:42     1.0             build this file
 ********************************************************************************/
/*!
 * \file     utime.h
 * \brief    utime���ݽṹ����
 * \author  open source
 * \version 1.0
 * \date  2008/8/30
 *******************************************************************************/
#ifndef __UTIME_H__
#define __UTIME_H__

#include <ucos/types.h>

#ifndef _ASSEMBLER_

/*!
 * \brief
 *      �ļ�����ʱ��ṹ����
 */
struct utimbuf
{
    /*! ��ȡʱ��*/
    vfs_time_t actime;
    /*! ����ʱ��*/
    vfs_time_t modtime;
};

extern int utime(const char *filename, struct utimbuf *buf);

#endif /*_ASSEMBLER_*/
#endif/*__UTIME_H__*/
