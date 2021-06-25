/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_utils.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason    2009-06-11         v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_utils.c
 * \brief    photo 通用函数
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/06/11
 *******************************************************************************/

#include "photo.h"

typedef struct menu_list_s
{
    menu_t *menu;
    struct menu_list_s *prev;
}menu_list_t;

typedef struct index_mode_s
{
    int             index;
    fusion_mode_e   mode;
}index_mode_t;

static dirlist_t *s_dirlist = NULL;                 /* dirlist指针保存 */
static menu_list_t *s_menu_list_tail = NULL;        /* 多级菜单数据保存 */
static level_info_t *s_thumbnail_level_info = NULL; /* 缩略图多级目录数据保存 */
static style_t *s_sty_private = NULL;
static style_t *s_sty_common = NULL;

/*
 * 幻灯片特效菜单与具体特效对应表
 */
static index_mode_t fusion_table[] =
{
    {ITEM_RANDOM,       FUSION_RANDOM},
    {ITEM_FLY_IN,       FUSION_PUSH},
    {ITEM_CHESS_BOARD,  FUSION_CHECK},
    {ITEM_CUT,          FUSION_CUT},
    {ITEM_ERASE,        FUSION_ERASE},
    {ITEM_LOUVER,       FUSION_LOUVER},
    {ITEM_RANDOM_LINE,  FUSION_RANDOMLINE},
    {ITEM_WATER_WAVE,   FUSION_WAVE},
    {ITEM_CUBE,         FUSION_CUBE},
    {ITEM_3D_ROTATION,  FUSION_ROTATE},
    {ITEM_OFF,          FUSION_NORMAL},
};

#define FUSION_TABLE_SIZE (sizeof(fusion_table)/sizeof(index_mode_t))

/*
 * 判断某个文件是否为目录
 */
bool path_is_directory(const char *path)
{
    bool ret = false;
    int fd = open(path, O_DIRECTORY, 0);

    if (-1 == fd)
    {
        ret = false;
    }
    else
    {
        ret = true;
        close(fd);
    }

    return ret;
}

/*
 * 获取某个文件的父亲目录
 */
bool get_parent_directory(char *pdir, size_t max_len, const char *filepath)
{
    int i;
    size_t len;

    if((NULL == pdir) || (NULL == filepath))
    {
        return false;
    }

    len = strlen(filepath);
    if(len > (size_t)max_len)  /*TODO 条件可修改，这里只是为了简化计算 */
    {
        return false;
    }

    strncpy(pdir, filepath, max_len);
    i = (int)len - 1;
    while(i > 0)
    {
        if(pdir[i] == '/')
        {
            break;
        }
        pdir[i] = 0;
        i --;
    }

    if(0 == i)
    {
        return false;
    }
    else
    {
        return true;
    }
}

/*
 * 获取卡top path
 */
char *get_card_top_path(void)
{
    hotplugin_dev_type_e main_disk;
    const char *ret = NULL;

    main_disk = sys_get_main_disk();
    if(main_disk == HOTPLUGIN_DEV_TYPE_CARD)
    {
        ret = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CARD,DIR_PHOTO);
    }
    else
    {
        ret = CARD_ROOT;
    }
    return (char *)ret;
}

/*
 * 获取外部存储器top path
 */
char *get_external_top_path(void)
{
    hotplugin_dev_type_e main_disk;
    const char *ret = NULL;

    main_disk = sys_get_main_disk();
    if(main_disk == HOTPLUGIN_DEV_TYPE_UHOST)
    {
        ret = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_UHOST,DIR_PHOTO);
    }
    else
    {
        ret = EXTERNAL_ROOT;
    }
    return (char *)ret;
}

/*
 * 获取某个路径的顶层路径(顶层路径：指的是到达某个路径之后，再向上一级目录就退出)
 */
char *get_top_path(const char *fullpath)
{
    hotplugin_dev_type_e main_disk;
    const char *ret = NULL;

    if(fullpath == NULL)
    {
        return NULL;
    }

    main_disk = sys_get_main_disk();
    if(!memcmp(fullpath, sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_PHOTO), 
            strlen(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_PHOTO))))
    {
        ret = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_PHOTO);
    }
    else if(!memcmp(fullpath, CARD_ROOT, strlen(CARD_ROOT)))
    {
        if(main_disk == HOTPLUGIN_DEV_TYPE_CARD)
        {
            ret = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CARD,DIR_PHOTO);
        }
        else
        {
            ret = CARD_ROOT;
        }
    }
    else if(!memcmp(fullpath, EXTERNAL_ROOT, strlen(EXTERNAL_ROOT)))
    {
        if(main_disk == HOTPLUGIN_DEV_TYPE_UHOST)
        {
            ret = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_UHOST,DIR_PHOTO);
        }
        else
        {
            ret = EXTERNAL_ROOT;
        }
    }
    else
    {
        ret = NULL;
    }

    return (char *)ret;
}

