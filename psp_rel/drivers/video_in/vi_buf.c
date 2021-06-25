/*
********************************************************************************
*                   213x --- video in device driver
*                (c) Copyright 2007 - 2010, Actions Co,Ld. 
*                        All Right Reserved 
*
* History:        
*      <author>      <time>       <version >    <desc>
*       houjingkun      2008/06/18     1.0         build this file 
*       new added vi_buf.h and vi_buf.c for video in device driver.
********************************************************************************/ 
/*!
 * \file   vi_buf.c
 * \brief  
 *      video in device driver's buffer management function.
 * \author houjingkun
 * \par GENERAL DESCRIPTION:
 *      video in device driver's buffer management function. 
 *      just called by dirver itself.
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
#include "tvin_inner.h"
#include "vi_sys.h"

/* for sensor gathered buffer queue management. */
static int ss_queue[QUEUE_CNT];
static int ss_que_head = 0;
static int ss_que_tail = 0;
static int ss_que_cnt = 0;

/******************************************************************************/
/*!                    
* \par  Description:
*       reset sensor gathered buffer queue. 
* \param[in]    null
* \retval       0 sucess, else failed.
*******************************************************************************/
int vi_reset_queue(void)
{
    int i;

    for (i = 0; i < vi_dev.queue_cnt; i++) 
    {
        ss_queue[i] = -1;
    }

    ss_que_head = 0;
    ss_que_tail = 0;
    ss_que_cnt = 0;

    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       if return 0, the vi queue isn't full, else is full. 
* \param[in]    null
* \retval       if return 0, the vi queue isn't full, else is full. 
*******************************************************************************/
int vi_que_whether_full(void)
{
    if (ss_que_cnt >= vi_dev.queue_cnt) 
    {
        return 1;  /* queue is full */
    } 
    return 0;
}

/* put in a new sensor gathered buffer into queue. */
static int ss_in_queue(int num)
{
    if (ss_que_cnt >= vi_dev.queue_cnt) 
    {
        return -1;  /* queue is full */
    }

    ss_queue[ss_que_head] = num;
    ss_que_head++;
    ss_que_cnt++;
    if (ss_que_head >= vi_dev.queue_cnt) 
    {
        ss_que_head = 0;
    }

    return 0;
}

/* put out a sensor gathered buffer out of queue. */
static int ss_out_queue(void)
{
    int value;

    if (ss_que_cnt <= 0) 
    {
        return -1;      /* queue is empty. */
    }

    value = ss_queue[ss_que_tail];
    ss_que_tail++;
    ss_que_cnt--;

    if (ss_que_tail >= vi_dev.queue_cnt) 
    {
        ss_que_tail = 0;
    }
    return value;
}

/* for output to middleware buffer queue management. */
static int mmm_queue[OUTPUT_CNT];
static int mmm_que_head = 0;
static int mmm_que_tail = 0;
static int mmm_que_cnt = 0;

/******************************************************************************/
/*!                    
* \par  Description:
*       mmm_xxx means media midware management.
* \param[in]    null
* \retval       0 sucess, else failed.
*******************************************************************************/
int mmm_reset_queue(void)
{
    int i;

    for (i = 0; i < vi_dev.output_cnt; i++) 
    {
        mmm_queue[i] = -1;
    }

    mmm_que_head = 0;
    mmm_que_tail = 0;
    mmm_que_cnt = 0;

    return 0;
}

static int mmm_in_queue(int num)
{
    if (mmm_que_cnt >= vi_dev.output_cnt) 
    {
        return -1;  /* queue is full */
    }

    mmm_queue[mmm_que_head] = num;
    mmm_que_head++;
    mmm_que_cnt++;
    if (mmm_que_head >= vi_dev.output_cnt) 
    {
        mmm_que_head = 0;
    }

    return 0;
}

static int mmm_out_queue(void)
{
    int value;

    if (mmm_que_cnt <= 0) 
    {
        return -1;      /* queue is empty. */
    }

    value = mmm_queue[mmm_que_tail];
    mmm_que_tail++;
    mmm_que_cnt--;
    if (mmm_que_tail >= vi_dev.output_cnt) 
    {
        mmm_que_tail = 0;
    }
  
    return value;
}
/* end of for output queue management. */


/******************************************************************************/
/*!                    
* \par  Description:
*       allocate video_in driver's memory.
* \param[in]    pDev -- pointer of vi_dev_t.
* \retval       0 sucess, else failed.
*******************************************************************************/
int vi_mem_init(vi_dev_t *pDev)
{
    unsigned long buf_size;
    
    //SS_PRINTF("mem app_type:%d \n", pDev->app_type);

    if (pDev->app_type == VI_PC_CAMERA_MODE)
    {
        pDev->buf_cnt = 4;
        pDev->queue_cnt = 2;
        pDev->output_cnt = 1; 
    }
    else
    {
        pDev->buf_cnt = BUF_CNT;
        pDev->queue_cnt = QUEUE_CNT;
        pDev->output_cnt = OUTPUT_CNT;  
    }
    
    switch (pDev->app_type)
    {
    case VI_SMALL_MEM_MODE:
        buf_size = (unsigned long)MAX_SMALL_MEMORY_FRAME_SIZE;
        break;
    case VI_DC_MODE: 
        buf_size = (unsigned long)MAX_DC_BUF_SIZE;
        break;          
    case VI_DV_MODE: 
        buf_size = (unsigned long)(MAX_DV_FRAME_SIZE * pDev->buf_cnt);
        break;
    case VI_PC_CAMERA_MODE: 
        buf_size = (unsigned long)(MAX_PC_CAMERA_FRAME_SIZE * pDev->buf_cnt);
        break;
    case VI_ANALOG_TVIN_MODE: 
    case VI_CVBS_TVIN_MODE: 
#if TEST_TVIN_ON_LCD
        buf_size = (unsigned long)(MAX_TVIN_FRAME_SIZE * pDev->buf_cnt + MAX_TVIN_FRAME_SIZE);
#else
        buf_size = (unsigned long)(MAX_TVIN_FRAME_SIZE * pDev->buf_cnt);
#endif
        break; 
    default:
        buf_size = MAX_DC_BUF_SIZE;          
        break; 
    }

#if defined(_ACT_UC_OS_)

#ifdef  _UCOS_NEW_DMA_MEM_MALLOC_
    pDev->mem_len = buf_size;
    
    pDev->mem_addr = (unsigned long)(((unsigned long)malloc_dma(buf_size) & 0x1fffffff) | 0xa0000000);
    printf("vi_mem_init. malloc_dma return addr is %x\n",pDev->mem_addr);
    if ((pDev->mem_addr & 0x1fffffff) == 0x0)
    {
        SS_PRINTF("fail to allocate buffer (size: %dK))", pDev->mem_len / 1024);
        return -1;
    }
#else
    pDev->mem_len = MAX_DC_BUF_SIZE;
    pDev->mem_addr = (unsigned long)SENSOR_BUFF_ADDR;
#endif

    pDev->phy_addr = (unsigned long)(pDev->mem_addr & 0x1fffffff);
#elif defined(_ACT_LINUX_KERNEL_2_6_)
    pDev->mem_len = buf_size;
    pDev->mem_addr = (unsigned long)act_allocate_mem(buf_size);

    if (pDev->mem_addr == (unsigned long)NULL) 
    {
        printk("<0> fail to allocate buffer (size: %dK))", pDev->mem_len / 1024);
        return -ENOMEM;
    }
    pDev->phy_addr = (unsigned long)__pa(pDev->mem_addr);
#endif
    //SS_PRINTF("sensor buffer memory at %x", (unsigned int)pDev->mem_addr);
    //SS_PRINTF("addr=0x%x, size=%d K", (unsigned int)pDev->mem_addr, pDev->mem_len/ 1024);

    return 0;   
}

/******************************************************************************/
/*!                    
* \par  Description:
*       free video_in driver's memory.
* \param[in]    pDev -- pointer of vi_dev_t.
* \retval       0 sucess, else failed.
*******************************************************************************/
int vi_mem_exit(vi_dev_t *pDev)
{
#if defined(_ACT_UC_OS_)

#ifdef  _UCOS_NEW_DMA_MEM_MALLOC_
    pDev->mem_addr &= 0x1fffffff;
    pDev->mem_addr |= 0x80000000;
    free_dma((void *)pDev->mem_addr);
    printf("vi_mem_exit. free_dma is %x. \n",pDev->mem_addr);
#endif

#elif defined(_ACT_LINUX_KERNEL_2_6_)
    act_free_mem((void *)pDev->mem_addr);
#endif

    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       when one new frame is gathered, we should update some frame buffer's state,
*       some buffer will be out of queue, and some will be put into queue.
* \param[in]    pointer of vi_dev_t.
* \retval       0 sucess, else failed.
*******************************************************************************/
int do_update_frame(vi_dev_t *pDev)
{
    int i;
    int ret = 0;
    int i_none = -1;
    unsigned long phy_addr;

    //printk("<0> update_irq\n");
    pDev->frame_cnt++;

    for (i = 0; i < pDev->buf_cnt; i++) 
    {
        switch (pDev->flag[i])
        {
        case DATA_GETTING:
            pDev->flag[i] = DATA_INQUEUE;

            if (ss_in_queue(i) != 0)     /* queue is full. */
            {
                ret = ss_out_queue();
                if (ret < 0) 
                {
                    SS_PRINTF("queue full and can't out!\n");
                    return -1;
                }
                pDev->flag[ret] = DATA_NONE;
                i_none = ret;

                if (ss_in_queue(i) != 0) 
                {
                    SS_PRINTF("can't put in queue!\n");
                    return -1;
                }

                /* one ready frame is lost, and we should signed it. */
                pDev->cur_lost_frames++;
            }

            if (pDev->wait_flag != 0) 
            { 
                pDev->wait_flag = 0;
#if defined(_ACT_LINUX_KERNEL_2_6_)
                wake_up_interruptible(&vi_wait);
#endif
            }
            break;
        case DATA_PREPARE:
            pDev->flag[i] = DATA_GETTING;
            break;
        case DATA_NONE:
            if (i_none == -1)
                i_none = i;
            break;
        default:
            break;
        }
    }

    /* set the next getting buffer's dma dst address. */
    if (i_none != -1) 
    {
        pDev->flag[i_none] = DATA_PREPARE;
        phy_addr = pDev->phy_addr + pDev->phy_offset + pDev->offset[i_none];
        set_dma_dst_addr(pDev->dma_chan, phy_addr);
        set_dma_count(pDev->dma_chan, pDev->resolution_len);
    } 
    else    /* there is no signed DATA_NONE buffer for next dma transfer. */ 
    {
        ret = ss_out_queue();
        if (ret < 0) 
        {
            SS_PRINTF("no DATA_NONE buf and queue is full!\n");
            return -1;
        } 
        else 
        {
            pDev->flag[ret] = DATA_PREPARE;
            phy_addr = pDev->phy_addr + pDev->phy_offset + pDev->offset[ret];
            set_dma_dst_addr(pDev->dma_chan, phy_addr);
            set_dma_count(pDev->dma_chan, pDev->resolution_len);

            /* one ready buffer is lost, and app may should do something! */
            pDev->cur_lost_frames++;
        }
    }

    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       get a frame of data in cycle getting buffer mode. 
* \param[in]    
*       -- type:    \n
*       type == 0:   will wait untill a frame of data is available when no buffer is ready; \n
*       type == 1:   will return 1 when no buffer is ready;
* \param[in]    pointer of vi_dev_t.
* \retval
*       a frame data's offset value in sensor's buffer,     \n
*       ==1 indicate that no frame is ready .
*******************************************************************************/
unsigned long get_frame(int type, vi_dev_t *pDev)
{
    int i_output = -1;
    int ret = 0;
    unsigned long off = 1;
#if defined(_ACT_UC_OS_)
    int i;
#endif
    
    unsigned long flags;
    local_irq_save(flags);

    ret = ss_out_queue();

    if (ret < 0)    /* no ready buffer for app, maybe should wait for it. */
    {
        pDev->wait_flag = 1;

        if (type == 1) 
        {
            off = 1;        /* return directly when no buffer is ready. */
            goto exit;
        }

        local_irq_restore(flags);
#if defined(_ACT_UC_OS_)
        /* wait for dma irq. */
        i = 0;
        do 
        {
            i++;
            if (i > 1800000) 
            {
                SS_PRINTF("wait dma irq too long!\n");
                i = 0;
                off = 1;
                return off;
            }
        } 
        while (pDev->wait_flag != 0);
#elif defined(_ACT_LINUX_KERNEL_2_6_)
        wait_event_interruptible(vi_wait, !pDev->wait_flag);
#endif
        local_irq_save(flags);

        ret = ss_out_queue();
        if (ret < 0) 
        {
            SS_PRINTF("still no ready buf after waiting!\n");
            off = 1;        /* return directly when no buffer is ready. */
            goto exit;
        }
    }

    /* look for signed as DATA_OUTPUT buffer. */
    if (mmm_in_queue(ret) != 0) 
    {
        //SS_PRINTF("free DATA_OUTPUT pls!\n");
        i_output = mmm_out_queue();
        if (i_output >= 0) 
        {
            pDev->flag[i_output] = DATA_NONE;
        }
        if (mmm_in_queue(ret) != 0)
        {
            goto exit;
        }
    }
    pDev->flag[ret] = DATA_OUTPUT;

    off = pDev->offset[ret] + pDev->phy_offset;

    pDev->pad_frames = pDev->cur_lost_frames - pDev->pre_lost_frames;
    pDev->pre_lost_frames = pDev->cur_lost_frames;
#if 0
    if (pDev->pad_frames) 
    {
        SS_PRINTF("in get pad_frames:%d\n", (unsigned int)pDev->pad_frames);
    }
#endif

    exit:
    local_irq_restore(flags);
    return (unsigned long)off;
}

/*
 * input: 
 */
/******************************************************************************/
/*!                    
* \par  Description:
*       free a frame's buffer that get_frame return.
* \param[in] 
*       offset -- offset that get_frame return;
* \param[in]    pointer of vi_dev_t.
* \retval       0 sucess, else failed.
*******************************************************************************/
int free_frame(unsigned long offset, vi_dev_t *pDev)
{
    int i, n;
    int ret = -1;

    unsigned long flags;
    local_irq_save(flags);
    
    for (i = 0; i <= vi_dev.output_cnt; i++) 
    {
        if (i == vi_dev.output_cnt) 
        {
            goto exit;
        }

        n = mmm_out_queue();
        if (n < 0)
        {
            goto exit;
        }

        if (pDev->offset[n] == offset) 
        {
            pDev->flag[n] = DATA_NONE;
            ret = 0;
            goto exit;
        } 
        else 
        {
            if (mmm_in_queue(n) != 0)
            {
                goto exit;
            }
        }
    }

    exit:
    local_irq_restore(flags);
    if (ret != 0)
    {
        SS_PRINTF("offset is wrong, can't free buf!\n");
    }
    return ret;
}


