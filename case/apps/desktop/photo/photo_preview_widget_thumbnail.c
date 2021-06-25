/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_preview_widget_thumbnail.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason    2009-06-14         v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_preview_widget_thumbnail.c
 * \brief    photo preview thumbnail widget
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/06/14
 *******************************************************************************/

#include "photo_scene_preview.h"
#include "photo_preview_widget_thumbnail.h"
#include "photo_dec_ctrl.h"
#include "photo_playlist.h"

static void _slider_cb(slider_t *slider, int signal);

static int _get_next_not_dec_index(preview_thumbnail_t *prv, int current);
static void _thumbnail_dec_cb(void);
static bool _get_items(preview_thumbnail_t *prv, int first, int num);

static bool _to_first_page(preview_thumbnail_t *prv, bool keepx);
static bool _to_last_page(preview_thumbnail_t *prv, bool keepx);
static bool _item_move_down(preview_thumbnail_t *prv, bool keepx);
static bool _item_move_up(preview_thumbnail_t *prv, bool keepx);
static void _thumbnail_move_sel(preview_thumbnail_t *prv, move_dir_e dir);
static bool _init_cur_dir_items(preview_thumbnail_t *prv);
static bool _cd_parent(preview_thumbnail_t *prv);
static bool _cd_child(preview_thumbnail_t *prv, const char *childname);

static int _thumbnail_init(preview_thumbnail_t *prv);
static int _thumbnail_deinit(preview_thumbnail_t *prv);
static void _draw_thumbnail_item(preview_thumbnail_t *prv, int idx, int idy, bool sel);
static bool _dec_current_pic(preview_thumbnail_t *prv);
static bool _dec_next_pic(preview_thumbnail_t *prv);

static preview_thumbnail_t * s_thumbnail = NULL;

static void _slider_cb(slider_t *slider, int signal)
{
}

/* 获取下一个未解码index，失败返回-1 */
static int _get_next_not_dec_index(preview_thumbnail_t *prv, int current)
{
    int i, ret;

    ret = -1;
    for (i = current + 1; i < prv->item_per_screen; i++)
    {
        if (((NOT_DECODE == prv->previewitem[i].info.dec_status)
                || (DECODING == prv->previewitem[i].info.dec_status))
                && (!prv->previewitem[i].is_dir))
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
            if (((NOT_DECODE == prv->previewitem[i].info.dec_status)
                    || (DECODING == prv->previewitem[i].info.dec_status))
                    && (!prv->previewitem[i].is_dir))
            {
                ret = i;
                break;
            }
        }
    }

    return ret;
}

static int _show_title_timer(void *param)
{
    kill_timer(g_preview->show_title_timer_id);
    g_preview->show_title_timer_id = -1;
    headbar_set_title_id(g_preview->hbar, g_preview->str_title->str_id);
    return 0;
}

/*
 * 解码完毕，启动的single shot定时器
 */
static int _single_dec_timer(void *param)
{
    preview_thumbnail_t *prv = (preview_thumbnail_t *) param;
    bool ret;

    print_dbg();
    if (prv->dec_index == prv->cur_level_info->cur_index)
    {
        ret = true;
    }
    else
    {
        ret = false;
    }
    _draw_thumbnail_item(prv, prv->dec_index % prv->numx, prv->dec_index / prv->numx, ret);
    _dec_next_pic(prv);
    return 0;
}

/*
 * 中间件解码回调函数
 * 函数里面不能发送解码命令，否则死锁
 */
static void _thumbnail_dec_cb(void)
{
    mmm_image_status_t status;

    print_dbg("dec_index:%d", s_thumbnail->dec_index);
    if (s_thumbnail->dec_index < 0)
    {
        return;
    }

    if (true == photo_dec_control(GET_DECODER_STATUS, (unsigned int) &status))
    {
        if (MMM_IMAGE_DECODER_FREE == status.status) /* 解码完成 */
        {
            print_dbg();
            s_thumbnail->previewitem[s_thumbnail->dec_index].info.dec_status = DECODE_FINISHED;
        }
        else if (MMM_IMAGE_DECODER_ERROR == status.status)
        {
            print_dbg();
            s_thumbnail->previewitem[s_thumbnail->dec_index].info.dec_status = DECODE_ERROR;
        }
        else
        {
        }

        if (MMM_IMAGE_DECODER_BUSY != status.status)
        {
            if (-1 == g_preview->dec_single_timer_id)
            {
                g_preview->dec_single_timer_id = set_single_shot_timer(1, _single_dec_timer, s_thumbnail);
            }
            else
            {
                restart_timer(g_preview->dec_single_timer_id);
            }
        }
    }
    print_dbg();

}

/*
 * 获取若干个item数据
 */
