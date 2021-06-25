/*
********************************************************************************
*                   213x --- sensor's driver
*                (c) Copyright 2007 - 2010, Actions Co,Ld. 
*                        All Right Reserved 
*
* History:        
*      <author>      <time>       <version >    <desc>
*       houjingkun      2008/06/18     1.0         build this file 
********************************************************************************/ 
/*!
 * \file   sensor_inner.c
 * \brief  
 *      Sensor driver logic layer's API, used in driver inner.
 * \author houjingkun
 * \par GENERAL DESCRIPTION:
 *      Sensor driver's internal API, just called by dirver itself.
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

static int start_cycle_update(void);
static int stop_cycle_update(int release_pin);
static int update_sensor_data(unsigned char flag);
//int   i2c_clk_lock=0;                      //µ˜∆µ∆⁄º‰Ω˚÷πI2C≤Ÿ◊˜

static int (*sensor_request_clkadjust)
           (unsigned int clk, 
           void (*handler)(unsigned int clk, curclk_t *curclk, void *dev_id, unsigned int cmd, unsigned int direction),
           const void *dev_id);

static int (*sensor_free_clkadjust)(unsigned int clk, const void *dev_id);

static int (*sensor_get_clock)(unsigned int cmd, const unsigned int *result);

static int(* get_pclock)(unsigned int cmd, unsigned int *result);

os_event_t *sensor_event=NULL;

unsigned int current_i2c_div=0x10;
unsigned int  set_sensor_i2c_clk_div(unsigned int pclk)
{
//    SS_PRINTF(" pclk= 0x%x",pclk);
    current_i2c_div= (pclk*1024)/(I2C_CLK*16);
//    SS_PRINTF(" i2c_div= 0x%x\n",current_i2c_div);
}


void   i2c_clk_init_set(void)
{
    unsigned int pclk;
    get_pclock=kernel_sym("get_clock");
    get_pclock(GET_PCLK, &pclk);
    current_i2c_div= (pclk*1024)/(I2C_CLK*16);
//     SS_PRINTF(" pclk= 0x%x\n",pclk);   
//     SS_PRINTF(" i2c_div= 0x%x\n",current_i2c_div);
} 


static void sensor_i2cclk_cb(unsigned int clk, curclk_t *curclk, void *dev_id, unsigned int cmd, unsigned int direction)
{    
    if(cmd == CHANGEPLL_END)
    {
        set_sensor_i2c_clk_div(curclk->corepll / (curclk->cclk_div  * curclk->sclk_div *curclk->pclk_div));    
    }
    else if(cmd == CHANGEPLL_START)
    {

    }
    else
    {       
    }
    
    return ;
}
/******************************************************************************/
/*!                    
* \par  Description:
*       open sensor device, and request some system resource.
* \param[in]
*   type --      \n
*   type == 0: used for DC;     \n
*   type == 1: used for DV;     \n
*   type == 2: used for pc camera; refer to enum of ss_work_mode_t.
* \return      
* \retval      0 sucess, else failed.
*******************************************************************************/
int ss_sensor_open(unsigned int type)
{
    int ret = 0;

    i2c_clk_init_set();
    
    sensor_set_flashlight(SS_FL_OFF);
    ret = sensor_init_param_reg();
    if (ret != 0)
    {
        printf("sensor dev is not normal!\n");
        goto exit;
    }

    if (vi_dev.opened != 0) 
    {
        SS_PRINTF("sensor dev is opened already!\n");
        vi_dev.opened++;
        return 0;
    }
    
    vi_dev.app_type = type;
    
    ret = vi_common_open(vi_dev.app_type);
    if (ret != 0)
    {
        vi_common_close(vi_dev.app_type);
        goto exit;
    }
        
    ret = sensor_init_work_mode(vi_dev.app_type);
    vi_dev.opened = 1;
    exit:
    return ret;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       close sensor device. release some system resource.
* \param[in]
*   type --      \n
*   type == 0: used for DC;     \n
*   type == 1: used for DV;     \n
*   type == 2: used for pc camera; refer to enum of ss_work_mode_t.
* \retval      0 sucess, else failed.
*******************************************************************************/
int ss_sensor_close(unsigned int type)
{
    if (vi_dev.opened == 0) 
    {
        SS_PRINTF("sensor not opened!\n");
        return 0;
    }
    vi_dev.opened--;
    
    if (vi_dev.opened == 0) 
    {    
        sensor_set_flashlight(SS_FL_OFF);
        stop_cycle_update(0);
    
        /* disable sensor interface */
        if (ic_type == 0x63) 
        {
            act_writel(0x0000000c, M_BT_MODESEL);
        } 
        else if (ic_type == 0x96) 
        {
            //act_writel(0x00000000, M_BT_MODESEL);
        }
        else
        {
            return -1;
        }
    
        udelay(1);
    
        vi_common_close(type);
    }

    return 0;
}

static void ss_irq_func(void)
{
    do_update_frame(&vi_dev);
    return;
}

inline static int update_sensor_data_once(unsigned char flag)
{
    //unsigned int flags;
    unsigned int count;

    if (vi_dev.mem_len == 0)
        return 0;

    //spin_lock_irqsave(&sensor_lock, flags);

    reset_dma(vi_dev.dma_chan);
    udelay(1);
    clear_dma_tcirq_pend(vi_dev.dma_chan);
    clear_dma_htcirq_pend(vi_dev.dma_chan);

    /*
     * 0xa08401ac DMA mode-Sdram burst=8
     * 0x608401ac DMA mode-Sdram burst=4
     * 0x008401ac DMA mode-Sdram single 
     */
    set_dma_mode(vi_dev.dma_chan, 0x608401ac);
    set_dma_src_addr(vi_dev.dma_chan, (M_VDCSTIFIFODATA - 0xa0000000));

    if (flag != 0) 
    {
        set_dma_dst_addr(vi_dev.dma_chan, (vi_dev.phy_addr + vi_dev.resolution_len));
    } 
    else 
    {
        set_dma_dst_addr(vi_dev.dma_chan, vi_dev.phy_addr);
    }

    count = vi_dev.resolution_len;
    set_dma_count(vi_dev.dma_chan, count);

    udelay(1);
    /* cmos sensor interface disable. */
    if (ic_type == 0x63) 
    {
        act_writel(0x0000000c, M_BT_MODESEL);
    } 
    else if (ic_type == 0x96) 
    {
        act_writel(0x00000016, M_BT_MODESEL);
    }
    else
    {
        return -1;
    }
    udelay(1);

    dma_mode_start(vi_dev.dma_chan);
    vi_dev.start_dma = 1;
    /* cmos sensor interface enable. */
    if (ic_type == 0x63) 
    {
        act_writel(0x0000000d, M_BT_MODESEL);
    } 
    else /* (ic_type == 0x96) */
    {
        act_writel(0x00000017, M_BT_MODESEL);
    }

    udelay(1);

    /* wait until a frame data is catched. */
    if (cycle_query_dma(vi_dev.dma_chan, TIME_VAL) != 0) 
    {
        reset_dma(vi_dev.dma_chan);
        //spin_unlock_irqrestore(&sensor_lock, flags);
        return -1;
    }

    udelay(1);
    /* cmos sensor interface disable. */
    //act_writel(0x0000000c, M_BT_MODESEL);
    //udelay(1);
    //spin_unlock_irqrestore(&sensor_lock, flags);

    return 0;
}

inline static int update_sensor_data_thrice(unsigned char flag)
{
    //unsigned int flags;
    unsigned int count, tmp;
    //printf("update_sensor_data_thrice \n");
    if (vi_dev.mem_len == 0)
    {
        return 0;
    }
    
    act_writel(0x00000016, M_BT_MODESEL);
    //spin_lock_irqsave(&sensor_lock, flags);
    
    act_writel(0x112, M_CSIFIFOCTL);
    udelay(1000);   

    reset_dma(vi_dev.dma_chan);
    
    udelay(1);
    clear_dma_tcirq_pend(vi_dev.dma_chan);
    clear_dma_htcirq_pend(vi_dev.dma_chan);
    tmp = act_readl(M_CSIFIFOCTL);      
    if ((tmp & 0x100)!=0)
    {
        printf("before trans csi fifo error.\n");
    } 
    /* disable yuv channel */
    //act_writel(0x000002cf, YUV2RGB_Control);

    set_dma_mode(vi_dev.dma_chan, 0x708401ac);
    set_dma_src_addr(vi_dev.dma_chan, (M_VDCSTIFIFODATA - 0xa0000000));

    if (flag != 0) 
    {
        set_dma_dst_addr(vi_dev.dma_chan, 
              vi_dev.phy_addr + vi_dev.resolution_len);
    } 
    else 
    {
        set_dma_dst_addr(vi_dev.dma_chan, vi_dev.phy_addr);
    }

    count = vi_dev.resolution_len;
    if (count>0xf0000)
    {
        set_dma_count(vi_dev.dma_chan, 0xF0000);
        count = count - 0xf0000;
    }
    else
    {
        set_dma_count(vi_dev.dma_chan, count);
        count = 0;      
    }
    udelay(1);
    /* cmos sensor interface disable. */
    if (ic_type == 0x63)
    {
        act_writel(0x0000000c, M_BT_MODESEL);
    }
    else if (ic_type == 0x96)
    {
        act_writel(0x00000016, M_BT_MODESEL);
    }
    else
    {
        return -1;
    }

    udelay(1);
    dma_mode_start(vi_dev.dma_chan);
    udelay(1);

    /* cmos sensor interface enable. */
    if (ic_type == 0x63)
    {
        act_writel(0x0000000d, M_BT_MODESEL);
    }
    else if (ic_type == 0x96)
    {
        act_writel(0x00000017, M_BT_MODESEL);
    }
    else
    {
        return -1;
    }
        
    while(count != 0)
    {
        clear_dma_tcirq_pend(vi_dev.dma_chan);
        set_dma_dst_addr(vi_dev.dma_chan, (vi_dev.phy_addr + vi_dev.resolution_len) - count);
        if (count>0xf0000)
        {
            set_dma_count(vi_dev.dma_chan, 0xF0000);
            count = count - 0xf0000;
        }
        else
        {
            set_dma_count(vi_dev.dma_chan, count);
            count = 0;      
        }       
        do
        {
            tmp = (unsigned int)get_dma_tcirq_pend(vi_dev.dma_chan);
        } while (!tmp);
    }
 
    clear_dma_tcirq_pend(vi_dev.dma_chan);
    udelay(1);
    set_dma_mode(vi_dev.dma_chan, 0x608401ac); /* quit reload mode */
    set_dma_count(vi_dev.dma_chan, 0x0);

    if (cycle_query_dma(vi_dev.dma_chan, TIME_VAL) != 0) 
    {
        reset_dma(vi_dev.dma_chan);
        //spin_unlock_irqrestore(&sensor_lock, flags);
        return -1;
    }

    udelay(1);
    /* cmos sensor interface disable. */
    if (ic_type == 0x63)
    {
        act_writel(0x0000000c, M_BT_MODESEL);
    }
    else if (ic_type == 0x96)
    {
        act_writel(0x00000016, M_BT_MODESEL);
    }
    else
    {
        return -1;
    }
    
    udelay(1);
    //spin_unlock_irqrestore(&sensor_lock, flags);
    tmp = act_readl(M_CSIFIFOCTL);  
    if ((tmp & 0x100)!=0)
    {
        printf("after trans csi fifo error.\n");
        return -1;
    }
    return 0;
}

int update_sensor_data(unsigned char flag)
{
    if ((vi_dev.resolution == VI_RES_1280X960) || (vi_dev.resolution == VI_RES_1280X1024)|| 
        (vi_dev.resolution == VI_RES_1600X1200))
    {
        return update_sensor_data_thrice(flag);
    } 
    else 
    {
        return update_sensor_data_once(flag);
    }
}

static int start_cycle_update(void)
{
    if ((vi_dev.resolution != VI_RES_320X240) 
     && (vi_dev.resolution != VI_RES_224X176)
     && (vi_dev.resolution != VI_RES_640X480)) 
    {
        SS_PRINTF("current resolution can't surpport cycle mode!\n");
        return -1;
    }

    if (vi_capture_prepare() != 0) 
    {
        return -1;
    }

    /*
     * 0xb08401ac DMA mode-Sdram burst=8
     * 0x708401ac DMA mode-Sdram burst=4
     * 0x108401ac DMA mode-Sdram single 
     */
    set_dma_mode(vi_dev.dma_chan, 0x708401ac);
    //SS_PRINTF("dma-mode is %x \n", get_dma_mode(vi_dev.dma_chan));
    set_dma_src_addr(vi_dev.dma_chan, (M_VDCSTIFIFODATA - 0xa0000000));
    set_dma_dst_addr(vi_dev.dma_chan, (vi_dev.phy_addr + vi_dev.phy_offset + vi_dev.offset[0]));
    set_dma_count(vi_dev.dma_chan, vi_dev.resolution_len);
    udelay(1);

    /* cmos sensor interface disable. */
    if (ic_type == 0x63) 
    {
        act_writel(0x0000000c, M_BT_MODESEL);
    } 
    else if (ic_type == 0x96) 
    {
        act_writel(0x00000016, M_BT_MODESEL);
    }
    else
    {
        return -1;
    }
    udelay(1);
    /* start dma! */
    dma_mode_start(vi_dev.dma_chan);

    /* cmos sensor interface enable. */
    if (ic_type == 0x63) 
    {
        act_writel(0x0000000d, M_BT_MODESEL);
    } 
    else /* (ic_type == 0x96) */
    {
        act_writel(0x00000017, M_BT_MODESEL);
    }

    udelay(1);

    //spin_unlock_irqrestore(&sensor_lock, flags);

    /*
     * in reload mode, we should set the next dest for next dma transfer 
     * before current dma transfer finished.
     */
    set_dma_dst_addr(vi_dev.dma_chan, (vi_dev.phy_addr + vi_dev.phy_offset + vi_dev.offset[1]));

    return 0;
}

/*
 * input: 
 *  release_pin == 1, release pin is needed;
 *  release_pin == 0, release pin is not needed;
 */
static int stop_cycle_update(int release_pin)
{
    /* stop dma! */
    set_dma_mode(vi_dev.dma_chan, 0x608401ac);
    set_dma_count(vi_dev.dma_chan, 0x0);

    vi_capture_finish();

    /* cmos sensor interface disable. */
    if (ic_type == 0x63) 
    {
        act_writel(0x0000000c, M_BT_MODESEL);
    } 
    else if (ic_type == 0x96) 
    {
        act_writel(0x00000016, M_BT_MODESEL);
    }
    else
    {
        return -1;
    }

    udelay(1);
    return 0;
}

inline static int ss_ioctl0(unsigned int cmd, unsigned long arg)
{
    int ret = 0;

    switch (cmd) 
    {
    case VI_IOSET_RESOLUTION:
        //if (vi_dev.resolution == arg) return 0;
        return sensor_change_resolution(arg);

    case SENSOR_IOSET_EXPOSE_MODE:      /* arg == 0: auto mode; arg == 1: night mode. */
        return sensor_set_expose_mode(arg);

    case SENSOR_IOSET_SPECIAL_MODE:
        return sensor_set_special_mode(arg);        

    case SENSOR_IOSET_CONTRAST:
        return sensor_set_contrast((unsigned int)arg);

    case SENSOR_IOSET_WHITE_BALANCE:
        return sensor_set_white_balance((unsigned int)arg);

    case SENSOR_IOSET_FOCUS:
        return sensor_set_focus((unsigned int)arg);

    case SENSOR_IOSET_FLASHLIGHT:
        return sensor_set_flashlight((unsigned int)arg);        

    case VI_IOSET_CYCLE_START:
        return start_cycle_update();

    case VI_IOSET_CYCLE_STOP:
        return stop_cycle_update(1);

    case SENSOR_IOSET_BRIGHTNESS:
        return sensor_set_brightness((unsigned int)arg);

    case VI_IOFREE_CYCLE_FRAME:
        return free_frame(arg, &vi_dev);

    default:
        ret = -1;
        break;
    }

    return ret;
}

inline static int ss_ioctl1(unsigned int cmd, unsigned long arg)
{
    unsigned long value;
    unsigned long * pValue;
    int tmp;
    int ret = 0;

    pValue = (unsigned long *)arg;

    switch (cmd) 
    {
    case VI_IOGET_BUFF_LEN:
        value = vi_dev.mem_len;
        put_user(value, pValue);
        return 0;       

    case VI_IOGET_PHYS_ADDR:
        value = vi_dev.phy_addr;
        put_user(value, pValue);
        return 0;       

    case VI_IOGET_RESOLUTION:
        value = vi_dev.resolution;
        put_user(value, pValue);
        return 0;       

    case SENSOR_IOGET_EXPOSE_MODE:
        value = sensor_get_expose_mode();
        put_user(value, pValue);
        return 0;

    case SENSOR_IOGET_SPECIAL_MODE:
        value = sensor_get_special_mode();
        put_user(value, pValue);
        return 0;

    case SENSOR_IOGET_CONTRAST:
        value = sensor_get_contrast();
        put_user(value, pValue);
        return 0;

    case SENSOR_IOGET_WHITE_BALANCE:
        value = sensor_get_white_balance();
        put_user(value, pValue);
        return 0;

    case SENSOR_IOGET_FOCUS:
        value = sensor_get_focus();
        put_user(value, pValue);
        return 0;

    case SENSOR_IOGET_BRIGHTNESS:
        value = sensor_get_brightness();
        put_user(value, pValue);
        return 0;

    case VI_IOGET_PIXEL_BITMAP:
        value = sensor_get_pixel_bitmap();
        put_user(value, pValue);
        return 0;

    case VI_IOGET_ONE_FRAME:
    case VI_IOGET_CYCLE_FRAME:
    case VI_IOGET_CYCLE_FRAME_TRY:
        if ( ((vi_dev.resolution == VI_RES_320X240) 
          || (vi_dev.resolution == VI_RES_224X176) 
          || (vi_dev.resolution == VI_RES_640X480))
          && (vi_dev.dma_status == 1)) 
        {
            /* currently pc camera shouldn't wait??? */
            if ((cmd == VI_IOGET_CYCLE_FRAME_TRY) 
              || (vi_dev.app_type == VI_PC_CAMERA_MODE)) 
            {
                tmp = 1;    /* if no data ready currently, return right now. */
            } 
            else 
            {
                tmp = 0;    /* if no data ready currently, waiting for it. */
            }

            value = get_frame(tmp, &vi_dev);
            if (value == 1) 
            {
                return -1;
            }
            put_user(value, pValue);
            modify_data(((value+vi_dev.phy_addr) | 0xA0000000), vi_dev.resolution, vi_dev.app_type);//–ﬁ∏¥0307±ﬂ—ÿ»±œ›
            return 0;       
        }

        if (cycle_query_dma(vi_dev.dma_chan, TIME_VAL) != 0) 
        {
            return -1;
        }
        update_sensor_data(0);
        value = 0;
        put_user(value, pValue);
        modify_data(((value+vi_dev.phy_addr) | 0xA0000000),  vi_dev.resolution, vi_dev.app_type);    //–ﬁ∏¥0307±ﬂ—ÿ»±œ›
        return 0; 

    default:
        ret = -1;
        break;
    }

    return ret;
}

inline static int ss_ioctl2(unsigned int cmd, unsigned long arg)
{
    unsigned long value;
    unsigned long * pValue;
    int ret = 0;

    pValue = (unsigned long *)arg;
    //SS_PRINTF("cmd is %x! \n", cmd);

    switch (cmd) 
    {
    case VI_IOGET_PAD_FRAMES:
        value = vi_dev.pad_frames;
        put_user(value, pValue);
        break;

    case VI_IOGET_FRAME_CNT:
        value = vi_dev.frame_cnt;
        put_user(value, pValue);
        break;

    case VI_IOGET_LOST_FRAMES:
        value = vi_dev.cur_lost_frames;
        put_user(value, pValue);
        break;

    case VI_IOGET_RATE_BITMAP:
        value = sensor_get_f_rate_bitmap();
        put_user(value, pValue);
        break;

    case VI_IOSET_FRAME_RATE:
        ret = sensor_set_f_rate(arg);
        break;

    case VI_IOSET_PIXEL_FORMAT:
        vi_dev.pix_fmt = arg;
        ret = sensor_set_pix_fmt(vi_dev.pix_fmt);
        break;

    default:
        ret = -1;
    }

    return ret;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       provide for video in driver's ioctl function.
* \param[in]   cmd -- command, refer to video_in.h.
* \param[in]   arg -- argument of ioctl.
* \retval      0 sucess, else failed.
*******************************************************************************/
int ss_sensor_ioctl(unsigned int cmd, unsigned long arg)
{
    unsigned int tmp;
    int err,ret;
    
    os_sem_pend(sensor_event, 0, &err);

    //SS_PRINTF("ss_cmd is %d -- %x : %d! \n", (cmd & 0xff), arg, arg);
    tmp = cmd & 0xff;

    if (tmp <= 16) 
    {
        ret = ss_ioctl0(cmd, arg);
        goto exit;
    } 
    else if (tmp <= 34) 
    {
        ret = ss_ioctl1(cmd, arg);
        goto exit;
    } 
    else 
    {
        ret = ss_ioctl2(cmd, arg);
        goto exit;
    }
    
    exit:
    os_sem_post(sensor_event);
    return ret;
};

/******************************************************************************/
/*!                    
* \par  Description:
*       init sensor's hardware resource, used in driver install.
* \param[in]
*   type --      \n
*   type == 0: used for DC;     \n
*   type == 1: used for DV;     \n
*   type == 2: used for pc camera; refer to enum of ss_work_mode_t.
* \retval      0 sucess, else failed.
*******************************************************************************/
int ss_sensor_init(unsigned int type)
{
    int ret = 0;
    if (get_sensor_dev(1) != 0) 
    {
        SS_PRINTF("get sensor pin error!\n");
    }
    
    sensor_event = os_sem_create(1);
    
    sensor_set_flashlight(SS_FL_OFF);
    vi_dev.app_type = type;
    vi_para_init(&vi_dev);
    vi_dev.irq_func = ss_irq_func;
    sensor_request_clkadjust = kernel_sym( "request_clkadjust" );
    sensor_free_clkadjust = kernel_sym( "free_clkadjust" ); 
    sensor_get_clock = kernel_sym( "get_clock" );   
    if ((sensor_request_clkadjust ==NULL)||(sensor_free_clkadjust==NULL)||(sensor_get_clock==NULL))
    {
        printk("can't find the kernel_sym fun address");
    }
    ret = sensor_hw_init(&vi_dev);

    //return ret; /* succeed */
    return 0;
};

/******************************************************************************/
/*!                    
* \par  Description:
*       release sensor's hardware resource.
* \param[in]   null
* \retval      0 sucess, else failed.
*******************************************************************************/
int ss_sensor_exit(void)
{
    unsigned char err;
    sensor_hw_exit(&vi_dev);
    release_sensor_dev();
    vi_para_exit(&vi_dev);
    os_sem_del( sensor_event, OS_DEL_ALWAYS,  &err);
    return 0;
};

