/*
********************************************************************************
*                   213x --- sensor's driver
*                (c) Copyright 2007 - 2010, Actions Co,Ld. 
*                        All Right Reserved 
*
* History:        
*      <author>      <time>       <version >    <desc>
*       houjingkun      2008/05/30     1.0      build file. 
*                                               this file is used for gc0307.
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

void stuff_point_4y4u(unsigned char *src,int width,int height);
void stuff_point_yuyv(unsigned char *src,int width,int height);
void stuff_point_yuv420(unsigned char *src,int width,int height);
static unsigned int reload_mode = 0;

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
        //========= close output
    0x43  ,0x00, 
    0x44  ,0xa2, 
        
        //========= close some functions
        // open them after configure their parmameters
    0x40  ,0x10,  
    0x41  ,0x00,            
    0x42  ,0x10,                        
    0x47  ,0x00,  //mode1,                  
    0x48  ,0xc3,  //mode2,  
    0x49  ,0x00,  //dither_mode         
    0x4a  ,0x00,  //clock_gating_en
    0x4b  ,0x00,  //mode_reg3
    0x4E  ,0x23,  //sync mode
    0x4F  ,0x01,  //AWB, AEC, every N frame 
        
        //========= frame timing
    0x01  ,0x6a, //HB
    0x02  ,0x70, //VB
    0x1C  ,0x00, //Vs_st
    0x1D  ,0x00, //Vs_et
    0x10  ,0x00, //high 4 bits of VB, HB
    0x11  ,0x05, //row_tail,  AD_pipe_number
        
        
        //========= windowing
    0x05  ,0x00, //row_start
    0x06  ,0x00,
    0x07  ,0x00, //col start
    0x08  ,0x00, 
    0x09  ,0x01, //win height
    0x0A  ,0xE8,
    0x0B  ,0x02, //win width, pixel array only 640
    0x0C  ,0x80,
        
        //========= analog
    0x0D  ,0x22, //  0522 rsh_width
                          
    0x0E  ,0x02, //CISCTL mode2,  
    0x0F  ,0x82, //CISCTL mode1
                  
    0x12  ,0x70, //7 hrst, 6_4 darsg,
    0x13  ,0x00, //7 CISCTL_restart, 0 apwd
    0x14  ,0x00, //NA
    0x15  ,0xba, //7_4 vref
    0x16  ,0x13, //5to4 _coln_r,  __1to0__da18
    0x17  ,0x52, //opa_r, ref_r, sRef_r
    0x18  ,0xc0, //analog_mode, best case for left band.
        
    0x1E  ,0x0d, //tsp_width           
    0x1F  ,0x32, //sh_delay
        
        //========= offset
    0x47  ,0x00,  //7__test_image, __6__fixed_pga, __5__auto_DN, __4__CbCr_fix, 
                    //__3to2__dark_sequence, __1__allow_pclk_vcync, __0__LSC_test_image
    0x19  ,0x06,  //pga_o            
    0x1a  ,0x06,  //pga_e            
        
    0x31  ,0x00,  //4   //pga_oFFset ,   high 8bits of 11bits
    0x3B  ,0x00,  //global_oFFset, low 8bits of 11bits
        
    0x59  ,0x0f,  //offset_mode     
    0x58  ,0x88,  //DARK_VALUE_RATIO_G,  DARK_VALUE_RATIO_RB
    0x57  ,0x08,  //DARK_CURRENT_RATE
    0x56  ,0x77,  //PGA_OFFSET_EVEN_RATIO, PGA_OFFSET_ODD_RATIO
        
        //========= blk
    0x35  ,0xd8,  //blk_mode
    
    0x36  ,0x40,  
        
    0x3C  ,0x00, 
    0x3D  ,0x00, 
    0x3E  ,0x00, 
    0x3F  ,0x00, 
        
    0xb5  ,0x70, 
    0xb6  ,0x40, 
    0xb7  ,0x00, 
    0xb8  ,0x38, 
    0xb9  ,0xc3,          
    0xba  ,0x0f, 
        
    0x7e  ,0x35, 
    0x7f  ,0x86, 
        
    0x5c  ,0x68, //78
    0x5d  ,0x78, //88
        
        
        //========= manual_gain 
    0x61  ,0x80, //manual_gain_g1   
    0x63  ,0x80, //manual_gain_r
    0x65  ,0x98, //manual_gai_b, 0xa0=1.25, 0x98=1.1875
    0x67  ,0x80, //manual_gain_g2
    0x68  ,0x18, // global_manual_gain   2.4bits
        
        //=========CC _R
    0x69  ,0x58,  //54
    0x6A  ,0xf6,  //ff
    0x6B  ,0xfb,  //fe
    0x6C  ,0xf4,  //ff
    0x6D  ,0x5a,  //5f
    0x6E  ,0xe6,  //e1
    
    0x6f  ,0x00,    
        
        //=========lsc                            
    0x70  ,0x14, 
    0x71  ,0x1c, 
    0x72  ,0x20, 
        
    0x73  ,0x10,    
    0x74  ,0x3c, 
    0x75  ,0x52, 
        
        //=========dn                                                                            
    0x7d  ,0x2f,  //dn_mode     
    0x80  ,0x0c, //when auto_dn, check 7e,7f
    0x81  ,0x0c,
    0x82  ,0x44,
                                                                                            
        //dd                                                                           
    0x83  ,0x18,  //DD_TH1                    
    0x84  ,0x18,  //DD_TH2                    
    0x85  ,0x04,  //DD_TH3                                                                                            
    0x87  ,0x34,  //32 b DNDD_low_range X16,  DNDD_low_range_C_weight_center                    
        
           
        //=========intp-ee                                                                         
    0x88  ,0x04,                                                       
    0x89  ,0x01,                                          
    0x8a  ,0x50,//60                                           
    0x8b  ,0x50,//60                                           
    0x8c  ,0x07,                                                                  
                                                                                          
    0x50  ,0x0c,                                
    0x5f  ,0x3c,                                                                                     
                                                                                         
    0x8e  ,0x02,                                                              
    0x86  ,0x02,                                                                  
                                                                                        
    0x51  ,0x20,                                                                
    0x52  ,0x08,  
    0x53  ,0x00, 
        
        
        //========= YCP 
        //contrast_center                                                                             
    0x77  ,0x80, //contrast_center       
    0x78  ,0x00, //fixed_Cb              
    0x79  ,0x00, //fixed_Cr              
    0x7a  ,0x20, //luma_offset  
    0x7b  ,0x40, //hue_cos               
    0x7c  ,0x00, //hue_sin               
                                         
        //saturation                     
    0xa0  ,0x40, //40global_saturation
    0xa1  ,0x40, //luma_contrast         
    0xa2  ,0x34, //saturation_Cb         
    0xa3  ,0x34, //saturation_Cr
                                           
    0xa4  ,0xc8,                                      
    0xa5  ,0x02, 
    0xa6  ,0x28,                                                                              
    0xa7  ,0x02, 
        
        //skin                                                                                                
    0xa8  ,0xee,                                                              
    0xa9  ,0x12,                                                              
    0xaa  ,0x01,                                                          
    0xab  ,0x20,                                                      
    0xac  ,0xf0,                                                          
    0xad  ,0x10,                                                              
            
        //========= ABS
    0xae  ,0x18, 
    0xaf  ,0x74, 
    0xb0  ,0xe0,      
    0xb1  ,0x20, 
    0xb2  ,0x6c, 
    0xb3  ,0x40, 
    0xb4  ,0x04, 
            
        //========= AWB 
    0xbb  ,0x42, 
    0xbc  ,0x60, 
    0xbd  ,0x50, 
    0xbe  ,0x50, 
        
    0xbf  ,0x0c, 
    0xc0  ,0x06, 
    0xc1  ,0x70, 
    0xc2  ,0xf1,  //f4
    0xc3  ,0x40, 
    0xc4  ,0x20, //18
    0xc5  ,0x33, //33
    0xc6  ,0x1d,   
    
    0xca  ,0x70,// 70  
    0xcb  ,0x70, // 70
    0xcc  ,0x78, // 78
        
    0xcd  ,0x80, //R_ratio                                   
    0xce  ,0x80, //G_ratio  , cold_white white                                 
    0xcf  ,0x80, //B_ratio      
        
        //=========  aecT  
    0x20  ,0x02, 
    0x21  ,0xc0, 
    0x22  ,0x60,    
    0x23  ,0x88, 
    0x24  ,0x96, 
    0x25  ,0x30, 
    0x26  ,0xd0, 
    0x27  ,0x00, 
    
    
    0x28  ,0x02, //AEC_exp_level_1bit11to8   
    0x29  ,0x58, //AEC_exp_level_1bit7to0     
    0x2a  ,0x03, //AEC_exp_level_2bit11to8   
    0x2b  ,0x84, //AEC_exp_level_2bit7to0            
    0x2c  ,0x04, //AEC_exp_level_3bit11to8   659 - 8FPS,  8ca - 6FPS  //     
    0x2d  ,0xb0, //AEC_exp_level_3bit7to0            
    0x2e  ,0x0a, //AEC_exp_level_4bit11to8   4FPS 
    0x2f  ,0x8c, //AEC_exp_level_4bit7to0    
    
        
    0x30  ,0x20,                          
    0x31  ,0x00,                       
    0x32  ,0x1c, 
    0x33  ,0x90,              
    0x34  ,0x10,    
        
    0xd0  ,0x34, 
                   
    0xd1  ,0x60, //AEC_target_Y                        
    0xd2  ,0xf2,      
    0xd4  ,0x96, 
    0xd5  ,0x10, 
    0xd6  ,0x96, //antiflicker_step                        
    0xd7  ,0x10, //AEC_exp_time_min                
    0xd8  ,0x02, 
                   
    0xdd  ,0x12, 
                                                                    
        //========= measure window                                      
    0xe0  ,0x03,                         
    0xe1  ,0x02,                             
    0xe2  ,0x27,                                 
    0xe3  ,0x1e,                 
    0xe8  ,0x3b,                     
    0xe9  ,0x6e,                         
    0xea  ,0x2c,                     
    0xeb  ,0x50,                     
    0xec  ,0x73,         
        
        //========= close_frame                                                 
    0xed  ,0x00, //close_frame_num1 ,can be use to reduce FPS                
    0xee  ,0x00, //close_frame_num2  
    0xef  ,0x00, //close_frame_num
        
        // page1
    0xf0  ,0x01, //select page1 
        
    0x00  ,0x20,                              
    0x01  ,0x20,                              
    0x02  ,0x20,                                    
    0x03  ,0x20,                            
    0x04  ,0x78, 
    0x05  ,0x78,                     
    0x06  ,0x78,                                  
    0x07  ,0x78,                                     
        
        
        
    0x10  ,0x04,                          
    0x11  ,0x04,                              
    0x12  ,0x04,                          
    0x13  ,0x04,                              
    0x14  ,0x01,                              
    0x15  ,0x01,                              
    0x16  ,0x01,                         
    0x17  ,0x01,                         
              
                                                         
    0x20  ,0x00,                      
    0x21  ,0x00,                      
    0x22  ,0x00,                          
    0x23  ,0x00,                          
    0x24  ,0x00,                      
    0x25  ,0x00,                          
    0x26  ,0x00,                      
    0x27  ,0x00,                          
        
    0x40  ,0x11, 
        
        //=============================lscP 
    0x45  ,0x06,     
    0x46  ,0x06,             
    0x47  ,0x05, 
        
    0x48  ,0x04,    
    0x49  ,0x03,         
    0x4a  ,0x03, 
        
    
    0x62  ,0xd8, 
    0x63  ,0x24, 
    0x64  ,0x24, 
    0x65  ,0x24, 
    0x66  ,0xd8, 
    0x67  ,0x24,
        
    0x5a  ,0x00, 
    0x5b  ,0x00, 
    0x5c  ,0x00, 
    0x5d  ,0x00, 
    0x5e  ,0x00, 
    0x5f  ,0x00, 
        
        
        //============================= ccP 
        
    0x69  ,0x03, //cc_mode
              
        //CC_G
    0x70  ,0x5d, 
    0x71  ,0xed, 
    0x72  ,0xff, 
    0x73  ,0xe5, 
    0x74  ,0x5f, 
    0x75  ,0xe6, 
        
          //CC_B
    0x76  ,0x41, 
    0x77  ,0xef, 
    0x78  ,0xff, 
    0x79  ,0xff, 
    0x7a  ,0x5f, 
    0x7b  ,0xfa,     
        
        
        //============================= AGP
        
    0x7e  ,0x00,  
    0x7f  ,0x00,  
    0x80  ,0xc8,  
    0x81  ,0x06,  
    0x82  ,0x08,  
        
    0x83  ,0x23,  
    0x84  ,0x38,  
    0x85  ,0x4F,  
    0x86  ,0x61,  
    0x87  ,0x72,  
    0x88  ,0x80,  
    0x89  ,0x8D,  
    0x8a  ,0xA2,  
    0x8b  ,0xB2,  
    0x8c  ,0xC0,  
    0x8d  ,0xCA,  
    0x8e  ,0xD3,  
    0x8f  ,0xDB,  
    0x90  ,0xE2,  
    0x91  ,0xED,  
    0x92  ,0xF6,  
    0x93  ,0xFD,  
        
        //about gamma1 is hex r oct
    0x94  ,0x04,  
    0x95  ,0x0E,  
    0x96  ,0x1B,  
    0x97  ,0x28,  
    0x98  ,0x35,  
    0x99  ,0x41,  
    0x9a  ,0x4E,  
    0x9b  ,0x67,  
    0x9c  ,0x7E,  
    0x9d  ,0x94,  
    0x9e  ,0xA7,  
    0x9f  ,0xBA,  
    0xa0  ,0xC8,  
    0xa1  ,0xD4,  
    0xa2  ,0xE7,  
    0xa3  ,0xF4,  
    0xa4  ,0xFA, 
        
        //========= open functions  
    0xf0  ,0x00, //set back to page0                                                                          
    0x45  ,0x24,
    0x40  ,0x7e, 
    0x41  ,0x2F,
    0x47  ,0x20,    
        
    
        //=========open output
    0x43  ,0x40,
    0x44  ,0xE2,

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
    unsigned int is_start = 0;
    unsigned int value;
    OS_INT_SAVE_AREA                          /*fix the bug of broke down ,20081224. by cccw*/
    /* 
     * for most sensor module, it's not need to call ss_sensor_ioctl(SENSOR_IOSET_CYCLE_STOP, 0);
     * but there is something wrong with ov7680, if not restart dma. 
     */
    if (vi_dev.start_dma != 0)
    {
        //AP_OS_ENTER_CRITICAL();              /*fix the bug of broke down ,20081224. by cccw*/
        is_start = 1;
        ss_sensor_ioctl(VI_IOSET_CYCLE_STOP, 0);
    }   

    if (whitebalance == SS_AUTO_BALANCE) 
    {
        val_0x41 |= 0x04;
    } 
    else 
    {
        val_0x41 &= ~(0x04);
    }

    sensor_i2c_write(0x41, val_0x41);
    switch(whitebalance) 
    {
    case SS_AUTO_BALANCE:       /* auto */
        //sensor_i2c_write(0x41, value);
//        sensor_i2c_write(0x41, 0x2f);
        sensor_i2c_write(0xc7, 0x40);
        sensor_i2c_write(0xc8, 0x40);
        sensor_i2c_write(0xc9, 0x40);
        break;
    case SS_INDOOR_BALANCE:     /* office */ //ÈÕ¹âµÆ
//        sensor_i2c_write(0x41, 0x2b);
        sensor_i2c_write(0xc7,0x40); 
        sensor_i2c_write(0xc8,0x42); 
        sensor_i2c_write(0xc9,0x44);
        break;
    case SS_OUTDOOR_BALANCE:        /* sunny */
//        sensor_i2c_write(0x41, 0x2b);
        sensor_i2c_write(0xc7,0x50);  //60 50
        sensor_i2c_write(0xc8,0x45);  //40 45
        sensor_i2c_write(0xc9,0x40);  //50
        break;
    case SS_FLUORE_BALANCE:     /* home */
//        sensor_i2c_write(0x41, 0x2b);
        sensor_i2c_write(0xc7,0x50);
        sensor_i2c_write(0xc8,0x40);
        sensor_i2c_write(0xc9,0x70);
        break;
    default:
        break;
    }

    vi_effect.ss_white_balance = whitebalance;
    if (is_start != 0)
    {
        ss_sensor_ioctl(VI_IOSET_CYCLE_START, 0);
            //AP_OS_EXIT_CRITICAL();              /*fix the bug of broke down ,20081224. by cccw*/
    }
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
    unsigned int is_start = 0;
    OS_INT_SAVE_AREA                          /*fix the bug of broke down ,20081224. by cccw*/
    /* 
     * for most sensor module, it's not need to call ss_sensor_ioctl(SENSOR_IOSET_CYCLE_STOP, 0);
     * but there is something wrong with ov7680, if not restart dma. 
     */
    if (vi_dev.start_dma != 0)
    {
        //AP_OS_ENTER_CRITICAL();              /*fix the bug of broke down ,20081224. by cccw*/
        is_start = 1;
        ss_sensor_ioctl(VI_IOSET_CYCLE_STOP, 0);
    }
    switch(brightness) 
    {
    case 0:
        sensor_i2c_write(0x7a, 0xe0);
        sensor_i2c_write(0xd1, 0x38);
        break;      
    case 1:
        sensor_i2c_write(0x7a, 0xf0);
        sensor_i2c_write(0xd1, 0x40);
        break;              
    case 2:
        sensor_i2c_write(0x7a, 0x00);
        sensor_i2c_write(0xd1, 0x50);
        break;              
    case 3:
        sensor_i2c_write(0x7a, 0x20);  // 00
        sensor_i2c_write(0xd1, 0x60); // 50
        break;              
    case 4:
        sensor_i2c_write(0x7a, 0x30);
        sensor_i2c_write(0xd1, 0x68);
        break;              
    case 5:
        sensor_i2c_write(0x7a, 0x40);
        sensor_i2c_write(0xd1, 0x70);
        break;              
    case 6:
        sensor_i2c_write(0x7a, 0x50);
        sensor_i2c_write(0xd1, 0x80);
        break;              
    default:
        return -1;
    }

    vi_effect.ss_brightness = brightness;
    if (is_start != 0)
    {
        mdelay(20);
        ss_sensor_ioctl(VI_IOSET_CYCLE_START, 0);
            //AP_OS_EXIT_CRITICAL();              /*fix the bug of broke down ,20081224. by cccw*/
    }
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
    unsigned int is_start = 0;
    OS_INT_SAVE_AREA                          /*fix the bug of broke down ,20081224. by cccw*/
    /* 
     * for most sensor module, it's not need to call ss_sensor_ioctl(SENSOR_IOSET_CYCLE_STOP, 0);
     * but there is something wrong with ov7680, if not restart dma. 
     */
    if (vi_dev.start_dma != 0)
    {
        //AP_OS_ENTER_CRITICAL();              /*fix the bug of broke down ,20081224. by cccw*/
        is_start = 1;
        ss_sensor_ioctl(VI_IOSET_CYCLE_STOP, 0);
    }

    switch (type) 
    {
    case SS_AUTO_EXPOSURE:
        
//      sensor_i2c_write(   0x41  ,0x2f);
        val_0x41 |=0x20;
        sensor_i2c_write(   0x21  ,0xc0);
        sensor_i2c_write(0xdd, 0x22);
        break;
    case SS_INDOOR_EXPOSURE:/*  night mode */
                    
//      sensor_i2c_write(   0x41  ,0x0f);
        val_0x41 &= (~0x20);                    
        sensor_i2c_write(   0xb0  ,0x10);
        sensor_i2c_write(   0x21  ,0xf0);
        sensor_i2c_write(0xdd, 0x32);
        break;
    case SS_OUTDOOR_EXPOSURE:
//      sensor_i2c_write(   0x41  ,0x2f);
        val_0x41 |=0x20;
        sensor_i2c_write(   0x21  ,0x80);
        sensor_i2c_write(0xdd, 0x12);
        break;
    default:
        return -1;
    }
    sensor_i2c_write(0x41, val_0x41);
    if (is_start != 0)
    {
        mdelay(30);
        ss_sensor_ioctl(VI_IOSET_CYCLE_START, 0);
            //AP_OS_EXIT_CRITICAL();              /*fix the bug of broke down ,20081224. by cccw*/
    }

    vi_effect.ss_expose_mode = type;
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
    unsigned int is_start = 0;
    
    OS_INT_SAVE_AREA                          /*fix the bug of broke down ,20081224. by cccw*/

    /* 
     * for most sensor module, it's not need to call ss_sensor_ioctl(SENSOR_IOSET_CYCLE_STOP, 0);
     * but there is something wrong with ov7680, if not restart dma. 
     */
    if (vi_dev.start_dma != 0)
    {
        //AP_OS_ENTER_CRITICAL();              /*fix the bug of broke down ,20081224. by cccw*/
        
        is_start = 1;
        ss_sensor_ioctl(VI_IOSET_CYCLE_STOP, 0);
    }
    val_0x41= sensor_i2c_read(0x41);
    switch(type) 
    {
    case SS_NONE_SPECIAL:       /* normal */
//                          sensor_i2c_write(0x41,0x2f);            
        val_0x41&= ~0x40;
        sensor_i2c_write(0x40,0x7e);
        sensor_i2c_write(0x42,0x10);
        sensor_i2c_write(0x47,0x20);
        sensor_i2c_write(0x48,0xc3);
        sensor_i2c_write(0x8a,0x50);//60
        sensor_i2c_write(0x8b,0x50);
        sensor_i2c_write(0x8c,0x07);
        sensor_i2c_write(0x50,0x0c);
        sensor_i2c_write(0x77,0x80);
        sensor_i2c_write(0xa1,0x40);
        sensor_i2c_write(0x7a,0x20);
        sensor_i2c_write(0x78,0x00);
        sensor_i2c_write(0x79,0x00);
        sensor_i2c_write(0x7b,0x40);
        sensor_i2c_write(0x7c,0x00);
        break;
    case SS_WHITE_AND_BLACK:    /* black and white */
//                          sensor_i2c_write(0x41,0x2f);            
        val_0x41&= ~0x40;
        sensor_i2c_write(0x40,0x7e);
        sensor_i2c_write(0x42,0x10);
        sensor_i2c_write(0x47,0x30);
        sensor_i2c_write(0x48,0xc3);
        sensor_i2c_write(0x8a,0x60);
        sensor_i2c_write(0x8b,0x60);
        sensor_i2c_write(0x8c,0x07);
        sensor_i2c_write(0x50,0x0c);
        sensor_i2c_write(0x77,0x80);
        sensor_i2c_write(0xa1,0x40);
        sensor_i2c_write(0x7a,0x20);
        sensor_i2c_write(0x78,0x00);
        sensor_i2c_write(0x79,0x00);
        sensor_i2c_write(0x7b,0x40);
        sensor_i2c_write(0x7c,0x00);    
        break;
    case SS_SEPIA:      /* Antique */
//                          sensor_i2c_write(0x41,0x2f);            
        val_0x41&= ~0x40;
        sensor_i2c_write(0x40,0x7e);
        sensor_i2c_write(0x42,0x10);
        sensor_i2c_write(0x47,0x30);
        sensor_i2c_write(0x48,0xc3);
        sensor_i2c_write(0x8a,0x60);
        sensor_i2c_write(0x8b,0x60);
        sensor_i2c_write(0x8c,0x07);
        sensor_i2c_write(0x50,0x0c);
        sensor_i2c_write(0x77,0x80);
        sensor_i2c_write(0xa1,0x40);
        sensor_i2c_write(0x7a,0x20);
        sensor_i2c_write(0x78,0xc0);
        sensor_i2c_write(0x79,0x20);
        sensor_i2c_write(0x7b,0x40);
        sensor_i2c_write(0x7c,0x00);    
        break;
    case SS_SOLARIZE:       /* negative */
        //CAM_EFFECT_ENC_SEPIABLUE:
//                        sensor_i2c_write(0x41,0x6f);          
        val_0x41 |= 0x40;
        sensor_i2c_write(0x40,0x7e);
        sensor_i2c_write(0x42,0x10);
        sensor_i2c_write(0x47,0x20);
        sensor_i2c_write(0x48,0xc3);
        sensor_i2c_write(0x8a,0x60);
        sensor_i2c_write(0x8b,0x60);
        sensor_i2c_write(0x8c,0x07);
        sensor_i2c_write(0x50,0x0c);
        
        sensor_i2c_write(0x77,0x80);
        sensor_i2c_write(0xa1,0x40);
        sensor_i2c_write(0x7a,0x20);
        sensor_i2c_write(0x78,0x00);
        sensor_i2c_write(0x79,0x00);
        sensor_i2c_write(0x7b,0x40);
        sensor_i2c_write(0x7c,0x00);    

        break;  

    default:
        return -1;
    }
    sensor_i2c_write(0x41,val_0x41);   
          
    if (is_start != 0)
    {
        ss_sensor_ioctl(VI_IOSET_CYCLE_START, 0);
//        AP_OS_EXIT_CRITICAL();              /*fix the bug of broke down ,20081224. by cccw*/

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
        sensor_i2c_write(0x44, 0xe2);
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
        sensor_i2c_write(0x44, 0xe0);
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
        mdelay(100);        
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
    unsigned int val;

    /* 0x0000-Cmos sensor controllor. cmos sensor interface disable. */
    act_writel(0x00000012, M_CSIFIFOCTL);   //fifo control (sp bus)
    //act_writel(0x00000052, M_CSIFIFOCTL); //fifo control (ahb bus)
    if (ic_type == 0x63) 
    {
        
        act_writel(0x0000000c, M_BT_MODESEL);
    
        sensor_i2c_write(   0xf0,  0x00);
        sensor_i2c_write(   0x0e , 0x0a); //row even skip
        sensor_i2c_write(   0x43 , 0xc0); //more boundary mode opclk output enable
        //sensor_i2c_write( 0x44 , 0xe2); // mtk is e0
        //sensor_i2c_write( 0x45,  0x2b); //col subsample  
        sensor_i2c_write(   0x45,  0x28); //col subsample  
        sensor_i2c_write(   0x4e,  0x33); //  32 opclk gate in subsample  // mtk is 33
    
        sensor_i2c_write(   0x01,  0xd1);
        sensor_i2c_write(   0x02,  0x82);
        sensor_i2c_write(   0x10,  0x00);
        sensor_i2c_write(   0xd6,  0xce);
                                
        sensor_i2c_write(   0x28,  0x02); //AEC_exp_level_1bit11to8   // 33.3fps
        sensor_i2c_write(   0x29,  0x6a); //AEC_exp_level_1bit7to0 
        sensor_i2c_write(   0x2a,  0x04); //AEC_exp_level_2bit11to8   // 20fps
        sensor_i2c_write(   0x2b,  0x06); //AEC_exp_level_2bit7to0  
        sensor_i2c_write(   0x2c,  0x06); //AEC_exp_level_3bit11to8    // 12.5fps
        sensor_i2c_write(   0x2d,  0x70); //AEC_exp_level_3bit7to0           
        sensor_i2c_write(   0x2e,  0x0c); //AEC_exp_level_4bit11to8   // 6.25fps
        sensor_i2c_write(   0x2f,  0xe0); //AEC_exp_level_4bit7to0   
                                
        sensor_i2c_write(   0xe1,  0x01); //big_win_y0                                                     
        sensor_i2c_write(   0xe3,  0x0f); //432, big_win_y1    , height                                 
        sensor_i2c_write(   0xea,  0x16); //small_win_height1                        
        sensor_i2c_write(   0xeb,  0x28); //small_win_height2                        
        sensor_i2c_write(   0xec,  0x39); //small_win_heigh3 //only for AWB 
        sensor_i2c_write(   0xae,  0x0c); //black pixel target number
        sensor_i2c_write(   0xc3,  0x20); //number limit
        sensor_i2c_write(   0x74,  0x1e); //lsc_row_center , 0x3c
        sensor_i2c_write(   0x75,  0x52); //lsc_col_center , 0x52
    
        mdelay(20);

        act_writel(0x00000000, M_CSIHSPOS); //+0x000c-Hsync start position in pclk
        act_writel(0x0000027f, M_CSIHEPOS); //+0x0010-Hsync end position in pclk (QVGA 320)
        act_writel(0x00000000, M_CSIVSPOS); //+0x001c-Vsync start position in hsync
        act_writel(0x000000ef, M_CSIVEPOS); //+0x0020-Vsync end position in hsync (QVGA 240)
    } 
    else if (ic_type == 0x96) 
    {
        val = act_readl(M_CSICTL);
        /* vertical sub sample ratio: 1/2. */
        val &= ~(0x3 << 10);
        val |= 0x1 << 10;
        /* horizontal sub sample ratio: 1/2. */
        val &= ~(0x3 << 8);
        val |= 0x1 << 8;
        act_writel(val, M_CSICTL);
    }
    else
    {
        return -1;
    }

    act_writel(0x00000003, M_CSIRQSTA); //+0x0028-Cmos sensor interface IRQ Status
    return 0;
}


