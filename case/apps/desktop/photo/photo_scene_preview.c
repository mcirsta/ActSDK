/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_scene_preview.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      carrie     2009-4-24           v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_scene_preview.c
 * \brief    photo preview scene
 * \author   carrie
 *
 * \version 1.0
 * \date  2009/04/24
 *******************************************************************************/

#include "photo.h"
#include "photo_playlist.h"
#include "photo_scene_preview.h"
#include "photo_preview_widget_nplist.h"
#include "photo_preview_widget_dirlist.h"
#include "photo_preview_widget_thumbnail.h"

/* global variable define */
scene_preview_t* g_preview = NULL;

/* internal static scene function declaration */
static void _headerbar_cb(headbar_t *headbar, widget_sig_headbar_e signal);
static void _volumebar_cb(volumebar_t *volumebar, volumebar_sig_e signal);
static void _msgbox_cb(msgbox_t * msgbox, msgbox_sig_e signal);
static void _widget_nplist_cb(preview_nplist_t *prv, preview_sig_e signal);
static void _widget_dirlist_cb(preview_dirlist_t *prv, preview_sig_e signal);
static void _widget_thumbnail_cb(preview_thumbnail_t *prv, preview_sig_e signal);

static void _preview_paint(scene_preview_t *prv);
static void _load_config(scene_preview_t *prv);
static void _save_config(scene_preview_t *prv);
static bool _open_theme(scene_preview_t *p);
static bool _close_theme(scene_preview_t *p);
static bool _open_str_res(void);
static bool _close_str_res(void);

static int _preview_init(const char *initpath, const char *toppath, fusion_enter_e fenter);
static int _preview_deinit(void);
static int _preview_proc_gui(scene_preview_t *prv, gui_wm_message_t *pmsg);
static void _preview_cb(gui_wm_message_t *pmsg);
static void _sty_res_init(scene_preview_t *prv);
static int _sys_cb(msg_apps_t *msg);

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
        g_preview->vbar = NULL;
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
    default:
        dialog_delete(dlg);
        g_preview->dialog = NULL;
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

static void _widget_nplist_cb(preview_nplist_t *prv, preview_sig_e signal)
{
    switch (signal)
    {
    case WIDGET_SIG_PREVIEW_OK:
        g_preview->exit_type = SCENE_PREVIEW_NEXT;
        exit_msg_loop();
        break;

    case WIDGET_SIG_PREVIEW_QUIT:
        g_preview->exit_type = SCENE_PREVIEW_RETURN;
        exit_msg_loop();
        break;

    default:
        break;
    }
}

static void _widget_dirlist_cb(preview_dirlist_t *prv, preview_sig_e signal)
{
    switch (signal)
    {
    case WIDGET_SIG_PREVIEW_OK:
        print_dbg();
        g_preview->exit_type = SCENE_PREVIEW_NEXT;
        exit_msg_loop();
        break;

    case WIDGET_SIG_PREVIEW_QUIT:
        g_preview->exit_type = SCENE_PREVIEW_RETURN;
        exit_msg_loop();
        break;

    default:
        break;
    }
}

static void _widget_thumbnail_cb(preview_thumbnail_t *prv, preview_sig_e signal)
{
    switch (signal)
    {
    case WIDGET_SIG_PREVIEW_OK:
        g_preview->exit_type = SCENE_PREVIEW_NEXT;
        exit_msg_loop();
        break;

    case WIDGET_SIG_PREVIEW_QUIT:
        g_preview->exit_type = SCENE_PREVIEW_RETURN;
        exit_msg_loop();
        break;

    default:
        break;
    }
}

void photo_preview_show_warning(string_resource_t *str_res)
{
    dialog_init_t dlg_init;

    dlg_init.head = DIALOG_HEAD_WARN;
    dlg_init.str_id = str_res->str_id;
    g_preview->dialog = dialog_create(&dlg_init, _dialog_cb);
    dialog_paint(g_preview->dialog);
    gui_screen_update();
}

