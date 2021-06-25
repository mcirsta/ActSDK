/********************************************************************************
 *                              USDK(1100)
 *                             Module: drv 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-05-21          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     operate_gpio.h
* \brief    
* \author   shaofeng
*               
* \version 1.0
* \date  2009/05/21
*******************************************************************************/

#ifndef __OPERATE_GPIO_H__
#define __OPERATE_GPIO_H__


typedef enum
{
    GPIO_GROUP_A = 0,
    GPIO_GROUP_B,
}gpio_group_e;

typedef struct
{
    gpio_group_e gpio_group;
    //·¶Î§: 0-31
    int pin_num;
}gpio_pin_t;

extern int gpio_enable_output( gpio_group_e gpio_group, int pin_num );
extern int gpio_enable_input( gpio_group_e gpio_group, int pin_num );
extern int gpio_set_state( gpio_group_e gpio_group, int pin_num, int state );
extern int gpio_get_state( gpio_group_e gpio_group, int pin_num );

#endif  /* __OPERATE_GPIO_H__ */

