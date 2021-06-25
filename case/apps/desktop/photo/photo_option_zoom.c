/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_option_zoom.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason    2009-06-14         v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_option_zoom.c
 * \brief    zoom of photo playing option
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/06/14
 *******************************************************************************/

#include "photo_scene_playing.h"
#include "photo_option.h"
#include "photo_option_zoom.h"
#include "photo_dec_ctrl.h"
#include "photo_option_widget_menu.h"

static int _proc_item(menuview_t *menuview, menuview_sig_e signal);
static void _fill_items(menu_t *menu, scene_playing_t *p);
static void _zoom_init(void);
static void _zoom_deinit(void);

/* menu list data define */
static menu_t *s_zoom = NULL;

static int _proc_item(menuview_t *menuview, menuview_sig_e signal)
{
    switch (signal)
    {
    case WIDGET_SIG_MENUVIEW_OK:
        /* gif不允许缩放和旋转 */
        if (!strcmp(g_playing->cur_file_info.file_info.file_format, "gif"))
        {
            scene_playing_dialog(g_playing, DIALOG_HEAD_FAIL, STR_ZOOM_NOT_SUPPORT, NULL);
            break;
        }

        switch (menuview->menu->current)
        {
        case OPT_ITEM_ZOOMIN:
            if (g_playing->cur_zoom < MAX_ZOOM)
            {
                photo_dec_control(ZOOMIN, 0);
                g_playing->cur_zoom++;
            }
            break;

        case OPT_ITEM_ZOOMOUT:
            if (g_playing->cur_zoom > 0)
            {
                photo_dec_control(ZOOMOUT, 0);
                g_playing->cur_zoom--;
            }
            break;

        default:
            break;
        }
        scene_playing_paint(g_playing, true);
        menuview_paint(menuview);
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        photo_option_zoom_exit(menuview);
        break;

    default:
        break;
    }
    return 0;
}

static void _fill_items(menu_t *menu, scene_playing_t *p)
{
    menuitem_t item;
    resg_opt_zoom_t *resg = &p->resg_opt_zoom;

    memset(&item, 0, sizeof(menuitem_t));

    /* 填充顺序要跟effect_item_e定义的一致 */
    _add_item(menu, &item, resg->str_zoomin, false, (item_cb_t) _proc_item);
    _add_item(menu, &item, resg->str_zoomout, false, (item_cb_t) _proc_item);
}

static void _zoom_init(void)
{
    g_cur_opt_state = OPT_ZOOM;
    if (NULL == s_zoom)
    {
        s_zoom = photo_menu_create();
        _fill_items(s_zoom, g_playing);
    }
}

static void _zoom_deinit(void)
{
    if (NULL != s_zoom)
    {
        photo_menu_delete(s_zoom);
        s_zoom = NULL;
    }
}

bool photo_option_zoom_enter(menuview_t *mv, fusion_enter_e fenter)
{
    _zoom_init();
    menuview_set_menu(mv, s_zoom);

    if (mv->type != MENUVIEW_OPTION)
    {
        menuview_load_style(mv, MENUVIEW_OPTION);
    }
    menuview_paint(mv);
    return true;
}

bool photo_option_zoom_exit(menuview_t *mv)
{
    _zoom_deinit();
    photo_option_enter(mv, FUSION_BACKWARD);
    return true;
}
