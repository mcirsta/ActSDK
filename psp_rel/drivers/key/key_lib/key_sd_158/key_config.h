/********************************************************************************
 *                              USDK(1100)
 *                             Module: drv 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      xkping    2009-05-26          v1.0               build this file 
 ********************************************************************************/
/*!
* \file    key_cfg.h
* \brief  本文件用于按键驱动配置,按键驱动中概念需特别说明：
*       adc按键      通过lradc采样的按键,也是常说的线控按键
*       单GPIO按键   该按键的一头是用GPIO input检测,另一头上拉到VCC or 下拉到地
*       GPIO矩阵按键 该按键的一头是用GPIO input检测,另一头GPIO output，若input与output相等，则表示该按键按下 
*       矩阵扫描按键 就是IC提供的3*3矩阵扫描按键，若实际使用的3*3的矩阵真子集，
*       需要修改key_hw_scan_matrix.c中宏定义KEY_CON_INIT 
* \author   xkping           
* \version 1.0
* \date  2009/05/26
*******************************************************************************/

#ifndef __KEY_CONFIG_H__
#define __KEY_CONFIG_H__

/******以下4项配置要点: 若有该类型的按键,则将其enable配置为1,否则配置为0*/
/*****若有该类型按键，还要继续配置其对应的配置项*************************/
#define ADC_KEY_ENABLE          1   /*adc按键,     1:有,0:无;  配置项2*/

#define SINGLE_GPIO_KEY_ENABLE  1   /*单GPIO按键,  1:有,0:无;  配置项3*/

#define GPIO_MATRIX_KEY_ENABLE  1   /*GPIO矩阵按键,1:有,0:无;  配置项4*/

#define SCAN_MATIRX_KEY_ENABLE  0   /*矩阵扫描按键 1:有,0:无;  配置项5*/


/****配置项2 adc 按键配置**/
#if(ADC_KEY_ENABLE==1)
/*!
*\brief
     ADC线控按键的实际个数（包括锁键，如果锁键也是lradc按键），当前默认最大支持6个，若超过6个，需要修改key_hw_adc.c文件 
*/
#define ADC_KEY_NUM    (6)
/*!
*\brief
*   adc值 与 键值 的映射. 请参照:lradc_key_map_t数据结构
*   以  ADC_KEY1    {2, 3, KEY_NEXT}为例, 说明映射关系:
*   adc value 在2<=adc_val<=3范围内时,键值为KEY_NEXT.
*   键值为KEY_NULL时, 表示该映射无效.
*   
*/
#define ADC_KEY0    {0, 3, KEY_VSUB}
#define ADC_KEY1    {4, 12, KEY_CAMERA}
#define ADC_KEY2    {13, 20, KEY_VADD}
#define ADC_KEY3    {21, 28, KEY_MENU}
#define ADC_KEY4    {29, 36,KEY_RETURN}
#define ADC_KEY5    {37, 44, KEY_NEXT} //无效映射
/*!
*\brief
     没有按下ADC按键时的ADC值*/
#define ADC_NULL_VAL    (0x3F)

#endif /* 'end of  '#if(ADC_KEY_ENABLE ==1)'*/
/***配置项2 over***/
/*..............................................................*/



/****配置项3 单GPIO按键配置 *****/
#if(SINGLE_GPIO_KEY_ENABLE == 1)
/*!
*\brief
     单GPIO按键的实际个数，当前默认最大支持8个，若超过8个，需要修改key_hw_single_gpio.c文件 
*/
#define SINGLE_GPIO_KEY_NUM (3)
/*!
*\brief
*   请参照:single_gpio_key_map_t数据结构
*   #define SINGLE_GPIO_KEY0    {GPIO_GROUP_A, 8, 0,KEY_TVOUT}说明映射关系:
*   gpio_a 8作为input,若gpio_a 8检测为0（低）,则表示该键被按下，该键映射为KEY_TVOUT
*   键值为KEY_NULL时, 表示该映射无效.
*   
*/
#define SINGLE_GPIO_KEY0    {GPIO_GROUP_A, 8, 0,KEY_OK}
#define SINGLE_GPIO_KEY1    {GPIO_GROUP_A, 9, 0,KEY_PREV}
#define SINGLE_GPIO_KEY2    {GPIO_GROUP_A, 10,0,KEY_PLAYBACK}
#define SINGLE_GPIO_KEY3    {GPIO_GROUP_A, 0, -1,KEY_NULL}
#define SINGLE_GPIO_KEY4    {GPIO_GROUP_A, 0, -1,KEY_NULL}
#define SINGLE_GPIO_KEY5    {GPIO_GROUP_A, 0, -1,KEY_NULL}
#define SINGLE_GPIO_KEY6    {GPIO_GROUP_A, 0, -1,KEY_NULL}
#define SINGLE_GPIO_KEY7    {GPIO_GROUP_A, 0, -1,KEY_NULL}

