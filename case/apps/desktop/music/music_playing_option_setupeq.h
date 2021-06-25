/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_playing_option_setupeq.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_playing_option_setupeq.h
* \brief    music_playing_option_setupeq operation
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#ifndef __MUSIC_PLAYING_OPTION_SETUPEQ_H__
#define __MUSIC_PLAYING_OPTION_SETUPEQ_H__

#include "music.h"


/* 进入主菜单 */
bool music_playing_option_setupeq_enter( menuview_t *menuview );

/* common process for usereq and srsuser */
bool com_create_usereq_widget( void );
bool com_delete_usereq_widget( void );
void com_usereq_widget_cb( usereq_widget_t *usereq_widget, usereq_widget_sig_e signal );

bool com_create_srsuser_widget( void );
bool com_delete_srsuser_widget( void );
void com_srsuser_widget_cb( srsuser_widget_t *srsuser_widget, srsuser_widget_sig_e signal );

#endif

