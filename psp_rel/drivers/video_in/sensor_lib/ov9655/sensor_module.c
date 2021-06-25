/*
********************************************************************************
*                   213x --- sensor's driver
*                (c) Copyright 2007 - 2010, Actions Co,Ld. 
*                        All Right Reserved 
*
* History:        
*      <author>      <time>       <version >    <desc>
*       houjingkun      2008/05/30     1.0      build file. 
*                                               this file is used for ov9655.
*       houjingkun      2008/06/18     1.1      modify for code checking.
*       houjingkun      2008/08/05     1.2      change name from sensor_param.c 
*                                               to sensor_module.c
********************************************************************************/ 
/*!
 * \file   sensor_module.c
 * \brief  
 *      Sensor module's param setting function.
 * \author houjingkun
 * \par GENERAL DESCRIPTION:
 *      Sensor module's param setting function.
 *      it's internal API, just called by dirver itself.
 *      it include initializing function, and can adjust image's effect, 
 *      such as white_balance, brightness ...
 *      if sensor module is changed, please just change this file.
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
#include "sensor_hw.h"

static int sensor_get_exposure_param(vi_resolution_t type);
static int sensor_set_exposure_param(vi_resolution_t type);

#define CLK_D  0x00
#define CLK_M   0x5a    /* reg 0x6b: 0x5a is (X 4); 0xda is (X 8). */

/*config the power-down pin active level, if  high level enable power-down mode,define the first*/
#define  PWDN_ACTIVE_HIGH
//#define  PWDN_ACTIVE_LOW

/*config reg CMU_BTCLK, the result M_clk is : 120 / (M_CLK_DIVIDE+1) Mhz*/
//#define  M_CLK_DIVIDE 0x0000000f
#define  M_CLK_DIVIDE 0x00000021    /* fix to 24 MHZ. */

/******************************************************************************/
/*!                    
* \par  Description:
*       sensor module in i2c bus's slave_addr.
*******************************************************************************/
unsigned int i2c_slave_addr = 0x60; 

/* ov9655; 130 million pixel. */
static unsigned char sensor_i2cdata[] = 
{
    0xb5,0x00,  0x35,0x00,  0xa8,0xc1,  0x3a,0x8c,  0x3d,0x99,
    0x77,0x02,  0x26,0x72,  0x27,0x08,  0x28,0x08,  0x29,0x15,
    0x2c,0x08,  0xab,0x04,  0x6e,0x00,  0x6d,0x55,  0x00,0x32,
    0x10,0x7b,  0xbb,0xae,  0x72,0x11,  0x3e,0x0e,
    0x74,0x10,  0x76,0x01,  0x75,0x10,  0x73,0x01,  0xc7,0x81,
    0xc3,0x4e,  0x33,0x00,  0xa4,0x50,  0xaa,0x92,  0xc2,0x01,
    0xc1,0xc8,  0x1e,0x04,  0xa9,0xfa,  0x0e,0x61,  0x39,0x57,
    0x0f,0x42,  0x24,0x3c,  0x25,0x36,  0x12,0x62,  0x03,0x02,
    0x32,0x24,  0x17,0x18,  0x18,0x04,  0x19,0x01,  0x1a,0x81,
    0x36,0x3a,  0x69,0x0a,  0x8c,0x8d,  0xc0,0xaa,  0x40,0xc0,
    0xc6,0x85,  0xcb,0xf0,  0xcc,0xd8,  0x71,0xa0,  0xa5,0x90,
    0x6f,0x9e,  0x42,0xc0,  0x3f,0x82,  0x8a,0x23,  0x14,0x1a,/*0x2a,*/
    0x3b,0x0c,  0x2d,0x00,  0x2e,0x00,  0x34,0x3d,  0x41,0x41,
    0xc9,0xe0,  0xca,0xe8,  0xcd,0x93,  0x7a,0x20,  0x7b,0x1c,
    0x7c,0x28,  0x7d,0x3c,  0x7e,0x5a,  0x7f,0x68,  0x80,0x76,
    0x81,0x80,  0x82,0x88,  0x83,0x8f,  0x84,0x96,  0x85,0xa3,
    0x86,0xaf,  0x87,0xc4,  0x88,0xd7,  0x89,0xe8,  0x4f,0x98,
    0x50,0x98,  0x51,0x00,  0x52,0x28,  0x53,0x70,  0x54,0x98,
    0x58,0x1a,  0x90,0x86,  0x91,0x84,  0x9f,0x75,  0xa0,0x73,
    0x16,0x14,  0x2a,0x00,  0x2b,0x00,  0xac,0x80,  0xad,0x80,
    0xad,0x80,  0xae,0x80,  0xaf,0x80,  0xb2,0xf2,  0xb3,0x20,
    0xb4,0x20,  0xb6,0xaf,  0xb6,0xaf,  0x04,0x03,  0x05,0x2b,
    0x06,0x35,  0x07,0x36,  0x08,0x3b,  0x2f,0x35,  0x4a,0xea,
    0x4b,0xe6,  0x4c,0xe6,  0x4d,0xe6,  0x70,0x0b,  0xa6,0x40,
    0xbc,0x04,  0xbd,0x01,  0xbe,0x03,  0xbf,0x01,  0xbf,0x01,
    0x43,0x14,  0x44,0xf0,  0x45,0x46,  0x46,0x62,  0x47,0x2a,
    0x48,0x3c,  0x59,0x85,  0x5a,0xa9,  0x5b,0x64,  0x5c,0x84,
        
    0x5d,0x53,  0x5e,0x0e,  0x6c,0x0c,  0x6d,0x55,  0x6e,0x00,
    0x6f,0x9e,  0x62,0x00,  0x63,0x00,  0x64,0x02,  0x65,0x20,
    0x66,0x05,  0x9d,0x02,  0x9e,0x04,  0x29,0x15,  0xa9,0xef,
    0x11,CLK_D, /*0x01,*/  0x6b,CLK_M,  0x92,0x64,  0x93,0x00,  0xa2,0x4b,
    0xa3,0x4b,  0x6a,0x4b,  0x13,0xe7
};

/* qvga */
static unsigned char sensor_qvga_data[] = 
{
    0x12,0x62, 0x26,0x72, 0x10,0x7b, 0x72,0x11, 0x3e,0x0e, 0x74,0x10,
    0x75,0x10, 0xc7,0x81, 0xc1,0xc8, 0x24,0x3c, 0x25,0x36, 0x03,0x02,
    0x32,0x24, 0x17,0x18, 0x18,0x04, 0x1a,0x81, 0x36,0x3a, 0x8c,0x8d,
    0x6f,0x9e, 0x3f,0x82, 0x14,0x1a, 0x3b,0x0c, 0xc0,0xaa, 0x41,0x41,
    0x6b,CLK_M, 0x73,0x01, 0x90,0x86, 0x91,0x84, 0x9f,0x75, 0xa0,0x73,
    0xa6,0x40, 0x43,0x14, 0x6c,0x0c, 0x6f,0x9e, 0x66,0x05, 0x9e,0x04,
    0x11,CLK_D, 0x92,0x64, 0x6a,0x4b, 0x69,0x0a, 0xa2,0x4b, 0xa3,0x4b,
    /* new added for adjusting bug of pixel's green color hue. */
    0x4f,0x98, 0x50,0x98, 0x51,0x00, 0x52,0x28, 0x53,0x70, 0x54,0x98,
    0x59,0x85, 0x5a,0xa9, 0x5b,0x64, 0x5c,0x84, 0x5d,0x53, 0x5e,0x0e,  
};  /* OV9653QVGA 320 * 240 */

