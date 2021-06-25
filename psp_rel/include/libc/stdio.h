/*!\cond LIBC*/
/******************************************************************************
 *                              usdk 1100
 *                            Module:libc_fs_dir
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *      husanxi    2008-09-27 15:00     1.0             build this file
 ******************************************************************************/
/*!
 * \file     stdio.h
 * \brief    本文件包含了C库函数标准输入输出函数的宏定义和函数原型。
 * \author   husanxi
 * \version 1.0
 * \date  2008/9/27
 *****************************************************************************/
#ifndef __STDIO_H__
#define __STDIO_H__

#include <libc/types.h>
#include <libc/stddef.h>
#include <libc/ctype.h>
#include <libc/stdarg.h>
#include <libc/pthread.h>

#define SIZE_MAX  (0xffffffff)
/*!
 * \brief
 *      缓冲区大小
 */
#define BUFSIZ 4096

/*!
 * \brief
 *      fseek函数whence参数的类型，标志文件的起始位置
 */
#define SEEK_SET	0	/* Seek from beginning of file.  */
/*!
 * \brief
 *      fseek函数whence参数的类型，标志文件的当前位置
 */
#define SEEK_CUR	1	/* Seek from current position.  */
/*!
 * \brief
 *      fseek函数whence参数的类型，标志文件的结束位置
 */
#define SEEK_END	2	/* Seek from end of file.  */

/*!\cond LIBC_FS_FUNCTION*/
#define  FLAG_FBF  0x0000
#define FLAG_LBF 0x1000
#define  FLAG_NBF  0x2000
#define FREE_BUF 0x4000
#define FREE_FILE 0x8000
#define FREE_MUTEX 0x0800
#define  WRITING 0x0008
#define  READIND    0x0004
#define  WRITONLY  0x0002
#define  READONLY  0x0001
#define  FLAG_EOF   0x0010
#define  FLAG_ERROR 0x0020
#define  MODEMASK 0x0003

#define _IOFBF 0
#define _IOLBF  1
#define _IONBF 2

#define  BUF_SET_RO(s)  (void)((s)->_flag|=READONLY)
#define  BUF_SET_WO(s)  (void)((s)->_flag|=WRITONLY)
#define  BUF_SET_RDING(s)  (void)((s)->_flag|=READIND)
#define  BUF_SET_WRTING(s)  (void)((s)->_flag|=WRITING)
#define  BUF_SET_ERROR(s)  (void)((s)->_flag|=FLAG_ERROR)
#define  BUF_SET_EOF(s)   (void)((s)->_flag|=FLAG_EOF)
#define BUF_CLEAR_RDING(s) (void)((s)->_flag&=~READIND)
#define BUF_CLEAR_WRTING(s) (void)((s)->_flag&=~WRITING)
#define BUF_CLEAR_ERROR(s)  (void)((s)->_flag&=~FLAG_ERROR)
#define BUF_CLEAR_EOF(s)  (void)((s)->_flag&=~FLAG_EOF)
#define  BUF_IS_RDONLY(s) ((s)->_flag & READONLY)!=0
#define  BUF_IS_WRONLY(s) ((s)->_flag & WRITONLY)!=0
#define  BUF_IS_RDWR(s)  (!((s)->_flag & MODEMASK))!=0
#define  BUF_HAS_READING(s)  ((s)->_flag & READIND)!=0
#define  BUF_IS_WRITED(s)  ((s)->_flag & WRITING)!=0
#define  IS_NBF(s)  ((s)->_flag & _IONBF)!=0
#define  IS_FBF(s)  ((s)->_flag & _IOFBF)!=0
#define BUF_USED_SIZE(s) ((s)->_ptr-(s)->_base)

#ifndef offsetof
#define offsetof(TYPE, MEMBER)  (size_t) (&((TYPE *)0)->MEMBER)
#endif
/*!\endcond*/

#define LIBC_HAS_PTHREADS
/*!
 * \brief
 *      文件流结构体变量，由函数fopen、fdopen、freopen等函数返回，做为fread、
 *      fwrite等文件流函数的输入，用户不要随意更改结构体分量的值。
 */
typedef struct filestream
{
    unsigned char * _ptr; /* 缓冲中下一个字符位置指针 */
    size_t _cnt; /* 缓存中剩下的有效缓冲个数 */
    unsigned char * _base;/* 缓冲的基址*/
    size_t _flag;/*文件流的一些状态信息*/
    int _file;/* 文件句柄 */
    size_t _bufsiz;/* 缓冲的大小*/
    size_t _curpage;/*当前的缓存文件内容的页号*/
#ifdef LIBC_HAS_PTHREADS
    int _user_locking;
    pthread_mutex_t _lock;
#endif
#ifdef FULLUSED
int _charbuf;
char *_tmpfname;
#endif
} FILE;

