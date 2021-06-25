/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_option_setbg.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason    2009-06-14         v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_option_setbg.c
 * \brief    set background of photo playing option
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/06/14
 *******************************************************************************/

#include "photo_scene_playing.h"
#include "photo_option.h"
#include "photo_option_setbg.h"
#include "photo_dec_ctrl.h"
#include "photo_option_widget_menu.h"

typedef struct
{
    unsigned short type;
    unsigned long size;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned long offBits;
}__attribute__ ((packed)) bitmap_file_header;

typedef struct
{
    unsigned long size;
    long width;
    long height;
    unsigned short planes;
    unsigned short bit_count;
    unsigned long compression;
    unsigned long size_image;
    long x_pixels_per_meter;
    long y_pixels_per_meter;
    unsigned long color_used;
    unsigned long color_important;
}__attribute__ ((packed)) bitmap_info_header;

typedef struct
{
    bitmap_info_header bh;
    unsigned long bmi_colors[4];
}__attribute__ ((packed)) bitmap_info;

static bool _save_bmp_logo(scene_playing_t* p, const char *path);
static bool _save_gif_animation(scene_playing_t* p, const char *path);
static int _proc_item(menuview_t *menuview, menuview_sig_e signal);
static void _fill_items(menu_t *menu, scene_playing_t *p);
static void _setbg_init(void);
static void _setbg_deinit(void);

/* menu list data define */
static menu_t *s_setbg = NULL;

/*
 * 静态图保存为bmp
 */
static bool _save_bmp_logo(scene_playing_t* p, const char *path)
{
    char *line_buff = NULL;
    int i;
    unsigned int dx, dy, width, height;
    unsigned int line_length, img_line_length, pos;
    bitmap_file_header bf;
    bitmap_info bi;

    int fd = open(path, O_CREAT | O_WRONLY | O_TRUNC);
    if (-1 == fd)
    {
        print_warning("file open error!");
        return false;
    }

    /* BMP文件头信息，这里假定宽度为4的倍数 */
    width = p->cur_file_info.dec_info.rect_width;
    height = p->cur_file_info.dec_info.rect_height;
    line_length = width * p->cur_file_info.dec_info.bpp;
    bf.type = 0x4d42;
    bf.offBits = sizeof(bitmap_file_header) + sizeof(bitmap_info);
    bf.size = (line_length * height) + bf.offBits;
    bf.reserved1 = 0;
    bf.reserved2 = 0;
    write(fd, &bf, sizeof(bitmap_file_header));

    bi.bh.size = sizeof(bitmap_info);
    bi.bh.width = (long) width;
    bi.bh.height = (long) height;
    bi.bh.planes = 1;
    bi.bh.bit_count = (unsigned short) (p->cur_file_info.dec_info.bpp * 8);
    bi.bh.compression = 3; /* BI_BITFIELDS */
    bi.bh.size_image = 0;
    bi.bh.x_pixels_per_meter = 0;
    bi.bh.y_pixels_per_meter = 0;
    bi.bh.color_used = 0;
    bi.bh.color_important = 0;
    bi.bmi_colors[0] = 0xF800; /* blue */
    bi.bmi_colors[1] = 0x07E0; /* green */
    bi.bmi_colors[2] = 0x001F; /* red */
    bi.bmi_colors[3] = 0x0000; /* ?? */
    write(fd, &bi, sizeof(bitmap_info));

    line_buff = (char *) malloc(line_length);
    if (NULL == line_buff)
    {
        print_err("malloc fail");
        close(fd);
        return false;
    }
    memset(line_buff, 0, line_length);

    dx = ((width - p->cur_file_info.dec_info.img_width) / 2) * p->cur_file_info.dec_info.bpp;
    dy = (height - p->cur_file_info.dec_info.img_height) / 2;

    for (i = (int) (height - 1); i >= (int) (height - dy); i--)
    {
        write(fd, line_buff, line_length);
    }

    img_line_length = p->cur_file_info.dec_info.img_width * p->cur_file_info.dec_info.bpp;
    pos = img_line_length * (p->cur_file_info.dec_info.img_height - 1);
    for (; i >= (int) ((height - dy) - p->cur_file_info.dec_info.img_height); i--)
    {
        write(fd, line_buff, dx);
        write(fd, p->cur_file_info.dec_info.buf + pos, img_line_length);
        write(fd, line_buff, (line_length - img_line_length) - dx);
        pos -= img_line_length;
    }

    for (; i >= 0; i--)
    {
        write(fd, line_buff, line_length);
    }

    free(line_buff);
    close(fd);

    return true;
}

