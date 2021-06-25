/*
********************************************************************************
*                   213x --- tvin's driver
*                (c) Copyright 2007 - 2010, Actions Co,Ld. 
*                        All Right Reserved 
*
* History:        
*      <author>      <time>       <version >    <desc>
*       houjingkun      2008/06/18     1.0         build this file 
*       new added tvin_hw.h and tvin_hw.c for video in device driver.
********************************************************************************/ 
/*!
 * \file   tvin_hw.h
 * \brief  
 *      Tvin driver hardware layer's API.
 * \author houjingkun
 * \par GENERAL DESCRIPTION:
 *      Tvin driver hardware layer's API.  just called by dirver itself.
 * \par EXTERNALIZED FUNCTIONS:
 *       null
 *
 *     Copyright(c) 2008-2012 Actions Semiconductor, All Rights Reserved.
 *
 * \version 1.0
 * \date  2008/06/18
 *******************************************************************************/

#ifndef _TVIN_HW_H_
#define _TVIN_HW_H_

#include "vi_config.h"

/*
 ************************ begin of tvin config. *******************************
 */
//#define _TVIN_HW_NO_OS_FUNC_

/* related bit in CMU_DEVCLKEN register. */
#define BT_CLK_BIT  (0x1 << 12)     /* BT clk enable bit. */
#define I2C_CLK_BIT  (0x1 << 23)     /* I2C clk enable. */
#define DMA_CLK_BIT  (0x1 << 8)      /* dma clk enable. */
#define GPIO_CLK_BIT  (0x1 << 26)     /* gpio clk enable. */
#define TVIN_NEED_CLK   (BT_CLK_BIT | I2C_CLK_BIT | DMA_CLK_BIT | GPIO_CLK_BIT)     /* tvin driver need clk bits. */

#define TVIN_PIN_MASK1  0x80000001
#define TVIN_PIN_VAL1   0x80000001

#define TVIN_INIT_ADDR  0xa0100000

/* tvin_hw_ioctl's cmd */
#define TVIN_DEBUG  0
#define TVIN_DMA_START  1
#define TVIN_DMA_STOP   2
#define TVIN_SET_DMA_ADDR   3
#define TVIN_GET_DMA_ADDR   4
#define TVIN_GET_DMA_REMAIN   6
#define TVIN_DMA_INIT   9
#define TVIN_SET_RESOLUTION   10

#ifdef _TVIN_HW_NO_OS_FUNC_

#if 0
static inline void act_writel(unsigned int val, int reg)
{
    *(volatile unsigned int *)(reg) = val;
}

static inline unsigned int act_readl(unsigned long port)
{
    return (*(volatile unsigned int *)port);
}
#endif

#endif  /* _TVIN_HW_NO_OS_FUNC_ */

extern int tvin_hw_init(void);
extern int tvin_hw_exit(void);
extern int tvin_hw_ioctl(unsigned int cmd, unsigned int param);
extern int init_tvin_ic(void);

#endif /* _TVIN_HW_H_ */