static int act_set_dc320x240_camera(void)
{
    unsigned int value;

    /* 0x0000-Cmos sensor controllor. cmos sensor interface disable. */
    act_writel(0x00000012, M_CSIFIFOCTL);   //fifo control (sp bus)
    //act_writel(0x00000052, M_CSIFIFOCTL); //fifo control (ahb bus)

    //act_writel(0x80, M_CSICTL);
    value = act_readl(M_CSICTL);
    value &= ~(0xf << 8);
    value |= 0x1 << 7;
    act_writel(value, M_CSICTL);
    
    mdelay(20);
    
    sensor_i2c_write(   0xf0,  0x00);
    sensor_i2c_write(   0x0e , 0x0a); //row even skip
    sensor_i2c_write(   0x43 , 0xc0); //more boundary mode opclk output enable
    //sensor_i2c_write( 0x44 , 0xe2); // mtk is e0
    //sensor_i2c_write( 0x45,  0x2b); //col subsample  
    sensor_i2c_write(   0x45,  0x28); //col subsample  
    sensor_i2c_write(   0x4e,  0x33); //  32 opclk gate in subsample  // mtk is 33
    
    sensor_i2c_write(   0x01,  0xd1);
    sensor_i2c_write(   0x02,  0x82);
    sensor_i2c_write(   0x10,  0x00);
    sensor_i2c_write(   0xd6,  0xce);
                            
    sensor_i2c_write(   0x28,  0x02); //AEC_exp_level_1bit11to8   // 33.3fps
    sensor_i2c_write(   0x29,  0x6a); //AEC_exp_level_1bit7to0 
    sensor_i2c_write(   0x2a,  0x04); //AEC_exp_level_2bit11to8   // 20fps
    sensor_i2c_write(   0x2b,  0x06); //AEC_exp_level_2bit7to0  
    sensor_i2c_write(   0x2c,  0x06); //AEC_exp_level_3bit11to8    // 12.5fps
    sensor_i2c_write(   0x2d,  0x70); //AEC_exp_level_3bit7to0           
    sensor_i2c_write(   0x2e,  0x0c); //AEC_exp_level_4bit11to8   // 6.25fps
    sensor_i2c_write(   0x2f,  0xe0); //AEC_exp_level_4bit7to0   
                            
    sensor_i2c_write(   0xe1,  0x01); //big_win_y0                                                     
    sensor_i2c_write(   0xe3,  0x0f); //432, big_win_y1    , height                                 
    sensor_i2c_write(   0xea,  0x16); //small_win_height1                        
    sensor_i2c_write(   0xeb,  0x28); //small_win_height2                        
    sensor_i2c_write(   0xec,  0x39); //small_win_heigh3 //only for AWB 
    sensor_i2c_write(   0xae,  0x0c); //black pixel target number
    sensor_i2c_write(   0xc3,  0x20); //number limit
    sensor_i2c_write(   0x74,  0x1e); //lsc_row_center , 0x3c
    sensor_i2c_write(   0x75,  0x52); //lsc_col_center , 0x52

    mdelay(200);

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
    act_writel(0x00000003, M_CSIRQSTA); //+0x0028-Cmos sensor interface IRQ Status
    mdelay(20);
    return 0;
}




