/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_playlist.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason    2009-06-14         v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_playlist.c
 * \brief    routine of photo playlist
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/06/14
 *******************************************************************************/

#ifndef __PHOTO_PLAYLIST_H__
#define __PHOTO_PLAYLIST_H__

#include "photo.h"

/*
 * 打开初始化playlist
 * 参数：无
 * 返回值：成功返回true，失败返回false
 */
bool photo_playlist_init(photo_plist_mode_e mode);

/*
 * 关闭playlist
 * 参数：无
 * 返回值：成功返回true，失败返回false
 */
bool photo_playlist_deinit(void);

/*
 * 检查播放列表是否初始化错误
 * 参数：无
 * 返回值：true--有错误，false-无错误
 */
bool photo_playlist_check_init_error(void);

/*
 * 添加目录下（包含子目录）所有文件到playlist
 * 参数
 *   dir: 目录全路径名
 *   device: 设备名（SYSTEMDISK, CARD, EXTERNAL)
 * 返回值：成功返回true，失败返回false
 */
bool photo_playlist_add_dir(const char *dir);

/*
 * 添加文件选择器中的文件到playlist
 * 参数
 *   dir: 目录全路径名
 *   device: 设备名（SYSTEMDISK, CARD, EXTERNAL)
 * 返回值：成功返回true，失败返回false
 */
bool photo_playlist_add_fsel(FSEL_HANDLE fsel);

/*
 * 添加单个文件到playlist
 * 参数
 *   fullpath: 文件全路径名
 *   device: 设备名（SYSTEMDISK, CARD, EXTERNAL)
 * 返回值：成功返回true，失败返回false
 */
bool photo_playlist_add_file(const char *fullpath);

/*
 * 添加选定文件所在目录下（包含子目录）所有文件到playlist
 * 参数
 *   fullpath: 文件全路径名
 *   device: 设备名（SYSTEMDISK, CARD, EXTERNAL)
 * 返回值：成功返回true，失败返回false
 */
bool photo_playlist_add_file_dir(const char *fullpath);


/*
 * 获取当前playlist中文件个数
 * 参数：无
 * 返回值：当前playlist中文件个数
 */
int photo_playlist_get_num(void);

/*
 * 获取文件在playlist中的index
 * 参数
 *   fullpath: 文件全路径名
 *   device: 设备名（SYSTEMDISK, CARD, EXTERNAL)
 * 返回值
 *   -1: playlist没有这个文件
 *   >=0: 文件在playlist中的index
 */
int photo_playlist_get_file_index(const char *fullpath);

/*
 * 获取文件详细信息
 * 参数
 *   info: 保存文件信息结构体指针，函数里面分配空间，使用完之后必须调用photo_playlist_free_detail_info
 *   start: 起始index
 *   num: 文件个数
 * 返回值：成功返回true，失败返回false
 */
bool photo_playlist_get_detail_info(plist_detail_info_t *info, int start, int num);

/*
 * 释放文件详细信息内容，必须跟photo_playlist_get_detail_info成对出现
 * 参数
 *   info: 文件信息结构体指针
 *   num: 文件个数
 * 返回值：无
 */
void photo_playlist_free_detail_info(plist_detail_info_t *info, int num);

/*
 * 从playlist中删除若干个文件
 * 参数
 *   start: 开始index
 *   num: 文件个数，-1则删除所有文件，不管start值
 * 返回值：成功返回true，失败返回false
 */
bool photo_playlist_del_items(int start, int num);

/*
 * 根据文件路径从playlist中删该文件
 * 参数
 *   path: 文件路径
 * 返回值：成功返回true，失败返回false
 */
bool photo_playlist_del_item_path(const char *path);

#endif

