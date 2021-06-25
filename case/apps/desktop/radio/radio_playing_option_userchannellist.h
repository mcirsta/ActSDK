/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : radio_playing_option_userchannellist.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    wangsheng    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     radio_playing_option_userchannellist.h
* \author   wangsheng
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#ifndef __RADIO_PLAYING_OPTION_USERCHANNELILST_H__
#define __RADIO_PLAYING_OPTION_USERCHANNELILST_H__

#include "include_case.h"
#include "radio_sty.h"

/* �������˵� */
bool radio_playing_option_userchannellist_enter( menuview_t *menuview, char *path);

/* �˳����˵� */
bool radio_playing_option_userchannellist_exit( menuview_t *menuview );

/* ��ʼ���˵����� */
menu_t* radio_playing_option_userchannellist_init(menuview_t *menuview, char *path);

/* ɾ���˵����� */
bool radio_playing_option_userchannellist_deinit( void );


#endif
