/*!\cond LIBC*/
/*****************************************************************************
 *                              usdk 1100
 *                            Module:libc_fs_dir
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *      husanxi    2008-08-27 15:00     1.0             build this file
 *      wurui      2008-12-31 17:00     1.1             add process call
 *****************************************************************************/
/*!
 * \file     unistd.h
 * \brief    本文件包含了C库函数中系统相关部分的宏定义和函数原型定义。
 * \author   husanxi
 * \version 1.0
 * \date  2008/9/27
 ****************************************************************************/
#ifndef __UNISTD_H__
#define __UNISTD_H__
#include <libc/types.h>

/*!
 * \brief
 *      文件解锁
 */
#define F_ULOCK     1

/*!
 * \brief
 *      文件上锁
 */
#define F_LOCK      2

/*!
 * \brief
 *      测试文件是否已经上锁，如没有上锁则对齐上锁
 */
#define F_TLOCK     4

/*!
 * \brief
 *      测试文件是否已经上锁
 */
#define F_TEST      8

extern int open(const char *pathname, int flags, ...);
extern int close(int fd);
extern int read(int fd, void *buf, unsigned int count);
extern int write(int fd, const void *buf, unsigned int count);
extern off_t lseek(int fd, off_t offset, int whence);
extern int chdir(const char *path);
extern char *getcwd(char *buf, unsigned int size);
extern int fchdir(int fd);
extern int fsync(int fd);
extern int truncate(const char * path, off_t length);
extern int ftruncate(int fd, off_t length);
extern int remove(const char *pathname);
extern int rename(const char *oldpath, const char *newpath);
extern int rmdir(const char *path);

extern int execl(const char *path, const char *arg0, ...);
extern int
        execle(const char *path, const char *arg0, ... /*, char * const *envp */);
extern int execlp(const char *file, const char *arg0, ...);
extern int execv(const char *path, char * const *argv);
extern int execve(const char *path, char * const *argv, char * const *envp);
extern int execvp(const char *file, char * const *argv);
extern int kill(pid_t pid, int sig);
extern pid_t getpid(void);
extern unsigned sleep(unsigned seconds);
extern int usleep(useconds_t useconds);
#endif
/*!\endcond*/
