/*
********************************************************************************
*                   213x --- tvin's driver
*                (c) Copyright 2007 - 2010, Actions Co,Ld. 
*                        All Right Reserved 
*
* History:        
*      <author>      <time>       <version >    <desc>
*       houjingkun      2008/06/18     1.0         build this file 
*       new added tvin_hw.h and tvin_hw.c for video in device driver.
********************************************************************************/ 
/*!
 * \file   tvin_hw.c
 * \brief  
 *      Tvin driver hardware layer's API.
 * \author houjingkun
 * \par GENERAL DESCRIPTION:
 *      realize tvin driver hardware layer's API.
 *      just called by dirver itself.
 * \par EXTERNALIZED FUNCTIONS:
 *       null
 *
 *     Copyright(c) 2008-2012 Actions Semiconductor, All Rights Reserved.
 *
 * \version 1.0
 * \date  2008/06/18
 *******************************************************************************/

#include "reg.h"
#include "i2c.h"
#include "video_in.h"
#include "tvin_hw.h"
#include "vi_config.h"
#include "errno.h"

#define _TVIN_BT601_

#if ADD_TVIN_DRV

typedef struct 
{
    unsigned int old_clk_state; /* reserved clk state before tvin device open or install. */
    void *pTvin_i2c;
    unsigned int* pTvin_init_data;
    int tvin_init_cnt;
    unsigned int sdr_ctl_reg;
} tvin_dev_t;

static tvin_dev_t tvin_dev;

static unsigned int tvin_ic_init_data[] = 
{
#ifdef TI_5150
    0x00000002,
    0x00000000,

#ifdef  _TVIN_BT601_
    0x00000003,
    //0x0000000f,
    0x0000002f,
    
    0x00000007,
    0x00000040,

    0x0000000d,
    0x00000007,
    
    0x00000016,
    //0x00000077,
    0x0000007d,
    
    0x0000000f,
    0x0000000a,
#else
    0x00000003,
    0x0000000d,               //Miscellaneous Control 0D,enable HS/VS,           
#endif

#endif  /* TI_5150 */

#ifdef ADI_7180
    0x00000000,
    0x00000004,               //Input Control:04H-CVBS;06H-S-Video
    
    0x00000001,
    0x000000C8,               //hsync,vsync enable
    
    0x00000003,
    0x0000000C,               //Output Format:8-Bit@LLC 4:2:2 CCIR656

    0x00000004,
    0x000000D6,               //Compliant with BT.656-4,SFL Enable
    
    0x00000008,
    0x00000080,               //Contrast Control
    
    0x0000000A,
    0x00000000,               //Brightness Control
    
    0x0000000B,
    0x00000000,              //Hue Control
    
    0x0000000C,
    0x00000084,              //Default Y Value
    
    0x0000000D,
    0x000000D6,              //Default C Value
    
    0x00000015,
    0x00000000,              //Digital Clamp
                
    0x00000017,
    0x00000041,              //Shaping Filter Control 1
                
    0x00000018,
    0x00000014,               //Shaping Filter Control 2
                
    0x0000001D,
    0x00000000,               //Shaping Filter Control 2---27MHz Clock
                                
    0x0000002B,
    0x000000E0,               //Misc Control
                                
    0x00000031,
    0x00000002,               //VS and Field Control 1

    0x0000003D,
    0x000000A2,               //Manual Window Control
    
    0x0000003E,
    0x0000006A,               //BLM
    
    0x0000003F,
    0x000000A0,               //BGB
#endif  /* ADI_7180 */
};


static unsigned int tvin_ic_exit_data[] = 
{
#ifdef TI_5150
    0x00000002,
    0x00000001,

    0x00000003,
    0x00000000,
#endif  /* TI_5150 */
};

#ifdef _TVIN_HW_NO_OS_FUNC_

/* if system is 120M HZ, and 120 MIPS, 10 instruction one cycle, then udelay's count is 120 / 10 * 1000 = 12000. */
static void udelay(unsigned int time)
{
    volatile unsigned int i, j;

    for (i = 0; i < time; i++) 
    {
        for (j = 0; j < 8000; j++) 
        {
            do {} while (0);
        }
    }
    return;
}
#endif  /* _TVIN_HW_NO_OS_FUNC_ */

