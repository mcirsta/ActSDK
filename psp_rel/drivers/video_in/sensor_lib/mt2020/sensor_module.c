/*
 ********************************************************************************
 *                   213x --- sensor's driver
 *                (c) Copyright 2007 - 2010, Actions Co,Ld.
 *                        All Right Reserved
 *
 * History:
 *      <author>      <time>       <version >    <desc>
 *       houjingkun      2008/05/30     1.0      build file.
 *                                               this file is used for mc2020.
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
/*config the power-down pin active level, if  high level enable power-down mode,define the first*/
#define  PWDN_ACTIVE_HIGH
//#define  PWDN_ACTIVE_LOW

/*config reg CMU_BTCLK, the result M_clk is : 120 / (M_CLK_DIVIDE+1) Mhz*/
//#define  M_CLK_DIVIDE 0x0000000f
#define  M_CLK_DIVIDE 0x00000020    /* fix to 24 MHZ. */

#define PREVIEW_SIZE VI_RES_320X240
/******************************************************************************/
/*!
 * \par  Description:
 *       sensor module in i2c bus's slave_addr.
 *******************************************************************************/
unsigned int i2c_slave_addr = 0x7a;

/* MT2020; 2 million pixel. */
static unsigned short sensor_i2cdata[] =
{ 
    0xfFFF, 1,  
    0x3386, 0x2500,
    
    //[initial]
    0x301A, 0x0ACC,     // RESET_REGISTER
    0x3202, 0x0008,     // STANDBY_CONTROL
    0xFFFF, 10,              
    
    //0x341E, 0x8F09,        //PLL/ Clk_in control: BYPASS PLL = 36617
    //0x341C, 0x0120,        //PLL Control 1 = 288
    //0x341E, 0x8F09,        //PLL/ Clk_in control: PLL ON, bypassed = 36617
    //0XFFFF, 0X0005,                 // More than ample time to allow PLL to lock
    //0x341E, 0x8F08,        //PLL/ Clk_in control: USE PLL = 36616
    //0x3214, 0x06E6,         //default 0x0480  -> make Slope work for 80 MHz Mclk    
    
    0x341E, 0x8F09,     // PLL_CLK_IN_CONTROL
    0x341C, 0x0218,     // PLL_DIVIDERS1
    0xFFFF, 10,                  
    0x341E, 0x8F09,     // PLL_CLK_IN_CONTROL
    0x341E, 0x8F08,     // PLL_CLK_IN_CONTROL
    0x3044, 0x0540,     // DARK_CONTROL
    0x3216, 0x02CF,     // INTERNAL_CLOCK_CONTROL
    0x321C, 0x0402,     // OF_CONTROL_STATUS
    0x3212, 0x0001,     // FACTORY_BYPASS
    0x341E, 0x8F09,     // PLL_CLK_IN_CONTROL
    0x341C, 0x0120,     // PLL_DIVIDERS1
    0x341E, 0x8F09,     // PLL_CLK_IN_CONTROL
    0x341E, 0x8F08,     // PLL_CLK_IN_CONTROL
    0x3044, 0x0540,     // DARK_CONTROL
    0x3216, 0x02CF,     // INTERNAL_CLOCK_CONTROL
    0x321C, 0x0402,     // OF_CONTROL_STATUS
    0x3212, 0x0001,     // FACTORY_BYPASS//供品干扰程序利用之前的07005
    
    ////////////////MZT ADD
    /*
    0x338C, 0xA115,// MCU_ADDRESS [SEQ_LLMODE]
    0x3390, 0x00ED,// MCU_DATA_0
    0x338C, 0xA352,// MCU_ADDRESS [AWB_SATURATION]
    0x3390, 0x00a0,// MCU_DATA_0                         //se du    
    */
    /////////////////
    
    //[CCM]                                                      
    0x338C, 0x2306,     // MCU_ADDRESS [AWB_CCM_L_0]      
    0x3390, 0x0055,     // MCU_DATA_0     
    0x337C, 0x0004,       //cgz oppo 20081021
    0x338C, 0x2308,     // MCU_ADDRESS [AWB_CCM_L_1]      
    0x3390, 0xFEC3,     // MCU_DATA_0                     
    0x338C, 0x230A,     // MCU_ADDRESS [AWB_CCM_L_2]      
    0x3390, 0x0236,     // MCU_DATA_0                     
    0x338C, 0x230C,     // MCU_ADDRESS [AWB_CCM_L_3]      
    0x3390, 0xFE2A,     // MCU_DATA_0                     
    0x338C, 0x230E,     // MCU_ADDRESS [AWB_CCM_L_4]      
    0x3390, 0x0229,     // MCU_DATA_0                     
    0x338C, 0x2310,     // MCU_ADDRESS [AWB_CCM_L_5]      
    0x3390, 0x014A,     // MCU_DATA_0                     
    0x338C, 0x2312,     // MCU_ADDRESS [AWB_CCM_L_6]      
    0x3390, 0xFEB0,     // MCU_DATA_0                     
    0x338C, 0x2314,     // MCU_ADDRESS [AWB_CCM_L_7]      
    0x3390, 0xF867,     // MCU_DATA_0                     
    0x338C, 0x2316,     // MCU_ADDRESS [AWB_CCM_L_8]      
    0x3390, 0x0ACD,     // MCU_DATA_0                     
    0x338C, 0x2318,     // MCU_ADDRESS [AWB_CCM_L_9]      
    0x3390, 0x0024,     // MCU_DATA_0                     
    0x338C, 0x231A,     // MCU_ADDRESS [AWB_CCM_L_10]     
    0x3390, 0x003D,     // MCU_DATA_0                     
    0x338C, 0x231C,     // MCU_ADDRESS [AWB_CCM_RL_0]     
    0x3390, 0x0353,     // MCU_DATA_0                     
    0x338C, 0x231E,     // MCU_ADDRESS [AWB_CCM_RL_1]     
    0x3390, 0xFECA,     // MCU_DATA_0                     
    0x338C, 0x2320,     // MCU_ADDRESS [AWB_CCM_RL_2]     
    0x3390, 0xFDE6,     // MCU_DATA_0                     
    0x338C, 0x2322,     // MCU_ADDRESS [AWB_CCM_RL_3]     
    0x3390, 0x011A,     // MCU_DATA_0                     
    0x338C, 0x2324,     // MCU_ADDRESS [AWB_CCM_RL_4]     
    0x3390, 0x01B2,     // MCU_DATA_0                     
    0x338C, 0x2326,     // MCU_ADDRESS [AWB_CCM_RL_5]     
    0x3390, 0xFCE2,     // MCU_DATA_0                     
    0x338C, 0x2328,     // MCU_ADDRESS [AWB_CCM_RL_6]     
    0x3390, 0x014A,     // MCU_DATA_0                     
    0x338C, 0x232A,     // MCU_ADDRESS [AWB_CCM_RL_7]     
    0x3390, 0x060E,     // MCU_DATA_0                     
    0x338C, 0x232C,     // MCU_ADDRESS [AWB_CCM_RL_8]     
    0x3390, 0xF81D,     // MCU_DATA_0                     
    0x338C, 0x232E,     // MCU_ADDRESS [AWB_CCM_RL_9]     
    0x3390, 0x0010,     // MCU_DATA_0                     
    0x338C, 0x2330,     // MCU_ADDRESS [AWB_CCM_RL_10]    
    0x3390, 0xFFEC,     // MCU_DATA_0                     
    0x338C, 0xA348,     // MCU_ADDRESS [AWB_GAIN_BUFFER_SP
    0x3390, 0x0008,     // MCU_DATA_0                     
    0x338C, 0xA349,     // MCU_ADDRESS [AWB_JUMP_DIVISOR] 
    0x3390, 0x0002,     // MCU_DATA_0                     
    0x338C, 0xA34A,     // MCU_ADDRESS [AWB_GAIN_MIN]     
    0x3390, 0x0059,     // MCU_DATA_0                     
    0x338C, 0xA34B,     // MCU_ADDRESS [AWB_GAIN_MAX]     
    0x3390, 0x00A6,     // MCU_DATA_0                     
    0x338C, 0xA34F,     // MCU_ADDRESS [AWB_CCM_POSITION_M
    0x3390, 0x0000,     // MCU_DATA_0                     
    0x338C, 0xA350,     // MCU_ADDRESS [AWB_CCM_POSITION_M
    0x3390, 0x007F,     // MCU_DATA_0                     
    0x338C, 0xA352,     // MCU_ADDRESS [AWB_SATURATION]   
    0x3390, 0x0050, //0x001A,   // MCU_DATA_0                     
    0x338C, 0xA353,     // MCU_ADDRESS [AWB_MODE]         
    0x3390, 0x0001,     // MCU_DATA_0                     
    0x338C, 0xA35B,     // MCU_ADDRESS [AWB_STEADY_BGAIN_O
    0x3390, 0x0078,     // MCU_DATA_0                     
    0x338C, 0xA35C,     // MCU_ADDRESS [AWB_STEADY_BGAIN_O
    0x3390, 0x0086,     // MCU_DATA_0                     
    0x338C, 0xA35D,     // MCU_ADDRESS [AWB_STEADY_BGAIN_I
    0x3390, 0x007E,     // MCU_DATA_0                     
    0x338C, 0xA35E,     // MCU_ADDRESS [AWB_STEADY_BGAIN_I
    0x3390, 0x0082,     // MCU_DATA_0                     
    0x338C, 0x235F,     // MCU_ADDRESS [AWB_CNT_PXL_TH]   
    0x3390, 0x0040,     // MCU_DATA_0                     
    0x338C, 0xA361,     // MCU_ADDRESS [AWB_TG_MIN0]      
    0x3390, 0x00D7,     // MCU_DATA_0                     
    0x338C, 0xA362,     // MCU_ADDRESS [AWB_TG_MAX0]      
    0x3390, 0x00F6,     // MCU_DATA_0                     
    0x338C, 0xA302,     // MCU_ADDRESS [AWB_WINDOW_POS]   
    0x3390, 0x0000,     // MCU_DATA_0                     
    0x338C, 0xA303,     // MCU_ADDRESS [AWB_WINDOW_SIZE]  
    0x3390, 0x00EF,     // MCU_DATA_0                     
                                            
    0x338C, 0xA364,  // MCU_ADDRESS [AWB_KR_L]        
    0x3390, 0x0098,     // MCU_DATA_0                     
    0x338C, 0xA365,     // MCU_ADDRESS [AWB_KG_L]         
    0x3390, 0x0096,     // MCU_DATA_0                     
    0x338C, 0xA366,     // MCU_ADDRESS [AWB_KB_L]         
    0x3390, 0x0084,     // MCU_DATA_0                     
    0x338C, 0xA367,     // MCU_ADDRESS [AWB_KR_R]         
    0x3390, 0x0087,     // MCU_DATA_0                     
    0x338C, 0xA368,     // MCU_ADDRESS [AWB_KG_R]         
    0x3390, 0x0080,     // MCU_DATA_0                     
    0x338C, 0xA369,     // MCU_ADDRESS [AWB_KB_R]         
    0x3390, 0x0089,     // MCU_DATA_0       
    //   0x338C, 0xA103,     // MCU_ADDRESS [SEQ_CMD]          
    //   0x3390, 0x0006,     // MCU_DATA_0     
    ////     0xfFFF, 50, 
    //     0x338C, 0xA103,   // MCU_ADDRESS [SEQ_CMD]          
    //     0x3390, 0x0005,   // MCU_DATA_0   
    ////       0xfFFF, 50, 
    
    
    //   //[Fix Frame rate]
    //   0x338C, 0xA123,     // MCU_ADDRESS [SEQ_PREVIEW_0_FD]
    //   0x3390, 0x0002,     // MCU_DATA_0
    //   0x338C, 0xA404,     // MCU_ADDRESS [FD_MODE]
    //   0x3390, 0x00C2,     // MCU_DATA_0
    //   0x338C, 0xA130,     // MCU_ADDRESS [SEQ_PREVIEW_2_AE]    // add by sheree 1008
    //   0x3390, 0x0000, // MCU_DATA_0 
    //   0x338C, 0xA103,  // MCU_ADDRESS
    //   0x3390, 0x0005,  // MCU_DATA_0
    
    //[noise reduce setting]
    0x338C, 0xA115,     // MCU_ADDRESS [SEQ_LLMODE]
    0x3390, 0x00ED, //0x00EF,   // MCU_DATA_0
    0x338C, 0xA118,     // MCU_ADDRESS [SEQ_LLSAT1]
    0x3390, 0x0036,     // MCU_DATA_0
    0x338C, 0xA119,     // MCU_ADDRESS [SEQ_LLSAT2]
    0x3390, 0x0003,     // MCU_DATA_0
    0x338C, 0xA11A,     // MCU_ADDRESS [SEQ_LLINTERPTHRESH1]
    0x3390, 0x000A,     // MCU_DATA_0
    0x338C, 0xA11B,     // MCU_ADDRESS [SEQ_LLINTERPTHRESH2]
    0x3390, 0x0020,     // MCU_DATA_0
    0x338C, 0xA11C,     // MCU_ADDRESS [SEQ_LLAPCORR1]
    0x3390, 0x0002,     // MCU_DATA_0
    0x338C, 0xA11D,     // MCU_ADDRESS [SEQ_LLAPCORR2]
    0x3390, 0x0000,     // MCU_DATA_0
    0x338C, 0xA11E,     // MCU_ADDRESS [SEQ_LLAPTHRESH1]
    0x3390, 0x0000,     // MCU_DATA_0
    0x338C, 0xA11F,     // MCU_ADDRESS [SEQ_LLAPTHRESH2]
    0x3390, 0x0004,     // MCU_DATA_0
    0x338C, 0xA13E,     // MCU_ADDRESS [SEQ_NR_TH1_R]
    0x3390, 0x0004,     // MCU_DATA_0
    0x338C, 0xA13F,     // MCU_ADDRESS [SEQ_NR_TH1_G]
    0x3390, 0x000E,     // MCU_DATA_0
    0x338C, 0xA140,     // MCU_ADDRESS [SEQ_NR_TH1_B]
    0x3390, 0x0004,     // MCU_DATA_0
    0x338C, 0xA141,     // MCU_ADDRESS [SEQ_NR_TH1_OL]
    0x3390, 0x0004,     // MCU_DATA_0
    0x338C, 0xA142,     // MCU_ADDRESS [SEQ_NR_TH2_R]
    0x3390, 0x0032,     // MCU_DATA_0
    0x338C, 0xA143,     // MCU_ADDRESS [SEQ_NR_TH2_G]
    0x3390, 0x000F,     // MCU_DATA_0
    0x338C, 0xA144,     // MCU_ADDRESS [SEQ_NR_TH2_B]
    0x3390, 0x0032,     // MCU_DATA_0
    0x338C, 0xA145,     // MCU_ADDRESS [SEQ_NR_TH2_OL]
    0x3390, 0x0032,     // MCU_DATA_0
    0x338C, 0xA146,     // MCU_ADDRESS [SEQ_NR_GAINTH1]
    0x3390, 0x0005,     // MCU_DATA_0
    0x338C, 0xA147,     // MCU_ADDRESS [SEQ_NR_GAINTH2]
    0x3390, 0x003A,     // MCU_DATA_0
    0x338C, 0xA14F,     // MCU_ADDRESS [SEQ_CLUSTERDC_TH]
    0x3390, 0x000D,     // MCU_DATA_0
    //   0x338C, 0xA103,        //Refresh Sequencer Mode
    //   0x3390, 0x06  ,      //      = 6
    ////     0xfFFF, 50, 
    //   0x338C, 0xA103,
    //   0x3390, 0x0005,
    //   0xfFFF, 50, 
    //  [test-5_max index change_target change]
    0x338C, 0xA118,    // MCU_ADDRESS [SEQ_LLSAT1]
    0x3390, 0x0036,    // MCU_DATA_0
    0x338C, 0xA119,    // MCU_ADDRESS [SEQ_LLSAT2]
    0x3390, 0x0003,    // MCU_DATA_0
    0x338C, 0xA206,    // MCU_ADDRESS [AE_TARGET]
    //0x3390, 0x0050,  // MCU_DATA_0
    0x3390, 0x0031,//0x0031,   // MCU_DATA_0
    0x338C, 0xA207,    // MCU_ADDRESS [AE_GATE]
    //0x3390, 0x0005,  // MCU_DATA_0
    0x3390, 0x000B,    // MCU_DATA_0
    0x338C, 0xA20C,    // MCU_ADDRESS [AE_MAX_INDEX]
    0x3390, 0x000A,    // MCU_DATA_0
    0x338C, 0xA109,    // MCU_ADDRESS [SEQ_AE_FASTBUFF]
    0x3390, 0x0020,    //0x0024 cgz oppo 2008-1020// MCU_DATA_0
    0x338C, 0xA76D,    // MCU_ADDRESS [MODE_GAM_CONT_A]
    0x3390, 0x0043,    // MCU_DATA_0   //liniinchao modify 0x0003 to 0x0043
    0x338C, 0xA76D,    // MCU_ADDRESS [MODE_GAM_CONT_B]
    0x3390, 0x0002,    // MCU_DATA_0   //add by liniinchao defalutr=0x0042
    0x338C, 0xA76F,    // MCU_ADDRESS [MODE_GAM_TABLE_A_0]
    0x3390, 0x0000,    // MCU_DATA_0
    0x338C, 0xA770,    // MCU_ADDRESS [MODE_GAM_TABLE_A_1]
    0x3390, 0x0007,    // MCU_DATA_0
    0x338C, 0xA771,    // MCU_ADDRESS [MODE_GAM_TABLE_A_2]
    0x3390, 0x0017,    // MCU_DATA_0
    0x338C, 0xA772,    // MCU_ADDRESS [MODE_GAM_TABLE_A_3]
    0x3390, 0x003B,    // MCU_DATA_0
    0x338C, 0xA773,    // MCU_ADDRESS [MODE_GAM_TABLE_A_4]
    0x3390, 0x0060,    // MCU_DATA_0
    0x338C, 0xA774,    // MCU_ADDRESS [MODE_GAM_TABLE_A_5]
    0x3390, 0x007A,    // MCU_DATA_0
    0x338C, 0xA775,    // MCU_ADDRESS [MODE_GAM_TABLE_A_6]
    0x3390, 0x008F,    // MCU_DATA_0
    0x338C, 0xA776,    // MCU_ADDRESS [MODE_GAM_TABLE_A_7]
    0x3390, 0x00A0,    // MCU_DATA_0
    0x338C, 0xA777,    // MCU_ADDRESS [MODE_GAM_TABLE_A_8]
    0x3390, 0x00AE,    // MCU_DATA_0
    0x338C, 0xA778,    // MCU_ADDRESS [MODE_GAM_TABLE_A_9]
    0x3390, 0x00BA,    // MCU_DATA_0
    0x338C, 0xA779,    // MCU_ADDRESS [MODE_GAM_TABLE_A_10]
    0x3390, 0x00C5,    // MCU_DATA_0
    0x338C, 0xA77A,    // MCU_ADDRESS [MODE_GAM_TABLE_A_11]
    0x3390, 0x00CE,    // MCU_DATA_0
    0x338C, 0xA77B,    // MCU_ADDRESS [MODE_GAM_TABLE_A_12]
    0x3390, 0x00D7,    // MCU_DATA_0
    0x338C, 0xA77C,    // MCU_ADDRESS [MODE_GAM_TABLE_A_13]
    0x3390, 0x00DF,    // MCU_DATA_0
    0x338C, 0xA77D,    // MCU_ADDRESS [MODE_GAM_TABLE_A_14]
    0x3390, 0x00E6,    // MCU_DATA_0
    0x338C, 0xA77E,    // MCU_ADDRESS [MODE_GAM_TABLE_A_15]
    0x3390, 0x00ED,    // MCU_DATA_0
    0x338C, 0xA77F,    // MCU_ADDRESS [MODE_GAM_TABLE_A_16]
    0x3390, 0x00F3,    // MCU_DATA_0
    0x338C, 0xA780,    // MCU_ADDRESS [MODE_GAM_TABLE_A_17]
    0x3390, 0x00F9,    // MCU_DATA_0
    0x338C, 0xA781,    // MCU_ADDRESS [MODE_GAM_TABLE_A_18]
    0x3390, 0x00FF,    // MCU_DATA_0
    //    0x338C, 0xA103,    // MCU_ADDRESS [SEQ_CMD]
    //    0x3390, 0x0006,    // MCU_DATA_0
    //    0xfFFF, 50, 
    
    0x338C, 0xA102,    // MCU_ADDRESS [SEQ_CMD]
    0x3390, 0x000F,    // MCU_DATA_0    
    
    0x338C, 0xA103,    // MCU_ADDRESS [SEQ_CMD]
    0x3390, 0x0005,    // MCU_DATA_0
    //    0xfFFF, 50,   
    
    //lens [Register Log 09/03/09 15:10:52]
    0x3210, 0x01F8,     // COLOR_PIPELINE_CONTROL [1]
    0x34CE, 0x01E8,     // LENS_CORRECTION_CONTROL [1]
    0x34D0, 0x6131,     // ZONE_BOUNDS_X1_X2 [1]
    0x34D2, 0x3492,     // ZONE_BOUNDS_X0_X3 [1]
    0x34D4, 0x9B68,     // ZONE_BOUNDS_X4_X5 [1]
    0x34D6, 0x4B25,     // ZONE_BOUNDS_Y1_Y2 [1]
    0x34D8, 0x2670,     // ZONE_BOUNDS_Y0_Y3 [1]
    0x34DA, 0x724C,     // ZONE_BOUNDS_Y4_Y5 [1]
    0x34DC, 0xFFFA,     // CENTER_OFFSET [1]
    0x34DE, 0x00B4,     // FX_RED [1]
    0x34E0, 0x007E,     // FX_GREEN [1]
    0x34E2, 0x00A0,     // FX_GREEN2 [1]
    0x34E4, 0x007C,     // FX_BLUE [1]
    0x34E6, 0x00B4,     // FY_RED [1]
    0x34E8, 0x007E,     // FY_GREEN [1]
    0x34EA, 0x00A0,     // FY_GREEN2 [1]
    0x34EC, 0x007C,     // FY_BLUE [1]
    0x34EE, 0x0A97,     // DF_DX_RED [1]
    0x34F0, 0x0CB0,     // DF_DX_GREEN [1]
    0x34F2, 0x0A7D,     // DF_DX_GREEN2 [1]
    0x34F4, 0x0CA7,     // DF_DX_BLUE [1]
    0x34F6, 0x0BE9,     // DF_DY_RED [1]
    0x34F8, 0x0B43,     // DF_DY_GREEN [1]
    0x34FA, 0x09F0,     // DF_DY_GREEN2 [1]
    0x34FC, 0x0A1C,     // DF_DY_BLUE [1]
    0x3500, 0xDF4C,     // SECOND_DERIV_ZONE_0_RED [1]
    0x3502, 0x2A34,     // SECOND_DERIV_ZONE_0_GREEN [1]
    0x3504, 0x4551,     // SECOND_DERIV_ZONE_0_GREEN2 [1]
    0x3506, 0x3D37,     // SECOND_DERIV_ZONE_0_BLUE [1]
    0x3508, 0x2DF9,     // SECOND_DERIV_ZONE_1_RED [1]
    0x350A, 0x1CE9,     // SECOND_DERIV_ZONE_1_GREEN [1]
    0x350C, 0x310B,     // SECOND_DERIV_ZONE_1_GREEN2 [1]
    0x350E, 0x30EA,     // SECOND_DERIV_ZONE_1_BLUE [1]
    0x3510, 0x202C,     // SECOND_DERIV_ZONE_2_RED [1]
    0x3512, 0x0C14,     // SECOND_DERIV_ZONE_2_GREEN [1]
    0x3514, 0x1824,     // SECOND_DERIV_ZONE_2_GREEN2 [1]
    0x3516, 0x1D1A,     // SECOND_DERIV_ZONE_2_BLUE [1]
    0x3518, 0x2438,     // SECOND_DERIV_ZONE_3_RED [1]
    0x351A, 0x181F,     // SECOND_DERIV_ZONE_3_GREEN [1]
    0x351C, 0x262B,     // SECOND_DERIV_ZONE_3_GREEN2 [1]
    0x351E, 0x1E16,     // SECOND_DERIV_ZONE_3_BLUE [1]
    0x3520, 0x3031,     // SECOND_DERIV_ZONE_4_RED [1]
    0x3522, 0x2721,     // SECOND_DERIV_ZONE_4_GREEN [1]
    0x3524, 0x1C17,     // SECOND_DERIV_ZONE_4_GREEN2 [1]
    0x3526, 0x1D20,     // SECOND_DERIV_ZONE_4_BLUE [1]
    0x3528, 0x202B,     // SECOND_DERIV_ZONE_5_RED [1]
    0x352A, 0x0A11,     // SECOND_DERIV_ZONE_5_GREEN [1]
    0x352C, 0xEF0E,     // SECOND_DERIV_ZONE_5_GREEN2 [1]
    0x352E, 0xF00E,     // SECOND_DERIV_ZONE_5_BLUE [1]
    0x3530, 0x1BD2,     // SECOND_DERIV_ZONE_6_RED [1]
    0x3532, 0x100B,     // SECOND_DERIV_ZONE_6_GREEN [1]
    0x3534, 0xF8D4,     // SECOND_DERIV_ZONE_6_GREEN2 [1]
    0x3536, 0x060B,     // SECOND_DERIV_ZONE_6_BLUE [1]
    0x3538, 0x13D1,     // SECOND_DERIV_ZONE_7_RED [1]
    0x353A, 0x134A,     // SECOND_DERIV_ZONE_7_GREEN [1]
    0x353C, 0xE30A,     // SECOND_DERIV_ZONE_7_GREEN2 [1]
    0x353E, 0xDD34,     // SECOND_DERIV_ZONE_7_BLUE [1]
    0x3540, 0x0001,     // X2_FACTORS [1]
    0x3542, 0x0007,     // GLOBAL_OFFSET_FXY_FUNCTION [24]
    0x3544, 0x07FF,     // K_FACTOR_IN_K_FX_FY_R_TR [1]
    0x3546, 0x0297,     // K_FACTOR_IN_K_FX_FY_G1_TR [1]
    0x3548, 0x0000,     // K_FACTOR_IN_K_FX_FY_G2_TR [1]
    0x354A, 0x03FF,     // K_FACTOR_IN_K_FX_FY_B_TR [1]
    0x354C, 0x0600,     // K_FACTOR_IN_K_FX_FY_R_TL [1]
    0x354E, 0x0625,     // K_FACTOR_IN_K_FX_FY_G1_TL [1]
    0x3550, 0x03FF,     // K_FACTOR_IN_K_FX_FY_G2_TL [1]
    0x3552, 0x0439,     // K_FACTOR_IN_K_FX_FY_B_TL [1]
    0x3554, 0x06d0,     // K_FACTOR_IN_K_FX_FY_R_BR [22]
    0x3556, 0x03FF,     // K_FACTOR_IN_K_FX_FY_G1_BR [1]
    0x3558, 0x07D9,     // K_FACTOR_IN_K_FX_FY_G2_BR [1]
    0x355A, 0x0000,     // K_FACTOR_IN_K_FX_FY_B_BR [1]
    0x355C, 0x0000,     // K_FACTOR_IN_K_FX_FY_R_BL [59]
    0x355E, 0x0000,     // K_FACTOR_IN_K_FX_FY_G1_BL [1]
    0x3560, 0x0000,     // K_FACTOR_IN_K_FX_FY_G2_BL [1]
    0x3562, 0x0000,     // K_FACTOR_IN_K_FX_FY_B_BL [1]
    
    
    //[MT9D112 (SOC2020) Register Wizard Defaults
    0x341E, 0x8F09,        //PLL/ Clk_in control: BYPASS PLL = 36617
    0x341C, 0x0466,        //PLL Control 1 = 1126
    0xffff, 0x0001,              // Allow PLL to lock
    0x341E, 0x8F09,        //PLL/ Clk_in control: PLL ON, bypassed = 36617
    0x341E, 0x8F08,        //PLL/ Clk_in control: USE PLL = 36616
    0x3044, 0x0542,        //Reserved = 1346
    0x3216, 0x02CF,        //Internal Clock Control = 719
    0x321C, 0x0402,        //OF Control Status = 1026
    0x3212, 0x0001,        //Factory Bypass = 1
    0x338C, 0x2703,        //Output Width (A)
    0x3390, 0x0280,        //      = 640
    0x338C, 0x2705,        //Output Height (A)
    0x3390, 0x01E0,        //      = 480
    0x338C, 0x2707,        //Output Width (B)
    0x3390, 0x0640,        //      = 1600
    0x338C, 0x2709,        //Output Height (B)
    0x3390, 0x04B0,        //      = 1200
    0x338C, 0x270D,        //Row Start (A)
    0x3390, 0x0000,        //      = 0
    0x338C, 0x270F,        //Column Start (A)
    0x3390, 0x0000,        //      = 0
    0x338C, 0x2711,        //Row End (A)
    0x3390, 0x04BD,        //      = 1213
    0x338C, 0x2713,        //Column End (A)
    0x3390, 0x064D,        //      = 1613
    0x338C, 0x2715,        //Extra Delay (A)
    0x3390, 0x009F,        //      = 159
    0x338C, 0x2717,        //Row Speed (A)
    0x3390, 0x2112,        //      = 8466
    0x338C, 0x2719,        //Read Mode (A)
    0x3390, 0x046C,        //      = 1132
    0x338C, 0x271B,        //sensor_sample_time_pck (A)
    0x3390, 0x0122,        //      = 290
    0x338C, 0x271D,        //sensor_fine_correction (A)
    //ELD =0x3390, 0xFFFF, 0x007B        //      = 123
    0x3390,0x007b,
    0x338C, 0x271F,        //sensor_fine_IT_min (A)
    //ELD = 0x3390, 0xFFFF, 0x013F        //      = 319
    0x3390,0x013f,
    0x338C, 0x2721,        //sensor_fine_IT_max_margin (A)
    //ELD = 0x3390, 0xFFFF, 0x00AB        //      = 171
    0x3390,0x00ab,
    0x338C, 0x2723,       //Frame Lines (A)
    0x3390, 0x0293,       //      = 659
    0x338C, 0x2725,       //Line Length (A)
    0x3390, 0x04E2,       //      = 1250
    0x338C, 0x2727,       //sensor_dac_id_4_5 (A)
    //ELD = 0x3390, 0xFFFF, 0x1010        //      = 4112
    0x3390,0x1010,
    0x338C, 0x2729,        //sensor_dac_id_6_7 (A)
    //ELD = 0x3390, 0xFFFF, 0x2010        //      = 8208
    0x3390,0x2010,
    0x338C, 0x272B,        //sensor_dac_id_8_9 (A)
    //ELD = 0x3390, 0xFFFF, 0x1010        //      = 4112
    0x3390,0x1010, 
    0x338C, 0x272D,        //sensor_dac_id_10_11 (A)
    //ELD = 0x3390, 0xFFFF, 0x1007        //      = 4103
    0x3390,0x1007,
    0x338C, 0x272F,        //Row Start (B)
    0x3390, 0x004 ,       //      = 4
    0x338C, 0x2731,        //Column Start (B)
    0x3390, 0x004 ,       //      = 4
    0x338C, 0x2733,        //Row End (B)
    0x3390, 0x4BB ,       //      = 1211
    0x338C, 0x2735,        //Column End (B)
    0x3390, 0x64B ,       //      = 1611
    0x338C, 0x2737,        //Extra Delay (B)
    //0x3390, 0x147 ,       //      = 327 uxga=0x7.5fps
    0x3390, 0x02f6,         //      =758  uxga=5.11fps
    0x338C, 0x2739,        //Row Speed (B)
    0x3390, 0x2111,        //      = 8465
    0x338C, 0x273B,        //Read Mode (B)
    0x3390, 0x0024,        //      = 36
    0x338C, 0x273D,        //sensor_sample_time_pck (B)
    0x3390, 0x0120,        //      = 288
    0x338C, 0x273F,        //sensor_fine_correction (B)
    //ELD = 0x3390, 0xFFFF, 0x00A4        //      = 164
    0x3390, 0x00a4,
    0x338C, 0x2741,        //sensor_fine_IT_min (B)
    //ELD = 0x3390, 0xFFFF, 0x0169        //      = 361
    0x3390,0x0169,
    0x338C, 0x2743,        //sensor_fine_IT_max_margin (B)
    //ELD = 0x3390, 0xFFFF, 0x00A4        //      = 164
    0x3390, 0x00a4,
    0x338C, 0x2745,        //Frame Lines (B)
    // 0x3390, 0x0588,        //      = 1672 -256
    //0x3390, 0x0688,        //      = 1672
    0x3390, 0x095b,        //      = 2395 uxga=5.11fps
    0x338C, 0x2747,        //Line Length (B)
    0x3390, 0x09C4,        //      = 2500
    0x338C, 0x2751,        //Crop_X0 (A)
    0x3390, 0x0000,        //      = 0
    0x338C, 0x2753,        //Crop_X1 (A)
    0x3390, 0x0320,        //      = 800
    0x338C, 0x2755,        //Crop_Y0 (A)
    0x3390, 0x0000,        //      = 0
    0x338C, 0x2757,        //Crop_Y1 (A)
    0x3390, 0x0258,        //      = 600
    0x338C, 0x275F,        //Crop_X0 (B)
    0x3390, 0x0000,        //      = 0
    0x338C, 0x2761,        //Crop_X1 (B)
    0x3390, 0x0640,        //      = 1600
    0x338C, 0x2763,        //Crop_Y0 (B)
    0x3390, 0x0000,        //      = 0
    0x338C, 0x2765,        //Crop_Y1 (B)
    0x3390, 0x04B0,        //      = 1200
    0x338C, 0x222E,        //R9 Step
    0x3390, 0x0066,        //      = 102
    0x338C, 0xA408,        //search_f1_50
    0x3390, 0x18  ,      //      = 24
    0x338C, 0xA409,        //search_f2_50
    0x3390, 0x1B  ,      //      = 27
    0x338C, 0xA40A,        //search_f1_60
    0x3390, 0x1D  ,      //      = 29
    0x338C, 0xA40B,        //search_f2_60
    0x3390, 0x20  ,      //      = 32
    0x338C, 0x2411,        //R9_Step_60 (A)
    0x3390, 0x0066,        //      = 102
    0x338C, 0x2413,        //R9_Step_50 (A)
    0x3390, 0x007A,        //      = 122
    0x338C, 0x2415,        //R9_Step_60 (B)
    0x3390, 0x0066,        //      = 102
    0x338C, 0x2417,        //R9_Step_50 (B)
    0x3390, 0x007A,        //      = 122
    0x338C, 0xA40D,        //Stat_min
    0x3390, 0x02  ,      //      = 2
    0x338C, 0xA410,        //Min_amplitude
    0x3390, 0x01  ,      //      = 1
    
    //go to mdr ae Mode
    //seq.previewParEnter.ae = 4
    //seq.previewParLeave.ae = 4
    //seq.capParEnter.ae = 4
    0x338C, 0xA122, //seq.previewParEnter.ae
    0x3390, 0x0004,      
    0x338C, 0xA130, //seq.previewParLeave.ae
    0x3390, 0x0004,      
    0x338C, 0xA137, //seq.capParEnter.ae
    0x3390, 0x0002, //manual    ：2  mdr ：4
    
    0x338C, 0xA76e, //gam_cont_B (RW) Contrast Setting
    0x3390, 0x0012, //contrast= +0,gramma =0.45
    0x338C, 0xA76d, //gam_cont_a (RW) Contrast Setting
    0x3390, 0x0012, //contrast= 125%,0gramma =0.45
    
    0x338C, 0xA215, //ae.IndexTH23
    0x3390, 0x0005, //default=0x008
    
    0x338C, 0xA216, //ae.maxgain23
    0x3390, 0x0078, //default=0x78
         
    ////[Low Power Preview 15 FPS]
    ////---------------------------------------------------
    //// Minimum 18.6 FPS (52 ms Int-Time)
    ////---------------------------------------------------
    0x338C, 0xA20C, // MCU_ADDRESS [AE_MAX_INDEX]
    0x3390, 0x0005, // MCU_DATA_0
    0x338C, 0xA20e,  //maxVirtGain
    0x3390, 0x0080,
    0x338C, 0xA214, // MCU_ADDRESS [AE_MAX_DGAIN_AE2]
    0x3390, 0x0080, // MCU_DATA_0
    //0x338C, 0xA103,   // MCU_ADDRESS [SEQ_CMD]
    //0x3390, 0x0005,   // MCU_DATA_0
    0x338C, 0xA206,  // MCU_ADDRESS [AE_TARGET]
    0x3390, 0x0041,   //0x0031,  // MCU_DATA_0  
    
    /********************linjinchao add************************/ 
    0x338C, 0xA123,     // MCU_ADDRESS [SEQ_PREVIEW_0_FD]
    0x3390, 0x0002,     // MCU_DATA_0    filker cancel-manual
    0x338C, 0xA404,     // MCU_ADDRESS [50/60 manual select and enable]
    0x3390, 0x00c0,  // MCU_DATA_0     [select 50hz environment] 
         
    /********************linjinchao add************************/    
    0x338C, 0xA103, // MCU_ADDRESS [SEQ_CMD]
    0x3390, 0x0005, // MCU_DATA_0 
    0x338C, 0xA103, // MCU_ADDRESS [SEQ_CMD]
    0x3390, 0x0006, // MCU_DATA_0    
    ////enter into qvga Mode
    0x301a,0x0acc,
    //0xffff,0x00c8,
    0x3202,0x0008,
    //0xffff,0x0032,
    //0x338c,0xa103,            
    //0x3390,0x0005,
    0x338c,0x2703,
    0x3390,0x0140,
    0x338c,0x2705,
    0x3390,0x00f0,
    0x338c,0x2751,
    0x3390,0x0000,
    0x338c,0x2753,//800
    0x3390,0x0320,
    0x338c,0x2755,
    0x3390,0x0000,
    0x338c,0x2757,
    0x3390,0x0258,//600    
    
    0x338C, 0xA103, // MCU_ADDRESS [SEQ_CMD]
    0x3390, 0x0001, // MCU_DATA_0 GO TO PREVIEW
}; 


