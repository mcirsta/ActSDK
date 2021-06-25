/*
********************************************************************************
*                   213x --- usdk130 source file
*                (c) Copyright 2003 - 2008, Actions Co,Ld. 
*                        All Right Reserved 
*
* History:        
*      <author>      <time>       <version >    <desc>
*      houjingkun    2008/08/06     1.0      build this file 
*                                            ada char device driver's inner layer.
********************************************************************************/ 
/*!
 * \file   ada_inner.h
 * \brief  
 *      ada char device driver's inner layer.
 * \author houjingkun
 * \par GENERAL DESCRIPTION:
 * \par EXTERNALIZED FUNCTIONS:
 *       null
 *
 *  Copyright(c) 2003-2008 Actions Semiconductor, All Rights Reserved.
 *
 * \version 1.0
 * \date  2008/08/06
 *******************************************************************************/

#ifndef  __ADA_INNER_H__
#define  __ADA_INNER_H__

#include "ada.h"

/*
 * ada driver's private data structure. used for record device driver's state or 
 * some important data. 
 */
typedef struct 
{
    int open_cnt;
    unsigned int dac_pcm_rate;
    unsigned int dac_channel;
    unsigned int dac_pcm_bits;
    unsigned int adc_pcm_rate;
    unsigned int adc_channel;
    unsigned int adc_pcm_bits;
    adc_input_t adc_input;
    adc_gain_t adc_gain;
    unsigned int max_sys_volume;    
    unsigned int volume;
    unsigned int dac_mute;  /* dac_mute is set by user. */
    unsigned int dsp_mute;  /* dsp_mute is set by dsp driver. */
    unsigned int dma_no;
    unsigned long phy_addr;
    unsigned long buf_len;
    volatile int play_status;       /* 0 is stoped, 1 is playing. */
    volatile int task_pend;       /* if set to 1, means a task is request to play. */
    unsigned int *pcm_data[2];
    unsigned int pcm_bytes;
} ada_dev_t;

extern ada_dev_t ada_dev;

int ada_dev_init(void);

int ada_dev_exit(void);

int ada_init_dma(int dma_no);

int ada_inner_open(void);

int ada_inner_close(void);


//int dac_ioctl(unsigned int cmd, unsigned long arg);
//
//int adc_ioctl(unsigned int cmd, unsigned long arg);

int pcm_play(void);

void print_ada_reg(void);

#endif  /* __ADA_INNER_H__ */

