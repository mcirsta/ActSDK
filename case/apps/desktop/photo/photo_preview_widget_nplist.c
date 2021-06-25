/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_preview_widget_nplist.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason    2009-06-14         v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_preview_widget_nplist.c
 * \brief    photo preview nowplaying list widget
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/06/14
 *******************************************************************************/

#include "photo_scene_preview.h"
#include "photo_preview_widget_nplist.h"
#include "photo_dec_ctrl.h"
#include "photo_playlist.h"

static int _get_next_not_dec_index(preview_nplist_t *prv, int current);
static void _menuview_cb(menuview_t *mv, menuview_sig_e signal);
static void _menu_item_cb(menuview_t *mv, menuview_sig_e signal);
static inline void _fill_item(preview_nplist_t *prv, char *filename, menuitem_t *item);
static bool _get_items(preview_nplist_t *prv, int first, int num);
static bool _to_first_page(preview_nplist_t *prv);
static bool _to_last_page(preview_nplist_t *prv);
static bool _menuview_supply_head(preview_nplist_t *prv);
static bool _menuview_supply_tail(preview_nplist_t *prv);

static void _nplist_dec_cb(void);
static int _nplist_init(preview_nplist_t *prv);
static int _nplist_deinit(preview_nplist_t *prv);
static bool _dec_current_pic(preview_nplist_t *prv);
static bool _dec_next_pic(preview_nplist_t *prv);
static void _draw_nplist_item(preview_nplist_t *prv, int index);

static preview_nplist_t *s_nplist = NULL;
static int s_start_index_saved = -1;
static int s_cur_index_saved = -1;

/* 获取下一个未解码index，失败返回-1 */
static int _get_next_not_dec_index(preview_nplist_t *prv, int current)
{
    int i, ret;

    ret = -1;
    for (i = current + 1; i < prv->item_per_screen; i++)
    {
        if ((NOT_DECODE == prv->previewitem[i].dec_status)
                || (DECODING == prv->previewitem[i].dec_status))
        {
            ret = i;
            break;
        }
    }

    /* 如果没找到，则从头开始搜索 */
    if(-1 == ret)
    {
        for(i = 0; i < (current - 1); i++)
        {
            if ((NOT_DECODE == prv->previewitem[i].dec_status)
                    || (DECODING == prv->previewitem[i].dec_status))
            {
                ret = i;
                break;
            }
        }
    }

    return ret;
}

static void _menuview_cb(menuview_t *mv, menuview_sig_e signal)
{
}

static void _menu_item_cb(menuview_t *mv, menuview_sig_e signal)
{
    int idx;

    switch (signal)
    {
    case WIDGET_SIG_MENUVIEW_OK:
        s_nplist->cur_index = menu_get_cur_index(s_nplist->menu);
        idx = s_nplist->start_index + s_nplist->cur_index;
        set_config(CFG_PHOTO_CURRENT_INDEX, (char *) &idx, sizeof(int));
        s_nplist->callback(s_nplist, WIDGET_SIG_PREVIEW_OK);
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        s_nplist->callback(s_nplist, WIDGET_SIG_PREVIEW_QUIT);
        break;

    case WIDGET_SIG_MENUVIEW_UPDATE_ALL:
        if(0 == mv->menu->global_offset)
        {
            _to_first_page(s_nplist); /* 从最后一项向下前进到第一页 */
        }
        else
        {
            _to_last_page(s_nplist); /* 从第一项向上回到最后一页 */
        }
        break;

    case WIDGET_SIG_MENUVIEW_EXCEED_FIRST:
        _menuview_supply_head(s_nplist);
        break;

    case WIDGET_SIG_MENUVIEW_EXCEED_LAST:
        _menuview_supply_tail(s_nplist);
        break;

    default:
        break;
    }
}

static inline void _fill_item(preview_nplist_t *prv, char *filename, menuitem_t *item)
{
    unsigned int len;

    memset(item, 0, sizeof(menuitem_t));
    len = strlen(filename) + 1;
    item->text = (char*) malloc(len);
    if(NULL == item->text)
    {
        print_err("malloc fail");
        return;
    }
    strncpy(item->text, filename, len);
    item->callback = (item_cb_t) _menu_item_cb;
    item->has_child = false;
}

/*
 * 获取若干个item数据
 */
