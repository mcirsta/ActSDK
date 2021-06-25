/********************************************************************************
 *                              USDK1100
 *                            Module: DMA
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
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
#ifndef __ASM_ATJ213X_DMA_H__
#define __ASM_ATJ213X_DMA_H__

#include <asm-mips/mach-atj213x/atj213x.h>

#define NUM_ACT213X_DMA_CHANNELS    8

/* DMA Channel Base Addresses */
#define DMA_CHANNEL_BASE        0xB0060100
#define DMA_CHANNEL_LEN         0x00000020

/* DMA Channel Register Offsets */
#define     DMA_MODE                   0x00
#define     DMA_SRC                    0x04
#define     DMA_DST                    0x08
#define     DMA_COUNT                  0x0C
#define     DMA_REMAIN                 0x10
#define     DMA_CMD                    0x14

/* DMA Mode register bits follow */

#define DMA_RESET                   (0x01<<16)
#define DMA_START                   0x01
#define DMA_PAUSE                   (0x01<<1)
#define DMA_COUNT_MASK              0x000fffff
#define DMA_BLOCK_MODE              (1<<4)

#ifndef _ASSEMBLER_
static __inline__ void set_dma_cpu_priority(unsigned int prio)
{
    act_writel(prio & 0x0000000f, DMA_CTL);
}

static __inline__ void reset_dma(unsigned int dmanr)
{
#ifndef NO_KERNEL_SUPPORT
    struct dma_chan *chan = get_dma_chan(dmanr);

    if (!chan)
        return;

    act_writel(act_readl(DMA_CTL) | (DMA_RESET << dmanr), DMA_CTL);
#else
    if(dmanr >= NUM_ACT213X_DMA_CHANNELS)
    return;
    act_writel(act_readl(DMA_CTL)|(DMA_RESET<<dmanr), DMA_CTL);
#endif
}

static __inline__ void start_dma(unsigned int dmanr)
{
#ifndef NO_KERNEL_SUPPORT
    struct dma_chan *chan = get_dma_chan(dmanr);

    if (!chan)
        return;

    act_writel(DMA_START, chan->io + DMA_CMD);
#else
    if(dmanr >= NUM_ACT213X_DMA_CHANNELS)
    return;
    act_writel(DMA_START, DMA_CHANNEL_BASE + dmanr * DMA_CHANNEL_LEN + DMA_CMD);
#endif
}

static __inline__ void start_dma_with_param(unsigned int dmanr,
        unsigned int param)
{
#ifndef NO_KERNEL_SUPPORT
    struct dma_chan *chan = get_dma_chan(dmanr);

    if (!chan)
        return;

    act_writel(DMA_START | param, chan->io + DMA_CMD);
#else
    if(dmanr >= NUM_ACT213X_DMA_CHANNELS)
    return;
    act_writel(DMA_START | param, DMA_CHANNEL_BASE + dmanr * DMA_CHANNEL_LEN + DMA_CMD);
#endif
}

static __inline__ int dma_started(unsigned int dmanr)
{
#ifndef NO_KERNEL_SUPPORT
    struct dma_chan *chan = get_dma_chan(dmanr);

    if (!chan)
        return 1;

    return (act_readl(chan->io + DMA_CMD) & DMA_START) ? 1 : 0;
#else
    if(dmanr >= NUM_ACT213X_DMA_CHANNELS)
    return 1;
    return (act_readl(DMA_CHANNEL_BASE + dmanr * DMA_CHANNEL_LEN + DMA_CMD) & DMA_START) ? 1 : 0;
#endif
}