#ifdef LIBC_FS_HAS_PTHREADS

#define __AUTO_THREADLOCK_VAR		int __infunc_user_locking

#define __AUTO_THREADLOCK(stream)  \
        __infunc_user_locking=(stream)->_user_locking; \
        if(__infunc_user_locking==0)   \
    	{                          \
    		pthread_mutex_lock(&((stream)->_lock));	\
    	}

#define __AUTO_THREADUNLOCK(stream)   \
        if(__infunc_user_locking==0)   \
    	{                          \
    		pthread_mutex_unlock(&((stream)->_lock));	\
    	}

#define __ALWAYS_THREADLOCK(stream) \
          pthread_mutex_lock(&((stream)->_lock));

#define __ALWAYS_THREADUNLOCK(stream) \
          pthread_mutex_unlock(&((stream)->_lock));

#define __SET_USER_LOCKING(stream) ((stream)->_user_locking)=1;

#else

#define __AUTO_THREADLOCK_VAR
#define __AUTO_THREADLOCK(stream)
#define __AUTO_THREADUNLOCK(stream)
#define __ALWAYS_THREADLOCK(stream)
#define __ALWAYS_THREADUNLOCK(stream)
#define __SET_USER_LOCKING(stream)

#endif

//enum
//{
//  /* 查询当前线程互斥状态，是用户负责互斥还是函数内部互斥*/
//  FSETLOCKING_QUERY = 0,
//#define FSETLOCKING_QUERY	FSETLOCKING_QUERY
//  /* 函数内部互斥，相当于调用flockfile().*/
//  FSETLOCKING_INTERNAL,
//#define FSETLOCKING_INTERNAL	 FSETLOCKING_INTERNAL
//  /* 用户负责互斥*/
//  FSETLOCKING_BYCALLER
//#define FSETLOCKING_BYCALLER	FSETLOCKING_BYCALLER
//};


/* 查询当前线程互斥状态，是用户负责互斥还是函数内部互斥*/
#define FSETLOCKING_QUERY	0
/* 函数内部互斥，相当于调用flockfile().*/
#define FSETLOCKING_INTERNAL 1
/* 用户负责互斥*/
#define FSETLOCKING_BYCALLER 2

#define EOF (-1)

extern FILE *fopen(const char * path, const char *mode);
extern int fclose(FILE *stream);
extern FILE *fdopen(int fd, const char *mode);
extern int feof(FILE *fd);
extern int fflush(FILE *fd);
extern size_t fread(void *ptr, size_t size, size_t nmemb, FILE *fd);
extern size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE * fd);
extern int fgetc(FILE *fd);
extern int fputc(int c, FILE *fd);
extern int fputs(const char *s, FILE *fd);
extern int fileno(FILE *fd);
extern int fseek(FILE *fd, long offset, int whence);
extern FILE *freopen(const char *path, const char *mode, FILE *fd);
extern long ftell(FILE *fd);
extern void rewind(FILE *fd);
extern void setbuf(FILE * stream, char *buf);
extern int ferror(FILE *fd);
extern void clearerr(FILE *fd);
extern char *fgets(char * s, int n, FILE * fd);
extern int fgetpos(FILE * fd, fpos_t *pos);
extern int fsetpos(FILE *fd, fpos_t pos);
extern int geterror(void);
extern void flockfile(FILE *fp);
extern void funlockfile(FILE *fp);
extern int __fsetlocking(FILE *stream, int locking_mode);

/* 非线程互斥的快速接口*/
extern void clearerr_unlocked(FILE *fd);
extern int feof_unlocked(FILE *fd);
extern int ferror_unlocked(FILE *fd);
extern int fflush_unlocked(FILE *fd);
extern int fgetc_unlocked(FILE *fd);
extern char *fgets_unlocked(char * s, int n, FILE * fd);
extern int fileno_unlocked(FILE *fd);
extern int fputc_unlocked(int c, FILE *fd);
extern int fputs_unlocked(const char *s, FILE *fd);
extern size_t fread_unlocked(void *ptr, size_t size, size_t nmemb, FILE *fd);
extern size_t fwrite_unlocked(const void *ptr, size_t size, size_t nmemb,
        FILE * fd);

extern int snprintf(char *restrict s, size_t n, const char *restrict format,
        ...);
extern int sprintf(char *restrict s, const char *restrict format, ...);
extern int sscanf(const char *restrict s, const char *restrict format, ...);
extern int vprintf(const char *restrict format, va_list ap);
extern int vscanf(const char *restrict format, va_list arg);
extern int vsnprintf(char *restrict s, size_t n, const char *restrict format,
        va_list ap);
extern int vsprintf(char *restrict s, const char *restrict format, va_list ap);
extern int vsscanf(const char *restrict s, const char *restrict format,
        va_list arg);
/*!\endcond*/

#endif /*__STDIO_H__*/