static bool _get_items(preview_nplist_t *prv, int first, int num)
{
    int i;
    unsigned int len;
    plist_detail_info_t info;

    for (i = first; i < (first + num); i++)
    {
        prv->previewitem[i].dec_status = NOT_DECODE;
        if (NULL != prv->previewitem[i].file_path)
        {
            free(prv->previewitem[i].file_path);
        }
        if (NULL != prv->previewitem[i].file_name)
        {
            free(prv->previewitem[i].file_name);
        }
        prv->previewitem[i].file_path = NULL;
        prv->previewitem[i].file_name = NULL;
    }

    for (i = first; i < (first + num); i++)
    {
        /* 播放列表没有目录 */
        photo_playlist_get_detail_info(&info, prv->start_index + i, 1);
        len = strlen(info.fullpath) + 1;
        prv->previewitem[i].file_path = (char *) malloc(len);
        if(NULL == prv->previewitem[i].file_path)
        {
            print_err("malloc fail");
            return false;
        }
        sprintf(prv->previewitem[i].file_path, "%s", info.fullpath);
        len = strlen(info.filename) + 1;
        prv->previewitem[i].file_name = (char *) malloc(len);
        if(NULL == prv->previewitem[i].file_name)
        {
            print_err("malloc fail");
            return false;
        }
        sprintf(prv->previewitem[i].file_name, "%s", info.filename);

        photo_playlist_free_detail_info(&info, 1);
    }

    return true;
}

/*
 * 回到第一页
 */
static bool _to_first_page(preview_nplist_t *prv)
{
    int i;
    menuitem_t item;
    bool one_page = false;
    int max_per_screen = menuview_get_lines(prv->menuview);

    if(prv->total_num <= max_per_screen)
    {
        one_page = true;
    }

    prv->item_per_screen = (true == one_page) ? prv->total_num : max_per_screen;
    prv->start_index = 0;
    prv->cur_index = 0;
    prv->dec_index = -1;

    if (!one_page) /* 只有总数至少两屏时，才重新取出一屏数据及解码 */
    {
        menu_remove_all_items(prv->menu);
        _get_items(prv, 0, prv->item_per_screen);
        for(i = 0 ; i < prv->item_per_screen; i++)
        {
            _fill_item(prv, prv->previewitem[i].file_name, &item);
            menu_append_item(prv->menu, &item);
        }
        prv->dec_index = _get_next_not_dec_index(prv, -1);
        if (prv->dec_index >= 0)
        {
            _dec_current_pic(prv);
        }
    }

    prv->menu->offset = 0;
    prv->menu->current = 0;
    prv->menu->global_offset = prv->start_index;
    photo_preview_widget_nplist_paint(prv);

    return true;
}

/*
 * 回到最后一页
 */
static bool _to_last_page(preview_nplist_t *prv)
{
    int i;
    menuitem_t item;
    bool one_page = false;
    int max_per_screen = menuview_get_lines(prv->menuview);

    if(prv->total_num <= max_per_screen)
    {
        one_page = true;
    }

    prv->item_per_screen = (true == one_page) ? prv->total_num : max_per_screen;
    prv->start_index = (true == one_page) ? 0 : (prv->total_num - max_per_screen);
    prv->cur_index = 0;
    prv->dec_index = -1;

    if (!one_page) /* 只有总数至少两屏时，才重新取出一屏数据及解码 */
    {
        menu_remove_all_items(prv->menu);
        _get_items(prv, 0, prv->item_per_screen);
        for(i = 0 ; i < prv->item_per_screen; i++)
        {
            _fill_item(prv, prv->previewitem[i].file_name, &item);
            menu_append_item(prv->menu, &item);
        }

        prv->dec_index = _get_next_not_dec_index(prv, -1);
        if (prv->dec_index >= 0)
        {
            _dec_current_pic(prv);
        }
    }

    prv->menu->offset = 0;
    prv->menu->current = prv->item_per_screen - 1;
    prv->menu->global_offset = prv->start_index;
    photo_preview_widget_nplist_paint(prv);
    return true;
}

/*
 * 前面补上一个文件，删掉列表最后一项
 */
