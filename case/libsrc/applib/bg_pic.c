/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : bg_pic.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason     2009-06-04        v1.0               build this file
 ********************************************************************************/
/*!
 * \file     read_bg_pic.c
 * \brief    背景图保存及显示
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/06/04
 *******************************************************************************/

#include "include_psp.h"
#include "include_case.h"

#define ONLY_ONE_BG_PIC 1   /* 所有应用使用同一个背景图开关 */

static bool s_dir_maked = false;

static bool _read_bg_pic(gui_bitmap_info_t *bitmap_info, const char *path)
{
    bg_pic_header header;
    int fd, len;

    fd = open(path, O_RDONLY);
    if(-1 == fd)
    {
        print_warning("file open error!");
        return false;
    }

    read(fd, &header, sizeof(bg_pic_header));
    bitmap_info->xsize = header.width;
    bitmap_info->ysize = header.height;
    bitmap_info->bytesperpixel = header.bpp;
    len = header.width * header.height * header.bpp;
    assert((len > 0) && (len < (2048 * 2048 * 2)));
    bitmap_info->pdata = (unsigned char *)malloc((unsigned int)len);
    if(NULL == bitmap_info->pdata)
    {
        print_err("malloc fail");
        close(fd);
        return false;
    }

    read(fd, (void *)bitmap_info->pdata, (unsigned int)len);
    close(fd);

    return true;
}

static inline void _assembly_full_path(char *path, const char *name, rotation_e angle)
{
    if((angle == ROTATION_0) || (angle == ROTATION_180))
    {
        sprintf(path, "%s/%s", APP_BG_SIDELONG_ROOT, name);
    }
    else
    {
        sprintf(path, "%s/%s", APP_BG_UPRIGHT_ROOT, name);
    }
}

static void _make_dir(const char *path)
{
    char path_tmp[PATH_MAX];
    int pos, pos_start;

    memset(path_tmp, 0, PATH_MAX);
    pos = 0;
    if(path[pos] == '/')
    {
        pos ++;
    }

    pos_start = 0;
    while(path[pos] != '\0')
    {
        if(path[pos] == '/')
        {
            memcpy(path_tmp + pos_start, path + pos_start, (unsigned int)(pos - pos_start));
            mkdir(path_tmp, 777);
            pos_start = pos;
        }

        pos ++;
    }

    if(((pos - pos_start) > 1) && ((pos - pos_start) < PATH_MAX))
    {
        memcpy(path_tmp + pos_start, path + pos_start, (unsigned int)(pos - pos_start));
        mkdir(path_tmp, 777);
    }

    s_dir_maked = true;
}

/*
 * 保存背景图
 */
static bool _save_bg_pic(mmm_img_info_t* img_info, const char *path)
{
    char *line_buff = NULL;
    int i, j;
    unsigned int dx, dy;
    unsigned int line_length, img_line_length, pos;
    bg_pic_header header;
    int count;

    int fd = open(path, O_CREAT | O_WRONLY | O_TRUNC);
    if (-1 == fd)
    {
        print_err("file open error!");
        return false;
    }
    memset(&header, 0, sizeof(bg_pic_header));
    header.magic = BG_PIC_MAGIC;
    header.width = (unsigned short) img_info->rect_width;
    header.height = (unsigned short) img_info->rect_height;
    header.bpp = (unsigned char) img_info->bpp;

    ftruncate(fd, sizeof(bg_pic_header));
    lseek(fd, 0, SEEK_END);
    line_length = img_info->rect_width * img_info->bpp;
    line_buff = (char *) malloc(line_length);
    if(NULL == line_buff)
    {
        print_err("malloc fail!");
        close(fd);
        return false;
    }
    memset(line_buff, 0, line_length);

    dx = ((header.width - img_info->img_width) / 2) * header.bpp;
    dy = (header.height - img_info->img_height) / 2;
    count = 0;
    for (i = 0; i < (int) dy; i++)
    {
        count += write(fd, line_buff, line_length);
    }

    img_line_length = img_info->img_width * img_info->bpp;
    pos = 0;
    for (; i < (int) (img_info->img_height + dy); i++)
    {
        count += write(fd, line_buff, dx);
        count += write(fd, img_info->buf + pos, img_line_length);
        count += write(fd, line_buff, line_length - img_line_length - dx);
        for (j = 0; j < (int) img_line_length; j++)
        {
            header.checksum += *(img_info->buf + pos);
            pos++;
        }
    }

    for (; i < (int) img_info->rect_height; i++)
    {
        count += write(fd, line_buff, line_length);
    }

    lseek(fd, 0, SEEK_SET);
    count += write(fd, &header, sizeof(bg_pic_header));
    free(line_buff);
    close(fd);

    return true;
}

