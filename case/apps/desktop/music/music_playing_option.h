/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_playing_option.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_playing_option.h
* \brief    music_playing_option operation
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#ifndef __MUSIC_PLAYING_OPTION_H__
#define __MUSIC_PLAYING_OPTION_H__

#include "music.h"

/* �������˵� */
bool music_playing_option_enter( menuview_t *menuview );

/* ɾ������ */
bool playing_delete_process( void );

#endif