/*
 * 动画图（gif）直接将原文件内容复制
 */
static bool _save_gif_animation(scene_playing_t* p, const char *path)
{
    char *buff = NULL;
    unsigned int size;
    int fd = -1, fdw = -1;
    bool ret = false;

    fd = open(p->cur_file_info.file_path, O_RDONLY);
    if (-1 == fd)
    {
        print_warning("open file error: %s", p->cur_file_info.file_path);
        goto out;
    }

    fdw = open(path, O_CREAT | O_WRONLY | O_TRUNC);
    if (-1 == fdw)
    {
        print_warning("open file error: %s", path);
        goto out;
    }

    size = (unsigned int) lseek(fd, 0, SEEK_END);
    buff = (char *) malloc(size);
    if (NULL == buff)
    {
        print_err("malloc fail");
        goto out;
    }
    lseek(fd, 0, SEEK_SET);
    read(fd, buff, size);
    write(fdw, buff, size);
    ret = true;

    out:
    if (fd >= 0)
    {
        close(fd);
    }

    if (fdw >= 0)
    {
        close(fdw);
    }

    if (NULL != buff)
    {
        free(buff);
    }

    return ret;
}

static int _proc_item(menuview_t *menuview, menuview_sig_e signal)
{
    char picpath[MAX_PATH_LENGTH], *picName;
    bool ret = false;
    int angle = sty_get_view_rotation();
    bool showinfo = true;
    int pos;
    msg_apps_t msg;

    switch (signal)
    {
    case WIDGET_SIG_MENUVIEW_OK:
        switch (menuview->menu->current)
        {
        case OPT_ITEM_BACKGROUND:
            /* gif不允许设置背景图 */
            if (!strcmp(g_playing->cur_file_info.file_info.file_format, "gif"))
            {
                scene_playing_dialog(g_playing, DIALOG_HEAD_FAIL, STR_ROTATE_NOT_SUPPORT, NULL);
                ret = false;
                showinfo = false;   /* 已经提示过了，不要再提示错误  */
            }
            else
            {
                ret = set_user_bg_pic(&g_playing->cur_file_info.dec_info, BG_ALL, angle);
                photo_dec_control(ROTATERIGHT90, 0);
                ret = set_user_bg_pic(&g_playing->cur_file_info.dec_info, BG_ALL, (angle + ROTATION_90) % 360);
                photo_dec_control(ROTATELEFT90, 0);
            }
            if (true == ret)
            {
                msg.type = MSG_BACKGROUND_PIC_CHANGE;
                broadcast_msg(&msg);
            }
            break;

        case OPT_ITEM_BOOTLOGO:
            sprintf(picpath, "%s", APP_NAME_ANIM_ON);
            pos = (int) strlen(picpath) - 1;
            while ((pos > 0) && (picpath[pos] != '/'))
            {
                pos--;
            }
            if (pos <= 0)
            {
                ret = false;
                break;
            }

            if ((ROTATION_0 == angle) || (ROTATION_180 == angle))
            {
                picName = "poweron_pic_sidelong_name.bin";
                sprintf(picpath + pos, "/%s/%s", sys_get_cur_theme_name(), picName);
                set_config(CFG_ANIMATION_POWERON_PIC_SIDELONG_NAME, picName, (int) strlen(picName) + 1);
            }
            else
            {
                picName = "poweron_pic_upright_name.bin";
                sprintf(picpath + pos, "/%s/%s", sys_get_cur_theme_name(), picName);
                set_config(CFG_ANIMATION_POWERON_PIC_UPRIGHT_NAME, picName, (int) strlen(picName) + 1);
            }

            if (!strcmp(g_playing->cur_file_info.file_info.file_format, "gif"))
            {
                ret = _save_gif_animation(g_playing, picpath);
            }
            else
            {
                ret = _save_bmp_logo(g_playing, picpath);
            }
            if (!ret)
            {
                break;
            }
            pos = 1;
            set_config(CFG_ANIMATION_POWERON_START_MODE, (char *) &pos, 4);
            break;

        case OPT_ITEM_STANDBYLOGO:
            sprintf(picpath, "%s", APP_NAME_ANIM_OFF);
            pos = (int) strlen(picpath) - 1;
            while ((pos > 0) && (picpath[pos] != '/'))
            {
                pos--;
            }
            if (pos <= 0)
            {
                ret = false;
                break;
            }

            if ((ROTATION_0 == angle) || (ROTATION_180 == angle))
            {
                picName = "poweroff_pic_sidelong_name.bin";
                sprintf(picpath + pos, "/%s/%s", sys_get_cur_theme_name(), picName);
                set_config(CFG_ANIMATION_POWEROFF_PIC_SIDELONG_NAME, picName, (int) strlen(picName) + 1);
            }
            else
            {
                picName = "poweroff_pic_upright_name.bin";
                sprintf(picpath + pos, "/%s/%s", sys_get_cur_theme_name(), picName);
                set_config(CFG_ANIMATION_POWEROFF_PIC_UPRIGHT_NAME, picName, (int) strlen(picName) + 1);
            }
            print_dbg();

            if (!strcmp(g_playing->cur_file_info.file_info.file_format, "gif"))
            {
                ret = _save_gif_animation(g_playing, picpath);
            }
            else
            {
                ret = _save_bmp_logo(g_playing, picpath);
            }
            if (!ret)
            {
                break;
            }
            pos = 1;
            set_config(CFG_ANIMATION_POWEROFF_START_MODE, (char *) &pos, 4);
            break;

        case OPT_ITEM_DEFAULT:
            if (!restore_default_bg_pic(BG_ALL))
            {
                break;
            }
            msg.type = MSG_BACKGROUND_PIC_CHANGE;
            broadcast_msg(&msg);
            ret = restore_default_all_animation();
            break;

        default:
            showinfo = false;
            break;
        }

        if (true == showinfo)
        {
            if (true == ret)
            {
                scene_playing_dialog(g_playing, DIALOG_HEAD_INFO, STR_SET_OK, NULL);
            }
            else
            {
                scene_playing_dialog(g_playing, DIALOG_HEAD_FAIL, STR_SET_FAIL, NULL);
            }
        }
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        photo_option_setbg_exit(menuview);
        break;

    default:
        break;
    }

    return 0;
}

