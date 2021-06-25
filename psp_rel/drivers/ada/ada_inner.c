/*
********************************************************************************
*                   213x --- usdk130 source file
*                (c) Copyright 2003 - 2008, Actions Co,Ld. 
*                        All Right Reserved 
*
* History:        
*      <author>      <time>       <version >    <desc>
*      houjingkun    2008/08/06     1.0      build this file 
*                                            ada char device driver's inner layer.
********************************************************************************/ 
/*!
 * \file   ada_inner.c
 * \brief  
 *      ada char device driver's inner layer.
 * \author houjingkun
 * \par GENERAL DESCRIPTION:
 * \par EXTERNALIZED FUNCTIONS:
 *       null
 *
 *  Copyright(c) 2003-2008 Actions Semiconductor, All Rights Reserved.
 *
 * \version 1.0
 * \date  2008/08/06
 *******************************************************************************/

#include "ada.h"
#include "ada_config.h"
#include "ada_sys.h"
#include "ada_hw.h"
#include "ada_inner.h"

ada_dev_t ada_dev;


/* reserved for ada test. */
#if 0   /* ADA_DEBUG */
static volatile int ada_call_cnt = 0; 
static volatile int ada_irq_cnt = 0; 

void print_ada_reg(void)
{
#if 0
    unsigned int * p_u32;
    unsigned int i;

    p_u32 = (unsigned int *) ((act_readl(DMA_SRC0) & 0x1fffffff) | 0xa0000000);
    for (i = 0; i < 100; i++)
    {
        printk("addr: 0x%x--0x%x\n", p_u32, *p_u32);
        p_u32++;
    }
#endif
    //printk("ADC_CON is %x \n", act_readl(ADC_CON));
    //printk("ADC_FIFO_CON is %x \n", act_readl(ADC_FIFO_CON));
    //printk("ADC_DATA is %x \n", act_readl(ADC_DATA));
    //printk("ADC_ANALOG is %x \n", act_readl(ADC_ANALOG));
    //printk("DMA_CTL is %x \n", act_readl(DMA_CTL));
    //printk("DMA_CMD0 is %x \n", act_readl(DMA_CMD0));
    //printk("DMA_CNT0 is %x \n", act_readl(DMA_CNT0));
    //printk("DMA_SRC0 is %x \n", act_readl(DMA_SRC0));
    //printk("DMA_DST0 is %x \n", act_readl(DMA_DST0));

    printk("CMU_AUDIOPLL is %x \n", act_readl(CMU_AUDIOPLL));
    printk("DAC_CTL is %x \n", act_readl(DAC_CTL));
    //printk("DAC_FIFOCTL is %x \n", act_readl(DAC_FIFOCTL));
    //printk("DAC_DAT is %x \n", act_readl(DAC_DAT));
    printk("DAC_ANALOG is %x \n", act_readl(DAC_ANALOG));
    return;
}
#endif  /* ADA_DEBUG */


/******************************************************************************/
/*!                    
* \par  Description:
*     this function is provided for volume controll.
* \param[in]   volume -- volume range: 0 ~ 31.
* \param[in]   dac_mute -- dac_mute != 0, dac set to mute, dac_mute == 0, dac set to unmute. 
* \param[in]   dsp_mute -- dsp_mute != 0, dac set to mute, dsp_mute == 0, dac set to unmute. 
*              dsp_mute is prior to dac_mute.
* \retval      0 sucess 
* \retval      -1 failed
*******************************************************************************/
static int dac_set_volume(unsigned int volume, unsigned int dac_mute, unsigned int dsp_mute)
{
    ada_dev.dac_mute = dac_mute;
    ada_dev.dsp_mute = dsp_mute;
    	
    if (volume > 31)
    {
        printk("volume out of range!\n");
        return -1;
    }
    else
    {
        dac_hw_set_volume(volume);
        ada_dev.volume = volume;
    }

    if (volume == 0)
    {        
        dac_hw_ioctl(DAC_HW_MUTE_EN, 1);
        return 0;
    }
       
    if (dsp_mute != 0)
    {
        dac_hw_ioctl(DAC_HW_MUTE_EN, 1);
    }
    else
    {
        if (dac_mute != 0)
        {
            dac_hw_ioctl(DAC_HW_MUTE_EN, 1);
        }
        else
        {
            dac_hw_ioctl(DAC_HW_MUTE_EN, 0);            
        }
    }

    return 0;
};


