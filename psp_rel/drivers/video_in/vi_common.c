/*
********************************************************************************
*                   213x --- video in device driver
*                (c) Copyright 2007 - 2010, Actions Co,Ld. 
*                        All Right Reserved 
*
* History:        
*      <author>      <time>       <version >    <desc>
*       houjingkun      2008/06/18     1.0         build this file 
*       new added vi_common.h and vi_common.c for unifying the sensor and tvin
*       application.
********************************************************************************/ 
/*!
 * \file   vi_common.c
 * \brief  
 *      video_in device driver's common API.
 * \author houjingkun
 * \par GENERAL DESCRIPTION:
 *      Sensor and tvin driver's common functions. it's video in's common func.
 *      new added vi_common.h for unifying the sensor and tvin and such as video in 
 *      application. it will be called by sensor_inner.c and tvin_inner.c.
 * \par EXTERNALIZED FUNCTIONS:
 *       null
 *
 *     Copyright(c) 2008-2012 Actions Semiconductor, All Rights Reserved.
 *
 * \version 1.0
 * \date  2008/06/18
 *******************************************************************************/

#include "video_in.h"
#include "vi_config.h"
#include "sensor_inner.h"
#include "vi_sys.h"
#include "vi_buf.h"
#include "sensor_hw.h"
#include "vi_common.h"
#include "errno.h"

unsigned int ic_type;

/* sensor's param is recorded in vi_dev and vi_effect. */
vi_dev_t vi_dev;
vi_effect_t vi_effect;

#if defined(_ACT_UC_OS_)
unsigned int vi_app_type = VI_DC_MODE;
#elif defined(_ACT_LINUX_KERNEL_2_6_)
uint vi_app_type = VI_DC_MODE;
wait_queue_head_t vi_wait;
#endif


/******************************************************************************/
/*!                    
* \par  Description:
*       init some param of vi_dev_t.
* \param[in]    pDev -- pointer of vi_dev_t.
* \retval       0 sucess, else failed.
*******************************************************************************/
int vi_para_init(vi_dev_t *pDev)
{
    //pDev->app_type = vi_app_type;    /* default: VI_DC_MODE. */

    pDev->opened = 0;
    pDev->resolution = VI_RES_320X240;
    pDev->resolution_len = 320 * 240 * 2;
    pDev->width = 320;
    pDev->height = 240;

    /* maybe will be changed in sensor_init_work_mode accordding current app_type. */
    //pDev->pix_fmt = VI_PIX_FMT_4Y4U4Y4V;
    pDev->first_frame = 1;
    pDev->start_dma = 0;
    pDev->phy_offset = VI_RES_320X240_PHY_ADDR_OFFSET;

    vi_effect.ss_contrast = 3;
    vi_effect.ss_brightness = 3;
    vi_effect.ss_expose_mode = SS_AUTO_EXPOSURE;
    vi_effect.ss_sp_effect = SS_NONE_SPECIAL;
    vi_effect.ss_white_balance = SS_AUTO_BALANCE;
    vi_effect.ss_focus = 3;

    /* init frame counter. */
    vi_dev.frame_cnt = 0;
    vi_dev.cur_lost_frames = 0;
    vi_dev.pre_lost_frames = 0;
    vi_dev.pad_frames = 0;

    pDev->pEffect = (struct vi_effect_t *)&vi_effect;

#if defined(_ACT_LINUX_KERNEL_2_6_)
    init_waitqueue_head(&vi_wait);
#else

#if _REQUEST_IRQ_IN_INSTALL_
    request_irq(IRQ_DMA, vi_dma_irq, SA_SHIRQ, "sensor", (void *)&vi_dma_irq);    
#endif  /* _REQUEST_IRQ_IN_INSTALL_ */
#endif  /* _ACT_LINUX_KERNEL_2_6_ or _ACT_UC_OS_ */

    return 0;
};

int vi_para_exit(vi_dev_t *pDev)
{
#if defined(_ACT_UC_OS_)
#if _REQUEST_IRQ_IN_INSTALL_
    free_irq(IRQ_DMA, (void *)&vi_dma_irq);
#endif  /* _REQUEST_IRQ_IN_INSTALL_ */
#endif  /* _ACT_UC_OS_ */
    return 0;
}