static bool _menuview_supply_head(preview_nplist_t *prv)
{
    int i;
    menuitem_t item, *temp_mitem;
    dec_file_info_t temp_pitem;

    if ((prv->start_index <= 0) || (prv->total_num <= 0))
    {
        /* 前面没有更多的图片 */
        return _to_last_page(prv);
    }

    prv->start_index--;

    /* 更新菜单项 */
    temp_mitem = menu_get_item(prv->menu, prv->item_per_screen - 1);
    assert(NULL != temp_mitem);
    if (NULL != temp_mitem->text)
    {
        free(temp_mitem->text);
    }

    if (NULL != temp_mitem->bitmap)
    {
        free(temp_mitem->bitmap);
    }

    menu_remove_last_item(prv->menu);

    /* preview item 后移一项 */
    temp_pitem = prv->previewitem[prv->item_per_screen - 1];

    for (i = prv->item_per_screen - 1; i > 0; i--)
    {
        prv->previewitem[i] = prv->previewitem[i - 1];
    }

    prv->previewitem[0] = temp_pitem;
    _get_items(prv, 0, 1);
    _fill_item(prv, prv->previewitem[0].file_name, &item);
    menu_prepend_item(prv->menu, &item);
    prv->dec_index = 0;
    _dec_current_pic(prv);

    prv->menu->global_offset = prv->start_index;

    return true;
}

/*
 * 后面补上一个文件，删掉列表前面一项
 */
static bool _menuview_supply_tail(preview_nplist_t *prv)
{
    int i;
    menuitem_t item, *temp_mitem;
    dec_file_info_t temp_pitem;


    if ((prv->start_index + prv->item_per_screen) >= prv->total_num)
    {
        /* 后面没有更多的图片 */
        return _to_first_page(prv);
    }

    prv->start_index++;

    /* 更新菜单项 */
    temp_mitem = menu_get_item(prv->menu, 0);
    assert(NULL != temp_mitem);
    if (NULL != temp_mitem->text)
    {
        free(temp_mitem->text);
    }

    if (NULL != temp_mitem->bitmap)
    {
        free(temp_mitem->bitmap);
    }

    menu_remove_first_item(prv->menu);

    /* preview item 前移一项 */
    temp_pitem = prv->previewitem[0];

    for (i = 0; i < (prv->item_per_screen - 1); i++)
    {
        prv->previewitem[i] = prv->previewitem[i + 1];
    }

    prv->previewitem[prv->item_per_screen - 1] = temp_pitem;
    if (NULL != prv->previewitem[prv->item_per_screen - 1].file_path)
    {
        free(prv->previewitem[prv->item_per_screen - 1].file_path);
    }

    _get_items(prv, prv->item_per_screen - 1, 1);
    _fill_item(prv, prv->previewitem[prv->item_per_screen - 1].file_name, &item);
    menu_append_item(prv->menu, &item);
    prv->dec_index = prv->item_per_screen - 1;
    _dec_current_pic(prv);

    prv->menu->global_offset = prv->start_index;

    return true;
}

/*
 * 解码完毕，启动的single shot定时器
 */
static int _single_dec_timer(void *param)
{
    preview_nplist_t *prv = (preview_nplist_t *) param;
    _draw_nplist_item(prv, prv->dec_index);
    _dec_next_pic(prv);
    return 0;
}

/*
 * 中间件解码回调函数
 * 函数里面不能发送解码命令，否则死锁
 */
static void _nplist_dec_cb(void)
{
    mmm_image_status_t status;

    if(s_nplist->dec_index < 0)
    {
        return;
    }

    if (true == photo_dec_control(GET_DECODER_STATUS, (unsigned int) &status))
    {
        if (MMM_IMAGE_DECODER_FREE == status.status) /* 解码完成 */
        {
            print_dbg();
            s_nplist->previewitem[s_nplist->dec_index].dec_status = DECODE_FINISHED;
        }
        else if (MMM_IMAGE_DECODER_ERROR == status.status)
        {
            print_dbg();
            s_nplist->previewitem[s_nplist->dec_index].dec_status = DECODE_ERROR;
        }
        else
        {

        }

        if (MMM_IMAGE_DECODER_BUSY != status.status)
        {
            if(-1 == g_preview->dec_single_timer_id)
            {
                g_preview->dec_single_timer_id = set_single_shot_timer(1, _single_dec_timer, s_nplist);
            }
            else
            {
                restart_timer(g_preview->dec_single_timer_id);
            }
        }
    }
}