#if (USE_PCM_PLAY_IOCTL || USE_PCM_PLAY_WRITE)


#define PCM_PLAY_SYNC_CNT   80000   /* provide for sync delay. */

/* type == 0, sleep, type == 1, reset dma right now. */
static int pcm_play_sync(int type)
{
    int ret = 0;
    unsigned int cnt = 0;
    
    if (!ada_dev.play_status) 
    {
        return 0;
    }

    if (type == 0)
    {
        while (((ada_dev.play_status) && (cnt < PCM_PLAY_SYNC_CNT)) 
            && (dma_started(ada_dev.dma_no)))    /* 1 second ? */
        {
            cnt++;
            //set_current_state(TASK_INTERRUPTIBLE);
            //schedule_timeout(1);
            //os_mdelay(1);
            udelay(10);
        }
        if (cnt >= PCM_PLAY_SYNC_CNT)
        {
            printk("wait pcm sync too long!\n");
            ret = -1;
        }
    }

    reset_dma(ada_dev.dma_no);
    ada_dev.play_status = 0;
    ada_dev.task_pend = 0;

    return ret;
}

/******************************************************************************/
/*!                    
* \par  Description:
*   dma irq processing function.
* \param[in]   irq -- irq number.
* \param[in]   dev_id -- device driver's some data structure pointer.
* \retval      null
*******************************************************************************/
static int dac_dma_irq(int irq, void *dev_id, struct pt_regs *reg)
{
    //printk("dma_irq:%d \n", ada_irq_cnt++);

    /* irq == 0x888 is provided for calling dac_dma_irq(). */
    if ((get_dma_tcirq_pend(ada_dev.dma_no)) || (irq == 0x888))     
    {  
        //printk("dac irq\n");
        clear_dma_tcirq_pend(ada_dev.dma_no);

        if (ada_dev.task_pend != 0) 
        {
            ada_dev.task_pend = 0;
        }

        if ((!dma_started(ada_dev.dma_no)) || (!ada_dev.play_status))
        {
            pcm_play_sync(1);
        }
        return IRQ_HANDLED;
    }

    return IRQ_NONE;
}

