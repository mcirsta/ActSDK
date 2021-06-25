/*
********************************************************************************
*                   213x --- tvin's driver
*                (c) Copyright 2007 - 2010, Actions Co,Ld. 
*                        All Right Reserved 
*
* History:        
*      <author>      <time>       <version >    <desc>
*       houjingkun      2008/06/18     1.0         build this file 
*       new added tvin_inner.h and tvin_inner.c for video in device driver.
********************************************************************************/ 
/*!
 * \file   tvin_inner.h
 * \brief  
 *      Tvin driver logic layer's API.
 * \author houjingkun
 * \par GENERAL DESCRIPTION:
 *      Tvin driver logic layer's API.  just called by dirver itself.
 * \par EXTERNALIZED FUNCTIONS:
 *       null
 *
 *     Copyright(c) 2008-2012 Actions Semiconductor, All Rights Reserved.
 *
 * \version 1.0
 * \date  2008/06/18
 *******************************************************************************/

#ifndef _TVIN_INNER_H_
#define _TVIN_INNER_H_

#include "vi_config.h"

#define TEST_TVIN_ON_LCD    0
/*
 ********************************************************
 * realized in tvin_inner.c
 ********************************************************
*/
extern int tvin_open(unsigned int type);

extern int tvin_init(unsigned int type);

extern int tvin_exit(void);

extern int tvin_close(unsigned int type);

extern int tvin_ioctl(unsigned int cmd, unsigned long arg);

#endif  /* _TVIN_INNER_H_ */