static bool _get_items(preview_thumbnail_t *prv, int first, int num)
{
    int i, idx;
    unsigned int len;
    fsel_filename_t filename;
    plist_detail_info_t info;
    char fullpath[PATH_MAX];

    for (i = first; i < (first + num); i++)
    {
        prv->previewitem[i].info.dec_status = NOT_DECODE;
        if (NULL != prv->previewitem[i].info.file_path)
        {
            free(prv->previewitem[i].info.file_path);
        }
        if (NULL != prv->previewitem[i].info.file_name)
        {
            free(prv->previewitem[i].info.file_name);
        }
        prv->previewitem[i].info.file_path = NULL;
        prv->previewitem[i].info.file_name = NULL;
    }

    if (g_preview->path_type == PATH_PLAYLIST)
    {
        for (i = first; i < (first + num); i++)
        {
            /* 播放列表没有目录 */
            if(!photo_playlist_get_detail_info(&info, prv->cur_level_info->start_index + i, 1))
            {
                print_err("photo_playlist_get_detail_info error!, start index:%d, "
                        "first index:%d, i:%d, num:%d",
                        prv->cur_level_info->start_index, first, i, 1);
                continue;
            }
            len = strlen(info.fullpath) + 1;
            prv->previewitem[i].info.file_path = (char *) malloc(len);
            if (NULL == prv->previewitem[i].info.file_path)
            {
                print_err("malloc fail");
                return false;
            }
            sprintf(prv->previewitem[i].info.file_path, "%s", info.fullpath);
            len = strlen(info.filename) + 1;
            prv->previewitem[i].info.file_name = (char *) malloc(len);
            if (NULL == prv->previewitem[i].info.file_name)
            {
                print_err("malloc fail");
                return false;
            }
            sprintf(prv->previewitem[i].info.file_name, "%s", info.filename);

            photo_playlist_free_detail_info(&info, 1);
            prv->previewitem[i].is_dir = false;
        }
    }
    else
    {
        for (i = first; i < (first + num); i++)
        {
            idx = prv->cur_level_info->start_index + i;
            if (idx < prv->total_dir_num)
            {
                fsel_listdir(prv->fsel, &filename, idx + 1, 1, FSEL_POSITIVE);
                fsel_getfullpath(prv->fsel, fullpath, idx + 1);
                prv->previewitem[i].is_dir = true;
            }
            else
            {
                fsel_listfile(prv->fsel, &filename, (idx + 1) - prv->total_dir_num, 1, FSEL_POSITIVE);
                fsel_getfullpath(prv->fsel, fullpath, (idx + 1) - prv->total_dir_num);
                prv->previewitem[i].is_dir = false;
            }
            len = strlen(fullpath) + strlen(filename.name) + 1;
            prv->previewitem[i].info.file_path = (char *) malloc(len);
            if (NULL == prv->previewitem[i].info.file_path)
            {
                print_err("malloc fail");
                return false;
            }

            sprintf(prv->previewitem[i].info.file_path, "%s%s", fullpath, filename.name);
            //print_err("\nfile path:%s\n", prv->previewitem[i].info.file_path);

            len = strlen(filename.name) + 1;
            prv->previewitem[i].info.file_name = (char *) malloc(len);
            if (NULL == prv->previewitem[i].info.file_name)
            {
                print_err("malloc fail");
                return false;
            }
            sprintf(prv->previewitem[i].info.file_name, "%s", filename.name);
        }
    }

    return true;
}

/*
 * 回到第一页
 */
static bool _to_first_page(preview_thumbnail_t *prv, bool keepx)
{
    bool one_page = false;
    int total = prv->total_file_num + prv->total_dir_num;
    int max_per_screen = prv->numx * prv->numy;
    int x = 0, y = 0;

    if (total <= max_per_screen)
    {
        one_page = true;
    }

    prv->item_per_screen = (true == one_page) ? total : max_per_screen;
    prv->cur_level_info->start_index = 0;

    if(true == keepx)
    {
        x = prv->cur_level_info->cur_index % prv->numx;
    }

    /* 计算当前index */
    prv->cur_level_info->cur_index = y * prv->numx + x;
    while(prv->cur_level_info->cur_index >= prv->item_per_screen)
    {
        prv->cur_level_info->cur_index -= prv->numx;
    }

    if(prv->cur_level_info->cur_index < 0)
    {
        prv->cur_level_info->cur_index = 0;
    }

    if (!one_page) /* 只有总数至少两屏时，才重新取出一屏数据及解码 */
    {
        _get_items(prv, 0, prv->item_per_screen);
        prv->dec_index = _get_next_not_dec_index(prv, -1);
        sty_draw_picture_res(gui_dc_get((unsigned short) g_preview->win_id), g_preview->pic_bg);
        if (prv->dec_index >= 0)
        {
            _dec_current_pic(prv);
        }
    }

    prv->slider->init.page_index = 1;
    photo_preview_widget_thumbnail_paint(prv);

    return true;
}

/*
 * 回到最后一页
 */
static bool _to_last_page(preview_thumbnail_t *prv, bool keepx)
{
    bool one_page = false;
    int total = prv->total_file_num + prv->total_dir_num;
    int max_per_screen = prv->numx * prv->numy;
    int i;
    int x, y;

    if (total <= max_per_screen)
    {
        one_page = true;
    }

    if (true == one_page)
    {
        prv->item_per_screen = total;
    }
    else
    {
        prv->item_per_screen = max_per_screen - (prv->numx - (total % prv->numx));
    }

    y = prv->item_per_screen/prv->numx;
    if(true == keepx)
    {
        x = prv->cur_level_info->cur_index % prv->numx;
    }
    else
    {
        x = (prv->item_per_screen - 1) % prv->numx;
    }

    print_dbg("item_per_screen:%d, x:%d, y:%d, keepx:%d", prv->item_per_screen, x, y, keepx);

    prv->cur_level_info->start_index = (true == one_page) ? 0 : (total - prv->item_per_screen);

    /* 计算当前index */
    prv->cur_level_info->cur_index = y * prv->numx + x;
    while(prv->cur_level_info->cur_index >= prv->item_per_screen)
    {
        prv->cur_level_info->cur_index -= prv->numx;
    }

    if(prv->cur_level_info->cur_index < 0)
    {
        prv->cur_level_info->cur_index = 0;
    }

    if (!one_page) /* 只有总数至少两屏时，才重新取出一屏数据及解码 */
    {
        _get_items(prv, 0, prv->item_per_screen);
        for (i = prv->item_per_screen; i < max_per_screen; i++)
        {
            prv->previewitem[i].info.dec_status = NOT_DECODE;
            if (NULL != prv->previewitem[i].info.file_path)
            {
                free(prv->previewitem[i].info.file_path);
            }
            if (NULL != prv->previewitem[i].info.file_name)
            {
                free(prv->previewitem[i].info.file_name);
            }
            prv->previewitem[i].info.file_path = NULL;
            prv->previewitem[i].info.file_name = NULL;
            prv->previewitem[i].is_dir = false;
        }
        prv->dec_index = _get_next_not_dec_index(prv, -1);
        sty_draw_picture_res(gui_dc_get((unsigned short) g_preview->win_id), g_preview->pic_bg);
        if (prv->dec_index >= 0)
        {
            _dec_current_pic(prv);
        }
    }

    prv->slider->init.page_index = (prv->cur_level_info->start_index / prv->numx) + 1;
    photo_preview_widget_thumbnail_paint(prv);

    return true;
}

