/********************************************************************************
 *                              USDK(1100)
 *                            Module: 213X_REG
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       SD     2008-02-01 15:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     actions_reg_213x.h
 * \brief    213x¼Ä´æÆ÷¶¨Òå
 * \author   SD
 * \version 1.0
 * \date  2008/2/1
 *******************************************************************************/
#ifndef __ASM_ATJ213X_REGS_H__
#define __ASM_ATJ213X_REGS_H__

#ifdef IC_TYPE
#if (IC_TYPE == 0x63)
#include "actions_reg_213x_63.h"
#elif (IC_TYPE == 0x96)
#include "actions_reg_213x_96.h"
#else
#error "IC_TYPE err"
#endif
#else
#include "actions_reg_213x_63.h"
#endif
#endif /*__ASM_ATJ213X_REGS_H__*/
