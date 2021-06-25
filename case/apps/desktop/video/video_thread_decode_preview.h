/*******************************************************************************
 *                              NOYA1100
 *                            Module: video
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-06-19          1.0             build this file 
*******************************************************************************/
/*!
 * \file     video_thread_decode_preview.h
 * \brief    
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2009/06/19
*******************************************************************************/

#ifndef __VIDEO_THREAD_DECODE_PREVIEW_H__
#define __VIDEO_THREAD_DECODE_PREVIEW_H__


#include "include_case.h"



/************************ extern function, º¯ÊýÉùÃ÷ ***************************/
extern bool video_thread_get_one_preview( char *fullpath, unsigned int time, mmm_cur_video_t *preview_info );
extern bool video_thread_decode_preview_stop( void );
extern bool video_thread_decode_preview_create( void *(*start_rtn)(void*) );
extern bool video_thread_decode_preview_delete( void );


#endif /* __VIDEO_THREAD_DECODE_PREVIEW_H__ */