/*
 * item往下移一行，前面文件补上
 */
static bool _item_move_down(preview_thumbnail_t *prv, bool keepx)
{
    int i, max_per_screen, get_num;
    thumbnail_item_t *tmp_items;

    prv->cur_level_info->start_index -= prv->numx;
    if (prv->cur_level_info->start_index < 0)
    {
        /* 已经移到最前面一行，前面没有文件补充了*/
        return _to_last_page(prv, keepx);
    }

    tmp_items = (thumbnail_item_t *) malloc(sizeof(thumbnail_item_t) * (unsigned int) prv->numx);
    if (NULL == tmp_items)
    {
        print_err("malloc error!");
        return false;
    }
    max_per_screen = prv->numx * prv->numy;

    for (i = 0; i < prv->numx; i++)
    {
        tmp_items[i] = prv->previewitem[(max_per_screen - prv->numx) + i];
    }

    for (i = max_per_screen - 1; i >= prv->numx; i--)
    {
        prv->previewitem[i] = prv->previewitem[i - prv->numx];
    }

    for (i = 0; i < prv->numx; i++)
    {
        prv->previewitem[i] = tmp_items[i];
        prv->previewitem[i].info.dec_status = NOT_DECODE;
    }

    /* 前面总是能够补上prv->numx个文件，所以移动之后总是满屏 */
    prv->item_per_screen = max_per_screen;
    get_num = prv->numx;
    _get_items(prv, 0, get_num);

    prv->dec_index = _get_next_not_dec_index(prv, -1);
    if (prv->dec_index >= 0)
    {
        _dec_current_pic(prv);
    }
    prv->slider->init.page_index = (prv->cur_level_info->start_index / prv->numx) + 1;
    photo_preview_widget_thumbnail_paint(prv);
    free(tmp_items);

    return true;
}

/*
 * item往上移一行，后面文件补上
 */
static bool _item_move_up(preview_thumbnail_t *prv, bool keepx)
{
    int i, total, get_num, max_per_screen;
    thumbnail_item_t *tmp_items;
    int temp;

    total = prv->total_file_num + prv->total_dir_num;
    max_per_screen = prv->numx * prv->numy;
    temp = prv->cur_level_info->start_index + prv->item_per_screen;
    if ((temp >= total) || ((true == keepx) && ((temp + prv->numx) >= total)))
    {
        /* 已经移到最后面一行，后面没有文件补充了 */
        return _to_first_page(prv, keepx);
    }
    prv->cur_level_info->start_index += prv->numx;

    tmp_items = (thumbnail_item_t *) malloc(sizeof(thumbnail_item_t) * (unsigned int) prv->numx);
    if (NULL == tmp_items)
    {
        print_err("malloc fail");
        return false;
    }
    for (i = 0; i < prv->numx; i++)
    {
        tmp_items[i] = prv->previewitem[i];
    }

    for (i = 0; i < (max_per_screen - prv->numx); i++)
    {
        prv->previewitem[i] = prv->previewitem[i + prv->numx];
    }

    for (i = 0; i < prv->numx; i++)
    {
        prv->previewitem[(max_per_screen - prv->numx) + i] = tmp_items[i];
        prv->previewitem[(max_per_screen - prv->numx) + i].info.dec_status = NOT_DECODE;
    }

    /* 计算当前页文件个数 */
    if ((prv->cur_level_info->start_index + max_per_screen) > total)
    {
        prv->item_per_screen = total - prv->cur_level_info->start_index;
    }
    else
    {
        prv->item_per_screen = max_per_screen;
    }

    get_num = prv->numx - (max_per_screen - prv->item_per_screen);
    _get_items(prv, max_per_screen - prv->numx, get_num);

    prv->dec_index = _get_next_not_dec_index(prv, max_per_screen - prv->numx - 1);

    if (prv->dec_index >= 0)
    {
        _dec_current_pic(prv);
    }
    prv->slider->init.page_index = (prv->cur_level_info->start_index / prv->numx) + 1;
    photo_preview_widget_thumbnail_paint(prv);
    free(tmp_items);

    return true;
}

static void _thumbnail_move_sel(preview_thumbnail_t *prv, move_dir_e dir)
{
    int old_id;

    if ((prv->total_file_num + prv->total_dir_num) <= 0)
    {
        return;
    }

    old_id = prv->cur_level_info->cur_index;

    switch (dir)
    {
    case MOVE_LEFT:
        prv->cur_level_info->cur_index--;
        if (prv->cur_level_info->cur_index < 0)
        {
            prv->cur_level_info->cur_index = old_id + (prv->numx - 1);
            _item_move_down(prv, false);
        }
        break;

    case MOVE_RIGHT:
        prv->cur_level_info->cur_index++;
        if (prv->cur_level_info->cur_index >= prv->item_per_screen)
        {
            prv->cur_level_info->cur_index = old_id - (prv->numx - 1);
            _item_move_up(prv, false);
        }
        break;

    case MOVE_UP:
        prv->cur_level_info->cur_index -= prv->numx;
        if (prv->cur_level_info->cur_index < 0)
        {
            prv->cur_level_info->cur_index = old_id;
            _item_move_down(prv, true);
        }
        break;

    case MOVE_DOWN:
        prv->cur_level_info->cur_index += prv->numx;
        if (prv->cur_level_info->cur_index >= prv->item_per_screen)
        {
            prv->cur_level_info->cur_index = old_id;
            _item_move_up(prv, true);
        }
        break;

    default:
        break;
    }

    _draw_thumbnail_item(prv, old_id % prv->numx, old_id / prv->numx, false);
    _draw_thumbnail_item(prv, prv->cur_level_info->cur_index % prv->numx,
            prv->cur_level_info->cur_index / prv->numx, true);
}