#endif
/*****配置项3 over******/
/*..............................................................*/


/**配置项4 GPIO矩阵按键 **/
#if(GPIO_MATRIX_KEY_ENABLE ==1)
/*!
*\brief
     GPIO矩阵按键的实际个数，当前默认最大支持6个，若超过6个，需要修改key_hw_gpio_matrix.c文件 
*/

#define GPIO_MATRIX_KEY_NUM (1)
/*!
*\brief
*   请参照:gpio_matrix_key_map_t数据结构
*   GPIO_MATRIX_KEY0    { GPIO_GROUP_A, 8, 0,GPIO_GROUP_A, 9, 0,KEY_MENU}说明映射关系:
*   gpio_a 8作为output且输出为0，gpio_a 9作为input,若gpio_a 9得电平为0，则该键被按下且映射为KEY_MENU
*   键值为KEY_NULL时, 表示该映射无效.
*   
*/
#define GPIO_MATRIX_KEY0    { GPIO_GROUP_A, 9, 0,GPIO_GROUP_A, 10, 0,KEY_DELETE}
#define GPIO_MATRIX_KEY1    { GPIO_GROUP_A, -1, 0,GPIO_GROUP_A, -1, 0,KEY_NULL}
#define GPIO_MATRIX_KEY2    { GPIO_GROUP_A, -1, 0,GPIO_GROUP_A, -1, 0,KEY_NULL}
#define GPIO_MATRIX_KEY3    { GPIO_GROUP_A, -1, 0,GPIO_GROUP_A, -1, 0,KEY_NULL}
#define GPIO_MATRIX_KEY4    { GPIO_GROUP_A, -1, 0,GPIO_GROUP_A, -1, 0,KEY_NULL}
#define GPIO_MATRIX_KEY5    { GPIO_GROUP_A, -1, 0,GPIO_GROUP_A, -1, 0,KEY_NULL}

#endif
/***配置项4 over****/
/*..............................................................*/



/***配置项5 矩阵扫描按键******/
#define KEY_CTL_INIT 0x00F30F05  
/*!
*\brief
     矩阵扫描按键 与 按键值的映射 */
#define KIN0_KOU0   KEY_MENU
#define KIN0_KOU1   KEY_VADD
#define KIN0_KOU2   KEY_NEXT

#define KIN0_KOU3   KEY_NULL

#define KIN1_KOU0   KEY_OK
#define KIN1_KOU1   KEY_RETURN
#define KIN1_KOU2   KEY_OK

#define KIN1_KOU3   KEY_NULL

#define KIN2_KOU0   KEY_CAMERA
#define KIN2_KOU1   KEY_VSUB
#define KIN2_KOU2   KEY_PREV

#define KIN2_KOU3   KEY_NULL

#define KIN3_KOU0   KEY_NULL
#define KIN3_KOU1   KEY_NULL
#define KIN3_KOU2   KEY_NULL
#define KIN3_KOU3   KEY_NULL


/******配置项5 over********/
/*..............................................................*/

/*定时器扫描间隔：单位ms*/
#define KEY_SCANE_TIME_INTERVAL_REALLY  (30) //单位：ms

#define KEY_SCANE_TIME_INTERVAL (KEY_SCANE_TIME_INTERVAL_REALLY/10)

#endif  /* __KEY_CFG__ */

