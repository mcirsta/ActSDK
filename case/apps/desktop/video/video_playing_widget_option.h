/*******************************************************************************
 *                              NOYA1100
 *                            Module: video
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-05-15          1.0             build this file 
*******************************************************************************/
/*!
 * \file     video_playing_widget_option.h
 * \brief    
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2009/05/15
*******************************************************************************/

#ifndef __VIDEO_PLAYING_WIDGET_OPTION_H__
#define __VIDEO_PLAYING_WIDGET_OPTION_H__


#include "include_case.h"



typedef bool (*option_entry_t)( menuview_t * );


/************************ extern function, º¯ÊýÉùÃ÷ ***************************/
extern bool video_playing_widget_option_create( menuview_cb_t callback );
extern bool video_playing_widget_option_delete( menuview_t *menuview );

#endif  /* __VIDEO_PLAYING_WIDGET_OPTION_H__ */

