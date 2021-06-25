/********************************************************************************
 *                              USDK(1100)
 *                            Module: SYSCALL
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-8-30 16:44     1.0             build this file
 ********************************************************************************/
/*!
 * \file     syscalls.h
 * \brief    系统调用接口函数声明
 * \author   hmwei
 * \version 1.0
 * \date  2008/8/30
 *******************************************************************************/
#ifndef __SYSCALLS_GENERAL_H__
#define __SYSCALLS_GENERAL_H__

//#include <ucos/vfs.h>
#include <ucos/types.h>
#include <ucos/stat.h>
#include <ucos/utime.h>

extern long sys_mount(char * dev_name, char * dir_name, char * type,
        unsigned long flags, void * data);
extern long sys_umount(char * name, int flags);
extern long sys_mknod(const char * filename, int mode, unsigned dev);

extern long sys_open(const char *filename, int flags, int mode);
extern long sys_close(unsigned int fd);
extern ssize_t sys_read(unsigned int fd, char * buf, size_t count);
extern ssize_t sys_write(unsigned int fd, const char * buf, size_t count);
extern off_t sys_lseek(unsigned int fd, off_t offset, unsigned int origin);
extern long sys_fcntl(unsigned int fd, unsigned int cmd, unsigned long arg);
extern long sys_creat(const char * pathname, int mode);
extern long sys_rename(const char * oldname, const char * newname);
extern long sys_chdir(const char * filename);
extern long sys_getcwd(char *buf, unsigned long size);
extern long sys_mkdir(const char * pathname, int mode);
extern long sys_rmdir(const char * pathname);

extern long sys_fstat(unsigned int fd, struct stat * statbuf);
extern long sys_stat(char * filename, struct stat * statbuf);
extern long sys_statfs(const char * path, struct statfs * buf);
extern long sys_fstatfs(unsigned int fd, struct statfs * buf);

extern long sys_utime(char * filename, struct utimbuf * times);

extern long sys_truncate(const char * path, off_t length);
extern long sys_ftruncate(unsigned int fd, off_t length);

extern long sys_fsync(unsigned int fd);
extern long sys_fchdir(unsigned int fd);
extern long sys_flock(unsigned int fd, unsigned int cmd);

extern int sys_remove(const char *pathname);
extern loff_t sys_tell(int dir);
extern int sys_readdir(int dir, off_t pos, void *ent, int len);
extern int sys_seekdir(int dir, loff_t offset);
extern int sys_rewinddir(int dir);
extern int sys_lastdir(int fd);
extern int sys_prevdir(int fd, off_t pos, void *buf, int len);
extern int sys_reset2parentdir(int fd);

extern long sys_ioctl(unsigned int fd, unsigned int cmd, unsigned long arg);

extern long sys_msync(unsigned long start, size_t len, int flags);
extern unsigned long sys_mmap(unsigned long addr, size_t len, int prot,
        int flags, int fd, off_t offset);

extern long sys_munmap(unsigned long addr, size_t len);

extern long insmod(const char *name_mod, void *arg);
extern long rmmod(const char *name_mod, unsigned int flags);

extern int fork(void);
extern int _execve(const char *file, char * const *argv, char * const *env);
extern void _exit(int entry);

#endif /*__SYSCALLS_GENERAL_H__*/
