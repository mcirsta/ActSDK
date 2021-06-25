/*
********************************************************************************
*                   213x --- usdk130 source file
*                (c) Copyright 2003 - 2008, Actions Co,Ld. 
*                        All Right Reserved 
*
* History:        
*      <author>      <time>       <version >   <desc>
*       houjingkun      2008/08/05     1.0     build this file 
*                                              new 1017 ada driver hardware layer.
********************************************************************************/ 
/*!
 * \file   ada_hw.c
 * \brief  
 *      new 1017 ada driver hardware layer.
 * \author houjingkun
 * \par GENERAL DESCRIPTION:
 * \par EXTERNALIZED FUNCTIONS:
 *       null
 *
 *  Copyright(c) 2003-2008 Actions Semiconductor, All Rights Reserved.
 *
 * \version 1.0
 * \date  2008/08/05
 *******************************************************************************/

#include "ada_hw.h"
#include "ada.h"
#include "ada_sys.h"

static unsigned int ic_type;

static unsigned int pre_volume=0;
static unsigned int pre_input_src =0;
static unsigned int need_restart_monitor =0;

#define _ADA_HW_NO_OS_      0

#if _ADA_HW_NO_OS_
static inline void act_writel(unsigned int val, unsigned int reg)
{
    *(volatile unsigned int *)(reg) = val;
}

static inline unsigned int act_readl(unsigned int port)
{
    return (*(volatile unsigned int *)port);
}

static void udelay(unsigned int time)
{
    volatile unsigned int i, j;

    for (i = 0; i < time; i++) 
    {
        for (j = 0; j < 2000; j++) 
        {
            do {} while (0);
        }
    }
    return;
}
#endif  /* _ADA_HW_NO_OS_ */