static bool _init_cur_dir_items(preview_thumbnail_t *prv)
{
    int total, i, max_screen, start_index;

    prv->total_file_num = fsel_getfilenum(prv->fsel, FSEL_FILENUM);
    prv->total_dir_num = fsel_getfilenum(prv->fsel, FSEL_DIRNUM);

    total = prv->total_file_num + prv->total_dir_num;
    if (0 == total)
    {
        /* 显示无文件对话框  */
        photo_preview_show_warning(g_preview->str_no_file);
    }

    max_screen = prv->numx * prv->numy;
    start_index = prv->cur_level_info->start_index;
    prv->item_per_screen = ((total - start_index) > max_screen) ?
                                    max_screen : (total - start_index);
    _get_items(prv, 0, prv->item_per_screen);
    for (i = prv->item_per_screen; i < (prv->numx * prv->numy); i++)
    {
        prv->previewitem[i].info.dec_status = NOT_DECODE;
        if (NULL != prv->previewitem[i].info.file_path)
        {
            free(prv->previewitem[i].info.file_path);
        }
        if (NULL != prv->previewitem[i].info.file_name)
        {
            free(prv->previewitem[i].info.file_name);
        }
        prv->previewitem[i].info.file_path = NULL;
        prv->previewitem[i].info.file_name = NULL;
        prv->previewitem[i].is_dir = false;
    }
    prv->dec_index = _get_next_not_dec_index(prv, -1);
    return true;
}

/*
 * 返回上级目录
 */
static bool _cd_parent(preview_thumbnail_t *prv)
{
    level_info_t *level_info;
    char *str_tmp;
    int total, len, pos, temp;
    char curname[PATH_MAX];

    if (NULL == prv->fsel)
    {
        return false;
    }

    if (prv->cur_level_info->prev == NULL) /* 没有上级目录了 */
    {
        return false;
    }
    
    /* 获取当前所在目录名 */
    memset(curname, 0, sizeof(curname));
    str_tmp = (char *)fsel_getworkdir(prv->fsel);
    if((str_tmp == NULL) || (strlen(str_tmp) < 2))
    {
        return false;
    }
    
    len = (int)strlen(str_tmp);
    if(str_tmp[len-1] == '/')
    {
        len --;     
    }
    assert(len < PATH_MAX);
    memcpy(curname, str_tmp, (unsigned int)len);

    str_tmp = "..";
    if (fsel_cd(prv->fsel, str_tmp) < 0)
    {
        return false;
    }

    level_info = prv->cur_level_info->prev;
    free(prv->cur_level_info);
    prv->cur_level_info = level_info;
    
    /* 从照相机预览图片的路径目录等级链表，父亲节点index为-1 */
    if(level_info->start_index == -1)
    {
        pos = fsel_getidbyname(prv->fsel, curname) - 1;
        if(pos < 0)
        {
            pos = 0;
        }
        
        if(pos < (prv->numx * prv->numy))
        {
            level_info->start_index = 0;
        }
        else
        {
            /* 起始行号 */
            temp = ((pos / prv->numx) - prv->numy) + 1;
            level_info->start_index = temp * prv->numx;
        }
        level_info->cur_index = pos - level_info->start_index;
    }

    _init_cur_dir_items(prv);

    if (prv->dec_index >= 0)
    {
        prv->dec_index--;
        _dec_next_pic(prv);
    }
    total = prv->total_dir_num + prv->total_file_num;
    prv->slider->init.total_item = ((int) total + (prv->numx - 1)) / prv->numx; /* 计算行数 */
    prv->slider->init.page_index = (prv->cur_level_info->start_index / prv->numx) + 1;
    photo_preview_widget_thumbnail_paint(prv);
    return true;
}

/*
 * 进入下级目录
 */
static bool _cd_child(preview_thumbnail_t *prv, const char *childname)
{
    level_info_t *level_info;
    char *str_tmp;
    int total;

    if (NULL == prv->fsel)
    {
        return false;
    }

    if (fsel_cd(prv->fsel, (char *) childname) < 0)
    {
        return false;
    }

    level_info = (level_info_t *) malloc(sizeof(level_info_t));
    if (NULL == level_info)
    {
        print_err("malloc fail");
        return false;
    }
    level_info->cur_index = 0;
    level_info->start_index = 0;
    level_info->prev = prv->cur_level_info;
    prv->cur_level_info = level_info;

    if (!_init_cur_dir_items(prv))
    {
        str_tmp = "..";
        fsel_cd(prv->fsel, str_tmp);
        return false;
    }

    if (prv->dec_index >= 0)
    {
        prv->dec_index--;
        _dec_next_pic(prv);
    }
    total = prv->total_dir_num + prv->total_file_num;
    prv->slider->init.total_item = ((int) total + (prv->numx - 1)) / prv->numx; /* 计算行数 */
    prv->slider->init.page_index = (prv->cur_level_info->start_index / prv->numx) + 1;
    photo_preview_widget_thumbnail_paint(prv);
    return true;
}

/*
 * 建立从根目录到当前目录的目录等级链表
 */
