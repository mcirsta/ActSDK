/********************************************************************************
 *                               1100
 *                            Module: fileselector Driver
 *                 Copyright(c) 2001-2008 Actions Semiconductor,
 *                          All Rights Reserved.
 *
 * History:
 *      <author>    <time>       <version >    <desc>
 *      lyxiang     2008/08/28     1100         build this file
 ********************************************************************************
 */

/*!
 * \file fileselector.h
 * \brief file selector api for 1100
 * \author   lyxiang
 * \par GENERAL DESCRIPTION:
 *  本文件包含目录选择器模块提供出去的所有接口和数据结构
 *
 *     Copyright(c) 2003-2008 Actions Semiconductor, All Rights Reserved.
 *
 *  \version 1.0
 *  \date 2008/08/28
 */

#ifndef __FILESELECTOR_H__
#define __FILESELECTOR_H__

#include <libc/stdio.h>
#include <libc/string.h>
#include <libc/unistd.h>
#include <libc/dirent.h>
#include <libc/sys/limits.h>
#include "act_errno.h"
//#include "types.h"

/*! \cond fsel_api*/

/*!
 * \brief
 * 最大支持目录层次深度
 */
#define FSEL_MAX_DIR_DEPTH 64

/*!
 * \brief
 * 最大支持文件名长度
 */
//#define FSEL_MAX_NAMELENGTH 1024

/*!
 * \brief
 * 扇区大小
 */
#define  FSEL_SECTORSIZE  512

/*!
 * \brief
 * 目录选择器句柄
 */
typedef void * FSEL_HANDLE;

/*!
 *  \brief
 *  文件/目录结构类型
 *\li FSEL_FILENAME:文件
 *\li FSEL_DIRNAME:目录
 *\li FSEL_DIRNAME:both,主要应用于子目录搜索时搜索范围的控制
 */
typedef enum
{
    FSEL_FILENAME = 1,
    FSEL_DIRNAME = 2,
    FSEL_RESERVE = 3,
} fsel_filenametype_t;

/*!
 * \brief
 *  文件/目录名结构.
 */
typedef struct
{
    /*! 目录/文件结构体*/
    fsel_filenametype_t type;
    /*! 文件名*/
    char name[PATH_MAX];
    /*!文件的路径信息*/
    char *filepath;//文件名路径。注意:当使用fsel_listfile、fsel_listdir获取文件时，不能获取到文件的路径
} fsel_filename_t;

/*!
 *  \brief
 *  排序类型判断
 *\li FSEL_NOSORTING:不排序
 *\li FSEL_PINYIN:拼音排序
 *\li FSEL_BINARY:ASC码排序
 */
typedef enum
{
    FSEL_NOSORTING = 0, //不排序
    FSEL_PINYIN = 1, //拼音排序
    FSEL_BINARY = 2,
} fsel_sortstyle_t;

/*!
 *  \brief
 *  查找文件有效文件的范围设置
 *\li FSEL_SUB:搜索范围含子目录
 *\li FSEL_NOSUB:不包含子目录
 */
typedef enum
{
    FSEL_SUB = 1,
    FSEL_NOSUB = 2,
} fsel_subdir_t;

/*!
 *  \brief
 * 文件/目录类型的设定
 *\li FSEL_FILENUM:当前目录下有效文件总数
 *\li FSEL_DIRNUM:当前目录下目录总数
 */
typedef enum
{
    FSEL_FILENUM = 1,
    FSEL_DIRNUM = 2,
} fsel_itemtype_t;

/*!
 *  \brief
 * 设定获取文件/目录的顺序
 *\li FSEL_POSITIVE:顺序获取文件/目录
 *\li FSEL_CONVERSE:逆序获取文件/目录
 */
typedef enum
{
    FSEL_POSITIVE = 0,
    FSEL_CONVERSE = 1,
} fsel_listtype_t;

/*!
 *  \brief
 * 更新数据库中记录（文件/目录）
 *\li ADDDATA:增加文件/目录
 *\li DELDATA:删除文件/目录
 */
typedef enum
{
    FSEL_ADDDATA = 0,
    FSEL_DELDATA = 1,
} fsel_update_mode;

/*!
 *  \brief
 * 设定数据库查询模式
 *\li FSEL_CANCEL:取消数据库查询
 *\li FSEL_SUBDIR:查询当前目录及子目录
 *\li FSEL_CURRENTDIR:查询当前目录
 */
typedef enum
{
    FSEL_CANCEL = 0,
    FSEL_QUERY = 1,
} fsel_queryset_t;

/*! \cond fsel_api*/

FSEL_HANDLE fsel_open(void);

int fsel_close(FSEL_HANDLE Handle);

int fsel_setworkdir(FSEL_HANDLE Handle, char *pDirName);

const char * fsel_getworkdir(FSEL_HANDLE Handle);

int fsel_setfilter(FSEL_HANDLE Handle, char* Ext);

int fsel_getfilenum(FSEL_HANDLE Handle, fsel_itemtype_t itemtype);

int fsel_listfile(FSEL_HANDLE Handle, fsel_filename_t *file_name, int startno,
        int num, fsel_listtype_t type);

int fsel_listdir(FSEL_HANDLE Handle, fsel_filename_t *file_name, int startno,
        int num, fsel_listtype_t type);

int fsel_cd(FSEL_HANDLE Handle, char*pDirName);

int fsel_sortstyle(FSEL_HANDLE Handle, fsel_sortstyle_t style);

int fsel_getidbyname(FSEL_HANDLE Handle, char * pFullpath);

int fsel_update(FSEL_HANDLE Handle, char *path, fsel_update_mode flag);

int fsel_next_fileinfo(FSEL_HANDLE Handle, fsel_filename_t *file_name,
        fsel_subdir_t flag);

int fsel_prev_fileinfo(FSEL_HANDLE Handle, fsel_filename_t *file_name,
        fsel_subdir_t flag);

int fsel_set_query(FSEL_HANDLE Handle, char *pchar, fsel_queryset_t flag);

int fsel_subdir_setting(FSEL_HANDLE Handle, fsel_subdir_t flag,
        fsel_filenametype_t type);

int fsel_getfullpath(FSEL_HANDLE Handle, char* path, int num);

/*! \endcond*/

#endif