#define TVIN_HW_DEBUG   0
#if TVIN_HW_DEBUG   /* just for debug. */
static void print_clk_reg(int i)
{
    printf("i: %d \n", i);
    printf("CMU_DEVCLKEN: %x \n", act_readl(CMU_DEVCLKEN));
    printf("CMU_COREPLL: %x \n", act_readl(CMU_COREPLL));
    printf("CMU_BUSCLK: %x \n", act_readl(CMU_BUSCLK));
    printf("I2C1_CLKDIV: %x \n", act_readl(I2C1_CLKDIV));
    //printf("CMU_BUSCLK: %x \n", act_readl(CMU_BUSCLK));

    printf("BT_FIFO_IDAT: %x \n", act_readl(BT_FIFO_IDAT));
    printf("BTI_MODESEL: %x \n", act_readl(BTI_MODESEL));
    printf("BT_VDICTL: %x \n", act_readl(BT_VDICTL));
    printf("BT_VDIFIFOCTL: %x \n", act_readl(BT_VDIFIFOCTL));

    printf("GPIO_MFCTL0: %x \n", act_readl(GPIO_MFCTL0));
    printf("GPIO_MFCTL1: %x \n", act_readl(GPIO_MFCTL1));
    return;
}
#endif  /* TVIN_HW_DEBUG */


static int tvin_hw_get_signal_ok(void)
{
    unsigned int val = 0;
    unsigned int cnt_88 = 0;
    unsigned int i = 0;
    unsigned int ret = 0;
    
#ifdef TI_5150    
    if (tvin_dev.pTvin_i2c != (void *)0)
    {                               
        while(i<25)
        {
            val = 0;            
            val = tvin_i2c_read_reg(tvin_dev.pTvin_i2c, 0x88);          
            
            if ((val & 0x6) != 0)
            {
                cnt_88++;  
            }             
            i++;                                  
            mdelay(1);
        }        
        printf(" cnt_88=%d\n",cnt_88);
        if (cnt_88 > 0)
        {
            return 0;
        }
    }
#endif  /* TI_5150 */
    
    return -1;
}

static void tvin_poweron(void)
{
#ifdef  TVIN_GPIO_B_STANDBY
    act_writel(act_readl(GPIO_BOUTEN) | (0x1<< TVIN_STANDBY_GPIO_NUM), GPIO_BOUTEN);
    act_writel(act_readl(GPIO_BINEN) & (~(0x1<<TVIN_STANDBY_GPIO_NUM)), GPIO_BINEN);
    act_writel(act_readl(GPIO_BDAT) | (0x1<<TVIN_STANDBY_GPIO_NUM),GPIO_BDAT);
#endif
#ifdef  TVIN_GPIO_A_STANDBY
    act_writel(act_readl(GPIO_AOUTEN) | (0x1<<TVIN_STANDBY_GPIO_NUM), GPIO_AOUTEN);
    act_writel(act_readl(GPIO_AINEN) & (~(0x1<<TVIN_STANDBY_GPIO_NUM)), GPIO_AINEN);
    act_writel(act_readl(GPIO_ADAT) | (0x1<<TVIN_STANDBY_GPIO_NUM),GPIO_ADAT);
#endif
    return;
}

static void tvin_poweroff(void)
{
#ifdef  TVIN_GPIO_B_STANDBY
    act_writel(act_readl(GPIO_BOUTEN) | (0x1<<TVIN_STANDBY_GPIO_NUM), GPIO_BOUTEN);
    act_writel(act_readl(GPIO_BINEN) & (~(0x1<<TVIN_STANDBY_GPIO_NUM)), GPIO_BINEN);
    act_writel(act_readl(GPIO_BDAT) & (~(0x1<<TVIN_STANDBY_GPIO_NUM)),GPIO_BDAT);
#endif
#ifdef  TVIN_GPIO_A_STANDBY
    act_writel(act_readl(GPIO_AOUTEN) | (0x1<<TVIN_STANDBY_GPIO_NUM), GPIO_AOUTEN);
    act_writel(act_readl(GPIO_AINEN) & (~(0x1<<TVIN_STANDBY_GPIO_NUM)), GPIO_AINEN);
    act_writel(act_readl(GPIO_ADAT) | (0x1<<TVIN_STANDBY_GPIO_NUM),GPIO_ADAT);
#endif
    return;
}

