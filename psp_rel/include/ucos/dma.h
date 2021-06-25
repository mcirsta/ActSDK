/********************************************************************************
 *                               USDK(1100)
 *                            Module: DMA
 *                 Copyright(c) 2001-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2008-6-12 9:42     1.0             build this file
 ********************************************************************************/
/*!
 * \file     dma.h
 * \brief    dma管理头文件
 * \author   hmwei
 * \version 1.0
 * \date  2008/6/12
 *******************************************************************************/
#ifndef __DMA_H__
#define __DMA_H__

#include <ucos/irq.h>
#include <asm-mips/exception.h>

#ifndef _ASSEMBLER_
/*!
 *  DMA类型 \n
 *  \li DMA_CHAN_TYPE_SPECIAL : 专业通道DMA
 *  \li DMA_CHAN_TYPE_BUS: 总线通道DMA
 *
 */
enum
{
    DMA_CHAN_TYPE_SPECIAL,
    DMA_CHAN_TYPE_BUS
};

struct dma_chan
{
    int is_used; // this channel is allocated if !=0, free if ==0
    unsigned int io;
    const char *dev_str;
    int irq;
    void *irq_dev;
    unsigned int chan_type;
};

/* These are in arch/mips/atj213x/common/dma.c */
//extern struct dma_chan act213x_dma_table[];
extern int init_dma(void);
extern int request_act213x_dma(unsigned int chan_type, const char *dev_str,
        irqreturn_t(*irqhandler)(int, void *, struct pt_regs *),
        unsigned long irqflags, void *irq_dev_id);
extern void free_act213x_dma(unsigned int dmanr);
extern struct dma_chan *get_dma_chan(unsigned int dmanr);

#include <asm-mips/mach-atj213x/dma.h>

#endif /*_ASSEMBLER_*/
#endif /* __DMA_H__ */