/* qvga */
static unsigned short sensor_qvga_data[] =
{
    0x301a,0x0acc,
    0x3202,0x0008,
    0x338c,0x2703,
    0x3390,0x0140,
    0x338c,0x2705,
    0x3390,0x00f0,
    0x338c,0x2751,
    0x3390,0x0000,
    0x338c,0x2753,  //800
    0x3390,0x0320,
    0x338c,0x2755,
    0x3390,0x0000,
    0x338c,0x2757,
    0x3390,0x0258,               //600    
           
    0x338c,0xa103,
    0x3390,0x0001,       
    0x338c,0xa103,
    0x3390,0x0005,      
};  /*  320 * 240 */


static unsigned short sensor_vga_data[] =
{
    0x301a,0x0acc,
    //0xffff,0x00c8,
    0x3202,0x0008,
    //0xffff,0x0032,
    //0x338c,0xa103,            
    //0x3390,0x0005,
    0x338c,0x2703,
    0x3390,0x0280,
    0x338c,0x2705,
    0x3390,0x01e0,
    0x338c,0x2751,
    0x3390,0x0000,
    0x338c,0x2753,//800
    0x3390,0x0320,
    0x338c,0x2755,
    0x3390,0x0000,
    0x338c,0x2757,
    0x3390,0x0258,//600    
    
    0x338C, 0xA103, // MCU_ADDRESS [SEQ_CMD]
    0x3390, 0x0001, // MCU_DATA_0 GO TO PREVIEW
    0x338C, 0xA103,
    0x3390, 0x0005,

};  /*  640 * 480 */

