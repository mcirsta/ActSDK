/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music.h
* \brief    music include files
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#ifndef __MUSIC_H__
#define __MUSIC_H__

#include "include_case.h"


#include "ssf.h"
#include "act_pictureflow_external.h"

#include "music_sty.h"
#include "style_menu.h"
#include "plistclass.h"
#include "music_config.h"
#include "music_control.h"

#include "music_playing_widget_srsuser.h"
#include "music_playing_widget_usereq.h"

#include "music_list_mainmenu.h"
#include "music_list_dirlist.h"
#include "music_list_filelist.h"
#include "music_list_category.h"
#include "music_list_mylist.h"
#include "music_list_recfile.h"
#include "music_list_level.h"

#include "music_playing_option.h"
#include "music_playing_option_seleq.h"
#include "music_playing_option_playmode.h"
#include "music_playing_option_displaymode.h"
#include "music_playing_option_mylist.h"
#include "music_playing_option_level.h"
#include "music_playing_option_advanced.h"
#include "music_playing_option_setupeq.h"
#include "music_playing_option_info.h"

#include "music_list_widget_menu.h"
#include "music_playing_widget_option.h"

#include "music_scene_list.h"
#include "music_scene_playing.h"

#include "music_list_coverflow.h"
#include "music_playing_lyric.h"
#include "music_playing_ui.h"


/* scene type define */
typedef enum
{
    SCENE_EXIT = 0,    //exit state
    SCENE_LIST,
    SCENE_PLAY,
    
}app_scene_t;


typedef struct app_music_s
{
    /* system resource */
    void *applib_lib;
    void *commonui_lib;
    void *fusion_lib;
    void *gui_lib;
    void *sty_lib;
    void *sqlite_lib;
    void *mnavi_lib;
    void *plist_lib;
    void *config_lib;
    void *codec_lib;
        
    /* other data */
    app_scene_t next_scene;    //场景退出后进入的下一场景
    
}app_music_t;

extern app_music_t* g_app_music;

bool app_extra_init( app_music_t *app_music );

#endif

