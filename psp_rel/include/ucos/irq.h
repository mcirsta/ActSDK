/********************************************************************************
 *                               USDK(1100)
 *                            Module: IRQ
 *                 Copyright(c) 2001-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-6-12 9:42     1.0             build this file
 ********************************************************************************/
/*!
 * \file     irq.h
 * \brief    中断处理相关宏定义、数据结构和函数声明
 * \author   hmwei
 * \version 1.0
 * \date  2008/6/12
 *******************************************************************************/
#ifndef __IRQ_H__
#define __IRQ_H__

#include <asm-mips/mach-atj213x/irq.h>
#include <asm-mips/exception.h>

#define SA_SHIRQ        0x04000000
#define IRQ_DISABLED    2   /* IRQ disabled - do not enter! */

#define IRQ_HANDLED     (1)
#define IRQ_NONE            (0)

#ifndef _ASSEMBLER_

typedef int irqreturn_t;

struct hw_interrupt_type
{
    const char *name;
    unsigned int (*startup)(unsigned int irq);
    void (*shutdown)(unsigned int irq);
    void (*enable)(unsigned int irq);
    void (*disable)(unsigned int irq);
    void (*ack)(unsigned int irq);
    void (*end)(unsigned int irq);
    void (*set_affinity)(unsigned int irq);
    void (*release)(unsigned int irq, void *dev_id);
};

struct irqaction
{
    irqreturn_t (*handler)(int irq, void *dev_id, struct pt_regs *regs);
    const char *name;
    void *dev_id;
    struct irqaction *next;
    unsigned long flags;
};

typedef struct irq_desc
{
    struct hw_interrupt_type *handler;
    struct irqaction *action; /* IRQ action list */
    unsigned int status; /* IRQ status */
    unsigned long flag; /*interrupt line if shared: 0--no, 1--yes*/
} irq_desc_t;

extern irq_desc_t irq_desc[NR_IRQS];

extern void init_irq(void);
extern int request_irq(unsigned int irq, irqreturn_t(*handler)(int irq,
        void *dev_id, struct pt_regs *regs), unsigned long irq_flags,
        const char * devname, void *dev_id);
extern void free_irq(unsigned int irq, void *dev_id);
extern void soc_init_irq(void);

#endif /*_ASSEMBLER_*/
#endif /*__IRQ_H__*/