static void _rotate_info(mmm_img_info_t *dst_info, mmm_img_info_t *src_info)
{
    int x, y;
    unsigned short *tmpbuf;
    unsigned short *tmpbuf_src;
    int width, height;

    tmpbuf = (unsigned short *) dst_info->buf;
    tmpbuf_src = (unsigned short *) src_info->buf;

    dst_info->rect_width = src_info->rect_height;
    dst_info->rect_height = src_info->rect_width;
    dst_info->img_width = src_info->img_height;
    dst_info->img_height = src_info->img_width;

    width = (int)dst_info->img_width;
    height = (int)dst_info->img_height;

    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            tmpbuf[y * width + x] = tmpbuf_src[(x * height) + (height - 1 - y)];
        }
    }
}

bool set_user_bg_pic(mmm_img_info_t *img_info, const char *name_list, int angle)
{
    char path[PATH_MAX];
    char name[PATH_MAX];
    char *str_start, *str_end;
    mmm_img_info_t img_info_local;
    int angle_now = sty_get_screen_rotation();

    if(!s_dir_maked)
    {
        _make_dir(APP_BG_UPRIGHT_ROOT);
        _make_dir(APP_BG_SIDELONG_ROOT);
    }

#if(ONLY_ONE_BG_PIC)
    name_list = BG_MUSIC;
#endif

    memcpy(&img_info_local, img_info, sizeof(mmm_img_info_t));
    if(angle != angle_now)
    {
        img_info_local.buf = (unsigned char *)malloc(img_info->len);
        if(NULL == img_info_local.buf)
        {
            print_err("malloc fail");
            return false;
        }
        _rotate_info(&img_info_local, img_info);
    }

    str_end = (char *)name_list;
    while(*str_end != '\0')
    {
        str_start = str_end;
        while((*str_end != '\0') && (*str_end != ';'))
        {
            str_end ++;
        }
        memcpy(name, str_start, (unsigned int)(str_end - str_start));
        name[str_end - str_start] = '\0';
        _assembly_full_path(path, name, angle);

        if(!_save_bg_pic(&img_info_local, path))
        {
            return false;
        }

        if(*str_end == ';')
        {
            str_end ++;
        }
    }

    if(angle != angle_now)
    {
        free(img_info_local.buf);
    }

    return true;
}

typedef struct
{
    int width;
    int height;
    int bpp;
    int length;
    char reserved[16];
}boot_pic_header;

bool set_boot_logo_pic(mmm_img_info_t *img_info, int angle)
{
    char *line_buff = NULL, *buff = NULL;
    unsigned int dx, dy;
    unsigned int i, line_length, img_line_length, pos;
    boot_pic_header header;

    memset(&header, 0, sizeof(boot_pic_header));
    header.width = (int) img_info->rect_width;
    header.height = (int) img_info->rect_height;
    header.bpp = (int) img_info->bpp;
    header.length = header.width * header.height * 2;

    buff = (char *)malloc(sizeof(boot_pic_header) + (unsigned int)header.length);
    if(NULL == buff)
    {
        print_err("malloc fail");
        return false;
    }
    memcpy(buff, &header, sizeof(boot_pic_header));

    dx = (((unsigned int)header.width - img_info->img_width) / 2) * (unsigned int)header.bpp;
    dy = ((unsigned int)header.height - img_info->img_height) / 2;

    line_length = img_info->rect_width * img_info->bpp;
    img_line_length = img_info->img_width * img_info->bpp;

    line_buff = buff + sizeof(boot_pic_header);
    memset(buff + sizeof(boot_pic_header), 0, line_length * dy);    /* 将前面若干行填0 */
    line_buff += line_length * dy;

    pos = 0;
    for (i = dy; i < (img_info->img_height + dy); i++)
    {
        memset(line_buff, 0, dx);
        memcpy(line_buff + dx, img_info->buf + pos, img_line_length);
        memset(line_buff + dx + img_line_length, 0, line_length - img_line_length - dx);
        line_buff += line_length;
        pos += img_line_length;
    }

    memset(line_buff, 0, (img_info->rect_height - i) * line_length);  /* 将后面若干行填0 */

    set_config(CFG_BOOT_LOGO, buff, (int)sizeof(boot_pic_header) + header.length);
    free(buff);

    return true;
}

