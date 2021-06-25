/*
 ********************************************************************************
 *                            Noya1100 --- 设置项读写
 *                 Copyright(c) 2002-2009 Actions Semiconductor,
 *                        		All Rights Reserved.
 *
 * History:
 *       <author>    <time>      <version >       <desc>
 *       cheason     2009/02/20     1.0           build this file
 ********************************************************************************
 */

/*!
 * \file config_api.h
 * \brief  应用全局设置项读写
 * \author   cheason
 * \par GENERAL DESCRIPTION:
 *          通过查询及写入配置文件实现应用中全局设置项的读写
 * \par EXTERNALIZED FUNCTIONS:
 *          null
 * \version 1.0
 * \date 2009/02/20
 *
 */

#ifndef __CONFIG_API_H__
#define __CONFIG_API_H__

#include <libc/sys/limits.h>

/*! value的最大长度 */
#define CFG_MAX_BUFF_LENGTH PATH_MAX

/******************************************************************************/
/*!
 * \par  Description:
 *     获取设置键对应的值
 * \param[in]    key -- 设置键的字符串指针
 * \param[out]   buff -- 值缓冲指针
 * \param[in]    len -- 值缓冲的最大长度
 * \retval   成功则返回实际读取到的长度，失败则返回-1.
 * \par
 * \code
 *       如果返回-1(错误)，可能的情况包括：
 *       -- key, buff为空: errno = EINVAL
 *       -- 实际的值比缓冲的最大长度大: errno = ENOMEM
 *       -- 键值不存在: errno = CONFIG_KEY_NOT_FOUND
 * \endcode
 * \par Example:
 * \code
 *       char key[] = "VER";
 *       char value[30];
 *       int len = get_config(key, value, 30);
 *       if(len != -1)
 *       {
 *           printf("actual length is %d, value is: %s", len, value);
 *       }
 * \endcode
 *******************************************************************************/
int get_config(const char *key, char *buff, int len);

/******************************************************************************/
/*!
 * \par  Description:
 *     设置键的值
 * \param[in]    key -- 设置键的字符串指针
 * \param[in]    buf -- 值缓冲指针
 * \param[in]    len -- 设置的值的长度
 * \retval   成功则返回0，失败则返回-1.
 * \par
 * \code
 *       如果返回-1(错误)，可能的情况包括：
 *       -- key, buff为空: errno = EINVAL
 *       -- 磁盘空间不足: errno = ENOSPC
 *       -- 设置的值长度超过最大限制值: errno = CONFIG_EXCEED_MAX_LENGTH
 * \endcode
 * \par Example:
 * \code
 *       char key[] = "VER";
 *       char value[] = "ACTIONS_2009_02_20";
 *       if(set_config(key, value, strlen(value) + 1) != -1)
 *       {
 *           printf("set successfully!");
 *       }
 * \endcode
 *******************************************************************************/
int set_config(const char * key, const char * buf, int len);

/******************************************************************************/
/*!
 * \par  Description:
 *     恢复键值默认设置
 * \param[in]    key -- 键的字符串指针
 * \retval   成功则返回0，失败则返回-1.
 * \par
 * \code
 *       如果返回-1(错误)，可能的情况包括：
 *       -- key为空: errno = EINVAL
 *       -- 默认键值不存在: errno = CONFIG_KEY_NOT_FOUND
 * \endcode
 * \par Example:
 * \code
 *       if(reset_config() != -1)
 *       {
 *           printf("reset successfully\n");
 *       }
 * \endcode
 *******************************************************************************/
int reset_config(const char *key);

/******************************************************************************/
/*!
 * \par  Description:
 *     恢复所有键值的默认设置
 * \retval   成功则返回0，失败则返回-1.
 * \par Example:
 * \code
 *       if(reset_all_config() != -1)
 *       {
 *           printf("reset successfully\n");
 *       }
 * \endcode
 *******************************************************************************/
int reset_all_config(void);

/******************************************************************************/
/*!
 * \par  Description:
 *     清除DRM HDS区
 * \param[in]    vram -- vram设备节点(一般为"/dev/vram")
 * \param[in]    path -- hds文件路径 (一般为"/mnt/sdisk/HDSFrame.hds")
 * \retval   成功则返回0，失败则返回-1.
 * \par
 * \code
 *       如果返回-1(错误)，可能的情况包括：
 *       -- vram, path为空: errno = EINVAL
 *       -- 磁盘空间不足: errno = ENOSPC
 * \endcode
 * \par Example:
 * \code
 *        int rc = clear_hds("/dev/vram", "/mnt/sdisk/HDSFrame.hds");
 *        if (rc < 0)
 *        {
 *            printf("Clear HDS region fail: %d\n", rc);
 *            goto fail;
 *        }
 * \endcode
 *******************************************************************************/
int clear_hds(const char *vram, const char *path);

/******************************************************************************/
/*!
 * \par  Description:
 *      配置文件校验（校验失败则表明文件已损坏）
 * \retval   校验成功则返回0，失败则返回-1.
 * \par Example:
 * \code
 *       if(!check_config_bin())
 *       {
 *           printf("check successfully\n");
 *       }
 * \endcode
 *******************************************************************************/
int check_config_bin(void);

/******************************************************************************/
/*!
 * \par  Description:
 *     缓存中的配置同步到config.bin
 * \retval   成功则返回0，失败则返回-1.
 * \par Example:
 * \code
 *       if(!sync_config())
 *       {
 *           printf("sync successfully\n");
 *       }
 * \endcode
 *******************************************************************************/
int sync_config(void);

/******************************************************************************/
/*!
 * \par  Description:
 *     flash表回写
 * \retval   成功则返回0，失败则返回-1.
 * \par Example:
 * \code
 *       if(!nandflash_update())
 *       {
 *           printf("write back successfully\n");
 *       }
 * \endcode
 *******************************************************************************/
int nandflash_update(void);

#endif // ifndef __CONFIG_API_H__