static bool _data_init(preview_nplist_t *prv)
{
    int i;
    unsigned int lines;
    unsigned int width, height;
    picregion_frame_t frame;

    lines = (unsigned int)menuview_get_lines(prv->menuview);
    prv->previewitem = (dec_file_info_t *) malloc(sizeof(dec_file_info_t) * lines);
    if(NULL == prv->previewitem)
    {
        print_err("malloc fail");
        return false;
    }

    width = (unsigned int)menuview_get_bitmap_width(prv->menuview);
    height = (unsigned int)menuview_get_bitmap_height(prv->menuview);
    for (i = 0; i < (int)lines; i++)
    {
        prv->previewitem[i].dec_info.bpp = 2;
        prv->previewitem[i].dec_info.formate = RGB_565;
        prv->previewitem[i].dec_info.len = width * height * 2;
        prv->previewitem[i].dec_info.buf = (unsigned char *) malloc(width * height * 2);
        if(NULL == prv->previewitem[i].dec_info.buf)
        {
            print_err("malloc fail");
            return false;
        }
        prv->previewitem[i].dec_info.rect_width = width;
        prv->previewitem[i].dec_info.rect_height = height;
        prv->previewitem[i].dec_status = NOT_DECODE;
        prv->previewitem[i].file_path = NULL;
        prv->previewitem[i].file_name = NULL;
    }

    get_picregion_frame(g_preview->picreg_err, 0, &frame);
    prv->bmp_err_unsel = sty_create_bitmap((int)frame.pic_id);

    get_picregion_frame(g_preview->picreg_err, 1, &frame);
    prv->bmp_err_sel = sty_create_bitmap((int)frame.pic_id);

    return true;
}

/*
 * 初始化index（包括状态恢复）
 */
static bool _init_index(preview_nplist_t *prv)
{
    int init_index;
    int lines;

    lines = menuview_get_lines(prv->menuview);

    /* 计算起始位置 */
    if(NULL == g_preview->init_sel_path)
    {
        init_index = 0;
    }
    else
    {
        init_index = photo_playlist_get_file_index(g_preview->init_sel_path);
    }

    if((s_start_index_saved >= 0) && (s_cur_index_saved >= 0))
    {
        /* 状态恢复 */
        int index_saved = s_start_index_saved + s_cur_index_saved;
        if(init_index != index_saved)
        {
            /* 文件index已经有变动, 重新计算初始index */
            if((init_index >= s_start_index_saved) && (init_index < (s_start_index_saved + lines)))
            {
                /* 在当前屏内 */
                prv->start_index = s_start_index_saved;
                prv->cur_index = init_index - prv->start_index;
            }
            else if(init_index < s_start_index_saved)
            {
                /* 当前屏之前 */
                prv->start_index = init_index;
                prv->cur_index = 0;
            }
            else
            {
                /* 当前屏之后 */
                prv->start_index = (init_index - lines) + 1;
                prv->cur_index = lines - 1;
            }
        }
        else
        {
            /* 防止旋转屏 */
            if(prv->cur_index >= lines)
            {
                prv->start_index = (init_index - lines) + 1;
                prv->cur_index = lines - 1;
            }
            else
            {
                prv->start_index = s_start_index_saved;
                prv->cur_index = s_cur_index_saved;
            }
        }
        s_start_index_saved = -1;
        s_cur_index_saved = -1;
    }
    else
    {
        prv->start_index = (init_index > (lines - 1)) ? (init_index - (lines - 1)) : 0;
        prv->cur_index = init_index - prv->start_index;
    }

    return true;
}

static int _nplist_init(preview_nplist_t *prv)
{
    int i;
    int lines;
    menuitem_t item;
    memset(prv, 0, sizeof(preview_nplist_t));

    prv->menuview = menuview_create(MENUVIEW_PICLIST_PICTURE, _menuview_cb);
    if(NULL == prv->menuview)
    {
        return -1;
    }
    prv->menu = photo_menu_create();
    if(NULL == prv->menu)
    {
        return -1;
    }

    _data_init(prv);

    lines = menuview_get_lines(prv->menuview);
    prv->total_num = photo_playlist_get_num();

    _init_index(prv);
    if((prv->start_index + lines) <= prv->total_num)
    {
        prv->item_per_screen = lines;
    }
    else
    {
        prv->item_per_screen = prv->total_num - prv->start_index;
    }

    /* 取出一屏个数的图片路径 */
    _get_items(prv, 0, prv->item_per_screen);

    for (i = 0; i < prv->item_per_screen; i++)
    {
        /* 菜单数据 */
        _fill_item(prv, prv->previewitem[i].file_name, &item);
        menu_append_item(prv->menu, &item);
    }

    prv->menu->global_offset = prv->start_index;
    prv->menu->global_size = prv->total_num;
    prv->menu->current = prv->cur_index;
    prv->menu->offset = 0;
    menuview_set_menu(prv->menuview, prv->menu);

    return 0;
}

