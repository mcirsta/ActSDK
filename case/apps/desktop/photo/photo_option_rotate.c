/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_option_rotate.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason    2009-06-14         v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_option_rotate.c
 * \brief    rotation of photo playing option
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/06/14
 *******************************************************************************/

#include "photo_scene_playing.h"
#include "photo_option.h"
#include "photo_option_rotate.h"
#include "photo_dec_ctrl.h"
#include "photo_option_widget_menu.h"
#include "photo_playing_ui.h"

static int _proc_item(menuview_t *menuview, menuview_sig_e signal);
static void _fill_items(menu_t *menu, scene_playing_t *p);
static void _rotate_init(void);
static void _rotate_deinit(void);

/* menu list data define */
static menu_t *s_rotate = NULL;
static char s_str_rotate[OPT_ITEM_ROTATE_MAX][10];


static int _proc_item(menuview_t *menuview, menuview_sig_e signal)
{
    int rotate = ROTATERIGHT90;

    switch (signal)
    {
    case WIDGET_SIG_MENUVIEW_OK:
        /* gif不允许缩放和旋转 */
        if(!strcmp(g_playing->cur_file_info.file_info.file_format, "gif"))
        {
            scene_playing_dialog(g_playing, DIALOG_HEAD_FAIL, STR_ROTATE_NOT_SUPPORT, NULL);
            break;
        }

        switch (menuview->menu->current)
        {
        case OPT_ITEM_ROTATE90:
            rotate = ROTATERIGHT90;
            g_playing->cur_rotate += 90;
            break;

        case OPT_ITEM_ROTATE180:
            rotate = ROTATE180;
            g_playing->cur_rotate += 180;
            break;

        case OPT_ITEM_ROTATE270:
            rotate = ROTATELEFT90;
            g_playing->cur_rotate += 270;
            break;

        default:
            break;
        }
        g_playing->user_opration_status = false;
        g_playing->cur_rotate = g_playing->cur_rotate % 360;
        switch(g_playing->cur_rotate)
        {
        case 90:
            g_playing->show_pic_rotate = g_playing->resg_directbar.pic_r90;
            break;
        case 180:
            g_playing->show_pic_rotate = g_playing->resg_directbar.pic_r180;
            break;
        case 270:
            g_playing->show_pic_rotate = g_playing->resg_directbar.pic_r270;
            break;
        default:
            g_playing->show_pic_rotate = NULL;
            break;
        }
        menuview_hide(menuview);
        gui_framebuffer_update();
        photo_dec_control(rotate, 0);
        draw_picture(g_playing, FUSION_ROTATE);
        menuview_show(menuview);
        menuview_paint(menuview);
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        photo_option_rotate_exit(menuview);
        break;

    default:
        break;
    }
    return 0;
}

static void _fill_items(menu_t *menu, scene_playing_t *p)
{
    menuitem_t item;
    int i;

    memset(&item, 0, sizeof(menuitem_t));
    memset(s_str_rotate, 0, sizeof(s_str_rotate));

    for (i = 0; i < OPT_ITEM_ROTATE_MAX; i++)
    {
        sprintf(s_str_rotate[i], "%d", (i + 1) * 90);
        item.callback = (item_cb_t) _proc_item;
        item.text = s_str_rotate[i];
        item.has_child = false;
        menu_append_item(menu, &item);
    }
}

static void _rotate_init(void)
{
    g_cur_opt_state = OPT_ROTATE;
    if (NULL == s_rotate)
    {
        s_rotate = photo_menu_create();
        _fill_items(s_rotate, g_playing);
    }
}

static void _rotate_deinit(void)
{
    if (NULL != s_rotate)
    {
        photo_menu_delete(s_rotate);
        s_rotate = NULL;
    }
}

bool photo_option_rotate_enter(menuview_t *mv, fusion_enter_e fenter)
{
    _rotate_init();
    menuview_set_menu(mv, s_rotate);

    if(mv->type != MENUVIEW_OPTION)
    {
        menuview_load_style(mv, MENUVIEW_OPTION);
    }
    menuview_paint(mv);
    return true;
}

bool photo_option_rotate_exit(menuview_t *mv)
{
    _rotate_deinit();
    photo_option_enter(mv, FUSION_BACKWARD);
    return true;
}
