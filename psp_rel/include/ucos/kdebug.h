/*******************************************************************************
 *                              USDK1100
 *                            Module: KDEBUG
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2009-9-23 15:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     kdebug.h
 * \brief    ¥Ú”°∫Í∂®“Â
 * \author   hmwei
 * \version 1.0
 * \date  2007/5/4
 *******************************************************************************/

#ifndef __KDEBUG_H__
#define __KDEBUG_H__

#include <ucos/serial.h>

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
#define DMSG_ERR(format, arg...)   do { printk(KERN_ERR "-%s,%d,%s--- "format"\n",\
                                                    __FILE__,__LINE__,__func__, ## arg);} \
                                        while (0)
#else
#define DMSG_ERR(format, arg...)   do {} while (0)
#endif

#if PRINT_WARN == 1
#define DMSG_WARNING(format, arg...)   do { printk(KERN_WARNING "-%s,%d,%s--- "format"\n",\
                                                         __FILE__,__LINE__,__func__,## arg);} \
                                            while (0)
#else
#define DMSG_WARNING(format, arg...)   do {} while (0)
#endif

#if PRINT_DBG == 1
#define DMSG_DBG(format, arg...)   do { printk(KERN_DEBUG "-%s,%d,%s--- "format"\n",\
                                                     __FILE__,__LINE__,__func__,## arg);} \
                                         while (0)
#else
#define DMSG_DBG(format, arg...)   do {} while (0)
#endif

#if PRINT_INFO == 1
#define DMSG_INFO(format, arg...)   do { printk(KERN_INFO "-%s,%d,%s--- "format"\n",\
                                                     __FILE__,__LINE__,__func__,## arg);} \
                                         while (0)
#else
#define DMSG_INFO(format, arg...)   do {} while (0)
#endif

#if PRINT_TIME == 1
#define DMSG_TIME(format, arg...)   do {printk("time:%d-%s,%d,%s--- "format"\n", \
                                                    get_ab_timer(),__FILE__,__LINE__,__func__, ## arg);} \
                                         while (0)
#else
#define DMSG_TIME(format, arg...)   do {} while (0)
#endif

#endif /*#ifndef __KDEBUG_H__*/
