/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_list_previewmode.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason    2009-06-14         v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_list_previewmode.c
 * \brief    previewmode of photo list
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/06/14
 *******************************************************************************/

#include "photo.h"
#include "photo_scene_list.h"
#include "photo_list_previewmode.h"
#include "photo_list_mainmenu.h"
#include "photo_list_slideeffect.h"

static int _proc_item(menuview_t *menuview, menuview_sig_e signal);
static void _fill_items(menu_t *menu, scene_list_t *sclist);
static void _previewmode_init(void);
static void _previewmode_deinit(void);

/* menu list data define */
static menu_t *s_previewmode = NULL;

static int _proc_item(menuview_t *menuview, menuview_sig_e signal)
{
    switch (signal)
    {
    case WIDGET_SIG_MENUVIEW_OK:
        g_scene_list->preview_mode = menuview->menu->current;
        photo_list_previewmode_exit(menuview);
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        photo_list_previewmode_exit(menuview);
        break;

    default:
        break;
    }
    return 0;
}

static void _fill_items(menu_t *menu, scene_list_t *sclist)
{
    menuitem_t item;
    resg_previewmode_t *resg = &sclist->resg_previewmode;

    memset(&item, 0, sizeof(menuitem_t));
    _add_item(menu, &item, resg->str_listmode, false, (item_cb_t) _proc_item);
    _add_item(menu, &item, resg->str_thumbnail, false, (item_cb_t) _proc_item);
}

static void _previewmode_init(void)
{
    menuitem_t *item;

    if (NULL == s_previewmode)
    {
        s_previewmode = photo_menu_create();
        assert(NULL != s_previewmode);
        _fill_items(s_previewmode, g_scene_list);
        item = menu_get_item(s_previewmode, g_scene_list->preview_mode);
        assert(NULL != item);
        item->sel_icon_id = (int)g_scene_list->resg_previewmode.pic_current->pic_id;
        item->unsel_icon_id = (int)g_scene_list->resg_previewmode.pic_current->pic_id;
    }
}

static void _previewmode_deinit(void)
{
    if (NULL != s_previewmode)
    {
        photo_menu_delete(s_previewmode);
        s_previewmode = NULL;
    }
}

bool photo_list_previewmode_enter(menuview_t *mv, fusion_enter_e fenter)
{
    fusion_effect_t eff;

    fusion_get_config(FUSION_TYPE_COMMON, fenter, &eff);
    fusion_set_effect(&eff);
    scene_fusion_set_first_frame(g_scene_list->scene);

    scene_list_paint(g_scene_list);

    _previewmode_init();
    menuview_set_menu(mv, s_previewmode);

    if(mv->type != MENUVIEW_MENULIST_ICON)
    {
        menuview_load_style(mv, MENUVIEW_MENULIST_ICON);
    }
    menuview_paint(mv);

    scene_fusion_set_last_frame(g_scene_list->scene);
    fusion_display(NULL);

    return true;
}

bool photo_list_previewmode_exit(menuview_t *mv)
{
    _previewmode_deinit();
    photo_list_mainmenu_enter(mv, FUSION_BACKWARD);
    return true;
}