/* 720p */
static unsigned short sensor_720p_data[] =
{
    0x301a,0x0acc,
    //0xffff,0x00c8,
    0x3202,0x0008,
    //0xffff,0x0032,
    //0x338c,0xa103,            
    //0x3390,0x0005,
    0x338c,0x2703,
    0x3390,0x02d0, //720
    0x338c,0x2705,
    0x3390,0x0240, //576
    0x338c,0x2751,
    0x3390,0x0000,
    0x338c,0x2753,//800
    0x3390,0x0320,
    0x338c,0x2755,
    0x3390,0x0000,
    0x338c,0x2757,
    0x3390,0x0258,//600    
    
    0x338C, 0xA103, // MCU_ADDRESS [SEQ_CMD]
    0x3390, 0x0001, // MCU_DATA_0 GO TO PREVIEW
    
    0x338C, 0xA103,
    0x3390, 0x0005,
};

/* xga */
static unsigned short sensor_xga_data[] =
{       
    0x301a,0x0acc,
    0x3202,0x0008,
    
    0x338c,0x2707,
    0x3390,0x0400,   //1024
    0x338c,0x2709,
    0x3390,0x0300,   //768
    0x338c,0x275f,
    0x3390,0x0000,
    0x338c,0x2761,
    0x3390,0x0640,   //1600
    0x338c,0x2763,
    0x3390,0x0000,
    0x338c,0x2765,
    0x3390,0x04b0,   //1200
    
    0x338c,0xa103,
    0x3390,0x0002,
    //0xffff,0x00f0,
    0x338c,0xa120,
    //0x3390,0x0001,   //for snap
    0x3390,0x0062,
    
    0x338C, 0xA103,
    0x3390, 0x0005,
};  /*  XGA 1027 * 768 */



