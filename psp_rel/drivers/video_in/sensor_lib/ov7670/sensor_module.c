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

/*config the power-down pin active level, if  high level enable power-down mode,define the first*/
#define  PWDN_ACTIVE_HIGH
//#define  PWDN_ACTIVE_LOW

/*config reg CMU_BTCLK, the result M_clk is : 120 / (M_CLK_DIVIDE+1) Mhz*/
//#define  M_CLK_DIVIDE 0x0000000f
#define  M_CLK_DIVIDE 0x00000020    /* fix to 24MHZ. */

/******************************************************************************/
/*!                    
* \par  Description:
*       sensor module in i2c bus's slave_addr.
*******************************************************************************/
unsigned int i2c_slave_addr = 0x42;     /* gc0306 write address. */

unsigned int val_0x41 = 0x2f;
static unsigned char sensor_i2cdata[] = 
/* init code for galaxycore. */
{   
    //0x12, 0x80,
    0x11, 0x80,//0x00,
    0x3a, 0x0c,//04,
    0x12, 0x00,
    
    0x17, 0x13,
    0x18, 0x01,
    0x32, 0xb6,
    0x19, 0x02,
    0x1a, 0x7a,
    0x03, 0x0a,
    
    0x0c, 0x00,
    0x3e, 0x00,
    0x70, 0x3a,
    0x71, 0x35,
    0x72, 0x11,
    0x73, 0xf0,
    0xa2, 0x02,
    
    0x7a, 0x24,
    0x7b, 0x04,
    0x7c, 0x07,
    0x7d, 0x10,
    0x7e, 0x28,
    0x7f, 0x36,
    0x80, 0x44,
    0x81, 0x52,
    0x82, 0x60,
    0x83, 0x6c,
    0x84, 0x78,
    0x85, 0x8c,
    0x86, 0x9e,
    0x87, 0xbb,
    0x88, 0xd2,
    0x89, 0xe5,
    
    0x13, 0xe0,
    0x00, 0x00,
    0x10, 0x00,
    0x0d, 0x60,
    0x42, 0x80,
    0x14, 0x28,  
    0xa5, 0x07,
    0xab, 0x08,
    0x24, 0x54,
    0x25, 0x4c,
    0x26, 0x82,
    0x9f, 0x78,
    0xa0, 0x68,
    0xa1, 0x03,
    0xa6, 0xd8,
    0xa7, 0xd8,
    0xa8, 0xf0,
    0xa9, 0x90,
    0xaa, 0x14,
    0x13, 0xe5,
    
    0x0e, 0x61,
    0x0f, 0x4b,
    0x16, 0x02,
    0x1e, 0x07, 
    0x21, 0x02,
    0x22, 0x91,
    0x29, 0x07,
    0x33, 0x0b, 
    0x35, 0x0b,
    0x37, 0x1d,
    0x38, 0x71,
    0x39, 0x2a, 
    0x3c, 0x78,
    0x4d, 0x40,
    0x4e, 0x20,
    0x69, 0x00,
    0x6b, 0x0a,
    0x74, 0x10,
    
    0x8d, 0x4f,
    0x8e, 0x00,
    0x8f, 0x00,
    0x90, 0x00,
    0x91, 0x00,
    0x92, 0x66,
    0x93, 0x00,
    0x2a, 0x00,
    0x2b, 0x00,
    0x96, 0x00,
    0x9a, 0x80,
    0xb0, 0x84, 
    0xb1, 0x0c,
    0xb2, 0x0e,
    0xb3, 0x82,
    0xb8, 0x0a,
    
    0x43, 0x0a,
    0x44, 0xf0,
    0x45, 0x34,
    0x46, 0x58,
    0x47, 0x28,
    0x48, 0x3a,
    0x59, 0x88,
    0x5a, 0x88,
    0x5b, 0xe4,
    0x5c, 0x67,
    0x5d, 0x49,
    0x5e, 0x1d,
    0x6c, 0x0a,
    0x6d, 0x55,
    0x6e, 0x11,
    0x6f, 0x9e, 
    
    0x6a, 0x40,
    0x01, 0x40,
    0x02, 0x40,
    0x13, 0xe7,
    
    0x4f, 0x73,
    0x50, 0x73,
    0x51, 0x00,
    0x52, 0x1f,
    0x53, 0x55,
    0x54, 0x73,
    0x58, 0x9e,
         
    0x3f, 0x00,
    0x75, 0x23, 
    0x76, 0xe1,
    0x4c, 0x00,
    0x77, 0x00, 
    0x3d, 0xc2,
    0x4b, 0x09,
    0xc9, 0x60,
    0x41, 0x38,
    0x56, 0x40,
    
    0x34, 0x11,
    0x3b, 0x0a,
    0xa4, 0x88,
    0x96, 0x00,
    0x97, 0x30,
    0x98, 0x20,
    0x99, 0x30,
    0x9a, 0x84,
    0x9b, 0x29,
    0x9c, 0x03,
    0x9d, 0x99,
    0x9e, 0x99,
    0x78, 0x04,
    
    0x79, 0x01,
    0xc8, 0xf0,
    0x79, 0x0f,
    0xc8, 0x00,
    0x79, 0x10,
    0xc8, 0x7e,
    0x79, 0x0a,
    0xc8, 0x80,
    0x79, 0x0b,
    0xc8, 0x01,
    0x79, 0x0c,
    0xc8, 0x0f,
    0x79, 0x0d,
    0xc8, 0x20,
    0x79, 0x09,
    0xc8, 0x80,
    0x79, 0x02,
    0xc8, 0xc0,
    0x79, 0x03,
    0xc8, 0x40,
    0x79, 0x05,
    0xc8, 0x30,
    0x79, 0x26,
    0x2d, 0x00,
    0x2e, 0x00,
    
    0x62, 0x00,
    0x63, 0x00,
    0x64, 0x10,
    0x65, 0x00,
    0x66, 0x05,
    0x94, 0x10,
    0x95, 0x13,
    
    //for qvga
    0x17, 0x15,
    0x18, 0x03,
    0x32, 0x36, 
    0x19, 0x02,
    0x1a, 0x7a,
    0x03, 0x0a,
    
    0x0c, 0x04,
    0x3e, 0x19,
    0x70, 0x3a,
    0x71, 0x35,
    0x72, 0x11,
    0x73, 0xf1,
    0xa2, 0x02,
};

