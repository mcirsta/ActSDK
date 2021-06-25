/*
********************************************************************************
*                   213x --- sensor's driver
*                (c) Copyright 2007 - 2010, Actions Co,Ld. 
*                        All Right Reserved 
*
* History:        
*      <author>      <time>       <version >    <desc>
*       houjingkun      2008/06/18     1.0         build this file 
*       new added sensor_hw.h and sensor_hw.c just for seperating the two 
*       hardware's api of 213x ic and sensor module.
********************************************************************************/
/*!
 * \file   sensor_hw.c
 * \brief  
 *      Sensor driver hardware layer's API.
 * \author houjingkun
 * \par GENERAL DESCRIPTION:
 *      Sensor driver's hardware API, provide some function that 213x ic
 *      can provide, just called by dirver itself.
 * \par EXTERNALIZED FUNCTIONS:
 *       null
 *
 *     Copyright(c) 2008-2012 Actions Semiconductor, All Rights Reserved.
 *
 *  \version 1.0
 *  \date  2008/06/18
 *******************************************************************************/

#include "vi_config.h"
//#include "reg.h"
#include "sensor_inner.h"
#include "vi_sys.h"
#include "sensor_hw.h"
#include "vi_common.h"
#include "sensor_module.h"

//#define HJK_DEBUG SS_PRINTF("%s : %d \n", __FILE__, __LINE__);

unsigned int M_BT_MODESEL;
unsigned int M_VDCSTIFIFODATA;
/* cmos sensor related register */
unsigned int M_CSICTL;
unsigned int M_CSIHSPOS;
unsigned int M_CSIHEPOS;
//unsigned int VDIVSEPOF;
//unsigned int VDIVSEPEF;
unsigned int M_CSIVSPOS;
unsigned int M_CSIVEPOS;
unsigned int M_CSIRQSTA;
//unsigned int CSITUXYDATA;
unsigned int M_CSIFIFOCTL;

static unsigned int sdram_ctl;  /* reserve sdram control's state. */

/* reserve gpio multi function state. */
static unsigned int gpio_multipin1;
static unsigned int gpio_multipin1_i2c;

int get_sensor_i2c(void)
{
    unsigned long flags;
    unsigned long value1,value2;
#if 0    
    local_irq_save(flags);
    get_multip_reg(&value1,&value2,2);
	gpio_multipin1_i2c = value2 & I2C_PIN;
    value2 =  SET_SENSOR_I2C(value2);
    set_multip_reg(value1, value2, 2);
    local_irq_restore(flags);
#else
    OS_INT_SAVE_AREA
    OS_ENTER_CRITICAL();
    
    value2 = act_readl(GPIO_MFCTL1);
    gpio_multipin1_i2c = value2 & I2C_PIN;
    value2 =  SET_SENSOR_I2C(value2);
    act_writel(value2, GPIO_MFCTL1);

    OS_EXIT_CRITICAL();
#endif
    return 0;
}

