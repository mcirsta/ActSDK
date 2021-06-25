/********************************************************************************
 *                              USDK(1100)
 *                            Module: STRING
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-8-30 16:44     1.0             build this file
 ********************************************************************************/
/*!
 * \file     string.h
 * \brief    �ַ���������������
 * \author   open source
 * \version 1.0
 * \date  2008/8/30
 *******************************************************************************/
#ifndef __OS_STRING_H__
#define __OS_STRING_H__

#include <ucos/types.h>	/* for size_t */

#ifdef __cplusplus
extern "C"
{
#endif

extern char * strpbrk(const char *, const char *);
extern char * strsep(char **, const char *);
extern size_t strspn(const char *, const char *);
extern size_t strcspn(const char *, const char *);

/*
 * Include machine specific inline routines
 */

#ifndef __HAVE_ARCH_STRCPY
extern char * strcpy(char *, const char *);
#endif
#ifndef __HAVE_ARCH_STRNCPY
extern char * strncpy(char *, const char *, size_t);
#endif
#ifndef __HAVE_ARCH_STRLCPY
size_t strlcpy(char *, const char *, size_t);
#endif
#ifndef __HAVE_ARCH_STRCAT
extern char * strcat(char *, const char *);
#endif
#ifndef __HAVE_ARCH_STRNCAT
extern char * strncat(char *, const char *, size_t);
#endif
#ifndef __HAVE_ARCH_STRLCAT
extern size_t strlcat(char *, const char *, size_t);
#endif
#ifndef __HAVE_ARCH_STRCMP
extern int strcmp(const char *, const char *);
#endif
#ifndef __HAVE_ARCH_STRNCMP
extern int strncmp(const char *, const char *, size_t);
#endif
#ifndef __HAVE_ARCH_STRNICMP
extern int strnicmp(const char *, const char *, size_t);
#endif
#ifndef __HAVE_ARCH_STRCHR
extern char * strchr(const char *, int);
#endif
#ifndef __HAVE_ARCH_STRNCHR
extern char * strnchr(const char *, size_t, int);
#endif
#ifndef __HAVE_ARCH_STRRCHR
extern char * strrchr(const char *, int);
#endif
#ifndef __HAVE_ARCH_STRSTR
extern char * strstr(const char *, const char *);
#endif
#ifndef __HAVE_ARCH_STRLEN
extern size_t strlen(const char *);
#endif
#ifndef __HAVE_ARCH_STRNLEN
extern size_t strnlen(const char *, size_t);
#endif

#ifndef __HAVE_ARCH_MEMSET
extern void * memset(void *, int, size_t);
#endif
#ifndef __HAVE_ARCH_MEMCPY
extern void * memcpy(void *, const void *, size_t);
#endif
#ifndef __HAVE_ARCH_MEMMOVE
extern void * memmove(void *, const void *, size_t);
#endif
#ifndef __HAVE_ARCH_MEMSCAN
extern void * memscan(void *, int, size_t);
#endif
#ifndef __HAVE_ARCH_MEMCMP
extern int memcmp(const void *, const void *, size_t);
#endif
#ifndef __HAVE_ARCH_MEMCHR
extern void * memchr(const void *, int, size_t);
#endif

//extern char *kstrdup(const char *s, gfp_t gfp);

#ifdef __cplusplus
}
#endif

#endif /* __OS_STRING_H__ */
