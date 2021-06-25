/********************************************************************************
 *                              USDK(1100)
 *                            Module: STAT
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-8-30 16:44     1.0             build this file
 ********************************************************************************/
/*!
 * \file     stat.h
 * \brief    文件/文件系统状态相关的宏定义
 * \author   hmwei
 * \version 1.0
 * \date  2008/8/30
 *******************************************************************************/
#ifndef __OS_STAT_H__
#define __OS_STAT_H__

//	#ifdef __KERNEL__

//	#include <asm-mips/stat.h>

//	#endif

//	#if defined(__KERNEL__) || !defined(__GLIBC__) || (__GLIBC__ < 2)

#define S_IFMT  00170000
//	#define S_IFSOCK 0140000
#define S_IFLNK	 0120000
#define S_IFREG  0100000
#define S_IFBLK  0060000
#define S_IFDIR  0040000
#define S_IFCHR  0020000
//	#define S_IFIFO  0010000
//	#define S_ISUID  0004000
//	#define S_ISGID  0002000
//	#define S_ISVTX  0001000

//	#define S_ISLNK(m)	(((m) & S_IFMT) == S_IFLNK)
#define S_ISREG(m)	(((m) & S_IFMT) == S_IFREG)
#define S_ISDIR(m)	(((m) & S_IFMT) == S_IFDIR)
#define S_ISCHR(m)	(((m) & S_IFMT) == S_IFCHR)
#define S_ISBLK(m)	(((m) & S_IFMT) == S_IFBLK)
//	#define S_ISFIFO(m)	(((m) & S_IFMT) == S_IFIFO)
//	#define S_ISSOCK(m)	(((m) & S_IFMT) == S_IFSOCK)

#define S_IRWXU 0000700	/* rwx, owner */
#define S_IRUSR	0000400	/* read permission, owner */
#define S_IWUSR	0000200	/* write permission, owner */
#define S_IXUSR	0000100	/* execute/search permission, owner */

#define S_IRWXG 0000070	/* rwx, group */
#define S_IRGRP	0000040	/* read permission, group */
#define S_IWGRP	0000020	/* write permission, grougroup */
#define S_IXGRP	0000010	/* execute/search permission, group */

#define S_IRWXO 0000007	/* rwx, other */
#define S_IROTH	0000004	/* read permission, other */
#define S_IWOTH	0000002	/* write permission, other */
#define S_IXOTH	0000001	/* execute/search permission, other */

//	#endif
//
//	#ifdef __KERNEL__
#define S_IRWXUGO	(S_IRWXU|S_IRWXG|S_IRWXO)
#define S_IALLUGO	(S_ISUID|S_ISGID|S_ISVTX|S_IRWXUGO)
#define S_IRUGO		(S_IRUSR|S_IRGRP|S_IROTH)
#define S_IWUGO		(S_IWUSR|S_IWGRP|S_IWOTH)
#define S_IXUGO		(S_IXUSR|S_IXGRP|S_IXOTH)

#include <ucos/types.h>

#ifndef _ASSEMBLER_

/*!
 * \brief
 *      文件状态结构定义
 */
typedef struct stat
{
    /*! 文件所在的设备的编号*/
    unsigned int st_dev;
    /*! 文件序列号*/
    unsigned int st_ino;
    /*! 文件的类型和存储权限*/
    unsigned int st_mode;
    /*! 连接该文件的硬连接数目*/
    unsigned int st_nlink;
    /*! 文件所有者的用户识别码*/
    unsigned int st_uid;
    /*! 文件所有者的组识别码*/
    unsigned int st_gid;
    /*! 如果是设备为设备号*/
    unsigned int st_rdev;
    /*! 文件大小，单位byte*/
    unsigned int st_size;
    /*! 文件系统块I/O缓冲区大小*/
    unsigned long st_blksize;
    /*! 文件占用文件区块的个数，区大小为512byte*/
    unsigned long st_blocks;
    /*! 文件最近一次被读取或被执行的时间*/
    vfs_time_t st_atime;
    /*! 文件最后一次数据被更改的时间*/
    vfs_time_t st_mtime;
    /*! 文件最后一次修改属性的时间*/
    vfs_time_t st_ctime;
} stat_t;

/*!
 * \brief
 *      文件系统状态结构定义
 */
typedef struct statfs
{
    /*! 文件系统类型*/
    unsigned int f_type;
    /*! 块大小或簇大小*/
    unsigned int f_bsize;
    /*! 基本文件系统块大小*/
    unsigned int f_frsize;
    /*! 总共含有多少个数据块，或含有多少个簇*/
    unsigned int f_blocks;
    /*! 总共没有使用的数据块，或没有使用的簇*/
    unsigned int f_bfree;
    /*! 空闲块可用于普通文件的个数*/
    unsigned int f_bavail;
    /*! 文件序号总数*/
    unsigned int f_files;
    /*! 空闲文件序号总数*/
    unsigned int f_ffree;
    /*! 普通文件还可以使用的空文件*/
    unsigned int f_favail;
    /*! 文件系统id*/
    unsigned int f_fsid;
    /*! 文件系统flag*/
    unsigned int f_flag;
    /*! 文件系统允许的文件名最大长度*/
    unsigned int f_namemax;
} statfs_t;
//	#endif
#endif /*_ASSEMBLER_*/
#endif	/*__OS_STAT_H__*/
