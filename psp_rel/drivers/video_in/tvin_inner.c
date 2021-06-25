/*
********************************************************************************
*                   213x --- tvin's driver
*                (c) Copyright 2007 - 2010, Actions Co,Ld. 
*                        All Right Reserved 
*
* History:        
*      <author>      <time>       <version >    <desc>
*       houjingkun      2008/06/18     1.0         build this file 
*       new added tvin_inner.h and tvin_inner.c for video in device driver.
********************************************************************************/ 
/*!
 * \file   tvin_inner.c
 * \brief  
 *      Tvin driver logic layer's API.
 * \author houjingkun
 * \par GENERAL DESCRIPTION:
 *      Tvin driver logic layer's API.  just called by dirver itself.
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
#include "tvin_hw.h"
#include "tvin_inner.h"

#if ADD_TVIN_DRV

/* begin to recieve tvin video frame data. */
static int start_tvin_capture(void)
{
    int ret = 0;
    ret = vi_capture_prepare();
    if (ret != 0) 
    {
        printf("can't prepare capture.\n");
        return -1;
    }

    tvin_hw_ioctl(TVIN_DMA_INIT, 0);
    set_dma_dst_addr(vi_dev.dma_chan, (vi_dev.phy_addr + vi_dev.phy_offset + vi_dev.offset[0]));
    set_dma_count(vi_dev.dma_chan, vi_dev.resolution_len);

    /* start dma! */
    tvin_hw_ioctl(TVIN_DMA_START, 0);
    udelay(1);
    set_dma_dst_addr(vi_dev.dma_chan, (vi_dev.phy_addr + vi_dev.phy_offset + vi_dev.offset[1]));

    return 0;
}

/* stop recieving tvin video frame data. */
static int stop_tvin_capture(void)
{
    /* stop dma! */
    tvin_hw_ioctl(TVIN_DMA_STOP, 0);
    set_dma_count(vi_dev.dma_chan, 0x0);

    vi_capture_finish();

    return 0;
}

static int tvin_set_resolution(unsigned int type)
{
    unsigned int width, height, i;
    unsigned int len = 0;
    int ret = 0;

    if (!((0x1 << type) & TVIN_PIXEL_BITMAP))
    {
        return -1;
    }

    switch (type) 
    {
    case VI_TVIN_RES_PAL352X288:
        width = 352;
        height = 288;
        break;
    case VI_TVIN_RES_NTSC352X240:
        width = 352;
        height = 240;
        break;
    case VI_TVIN_RES_PAL360X288:
        width = 360;
        height = 288;
        break;
    case VI_TVIN_RES_NTSC360X240:
        width = 360;
        height = 240;
        break;
    default:
        printf("resolution type error!\n");
        return -1;
    }
    len = width * height * 2;

    vi_dev.width = width;
    vi_dev.height = height;
    vi_dev.resolution_len = len;
    vi_dev.resolution = type;
    vi_dev.phy_offset = TVIN_PHY_ADDR_OFFSET;
    vi_dev.first_frame = 1;

    for (i = 0; i < BUF_CNT; i++)
    {
        vi_dev.offset[i] = vi_dev.resolution_len * i;
    }

    ret = tvin_hw_ioctl(TVIN_SET_RESOLUTION, type);

    return ret;
}

/* how many video frames is passed between every dma restart. */
#define DEFAULT_RESTART_FRAME_CNT   50     

