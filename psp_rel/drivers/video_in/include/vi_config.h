/*
********************************************************************************
*                   213x --- video in device driver
*                (c) Copyright 2007 - 2010, Actions Co,Ld. 
*                        All Right Reserved 
*
* History:        
*      <author>      <time>       <version >    <desc>
*       houjingkun      2008/06/25     1.0         build this file 
*       new added vi_config.h for driver's configurable.
********************************************************************************/ 
/*!
 * \file   vi_config.h
 * \brief  
 *      video_in device driver's configure file.
 * \author houjingkun
 * \par GENERAL DESCRIPTION:
 *      If Sensor and tvin driver's some hardware configure is changed, or something
 *      system related is changed, such as memory allocating, this file is need to
 *      be modified.
 *
 *     Copyright(c) 2008-2012 Actions Semiconductor, All Rights Reserved.
 *
 * \version 1.0
 * \date  2008/06/25
 *******************************************************************************/

#ifndef _VI_CONFIG_H_
#define _VI_CONFIG_H_

#include "video_in.h"

#define I2C_CLK                  100                //µ¥Î»k  
/*************  system config  *************/
#define _ACT_UC_OS_
//#define _ACT_LINUX_KERNEL_2_6_
/**********  end of system config  *********/


//#define _SS_DEBUG_  printf("%s: %d \n", __FILE__, __LINE__);

/*******************************************************************************/
/**********************  memory management configure.  *************************/
/*******************************************************************************/

#define _UCOS_NEW_DMA_MEM_MALLOC_     /* use new virtual memory management policy in 1017 sdk. */

/* the max frame data size in supported resolution, ensure to be page aligned. */
#define MAX_DC_BUF_SIZE    (1280 * 1024 * 2)

/* default frame video data's size in dv or preview mode. */
#define MAX_DV_FRAME_SIZE   (640 * 480 * 2) 

/* default pc camera frame data's size in pc camera mode. */
#define MAX_PC_CAMERA_FRAME_SIZE   (640 * 480 * 2) 

/* default small memory frame data's size in DC and DV mode. */
#define MAX_SMALL_MEMORY_FRAME_SIZE   (320 * 240 * 2 * 5)

#define BUF_CNT  5      /* the framebuffer count. */
#define QUEUE_CNT  (BUF_CNT - 2)    /* the framebuffer's count that is ready in queue. */
#define OUTPUT_CNT  2   /* OUTPUT_CNT should set no more than (QUEUE_CNT - 1). */  


#ifdef _ACT_UC_OS_

#ifdef  _UCOS_NEW_DMA_MEM_MALLOC_
#define VI_RES_224X176_PHY_ADDR_OFFSET       0x0
#define VI_RES_320X240_PHY_ADDR_OFFSET       0x0
#define VI_RES_640X480_PHY_ADDR_OFFSET       0x0

#else
#define VI_RES_224X176_PHY_ADDR_OFFSET       0x2ee000
#define VI_RES_320X240_PHY_ADDR_OFFSET       0x2ee000
#define VI_RES_640X480_PHY_ADDR_OFFSET       0x0
//#define SENSOR_BUFF_ADDR              0xa0418000  
#define SENSOR_BUFF_ADDR                0xa024d000  
#endif  /* _UCOS_NEW_DMA_MEM_MALLOC_ */
#endif  /* _ACT_UC_OS_ */

#ifdef _ACT_LINUX_KERNEL_2_6_
#define VI_RES_224X176_PHY_ADDR_OFFSET       0x0
#define VI_RES_320X240_PHY_ADDR_OFFSET       0x0
#define VI_RES_640X480_PHY_ADDR_OFFSET       0x0
#endif  /* _ACT_LINUX_KERNEL_2_6_ */

/*******************************************************************************/
/********************  other sys procession configure.  ************************/
/*******************************************************************************/
/* 
 * should be defined in old version, for api such as sensor_open, sensor_ioctl, etc. 
 * if driver is used in posix version, such as 1100 version, please undefine it.
 */
//#define	_USE_OLD_SENSOR_API_    

/* request irq processing function in install. else will request in vi_open. */
/* there is something wrong if set to 0 (request irq in open) in 5002. */
#define _REQUEST_IRQ_IN_INSTALL_    0    


/*******************************************************************************/
/**********************  sensor's hardware configure.  *************************/
/*******************************************************************************/
/* select a ic type. */
#define _IC_TYPE_   0x0     /* set driver itself adaptive ic type, it need getICType() is provided. */
//#define _IC_TYPE_   0x63  /* set ic type fix to 3996. */
//#define _IC_TYPE_   0x96  /* set ic type fix to 3963 or 5002. */

#define _USE_I2C_PIN_1_     /* use i2c_0 pin */
//#define _USE_I2C_PIN_2_     /* use i2c_1 pin */

//#define SENSOR_GPIO_A_FLASHLIGHT
#define SENSOR_GPIO_B_FLASHLIGHT
//#define SENSOR_FLASHLIGHT_GPIO_NUM 14

#ifdef SENSOR_GPIO_A_FLASHLIGHT
#define GPIO_FLASHLIGHT_OUTEN GPIO_AOUTEN
#define GPIO_FLASHLIGHT_INEN  GPIO_AINEN
#define GPIO_FLASHLIGHT_DAT GPIO_ADAT
#endif
#ifdef SENSOR_GPIO_B_FLASHLIGHT
#define GPIO_FLASHLIGHT_OUTEN GPIO_BOUTEN
#define GPIO_FLASHLIGHT_INEN  GPIO_BINEN
#define GPIO_FLASHLIGHT_DAT GPIO_BDAT
#endif


/*******************************************************************************/
/***********************  tvin's hardware configure.  **************************/
/*******************************************************************************/

#define ADD_TVIN_DRV     1      /* add tvin function in video_in driver or not. */

/*
 * if set to 0, the video's in and out's buffer will always be the same.
 * set 0 is just used in test currently.
 */
#define _TVIN_USE_MULTI_FRAME_BUF_	1

/* select a tvin ic module. */
//#define ADI_7180
#define TI_5150

//#define TVIN_GPIO_B_STANDBY
//#define TVIN_GPIO_A_STANDBY

#define TVIN_STANDBY_GPIO_NUM	14
/* default tvin frame data's size in tvin mode. */
#define MAX_TVIN_FRAME_SIZE     (360 * 288 * 2) 
//#define TVIN_FRAME_SIZE  0x00032a00      /* PAL: 720 * 576 * 2 = 0x000ca800 */
//#define TVIN_FRAME_SIZE  0x000b2200      /* NTSC: 760 * 480 * 2 = 0x000b2200 */

#define DEFAULT_TVIN_MODE       VI_TVIN_RES_PAL360X288

#define TVIN_PHY_ADDR_OFFSET      0x0

#define TVIN_PIXEL_BITMAP     ((0x1 << VI_TVIN_RES_PAL360X288) | (0x1 << VI_TVIN_RES_PAL352X288) | (0x1 << VI_TVIN_RES_NTSC360X240) | (0x1 << VI_TVIN_RES_NTSC352X240))

#endif  /* _VI_CONFIG_H_ */

