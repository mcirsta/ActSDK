#ifndef __LIBC_EXTRA_H__
#define __LIBC_EXTRA_H__

#include <stdio.h>


#define PRINTF_LONG_SUPPORT

extern int fprintf(FILE *, const char *, ...);
extern int vfprintf(FILE *, const char *, va_list);

#endif /*__LIBC_EXTRA_H__ */