static void _preview_paint(scene_preview_t *prv)
{
    int hdc = gui_dc_get((unsigned short) prv->win_id);

    print_dbg();
    sty_draw_bitmap(hdc, prv->bg_bitmap_info, 0, 0);
    if (NULL != prv->pic_bg2)
    {
        sty_draw_picture_res(hdc, prv->pic_bg2);
    }

    if(NULL != prv->hbar)
    {
        headbar_paint(prv->hbar);
    }

    if (NULL != prv->preview_thumbnail)
    {
        photo_preview_widget_thumbnail_paint((preview_thumbnail_t *) prv->preview_thumbnail);
    }

    if (NULL != prv->preview_nplist)
    {
        photo_preview_widget_nplist_paint((preview_nplist_t *) prv->preview_nplist);
    }

    if (NULL != prv->preview_dirlist)
    {
        photo_preview_widget_dirlist_paint((preview_dirlist_t *) prv->preview_dirlist);
    }
    print_dbg();
}

static void _load_config(scene_preview_t *prv)
{
    if (get_config(CFG_PHOTO_PREVIEWMODE, (char *) &prv->preview_mode, sizeof(int)) < 0)
    {
        prv->preview_mode = PREV_LIST;
    }

    if (get_config(CFG_PHOTO_PATH_TYPE, (char *) &prv->path_type, sizeof(int)) < 0)
    {
        prv->path_type = PATH_INTERNAL;
    }
}

static void _save_config(scene_preview_t *prv)
{
    set_config(CFG_PHOTO_PREVIEWMODE, (const char *) &prv->preview_mode, sizeof(int));
}

static bool _open_theme(scene_preview_t *p)
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

    /* 载入 style 场景*/
    if ((angle == ROTATION_0) || (angle == ROTATION_180))
    {
        p->common_scene = photo_sty_load_scene(STY_COMMON, COMMONUI_SIDELONG_SCENE);
        p->scene = photo_sty_load_scene(STY_PRIVATE, PREVIEW_SIDELONG_SCENE);
    }
    else
    {
        p->common_scene = photo_sty_load_scene(STY_COMMON, COMMONUI_UPRIGHT_SCENE);
        p->scene = photo_sty_load_scene(STY_PRIVATE, PREVIEW_UPRIGHT_SCENE);
    }

    if (p->common_scene == NULL)
    {
        print_err("load commonui scene failed");
        return false;
    }
    else
    {
        /* 设置 commonui 的 style 场景*/
        sty_set_common_scene(p->common_scene);
    }

    if (p->scene == NULL)
    {
        print_err("load scene MENU_SCENE failed");
        return false;
    }
    else
    {
        /* 设置应用的 style 场景*/
        sty_set_app_scene(p->scene);
    }

    /*load sty resource*/
    _sty_res_init(p);

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

