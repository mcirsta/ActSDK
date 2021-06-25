/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_scene_list.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *       carrie     2009-4-24          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_scene_list.c
 * \brief    listmenu scene
 * \author   carrie
 *
 * \version 1.0
 * \date  2009/04/24
 *******************************************************************************/

#include "photo_scene_list.h"
#include "photo_list_widget_menu.h"
#include "photo_list_mainmenu.h"
#include "photo_list_mem.h"
#include "photo_playlist.h"

/* global variable define */
scene_list_t* g_scene_list = NULL;

/* internal static callback function declaration */
static void _headerbar_cb(headbar_t *headbar, widget_sig_headbar_e signal);
static void _volumebar_cb(volumebar_t *volumebar, volumebar_sig_e signal);
static void _menuview_cb(menuview_t *mv, menuview_sig_e signal);
static void _msgbox_cb(msgbox_t * msgbox, msgbox_sig_e signal);
static void _dialog_cb(dialog_t *dlg, widget_sig_dialog_e signal);
static void _sty_res_init(scene_list_t *sclist);

static void _load_config(scene_list_t *scene_list);
static void _save_config(scene_list_t *scene_list);
static bool _open_theme(scene_list_t *scene_list);
static bool _close_theme(scene_list_t *scene_list);
static bool _open_str_res(void);
static bool _close_str_res(void);

static int _list_init(fusion_enter_e fenter);
static int _list_deinit(void);
static int _list_proc_gui(scene_list_t *scene_list, gui_wm_message_t *pmsg);
static int _sys_cb(msg_apps_t *msg);
static void _list_cb(gui_wm_message_t *pmsg);

static void _headerbar_cb(headbar_t *headbar, widget_sig_headbar_e signal)
{

}

static void _volumebar_cb(volumebar_t *volumebar, volumebar_sig_e signal)
{
    switch (signal)
    {
    case WIDGET_SIG_VOLUMEBAR_CHANGED:
        print_dbg("SIG_VOL_CHANGED: %s---%d---\n", __FILE__, __LINE__);
        break;

    case WIDGET_SIG_VOLUMEBAR_QUIT:
        print_dbg("SIG_VOL_DONE: %s---%d---\n", __FILE__, __LINE__);
        volumebar_delete(volumebar);
        g_scene_list->vbar = NULL;
        break;

    default:
        break;
    }
}

static void _menuview_cb(menuview_t *mv, menuview_sig_e signal)
{
    switch (signal)
    {
    case WIDGET_SIG_MENUVIEW_OK:
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        exit_msg_loop();
        break;

    default:
        break;
    }
}

static void _msgbox_cb(msgbox_t * msgbox, msgbox_sig_e signal)
{
    switch (signal)
    {
    case WIDGET_SIG_MSGBOX_SHOW:
        break;
    case WIDGET_SIG_MSGBOX_QUIT:
        break;
    default:
        break;
    }
}

static void _dialog_cb(dialog_t *dlg, widget_sig_dialog_e signal)
{
    switch (signal)
    {
    case WIDGET_SIG_DIALOG_OK:
    case WIDGET_SIG_DIALOG_CANCAL:
        dialog_delete(dlg);
        g_scene_list->dialog = NULL;
    default:
        break;
    }
}

static bool _show_dialog(scene_list_t *slist, int type, string_resource_t *str_res, dialog_cb_t callback)
{
    dialog_init_t init;
    dialog_cb_t cb;

    if(NULL == callback)
    {
        cb = _dialog_cb;
    }
    else
    {
        cb = callback;
    }

    if(NULL != slist->dialog)
    {
        dialog_delete(slist->dialog);
        slist->dialog = NULL;
    }

    init.head = type;
    init.str_id = str_res->str_id;
    slist->dialog = dialog_create(&init, cb);
    if(NULL == slist->dialog)
    {
        print_err("dialog create error!");
        return false;
    }
    dialog_paint(slist->dialog);
    gui_screen_update();

    return true;
}