/* when on tvin video frame is recieved, we should do something in dma' irq function. */
static void tvin_irq_func(void)
{
    unsigned int val = 0;
    static unsigned int irq_cnt = 0;
    irq_cnt++;

    //printf("tvin irq\n");
    
    /* DEFAULT_RESTART_FRAME_CNT video frames is passed between every dma restart. */
    if ((irq_cnt % DEFAULT_RESTART_FRAME_CNT) == 0) 
    {
        // || (vi_que_whether_full())) 
        /* 
        * in order to fix the hareware's bug that when write or read nand flash, 
        * the tvin's frame data will have some data lost. 
        * so, we add hardware reset function, restart dma to ensure vsync is synchronous.
        */
        tvin_hw_ioctl(TVIN_GET_DMA_REMAIN, (unsigned int)&val);

        /* if dma remain is less than 1/4 dma count, there have some margin risk to process something. */
        if (val > (vi_dev.resolution_len >> 2)) 
        {
            //tvin_hw_ioctl(TVIN_GET_DMA_ADDR, &val);
            //vi_dev.cur_dma_dst = (unsigned long)val;
            reset_dma(vi_dev.dma_chan);
            tvin_hw_ioctl(TVIN_DMA_START, 0);
            //printf("tv restart\n");
            //return;
        }
    }

#if _TVIN_USE_MULTI_FRAME_BUF_
    do_update_frame(&vi_dev);
#endif
    return;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       initialize tvin driver, used when install driver.
* \param[in]    type -- refer to ss_work_mode_t, here must be VI_CVBS_TVIN_MODE.
* \retval       0 sucess, else failed.
*******************************************************************************/
int tvin_init(unsigned int type)
{
    int ret = 0;

    ret = vi_para_init(&vi_dev);
    if (ret != 0)
    {
        goto exit;
    }
    vi_dev.app_type = VI_CVBS_TVIN_MODE;
    vi_dev.irq_func = tvin_irq_func;
    vi_dev.phy_offset = TVIN_PHY_ADDR_OFFSET;

    ret = tvin_hw_init();
    if (ret != 0)
    {
        goto exit;
    }
    ret = tvin_set_resolution(DEFAULT_TVIN_MODE);
    if (ret != 0)
    {
        goto exit;
    }

    exit:
    return ret;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       uninstall tvin driver, used when uninstall driver.
* \param[in]    null
* \retval       0 sucess, else failed.
*******************************************************************************/
int tvin_exit(void)
{
    vi_para_exit(&vi_dev);
    return tvin_hw_exit();
}

/******************************************************************************/
/*!                    
* \par  Description:
*       realize for device open, request some system resource.
* \param[in]    type -- refer to ss_work_mode_t, here must be VI_CVBS_TVIN_MODE.
* \retval       0 sucess, else failed.
*******************************************************************************/
int tvin_open(unsigned int type)
{
    int ret = 0;
    
    ret = init_tvin_ic();
    if (ret != 0)
    {
        printf("tvin dev is not normal!\n");
        goto exit;
    }
    
    if (vi_dev.opened != 0) 
    {
        printf("tvin dev is opened already!\n");
        vi_dev.opened++;
        return 0;
    }
    
    ret = vi_common_open(VI_CVBS_TVIN_MODE);
    if (ret != 0)
    {
        goto exit;
    }    
    vi_dev.opened = 1;
    
    exit:
    return ret;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       realize for device close, release some system resource.
* \param[in]    type -- refer to ss_work_mode_t, here must be VI_CVBS_TVIN_MODE.
* \retval       0 sucess, else failed.
*******************************************************************************/
int tvin_close(unsigned int type)
{
    if (vi_dev.opened == 0) 
    {
        printf("tvin not opened!\n");
        return 0;
    }
    
    vi_dev.opened--;
    
    if (vi_dev.opened == 0) 
    {
        stop_tvin_capture();
        vi_common_close(VI_CVBS_TVIN_MODE);
    }
    return 0;
}

#if TEST_TVIN_ON_LCD
static int copy_pal_to_qvga(short *src, short *dst)
{
    int i, j;

#if 0
    printf("src:%x dst%x \n", (unsigned int)src, (unsigned int)dst);
    printf("vi_dev.resolution:%x %d\n", vi_dev.resolution, vi_dev.resolution);
    printf("vi_dev.width:%x  %d\n", vi_dev.width, vi_dev.width);
    printf("vi_dev.height:%x  %d\n", vi_dev.height, vi_dev.height);
    printf("vi_dev.resolution_len:%x  %d\n", vi_dev.resolution_len, vi_dev.resolution_len);
    printf("vi_dev.phy_offset:%x  %d\n", vi_dev.phy_offset, vi_dev.phy_offset);
#endif
    printf("vi_dev.width:%x  %d\n", vi_dev.width, vi_dev.width);
    printf("vi_dev.height:%x  %d\n", vi_dev.height, vi_dev.height);
    //src += 360 * 24 + 16;
    src += vi_dev.width * (vi_dev.height-240) / 2 + (vi_dev.width - 320)/2;

    //for (i = 0; i < 60; i++)
    for (i = 0; i < 240; i++)
    {
        for (j = 0; j < 320; j++)
        {
            *dst++ = *src++;
        }
        //src += 40;
        src += (vi_dev.width-320);
    }

    return 0;
}
#endif  /* TEST_TVIN_ON_LCD */ 

#define _TVIN_DEBUG_ 0
#if _TVIN_DEBUG_
static int get_cnt = 0;
static int cur_buf_index = 0;
#endif

#if _TVIN_USE_MULTI_FRAME_BUF_
/******************************************************************************/
/*!                    
* \par  Description:
*       provide for video in driver's ioctl function.
* \param[in]   cmd -- command, refer to video_in.h.
* \param[in]   arg -- argument of ioctl.
* \retval      0 sucess, else failed.
*******************************************************************************/
int tvin_ioctl(unsigned int cmd, unsigned long arg)
{
    int ret = 0;
    unsigned int value;
    unsigned long * pValue;
#if TEST_TVIN_ON_LCD
    unsigned short* pSrc;
    unsigned short* pDst;
#endif

    pValue = (unsigned long *)arg;

    switch (cmd)
    {
    case 0:
        tvin_hw_ioctl(0, 0);
        break;
    case VI_IOGET_PIXEL_BITMAP:
        value = TVIN_PIXEL_BITMAP;
        put_user(value, pValue);
        return 0;
    case VI_IOGET_BUFF_LEN:
        value = vi_dev.mem_len;
        put_user(value, pValue);
        break;
    case VI_IOGET_PHYS_ADDR:
        value = vi_dev.phy_addr;
        put_user(value, pValue);
        break;
    case VI_IOGET_RESOLUTION:
        value = vi_dev.resolution;
        put_user(value, pValue);
        return 0;       
    case VI_IOSET_RESOLUTION:
        return tvin_set_resolution(arg);
/*
    case SENSOR_IOGET_WORK_MODE:
        value = vi_dev.app_type;
        put_user(value, pValue);
        break;
*/        
    case VI_IOSET_CYCLE_START:
        return start_tvin_capture();

    case VI_IOSET_CYCLE_STOP:
        return stop_tvin_capture();

    case VI_IOGET_ONE_FRAME:
    case VI_IOGET_CYCLE_FRAME:
    case VI_IOGET_CYCLE_FRAME_TRY:
#if _TVIN_DEBUG_
        get_cnt++;

        if (get_cnt > 100) 
        {
            cur_buf_index++;
            if (cur_buf_index >= 4) 
                cur_buf_index = 0;

            value = 360 * 288 * 2 * cur_buf_index + vi_dev.phy_offset;
            pSrc = (short *)(vi_dev.mem_addr + value);
            pDst = (short *)(vi_dev.mem_addr + (vi_dev.resolution_len * 4) + vi_dev.phy_offset);
            copy_pal_to_qvga(pSrc, pDst);

            value = (360 * 288 * 2 * 4) + vi_dev.phy_offset;
            put_user(value, pValue);
            return 0;
        } 
        else if (get_cnt == 100) 
        {
            stop_tvin_capture();
            value = (360 * 288 * 2 * 4) + vi_dev.phy_offset;
            put_user(value, pValue);
            return 0;
        } 
        else 
        {
        }
#endif
        if (vi_dev.dma_status == 1) 
        {
            /* currently pc camera shouldn't wait??? */
            if (cmd == VI_IOGET_CYCLE_FRAME_TRY) 
            {
                value = get_frame(1, &vi_dev);
            } 
            else 
            {
                value = get_frame(0, &vi_dev);
            }
        } 
        else 
        {
            return -1;
        }

        //printf("offset:%d %d\n", value, (value / (360 * 288 * 2)));
        if (value == 1) 
        {
            value = vi_dev.phy_offset;  /* inorder to avoid unpredict processing. */
            put_user(value, pValue);
            return -1;
        } 
        else 
        {
#if TEST_TVIN_ON_LCD
            pSrc = (short *)(vi_dev.mem_addr + value);
            pDst = (short *)(vi_dev.mem_addr + (vi_dev.resolution_len * 5) + vi_dev.phy_offset);
            copy_pal_to_qvga(pSrc, pDst);

            value = (vi_dev.resolution_len * 5) + vi_dev.phy_offset;
            put_user(value, pValue);
            return 0;
#endif
            put_user(value, pValue);
        }
        return 0;       

    default:
        if ((cmd & 0xfff00) == VI_IO_CMD_BASE)
        {
            return tvin_hw_ioctl(cmd, arg);
        }
        else
        {
            return -1;
        }
    }

    return ret;
}

#else

/******************************************************************************/
/*!                    
* \par  Description:
*       provide for video in driver's ioctl function.
* \param[in]   cmd -- command, refer to video_in.h.
* \param[in]   arg -- argument of ioctl.
* \retval      0 sucess, else failed.
*******************************************************************************/
int tvin_ioctl(unsigned int cmd, unsigned long arg)
{
    int ret = 0;
    unsigned int value;
    unsigned long * pValue;
#if TEST_TVIN_ON_LCD
    unsigned short* pSrc;
    unsigned short* pDst;
#endif

    pValue = (unsigned long *)arg;

    switch (cmd)
    {
    case 0:
        tvin_hw_ioctl(0, 0);
        break;
    case VI_IOGET_BUFF_LEN:
        value = vi_dev.mem_len;
        put_user(value, pValue);
        break;
    case VI_IOGET_PHYS_ADDR:
        value = vi_dev.phy_addr;
        put_user(value, pValue);
        break;
/*      
    case SENSOR_IOGET_WORK_MODE:
        value = vi_dev.app_type;
        put_user(value, pValue);
        break;
*/        
    case VI_IOSET_CYCLE_START:
        start_tvin_capture();
        set_dma_dst_addr(vi_dev.dma_chan, (vi_dev.phy_addr + vi_dev.phy_offset + vi_dev.offset[0]));
        disable_dma_tcirq(vi_dev.dma_chan);
        disable_dma_htcirq(vi_dev.dma_chan);
        return 0;

    case VI_IOSET_CYCLE_STOP:
        return stop_tvin_capture();

    case VI_IOGET_ONE_FRAME:
    case VI_IOGET_CYCLE_FRAME:
    case VI_IOGET_CYCLE_FRAME_TRY:
        value = vi_dev.phy_offset;  /* inorder to avoid unpredict processing. */
        pSrc = (short *)(vi_dev.mem_addr + value);
        pDst = (short *)(vi_dev.mem_addr + (vi_dev.resolution_len * 4) + vi_dev.phy_offset);
        //printf("offset:%x %d, mem_addr(pSrc): %x\n", value, (value / (360 * 288 * 2)), pSrc);
#if TEST_TVIN_ON_LCD
        copy_pal_to_qvga(pSrc, pDst);

        value = (vi_dev.resolution_len * 4) + vi_dev.phy_offset;
        put_user(value, pValue);
        return 0;
#endif
        put_user(value, pValue);
        return 0;       

    default:
        break;
    }

    return ret;
}

#endif /* _TVIN_USE_MULTI_FRAME_BUF_ */

#else

/******************************************************************************/
/*!                    
* \par  Description:
*       realize for device open, request some system resource.
* \param[in]    type -- refer to ss_work_mode_t, here must be VI_CVBS_TVIN_MODE.
* \retval       0 sucess, else failed.
*******************************************************************************/
int tvin_open(unsigned int type)
{
    return 0;
}

/* when on tvin video frame is recieved, we should do something in dma' irq function. */
static void tvin_irq_func(void)
{
    return;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       initialize tvin driver, used when install driver.
* \param[in]    type -- refer to ss_work_mode_t, here must be VI_CVBS_TVIN_MODE.
* \retval       0 sucess, else failed.
*******************************************************************************/
int tvin_init(unsigned int type)
{
    vi_dev.app_type = VI_CVBS_TVIN_MODE;
    vi_dev.irq_func = tvin_irq_func;

    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       uninstall tvin driver, used when uninstall driver.
* \param[in]    null
* \retval       0 sucess, else failed.
*******************************************************************************/
int tvin_exit(void)
{
    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       realize for device close, release some system resource.
* \param[in]    type -- refer to ss_work_mode_t, here must be VI_CVBS_TVIN_MODE.
* \retval       0 sucess, else failed.
*******************************************************************************/
int tvin_close(unsigned int type)
{
    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       provide for video in driver's ioctl function.
* \param[in]   cmd -- command, refer to video_in.h.
* \param[in]   arg -- argument of ioctl.
* \retval      0 sucess, else failed.
*******************************************************************************/
int tvin_ioctl(unsigned int cmd, unsigned long arg)
{
    return 0;
}

#endif   /* ADD_TVIN_DRV */

