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
 * \file   fm_inner.h
 * \brief  inner functions to implemet user ioctl
 * \author xkping
 * \par GENERAL DESCRIPTION:
 *       In This File 
 *       1) implement functions for really funtions    
 * \par EXTERNALIZED FUNCTIONS:
 *     Copyright(c) 2001-2007 Actions Semiconductor, All Rights Reserved.
 *
 *  \version 1.0
 *  \date  2008/7/3
 *******************************************************************************/

#ifndef __FM_INNER_H__
#define __FM_INNER_H__

#include "fm_sys.h"
 
 
int act_radio_open(void);
ssize_t act_radio_read(void);
ssize_t act_radio_write(void);
int act_radio_release(void);
int act_radio_ioctl( unsigned int cmd,unsigned long param);

extern int threshold;
extern struct radio_info_t radio_info;
#endif 

