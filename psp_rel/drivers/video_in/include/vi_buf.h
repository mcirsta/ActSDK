/*
********************************************************************************
*                   213x --- video in device driver
*                (c) Copyright 2007 - 2010, Actions Co,Ld. 
*                        All Right Reserved 
*
* History:        
*      <author>      <time>       <version >    <desc>
*       houjingkun      2008/06/18     1.0         build this file 
*       new added vi_buf.h and vi_buf.c for video in device driver.
********************************************************************************/ 
/*!
 * \file   vi_buf.h
 * \brief  
 *      video in device driver's buffer management function.
 * \author houjingkun
 * \par GENERAL DESCRIPTION:
 *      video in device driver's buffer management function. 
 *      just called by dirver itself.
 * \par EXTERNALIZED FUNCTIONS:
 *       null
 *
 *     Copyright(c) 2008-2012 Actions Semiconductor, All Rights Reserved.
 *
 * \version 1.0
 * \date  2008/06/18
 *******************************************************************************/

#ifndef _VI_BUF_H_
#define _VI_BUF_H_

#include "video_in.h"
#include "vi_config.h"
#include "vi_sys.h"
#include "vi_common.h"

/**** buffer management, realized in vi_buf.c. ****/
int vi_mem_init(vi_dev_t *pDev);

int vi_mem_exit(vi_dev_t *pDev);

/* for video_in gathered buffer queue management. */
/* vi_xxx means video in xxx. */
int vi_reset_queue(void);
int vi_que_whether_full(void);      /* if return 0, the vi queue isn't full, else is full. */

/* for output to middleware buffer queue management. */
/* mmm_xxx means media midware management. */
int mmm_reset_queue(void);

/* when one new frame is received, update the buffer queue's state. */
int do_update_frame(vi_dev_t *pDev);

unsigned long get_frame(int type, vi_dev_t *pDev);

int free_frame(unsigned long offset, vi_dev_t *pDev);

#endif  /* _VI_BUF_H_ */