static void _sty_res_init(scene_list_t *sclist)
{
    resgroup_resource_t *resg_tmp;

    sclist->pic_bg = (picture_resource_t *) get_scene_child(g_scene_list->scene, PIC_BG);

    /*load mainmenu str res*/
    resg_tmp = (resgroup_resource_t *) get_scene_child(sclist->scene, MAINMENU_RESGROUP);
    sclist->resg_mainmenu.str_nowplaying = (string_resource_t *) get_resgroup_child(resg_tmp, STR_NOWPLAYING);
    sclist->resg_mainmenu.str_dirlist = (string_resource_t *) get_resgroup_child(resg_tmp, STR_DIRLIST);
    sclist->resg_mainmenu.str_previewmode = (string_resource_t *) get_resgroup_child(resg_tmp, STR_PREVIEWMODE);
    sclist->resg_mainmenu.str_slideinterval = (string_resource_t *) get_resgroup_child(resg_tmp, STR_SLIDEINTERVAL);
    sclist->resg_mainmenu.str_slideeffect = (string_resource_t *) get_resgroup_child(resg_tmp, STR_SLIDEEFFECT);
    sclist->resg_mainmenu.str_nodisk = (string_resource_t *) get_resgroup_child(resg_tmp, STR_NODISK);
    sclist->resg_mainmenu.str_plist_error = (string_resource_t *) get_resgroup_child(resg_tmp, STR_PLIST_ERROR);

    /*load preview mode str res*/
    resg_tmp = (resgroup_resource_t *) get_scene_child(g_scene_list->scene, PREVIEWMODE_RESGROUP);
    sclist->resg_previewmode.pic_current = (picture_resource_t *) get_resgroup_child(resg_tmp, PIC_CURRENT);
    sclist->resg_previewmode.str_listmode = (string_resource_t *) get_resgroup_child(resg_tmp, STR_LISTMODE);
    sclist->resg_previewmode.str_thumbnail = (string_resource_t *) get_resgroup_child(resg_tmp, STR_THUMBNAIL);

    /*load slide interval str res*/
    resg_tmp = (resgroup_resource_t *) get_scene_child(g_scene_list->scene, SLIDEINTERVAL_RESGROUP);
    sclist->resg_slideinterval.str_sec = (string_resource_t *) get_resgroup_child(resg_tmp, STR_SEC);

    /*load slide effect str res*/
    resg_tmp = (resgroup_resource_t *) get_scene_child(g_scene_list->scene, SLIDEEFFECT_RESGROUP);
    sclist->resg_slideeffect.pic_current = (picture_resource_t *) get_resgroup_child(resg_tmp, PIC_CURRENT);
    sclist->resg_slideeffect.str_random = (string_resource_t *) get_resgroup_child(resg_tmp, STR_RANDOM);
    sclist->resg_slideeffect.str_flyin = (string_resource_t *) get_resgroup_child(resg_tmp, STR_FLYIN);
    sclist->resg_slideeffect.str_check = (string_resource_t *) get_resgroup_child(resg_tmp, STR_CHESSBOARD);
    sclist->resg_slideeffect.str_cut = (string_resource_t *) get_resgroup_child(resg_tmp, STR_CUT);
    sclist->resg_slideeffect.str_erase = (string_resource_t *) get_resgroup_child(resg_tmp, STR_ERASE);
    sclist->resg_slideeffect.str_louver = (string_resource_t *) get_resgroup_child(resg_tmp, STR_LOUVER);
    sclist->resg_slideeffect.str_randomline = (string_resource_t *) get_resgroup_child(resg_tmp, STR_RANDOMLINE);
    sclist->resg_slideeffect.str_waterwave = (string_resource_t *) get_resgroup_child(resg_tmp, STR_WATERWAVE);
    sclist->resg_slideeffect.str_cube = (string_resource_t *) get_resgroup_child(resg_tmp, STR_CUBE);
    sclist->resg_slideeffect.str_3drotation = (string_resource_t *) get_resgroup_child(resg_tmp, STR_3DROTATION);
    sclist->resg_slideeffect.str_off = (string_resource_t *) get_resgroup_child(resg_tmp, STR_OFF);

    /* headbar pic */
    resg_tmp = (resgroup_resource_t *) get_scene_child(g_scene_list->scene, WIDGET_HEADBAR_RESGROUP);
    sclist->pic_logo = (picture_resource_t *) get_resgroup_child(resg_tmp, PIC_LOGO);
    sclist->str_title = (string_resource_t *) get_resgroup_child(resg_tmp, STR_TITLE);

    /* dirlist */
    resg_tmp = (resgroup_resource_t *) get_scene_child(g_scene_list->scene, DIRLIST_RESGROUP);
    sclist->resg_dirlist.str_int_mem = (string_resource_t *) get_resgroup_child(resg_tmp, STR_INT_MEM);
    sclist->resg_dirlist.str_car_mem = (string_resource_t *) get_resgroup_child(resg_tmp, STR_CAR_MEM);
    sclist->resg_dirlist.str_ext_mem = (string_resource_t *) get_resgroup_child(resg_tmp, STR_EXT_MEM);
}