static int act_set_dc640x480_resolution(void)
{
    unsigned int val;

    /* 0x0000-Cmos sensor controllor. cmos sensor interface disable. */
    act_writel(0x00000012, M_CSIFIFOCTL);   //fifo control (sp bus)
    //act_writel(0x00000052, M_CSIFIFOCTL); //fifo control (ahb bus)
    
    if (ic_type == 0x63) 
    {   
        act_writel(0x0000000c, M_BT_MODESEL);
        mdelay(2);
        sensor_i2c_write(   0xf0,  0x00);
        sensor_i2c_write(   0x0e,  0x02); //row even skip
        sensor_i2c_write(   0x43,  0x40); //more boundary mode opclk output enable
        //sensor_i2c_write( 0x44,  0xe2); // mtk is e0
    //  sensor_i2c_write(   0x45  0x2b); //col subsample  
    //sensor_i2c_write( 0x45  0x28); //col subsample  
    //  sensor_i2c_write(   0x4e  0x32); //opclk gate in subsample  // mtk is 33
        sensor_i2c_write(0X45, 0X24);   // 25
        sensor_i2c_write(0x4e, 0x23);
        
        sensor_i2c_write(   0x01,  0x6a);
        sensor_i2c_write(   0x02,  0x70);
        sensor_i2c_write(   0x10,  0x00);
        sensor_i2c_write(   0xd6,  0x96);
                                
        sensor_i2c_write(   0x28,  0x02); //AEC_exp_level_1bit11to8   // 33.3fps
        sensor_i2c_write(   0x29,  0x58); //AEC_exp_level_1bit7to0 
        sensor_i2c_write(   0x2a,  0x03); //AEC_exp_level_2bit11to8   // 20fps
        sensor_i2c_write(   0x2b,  0x84); //AEC_exp_level_2bit7to0  
        sensor_i2c_write(   0x2c,  0x04); //AEC_exp_level_3bit11to8    // 12.5fps
        sensor_i2c_write(   0x2d,  0xb0); //AEC_exp_level_3bit7to0           
        sensor_i2c_write(   0x2e,  0x0a); //AEC_exp_level_4bit11to8   // 6.25fps
        sensor_i2c_write(   0x2f,  0x8c); //AEC_exp_level_4bit7to0   
                                
        sensor_i2c_write(   0xe1,  0x02); //big_win_y0                                                     
        sensor_i2c_write(   0xe3,  0x1e); //432, big_win_y1    , height                                 
        sensor_i2c_write(   0xea,  0x2c); //small_win_height1                        
        sensor_i2c_write(   0xeb,  0x50); //small_win_height2                        
        sensor_i2c_write(   0xec,  0x73); //small_win_heigh3 //only for AWB 
        sensor_i2c_write(   0xae,  0x18); //black pixel target number
        sensor_i2c_write(   0xc3,  0x40); //number limit
        sensor_i2c_write(   0x74,  0x3c); //lsc_row_center , 0x3c
        sensor_i2c_write(   0x75,  0x52); //lsc_col_center , 0x52

        act_writel(0x00000000, M_CSIHSPOS); //+0x000c-Hsync start position in pclk
        act_writel(0x000004ff, M_CSIHEPOS); //+0x0010-Hsync end position in pclk (VGA 640)
        act_writel(0x00000000, M_CSIVSPOS); //+0x001c-Vsync start position in hsync
        act_writel(0x000001df, M_CSIVEPOS); //+0x0020-Vsync end position in hsync (VGA 480)
    } 
    else if (ic_type == 0x96) 
    {
        val = act_readl(M_CSICTL);
        /* vertical sub sample ratio: 1/2. */
        val &= ~(0x3 << 10);
        /* horizontal sub sample ratio: 1/2. */
        val &= ~(0x3 << 8);
        act_writel(val, M_CSICTL);

    }
    else 
    {
        return -1;
    }

    act_writel(0x00000003, M_CSIRQSTA); //+0x0028-Cmos sensor interface IRQ Status
    return 0;
}


