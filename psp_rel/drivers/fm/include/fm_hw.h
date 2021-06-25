/********************************************************************************
*                              这里填写项目名
*                            Module: FM Drvier
*                 Copyright(c) 2001-2007 Actions Semiconductor,
*                            All Rights Reserved. 
*
* History:         
*      <author>    <time>           <version >             <desc>
*       xkping     2008-07-03 09:00     1.0             build this file 
********************************************************************************/
/*!
 * \file   fm_hw.h
 * \brief  inner functions to implemet user ioctl
 * \author xkping
 * \par GENERAL DESCRIPTION:
 *       In This File 
 *       1) FM IC related code,
 *       2) using i2c bus to control FM IC.       
 * \par EXTERNALIZED FUNCTIONS:
 *     Copyright(c) 2001-2007 Actions Semiconductor, All Rights Reserved.
 *
 *  \version 1.0
 *  \date  2008/7/3
 *******************************************************************************/
#ifndef __FM_HW_H__
#define __FM_HW_H__

#include "fm_config.h"
#include "fm_sys.h"
#include "radio.h"

int fm_init(int band_mode_init);

int fm_exit(void);

int fm_ioctl( unsigned int cmd,unsigned long param); 

#define GET_FMHW_STEREO_MODE   0x1001          // 1:stereo mode; 0 singled mode 获取电台频段模式
#define GET_FMHW_INTENSITY     0x1002          //获取电台的信号强度
#define GET_FMHW_BAND_MODE     0x1003          //获取电台波段

#define SET_FMHW_FREQ          0x1031        //设置收音频率
#define SET_FMHW_MUTE          0x1032        //设置MUTE
#define SET_FMHW_BAND_MODE     0x1033        // 0 :US mode; 1: jpan mode
#define SET_FMHW_ENTER_STANDBY 0x1034        //设置FM 进入standby 

extern int check_seek_status(struct radio_info_t * radio_info_check);
void fm_get_freq(struct radio_info_t * radio_info_getfreq);
extern int fm_seek_break(unsigned int param);
extern int check_linein(unsigned int *state);
#endif 

