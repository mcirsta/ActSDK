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
 * \brief    �ļ�/�ļ�ϵͳ״̬��صĺ궨��
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
 *      �ļ�״̬�ṹ����
 */
typedef struct stat
{
    /*! �ļ����ڵ��豸�ı��*/
    unsigned int st_dev;
    /*! �ļ����к�*/
    unsigned int st_ino;
    /*! �ļ������ͺʹ洢Ȩ��*/
    unsigned int st_mode;
    /*! ���Ӹ��ļ���Ӳ������Ŀ*/
    unsigned int st_nlink;
    /*! �ļ������ߵ��û�ʶ����*/
    unsigned int st_uid;
    /*! �ļ������ߵ���ʶ����*/
    unsigned int st_gid;
    /*! ������豸Ϊ�豸��*/
    unsigned int st_rdev;
    /*! �ļ���С����λbyte*/
    unsigned int st_size;
    /*! �ļ�ϵͳ��I/O��������С*/
    unsigned long st_blksize;
    /*! �ļ�ռ���ļ�����ĸ���������СΪ512byte*/
    unsigned long st_blocks;
    /*! �ļ����һ�α���ȡ��ִ�е�ʱ��*/
    vfs_time_t st_atime;
    /*! �ļ����һ�����ݱ����ĵ�ʱ��*/
    vfs_time_t st_mtime;
    /*! �ļ����һ���޸����Ե�ʱ��*/
    vfs_time_t st_ctime;
} stat_t;

/*!
 * \brief
 *      �ļ�ϵͳ״̬�ṹ����
 */
typedef struct statfs
{
    /*! �ļ�ϵͳ����*/
    unsigned int f_type;
    /*! ���С��ش�С*/
    unsigned int f_bsize;
    /*! �����ļ�ϵͳ���С*/
    unsigned int f_frsize;
    /*! �ܹ����ж��ٸ����ݿ飬���ж��ٸ���*/
    unsigned int f_blocks;
    /*! �ܹ�û��ʹ�õ����ݿ飬��û��ʹ�õĴ�*/
    unsigned int f_bfree;
    /*! ���п��������ͨ�ļ��ĸ���*/
    unsigned int f_bavail;
    /*! �ļ��������*/
    unsigned int f_files;
    /*! �����ļ��������*/
    unsigned int f_ffree;
    /*! ��ͨ�ļ�������ʹ�õĿ��ļ�*/
    unsigned int f_favail;
    /*! �ļ�ϵͳid*/
    unsigned int f_fsid;
    /*! �ļ�ϵͳflag*/
    unsigned int f_flag;
    /*! �ļ�ϵͳ������ļ�����󳤶�*/
    unsigned int f_namemax;
} statfs_t;
//	#endif
#endif /*_ASSEMBLER_*/
#endif	/*__OS_STAT_H__*/
