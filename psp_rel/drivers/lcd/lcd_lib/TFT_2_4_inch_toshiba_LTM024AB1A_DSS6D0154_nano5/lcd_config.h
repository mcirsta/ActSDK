/*
********************************************************************************
*                       usdk130
*                    Module: LCM Driver
*                Copyright(c) 2001-2007 Actions Semiconductor, 
*                        All Right Reserved 
*
* History:         
*      <author>    <time>       <version >    <desc>
*      shaofeng    2007/11/25     1.0         build this file 
********************************************************************************
*/


/*!
* \file lcd_config.h
* \brief lcm driver's config file
* \author shaofeng
* \par GENERAL DESCRIPTION:
*       This file provide the configuration MACRO of lcm driver. \n
*       it defines the back light mode, reset, scan mode and lcd xy length, etc. \n
*       
* \par DECLARATIONS:
*       (c) Copyright 2002-2004, Actions Co,Ld. All Right Reserved.
*
* \version 1.0
* \date 2007/11/25
*/

#ifndef  _LCM_CONFIG_H
#define _LCM_CONFIG_H

/*LCM模块配置宏定义，关系到全局设置，更改前务必先弄清楚每项的确切含义*/

/********************************************************************************************************************
*********************研发专用DEMO小机背光及复位项配置，定义对应的一项，注释其他两项**********
********************************客户定制方案三项全注释，下方另行配置背光及复位************************
*********************************************************************************************************************/
//#define   ACT2135_DEMO_VERSION_0_1      /* DEMO_NOYA_E                                2007-05 */
//#define   ACT2135_DEMO_VERSION_0_2      /* DEMO_NOYA_F_or_O                     2007-09 */
//#define   ACT2137_DEMO_VERSION_0_1      /* DEMO_NOYA_II_2.4inch                 2007-06 */
//#define   ACT2137_DEMO_VERSION_0_2      /* DEMO_NOYA_II_2.8inch                 2008-01 */


#ifdef     ACT2135_DEMO_VERSION_0_1                  //pwm(DS9300) ctrl backlight,  RC circuit  reset
#define          BL_PWM 
#define          IC_DS9300
#elif defined(ACT2135_DEMO_VERSION_0_2)        //gpioB31 ctrl backlight,  A16 as reset pin
#define          BL_GPIO   
 #define         IC_DS9364                     
#define          GPIO_A_RESET
#define          RESET_PIN_NUM  16
#elif defined(ACT2137_DEMO_VERSION_0_1)        //pwm(DS9300) ctrl backlight,  B15 as reset pin
#define         BL_PWM 
#define         IC_DS9300
#define  GPIO_B_RESET
#define  RESET_PIN_NUM  15
#elif defined(ACT2137_DEMO_VERSION_0_2)        //pwm(G5121) ctrl backlight,  B15 as reset pin
#define         BL_PWM 
#define         IC_G5121
#define  GPIO_B_RESET
#define  RESET_PIN_NUM  15
#else       //客户自定义方案
    /*  配置下方两项*/
#endif
/*********************************************************************************************************************
*********************************************************************************************************************/


/********************************************************************************************************************
****三种背光控制方式，如下是配置项。从以下三项定义中选择所需的一项，注释掉两项 *
**********************************************************PWM****************************************************/
#define BL_PWM           /* 定义-----PWM控制（F版demo，noya2_2.4），背光IC以下二选一   ***/
//#ifdef             BL_PWM                 /*DS9300,G5121均为PWM控制型背光IC，输入占空比不一样，以下选一*/
//#define          IC_DS9300           /* power DC2DC driver IC selection. */
#define          IC_G5121              /* power DC2DC driver IC selection. */
//#endif
/*********************************************************GPIO*********************************************************/
//#define BL_GPIO           /* 定义-----GPIO脉冲控制（O版demo），默认使用GPIO_B31****************/
//#ifdef             BL_GPIO    /*DS9364，EDS5921均为脉冲控制型背光IC，输入脉冲时序不一样，以下二选一*/
//#define          IC_DS9364
//#define          IC_EDS5921
//#endif
/*********************************************************DC2DC*******************************************************/
//#define BL_DC2DC       /*  定义-----内部DC-DC加外围电路控制（如力盛屏），无须外接背光控制IC */

/*********************************************************************************************************************
*********************************************************************************************************************/



/**********************************************************************************************************************
RESET引脚定义。若不用GPIO_A/B进行RESET（F版demo），下列三项全部注释；O版demo需定义一、三项
***********************************************************************************************************************/
//#define  GPIO_A_RESET              /*若RESET使用的GPIO_A，则定义此项，注释掉第二项*/
#define  GPIO_B_RESET              /*若RESET使用的GPIO_B，则定义此项，注释掉第一项*/
#define  RESET_PIN_NUM  31   /* RESET使用的GPIO_A/B 里的引脚编号 ，配合上两项之一来指定复位引脚*/
/***********************************************************************************************************************
***********************************************************************************************************************/


/**********************************************************************************************************************
********************************************************横屏或竖配置**********************************************
******* 配置GUI默认显示风格，正视觉时长边水平放置则为GUI_SIDELONG，否则为GUI_UPRIGHT
**********************************************************************************************************************/
/*二选一*/
#define   GUI_SIDELONG
//#define   GUI_UPRIGHT
                                      

     
/***************************************************************************************************************
**************************************************数据宽度配置*********************************************
******************************************************************************************/                       
/*!
* \def RGB_DATA_WIDTH_XX
* the width of lcd data bus. \n
* \li "XX"=="16BIT" ,the width is 16 bit.
* \li "XX"=="18BIT", the width is 18 bit.
*/
//#define  DATA_WIDTH_18BIT          /* 屏接口数据宽度：  18位*/
#define  DATA_WIDTH_16BIT        /* 屏接口数据宽度：  16位*/
/********************************************************************************************************************
********************************************************************************************************************/



/********************************************************************************************************************
****************************************************屏接口类型配置*********************************************
********************************************************************************************************************/ 
/*!
* \def CMD_DATA_SPLIT
* the mode of write lcd register. \n
* \li define      the address and data of register are write one time, such as foxconn.
* \li undefine the address and data of register are write separatly, such as ILI9320.
*/
 /* 取决于屏驱动IC内部寄存器值的更新方式： 定义---ILI9320类方式， 不定义---foxconn类方式
                            ILI9320类  ： 先送寄存器地址，再送寄存器值，完成一次寄存器写操作
                            foxconn类： 寄存器地址和寄存器值拼成16位数据一起送出完成一次寄存器写操作*/
#define  CMD_DATA_SPLIT              
/******************************************************************************************************************
******************************************************************************************************************/



/*!
* \def H_XRES
* the xy length, when lcd is placed sidelong.
*/
#define H_XRES 320

/*!
* \def H_YRES
* the xy length, when lcd is placed sidelong.
*/
#define H_YRES 240

/*!
* \def S_XRES
* the xy length, when lcd is placed upright.
*/
#define S_XRES   H_YRES

/*!
* \def S_YRES 
* the xy length, when lcd is placed upright.
*/
#define S_YRES   H_XRES


/*!
* \def DEFAUT_BACKLIGHT_LEVEL 
* the default back light intensity.
*/
#define DEFAUT_BACKLIGHT_LEVEL 3

#define MODIFY_MATCH_ID  0xcccc2000      /* for modify tool only */

#define ENTER_ADFU_GPIO_NUM  8
#define ENTER_STUB_GPIO_NUM  9
#endif

