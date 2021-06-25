/*********************************************************************************
 *                       213x---gsensor driver
 *                (c) Copyright 2007-2010, Actions Co,Ld.
 *                        All Right Reserved
 *
 * FileName:data_buf.h    Author:yangrui  Date:2009/05/11
 * Description: definition of gsensor_data_t  and other declaration.
 * Others:
 * History:
 *      <author>    <time>       <version >    <desc>
 *      yangrui     2009/05/11     1.0         build this moudle
 *********************************************************************************/
#ifndef _DATA_BUF_H_
#define _DATA_BUF_H_

#include "gsensor_sys.h"   /* size_t */
#include "ucos/jiffies.h"

extern atomic_t w_enable;
extern unsigned long w_interval;
extern int data_buf_write(void);
extern ssize_t data_buf_read(char *, size_t);
extern int data_buf_ioctl(unsigned int cmd, unsigned long interval);

extern unsigned long * g_jiffies;
extern unsigned long w_interval;
extern struct timer_list g_timer;
extern void timer_handle(unsigned long arg);

#endif

