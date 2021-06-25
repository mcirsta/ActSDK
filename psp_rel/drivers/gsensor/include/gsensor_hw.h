/*
 ********************************************************************************
 *                   213x --- G Sensor's driver
 *                (c) Copyright 2007 - 2010, Actions Co,Ld.
 *                        All Right Reserved
 *
 * History:
 *      <author>      <time>       <version >    <desc>
 *       houjingkun      2008/06/26     1.0         build this file
 ********************************************************************************/
/*!
 * \file   gsensor_hw.h
 * \brief
 *      gsensor's driver hardware layer's API.
 * \author houjingkun
 * \par GENERAL DESCRIPTION:
 *      realize gsensor driver's hardware layer's API.
 * \par EXTERNALIZED FUNCTIONS:
 *       null
 *
 *     Copyright(c) 2008-2012 Actions Semiconductor, All Rights Reserved.
 *
 * \version 1.0
 * \date  2008/06/26
 *******************************************************************************/

/*******************************************************************************/
/***************** gsensor hardware configure ****************************/
/*******************************************************************************/
#ifndef _GSENSOR_HW_H_
#define _GSENSOR_HW_H_

#include "gsensor.h"

#define _MS_HW_NO_OS_

/* related bit in CMU_DEVCLKEN register. */
#define I2C_CLK_BIT  (0x1 << 23)     /* I2C clk enable. */
#define GPIO_CLK_BIT  (0x1 << 26)     /* gpio clk enable. */
#define MS_NEED_CLK   (I2C_CLK_BIT | GPIO_CLK_BIT)     /* gsensor driver need clk bits. */

/* ms_hw_ioctl cmmand. */
#define MS_CMD_RD_DATA  0x2
#define MS_CMD_WT_DATA  0x3


extern int gsensor_hw_init(void);
extern int gsensor_hw_exit(void);
extern int read_gsensor_data(gsensor_data_t* p_data);

#endif