static int _data_deinit(preview_nplist_t *prv)
{
    int i;
    menuitem_t *item;

    if(-1 != g_preview->dec_single_timer_id)
    {
        kill_timer(g_preview->dec_single_timer_id);
        g_preview->dec_single_timer_id = -1;
    }

    for (i = 0; i < menuview_get_lines(prv->menuview); i++)
    {
        if (NULL != prv->previewitem[i].dec_info.buf)
        {
            free(prv->previewitem[i].dec_info.buf);
        }

        if (NULL != prv->previewitem[i].file_path)
        {
            free(prv->previewitem[i].file_path);
        }

        if (NULL != prv->previewitem[i].file_name)
        {
            free(prv->previewitem[i].file_name);
        }
    }

    sty_delete_bitmap(prv->bmp_err_unsel);
    sty_delete_bitmap(prv->bmp_err_sel);

    /* 释放item资源 */
    item = menu_get_item(prv->menu, 0);
    while (NULL != item)
    {
        if (NULL != item->text)
        {
            free(item->text);
        }
        if (NULL != item->bitmap)
        {
            free(item->bitmap);
        }
        menu_remove_first_item(prv->menu);
        item = menu_get_item(prv->menu, 0);
    }

    return 0;
}

static int _nplist_deinit(preview_nplist_t *prv)
{
    photo_dec_control(ABORT_DECODE, 0);

    if (NULL == prv)
    {
        return 0;
    }

    if(SCENE_PREVIEW_NEXT == g_preview->exit_type)
    {
        s_start_index_saved = prv->start_index;
        s_cur_index_saved = menu_get_cur_index(prv->menu);
    }
    else
    {
        s_start_index_saved = -1;
        s_cur_index_saved = -1;
    }

    _data_deinit(prv);
    photo_menu_delete(prv->menu);
    menuview_delete(prv->menuview);

    free(prv->previewitem);
    free(prv);

    s_nplist = NULL;

    return 0;
}

static bool _dec_current_pic(preview_nplist_t *prv)
{
    photo_dec_control(ABORT_DECODE, 0);

    if (!photo_dec_control(SET_FILE, (unsigned int) prv->previewitem[prv->dec_index].file_path))
    {
        print_warning("set file err");
        goto err;
    }

    if (!photo_dec_control(GET_MEDIA_INFO, (unsigned int) &prv->previewitem[prv->dec_index].file_info))
    {
        print_warning("get media info err");
        goto err;
    }
    else
    {
        print_info("width = %d,height=%d,format=%s\n",
                prv->previewitem[prv->dec_index].file_info.width,
                prv->previewitem[prv->dec_index].file_info.height,
                prv->previewitem[prv->dec_index].file_info.file_format);
    }

    if (!photo_dec_control(SET_CALLBACK, (unsigned int) _nplist_dec_cb))
    {
        print_warning("set callback err");
        goto err;
    }

    if (!photo_dec_control(IMG_BROWSE, (unsigned int) &prv->previewitem[prv->dec_index].dec_info))
    {
        print_warning("decode image err");
        goto err;
    }

    prv->previewitem[prv->dec_index].dec_status = DECODING;
    return true;

    err:
    prv->previewitem[prv->dec_index].dec_status = DECODE_ERROR;
    return false;
}

static bool _dec_next_pic(preview_nplist_t *prv)
{
    int idx;
    bool ret = false;

    photo_dec_control(ABORT_DECODE, 0);

    /* 解码下一幅图片，直到有图片setfile成功，或者到达当前屏幕的最后一个文件 */
    idx = prv->dec_index;
    do
    {
        idx = _get_next_not_dec_index(prv, idx);
        if (-1 != idx)
        {
            prv->dec_index = idx;
            ret = _dec_current_pic(prv);
        }
    }while((-1 != idx) && (false == ret));

    return ret;
}