/* init rgb controller related clk and register. */
static int init_tvin_reg(void)
{
    unsigned int tmp;

    /* module driver related clk enable */
    tmp = act_readl(CMU_DEVCLKEN);
    tvin_dev.old_clk_state = tmp;

    tmp |= TVIN_NEED_CLK;   /* enable need clk. */
    act_writel(tmp, CMU_DEVCLKEN);

    /* rerest. */
    tmp = act_readl(CMU_DEVRST);
    tmp &= 0xffffeeff;      /* reset bt and tv encode. */
    act_writel(tmp, CMU_DEVRST);
    udelay(5);        
    tmp |= 0x00001100;
    act_writel(tmp, CMU_DEVRST);
    udelay(1);        

    tmp = act_readl(CMU_DMACLK) | 0x1;     /* dma4 enable. */
    act_writel(tmp, CMU_DMACLK);

    /* cofigure BT601 clk. */
    //act_writel(0x00000049, CMU_DSPPLL);  /* DSPPLL EN, DSPCLK=6M*9=54MHz. */ 
    //act_writel(0x00000384, CMU_BUSCLK);  /* SCLK=CCLK=CORECLK/2=54MHz, PCLK=SCLK/4=13.5MHz. */
    //act_writel(0x00000011, CMU_BTCLK);  /* BTCLK = DSPCLK / 2 = 27MHz. */

    /*
     * GPIO Multiple Functions; selected pin: I2C and BT601.
     * maybe be changed in real application, because different solution will use different pin multi function.
     */
    tmp = act_readl(GPIO_MFCTL1);
    tmp &= (~TVIN_PIN_MASK1);
    tmp |= TVIN_PIN_VAL1;
    act_writel(tmp, GPIO_MFCTL1);

    //act_writel(0x00000010, BT_VDIFIFOCTL);  /* set fifio drq. */
    act_writel(0x00000012, BT_VDIFIFOCTL);  /* set fifio drq. */

#ifdef  _TVIN_BT601_
    act_writel(0x00001110, BT_VDICTL);
    act_writel(0x005a0089, BT_VDIHSPOS);
    act_writel(0x00120017, BT_VDIVSEPOF);   //288 23
    act_writel(0x00120017, BT_VDIVSEPEF);   //288 23
#else

    act_writel(0x00001100, BT_VDICTL);  /* vertical and horizontal sampel rate set. */
    act_writel(0x00000000, BT_IVEOUTCTL);   /* DAC close, blue color. */
#endif

    tvin_dev.sdr_ctl_reg = act_readl(SDR_CTL);
    tmp = tvin_dev.sdr_ctl_reg;
    tmp &= ~(0xf << 12);
    tmp |= (0x7 << 12);   /* DMA4>DMA5>DMA6>DMA7>BUS */
    act_writel(tmp, SDR_CTL);

    return 0;
}

/* init dma. */
static int init_tvin_dma(void)
{
    unsigned int val;

    //reset dma
    val = act_readl(DMA_CTL);
    val |= (0x1 << 20);   /* dma4, if dma5 is 21 */
    act_writel(val, DMA_CTL);
    //pause and stop dma
    act_writel(0x0, DMA_CMD4);
    act_writel((0x3 << 8), DMA_IRQPD); /* clear dma4's pendding. */
    val = act_readl(DMA_IRQEN);
    //val &= ~(0x3 << 8);     /* disable dma4's irq. */
    val |= 0x1 << 8;        /* enable dma4's full transfered irq. */
    act_writel(val, DMA_IRQEN);


    //act_writel(0x708401ac, DMA_MODE4);  /* DMA4 mode-SDRAM->ITU, SDRAM Burst=4,Reload. */
    act_writel(0x608401ac, DMA_MODE4);  /* not reload. */
    //act_writel(0x01ac6084, DMA_MODE4);    /* DMA4 mode-SDRAM->ITU, SDRAM Burst=4 */
    //act_writel(0x11ac0084, DMA_MODE4);    /* DMA4 mode-SDRAM->ITU, SDRAM Single,Reload. */

    //val = 0x1fffffff & BT_FIFO_ODAT;
#ifndef PC
    val = 0x1fffffff & BT_FIFO_IDAT;
    act_writel(val, DMA_SRC4);
    val = 0x1fffffff & TVIN_INIT_ADDR;
    act_writel(val, DMA_DST4);
#endif

    act_writel(MAX_TVIN_FRAME_SIZE, DMA_CNT4);

    return 0;
}

