/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_scene_playing.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      carrie     2009-4-24           v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_scene_playing.c
 * \brief    photo playing scene
 * \author   carrie
 *
 * \version 1.0
 * \date  2009/04/24
 *******************************************************************************/
#include "photo.h"
#include "photo_scene_playing.h"
#include "photo_option.h"
#include "photo_playlist.h"
#include "photo_dec_ctrl.h"
#include "photo_option_widget_menu.h"
#include "photo_playing_ui.h"

/* global variable define */
scene_playing_t* g_playing = NULL;

/* internal static scene function declaration */
static void _headerbar_cb(headbar_t *headbar, widget_sig_headbar_e signal);
static void _volumebar_cb(volumebar_t *volumebar, volumebar_sig_e signal);
static void _menuview_cb(menuview_t *mv, menuview_sig_e signal);
static void _msgbox_cb(msgbox_t * msgbox, msgbox_sig_e signal);

static void _load_config(scene_playing_t *p);
static void _save_config(scene_playing_t *p);

static void _playing_dec_cb(void);

static void _playing_timer(void *param);
static void _operation_timer(void *param);
static bool _open_theme(scene_playing_t *p);
static bool _close_theme(scene_playing_t *p);
static bool _open_str_res(void);
static bool _close_str_res(void);

static int _playing_init(fusion_enter_e fenter);
static int _playing_deinit(void);
static int _playing_proc_gui(scene_playing_t *p, gui_wm_message_t *pmsg);
static int _sys_cb(msg_apps_t *msg);
static void _playing_cb(gui_wm_message_t *pmsg);
static bool _sty_res_init(scene_playing_t *p);
static void _update_fb(void);


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
        g_playing->vbar = NULL;
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
        if (mv->menu == NULL) /* 整个菜单已经退出 */
        {
            photo_option_widget_menu_delete(mv);
            g_playing->option = NULL;
            scene_playing_paint(g_playing, true);
        }
        break;

    default:
        break;
    }
}

static void _msgbox_cb(msgbox_t * msgbox, msgbox_sig_e signal)
{
    switch(signal)
    {
    case WIDGET_SIG_MSGBOX_SHOW:
        break;
    case WIDGET_SIG_MSGBOX_QUIT:
        break;
    default:
        break;
    }
}

static void _loadingbox_cb(loadingbox_t *loadingbox, loadingbox_sig_e signal)
{
    gui_wm_message_t msg;
    key_event_t kevent;

    switch(signal)
    {
    case WIDGET_SIG_LOADINGBOX_OK:
        msg.msgid = WM_MSG_KEY;
        kevent.val = KEY_OK;
        kevent.type = KEY_TYPE_DOWN;
        msg.data.p = &kevent;
        _playing_proc_gui(g_playing, &msg);
        break;
    case WIDGET_SIG_LOADINGBOX_QUIT:
        loadingbox_delete(loadingbox);
        g_playing->loadingbox = NULL;
        g_playing->exit_type = SCENE_PLAYING_RETURN;
        exit_msg_loop();
        break;
    default:
        break;
    }
}

static void _load_config(scene_playing_t *p)
{
    int temp;

    if (get_config(CFG_PHOTO_SLIDE_EFFECT_MODE, (char *) &temp, sizeof(int)) == -1)
    {
        temp = ITEM_OFF; /* no effect */
    }
    p->fusion_mode =photo_slide_get_fusion_mode(temp);

    if (get_config(CFG_PHOTO_SLIDE_INTERVAL, (char *) &p->slide_interval, sizeof(int)) == -1)
    {
        p->slide_interval = 3;
    }

    if (get_config(CFG_PHOTO_CURRENT_INDEX, (char *) &p->cur_file_index, sizeof(int)) == -1)
    {
        p->cur_file_index = 0;
    }

    if (get_config(CFG_PHOTO_PATH_TYPE, (char *) &p->path_type, sizeof(int)) < 0)
    {
        p->path_type = PATH_INTERNAL;
    }
}

static void _save_config(scene_playing_t *p)
{
    bool flag = false;
    int temp = photo_slide_get_item_index(p->fusion_mode);

    set_config(CFG_PHOTO_SLIDE_EFFECT_MODE, (const char *) &temp, sizeof(int));
    set_config(CFG_PHOTO_SLIDE_INTERVAL, (const char *) &p->slide_interval, sizeof(int));
    set_config(CFG_PHOTO_CURRENT_INDEX, (const char *) &p->cur_file_index, sizeof(int));
    if(NULL != p->cur_file_info.file_path)
    {
        set_config(CFG_PHOTO_NOWPLAY_PATH, (const char *) p->cur_file_info.file_path,
                (int)strlen(p->cur_file_info.file_path) + 1);
    }
    else
    {
        const char *path = sys_get_main_disk_dir(DIR_PHOTO);
        if(path != NULL)
        {
            set_config(CFG_PHOTO_NOWPLAY_PATH, path, (int)strlen(path) + 1);
        }
    }

    if(photo_playlist_get_num() > 0)
    {
        flag = true;
    }
    set_config(CFG_PHOTO_NOWPLAYING, (const char *) &flag, sizeof(bool));
}

static void _update_fb(void)
{
    int fd = open(DEVICE_FB, O_RDWR, 0);
    ioctl(fd, FBIOSET_UPDATE_FB, 0);
    close(fd);
}

