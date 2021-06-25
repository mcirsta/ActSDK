/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : launcher_scene_desktop.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *       chencm    2009-0-23          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     launcher_scene_desktop.h
 * \brief    main menu scene
 * \author   chencm
 *
 * \version 1.0
 * \date  2009/04/23
 *******************************************************************************/

#ifndef __LAUNCHER_SCENE_DESKTOP_H__
#define __LAUNCHER_SCENE_DESKTOP_H__

#include <include_case.h>
#include "launcher.h"
#include "desktop_appinfo.h"


typedef enum
{
    DEFAULT = 0,
    PREV_ANIMATION,
    NEXT_ANIMATION,
    DISPLAY_CHILD
}desktop_display_e;


/* define scene data */
typedef struct scene_desktop_s
{
    void *gui_lib;

    /* if load by console, default is true*/
    bool is_console;
    bool language_changed;
    bool app_exited;

    /* style resource */
    style_t* app_style;
    style_t* common_style;
    scene_t* scene;
    scene_t* common_scene;

    /*common ui*/
    headbar_t * headbar;
    menu_t * menu;
    menuview_t * menuview;
    volumebar_t * volumebar;
    msgbox_t * msgbox;

    /* window resource */
    int win_id;
    /* hdc */
    int hdc;
    int fontdes;

    appinfo_list_t * cur_desktop_list;
    appinfo_list_t * last_desktop_list;
    appinfo_list_t * cur_desktop;
    int  focus_app_id;

    desktop_display_e display_mode;
    scene_status_e exit_type;

    picture_resource_t * on_headbar_icon;

} scene_desktop_t;

extern scene_desktop_t *g_scene_desktop;

/* scene entry */
extern int launcher_scene_desktop(scene_desktop_t * scene_data);

#endif /* #ifndef __LAUNCHER_SCENE_DESKTOP_H__ */