static bool _init_root_level_info(preview_thumbnail_t *prv)
{
    size_t len1, len2, pos1, pos2;
    char *rootpath, *str_tmp;
    char workdir[MAX_PATH_LENGTH];
    char *strname = NULL;
    bool first;
    level_info_t *level_info = NULL;

    if((NULL == g_preview->init_sel_path) || (PATH_PLAYLIST == g_preview->path_type)
            || (NULL == prv->fsel))
    {
        return false;
    }

    rootpath = get_root_path(g_preview->init_sel_path);
    if(NULL == rootpath)
    {
        return false;
    }

    str_tmp = (char *)fsel_getworkdir(prv->fsel);
    if(NULL == str_tmp)
    {
        print_err();
        return false;
    }
    strcpy(workdir, str_tmp);

    len1 = strlen(rootpath);
    len2 = strlen(g_preview->init_sel_path);
    if(len1 == len2)
    {
        return false;
    }

    str_tmp = g_preview->init_sel_path;
    pos1 = len1;
    if(str_tmp[pos1] == '/')
    {
        pos1 ++;
    }
    strname = (char *)malloc(MAX_PATH_LENGTH);
    if(NULL == strname)
    {
        print_err("malloc fail");
        goto err;
    }

    prv->cur_level_info = (level_info_t *)malloc(sizeof(level_info_t));
    if(NULL == prv->cur_level_info)
    {
        print_err("malloc fail");
        goto err;
    }

    prv->cur_level_info->prev = NULL;

//    fsel_setworkdir(prv->fsel, rootpath);
    pos2 = pos1;
    first = true;
    while(pos2 < len2)
    {       
        if(str_tmp[pos2] == '/')
        {
            prv->cur_level_info->start_index = -1;
            prv->cur_level_info->cur_index = -1;
        
            level_info = (level_info_t *) malloc(sizeof(level_info_t));
            if(NULL == level_info)
            {
                print_err("malloc fail");
                goto err;
            }
            level_info->prev = prv->cur_level_info;
            level_info->start_index = 0;
            level_info->cur_index = 0;
            prv->cur_level_info = level_info;

            pos1 = pos2 + 1;
        }
        pos2 ++;
    }

    free(strname);
//    fsel_setworkdir(prv->fsel, workdir);
    return true;

    err:
    if(NULL != strname)
    {
        free(strname);
    }
    return false;
}


/*
 * 初始化目录等级信息链表（或者状态恢复）
 */
static bool _init_level_info(preview_thumbnail_t *prv)
{
    int temp, init_index, max_per_screen, total;
    int start_index, cur_index;
    level_info_t *level_info_saved;

    if (NULL == g_preview->init_sel_path)
    {
        init_index = 0;
    }
    else if (PATH_PLAYLIST == g_preview->path_type)
    {
        init_index = photo_playlist_get_file_index(g_preview->init_sel_path);
    }
    else
    {
        init_index = fsel_getidbyname(prv->fsel, g_preview->init_sel_path) - 1;
    }

    if (init_index < 0)
    {
        init_index = 0;
    }
    max_per_screen = prv->numx * prv->numy;
    level_info_saved = photo_level_info_restore();
    if (NULL != level_info_saved)
    {
        /* 恢复状态 */
        total = prv->total_dir_num + prv->total_file_num;
        start_index = level_info_saved->start_index;
        cur_index = level_info_saved->cur_index;
        if ((start_index + cur_index) != init_index)
        {
            /* 文件index已经有变动, 重新计算初始index */
            if ((init_index < start_index) || (init_index >= (start_index + max_per_screen)))
            {
                temp = ((init_index / prv->numx) - prv->numy) + 1;
                temp = (temp < 0) ? 0 : temp;
                level_info_saved->start_index = temp * prv->numx;
            }
        }
        else
        {
            /* 防止旋转屏 */
            while (init_index >= (start_index + max_per_screen))
            {
                start_index += prv->numx;
            }
            level_info_saved->start_index = start_index;
        }
        level_info_saved->cur_index = init_index - level_info_saved->start_index;
        prv->cur_level_info = level_info_saved;
        return true;
    }

    if(!_init_root_level_info(prv))
    {
        prv->cur_level_info = (level_info_t *) malloc(sizeof(level_info_t));
        if (NULL == prv->cur_level_info)
        {
            print_err("malloc fail");
            return false;
        }
        prv->cur_level_info->prev = NULL;
    }

    /* 起始行号 */
    temp = ((init_index / prv->numx) - prv->numy) + 1;
    temp = (temp < 0) ? 0 : temp;
    prv->cur_level_info->start_index = temp * prv->numx;
    prv->cur_level_info->cur_index = init_index - prv->cur_level_info->start_index;

    return true;
}

static bool _data_init(preview_thumbnail_t *prv)
{
    int i;
    picregion_frame_t picframe;
    unsigned int width, height, max_per_screen;

    /* 根据sty文件设置图片、边框等位置信息，计算水平、垂直方向的预览图片个数 */
    prv->first_x = g_preview->resg_pic->x;
    prv->first_y = g_preview->resg_pic->y;
    prv->first_frame_x = g_preview->picreg_sel00->x;
    prv->first_frame_y = g_preview->picreg_sel00->y;
    prv->dist_x = g_preview->picreg_sel01->x - g_preview->picreg_sel00->x;
    prv->dist_y = g_preview->picreg_sel10->y - g_preview->picreg_sel00->y;
    prv->numx = (g_preview->scene->width - prv->first_x) / (g_preview->picreg_sel00->width);/*TODO 计算个数 */
    prv->numy = (g_preview->scene->height - prv->first_y) / (g_preview->picreg_sel00->height);
    prv->previewitem = (thumbnail_item_t *) malloc(sizeof(thumbnail_item_t) * (unsigned int) prv->numx
            * (unsigned int) prv->numy);
    if (NULL == prv->previewitem)
    {
        print_err("malloc fail");
        return false;
    }

    max_per_screen = (unsigned int) (prv->numx * prv->numy);
    width = (unsigned int) g_preview->resg_pic->width;
    height = (unsigned int) g_preview->resg_pic->height;
    for (i = 0; i < (int) max_per_screen; i++)
    {
        prv->previewitem[i].info.dec_info.bpp = 2;
        prv->previewitem[i].info.dec_info.formate = RGB_565;
        prv->previewitem[i].info.dec_info.len = width * height * 2;
        prv->previewitem[i].info.dec_info.buf = (unsigned char *) malloc(width * height * 2);
        if (NULL == prv->previewitem[i].info.dec_info.buf)
        {
            print_err("malloc fail");
            return false;
        }
        prv->previewitem[i].info.dec_info.rect_width = width;
        prv->previewitem[i].info.dec_info.rect_height = height;
        prv->previewitem[i].info.dec_status = NOT_DECODE;
        prv->previewitem[i].info.file_path = NULL;
        prv->previewitem[i].info.file_name = NULL;
        prv->previewitem[i].is_dir = false;
    }

    /* 获取选择/非选择框的位图 */
    get_picregion_frame(g_preview->picreg_sel00, 0, &picframe);
    prv->bmp_unsel = sty_create_bitmap((int) picframe.pic_id);

    get_picregion_frame(g_preview->picreg_sel00, 1, &picframe);
    prv->bmp_sel = sty_create_bitmap((int) picframe.pic_id);
    return true;
}