/*
 * 解码完毕，启动的single shot定时器
 */
static int _single_dec_timer(void *param)
{
    scene_playing_t *p = (scene_playing_t *) param;
    print_dbg();
    kill_timer(g_playing->dec_single_timer_id);
    g_playing->dec_single_timer_id = -1;
    if(NULL != p->loadingbox)
    {
        loadingbox_delete(p->loadingbox);
        p->loadingbox = NULL;
    }
    draw_picture(p, p->fusion_mode);
    
    /* 解码并显示完一幅才启动slideshow timer */
    if((true == g_playing->slideshow) && (g_playing->slideshow_timer_id < 0))
    {
        g_playing->slideshow_timer_id = set_single_shot_timer(
                        (unsigned int)g_playing->slide_interval * 1000,
                        (timer_proc)_playing_timer, g_playing);
    }
    return 0;
}

static int _single_dec_gif_timer(void *param)
{
    scene_playing_t *p = (scene_playing_t *) param;

    print_dbg();
    kill_timer(g_playing->dec_gif_single_timer_id);
    g_playing->dec_gif_single_timer_id = -1;
    if(NULL != p->loadingbox)
    {
        loadingbox_delete(p->loadingbox);
        p->loadingbox = NULL;
    }
    if(0 == p->cur_gif_frames)
    {
        draw_picture(p, p->fusion_mode);
        /* 解码并显示完第一帧才启动slideshow timer */
        if((true == g_playing->slideshow) && (g_playing->slideshow_timer_id < 0))
        {
            g_playing->slideshow_timer_id = set_single_shot_timer(
                            (unsigned int)g_playing->slide_interval * 1000,
                            (timer_proc)_playing_timer, g_playing);
        }
    }
    else
    {
        draw_picture(p, FUSION_NORMAL);
    }

    g_playing->cur_gif_frames ++;
    if(DECODE_ERROR != g_playing->cur_file_info.dec_status)
    {
        dec_gif_next_frame(p);
    }
    return 0;
}

static void _playing_dec_cb(void)
{
    mmm_image_status_t status;

    print_dbg("");

    if (true == photo_dec_control(GET_DECODER_STATUS, (unsigned int) &status))
    {
        if (MMM_IMAGE_DECODER_FREE == status.status) /* 解码完成 */
        {
            print_dbg();
            g_playing->cur_file_info.dec_status = DECODE_FINISHED;
        }
        else if (MMM_IMAGE_DECODER_ERROR == status.status)
        {
            print_dbg();
            g_playing->cur_file_info.dec_status = DECODE_ERROR;
            g_playing->cur_file_info.dec_errno = status.err_no;
        }
        else
        {
        }

        if (MMM_IMAGE_DECODER_BUSY != status.status)
        {
            if (!strcmp(g_playing->cur_file_info.file_info.file_format, "gif"))
            {
                g_playing->dec_gif_single_timer_id = set_single_shot_timer(1,
                                _single_dec_gif_timer, g_playing);
            }
            else
            {
                g_playing->dec_single_timer_id = set_single_shot_timer(1, _single_dec_timer, g_playing);
            }
        }
    }
}