static bool _close_theme(scene_preview_t *p)
{
    if (NULL != p->common_scene)
    {
        free_scene(p->common_scene);
        p->common_scene = NULL;
    }

    if (NULL != p->scene)
    {
        free_scene(p->scene);
        p->scene = NULL;
    }

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

static bool _preview_set_default_font(scene_preview_t *p)
{
    if (p->fontdes > 0)
    {
        return false;
    }
    p->fontdes = gui_create_font(sys_get_default_font_file(), 16);
    if (p->fontdes < 0)
    {
        print_err("gui_create_font fail");
        return false;
    }

    if (gui_dc_set_default_fontface(p->fontdes) < 0)
    {
        print_err("gui_dc_set_default_fontface fail");
        return false;
    }

    return true;
}

static bool _preview_unset_default_font(scene_preview_t *p)
{
    if (p->fontdes <= 0)
    {
        return false;
    }
    gui_destroy_font(p->fontdes);
    p->fontdes = 0;
    return true;
}

static int _preview_init(const char *initpath, const char *toppath, fusion_enter_e fenter)
{
    headbar_init_t headbar_init;
    char *str_tmp = NULL;
    int ret = 0;
    size_t len;
    fusion_effect_t eff;
    char path[MAX_PATH_LENGTH];
    plist_detail_info_t info;

    if (NULL == g_preview)
    {
        g_preview = (scene_preview_t *) malloc(sizeof(scene_preview_t));
        if (NULL == g_preview)
        {
            print_err("allocate failure!\n");
            return -1;
        }
        else
        {
            memset(g_preview, 0, sizeof(scene_preview_t));
        }
    }

    g_preview->exit_type = SCENE_PREVIEW_RETURN;

    if (!_open_theme(g_preview))
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
    if (_preview_set_default_font(g_preview) == false)
    {
        print_err("create_font failed");
        return -1;
    }

    _load_config(g_preview);
    g_preview->dec_single_timer_id = -1;
    g_preview->show_title_timer_id = -1;

    /* 如果为正在播放，检查播放列表文件所在的目录是否存在 */
    if((PATH_PLAYLIST == g_preview->path_type) && (photo_playlist_get_num() > 0))
    {
        photo_playlist_get_detail_info(&info, 0, 1);
        get_parent_directory(path, MAX_PATH_LENGTH, info.fullpath);
        photo_playlist_free_detail_info(&info, 1);
        if(!path_is_directory(path))
        {
            print_err("parent dir not exist!!:%s\n", path);
            photo_playlist_del_items(0, -1);
            photo_preview_show_warning(g_preview->str_no_file);
            mdelay(300);
            return -1;
        }
    }

    /* 设置初始路径 */
    if (NULL == initpath)
    {
        switch (g_preview->path_type)
        {
        case PATH_CARD:
            g_preview->init_sel_path = get_card_top_path();
            break;
        case PATH_EXTERNAL:
            g_preview->init_sel_path = get_external_top_path();
            break;
        case PATH_PLAYLIST:
            g_preview->init_sel_path = NULL;
            break;
        case PATH_INTERNAL:
        default:
            g_preview->init_sel_path = (char *)sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_PHOTO);
            break;
        }
    }
    else
    {
        g_preview->init_sel_path = (char *) initpath;
    }

    /* 设置顶层目录 */
    if (NULL == toppath)
    {
        str_tmp = get_top_path(g_preview->init_sel_path);
    }
    else
    {
        str_tmp = (char *) toppath;
    }

    if (NULL != str_tmp)
    {
        len = strlen(str_tmp);
        g_preview->top_dir = (char *) malloc(len + 1);
        if (NULL == g_preview->top_dir)
        {
            print_err("malloc fail");
            return -1;
        }
        if (!path_is_directory(str_tmp))
        {
            get_parent_directory(g_preview->top_dir, len, str_tmp);
        }
        else
        {
            strcpy(g_preview->top_dir, str_tmp);
        }
    }
    else
    {
        g_preview->top_dir = strdup(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_PHOTO));
    }

    fusion_get_config(FUSION_TYPE_COMMON, fenter, &eff);
    fusion_set_effect(&eff);
    scene_fusion_set_first_frame(g_preview->scene);

    /*创建窗口*/
    g_preview->win_id = gui_wm_create_window(g_preview->scene->x, g_preview->scene->y, g_preview->scene->width,
            g_preview->scene->height, WM_CREATE_SHOW, _preview_cb, 0);
    if (g_preview->win_id == 0)
    {
        print_err();
        ret = -1;
        goto end_init;
    }

    /*设置焦点*/
    gui_wm_set_focus(g_preview->win_id);

    g_preview->bg_bitmap_info = get_user_bg_pic(BG_PHOTO);
    if (NULL == g_preview->bg_bitmap_info)
    {
        g_preview->bg_bitmap_info = sty_create_bitmap((int) g_preview->pic_bg->pic_id);
    }
    if (NULL == g_preview->bg_bitmap_info)
    {
        print_err();
        return -1;
    }

    /*create widget*/
    headbar_init.type = HEADBAR_TYPE_NORMAL;
    headbar_init.pic_id_head = g_preview->pic_logo->pic_id;
    headbar_init.str_id_title = g_preview->str_title->str_id;
    g_preview->hbar = headbar_create(&headbar_init, _headerbar_cb);
    if (NULL == g_preview->hbar)
    {
        print_err();
        ret = -2;
        goto end_init;
    }

    g_preview->msgbox = msgbox_create(_msgbox_cb);

    if (PREV_LIST == g_preview->preview_mode)
    {
        if (PATH_PLAYLIST == g_preview->path_type)
        {
            g_preview->preview_nplist = photo_preview_widget_nplist_create(_widget_nplist_cb);
        }
        else
        {
            g_preview->preview_dirlist = photo_preview_widget_dirlist_create(_widget_dirlist_cb);
        }
    }
    else if (PREV_THUMBNAIL == g_preview->preview_mode)
    {
        g_preview->preview_thumbnail = photo_preview_widget_thumbnail_create(_widget_thumbnail_cb);
    }
    else
    {

    }

    if ((NULL == g_preview->preview_nplist) && (NULL == g_preview->preview_thumbnail) && (NULL
            == g_preview->preview_dirlist))
    {
        print_err("create preview widget error");
        ret = -2;
        goto end_init;
    }

    /*paint all*/
    _preview_paint(g_preview);
    scene_fusion_set_last_frame(g_preview->scene);

    end_init:
    fusion_display(NULL);
    clear_key_msg_queue(-1, -1);
    return ret;
}