int init_tvin_ic(void)
{
    void *pI2c;
    unsigned int* pVal;
    int i;
    int ret = 0;
    unsigned int i2c_port = 0;
    
#ifdef _USE_I2C_PIN_1_
    i2c_port = 0;
#else
    i2c_port = 1;
#endif

    tvin_dev.pTvin_init_data = (unsigned int*)tvin_ic_init_data;
    tvin_dev.tvin_init_cnt = sizeof(tvin_ic_init_data) / sizeof(int);

#ifdef ADI_7180
    tvin_dev.pTvin_i2c = i2c_open(0, i2c_port, 0x42, 0x42);
#endif
#ifdef TI_5150
    tvin_dev.pTvin_i2c = i2c_open(0, i2c_port, 0xb8, 0xb8);
#endif

    pI2c = tvin_dev.pTvin_i2c;
    if ((int)pI2c == -1)
    {
        return -1;
    }
    udelay(10);

    pVal = tvin_dev.pTvin_init_data;
    for (i = 0; i < tvin_dev.tvin_init_cnt; i += 2)
    {
        ret = i2c_write_data(pI2c, pVal, 2);
        
        if (ret != 0) 
        {
            printf("i2c write failed!\n");
            return -EIO;
        }
        pVal++;
        pVal++;
    }
    mdelay(10);

    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       tvin related hardware's initialization. 
* \param[in]    null
* \retval       0 sucess, else failed.
*******************************************************************************/
int tvin_hw_init(void)
{
    int ret = 0;

    tvin_poweron();
    ret = init_tvin_reg();
    
    if (ret != 0)
    {
        goto exit;
    }
        
    ret = init_tvin_ic();
    if (ret != 0)
    {
        goto exit;
    }
    ret = init_tvin_dma();
    if (ret != 0)
    {
        goto exit;
    }
    
    exit:
#if TVIN_HW_DEBUG   /* just for debug. */
    print_clk_reg(0);
#endif  /* just for debug. */

    if (ret < 0)
    {
        tvin_poweroff();
    }
    return ret;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       release tvin related hardware.
* \param[in]    null
* \retval       0 sucess, else failed.
*******************************************************************************/
int tvin_hw_exit(void)
{
    unsigned int tmp;
    void *pI2c;
    unsigned int* pVal;
    int i;
    int ret = 0;

    tvin_dev.pTvin_init_data = (unsigned int*)tvin_ic_exit_data;
    tvin_dev.tvin_init_cnt = sizeof(tvin_ic_exit_data) / sizeof(int);

    pI2c = tvin_dev.pTvin_i2c;
    udelay(10);

    pVal = tvin_dev.pTvin_init_data;
    for (i = 0; i < tvin_dev.tvin_init_cnt; i += 2)
    {
        ret = i2c_write_data(pI2c, pVal, 2);
        
        if (ret != 0) 
        {
            printf("i2c write failed!\n");
            return ret;
        }
        pVal++;
        pVal++;
    }
    mdelay(1);

    /* Clock disable */
    tmp = act_readl(CMU_DEVCLKEN) & (~(TVIN_NEED_CLK));
    tmp |= tvin_dev.old_clk_state & TVIN_NEED_CLK;
    act_writel(tmp, CMU_DEVCLKEN);

    i2c_close(tvin_dev.pTvin_i2c);

    act_writel(tvin_dev.sdr_ctl_reg, SDR_CTL);

    tvin_poweroff();
    return 0;
}


static int tvin_hw_set_resolution(unsigned int type)
{
    unsigned int val;

    switch (type) 
    {
    case VI_TVIN_RES_PAL360X288:
        act_writel(0x005a0089, BT_VDIHSPOS);
        act_writel(0x00120017, BT_VDIVSEPOF);   //288 23
        act_writel(0x00120017, BT_VDIVSEPEF);   //288 23
        val = (360 * 288) * 2;
        act_writel(val, DMA_CNT4);
        break;
    case VI_TVIN_RES_PAL352X288:
        act_writel(0x00580089, BT_VDIHSPOS);
        act_writel(0x00120017, BT_VDIVSEPOF);   //288 23
        act_writel(0x00120017, BT_VDIVSEPEF);   //288 23
        val = (352 * 288) * 2;
        act_writel(val, DMA_CNT4);
        break;
    case VI_TVIN_RES_NTSC360X240:
        act_writel(0x005a0075, BT_VDIHSPOS);
        act_writel(0x000f0012, BT_VDIVSEPOF);   //288 23
        act_writel(0x000f0012, BT_VDIVSEPEF);   //288 23
        val = (360 * 240) * 2;
        act_writel(val, DMA_CNT4);
        break;
    case VI_TVIN_RES_NTSC352X240:
        act_writel(0x00580075, BT_VDIHSPOS);
        act_writel(0x000f0012, BT_VDIVSEPOF);   //288 23
        act_writel(0x000f0012, BT_VDIVSEPEF);   //288 23
        val = (352 * 240) * 2;
        act_writel(val, DMA_CNT4);
        break;
    default:
        return -1;
    }

    return 0;
}


/******************************************************************************/
/*!                    
* \par  Description:
*       realize tvin driver's hardware function.
* \param[in]    cmd -- refer to tvin_hw.h, see tvin_hw_ioctl command.
* \param[in]    arg -- ioctl's argument.
* \retval       0 sucess, else failed.
*******************************************************************************/
int tvin_hw_ioctl(unsigned int cmd, unsigned int param)
{
    unsigned int tmp;

    switch (cmd)
    {
    case TVIN_DEBUG:
        //tvout_hw_init(0);
        break;
    case TVIN_DMA_START:     /* input interface enable. dma start. */
#ifndef PC
        act_writel(0x708401ac, DMA_MODE4);  /* DMA4 mode-SDRAM->ITU, SDRAM Burst=4,Reload. */
#ifdef  _TVIN_BT601_
        act_writel(0x00000002, BTI_MODESEL);    /* Enable decoder, bt601 */
#else
        act_writel(0x00000000, BTI_MODESEL);    /* Ch*Enable Encoder, DMA Block. input interface disable. */
#endif  /* _TVIN_BT601_ */
        udelay(1);
        act_writel(0x1, DMA_CMD4);
#ifdef  _TVIN_BT601_
        act_writel(0x00000003, BTI_MODESEL);    /* Enable decoder, bt601 */
#else
        act_writel(0x00000001, BTI_MODESEL);    /* Ch*Enable Encoder, DMA Block. input interface enable. */
#endif  /* _TVIN_BT601_ */
#endif  /* PC   */
        break;
    case TVIN_DMA_STOP:     /* input interface disable, dma stop. */
#ifndef PC
        act_writel(0x0, DMA_CMD4);
        udelay(1);
        act_writel(0x608401ac, DMA_MODE4);  /* not reload. */
        udelay(1);
#ifdef  _TVIN_BT601_
        act_writel(0x00000002, BTI_MODESEL);    /* Enable decoder, bt601 */
#else
        act_writel(0x00000000, BTI_MODESEL);    /* Ch*Enable Encoder, DMA Block. input interface disable. */
#endif  /* _TVIN_BT601_ */
#endif  /* PC   */
        break;
    case TVIN_SET_DMA_ADDR:
        act_writel(param, DMA_DST4);    
        break;
    case TVIN_GET_DMA_ADDR:
        *((unsigned int*)param) = act_readl(DMA_DST4);    
        break;
    case TVIN_GET_DMA_REMAIN:
        *((unsigned int*)param) = act_readl(DMA_REM4); 
        break;
    case TVIN_DMA_INIT:
        return init_tvin_dma();
    case TVIN_SET_RESOLUTION:
        return tvin_hw_set_resolution(param);
    case VI_IOGET_TVIN_SIGNAL:
        return (int)tvin_hw_get_signal_ok();

    default:
        return -1;
    }

    return 0;
}


/************* tvout related function. just for debug tvin. *****************/
/******************* modified from CMU_INIT(void) ***************************/
#if 0
static int tvout_clk_init(int type)
{
    unsigned int value;
    
#if 0
    value = act_readl(CMU_DSPPLL);
    //value |= 0x00000052;
    value |= 0x00000064;    
    act_writel(value, CMU_DSPPLL);
    udelay(5);

    value = act_readl(CMU_BTCLK);
    value &= ~(0x3f);
    value |= 0x00000017;    
    act_writel(value, CMU_BTCLK);
    udelay(5);        
#endif
    act_writel(0x00000049, CMU_DSPPLL);  /* DSPPLL EN, DSPCLK=6M*9=54MHz. */ 
    act_writel(0x00000011, CMU_BTCLK);  /* BTCLK = DSPCLK / 2 = 27MHz. */

    value = act_readl(CMU_DMACLK) | 0x2;     /* dma5 enable. */
    act_writel(value, CMU_DMACLK);
    udelay(5);        

    return 0;
}

/* modified from TV_ENCODER(void) */
static int tvout_reg_init(int type)
{
    unsigned int value;

    act_writel(0x00000012, BT_VEIFIFOCTL);
    act_writel(0x00000000, BTO_MODESEL);

    act_writel(0x00000400, BT_VEICTL);
    //act_writel(0x00011e2f, BT_VEIALSEOF);     //Line 23~310,共288行
    act_writel(0x00013617, BT_VEIALSEOF);       //Line 23~310,共288行

    //act_writel(0x00257168, BT_VEIALSEEF);       //Line 336~623,共288行
    act_writel(0x0026f150, BT_VEIALSEEF);       //Line 336~623,共288行

    //act_writel(0x066f0170, BT_VEIADLSE);              
    act_writel(0x06bf0120, BT_VEIADLSE);            

    /* TVEncoder Set. */
    act_writel(0x00000001, BT_IVEOUTCTL);
    act_writel(0x00000007, BT_IVECOTCTL);
    act_writel(0x00000000, BT_IVEBRGCTL);
    act_writel(0x00000007, BT_IVECSATCTL);
    act_writel(0x00000000, BT_IVECBURCTL);
    act_writel(0x00000000, BT_IVESYNCAMCTL);
    act_writel(0x00000054, BT_IVECTL);  //PAL-D;27MHz PLL
        
    /* BT IF DISbale */
    act_writel(0x00000000,BTO_MODESEL); 
    return 0;
}

static int tvout_dma_start(void)
{
    unsigned int val;

    val = act_readl(DMA_CTL);
    val |= (0x1 << 21);   /* dma5, if dma4 is 20 */
    act_writel(val, DMA_CTL);

    val = act_readl(DMA_IRQEN);
    val &= ~(0x3 << 10);     /* disable dma5's irq. */
    act_writel(val, DMA_IRQEN);

    act_writel((0x3 << 10), DMA_IRQPD); /* clear dma5's pendding. */

    //act_writel(0x008c6084, DMA_MODE5);  /* sdram --> dspram, not reload. */
    act_writel(0x11d46084, DMA_MODE5);  /* DMA5 mode-SDRAM->ITU, SDRAM Burst=4,Reload. */

    val = 0x1fffffff & 0xa053b000;      /* sdram physical address. */
    act_writel(val, DMA_SRC5);
    act_writel(0x100d0004, DMA_DST5);

    act_writel(0x00032a00, DMA_CNT5);
    act_writel(0x1, DMA_CMD5);

    return 0;
}

int tvout_hw_init(int type)
{   
    tvout_clk_init(type);
    tvout_reg_init(type);
    tvout_dma_start();

    /* BT IF enabale */
    act_writel(0x00000001, BTO_MODESEL); 
    return 0;
}

#endif
/******** end of tvout related function. ***********/

#endif  /* ADD_TVIN_DRV */

