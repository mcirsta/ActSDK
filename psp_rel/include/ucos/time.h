/********************************************************************************
 *                               USDK(1100)
 *                            Module: TIME
 *                 Copyright(c) 2001-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-6-12 9:42     1.0             build this file
 ********************************************************************************/
/*!
 * \file     time.h
 * \brief    cp0_count计时头文件
 * \author   hmwei
 * \version 1.0
 * \date  2008/6/12
 *******************************************************************************/
#ifndef __OS_TIME_H__
#define __OS_TIME_H__

#ifndef _ASSEMBLER_

#include <ucos/types.h>

typedef long _time_t;
typedef long _suseconds_t;

struct timeval
{
    _time_t tv_sec; /* seconds */
    _suseconds_t tv_usec; /* microseconds */
};

/* Parameters used to convert the timespec values */
#define MSEC_PER_SEC (1000L)
#define USEC_PER_SEC (1000000L)
//#define NSEC_PER_SEC (1000000000L)
//#define NSEC_PER_USEC (1000L)

//extern unsigned char losc_softtimer;
extern struct timer_list timer_losc;
extern unsigned long jiffies;

extern void hw_timer_setup();

extern unsigned int get_count(void);//返回值单位为ms
extern unsigned int get_c0_count(void);//返回值为cp0 count计数值

extern void do_gettimeofday(struct timeval *tv);

//#include "pm.h"
//

#endif /*_ASSEMBLER_*/
#endif /*__OS_TIME_H__*/
