/********************************************************************************
 *                              USDK(1100)
 *                             Module: commonui
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       binannan   2009-5-7 13:04:15     1.0                build this file
 *******************************************************************************/
/*!
 * \file     dirlist_private.h
 * \brief    dirlist private header file
 * \author   binannan
 *******************************************************************************/
#ifndef __DIRLIST_PRIVATE_H__
#define __DIRLIST_PRIVATE_H__

#include "include_case.h"


typedef struct dir_level_s
{
    char* workdir; //当前工作目录

    int dirs; //当前目录下的子目录数
    int dir_startno; //列表显示中的子目录条目的开始号（针对文件选择器）
    int dir_num; //列表显示中的子目录条目数

    int files;
    int file_startno;
    int file_num;

    int item_group_size;
    menu_t* item_group; //用于显示的菜单数据集合


    struct dir_level_s* parent;
    struct dir_level_s* child;
} dir_level_t;

typedef struct entry_list_s
{
    menuview_t* entry_list;
    menu_t* entry_menu;
    menuview_type_e entry_list_type;

    string_resource_t* str_intmem;
    string_resource_t* str_extmem;
    string_resource_t* str_carmem;
    string_resource_t* str_nodisk;
} entry_list_t;

typedef struct content_list_s
{
    menuview_t* content_list;
    menuview_type_e content_list_type;

    void* p_mnavi;
    FSEL_HANDLE p_fsel;

    char* default_top_path_name;

    char* search_string;

    char* filter_string;

    dir_level_t* p_root;
    dir_level_t* p_cur_dir;

    string_resource_t*  str_nofile;
} content_list_t;

typedef enum
{
    NOTSET,ENTRY, CONTENT,
} list_type;

typedef enum
{
    ENTRY_INTERNAL,ENTRY_CARD,ENTRY_EXTERNAL,ENTRY_NONE,
}entryitem_e;

typedef enum
{
    ENTRYOP_CD,ENTRYOP_CDUP,ENTRYOP_NONE,
}entryop_e;

typedef struct dirlist_private_data_s
{
    list_type list_inuse;
    entry_list_t* entry;
    content_list_t* content;
} dirlist_private_data_t;

#endif /* __DIRLIST_PRIVATE_H__ */
