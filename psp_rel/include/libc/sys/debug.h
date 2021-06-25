
#ifndef __SYS_DEBUG_H__
#define __SYS_DEBUG_H__

/* 是否打开libc_generel的管道debug功能 */
#define LIBC_PIPE_DEBUG_ON 1
#if LIBC_PIPE_DEBUG_ON > 0
#define LIBC_PIPE_DEBUG_MAX_REF_NUM 10
#endif

#endif /* #ifndef __SYS_DEBUG_H__ */
