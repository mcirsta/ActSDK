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
 * \file   sensor_module.h
 * \brief  
 *      Sensor driver logic layer's API, used in driver inner.
 * \author houjingkun
 * \par GENERAL DESCRIPTION:
 *      Sensor driver's internal API, just called by dirver itself.
 *      functions realized in sensor_module.c.
 * \par EXTERNALIZED FUNCTIONS:
 *       null
 *
 *     Copyright(c) 2008-2012 Actions Semiconductor, All Rights Reserved.
 *
 * \version 1.0
 * \date  2008/06/18
 *******************************************************************************/
#include "video_in.h"

#ifndef _SENSOR_MODULE_H_
#define _SENSOR_MODULE_H_

/*
 ********************************************************
 * realized in sensor_module.c
 ********************************************************
*/
extern unsigned int i2c_slave_addr;  /* i2c bus's slave_addr. */

extern unsigned int sensor_i2c_read(unsigned int reg_addr);  /* read sensor's control register. */
extern int sensor_i2c_write(unsigned int reg_addr, unsigned int reg_data);  /* write sensor's control register. */

extern int sensor_init_param_reg(void); /* init sensor module's register. */
extern int sensor_exit_param_reg(void); /* set sensor module to standby status. */

extern int sensor_change_resolution(vi_resolution_t cmd);

/* contrast value range: 0 ~ 6. */
extern unsigned int sensor_get_contrast(void);
extern int sensor_set_contrast(unsigned int contrast);

extern unsigned int sensor_get_white_balance(void);
extern int sensor_set_white_balance(unsigned int whitebalance);

extern unsigned int sensor_get_focus(void);
extern int sensor_set_focus(unsigned int focus);

extern int sensor_set_flashlight(unsigned intarg);

/* brightness value range: 0 ~ 6. */
extern unsigned int sensor_get_brightness(void);
extern int sensor_set_brightness(unsigned int brightness);

extern unsigned int sensor_get_expose_mode(void);
extern int sensor_set_expose_mode(unsigned int type);

extern unsigned int sensor_get_special_mode(void);
extern int sensor_set_special_mode(unsigned int type);

extern int sensor_set_pix_fmt(unsigned int type);

extern unsigned int sensor_get_pixel_bitmap(void);

extern unsigned int sensor_get_f_rate_bitmap(void);

extern int sensor_set_f_rate(unsigned int f_rate);

extern void  sensor_power_on(void);

extern void  sensor_power_off(void);

extern void   sensor_init_m_clk(void);

#endif  /* _SENSOR_INNER_H_ */

