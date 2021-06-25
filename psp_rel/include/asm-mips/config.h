/********************************************************************************
 *                              USDK(1100)
 *                            Module: CONFIG
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-8-30 16:45     1.0             build this file
 ********************************************************************************/
/*!
 * \file     config.h
 * \brief    mips��ϵ�ܹ���ϸ������
 * \author   hmwei
 * \version 1.0
 * \date  2008/8/30
 *******************************************************************************/
#ifndef __CONFIG_H__
#define __CONFIG_H__

#define CONFIG_CPU_MIPSR2
//#define CONFIG_IRQ_CPU
#define CONFIG_CPU_HAS_SYNC

#define cpu_has_llsc 0
#define R10000_LLSC_WAR 0

#define _MIPS_SZLONG 32

#define CONFIG_BUG

#endif /* __CONFIG_H__ */
