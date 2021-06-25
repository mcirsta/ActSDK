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
* \brief  ���ļ����ڰ�����������,���������и������ر�˵����
*       adc����      ͨ��lradc�����İ���,Ҳ�ǳ�˵���߿ذ���
*       ��GPIO����   �ð�����һͷ����GPIO input���,��һͷ������VCC or ��������
*       GPIO���󰴼� �ð�����һͷ����GPIO input���,��һͷGPIO output����input��output��ȣ����ʾ�ð������� 
*       ����ɨ�谴�� ����IC�ṩ��3*3����ɨ�谴������ʵ��ʹ�õ�3*3�ľ������Ӽ���
*       ��Ҫ�޸�key_hw_scan_matrix.c�к궨��KEY_CON_INIT 
* \author   xkping           
* \version 1.0
* \date  2009/05/26
*******************************************************************************/

#ifndef __KEY_CONFIG_H__
#define __KEY_CONFIG_H__

/******����4������Ҫ��: ���и����͵İ���,����enable����Ϊ1,��������Ϊ0*/
/*****���и����Ͱ�������Ҫ�����������Ӧ��������*************************/
#define ADC_KEY_ENABLE          1   /*adc����,     1:��,0:��;  ������2*/

#define SINGLE_GPIO_KEY_ENABLE  1   /*��GPIO����,  1:��,0:��;  ������3*/

#define GPIO_MATRIX_KEY_ENABLE  1   /*GPIO���󰴼�,1:��,0:��;  ������4*/

#define SCAN_MATIRX_KEY_ENABLE  0   /*����ɨ�谴�� 1:��,0:��;  ������5*/


/****������2 adc ��������**/
#if(ADC_KEY_ENABLE==1)
/*!
*\brief
     ADC�߿ذ�����ʵ�ʸ����������������������Ҳ��lradc����������ǰĬ�����֧��6����������6������Ҫ�޸�key_hw_adc.c�ļ� 
*/
#define ADC_KEY_NUM    (6)
/*!
*\brief
*   adcֵ �� ��ֵ ��ӳ��. �����:lradc_key_map_t���ݽṹ
*   ��  ADC_KEY1    {2, 3, KEY_NEXT}Ϊ��, ˵��ӳ���ϵ:
*   adc value ��2<=adc_val<=3��Χ��ʱ,��ֵΪKEY_NEXT.
*   ��ֵΪKEY_NULLʱ, ��ʾ��ӳ����Ч.
*   
*/
#define ADC_KEY0    {0, 3, KEY_VSUB}
#define ADC_KEY1    {4, 12, KEY_CAMERA}
#define ADC_KEY2    {13, 20, KEY_VADD}
#define ADC_KEY3    {21, 28, KEY_MENU}
#define ADC_KEY4    {29, 36,KEY_RETURN}
#define ADC_KEY5    {37, 44, KEY_NEXT} //��Чӳ��
/*!
*\brief
     û�а���ADC����ʱ��ADCֵ*/
#define ADC_NULL_VAL    (0x3F)

#endif /* 'end of  '#if(ADC_KEY_ENABLE ==1)'*/
/***������2 over***/
/*..............................................................*/



/****������3 ��GPIO�������� *****/
#if(SINGLE_GPIO_KEY_ENABLE == 1)
/*!
*\brief
     ��GPIO������ʵ�ʸ�������ǰĬ�����֧��8����������8������Ҫ�޸�key_hw_single_gpio.c�ļ� 
*/
#define SINGLE_GPIO_KEY_NUM (3)
/*!
*\brief
*   �����:single_gpio_key_map_t���ݽṹ
*   #define SINGLE_GPIO_KEY0    {GPIO_GROUP_A, 8, 0,KEY_TVOUT}˵��ӳ���ϵ:
*   gpio_a 8��Ϊinput,��gpio_a 8���Ϊ0���ͣ�,���ʾ�ü������£��ü�ӳ��ΪKEY_TVOUT
*   ��ֵΪKEY_NULLʱ, ��ʾ��ӳ����Ч.
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
/*****������3 over******/
/*..............................................................*/


/**������4 GPIO���󰴼� **/
#if(GPIO_MATRIX_KEY_ENABLE ==1)
/*!
*\brief
     GPIO���󰴼���ʵ�ʸ�������ǰĬ�����֧��6����������6������Ҫ�޸�key_hw_gpio_matrix.c�ļ� 
*/

#define GPIO_MATRIX_KEY_NUM (1)
/*!
*\brief
*   �����:gpio_matrix_key_map_t���ݽṹ
*   GPIO_MATRIX_KEY0    { GPIO_GROUP_A, 8, 0,GPIO_GROUP_A, 9, 0,KEY_MENU}˵��ӳ���ϵ:
*   gpio_a 8��Ϊoutput�����Ϊ0��gpio_a 9��Ϊinput,��gpio_a 9�õ�ƽΪ0����ü���������ӳ��ΪKEY_MENU
*   ��ֵΪKEY_NULLʱ, ��ʾ��ӳ����Ч.
*   
*/
#define GPIO_MATRIX_KEY0    { GPIO_GROUP_A, 9, 0,GPIO_GROUP_A, 10, 0,KEY_DELETE}
#define GPIO_MATRIX_KEY1    { GPIO_GROUP_A, -1, 0,GPIO_GROUP_A, -1, 0,KEY_NULL}
#define GPIO_MATRIX_KEY2    { GPIO_GROUP_A, -1, 0,GPIO_GROUP_A, -1, 0,KEY_NULL}
#define GPIO_MATRIX_KEY3    { GPIO_GROUP_A, -1, 0,GPIO_GROUP_A, -1, 0,KEY_NULL}
#define GPIO_MATRIX_KEY4    { GPIO_GROUP_A, -1, 0,GPIO_GROUP_A, -1, 0,KEY_NULL}
#define GPIO_MATRIX_KEY5    { GPIO_GROUP_A, -1, 0,GPIO_GROUP_A, -1, 0,KEY_NULL}

#endif
/***������4 over****/
/*..............................................................*/



/***������5 ����ɨ�谴��******/
#define KEY_CTL_INIT 0x00F30F05  
/*!
*\brief
     ����ɨ�谴�� �� ����ֵ��ӳ�� */
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


/******������5 over********/
/*..............................................................*/

/*��ʱ��ɨ��������λms*/
#define KEY_SCANE_TIME_INTERVAL_REALLY  (30) //��λ��ms

#define KEY_SCANE_TIME_INTERVAL (KEY_SCANE_TIME_INTERVAL_REALLY/10)

#endif  /* __KEY_CFG__ */

