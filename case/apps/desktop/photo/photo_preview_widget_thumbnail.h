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

    level_info_t        *cur_level_info;    /* 当前目录级别信息链表 */
    int                 item_per_screen;    /* 当前屏幕的文件数 */
    int                 total_file_num;     /* 当前目录下总文件数 */
    int                 total_dir_num;      /* 当前目录下总目录数 */
    int                 dec_index;          /* 当前解码index */

    int                 numx;               /* 水平方向预览图个数 */
    int                 numy;               /* 垂直方向预览图个数 */
    int                 first_x;            /* 第一个图片x坐标 */
    int                 first_y;            /* 第一个图片 y坐标*/
    int                 first_frame_x;      /* 第一个边框x坐标 */
    int                 first_frame_y;      /* 第一个边框y坐标 */
    int                 dist_x;             /* 两个水平相邻边框间距 */
    int                 dist_y;             /* 两个垂直相邻边框间距 */

    gui_bitmap_info_t   *bmp_sel;           /* 选择边框图片缓冲 */
    gui_bitmap_info_t   *bmp_unsel;         /* 非选择边框 图片缓冲 */

    slider_t            *slider;

} preview_thumbnail_t;

typedef void (* preview_thumbnail_cb_t)(preview_thumbnail_t *prv, preview_sig_e signal);

preview_thumbnail_t *photo_preview_widget_thumbnail_create(preview_thumbnail_cb_t callback);
void photo_preview_widget_thumbnail_paint(preview_thumbnail_t *prv);
void photo_preview_widget_thumbnail_rotate(preview_thumbnail_t *prv);
bool photo_preview_widget_thumbnail_delete(preview_thumbnail_t *prv);
bool photo_preview_widget_thumbnail_proc_gui(preview_thumbnail_t *prv, gui_wm_message_t *pmsg);

#endif