void scene_list_paint(scene_list_t *scene_list)
{
    /*获取dc*/
    int hdc = gui_dc_get((unsigned short) scene_list->win_id);

    sty_draw_bitmap(hdc, scene_list->bg_bitmap_info, 0, 0);
    if (NULL != scene_list->hbar)
    {
        headbar_paint(scene_list->hbar);
    }
}

static void _load_config(scene_list_t *scene_list)
{
    if (get_config(CFG_PHOTO_PREVIEWMODE, (char *) &scene_list->preview_mode, sizeof(int)) < 0)
    {
        scene_list->preview_mode = PREV_LIST;
    }

    if (get_config(CFG_PHOTO_SLIDE_INTERVAL, (char *) &scene_list->slide_interval, sizeof(int)) < 0)
    {
        scene_list->slide_interval = 3;
    }

    if (get_config(CFG_PHOTO_SLIDE_EFFECT_MODE, (char *) &scene_list->slide_effect_index, sizeof(int)) < 0)
    {
        scene_list->slide_effect_index = ITEM_OFF;
    }

    if((scene_list->slide_effect_index >= ITEM_MAX) || (scene_list->slide_effect_index < 0))
    {
        scene_list->slide_effect_index = ITEM_OFF;
    }

    if (get_config(CFG_PHOTO_NOWPLAYING, (char *) &scene_list->nowplaying_flag, sizeof(bool)) < 0)
    {
        scene_list->nowplaying_flag = false;
    }
}

static void _save_config(scene_list_t *scene_list)
{
    set_config(CFG_PHOTO_PREVIEWMODE, (const char *) &scene_list->preview_mode, sizeof(int));
    set_config(CFG_PHOTO_SLIDE_INTERVAL, (const char *) &scene_list->slide_interval, sizeof(int));
    set_config(CFG_PHOTO_SLIDE_EFFECT_MODE, (const char *) &scene_list->slide_effect_index, sizeof(int));
    set_config(CFG_PHOTO_NOWPLAYING, (const char *) &scene_list->nowplaying_flag, sizeof(bool));
}