/*
 * 获取某个路径根目录
 */
char *get_root_path(const char *fullpath)
{
    char *ret = NULL;

    if(fullpath == NULL)
    {
        return NULL;
    }

    if(!memcmp(fullpath, UDISK_ROOT, strlen(UDISK_ROOT)))
    {
        ret = UDISK_ROOT;
    }
    else if(!memcmp(fullpath, CARD_ROOT, strlen(CARD_ROOT)))
    {
        ret = CARD_ROOT;
    }
    else if(!memcmp(fullpath, EXTERNAL_ROOT, strlen(EXTERNAL_ROOT)))
    {
        ret = EXTERNAL_ROOT;
    }
    else
    {
        ret = NULL;
    }

    return ret;
}

/*
 * 设置当前场景全部区域作为特效的第一帧
 */
void scene_fusion_set_first_frame(scene_t *scene)
{
    fusion_frame_t frame;

    frame.rect.x = scene->x;
    frame.rect.y = scene->y;
    frame.rect.width = scene->width;
    frame.rect.height = scene->height;
    frame.pdata = NULL;
    frame.alpha = false;

    fusion_set_first_frame(&frame);
}

/*
 * 设置当前场景全部区域作为特效的最后一帧
 */
void scene_fusion_set_last_frame(scene_t *scene)
{
    fusion_frame_t frame;

    frame.rect.x = scene->x;
    frame.rect.y = scene->y;
    frame.rect.width = scene->width;
    frame.rect.height = scene->height;
    frame.pdata = NULL;
    frame.alpha = false;

    fusion_set_last_frame(&frame);
}

/*
 * Option菜单特效第一帧
 */
void option_fusion_set_first_frame(menuview_t *mv)
{
    fusion_frame_t frame;
    canvas_info_t info;

    canvas_get_info(mv->canvas_id, &info);

    frame.rect.x = info.rect.x;
    frame.rect.y = info.rect.y;
    frame.rect.width = info.rect.width;
    frame.rect.height = info.rect.height;
    frame.pdata = NULL;
    frame.alpha = false;
    fusion_set_first_frame(&frame);
}

/*
 * Option菜单特效最后一帧
 */
void option_fusion_set_last_frame(menuview_t *mv)
{
    fusion_frame_t frame;
    canvas_info_t info;

    canvas_get_info(mv->canvas_id, &info);

    frame.rect.x = info.rect.x;
    frame.rect.y = info.rect.y;
    frame.rect.width = info.rect.width;
    frame.rect.height = info.rect.height;
    frame.pdata = (unsigned char *)info.pdata;
    frame.alpha = true;
    fusion_set_last_frame(&frame);
}

/*
 * dirlist 创建、获取、删除
 */
dirlist_t *photo_dirlist_get_current(void)
{
    return s_dirlist;
}

dirlist_t *photo_dirlist_create(dirlist_init_t* init, dirlist_cb_t callback)
{
    if(NULL == s_dirlist)
    {
        s_dirlist = dirlist_create(init, callback);
    }

    return s_dirlist;
}

bool photo_dirlist_update(const char *path, fsel_update_mode mode)
{
    FSEL_HANDLE hfsel;
    if(NULL == s_dirlist)
    {
        return true;
    }
    hfsel = dirlist_get_fsel_handle(s_dirlist);
    fsel_update(hfsel, (char *)path, mode);
    return true;
}

bool photo_dirlist_delete(dirlist_t *dirlist)
{
    if(dirlist != NULL)
    {
        dirlist_delete(dirlist);
        if(s_dirlist == dirlist)
        {
            s_dirlist = NULL;
        }
    }

    return true;
}

bool photo_dirlist_delete_all(void)
{
    photo_dirlist_delete(s_dirlist);
    return true;
}

/*
 * 创建menu，并在链表中添加该项
 */
menu_t *photo_menu_create(void)
{
    menu_list_t *list_node;
    menu_t *menu = menu_create();
    if(NULL == menu)
    {
        return NULL;
    }

    list_node = (menu_list_t *)malloc(sizeof(menu_list_t));
    if(NULL == list_node)
    {
        menu_delete(menu);
        return NULL;
    }
    list_node->menu = menu;
    list_node->prev = s_menu_list_tail;
    s_menu_list_tail = list_node;
    return menu;
}

/*
 * 删除menu，并在链表中删除该项
 */
bool photo_menu_delete(menu_t *menu)
{
    menu_list_t *cur_node, *prev_node;

    if((NULL == s_menu_list_tail) || (NULL == menu))
    {
        return false;
    }

    menu_delete(menu);

    if(s_menu_list_tail->menu == menu)
    {
        cur_node = s_menu_list_tail->prev;
        free(s_menu_list_tail);
        s_menu_list_tail = cur_node;
        return true;
    }

    cur_node = s_menu_list_tail;
    prev_node = s_menu_list_tail->prev;
    while(prev_node != NULL)
    {
        if(prev_node->menu == menu)
        {
            cur_node->prev = prev_node->prev;
            free(prev_node);
            break;
        }
        cur_node = prev_node;
        prev_node = prev_node->prev;
    }

    return true;
}