bool restore_default_bg_pic(const char *name_list)
{
    char path[PATH_MAX];
    char name[PATH_MAX];
    char *str_start, *str_end;

    str_end = (char *)name_list;
    while(*str_end != '\0')
    {
        str_start = str_end;
        while((*str_end != '\0') && (*str_end != ';'))
        {
            str_end ++;
        }
        memcpy(name, str_start, (unsigned int)(str_end - str_start));
        name[str_end - str_start] = '\0';
        _assembly_full_path(path, name, ROTATION_0);
        remove(path);
        _assembly_full_path(path, name, ROTATION_90);
        remove(path);

        if(*str_end == ';')
        {
            str_end ++;
        }
    }

    return true;
}


gui_bitmap_info_t *get_user_bg_pic(const char *name)
{
    gui_bitmap_info_t *bitmap_info = NULL;
    char path[PATH_MAX];

    if(!strcmp(name, BG_ALL))
    {
        return NULL;
    }

#if(ONLY_ONE_BG_PIC)
    name = BG_MUSIC;
#endif

    bitmap_info = (gui_bitmap_info_t *)malloc(sizeof(gui_bitmap_info_t));
    if(NULL == bitmap_info)
    {
        print_err("malloc fail");
        return NULL;
    }

    _assembly_full_path(path, name, sty_get_screen_rotation());
    if(!_read_bg_pic(bitmap_info, path))
    {
        free(bitmap_info);
        return NULL;
    }

    return bitmap_info;
}


bool restore_default_all_animation(void)
{
    char picpath[PATH_MAX];

    if(get_config(CFG_DEF_ANIMATION_POWERON_PIC_SIDELONG_NAME, picpath, PATH_MAX) < 0)
    {
        print_err("get config CFG_DEF_ANIMATION_POWEROFF_PIC_SIDELONG_NAME failure");
        return false;
    }
    set_config(CFG_ANIMATION_POWERON_PIC_SIDELONG_NAME, picpath, (int)strlen(picpath) + 1);

    if(get_config(CFG_DEF_ANIMATION_POWERON_PIC_UPRIGHT_NAME, picpath, PATH_MAX) < 0)
    {
        print_err("get config CFG_DEF_ANIMATION_POWERON_PIC_UPRIGHT_NAME failure");
        return false;
    }
    set_config(CFG_ANIMATION_POWERON_PIC_UPRIGHT_NAME, picpath, (int)strlen(picpath) + 1);

    if(get_config(CFG_DEF_ANIMATION_POWEROFF_PIC_SIDELONG_NAME, picpath, PATH_MAX) < 0)
    {
        print_err("get config CFG_DEF_ANIMATION_POWEROFF_PIC_SIDELONG_NAME failure");
        return false;
    }
    set_config(CFG_ANIMATION_POWEROFF_PIC_SIDELONG_NAME, picpath, (int)strlen(picpath) + 1);

    if(get_config(CFG_DEF_ANIMATION_POWEROFF_PIC_UPRIGHT_NAME, picpath, PATH_MAX) < 0)
    {
        print_err("get config CFG_DEF_ANIMATION_POWEROFF_PIC_UPRIGHT_NAME failure");
        return false;
    }
    set_config(CFG_ANIMATION_POWEROFF_PIC_UPRIGHT_NAME, picpath, (int)strlen(picpath) + 1);

    reset_config(CFG_BOOT_LOGO);

    return true;
}


