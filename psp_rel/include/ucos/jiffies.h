/********************************************************************************
 *                               USDK(1100)
 *                            Module: JIFFIES
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-6-12 9:42     1.0             build this file
 ********************************************************************************/
/*!
 * \file     jiffies.h
 * \brief    jiffies变量及jiffies转换宏定义
 * \author   hmwei
 * \version 1.0
 * \date  2008/6/12
 *******************************************************************************/
#ifndef __JIFFIES_H__
#define __JIFFIES_H__

#ifndef _ASSEMBLER_

#include <ucos/param.h> /* for HZ */
#include <ucos/time.h> /* for MSEC_PER_SEC/USEC_PER_SEC */

extern unsigned long jiffies;

/*
 * Change timeval to jiffies, trying to avoid the
 * most obvious overflows..
 *
 * And some not so obvious.
 *
 * Note that we don't want to return MAX_LONG, because
 * for various timeout reasons we often end up having
 * to wait "jiffies+1" in order to guarantee that we wait
 * at _least_ "jiffies" - so "jiffies+1" had better still
 * be positive.
 */
#define MAX_JIFFY_OFFSET ((~0UL >> 1)-1)
/*
 * Convert jiffies to milliseconds and back.
 *
 * Avoid unnecessary multiplications/divisions in the
 * two most common HZ cases:
 */
static inline unsigned int jiffies_to_msecs(const unsigned long j)
{
#if HZ <= MSEC_PER_SEC && !(MSEC_PER_SEC % HZ)
    return (MSEC_PER_SEC / HZ) * j;
#elif HZ > MSEC_PER_SEC && !(HZ % MSEC_PER_SEC)
    return (j + (HZ / MSEC_PER_SEC) - 1)/(HZ / MSEC_PER_SEC);
#else
    return (j * MSEC_PER_SEC) / HZ;
#endif
}

static inline unsigned int jiffies_to_usecs(const unsigned long j)
{
#if HZ <= USEC_PER_SEC && !(USEC_PER_SEC % HZ)
    return (USEC_PER_SEC / HZ) * j;
#elif HZ > USEC_PER_SEC && !(HZ % USEC_PER_SEC)
    return (j + (HZ / USEC_PER_SEC) - 1)/(HZ / USEC_PER_SEC);
#else
    return (j * USEC_PER_SEC) / HZ;
#endif
}

static inline unsigned long msecs_to_jiffies(const unsigned int m)
{
    if (m > jiffies_to_msecs(MAX_JIFFY_OFFSET))
        return MAX_JIFFY_OFFSET;
#if HZ <= MSEC_PER_SEC && !(MSEC_PER_SEC % HZ)
    return (m + (MSEC_PER_SEC / HZ) - 1) / (MSEC_PER_SEC / HZ);
#elif HZ > MSEC_PER_SEC && !(HZ % MSEC_PER_SEC)
    return m * (HZ / MSEC_PER_SEC);
#else
    return (m * HZ + MSEC_PER_SEC - 1) / MSEC_PER_SEC;
#endif
}

static inline unsigned long usecs_to_jiffies(const unsigned int u)
{
    if (u > jiffies_to_usecs(MAX_JIFFY_OFFSET))
        return MAX_JIFFY_OFFSET;
#if HZ <= USEC_PER_SEC && !(USEC_PER_SEC % HZ)
    return (u + (USEC_PER_SEC / HZ) - 1) / (USEC_PER_SEC / HZ);
#elif HZ > USEC_PER_SEC && !(HZ % USEC_PER_SEC)
    return u * (HZ / USEC_PER_SEC);
#else
    return (u * HZ + USEC_PER_SEC - 1) / USEC_PER_SEC;
#endif
}

#endif /*_ASSEMBLER_*/
#endif /*__JIFFIES_H__*/
