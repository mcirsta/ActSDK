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

/*�����б��νṹ������ʽ*/
static plistclass_level_info_t _level_info = 
{
    LEVEL_ONE_NONE_LIST,
    LEVEL_TWO_NONE_LIST,
    LEVEL_THREE_NONE_LIST,
    PINYINSORT
};

/*�����б��νṹ������ʽ*/
plistclass_level_info_t *g_plist_level = &_level_info;

/*�û����б�ѡ�����Ŀ�����ݵĻ�����*/
plistclass_usrselcontent_u g_usrsel_content;

/*PlayList����ʽ*/
plist_sortstyle_t g_plist_sort_style = PINYINSORT;

/*��ǰ��������*/
int g_file_index = -1;

char g_dirlist_path[PATH_MAX];
char g_dirlist_top[PATH_MAX];

/* album coverflow mode */
bool g_coverflow_mode = false;

/* Ŀ¼�б�*/
dirlist_t *g_dirlist = NULL;

/* Ӧ�ü�style��Դ */
style_t *g_music_style = NULL;
style_t *g_common_style = NULL;
