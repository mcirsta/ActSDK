/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_preview_widget_dirlist.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason    2009-06-14          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_preview_widget_dirlist.h
 * \brief    photo dir list
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/06/14
 *******************************************************************************/

#ifndef __PHOTO_PREVIEW_WIDGET_DIRLIST_H__
#define __PHOTO_PREVIEW_WIDGET_DIRLIST_H__

#include "photo.h"

typedef struct preview_dirlist_s
{
    mmm_img_info_t      dec_info;

    void (*callback)(struct preview_dirlist_s *, preview_sig_e);

    dirlist_t           *dirlist;       /* dirlist控件 */

    int                 cur_dec_index;  /* 当前解码的图片在dirlist菜单屏幕中的index */
    gui_bitmap_info_t   *bmp_err;
    gui_bitmap_info_t   *bmp_folder;

} preview_dirlist_t;

typedef void (* preview_dirlist_cb_t)(preview_dirlist_t *prv, preview_sig_e signal);

preview_dirlist_t *photo_preview_widget_dirlist_create(preview_dirlist_cb_t callback);
void photo_preview_widget_dirlist_paint(preview_dirlist_t *prv);
void photo_preview_widget_dirlist_rotate(preview_dirlist_t *prv);
bool photo_preview_widget_dirlist_delete(preview_dirlist_t *prv);
bool photo_preview_widget_dirlist_proc_gui(preview_dirlist_t *prv, gui_wm_message_t *pmsg);

#endif

