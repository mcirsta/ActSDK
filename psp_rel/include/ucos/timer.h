/********************************************************************************
 *                              USDK(1100)
 *                            Module: TIMER
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-8-30 16:43     1.0             build this file
 ********************************************************************************/
/*!
 * \file     timer.h
 * \brief    ϵͳ��ʱ��ͷ�ļ�
 * \author   open source
 * \version 1.0
 * \date  2008/8/30
 *******************************************************************************/
#ifndef __TIMER_H__
#define __TIMER_H__

#include <ucos/list.h>
#include <ucos/interrupt.h>

#ifndef _ASSEMBLER_

/*!
 * \brief
 *      ��ʱ������ṹ����.
 */
struct timer_list
{
    /*! ������˫������*/
    struct list_head entry;
    /*! ����ʱ��*/
    unsigned long expires;
    /*! ��ʱ���ص�����*/
    void (*function)(unsigned long);
    /*! ��ʱ���ص��������������*/
    unsigned long data;
    /*! ����ʱ��*/
    unsigned long base;
};

static inline int timer_pending(const struct timer_list * timer)
{
    return timer->entry.next != NULL;
}

extern void init_timer(struct timer_list * timer);
extern void add_timer(struct timer_list * timer);
extern int mod_timer(struct timer_list *timer, unsigned long expires);
extern int del_timer(struct timer_list * timer);
extern int del_timer_sync(struct timer_list *timer);

extern int tell_timer_softirq(void);

extern void init_timers(void);

#endif /*_ASSEMBLER_*/
#endif 	/*__TIMER_H__*/
