/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_option_slideeffect.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason    2009-06-14         v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_option_slideeffect.c
 * \brief    slide effect of photo playing option
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/06/14
 *******************************************************************************/

#include "photo_scene_playing.h"
#include "photo_option.h"
#include "photo_option_slideeffect.h"
#include "photo_option_widget_menu.h"

static int _proc_item(menuview_t *menuview, menuview_sig_e signal);
static void _fill_items(menu_t *menu, scene_playing_t *p);
static void _slideeffect_init(void);
static void _slideeffect_deinit(void);

/* menu list data define */
static menu_t *s_slideeffect = NULL;

static int _proc_item(menuview_t *menuview, menuview_sig_e signal)
{
    switch (signal)
    {
    case WIDGET_SIG_MENUVIEW_OK:
        g_playing->fusion_mode = photo_slide_get_fusion_mode(menuview->menu->current);

    case WIDGET_SIG_MENUVIEW_QUIT:
        photo_option_slideeffect_exit(menuview);
        break;

    default:
        break;
    }
    return 0;
}

static void _fill_items(menu_t *menu, scene_playing_t *p)
{
    menuitem_t item;
    resg_opt_slideeffect_t *resg = &p->resg_opt_slideeffect;

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

    g_cur_opt_state = OPT_SLIDE_EFFECT;
    if (NULL == s_slideeffect)
    {
        s_slideeffect = photo_menu_create();
        assert(NULL != s_slideeffect);
        _fill_items(s_slideeffect, g_playing);
        item = menu_get_item(s_slideeffect, photo_slide_get_item_index(g_playing->fusion_mode));
        assert(NULL != item);
        item->sel_icon_id = (int)g_playing->resg_opt_slideeffect.pic_current->pic_id;
        item->unsel_icon_id = (int)g_playing->resg_opt_slideeffect.pic_current->pic_id;
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

bool photo_option_slideeffect_enter(menuview_t *mv, fusion_enter_e fenter)
{
    _slideeffect_init();
    menuview_set_menu(mv, s_slideeffect);

    if(mv->type != MENUVIEW_OPTION)
    {
        menuview_load_style(mv, MENUVIEW_OPTION);
    }
    menuview_paint(mv);
    return true;
}

bool photo_option_slideeffect_exit(menuview_t *mv)
{
    _slideeffect_deinit();
    photo_option_enter(mv, FUSION_BACKWARD);
    return true;
}