static int act_set_dc640x480_camera(void)
{
    unsigned int value;

    /* 0x0000-Cmos sensor controllor. cmos sensor interface disable. */
    act_writel(0x00000012, M_CSIFIFOCTL);   //fifo control (sp bus)
    //act_writel(0x00000052, M_CSIFIFOCTL); //fifo control (ahb bus)
    //act_writel(0x80, M_CSICTL);
    value = act_readl(M_CSICTL);
    value &= ~(0xf << 8);
    value |= 0x1 << 7;
    act_writel(value, M_CSICTL);
    
    mdelay(20);
    
    sensor_i2c_write(   0xf0,  0x00);
    sensor_i2c_write(   0x0e,  0x02); //row even skip
    sensor_i2c_write(   0x43,  0x40); //more boundary mode opclk output enable
    
    sensor_i2c_write(0X45, 0X24);   // 25
    sensor_i2c_write(0x4e, 0x23);
    
    sensor_i2c_write(   0x01,  0x6a);
    sensor_i2c_write(   0x02,  0x70);
    sensor_i2c_write(   0x10,  0x00);
    sensor_i2c_write(   0xd6,  0x96);
                            
    sensor_i2c_write(   0x28,  0x02); //AEC_exp_level_1bit11to8   // 33.3fps
    sensor_i2c_write(   0x29,  0x58); //AEC_exp_level_1bit7to0 
    sensor_i2c_write(   0x2a,  0x03); //AEC_exp_level_2bit11to8   // 20fps
    sensor_i2c_write(   0x2b,  0x84); //AEC_exp_level_2bit7to0  
    sensor_i2c_write(   0x2c,  0x04); //AEC_exp_level_3bit11to8    // 12.5fps
    sensor_i2c_write(   0x2d,  0xb0); //AEC_exp_level_3bit7to0           
    sensor_i2c_write(   0x2e,  0x0a); //AEC_exp_level_4bit11to8   // 6.25fps
    sensor_i2c_write(   0x2f,  0x8c); //AEC_exp_level_4bit7to0   
                            
    sensor_i2c_write(   0xe1,  0x02); //big_win_y0                                                     
    sensor_i2c_write(   0xe3,  0x1e); //432, big_win_y1    , height                                 
    sensor_i2c_write(   0xea,  0x2c); //small_win_height1                        
    sensor_i2c_write(   0xeb,  0x50); //small_win_height2                        
    sensor_i2c_write(   0xec,  0x73); //small_win_heigh3 //only for AWB 
    sensor_i2c_write(   0xae,  0x18); //black pixel target number
    sensor_i2c_write(   0xc3,  0x40); //number limit
    sensor_i2c_write(   0x74,  0x3c); //lsc_row_center , 0x3c
    sensor_i2c_write(   0x75,  0x52); //lsc_col_center , 0x52
    
    mdelay(200);    
    
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


    act_writel(0x00000003, M_CSIRQSTA); //+0x0028-Cmos sensor interface IRQ Status
    
    mdelay(20);
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
        if(vi_dev.pix_fmt == VI_PIX_FMT_YUV422)
        {    
            act_set_dc320x240_camera();
            printf("act_set_dc320x240_camera\n");
        }
        else
        {
            act_set_dc320x240_resolution();
        }
        vi_dev.phy_offset = VI_RES_320X240_PHY_ADDR_OFFSET;
        vi_dev.width = 320;
        vi_dev.height = 240;
        break;
    case VI_RES_640X480:
        len = 640 * 480 * 2;
        if(vi_dev.pix_fmt == VI_PIX_FMT_YUV422)
        {    
            act_set_dc640x480_camera();
            os_time_dly(20);    
            printf("act_set_dc640x480_camera\n");
        }
        else
        {
            act_set_dc640x480_resolution();
        }        
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
    unsigned int val;
    int ret = 0;
    sensor_i2cinfo_t sensor_i2cinfo;

    sensor_power_off();
    mdelay(10);
    sensor_power_on();
    mdelay(10);

    /* hardware pin reset. */
#if 0
    val = act_readl(GPIO_BINEN);
    val &= ~(0x1 << 31);
    act_writel(val, GPIO_BINEN);

    val = act_readl(GPIO_BOUTEN);
    val |= (0x1 << 31);
    act_writel(val, GPIO_BOUTEN);

    val = act_readl(GPIO_BDAT);
    val |= (0x1 << 31);
    act_writel(val, GPIO_BDAT);
    mdelay(6);

    val = act_readl(GPIO_BDAT);
    val &= ~(0x1 << 31);
    act_writel(val, GPIO_BDAT);
    mdelay(6);

    val = act_readl(GPIO_BDAT);
    val |= (0x1 << 31);
    act_writel(val, GPIO_BDAT);
    mdelay(6);

#endif

    sensor_i2cinfo.slave_addr = i2c_slave_addr;
    sensor_i2cinfo.reg_addr = 0x13;
    sensor_i2cinfo.reg_data = 0x1;
    sensor_i2c_write(sensor_i2cinfo.reg_addr, sensor_i2cinfo.reg_data);
    mdelay(3);
    sensor_i2cinfo.reg_data = 0x0;
    sensor_i2c_write(sensor_i2cinfo.reg_addr, sensor_i2cinfo.reg_data);
    mdelay(3);
    sensor_i2cinfo.reg_addr = 0xf1;
    sensor_i2cinfo.reg_data = 0x00;
    sensor_i2c_write(sensor_i2cinfo.reg_addr, sensor_i2cinfo.reg_data);
    mdelay(3);


#if 1
    for (i=0; i < sizeof(sensor_i2cdata); i+=2) 
    {
        sensor_i2cinfo.reg_addr = (unsigned int)sensor_i2cdata[i];
        sensor_i2cinfo.reg_data = (unsigned int)sensor_i2cdata[i+1];
        ret = sensor_i2c_write(sensor_i2cinfo.reg_addr, sensor_i2cinfo.reg_data);
        if (ret != 0)
        {
            return -1;
        }
        //mdelay(4);
    }
#endif


    if (ic_type == 0x63) 
    {
////        mdelay(4);
//        sensor_i2c_write(0X45, 0X48);
//        sensor_i2c_write(0x4e, 0x33);
        
        
        sensor_i2c_write(   0xf0,  0x00);
        sensor_i2c_write(   0x0e , 0x0a); //row even skip
        sensor_i2c_write(   0x43 , 0xc0); //more boundary mode opclk output enable
        //sensor_i2c_write( 0x44 , 0xe2); // mtk is e0
        //sensor_i2c_write( 0x45 , 0x2b); //col subsample  
        sensor_i2c_write(   0x45,  0x28); //col subsample  
        sensor_i2c_write(   0x4e,  0x33); //  32 opclk gate in subsample  // mtk is 33
    
        sensor_i2c_write(   0x01,  0xd1);
        sensor_i2c_write(   0x02,  0x82);
        sensor_i2c_write(   0x10,  0x00);
        sensor_i2c_write(   0xd6,  0xce);
                                
        sensor_i2c_write(   0x28,  0x02); //AEC_exp_level_1bit11to8   // 33.3fps
        sensor_i2c_write(   0x29,  0x6a); //AEC_exp_level_1bit7to0 
        sensor_i2c_write(   0x2a,  0x04); //AEC_exp_level_2bit11to8   // 20fps
        sensor_i2c_write(   0x2b,  0x06); //AEC_exp_level_2bit7to0  
        sensor_i2c_write(   0x2c,  0x06); //AEC_exp_level_3bit11to8    // 12.5fps
        sensor_i2c_write(   0x2d,  0x70); //AEC_exp_level_3bit7to0           
        sensor_i2c_write(   0x2e,  0x0c); //AEC_exp_level_4bit11to8   // 6.25fps
        sensor_i2c_write(   0x2f,  0xe0); //AEC_exp_level_4bit7to0   
                                
        sensor_i2c_write(   0xe1,  0x01); //big_win_y0                                                     
        sensor_i2c_write(   0xe3,  0x0f); //432, big_win_y1    , height                                 
        sensor_i2c_write(   0xea,  0x16); //small_win_height1                        
        sensor_i2c_write(   0xeb,  0x28); //small_win_height2                        
        sensor_i2c_write(   0xec,  0x39); //small_win_heigh3 //only for AWB 
        sensor_i2c_write(   0xae,  0x0c); //black pixel target number
        sensor_i2c_write(   0xc3,  0x20); //number limit
        sensor_i2c_write(   0x74,  0x1e); //lsc_row_center , 0x3c
        sensor_i2c_write(   0x75,  0x52); //lsc_col_center , 0x52
    
        act_writel(0x00000000, M_CSIHSPOS); //+0x000c-Hsync start position in pclk
        act_writel(0x0000027f, M_CSIHEPOS); //+0x0010-Hsync end position in pclk (QVGA 320)
        act_writel(0x00000000, M_CSIVSPOS); //+0x001c-Vsync start position in hsync
        act_writel(0x000000ef, M_CSIVEPOS); //+0x0020-Vsync end position in hsync (QVGA 240)
    } 
    else if (ic_type == 0x96) 
    {
        act_writel(0x508, M_CSICTL);
        val = act_readl(M_CSIHSPOS);
        val &= ~(0x7ff);
        val |= 0x0;
        act_writel(val, M_CSIHSPOS);  //+0x000c-Hsync start position in pclk

        val = act_readl(M_CSIHEPOS);
        val &= ~(0x1fff << 12);
        val |= 0x500 << 12;
        act_writel(val, M_CSIHEPOS);  //+0x000c-Hsync start position in pclk

        val = act_readl(M_CSIVSPOS);
        val &= ~(0x7ff);
        val |= 0x0;
        act_writel(val, M_CSIVSPOS);  //+0x000c-Hsync start position in pclk

        val = act_readl(M_CSIVEPOS);
        val &= ~(0x1fff << 12);
        val |= 0x1e0 << 12;
        act_writel(val, M_CSIVEPOS);  //+0x000c-Hsync start position in pclk
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
    sensor_i2c_write(0x13,0x01);
    mdelay(3);     
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

#pragma __PRQA_IGNORE_START__
void modify_data(unsigned long addr, unsigned int resolution, unsigned int app_type) //ÐÞ¸´0307±ßÑØÈ±ÏÝ
{
    if (app_type == VI_PC_CAMERA_MODE)
    {
        //        stuff_point_yuyv((unsigned char *)addr, 640 ,480);
        if (resolution == VI_RES_320X240)
        {
            stuff_point_yuyv((unsigned char *) addr, 320, 240);
        }
        else if (resolution == VI_RES_640X480)
        {
            stuff_point_yuyv((unsigned char *) addr, 640, 480);
        }
        else
        {
            return;
        }
        return;
    }
    else
    {
        if (reload_mode == 0)
        {
            if (resolution == VI_RES_320X240)
            {
                stuff_point_4y4u((unsigned char *) addr, 320, 240);
            }
            else if (resolution == VI_RES_640X480)
            {
                stuff_point_4y4u((unsigned char *) addr, 640, 480);
            }
            else
            {
                return;
            }
        }
        else
        {
            if (resolution == VI_RES_320X240)
            {
                stuff_point_yuv420((unsigned char *) addr, 320, 240);
            }
            else if (resolution == VI_RES_640X480)
            {
                stuff_point_yuv420((unsigned char *) addr, 640, 480);
            }
            else
            {
                return;
            }
        }
        return;
    }
}

/*
 void stuff_point(unsigned char *src,int width,int height)
 {
 unsigned int i;
 unsigned char t0,t1,t2,t3;
 unsigned char *psrc = src;

 for (i=0;i<height;i++) {
 t0 = (*(unsigned char*)(src+8)+*(unsigned char*)(src+9))/2;
 t1 = (*(unsigned char*)(src+9)+*(unsigned char*)(src+10))/2;
 t2 = (*(unsigned char*)(src+10)+*(unsigned char*)(src+11))/2;
 t3 = (*(unsigned char*)(src+11)+*(unsigned char*)(src+16))/2;

 *(unsigned char*)(src+0) = *(unsigned char*)(src+8);
 *(unsigned char*)(src+1) = t0;
 *(unsigned char*)(src+2) = *(unsigned char*)(src+9);
 *(unsigned char*)(src+3) = t1;

 *(unsigned char*)(src+8) = *(unsigned char*)(src+10);
 *(unsigned char*)(src+9) = t2;
 *(unsigned char*)(src+10) = *(unsigned char*)(src+11);
 *(unsigned char*)(src+11) = t3;

 *(unsigned char*)(src+4) = *(unsigned char*)(src+6);
 *(unsigned char*)(src+5) = (*(unsigned char*)(src+7)+*(unsigned char*)(src+6))/2;
 *(unsigned char*)(src+6) = *(unsigned char*)(src+7);
 *(unsigned char*)(src+7) = (*(unsigned char*)(src+7)+*(unsigned char*)(src+20))/2;

 *(unsigned char*)(src+12) = *(unsigned char*)(src+14);
 *(unsigned char*)(src+13) = (*(unsigned char*)(src+14)+*(unsigned char*)(src+15))/2;
 *(unsigned char*)(src+14) = *(unsigned char*)(src+15);
 *(unsigned char*)(src+15) = (*(unsigned char*)(src+15)+*(unsigned char*)(src+28))/2;
 src += (width*2);
 }
 }
 */
void stuff_point_4y4u(unsigned char *src, int width, int height)
{
    unsigned int i;
    unsigned char t0, t1, t2, t3, t4, t5, t6, t7, t8;
    unsigned char d0, d1, d2, d3;
    unsigned char *psrc = src;
    unsigned int stride = width * 2;

    for (i = 0; i < height; i++)
    {
        t0 = (*(unsigned char*) (src + 8) + *(unsigned char*) (src + 9)) / 2;
        t1 = (*(unsigned char*) (src + 9) + *(unsigned char*) (src + 10)) / 2;
        t2 = (*(unsigned char*) (src + 10) + *(unsigned char*) (src + 11)) / 2;
        t3 = (*(unsigned char*) (src + 11) + *(unsigned char*) (src + 16)) / 2;

        *(unsigned char*) (src + 0) = *(unsigned char*) (src + 8);
        *(unsigned char*) (src + 1) = t0;
        *(unsigned char*) (src + 2) = *(unsigned char*) (src + 9);
        *(unsigned char*) (src + 3) = t1;

        *(unsigned char*) (src + 8) = *(unsigned char*) (src + 10);
        *(unsigned char*) (src + 9) = t2;
        *(unsigned char*) (src + 10) = *(unsigned char*) (src + 11);
        *(unsigned char*) (src + 11) = t3;

        *(unsigned char*) (src + 4) = *(unsigned char*) (src + 6);
        *(unsigned char*) (src + 5) = (*(unsigned char*) (src + 7) + *(unsigned char*) (src + 6)) / 2;
        *(unsigned char*) (src + 6) = *(unsigned char*) (src + 7);
        *(unsigned char*) (src + 7) = (*(unsigned char*) (src + 7) + *(unsigned char*) (src + 20)) / 2;

        *(unsigned char*) (src + 12) = *(unsigned char*) (src + 14);
        *(unsigned char*) (src + 13) = (*(unsigned char*) (src + 14) + *(unsigned char*) (src + 15)) / 2;
        *(unsigned char*) (src + 14) = *(unsigned char*) (src + 15);
        *(unsigned char*) (src + 15) = (*(unsigned char*) (src + 15) + *(unsigned char*) (src + 28)) / 2;

        t0 = (*(unsigned char*) (src + stride - 8) + *(unsigned char*) (src + stride - 13)) / 2;
        t1 = (*(unsigned char*) (src + stride - 8) + *(unsigned char*) (src + stride - 7)) / 2;

        *(unsigned char*) (src + stride - 8) = t0;
        *(unsigned char*) (src + stride - 7) = *(unsigned char*) (src + stride - 8);
        *(unsigned char*) (src + stride - 6) = t1;
        *(unsigned char*) (src + stride - 5) = *(unsigned char*) (src + stride - 7);

        *(unsigned char*) (src + stride - 10) = (*(unsigned char*) (src + stride - 10) + *(unsigned char*) (src
                + stride - 11)) / 2;
        *(unsigned char*) (src + stride - 9) = *(unsigned char*) (src + stride - 10);
        *(unsigned char*) (src + stride - 2) = (*(unsigned char*) (src + stride - 2) + *(unsigned char*) (src + stride
                - 3)) / 2;
        *(unsigned char*) (src + stride - 1) = *(unsigned char*) (src + stride - 2);

        src += stride;
    }
}

#if 1
void stuff_point_yuyv(unsigned char *src, int width, int height)
{
    unsigned int i;
    unsigned char t0, t1, t2, t3, t4, t5, t6, t7, t8;
    unsigned char d0, d1, d2, d3;
    unsigned char *psrc = src;
    unsigned int stride = width * 2;

    for (i = 0; i < height; i++)
    {
        t0 = (*(unsigned char*) (src + 8) + *(unsigned char*) (src + 10)) / 2;
        t1 = (*(unsigned char*) (src + 12) + *(unsigned char*) (src + 10)) / 2;
        t2 = (*(unsigned char*) (src + 12) + *(unsigned char*) (src + 14)) / 2;
        t3 = (*(unsigned char*) (src + 14) + *(unsigned char*) (src + 16)) / 2;

        *(unsigned char*) (src + 0) = *(unsigned char*) (src + 8);
        *(unsigned char*) (src + 2) = t0;
        *(unsigned char*) (src + 4) = *(unsigned char*) (src + 10);
        *(unsigned char*) (src + 6) = t1;

        *(unsigned char*) (src + 8) = *(unsigned char*) (src + 12);
        *(unsigned char*) (src + 10) = t2;
        *(unsigned char*) (src + 12) = *(unsigned char*) (src + 14);
        *(unsigned char*) (src + 14) = t3;

        *(unsigned char*) (src + 1) = *(unsigned char*) (src + 9);
        *(unsigned char*) (src + 5) = (*(unsigned char*) (src + 13) + *(unsigned char*) (src + 9)) / 2;
        *(unsigned char*) (src + 9) = *(unsigned char*) (src + 13);
        *(unsigned char*) (src + 13) = (*(unsigned char*) (src + 13) + *(unsigned char*) (src + 17)) / 2;

        *(unsigned char*) (src + 3) = *(unsigned char*) (src + 11);
        *(unsigned char*) (src + 7) = (*(unsigned char*) (src + 11) + *(unsigned char*) (src + 15)) / 2;
        *(unsigned char*) (src + 11) = *(unsigned char*) (src + 15);
        *(unsigned char*) (src + 15) = (*(unsigned char*) (src + 15) + *(unsigned char*) (src + 19)) / 2;

        t0 = (*(unsigned char*) (src + stride - 8) + *(unsigned char*) (src + stride - 10)) / 2;
        t1 = (*(unsigned char*) (src + stride - 8) + *(unsigned char*) (src + stride - 6)) / 2;

        *(unsigned char*) (src + stride - 8) = t0;
        *(unsigned char*) (src + stride - 6) = *(unsigned char*) (src + stride - 8);
        *(unsigned char*) (src + stride - 4) = t1;
        *(unsigned char*) (src + stride - 2) = *(unsigned char*) (src + stride - 6);

        *(unsigned char*) (src + stride - 3) = *(unsigned char*) (src + stride - 7);
        *(unsigned char*) (src + stride - 7) = (*(unsigned char*) (src + stride - 7) + *(unsigned char*) (src + stride
                - 11)) / 2;
        *(unsigned char*) (src + stride - 1) = *(unsigned char*) (src + stride - 5);
        *(unsigned char*) (src + stride - 5) = (*(unsigned char*) (src + stride - 5) + *(unsigned char*) (src + stride
                - 9)) / 2;

        src += stride;
    }
}
#endif

void stuff_point_yuv420(unsigned char *src, int width, int height)
{
    unsigned int i;
    unsigned char t0, t1, t2, t3, t4, t5, t6, t7, t8;
    unsigned char d0, d1, d2, d3;
    unsigned int stride = width;
    unsigned int stride2 = width / 2;
    unsigned char *p_Y = src;
    unsigned char *p_Cb = src + width * height;
   // unsigned char *p_Cr = src + width * height + width * height / 4;
   // return 0;
    for (i = 0; i < height; i++)
    {
        t0 = (*(unsigned char*) (p_Y + 4) + *(unsigned char*) (p_Y + 5)) / 2;
        t1 = (*(unsigned char*) (p_Y + 5) + *(unsigned char*) (p_Y + 6)) / 2;
        t2 = (*(unsigned char*) (p_Y + 6) + *(unsigned char*) (p_Y + 7)) / 2;
        t3 = (*(unsigned char*) (p_Y + 7) + *(unsigned char*) (p_Y + 8)) / 2;

        *(unsigned char*) (p_Y + 0) = *(unsigned char*) (p_Y + 4);
        *(unsigned char*) (p_Y + 1) = t0;
        *(unsigned char*) (p_Y + 2) = *(unsigned char*) (p_Y + 5);
        *(unsigned char*) (p_Y + 3) = t1;

        *(unsigned char*) (p_Y + 4) = *(unsigned char*) (p_Y + 6);
        *(unsigned char*) (p_Y + 5) = t2;
        *(unsigned char*) (p_Y + 6) = *(unsigned char*) (p_Y + 7);
        *(unsigned char*) (p_Y + 7) = t3;

        *(unsigned char*) (p_Cb + 0) = *(unsigned char*) (p_Cb + 2);
        *(unsigned char*) (p_Cb + 1) = (*(unsigned char*) (p_Cb + 3) + *(unsigned char*) (p_Cb + 2)) / 2;
        *(unsigned char*) (p_Cb + 2) = *(unsigned char*) (p_Cb + 3);
        *(unsigned char*) (p_Cb + 3) = (*(unsigned char*) (p_Cb + 3) + *(unsigned char*) (p_Cb + 4)) / 2;
//        
//        *(unsigned char*) (p_Cr + 0) = *(unsigned char*) (p_Cr + 2);
//        *(unsigned char*) (p_Cr + 1) = (*(unsigned char*) (p_Cr + 3) + *(unsigned char*) (p_Cr + 2)) / 2;
//        *(unsigned char*) (p_Cr + 2) = *(unsigned char*) (p_Cr + 3);
//        *(unsigned char*) (p_Cr + 3) = (*(unsigned char*) (p_Cr + 3) + *(unsigned char*) (p_Cr + 4)) / 2;
        
        t0 = (*(unsigned char*) (p_Y + stride - 5) + *(unsigned char*) (p_Y + stride - 4)) / 2;
        t1 = (*(unsigned char*) (p_Y + stride - 4) + *(unsigned char*) (p_Y + stride - 3)) / 2;

        *(unsigned char*) (p_Y + stride - 4) = t0;
        *(unsigned char*) (p_Y + stride - 3) = *(unsigned char*) (p_Y + stride - 4);
        *(unsigned char*) (p_Y + stride - 2) = t1;
        *(unsigned char*) (p_Y + stride - 1) = *(unsigned char*) (p_Y + stride - 3);

        *(unsigned char*) (p_Cb + stride2 - 1) = *(unsigned char*) (p_Cb + stride2 - 2);
        *(unsigned char*) (p_Cb + stride2 - 2) = (*(unsigned char*) (p_Cb + stride2 - 2) + *(unsigned char*) (p_Cb
                + stride2 - 3)) / 2;
//        
//        *(unsigned char*) (p_Cr + stride2 - 1) = *(unsigned char*) (p_Cr + stride2 - 2);
//        *(unsigned char*) (p_Cr + stride2 - 2) = (*(unsigned char*) (p_Cr + stride2 - 2) + *(unsigned char*) (p_Cr
//                + stride2 - 3)) / 2;
                
        p_Y += stride;
        p_Cb += stride2;
        //p_Cr += stride2;
    }
}
#pragma __PRQA_IGNORE_END__
