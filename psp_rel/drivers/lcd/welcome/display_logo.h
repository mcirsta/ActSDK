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
* \file Display_logo.h
* \brief to provide external APIs
* \author shaofeng
* \par GENERAL DESCRIPTION:
*       This file provide function prototypes and definations.
* \par DECLARATIONS:
*       (c) Copyright 2002-2004, Actions Co,Ld. All Right Reserved.
*
* \version 1.0
* \date 2007/11/25
*/


#include "lcd_yuv2rgb.h"
#include "lcd_inner.h"
#include "brec_info.h"


#ifndef _LCM_BREC_H
#define _LCM_BREC_H

#define BPP 16
//#define XRES 320
//#define YRES 240
/**
external interface
*/
void  display_logo(void);
int  is_usb_connected( char ver);
extern int brec_switch_adfu(void);
extern void act_buttons_init(void);
void * lcd_memcpy(void * dest,const void *src,unsigned int count);
void act_buttons_init(void);
void act_buttons_exit(void);
#endif //_LCM_BREC_H

