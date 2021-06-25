/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_option.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason    2009-06-14         v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_option.c
 * \brief    photo playing option
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/06/14
 *******************************************************************************/

#include "photo_scene_playing.h"
#include "photo_option.h"
#include "photo_scene_playing.h"
#include "photo_option_rotate.h"
#include "photo_option_setbg.h"
#include "photo_option_slideeffect.h"
#include "photo_option_zoom.h"
#include "photo_option_info.h"
#include "photo_dec_ctrl.h"
#include "photo_option_widget_menu.h"
#include "photo_playlist.h"

static void _proc_item_rotate(menuview_t *mv, menuview_sig_e signal);
static void _proc_item_zoom(menuview_t *mv, menuview_sig_e signal);
static void _proc_item_zoom(menuview_t *mv, menuview_sig_e signal);
static void _adjust_cb(adjust_t *pAdjust, adjust_signal_e signal);
static void _adjust_slideinterval(void);
static void _proc_item_slideinterval(menuview_t *mv, menuview_sig_e signal);
static void _proc_item_slideeffect(menuview_t *mv, menuview_sig_e signal);
static void _proc_item_setbg(menuview_t *mv, menuview_sig_e signal);
static void _delete_file_dialog_cb(dialog_t *dlg, widget_sig_dialog_e signal);
static void _proc_item_delete(menuview_t *mv, menuview_sig_e signal);
static void _proc_item_info(menuview_t *mv, menuview_sig_e signal);


/* menu list data define */
static menu_t *s_option = NULL;

void option_sty_res_init(scene_playing_t *p)
{
    resgroup_resource_t *resg;

    /* load options str res */
    resg = (resgroup_resource_t *) get_scene_child(p->scene, OPTIONS_RESGROUP);
    p->resg_option.str_rotate = (string_resource_t *) get_resgroup_child(resg, STR_ROTATE);
    p->resg_option.str_zoom = (string_resource_t *) get_resgroup_child(resg, STR_ZOOM);
    p->resg_option.str_slideinterval = (string_resource_t *) get_resgroup_child(resg, STR_SLIDEINTERVAL);
    p->resg_option.str_slideeffect = (string_resource_t *) get_resgroup_child(resg, STR_SLIDEEFFECT);
    p->resg_option.str_setbg = (string_resource_t *) get_resgroup_child(resg, STR_SETBG);
    p->resg_option.str_delete = (string_resource_t *) get_resgroup_child(resg, STR_DELETE);
    p->resg_option.str_info = (string_resource_t *) get_resgroup_child(resg, STR_INFO);

    /* load slide interval */
    resg = (resgroup_resource_t *) get_scene_child(p->scene, SLIDEINTERVAL_RESGROUP);
    p->resg_opt_slideinterval.str_sec = (string_resource_t *) get_resgroup_child(resg, STR_SEC);

    /* load zoom str res */
    resg = (resgroup_resource_t *) get_scene_child(p->scene, ZOOM_RESGROUP);
    p->resg_opt_zoom.str_zoomin = (string_resource_t *) get_resgroup_child(resg, STR_ZOOMIN);
    p->resg_opt_zoom.str_zoomout = (string_resource_t *) get_resgroup_child(resg, STR_ZOOMOUT);

    /* load slide effect str res */
    resg = (resgroup_resource_t *) get_scene_child(p->scene, SLIDEEFFECT_RESGROUP);
    p->resg_opt_slideeffect.pic_current = (picture_resource_t *) get_resgroup_child(resg, PIC_CURRENT);
    p->resg_opt_slideeffect.str_random = (string_resource_t *) get_resgroup_child(resg, STR_RANDOM);
    p->resg_opt_slideeffect.str_flyin = (string_resource_t *) get_resgroup_child(resg, STR_FLYIN);
    p->resg_opt_slideeffect.str_check = (string_resource_t *) get_resgroup_child(resg, STR_CHESSBOARD);
    p->resg_opt_slideeffect.str_cut = (string_resource_t *) get_resgroup_child(resg, STR_CUT);
    p->resg_opt_slideeffect.str_erase = (string_resource_t *) get_resgroup_child(resg, STR_ERASE);
    p->resg_opt_slideeffect.str_louver = (string_resource_t *) get_resgroup_child(resg, STR_LOUVER);
    p->resg_opt_slideeffect.str_randomline = (string_resource_t *) get_resgroup_child(resg, STR_RANDOMLINE);
    p->resg_opt_slideeffect.str_waterwave = (string_resource_t *) get_resgroup_child(resg, STR_WATERWAVE);
    p->resg_opt_slideeffect.str_cube = (string_resource_t *) get_resgroup_child(resg, STR_CUBE);
    p->resg_opt_slideeffect.str_3drotation = (string_resource_t *) get_resgroup_child(resg, STR_3DROTATION);
    p->resg_opt_slideeffect.str_off = (string_resource_t *) get_resgroup_child(resg, STR_OFF);

    /* load setbg str res */
    resg = (resgroup_resource_t *) get_scene_child(p->scene, SETBG_RESGROUP);
    p->resg_opt_setbg.str_background = (string_resource_t *) get_resgroup_child(resg, STR_BACKGROUND);
    p->resg_opt_setbg.str_bootlogo = (string_resource_t *) get_resgroup_child(resg, STR_BOOTLOGO);
    p->resg_opt_setbg.str_standbylogo = (string_resource_t *) get_resgroup_child(resg, STR_STANDBYLOGO);
    p->resg_opt_setbg.str_default = (string_resource_t *) get_resgroup_child(resg, STR_DEFAULT);

    /* file info */
    resg = (resgroup_resource_t *) get_scene_child(p->scene, INFO_RESGROUP);
    p->resg_opt_info.str_filename = (string_resource_t *) get_resgroup_child(resg, STR_FILENAME);
    p->resg_opt_info.str_resolution = (string_resource_t *) get_resgroup_child(resg, STR_RESOLUTION);
    p->resg_opt_info.str_filesize = (string_resource_t *) get_resgroup_child(resg, STR_FILESIZE);
    p->resg_opt_info.str_date = (string_resource_t *) get_resgroup_child(resg, STR_DATE);

    p->resg_opt_info.pic_filename = (picture_resource_t *) get_resgroup_child(resg, PIC_FILE_NAME);
    p->resg_opt_info.pic_resolution = (picture_resource_t *) get_resgroup_child(resg, PIC_DIMENSION);
    p->resg_opt_info.pic_filesize = (picture_resource_t *) get_resgroup_child(resg, PIC_FILE_SIZE);
}