static bool _is_big_pic(mmm_img_file_info_t *info)
{
    if((!strcmp("png", info->file_format)) && ((info->width * info->height) > 1000000))
    {
        return true;
    }
    else if((!strcmp("jpg", info->file_format)) && ((info->width * info->height) > 3000000))
    {
        return true;
    }
    else if((!strcmp("gif", info->file_format)) && ((info->width * info->height) > 250000))
    {
        return true;
    }
    else if((!strcmp("bmp", info->file_format)) && ((info->width * info->height) > 5000000))
    {
        return true;
    }
    else if((!strcmp("tif", info->file_format)) && ((info->width * info->height) > 1000000))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool dec_cur_file(scene_playing_t *p)
{
    unsigned int len;
    plist_detail_info_t info;
    mmm_image_status_t status;

    if(NULL != p->dialog)
    {
        dialog_delete(p->dialog);
        p->dialog = NULL;
    }

    p->cur_rotate = 0;
    p->cur_zoom = 0;
    p->show_pic_rotate = NULL;
    p->cur_gif_frames = 0;

    photo_dec_control(ABORT_DECODE, 0);

    if (p->cur_file_info.file_path != NULL)
    {
        free(p->cur_file_info.file_path);
        p->cur_file_info.file_path = NULL;
    }

    if (NULL != p->cur_file_info.file_name)
    {
        free(p->cur_file_info.file_name);
        p->cur_file_info.file_name = NULL;
    }

    if (!photo_playlist_get_detail_info(&info, p->cur_file_index, 1))
    {
        print_warning("get file path err");
        return false;
    }

    len = strlen(info.fullpath) + 1;
    p->cur_file_info.file_path = (char *) malloc(len);
    if(NULL == p->cur_file_info.file_path)
    {
        print_err("malloc fail");
        return false;
    }
    strncpy(p->cur_file_info.file_path, info.fullpath, len);

    len = strlen(info.filename) + 1;
    p->cur_file_info.file_name = (char *) malloc(len);
    if(NULL == p->cur_file_info.file_name)
    {
        print_err("malloc fail");
        return false;
    }
    strncpy(p->cur_file_info.file_name, info.filename, len);

    photo_playlist_free_detail_info(&info, 1);

    if (!photo_dec_control(SET_FILE, (unsigned int) p->cur_file_info.file_path))
    {
        print_warning("set file err");
        goto err;
    }

    if (!photo_dec_control(GET_MEDIA_INFO, (unsigned int) &p->cur_file_info.file_info))
    {
        print_warning("get media info err");
        goto err;
    }
    else
    {
        print_info("width = %d,height=%d,format=%s\n",
                p->cur_file_info.file_info.width,
                p->cur_file_info.file_info.height,
                p->cur_file_info.file_info.file_format);
    }


    /* 大图片要显示等待信息 */
    if(true == _is_big_pic(&p->cur_file_info.file_info))
    {
        if(NULL == p->loadingbox)
        {
            p->loadingbox = loadingbox_create(LOADINGBOX_WAITING, _loadingbox_cb);
        }

        if (NULL == p->loadingbox)
        {
            print_err("loadingbox_create failed\n\n");
            return false;
        }
        loadingbox_paint(p->loadingbox);
    }

    if (!photo_dec_control(SET_CALLBACK, (unsigned int) _playing_dec_cb))
    {
        print_warning("set callback err");
        goto err;
    }

    if (!photo_dec_control(IMG_DECODE, (unsigned int) &p->cur_file_info.dec_info))
    {
        print_warning("decode image err");
        goto err;
    }

    p->cur_file_info.dec_status = DECODING;
    if(g_playing->slideshow_timer_id >= 0)
    {
        kill_timer(p->slideshow_timer_id);
        p->slideshow_timer_id = -1;
    }
    return true;

    err:
    memset(p->cur_file_info.dec_info.buf, 0, p->cur_file_info.dec_info.len);
    p->cur_file_info.dec_status = DECODE_ERROR;
    photo_dec_control(GET_DECODER_STATUS, (unsigned int) &status);
    g_playing->cur_file_info.dec_errno = status.err_no;
    gui_dc_clear(gui_dc_get((unsigned short)p->win_id));
    g_playing->dec_single_timer_id = set_single_shot_timer(1, _single_dec_timer, g_playing);
    return false;
}

void dec_next_file(scene_playing_t *p)
{
    int num = photo_playlist_get_num();
    if(num == 0)
    {
        return;
    }

    p->cur_file_index++;
    if (p->cur_file_index >= num)
    {
        p->cur_file_index = 0;
    }
    dec_cur_file(p);
}

void dec_prev_file(scene_playing_t *p)
{
    int num = photo_playlist_get_num();
    if(num == 0)
    {
        return;
    }

    p->cur_file_index--;
    if (p->cur_file_index < 0)
    {
        p->cur_file_index = num - 1;
    }
    dec_cur_file(p);
}

bool dec_gif_next_frame(scene_playing_t *p)
{
    if (!photo_dec_control(IMG_DECODE, (unsigned int) &p->cur_file_info.dec_info))
    {
        print_warning("decode image err");
        return false;
    }
    return true;
}

/*
 * slide show timer
 */
static void _playing_timer(void *param)
{
    scene_playing_t *p = (scene_playing_t *) param;
    print_dbg();
    kill_timer(p->slideshow_timer_id);
    p->slideshow_timer_id = -1;
    dec_next_file(p);
}

static void _operation_timer(void *param)
{
    scene_playing_t *p = (scene_playing_t *) param;
    print_dbg();
    p->user_opration_status = false;
    p->show_pic_prev = p->resg_directbar.pic_prev;
    p->show_pic_next = p->resg_directbar.pic_next;
    kill_timer(p->opration_timer_id);
    p->opration_timer_id = -1;
    scene_playing_paint(p, true);
}

void set_operation_timer(scene_playing_t *p)
{
    if(p->opration_timer_id < 0)
    {
        p->opration_timer_id = set_single_shot_timer(3 * 1000, (timer_proc) _operation_timer, p);
    }
    else
    {
        restart_timer(p->opration_timer_id);
    }
}

static bool _open_theme(scene_playing_t *p)
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
        p->scene = photo_sty_load_scene(STY_PRIVATE, PLAYING_SIDELONG_SCENE);
    }
    else
    {
        p->common_scene = photo_sty_load_scene(STY_COMMON, COMMONUI_UPRIGHT_SCENE);
        p->scene = photo_sty_load_scene(STY_PRIVATE, PLAYING_UPRIGHT_SCENE);
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
    if(!_sty_res_init(p))
    {
        print_warning("style init fail");
        return false;
    }

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

static bool _close_theme(scene_playing_t *p)
{

    free_scene(p->common_scene);
    p->common_scene = NULL;

    free_scene(p->scene);
    p->scene = NULL;

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

static bool _playing_set_default_font(scene_playing_t *p)
{
    if(p->fontdes > 0)
    {
        return false;
    }
    p->fontdes = gui_create_font(sys_get_default_font_file(), 16);
    if(p->fontdes < 0)
    {
        print_err("gui_create_font fail");
        return false;
    }

    if(gui_dc_set_default_fontface(p->fontdes) < 0)
    {
        print_err("gui_dc_set_default_fontface fail");
        return false;
    }

    return true;
}

static bool _playing_unset_default_font(scene_playing_t *p)
{
    if(p->fontdes <= 0)
    {
        return false;
    }
    gui_destroy_font(p->fontdes);
    p->fontdes = 0;
    return true;
}

static int _playing_init(fusion_enter_e fenter)
{
    int width, height;
    headbar_init_t headbar_init;
    fusion_effect_t eff;

    if (NULL == g_playing)
    {
        g_playing = (scene_playing_t *) malloc(sizeof(scene_playing_t));
        if (NULL == g_playing)
        {
            print_err("allocate failure!\n");
            return -1;
        }
        else
        {
            memset(g_playing, 0, sizeof(scene_playing_t));
        }
    }

    g_playing->exit_type = SCENE_PLAYING_RETURN;

    if (!_open_theme(g_playing))
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
    if (_playing_set_default_font(g_playing) == false)
    {
        print_err("create_font failed");
        return -1;
    }

    /* 检查是否播放列表初始化错误 */
    if(true == photo_playlist_check_init_error())
    {
        print_err("check playlist init init error");
        scene_playing_dialog(g_playing, DIALOG_HEAD_WARN, STR_PLIST_ERROR, NULL);
        sleep(1);
        return -1;
    }

    if (photo_playlist_get_num() == 0)
    {
        print_warning();
        g_playing->exit_type = SCENE_PLAYING_NO_FILE;
        scene_playing_dialog(g_playing, DIALOG_HEAD_WARN, STR_NO_FILE, NULL);
        return -1;
    }

    _load_config(g_playing);
    g_playing->dec_single_timer_id = -1;
    g_playing->dec_gif_single_timer_id = -1;
    grab_msg(MSG_GSENSOR_SHAKED);

    /*创建窗口*/
    width = g_playing->scene->width;
    height = g_playing->scene->height;
    g_playing->win_id = gui_wm_create_window(g_playing->scene->x, g_playing->scene->y,
                width, height, WM_CREATE_SHOW, _playing_cb, 0);
    if (g_playing->win_id == 0)
    {
        print_err();
        return -1;
    }

    /*设置焦点*/
    gui_wm_set_focus(g_playing->win_id);

    /*create widget*/
    headbar_init.type = HEADBAR_TYPE_YUV;
    headbar_init.pic_id_head = g_playing->pic_logo->pic_id;
    headbar_init.str_id_title = g_playing->str_title->str_id;
    g_playing->hbar = headbar_create(&headbar_init, _headerbar_cb);

    g_playing->msgbox = msgbox_create(_msgbox_cb);

    g_playing->cur_file_info.dec_info.bpp = 2;
    g_playing->cur_file_info.dec_info.formate = RGB_565;
    g_playing->cur_file_info.dec_info.len = (unsigned int)(width * height) * 2;
    g_playing->cur_file_info.dec_info.buf = (unsigned char *) malloc((unsigned int)(width * height) * 2);
    if(NULL == g_playing->cur_file_info.dec_info.buf)
    {
        print_err("malloc fail");
        return false;
    }
    g_playing->cur_file_info.dec_info.rect_width = (unsigned int)width;
    g_playing->cur_file_info.dec_info.rect_height = (unsigned int)height;
    g_playing->cur_file_info.dec_status = NOT_DECODE;
    g_playing->cur_file_info.file_path = NULL;
    g_playing->cur_file_info.file_name = NULL;

    dec_cur_file(g_playing);
    g_playing->slideshow_timer_id = -1;
    g_playing->opration_timer_id = -1;

    g_playing->show_pic_prev = g_playing->resg_directbar.pic_prev;
    g_playing->show_pic_next = g_playing->resg_directbar.pic_next;
    g_playing->show_pic_rotate = NULL;

    g_playing->cur_rotate = 0;

    fusion_get_config(FUSION_TYPE_COMMON, fenter, &eff);
    fusion_set_effect(&eff);
    scene_fusion_set_first_frame(g_playing->scene);
    gui_dc_clear(gui_dc_get((unsigned short)g_playing->win_id));

    /*paint all*/
    scene_playing_paint(g_playing, false);
    scene_fusion_set_last_frame(g_playing->scene);

    fusion_display(NULL);
    clear_key_msg_queue(-1, -1);

    sys_forbid_screensaver(true);
    sys_forbid_auto_standby(true);

    return 0;
}

static int _playing_deinit(void)
{
    photo_dec_control(ABORT_DECODE, 0);

    if (NULL == g_playing)
    {
        return 0;
    }

    release_msg(MSG_GSENSOR_SHAKED);

    sys_forbid_screensaver(false);
    sys_forbid_auto_standby(false);

    _playing_unset_default_font(g_playing);
    _save_config(g_playing);

    /*删除控件*/
    if(NULL != g_playing->option)
    {
        photo_option_widget_menu_delete(g_playing->option);
        g_playing->option = NULL;
    }

    if(NULL != g_playing->dialog)
    {
        dialog_delete(g_playing->dialog);
        g_playing->dialog = NULL;
    }

    if(NULL != g_playing->hbar)
    {
        headbar_delete(g_playing->hbar);
        g_playing->hbar = NULL;
    }

    if(NULL != g_playing->vbar)
    {
        volumebar_delete(g_playing->vbar);
        g_playing->vbar = NULL;
    }

    if(NULL != g_playing->msgbox)
    {
        msgbox_delete(g_playing->msgbox);
        g_playing->msgbox = NULL;
    }

    if(NULL != g_playing->loadingbox)
    {
        loadingbox_delete(g_playing->loadingbox);
        g_playing->loadingbox = NULL;
    }
    if(g_playing->canvas_directbar_id > 0)
    {
        canvas_delete(g_playing->canvas_directbar_id);
        g_playing->canvas_directbar_id = 0;
    }

    if(g_playing->canvas_txtbar_id > 0)
    {
        canvas_delete(g_playing->canvas_txtbar_id);
        g_playing->canvas_txtbar_id = 0;
    }

    /*删除窗体*/
    gui_wm_delete_window(g_playing->win_id);
    g_playing->win_id = 0;

    if(-1 != g_playing->dec_single_timer_id)
    {
        kill_timer(g_playing->dec_single_timer_id);
        g_playing->dec_single_timer_id = -1;
    }

    if(-1 != g_playing->dec_gif_single_timer_id)
    {
        kill_timer(g_playing->dec_gif_single_timer_id);
        g_playing->dec_gif_single_timer_id = -1;
    }

    _close_theme(g_playing);
    _close_str_res();

    if (NULL != g_playing->cur_file_info.dec_info.buf)
    {
        free(g_playing->cur_file_info.dec_info.buf);
        g_playing->cur_file_info.dec_info.buf = NULL;
    }

    if (NULL != g_playing->cur_file_info.file_path)
    {
        free(g_playing->cur_file_info.file_path);
    }

    if (NULL != g_playing->cur_file_info.file_name)
    {
        free(g_playing->cur_file_info.file_name);
    }

    if (-1 != g_playing->slideshow_timer_id)
    {
        kill_timer(g_playing->slideshow_timer_id);
    }

    if (-1 != g_playing->opration_timer_id)
    {
        kill_timer(g_playing->opration_timer_id);
    }

    free(g_playing);
    g_playing = NULL;

    return 0;
}

/*
 * 如果当前正在旋转、缩放，返回正常状态显示
 */
bool scene_playing_normal_display(scene_playing_t *p)
{
    bool change = true;

    if((p->cur_zoom == 0) && (p->cur_rotate == 0))
    {
        change = false;
    }

    if(p->cur_rotate == 90)
    {
        photo_dec_control(ROTATELEFT90, 0);
    }
    else if(p->cur_rotate == 180)
    {
        photo_dec_control(ROTATE180, 0);
    }
    else if(p->cur_rotate == 270)
    {
        photo_dec_control(ROTATERIGHT90, 0);
    }
    else
    {
    }

    while(p->cur_zoom > 0)
    {
        photo_dec_control(ZOOMOUT, 0);
        p->cur_zoom --;
    }

    p->cur_rotate = 0;
    p->show_pic_rotate = NULL;
    p->cur_zoom = 0;

    if(true == change)
    {
        scene_playing_paint(p, true);
    }

    return change;
}

static void _dialog_cb(dialog_t *dlg, widget_sig_dialog_e signal)
{
    switch (signal)
    {
    case WIDGET_SIG_DIALOG_OK:
    case WIDGET_SIG_DIALOG_CANCAL:
        dialog_delete(dlg);
        g_playing->dialog = NULL;
    default:
        break;
    }
}

bool scene_playing_dialog(scene_playing_t *p, int type, unsigned int resid, dialog_cb_t callback)
{
    dialog_init_t init;
    resgroup_resource_t *resg_dialog = NULL;
    string_resource_t *str_res = NULL;
    dialog_cb_t cb;

    if(NULL == callback)
    {
        cb = _dialog_cb;
    }
    else
    {
        cb = callback;
    }

    resg_dialog = (resgroup_resource_t *)get_scene_child(p->scene, WIDGET_DIALOG_RESGROUP);
    assert(resg_dialog != NULL);
    str_res = (string_resource_t *)get_resgroup_child(resg_dialog, resid);
    assert(str_res != NULL);

    if(NULL != p->dialog)
    {
        dialog_delete(p->dialog);
        p->dialog = NULL;
    }

    init.head = type;
    init.str_id = str_res->str_id;
    if(type == DIALOG_HEAD_QUERY)
    {
        init.def_sel = 1;
    }
    else
    {
        init.def_sel = 0;
    }
    p->dialog = dialog_create(&init, cb);
    if(NULL == p->dialog)
    {
        print_err("dialog create error!");
        return false;
    }
    dialog_paint(p->dialog);
    gui_screen_update();

    return true;
}

void scene_playing_paint(scene_playing_t *p, bool update_fb)
{
    int hdc = gui_dc_get((unsigned short)p->win_id);

    if (p->cur_file_info.dec_status != DECODING)
    {
        draw_picture(p, FUSION_NORMAL);
    }
    else
    {
        if(DECODE_ERROR == p->cur_file_info.dec_status)
        {
            gui_dc_clear(hdc);
        }
        draw_common_canvas(p);
    }
    gui_framebuffer_update();
    if(true == update_fb)
    {
        _update_fb();
    }
    print_dbg();
}

static int _playing_proc_gui(scene_playing_t *p, gui_wm_message_t *pmsg)
{
    key_event_t *kmsg;
    int ret = 0;
    static int last_key;

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
        case KEY_DELETE | KEY_TYPE_DOWN:
            scene_playing_delete_file_dialog();
            break;

        case KEY_VSUB | KEY_TYPE_DOWN:
        case KEY_VADD | KEY_TYPE_DOWN:
#if(TEST_ROTATE)
{
   msg_apps_t m;
   m.type = MSG_GSENSOR_ROTATION_CHANGED;
   send_async_msg(APP_NAME_PHOTO, &m);
   return 0;
}
#endif
            if (p->vbar == NULL)
            {
                p->vbar = volumebar_create(0, _volumebar_cb);
                if (p->vbar == NULL)
                {
                    print_err("volumeber create err!");
                    return -1;
                }
                else
                {
                    volumebar_paint(p->vbar);
                }
            }
            break;

        case KEY_NEXT | KEY_TYPE_DOWN:
            if(p->cur_zoom > 0) /* 放大状态时，移动图片 */
            {
                photo_dec_control(MOVEDOWN, 0);
                scene_playing_paint(p, true);
            }
            else
            {
                dec_next_file(p);
            }
            break;

        case KEY_PREV | KEY_TYPE_DOWN:
            if(p->cur_zoom > 0) /* 放大状态时，移动图片 */
            {
                photo_dec_control(MOVEUP, 0);
                scene_playing_paint(p, true);
            }
            else
            {
                dec_prev_file(p);
            }
            break;

        case KEY_OK | KEY_TYPE_DOWN:
            if(p->cur_zoom > 0) /* 放大状态时，移动图片 */
            {
                photo_dec_control(MOVERIGHT, 0);
                scene_playing_paint(p, true);
            }
            else
            {
                p->slideshow = !p->slideshow;
                if(g_playing->slideshow_timer_id >= 0)
                {
                    kill_timer(g_playing->slideshow_timer_id);
                    g_playing->slideshow_timer_id = -1;
                }

                if(true == p->slideshow)
                {
                    g_playing->slideshow_timer_id = set_single_shot_timer(
                            (unsigned int)g_playing->slide_interval * 1000,
                            (timer_proc)_playing_timer, g_playing);
                }
            }
            break;

        case KEY_MENU | KEY_TYPE_DOWN:
            /*
             * 如果当前正在旋转、缩放，恢复正常显示
             * slideshow状态、正在解码时不能弹出option
             */
            if((!scene_playing_normal_display(p)) && (!p->slideshow) 
                    && (p->cur_file_info.dec_status != DECODING))
            {
                p->option = photo_option_widget_menu_create(_menuview_cb, FUSION_FORWARD);
            }
            break;

        case KEY_RETURN | KEY_TYPE_SHORT_UP:
            if(p->cur_zoom > 0) /* 放大状态时，移动图片 */
            {
                photo_dec_control(MOVELEFT, 0);
                scene_playing_paint(p, true);
            }
            else
            {
                if(last_key == (KEY_RETURN | KEY_TYPE_DOWN))
                {
                    p->exit_type = SCENE_PLAYING_RETURN;
                    exit_msg_loop();
                    return 0;
                }
            }
            break;

        default:
            break;
        }
        last_key = (kmsg->val | kmsg->type);
        if ((kmsg->type != KEY_TYPE_SHORT_UP) && (kmsg->val != KEY_MENU)
                && (kmsg->val != KEY_RETURN) && (p->cur_zoom == 0))
        {
            if(kmsg->val == KEY_PREV)   /* 显示上一幅高亮 */
            {
                p->show_pic_prev = p->resg_directbar.pic_prevh;
            }

            if(kmsg->val == KEY_NEXT)   /* 显示下一幅高亮 */
            {
                p->show_pic_next = p->resg_directbar.pic_nexth;
            }
            p->user_opration_status = true;
            scene_playing_paint(p, true);
            set_operation_timer(g_playing);
            clear_key_msg_queue(-1, -1);
        }

        break;

    default:
        gui_wm_default_callback(pmsg);
        break;
    }
    return ret;
}