int release_sensor_i2c(void)
{
    unsigned long flags;
    unsigned long value1,value2;
#if 0    
    local_irq_save(flags);
    get_multip_reg(&value1,&value2,2);
	value2 = (value2 & (~I2C_PIN))|gpio_multipin1_i2c;
    set_multip_reg(value1, value2, 2);
    local_irq_restore(flags);	
#else
    OS_INT_SAVE_AREA
    OS_ENTER_CRITICAL();
    
    value2 = act_readl(GPIO_MFCTL1);
    value2 = (value2 & (~I2C_PIN))|gpio_multipin1_i2c;
    act_writel(value2, GPIO_MFCTL1);

    OS_EXIT_CRITICAL();
#endif    
    return 0;
}
/******************************************************************************/
/*!                    
* \par  Description:
*       get sensor device's pin resource.
* \param[in]   wait_type -- 0 - can sleep; 1 - can't sleep while get pin resource; 
* \return      
* \retval      0 sucess, else failed.
*******************************************************************************/
int get_sensor_dev(unsigned int wait_type)
{
    unsigned long flags;
    unsigned long value1,value2;
    local_irq_save(flags);
    get_multip_reg(&value1,&value2,2);
    gpio_multipin1 = value2 & Multi_CON1_VAL;
    set_multip_reg(value1, SET_SENSOR_GPIO(value2), 2);
    local_irq_restore(flags);

    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       release sensor device's pin resource.
* \param[in]   NULL
* \return      
* \retval      0 sucess, else failed.
*******************************************************************************/
int release_sensor_dev(void)
{
    unsigned long flags;
    unsigned long value1,value2;
    local_irq_save(flags);
    get_multip_reg(&value1,&value2,2);
    value2 = (value2 & (~Multi_CON1_VAL)) | gpio_multipin1;	
    set_multip_reg(value1, value2, 2);
    local_irq_restore(flags);	
    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       init sensor work type.(DC, DV, PC Camera or others.
* \param[in]   type -- refor to ss_work_mode_t.
* \return      
* \retval      0 sucess, else failed.
*******************************************************************************/
int sensor_init_work_mode(unsigned int type)
{
    if (type == VI_PC_CAMERA_MODE)
    {
        vi_dev.pix_fmt = VI_PIX_FMT_YUV422;
    }
    else
    {
        vi_dev.pix_fmt = VI_PIX_FMT_4Y4U4Y4V; 
    }
    sensor_set_pix_fmt(vi_dev.pix_fmt); 
    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       cyclely query dma untill it's not busy, if success, return 0. 
* \param[in]   dma -- dma number, 0 - 7.
* \param[in]   val -- timeout value, unit is deault ms.
* \return      
* \retval      0 sucess, else failed.
*******************************************************************************/
int cycle_query_dma(unsigned int dma, unsigned int val)
{
    unsigned long count;
    int ret = 1;

    count = 0;

    while (ret != 0) 
    {
        if ( !(count % 100) ) 
        {
            if (count > 500000 * val) 
            {
                SS_PRINTF("sensor's dma always busy! something is error!\n");
                //print_dma_info(dma);
                return -1;
            }
            ret = dma_started(dma);
        }
        count++;
    }   
    
    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       init hardware resource.
* \param[in]   pDev -- pointer of structure vi_dev_t, it's the key data structure.
* \return      
* \retval      0 sucess, else failed.
*******************************************************************************/
int sensor_hw_init(vi_dev_t *pDev)
{
    unsigned int value;
    int ret = 0;

#if _IC_TYPE_
    ic_type = _IC_TYPE_;
#else
    ic_type = get_ic_type();
#endif
    //printf("ic_type: %x\n", ic_type);

    if (ic_type == 0x63) 
    {
        M_BT_MODESEL = IC63_BT_MODESEL;
        M_VDCSTIFIFODATA = IC63_VDCSTIFIFODATA;
        /* cmos sensor related register */
        M_CSICTL = IC63_CSICTL;
        M_CSIHSPOS = IC63_CSIHSPOS;
        M_CSIHEPOS = IC63_CSIHEPOS;
        //VDIVSEPOF = IC63_VDIVSEPOF;
        //VDIVSEPEF = IC63_VDIVSEPEF;
        M_CSIVSPOS = IC63_CSIVSPOS;
        M_CSIVEPOS = IC63_CSIVEPOS;
        M_CSIRQSTA = IC63_CSIRQSTA;
        //CSITUXYDATA = IC63_CSITUXYDATA;
        M_CSIFIFOCTL = IC63_CSIFIFOCTL;
    } 
    else if (ic_type == 0x96) 
    {
        M_BT_MODESEL = IC96_BT_MODESEL;
        M_VDCSTIFIFODATA = IC96_VDCSTIFIFODATA;
        /* cmos sensor related register */
        M_CSICTL = IC96_CSICTL;
        M_CSIHSPOS = IC96_CSIHSEPOS;
        M_CSIHEPOS = IC96_CSIHSEPOS;
        //VDIVSEPOF = IC96_VDIVSEPOF;
        //VDIVSEPEF = IC96_VDIVSEPEF;
        M_CSIVSPOS = IC96_CSIVSEPOS;
        M_CSIVEPOS = IC96_CSIVSEPOS;
        M_CSIRQSTA = IC96_CSIRQSTA;
        //CSITUXYDATA = IC96_CSITUXYDATA;
        M_CSIFIFOCTL = IC96_CSIFIFOCTL;
    }
    else
    {
        return -1;
    }

    /*write reg PMU_CTL  moved to sensor_module.c,  for  it is changeable,  cccw,20081029*/
    sensor_power_on();
 
    /* set related dev clk. */
    value = act_readl(CMU_DEVCLKEN);
    value |= 0x1 << 12; /* BT clk enable. */
    value |= 0x1 << 23; /* I2C clk enable. */
    act_writel(value, CMU_DEVCLKEN);

    value = act_readl(CMU_DMACLK) | 0x1; /* enable dma4 clk. */
    act_writel(value, CMU_DMACLK);
    //act_writel(0x0000000f, CMU_DMACLK);

    /*config reg CMU_BTCLK  moved to sensor_module.c,  for  it is changeable,  cccw,20081029*/
    sensor_init_m_clk();

    /* disable sensor interface */
    if (ic_type == 0x63) 
    {
        act_writel(0x0000000c, M_BT_MODESEL);
    } 
    else if (ic_type == 0x96) 
    {
        act_writel(0x00000017, M_BT_MODESEL);
    }
    else
    {
        return -1;
    }
    mdelay(10);

    //ret = sensor_init_param_reg();

    act_writel(0x00000012, M_CSIFIFOCTL);   //fifo control (sp bus)
    //act_writel(0x00000052, M_CSIFIFOCTL); //fifo control (ahb bus)

//    sensor_init_work_mode(vi_dev.app_type);

    //act_writel(0x000009ff, M_CSIHEPOS);   //+0x0010-Hsync end position in pclk (SXGA 1280)
    
    act_writel(0x00000003, M_CSIRQSTA); //+0x0028-Cmos sensor interface IRQ Status

    sdram_ctl = act_readl(SDRAM_CTL);
    value = sdram_ctl;
    value &= ~(0xf << 12);
    value |= (0x7 << 12);   /* DMA4>DMA5>DMA6>DMA7>BUS */
    act_writel(value, SDRAM_CTL);
    //SS_PRINTF("SDRAM_CTL is %x \n", act_readl(SDRAM_CTL));
    
    return ret;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       release sensor device's hardware resource.
* \param[in]   pDev -- pointer of structure vi_dev_t, it's the key data structure.
* \return      
* \retval      0 sucess, else failed.
*******************************************************************************/
int sensor_hw_exit(vi_dev_t *pDev)
{
    unsigned int value;

    sensor_exit_param_reg();
    sensor_power_off();
    mdelay(200);

//    act_writel(0x0, I2CCTL);    //reset I2C
//    //act_writel(0x80, I2CCTL); //Master mode,disable IRQ
//    act_writel(0x0, I2CDIV);    //set I2C 100K,here APH=12M
//    act_writel(0x0, I2CSTA);    //reset I2C
//    act_writel(0x0, I2CADDR);
//    act_writel(0x0, I2CDAT);

    if (ic_type == 0x63) 
    {
        act_writel(0x0000000c, M_BT_MODESEL);
    } 
    else if (ic_type == 0x96) 
    {
        act_writel(0x00000000, M_BT_MODESEL);
    }
    else
    {
        return -1;
    }

    value = act_readl(CMU_DEVCLKEN);
    value &= ~(0x1 << 12);  /* BT clk disable. */
//    value &= ~(0x1 << 23);  /* I2C clk disable. */
    act_writel(value, CMU_DEVCLKEN);  

    act_writel(sdram_ctl, SDRAM_CTL);

    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       just added for 5002 dma's yuv mode. 
* \param[in]   dma_no -- dma's number.
* \retval      0 sucess, else failed.
*******************************************************************************/
int dma_mode_start(unsigned int dma_no)
{
    unsigned int val;
    //vi_dev.pix_fmt = VI_PIX_FMT_4Y4U4Y4V;

    /* pix_fmt -- refer to ss_pix_fmt_t. */
    if (vi_dev.pix_fmt == VI_PIX_FMT_4Y4U4Y4V)
    {
        val = act_readl(DMA_CMD4);
        val &= ~(0x3 << 8); 
        val |= 0x1;
        act_writel(val, DMA_CMD4);
    }
    else if (vi_dev.pix_fmt == VI_PIX_FMT_YUV420)
    {
        val = 0;
        val |= vi_dev.width << 20;
        act_writel(val, DMA_VIDEO4);

        val = act_readl(DMA_CMD4);
        val &= ~(0x1 << 9); 
        val |= 0x1 | (0x1 << 8); 
        act_writel(val, DMA_CMD4);
    }
    else if (vi_dev.pix_fmt == VI_PIX_FMT_YUV422)      /* to be finish. */
    {
        val = act_readl(DMA_CMD4);
        val &= ~(0x3 << 8); 
        val |= 0x1;
        act_writel(val, DMA_CMD4);
    }
    else
    {
#if 0   /* stride mode. */
        val = 0;
        val |= vi_dev.width << 20;
        //val |= vi_dev.width << 10;
        //val |= vi_dev.width << 0;
        act_writel(val, DMA_VIDEO4);

        val = act_readl(DMA_CMD4);
        val |= 0x1 << 9; 
        val |= 0x1 | (0x1 << 8); 
        act_writel(val, DMA_CMD4);
#endif
        val = act_readl(DMA_CMD4);
        val &= ~(0x3 << 8); 
        val |= 0x1;
        act_writel(val, DMA_CMD4);
    }

    return 0;
}