static bool _open_theme(scene_list_t *scene_list)
{
    rotation_e angle;
    char comm_file[MAX_PATH_LENGTH], app_file[MAX_PATH_LENGTH];

    /* 取视图角度*/
    angle = sty_get_view_rotation();

#if(TEST_ROTATE)
    angle = sty_get_screen_rotation();
#endif
    /* 设置屏幕方向*/
    sty_set_screen_rotation(angle);

    /* 载入style 场景*/
    if ((angle == ROTATION_0) || (angle == ROTATION_180))
    {
        scene_list->common_scene = photo_sty_load_scene(STY_COMMON, COMMONUI_SIDELONG_SCENE);
        scene_list->scene = photo_sty_load_scene(STY_PRIVATE, LIST_SIDELONG_SCENE);
    }
    else
    {
        scene_list->common_scene = photo_sty_load_scene(STY_COMMON, COMMONUI_UPRIGHT_SCENE);
        scene_list->scene = photo_sty_load_scene(STY_PRIVATE, LIST_UPRIGHT_SCENE);
    }

    if (scene_list->common_scene == NULL)
    {
        print_err("load commonui scene failed");
        return false;
    }
    else
    {
        /* 设置 commonui 的 style 场景*/
        sty_set_common_scene(scene_list->common_scene);
    }

    if (scene_list->scene == NULL)
    {
        print_err("load app scene failed");
        return false;
    }
    else
    {
        /* 设置应用的 style 场景*/
        sty_set_app_scene(scene_list->scene);
    }

    /*load sty resource*/
    _sty_res_init(scene_list);

    /* 获取commonui 的图片资源文件名*/
    sprintf(comm_file, "%s/%s.res", sys_get_common_theme_res_dir(), "commonui");

    /* 打开 commonui 图片资源文件*/
    if (sty_open_common_pic_res(comm_file) == false)
    {
        print_err("open %s failed!", comm_file);
        return false;
    }

    /* 获取photo 的图片资源文件名*/
    sprintf(app_file, "%s/%s.res", sys_get_app_theme_res_dir(), "photo");

    /* 打开应用的图片资源文件*/
    if (sty_open_app_pic_res(app_file) == false)
    {
        print_err("open %s failed!", app_file);
        return false;
    }

    return true;
}

static bool _close_theme(scene_list_t *scene_list)
{

    free_scene(scene_list->common_scene);
    scene_list->common_scene = NULL;

    free_scene(scene_list->scene);
    scene_list->scene = NULL;

    if (sty_close_common_pic_res() == false)
    {
        print_warning("close common picture resource error!");
        return false;
    }

    if (sty_close_app_pic_res() == false)
    {
        print_warning("close app picture resource error!");
        return false;
    }

    return true;
}

static bool _open_str_res(void)
{
    char file[MAX_PATH_LENGTH];

    /* 获取commonui 字符串资源文件名*/
    sprintf(file, "%s/%s_%s.str", sys_get_common_string_res_dir(), "commonui", sys_get_cur_language_codes());

    /* 打开 commonui 字符串资源文件*/
    if (sty_open_common_str_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }

    /* 获取photo应用字符串资源文件名*/
    sprintf(file, "%s/%s_%s.str", sys_get_app_string_res_dir(), "photo", sys_get_cur_language_codes());

    /* 打开应用的字符串资源文件*/
    if (sty_open_app_str_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }

    /* 设置语言编码*/
    sty_set_text_encoding(GUI_UTF8_ENC);
    gui_set_language_mode(sys_get_cur_language_id());
    return true;
}

static bool _close_str_res(void)
{
    if (sty_close_common_str_res() == false)
    {
        print_warning("close common string resource error!");
        return false;
    }

    if (sty_close_app_str_res() == false)
    {
        print_warning("close app string resource error!");
        return false;
    }

    return true;
}

static bool _list_set_default_font(scene_list_t *scene_list)
{
    if (scene_list->fontdes > 0)
    {
        return false;
    }
    scene_list->fontdes = gui_create_font(sys_get_default_font_file(), 16);
    if (scene_list->fontdes < 0)
    {
        print_err("gui_create_font fail");
        return false;
    }

    if (gui_dc_set_default_fontface(scene_list->fontdes) < 0)
    {
        print_err("gui_dc_set_default_fontface fail");
        return false;
    }

    return true;
}

static bool _list_unset_default_font(scene_list_t *scene_list)
{
    if (scene_list->fontdes <= 0)
    {
        return false;
    }
    gui_destroy_font(scene_list->fontdes);
    scene_list->fontdes = 0;
    return true;
}

