/*******************************************************************************
 *                              USDK1100
 *                            Module: IRQ_TIMER
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2009-11-24 16:23     1.0             build this file
 *******************************************************************************/
/*!
 * \file     irq_timer.h
 * \brief    在中断服务程序中运行的定时器
 * \author   hmwei
 * \version 1.0
 * \date  2007/5/4
 *******************************************************************************/

#ifndef __IRQ_TIMER_H__
#define __IRQ_TIMER_H__

#ifndef _ASSEMBLER_

/*!
 *  \brief
 *  中断定时器类型
 *  \li IRQ_TIMER_SINGLE : 单次timer
 *  \li IRQ_TIMER_CYCLE : 循环timer
 */
enum
{
    IRQ_TIMER_SINGLE,
    IRQ_TIMER_CYCLE
};

/*!
 * \brief
 *      中断定时器初始化参数结构
 */
typedef struct
{
    /* 处理函数 */
    void (*handler)(unsigned int);
    /* 时间间隔 */
    unsigned int period;
    /* 处理函数的参数 */
    unsigned int data;
    /* 类型:IRQ_TIMER_CYCLE/IRQ_TIMER_SINGLE */
    unsigned char type;
} irq_timer_arg_t;

extern int set_irq_timer(irq_timer_arg_t *arg);
extern int mod_irq_timer(unsigned int timer, unsigned int new_period);
extern int del_irq_timer(unsigned int timer);

#endif /*_ASSEMBLER_*/
#endif /*__IRQ_TIMER_H__*/
