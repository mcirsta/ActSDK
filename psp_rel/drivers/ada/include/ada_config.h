/*
********************************************************************************
*                   213x --- usdk130 source file
*                (c) Copyright 2003 - 2008, Actions Co,Ld. 
*                        All Right Reserved 
*
* History:        
*      <author>      <time>       <version >    <desc>
*      houjingkun    2008/08/15     1.0      build this file 
*                                            ada char device driver's config file.
********************************************************************************/ 
/*!
 * \file   ada_config.h
 * \brief  
 *      ada char device driver's config file.
 * \author houjingkun
 * \par GENERAL DESCRIPTION:
 * \par EXTERNALIZED FUNCTIONS:
 *       null
 *
 *  Copyright(c) 2003-2008 Actions Semiconductor, All Rights Reserved.
 *
 * \version 1.0
 * \date  2008/08/15
 *******************************************************************************/

#ifndef  __ADA_CONFIG_H__
#define  __ADA_CONFIG_H__

#define USE_PCM_PLAY_IOCTL     1
#define USE_PCM_PLAY_WRITE     1

/* if define USE_PCM_PLAY_XXX, please define below... */
#define PCM_FRAME_LEN               (1024 * 16)    /* bytes. */
#define MIN_FRAME_LEN               (1024 * 6)

#define DEBUG_PRINT
//#define DEBUG_PRINT   printk(KERN_EMERG "%s:%d \n", __FILE__, __LINE__);

#endif  /* __ADA_CONFIG_H__ */

