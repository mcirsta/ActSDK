/*!\cond LIBC*/
/****************************************************************************
 *                              usdk 1100
 *                            Module:libc_fs_dir
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *      husanxi    2008-09-27 15:00     1.0             build this file
 ****************************************************************************/
/*!
 * \file     dirent.h
 * \brief    本文件包含了C库函数中目录流函数的结构体定义以及函数原型。
 * \author   husanxi
 * \version 1.0
 * \date  2008/9/27
 ****************************************************************************/
#ifndef __DIRENT_H__
#define __DIRENT_H__

#include <libc/types.h>
#include <libc/pthread.h>
#include <libc/stdio.h>

#define FORWARD 0x01
#define BACKWARD 0x02
#define MAX_INT  0x7fffffff
#define  DIR_SET_FORWARD(s) ((s)->dd_flag |= FORWARD)
#define  DIR_CLEAR_FORWARD(s) ((s)->dd_flag &= ~FORWARD)
#define  DIR_SET_BACKWARD(s) ((s)->dd_flag|= BACKWARD)
#define  DIR_CLEAR_BACKWARD(s) ((s)->dd_flag &= ~BACKWARD)
#define  DIR_IS_FORWARD(s) ((s)->dd_flag & FORWARD)!=0
#define  DIR_IS_BACKWARD(s) ((s)->dd_flag & BACKWARD)!=0

/*!
 * \brief
 *      该位置位表示该目录项对应的文件是只读的
 */
#define ATTR_READONLY       0x0001
/*!
 * \brief
 *      该位置位表示该目录项对应的文件是隐藏的
 */
#define ATTR_HIDDEN         0x0002
/*!
 * \brief
 *      该位置位表示该目录项对应的文件是系统文件
 */
#define ATTR_SYSTEM         0x0004
/*!
 * \brief
 *      该位置位表示该目录项对应卷标
 */
#define ATTR_VOLUME         0x0008
/*!
 * \brief
 *      该位置位表示该目录项对应的是子目录
 */
#define ATTR_DIRECTORY      0x0010
/*!
 * \brief
 *      该位置位表示该目录项对应文件为存档文件
 */
#define ATTR_ARCHIVE        0x0020

/*!
 * \brief
 *      目录流结构体变量，有函数opendir返回，做为readdir，prevdir等目录流函数
 *      的输入，用户不能随意更改结构体分量的值。
 */
typedef struct dirstream
{
    /*目录文件句柄*/
    int dd_fd;

    /* 目录流旗标*/
    unsigned char dd_flag;

    /*目录项在缓冲中的偏移*/
    size_t dd_nextloc;

    /* 目录流缓冲*/
    void *dd_buf;

    /*目录项在目录文件中的偏移*/
    off_t dd_nextoff;

    /* 缓冲中总的目录项数目*/
    ssize_t dd_totalNum;

    /* 当前目录项在缓冲的序号*/
    ssize_t dd_curNum;

    /* 目录流缓冲的大小*/
    ssize_t dd_max;

#ifdef LIBC_HAS_PTHREADS
/*线程互斥锁*/
pthread_mutex_t dd_lock;
#endif
} DIR;

/*!
 * \brief
 *      目录项结构体定义
 */
typedef struct dirent
{
    /*! 文件序列号*/
    unsigned int d_ino;

    /*! 目录头到该文件目录项的偏移量*/
    unsigned int d_off;

    /*! 文件修改时间 */
    unsigned int d_mtime;

    /*! 该记录的总长度,包含记录头和文件名.32b对齐*/
    unsigned short int d_reclen;

    /*! 文件类型属性，位定义如下：
     * \n ATTR_READONLY置位:只读文件
     * \n ATTR_HIDDEN置位:隐藏文件
     * \n ATTR_SYSTEM置位：系统文件
     * \n ATTR_VOLUME置位:卷标文件
     * \n ATTR_DIRECTORY置位:目录文件
     * \n ATTR_ARCHIVE置位:存档文件 */
    unsigned char d_attrs;

    /*! 文件名大小写标示位*/
    unsigned char d_lcase;

    /*! 文件名,可变长.长度为 = 记录长度(d_reclen) -
     offsetof(struct dirent, d_name)
     */
    char d_name[1000];
} dirent_t;

extern DIR *opendir(const char *name);
extern struct dirent *readdir(DIR *dir);
extern void seekdir(DIR *dir, off_t offset);
extern off_t telldir(DIR *dir);
extern void rewinddir(DIR *dir);
extern int closedir(DIR *dir);
extern int lastdir(DIR *fd);
extern struct dirent *prevdir(DIR *fd);
extern int reset2parentdir(DIR *fd);
/*!\endcond*/
#endif/*__DIRENT_H__*/
