/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_preview_widget_dirlist.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason    2009-06-14          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_preview_widget_dirlist.c
 * \brief    photo dir list
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/06/14
 *******************************************************************************/

#include "photo_scene_preview.h"
#include "photo_preview_widget_dirlist.h"
#include "photo_dec_ctrl.h"
#include "photo_playlist.h"

static void _dirlist_dec_cb(void);
static bool _dec_current_pic(preview_dirlist_t *prv, const char *fullpath);
static bool _dec_next_pic(preview_dirlist_t *prv);
static void _dirlist_cb(dirlist_t* dirlist, dirlist_sig_e sig);
static bool _dirlist_init(preview_dirlist_t *prv);
static bool _dirlist_deinit(preview_dirlist_t *prv);

static preview_dirlist_t *s_prev_dirlist = NULL;
static int s_search_num = 0;

/*
 * 解码完毕，启动的single shot定时器
 */
static int _single_dec_timer(void *param)
{
    preview_dirlist_t *prv = (preview_dirlist_t *) param;
    dirlist_paint_bitmap(prv->dirlist, prv->cur_dec_index);
    _dec_next_pic(prv);
    return 0;
}

static void _dirlist_dec_cb(void)
{
    mmm_image_status_t status;
    gui_bitmap_info_t bitmap = {0};

    if(s_prev_dirlist->cur_dec_index < 0)
    {
        return;
    }

    if (true == photo_dec_control(GET_DECODER_STATUS, (unsigned int) &status))
    {
        if (MMM_IMAGE_DECODER_FREE == status.status) /* 解码完成 */
        {
            print_dbg();
            bitmap.bytesperpixel = 2;
            bitmap.xsize = (unsigned short)s_prev_dirlist->dec_info.img_width;
            bitmap.ysize = (unsigned short)s_prev_dirlist->dec_info.img_height;
            bitmap.pdata = s_prev_dirlist->dec_info.buf;

            dirlist_set_item_bitmap(s_prev_dirlist->dirlist, s_prev_dirlist->cur_dec_index, &bitmap);
        }
        else if (MMM_IMAGE_DECODER_ERROR == status.status)
        {
            print_warning("\ndecode file error");
            dirlist_set_item_bitmap(s_prev_dirlist->dirlist, s_prev_dirlist->cur_dec_index,
                    s_prev_dirlist->bmp_err);
        }
        else
        {

        }

        if (MMM_IMAGE_DECODER_BUSY != status.status)
        {
            if(-1 == g_preview->dec_single_timer_id)
            {
                g_preview->dec_single_timer_id = set_single_shot_timer(1, _single_dec_timer, s_prev_dirlist);
            }
            else
            {
                restart_timer(g_preview->dec_single_timer_id);
            }
        }
    }
}

static bool _dec_current_pic(preview_dirlist_t *prv, const char *fullpath)
{
    photo_dec_control(ABORT_DECODE, 0);

    if (!photo_dec_control(SET_FILE, (unsigned int) fullpath))
    {
        print_warning("set file err");
        return false;
    }

    if (!photo_dec_control(SET_CALLBACK, (unsigned int) _dirlist_dec_cb))
    {
        print_warning("set callback err");
        return false;
    }

    if (!photo_dec_control(IMG_BROWSE, (unsigned int) &prv->dec_info))
    {
        print_warning("decode image err");
        return false;
    }

    return true;
}

static bool _dec_next_pic(preview_dirlist_t *prv)
{
    int i;
    gui_bitmap_info_t *bitmap_info = NULL;
    char fullpath[MAX_PATH_LENGTH], *name;
    int total = dirlist_get_total(prv->dirlist);
    int start_id = prv->cur_dec_index + 1;

    photo_dec_control(ABORT_DECODE, 0);

    prv->dec_info.bpp = 2;
    prv->dec_info.rect_width = (unsigned int)dirlist_get_bitmap_width(prv->dirlist);
    prv->dec_info.rect_height = (unsigned int)dirlist_get_bitmap_height(prv->dirlist);
    prv->dec_info.len = (prv->dec_info.rect_width * prv->dec_info.rect_height)
                                * prv->dec_info.bpp;
    if(NULL != prv->dec_info.buf)
    {
        free(prv->dec_info.buf);
    }
    prv->dec_info.buf = (unsigned char *)malloc(prv->dec_info.len);
    if(NULL == prv->dec_info.buf)
    {
        print_err("malloc fail");
        return false;
    }

    for(i = start_id; i < total; i++)
    {
        bitmap_info = dirlist_get_item_bitmap(prv->dirlist, i);
        if(NULL == bitmap_info)
        {
            name = dirlist_get_item_name(prv->dirlist, i);
            sprintf(fullpath, "%s%s", prv->dirlist->path_name, name);

            if (true == path_is_directory(fullpath))
            {
                /*
                 * dirlist_set_item_bitmap(prv->dirlist, i, prv->bmp_folder);
                 * dirlist_paint_bitmap(prv->dirlist, i);
                 */
                continue;
            }

            prv->cur_dec_index = i;
            if (true == _dec_current_pic(prv, fullpath))
            {
                break;
            }
            else
            {
                print_warning("decode error, path: %s", fullpath);
                dirlist_set_item_bitmap(prv->dirlist, i, prv->bmp_err);
                dirlist_paint_bitmap(prv->dirlist, i);
            }
        }
    }
    return true;
}


