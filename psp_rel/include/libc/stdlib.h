/********************************************************************************
 *                              NOYA 1100
 *                            Module: libc_sysindep_api
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       wurui     2009-01-04          1.0             build this file
 ********************************************************************************/
/*!
 * \file     stdlib.h
 * \brief    定义通用类型,变量以及函数
 * \author   wurui
 * \version  version1.0
 * \date  2009/01/04
 *******************************************************************************/
#ifndef _STDLIB_H_
#define _STDLIB_H_

#include "syscalls.h"
#include <libc/stddef.h>
#include <libc/string.h>
#include <libc/ctype.h>

#define	INT_MAX	2147483647
#define	INT_MIN	(-INT_MAX - 1)

#define	LONG_MIN	INT_MIN
#define	LONG_MAX	INT_MAX

#define	ULONG_MAX	4294967295U

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

extern int abs(int i);
extern int atoi(const char *str);
extern long atol(const char *str);
extern void qsort(void *base, size_t nel, size_t width, int(*compar)(
        const void *, const void *));
extern int rand(void);
extern int rand_r(unsigned *seed);
extern long random(void);
extern void srand(unsigned seed);
extern void srandom(unsigned seed);
extern unsigned long strtoul(const char *restrict str, char **restrict endptr,
        int base);
extern long strtol(const char *restrict str, char **restrict endptr, int base);

extern void exit(int status);
extern char *getenv(const char *name);
extern int putenv(char *string);
extern int setenv(const char *name, const char *value, int rewrite);
extern int unsetenv(const char *name);
extern int atexit(void(*func)(void));

#endif/**/
