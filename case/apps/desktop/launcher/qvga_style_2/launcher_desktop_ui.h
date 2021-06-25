/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : launcher_desktop_ui.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *      chencm    2009-04-22          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     launcher_desktop_ui.h
 * \brief    desktop scene
 * \author   chencm
 *
 * \version 1.0
 * \date  2009/06/09
 *******************************************************************************/

#ifndef __LAUNCHER_DESKTOP_UI__
#define __LAUNCHER_DESKTOP_UI__

#include "./../launcher_scene_desktop.h"

#define ARRAY_NUM(A) sizeof(A)/sizeof((A)[0])

typedef enum
{
    STEP_1 = 0,
    STEP_2,
    STEP_3,
    STEP_INIT,
    STEP_COUNT
} animation_step_e;

typedef enum
{
    TYPE_MUSIC = 0,
    TYPE_VIDEO,
    TYPE_TVIN,
    TYPE_PHOTO,
    TYPE_CAMERA,
    TYPE_RECORDER,
    TYPE_RADIO,
    TYPE_EBOOK,
    TYPE_GAMES,
    TYPE_TOOLS,
    TYPE_BROWSER,
    TYPE_SETTING,
    TYPE_TEST_COM,
    TYPE_USB,
    TYPE_COUNT
} app_e;

typedef struct app_style_map_s
{
    char * app_name;
    app_e app_type;
} app_map_t;

#define ROW_NUM     3
#define MAX_IN_ROW  4
#define ALPHA_30PAR 77  //255*30/100
#define ALPHA_70PAR 178 //255*70/100

extern int debug_time;

extern bool desktop_load_style(void);
extern bool desktop_buffer_resource(void);
extern bool desktop_release_resource(void);
extern picture_resource_t * desktop_get_headbar_appicon(appinfo_list_t *data);
extern void show_desktop_bg(void);
extern void show_next_desktop(scene_desktop_t * data);
extern void show_child_desktop(scene_desktop_t * data);
extern void show_current_desktop(scene_desktop_t * data);

#endif  /* #ifndef __LAUNCHER_DESKTOP_UI__ */

