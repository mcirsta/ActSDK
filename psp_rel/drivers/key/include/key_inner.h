/********************************************************************************
*                              这里填写项目名
*                            Module: KEY Drvier
*                 Copyright(c) 2001-2007 Actions Semiconductor,
*                            All Rights Reserved. 
*
* History:         
*      <author>    <time>           <version >             <desc>
*       xkping     2008-07-03 09:00     1.0             build this file 
********************************************************************************/
/*!
 * \file   key_inner.h
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

#ifndef __KEY_INNER_H__
#define __KEY_INNER_H__
/*! \cond KEY_API*/

#include "key.h" 
#include "key_sys.h"
#include "operate_gpio.h"
 
int act_buttons_open(void);
int act_buttons_read(char *buf, size_t count);
ssize_t act_buttons_write(void);
int act_buttons_release(void);
int act_buttons_ioctl( unsigned int cmd,unsigned long param);

/*adc 按键*/
typedef struct
{    
    int min_adc_val; /*! adc采样值，下限 */

    int max_adc_val; /*! adc采样值，上限 */
    /*! 对应按键值 */
    key_val_e keyval;
}lradc_key_map_t;

/*单GPIO按键*/
typedef struct
{
    /*! GPIO属性*/
    gpio_pin_t input_gpio;
    
    /*! press按键时，作输入的GPIO电平高低*/
    int input_gpio_state;
    
    /*! 对应按键值 */
    key_val_e keyval;
}single_gpio_key_map_t;

/*GPIO矩阵按键*/
typedef struct
{
    /*! 作输出GPIO属性*/
    gpio_pin_t output_gpio;    
    /*! 作输出GPIO电平高低*/
    int output_gpio_state; 

    /*! 作输入GPIO属性 */
    gpio_pin_t input_gpio;    
    /*! press按键时，作输入的GPIO电平高低*/
    int input_gpio_state;
   
    /*! 对应按键值 */
    key_val_e keyval;
}gpio_matrix_key_map_t;

/*IC自带的扫描矩阵按键*/
extern int key_scan_matrix_init( void );
extern int key_scan_matrix_exit( void );
extern int get_scan_matrix_keyval( void );

/*ADC采样按键：线控按键*/
extern int key_adc_init(void);
extern int key_adc_exit(void);
extern int get_adc_keyval(void);

/*单GPIO按键：一个gpio对应一个按键*/
extern int key_single_gpio_init( void );
extern int key_single_gpio_exit( void );
extern int get_single_gpio_keyval( void );

/*GPIO矩阵按键：一个GPIO做输入，一个GPIO作输出*/
extern int key_gpio_matrix_init( void );
extern int key_gpio_matrix_exit( void );
extern int get_gpio_matrix_keyval( void );
extern void key_init(void);
extern void key_exit(void);

/*! \endcond*/
#endif 