/******************************************************************************/
/*!                    
* \par  Description:
*   playing pcm data that set by DACIO_SET_PCM_XXX.
* \param[in]   null
* \retval      0 -- success, else -- failed. 
*******************************************************************************/
int pcm_play(void)
{
    unsigned int val;
    int ret = 0;

    //printk("pcm_play:%d \n", ada_call_cnt++);
    if (ada_dev.buf_len <= 0x40) 
    {  
        /* should not be too short, otherwise will no vioce be heard, and dma is abort quickly. */
        return -1;
    }
    if (ada_dev.play_status != 0) 
    {
        ada_dev.task_pend = 1;
        set_dma_src_addr(ada_dev.dma_no, ada_dev.phy_addr);
        set_dma_dst_addr(ada_dev.dma_no, _PA(DAC_DAT));
        set_dma_count(ada_dev.dma_no, ada_dev.buf_len);
        if (ada_dev.buf_len >= MIN_FRAME_LEN)
        {
            set_dma_mode(ada_dev.dma_no, ACT_DAC_DMA_RELOAD_MOD);
        }
        else
        {
            set_dma_mode(ada_dev.dma_no, ACT_DAC_DMA_MOD);
        }

        val = 0;
        while ((val < 400000) && (ada_dev.task_pend))
        {
#if 0
            if (get_dma_remain(ada_dev.dma_no) > MIN_FRAME_LEN)
            {
                set_current_state(TASK_INTERRUPTIBLE);
                schedule_timeout(1);
                if (val > 100)
                    val = 400000;
            }
            else
            {
                udelay(1);
            }
#endif
            udelay(1);
            val++;
        }
        if (val >= 400000) 
        {
            printk("wait pcm playing too long!\n");
            pcm_play_sync(0);
        }
        set_dma_count(ada_dev.dma_no, 0);
        set_dma_mode(ada_dev.dma_no, ACT_DAC_DMA_MOD);

        return 0;
    }

    ret = dac_hw_check_play_set();
    if (ret != 0)
    {
        printk("dac register setting isn't ok! error:%d\n", ret);
    }

    reset_dma(ada_dev.dma_no);
    udelay(1);
    set_dma_src_addr(ada_dev.dma_no, ada_dev.phy_addr);
    set_dma_dst_addr(ada_dev.dma_no, _PA(DAC_DAT));
    set_dma_count(ada_dev.dma_no, ada_dev.buf_len);
    //set_dma_mode(ada_dev.dma_no, ACT_DAC_DMA_MOD);
    set_dma_mode(ada_dev.dma_no, ACT_DAC_DMA_RELOAD_MOD);
    mdelay(1);
    //S_OSTimeDly(60);
    start_dma(ada_dev.dma_no);
    ada_dev.play_status = 1;
    //udelay(1);
    set_dma_count(ada_dev.dma_no, 0);
    set_dma_mode(ada_dev.dma_no, ACT_DAC_DMA_MOD);

    return ret;
}

#endif  /* (USE_PCM_PLAY_IOCTL || USE_PCM_PLAY_WRITE) */

/******************************************************************************/
/*!                    
* \par  Description:
*   dac and adc open, request some resouce, such as dma and memory, and enable 
*   some hardware circuit.
* \param[in]   null
* \retval      0 -- success, else -- failed. 
*******************************************************************************/
int ada_inner_open(void)
{
    int ret = 0;

    /* open pa need 500ms delay before can using it. */
    //mdelay(600);    
//    os_time_dly(60);

    ada_hw_open();
    dac_hw_ioctl(DACIO_SET_PCM_RATE, ada_dev.dac_pcm_rate);
    dac_hw_ioctl(DACIO_SET_PCM_CHANNEL, ada_dev.dac_channel);
    dac_hw_ioctl(DACIO_SET_PCM_BITS, ada_dev.dac_pcm_bits);

    dac_hw_ioctl(ADCIO_SET_PCM_RATE, ada_dev.adc_pcm_rate);
    dac_hw_ioctl(ADCIO_SET_PCM_CHANNEL, ada_dev.adc_channel);
    dac_hw_ioctl(ADCIO_SET_PCM_BITS, ada_dev.adc_pcm_bits);

    adc_hw_set_input(&ada_dev.adc_input);
    adc_hw_set_gain(&ada_dev.adc_gain);
    dac_set_volume(ada_dev.volume, ada_dev.dac_mute, ada_dev.dsp_mute);
#if (USE_PCM_PLAY_IOCTL || USE_PCM_PLAY_WRITE)
    ret = request_act213x_dma(DMA_CHAN_TYPE_BUS, "ada", dac_dma_irq, SA_SHIRQ, (void *)&dac_dma_irq);
    //printk("dac_dma_no is %d \n", ada_dev.dma_no);

    if ((ret < 0) || (ret > 3)) 
    {
        printk("request dma irq failed! \n");
        return -1;
    }
    ada_dev.dma_no = (unsigned int)ret;
    ret = 0;
    clear_dma_htcirq_pend(ada_dev.dma_no);
    clear_dma_tcirq_pend(ada_dev.dma_no);

    //stop_dma(ada_dev.dma_no);
    reset_dma(ada_dev.dma_no);
    enable_dma_tcirq(ada_dev.dma_no);
    disable_dma_htcirq(ada_dev.dma_no);

    set_dma_src_addr(ada_dev.dma_no, ada_dev.phy_addr);
    set_dma_dst_addr(ada_dev.dma_no, _PA(DAC_DAT));
    set_dma_count(ada_dev.dma_no, ada_dev.buf_len);
    set_dma_mode(ada_dev.dma_no, ACT_DAC_DMA_MOD);
#endif  /* (USE_PCM_PLAY_IOCTL || USE_PCM_PLAY_WRITE) */

    dac_set_volume(ada_dev.volume, ada_dev.dac_mute, ada_dev.dsp_mute);
    //adc_hw_set_input(&ada_dev.adc_input);
//exit:
    return ret;
}

