/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_list_mainmenu.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason     2009-06-14         v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_list_mainmenu.c
 * \brief    mainmenu photo
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/06/14
 *******************************************************************************/

#include "photo.h"
#include "photo_scene_list.h"
#include "photo_list_mainmenu.h"
#include "photo_list_mem.h"
#include "photo_list_previewmode.h"
#include "photo_list_slideeffect.h"
#include "photo_list_widget_menu.h"

static int _proc_item_nowplaying(menuview_t *menuview, menuview_sig_e signal);
static int _proc_item_dirlist(menuview_t *menuview, menuview_sig_e signal);
static int _proc_item_previewmode(menuview_t *menuview, menuview_sig_e signal);
static int _proc_item_slideeffect(menuview_t *menuview, menuview_sig_e signal);
static void _adjust_cb(adjust_t *pAdjust, adjust_signal_e signal);
static void _adjust_slideinterval(void);
static int _proc_item_slideinterval(menuview_t *menuview, menuview_sig_e signal);
static void _fill_items(menu_t *menu, scene_list_t *sclist);
static void _mainmenu_init(void);
static void _mainmenu_deinit(void);

/* menu list data define */
static menu_t *s_mainmenu = NULL;

static int _proc_item_nowplaying(menuview_t *menuview, menuview_sig_e signal)
{
    int type;
    switch (signal)
    {
    case WIDGET_SIG_MENUVIEW_OK:
        type = PATH_PLAYLIST;
        set_config(CFG_PHOTO_PATH_TYPE, (const char *) &type, sizeof(type));
        g_scene_list->exit_type = SCENE_LIST_NOWPLAYING_NEXT;
        photo_list_widget_menu_exit(menuview, photo_list_mainmenu_enter);
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        photo_list_mainmenu_exit(menuview);
        break;

    default:
        break;
    }
    return 0;
}

static int _proc_item_dirlist(menuview_t *menuview, menuview_sig_e signal)
{
    switch (signal)
    {
    case WIDGET_SIG_MENUVIEW_OK:
        photo_list_mem_enter(menuview, FUSION_FORWARD);
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        photo_list_mainmenu_exit(menuview);
        break;

    default:
        break;
    }
    return 0;
}

static int _proc_item_previewmode(menuview_t *menuview, menuview_sig_e signal)
{
    switch (signal)
    {
    case WIDGET_SIG_MENUVIEW_OK:
        photo_list_previewmode_enter(menuview, FUSION_FORWARD);
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        photo_list_mainmenu_exit(menuview);
        break;

    default:
        break;
    }
    return 0;
}

static int _proc_item_slideeffect(menuview_t *menuview, menuview_sig_e signal)
{
    switch (signal)
    {
    case WIDGET_SIG_MENUVIEW_OK:
        photo_list_slideeffect_enter(menuview, FUSION_FORWARD);
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        photo_list_mainmenu_exit(menuview);
        break;

    default:
        break;
    }

    return 0;
}

static void _adjust_cb(adjust_t *pAdjust, adjust_signal_e signal)
{
    switch (signal)
    {
    case WIDGET_SIG_ADJUST_OK:
        print_dbg("WIDGET_SIG_ADJUST_OK\n");
        adjust_get_current_val(&g_scene_list->slide_interval);
        adjust_delete(pAdjust);
        g_scene_list->adjust = NULL;
        break;

    case WIDGET_SIG_ADJUST_CANCEL:
        print_dbg("WIDGET_SIG_ADJUST_CANCEL\n");
        adjust_delete(pAdjust);
        g_scene_list->adjust = NULL;
        break;
    default:
        break;
    }
}

