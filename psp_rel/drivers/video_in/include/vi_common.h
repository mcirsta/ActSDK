/*
********************************************************************************
*                   213x --- video in device driver
*                (c) Copyright 2007 - 2010, Actions Co,Ld. 
*                        All Right Reserved 
*
* History:        
*      <author>      <time>       <version >    <desc>
*       houjingkun      2008/06/18     1.0         build this file 
*       new added vi_common.h and vi_common.c for unifying the sensor and tvin
*       application.
********************************************************************************/ 
/*!
 * \file   vi_common.h
 * \brief  
 *      video_in device driver's common API.
 * \author houjingkun
 * \par GENERAL DESCRIPTION:
 *      Sensor and tvin driver's common functions. it's video_in's common func.
 *      new added vi_common.h for unifying the sensor and tvin and such as video_in 
 *      application. it will be called by sensor_inner.c and tvin_inner.c.
 * \par EXTERNALIZED FUNCTIONS:
 *       null
 *
 *     Copyright(c) 2008-2012 Actions Semiconductor, All Rights Reserved.
 *
 * \version 1.0
 * \date  2008/06/18
 *******************************************************************************/

#ifndef _VI_COMMON_H_
#define _VI_COMMON_H_

#include "vi_config.h"
#include "vi_sys.h"

typedef struct 
{
    unsigned int ss_contrast; /* sensor's contrast value, default: 3 */
    unsigned int ss_brightness; /* sensor's brightness, default: 3 */
    unsigned int ss_expose_mode;  /* sensor's expose mode, default: AUTO_EXPOSURE */
    unsigned int ss_sp_effect; /* sensor's special effect, default: NONE_SPECIAL */
    unsigned int ss_white_balance; /* sensor's white balance effect, default: AUTO_BALANCE */
    unsigned int ss_focus;
} vi_effect_t;

/* sensor or tvin or some other video in device need structure. */
typedef struct 
{
    unsigned int app_type;     /* indicating the sensor's application type, 0 is DC, 1 is DV, 2 is PC Camera; */

    //unsigned int ic_type;
    unsigned int dma_chan; 
    /* dma related */
    unsigned int mem_len;
    unsigned long mem_addr;
    unsigned long phy_addr;
    unsigned long cur_dma_dst;      /* record current dma dest address. current no use. */

    /* framebuffer's count. */
    int buf_cnt;  /* the framebuffer count. */
    int queue_cnt;  /* the max framebuffer's count in ready queue. */
    int output_cnt; /* the max framebuffer's count in output queue, should set no more than (QUEUE_CNT - 1). */ 
         
    /* 
     * sensor resolution:
     * DC320X240, DC640X480, DC1024X768, DC1280X960, DC1600X1200, DC2048X1536
     */
    vi_resolution_t resolution;
    unsigned int resolution_len;
    unsigned int width;
    unsigned int height;
    unsigned int pix_fmt;  /* refer to struct ss_pix_fmt_t. */
    
    /* first_frame is used when getting a new resolution frame, we should get twice for stable. */
    unsigned char first_frame;
    unsigned char start_dma;
    unsigned char opened;   /* 0 is no app open sensor dev, 1 is opened already. */
    unsigned char dma_status;   /* indicating dma in reload mode or not, 0: normal; 1: reload; */
    unsigned char flag[BUF_CNT];        /* there are BUF_CNT buffers used for catching sensor's data. */
                    /* it's signed as: DATA_NONE, DATA_GETTING ..., see below's enum type. */
    unsigned long offset[BUF_CNT];    /* BUF_CNT buffer's offset from (phy_addr + phy_offset). */
    unsigned long phy_offset;   /* the first frame's offset from phy_addr. */
    volatile unsigned int wait_flag;  /* if 1, one process waiting for DATA_INQUEUE block; if 0, no process waiting. */
    unsigned long frame_cnt;    /* all input frames after start_cycle_update! */
    unsigned long cur_lost_frames;   /* all lost frames after start_cycle_update! */
    unsigned long pre_lost_frames;   /* all lost frames when last get_cycle_frame! */
    unsigned long pad_frames;   /* should be padded frame count between twice get_cycle_frame! */
    struct vi_effect_t * pEffect;
    void (* irq_func)(void);      /* irq hander. */
} vi_dev_t;     

/* catching sensor data buffer's status. */
enum 
{  
    DATA_NONE = 0, 
    DATA_GETTING, 
    DATA_READY,     /* current no use. */
    DATA_PREPARE, 
    DATA_OUTPUT, 
    DATA_INQUEUE, 
};

extern unsigned int ic_type;
extern vi_dev_t vi_dev;
extern vi_effect_t vi_effect;

#if defined(_ACT_UC_OS_)
extern unsigned int vi_app_type;
//void vi_dma_irq(unsigned int irq, void *dev_id, struct pt_regs *regs);
int vi_dma_irq(int irq, void *dev_id, struct pt_regs *reg);

#elif defined(_ACT_LINUX_KERNEL_2_6_)
extern uint vi_app_type;
extern wait_queue_head_t vi_wait;

int vi_dma_irq(int irq, void *dev_id, struct pt_regs *reg);
#endif

int vi_para_init(vi_dev_t *pDev);

int vi_para_exit(vi_dev_t *pDev);

int vi_common_open(unsigned int type);

int vi_common_close(unsigned int type);

int vi_capture_prepare(void);

int vi_capture_finish(void);


#endif  /* _VI_COMMON_H_ */