static __inline__ void stop_dma(unsigned int dmanr)
{
#ifndef NO_KERNEL_SUPPORT
    struct dma_chan *chan = get_dma_chan(dmanr);

    if (!chan)
        return;

    act_writel((~DMA_START) & act_readl(chan->io + DMA_CMD), chan->io + DMA_CMD);
#else
    if(dmanr >= NUM_ACT213X_DMA_CHANNELS)
    return;
    act_writel((~DMA_START)&act_readl(DMA_CHANNEL_BASE + dmanr * DMA_CHANNEL_LEN + DMA_CMD), DMA_CHANNEL_BASE + dmanr * DMA_CHANNEL_LEN + DMA_CMD);
#endif
}

static __inline__ void wait_dma_stopped(unsigned int dmanr)
{
#ifndef NO_KERNEL_SUPPORT
    struct dma_chan *chan = get_dma_chan(dmanr);

    if (!chan)
        return;
    while (act_readl(chan->io + DMA_CMD) & DMA_START)
        ;
#else
    if(dmanr >= NUM_ACT213X_DMA_CHANNELS)
    return;
    while(act_readl(DMA_CHANNEL_BASE + dmanr * DMA_CHANNEL_LEN + DMA_CMD) & DMA_START);
#endif
}

#define DMA_HALT_POLL 0x5000

static __inline__ void pause_dma(unsigned int dmanr)
{
#ifndef NO_KERNEL_SUPPORT
    struct dma_chan *chan = get_dma_chan(dmanr);

    if (!chan)
        return;
    act_writel((DMA_PAUSE | act_readl(chan->io + DMA_CMD)), chan->io + DMA_CMD);
#else
    if(dmanr >= NUM_ACT213X_DMA_CHANNELS)
    return;
    act_writel((DMA_PAUSE|act_readl(DMA_CHANNEL_BASE + dmanr * DMA_CHANNEL_LEN + DMA_CMD)), DMA_CHANNEL_BASE + dmanr * DMA_CHANNEL_LEN + DMA_CMD);
#endif
}

static __inline__ void resume_dma(unsigned int dmanr)
{
#ifndef NO_KERNEL_SUPPORT
    struct dma_chan *chan = get_dma_chan(dmanr);

    if (!chan)
        return;

    act_writel((~DMA_PAUSE) & act_readl(chan->io + DMA_CMD), chan->io + DMA_CMD);
#else
    if(dmanr >= NUM_ACT213X_DMA_CHANNELS)
    return;
    act_writel((~DMA_PAUSE)&act_readl(DMA_CHANNEL_BASE + dmanr * DMA_CHANNEL_LEN + DMA_CMD), DMA_CHANNEL_BASE + dmanr * DMA_CHANNEL_LEN + DMA_CMD);
#endif
}

static __inline__ int dma_paused(unsigned int dmanr)
{
#ifndef NO_KERNEL_SUPPORT
    struct dma_chan *chan = get_dma_chan(dmanr);

    if (!chan)
        return 1;

    return (act_readl(chan->io + DMA_CMD) & DMA_PAUSE) ? 1 : 0;
#else
    if(dmanr >= NUM_ACT213X_DMA_CHANNELS)
    return 1;
    return (act_readl(DMA_CHANNEL_BASE + dmanr * DMA_CHANNEL_LEN + DMA_CMD) & DMA_PAUSE) ? 1 : 0;
#endif
}

static __inline__ void set_dma_mode(unsigned int dmanr, unsigned int mode)
{
#ifndef NO_KERNEL_SUPPORT
    struct dma_chan *chan = get_dma_chan(dmanr);

    if (!chan)
        return;

    act_writel(mode, chan->io + DMA_MODE);
#else
    if(dmanr >= NUM_ACT213X_DMA_CHANNELS)
    return;
    act_writel(mode, DMA_CHANNEL_BASE + dmanr * DMA_CHANNEL_LEN + DMA_MODE);
#endif
}

static __inline__ unsigned int get_dma_mode(unsigned int dmanr)
{
#ifndef NO_KERNEL_SUPPORT
    struct dma_chan *chan = get_dma_chan(dmanr);

    if (!chan)
        return 0;

    return act_readl(chan->io + DMA_MODE);
#else
    if(dmanr >= NUM_ACT213X_DMA_CHANNELS)
    return 0;
    return act_readl(DMA_CHANNEL_BASE + dmanr * DMA_CHANNEL_LEN + DMA_MODE);
#endif
}