/* scene initial function */
static int _list_init(fusion_enter_e fenter)
{
    headbar_init_t headbar_init;
    bool ret;

    if (NULL == g_scene_list)
    {
        g_scene_list = (scene_list_t *) malloc(sizeof(scene_list_t));
        if (NULL == g_scene_list)
        {
            print_err("allocate failure!\n");
            return -1;
        }
        else
        {
            memset(g_scene_list, 0, sizeof(scene_list_t));
        }
    }

    g_scene_list->exit_type = SCENE_LIST_RETURN;

    if (!_open_theme(g_scene_list))
    {
        print_warning();
        return -1;
    }

    if (!_open_str_res())
    {
        print_warning();
        return -1;
    }

    /* 创建字体 */
    if (_list_set_default_font(g_scene_list) == false)
    {
        print_err("create_font failed");
        return -1;
    }

    _load_config(g_scene_list);

    if(sys_get_main_disk_dir(DIR_PHOTO) == NULL)
    {
        _show_dialog(g_scene_list, DIALOG_HEAD_WARN, g_scene_list->resg_mainmenu.str_nodisk, _dialog_cb);
        sleep(1);
        return -1;
    }

    /*创建窗口*/
    g_scene_list->win_id = gui_wm_create_window(g_scene_list->scene->x, g_scene_list->scene->y,
            g_scene_list->scene->width, g_scene_list->scene->height, WM_CREATE_SHOW, _list_cb, 0);
    if (g_scene_list->win_id == 0)
    {
        print_err();
        return -1;
    }

    /*设置焦点*/
    gui_wm_set_focus(g_scene_list->win_id);

    g_scene_list->bg_bitmap_info = get_user_bg_pic(BG_PHOTO);
    if (NULL == g_scene_list->bg_bitmap_info)
    {
        g_scene_list->bg_bitmap_info = sty_create_bitmap((int) g_scene_list->pic_bg->pic_id);
    }
    if (NULL == g_scene_list->bg_bitmap_info)
    {
        print_err();
        return -1;
    }

    /*create widget*/
    headbar_init.type = HEADBAR_TYPE_NORMAL;
    headbar_init.pic_id_head = g_scene_list->pic_logo->pic_id;
    headbar_init.str_id_title = g_scene_list->str_title->str_id;
    g_scene_list->hbar = headbar_create(&headbar_init, _headerbar_cb);
    if (NULL == g_scene_list->hbar)
    {
        print_err();
        return -2;
    }

    g_scene_list->msgbox = msgbox_create(_msgbox_cb);

    g_scene_list->menuview = photo_list_widget_menu_create(_menuview_cb, fenter);
    if (g_scene_list->menuview == NULL)
    {
        print_err();
        return -1;
    }

    gui_screen_update();
    clear_key_msg_queue(-1, -1);
    ret = photo_playlist_init(PHOTO_PLIST_DEFAULT);
    if(!ret)
    {
        _show_dialog(g_scene_list, DIALOG_HEAD_WARN,
                g_scene_list->resg_mainmenu.str_plist_error, _dialog_cb);
        sleep(1);
        return -1;
    }

    if((photo_playlist_get_num() == 0))
    {
        g_scene_list->nowplaying_flag = false;
        photo_list_mainmenu_remove_nowplaying(g_scene_list->menuview);
        menuview_paint(g_scene_list->menuview);
    }

    return 0;
}

/* scene exit function */
static int _list_deinit(void)
{
    if (NULL == g_scene_list)
    {
        return 0;
    }

    _list_unset_default_font(g_scene_list);

    /*删除控件*/
    if (NULL != g_scene_list->menuview)
    {
        menuview_delete(g_scene_list->menuview);
        g_scene_list->menuview = NULL;
    }

    if (NULL != g_scene_list->hbar)
    {
        headbar_delete(g_scene_list->hbar);
        g_scene_list->hbar = NULL;
    }

    if (NULL != g_scene_list->vbar)
    {
        volumebar_delete(g_scene_list->vbar);
        g_scene_list->vbar = NULL;
    }

    if (NULL != g_scene_list->adjust)
    {
        adjust_delete(g_scene_list->adjust);
        g_scene_list->adjust = NULL;
    }

    if (NULL != g_scene_list->msgbox)
    {
        msgbox_delete(g_scene_list->msgbox);
        g_scene_list->msgbox = NULL;
    }

    if (NULL != g_scene_list->dialog)
    {
        dialog_delete(g_scene_list->dialog);
        g_scene_list->dialog = NULL;
    }

    if (NULL != g_scene_list->bg_bitmap_info)
    {
        sty_delete_bitmap(g_scene_list->bg_bitmap_info);
        g_scene_list->bg_bitmap_info = NULL;
    }

    /*删除窗体*/
    if(g_scene_list->win_id > 0)
    {
        gui_wm_delete_window(g_scene_list->win_id);
        g_scene_list->win_id = 0;
    }

    /* 关闭 资源 */
    _close_theme(g_scene_list);
    _close_str_res();

    _save_config(g_scene_list);

    free(g_scene_list);
    g_scene_list = NULL;

    return 0;
}