static int _preview_deinit(void)
{
    if (NULL == g_preview)
    {
        return 0;
    }

    _preview_unset_default_font(g_preview);

    /*删除控件*/
    if (NULL != g_preview->hbar)
    {
        headbar_delete(g_preview->hbar);
    }
    g_preview->hbar = NULL;

    if (NULL != g_preview->vbar)
    {
        volumebar_delete(g_preview->vbar);
    }
    g_preview->vbar = NULL;

    if (NULL != g_preview->dialog)
    {
        dialog_delete(g_preview->dialog);
    }
    g_preview->dialog = NULL;

    if (NULL != g_preview->msgbox)
    {
        msgbox_delete(g_preview->msgbox);
    }
    g_preview->msgbox = NULL;

    if (NULL != g_preview->softkey)
    {
        softkeyboard_delete(g_preview->softkey);
    }
    g_preview->softkey = NULL;

    if (NULL != g_preview->loadingbox)
    {
        loadingbox_delete(g_preview->loadingbox);
    }
    g_preview->loadingbox = NULL;

    if (NULL != g_preview->preview_nplist)
    {
        photo_preview_widget_nplist_delete(g_preview->preview_nplist);
    }
    g_preview->preview_nplist = NULL;

    if (NULL != g_preview->preview_dirlist)
    {
        photo_preview_widget_dirlist_delete(g_preview->preview_dirlist);
    }
    g_preview->preview_dirlist = NULL;

    if (NULL != g_preview->preview_thumbnail)
    {
        photo_preview_widget_thumbnail_delete(g_preview->preview_thumbnail);
    }
    g_preview->preview_thumbnail = NULL;

    if (NULL != g_preview->bg_bitmap_info)
    {
        sty_delete_bitmap(g_preview->bg_bitmap_info);
    }
    g_preview->bg_bitmap_info = NULL;

    /*删除窗体*/
    if (0 != g_preview->win_id)
    {
        gui_wm_delete_window(g_preview->win_id);
    }
    g_preview->win_id = 0;

    if (-1 != g_preview->dec_single_timer_id)
    {
        kill_timer(g_preview->dec_single_timer_id);
        g_preview->dec_single_timer_id = -1;
    }

    if (-1 != g_preview->show_title_timer_id)
    {
        kill_timer(g_preview->show_title_timer_id);
        g_preview->show_title_timer_id = -1;
    }

    if (NULL != g_preview->top_dir)
    {
        free(g_preview->top_dir);
        g_preview->top_dir = NULL;
    }

    _close_theme(g_preview);
    _close_str_res();

    _save_config(g_preview);

    free(g_preview);
    g_preview = NULL;

    return 0;
}