static void _proc_item_rotate(menuview_t *mv, menuview_sig_e signal)
{
    switch (signal)
    {
    case WIDGET_SIG_MENUVIEW_OK:
        photo_option_rotate_enter(mv, FUSION_FORWARD);
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        photo_option_exit(mv);
        break;

    default:
        break;
    }
}

static void _proc_item_zoom(menuview_t *mv, menuview_sig_e signal)
{
    switch (signal)
    {
    case WIDGET_SIG_MENUVIEW_OK:
        photo_option_zoom_enter(mv, FUSION_FORWARD);
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        photo_option_exit(mv);
        break;

    default:
        break;
    }
}

static void _adjust_cb(adjust_t *pAdjust, adjust_signal_e signal)
{
    int temp;

    switch (signal)
    {
    case WIDGET_SIG_ADJUST_OK:
        print_dbg("WIDGET_SIG_ADJUST_OK\n");
        adjust_get_current_val(&temp);
        if((g_playing->slide_interval != temp) && (g_playing->slideshow_timer_id >= 0))
        {
            modify_timer(g_playing->slideshow_timer_id, (unsigned int)temp * 1000);
        }
        g_playing->slide_interval = temp;
        adjust_delete(pAdjust);
        g_playing->adjust = NULL;
        break;

    case WIDGET_SIG_ADJUST_CANCEL:
        print_dbg("WIDGET_SIG_ADJUST_CANCEL\n");
        adjust_delete(pAdjust);
        g_playing->adjust = NULL;
        break;
    default:
        break;
    }
}

static void _adjust_slideinterval(void)
{
    adjust_textdigit_init_t adjust_init;

    memset(&adjust_init, 0, sizeof(adjust_textdigit_init_t));
    adjust_init.str_unit_id = (int)g_playing->resg_opt_slideinterval.str_sec->str_id;
    adjust_init.max_val = 10;
    adjust_init.min_val = 1;
    adjust_init.cur_val = g_playing->slide_interval;
    adjust_init.step = 1;
    adjust_init.str_title_id = (int)g_playing->resg_option.str_slideinterval->str_id;

    g_playing->adjust = adjust_create(&adjust_init, ADJ_STYLE_TEXTDIGIT, _adjust_cb);
    if(NULL == g_playing->adjust)
    {
        print_err("adjust create error");
        return;
    }

    adjust_paint(g_playing->adjust);
}

static void _proc_item_slideinterval(menuview_t *mv, menuview_sig_e signal)
{
    switch (signal)
    {
    case WIDGET_SIG_MENUVIEW_OK:
        _adjust_slideinterval();
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        photo_option_exit(mv);
        break;

    default:
        break;
    }
}

static void _proc_item_slideeffect(menuview_t *mv, menuview_sig_e signal)
{
    switch (signal)
    {
    case WIDGET_SIG_MENUVIEW_OK:
        photo_option_slideeffect_enter(mv, FUSION_FORWARD);
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        photo_option_exit(mv);
        break;

    default:
        break;
    }
}

static void _proc_item_setbg(menuview_t *mv, menuview_sig_e signal)
{
    switch (signal)
    {
    case WIDGET_SIG_MENUVIEW_OK:
        photo_option_setbg_enter(mv, FUSION_FORWARD);
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        photo_option_exit(mv);
        break;

    default:
        break;
    }
}

