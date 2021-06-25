/*
********************************************************************************
*                       usdk130
*                    Module: LCM Driver
*                Copyright(c) 2001-2007 Actions Semiconductor, 
*                        All Right Reserved 
*
* History:         
*      <author>    <time>       <version >    <desc>
*      shaofeng    2007/11/25     1.0         build this file 
********************************************************************************
*/


/*!
* \file Lcd_yuv2rgb.h
* \brief yuv2rgb module address and operation
* \author shaofeng
* \par GENERAL DESCRIPTION:
*       This file provide the yuv2rgb module's macro. \n
*       
* \par DECLARATIONS:
*       (c) Copyright 2002-2004, Actions Co,Ld. All Right Reserved.
*
* \version 1.0
* \date 2007/11/25
*/
#ifndef _LCD_YUV2RGB_H_
#define _LCD_YUV2RGB_H_

#include "kmod_calls.h"
#include "lcd_config.h"


#define CLK_DIV_120         0x0306
#define CLK_DIV_72          0x0204
#define CLK_DIV_48          0x0102
#define CLK_DIV_168         0x0408
#define CLK_DIV_DEFAULT     0x050A

/*-----for multifunc gpio--------------------------*/
/* multi ctl0 */
#define PIN_NA2_0_OFFSET         22
#define PIN_NCE3_OFFSET          14
#define PIN_NWRRD_OFFSET         6
#define PIN_NDATH_OFFSET         3
#define PIN_NDATL_OFFSET         0

#define PIN_MASK_NA2_0      (0x7 << PIN_NA2_0_OFFSET )
#define PIN_MASK_NCE3       (0x3 << PIN_NCE3_OFFSET  )
#define PIN_MASK_NWRRD      (0x3 << PIN_NWRRD_OFFSET )
#define PIN_MASK_NDATH      (0x7 << PIN_NDATH_OFFSET )
#define PIN_MASK_NDATL      (0x7 << PIN_NDATL_OFFSET )

#define GPIO_MULTI_LCD_MASK (PIN_MASK_NA2_0 | PIN_MASK_NCE3 | PIN_MASK_NWRRD | \
            PIN_MASK_NDATH | PIN_MASK_NDATL)
#define GPIO_MULTI_LCD_VAL	((0x2 << PIN_NA2_0_OFFSET) | (0x2 << PIN_NCE3_OFFSET) | \
           (0x2 << PIN_NWRRD_OFFSET) | (0x2 << PIN_NDATH_OFFSET) | \
           (0x2 << PIN_NDATL_OFFSET))

//moved to here  from "lcd_config.h"
#ifdef      DATA_WIDTH_18BIT
#define  RGB_DATA_WIDTH_18       1
#else       //default 16bits interface
#define  RGB_DATA_WIDTH_18       0
#endif
/*!
* \def Y2R_Ctrl_CMD
* write command of yuv2rgb module.
*/
#define  Y2R_Ctrl_CMD                 (0x000802ae | (RGB_DATA_WIDTH_18<<11))

#endif