static void _adjust_slideinterval(void)
{
    adjust_textdigit_init_t adjust_init;

    memset(&adjust_init, 0, sizeof(adjust_textdigit_init_t));
    adjust_init.str_unit_id = (int)g_scene_list->resg_slideinterval.str_sec->str_id;
    adjust_init.max_val = 10;
    adjust_init.min_val = 1;
    adjust_init.cur_val = g_scene_list->slide_interval;
    adjust_init.step = 1;
    adjust_init.str_title_id = (int)g_scene_list->resg_mainmenu.str_slideinterval->str_id;

    g_scene_list->adjust = adjust_create(&adjust_init, ADJ_STYLE_TEXTDIGIT, _adjust_cb);
}

static int _proc_item_slideinterval(menuview_t *menuview, menuview_sig_e signal)
{
    switch (signal)
    {
    case WIDGET_SIG_MENUVIEW_OK:
        _adjust_slideinterval();
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        photo_list_mainmenu_exit(menuview);
        break;

    default:
        break;
    }

    return 0;
}

static void _fill_items(menu_t *menu, scene_list_t *sclist)
{
    menuitem_t item;
    resg_mainmenu_t *resg = &sclist->resg_mainmenu;

    memset(&item, 0, sizeof(menuitem_t));
    if (true == sclist->nowplaying_flag)
    {
        _add_item(menu, &item, resg->str_nowplaying, false, (item_cb_t) _proc_item_nowplaying);
    }
    _add_item(menu, &item, resg->str_dirlist, true, (item_cb_t) _proc_item_dirlist);
    _add_item(menu, &item, resg->str_previewmode, true, (item_cb_t) _proc_item_previewmode);
    _add_item(menu, &item, resg->str_slideeffect, true, (item_cb_t) _proc_item_slideeffect);
    _add_item(menu, &item, resg->str_slideinterval, false, (item_cb_t) _proc_item_slideinterval);
}

static void _mainmenu_init(void)
{
    int cur_index = 0;
    if (NULL == s_mainmenu)
    {
        s_mainmenu = photo_menu_create();
    }
    else
    {
        cur_index = menu_get_cur_index(s_mainmenu);
        menu_remove_all_items(s_mainmenu);
    }
    _fill_items(s_mainmenu, g_scene_list);
    menu_set_cur_index(s_mainmenu, cur_index);
}

static void _mainmenu_deinit(void)
{
    if (NULL != s_mainmenu)
    {
        photo_menu_delete(s_mainmenu);
        s_mainmenu = NULL;
    }
}

/*
 * É¾³ýnowplayingÏî
 */
bool photo_list_mainmenu_remove_nowplaying(menuview_t *mv)
{
    menuitem_t *item;

    if(NULL == s_mainmenu)
    {
        return false;
    }

    item = menu_get_item(s_mainmenu, 0);
    assert(NULL != item);

    if(item->callback == (item_cb_t)_proc_item_nowplaying)
    {
        menu_remove_item(s_mainmenu, 0);
    }

    return true;
}

bool photo_list_mainmenu_enter(menuview_t *mv, fusion_enter_e fenter)
{
    fusion_effect_t eff;

    if(fenter == FUSION_FORWARD)
    {
        fusion_get_config(FUSION_TYPE_APP_CHANGE, fenter, &eff);
    }
    else
    {
        fusion_get_config(FUSION_TYPE_COMMON, fenter, &eff);
    }

    fusion_set_effect(&eff);
    scene_fusion_set_first_frame(g_scene_list->scene);

    scene_list_paint(g_scene_list);

    _mainmenu_init();
    menuview_set_menu(mv, s_mainmenu);

    if(mv->type != MENUVIEW_MENULIST_TEXT)
    {
        menuview_load_style(mv, MENUVIEW_MENULIST_TEXT);
    }
    menuview_paint(mv);

    scene_fusion_set_last_frame(g_scene_list->scene);
    fusion_display(NULL);

    return true;
}

bool photo_list_mainmenu_exit(menuview_t *mv)
{
    _mainmenu_deinit();

    photo_list_widget_menu_exit(mv, NULL);
    return true;
}