/******************************************************************************/
/*!                    
* \par  Description:
*       it will be called by device's open api.
* \param[in]    type -- refer to ss_work_mode_t.
* \retval       0 sucess, else failed.
*******************************************************************************/
int vi_common_open(unsigned int type)
{
    int ret = 0;
//    printf("vi_common_open.\n");
#if defined(_ACT_UC_OS_)


    vi_dev.app_type = (unsigned int)type;

    vi_dev.dma_chan = 4;

#if _REQUEST_IRQ_IN_INSTALL_
    /* do nothing. */
#else
    ret = request_irq(IRQ_DMA, vi_dma_irq, SA_SHIRQ, "sensor", (void *)&vi_dma_irq);    
    if (ret != 0) 
    {
        SS_PRINTF(" can't request sensor's dma irqs\n");
        return ret;
    }
#endif  /* _REQUEST_IRQ_IN_INSTALL_ */

#elif defined(_ACT_LINUX_KERNEL_2_6_)
    vi_dev.dma_chan = request_act213x_dma(DMA_CHAN_TYPE_HIGH_SPECIAL, 
        "sensor", vi_dma_irq, SA_SHIRQ, &vi_dma_irq);
    SS_PRINTF("dma num is %d\n", vi_dev.dma_chan);
#endif
    //SS_PRINTF("dma num is %d\n", vi_dev.dma_chan);
    if ((vi_dev.dma_chan < 4)||(vi_dev.dma_chan > 7)) 
    {
        SS_PRINTF("sensor request dma error!: %d\n", vi_dev.dma_chan);
        return -1;
    }

    //act_writel(0xffffffff, CMU_DMACLK);
    reset_dma(vi_dev.dma_chan);
    clear_dma_tcirq_pend(vi_dev.dma_chan);
    clear_dma_htcirq_pend(vi_dev.dma_chan);
    //enable_dma_tcirq(vi_dev.dma_chan);
    disable_dma_tcirq(vi_dev.dma_chan);
    disable_dma_htcirq(vi_dev.dma_chan);

#if defined(_ACT_UC_OS_)
    if (vi_mem_init(&vi_dev) != 0) 
    {
        //vi_mem_exit(&vi_dev);
       //vi_para_exit(&vi_dev);
        printf("vi_common_open mem error. ret is %d\n",-ENOMEM);
        return -ENOMEM; 
    }
#elif defined(_ACT_LINUX_KERNEL_2_6_)
    if (vi_mem_init(&vi_dev)) 
    {
        free_act213x_dma(vi_dev.dma_chan);
        return -ENOMEM;
    }

    /* in tv mode, in order to make sure that flash driver wouldn't lock irq too long! */
    set_pin_user_mode(0, PIN_TV_MODE);  
    set_pin_user_mode(1, PIN_TV_MODE);  
#endif

    return 0;   /* success */
}