static void _playing_cb(gui_wm_message_t *pmsg)
{
    int top;

    top = canvas_get_focus(pmsg);

    /*dispatch msg*/
    if ((g_playing->msgbox != NULL) && (top == g_playing->msgbox->canvas_id))
    {
        msgbox_proc_gui(g_playing->msgbox, pmsg);
        return;
    }

    if ((g_playing->adjust != NULL) && (top == g_playing->adjust->canvas_id))
    {
        adjust_proc_gui(g_playing->adjust, pmsg);
        return;
    }

    if ((g_playing->vbar != NULL) && (top == g_playing->vbar->canvas_id))
    {
        volumebar_proc_gui(g_playing->vbar, pmsg);
        return;
    }

    if ((g_playing->option != NULL) && (top == g_playing->option->canvas_id))
    {
        photo_option_widget_menu_proc_gui(g_playing->option, pmsg);
        return;
    }

    if ((g_playing->dialog != NULL) && (top == g_playing->dialog->canvas_id))
    {
        if((pmsg->msgid == WM_MSG_KEY) && (g_playing->option == NULL)
            && (g_playing->dialog->head != DIALOG_HEAD_QUERY))
        {
            key_event_t *kmsg = (key_event_t *) (pmsg->data.p);
            if((kmsg->val == KEY_NEXT) || (kmsg->val == KEY_PREV)
                || (kmsg->val == KEY_RETURN))
            {
                _playing_proc_gui(g_playing, pmsg);
            }
            else if(kmsg->val == KEY_MENU)
            {
                dialog_delete(g_playing->dialog);
                g_playing->dialog = NULL;
                _playing_proc_gui(g_playing, pmsg);
            }
            else
            {
                dialog_proc_gui(g_playing->dialog, pmsg);
            }
        }
        else
        {
            dialog_proc_gui(g_playing->dialog, pmsg);
        }
        return;
    }

    if ((g_playing->loadingbox != NULL) && (top == g_playing->loadingbox->canvas_id))
    {
        if(pmsg->msgid == WM_MSG_KEY)
        {
            key_event_t *kmsg = (key_event_t *) (pmsg->data.p);
            if((kmsg->val == KEY_NEXT) || (kmsg->val == KEY_PREV))
            {
                _playing_proc_gui(g_playing, pmsg);
            }
            else
            {
                loadingbox_proc_gui(g_playing->loadingbox, pmsg);
            }
        }
        else
        {
            loadingbox_proc_gui(g_playing->loadingbox, pmsg);
        }
        return;
    }

    _playing_proc_gui(g_playing, pmsg);
}