/* sxga */
static unsigned short sensor_sxga_data[] =
{       
    0x301a,0x0acc,
    0x3202,0x0008,
    
    0x338c,0x2707,
    0x3390,0x0500,
    0x338c,0x2709,
    0x3390,0x03C0,
    0x338c,0x275f,
    0x3390,0x0000,
    0x338c,0x2761,
    0x3390,0x0640,
    0x338c,0x2763,
    0x3390,0x0000,
    0x338c,0x2765,
    0x3390,0x04b0,
    
    0x338c,0xa103,
    0x3390,0x0002,
    //0xffff,0x00f0,
    0x338c,0xa120,
    //0x3390,0x0001,   //for snap
    0x3390,0x0062,
    
    0x338C, 0xA103,
    0x3390, 0x0005,
};  /*  SXGA 1280 * 960 */

/* svga */
//static unsigned short sensor_svga_data[] = {
    /*
0x0007, 0x0320,     // MODE_OUTPUT_WIDTH_B [1]
0x0009, 0x0258,     // MODE_OUTPUT_HEIGHT_B [1]
0x005F, 0x0000,     // MODE_CROP_X0_B [1]
0x0061, 0x0640,     // MODE_CROP_X1_B [1]
0x0063, 0x0000,     // MODE_CROP_Y0_B [1]
0x0065, 0x04B0,     // MODE_CROP_Y1_B [1]
*/
//};  /*  SVGA 800 * 600 */


/* uxga */
static unsigned short sensor_uxga_data[] = {
    //CAPTURE 1600*1200
    0x301a,0x0acc,
    0x3202,0x0008,
    
    //1600*1200
    0x338C, 0x2707,     // MCU_ADDRESS [MODE_OUTPUT_WIDTH_B]
    0x3390, 0x0640,     // MCU_DATA_0
    0x338C, 0x2709,     // MCU_ADDRESS [MODE_OUTPUT_HEIGHT_B]
    0x3390, 0x04B0,     // MCU_DATA_0
    0x338C, 0x275F,     // MCU_ADDRESS [MODE_CROP_X0_B]
    0x3390, 0x0000,     // MCU_DATA_0
    0x338C, 0x2761,     // MCU_ADDRESS [MODE_CROP_X1_B]
    0x3390, 0x0640,     // MCU_DATA_0
    0x338C, 0x2763,     // MCU_ADDRESS [MODE_CROP_Y0_B]
    0x3390, 0x0000,     // MCU_DATA_0
    0x338C, 0x2765,     // MCU_ADDRESS [MODE_CROP_Y1_B]
    0x3390, 0x04B0,     // MCU_DATA_0
    //////
    
    0x338c,0xa103,
    0x3390,0x0002,
    //0xffff,0x00f0,
    0x338c,0xa120,
    //0x3390,0x0001,   //for snap
    0x3390,0x0062,
    //0xffff,0x001f2,
    //0x338c,0xa121,  // for  snap
    //0x3390,0x0020,
    //0xffff,0x0032,
    
    0x338C, 0xA103,
    0x3390, 0x0005,
};  /*  UXGA 1600 * 1200 */

static unsigned int cpu_clk = CPU_120MHZ;

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

