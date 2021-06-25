/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : browser.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *    binannan    2009-05-25          v1.0               build this file
 ********************************************************************************/

#ifndef __BROWSER_H_
#define __BROWSER_H_

#include "include_case.h"
#include "include_psp.h"

/* scene type define */
typedef enum
{
    SCENE_EXIT = 0,
    SCENE_BROWSING,
} scene_status_t;

typedef struct app_browser_s
{
    /* system resource */
    void* applib_lib;
    void* commonui_lib;
    void* fusion_lib;
    void* gui_lib;
    void* sty_lib;
    void* apconfig_lib;

    style_t* browser_style;
    style_t* commonui_style;

    scene_status_t next_scene;
} app_browser_t;

extern app_browser_t* g_app_browser;

extern bool g_resume_boot;

#endif /* BROWSER_H_ */
