/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_config.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_config.c
* \brief    music config
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#include "music_config.h"

/*播放列表层次结构和排序方式*/
static plistclass_level_info_t _level_info = 
{
    LEVEL_ONE_NONE_LIST,
    LEVEL_TWO_NONE_LIST,
    LEVEL_THREE_NONE_LIST,
    PINYINSORT
};

/*播放列表层次结构和排序方式*/
plistclass_level_info_t *g_plist_level = &_level_info;

/*用户从列表选择的条目的内容的缓存区*/
plistclass_usrselcontent_u g_usrsel_content;

/*PlayList排序方式*/
plist_sortstyle_t g_plist_sort_style = PINYINSORT;

/*当前播放索引*/
int g_file_index = -1;

char g_dirlist_path[PATH_MAX];
char g_dirlist_top[PATH_MAX];

/* album coverflow mode */
bool g_coverflow_mode = false;

/* 目录列表*/
dirlist_t *g_dirlist = NULL;

/* 应用级style资源 */
style_t *g_music_style = NULL;
style_t *g_common_style = NULL;
