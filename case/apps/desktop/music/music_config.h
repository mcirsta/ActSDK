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


/*�����б��νṹ������ʽ*/
extern plistclass_level_info_t *g_plist_level;

/*�û����б�ѡ�����Ŀ�����ݵĻ�����*/
extern plistclass_usrselcontent_u g_usrsel_content;

/*PlayList����ʽ*/
extern plist_sortstyle_t g_plist_sort_style;

/*��ǰ��������*/
extern int g_file_index;

extern char g_dirlist_path[PATH_MAX];
extern char g_dirlist_top[PATH_MAX];

/* album coverflow mode */
extern bool g_coverflow_mode;

/* Ŀ¼�б�*/
extern dirlist_t *g_dirlist;

/* Ӧ�ü�style��Դ */
extern style_t *g_music_style;
extern style_t *g_common_style;

#endif

