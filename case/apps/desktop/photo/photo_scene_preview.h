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

/* preview�����˳����� */
typedef enum
{
    SCENE_PREVIEW_RETURN,   /* ������һ�� */
    SCENE_PREVIEW_NEXT,     /* ������һ������ */
    SCENE_PREVIEW_EXIT_APP, /* �˳�Ӧ�� */
    SCENE_PREVIEW_STORAGE_OUT,  /* �����/�ⲿ�洢��Ŀ¼ʱ��������豸���γ� */

} scene_preview_ret_e;

/* define scene data */
typedef struct
{
    /* style resource */
    scene_t                 *common_scene;
    scene_t                 *scene;
    picture_resource_t      *pic_bg;        /* ����ͼ */
    picture_resource_t      *pic_bg2;       /* ����ͼ2 */
    picture_resource_t      *pic_logo;
    string_resource_t       *str_title;
    picregion_resource_t    *picreg_sel00;  /* ���ϱ߿� */
    picregion_resource_t    *picreg_sel01;  /* �ұ����ڱ߿���������ˮƽԤ��ͼƬ����  */
    picregion_resource_t    *picreg_sel10;  /* �±����ڱ߿��������㴹ֱԤ��ͼƬ����  */
    resgroup_resource_t     *resg_pic;      /* ͼƬ��Դ�飬��������ͼƬ���� */

    picregion_resource_t    *picreg_err;    /* �������ͼƬ */
    picregion_resource_t    *picreg_folder; /* �ļ���ͼ�� */
    string_resource_t       *str_no_file;   /* ���ļ���ʾ�Ի��� */
    string_resource_t       *str_plist_error;   /* ����ļ��������б�ʧ�� */
    string_resource_t       *str_not_found; /* û���������κ��ļ� */

    /* window resource */
    int                     win_id;

    preview_mode_e          preview_mode;
    int                     path_type;
    char                    *init_sel_path; /* ����previewʱ���������ļ�·�� */
    char                    *top_dir;       /* �������Ŀ¼��������һ��ʱ�˳�preview���� */

    int                     dec_single_timer_id;   /* ������һ��ͼƬ֮��������single shot timer */
    int                     show_title_timer_id;   /* ��headbar����ʾ��ǰ�����ļ�����ʱ��ʧ */
    int                     search_single_timer_id;/* �������֮��������timer */
    gui_bitmap_info_t       *bg_bitmap_info;    /* ����ͼ���� */
    int                     fontdes;

    /* ����preview���ͣ�ͬһʱ��ֻ����һ�� */
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

