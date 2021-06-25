/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_option_info.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason    2009-06-14         v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_option_info.c
 * \brief    information of photo playing option
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/06/14
 *******************************************************************************/

#include "photo_scene_playing.h"
#include "photo_option.h"
#include "photo_option_info.h"
#include "photo_dec_ctrl.h"
#include "photo_option_widget_menu.h"

static int _proc_item(menuview_t *menuview, menuview_sig_e signal);
static void _fill_items(menu_t *menu, scene_playing_t *p);
static void _info_init(void);
static void _info_deinit(void);

/* menu list data define */
static menu_t *s_info = NULL;

static int _proc_item(menuview_t *menuview, menuview_sig_e signal)
{
    switch (signal)
    {
    case WIDGET_SIG_MENUVIEW_OK:
    case WIDGET_SIG_MENUVIEW_QUIT:
        photo_option_info_exit(menuview);
        break;

    default:
        break;
    }
    return 0;
}

static void _fill_items(menu_t *menu, scene_playing_t *p)
{
    static char str_resolution[33];
    static char str_filesize[33];
    static char str_date[33];

    menuitem_t item;
    stat_t st;

    memset(&item, 0, sizeof(menuitem_t));

    item.has_child = false;
    item.callback = (item_cb_t)_proc_item;

    /* file name */
    item.text_id = (int)p->resg_opt_info.str_filename->str_id;
    item.info1 = p->cur_file_info.file_name;
    item.unsel_icon_id = (int)p->resg_opt_info.pic_filename->pic_id;
    item.sel_icon_id = item.unsel_icon_id;
    menu_append_item(menu, &item);

    /* resolution */
    item.text_id = (int)p->resg_opt_info.str_resolution->str_id;
    sprintf(str_resolution, "%d * %d", p->cur_file_info.file_info.width, p->cur_file_info.file_info.height);
    item.info1 = str_resolution;
    item.unsel_icon_id = (int)p->resg_opt_info.pic_resolution->pic_id;
    item.sel_icon_id = item.unsel_icon_id;
    menu_append_item(menu, &item);

    /* file size */
    item.text_id = (int)p->resg_opt_info.str_filesize->str_id;
    stat(p->cur_file_info.file_path, &st);
    sprintf(str_filesize, "%d KB", st.st_size/1024);
    item.info1 = str_filesize;
    item.unsel_icon_id = (int)p->resg_opt_info.pic_filesize->pic_id;
    item.sel_icon_id = item.unsel_icon_id;
    menu_append_item(menu, &item);

    /* date */
    item.text_id = (int)p->resg_opt_info.str_date->str_id;
    sprintf(str_date, "%04d-%02d-%02d", p->cur_file_info.file_info.date.year,
            p->cur_file_info.file_info.date.month,
            p->cur_file_info.file_info.date.day);
    item.info1 = str_date;
    item.unsel_icon_id = 0;
    item.sel_icon_id = 0;
    menu_append_item(menu, &item);
}

static void _info_init(void)
{
    g_cur_opt_state = OPT_INFO;
    if (NULL == s_info)
    {
        s_info = photo_menu_create();
        _fill_items(s_info, g_playing);
    }
}

static void _info_deinit(void)
{
    if (NULL != s_info)
    {
        photo_menu_delete(s_info);
        s_info = NULL;
    }
}

bool photo_option_info_enter(menuview_t *mv, fusion_enter_e fenter)
{
    _info_init();
    menuview_set_menu(mv, s_info);

    if(mv->type != MENUVIEW_OPTION_INFO)
    {
        menuview_load_style(mv, MENUVIEW_OPTION_INFO);
    }
    menuview_paint(mv);
    return true;
}

bool photo_option_info_exit(menuview_t *mv)
{
    _info_deinit();
    photo_option_enter(mv, FUSION_BACKWARD);
    return true;
}