/******************************************************************************/
/*!                    
* \par  Description:
*       dma transfered irq handler. 
* \param[in]    irq -- irq number.
* \param[in]    dev_id -- data struct pointer that maybe should use.
* \param[in]    regs -- pointer of pt_regs, refer to struct pt_regs.
* \retval        
*******************************************************************************/
int vi_dma_irq(int irq, void *dev_id, struct pt_regs *reg)
{
//  printf("vi_dma_irq begin.\n");
    //INT32U calladdr = (INT32U) __builtin_return_address(0);
    //printf("vi_dma_irq begin.calladdr is %x\n",calladdr);
    if (get_dma_tcirq_pend(vi_dev.dma_chan) != 0) 
    {
        #if 0
        printk("dma4_mode:0x%x\n",DMA_CHANNEL_BASE+4*DMA_CHANNEL_LEN+DMA_MODE);
        printk("dma4_src:0x%x\n",DMA_CHANNEL_BASE+4*DMA_CHANNEL_LEN+DMA_SRC);
        printk("dma4_dst:0x%x\n",DMA_CHANNEL_BASE+4*DMA_CHANNEL_LEN+DMA_DST);
        printk("dma4_count:0x%x\n",DMA_CHANNEL_BASE+4*DMA_CHANNEL_LEN+DMA_COUNT);
        printk("dma4_rem:0x%x\n",DMA_CHANNEL_BASE+4*DMA_CHANNEL_LEN+DMA_REMAIN);
        printk("dma4_cmd:0x%x\n",DMA_CHANNEL_BASE+4*DMA_CHANNEL_LEN+DMA_CMD);        
        #endif
        clear_dma_tcirq_pend(vi_dev.dma_chan);
//        printf("vi_dma_irq begin second.\n");

        if (vi_dev.dma_status != 0) 
        {
            vi_dev.irq_func();
            return IRQ_HANDLED;
        }

        vi_dev.start_dma = 0;
        return IRQ_HANDLED;
    }

    return IRQ_NONE;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       it will be called by device's close api.
* \param[in]    type -- refer to ss_work_mode_t.
* \retval       0 sucess, else failed.
*******************************************************************************/
int vi_common_close(unsigned int type)
{
//    printf("vi_common_close.\n");
    disable_dma_tcirq(vi_dev.dma_chan);
#if defined(_ACT_UC_OS_)

#if _REQUEST_IRQ_IN_INSTALL_
    /* do nothing. */
#else
    free_irq(IRQ_DMA, (void *)&vi_dma_irq);
#endif  /* _REQUEST_IRQ_IN_INSTALL_ */

#elif defined(_ACT_LINUX_KERNEL_2_6_)
    free_act213x_dma(vi_dev.dma_chan);

    set_pin_user_mode(0, PIN_NORMAL);
    set_pin_user_mode(1, PIN_NORMAL);
#endif

    vi_mem_exit(&vi_dev);
    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       prepare to start dma, and sign some status and reset dma. 
* \param[in]    null
* \retval       0 sucess, else failed.
*******************************************************************************/
int vi_capture_prepare(void)
{
    int n;

    if (vi_dev.resolution_len > vi_dev.mem_len) 
    {
        SS_PRINTF("video_in's buffer is not enough for current resolution!\n");
        return -1;
    }

    /* prepare to get data. */
    if (cycle_query_dma(vi_dev.dma_chan, TIME_VAL) != 0) 
    {
        reset_dma(vi_dev.dma_chan);    /* stop dma first. */
    }

    /* init frame counter. */
    vi_dev.frame_cnt = 0;
    vi_dev.cur_lost_frames = 0;
    vi_dev.pre_lost_frames = 0;
    vi_dev.pad_frames = 0;

    vi_reset_queue();
    mmm_reset_queue();

    vi_dev.flag[0] = DATA_GETTING;
    vi_dev.flag[1] = DATA_PREPARE;
    vi_dev.offset[0] = 0;
    vi_dev.offset[1] = vi_dev.resolution_len * 1;

    for (n = 2; n < (int)vi_dev.buf_cnt; n++) 
    {
        vi_dev.flag[n] = DATA_NONE;
        vi_dev.offset[n] = (unsigned int)(vi_dev.resolution_len * (unsigned int)n);
    } /* end of preparing. */

    //spin_lock_irqsave(&sensor_lock, flags);
    reset_dma(vi_dev.dma_chan);
    udelay(1);
    clear_dma_tcirq_pend(vi_dev.dma_chan);
    clear_dma_htcirq_pend(vi_dev.dma_chan);

    enable_dma_tcirq(vi_dev.dma_chan);

    vi_dev.start_dma = 1;
    vi_dev.dma_status = 1;

    vi_dev.flag[1] = DATA_PREPARE;

    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       capture finished, stop dma and clear some status signs.
* \param[in]    null
* \retval       0 sucess, else failed.
*******************************************************************************/
int vi_capture_finish(void)
{
    if (cycle_query_dma(vi_dev.dma_chan, TIME_VAL) != 0) 
    {
        reset_dma(vi_dev.dma_chan);
        udelay(1);
        return -1;
    }
    reset_dma(vi_dev.dma_chan);
    udelay(1);

    disable_dma_tcirq(vi_dev.dma_chan);

    vi_dev.start_dma = 0;
    vi_dev.dma_status = 0;

    return 0;
}

