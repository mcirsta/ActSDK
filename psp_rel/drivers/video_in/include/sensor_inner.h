/*
********************************************************************************
*                   213x --- sensor's driver
*                (c) Copyright 2007 - 2010, Actions Co,Ld. 
*                        All Right Reserved 
*
* History:        
*      <author>      <time>       <version >    <desc>
*       houjingkun      2008/06/18     1.0         build this file 
********************************************************************************/ 
/*!
 * \file   sensor_inner.h
 * \brief  
 *      Sensor driver logic layer's API, used in driver inner.
 * \author houjingkun
 * \par GENERAL DESCRIPTION:
 *      Sensor driver's internal API, just called by dirver itself.
 *      some functions realized in sensor_module.c and sensor_inner.c.
 * \par EXTERNALIZED FUNCTIONS:
 *       null
 *
 *     Copyright(c) 2008-2012 Actions Semiconductor, All Rights Reserved.
 *
 * \version 1.0
 * \date  2008/06/18
 *******************************************************************************/

#ifndef _SENSOR_INNER_H_
#define _SENSOR_INNER_H_

#include "vi_config.h"
#include "video_in.h"
#include "vi_sys.h"
#include "vi_common.h"
#include "sensor_module.h"
/*
 ********************************************************
 * realized in sensor_inner.c
 ********************************************************
*/
extern int ss_sensor_open(unsigned int type);

extern int ss_sensor_init(unsigned int type);

extern int ss_sensor_exit(void);

extern int ss_sensor_close(unsigned int type);

extern int ss_sensor_ioctl(unsigned int cmd, unsigned long arg);

extern  unsigned int current_i2c_div;
#endif  /* _SENSOR_INNER_H_ */

