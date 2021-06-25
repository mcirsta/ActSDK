/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_scene_listmenu.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *      carrie     2009-4-24          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_scene_listmenu.h
 * \brief    photo listmenu scene
 * \author   carrie
 *
 * \version 1.0
 * \date  2009/04/24
 *******************************************************************************/

#ifndef __PHOTO_SCENE_LIST_H__
#define __PHOTO_SCENE_LIST_H__

#include "photo.h"

#include "fusion.h"

/* exit type */
typedef enum
{
    SCENE_LIST_RETURN,
    SCENE_LIST_NOWPLAYING_NEXT,
    SCENE_LIST_DIRLIST_NEXT,
    SCENE_LIST_EXIT_APP,

} scene_list_ret_e;

typedef struct
{
    string_resource_t *str_nowplaying;
    string_resource_t *str_dirlist;
    string_resource_t *str_previewmode;
    string_resource_t *str_slideinterval;
    string_resource_t *str_slideeffect;
    string_resource_t *str_nodisk;
    string_resource_t *str_plist_error;

} resg_mainmenu_t;

typedef struct
{
    picture_resource_t *pic_current;
    string_resource_t *str_listmode;
    string_resource_t *str_thumbnail;

} resg_previewmode_t;

typedef struct
{
    string_resource_t *str_sec;

} resg_slideinterval_t;

typedef struct
{
    picture_resource_t *pic_current;
    string_resource_t *str_random;
    string_resource_t *str_flyin;
    string_resource_t *str_check;
    string_resource_t *str_cut;
    string_resource_t *str_erase;
    string_resource_t *str_louver;
    string_resource_t *str_randomline;
    string_resource_t *str_waterwave;
    string_resource_t *str_cube;
    string_resource_t *str_3drotation;
    string_resource_t *str_off;

} resg_slideeffect_t;

typedef struct
{
    string_resource_t *str_int_mem;
    string_resource_t *str_car_mem;
    string_resource_t *str_ext_mem;
} resg_dirlist_t;

/* define scene data */
typedef struct scene_list_s
{
    /* window resource */
    int                     win_id;

    /* style resource */
    scene_t                 *common_scene;
    scene_t                 *scene;
    picture_resource_t      *pic_bg;
    picture_resource_t      *pic_logo;
    string_resource_t       *str_title;

    resg_mainmenu_t         resg_mainmenu;
    resg_dirlist_t          resg_dirlist;
    resg_previewmode_t      resg_previewmode;
    resg_slideinterval_t    resg_slideinterval;
    resg_slideeffect_t      resg_slideeffect;

    /*previewmode*/
    preview_mode_e          preview_mode;
    int                     slide_interval;
    int                     slide_effect_index;     /* slide 特效模式索引 */
    bool                    nowplaying_flag;        /* 是否在主菜单上有nowplaying项 */
    gui_bitmap_info_t       *bg_bitmap_info;        /* 背景图缓存 */
    int                     fontdes;

    /* widget resource */
    volumebar_t             *vbar;
    headbar_t               *hbar;
    menuview_t              *menuview;
    adjust_t                *adjust;
    msgbox_t                *msgbox;
    dialog_t                *dialog;

    scene_list_ret_e        exit_type;

} scene_list_t;

/* list scene entry */
int photo_scene_list(fusion_enter_e fenter);
void scene_list_paint(scene_list_t *scene_list);

extern scene_list_t* g_scene_list;

#endif

