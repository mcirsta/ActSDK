/********************************************************************************
 *                              USDK(1100)
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
#ifndef __ASM_ATJ213X_IRQ_H__
#define __ASM_ATJ213X_IRQ_H__

#include <asm-mips/mach-atj213x/actions_reg_213x.h>
#include <asm-mips/mach-atj213x/atj213x.h>

#define NR_IRQS			32

#ifndef _ASSEMBLER_
/*!
 *  中断源 \n
 *  \li 0~31 : 共32个中断源
 *
 */
enum
{
    IRQ_MCA,
    IRQ_ATA,
    IRQ_SD,
    IRQ_MHA,
    IRQ_USB,
    IRQ_DSP,
    IRQ_BT656,
    IRQ_PC,
    IRQ_WD,
    IRQ_TIMER1,
    IRQ_TIMER0,
    IRQ_RTC,
    IRQ_DMA,
    IRQ_KEY,
    IRQ_EXT,
    IRQ_TP,
    IRQ_SPI,
    IRQ_IIC2,
    IRQ_IIC1,
    IRQ_UART2,
    IRQ_UART1,
    IRQ_ADC,
    IRQ_DAC,
    IRQ_SPDIF,
    IRQ_NAND,
    IRQ_PCM,
    IRQ_YUV2RGB,
    IRQ_Reserved0,
    IRQ_Reserved1,
    IRQ_Reserved2,
    IRQ_Reserved3,
    IRQ_Reserved4
};

/*! \cond KMODULE_API*/
/******************************************************************************/
/*!
 * \par  Description:
 *	  使能中断线
 * \param[in]    unsigned int irq_nr 中断号
 * \return       void
 * \ingroup      irq
 * \par
 * \code
 * \endcode
 *******************************************************************************/
static __inline__ void local_enable_irq(unsigned int irq_nr)
{
    act_writel(act_readl(INTC_MSK) | (1 << irq_nr), INTC_MSK);
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  关闭中断线
 * \param[in]    unsigned int irq_nr 中断号
 * \return       void
 * \ingroup      irq
 * \par
 * \code
 * \endcode
 *******************************************************************************/
static __inline__ void local_disable_irq(unsigned int irq_nr)
{
    act_writel(act_readl(INTC_MSK) & ~(1 << irq_nr), INTC_MSK);
}
/*! \endcond */

#endif /*_ASSEMBLER_*/
#endif /*__ASM_ATJ213X_IRQ_H__*/
