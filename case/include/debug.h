/********************************************************************************
*                              USDK(1100)
*                             Module: app 
*                 Copyright(c) 2003-2008 Actions Semiconductor,
*                            All Rights Reserved. 
* file : debug.h
* History:         
*      <author>    <time>           <version >             <desc>
*      Herb Hu    2009-3-3             v1.0             build this file 
********************************************************************************/

#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <libc.h>


#ifndef PRINT_ERR
#define PRINT_ERR   1
#endif

#ifndef PRINT_WARN
#define PRINT_WARN   1
#endif

#ifndef PRINT_DBG
#define PRINT_DBG   1
#endif

#ifndef PRINT_INFO
#define PRINT_INFO   1
#endif

#ifndef PRINT_TIME
#define PRINT_TIME   1
#endif


#if PRINT_ERR == 1
    #define print_err(format, arg...)   do { printf("ERROR(errno=%d)-pid:%d-%s,%d,%s--- "format"\n",\
                                                    errno,getpid(),__FILE__,__LINE__,__func__, ## arg);} \
                                        while (0)
#else
    #define print_err(format, arg...)   do {} while (0)
#endif


                                       
#if PRINT_WARN == 1
    #define print_warning(format, arg...)   do { printf("WARNING-pid:%d-%s,%d,%s--- "format"\n",\
                                                         getpid(),__FILE__,__LINE__,__func__,## arg);} \
                                            while (0)
#else
    #define print_warning(format, arg...)   do {} while (0)
#endif

#if PRINT_DBG == 1
    #define print_dbg(format, arg...)   do { printf("DEBUG-pid:%d-%s,%d,%s--- "format"\n",\
                                                     getpid(),__FILE__,__LINE__,__func__,## arg);} \
                                         while (0)
#else
    #define print_dbg(format, arg...)   do {} while (0)
#endif


#if PRINT_INFO == 1
    #define print_info(format, arg...)   do { printf("INFO-pid:%d-%s,%d,%s--- "format"\n",\
                                                     getpid(),__FILE__,__LINE__,__func__,## arg);} \
                                         while (0)
#else
    #define print_info(format, arg...)   do {} while (0)
#endif



#if PRINT_TIME == 1
    #define print_time(format, arg...)   do {printf("time:%d-pid:%d-%s,%d,%s--- "format"\n", \
                                                    get_ab_timer(), getpid(),__FILE__,__LINE__,__func__, ## arg);} \
                                         while (0)
#else
    #define print_time(format, arg...)   do {} while (0)
#endif
    

#endif /*#ifndef __DEBUG_H__*/