static __inline__ void set_dma_src_addr(unsigned int dmanr, unsigned int a)
{
#ifndef NO_KERNEL_SUPPORT
    struct dma_chan *chan = get_dma_chan(dmanr);

    if (!chan)
        return;

    act_writel(a, chan->io + DMA_SRC);
#else
    if(dmanr >= NUM_ACT213X_DMA_CHANNELS)
    return;
    act_writel(a, DMA_CHANNEL_BASE + dmanr * DMA_CHANNEL_LEN + DMA_SRC);
#endif
}

static __inline__ void set_dma_dst_addr(unsigned int dmanr, unsigned int a)
{
#ifndef NO_KERNEL_SUPPORT
    struct dma_chan *chan = get_dma_chan(dmanr);

    if (!chan)
        return;

    act_writel(a, chan->io + DMA_DST);
#else
    if(dmanr >= NUM_ACT213X_DMA_CHANNELS)
    return;
    act_writel(a, DMA_CHANNEL_BASE + dmanr * DMA_CHANNEL_LEN + DMA_DST);
#endif
}

static __inline__ void set_dma_count(unsigned int dmanr, unsigned int count)
{
#ifndef NO_KERNEL_SUPPORT
    struct dma_chan *chan = get_dma_chan(dmanr);

    if (!chan)
        return;

    count &= DMA_COUNT_MASK;
    act_writel(count, chan->io + DMA_COUNT);
#else
    if(dmanr >= NUM_ACT213X_DMA_CHANNELS)
    return;
    act_writel(count, DMA_CHANNEL_BASE + dmanr * DMA_CHANNEL_LEN + DMA_COUNT);
#endif
}

static __inline__ int get_dma_remain(unsigned int dmanr)
{
    int count;

#ifndef NO_KERNEL_SUPPORT
    struct dma_chan *chan = get_dma_chan(dmanr);

    if (!chan)
        return -1;

    count = act_readl(chan->io + DMA_REMAIN) & DMA_COUNT_MASK;

#else
    if(dmanr >= NUM_ACT213X_DMA_CHANNELS)
    return -1;

    count = act_readl(DMA_CHANNEL_BASE + dmanr * DMA_CHANNEL_LEN + DMA_REMAIN) & DMA_COUNT_MASK;
#endif
    return count;
}

static __inline__ int clear_dma_remain(unsigned int dmanr)
{
#ifndef NO_KERNEL_SUPPORT
    struct dma_chan *chan = get_dma_chan(dmanr);

    if (!chan)
        return -1;

    act_writel(0, chan->io + DMA_REMAIN);

#else
    if(dmanr >= NUM_ACT213X_DMA_CHANNELS)
    return -1;

    act_writel(0, DMA_CHANNEL_BASE + dmanr * DMA_CHANNEL_LEN + DMA_REMAIN);
#endif
    return 0;
}

static __inline__ int get_dma_tcirq_pend(unsigned int dmanr)
{
#ifndef NO_KERNEL_SUPPORT
    struct dma_chan *chan = get_dma_chan(dmanr);

    if (!chan)
        return -1;

    return (act_readl(DMA_IRQPD) & (1 << (dmanr * 2))) ? 1 : 0;
#else
    if(dmanr >= NUM_ACT213X_DMA_CHANNELS)
    return -1;

    return (act_readl(DMA_IRQPD) & (1<<(dmanr*2))) ? 1 : 0;
#endif
}