static bool _sty_res_init(scene_playing_t *p)
{
    resgroup_resource_t *resg = NULL;
    sty_rect_t rect;

    p->pic_bg = (picture_resource_t *) get_scene_child(p->scene, PIC_BG);

    /* direct bar */
    resg = (resgroup_resource_t *) get_scene_child(p->scene, DIRECT_RESGROUP);
    if(NULL == resg)
    {
        return false;
    }

    p->resg_directbar.resg = resg;
    p->resg_directbar.pic_prev = (picture_resource_t *) get_resgroup_child(resg, PIC_PREV);
    p->resg_directbar.pic_prevh = (picture_resource_t *) get_resgroup_child(resg, PIC_PREVH);
    p->resg_directbar.pic_next = (picture_resource_t *) get_resgroup_child(resg, PIC_NEXT);
    p->resg_directbar.pic_nexth = (picture_resource_t *) get_resgroup_child(resg, PIC_NEXTH);
    p->resg_directbar.pic_r90 = (picture_resource_t *) get_resgroup_child(resg, PIC_R90);
    p->resg_directbar.pic_r180 = (picture_resource_t *) get_resgroup_child(resg, PIC_R180);
    p->resg_directbar.pic_r270 = (picture_resource_t *) get_resgroup_child(resg, PIC_R270);
    p->resg_directbar.pic_r360 = (picture_resource_t *) get_resgroup_child(resg, PIC_R360);

    rect.x = p->resg_directbar.resg->absolute_x;
    rect.y = p->resg_directbar.resg->absolute_y;
    rect.width = p->resg_directbar.resg->width;
    rect.height = p->resg_directbar.resg->height;
    if(p->canvas_directbar_id > 0)
    {
        canvas_set_region(p->canvas_directbar_id, &rect);
    }
    else
    {
        p->canvas_directbar_id = canvas_create(&rect, TRANSPARENT);
    }

    /* text bar */
    resg = (resgroup_resource_t *) get_scene_child(p->scene, TXTBAR_RESGROUP);
    if(NULL == resg)
    {
        return false;
    }
    p->resg_txtbar.resg = resg;
    p->resg_txtbar.pic_tbar = (picture_resource_t *) get_resgroup_child(resg, PIC_TBAR);
    p->resg_txtbar.pic_auto = (picture_resource_t *) get_resgroup_child(resg, PIC_AUTO);
    p->resg_txtbar.str_name = (string_resource_t *) get_resgroup_child(resg, STR_NAME);
    p->resg_txtbar.str_num = (string_resource_t *) get_resgroup_child(resg, STR_NUM);

    rect.x = p->resg_txtbar.resg->absolute_x;
    rect.y = p->resg_txtbar.resg->absolute_y;
    rect.width = p->resg_txtbar.resg->width;
    rect.height = p->resg_txtbar.resg->height;
    if(p->canvas_txtbar_id > 0)
    {
        canvas_set_region(p->canvas_txtbar_id, &rect);
    }
    else
    {
        p->canvas_txtbar_id = canvas_create(&rect, TRANSPARENT);
    }

    /* headbar pic */
    resg = (resgroup_resource_t *) get_scene_child(p->scene, WIDGET_HEADBAR_RESGROUP);
    p->pic_logo = (picture_resource_t *) get_resgroup_child(resg, PIC_LOGO);
    p->str_title = (string_resource_t *) get_resgroup_child(resg, STR_TITLE);

    /* option */
    option_sty_res_init(p);
    return true;
}

