/********************************************************************************
 *                               USDK(1100)
 *                            Module: CONSOLE
 *                 Copyright(c) 2001-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-6-12 9:42     1.0             build this file
 ********************************************************************************/
/*!
 * \file     console.h
 * \brief    可变参数列表相关宏定义
 * \author   hmwei
 * \version 1.0
 * \date  2008/6/12
 *******************************************************************************/
/**
 * extracted from cygwin stdarg.h for simplicity
 */
#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#ifndef _ASSEMBLER_
/* Define __gnuc_va_list.  */

#ifndef __GNUC_VA_LIST
#define __GNUC_VA_LIST
typedef __builtin_va_list __gnuc_va_list;
#endif

typedef __gnuc_va_list va_list;

/* Note that the type used in va_arg is supposed to match the
 actual type **after default promotions**.
 Thus, va_arg (..., short) is not valid.  */

#define va_start(v,l)	__builtin_stdarg_start((v),l)
#define va_end		__builtin_va_end
#define va_arg		__builtin_va_arg

#define __libc	  //__attribute__ ((__section__ (".libc")))
int vsprintf(char *buf, const char *fmt, va_list args)__libc;
int sprintf(char * buf, const char *fmt, ...)__libc;

char *strswab(const char *s);
unsigned long simple_strtoul(const char *cp, char **endp, unsigned int base);
long simple_strtol(const char *cp, char **endp, unsigned int base);

#endif /*_ASSEMBLER_*/
#endif  /*__CONSOLE_H__*/
