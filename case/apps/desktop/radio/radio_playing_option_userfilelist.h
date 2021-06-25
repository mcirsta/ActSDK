/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : radio_playing_option_userfilelist.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    wangsheng    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     radio_playing_option_userfilelist.h
* \author   wangsheng
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#ifndef __RADIO_PLAYING_OPTION_USERFILEILST_H__
#define __RADIO_PLAYING_OPTION_USERFILEIST_H__

#include "include_case.h"


#include "radio_sty.h"
#include "radio_scene_playing.h"
#include "radio_playing_option.h"

#define FILE_MODE   "ini"
#define WANNA_GET_NUM 6

/* 进入主菜单 */
bool radio_playing_option_userfilelist_enter( menuview_t *menuview );

/* 退出主菜单 */
bool radio_playing_option_userfilelist_exit( menuview_t *menuview );

/* 初始化菜单数据 */
menu_t* radio_playing_option_userfilelist_init( void );

/* 删除菜单数据 */
bool radio_playing_option_userfilelist_deinit( void );

bool update_userfilelist(void);

#endif