static int _get_rotation_angle(int old, int new)
{
    int rotate = 0;
    if(((old == ROTATION_0) && (new == ROTATION_90))
            || ((old == ROTATION_90) && (new == ROTATION_180))
            || ((old == ROTATION_180) && (new == ROTATION_270))
            || ((old == ROTATION_270) && (new == ROTATION_0)))
    {
        rotate = ROTATERIGHT90;
    }
    else if(((old == ROTATION_90) && (new == ROTATION_0))
            || ((old == ROTATION_180) && (new == ROTATION_90))
            || ((old == ROTATION_270) && (new == ROTATION_180))
            || ((old == ROTATION_0) && (new == ROTATION_270)))
    {
        rotate = ROTATELEFT90;
    }
    else if(((old == ROTATION_0) && (new == ROTATION_180))
            || ((old == ROTATION_90) && (new == ROTATION_270))
            || ((old == ROTATION_180) && (new == ROTATION_0))
            || ((old == ROTATION_270) && (new == ROTATION_90)))
    {
        rotate = ROTATE180;
    }
    else
    {
        rotate = 0;
    }

    return rotate;
}

static bool _playing_rotate(scene_playing_t *p)
{
    int old_angle, new_angle;
    int rotate, hdc;
    fusion_effect_t eff;

    old_angle = sty_get_screen_rotation();

    fusion_get_config(FUSION_TYPE_ROTATE, FUSION_FORWARD, &eff);
    fusion_set_effect(&eff);
    scene_fusion_set_first_frame(p->scene);

    _close_theme(p);
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
    _open_theme(p);
    gui_wm_set_size(p->win_id, p->scene->width, p->scene->height);

    new_angle = sty_get_screen_rotation();
    rotate = _get_rotation_angle(old_angle, new_angle);
    if(0 != rotate)
    {
        /*photo_dec_control(rotate, 0);*/
    }
    p->cur_file_info.dec_info.rect_width = (unsigned int)p->scene->width;
    p->cur_file_info.dec_info.rect_height = (unsigned int)p->scene->height;
    hdc = gui_dc_get((unsigned short)p->win_id);
    gui_dc_clear(hdc);
    if (p->cur_file_info.dec_status != DECODING)
    {
        int x, y;
        x = (int)((p->cur_file_info.dec_info.rect_width - p->cur_file_info.dec_info.img_width) >> 1);
        y = (int)((p->cur_file_info.dec_info.rect_height - p->cur_file_info.dec_info.img_height) >> 1);
        gui_dc_draw_bitmap_ext(hdc, p->cur_file_info.dec_info.buf, x, y,
                (int)p->cur_file_info.dec_info.img_width,
                (int)p->cur_file_info.dec_info.img_height,
                (int)p->cur_file_info.dec_info.bpp);
    }

    draw_directbar(p, p->user_opration_status);
    draw_txtbar(p, p->user_opration_status);

    if (p->hbar != NULL)
    {
        headbar_load_style(p->hbar);
        p->hbar->pic_id_head = p->pic_logo->pic_id;
        headbar_paint(p->hbar);
    }

    if(p->adjust != NULL)
    {
        adjust_load_style(p->adjust);
        adjust_paint(p->adjust);
    }

    if(p->vbar != NULL)
    {
        volumebar_load_style(p->vbar);
        volumebar_paint(p->vbar);
    }

    if(p->msgbox != NULL)
    {
        msgbox_load_style(p->msgbox);
        msgbox_paint(p->msgbox);
    }

    if(p->loadingbox != NULL)
    {
        loadingbox_load_style(p->loadingbox);
        loadingbox_paint(p->loadingbox);
    }

    if (p->option != NULL)
    {
        menuview_load_style(p->option, p->option->type);
        menuview_paint(p->option);
    }

    if (p->dialog != NULL)
    {
        dialog_load_style(p->dialog);
        dialog_paint(p->dialog);
    }

    scene_fusion_set_last_frame(p->scene);

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

    case MSG_GSENSOR_SHAKED:
        if((NULL != g_playing) && (NULL == g_playing->option))
        {
            dec_next_file(g_playing);
        }
        break;

    case MSG_GSENSOR_ROTATION_CHANGED:
        if(NULL != g_playing)
        {
            _playing_rotate(g_playing);
        }
        break;

    case MSG_MAIN_DISK_CHANGED:
    case MSG_APP_QUIT:
        exit_msg_loop();
        if(NULL != g_playing)
        {
            g_playing->exit_type = SCENE_PLAYING_EXIT_APP;
        }
        break;

    default:
        if(g_playing == NULL)
        {
            break;
        }

        if(g_playing->msgbox != NULL)
        {
            msgbox_proc_sys(g_playing->msgbox, msg);
        }

        /* 正在浏览卡目录 */
        if(((MSG_CARD_OUT == msg->type) && (PATH_CARD == g_playing->path_type))
             || ((MSG_USB_HOST_DISK_OUT == msg->type) && (PATH_EXTERNAL == g_playing->path_type)))
        {
            photo_playlist_del_items(0, -1);
            exit_msg_loop();
            g_playing->exit_type = SCENE_PLAYING_STORAGE_OUT;
            if(NULL != g_playing->cur_file_info.file_path)
            {
                free(g_playing->cur_file_info.file_path);
                g_playing->cur_file_info.file_path = NULL;
            }
            photo_dirlist_delete_all();
            photo_level_info_delete_all();
        }
        break;
    }
    return 0;
}

/* scene entry function */
int photo_scene_playing(fusion_enter_e fenter)
{
    int ret;
    msg_apps_t msg;
    scene_playing_ret_e exit_type = SCENE_PLAYING_RETURN;

    register_sys_dispatcher(_sys_cb);

    ret = _playing_init(fenter);
    if (ret < 0)
    {
        print_warning("scene init error! \n");
        if(NULL != g_playing)
        {
            exit_type = g_playing->exit_type;
        }
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

    exit_type = g_playing->exit_type;

    /* 场景退出 */
    ret = _playing_deinit();

    return exit_type;
}