static void i2c_receive_byte(sensor_i2cinfo_t * sensor_i2cinfo)
{
    unsigned int value;
    unsigned int count;
    unsigned int pin0, pin1;

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
            break;
        }
    } while (!value);   /* wait for transmit complete! */

    /* ACK_OK ? */
    act_writel(0x80, I2CSTA);

    return;
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

    act_writel((reg_addr&0xFF00)>>8, I2CDAT);
    act_writel(0x82, I2CCTL);   //release I2C
    i2c_transmit_byte(&sensor_i2cinfo);
    
    act_writel((reg_addr&0x00FF), I2CDAT);
    act_writel(0x82, I2CCTL);   //release I2C; should it placed after i2c_transmit_byte??
    i2c_transmit_byte(&sensor_i2cinfo);

//  act_writel(0x8b, I2CCTL);   //generate stop condition
//  i2c_wait_stop(&sensor_i2cinfo);

    act_writel(sensor_i2cinfo.slave_addr + 1, I2CADDR); //write device address of AT24C02
    act_writel(0x8e, I2CCTL);   //generate start condition
    i2c_transmit_byte(&sensor_i2cinfo);
    
//////////////////////////////////////////////////////////added for micro. 2007 1106,  cccw
    act_writel(0x82, I2CCTL);   //release I2C
    i2c_receive_byte(&sensor_i2cinfo);
    sensor_i2cinfo.reg_data = ((act_readl(I2CDAT)) & 0xff)<< 8;
    
    
    act_writel(0x83, I2CCTL);   //release I2C
    i2c_receive_byte(&sensor_i2cinfo);   //is receive or transmit

    sensor_i2cinfo.reg_data |= ((act_readl(I2CDAT))&0xff);
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
    act_writel((reg_addr&0xFF00)>>8, I2CDAT);
    act_writel(0x82, I2CCTL);   //release I2C
    i2c_transmit_byte(&sensor_i2cinfo);
    
    act_writel(reg_addr&0x00FF, I2CDAT);
    act_writel(0x82, I2CCTL);   //release I2C
    i2c_transmit_byte(&sensor_i2cinfo);     

    act_writel((reg_data&0xFF00)>>8, I2CDAT);
    act_writel(0x82, I2CCTL);   //release I2C
    ret = i2c_transmit_byte(&sensor_i2cinfo);
    if (ret != 0)
    {
        goto exit;
    }
    act_writel(reg_data&0x00FF, I2CDAT);
    act_writel(0x82, I2CCTL);   //release I2C
    i2c_transmit_byte(&sensor_i2cinfo);

    act_writel(0x8b, I2CCTL);   //generate stop condition

    i2c_wait_stop(&sensor_i2cinfo);
    
    exit:
    /*! I2C clk disable*/
    act_writel((act_readl(CMU_DEVCLKEN) & (~(0x1 << 23))), CMU_DEVCLKEN); 
            
    release_sensor_i2c();       
    multip_release_pinowner(1,0);

    return ret;
}

int sensor_cap_start_ae(void)
{
    return 0;
}
int sensor_enable_awb(void)
{
    unsigned int val;
    sensor_i2c_write(0x338c,0xa102);
    val=sensor_i2c_read(0x3390);
    val|=1<<2;
    sensor_i2c_write(0x3390,val);           
}

int sensor_disable_awb(void)
{
    unsigned int val;
    sensor_i2c_write(0x338c,0xa102);
    val=sensor_i2c_read(0x3390);
    val|=~(1<<2);
    sensor_i2c_write(0x3390,val);   
}
int sensor_enable_ae(void)
{
    unsigned int val;
    sensor_i2c_write(0x338c,0xa102);
    val=sensor_i2c_read(0x3390);
    val|=1<<0;
    sensor_i2c_write(0x3390,val);           
}

int sensor_disable_ae(void)
{
    unsigned int val;
    sensor_i2c_write(0x338c,0xa102);
    val=sensor_i2c_read(0x3390);
    val|=~(1<<0);
    sensor_i2c_write(0x3390,val);   
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
    sensor_i2cinfo_t sensor_i2cinfo;
#if 0
    switch(whitebalance) 
    {
    case SS_AUTO_BALANCE:       /* auto */
        sensor_i2c_write(0x338C, 0xA102);   // MCU_ADDRESS [SEQ_MODE]
        sensor_i2c_write(0x3390, 0x000F);   // MCU_DATA_0
        
        sensor_i2c_write(0x3210, 0x01FC);   // COLOR_PIPELINE_CONTROL [3]
        sensor_i2c_write(0x32D4, 0x0080);   // DIGITAL_GAIN_1_RED [29]
        sensor_i2c_write(0x32D6, 0x0080);   // DIGITAL_GAIN_1_GREEN1 [33]
        sensor_i2c_write(0x32D8, 0x0080);   // DIGITAL_GAIN_1_GREEN2 [33]
        sensor_i2c_write(0x32DA, 0x0080);   // DIGITAL_GAIN_1_BLUE [41]
        break;
    case SS_INDOOR_BALANCE:     /* office */
        sensor_i2c_write(0x3210, 0x01FC);   // COLOR_PIPELINE_CONTROL [3]
        sensor_i2c_write(0x32D4, 0x0048);   // DIGITAL_GAIN_1_RED [29]
        sensor_i2c_write(0x32D6, 0x0048);   // DIGITAL_GAIN_1_GREEN1 [33]
        sensor_i2c_write(0x32D8, 0x0048);   // DIGITAL_GAIN_1_GREEN2 [33]
        sensor_i2c_write(0x32DA, 0x0048);   // DIGITAL_GAIN_1_BLUE [41]
        break;
    case SS_OUTDOOR_BALANCE:        /* sunny */
        sensor_i2c_write(0x3210, 0x01DC);   // COLOR_PIPELINE_CONTROL [1]
        sensor_i2c_write(0x32D4, 0x01FE);   // DIGITAL_GAIN_1_RED [15]
        sensor_i2c_write(0x32D6, 0x01FE);   // DIGITAL_GAIN_1_GREEN1 [23]
        sensor_i2c_write(0x32D8, 0x01FE);   // DIGITAL_GAIN_1_GREEN2 [23]
        sensor_i2c_write(0x32DA, 0x01FE);   // DIGITAL_GAIN_1_BLUE [17]
        break;
    case SS_FLUORE_BALANCE:     /* home */

        break;

    default:
        break;
    }
    sensor_i2c_write(0x338C, 0xA103);
    sensor_i2c_write(0x3390, 0x0005);
    mdelay(10);    
#endif
    vi_effect.ss_white_balance = whitebalance;

    return 0;
}
static unsigned int previe_exposurexgain; 
static unsigned int capture_gain;
static unsigned int capture_intergtime = 1;
static unsigned int capture_intergtime_zone;
static unsigned int capture_shutdelay;
static unsigned int preview_r9_step;
static unsigned int capture_r9_step;

#define MAX_PREVIEW_INTERGTIME    5
#define MAX_CAPTURE_INTERGTIME    0x14
#define MAX_GAIN                  0x18

static int sensor_get_exposure_param(vi_resolution_t type)
{
    unsigned int total_val,vir_gain,current_zone,shut_delay;
    unsigned int integatime,r9_step,digtal_gain;
    unsigned int physGainR,physGainG,physGainB;
    sensor_i2c_write(0x338c,0xa21d);
    vir_gain=sensor_i2c_read(0x3390);//Current virtual gain
    //   printf("Current virtual gain=0x%x\n",vir_gain);
    sensor_i2c_write(0x338c,0xa21c);   
    current_zone=sensor_i2c_read(0x3390);//Current zone (integration time) 
    //   printf("Current zone (integration time)=0x%x\n",current_zone);   
    sensor_i2c_write(0x338c,0xa21a);   
    shut_delay=sensor_i2c_read(0x3390);// Current shutter delay value ,used for the very brgiht environment
    //   printf("Current shutter delay value=0x%x\n",shut_delay); 
    sensor_i2c_write(0x338c,0xa222);                                      
    digtal_gain=sensor_i2c_read(0x3390);//Current digital gain post-LC
    //   printf("Current digital gain post-LC=0x%x\n",digtal_gain);   
    sensor_i2c_write(0x338c,0x2223);     
    integatime=sensor_i2c_read(0x3390);//Current R9:0 value (integration time)R9
    //   printf("Current R9:0 value (integration time)R9=0x%x\n",integatime);  
    sensor_i2c_write(0x338c,0x222E);      
    r9_step=sensor_i2c_read(0x3390); //Integration time of one zone  R9_STEP  
    //   printf("Integration time of one zone  R9_STEP  =0x%x\n",r9_step);  
    sensor_i2c_write(0x338c,0x2232);      
    physGainR=sensor_i2c_read(0x3390); //physGainR
    //   printf("physGainR =0x%x\n",physGainR);  
    sensor_i2c_write(0x338c,0x2234);      
    physGainG=sensor_i2c_read(0x3390); //physGainG
    //   printf("physGainG =0x%x\n",physGainG);  
    sensor_i2c_write(0x338c,0x2236);      
    physGainB=sensor_i2c_read(0x3390); //physGainB
    //   printf("physGainB =0x%x\n",physGainB);  

    previe_exposurexgain=vir_gain*integatime;
    //   previe_exposurexgain=previe_exposurexgain*11/10;
    if (current_zone<=MAX_PREVIEW_INTERGTIME-3)
    {
        goto exit;   
    }    
    //   r9_step=r9_step*2;
    if (r9_step==0)
    {
        r9_step = 1;
    }    
    if (previe_exposurexgain > (MAX_CAPTURE_INTERGTIME*r9_step*25))    // Exposure > Capture_Maximum_Shutter
    {
        capture_intergtime = MAX_CAPTURE_INTERGTIME*r9_step;

        capture_gain = (previe_exposurexgain*2 + 1)/capture_intergtime/2;
        if (capture_gain > MAX_GAIN)
        {
              //insert by linjinchao
            capture_intergtime=capture_intergtime+r9_step;            
            // For 50Hz, Exposure = n/100; //For 60Hz, Exposure = n/120
            capture_intergtime = capture_intergtime/ r9_step;
            capture_intergtime = capture_intergtime * r9_step;
            if (capture_intergtime==0)
            {
                capture_intergtime = 1;
            }            
            capture_gain = (previe_exposurexgain *2+1)/ capture_intergtime/2;
        }   
    }
    else // 1/100(120) < Exposure < Capture_Maximum_Shutter, Exposure = n/100(120)
    {
        // 1/100(120) < Exposure < Capture_Maximum_Shutter, Exposure = n/100(120)
        capture_intergtime = previe_exposurexgain / 25/ r9_step;
        capture_intergtime = capture_intergtime * r9_step;
        if(0 == capture_intergtime)
        {
            capture_intergtime = 1;
        }
        capture_gain = (previe_exposurexgain*2 +1) / capture_intergtime/2;
    }
    
    if (capture_intergtime<=(0x13*r9_step))
    {
        capture_intergtime=capture_intergtime+r9_step;
    }    
    else
    {
        vir_gain=vir_gain*11/10;
    }
    if (capture_intergtime>(0x15*r9_step))
    {
        vir_gain=vir_gain+(capture_intergtime-0x15*r9_step)/r9_step;
        capture_intergtime=0x15*r9_step; 
    }    

    capture_intergtime_zone=capture_intergtime/r9_step;
    //   r9_step=r9_step/2;
    return 0;
    
    exit :     
    capture_gain=vir_gain;
    capture_intergtime_zone=current_zone;
    capture_intergtime=integatime;
    return 0;    
};

