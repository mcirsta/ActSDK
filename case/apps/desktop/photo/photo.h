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
 * \brief    photo Ӧ��ͨ�ö���
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

#define TEST_ROTATE 0       /* ��ת�����ܲ���������رգ������ã� */

#define MAX_PATH_LENGTH     PATH_MAX

#define MAX_ZOOM        10    /* ���Ŵ��� */

/* decode status */
typedef enum
{
    NOT_DECODE,     /* δ�������*/
    DECODING,       /* ���ڽ��� */
    DECODE_FINISHED,/* ������ȷ��� */
    DECODE_ERROR,   /* ������� */
} photo_dec_status_e;

typedef struct
{
    mmm_img_info_t dec_info;        /* ͼƬ������Ϣ */
    mmm_img_file_info_t file_info;  /* ͼƬԭʼ��Ϣ */
    char *file_path;                /* ͼƬ�ļ�·�� */
    char *file_name;                /* ͼƬ�ļ��� */
    photo_dec_status_e dec_status;  /* ����״̬ */
    unsigned int    dec_errno;
} dec_file_info_t;

/* �༶Ŀ¼���� */
typedef struct level_info_s
{
    int         start_index;    /* ��ǰ��Ļ��ʼ�ļ������ļ��е�index */
    int         cur_index;      /* ��ǰ��Ļ��ѡ���index */
    struct level_info_s *prev;  /* ָ����һ��Ŀ¼��Ϣ */
} level_info_t;

/* preview mode */
typedef enum
{
    PREV_LIST = 0,      /* �б�ģʽ */
    PREV_THUMBNAIL,     /* ����ͼģʽ */
} preview_mode_e;

/*
 * preview�ź�
 * OK   -- ѡ����ĳ��ͼƬ����
 * QUIT -- �˳�preview
 */
typedef enum
{
    WIDGET_SIG_PREVIEW_OK,
    WIDGET_SIG_PREVIEW_QUIT,
} preview_sig_e;

/*
 * Ԥ���洢������
 */
typedef enum
{
    PATH_INTERNAL = 0,  /* Ԥ���ڲ��洢�� */
    PATH_CARD,      /* Ԥ�������� */
    PATH_EXTERNAL,  /* Ԥ���ⲿ�洢�� */
    PATH_PLAYLIST,  /* Ԥ�������б� */
} preview_path_e;

/*
 * �õ�Ƭ��Ч�˵���
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
 * �����б�ģʽ
 */
typedef enum
{
    PHOTO_PLIST_DEFAULT = 0,/* Ĭ��ʹ�� */
    PHOTO_PLIST_SPECIAL,    /* ��dcӦ��ֱ�ӽ���ʱʹ�� */
} photo_plist_mode_e;

/*
 * style�ļ����ͣ�������˽�У�
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
 * Ŀ¼��غ���
 */
bool path_is_directory(const char *path);
bool get_parent_directory(char *pdir, size_t max_len, const char *filepath);
char *get_card_top_path(void);
char *get_external_top_path(void);
char *get_top_path(const char *fullpath);
char *get_root_path(const char *fullpath);

/*
 * ��Ч��غ���
 */
void scene_fusion_set_first_frame(scene_t *scene);
void scene_fusion_set_last_frame(scene_t *scene);

void option_fusion_set_first_frame(menuview_t *mv);
void option_fusion_set_last_frame(menuview_t *mv);

/*
 * ȫ�����ݱ��桢�ָ���ɾ��
 */
/* dirlist */
dirlist_t *photo_dirlist_create(dirlist_init_t* init, dirlist_cb_t callback);
dirlist_t *photo_dirlist_get_current(void);
bool photo_dirlist_update(const char *path, fsel_update_mode mode);
bool photo_dirlist_delete(dirlist_t *dirlist);
bool photo_dirlist_delete_all(void);

/* �༶�˵� */
menu_t *photo_menu_create(void);
bool photo_menu_delete(menu_t *menu);
bool photo_menu_delete_all(void);

/* ����ͼĿ¼ջ���� */
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
 * �õ�Ƭ��Ч�˵��������fusionת��
 */
fusion_mode_e photo_slide_get_fusion_mode(int index);
int photo_slide_get_item_index(fusion_mode_e mode);

/*
 * �������߳�
 */
bool photo_create_thread( void *proc, void *param );

#endif