/* vga */ 
static unsigned char sensor_vga_data[] = 
{
    0x12,0x62, 0x26,0x82, 0x10,0xf0, 0x72,0x00, 0x3e,0x0c, 0x74,0x3a,
    0x75,0x35, 0xc7,0x80, 0xc1,0xc0, 0x24,0x4c, 0x25,0x46, 0x03,0x12,
    0x32,0xff, 0x17,0x16, 0x18,0x02, 0x1a,0x3d, 0x36,0xfa, 0x8c,0x8d,
    0x6f,0x9d, 0x3f,0x42, 0x14,0x1a, 0x3b,0x04, 0xc0,0xaa, 0x41,0x40,
    0x6b,CLK_M, 0x73,0x00, 0x90,0x20, 0x91,0x20, 0x9f,0x20, 0xa0,0x20,
    0xa6,0x4a, 0x43,0x00, 0x6c,0x04, 0x6f,0x9d, 0x66,0x01, 0x9e,0x03,
    0x11,CLK_D, 0x92,0x64, 0x6a,0x07, 0x69,0x0a, 0xa2,0x95, 0xa3,0x4b,
    /* new added for adjusting bug of pixel's green color hue. */
    0x4f,0x98, 0x50,0x98, 0x51,0x00, 0x52,0x28, 0x53,0x70, 0x54,0x98,
    0x59,0x85, 0x5a,0xa9, 0x5b,0x64, 0x5c,0x84, 0x5d,0x53, 0x5e,0x0e,  
};  /* OV9655VGA 640 * 480 */

#if 1
/* sxga */
static unsigned char sensor_sxga_data[] = 
{
    0x12,0x02, 0x26,0x82, 0x10,0xf0, 0x72,0x00, 0x3e,0x0c, 0x74,0x3a,
    0x75,0x35, 0xc7,0x80, 0xc1,0xc0, 0x24,0x4c, 0x25,0x46, 0x03,0x1b,
    0x32,0xff, 0x17,0x1d, 0x18,0xbd, 0x1a,0x81, 0x36,0xf8, 0x8c,0x0d,
    0x6f,0x9d, 0x3f,0x42, 0x14,0x1a, 0x3b,0x04, 0xc0,0xaa, 0x41,0x40,
    0x6b,0xda,/*0x5a,*/ 0x73,0x01, 0x90,0x20, 0x91,0x20, 0x9f,0x20, 0xa0,0x20,
    0xa6,0x4a, 0x43,0x00, 0x6c,0x04, 0x6f,0x9d, 0x66,0x01, 0x9e,0x03,
    0x11,CLK_D, 0x92,0x37, 0x6a,0x4e, 0x69,0x02, 0xa2,0x4e, 0xa3,0x4e,
    /* new added for adjusting bug of pixel's green color hue. */
    0x4f,0x98, 0x50,0x98, 0x51,0x00, 0x52,0x28, 0x53,0x70, 0x54,0x98,
    0x59,0x85, 0x5a,0xa9, 0x5b,0x64, 0x5c,0x84, 0x5d,0x53, 0x5e,0x0e, 

    0x65,0x20, 0x66,0x01, 0x9e,0x03, 
    //0x4f,0x96, 0x65,0x08, 0x66,0x05, 0x9e,0x03, 
};  /* OV9653SXGA 1280 * 960 */

#else   /* full param list */
static unsigned char sensor_sxga_data[] = 
{
    /*0x12, 0x80,*/ 0x13, 0x00, 0x00, 0x00, 0x01, 0x80, 0x02, 0x80,
    0x03, 0x1b, 0x0e, 0x61, 0x0f, 0x42, 0x10, 0xf0, 0x11, /*0x01*/CLK_D,
    0x12, 0x02, 0x14, 0x1a, 0x16, 0x14, 0x17, 0x1d, 0x18, 0xbd,
    0x19, 0x01, 0x1a, 0x81, 0x1e, 0x04, 0x24, 0x4c, 0x25, 0x46,
    0x26, 0x82, 0x27, 0x08, 0x28, 0x08, 0x2a, 0x00, 0x2b, 0x00,
    0x2c, 0x08, 0x32, 0xff, 0x33, 0x00, 0x34, 0x3d, 0x35, 0x00,
    0x36, 0xf8, 0x39, 0x57, 0x3a, 0x8c, 0x3b, 0xac, 0x3d, 0x99,
    0x3e, 0x0c, 0x3f, 0x42, 0x41, 0x40, 0x42, 0xc0, 0x6b, 0xda, //0x5a,
    0x71, 0xa0, 0x72, 0x00, 0x73, 0x00,/*0x01,*/ 0x74, 0x3a, 0x75, 0x35,
    0x76, 0x01, 0x77, 0x02, 0x7a, 0x20, 0x7b, 0x1C, 0x7c, 0x28,
    0x7d, 0x3C, 0x7e, 0x5A, 0x7f, 0x68, 0x80, 0x76, 0x81, 0x80,
    0x82, 0x88, 0x83, 0x8f, 0x84, 0x96, 0x85, 0xa3, 0x86, 0xaf,
    0x87, 0xc4, 0x88, 0xd7, 0x89, 0xe8, 0x8a, 0x23, 0x8c, 0x0d,
    0x90, 0x20, 0x91, 0x20, 0x9f, 0x20, 0xa0, 0x20, 0xa4, 0x50,
    0xa5, 0x90, 0xa6, 0x4a, 0xa8, 0xc1, 0xa9, 0xfa, 0xaa, 0x92,
    0xab, 0x04, 0xac, 0x80, 0xad, 0x80, 0xae, 0x80, 0xaf, 0x80,
    0xb2, 0xf2, 0xb3, 0x20, 0xb4, 0x20, 0xb5, 0x52, 0xb6, 0xaf,
    0xbb, 0xae, 0xb5, 0x00, 0xc0, 0xaa, 0xc1, 0xc0, 0xc2, 0x01, 
    0xc3, 0x4e, 0xC6, 0x05, 0xc7, 0x80, 0xc9, 0xe0, 0xca, 0xe8, 
    0xcb, 0xf0, 0xcc, 0xd8, 0xcd, 0x93, 0x4f, 0x98, 0x50, 0x98,
    0x51, 0x00, 0x52, 0x28, 0x53, 0x70, 0x54, 0x98, 0x3b, 0x04,
    0x43, 0x00, 0x44, 0xf0, 0x45, 0x46, 0x46, 0x62, 0x47, 0x2a,
    0x48, 0x3c, 0x59, 0x85, 0x5a, 0xa9, 0x5b, 0x64, 0x5c, 0x84,
    0x5d, 0x53, 0x5e, 0x0e, 0x69, 0x02, 0x6c, 0x04, 0x6d, 0x55,
    0x6e, 0x00, 0x6f, 0x9d, 0x62, 0x00, 0x63, 0x00, 0x64, 0x02,
    0x65, 0x20, 0x66, 0x01, 0x9d, 0x02, 0x9e, 0x03, 0x29, 0x15,
    0xa9, 0xef, 0x92, 0x37, 0x93, 0x00, 0xa2, 0x4e, 0xa3, 0x4e,
    0x6a, 0x4e, 0x13, 0xe7,//0xe7,
};  /* OV9655SXGA 1280 * 960 */
#endif
/* 中间偏红:  请修改为 0x4f=0x96 ;  0x65=0x08 ; 0x66=0x05 ;  (0x9e=0x03 or 0x04); */


static unsigned int Reg0x13,Reg0xa1,Reg0x10,Reg0x04,Reg0x2d,Reg0x2e,Reg0x2a,Reg0x2b,Reg0x00;
static unsigned int Extra_lines, Shutter, Preview_Gain16, Preview_dummy_pixels, Preview_Exposure; 
static unsigned int Gain_Exposure, Capture_Exposure, Capture_banding_Filter, Capture_Gain16;
static unsigned int Capture_maximum_shutter = 1100;
static unsigned int Capture_max_gain16 = 4 * 16;
static unsigned int cpu_clk = CPU_120MHZ;

static unsigned int preview_type = VI_RES_320X240;

static int ov9655_set_expose_mode(unsigned int type);

static int i2c_transmit_byte(sensor_i2cinfo_t * sensor_i2cinfo)
{
    unsigned int value;
    unsigned int count;
    unsigned int pin0, pin1;
    int ret =0;

    count = 0;
    do 
    {
        value = act_readl(I2CSTA);
        value &= 0x80;
        count++;
        if (count > 3000) 
        {
            SS_PRINTF("too long time not quit while act_readl(I2CSTA)!\n");
            get_multip_reg(&pin0, &pin1, 2);
            SS_PRINTF("pin0: %x  pin1:%x \n", pin0, pin1);
            ret = -1;
            break;
        }
    } 
    while (!value);   /* wait for transmit complete! */

#if 1
    count = 0;
    do 
    {
        value = act_readl(I2CCTL);
        value &= 0x1;
        count++;
        if (count > 3000) 
        {
            SS_PRINTF("too long time not quit while act_readl(I2CCTL)!\n");
            get_multip_reg(&pin0, &pin1, 2);
            SS_PRINTF("pin0: %x  pin1:%x \n", pin0, pin1);
            ret = -1;
            break;
        }
    } 
    while (!value);   /* wait for ack signal! */
#endif

    /* ACK_OK ? */
    act_writel(0x80, I2CSTA);

    return ret;
}

