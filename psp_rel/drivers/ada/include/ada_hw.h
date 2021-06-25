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
 * \file   ada_hw.h
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

#ifndef __ADA_HW_H__
#define __ADA_HW_H__

#include "ada.h"

/**************************** some configure. **********************************/
#define SPEAKER_GPIO_OPT    0


/********************** Some register related define. **************************/
#if 0
/* controll the speaker enable or not. */
#define DAC_GPIO_EN_NUM 26
#define DAC_GPIO_EN_OUTEN   GPIO_AOUTEN
#define DAC_GPIO_EN_INEN    GPIO_AINEN
#define DAC_GPIO_EN_DAT     GPIO_ADAT

/* detect the headphone input or not. */
#define DAC_GPIO_DETECT_NUM 12
#define DAC_GPIO_DETECT_OUTEN   GPIO_AOUTEN
#define DAC_GPIO_DETECT_INEN    GPIO_AINEN
#define DAC_GPIO_DETECT_DAT     GPIO_ADAT

#define ADC_DATA_PHYS               (ADC_DATA & 0x1fffffff)  

#define ADC_CON_INIT                0x8b84bdfa  //mic input
#define ADC_FIFO_CON_INIT           0x201   //apb_en, dsp_disable.... set before:0x00000155
#define ADC_DATA_INIT               0x00000008 
#define ADC_ANALOG_INIT             0x0000000C

#define DMA_IRQ_NUM 12
#define ACT_ADC_DMA_MOD             0x10840084  //sdram --> sdram
#define ACT_ADC_DMA_MOD             0x1084008c  //dsp ram --> sdram
#define ACT_ADC_DMA_MOD             0x1084013c  //adc fifo --> sdram
#endif
/********************** end of Some register define. ***************************/

#define _PA(x)                      (0x1fffffff & (unsigned int)(x))    /* set to physical address. */
#define _UNCACHE_ADDR(x)            ((0x1fffffff & (unsigned int)(x)) | 0xa0000000)    /* set to uncache address. */
#define _CACHE_ADDR(x)              ((0x1fffffff & (unsigned int)(x)) | 0x80000000)    /* set to cache address. */

#define ACT_DAC_DMA_MOD             0x01340084
#define ACT_DAC_DMA_RELOAD_MOD      0x11340084

/* hw_ioctl command. */
#define DAC_HW_SPEAKER_EN       1
#define DAC_HW_MUTE_EN          2
#define DAC_HW_DSP_EN           3   /* enable dsp access dac fifo. */

int ada_hw_init(void);
int ada_hw_exit(void);
int ada_hw_open(void);
int ada_hw_close(void);

int dac_hw_init(void);
int dac_hw_exit(void);
int dac_hw_set_volume(unsigned int volume);
int dac_hw_check_play_set(void);
int dac_hw_ioctl(unsigned int cmd, unsigned int param);


#define ADC_HW_MONITOR_EN   1

int adc_hw_init(void);
int adc_hw_exit(void);
int adc_hw_set_input(adc_input_t *p_adc_input);
int adc_hw_set_gain(adc_gain_t *p_adc_gain);
int adc_hw_ioctl(unsigned int cmd, unsigned int param);

#endif  /* __ADA_HW_H__ */