static int _thumbnail_init(preview_thumbnail_t *prv)
{
    int temp;
    int total, max_per_screen;
    slider_init_t slider_init;
    char *str_tmp, pdir[MAX_PATH_LENGTH];
    char ext[CFG_MAX_BUFF_LENGTH];

    if(get_config(CFG_APP_SUPPORT_EXT_PHOTO, ext, CFG_MAX_BUFF_LENGTH) < 0)
    {
        print_err();
        return false;
    }

    memset(prv, 0, sizeof(preview_thumbnail_t));

    if (g_preview->path_type != PATH_PLAYLIST)
    {
        prv->fsel = fsel_open();
        if (NULL == prv->fsel)
        {
            print_err("fsel open failure!\n");
            return -1;
        }
    }

    _data_init(prv);

    max_per_screen = prv->numx * prv->numy;

    /* 根据存储位置不同设定工作路径  */
    if (PATH_PLAYLIST == g_preview->path_type)
    {
        prv->total_file_num = photo_playlist_get_num();
        prv->total_dir_num = 0;
    }
    else
    {
        if (!strcmp(g_preview->init_sel_path, g_preview->top_dir))
        {
            strcpy(pdir, g_preview->init_sel_path);
        }
        else
        {
            if (false == get_parent_directory(pdir, MAX_PATH_LENGTH, g_preview->init_sel_path))
            {
                return false;
            }
        }
        str_tmp = pdir;
        print_dbg("\nworkdir: %s\n", pdir);

        fsel_setworkdir(prv->fsel, str_tmp);
        fsel_setfilter(prv->fsel, ext);
        fsel_sortstyle(prv->fsel, FSEL_PINYIN);
        prv->total_file_num = fsel_getfilenum(prv->fsel, FSEL_FILENUM);
        prv->total_dir_num = fsel_getfilenum(prv->fsel, FSEL_DIRNUM);
        if (prv->total_file_num == -1)
        {
            prv->total_file_num = 0;
        }
        if (prv->total_dir_num == -1)
        {
            prv->total_dir_num = 0;
        }
    }

    total = prv->total_file_num + prv->total_dir_num;
    if (0 == total)
    {
        /* 显示无文件对话框  */
        photo_preview_show_warning(g_preview->str_no_file);
    }

    if (false == _init_level_info(prv))
    {
        print_warning("");
        return -2;
    }
    if ((prv->cur_level_info->start_index + max_per_screen) <= total)
    {
        prv->item_per_screen = max_per_screen;
    }
    else
    {
        prv->item_per_screen = total - prv->cur_level_info->start_index;
    }

    prv->slider = slider_create(W_SB_TYPE_LIST, _slider_cb, -1);
    if (NULL == prv->slider)
    {
        print_err("");
        return -3;
    }
    slider_init.item_perpage = prv->numy;
    temp = prv->cur_level_info->start_index / prv->numx;
    slider_init.page_index = temp + 1;
    slider_init.total_item = (total + (prv->numx - 1)) / prv->numx; /* 计算行数 */
    slider_set_parameter(prv->slider, &slider_init);

    _get_items(prv, 0, prv->item_per_screen);
    prv->dec_index = _get_next_not_dec_index(prv, -1);

    return 0;
}

static int _data_deinit(preview_thumbnail_t *prv)
{
    int i;

    if (-1 != g_preview->dec_single_timer_id)
    {
        kill_timer(g_preview->dec_single_timer_id);
        g_preview->dec_single_timer_id = -1;
    }

    for (i = 0; i < (prv->numx * prv->numy); i++)
    {
        if (NULL != prv->previewitem[i].info.dec_info.buf)
        {
            free(prv->previewitem[i].info.dec_info.buf);
        }

        if (NULL != prv->previewitem[i].info.file_path)
        {
            free(prv->previewitem[i].info.file_path);
        }

        if (NULL != prv->previewitem[i].info.file_name)
        {
            free(prv->previewitem[i].info.file_name);
        }
    }

    if (NULL != prv->bmp_sel)
    {
        sty_delete_bitmap(prv->bmp_sel);
    }

    if (NULL != prv->bmp_unsel)
    {
        sty_delete_bitmap(prv->bmp_unsel);
    }
    return 0;
}

static int _thumbnail_deinit(preview_thumbnail_t *prv)
{
    photo_dec_control(ABORT_DECODE, 0);

    if (NULL == prv)
    {
        return 0;
    }

    if (NULL != prv->slider)
    {
        slider_delete(prv->slider);
    }

    if (NULL != prv->fsel)
    {
        fsel_close(prv->fsel);
    }

    _data_deinit(prv);

    if (g_preview->exit_type == SCENE_PREVIEW_NEXT)
    {
        photo_level_info_save(prv->cur_level_info);
    }
    else
    {
        photo_level_info_delete(prv->cur_level_info);
        prv->cur_level_info = NULL;
    }

    free(prv->previewitem);
    free(prv);
    s_thumbnail = NULL;
    return 0;
}

