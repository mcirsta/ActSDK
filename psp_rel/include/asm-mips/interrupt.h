/********************************************************************************
 *                              USDK(1100)
 *                            Module: INTERRUPT
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-8-30 16:45     1.0             build this file
 ********************************************************************************/
/*!
 * \file     interrupt.h
 * \brief    �����ж���������ʵ��
 * \author   open source
 * \version 1.0
 * \date  2008/8/30
 *******************************************************************************/
#ifndef __ASM_INTERRUPT_H__
#define __ASM_INTERRUPT_H__

#include <asm-mips/config.h>
#include <asm-mips/hazards.h>

__asm__ (
        "	.macro	local_irq_enable				\n"
        "	.set	push						\n"
        "	.set	reorder						\n"
        "	.set	noat						\n"
#ifdef CONFIG_CPU_MIPSR2
        "	ei							\n"
#else
        "	mfc0	$1,$12						\n"
        "	ori	$1,0x1f						\n"
        "	xori	$1,0x1e						\n"
        "	mtc0	$1,$12						\n"
#endif
        "	irq_enable_hazard					\n"
        "	.set	pop						\n"
        "	.endm");

static inline void local_irq_enable(void)
{
    __asm__ __volatile__(
            "local_irq_enable"
            : /* no outputs */
            : /* no inputs */
            : "memory");
}

/*
 * For cli() we have to insert nops to make sure that the new value
 * has actually arrived in the status register before the end of this
 * macro.
 * R4000/R4400 need three nops, the R4600 two nops and the R10000 needs
 * no nops at all.
 */
__asm__ (
        "	.macro	local_irq_disable\n"
        "	.set	push						\n"
        "	.set	noat						\n"
#ifdef CONFIG_CPU_MIPSR2
        "	di							\n"
#else
        "	mfc0	$1,$12						\n"
        "	ori	$1,1						\n"
        "	xori	$1,1						\n"
        "	.set	noreorder					\n"
        "	mtc0	$1,$12						\n"
#endif
        "	irq_disable_hazard					\n"
        "	.set	pop						\n"
        "	.endm							\n");

static inline void local_irq_disable(void)
{
    __asm__ __volatile__(
            "local_irq_disable"
            : /* no outputs */
            : /* no inputs */
            : "memory");
}

__asm__ (
        "	.macro	local_save_flags flags				\n"
        "	.set	push						\n"
        "	.set	reorder						\n"
        "	mfc0	\\flags, $12					\n"
        "	.set	pop						\n"
        "	.endm							\n");

#define local_save_flags(x)						\
__asm__ __volatile__(							\
	"local_save_flags %0"						\
	: "=r" (x))

__asm__ (
        "	.macro	local_irq_save result				\n"
        "	.set	push						\n"
        "	.set	reorder						\n"
        "	.set	noat						\n"
#ifdef CONFIG_CPU_MIPSR2
        "	di	\\result					\n"
#else
        "	mfc0	\\result, $12					\n"
        "	ori	$1, \\result, 1					\n"
        "	xori	$1, 1						\n"
        "	.set	noreorder					\n"
        "	mtc0	$1, $12						\n"
#endif
        "	irq_disable_hazard					\n"
        "	.set	pop						\n"
        "	.endm							\n");

/*!
 * \brief
 *      �������жϣ������浱ǰ�жϵ�״̬
 * \li  x: �洢��ǰ�ж�״̬�ı���
 * \par          exmaple code
 * \code
 *unsigned int flags;
 *local_irq_save(flags);
 *...
 *local_irq_restore(flags);
 * \endcode
 */
#define local_irq_save(x)						\
__asm__ __volatile__(							\
	"local_irq_save\t%0"						\
	: "=r" (x)							\
	: /* no inputs */						\
	: "memory")

__asm__ (
        "	.macro	local_irq_restore flags				\n"
        "	.set	noreorder					\n"
        "	.set	noat						\n"
#if defined(CONFIG_CPU_MIPSR2) && defined(CONFIG_IRQ_CPU)
        /*
         * Slow, but doesn't suffer from a relativly unlikely race
         * condition we're having since days 1.
         */
        "	beqz	\\flags, 1f					\n"
        "	 di							\n"
        "	ei							\n"
        "1:								\n"
#elif defined(CONFIG_CPU_MIPSR2)
        /*
         * Fast, dangerous.  Life is fun, life is good.
         */
        "	mfc0	$1, $12						\n"
        "	ins	$1, \\flags, 0, 1				\n"
        "	mtc0	$1, $12						\n"
#else
        "	mfc0	$1, $12						\n"
        "	andi	\\flags, 1					\n"
        "	ori	$1, 1						\n"
        "	xori	$1, 1						\n"
        "	or	\\flags, $1					\n"
        "	mtc0	\\flags, $12					\n"
#endif
        "	irq_disable_hazard					\n"
        "	.set	at						\n"
        "	.set	reorder						\n"
        "	.endm							\n");

/*!
 * \brief
 *      �ָ�local_irq_save()�����״̬
 * \li  flags: local_irq_save()�����״ֵ̬
 * \par          exmaple code
 * \code
 *unsigned int flags;
 *local_irq_save(flags);
 *...
 *local_irq_restore(flags);
 * \endcode
 */
#define local_irq_restore(flags)					\
do {									\
	unsigned long __tmp1;						\
									\
	__asm__ __volatile__(						\
		"local_irq_restore\t%0"					\
		: "=r" (__tmp1)						\
		: "0" (flags)						\
		: "memory");						\
} while(0)

#define irqs_disabled()							\
({									\
	unsigned long flags;						\
	local_save_flags(flags);					\
	!(flags & 1);							\
})

#endif/*__ASM_INTERRUPT_H__*/