static int _preview_proc_gui(scene_preview_t *prv, gui_wm_message_t *pmsg)
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
        case KEY_MENU | KEY_TYPE_LONG:
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
            if (prv->vbar == NULL)
            {
                print_dbg();
                ret = 1;
                prv->vbar = volumebar_create(0, _volumebar_cb);
                if (prv->vbar == NULL)
                {
                    print_err("volumeber create err!");
                    return -1;
                }
                else
                {
                    volumebar_paint(prv->vbar);
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

static void _preview_cb(gui_wm_message_t *pmsg)
{
    int top, ret;

    print_dbg();

    ret = _preview_proc_gui(g_preview, pmsg);
    if (0 != ret)
    {
        return;
    }

    top = canvas_get_focus(pmsg);

    /*dispatch msg*/
    if ((g_preview->msgbox != NULL) && (top == g_preview->msgbox->canvas_id))
    {
        msgbox_proc_gui(g_preview->msgbox, pmsg);
        return;
    }
    if ((g_preview->vbar != NULL) && (top == g_preview->vbar->canvas_id))
    {
        volumebar_proc_gui(g_preview->vbar, pmsg);
        return;
    }

    if ((g_preview->dialog != NULL) && (top == g_preview->dialog->canvas_id))
    {
        dialog_proc_gui(g_preview->dialog, pmsg);
        return;
    }

    if ((g_preview->softkey != NULL) && (top == g_preview->softkey->canvas_id))
    {
        softkeyboard_proc_gui(g_preview->softkey, pmsg);
        return;
    }

    if (NULL != g_preview->preview_thumbnail)
    {
        photo_preview_widget_thumbnail_proc_gui(g_preview->preview_thumbnail, pmsg);
        return;
    }

    if (NULL != g_preview->preview_nplist)
    {
        photo_preview_widget_nplist_proc_gui(g_preview->preview_nplist, pmsg);
        return;
    }

    if (NULL != g_preview->preview_dirlist)
    {
        photo_preview_widget_dirlist_proc_gui(g_preview->preview_dirlist, pmsg);
        return;
    }

    print_dbg();
}

static void _sty_res_init(scene_preview_t *prv)
{
    resgroup_resource_t * resg;

    prv->pic_bg = (picture_resource_t *) get_scene_child(prv->scene, PIC_BG);
    prv->pic_bg2 = (picture_resource_t *) get_scene_child(prv->scene, PIC_BG2);
    prv->picreg_sel00 = (picregion_resource_t *) get_scene_child(prv->scene, PICREG_SEL00);
    prv->picreg_sel01 = (picregion_resource_t *) get_scene_child(prv->scene, PICREG_SEL01);
    prv->picreg_sel10 = (picregion_resource_t *) get_scene_child(prv->scene, PICREG_SEL10);
    prv->resg_pic = (resgroup_resource_t *) get_scene_child(prv->scene, RESG_PIC);
    prv->picreg_err = (picregion_resource_t *) get_scene_child(prv->scene, PICREG_ERR);
    prv->picreg_folder = (picregion_resource_t *) get_scene_child(prv->scene, PICREG_FOLDER);

    /* no file dialog string */
    resg = (resgroup_resource_t *) get_scene_child(prv->scene, WIDGET_DIALOG_RESGROUP);
    prv->str_no_file = (string_resource_t *) get_resgroup_child(resg, STR_NO_FILE);
    prv->str_plist_error = (string_resource_t *) get_resgroup_child(resg, STR_PLIST_ERROR);
    prv->str_not_found = (string_resource_t *) get_resgroup_child(resg, STR_NOT_FOUND);

    /* headbar pic */
    resg = (resgroup_resource_t *) get_scene_child(prv->scene, WIDGET_HEADBAR_RESGROUP);
    prv->pic_logo = (picture_resource_t *) get_resgroup_child(resg, PIC_LOGO);
    prv->str_title = (string_resource_t *) get_resgroup_child(resg, STR_TITLE);
}

static bool _preview_rotate(scene_preview_t *prv)
{
    fusion_effect_t eff;

    fusion_get_config(FUSION_TYPE_ROTATE, FUSION_FORWARD, &eff);
    fusion_set_effect(&eff);
    scene_fusion_set_first_frame(prv->scene);

    _close_theme(prv);
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
    _open_theme(prv);

    gui_wm_set_size(prv->win_id, prv->scene->width, prv->scene->height);
    if (NULL != g_preview->bg_bitmap_info)
    {
        sty_delete_bitmap(g_preview->bg_bitmap_info);
    }
    g_preview->bg_bitmap_info = get_user_bg_pic(BG_PHOTO);
    if (NULL == g_preview->bg_bitmap_info)
    {
        g_preview->bg_bitmap_info = sty_create_bitmap((int) g_preview->pic_bg->pic_id);
    }
    if (NULL == g_preview->bg_bitmap_info)
    {
        print_err();
        return false;
    }

    if (NULL != g_preview->preview_thumbnail)
    {
        photo_preview_widget_thumbnail_rotate(g_preview->preview_thumbnail);
    }
    else if (NULL != g_preview->preview_nplist)
    {
        photo_preview_widget_nplist_rotate(g_preview->preview_nplist);
    }
    else if (NULL != g_preview->preview_dirlist)
    {
        photo_preview_widget_dirlist_rotate(g_preview->preview_dirlist);
    }
    else
    {
    }

    _preview_paint(prv);

    if (prv->hbar != NULL)
    {
        headbar_load_style(prv->hbar);
        prv->hbar->pic_id_head = prv->pic_logo->pic_id;
        headbar_paint(prv->hbar);
    }

    if (prv->dialog != NULL)
    {
        dialog_load_style(prv->dialog);
        dialog_paint(prv->dialog);
    }

    if (prv->vbar != NULL)
    {
        volumebar_load_style(prv->vbar);
        volumebar_paint(prv->vbar);
    }

    if (prv->msgbox != NULL)
    {
        msgbox_load_style(prv->msgbox);
        msgbox_paint(prv->msgbox);
    }

    if (prv->softkey != NULL)
    {
        softkeyboard_load_style(prv->softkey);
        softkeyboard_paint(prv->softkey);
    }

    scene_fusion_set_last_frame(prv->scene);
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
        if (NULL != g_preview)
        {
            _preview_rotate(g_preview);
        }
        break;

    case MSG_MAIN_DISK_CHANGED:
    case MSG_APP_QUIT:
        exit_msg_loop();
        if (NULL != g_preview)
        {
            g_preview->exit_type = SCENE_PREVIEW_EXIT_APP;
        }
        break;

    default:
        if (NULL == g_preview)
        {
            break;
        }
        if (NULL != g_preview->msgbox)
        {
            msgbox_proc_sys(g_preview->msgbox, msg);
        }

        /* 正在浏览卡目录 */
        if (((MSG_CARD_OUT == msg->type) && (PATH_CARD == g_preview->path_type))
                || ((MSG_USB_HOST_DISK_OUT == msg->type) && (PATH_EXTERNAL == g_preview->path_type)))
        {
            exit_msg_loop();
            g_preview->exit_type = SCENE_PREVIEW_STORAGE_OUT;
        }
        break;
    }
    return 0;
}

/* scene entry function */
int photo_scene_preview(const char *initpath, const char *toppath, fusion_enter_e fenter)
{
    int ret;
    msg_apps_t msg;
    scene_preview_ret_e exit_type = SCENE_PREVIEW_RETURN;

    register_sys_dispatcher(_sys_cb);

    ret = _preview_init(initpath, toppath, fenter);
    if (ret < 0)
    {
        _preview_deinit();
        unregister_sys_dispatcher();
        print_warning("scene init error! \n");
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

    exit_type = g_preview->exit_type;

    /* 场景退出 */
    ret = _preview_deinit();

    return exit_type;
}

