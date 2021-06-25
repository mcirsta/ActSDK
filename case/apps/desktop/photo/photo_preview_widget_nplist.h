/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_preview_widget_nplist.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason    2009-06-14         v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_preview_widget_nplist.h
 * \brief    photo preview nowplaying list
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/06/14
 *******************************************************************************/

#ifndef __PHOTO_PREVIEW_WIDGET_NPLIST_H__
#define __PHOTO_PREVIEW_WIDGET_NPLIST_H__

#include "photo.h"

typedef struct preview_nplist_s
{
    dec_file_info_t     *previewitem;
    mmm_image_status_t  preview_dec_status;

    void (*callback)(struct preview_nplist_s *, preview_sig_e);

    int                 cur_index;      /* 当前屏幕中选择的index */
    int                 start_index;    /* 当前屏幕起始文件在总文件中的index */
    int                 total_num;      /* 总文件数 */
    int                 item_per_screen; /* 每屏幕文件数 */

    menuview_t          *menuview;      /* 菜单视图 */
    menu_t              *menu;          /* 菜单数据 */

    int                 dec_index;      /* 正在解码图片的index */
    gui_bitmap_info_t   *bmp_err_sel;
    gui_bitmap_info_t   *bmp_err_unsel;

} preview_nplist_t;

typedef void (* preview_nplist_cb_t)(preview_nplist_t *prv, preview_sig_e signal);

preview_nplist_t *photo_preview_widget_nplist_create(preview_nplist_cb_t callback);
void photo_preview_widget_nplist_paint(preview_nplist_t *prv);
void photo_preview_widget_nplist_rotate(preview_nplist_t *prv);
bool photo_preview_widget_nplist_delete(preview_nplist_t *prv);
bool photo_preview_widget_nplist_proc_gui(preview_nplist_t *prv, gui_wm_message_t *pmsg);

#endif