static void _fill_items(menu_t *menu, scene_playing_t *p)
{
    menuitem_t item;
    resg_opt_setbg_t *resg = &p->resg_opt_setbg;

    memset(&item, 0, sizeof(menuitem_t));

    /* 填充顺序要跟effect_item_e定义的一致 */
    _add_item(menu, &item, resg->str_background, false, (item_cb_t) _proc_item);
    _add_item(menu, &item, resg->str_bootlogo, false, (item_cb_t) _proc_item);
    _add_item(menu, &item, resg->str_standbylogo, false, (item_cb_t) _proc_item);
    _add_item(menu, &item, resg->str_default, false, (item_cb_t) _proc_item);
}

static void _setbg_init(void)
{
    g_cur_opt_state = OPT_SET_BACKGROUND;
    if (NULL == s_setbg)
    {
        s_setbg = photo_menu_create();
        _fill_items(s_setbg, g_playing);
    }
}

static void _setbg_deinit(void)
{
    if (NULL != s_setbg)
    {
        photo_menu_delete(s_setbg);
        s_setbg = NULL;
    }
}

bool photo_option_setbg_enter(menuview_t *mv, fusion_enter_e fenter)
{
    _setbg_init();
    menuview_set_menu(mv, s_setbg);

    if (mv->type != MENUVIEW_OPTION)
    {
        menuview_load_style(mv, MENUVIEW_OPTION);
    }
    menuview_paint(mv);
    return true;
}

bool photo_option_setbg_exit(menuview_t *mv)
{
    _setbg_deinit();
    photo_option_enter(mv, FUSION_BACKWARD);
    return true;
}