bool photo_menu_delete_all(void)
{
    menu_list_t *cur_node, *prev_node;

    cur_node = s_menu_list_tail;
    while(NULL != cur_node)
    {
        prev_node = cur_node->prev;
        photo_menu_delete(cur_node->menu);
        cur_node = prev_node;
    }

    s_menu_list_tail = NULL;
    return true;
}

/*
 * 缩略图目录栈操作
 */
bool photo_level_info_save(level_info_t *level_info)
{
    s_thumbnail_level_info = level_info;
    return true;
}

level_info_t *photo_level_info_restore(void)
{
    level_info_t *level_info = s_thumbnail_level_info;
    s_thumbnail_level_info = NULL;
    return level_info;
}

bool photo_level_info_delete(level_info_t *level_info)
{
    level_info_t *cur_info, *prev_info;

    cur_info = level_info;
    while (NULL != cur_info)
    {
        prev_info = cur_info->prev;
        free(cur_info);
        cur_info = prev_info;
    }

    if(s_thumbnail_level_info == level_info)
    {
        s_thumbnail_level_info = NULL;
    }
    return true;
}

bool photo_level_info_delete_all(void)
{
    photo_level_info_delete(s_thumbnail_level_info);
    return true;
}

/*
 * style相关
 */
bool photo_sty_open(void)
{
    char app_file[MAX_PATH_LENGTH];

    sprintf(app_file, "%s/%s.sty", sys_get_app_theme_res_dir(), "photo");
    s_sty_private = open_style_file(app_file);
    if(NULL == s_sty_private)
    {
        print_err("open sty file error:%s", app_file);
        return false;
    }

    sprintf(app_file, "%s/%s.sty", sys_get_common_theme_res_dir(), "commonui");
    s_sty_common = open_style_file(app_file);
    if(NULL == s_sty_common)
    {
        print_err("open sty file error:%s", app_file);
        return false;
    }

    return true;
}

bool photo_sty_close(void)
{
    if(NULL != s_sty_private)
    {
        close_style_file(s_sty_private);
    }

    if(NULL != s_sty_common)
    {
        close_style_file(s_sty_common);
    }
    return true;
}

scene_t *photo_sty_load_scene(photo_sty_type_e type, int id)
{
    scene_t *ret = NULL;
    switch(type)
    {
    case STY_PRIVATE:
        ret = load_scene(s_sty_private, (unsigned int)id);
        break;
    case STY_COMMON:
        ret = load_scene(s_sty_common, (unsigned int)id);
        break;
    default:
        break;
    }
    return ret;
}

/*
 * 幻灯片特效菜单与具体特效相互转换
 */
fusion_mode_e photo_slide_get_fusion_mode(int item_index)
{
    fusion_mode_e ret = FUSION_NORMAL;
    int i;

    if((item_index < 0) || (item_index > ITEM_MAX))
    {
        return FUSION_NORMAL;
    }

    for(i = 0; i < (int)FUSION_TABLE_SIZE; i++)
    {
        if(item_index == fusion_table[i].index)
        {
            ret = fusion_table[i].mode;
            break;
        }
    }
    return ret;
}

int photo_slide_get_item_index(fusion_mode_e mode)
{
    int ret = ITEM_OFF;
    int i;

    for(i = 0; i < (int)FUSION_TABLE_SIZE; i++)
    {
        if(mode == fusion_table[i].mode)
        {
            ret = fusion_table[i].index;
            break;
        }
    }
    return ret;
}


bool photo_create_thread( void *proc, void *param )
{
    pthread_t tid;
    pthread_attr_t pthread_attr;
    struct sched_param self_param;
    int policy;
    int result = -1;

    /*
     * process manger's msg loop should be activated as long as there is
     * a msg in the process manager's msg queue
     */
    pthread_attr_init(&pthread_attr);
    pthread_attr_setinheritsched(&pthread_attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setdetachstate(&pthread_attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setscope(&pthread_attr, PTHREAD_SCOPE_PROCESS);

    //创建比主进程低一级的线程, 保证主进程先运行
    pthread_getschedparam( pthread_self(), &policy, &self_param );
    self_param.sched_priority = self_param.sched_priority - 2;
    pthread_attr_setschedparam(&pthread_attr, &self_param);

    print_dbg(" manager_msg_loop  thread's priority is: %d\n", self_param.sched_priority );

    result = pthread_create( &tid, &pthread_attr, proc, param );
    pthread_attr_destroy( &pthread_attr );
    if( result != 0 )
    {
        print_err("can’t create thread, result:%d\n", result);
        return false;
    }

    return true;
}