static int sensor_set_exposure_param(vi_resolution_t type)
{
    if (capture_intergtime_zone==0)
    {
        return 0; 
    }
    sensor_i2c_write(0x338c,0x2223);     
    sensor_i2c_write(0x3390,capture_intergtime);//Current R9:0 value (integration time)R9
    //   printf("set Current R9:0 value (integration time)R9=0x%x\n",capture_intergtime);
    
    sensor_i2c_write(0x338c,0xa21c);   
    sensor_i2c_write(0x3390,capture_intergtime_zone);//Current zone (integration time) 
    //   printf("set Current zone (integration time)=0x%x\n",capture_intergtime_zone);   
    //   sensor_i2c_write(0x338c,0xa21a);   
    //   shut_delay=sensor_i2c_read(0x3390);// Current shutter delay value ,used for the very brgiht environment
    //   printf("Current shutter delay value=0x%x\n",shut_delay); 
    //   sensor_i2c_write(0x338c,0xa222);                                      
    //   sensor_i2c_write(0x3390,capture_gain);//Current digital gain post-LC
    //   printf("set Current digital gain post-LC=0x%x\n",capture_gain);  
    sensor_i2c_write(0x338c,0xa21d);
    sensor_i2c_write(0x3390,capture_gain);//Current virtual gain
    //   printf("set Current virtual gain=0x%x\n",capture_gain);  
    sensor_i2c_write(0x338c,0xa21d);
    capture_gain=sensor_i2c_read(0x3390);//Current virtual gain
    //   printf("Current virtual gain=0x%x\n",capture_gain);  
    sensor_i2c_write(0x338c,0xa21c);   
    capture_intergtime_zone=sensor_i2c_read(0x3390);//Current zone (integration time) 
    //   printf(" Current zone (integration time)=0x%x\n",capture_intergtime_zone);   
    return 0;
}
;

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
    return 0;
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
    switch(contrast) 
    {  
    case 0:
        
        break;
    case 1:
      
        break;
    case 2:
       
        break;
    case 3:
      
        break;
    case 4:
     
        break;
    case 5:
      
        break;
    case 6:
        
        break;
    default:
        return -1;
    }

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
    int ret;

    //vi_effect.ss_white_balance = (unsigned int) ret;

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
    /* note:brihtness is variable with diffrent explosure value */
    //0006 00-c8
    sensor_i2c_write(0x338c,0xa206);
    switch(brightness) 
    {
    case 0:
        sensor_i2c_write(0x3390,0x0018);
        break;
    case 1:
        sensor_i2c_write(0x3390,0x0028);
        break;
    case 2:
        sensor_i2c_write(0x3390,0x0038);
        break;
    case 3:
        sensor_i2c_write(0x3390,0x0048);
        break;
    case 4:
        sensor_i2c_write(0x3390,0x0058);
        break;        
    case 5:
        sensor_i2c_write(0x3390,0x0068);
        break;
    case 6:
        sensor_i2c_write(0x3390,0x0078);
        break;
    default:
        return -1;
    }
    sensor_i2c_write(0x338C, 0xA103);
    sensor_i2c_write(0x3390, 0x0005);
    
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
#if 0   
    unsigned int iValue;

    iValue = sensor_i2c_read(0x13);
    iValue = iValue & 0x01;

    if (iValue) /* auto expose. */
    {
        return SS_AUTO_EXPOSURE;
    }

    return vi_effect.ss_expose_mode;
#endif
    return 0;   
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
    switch(type) 
    {
    case SS_AUTO_EXPOSURE:       /* auto */
        sensor_i2c_write(0x338C, 0xA102);   // MCU_ADDRESS [SEQ_MODE]
        sensor_i2c_write(0x3390, 0x000F);   // MCU_DATA_0
        sensor_i2c_write(0x338C, 0xA206);   // MCU_ADDRESS [AE_TARGET]
        sensor_i2c_write(0x3390, 0x0048);   // MCU_DATA_0
        sensor_i2c_write(0x338C, 0xA207);   // MCU_ADDRESS [AE_GATE]
        sensor_i2c_write(0x3390, 0x0018);   // MCU_DATA_0
        break;
    case SS_INDOOR_EXPOSURE:     /* office */
        sensor_i2c_write(0x338C, 0xA102);   // MCU_ADDRESS [SEQ_MODE]
        sensor_i2c_write(0x3390, 0x000F);   // MCU_DATA_0
        sensor_i2c_write(0x338C, 0xA206);   // MCU_ADDRESS [AE_TARGET]
        sensor_i2c_write(0x3390, 0x0068);   // MCU_DATA_0
        sensor_i2c_write(0x338C, 0xA207);   // MCU_ADDRESS [AE_GATE]
        sensor_i2c_write(0x3390, 0x0018);   // MCU_DATA_0
        break;
    case SS_OUTDOOR_EXPOSURE:        /* sunny */
        sensor_i2c_write(0x338C, 0xA102);   // MCU_ADDRESS [SEQ_MODE]
        sensor_i2c_write(0x3390, 0x000F);   // MCU_DATA_0
        sensor_i2c_write(0x338C, 0xA206);   // MCU_ADDRESS [AE_TARGET]
        sensor_i2c_write(0x3390, 0x0028);   // MCU_DATA_0
        sensor_i2c_write(0x338C, 0xA207);   // MCU_ADDRESS [AE_GATE]
        sensor_i2c_write(0x3390, 0x0004);   // MCU_DATA_0
        break;
    default:
        break;
    }

    //vi_effect.ss_white_balance = type;

    return 0;
}
;

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
}
;

/******************************************************************************/
/*!
 * \par  Description:
 *       set sensor image's special mode.
 * \param[in]    refer to ss_special_effect_t.
 * \retval       0 sucess, else failed.
 *******************************************************************************/
int sensor_set_special_mode(unsigned int type)
{
    int i;
/*
 * --input:
 * spEffect:
 * 0 : NONE,default configure
 * 1 : white and black
 * 2 : ancientry
 * 3 : negative film
 */
    switch(type) 
    {
    case SS_NONE_SPECIAL:       /* normal */
        SS_PRINTF("none sp\n");
        sensor_i2c_write(0x338C, 0x2799);
        sensor_i2c_write(0x3390, 0x6408);
        sensor_i2c_write(0x338C, 0x279B);
        sensor_i2c_write(0x3390, 0x6408);
        sensor_i2c_write(0x338C, 0xA103);
        sensor_i2c_write(0x3390, 0x0005);
        break;
    case SS_WHITE_AND_BLACK:        /* black and white */
        SS_PRINTF("white black\n");
        sensor_i2c_write(0x338C, 0x2799);
        sensor_i2c_write(0x3390, 0x6409);
        sensor_i2c_write(0x338C, 0x279B);
        sensor_i2c_write(0x3390, 0x6409);
        sensor_i2c_write(0x338C, 0xA103);
        sensor_i2c_write(0x3390, 0x0005);
        break;
    case SS_SEPIA:      /* Antique */
        SS_PRINTF("antique\n");        
        sensor_i2c_write(0x338C, 0x2799);
        sensor_i2c_write(0x3390, 0x640A);
        sensor_i2c_write(0x338C, 0x279B);
        sensor_i2c_write(0x3390, 0x640A);
        sensor_i2c_write(0x338C, 0xA103);
        sensor_i2c_write(0x3390, 0x0005);
        break;
    case SS_SOLARIZE:       /* negative */
        SS_PRINTF("solarize\n");
        sensor_i2c_write(0x338C, 0x2799);
        sensor_i2c_write(0x3390, 0x640B);
        sensor_i2c_write(0x338C, 0x279B);
        sensor_i2c_write(0x3390, 0x640B);
        sensor_i2c_write(0x338C, 0xA103);
        sensor_i2c_write(0x3390, 0x0005);
        break;
    default:
        return -1;
    }

    vi_effect.ss_sp_effect = type;

    return 0;
}
;

/******************************************************************************/
/*!
 * \par  Description:
 *       set sensor image's work mode.
 * \param[in]    refer to ss_pix_fmt_t.
 * \retval       0 sucess, else failed.
 *******************************************************************************/
int sensor_set_pix_fmt(unsigned int type)
{
#if 1   
    unsigned int val;

    if (type == VI_PIX_FMT_YUV422) 
    {
        //sensor_i2c_write(0x332e, 0x0002);
        sensor_i2c_write(0x338c, 0x2795);
        sensor_i2c_write(0x3390, 0x0002);
        sensor_i2c_write(0x338c, 0x2797);
        sensor_i2c_write(0x3390, 0x0002);
        sensor_i2c_write(0x338c, 0xa103);
        sensor_i2c_write(0x3390, 0x0005);
        mdelay(10);
        if (ic_type == 0x63)
        {
            act_writel(0x00000689, M_CSICTL); //interface control (Cb0Y0Cr0Y1, Cb1Y2Cr1Y2, ...)
        }
        else if (ic_type == 0x96)
        {
            val = act_readl(M_CSICTL);
            val |= (0x1 << 7);
            act_writel(val, M_CSICTL);               
        }
        else
        {
            return -1;
        }
    }
    else   /* VI_PIX_FMT_4Y4U4Y4V */
    {
        //sensor_i2c_write(0x332e, 0x0000);
        sensor_i2c_write(0x338c, 0x2795);
        sensor_i2c_write(0x3390, 0x0000);
        sensor_i2c_write(0x338c, 0x2797);
        sensor_i2c_write(0x3390, 0x0000);
        sensor_i2c_write(0x338c, 0xa103);
        sensor_i2c_write(0x3390, 0x0005);
        mdelay(10);        
        if (ic_type == 0x63)
        {
            act_writel(0x00000289, M_CSICTL); //interface control (Y0Y1Y2Y3, U0U1U2U3, Y4Y5Y6Y7, V0V1V2V3...)
        }
        else if (ic_type == 0x96)
        {
            val = act_readl(M_CSICTL);
            val &= ~(0x1 << 7);
            act_writel(val, M_CSICTL); 
        }
        else
        {
            return -1;
        }
    }
#endif
    /* temp fix the bug of pc camera unstable in vga 24f/s. just set it down to 15f/s. */
#if 0
    if (vi_dev.app_type == VI_PC_CAMERA_MODE)
    {
        act_writel(0x0000000f, CMU_BTCLK); /* 7.5 MHZ clock out. */
    }
    else
    {
        act_writel(M_CLK_DIVIDE, CMU_BTCLK); /* 12 MHZ clock out. */
    }
#endif
    mdelay(10);

    //vi_dev.app_type = type;
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
        iValue |= 0x1 << VI_RES_1600X1200;
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
    iValue |= 0x1 << 12; /* 12 frames per second is supported. */
    iValue |= 0x1 << 16;
    iValue |= 0x1 << 20;
    iValue |= 0x1 << 24;
//    iValue |= 0x1 << 25;

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
    
    int ret = 0;
#if 0
    switch (f_rate)
    {
        case 12:
            
        if (cpu_clk == CPU_120MHZ)
        {
            act_writel(0x00000009, CMU_BTCLK); /* 1/10 bus clock */
            //act_writel(0x00000004, CMU_BTCLK);
        }
        else if (cpu_clk == CPU_144MHZ)
        {
            act_writel(0x0000000b, CMU_BTCLK); /* 1/12 bus clock */
        }
        else if (cpu_clk == CPU_168MHZ)
        {
            act_writel(0x0000000d, CMU_BTCLK); /* 1/14 bus clock */
        }
        //sensor_i2c_write(0x11, 0x01);
        //sensor_i2c_write(0x6b, 0x5a);
        break;
        case 16:
        if (cpu_clk == CPU_120MHZ)
        {
            act_writel(0x00000004, CMU_BTCLK);
        }
        else if (cpu_clk == CPU_144MHZ)
        {
            act_writel(0x00000005, CMU_BTCLK);
        }
        else if (cpu_clk == CPU_168MHZ)
        {
            act_writel(0x00000006, CMU_BTCLK);
        }
        //sensor_i2c_write(0x11, 0x02);
        //sensor_i2c_write(0x6b, 0x5a);
        break;
        case 20:
        if (cpu_clk == CPU_120MHZ)
        {
            act_writel(0x00000004, CMU_BTCLK);
        }
        else if (cpu_clk == CPU_144MHZ)
        {
            act_writel(0x00000005, CMU_BTCLK);
        }
        else if (cpu_clk == CPU_168MHZ)
        {
            act_writel(0x00000006, CMU_BTCLK);
        }
        //sensor_i2c_write(0x11, 0x04);
        //sensor_i2c_write(0x6b, 0xda);
        break;
        case 24:
        if (cpu_clk == CPU_120MHZ)
        {
            act_writel(0x00000004, CMU_BTCLK);
        }
        else if (cpu_clk == CPU_144MHZ)
        {
            act_writel(0x0000000b, CMU_BTCLK); /* 1/12 bus clock */
        }
        else if (cpu_clk == CPU_168MHZ)
        {
            act_writel(0x0000000d, CMU_BTCLK); /* 1/14 bus clock */
        }
        //sensor_i2c_write(0x11, 0x00);
        //sensor_i2c_write(0x6b, 0x5a);
        break;
        default:
        return -1;
    }
#endif
    return ret;
}

