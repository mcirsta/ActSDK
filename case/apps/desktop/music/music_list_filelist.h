/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_list_filelist.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_list_filelist.h
* \brief    music_list_filelist operation
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#ifndef __MUSIC_LIST_FILELIST_H__
#define __MUSIC_LIST_FILELIST_H__

#include "music.h"

/* 进入主菜单 */
bool music_list_filelist_enter( menuview_t *menuview, fusion_enter_e fenter );

/* 检查文件是否可以播放*/
bool music_list_check_file( char *filepath, int file_index );

#endif

