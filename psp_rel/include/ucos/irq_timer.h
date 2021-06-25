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
 * \brief    ���жϷ�����������еĶ�ʱ��
 * \author   hmwei
 * \version 1.0
 * \date  2007/5/4
 *******************************************************************************/

#ifndef __IRQ_TIMER_H__
#define __IRQ_TIMER_H__

#ifndef _ASSEMBLER_

/*!
 *  \brief
 *  �ж϶�ʱ������
 *  \li IRQ_TIMER_SINGLE : ����timer
 *  \li IRQ_TIMER_CYCLE : ѭ��timer
 */
enum
{
    IRQ_TIMER_SINGLE,
    IRQ_TIMER_CYCLE
};

/*!
 * \brief
 *      �ж϶�ʱ����ʼ�������ṹ
 */
typedef struct
{
    /* ������ */
    void (*handler)(unsigned int);
    /* ʱ���� */
    unsigned int period;
    /* �������Ĳ��� */
    unsigned int data;
    /* ����:IRQ_TIMER_CYCLE/IRQ_TIMER_SINGLE */
    unsigned char type;
} irq_timer_arg_t;

extern int set_irq_timer(irq_timer_arg_t *arg);
extern int mod_irq_timer(unsigned int timer, unsigned int new_period);
extern int del_irq_timer(unsigned int timer);

#endif /*_ASSEMBLER_*/
#endif /*__IRQ_TIMER_H__*/
