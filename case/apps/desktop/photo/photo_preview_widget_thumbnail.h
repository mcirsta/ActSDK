/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_preview_widget_thumbnail.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason    2009-06-14         v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_preview_widget_thumbnail.h
 * \brief    photo preview thumbnail widget
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/06/14
 *******************************************************************************/

#ifndef __PHOTO_PREVIEW_WIDGET_THUMBNAIL_H__
#define __PHOTO_PREVIEW_WIDGET_THUMBNAIL_H__

#include "photo.h"

typedef enum
{
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN,
} move_dir_e;

typedef struct
{
    dec_file_info_t info;
    bool is_dir;
} thumbnail_item_t;

typedef struct preview_thumbnail_s
{
    thumbnail_item_t    *previewitem;

    FSEL_HANDLE         fsel;

    void (*callback)(struct preview_thumbnail_s *, preview_sig_e);

    level_info_t        *cur_level_info;    /* ��ǰĿ¼������Ϣ���� */
    int                 item_per_screen;    /* ��ǰ��Ļ���ļ��� */
    int                 total_file_num;     /* ��ǰĿ¼�����ļ��� */
    int                 total_dir_num;      /* ��ǰĿ¼����Ŀ¼�� */
    int                 dec_index;          /* ��ǰ����index */

    int                 numx;               /* ˮƽ����Ԥ��ͼ���� */
    int                 numy;               /* ��ֱ����Ԥ��ͼ���� */
    int                 first_x;            /* ��һ��ͼƬx���� */
    int                 first_y;            /* ��һ��ͼƬ y����*/
    int                 first_frame_x;      /* ��һ���߿�x���� */
    int                 first_frame_y;      /* ��һ���߿�y���� */
    int                 dist_x;             /* ����ˮƽ���ڱ߿��� */
    int                 dist_y;             /* ������ֱ���ڱ߿��� */

    gui_bitmap_info_t   *bmp_sel;           /* ѡ��߿�ͼƬ���� */
    gui_bitmap_info_t   *bmp_unsel;         /* ��ѡ��߿� ͼƬ���� */

    slider_t            *slider;

} preview_thumbnail_t;

typedef void (* preview_thumbnail_cb_t)(preview_thumbnail_t *prv, preview_sig_e signal);

preview_thumbnail_t *photo_preview_widget_thumbnail_create(preview_thumbnail_cb_t callback);
void photo_preview_widget_thumbnail_paint(preview_thumbnail_t *prv);
void photo_preview_widget_thumbnail_rotate(preview_thumbnail_t *prv);
bool photo_preview_widget_thumbnail_delete(preview_thumbnail_t *prv);
bool photo_preview_widget_thumbnail_proc_gui(preview_thumbnail_t *prv, gui_wm_message_t *pmsg);

#endif

