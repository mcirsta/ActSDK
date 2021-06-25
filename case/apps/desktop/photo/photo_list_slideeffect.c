/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_list_slideeffect.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      cheason     2009-06-14         v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_list_slideeffect.c
 * \brief    slideeffect photo list
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/06/14
 *******************************************************************************/

#include "photo.h"
#include "photo_scene_list.h"
#include "photo_list_slideeffect.h"
#include "photo_list_mainmenu.h"

static int _proc_item(menuview_t *menuview, menuview_sig_e signal);
static void _fill_items(menu_t *menu, scene_list_t *sclist);
static void _slideeffect_init(void);
static void _slideeffect_deinit(void);

/* menu list data define */
static menu_t *s_slideeffect = NULL;

static int _proc_item(menuview_t *menuview, menuview_sig_e signal)
{
    switch (signal)
    {
    case WIDGET_SIG_MENUVIEW_OK:
        g_scene_list->slide_effect_index = menuview->menu->current;

    case WIDGET_SIG_MENUVIEW_QUIT:
        photo_list_slideeffect_exit(menuview);
        break;

    default:
        break;
    }

    return 0;
}

static void _fill_items(menu_t *menu, scene_list_t *sclist)
{
    menuitem_t item;
    resg_slideeffect_t *resg = &sclist->resg_slideeffect;

    memset(&item, 0, sizeof(menuitem_t));

    /* 填充顺序要跟effect_item_e定义的一致 */
    _add_item(menu, &item, resg->str_random, false, (item_cb_t) _proc_item);
    _add_item(menu, &item, resg->str_flyin, false, (item_cb_t) _proc_item);
    _add_item(menu, &item, resg->str_check, false, (item_cb_t) _proc_item);
    _add_item(menu, &item, resg->str_cut, false, (item_cb_t) _proc_item);
    _add_item(menu, &item, resg->str_erase, false, (item_cb_t) _proc_item);
    _add_item(menu, &item, resg->str_louver, false, (item_cb_t) _proc_item);
    _add_item(menu, &item, resg->str_randomline, false, (item_cb_t) _proc_item);
    _add_item(menu, &item, resg->str_waterwave, false, (item_cb_t) _proc_item);
    _add_item(menu, &item, resg->str_cube, false, (item_cb_t) _proc_item);
    _add_item(menu, &item, resg->str_3drotation, false, (item_cb_t) _proc_item);
    _add_item(menu, &item, resg->str_off, false, (item_cb_t) _proc_item);
}

static void _slideeffect_init(void)
{
    menuitem_t *item;

    if (NULL == s_slideeffect)
    {
        s_slideeffect = photo_menu_create();
        _fill_items(s_slideeffect, g_scene_list);
        item = menu_get_item(s_slideeffect, g_scene_list->slide_effect_index);
        assert(NULL != item);
        item->sel_icon_id = (int)g_scene_list->resg_slideeffect.pic_current->pic_id;
        item->unsel_icon_id = (int)g_scene_list->resg_slideeffect.pic_current->pic_id;
    }
}

static void _slideeffect_deinit(void)
{
    if (NULL != s_slideeffect)
    {
        photo_menu_delete(s_slideeffect);
        s_slideeffect = NULL;
    }
}

bool photo_list_slideeffect_enter(menuview_t *mv, fusion_enter_e fenter)
{
    fusion_effect_t eff;

    fusion_get_config(FUSION_TYPE_COMMON, fenter, &eff);
    fusion_set_effect(&eff);
    scene_fusion_set_first_frame(g_scene_list->scene);

    scene_list_paint(g_scene_list);

    _slideeffect_init();
    menuview_set_menu(mv, s_slideeffect);

    if(mv->type != MENUVIEW_MENULIST_ICON)
    {
        menuview_load_style(mv, MENUVIEW_MENULIST_ICON);
    }
    menuview_paint(mv);

    scene_fusion_set_last_frame(g_scene_list->scene);
    fusion_display(NULL);

    return true;
}

bool photo_list_slideeffect_exit(menuview_t *mv)
{
    _slideeffect_deinit();
    photo_list_mainmenu_enter(mv, FUSION_BACKWARD);
    return true;
}