/******************************************************************************/
/*!                    
* \par  Description:
*       set system's volume. 
* \param[in]    volume -- range: 0 ~ 31.
* \retval       0 sucess, else failed.
*******************************************************************************/
int dac_hw_set_volume(unsigned int volume)
{
    unsigned int val;

    if (volume > 0x1f) 
    {
        //printf("the volume of amplifier is out of range! \n ");
        return -1;
    }
    val = act_readl(DAC_ANALOG);
    val &= ~(0x1f << 3);
    val |= (volume << 3);
    act_writel(val, DAC_ANALOG);

    if(volume ==0)
    {
        val= act_readl(DAC_ANALOG);
        pre_input_src = (val>>8)&0x7;
        val &= ~(0x7<<8);
        act_writel(val,DAC_ANALOG);    	
    }
    if ((pre_volume==0) && (volume ==1) )
    {

        if(need_restart_monitor == 1)
        {
            val |= (0x7<<8);
            need_restart_monitor =0;
        }
        else
        {
            val = act_readl(DAC_ANALOG);
            val|= (pre_input_src<<8);    	     
        }
        act_writel(val,DAC_ANALOG);    	
    }
    pre_volume = volume;
    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       get system's volume. 
* \param[in]    null
* \retval       0 ~ 31. volume value.
*******************************************************************************/
unsigned int dac_hw_get_volume(void)
{
    unsigned int val;

    val = act_readl(DAC_ANALOG);
    val &= (0x1f << 3);
    val = val >> 3;

    return val;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       dac and adc hardware initialization for driver install.
* \param[in]    null
* \retval       0 sucess, else failed.
*******************************************************************************/
int ada_hw_init(void)
{
    unsigned int val;

    val = (0x1 << 17) | act_readl(CMU_DEVCLKEN);    /* dac clk enable. */
    act_writel(val, CMU_DEVCLKEN);

    /* enable adc and dac clock */
    val = (0x3 << 17) | act_readl(CMU_DEVCLKEN);
    act_writel(val, CMU_DEVCLKEN);

    /* enable audio pll */
    val = (0x1 << 4) | act_readl(CMU_AUDIOPLL);
    act_writel(val, CMU_AUDIOPLL);

    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       dac and adc hardware dispose for driver uninstall.
* \param[in]    null
* \retval       0 sucess, else failed.
*******************************************************************************/
int ada_hw_exit(void)
{
    unsigned int val;

    /* disable dac clk */
    val = (~(0x1 << 17) ) & act_readl(CMU_DEVCLKEN);
    act_writel(val, CMU_DEVCLKEN);

    /* disable adc clk */  //2007-3-28 10:49 
    /* dac clk can not disabled because we need to change volume from fm recording to fm playing */ 
    val = (~(0x1 << 18)) & act_readl(CMU_DEVCLKEN);
    act_writel(val, CMU_DEVCLKEN);
    
    /* disable audio pll */
    val = (~(0x1 << 4)) & act_readl(CMU_AUDIOPLL);
    act_writel(val, CMU_AUDIOPLL);

    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       dac and adc hardware enable when /dev/ada device is opened.
*       some operation is for avoiding the "PA" noise when analog circuit is enable.
* \param[in]    null
* \retval       0 sucess, else failed.
*******************************************************************************/
int ada_hw_open(void)
{
    unsigned int val;
    unsigned int volume;

    volume = dac_hw_get_volume();

    /* 
     * set volume to 0 and mute dac playback, and then enable dac, so that the 
     * noise of "pa" will be lightened.
     */
    dac_hw_set_volume(0);
    udelay(10);
    dac_hw_ioctl(DAC_HW_MUTE_EN, 1);
    udelay(10);

    val = act_readl(DAC_CTL);
    val |= 0x1;      /* enable dac. */
    act_writel(val, DAC_CTL);
    val = act_readl(DAC_ANALOG);
    val |= 0x3;      /* analog circuit enable. */
    act_writel(val, DAC_ANALOG);

    mdelay(3);

    dac_hw_set_volume(volume);

    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       dac and adc hardware disable when /dev/ada device is closed.
*       some operation is for battery costdown.
* \param[in]    null
* \retval       0 sucess, else failed.
*******************************************************************************/
int ada_hw_close(void)
{
    unsigned int val;
    unsigned int volume;

    volume = dac_hw_get_volume();
    /* 
     * set volume to 0 and mute dac playback, and then disable dac, so that the 
     * noise of "pa" will be lightened.
     */
    dac_hw_set_volume(0);
    udelay(10);
    dac_hw_ioctl(DAC_HW_MUTE_EN, 1);
    udelay(10);

    val = act_readl(DAC_CTL);
    val &= 0xfffffffe;      /* disable dac. */
    act_writel(val, DAC_CTL);
    val = act_readl(DAC_ANALOG);
    val &= 0xfffffffe;      /* analog circuit disable. */
    act_writel(val, DAC_ANALOG);

    /* disable vmic. */
    val = act_readl(ADC_CTL);
    val &= ~(0x1 << 28);
    act_writel(val, ADC_CTL);

    dac_hw_set_volume(volume);
    
    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       dac initialization for install.
* \param[in]    null
* \retval       0 sucess, else failed.
*******************************************************************************/
int dac_hw_init(void)
{
    unsigned int val = 0;
    unsigned int volume;

    volume = dac_hw_get_volume();

    //act_writel((0x1 << 20), CMU_DEVRST) ;
    //act_writel((0x3 << 21), CMU_DEVRST) ;
    //udelay(1);

    //val = act_readl(DAC_CTL);
    //val = val & 0xfffffffe;      /* disable dac. */
    //val = val | 0x1;      /* enable dac. */
    //val = val | (0x1 << 10);  /* dac mono enable. */
    act_writel(0x90b1,DAC_CTL);
    
    ic_type = get_ic_type();
    /* 
     * 3963的最佳Bias设置为0x200088ff（取其中的BIT[29:13]），
     * 3996和5002的最佳Bias设置为0x200008ff（取其中的BIT[29:13]）。
     */
    if (ic_type == 0x63)
    {
        val &= ~(0x1ffff << 13);
        val |= 0x200088ff & (0x1ffff << 13);
    }
    else if (ic_type == 0x96)
    {
        val &= ~(0x1ffff << 13);
        val |= 0x200008ff & (0x1ffff << 13);
    }
    else
    {
        val &= ~(0x1ffff << 13);
        val |= 0x200008ff & (0x1ffff << 13);
    }
    act_writel(val, DAC_ANALOG);

    val = act_readl(DAC_FIFOCTL);
    val = val | 0x1;        /* enable dac fifo. */
    val = val | (0x1 << 8);     /* dac fifo drq enable. */
    //val = val & (~(0x1 << 4));  /* dsp port disable. */
    val = val | (0x1 << 4);     /* dsp port enable. */
    val = val & (~(0x3 << 5));  /* dac file 14/16 empty. */
    val = val | (0x2 << 5);     /* dac file 14/16 empty. */
    act_writel(val, DAC_FIFOCTL);

    val = act_readl(DAC_ANALOG);    /* init,volume is 0*/  
    val = val & 0xfffffff8;
    val = val | 0x6;      /* analog circuit pa enable. */
    val = val | (0x1 << 11);  /* enable dac playback. */
    val &= ~(0x1f << 3);    /* set volume to 0. */
    act_writel(val, DAC_ANALOG);
    mdelay(2);

    //act_writel(0x00000011, CMU_AUDIOPLL);       //DACFS==48k

#if SPEAKER_GPIO_OPT
    act_writel(act_readl(GPIO_BINEN) | (1<<31), GPIO_BINEN);    /* GPIOB31 input disable; */
    act_writel(act_readl(GPIO_BOUTEN) | (1<<31), GPIO_BOUTEN);  /* GPIOB31 out enable; */
    act_writel(act_readl(GPIO_BDAT) & ~(1<<31), GPIO_BDAT);     /* GPIOB31 set low level; */
#endif

    dac_hw_set_volume(volume);
    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       dac dispose for uninstall.
* \param[in]    null
* \retval       0 sucess, else failed.
*******************************************************************************/
int dac_hw_exit(void)
{
    unsigned int val;
    unsigned int volume;

    volume = dac_hw_get_volume();
    dac_hw_set_volume(0);

    val = act_readl(DAC_ANALOG);    /* init,volume is 0 */  
    val &= 0xfffffffc;      /* analog circuit disable. */
    val &= 0xfffff0ff;
    act_writel(val, DAC_ANALOG);    /* setMute(0); */

    /* disble digital circuit. */
    val = act_readl(DAC_CTL);
    val &= 0xfffffffe;
    act_writel(val, DAC_CTL);

    dac_hw_set_volume(volume);
    return 0;
}

/* pcm_rate set to ACT_SAMPLE_96K ~ ACT_SAMPLE_8K. */
static inline int dac_hw_set_pcm_rate(unsigned int pcm_rate)
{
    unsigned int val;

    if (pcm_rate > 0xf) 
    {
        //printf("pcm_rate is out of range! \n");
        return -1;
    }
    val = pcm_rate;
    val |= (act_readl(CMU_AUDIOPLL) & (~0xf));

    /* enable audiopll */
    val |= 0x1 << 4;

    act_writel(val,  CMU_AUDIOPLL);
    return 0;
}

/* channel == 0, stereo, else, mono. */
static inline int dac_hw_set_channel(unsigned int channel)
{
    unsigned int val;

    /* set dac channel. */
    val = act_readl(DAC_CTL);
    if (channel == ADA_MONO)
    {
        val |= (0x1 << 10);
    }
    else if (channel == ADA_STEREO)
    {
        val &= ~(0x1 << 10);
    }
    else
    {
        return -1;
    }
    act_writel(val, DAC_CTL);

    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       check whether register's setting is ok for pcm playing!
* \param[in]    null
* \retval       0 sucess, else failed.
*******************************************************************************/
int dac_hw_check_play_set(void)
{
    unsigned int val;

    val = act_readl(DAC_CTL);
    if ((val & 0x1) == 0)   /* check enable dac. */
    {
        return -1;
    }

    val = act_readl(DAC_FIFOCTL);
    if ((val & 0x1) == 0)   /* check enable dac fifo. */
    {
        return -2;
    }
    if ((val & (0x1 << 8)) == 0)   /* check enable dac fifo drq. */
    {
        return -3;
    }
    if ((val & (0x1 << 4)) != 0)   /* check disable dsp port. */
    {
        return -4;
    }

    val = act_readl(DAC_ANALOG);
    if ((val & 0x3) != 0x3)        /* analog circuit enable. */
    {
        return -5;
    }

    return 0;
}

/*----------------------------------------------------------------------------*/

/******************************************************************************/
/*!                    
* \par  Description:
*       realize ada driver's DAC hardware command function.
* \param[in]    cmd -- refer to ada_hw.h and ada.h, see related command.
* \param[in]    arg -- ioctl's argument.
* \retval       0 sucess, else failed.
*******************************************************************************/
int dac_hw_ioctl(unsigned int cmd, unsigned int param)
{
    int ret = 0;
    unsigned int val;
    //unsigned int *p_val = (unsigned int *)param;

    switch (cmd)
    {
    case 0:
        /* debug here */
        break;
    case DAC_HW_SPEAKER_EN:     /* enable dac speaker or not. */
#if SPEAKER_GPIO_OPT
        if (param)
        {
            val = act_readl(GPIO_BOUTEN); /* GPIOB31 out diable，GPIOB31 set to input, detect headphone. */
            val = val & ~(0x1 << 31);
            act_writel(val, GPIO_BOUTEN);
        }
#endif
        break;

    case DAC_HW_MUTE_EN:
        val = act_readl(DAC_ANALOG);
        if (param != 0)
        {
            val &= ~(0x1 << 11);        /* mute dac playback. */
        }
        else
        {
            val |= (0x1 << 11);        /* not mute dac playback. */
        }
        act_writel(val, DAC_ANALOG);
        break;

    case DAC_HW_DSP_EN:
        val = act_readl(DAC_FIFOCTL);
        if (param != 0)
        {
            val |= (0x1 << 4);          /* enalbe dsp port. */
        }
        else
        {
            val &= ~(0x1 << 4);         /* not enalbe dsp port. */
            //val |= (0x1 << 8);          /* check enable dac fifo drq. */
        }
        act_writel(val, DAC_FIFOCTL);
        break;

    case DACIO_SET_PCM_RATE:
        ret = dac_hw_set_pcm_rate(param);
        break;
    case DACIO_SET_PCM_CHANNEL:
        ret = dac_hw_set_channel(param);
        break;
    case DACIO_SET_PCM_BITS:
        if (param != 32)
        {
            ret = -1;
        }
        break;
    case DACIO_GET_FIFO:
        val = act_readl(DAC_FIFOCTL);
        ret = copy_to_user((void *)param, (void *)&val, sizeof(unsigned int));        
        break;
    case DACIO_SET_FIFO:
        act_writel(param, DAC_FIFOCTL);
        break;
        
    default:
        ret = -1;
    }

    return ret;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       adc initialization for driver install.
* \param[in]    null
* \retval       0 sucess, else failed.
*******************************************************************************/
int adc_hw_init(void)
{
    //unsigned int val;

    /* set adc's registers' init val */
    //act_writel(0x009, ADC_FIFO_CON);    /* enable dsp's adc fifo data accessing, no irq,if==0x009 has irq */

    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       adc dispose for uninstall.
* \param[in]    null
* \retval       0 sucess, else failed.
*******************************************************************************/
int adc_hw_exit(void)
{
    unsigned int val;

    val = act_readl(ADC_CTL);
    val &= ~(0x3 << 4);
    act_writel(val, ADC_CTL);
    
    /* disable mic and linein channel,fm channel will be disabled in fm driver */
    val = act_readl(ADC_CTL);
    val &= ~(0x3 << 6);
    val &= ~(0x3 << 16);
    act_writel(val, ADC_CTL);
    
    return 0;       /* succeed */
}

/* pcm_rate set to ACT_SAMPLE_96K ~ ACT_SAMPLE_8K. */
static inline int adc_hw_set_pcm_rate(unsigned int pcm_rate)
{
    unsigned int val;

    if (pcm_rate > 0xf) 
    {
        //printf("pcm_rate is out of range! \n");
        return -1;
    }
    val = act_readl(CMU_AUDIOPLL);

    /* enable audiopll */
    val |= 0x1 << 4;

    /* set adc sample_rate */
    val &= 0xfffff0ff;
    val |= (pcm_rate << 8);
    act_writel(val,  CMU_AUDIOPLL);

    return 0;
}

/* channel == 0, stereo, else, mono. */
static inline int adc_hw_set_channel(unsigned int channel)
{
    unsigned int val;

    /* mono is not support currentlly. */
    if (channel != 0)
    {
        return -1;
    }

    /* set adc channel. */
    val = act_readl(ADC_CTL);
    val &= ~(0x3 << 4);
    /* actually, left or right is decided by dsp encoder. */
    val |= (0x3 << 4);      /* enable both left and right, bit 4--ADC left, bit 5--ADC right. */
    act_writel(val, ADC_CTL);

    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       set adc source input for adc voice recording.
* \param[in]    refer to struct adc_input_t.
* \retval       0 sucess, else failed.
*******************************************************************************/
int adc_hw_set_input(adc_input_t *p_input)
{
    unsigned int adc_ctl;
    unsigned int val;

    /* get register value */
    adc_ctl = act_readl(ADC_CTL);
    //dac_analog = act_readl(DAC_ANALOG);

    if (p_input->mix != 0) 
    {
        adc_ctl |= (0x3 << 21);     /* mix input */
    } 
    else 
    {
        adc_ctl &= ~(0x3 << 21);

        val = (unsigned int)((p_input->mic << 0) | (p_input->fm << 1) | (p_input->linein << 2));
        switch (val)
        {
        case 0x1:       /* set mic input */
            adc_ctl |= (0x0 << 21);
            break;
        case 0x2:       /* set fm input. */
            adc_ctl |= (0x1 << 21);
            break;
        case 0x4:       /* set linein input. */
            adc_ctl |= (0x2 << 21);
            break;
        default:
            if (val != 0)   /* unreasonable for mix == 0 and val != 0. */
            {
                return -1;
            }
            else    /* if val == 0, set to no input. */
            {
            }
            break;
        }
    }

    if (p_input->mic != 0)
    {
        adc_ctl |= (0x3 << 6);
        adc_ctl |= (0x1 << 28);     /* enable vmic. */
    }
    else
    {
        adc_ctl &= ~(0x3 << 6);
        adc_ctl &= ~(0x1 << 28);     /* disable vmic. */
    }

    if (p_input->fm != 0)
    {
        adc_ctl |= (0x3 << 11);
    }
    else
    {
        adc_ctl &= ~(0x3 << 11);
    }

    if (p_input->linein != 0)
    {
        adc_ctl |= (0x3 << 16);
    }
    else
    {
        adc_ctl &= ~(0x3 << 16);
    }

    act_writel(adc_ctl,  ADC_CTL);  
    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       set adc source input gain for adc voice recording.
* \param[in]    refer to struct adc_gain_t.
* \retval       0 sucess, else failed.
*******************************************************************************/
int adc_hw_set_gain(adc_gain_t *p_gain)
{
    unsigned int adc_ctl;

    adc_ctl = act_readl(ADC_CTL);       /* get register value */

    adc_ctl &= ~(0x7 << 8);
    adc_ctl |= (unsigned int)(p_gain->mic_gain & 0x7) << 8; 

    adc_ctl &= ~(0x7 << 13);
    adc_ctl |= (unsigned int)(p_gain->fm_gain & 0x7) << 13; 

    adc_ctl &= ~(0x7 << 18);
    adc_ctl |= (unsigned int)(p_gain->linein_gain & 0x7) << 18; 

    /* set adc gain */
    adc_ctl &= ~(0x3 << 23);
    adc_ctl |= (unsigned int)(p_gain->adc_gain & 0x3) << 23;    

    act_writel(adc_ctl,  ADC_CTL);  

    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       realize ada driver's ADC hardware command function.
* \param[in]    cmd -- refer to ada_hw.h and ada.h, see related command.
* \param[in]    arg -- ioctl's argument.
* \retval       0 sucess, else failed.
*******************************************************************************/
int adc_hw_ioctl(unsigned int cmd, unsigned int param)
{
    unsigned int val;
    unsigned int tmp;
    int ret = 0;

    switch (cmd)
    {
    case 0:
        /* debug here */
        break;
    case ADC_HW_MONITOR_EN:
        
        val = act_readl(DAC_ANALOG); 
               
        if (param == 0) 
        {
            val &= ~(0x7<<8);
            pre_input_src =0;                      
        } 
        else 
        {
            if(0 ==dac_hw_get_volume())
            {
                need_restart_monitor =1; 
                break; 
            }            
            val |= (0x7<<8);             
        }
        
        need_restart_monitor =0;
        act_writel(val, DAC_ANALOG);   
        break;

    case ADCIO_SET_PCM_RATE:
        ret = adc_hw_set_pcm_rate(param);
        break;
    case ADCIO_SET_PCM_CHANNEL:
        ret = adc_hw_set_channel(param);
        break;
    case ADCIO_SET_PCM_BITS:
        if (param != 32)
        {
            ret = -1;
        }
        break;
    
    case ADCIO_GET_FIFO:
        val = act_readl(ADC_FIFOCTL);
        ret = copy_to_user((void *)param, (void *)&val, sizeof(int));        
        break;
    case ADCIO_SET_FIFO:
        act_writel(param, ADC_FIFOCTL);
        break;
                
    default:
        ret = -1;
    }

    return ret;
}