static void _draw_thumbnail_item(preview_thumbnail_t *prv, int idx, int idy, bool sel)
{
    int x, y, id;
    unsigned int temp;
    dec_file_info_t *info;
    picregion_frame_t frame;
    int hdc;

    print_dbg();
    hdc = gui_dc_get((unsigned short) g_preview->win_id);

    /* TODO 删除调试 */
#if 1
    x = prv->first_frame_x + (idx * prv->dist_x);
    y = prv->first_frame_y + (idy * prv->dist_y);

    gui_dc_clear_rect(hdc, x, y, x + prv->bmp_sel->xsize, y + prv->bmp_sel->ysize);
#endif

    x = prv->first_x + (idx * prv->dist_x);
    y = prv->first_y + (idy * prv->dist_y);

    id = (idy * prv->numx) + idx;
    info = &prv->previewitem[id].info;
    if (true == sel)
    {
        headbar_set_title_buffer(g_preview->hbar, info->file_name);
        if (-1 != g_preview->show_title_timer_id)
        {
            restart_timer(g_preview->show_title_timer_id);
        }
        else
        {
            g_preview->show_title_timer_id = set_timer(3 * 1000, _show_title_timer, g_preview);
        }
    }
    if (info->dec_status == DECODE_FINISHED) /* 成功解码完 */
    {
        temp = info->dec_info.rect_width - info->dec_info.img_width;
        x += (int) (temp >> 1);
        temp = info->dec_info.rect_height - info->dec_info.img_height;
        y += (int) (temp >> 1);
        gui_dc_draw_bitmap_ext(hdc, info->dec_info.buf, x, y, (int) info->dec_info.img_width,
                (int) info->dec_info.img_height, (int) info->dec_info.bpp);

        x = prv->first_frame_x + (idx * prv->dist_x);
        y = prv->first_frame_y + (idy * prv->dist_y);
        if (true == sel)
        {
            sty_draw_bitmap(hdc, prv->bmp_sel, x, y);
        }
        else
        {
            sty_draw_bitmap(hdc, prv->bmp_unsel, x, y);
        }
    }
    else if ((info->dec_status == NOT_DECODE) || (info->dec_status == DECODING)
            || (info->dec_status == DECODE_ERROR)) /* 未解码完或目录或解码错误 */
    {
        x = prv->first_frame_x + (idx * prv->dist_x);
        y = prv->first_frame_y + (idy * prv->dist_y);
        if (true == prv->previewitem[id].is_dir)
        {
            if (true == sel)
            {
                get_picregion_frame(g_preview->picreg_folder, 1, &frame);
            }
            else
            {
                get_picregion_frame(g_preview->picreg_folder, 0, &frame);
            }
        }
        else
        {
            if (true == sel)
            {
                /*sty_draw_bitmap(hdc, prv->bmp_sel, x, y);*/
                get_picregion_frame(g_preview->picreg_err, 1, &frame);
            }
            else
            {
                /*sty_draw_bitmap(hdc, prv->bmp_unsel, x, y);*/
                get_picregion_frame(g_preview->picreg_err, 0, &frame);
            }
        }

        /* 没有图片的格子显示为空 */
        if (((idy * prv->numx) + idx) >= prv->item_per_screen)
        {
            sty_draw_bitmap(hdc, prv->bmp_unsel, x, y);
        }
        else
        {
            sty_draw_picture_id(hdc, (int) frame.pic_id, x, y);
        }
    }
    else
    {
        x = prv->first_frame_x + (idx * prv->dist_x);
        y = prv->first_frame_y + (idy * prv->dist_y);
        if (true == sel)
        {
            get_picregion_frame(g_preview->picreg_err, 1, &frame);
        }
        else
        {
            get_picregion_frame(g_preview->picreg_err, 0, &frame);
        }
        sty_draw_picture_id(hdc, (int) frame.pic_id, x, y);
    }

    print_dbg();
}

static bool _dec_current_pic(preview_thumbnail_t *prv)
{
    if ((prv->total_dir_num + prv->total_file_num) == 0)
    {
        return true;
    }

    assert(prv->dec_index >= 0);

    print_dbg();
    photo_dec_control(ABORT_DECODE, 0);

    if (!photo_dec_control(SET_FILE, (unsigned int) prv->previewitem[prv->dec_index].info.file_path))
    {
        print_warning("set file err: %s", prv->previewitem[prv->dec_index].info.file_path);
        goto err;
    }

    if (!photo_dec_control(GET_MEDIA_INFO, (unsigned int) &prv->previewitem[prv->dec_index].info.file_info))
    {
        print_warning("get media info err");
        goto err;
    }
    else
    {
        print_info("width = %d,height=%d,format=%s\n",
                prv->previewitem[prv->dec_index].info.file_info.width,
                prv->previewitem[prv->dec_index].info.file_info.height,
                prv->previewitem[prv->dec_index].info.file_info.file_format);
    }

    if (!photo_dec_control(SET_CALLBACK, (unsigned int) _thumbnail_dec_cb))
    {
        print_warning("set callback err");
        goto err;
    }

    if (!photo_dec_control(IMG_BROWSE, (unsigned int) &prv->previewitem[prv->dec_index].info.dec_info))
    {
        print_warning("decode image err");
        goto err;
    }

    print_dbg();
    prv->previewitem[prv->dec_index].info.dec_status = DECODING;
    return true;

    err:
    print_dbg();
    prv->previewitem[prv->dec_index].info.dec_status = DECODE_ERROR;
    return false;
}

static bool _dec_next_pic(preview_thumbnail_t *prv)
{
    int idx;
    bool ret = false;

    photo_dec_control(ABORT_DECODE, 0);

    /* 解码下一幅图片，直到有图片setfile成功，或者到达当前屏幕的最后一个文件 */
    idx = prv->dec_index;
    do
    {
        idx = _get_next_not_dec_index(prv, idx);
        if (idx >= 0)
        {
            prv->dec_index = idx;
            ret = _dec_current_pic(prv);
        }
    } while ((idx >= 0) && (false == ret));

    return ret;
}