static unsigned char sensor_qvga_data[] = 
/* init code for galaxycore. */
{
#if 0
    0x17, 0x13,
    0x18, 0x01,
    0x32, 0xb6, 
    0x19, 0x02,
    0x1a, 0x7a,
    0x03, 0x0a, 
    0x0c, 0x00,
    0x3e, 0x00,
    0x70, 0x3a,
    0x71, 0x35,
    0x72, 0x11,
    0x73, 0xf0,
    0xa2, 0x02,
    0xb1, 0x0c,
#else
    0x17, 0x15,
    0x18, 0x03,
    0x32, 0x36, 
    0x19, 0x02,
    0x1a, 0x7a,
    0x03, 0x0a, 
    0x0c, 0x04,
    0x3e, 0x19,
    0x70, 0x3a,
    0x71, 0x35,
    0x72, 0x11,
    0x73, 0xf1,
    0xa2, 0x02,
    //0xb1, 0x00,
    //0xb1, 0x0c,   
    0x3d, 0xc2,
#endif
};

static unsigned char sensor_vga_data[] = 
/* init code for galaxycore. */
{
    0x17, 0x14,
    0x18, 0x02,
    0x32, 0x92, 
    0x19, 0x02,
    0x1a, 0x7a,
    0x03, 0x0a,
    
    0x0c, 0x00,
    0x3e, 0x00,
    0x70, 0x3a,
    0x71, 0x35,
    0x72, 0x11,
    0x73, 0xf0,
    0xa2, 0x02,
    
    0x3d, 0xc3,
};

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
    act_writel (current_i2c_div,I2CDIV);
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
    switch(whitebalance) 
    {
    case SS_AUTO_BALANCE:       /* auto */
        sensor_i2c_write(0x13, 0xe7);/*AWB ON*/
        sensor_i2c_write(0x3b,0x0a);
        sensor_i2c_write(0x2d,0x00);
        sensor_i2c_write(0x2e,0x00); 
        break;
    case SS_INDOOR_BALANCE:     /* office */
        sensor_i2c_write(0x13, 0xe5);/*AWB OFF*/
        sensor_i2c_write(0x01,0x5a);
        sensor_i2c_write(0x02,0x5c);
        sensor_i2c_write(0x3b,0x0a);
        sensor_i2c_write(0x2d,0x00);
        sensor_i2c_write(0x2e,0x00);        
        break;
    case SS_OUTDOOR_BALANCE:        /* sunny */
        sensor_i2c_write(0x13, 0xe5);/*AWB OFF*/
        sensor_i2c_write(0x01,0x84);
        sensor_i2c_write(0x02,0x4c);
        sensor_i2c_write(0x3b,0x0a);
        sensor_i2c_write(0x2d,0x00);
        sensor_i2c_write(0x2e,0x00);  
        break;
    case SS_FLUORE_BALANCE:     /* home */
        sensor_i2c_write(0x13, 0xe5);/*AWB OFF*/
        sensor_i2c_write(0x01,0x96);
        sensor_i2c_write(0x02,0x40);
        sensor_i2c_write(0x3b,0x0a);
        sensor_i2c_write(0x2d,0x00);
        sensor_i2c_write(0x2e,0x00);  
        break;
    default:
        break;
    }

    vi_effect.ss_white_balance = whitebalance;

    return 0;
}

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
    return vi_effect.ss_contrast;
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
    vi_effect.ss_contrast = contrast;
    return 0;
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
    unsigned int reg04,reg03,reg40,regdb,regdc,result;

    act_writel(act_readl(GPIO_FLASHLIGHT_OUTEN) | (0x1 << SENSOR_FLASHLIGHT_GPIO_NUM), GPIO_FLASHLIGHT_OUTEN);
    act_writel(act_readl(GPIO_FLASHLIGHT_INEN) & (~(0x1 << SENSOR_FLASHLIGHT_GPIO_NUM)), GPIO_FLASHLIGHT_INEN);
    act_writel(act_readl(GPIO_FLASHLIGHT_DAT) & (~(0x1 << SENSOR_FLASHLIGHT_GPIO_NUM)), GPIO_FLASHLIGHT_DAT);
    
    if (arg == SS_FL_OFF)
    {
        act_writel(act_readl(GPIO_FLASHLIGHT_DAT) & (~(0x1 << SENSOR_FLASHLIGHT_GPIO_NUM)), GPIO_FLASHLIGHT_DAT);
        printf("set_flashlight,OFF.\n");
    }
    else if (arg == SS_FL_ON)
    {
        act_writel(act_readl(GPIO_FLASHLIGHT_DAT) | (0x1 << SENSOR_FLASHLIGHT_GPIO_NUM), GPIO_FLASHLIGHT_DAT);
        printf("set_flashlight,ON.\n");
    }
    else if (arg == SS_FL_AUTO)
    {        
        reg04 = sensor_i2c_read(0x04);
        reg03 = sensor_i2c_read(0x03);
        regdb = sensor_i2c_read(0xdb);
        regdc = sensor_i2c_read(0xdc);
        reg40 = sensor_i2c_read(0x40);
        if (reg40 != 0)
        {
            result = ((reg03 * 256) + reg04 ) * regdb * regdc / reg40 / reg40;
        }
        else
        {
            result = 0;
        }
        if (result > 0x300)
        {
            act_writel(act_readl(GPIO_FLASHLIGHT_DAT) | (0x1 << SENSOR_FLASHLIGHT_GPIO_NUM), GPIO_FLASHLIGHT_DAT);
        }
        else
        {
            act_writel(act_readl(GPIO_FLASHLIGHT_DAT) & (~(0x1 << SENSOR_FLASHLIGHT_GPIO_NUM)), GPIO_FLASHLIGHT_DAT);
        }
        printf("set_flashlight,AUTO result value is %x.\n",result);
    }
    else
    {
        return -1;
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

    switch(brightness) 
    {
    case 0:
        sensor_i2c_write(0x55, 0xe0);
        break;      
    case 1:
        sensor_i2c_write(0x55, 0xb0);
        break;              
    case 2:
        sensor_i2c_write(0x55, 0x98);
        break;              
    case 3:
        sensor_i2c_write(0x55, 0x10);  // 00
        break;              
    case 4:
        sensor_i2c_write(0x55, 0x18);
        break;              
    case 5:
        sensor_i2c_write(0x55, 0x30);
        break;              
    case 6:
        sensor_i2c_write(0x55, 0x60);
        break;              
    default:
        return -1;
    }

    vi_effect.ss_brightness = brightness;
  
    return 0;
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
    return 0;
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
    switch(type) 
    {
    case SS_NONE_SPECIAL:       /* normal */
        sensor_i2c_write(0x3a, 0x0c); 
        sensor_i2c_write(0x67, 0x80);
        sensor_i2c_write(0x68, 0x80);
        break;
    case SS_WHITE_AND_BLACK:    /* black and white */
        sensor_i2c_write(0x3a, 0x1c); 
        sensor_i2c_write(0x67, 0x80);
        sensor_i2c_write(0x68, 0x80);
        break;
    case SS_SEPIA:      /* Antique */
        sensor_i2c_write(0x3a, 0x1c); 
        sensor_i2c_write(0x67, 0xa0);
        sensor_i2c_write(0x68, 0x40);   
        break;
    case SS_SOLARIZE:       /* negative */
        sensor_i2c_write(0x3a, 0x2c); //24
        sensor_i2c_write(0x67, 0x80);
        sensor_i2c_write(0x68, 0x80);
        break;  

    default:
        return -1;
    }


    vi_effect.ss_sp_effect = type;
    return 0;
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
    
    if (type == VI_PIX_FMT_YUV422) 
    {
        sensor_i2c_write(0x3a, 0x04);
        sensor_i2c_write(0x3d, 0xc2);
        if (ic_type == 0x63) 
        {
            act_writel(0x00000689, M_CSICTL);   //interface control (Cb0Y0Cr0Y1, Cb1Y2Cr1Y2, ...)
        } 
        else if (ic_type == 0x96) 
        {
            val = act_readl(M_CSICTL) | (0x1 << 7);
            act_writel(val, M_CSICTL);
        } 
        else   /* SS_4Y4U4Y4V */
        {
            return -1;
        }
    } 
    else   /* VI_PIX_FMT_4Y4U4Y4V */
    {
        sensor_i2c_write(0x3a, 0x0c);
        if (ic_type == 0x63) 
        {
            act_writel(0x00000289, M_CSICTL);       //interface control (Y0Y1Y2Y3, U0U1U2U3, Y4Y5Y6Y7, V0V1V2V3...)
        } 
        else if (ic_type == 0x96) 
        {

            val = act_readl(M_CSICTL) & (~(0x1 << 7));
            act_writel(val, M_CSICTL);
        }
        else
        {
            return -1;
        }
    }
    
#if 1
    if (vi_dev.app_type == VI_PC_CAMERA_MODE)
    {
        act_writel(0x00000021, CMU_BTCLK);  /* 12 MHZ clock out. */
        mdelay(20);
    }
    else
    {
        act_writel(M_CLK_DIVIDE, CMU_BTCLK);    /* 24 MHZ clock out. */
    }
#endif    
    if ((vi_dev.app_type == VI_DV_MODE)&&(vi_dev.resolution == VI_RES_640X480))
    {   
        mdelay(400);
    }
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
    //iValue |= 0x1 << VI_RES_1280X960;
    //iValue |= 0x1 << VI_RES_1280X1024;

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
//    unsigned int iValue;
//
//    iValue = 0x0;
//    iValue |= 0x1 << 24;    /* 24 frames per second is supported. */
//
//    return iValue;
    return 15;
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

static int act_set_dc640x480_resolution(void)
{
    unsigned int i;
    unsigned int value;
    sensor_i2cinfo_t sensor_i2cinfo;

    for (i=0; i < sizeof(sensor_vga_data); i+=2) 
    {
        sensor_i2cinfo.reg_addr = (unsigned int)sensor_vga_data[i];
        sensor_i2cinfo.reg_data = (unsigned int)sensor_vga_data[i+1];
        sensor_i2c_write(sensor_i2cinfo.reg_addr, sensor_i2cinfo.reg_data);
        //udelay(10);
    }
    if (vi_dev.app_type == VI_PC_CAMERA_MODE)
    {
        sensor_i2c_write(0x3d, 0xc2);
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

    switch (cmd) 
    {
    case VI_RES_320X240:
        len = 320 * 240 * 2;
        act_set_dc320x240_resolution();
        vi_dev.phy_offset = VI_RES_320X240_PHY_ADDR_OFFSET;
        vi_dev.width = 320;
        vi_dev.height = 240;
        break;
    case VI_RES_640X480:
        len = 640 * 480 * 2;
        act_set_dc640x480_resolution();
        vi_dev.phy_offset = VI_RES_640X480_PHY_ADDR_OFFSET;
        vi_dev.width = 640;
        vi_dev.height = 480;
        break;
    default:
        SS_PRINTF("resolution type error!\n");
        ret = -1;
    }

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
    int ret;
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
            return -1;
        }
    }


    if (ic_type == 0x63) 
    {
    
//        sensor_i2c_write(0X45, 0X49);
//        sensor_i2c_write(0x4e, 0x33);
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
