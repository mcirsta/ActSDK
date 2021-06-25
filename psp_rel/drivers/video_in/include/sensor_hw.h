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
 * \file   sensor_hw.h
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
 * \version 1.0
 * \date  2008/06/18
 *******************************************************************************/

#ifndef _SENSOR_HW_H_
#define _SENSOR_HW_H_

#include "vi_config.h"
#include "vi_common.h"

/******************* hardware configure. **************************/

#define Multi_CON1_VAL  0x80000001

#ifdef  _USE_I2C_PIN_1_
#define I2CBASEADDR_1   0xB0180000
#define I2CBASEADDR     I2CBASEADDR_1
#define I2C_1_PIN       0x00000030
#define I2C_PIN         I2C_1_PIN

/* I2C and BT656's related pin should be set enable. */
#define SET_SENSOR_GPIO(x)    ((x) | Multi_CON1_VAL)
#define SET_SENSOR_I2C(x)    (((x) | Multi_CON1_VAL) & (~(0x3 << 4)))	/* use i2c_1 */
#endif  /* _USE_I2C_PIN_1_ */

#ifdef  _USE_I2C_PIN_2_
#define I2CBASEADDR_2   0xB0180020
#define I2CBASEADDR     I2CBASEADDR_2
#define I2C_2_PIN       0x000000C0
#define I2C_PIN         I2C_2_PIN
/* I2C and BT656's related pin should be set enable. */
#define SET_SENSOR_GPIO(x)	((x) | Multi_CON1_VAL)	/* use i2c_2 */
#define SET_SENSOR_I2C(x)	(((x) | Multi_CON1_VAL) & (~(0x3 << 6)) | (0x2 << 6))	/* use i2c_2 */

#endif  /* _USE_I2C_PIN_2_ */

#define I2CCTL      (0x00 + I2CBASEADDR)
#define I2CDIV      (0x04 + I2CBASEADDR)
#define I2CSTA      (0x08 + I2CBASEADDR)
#define I2CADDR     (0x0c + I2CBASEADDR)
#define I2CDAT      (0x10 + I2CBASEADDR)

/************* end of sensor's hardware configure. ****************/

#define SDRAM_CTL       0xB0070000

///* YUV2RGB model */
//#define YUV2RGBBaseAddr         0xb00f0000
//#define YUV2RGB_Control         (YUV2RGBBaseAddr + 0x00)
//#define YUV2RGB_FifoData        (YUV2RGBBaseAddr + 0x04)
//#define YUV2RGB_CLK             (YUV2RGBBaseAddr + 0x08)
//#define YUV2RGB_Counter         (YUV2RGBBaseAddr + 0x0c)

/* ITU656 */
#define ITU_CLK_CTL     0xB0010024
#define ITU656_BASE     0xB00D0000

/* ITU656 PORT General Register */
#define IC63_BT_MODESEL        (ITU656_BASE +0x00000000)
#define IC63_VDCSTIFIFODATA  (ITU656_BASE + 0x00000004)
/* cmos sensor related register */
#define IC63_CSICTL          (ITU656_BASE + 0x00000008)
#define IC63_CSIHSPOS        (ITU656_BASE + 0x0000000C)
#define IC63_CSIHEPOS        (ITU656_BASE + 0x00000010)
//#define IC63_VDIVSEPOF       (ITU656_BASE + 0x00000014)
//#define IC63_VDIVSEPEF       (ITU656_BASE + 0x00000018)
#define IC63_CSIVSPOS        (ITU656_BASE + 0x0000001C)
#define IC63_CSIVEPOS        (ITU656_BASE + 0x00000020)
#define IC63_CSIRQSTA        (ITU656_BASE + 0x00000028)
#define IC63_CSITUXYDATA     (ITU656_BASE + 0x0000002C)
#define IC63_CSIFIFOCTL      (ITU656_BASE + 0x00000030)

/* ITU656 PORT General Register */
#define IC96_BT_MODESEL        (ITU656_BASE + 0x00000000 + 0x20)
#define IC96_VDCSTIFIFODATA  (ITU656_BASE + 0x00000004 + 0x20)
/* cmos sensor related register */
#define IC96_CSICTL          (ITU656_BASE + 0x00000008 + 0x20)
#define IC96_CSIHSEPOS        (ITU656_BASE + 0x0000002C)
//#define IC96_VDIVSEPOF       (ITU656_BASE + 0x00000014 + 0x20)
//#define IC96_VDIVSEPEF       (ITU656_BASE + 0x00000018 + 0x20)
#define IC96_CSIVSEPOS        (ITU656_BASE + 0x00000030)
#define IC96_CSIRQSTA        (ITU656_BASE + 0x00000038)
//#define IC96_CSITUXYDATA     (ITU656_BASE + 0x0000002C + 0x20)
#define IC96_CSIFIFOCTL      (ITU656_BASE + 0x0000003c)

#define  DMA_VIDEO4                  (0xB0060180 + 0x00000018)

//#define  PMU_CTL                     (0xB0000000 + 0x00000000)
//#define  CMU_DEVCLKEN                (0xB0010000 + 0x00000080)
//#define  CMU_DMACLK                  (0xB0010000 + 0x00000030)
//#define  CMU_BTCLK                   (0xB0010000 + 0x00000024)

//#define  DMA_MODE4                   (0xB0060180 + 0x00000000)
//#define  DMA_SRC4                    (0xB0060180 + 0x00000004)
//#define  DMA_DST4                    (0xB0060180 + 0x00000008)
//#define  DMA_CNT4                    (0xB0060180 + 0x0000000C)
//#define  DMA_REM4                    (0xB0060180 + 0x00000010)
//#define  DMA_CMD4                    (0xB0060180 + 0x00000014)


/* support core clk type. */
enum 
{
    CPU_120MHZ = 0,
    CPU_144MHZ,
    CPU_168MHZ,
    CPU_60MHZ,
    CPU_96MHZ,
};

typedef struct 
{
    unsigned int slave_addr;
    unsigned int reg_addr;
    unsigned int reg_data;
} sensor_i2cinfo_t;

/* cmos sensor related register address. */
extern unsigned int M_BT_MODESEL;
extern unsigned int M_VDCSTIFIFODATA;
extern unsigned int M_CSICTL;
extern unsigned int M_CSIHSPOS;
extern unsigned int M_CSIHEPOS;
//extern unsigned int VDIVSEPOF;
//extern unsigned int VDIVSEPEF;
extern unsigned int M_CSIVSPOS;
extern unsigned int M_CSIVEPOS;
extern unsigned int M_CSIRQSTA;
//extern unsigned int CSITUXYDATA;
extern unsigned int M_CSIFIFOCTL;

#define TIME_VAL    3       //unit: second

extern int get_sensor_dev(unsigned int wait_type);  /* get sensor device's pin resource. */

extern int release_sensor_dev(void);    /* release sensor device's pin resource. */

extern int get_sensor_i2c(void);

extern int release_sensor_i2c(void);

/* cyclely query dma untill it's not busy, if success, return 0. */
extern int cycle_query_dma(unsigned int dma, unsigned int val);  

extern int sensor_hw_init(vi_dev_t *pDev);  /* init hardware resource. */

extern int sensor_hw_exit(vi_dev_t *pDev);  /* release hardware resource. */

extern int sensor_init_work_mode(unsigned int type); /* init sensor work type.(DC, DV, PC Camera or others.  */

extern int dma_mode_start(unsigned int dma_no);

#endif  /* _SENSOR_HW_H_ */