#if 1
static int act_set_dc320x240_resolution(void)
{

    unsigned int i;
    unsigned int value;
    sensor_i2cinfo_t sensor_i2cinfo;

    /* cmos sensor interface disable. */
    sensor_i2cinfo.slave_addr = i2c_slave_addr;

    for (i = 0; i < (sizeof(sensor_qvga_data)/sizeof(unsigned short)); i=i+2)
    {
        sensor_i2cinfo.reg_addr = (unsigned int) sensor_qvga_data[i];
        //i++;
        sensor_i2cinfo.reg_data = (unsigned int) sensor_qvga_data[i+1];
        
        if (sensor_i2cinfo.reg_addr==0xffff)
        {
            mdelay((int)sensor_i2cinfo.reg_data);
            continue;
        }
        sensor_i2c_write(sensor_i2cinfo.reg_addr, sensor_i2cinfo.reg_data);
        //udelay(1);
        if (i>2)
        {
            if ((sensor_qvga_data[i-1]==0xa103) && (sensor_qvga_data[i-2]==0x338c))
            {
                if (sensor_qvga_data[i+1]!=0)
                {
                    do
                    {
                        mdelay(10);
                        value=sensor_i2c_read(sensor_i2cinfo.reg_addr);
                        printf("seq_cmd=%d\n",value);
                        
                    } 
                    while (value!=0);   
                }
            }     
            //mdelay(4);
        }           
    }
    /* 0x0000-Cmos sensor controllor. cmos sensor interface disable. */
    act_writel(0x00000012, M_CSIFIFOCTL); //fifo control (sp bus)
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
        act_writel(value, M_CSIHSPOS); //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIHEPOS);
        value &= ~(0x1fff << 12);
        value |= 0x280 << 12;
        act_writel(value, M_CSIHEPOS); //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIVSPOS);
        value &= ~(0x7ff);
        value |= 0x0;
        act_writel(value, M_CSIVSPOS); //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIVEPOS);
        value &= ~(0x1fff << 12);
        value |= 0xf0 << 12;
        act_writel(value, M_CSIVEPOS); //+0x000c-Hsync start position in pclk
    }
    else
    {
        return -1;
    }

    act_writel(0x00000003, M_CSIRQSTA); //+0x0028-Cmos sensor interface IRQ Status

    return 0;
}
#else
static int act_set_dc320x240_resolution(void)
{
    unsigned int i;
    unsigned int value;
    sensor_i2cinfo_t sensor_i2cinfo;

    /* cmos sensor interface enable. */
    sensor_i2cinfo.slave_addr = i2c_slave_addr;

    for (i = 0; i < (sizeof(sensor_vga_data)/sizeof(unsigned short)); i=i+2)
    {
        sensor_i2cinfo.reg_addr = (unsigned int) sensor_vga_data[i];
        //i++;
        sensor_i2cinfo.reg_data = (unsigned int) sensor_vga_data[i+1];
        sensor_i2c_write(sensor_i2cinfo.reg_addr, sensor_i2cinfo.reg_data);
        //udelay(10);
    }

    /* 0x0000-Cmos sensor controllor. cmos sensor interface disable. */
    act_writel(0x00000012, M_CSIFIFOCTL); //fifo control (sp bus)
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
        act_writel(value, M_CSIHSPOS); //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIHEPOS);
        value &= ~(0x1fff << 12);
        value |= 0x500 << 12;
        act_writel(value, M_CSIHEPOS); //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIVSPOS);
        value &= ~(0x7ff);
        value |= 0x0;
        act_writel(value, M_CSIVSPOS); //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIVEPOS);
        value &= ~(0x1fff << 12);
        value |= 0x1e0 << 12;
        act_writel(value, M_CSIVEPOS); //+0x000c-Hsync start position in pclk
    }
    else
    {
        return -1;
    }
    
    value = act_readl(M_CSICTL);
    /* vertical sub sample ratio: 1/2. */
    value &= ~(0x3 << 10);
    value |= 0x1 << 10;
    /* horizontal sub sample ratio: 1/2. */
    value &= ~(0x3 << 8);
    value |= 0x1 << 8;
    act_writel(value, M_CSICTL);
    
    act_writel(0x00000003, M_CSIRQSTA); //+0x0028-Cmos sensor interface IRQ Status
   
    return 0;
}
#endif

inline static int act_set_dc224x176_resolution(void)
{
    
    unsigned int value;
    int ret = 0;
#if 1
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
        act_writel(value, M_CSIHSPOS); //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIHEPOS);
        value &= ~(0x1fff << 12);
        value |= 0x220 << 12;
        act_writel(value, M_CSIHEPOS); //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIVSPOS);
        value &= ~(0x7ff);
        value |= 0x20;
        act_writel(value, M_CSIVSPOS); //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIVEPOS);
        value &= ~(0x1fff << 12);
        value |= 0xd0 << 12;
        act_writel(value, M_CSIVEPOS); //+0x000c-Hsync start position in pclk
    }
    else
    {
        return -1;
    }
#endif
    return ret;
}

static int act_set_dc640x480_resolution(void)
{
#if 1   
    unsigned int i;
    unsigned int value,val;
    sensor_i2cinfo_t sensor_i2cinfo;

    /* cmos sensor interface enable. */
    sensor_i2cinfo.slave_addr = i2c_slave_addr;

    for (i = 0; i < (sizeof(sensor_vga_data)/sizeof(unsigned short)); i=i+2)
    {
        sensor_i2cinfo.reg_addr = (unsigned int) sensor_vga_data[i];
        //i++;
        sensor_i2cinfo.reg_data = (unsigned int) sensor_vga_data[i+1];
        
        if (sensor_i2cinfo.reg_addr==0xffff)
        {
            mdelay((int)sensor_i2cinfo.reg_data);
            continue;
        }
        sensor_i2c_write(sensor_i2cinfo.reg_addr, sensor_i2cinfo.reg_data);
        //udelay(10);
        if (i>2)
        {
            if ((sensor_vga_data[i-1]==0xa103) && (sensor_vga_data[i-2]==0x338c))
            {
                if (sensor_vga_data[i+1]!=0)
                {
                    do
                    {
                        mdelay(10);
                        val=sensor_i2c_read(sensor_i2cinfo.reg_addr);
                        printf("seq_cmd=%d\n",val);
                        
                    } 
                    while (val!=0);   
                }
            }     
            //mdelay(4);
        } 
    }

    /* 0x0000-Cmos sensor controllor. cmos sensor interface disable. */
    act_writel(0x00000012, M_CSIFIFOCTL); //fifo control (sp bus)
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
        act_writel(value, M_CSIHSPOS); //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIHEPOS);
        value &= ~(0x1fff << 12);
        value |= 0x500 << 12;
        act_writel(value, M_CSIHEPOS); //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIVSPOS);
        value &= ~(0x7ff);
        value |= 0x0;
        act_writel(value, M_CSIVSPOS); //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIVEPOS);
        value &= ~(0x1fff << 12);
        value |= 0x1e0 << 12;
        act_writel(value, M_CSIVEPOS); //+0x000c-Hsync start position in pclk
    }
    else
    {
        return -1;
    }

    act_writel(0x00000003, M_CSIRQSTA); //+0x0028-Cmos sensor interface IRQ Status
#endif    
    return 0;
}

