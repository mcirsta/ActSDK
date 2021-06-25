/********************************************************************************
*                              ������д��Ŀ��
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

/*adc ����*/
typedef struct
{    
    int min_adc_val; /*! adc����ֵ������ */

    int max_adc_val; /*! adc����ֵ������ */
    /*! ��Ӧ����ֵ */
    key_val_e keyval;
}lradc_key_map_t;

/*��GPIO����*/
typedef struct
{
    /*! GPIO����*/
    gpio_pin_t input_gpio;
    
    /*! press����ʱ���������GPIO��ƽ�ߵ�*/
    int input_gpio_state;
    
    /*! ��Ӧ����ֵ */
    key_val_e keyval;
}single_gpio_key_map_t;

/*GPIO���󰴼�*/
typedef struct
{
    /*! �����GPIO����*/
    gpio_pin_t output_gpio;    
    /*! �����GPIO��ƽ�ߵ�*/
    int output_gpio_state; 

    /*! ������GPIO���� */
    gpio_pin_t input_gpio;    
    /*! press����ʱ���������GPIO��ƽ�ߵ�*/
    int input_gpio_state;
   
    /*! ��Ӧ����ֵ */
    key_val_e keyval;
}gpio_matrix_key_map_t;

/*IC�Դ���ɨ����󰴼�*/
extern int key_scan_matrix_init( void );
extern int key_scan_matrix_exit( void );
extern int get_scan_matrix_keyval( void );

/*ADC�����������߿ذ���*/
extern int key_adc_init(void);
extern int key_adc_exit(void);
extern int get_adc_keyval(void);

/*��GPIO������һ��gpio��Ӧһ������*/
extern int key_single_gpio_init( void );
extern int key_single_gpio_exit( void );
extern int get_single_gpio_keyval( void );

/*GPIO���󰴼���һ��GPIO�����룬һ��GPIO�����*/
extern int key_gpio_matrix_init( void );
extern int key_gpio_matrix_exit( void );
extern int get_gpio_matrix_keyval( void );
extern void key_init(void);
extern void key_exit(void);

/*! \endcond*/
#endif 

