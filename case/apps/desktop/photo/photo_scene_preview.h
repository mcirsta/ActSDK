/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_scene_preview.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *      carrie     2009-4-24          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_scene_preview.h
 * \brief    photo preview scene
 * \author   carrie
 *
 * \version 1.0
 * \date  2009/04/24
 *******************************************************************************/

#ifndef __PHOTO_SCENE_PREVIEW_H__
#define __PHOTO_SCENE_PREVIEW_H__

#include "photo.h"
#include "photo_preview_widget_thumbnail.h"
#include "photo_preview_widget_nplist.h"
#include "photo_preview_widget_dirlist.h"

/* preview场景退出类型 */
typedef enum
{
    SCENE_PREVIEW_RETURN,   /* 返回上一级 */
    SCENE_PREVIEW_NEXT,     /* 进入下一个场景 */
    SCENE_PREVIEW_EXIT_APP, /* 退出应用 */
    SCENE_PREVIEW_STORAGE_OUT,  /* 浏览卡/外部存储器目录时，浏览的设备被拔出 */

} scene_preview_ret_e;

/* define scene data */
typedef struct
{
    /* style resource */
    scene_t                 *common_scene;
    scene_t                 *scene;
    picture_resource_t      *pic_bg;        /* 背景图 */
    picture_resource_t      *pic_bg2;       /* 背景图2 */
    picture_resource_t      *pic_logo;
    string_resource_t       *str_title;
    picregion_resource_t    *picreg_sel00;  /* 左上边框 */
    picregion_resource_t    *picreg_sel01;  /* 右边相邻边框，用来计算水平预览图片个数  */
    picregion_resource_t    *picreg_sel10;  /* 下边相邻边框，用来计算垂直预览图片个数  */
    resgroup_resource_t     *resg_pic;      /* 图片资源组，坐标用来图片绘制 */

    picregion_resource_t    *picreg_err;    /* 解码错误图片 */
    picregion_resource_t    *picreg_folder; /* 文件夹图标 */
    string_resource_t       *str_no_file;   /* 无文件提示对话框 */
    string_resource_t       *str_plist_error;   /* 添加文件到播放列表失败 */
    string_resource_t       *str_not_found; /* 没有搜索到任何文件 */

    /* window resource */
    int                     win_id;

    preview_mode_e          preview_mode;
    int                     path_type;
    char                    *init_sel_path; /* 进入preview时，高亮的文件路径 */
    char                    *top_dir;       /* 当到达此目录，再向上一级时退出preview场景 */

    int                     dec_single_timer_id;   /* 解码完一副图片之后启动的single shot timer */
    int                     show_title_timer_id;   /* 在headbar上显示当前高亮文件名定时消失 */
    int                     search_single_timer_id;/* 搜索完成之后启动的timer */
    gui_bitmap_info_t       *bg_bitmap_info;    /* 背景图缓存 */
    int                     fontdes;

    /* 几种preview类型，同一时刻只能有一种 */
    preview_nplist_t        *preview_nplist;
    preview_dirlist_t       *preview_dirlist;
    preview_thumbnail_t     *preview_thumbnail;

    /* commonui widget */
    volumebar_t             *vbar;
    headbar_t               *hbar;
    msgbox_t                *msgbox;
    dialog_t                *dialog;
    softkeyboard_t          *softkey;
    loadingbox_t            *loadingbox;

    scene_preview_ret_e     exit_type;

} scene_preview_t;

/* scene entry */
int photo_scene_preview(const char *initpath, const char *toppath, fusion_enter_e fenter);
void photo_preview_show_warning(string_resource_t *str_res);

extern scene_preview_t* g_preview;

#endif

