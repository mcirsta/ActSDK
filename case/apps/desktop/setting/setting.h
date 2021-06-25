/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : setting_common.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-5-4             v1.0             build this file 
 ********************************************************************************/

#ifndef __SETTING_COMMON_H__
#define __SETTING_COMMON_H__

#include <include_case.h>


#ifdef ARRAY_SIZE
#undef ARRAY_SIZE
#endif

#define ARRAY_SIZE(A)    (sizeof(A)/sizeof((A)[0]))

/* scene type define */
typedef enum
{
    SCENE_EXIT = 0,
    SCENE_LIST,
}scene_status_t;

typedef struct 
{
    int win_id;

    scene_t* scene;
    scene_t* common_scene;

    /* font description*/
    int fontdes;

    /* widget */
    headbar_t *headbar;
    menuview_t *menu;
    adjust_t *adjust;
    dialog_t *dialog;
    volumebar_t *volumebar;
    softkeyboard_t *softkeyboard;
    msgbox_t * msgbox;
    progress_box_t * progress_box;
    loadingbox_t * loadingbox;

    /* resource for painting */
    picture_resource_t * bg_pic;
    gui_bitmap_info_t  * bg_bitmap_info;
    picture_resource_t * on_headbar_icon;
    string_resource_t  * on_headbar_title;

    style_t * commonui_style;
    style_t * app_style;

    scene_status_t exit_type;

}scene_list_t;

extern scene_list_t * g_scene_list;


#endif /* #ifndef __SETTING_COMMON_H__ */

