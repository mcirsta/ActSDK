/********************************************************************************
 *                        USDK130
 *                     watchdog driver
 *
 *                (c) Copyright 2002, Actions Co,Ld.
 *                        All Right Reserved
 *
 * File: watchdog.h
 * By  : wurui
 * Version: 1> v1.00     first version     2007-10-30 15:51
 *******************************************************************************
 */

/*!
 * \file  watchdog.h
 * \brief ��������״̬������������
 * \author wurui
 * \par GENERAL DESCRIPTION:
 *           ÿһ�������ڴ�������Ҫ���ϵ���������򱨸�����״̬, ����������ٱ����������Ƿ��������ڲ�������״̬,
 *       ��������������ϵͳ.
 * \par DECLARATIONS:
 *       (c) Copyright 2002-2004, Actions Co,Ld. All Right Reserved.
 *
 *  \version 1.0
 *  \date  2007/10/30
 */

#ifndef __WDOG_H__
#define __WDOG_H__

#include "kmod_calls.h"

/*!
 *  ��ѯ���,��λ: ms \n
 *  \ingroup     Drivers.WDM
 */
#define WDTASK_INTERVAL 500

/*!
 *  WDD �豸��� \n
 *  \li\b time_out   : WDD�ĳ�ʱ�趨
 *  \li\b count      : WDD�ļ�ʱ��
 *  \ingroup     Drivers.WDM
 */
typedef struct
{
    u32 time_out;
    u32 count;
    s32 index;
} WDD_handle_t;

/*!
 *  WDD ���� \n
 *  \li\b WDD_CMD_SET_TIME   : ���ü�ʱ��ʱʱ��, ������λ:ms
 *  \ingroup     Drivers.WDM
 */
typedef enum
{
    WDD_CMD_SET_TIME,
} WDD_cmd_t;

#endif  /*#ifndef __WDOG_H__*/
