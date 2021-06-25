/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_config.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_config.h
* \brief    music config
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#ifndef __MUSIC_CONFIG_H__
#define __MUSIC_CONFIG_H__

#include "music.h"


/* id3 cache size */
#define ID3_MAX_ITEMS 10

/* for test */
#define MUSIC_TEST  1


/*播放列表层次结构和排序方式*/
extern plistclass_level_info_t *g_plist_level;

/*用户从列表选择的条目的内容的缓存区*/
extern plistclass_usrselcontent_u g_usrsel_content;

/*PlayList排序方式*/
extern plist_sortstyle_t g_plist_sort_style;

/*当前播放索引*/
extern int g_file_index;

extern char g_dirlist_path[PATH_MAX];
extern char g_dirlist_top[PATH_MAX];

/* album coverflow mode */
extern bool g_coverflow_mode;

/* 目录列表*/
extern dirlist_t *g_dirlist;

/* 应用级style资源 */
extern style_t *g_music_style;
extern style_t *g_common_style;

#endif