static __inline__ int get_dma_htcirq_pend(unsigned int dmanr)
{
#ifndef NO_KERNEL_SUPPORT
    struct dma_chan *chan = get_dma_chan(dmanr);

    if (!chan)
        return -1;

    return (act_readl(DMA_IRQPD) & (1 << (dmanr * 2 + 1))) ? 1 : 0;
#else
    if(dmanr >= NUM_ACT213X_DMA_CHANNELS)
    return -1;

    return (act_readl(DMA_IRQPD) & (1<<(dmanr*2+1))) ? 1 : 0;
#endif
}

static __inline__ int clear_dma_tcirq_pend(unsigned int dmanr)
{
#ifndef NO_KERNEL_SUPPORT
    struct dma_chan *chan = get_dma_chan(dmanr);

    if (!chan)
        return -1;

    act_writel(1 << (dmanr * 2), DMA_IRQPD);
#else
    if(dmanr >= NUM_ACT213X_DMA_CHANNELS)
    return -1;

    act_writel(1<<(dmanr*2), DMA_IRQPD);
#endif
    return 0;
}

static __inline__ int clear_dma_htcirq_pend(unsigned int dmanr)
{
#ifndef NO_KERNEL_SUPPORT
    struct dma_chan *chan = get_dma_chan(dmanr);

    if (!chan)
        return -1;

    act_writel(1 << (dmanr * 2 + 1), DMA_IRQPD);

#else
    if(dmanr >= NUM_ACT213X_DMA_CHANNELS)
    return -1;

    act_writel(1<<(dmanr*2+1), DMA_IRQPD);
#endif
    return 0;
}

static __inline__ int enable_dma_tcirq(unsigned int dmanr)
{
#ifndef NO_KERNEL_SUPPORT
    struct dma_chan *chan = get_dma_chan(dmanr);

    if (!chan)
        return -1;

    act_writel(act_readl(DMA_IRQEN) | (1 << (dmanr * 2)), DMA_IRQEN);
#else
    if(dmanr >= NUM_ACT213X_DMA_CHANNELS)
    return -1;
    act_writel(act_readl(DMA_IRQEN)|(1<<(dmanr*2)), DMA_IRQEN);
#endif
    return 0;
}

static __inline__ int enable_dma_htcirq(unsigned int dmanr)
{
#ifndef NO_KERNEL_SUPPORT
    struct dma_chan *chan = get_dma_chan(dmanr);

    if (!chan)
        return -1;

    act_writel(act_readl(DMA_IRQEN) | (1 << (dmanr * 2 + 1)), DMA_IRQEN);
#else
    if(dmanr >= NUM_ACT213X_DMA_CHANNELS)
    return -1;
    act_writel(act_readl(DMA_IRQEN)|(1<<(dmanr*2+1)), DMA_IRQEN);
#endif
    return 0;
}

static __inline__ int disable_dma_tcirq(unsigned int dmanr)
{
#ifndef NO_KERNEL_SUPPORT
    struct dma_chan *chan = get_dma_chan(dmanr);

    if (!chan)
        return -1;

    act_writel(act_readl(DMA_IRQEN) & (~(1 << (dmanr * 2))), DMA_IRQEN);
#else
    if(dmanr >= NUM_ACT213X_DMA_CHANNELS)
    return -1;

    act_writel(act_readl(DMA_IRQEN)&(~(1<<(dmanr*2))), DMA_IRQEN);
#endif
    return 0;
}

static __inline__ int disable_dma_htcirq(unsigned int dmanr)
{
#ifndef NO_KERNEL_SUPPORT
    struct dma_chan *chan = get_dma_chan(dmanr);

    if (!chan)
        return -1;

    act_writel(act_readl(DMA_IRQEN) & (~(1 << (dmanr * 2 + 1))), DMA_IRQEN);
#else
    if(dmanr >= NUM_ACT213X_DMA_CHANNELS)
    return -1;
    act_writel(act_readl(DMA_IRQEN)&(~(1<<(dmanr*2+1))), DMA_IRQEN);

#endif
    return 0;
}
#endif /*_ASSEMBLER_*/
#endif /* __ASM_ATJ213X_DMA_H__ */
