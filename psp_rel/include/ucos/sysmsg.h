/*******************************************************************************
 *                              USDK213F
 *                            Module: SYSMSG
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-6-12 9:42     1.0             build this file
 *******************************************************************************/
/*!
 * \file     sysmsg.h
 * \brief    ϵͳ��Ϣͷ�ļ�
 * \author   hmwei
 * \par      GENERAL DESCRIPTION:
 *               ������ļ���������
 * \par      EXTERNALIZED FUNCTIONS:
 *               �����������õ������ģ��
 * \version 1.0
 * \date  2008/6/12
 *******************************************************************************/
#ifndef __SYSMSG_H__
#define __SYSMSG_H__

#define msg_LowBat		0x06
#define msg_LowBatRec	0x07
#define msg_StubEnable 	0x08
#define msg_StubDisable	0x09
#define msg_StubSuspend	0x0a
#define msg_StubResume	0x0b

#ifndef _ASSEMBLER_

extern int put_sysmsg(int msg);
extern int get_sysmsg(void);
extern void flush_sysmsg(void);

#endif /*_ASSEMBLER_*/
#endif /*__SYSMSG_H__*/