static void _searching_done(void)
{
    dialog_init_t dialog_init;

    loadingbox_delete(g_preview->loadingbox);
    g_preview->loadingbox = NULL;

    if (s_search_num <= 0)
    {
        //clear filter string
        dirlist_set_search_string(s_prev_dirlist->dirlist, "");

        //show cann't found dialog
        dialog_init.head = DIALOG_HEAD_WARN;
        dialog_init.str_id = g_preview->str_not_found->str_id;
        dialog_modal(&dialog_init);
    }
    else
    {
        //delete softkeyboard
        softkeyboard_delete(g_preview->softkey);
        g_preview->softkey = NULL;
        dirlist_show(s_prev_dirlist->dirlist);
        dirlist_paint(s_prev_dirlist->dirlist);
        kill_timer(g_preview->search_single_timer_id);
    }
}

static void _deal_searching( void )
{
    char text[256];

    softkeyboard_get_text(g_preview->softkey, text, sizeof(text));
    s_search_num = dirlist_set_search_string(s_prev_dirlist->dirlist, text);
    g_preview->search_single_timer_id = set_single_shot_timer(1, (timer_proc)_searching_done, NULL);
}

static void _loadingbox_cb(loadingbox_t *loadingbox, loadingbox_sig_e signal)
{
}

static void _softkey_cb(softkeyboard_t *softkey, softkeyboard_sig_e sig)
{
    switch(sig)
    {
    case WIDGET_SIG_SKB_OK:
        if(g_preview->loadingbox == NULL)
        {
            g_preview->loadingbox = loadingbox_create(LOADINGBOX_SEARCHING, _loadingbox_cb);
            if ( NULL == g_preview->loadingbox)
            {
                print_err("loadingbox_create failed\n\n");
                return;
            }
            else
            {
                loadingbox_paint(g_preview->loadingbox);
            }
        }
        photo_create_thread(_deal_searching, NULL);
        break;

    case WIDGET_SIG_SKB_CANCEL:
        softkeyboard_delete(softkey);
        g_preview->softkey = NULL;
        //dirlist_set_search_string(s_prev_dirlist->dirlist, "");
        dirlist_show(s_prev_dirlist->dirlist);
        dirlist_paint(s_prev_dirlist->dirlist);
        break;

    default:
        break;
    }
}

static void _dirlist_cb(dirlist_t* dirlist, dirlist_sig_e sig)
{
    char fullpath[MAX_PATH_LENGTH];
    int idx;

    switch (sig)
    {
    case WIDGET_SIG_DIRLIST_OK:
        print_dbg(" filepath=%s filename = %s", dirlist->path_name, dirlist->file_name);

        photo_playlist_del_items(0, -1);
        if(!photo_playlist_add_dir(dirlist->path_name))
        {
            photo_preview_show_warning(g_preview->str_plist_error);
        }
        else
        {
            sprintf(fullpath, "%s%s", dirlist->path_name, dirlist->file_name);
            idx = photo_playlist_get_file_index(fullpath);
            set_config(CFG_PHOTO_CURRENT_INDEX, (const char *) &idx, sizeof(idx));
            s_prev_dirlist->callback(s_prev_dirlist, WIDGET_SIG_PREVIEW_OK);
        }
        break;

    case WIDGET_SIG_DIRLIST_QUIT:
        print_dbg();
        s_prev_dirlist->callback(s_prev_dirlist, WIDGET_SIG_PREVIEW_QUIT);
        break;

    case WIDGET_SIG_DIRLIST_DECODE:
        s_prev_dirlist->cur_dec_index = -1;
        _dec_next_pic(s_prev_dirlist);
        break;

    case WIDGET_SIG_DIRLIST_SHORT_MENU:
        if(NULL == g_preview->softkey)
        {
            g_preview->softkey = softkeyboard_create(IME_TYPE_PINYIN, _softkey_cb);
            if(NULL == g_preview->softkey)
            {
                print_err("create softkeyboard fail");
                break;
            }
        }
        dirlist_hide(s_prev_dirlist->dirlist);
        softkeyboard_paint(g_preview->softkey);
        break;

    default:
        break;
    }
}