static int act_set_dc720x576_resolution(void)
{
    unsigned int i;
    unsigned int value,val;
    sensor_i2cinfo_t sensor_i2cinfo;

    /* cmos sensor interface enable. */
    sensor_i2cinfo.slave_addr = i2c_slave_addr;

    for (i = 0; i < (sizeof(sensor_720p_data)/sizeof(unsigned short)); i=i+2)
    {
        sensor_i2cinfo.reg_addr = (unsigned int) sensor_720p_data[i];
        //i++;
        sensor_i2cinfo.reg_data = (unsigned int) sensor_720p_data[i+1];
        
        if (sensor_i2cinfo.reg_addr==0xffff)
        {
            mdelay((int)sensor_i2cinfo.reg_data);
            continue;
        }
        sensor_i2c_write(sensor_i2cinfo.reg_addr, sensor_i2cinfo.reg_data);
        if (i>2)
        {
            if ((sensor_720p_data[i-1]==0xa103) && (sensor_720p_data[i-2]==0x338c))
            {
                if (sensor_720p_data[i+1]!=0)
                {
                    do
                    {
                        mdelay(10);
                        val=sensor_i2c_read(sensor_i2cinfo.reg_addr);
                        printf("seq_cmd=%d\n",val);
                        
                    } 
                    while (val!=0);   
                }
            }     
            //mdelay(4);
        } 
        //mdelay(40);
    }

    /* 0x0000-Cmos sensor controllor. cmos sensor interface disable. */
    act_writel(0x00000012, M_CSIFIFOCTL); //fifo control (sp bus)
    //act_writel(0x00000052, M_CSIFIFOCTL); //fifo control (ahb bus)
    if (ic_type == 0x63)
    {
        act_writel(0x00000000, M_CSIHSPOS); //+0x000c-Hsync start position in pclk
        act_writel(0x000005a0, M_CSIHEPOS); //+0x0010-Hsync end position in pclk (SXGA 1280)
        act_writel(0x00000000, M_CSIVSPOS); //+0x001c-Vsync start position in hsync
        act_writel(0x00000240, M_CSIVEPOS); //+0x0020-Vsync end position in hsync (SXGA 960)
    }
    else if (ic_type == 0x96)
    {
        value = act_readl(M_CSIHSPOS);
        value &= ~(0x7ff);
        value |= 0x00000000;
        act_writel(value, M_CSIHSPOS); //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIHEPOS);
        value &= ~(0x1fff << 12);
        value |= 0x5a0 << 12;
        act_writel(value, M_CSIHEPOS); //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIVSPOS);
        value &= ~(0x7ff);
        value |= 0x0;
        act_writel(value, M_CSIVSPOS); //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIVEPOS);
        value &= ~(0x1fff << 12);
        value |= 0x240 << 12;
        act_writel(value, M_CSIVEPOS); //+0x000c-Hsync start position in pclk
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
#if 1   
    unsigned int i;
    unsigned int value;
    sensor_i2cinfo_t sensor_i2cinfo;

    /* cmos sensor interface enable. */
    sensor_i2cinfo.slave_addr = i2c_slave_addr;


    for (i = 0; i < (sizeof(sensor_sxga_data)/sizeof(unsigned short)); i=i+2)
    {
        sensor_i2cinfo.reg_addr = (unsigned int) sensor_sxga_data[i];
        //i++;
        sensor_i2cinfo.reg_data = (unsigned int) sensor_sxga_data[i+1];
        
        if (sensor_i2cinfo.reg_addr==0xffff)
        {
            mdelay((int)sensor_i2cinfo.reg_data);
            continue;
        }
        sensor_i2c_write(sensor_i2cinfo.reg_addr, sensor_i2cinfo.reg_data);
        if (i>2)
        {
            if ((sensor_sxga_data[i-1]==0xa103) && (sensor_sxga_data[i-2]==0x338c))
            {
                if (sensor_sxga_data[i+1]!=0)
                {
                    do
                    {
                        mdelay(10);
                        value=sensor_i2c_read(sensor_i2cinfo.reg_addr);
                        printf("seq_cmd=%d\n",value);
                        
                    } 
                    while (value!=0);   
                }
            }     
            //mdelay(4);
        } 
        //mdelay(40);
    }
    /* 0x0000-Cmos sensor controllor. cmos sensor interface disable. */
    act_writel(0x00000012, M_CSIFIFOCTL); //fifo control (sp bus)
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
        //value |= 0x00000100;
        value |= 0x00000000;
        act_writel(value, M_CSIHSPOS); //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIHEPOS);
        value &= ~(0x1fff << 12);
        value |= 0xa00 << 12;
        act_writel(value, M_CSIHEPOS); //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIVSPOS);
        value &= ~(0x7ff);
        //value |= 0x10;
        value |= 0x0;
        act_writel(value, M_CSIVSPOS); //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIVEPOS);
        value &= ~(0x1fff << 12);
        value |= 0x3c0 << 12;
        act_writel(value, M_CSIVEPOS); //+0x000c-Hsync start position in pclk
    }
    else
    {
        return -1;
    }

    act_writel(0x00000003, M_CSIRQSTA); //+0x0028-Cmos sensor interface IRQ Status
#endif    
    return 0;
}

static int act_set_dc1280x1024_resolution(void)
{

    int ret = 0;
#if 1       
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
        act_writel(value, M_CSIVEPOS); //+0x000c-Hsync start position in pclk
    }
    else
    {
        return -1;
    }
#endif
    return ret;
}

static int act_set_dc1600x1200_resolution(void)
{
    unsigned int i;
    unsigned int value,val;
    sensor_i2cinfo_t sensor_i2cinfo;

    /* cmos sensor interface enable. */
    sensor_i2cinfo.slave_addr = i2c_slave_addr;
    sensor_i2cinfo.reg_addr = 0x12;
    sensor_i2cinfo.reg_data = 0x80;

    for (i = 0; i < (sizeof(sensor_uxga_data)/sizeof(unsigned short)); i=i+2)
    {
        sensor_i2cinfo.reg_addr = (unsigned int) sensor_uxga_data[i];
        //i++;
        sensor_i2cinfo.reg_data = (unsigned int) sensor_uxga_data[i+1];
        
        if (sensor_i2cinfo.reg_addr==0xffff)
        {
            mdelay((int)sensor_i2cinfo.reg_data);
            continue;
        }
        sensor_i2c_write(sensor_i2cinfo.reg_addr, sensor_i2cinfo.reg_data);
        if (i>2)
        {
            if ((sensor_uxga_data[i-1]==0xa103) && (sensor_uxga_data[i-2]==0x338c))
            {
                if (sensor_uxga_data[i+1]!=0)
                {
                    do
                    {
                        mdelay(10);
                        val=sensor_i2c_read(sensor_i2cinfo.reg_addr);
                        printf("seq_cmd=%d\n",val);                        
                    } 
                    while (val!=0);   
                }
            }     
            //mdelay(4);
        } 
        //mdelay(40);
    }

    /* 0x0000-Cmos sensor controllor. cmos sensor interface disable. */
    act_writel(0x00000012, M_CSIFIFOCTL); //fifo control (sp bus)
    //act_writel(0x00000052, M_CSIFIFOCTL); //fifo control (ahb bus)
    if (ic_type == 0x63)
    {
        act_writel(0x00000000, M_CSIHSPOS); //+0x000c-Hsync start position in pclk
        act_writel(0x00000c7f, M_CSIHEPOS); //+0x0010-Hsync end position in pclk (SXGA 1280)
        act_writel(0x00000000, M_CSIVSPOS); //+0x001c-Vsync start position in hsync
        act_writel(0x000004af, M_CSIVEPOS); //+0x0020-Vsync end position in hsync (SXGA 960)
    }
    else if (ic_type == 0x96)
    {
        value = act_readl(M_CSIHSPOS);
        value &= ~(0x7ff);
        value |= 0x00000000;
        act_writel(value, M_CSIHSPOS); //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIHEPOS);
        value &= ~(0x1fff << 12);
        value |= 0xc80 << 12;
        act_writel(value, M_CSIHEPOS); //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIVSPOS);
        value &= ~(0x7ff);
        value |= 0x0;
        act_writel(value, M_CSIVSPOS); //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIVEPOS);
        value &= ~(0x1fff << 12);
        value |= 0x4b0 << 12;
        act_writel(value, M_CSIVEPOS); //+0x000c-Hsync start position in pclk
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
    unsigned int val;
    int ret = 0;

    vi_dev.phy_offset = 0;
    if ( vi_dev.resolution ==cmd)
    {
        return 0;   
    }
    if (vi_dev.resolution==PREVIEW_SIZE)
    {
        sensor_get_exposure_param(vi_dev.resolution);
    }
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
        //act_set_dc224x176_resolution();
        vi_dev.phy_offset = VI_RES_224X176_PHY_ADDR_OFFSET;
        vi_dev.width = 224;
        vi_dev.height = 176;
        ret = -1;
        break;
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
//        if (vi_dev.app_type == VI_PC_CAMERA_MODE)
//        {
//            sensor_i2c_write(0x338C, 0xA103);   // MCU_ADDRESS [SEQ_CMD]
//            sensor_i2c_write(0x3390, 0x0005); // 
//          do 
//          {
//              val=sensor_i2c_read(0x3390);
//              mdelay(5);
//          }
//          while(val!=0);
//        }
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
//        os_time_dly(10);
//        if (capture_intergtime_zone==0)
//        {
//            os_time_dly(2);
//        }
//        if (capture_intergtime_zone<=1)
//        {
//            capture_intergtime_zone=1;
//            os_time_dly(capture_intergtime_zone+2);  
//        }
//        else
//        {
//            os_time_dly(capture_intergtime_zone*3+2);
//        }        
//        break;
    case VI_RES_1280X1024:
        len = 1280 * 1024 * 2;
        act_set_dc1280x1024_resolution();
        sensor_set_exposure_param(cmd);
        vi_dev.width = 1280;
        vi_dev.height = 1024;
        break;
    case VI_RES_1600X1200:
        len = 1600 * 1200 * 2;
        act_set_dc1600x1200_resolution();
        sensor_set_exposure_param(cmd);
        vi_dev.width = 1600;
        vi_dev.height = 1200;
//        os_time_dly(10);
//        if (capture_intergtime_zone==0)
//        {
//            os_time_dly(2);
//        }
//        if (capture_intergtime_zone<=1)
//        {
//            capture_intergtime_zone=1;
//            os_time_dly(capture_intergtime_zone+2);  
//        }
//        else
//        {
//            os_time_dly(capture_intergtime_zone*3+2);
//        }
        break;
    case VI_RES_2048X1536:
        len = 2048 * 1536 * 2;
        ret = -1;
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

    mdelay(10);    

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
    unsigned int value,val;
    sensor_i2cinfo_t sensor_i2cinfo;

    sensor_i2cinfo.slave_addr = i2c_slave_addr;

    for (i = 0; i < (sizeof(sensor_i2cdata)/sizeof(unsigned short)); i=i+2)
    {
        sensor_i2cinfo.reg_addr = (unsigned int) sensor_i2cdata[i];
        //i++;
        sensor_i2cinfo.reg_data = (unsigned int) sensor_i2cdata[i+1];
        
        if (sensor_i2cinfo.reg_addr==0xffff)
        {
            mdelay((int)sensor_i2cinfo.reg_data);
            continue;
        }
        sensor_i2c_write(sensor_i2cinfo.reg_addr, sensor_i2cinfo.reg_data);
        if (i>2)
        {
            if ((sensor_i2cdata[i-1]==0xa103) && (sensor_i2cdata[i-2]==0x338c))
            {
                if (sensor_i2cdata[i+1]!=0)
                {
                    do
                    {
                        mdelay(10);
                        val=sensor_i2c_read(sensor_i2cinfo.reg_addr);
                        printf("seq_cmd=%d\n",val);   
                    } 
                    while (val!=0);   
                }
            }     
            //mdelay(4);
        }
    }
    val=act_readl(M_CSICTL);   //select pclk negative edage as the active edage       
    val&=~(0x01<<2);
    val&=~(0x03<<8);
    val&=~(0x03<<10);    
    val&=~(0x03<<3);      
    act_writel(val, M_CSICTL);   
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
        act_writel(value, M_CSIHSPOS); //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIHEPOS);
        value &= ~(0x1fff << 12);
        value |= 0x280 << 12;
        act_writel(value, M_CSIHEPOS); //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIVSPOS);
        value &= ~(0x7ff);
        value |= 0x0;
        act_writel(value, M_CSIVSPOS); //+0x000c-Hsync start position in pclk

        value = act_readl(M_CSIVEPOS);
        value &= ~(0x1fff << 12);
        value |= 0xf0 << 12;
        act_writel(value, M_CSIVEPOS); //+0x000c-Hsync start position in pclk
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
#if 0
    unsigned int value;

    /* disable sensor's AGC, AWB, AEC. */
    value = sensor_i2c_read(0x13);
    value &= ~(0x7);
    sensor_i2c_write(0x13, value);

    /* soft sleep mode. */
    value = sensor_i2c_read(0x09);
    value |= (0x1 << 4);
    sensor_i2c_write(0x09, value);

    sensor_i2c_write(0x00, 0); /* gain = 0 */
#endif   
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
    act_writel(M_CLK_DIVIDE, CMU_BTCLK); /* 1/10 bus clock */
    return;
}

void modify_data(unsigned long addr, unsigned int resolution, unsigned int app_type)
{
    return;
}
