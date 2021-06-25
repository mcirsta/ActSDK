/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason    2009-06-14          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo.h
 * \brief    photo 应用通用定义
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/06/14
 *******************************************************************************/

#ifndef __PHOTO_H__
#define __PHOTO_H__

#include "photo_sty.h"
#include "include_case.h"
#include "include_psp.h"

#include "fusion.h"

#define TEST_ROTATE 0       /* 旋转屏功能测试启动与关闭（测试用） */

#define MAX_PATH_LENGTH     PATH_MAX

#define MAX_ZOOM        10    /* 最大放大倍数 */

/* decode status */
typedef enum
{
    NOT_DECODE,     /* 未曾解码过*/
    DECODING,       /* 正在解码 */
    DECODE_FINISHED,/* 解码正确完成 */
    DECODE_ERROR,   /* 解码出错 */
} photo_dec_status_e;

typedef struct
{
    mmm_img_info_t dec_info;        /* 图片解码信息 */
    mmm_img_file_info_t file_info;  /* 图片原始信息 */
    char *file_path;                /* 图片文件路径 */
    char *file_name;                /* 图片文件名 */
    photo_dec_status_e dec_status;  /* 解码状态 */
    unsigned int    dec_errno;
} dec_file_info_t;

/* 多级目录数据 */
typedef struct level_info_s
{
    int         start_index;    /* 当前屏幕起始文件在总文件中的index */
    int         cur_index;      /* 当前屏幕中选择的index */
    struct level_info_s *prev;  /* 指向上一级目录信息 */
} level_info_t;

/* preview mode */
typedef enum
{
    PREV_LIST = 0,      /* 列表模式 */
    PREV_THUMBNAIL,     /* 缩略图模式 */
} preview_mode_e;

/*
 * preview信号
 * OK   -- 选定了某幅图片播放
 * QUIT -- 退出preview
 */
typedef enum
{
    WIDGET_SIG_PREVIEW_OK,
    WIDGET_SIG_PREVIEW_QUIT,
} preview_sig_e;

/*
 * 预览存储器类型
 */
typedef enum
{
    PATH_INTERNAL = 0,  /* 预览内部存储器 */
    PATH_CARD,      /* 预览卡储器 */
    PATH_EXTERNAL,  /* 预览外部存储器 */
    PATH_PLAYLIST,  /* 预览播放列表 */
} preview_path_e;

/*
 * 幻灯片特效菜单项
 */
typedef enum
{
    ITEM_RANDOM = 0,
    ITEM_FLY_IN,
    ITEM_CHESS_BOARD,
    ITEM_CUT,
    ITEM_ERASE,
    ITEM_LOUVER,
    ITEM_RANDOM_LINE,
    ITEM_WATER_WAVE,
    ITEM_CUBE,
    ITEM_3D_ROTATION,
    ITEM_OFF,
    ITEM_MAX,
} slide_effect_item_e;

/*
 * 播放列表模式
 */
typedef enum
{
    PHOTO_PLIST_DEFAULT = 0,/* 默认使用 */
    PHOTO_PLIST_SPECIAL,    /* 从dc应用直接进入时使用 */
} photo_plist_mode_e;

/*
 * style文件类型（公共、私有）
 */
typedef enum
{
    STY_PRIVATE = 0,
    STY_COMMON,
}photo_sty_type_e;

typedef bool (* menu_entry_t)(menuview_t *, fusion_enter_e);

static inline void _add_item(menu_t *menu, menuitem_t *item, string_resource_t *str,
        bool sub_menu_flag, item_cb_t cb)
{
    item->callback = cb;
    item->text_id = (int)str->str_id;
    item->has_child = sub_menu_flag;
    menu_append_item(menu, item);
}

/*
 * 目录相关函数
 */
bool path_is_directory(const char *path);
bool get_parent_directory(char *pdir, size_t max_len, const char *filepath);
char *get_card_top_path(void);
char *get_external_top_path(void);
char *get_top_path(const char *fullpath);
char *get_root_path(const char *fullpath);

/*
 * 特效相关函数
 */
void scene_fusion_set_first_frame(scene_t *scene);
void scene_fusion_set_last_frame(scene_t *scene);

void option_fusion_set_first_frame(menuview_t *mv);
void option_fusion_set_last_frame(menuview_t *mv);

/*
 * 全局数据保存、恢复、删除
 */
/* dirlist */
dirlist_t *photo_dirlist_create(dirlist_init_t* init, dirlist_cb_t callback);
dirlist_t *photo_dirlist_get_current(void);
bool photo_dirlist_update(const char *path, fsel_update_mode mode);
bool photo_dirlist_delete(dirlist_t *dirlist);
bool photo_dirlist_delete_all(void);

/* 多级菜单 */
menu_t *photo_menu_create(void);
bool photo_menu_delete(menu_t *menu);
bool photo_menu_delete_all(void);

/* 缩略图目录栈数据 */
bool photo_level_info_save(level_info_t *level_info);
level_info_t *photo_level_info_restore(void);
bool photo_level_info_delete(level_info_t *level_info);
bool photo_level_info_delete_all(void);

/*
 * style
 */
bool photo_sty_open(void);
bool photo_sty_close(void);
scene_t *photo_sty_load_scene(photo_sty_type_e type, int id);


/*
 * 幻灯片特效菜单项与具体fusion转换
 */
fusion_mode_e photo_slide_get_fusion_mode(int index);
int photo_slide_get_item_index(fusion_mode_e mode);

/*
 * 创建子线程
 */
bool photo_create_thread( void *proc, void *param );

#endif