static void _draw_nplist_item(preview_nplist_t *prv, int idx)
{
    int hdc;
    menuitem_t *item;

    hdc = gui_dc_get((unsigned short)g_preview->win_id);
    item = menu_get_item(prv->menu, idx);
    assert(NULL != item);

    if (NULL == item->bitmap)
    {
        item->bitmap = (gui_bitmap_info_t *) malloc(sizeof(gui_bitmap_info_t));
        if(NULL == item->bitmap)
        {
            print_err("malloc fail");
            return;
        }
    }

    if(DECODE_FINISHED == prv->previewitem[prv->dec_index].dec_status)
    {
        item->bitmap->bytesperpixel = (unsigned short)prv->previewitem[prv->dec_index].dec_info.bpp;
        item->bitmap->xsize = (unsigned short)prv->previewitem[prv->dec_index].dec_info.img_width;
        item->bitmap->ysize = (unsigned short)prv->previewitem[prv->dec_index].dec_info.img_height;
        item->bitmap->pdata = prv->previewitem[prv->dec_index].dec_info.buf;
    }
    else
    {
        if(prv->dec_index == prv->menu->current)
        {
            item->bitmap->bytesperpixel = prv->bmp_err_sel->bytesperpixel;
            item->bitmap->xsize = prv->bmp_err_sel->xsize;
            item->bitmap->ysize = prv->bmp_err_sel->ysize;
            item->bitmap->pdata = prv->bmp_err_sel->pdata;
        }
        else
        {
            item->bitmap->bytesperpixel = prv->bmp_err_unsel->bytesperpixel;
            item->bitmap->xsize = prv->bmp_err_unsel->xsize;
            item->bitmap->ysize = prv->bmp_err_unsel->ysize;
            item->bitmap->pdata = prv->bmp_err_unsel->pdata;
        }
    }
    menuview_paint_bitmap(prv->menuview, idx);
}

void photo_preview_widget_nplist_paint(preview_nplist_t *prv)
{
    menuview_paint(prv->menuview);
}

void photo_preview_widget_nplist_rotate(preview_nplist_t *prv)
{
    int i, init_index;
    int lines;
    menuitem_t item;

    photo_dec_control(ABORT_DECODE, 0);
    prv->cur_index = menu_get_cur_index(prv->menu);

    _data_deinit(prv);
    menuview_load_style(prv->menuview, MENUVIEW_PICLIST_PICTURE);
    _data_init(prv);

    lines = menuview_get_lines(prv->menuview);
    init_index = prv->start_index + prv->cur_index;
    if(prv->cur_index >= lines)
    {
        prv->start_index = (init_index - lines) + 1;
        prv->cur_index = lines - 1;
    }
    prv->item_per_screen = ((prv->start_index + lines) <= prv->total_num) ? (lines)
                            :  (prv->total_num - prv->start_index);

    /* 取出一屏个数的图片路径 */
    _get_items(prv, 0, prv->item_per_screen);

    for (i = 0; i < prv->item_per_screen; i++)
    {
        /* 菜单数据 */
        _fill_item(prv, prv->previewitem[i].file_name, &item);
        menu_append_item(prv->menu, &item);
    }

    prv->menu->global_offset = prv->start_index;
    prv->menu->global_size = prv->total_num;
    prv->menu->current = prv->cur_index;
    prv->menu->offset = 0;
    menuview_set_menu(prv->menuview, prv->menu);

    /* 重新解码 */
    prv->dec_index = -1;
    _dec_next_pic(prv);
}

preview_nplist_t *photo_preview_widget_nplist_create(preview_nplist_cb_t callback)
{
    preview_nplist_t *prv;

    prv = (preview_nplist_t *) malloc(sizeof(preview_nplist_t));
    if (NULL == prv)
    {
        print_err("allocate error!\n");
        return NULL;
    }

    s_nplist = prv;

    if (_nplist_init(prv) < 0)
    {
        print_warning("init err");
        _nplist_deinit(prv);
        return NULL;
    }

    if (prv->total_num > 0)
    {
        prv->dec_index = -1;
        _dec_next_pic(prv);
    }

    prv->callback = callback;
    return prv;
}

bool photo_preview_widget_nplist_delete(preview_nplist_t *prv)
{
    if (NULL != prv)
    {
        _nplist_deinit(prv);
        s_nplist = NULL;
    }
    return true;
}

bool photo_preview_widget_nplist_proc_gui(preview_nplist_t *prv, gui_wm_message_t *pmsg)
{
    return menuview_proc_gui(prv->menuview, pmsg);
}