/* scene msg process */
static int _list_proc_gui(scene_list_t *scene_list, gui_wm_message_t *pmsg)
{
    key_event_t *kmsg;
    int ret = 0;

    switch (pmsg->msgid)
    {
    case WM_MSG_CREATE:
        break;

    case WM_MSG_PAINT:
        break;

    case WM_MSG_KEY:
        kmsg = (key_event_t *) (pmsg->data.p);

        switch (kmsg->val | kmsg->type)
        {
        case KEY_VSUB | KEY_TYPE_DOWN:
        case KEY_VADD | KEY_TYPE_DOWN:
#if(TEST_ROTATE)
            {
                msg_apps_t m;
                m.type = MSG_GSENSOR_ROTATION_CHANGED;
                send_async_msg(APP_NAME_PHOTO, &m);
                break;
            }
#endif
            if (scene_list->vbar == NULL)
            {
                print_dbg();
                ret = 1;
                scene_list->vbar = volumebar_create(0, _volumebar_cb);
                if (scene_list->vbar == NULL)
                {
                    print_err("volumeber create err!");
                    return -1;
                }
                else
                {
                    volumebar_paint(scene_list->vbar);
                }
            }
            break;

        case KEY_OK | KEY_TYPE_DOWN:
            break;

        case KEY_RETURN | KEY_TYPE_DOWN:
            break;

        default:
            break;
        }
        break;

    default:
        gui_wm_default_callback(pmsg);
        break;
    }
    return ret;
}

static bool _list_rotate(scene_list_t *scene_list)
{
    fusion_effect_t eff;

    fusion_get_config(FUSION_TYPE_ROTATE, FUSION_FORWARD, &eff);
    fusion_set_effect(&eff);
    scene_fusion_set_first_frame(scene_list->scene);

    _close_theme(scene_list);
#if (TEST_ROTATE)
    {
        int fd = open(DEVICE_FB, O_RDWR, 0);
        int angle;
        ioctl(fd, FBIOGET_SCREEN_ROTATE, (int)&angle);
        if((angle == ROTATION_0) || (angle == ROTATION_180))
        {
            angle = ROTATION_90;
        }
        else
        {
            angle = ROTATION_0;
        }
        ioctl(fd, FBIOSET_SCREEN_ROTATE, angle);
        close(fd);
    }
#endif
    _open_theme(scene_list);

    gui_wm_set_size(scene_list->win_id, scene_list->scene->width, scene_list->scene->height);

    if (NULL != g_scene_list->bg_bitmap_info)
    {
        sty_delete_bitmap(g_scene_list->bg_bitmap_info);
        g_scene_list->bg_bitmap_info = NULL;
    }
    g_scene_list->bg_bitmap_info = get_user_bg_pic(BG_PHOTO);
    if (NULL == g_scene_list->bg_bitmap_info)
    {
        g_scene_list->bg_bitmap_info = sty_create_bitmap((int) g_scene_list->pic_bg->pic_id);
    }
    if (NULL == g_scene_list->bg_bitmap_info)
    {
        print_err();
        return false;
    }

    scene_list_paint(scene_list);

    if (scene_list->menuview != NULL)
    {
        menuview_load_style(scene_list->menuview, scene_list->menuview->type);
        menuview_paint(scene_list->menuview);
    }

    if (scene_list->hbar != NULL)
    {
        headbar_load_style(scene_list->hbar);
        scene_list->hbar->pic_id_head = scene_list->pic_logo->pic_id;
        headbar_paint(scene_list->hbar);
    }

    if (scene_list->adjust != NULL)
    {
        adjust_load_style(scene_list->adjust);
        adjust_paint(scene_list->adjust);
    }

    if (scene_list->vbar != NULL)
    {
        volumebar_load_style(scene_list->vbar);
        volumebar_paint(scene_list->vbar);
    }

    if (scene_list->msgbox != NULL)
    {
        msgbox_load_style(scene_list->msgbox);
        msgbox_paint(scene_list->msgbox);
    }

    scene_fusion_set_last_frame(scene_list->scene);
    fusion_display(NULL);

    return true;
}