/* Wait_stop */
static void i2c_wait_stop(sensor_i2cinfo_t * sensor_i2cinfo)
{
    unsigned int value;
    unsigned int cnt = 0;

    do 
    {
        cnt++;
        value = act_readl(I2CSTA);
        value &= 0x40;
    } 
    while ((!value) && (cnt < 20000));   /* wait for stop condition. */

    if (cnt >= 20000)
    {
        SS_PRINTF("i2c_wait_stop timeout!\n");
    }

    act_writel(0x40, I2CSTA);   //clear the the stop bit
}

/******************************************************************************/
/*!                    
* \par  Description:
*       read sensor's control register.
* \param[in]   reg_addr -- sensor module's register addr.
* \retval      0 sucess, else failed.
*******************************************************************************/
unsigned int sensor_i2c_read(unsigned int reg_addr)
{
    int ret;    
    sensor_i2cinfo_t sensor_i2cinfo;

    ret = multip_get_pinowner(1,0,0,0);

    if (ret != 1)
    {
        return -1;
    }   
    get_sensor_i2c();
    sensor_i2cinfo.slave_addr = i2c_slave_addr;
    //    /*! open I2C CLK */
    act_writel(((0x1 << 23) | act_readl(CMU_DEVCLKEN)), CMU_DEVCLKEN);
    udelay(1);
    //DSC_I2C_RD:
    act_writel(0x00, I2CCTL);   //reset I2C 
    act_writel(0x80, I2CCTL);   //Master mode,disable IRQ

    /* 
     * here must set the PCLK to set the speed 100K or 400K 
     * I2CCLK*16=PCLK/I2CDIV 
     */
    act_writel(current_i2c_div, I2CDIV);   //set I2C 100K,here APH=12M
    act_writel(0x80, I2CCTL);   //Enable I2C 

    act_writel(sensor_i2cinfo.slave_addr, I2CADDR); //write device address of AT24C02
    act_writel(0x86, I2CCTL);   //generate start condition
    i2c_transmit_byte(&sensor_i2cinfo);

    act_writel(reg_addr, I2CDAT);
    act_writel(0x82, I2CCTL);   //release I2C; should it placed after i2c_transmit_byte??
    i2c_transmit_byte(&sensor_i2cinfo);

    act_writel(0x8b, I2CCTL);   //generate stop condition
    i2c_wait_stop(&sensor_i2cinfo);

    act_writel(sensor_i2cinfo.slave_addr + 1, I2CADDR); //write device address of AT24C02
    act_writel(0x86, I2CCTL);   //generate start condition
    i2c_transmit_byte(&sensor_i2cinfo);
    act_writel(0x83, I2CCTL);   //release I2C
    i2c_transmit_byte(&sensor_i2cinfo);

    sensor_i2cinfo.reg_data = act_readl(I2CDAT);
    act_writel(0x8b, I2CCTL);   //generate stop condition
    i2c_wait_stop(&sensor_i2cinfo);
    /*! I2C clk disable*/
    act_writel((act_readl(CMU_DEVCLKEN) & (~(0x1 << 23))), CMU_DEVCLKEN); 
        
    release_sensor_i2c();
    multip_release_pinowner(1,0);

    return (int)sensor_i2cinfo.reg_data;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       write sensor's control register.
* \param[in]   reg_addr -- sensor module's register addr.
* \param[in]   reg_data -- sensor module's register setting value.
* \retval      0 sucess, else failed.
*******************************************************************************/
int sensor_i2c_write(unsigned int reg_addr, unsigned int reg_data)
{
    int ret;    
    sensor_i2cinfo_t sensor_i2cinfo;

    ret = multip_get_pinowner(1,0,0,0);

    if (ret != 1)
    {
        return -1;
    }
    get_sensor_i2c();
    //    /*! open I2C CLK */
    act_writel(((0x1 << 23) | act_readl(CMU_DEVCLKEN)), CMU_DEVCLKEN);
    udelay(1);    
    act_writel(0x0, I2CCTL);    //reset I2C

    act_writel(0x80, I2CCTL);   //Master mode,disable IRQ

    /* 
     * here must set the PCLK to set the speed 100K or 400K
     * I2CCLK*16=PCLK/I2CDIV
     */
    act_writel(current_i2c_div, I2CDIV);   //set I2C 100K,here APH=12M
    act_writel(0x80, I2CCTL);   //Enable I2C

    act_writel(i2c_slave_addr, I2CADDR);    //write device address of AT24C02
    act_writel(0x86, I2CCTL);   //generate start condition

    ret = i2c_transmit_byte(&sensor_i2cinfo);
    if (ret != 0)
    {
        goto exit;
    }

    act_writel(reg_addr, I2CDAT);
    act_writel(0x82, I2CCTL);   //release I2C
    i2c_transmit_byte(&sensor_i2cinfo);

    act_writel(reg_data, I2CDAT);
    act_writel(0x82, I2CCTL);   //release I2C
    ret = i2c_transmit_byte(&sensor_i2cinfo);
    if (ret != 0)
    {
        goto exit;
    }

    act_writel(0x8b, I2CCTL);   //generate stop condition

    i2c_wait_stop(&sensor_i2cinfo);
    
    exit:
    /*! I2C clk disable*/
    act_writel((act_readl(CMU_DEVCLKEN) & (~(0x1 << 23))), CMU_DEVCLKEN); 
            
    release_sensor_i2c();       
    multip_release_pinowner(1,0);

    return ret;
}


inline static int ov9655_calc_sxga_exposure(void)
{
    //Capture_Exposure = Preview_Exposure * 800 / 1520; /* 24MHz, The same with vga and qvga. */
    //Capture_banding_Filter = 24000000/100/(2*1520);
    Capture_Exposure = ((Preview_Exposure * 2) * 800) / 1520;   /* 48MHz, different with vga and qvga. */
    Capture_banding_Filter = 48000000/100/(2*1520);
    Gain_Exposure = Preview_Gain16 * Capture_Exposure;

    if (Gain_Exposure < (Capture_banding_Filter * 16))
    {
        Capture_Exposure = Gain_Exposure /16;
        if (Capture_Exposure == 0)
        {
            return -1;
        }
        Capture_Gain16 = (((Gain_Exposure * 2) + 1) / Capture_Exposure) / 2;
    } 
    else 
    {
        if (Gain_Exposure > (Capture_maximum_shutter * 16)) 
        {
            Capture_Exposure = Capture_maximum_shutter;
            if (Capture_maximum_shutter == 0)
            {
                return -1;
            }
            Capture_Gain16 = (((Gain_Exposure * 2) + 1) / Capture_maximum_shutter) / 2;
            if (Capture_Gain16 > Capture_max_gain16) 
            {
                /* gain reach maximum, insert extra line */
                /* Capture_Exposure = Gain_Exposure*1.1/Capture_max_gain16; */
                /* Exposure = n/100 */
                Capture_Exposure = Gain_Exposure/16/Capture_banding_Filter;
                Capture_Exposure = Capture_Exposure * Capture_banding_Filter;

//                if (Capture_Exposure == 0) 
//                {
//                    return -1;
//                }
                Capture_Gain16 = (((Gain_Exposure * 2) + 1) / Capture_Exposure) / 2;
            }
        } 
        else 
        {    /* 1/100 < Exposure < Capture_Maximum_Shutter, Exposure = n/100 */
            Capture_Exposure = Gain_Exposure/16/Capture_banding_Filter;
            Capture_Exposure = Capture_Exposure * Capture_banding_Filter;
            Capture_Gain16 = (((Gain_Exposure * 2) +1) / Capture_Exposure) / 2;
        }
    }

    return 0;
}

static int sensor_set_exposure_param(vi_resolution_t type)
{
    unsigned int value;

    if (preview_type == (unsigned int)type)
    {
        return 0;
    }

    /* Stop AE/AG */
    Reg0x13 = sensor_i2c_read(0x13);
    value = Reg0x13 & 0xfa;
    sensor_i2c_write(0x13, value);

    /* calculate exposure value. */
    if ((type == VI_RES_320X240) || (type == VI_RES_224X176)) 
    {
        if (preview_type == VI_RES_640X480) 
        {
            Capture_Exposure = Preview_Exposure;
            Capture_Gain16 = Preview_Gain16;
        } 
        else if ((preview_type == VI_RES_1280X960) || (preview_type == VI_RES_1280X1024)) 
        {
            Capture_Exposure = Preview_Exposure * 2;
            Capture_Gain16 = Preview_Gain16 / 2;
        }
        else
        {
        }
    } 
    else if (type == VI_RES_640X480) 
    {
        if ((preview_type == VI_RES_320X240) || (preview_type == VI_RES_224X176)) 
        {
            Capture_Exposure = Preview_Exposure;
            Capture_Gain16 = Preview_Gain16;
        } 
        else if ((preview_type == VI_RES_1280X960) || (preview_type == VI_RES_1280X1024)) 
        {
            Capture_Exposure = Preview_Exposure * 2;
            Capture_Gain16 = Preview_Gain16 / 2;
        }
        else
        {
        }
    } 
    else if ((type == VI_RES_1280X960) || (type == VI_RES_1280X1024)) 
    {
        ov9655_calc_sxga_exposure();
    }
    else 
    {
    }

    Reg0xa1 &= ~(0x3f);
    Reg0xa1 |= (Capture_Exposure >> 10) & 0x3f;
    Reg0x10 &= 0x0;
    Reg0x10 |= (Capture_Exposure >> 2) & 0xff;
    Reg0x04 &= ~(0x03);
    Reg0x04 |= Capture_Exposure & 0x03;

    /* set exposure value. */
    sensor_i2c_write(0xa1, Reg0xa1);
    sensor_i2c_write(0x10, Reg0x10);
    sensor_i2c_write(0x04, Reg0x04);

    /* Read back preview shutter */
    Reg0xa1 = sensor_i2c_read(0xa1);
    Reg0x10 = sensor_i2c_read(0x10);
    Reg0x04 = sensor_i2c_read(0x04);
    Shutter = ((Reg0xa1 & 0x3f)<<10) + (Reg0x10<<2) + (Reg0x04 & 0x03);  /* before Preview_Exposure? */
    //SS_PRINTF("Shutter set to: %x\n", Shutter);

    /* write gain. */
    value = 0;
    if (Capture_Gain16 > 31) 
    {
        Capture_Gain16 = Capture_Gain16 / 2;
        value = 0x10;
    }
    if (Capture_Gain16 > 31) 
    {
        Capture_Gain16 = Capture_Gain16 / 2;
        value |= 0x20;
    }
    if (Capture_Gain16 > 31) 
    {
        Capture_Gain16 = Capture_Gain16 / 2;
        value |= 0x40;
    }
    if (Capture_Gain16 > 31) 
    {
        Capture_Gain16 = Capture_Gain16 / 2;
        value |= 0x80;
    }

    if (Capture_Gain16 > 16) 
    {
        Capture_Gain16 -= 16;
    } 
    else 
    {
        Capture_Gain16 = 1;
    }
    value = value | ((unsigned int)Capture_Gain16);
    //Capture_Gain16 = (((Reg0x00 & 0xf0)>>4) + 1) * (16 + (Reg0x00 & 0x0f));
    sensor_i2c_write(0x00, value);
                
    if (vi_dev.app_type != VI_DC_MODE) 
    {
        ov9655_set_expose_mode(vi_effect.ss_expose_mode);
    } 
    else if ((type == VI_RES_320X240) || (type == VI_RES_224X176)) 
    {
        ov9655_set_expose_mode(vi_effect.ss_expose_mode);
#if 0       
        /* maybe start AE/AG */
        if (vi_effect.ss_expose_mode == SS_AUTO_EXPOSURE) {
            Reg0x13 = Reg0x13 | 0x05;
        }
        sensor_i2c_write(0x13, Reg0x13);
#endif
    }
    else
    {
    }
                
    return 0;
};

/******************************************************************************/
/*!                    
* \par  Description:
*       set sensor image's white balance effect.
* \param[in]    white balance effect. refer to ss_white_balance_t.      \n
*       typedef enum {  \n
*           SS_AUTO_BALANCE,  \n
*           SS_INDOOR_BALANCE,  \n
*           SS_OUTDOOR_BALANCE,  \n
*           SS_FLUORE_BALANCE  \n
*       } white_balance_t;
*
* \retval       0 sucess, else failed.
*******************************************************************************/
int sensor_set_white_balance(unsigned int whitebalance)
{
    unsigned int value;
    //ov_get_white_balance = whitebalance;

    value = sensor_i2c_read(0x13);
    if (whitebalance == SS_AUTO_BALANCE) 
    {
        value |= 0x2;
    } 
    else 
    {
        value &= ~(0x2);
    }

    switch(whitebalance) 
    {
    case SS_AUTO_BALANCE:       /* auto */
        sensor_i2c_write(0x13, value);
        break;
    case SS_INDOOR_BALANCE:     /* office */
        sensor_i2c_write(0x13, value);
        sensor_i2c_write(0x01, 0x74);
        sensor_i2c_write(0x02, 0x52);
        break;
    case SS_OUTDOOR_BALANCE:        /* sunny */
        sensor_i2c_write(0x13, value);
        sensor_i2c_write(0x01, 0x50);
        sensor_i2c_write(0x02, 0x52);
        break;
    case SS_FLUORE_BALANCE:     /* home */
        sensor_i2c_write(0x13, value);
        sensor_i2c_write(0x01, 0x90);
        sensor_i2c_write(0x02, 0x40);
        break;
    case SS_CLOUDY_BALANCE:     /* cloudy */
        sensor_i2c_write(0x13, value);
        sensor_i2c_write(0x01, 0x9e);
        sensor_i2c_write(0x02, 0x39);
        break;
    default:
        break;
    }

    vi_effect.ss_white_balance = whitebalance;

    return 0;
}

/*
 *  --output:
 typedef enum { 
    SS_AUTO_BALANCE, 
    SS_INDOOR_BALANCE, 
    SS_OUTDOOR_BALANCE, 
    SS_FLUORE_BALANCE 
 } white_balance_t;
 */
inline static int ov9655_get_white_balance(void)
{
    unsigned int val0, val1;

    val0 = sensor_i2c_read(0x13);

    if ((val0 & 0x2) != 0)
    {
        return SS_AUTO_BALANCE; /* auto */
    } 
    else 
    {
        val0 = sensor_i2c_read(0x01);
        val1 = sensor_i2c_read(0x02);
        if ((val0 == 0x50) && (val1 == 0x52))  /* sunny */
        {
            return SS_OUTDOOR_BALANCE;
        }
        if ((val0 == 0x74) && (val1 == 0x52))  /* office */
        {
            return SS_INDOOR_BALANCE;
        }
        if ((val0 == 0x90) && (val1 == 0x40))  /* home */
        {
            return SS_FLUORE_BALANCE;
        }
        //if ((val0 == 0x9e) && (val1 == 0x39))  /* cloudy */
            //return 2;
    }
    return -1;
}

/*
 * --input:
 * spEffect:
 * 0 : NONE,default configure
 * 1 : white and black
 * 2 : ancientry
 * 3 : negative film
 */
inline static int ov9655v1_set_special(unsigned int spEffect)
{
    //unsigned char cValue;
    unsigned int val;
    
    val = sensor_i2c_read(0x3a);       
    val &= 0xf;
#if 0
    cValue = sensor_i2c_read(0x0f);
    cValue |= 0x01;     /* enable ADBLC. */
    sensor_i2c_write(0x0f, cValue);
#endif
        
    switch(spEffect) 
    {
    case SS_NONE_SPECIAL:       /* normal */
        SS_PRINTF("none sp\n");
        //sensor_i2c_write(0x3a, 0x00); 
        sensor_i2c_write(0x3a, 0x80|val); 
        sensor_i2c_write(0x67, 0x80);
        sensor_i2c_write(0x68, 0x80);
        break;
    case SS_WHITE_AND_BLACK:        /* black and white */
        SS_PRINTF("white black\n");
        sensor_i2c_write(0x3a, 0x90|val); 
        sensor_i2c_write(0x67, 0x80); 
        sensor_i2c_write(0x68, 0x80); 
#if 0       /* ; B/W Negative image */
        3a 30
        67 80
        68 80
#endif
        break;
    case SS_SEPIA:      /* Antique */
        SS_PRINTF("antique\n");
        sensor_i2c_write(0x3a, 0x90|val); 
        sensor_i2c_write(0x67, 0x40); 
        sensor_i2c_write(0x68, 0xa0);   /* sepia */
        //sensor_i2c_write(0x68, 0xb0);  /* antique */
        break;
    case SS_SOLARIZE:       /* negative */
        SS_PRINTF("solarize\n");
        sensor_i2c_write(0x3a, 0xa0|val); 
        sensor_i2c_write(0x67, 0x80);
        sensor_i2c_write(0x68, 0x80); 
        break;
    default:
        return -1;
    }

    vi_effect.ss_sp_effect = spEffect;

    return 0;
}

/*
 * contrast = 0 ~6;
 */
inline static int ov9655v1_set_contrast(unsigned int contrast)
{
    switch(contrast) 
    {  
    case 0:
        sensor_i2c_write(0x56, 0x10);
        break;
    case 1:
        sensor_i2c_write(0x56, 0x20);
        break;
    case 2:
        sensor_i2c_write(0x56, 0x30);
        break;
    case 3:
        sensor_i2c_write(0x56, 0x40);
        break;
    case 4:
        sensor_i2c_write(0x56, 0x50);
        break;
    case 5:
        sensor_i2c_write(0x56, 0x60);
        break;
    case 6:
        sensor_i2c_write(0x56, 0x70);
        break;
    default:
        return -1;
    }

    vi_effect.ss_contrast = contrast;
    return 0;
}

/* note:brihtness is variable with diffrent explosure value */
inline static int ov9655v1_set_brightness(unsigned int brightness)
{
    switch(brightness) 
    {
    case 0:
        sensor_i2c_write(0x55, 0xb0); 
        break;
    case 1:
        sensor_i2c_write(0x55, 0xa0); 
        break;
    case 2:
        sensor_i2c_write(0x55, 0x90); 
        break;
    case 3:
        sensor_i2c_write(0x55, 0x00); 
        break;
    case 4:
        sensor_i2c_write(0x55, 0x18); 
        break;
    case 5:
        sensor_i2c_write(0x55, 0x30); 
        break;
    case 6:
        sensor_i2c_write(0x55, 0x48); 
        break;
    default:
        return -1;
    }

    vi_effect.ss_brightness = brightness;

    return 0;
}

/*
 *
 * exposure param:
 typedef enum{
    SS_AUTO_EXPOSURE, 
    SS_INDOOR_EXPOSURE,
    SS_OUTDOOR_EXPOSURE
 } exposure_mode_t;
 */
static int ov9655_set_expose_mode(unsigned int type)
{
    unsigned int value;
    unsigned int iCapture_Gain16;
    unsigned int iCapture_Exposure;

    vi_effect.ss_expose_mode = type;

    if (type == SS_AUTO_EXPOSURE)   /* auto mode */
    {
        value = sensor_i2c_read(0x13);
        if ((value & 0x05)!=0x05)
        {
            value = value | 0x05;
            sensor_i2c_write(0x13, value);
            mdelay(400);
        }
        return 0;
    } 
    else 
    {
        value = sensor_i2c_read(0x13);
        value = value & (~(0x5));
        sensor_i2c_write(0x13, value);

        if (type == SS_INDOOR_EXPOSURE) 
        {
            iCapture_Gain16 = 0x3c;
            iCapture_Exposure = 0x253;
        } 
        else 
        {
            iCapture_Gain16 = 0x11;
            iCapture_Exposure = 0x29;
        }
    }

    /* set exposure value. */
    value = sensor_i2c_read(0xa1);
    value &= ~(0x3f);
    value |= (iCapture_Exposure >> 10) & 0x3f;
    sensor_i2c_write(0xa1, value);

    value = sensor_i2c_read(0x10);
    value &= 0x0;
    value |= (iCapture_Exposure >> 2) & 0xff;
    sensor_i2c_write(0x10, value);

    value = sensor_i2c_read(0x04);
    value &= ~(0x03);
    value |= iCapture_Exposure & 0x03;
    sensor_i2c_write(0x04, value);

    /* write gain. */
    value = 0;
    if (iCapture_Gain16 > 31) 
    {
        iCapture_Gain16 = iCapture_Gain16 / 2;
        value = 0x10;
    }

#if 0
    if (iCapture_Gain16 > 16) 
    {
        iCapture_Gain16 -= 16;
    } 
    else 
    {
        iCapture_Gain16 = 1;
    }
#endif
    iCapture_Gain16 -= 16;

    value = value | ((unsigned int)iCapture_Gain16);
    //iCapture_Gain16 = (((Reg0x00 & 0xf0)>>4) + 1) * (16 + (Reg0x00 & 0x0f));
    sensor_i2c_write(0x00, value);
                
    return 0;
};

static int sensor_get_exposure_param(vi_resolution_t type)
{
    unsigned int value;

    /* Stop AE/AG */
    Reg0x13 = sensor_i2c_read(0x13);
    value = Reg0x13 & 0xfa;
    sensor_i2c_write(0x13, value);
    
    /* Read back preview shutter */
    Reg0xa1 = sensor_i2c_read(0xa1);
    Reg0x10 = sensor_i2c_read(0x10);
    Reg0x04 = sensor_i2c_read(0x04);
                
    /* Read back extra line */
    Reg0x2d = sensor_i2c_read(0x2d);
    
    Reg0x2e = sensor_i2c_read(0x2e);

    Extra_lines = Reg0x2d + (Reg0x2e<<8);
    Shutter = ((Reg0xa1 & 0x3f)<<10) + (Reg0x10<<2) + (Reg0x04 & 0x03); //before Preview_Exposure?

    Preview_Exposure = Shutter + Extra_lines;
                
    /* Read Back Gain for preview */
    Reg0x00 = sensor_i2c_read(0x00);
    Preview_Gain16 = (((Reg0x00 & 0xf0)>>4) + 1) * (16 + (Reg0x00 & 0x0f));
                
    /* Read back dummy pixels */
    Reg0x2a = sensor_i2c_read(0x2a);
    Reg0x2b = sensor_i2c_read(0x2b);
    Preview_dummy_pixels = ((Reg0x2a & 0xf0)>>4) + Reg0x2b;

    /* maybe should start AE/AG */
    sensor_i2c_write(0x13, Reg0x13);
    preview_type = type;
                
    return 0;
};

/******************************************************************************/
/*!                    
* \par  Description:
*       get sensor module's current contrast value.
* \param[in]   null
* \return   value of current contrast level.
* \retval   range: 0 ~ 6.
*******************************************************************************/
unsigned int sensor_get_contrast(void)
{
    unsigned int val;

    val = sensor_i2c_read(0x56);
    //SS_PRINTF("reg0x56: %x \n", val);
    val &= 0xf0;
    val = val >> 4;

    if (val > 0)
    {
        val -= 1;
    }

    return val;
    //return vi_effect.ss_contrast;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       set sensor module's current contrast value.
* \param[in]   contrast -- range: 0 ~ 6.
* \retval      0 sucess, else failed.
*******************************************************************************/
int sensor_set_contrast(unsigned int contrast)
{
    return ov9655v1_set_contrast(contrast);
}

/******************************************************************************/
/*!                    
* \par  Description:
*       get sensor image's white balance effect.
* \param[in]   null
* \return       current white balance effect. refer to ss_white_balance_t.
*******************************************************************************/
unsigned int sensor_get_white_balance(void)
{
    int ret;

    ret = ov9655_get_white_balance();
    if (ret == -1) 
    {
        return ret;
    }
    vi_effect.ss_white_balance = (unsigned int) ret;

    return vi_effect.ss_white_balance;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       get sensor's focus level.
* \param[in]    null
* \return       focus level.
*******************************************************************************/
unsigned int sensor_get_focus(void)
{
    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       set sensor's focus level.
* \param[in]    sensor's focus level.
* \retval      0 sucess, else failed.
*******************************************************************************/
int sensor_set_focus(unsigned int focus)
{
    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       set sensor flashlight.
* \param[in]    flashlight level. 0 is low level, 1 is hight level
* \retval       
*******************************************************************************/
int sensor_set_flashlight(unsigned int arg)
{
#ifdef  SENSOR_FLASHLIGHT_GPIO_NUM
    act_writel(act_readl(GPIO_FLASHLIGHT_OUTEN) | (0x1 << SENSOR_FLASHLIGHT_GPIO_NUM), GPIO_FLASHLIGHT_OUTEN);
    act_writel(act_readl(GPIO_FLASHLIGHT_INEN) & (~(0x1 << SENSOR_FLASHLIGHT_GPIO_NUM)), GPIO_FLASHLIGHT_INEN);
    act_writel(act_readl(GPIO_FLASHLIGHT_DAT) & (~(0x1 << SENSOR_FLASHLIGHT_GPIO_NUM)), GPIO_FLASHLIGHT_DAT);
    
    if (arg == SS_FL_OFF)
    {
        act_writel(act_readl(GPIO_FLASHLIGHT_DAT) & (~(0x1 << SENSOR_FLASHLIGHT_GPIO_NUM)), GPIO_FLASHLIGHT_DAT);
    }
    else if (arg == SS_FL_ON)
    {
        act_writel(act_readl(GPIO_FLASHLIGHT_DAT) | (0x1 << SENSOR_FLASHLIGHT_GPIO_NUM), GPIO_FLASHLIGHT_DAT);
    }
    else if (arg == SS_FL_AUTO)
    {
        
    }
    else
    {
        return 0;
    }       
#endif
    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       get sensor image's brightness level.
* \param[in]    null
* \retval       brightness level, 0 ~ 6.
*******************************************************************************/
unsigned int sensor_get_brightness(void)
{
    return vi_effect.ss_brightness;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       set sensor image's brightness level.
* \param[in]    brightness level, 0 ~ 6.
* \retval      0 sucess, else failed.
*******************************************************************************/
int sensor_set_brightness(unsigned int brightness)
{
    return ov9655v1_set_brightness(brightness);
}

/******************************************************************************/
/*!                    
* \par  Description:
*       get sensor image's expose mode.
* \param[in]    null
* \retval       refer to ss_expose_mode_t.
*******************************************************************************/
unsigned int sensor_get_expose_mode(void)
{
    unsigned int val;

    val = sensor_i2c_read(0x13);
    val = val & 0x01;

    if (val != 0)     /* auto expose. */
    {
        return SS_AUTO_EXPOSURE;
    }

    return vi_effect.ss_expose_mode;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       set sensor image's expose mode.
* \param[in]    refer to ss_expose_mode_t.
* \retval       0 sucess, else failed.
*******************************************************************************/
int sensor_set_expose_mode(unsigned int type)
{
    return ov9655_set_expose_mode(type);
};

/******************************************************************************/
/*!                    
* \par  Description:
*       get sensor image's special mode.
* \param[in]    null
* \retval       refer to ss_special_effect_t.
*******************************************************************************/
unsigned int sensor_get_special_mode(void)
{
    return vi_effect.ss_sp_effect;
};

/******************************************************************************/
/*!                    
* \par  Description:
*       set sensor image's special mode.
* \param[in]    refer to ss_special_effect_t.
* \retval       0 sucess, else failed.
*******************************************************************************/
int sensor_set_special_mode(unsigned int type)
{
    return ov9655v1_set_special(type);
};

/******************************************************************************/
/*!                    
* \par  Description:
*       set sensor image's work mode.
* \param[in]    refer to ss_pix_fmt_t.
* \retval       0 sucess, else failed.
*******************************************************************************/
int sensor_set_pix_fmt(unsigned int type)
{
    unsigned int val;
    
    val = sensor_i2c_read(0x3a);    
    
    if (type == VI_PIX_FMT_YUV422) 
    {
        val &= 0xf3;
        sensor_i2c_write(0x3a, val);
        if (ic_type == 0x63) 
        {
            act_writel(0x00000689, M_CSICTL);   //interface control (Cb0Y0Cr0Y1, Cb1Y2Cr1Y2, ...)
        } 
        else if (ic_type == 0x96) 
        {
            act_writel(0x00000080, M_CSICTL);
        }
        else
        {
            return -1;
        }
    } 
    else   /* VI_PIX_FMT_4Y4U4Y4V */
    {
        val &= 0xf3;
        val |= 0x0c;
        sensor_i2c_write(0x3a, val);
        if (ic_type == 0x63) 
        {
            act_writel(0x00000289, M_CSICTL);       //interface control (Y0Y1Y2Y3, U0U1U2U3, Y4Y5Y6Y7, V0V1V2V3...)
        } 
        else if (ic_type == 0x96) 
        {
            act_writel(0x00000000, M_CSICTL);
        }
        else
        {
            return -1;
        }
    }
    
    /* temp fix the bug of pc camera unstable in vga 24f/s. just set it down to 15f/s. */
#if 1
    if (vi_dev.app_type == VI_PC_CAMERA_MODE)
    {
        act_writel(0x00000022, CMU_BTCLK); 
    }
    else
    {
        act_writel(M_CLK_DIVIDE, CMU_BTCLK);         
    }
#endif    
    mdelay(500);
    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       get sensor module currently supported resolution.
* \param[in]    null
* \retval       bitmap of currently supported resolution, refer to vi_resolution_t.
*******************************************************************************/
unsigned int sensor_get_pixel_bitmap(void)
{
    unsigned int iValue;

    iValue = 0x0;
    //iValue |= 0x1 << VI_RES_224X176;
    iValue |= 0x1 << VI_RES_320X240;
    iValue |= 0x1 << VI_RES_640X480;
    if (vi_dev.app_type !=VI_SMALL_MEM_MODE)
    {
        iValue |= 0x1 << VI_RES_1280X960;
        iValue |= 0x1 << VI_RES_1280X1024;
    }
    return iValue;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       get sensor module currently supported frame rate.
* \param[in]    null
* \retval       bitmap of currently supported frame rate.   \n
*               for example, if retval is 0x81, it means bit 7 and bit 0 is set, \n
*               so 8 frames/second and 1 frames/second is supported.
*******************************************************************************/
unsigned int sensor_get_f_rate_bitmap(void)
{
    unsigned int iValue;

    iValue = 0x0;

    //if (cpu_clk == CPU_120MHZ) 
    iValue |= 0x1 << 12;    /* 12 frames per second is supported. */
    iValue |= 0x1 << 16;
    iValue |= 0x1 << 20;
    iValue |= 0x1 << 24;

    return iValue;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       set sensor's frame rate in DV mode.
* \param[in]    f_rate -- set to f_rate frames/second.
* \retval       0 sucess, else failed.
* \par      exmaple code
* \code 
*           sensor_set_f_rate(24);  // set to 24 frames/second. 
* \endcode
*******************************************************************************/
int sensor_set_f_rate(unsigned int f_rate)
{
    return -1;
}

static int act_set_dc320x240_resolution(void)
{
    unsigned int i;
    unsigned int value;
    sensor_i2cinfo_t sensor_i2cinfo;

    /* cmos sensor interface disable. */
    sensor_i2cinfo.slave_addr = i2c_slave_addr;
    sensor_i2cinfo.reg_addr = 0x12;
    sensor_i2cinfo.reg_data = 0x80;

    for (i=0; i < sizeof(sensor_qvga_data); i+=2) 
    {
        sensor_i2cinfo.reg_addr = (unsigned int)sensor_qvga_data[i];
        sensor_i2cinfo.reg_data = (unsigned int)sensor_qvga_data[i+1];
        sensor_i2c_write(sensor_i2cinfo.reg_addr, sensor_i2cinfo.reg_data);
        //udelay(1);
    }

    /* 0x0000-Cmos sensor controllor. cmos sensor interface disable. */
    act_writel(0x00000012, M_CSIFIFOCTL);   //fifo control (sp bus)
    //act_writel(0x00000052, M_CSIFIFOCTL); //fifo control (ahb bus)
    if (ic_type == 0x63) 
    {
        act_writel(0x00000000, M_CSIHSPOS); //+0x000c-Hsync start position in pclk
        act_writel(0x0000027f, M_CSIHEPOS); //+0x0010-Hsync end position in pclk (QVGA 320)
        act_writel(0x00000000, M_CSIVSPOS); //+0x001c-Vsync start position in hsync
        act_writel(0x000000ef, M_CSIVEPOS); //+0x0020-Vsync end position in hsync (QVGA 240)
    } 
    else if (ic_type == 0x96) 
    {
        value = act_readl(M_CSIHSPOS);
        value &= ~(0x7ff);
        value |= 0x0;
        act_writel(value, M_CSIHSPOS);  //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIHEPOS);
        value &= ~(0x1fff << 12);
        value |= 0x280 << 12;
        act_writel(value, M_CSIHEPOS);  //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIVSPOS);
        value &= ~(0x7ff);
        value |= 0x0;
        act_writel(value, M_CSIVSPOS);  //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIVEPOS);
        value &= ~(0x1fff << 12);
        value |= 0xf0 << 12;
        act_writel(value, M_CSIVEPOS);  //+0x000c-Hsync start position in pclk
    }
    else
    {
        return -1;
    }

    act_writel(0x00000003, M_CSIRQSTA); //+0x0028-Cmos sensor interface IRQ Status
    return 0;
}

inline static int act_set_dc224x176_resolution(void)
{
    unsigned int value;
    int ret = 0;

    ret = act_set_dc320x240_resolution();

    /* set to 224X176 */
    if (ic_type == 0x63) 
    {
        act_writel(0x00000060, M_CSIHSPOS); //+0x000c-Hsync start position in pclk
        act_writel(0x0000021f, M_CSIHEPOS); //+0x0010-Hsync end position in pclk (QVGA 320)
        act_writel(0x00000020, M_CSIVSPOS); //+0x001c-Vsync start position in hsync
        act_writel(0x000000cf, M_CSIVEPOS); //+0x0020-Vsync end position in hsync (QVGA 240)
    } 
    else if (ic_type == 0x96) 
    {
        value = act_readl(M_CSIHSPOS);
        value &= ~(0x7ff);
        value |= 0x00000060;
        act_writel(value, M_CSIHSPOS);  //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIHEPOS);
        value &= ~(0x1fff << 12);
        value |= 0x220 << 12;
        act_writel(value, M_CSIHEPOS);  //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIVSPOS);
        value &= ~(0x7ff);
        value |= 0x20;
        act_writel(value, M_CSIVSPOS);  //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIVEPOS);
        value &= ~(0x1fff << 12);
        value |= 0xd0 << 12;
        act_writel(value, M_CSIVEPOS);  //+0x000c-Hsync start position in pclk
    }
    else
    {
        return -1;
    }

    return ret;
}

static int act_set_dc640x480_resolution(void)
{
    unsigned int i;
    unsigned int value;
    sensor_i2cinfo_t sensor_i2cinfo;

    /* cmos sensor interface enable. */
    sensor_i2cinfo.slave_addr = i2c_slave_addr;
    sensor_i2cinfo.reg_addr = 0x12;
    sensor_i2cinfo.reg_data = 0x80;

    for (i=0; i < sizeof(sensor_vga_data); i+=2) 
    {
        sensor_i2cinfo.reg_addr = (unsigned int)sensor_vga_data[i];
        sensor_i2cinfo.reg_data = (unsigned int)sensor_vga_data[i+1];
        sensor_i2c_write(sensor_i2cinfo.reg_addr, sensor_i2cinfo.reg_data);
        //udelay(10);
    }

    /* 0x0000-Cmos sensor controllor. cmos sensor interface disable. */
    act_writel(0x00000012, M_CSIFIFOCTL);   //fifo control (sp bus)
    //act_writel(0x00000052, M_CSIFIFOCTL); //fifo control (ahb bus)
    
    if (ic_type == 0x63) 
    {
        act_writel(0x00000000, M_CSIHSPOS); //+0x000c-Hsync start position in pclk
        act_writel(0x000004ff, M_CSIHEPOS); //+0x0010-Hsync end position in pclk (VGA 640)
        act_writel(0x00000000, M_CSIVSPOS); //+0x001c-Vsync start position in hsync
        act_writel(0x000001df, M_CSIVEPOS); //+0x0020-Vsync end position in hsync (VGA 480)
    } 
    else if (ic_type == 0x96) 
    {
        value = act_readl(M_CSIHSPOS);
        value &= ~(0x7ff);
        value |= 0x00000000;
        act_writel(value, M_CSIHSPOS);  //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIHEPOS);
        value &= ~(0x1fff << 12);
        value |= 0x500 << 12;
        act_writel(value, M_CSIHEPOS);  //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIVSPOS);
        value &= ~(0x7ff);
        value |= 0x0;
        act_writel(value, M_CSIVSPOS);  //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIVEPOS);
        value &= ~(0x1fff << 12);
        value |= 0x1e0 << 12;
        act_writel(value, M_CSIVEPOS);  //+0x000c-Hsync start position in pclk
    }
    else 
    {
        return -1;
    }

    act_writel(0x00000003, M_CSIRQSTA); //+0x0028-Cmos sensor interface IRQ Status
    return 0;
}

static int act_set_dc1280x960_resolution(void)
{
    unsigned int i;
    unsigned int value;
    sensor_i2cinfo_t sensor_i2cinfo;

    /* cmos sensor interface enable. */
    sensor_i2cinfo.slave_addr = i2c_slave_addr;
    sensor_i2cinfo.reg_addr = 0x12;
    sensor_i2cinfo.reg_data = 0x80;

    for (i=0; i<sizeof(sensor_sxga_data); i+=2) 
    {
        sensor_i2cinfo.reg_addr = (unsigned int)sensor_sxga_data[i];
        sensor_i2cinfo.reg_data = (unsigned int)sensor_sxga_data[i+1];
        sensor_i2c_write(sensor_i2cinfo.reg_addr, sensor_i2cinfo.reg_data);
        //mdelay(40);
    }

    /* 0x0000-Cmos sensor controllor. cmos sensor interface disable. */
    act_writel(0x00000012, M_CSIFIFOCTL);   //fifo control (sp bus)
    //act_writel(0x00000052, M_CSIFIFOCTL); //fifo control (ahb bus)
    if (ic_type == 0x63) 
    {
        act_writel(0x00000000, M_CSIHSPOS); //+0x000c-Hsync start position in pclk
        act_writel(0x000009ff, M_CSIHEPOS); //+0x0010-Hsync end position in pclk (SXGA 1280)
        act_writel(0x00000000, M_CSIVSPOS); //+0x001c-Vsync start position in hsync
        act_writel(0x000003bf, M_CSIVEPOS); //+0x0020-Vsync end position in hsync (SXGA 960)
    } 
    else if (ic_type == 0x96) 
    {
        value = act_readl(M_CSIHSPOS);
        value &= ~(0x7ff);
        value |= 0x00000000;
        act_writel(value, M_CSIHSPOS);  //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIHEPOS);
        value &= ~(0x1fff << 12);
        value |= 0xa00 << 12;
        act_writel(value, M_CSIHEPOS);  //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIVSPOS);
        value &= ~(0x7ff);
        value |= 0x0;
        act_writel(value, M_CSIVSPOS);  //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIVEPOS);
        value &= ~(0x1fff << 12);
        value |= 0x3c0 << 12;
        act_writel(value, M_CSIVEPOS);  //+0x000c-Hsync start position in pclk
    }
    else 
    {
        return -1;
    }

    act_writel(0x00000003, M_CSIRQSTA); //+0x0028-Cmos sensor interface IRQ Status
    return 0;
}

static int act_set_dc1280x1024_resolution(void)
{
    int ret = 0;
    unsigned int value;

    ret = act_set_dc1280x960_resolution();
    if (ic_type == 0x63) 
    {
        act_writel(0x000003ff, M_CSIVEPOS); //+0x0020-Vsync end position in hsync (SXGA 1024)
    } 
    else if (ic_type == 0x96) 
    {
        value = act_readl(M_CSIVEPOS);
        value &= ~(0x1fff << 12);
        value |= 0x400 << 12;
        act_writel(value, M_CSIVEPOS);  //+0x000c-Hsync start position in pclk
    }
    else
    {
        return -1;
    }

    return ret;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       set sensor's resolution.
* \param[in]   cmd -- resolution enum, refer to vi_resolution_t.
* \retval      0 sucess, else failed.
*******************************************************************************/
int sensor_change_resolution(vi_resolution_t cmd)
{
    unsigned int len = 0;
    int ret = 0;

    vi_dev.phy_offset = 0;
    if (cmd == vi_dev.resolution)
    {
        return ret;
    }
    sensor_get_exposure_param(vi_dev.resolution);

    switch (cmd) 
    {
    case VI_RES_160X120:
        len = 160 * 120 * 2;
        ret = -1;
        break;
    case VI_RES_176X144:
        len = 176 * 144 * 2;
        ret = -1;
        break;
    case VI_RES_224X176:
        len = 224 * 176 * 2;
        act_set_dc224x176_resolution();
        //sensor_set_exposure_param(cmd);
        vi_dev.phy_offset = VI_RES_224X176_PHY_ADDR_OFFSET;
        vi_dev.width = 224;
        vi_dev.height = 176;
        break;
    case VI_RES_320X240:
        len = 320 * 240 * 2;
        act_set_dc320x240_resolution();
        sensor_set_exposure_param(cmd);
        vi_dev.phy_offset = VI_RES_320X240_PHY_ADDR_OFFSET;
        vi_dev.width = 320;
        vi_dev.height = 240;
        //sensor is unstable when it run long time.
        ov9655v1_set_special(vi_effect.ss_sp_effect);
        if (vi_effect.ss_expose_mode==SS_AUTO_EXPOSURE)
        {
            ov9655_set_expose_mode(SS_INDOOR_EXPOSURE);
            mdelay(400);
            ov9655_set_expose_mode(SS_AUTO_EXPOSURE);
        }
        else
        {
            ov9655_set_expose_mode(vi_effect.ss_expose_mode);
        }
        printf("reset special and expose.\n");     
        break;
    case VI_RES_640X480:
        len = 640 * 480 * 2;
        act_set_dc640x480_resolution();
        sensor_set_exposure_param(cmd);
        vi_dev.phy_offset = VI_RES_640X480_PHY_ADDR_OFFSET;
        vi_dev.width = 640;
        vi_dev.height = 480;
        break;
    case VI_RES_1024X768:
        len = 1024 * 768 * 2;
        ret = -1;
        break;
    case VI_RES_1280X960:
        len = 1280 * 960 * 2;
        act_set_dc1280x960_resolution();
        sensor_set_exposure_param(cmd);
        vi_dev.width = 1280;
        vi_dev.height = 960;
        break;
    case VI_RES_1280X1024:
        len = 1280 * 1024 * 2;
        act_set_dc1280x1024_resolution();
        sensor_set_exposure_param(cmd);
        vi_dev.width = 1280;
        vi_dev.height = 1024;
        break;      
    case VI_RES_1600X1200:
        len = 1600 * 1200 * 2;
        ret = -1;
        break;
    case VI_RES_2048X1536:
        len = 2048 * 1536 * 2;
        ret = -1;
        break;
    default:
        SS_PRINTF("resolution type error!\n");
        ret = -1;
    }
    //os_time_dly(1);

    if (len > vi_dev.mem_len) 
    {
        SS_PRINTF("This sensor doesn't support this resolution\n");
        return -1;
    }
    vi_dev.resolution = cmd;
    vi_dev.resolution_len = len;
    vi_dev.first_frame = 1;

    return ret;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       initialize sensor module.
* \param[in]   null
* \retval      0 sucess, else failed.
*******************************************************************************/
int sensor_init_param_reg(void)
{
    unsigned int i;
    unsigned int value;
    int ret = 0;
    sensor_i2cinfo_t sensor_i2cinfo;

    sensor_i2cinfo.slave_addr = i2c_slave_addr;
    sensor_i2cinfo.reg_addr = 0x12;
    sensor_i2cinfo.reg_data = 0x80;

    sensor_i2c_write(sensor_i2cinfo.reg_addr, sensor_i2cinfo.reg_data);
    mdelay(10);

    for (i=0; i < sizeof(sensor_i2cdata); i+=2) 
    {
        sensor_i2cinfo.reg_addr = (unsigned int)sensor_i2cdata[i];
        sensor_i2cinfo.reg_data = (unsigned int)sensor_i2cdata[i+1];
        ret = sensor_i2c_write(sensor_i2cinfo.reg_addr, sensor_i2cinfo.reg_data);
        if (ret != 0)
        {
            return -EIO;
        }
        //mdelay(4);
    }

    if (ic_type == 0x63) 
    {
        act_writel(0x00000000, M_CSIHSPOS); //+0x000c-Hsync start position in pclk
        act_writel(0x0000027f, M_CSIHEPOS); //+0x0010-Hsync end position in pclk (QVGA 320)
        act_writel(0x00000000, M_CSIVSPOS); //+0x001c-Vsync start position in hsync
        act_writel(0x000000ef, M_CSIVEPOS); //+0x0020-Vsync end position in hsync (QVGA 240)
    } 
    else if (ic_type == 0x96) 
    {
        value = act_readl(M_CSIHSPOS);
        value &= ~(0x7ff);
        value |= 0x0;
        act_writel(value, M_CSIHSPOS);  //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIHEPOS);
        value &= ~(0x1fff << 12);
        value |= 0x280 << 12;
        act_writel(value, M_CSIHEPOS);  //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIVSPOS);
        value &= ~(0x7ff);
        value |= 0x0;
        act_writel(value, M_CSIVSPOS);  //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIVEPOS);
        value &= ~(0x1fff << 12);
        value |= 0xf0 << 12;
        act_writel(value, M_CSIVEPOS);  //+0x000c-Hsync start position in pclk
    }
    else
    {
        return -1;
    }

    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       set sensor module to standby status.
* \param[in]   null
* \retval      0 sucess, else failed.
*******************************************************************************/
int sensor_exit_param_reg(void) 
{
    unsigned int value;

    /* disable sensor's AGC, AWB, AEC. */
    value = sensor_i2c_read(0x13);
    value &= ~(0x7);
    sensor_i2c_write(0x13, value);

    /* soft sleep mode. */
    value = sensor_i2c_read(0x09);
    value |= (0x1 << 4);
    sensor_i2c_write(0x09, value);

    sensor_i2c_write(0x00, 0);  /* gain = 0 */
    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       sensor module power on.
* \param[in]   null
* \retval      null.
*******************************************************************************/
void sensor_power_on(void)
{
    unsigned int value;

    /* power on. */
#ifdef PWDN_ACTIVE_HIGH    
    value = act_readl(PMU_CTL);
    value &= ~(0x1 << 14);   
    act_writel(value, PMU_CTL);
#elif defined(PWDN_ACTIVE_LOW)
    value = act_readl(PMU_CTL);
    value |= (0x1 << 14); 
    act_writel(value, PMU_CTL);
#endif
    return;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       sensor module power off.
* \param[in]   null
* \retval      null.
*******************************************************************************/
void sensor_power_off(void)
{
    unsigned int value;

    /* power off. */
#ifdef PWDN_ACTIVE_HIGH    
    value = act_readl(PMU_CTL);
    value |= (0x1 << 14); 
    act_writel(value, PMU_CTL);
#elif defined(PWDN_ACTIVE_LOW)
    value = act_readl(PMU_CTL);
    value &= ~(0x1 << 14);      
    act_writel(value, PMU_CTL);
#endif
    return;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       sensor module clk in setting, it will affect frame rate.
* \param[in]   null
* \retval      null.
*******************************************************************************/
void sensor_init_m_clk(void)
{
    act_writel(M_CLK_DIVIDE, CMU_BTCLK);  /* 1/10 bus clock */    
    return;
}

void modify_data(unsigned long addr, unsigned int resolution, unsigned int app_type)
{
    return;
}
