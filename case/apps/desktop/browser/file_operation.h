/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : file_operation.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *    binannan    2009-05-25          v1.0               build this file
 ********************************************************************************/

#ifndef __FILE_OPERATION_H__
#define __FILE_OPERATION_H__

#include <include_psp.h>

typedef enum file_type_enum
{
    TYPE_FOLD = 0,
    TYPE_MUSIC,
    TYPE_VIDEO,
    TYPE_PHOTO,
    TYPE_TXT,
    TYPE_EMULATOR,
    TYPE_DEFAULT,
}file_type_e;

typedef struct app_ext_s
{
    file_type_e app_type;
    char *app_name;
    char *ext;
    char *scene;
}app_ext_t;

extern int g_file_op_errno;

extern bool g_update_fsel;

bool is_file_exist(const char* file_path);

bool is_dir_exist(const char* dir_path);

bool is_sub_dir(const char* parent, const char* sub);

bool in_same_disk(const char* src, const char* dest);

int copy_dir(const char* src, const char* dest);

int copy_file(const char* src, const char* dest);


int move_dir(const char* src, const char* dest);

int move_file(const char* src, const char* dest);


int remove_dir_recursive(const char* dir);

int remove_file(const char* file);

app_ext_t* find_open_app(const char* file_name);

file_type_e find_file_type(const char* file_name);

#endif /* __FILE_OPERATION_H__ */
