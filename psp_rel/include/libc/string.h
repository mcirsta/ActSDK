/********************************************************************************
 *                              NOYA 1100
 *                            Module: libc_sysindep_api
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       rcmai     2008-00-01 15:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     string.h
 * \brief    字符串处理函数，以及其他处理字节数组的函数。这些函数均以str开始
 *			(比如strcpy()),或以mem开始(例如memcpy())。
 * \author   lbkang
 * \version  version 1.0
 * \date  2008/08/04
 *******************************************************************************/
#ifndef _STRING_H_
#define _STRING_H_

#include <libc/stdio.h>
#include <libc/ctype.h>
#include <libc/stddef.h>
/*!
 * \brief
 *     根据系统需求定义ssize_t，这里将ssize_t类型定义为signed int类型。
 * \li  ssize_t: signed int
 */
#define ssize_t signed int

extern void *memccpy(void *restrict s1, const void *restrict s2, int c,
        size_t n);
extern void *memchr(const void *s, int c, size_t n);
extern void *memcpy(void *restrict s1, const void *restrict s2, size_t n);
extern void *memmove(void *s1, const void *s2, size_t n);
extern void *memset(void *s, int c, size_t n);
extern char *strcat(char *restrict s1, const char *restrict s2);
extern char *strchr(const char *s, int c);
extern char *strcpy(char *restrict s1, const char *restrict s2);

extern char *strdup(const char *s1);
extern char *strerror(int errnum);
extern char *strncat(char *restrict s1, const char *restrict s2, size_t n);
extern char *strncpy(char *restrict s1, const char *restrict s2, size_t n);
extern char *strpbrk(const char *s1, const char *s2);
extern char *strrchr(const char *s, int c);

extern char *strstr(const char *s1, const char *s2);
extern char *strtok(char *restrict s1, const char *restrict s2);
extern char *strtok_r(char *restrict s, const char *restrict sep,
        char **restrict lasts);
extern char *index(const char *s, int c);
extern char *rindex(const char *s, int c);
extern void bcopy(const void *s1, void *s2, size_t n);
extern void bzero(void *s, size_t n);

extern size_t strcspn(const char *s1, const char *s2);
extern size_t strlen(const char *s);
extern size_t strspn(const char *s1, const char *s2);
extern size_t strxfrm(char *restrict s1, const char *restrict s2, size_t n);

extern int strncmp(const char *s1, const char *s2, size_t n);
extern int bcmp(const void *s1, const void *s2, size_t n);
extern int strerror_r(int errnum, char *strerrbuf, size_t buflen);
extern int strcmp(const char *s1, const char *s2);
extern int strcoll(const char *s1, const char *s2);
extern int memcmp(const void *s1, const void *s2, size_t n);
extern int ffs(int i);
extern int strcasecmp(const char *s1, const char *s2);
extern int strncasecmp(const char *s1, const char *s2, size_t n);
/*
 long simple_strtol(const char *cp,char **endp,unsigned int base);
 unsigned long simple_strtoul(const char *cp,char **endp,unsigned int base);
 */
extern const int sys_nerr;
extern const char *sys_errlist[];

#endif /* _STRING_H_ */