static void _delete_file_dialog_cb(dialog_t *dlg, widget_sig_dialog_e signal)
{
    int num;
    bool running_timer = false;

    if(-1 != g_playing->dec_gif_single_timer_id)
    {
        running_timer = true;
        kill_timer(g_playing->dec_gif_single_timer_id);
        g_playing->dec_gif_single_timer_id = -1;
    }
    if(-1 != g_playing->dec_single_timer_id)
    {
        running_timer = true;
        kill_timer(g_playing->dec_single_timer_id);
        g_playing->dec_single_timer_id = -1;
    }

    if(!strcmp(g_playing->cur_file_info.file_info.file_format, "gif"))
    {
        running_timer = true;
    }

    switch(signal)
    {
    case WIDGET_SIG_DIALOG_OK:

        photo_dec_control(ABORT_DECODE, 0);
        num = photo_playlist_get_num();
        remove(g_playing->cur_file_info.file_path);
        photo_playlist_del_item_path(g_playing->cur_file_info.file_path);
        photo_dirlist_update(g_playing->cur_file_info.file_path, FSEL_DELDATA);
        dialog_delete(dlg);
        g_playing->dialog = NULL;
        num = photo_playlist_get_num();
        if (0 == num)
        {
            print_dbg();
            g_playing->exit_type = SCENE_PLAYING_NO_FILE;
            scene_playing_dialog(g_playing, DIALOG_HEAD_WARN, STR_NO_FILE, NULL);
            print_dbg();
            photo_option_exit(g_playing->option);
            exit_msg_loop();
            break;
        }

        if (g_playing->cur_file_index >= num)
        {
            g_playing->cur_file_index = 0;
        }
        dec_cur_file(g_playing);
        photo_option_exit(g_playing->option);
        break;

    case WIDGET_SIG_DIALOG_CANCAL:
    default:
        dialog_delete(dlg);
        g_playing->dialog = NULL;
        if(true == running_timer)
        {
            dec_cur_file(g_playing);
        }
        break;
    }
}

static void _proc_item_delete(menuview_t *mv, menuview_sig_e signal)
{
    switch (signal)
    {
    case WIDGET_SIG_MENUVIEW_OK:
        scene_playing_delete_file_dialog();
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        photo_option_exit(mv);
        break;

    default:
        break;
    }
}

static void _proc_item_info(menuview_t *mv, menuview_sig_e signal)
{
    switch (signal)
    {
    case WIDGET_SIG_MENUVIEW_OK:
        photo_option_info_enter(mv, FUSION_FORWARD);
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        photo_option_exit(mv);
        break;

    default:
        break;
    }
}

void scene_playing_delete_file_dialog(void)
{
    photo_dec_control(ABORT_DECODE, 0);
    if(-1 != g_playing->dec_gif_single_timer_id)
    {
        stop_timer(g_playing->dec_gif_single_timer_id);
    }
    if(-1 != g_playing->dec_single_timer_id)
    {
        stop_timer(g_playing->dec_single_timer_id);
    }
    scene_playing_dialog(g_playing, DIALOG_HEAD_QUERY, STR_DELETE_FILE, _delete_file_dialog_cb);
}

static void _option_init(void)
{
    menuitem_t item;
    bool support_setbg = false;
    memset(&item, 0, sizeof(menuitem_t));

    g_cur_opt_state = OPT_MAIN;

    get_config(CFG_PHOTO_ENABLE_BACKGROUND, (char *)&support_setbg, sizeof(bool));
    if (NULL == s_option)
    {
        s_option = photo_menu_create();

        _add_item(s_option, &item, g_playing->resg_option.str_rotate, true, (item_cb_t) _proc_item_rotate);
        _add_item(s_option, &item, g_playing->resg_option.str_zoom, true, (item_cb_t) _proc_item_zoom);
        _add_item(s_option, &item, g_playing->resg_option.str_slideinterval, false,
                (item_cb_t) _proc_item_slideinterval);
        _add_item(s_option, &item, g_playing->resg_option.str_slideeffect, true,
                (item_cb_t) _proc_item_slideeffect);
        if(true == support_setbg)
        {
            _add_item(s_option, &item, g_playing->resg_option.str_setbg, true, (item_cb_t) _proc_item_setbg);
        }
        _add_item(s_option, &item, g_playing->resg_option.str_delete, false, (item_cb_t) _proc_item_delete);
        _add_item(s_option, &item, g_playing->resg_option.str_info, true, (item_cb_t) _proc_item_info);
    }
}

static void _option_deinit(void)
{
    if (NULL != s_option)
    {
        photo_menu_delete(s_option);
        s_option = NULL;
    }
}

bool photo_option_enter(menuview_t *mv, fusion_enter_e fenter)
{
    fusion_effect_t eff;
    fusion_get_config(FUSION_TYPE_OPTION_RGB, fenter, &eff);
    fusion_set_effect(&eff);
    option_fusion_set_first_frame(mv);

    _option_init();
    menuview_set_menu(mv, s_option);
    if(mv->type != MENUVIEW_OPTION)
    {
        menuview_load_style(mv, MENUVIEW_OPTION);
    }
    menuview_paint(mv);

    option_fusion_set_last_frame(mv);
    fusion_display(NULL);

    return true;
}

bool photo_option_exit(menuview_t *mv)
{
    if(NULL == mv)
    {
        return false;
    }
    _option_deinit();
    mv->menu = NULL;
    photo_option_widget_menu_exit(mv, NULL);
    return true;
}

