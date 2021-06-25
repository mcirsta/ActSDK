/********************************************************************************
 *                               USDK(1100)
 *                            Module: CTYPE
 *                 Copyright(c) 2001-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-6-12 9:42     1.0             build this file
 ********************************************************************************/
/*!
 * \file     delay.h
 * \brief    延时函数(无任务切换)声明
 * \author   hmwei
 * \version 1.0
 * \date  2008/6/12
 *******************************************************************************/
#ifndef __DELAY_H__
#define __DELAY_H__

#ifndef _ASSEMBLER_

extern void mdelay(int num);
extern void udelay(int num);

#endif /*_ASSEMBLER_*/
#endif /*__DELAY_H__*/