static int _sys_cb(msg_apps_t *msg)
{
    msg_reply_t t;

    print_dbg("msg.type:%d\n", msg->type);
    switch (msg->type)
    {
    case MSG_SHORTCUT_PLAYBACK:
        break;
    case MSG_ACTIVE_SYNC:
        t.type = MSG_REPLY_SUCCESS;
        reply_sync_msg(&t);
        break;

    case MSG_GSENSOR_ROTATION_CHANGED:
        if (NULL != g_scene_list)
        {
            _list_rotate(g_scene_list);
        }
        break;

    case MSG_MAIN_DISK_CHANGED:
    case MSG_APP_QUIT:
        exit_msg_loop();
        if (NULL != g_scene_list)
        {
            g_scene_list->exit_type = SCENE_LIST_EXIT_APP;
        }
        break;

    case MSG_CARD_IN:
    case MSG_CARD_OUT:
    case MSG_USB_HOST_DISK_IN:
    case MSG_USB_HOST_DISK_OUT:
        photo_list_mem_refresh(g_scene_list->menuview);
    default:
        if ((g_scene_list != NULL) && (g_scene_list->msgbox != NULL))
        {
            msgbox_proc_sys(g_scene_list->msgbox, msg);
        }
        break;
    }
    return 0;
}

/* scene callback function */
static void _list_cb(gui_wm_message_t *pmsg)
{

    int ret, top;

    print_dbg();

    ret = _list_proc_gui(g_scene_list, pmsg);
    if (0 != ret)
    {
        return;
    }
    print_dbg();

    top = canvas_get_focus(pmsg);

    /*dispatch msg*/
    if ((g_scene_list->msgbox != NULL) && (top == g_scene_list->msgbox->canvas_id))
    {
        msgbox_proc_gui(g_scene_list->msgbox, pmsg);
    }
    else if ((g_scene_list->vbar != NULL) && (top == g_scene_list->vbar->canvas_id))
    {
        volumebar_proc_gui(g_scene_list->vbar, pmsg);
    }
    else if ((g_scene_list->adjust != NULL) && (top == g_scene_list->adjust->canvas_id))
    {
        adjust_proc_gui(g_scene_list->adjust, pmsg);
    }
    else if ((g_scene_list->menuview != NULL) && (top == g_scene_list->menuview->canvas_id))
    {
        photo_list_widget_menu_proc_gui(g_scene_list->menuview, pmsg);
    }
    else if ((g_scene_list->msgbox != NULL) && (top == g_scene_list->msgbox->canvas_id))
    {
        msgbox_proc_gui(g_scene_list->msgbox, pmsg);
    }
    else
    {
        /*_list_proc_gui(g_scene_list, pmsg);*/
    }
}

/* scene entry function */
int photo_scene_list(fusion_enter_e fenter)
{
    int ret;
    msg_apps_t msg;
    scene_list_ret_e exit_type = SCENE_LIST_RETURN;

    register_sys_dispatcher(_sys_cb);

    /* 场景初始化 */
    ret = _list_init(fenter);
    if (ret < 0)
    {
        _list_deinit();
        print_err("scene init error! \n");
        return exit_type;
    }
    else
    {
        print_dbg();
    }

    /* 场景消息循环 */
    while (true == get_msg(&msg))
    {
        dispatch_msg(&msg);
    }

    unregister_sys_dispatcher();

    exit_type = g_scene_list->exit_type;

    /* 场景退出 */
    ret = _list_deinit();

    return exit_type;
}