/******************************************************************************/
/*!                    
* \par  Description:
*   dac and adc close, release some resouce, such as dma and memory, and disable
*   some circuit.
* \param[in]   null
* \retval      0 -- success, else -- failed. 
*******************************************************************************/
int ada_inner_close(void)
{
#if (USE_PCM_PLAY_IOCTL || USE_PCM_PLAY_WRITE)
    pcm_play_sync(0);
    free_act213x_dma(ada_dev.dma_no);
#endif  /* (USE_PCM_PLAY_IOCTL || USE_PCM_PLAY_WRITE) */
    
    ada_hw_close();

    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*   ada dev's para initialize.
* \param[in]   null.
* \retval      0 -- success, else -- failed. 
*******************************************************************************/
int ada_dev_init(void)
{
    int ret = 0;
    
    ret = ada_hw_init();
    if (ret < 0)
    {
        goto exit;
    }
    ret = dac_hw_init();
    if (ret < 0)
    {
        goto exit;
    }
    ret = adc_hw_init();
    if (ret < 0)
    {
        goto exit;
    }


    ada_dev.open_cnt = 0;
    ada_dev.dac_pcm_rate = ACT_SAMPLE_48K;
    ada_dev.dac_channel = ADA_STEREO;
    ada_dev.dac_pcm_bits = 32;

    ada_dev.adc_pcm_rate = ACT_SAMPLE_48K;
    ada_dev.adc_channel = ADA_STEREO;
    ada_dev.adc_pcm_bits = 32;

    /* adc para init. */
    ada_dev.adc_input.mic = 0;
    ada_dev.adc_input.fm = 0;
    ada_dev.adc_input.linein = 0;
    ada_dev.adc_input.mix = 0;

    /* set input gain */
    ada_dev.adc_gain.mic_gain = 5; 
    ada_dev.adc_gain.linein_gain = 5; 
    ada_dev.adc_gain.fm_gain = 5; 
    ada_dev.adc_gain.adc_gain = 3; 

    ada_dev.volume = 24; 
    ada_dev.max_sys_volume = 0;  
    ada_dev.dac_mute = 0; 
    ada_dev.dsp_mute = 0; 

    ada_dev.dma_no = 0; 
    ada_dev.phy_addr = 0; 
    ada_dev.buf_len = 0; 

    ada_dev.play_status = 0;    /* no task is playing. */
    ada_dev.task_pend = 0;      /* no task is suspend. */

#if USE_PCM_PLAY_WRITE
    ada_dev.pcm_data[0] = (unsigned int *)malloc_dma(PCM_FRAME_LEN * 2);
    ada_dev.pcm_data[0] = (unsigned int *)_UNCACHE_ADDR(ada_dev.pcm_data[0]);
    ada_dev.pcm_data[1] = (unsigned int *)((unsigned int)ada_dev.pcm_data[0] + PCM_FRAME_LEN);
    ada_dev.pcm_bytes = PCM_FRAME_LEN;
#else
    ada_dev.pcm_data[0] = NULL;
    ada_dev.pcm_data[1] = NULL;
    ada_dev.pcm_bytes = 0;
#endif  /* USE_PCM_PLAY_WRITE */

    exit:
    return ret;
}

/******************************************************************************/
/*!                    
* \par  Description:
*   ada dev's para exit.
* \param[in]   null.
* \retval      0 -- success, else -- failed. 
*******************************************************************************/
int ada_dev_exit(void)
{
    int ret = 0;

    ret = dac_hw_exit();
    if (ret < 0)
    {
        goto exit;
    }
    ret = adc_hw_exit();
    if (ret < 0)
    {
        goto exit;
    }
    ret = ada_hw_exit();
    if (ret < 0)
    {
        goto exit;
    }

#if USE_PCM_PLAY_WRITE
    ada_dev.pcm_data[0] = (unsigned int *)_CACHE_ADDR(ada_dev.pcm_data[0]);
    free_dma(ada_dev.pcm_data[0]);
#endif  /* USE_PCM_PLAY_WRITE */

    exit:
    return ret;
}

/******************************************************************************/
/*!                    
* \par  Description:
*     dac's some command request processing.
* \param[in]   cmd -- command for ioctl.
* \param[in]   arg -- argument for ioctl.
* \retval      0 sucess 
* \retval      -1 failed
*******************************************************************************/
int dac_ioctl(unsigned int cmd, unsigned long arg)
{
    unsigned int val; 
    int ret = 0;

    switch (cmd) 
    {
    case DACIO_SET_PCM_RATE:
        pcm_play_sync(0);
        ret = dac_hw_ioctl(cmd, arg);
        if (!ret)
        {
            ada_dev.dac_pcm_rate = arg;
        }
        break;

    case DACIO_GET_PCM_RATE:
        val = ada_dev.dac_pcm_rate;
        put_user(val, (unsigned int*)arg);
        break;

    case DACIO_SET_PCM_CHANNEL:
        pcm_play_sync(0);
        ret = dac_hw_ioctl(cmd, arg);
        if (!ret)
        {
            ada_dev.dac_channel = arg;
        }
        break;

    case DACIO_GET_PCM_CHANNEL:
        val = ada_dev.dac_channel;
        put_user(val, (unsigned int*)arg);
        break;

    case DACIO_SET_PCM_BITS:
        pcm_play_sync(0);
        ret = dac_hw_ioctl(cmd, arg);
        if (!ret)
        {
            ada_dev.dac_pcm_bits = arg;
        }
        break;

    case DACIO_GET_PCM_BITS:
        val = ada_dev.dac_pcm_bits;
        put_user(val, (unsigned int*)arg);
        break;

    case DACIO_SET_VOLUME:
        ret = dac_set_volume(arg, ada_dev.dac_mute, ada_dev.dsp_mute);
        break;

    case DACIO_GET_VOLUME:
        val = ada_dev.volume;
        put_user(val, (unsigned int*)arg);
        break;

    case DACIO_SET_MAX_SYS_VOLUME:
        if( (arg<31) || (arg>42))    
        {
            ret = -1;
        }
        else
        {
            ada_dev.max_sys_volume = arg;
        }
       
        break;

    case DACIO_SET_MUTE:
        ret = dac_set_volume(ada_dev.volume, arg, ada_dev.dsp_mute);
        break;

    case DACIO_PCM_PLAY_ON:
        ret = dac_hw_ioctl(DAC_HW_DSP_EN, (unsigned int)!arg);
        break;

#if (USE_PCM_PLAY_IOCTL || USE_PCM_PLAY_WRITE)
    case DACIO_SET_PCM_ADDR:
        ada_dev.phy_addr = arg;
        break;
        
    case DACIO_SET_PCM_LEN:
        ada_dev.buf_len = arg;
        break;

    case DACIO_SET_PCM_PLAY:
        return pcm_play();

    case DACIO_GET_PCM_SYNC:
        return pcm_play_sync(0);
#endif  /* (USE_PCM_PLAY_IOCTL || USE_PCM_PLAY_WRITE) */

    default:
        ret = dac_hw_ioctl(cmd,arg);
    }

    return ret;
} 

/******************************************************************************/
/*!                    
* \par  Description:
*     adc's some command request processing.
* \param[in]   cmd -- command for ioctl.
* \param[in]   arg -- argument for ioctl.
* \retval      0 sucess 
* \retval      -1 failed
*******************************************************************************/
int adc_ioctl(unsigned int cmd, unsigned long arg)
{
    adc_input_t adc_input;
    adc_gain_t adc_gain;
    unsigned int val; 
    int ret = 0;

    switch (cmd) 
    {
    case ADCIO_SET_PCM_RATE:
        ret = adc_hw_ioctl(cmd, arg);
        if (!ret)
        {
            ada_dev.adc_pcm_rate = arg;
        }
        break;

    case ADCIO_GET_PCM_RATE:
        val = ada_dev.adc_pcm_rate;
        put_user(val, (unsigned int*)arg);
        break;

    case ADCIO_SET_PCM_CHANNEL:
        ret = adc_hw_ioctl(cmd, arg);
        if (!ret)
        {
            ada_dev.adc_pcm_rate = arg;
        }
        break;

    case ADCIO_GET_PCM_CHANNEL:
        val = ada_dev.adc_channel;
        put_user(val, (unsigned int*)arg);
        break;

    case ADCIO_SET_PCM_BITS:
        ret = adc_hw_ioctl(cmd, arg);
        if (!ret)
        {
            ada_dev.adc_pcm_rate = arg;
        }
        break;

    case ADCIO_GET_PCM_BITS:
        val = ada_dev.adc_pcm_bits;
        put_user(val, (unsigned int*)arg);
        break;

    case ADCIO_SET_INPUT:
        copy_from_user((void *)&adc_input, (void *)arg, sizeof(adc_input_t));
        ret = adc_hw_set_input(&adc_input);
        if (!ret)
        {
            ada_dev.adc_input = adc_input;
        }
        else
        {
            printk("set adc input failed!\n");
        }
        break;
    case ADCIO_GET_INPUT:
        copy_to_user((void *)arg, (void *)&ada_dev.adc_input, sizeof(adc_input_t));
        break;

    case ADCIO_SET_GAIN:
        copy_from_user((void *)&adc_gain, (void *)arg, sizeof(adc_gain_t));
        ret = adc_hw_set_gain(&adc_gain);
        if (!ret)
        {
            ada_dev.adc_gain = adc_gain;
        }
        break;
    case ADCIO_GET_GAIN:
        copy_to_user((void *)arg, (void *)&ada_dev.adc_gain, sizeof(adc_gain_t));
        break;

    case ADCIO_SET_MONITOR:
        return adc_hw_ioctl(ADC_HW_MONITOR_EN, arg);
    default:
        ret = adc_hw_ioctl(cmd,arg);
        //return -1;
    }

    return ret;
};

/******************************************************************************/
/*!                    
* \par  Description:
*     this function is provided for dsp driver, and don't call it in application code.
* \param[in]   is_mute -- is_mute != 0, dac set to mute, is_mute == 0, dac set to unmute. 
* \retval      0 sucess 
* \retval      -1 failed
*******************************************************************************/
int ada_set_dsp_mute(int is_mute)
{
    return dac_set_volume(ada_dev.volume, ada_dev.dac_mute, (unsigned int)is_mute);
};


int ada_get_volum_state(unsigned int *max_sys_vol, unsigned int * cur_vol)
{
    if( (max_sys_vol == NULL) ||(cur_vol ==NULL))
    {
        return -1;
    }
    *max_sys_vol = ada_dev.max_sys_volume;
    *cur_vol = ada_dev.volume;
    return 0;
}