static bool _dirlist_init(preview_dirlist_t *prv)
{
    dirlist_init_t init;
    picregion_frame_t frame;
    char ext[CFG_MAX_BUFF_LENGTH];

    if(get_config(CFG_APP_SUPPORT_EXT_PHOTO, ext, CFG_MAX_BUFF_LENGTH) < 0)
    {
        print_err();
        return false;
    }

    memset(prv, 0, sizeof(preview_dirlist_t));

    init.type = DIRLIST_PHOTO;
    init.filter_string = ext;
    init.init_path_name = g_preview->init_sel_path;
    init.top_path_name = g_preview->top_dir;

    prv->dirlist = photo_dirlist_get_current();
    if(NULL == prv->dirlist)
    {
        prv->dirlist = photo_dirlist_create(&init, _dirlist_cb);
        if(NULL == prv->dirlist)
        {
            print_err("dirlist create failure");
            return false;
        }
    }
    else
    {
        dirlist_show(prv->dirlist);
    }

    get_picregion_frame(g_preview->picreg_err, 0, &frame);
    s_prev_dirlist->bmp_err = sty_create_bitmap((int)frame.pic_id);

    get_picregion_frame(g_preview->picreg_folder, 0, &frame);
    s_prev_dirlist->bmp_folder = sty_create_bitmap((int)frame.pic_id);

    return true;
}

static bool _dirlist_deinit(preview_dirlist_t *prv)
{
    photo_dec_control(ABORT_DECODE, 0);

    if(NULL == prv)
    {
        return false;
    }

    if(NULL != prv->dirlist)
    {
        if(g_preview->exit_type != SCENE_PREVIEW_NEXT)
        {
            photo_dirlist_delete(prv->dirlist);
            prv->dirlist = NULL;
        }
        else
        {
            dirlist_hide(prv->dirlist);
        }
    }

    if(-1 != g_preview->dec_single_timer_id)
    {
        kill_timer(g_preview->dec_single_timer_id);
        g_preview->dec_single_timer_id = -1;
    }

    if(NULL != prv->bmp_err)
    {
        sty_delete_bitmap(prv->bmp_err);
        prv->bmp_err = NULL;
    }

    if(NULL != prv->bmp_folder)
    {
        sty_delete_bitmap(prv->bmp_folder);
        prv->bmp_folder = NULL;
    }

    if(NULL != prv->dec_info.buf)
    {
        free(prv->dec_info.buf);
    }

    free(prv);

    return true;
}

void photo_preview_widget_dirlist_paint(preview_dirlist_t *prv)
{
    dirlist_paint(prv->dirlist);
}

void photo_preview_widget_dirlist_rotate(preview_dirlist_t *prv)
{
    picregion_frame_t frame;

    photo_dec_control(ABORT_DECODE, 0);

    /* 重新读取资源 */
    if(NULL != prv->bmp_err)
    {
        sty_delete_bitmap(prv->bmp_err);
    }
    get_picregion_frame(g_preview->picreg_err, 0, &frame);
    prv->bmp_err = sty_create_bitmap((int)frame.pic_id);

    if(NULL != prv->bmp_folder)
    {
        sty_delete_bitmap(prv->bmp_folder);
    }
    get_picregion_frame(g_preview->picreg_folder, 0, &frame);
    prv->bmp_folder = sty_create_bitmap((int)frame.pic_id);

    if(NULL != prv->dirlist)
    {
        dirlist_load_style(prv->dirlist);
    }
}

preview_dirlist_t *photo_preview_widget_dirlist_create(preview_dirlist_cb_t callback)
{
    preview_dirlist_t *prv;

    prv = (preview_dirlist_t *) malloc(sizeof(preview_dirlist_t));
    if (NULL == prv)
    {
        print_err("allocate error!\n");
        return NULL;
    }

    s_prev_dirlist = prv;

    if (false == _dirlist_init(prv))
    {
        print_warning("init err");
        goto err;
    }

    prv->callback = callback;
    return prv;

    err:
    _dirlist_deinit(prv);
    return NULL;
}

bool photo_preview_widget_dirlist_delete(preview_dirlist_t *prv)
{
    if (NULL != prv)
    {
        _dirlist_deinit(prv);
        s_prev_dirlist = NULL;
    }
    return true;
}

bool photo_preview_widget_dirlist_proc_gui(preview_dirlist_t *prv, gui_wm_message_t *pmsg)
{
    return dirlist_proc_gui(prv->dirlist, pmsg);
}