void photo_preview_widget_thumbnail_paint(preview_thumbnail_t *prv)
{
    int i, j, temp;

    for (i = 0; i < prv->numy; i++)
    {
        for (j = 0; j < prv->numx; j++)
        {
            temp = (i * prv->numx) + j;
            if (temp == prv->cur_level_info->cur_index)
            {
                _draw_thumbnail_item(prv, j, i, true);
            }
            else
            {
                _draw_thumbnail_item(prv, j, i, false);
            }
        }
    }

    slider_paint(prv->slider);
}

void photo_preview_widget_thumbnail_rotate(preview_thumbnail_t *prv)
{
    int temp, init_index, start_index;
    int total, max_per_screen;

    photo_dec_control(ABORT_DECODE, 0);

    _data_deinit(prv);
    slider_load_style(prv->slider);
    _data_init(prv);

    max_per_screen = prv->numx * prv->numy;
    total = prv->total_file_num + prv->total_dir_num;

    init_index = prv->cur_level_info->start_index + prv->cur_level_info->cur_index;
    start_index = (prv->cur_level_info->start_index / prv->numx) * prv->numx;
    while (init_index >= (start_index + max_per_screen))
    {
        start_index += prv->numx;
    }
    prv->cur_level_info->start_index = start_index;
    prv->cur_level_info->cur_index = init_index - start_index;

    if ((prv->cur_level_info->start_index + max_per_screen) <= total)
    {
        prv->item_per_screen = max_per_screen;
    }
    else
    {
        prv->item_per_screen = total - prv->cur_level_info->start_index;
    }

    prv->slider->init.item_perpage = prv->numy;
    temp = prv->cur_level_info->start_index / prv->numx;
    prv->slider->init.page_index = temp + 1;
    prv->slider->init.total_item = (total + (prv->numx - 1)) / prv->numx; /* 计算行数 */

    _get_items(prv, 0, prv->item_per_screen);
    prv->dec_index = -1;
    _dec_next_pic(prv);
}

preview_thumbnail_t *photo_preview_widget_thumbnail_create(preview_thumbnail_cb_t callback)
{
    preview_thumbnail_t *prv;

    prv = (preview_thumbnail_t *) malloc(sizeof(preview_thumbnail_t));
    if (NULL == prv)
    {
        print_err("allocate error!\n");
        return NULL;
    }

    s_thumbnail = prv;

    if (_thumbnail_init(prv) < 0)
    {
        print_warning("init err");
        _thumbnail_deinit(prv);
        return NULL;
    }

    if (prv->total_file_num > 0)
    {
        prv->dec_index = -1;
        _dec_next_pic(prv);
    }

    prv->callback = callback;
    return prv;
}

bool photo_preview_widget_thumbnail_delete(preview_thumbnail_t *prv)
{
    if (NULL != prv)
    {
        _thumbnail_deinit(prv);
    }
    return true;
}

bool photo_preview_widget_thumbnail_proc_gui(preview_thumbnail_t *prv, gui_wm_message_t *pmsg)
{
    key_event_t *kmsg;
    thumbnail_item_t *item;
    int idx;
    static int last_key = 0;

    switch (pmsg->msgid)
    {
    case WM_MSG_KEY:
        kmsg = (key_event_t *) (pmsg->data.p);

        switch (kmsg->val | kmsg->type)
        {
        case KEY_NEXT | KEY_TYPE_LONG:
        case KEY_NEXT | KEY_TYPE_HOLD:
            _thumbnail_move_sel(prv, MOVE_DOWN);
            clear_key_msg_queue(-1, KEY_TYPE_HOLD);
            break;

        case KEY_PREV | KEY_TYPE_LONG:
        case KEY_PREV | KEY_TYPE_HOLD:
            _thumbnail_move_sel(prv, MOVE_UP);
            clear_key_msg_queue(-1, KEY_TYPE_HOLD);
            break;

        case KEY_PREV | KEY_TYPE_SHORT_UP:
            if (last_key == (KEY_PREV | KEY_TYPE_DOWN))
            {
                _thumbnail_move_sel(prv, MOVE_LEFT);
            }
            break;

        case KEY_NEXT | KEY_TYPE_SHORT_UP:
            if (last_key == (KEY_NEXT | KEY_TYPE_DOWN))
            {
                _thumbnail_move_sel(prv, MOVE_RIGHT);
            }
            break;

        case KEY_OK | KEY_TYPE_SHORT_UP:
            if((prv->total_dir_num + prv->total_file_num) == 0)
            {
                break;
            }

            item = &prv->previewitem[prv->cur_level_info->cur_index];
            if (true == item->is_dir)
            {
                _cd_child(prv, item->info.file_name); /* 进入下一级目录 */
            }
            else
            {

                if (g_preview->path_type != PATH_PLAYLIST)
                {
                    photo_playlist_del_items(0, -1);
                    if(!photo_playlist_add_dir(fsel_getworkdir(prv->fsel)))
                    {
                        photo_preview_show_warning(g_preview->str_plist_error);
                        break;
                    }
                }
                idx = photo_playlist_get_file_index(item->info.file_path);
                set_config(CFG_PHOTO_CURRENT_INDEX, (const char *) &idx, sizeof(int));
                prv->callback(prv, WIDGET_SIG_PREVIEW_OK);
            }

            break;

        case KEY_RETURN | KEY_TYPE_SHORT_UP:
            if (!_cd_parent(prv))
            {
                prv->callback(prv, WIDGET_SIG_PREVIEW_QUIT);
            }
            break;

        default:
            break;
        }
        last_key = (kmsg->val | kmsg->type);
        break;

    default:
        break;
    }

    return true;
}

