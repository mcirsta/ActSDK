/*! \cond CHARGE_API*/
/*********************************************************************************
*                                       NOYA
*                            Module: charge driver
*                (c) Copyright 2003 - 2008, Actions Co,Ld. 
*                        All Right Reserved 
*
* History:        
*      <author>      <time>       <version >    <desc>
*       cccw        2008/09/11         1.0         build this file 
********************************************************************************/ 
/*!
 * \file   charge_drv.h
 * \brief  
 *      Some data structure defines and function declaration.
 * \author cccw
 * \par GENERAL DESCRIPTION:
 * \par EXTERNALIZED FUNCTIONS:
 *       null
 *
 *  Copyright(c) 2008-2012 Actions Semiconductor, All Rights Reserved.
 *
 * \version 1.0
 * \date  2008/09/11
 *******************************************************************************/
 
#ifndef __CHARGE_H__
#define __CHARGE_H__



/*!
 *    ioctl CMD �����ֶ��� \n  
 * 
 */
#define CHARGE_BASE                                        0x4800
#define CHARGE_IOSET_START                       (CHARGE_BASE+1)        /*��ʼ�������*/
#define CHARGE_IOSET_STOP                          (CHARGE_BASE+2)        /*ֹͣ�������*/
#define CHARGE_IOSET_PARAM                      (CHARGE_BASE+3)        /*���ó�����*/
#define CHARGE_IOGET_PARAM                      (CHARGE_BASE+4)       /*��ȡ������*/
#define CHARGE_IOSET_PROCESS                 (CHARGE_BASE+5)        /*��ʼ�������*/
#define CHARGE_IOGET_BATTERY                  (CHARGE_BASE+6)         /*��ȡ��ǰ����*/
#define CHARGE_IOGET_BATTERY_SHOW   (CHARGE_BASE+7)   /*��ȡ��ǰ����,������ƽ����������ʾͼ����*/
#define CHARGE_IOGET_DET_ADAPTER       (CHARGE_BASE+8)        /*���Բͷ������Ƿ����ӣ�USB VBUS��usbģ�鸺����*/
#define CHARGE_IOGET_DET_SPK                   (CHARGE_BASE+9)        /*����������Ƿ�����*/
#define CHARGE_IOGET_STATE                        (CHARGE_BASE+10)       /*��ȡ��ǰ���״̬*/

/*!
 *  \brief
 *  ��ѹ���ֵ���壨Vref=1.522V������ӦPMU_LRADC -- BATADC6 ��ֵ��
 *  if  Vref is changed,the below value should  be changed two .
 *
 *  \li LI280 : 2.80v 
 *  \li LI285 : 2.85v 
 *  \li LI290 : 2.90v 
 *  \li LI295 : 2.95v
 *  \li LI300 : 3.00v
 *  \li ��������
 *  \li ��������
 *  \li ��������
 *  \li LI415 : 4.15v
 *  \li LI420 : 4.20v
 *  \li LI425 : 4.25v
 *  \li LI430 : 4.30v        
 */ 
/******for Vref=1.50********/
typedef enum 
{
    LI210    = 0x00,
    LI220    = 0x03,
    LI280    = 0x12,
    LI285    = 0x14,
    LI290    = 0x15,
    LI295    = 0x16,
    LI300    = 0x18,
    LI305    = 0x19,
    LI310    = 0x1a,
    LI315    = 0x1c,
    LI320    = 0x1d,
    LI325    = 0x1e,
    LI330    = 0x20,
    LI335    = 0x21,
    LI340    = 0x22,
    LI345    = 0x24,
    LI350    = 0x25,
    LI355    = 0x26,
    LI360    = 0x28,
    LI365    = 0x29,
    LI370    = 0x2a,
    LI375    = 0x2c,
    LI380    = 0x2d,
    LI385    = 0x2e,
    LI390    = 0x30,
    LI395    = 0x31,
    LI400    = 0x32,
    LI405    = 0x33,
    LI410    = 0x34,
    LI415    = 0x36,
    LI420    = 0x37,
    LI425    = 0x38,    
    LI430    = 0x3a,
    LIMAX    = 0x3f

}charge_voltage_t;


/*!
 *  \brief
 *  ������ֵ���壬��ӦPMU_CHG -- CURRENT ��ֵ��
 *
 *  \li CHARGE_CURRENT_50 :    50Ma 
 *  \li CHARGE_CURRENT_100 : 100Ma 
 *  \li CHARGE_CURRENT_150 : 150Ma 
 *  \li CHARGE_CURRENT_200 : 200Ma
 *  \li CHARGE_CURRENT_250 : 250Ma
 *  \li CHARGE_CURRENT_300 : 300Ma
 *  \li CHARGE_CURRENT_400 : 400Ma
 *  \li CHARGE_CURRENT_500 : 500Ma  
 */ 

typedef enum 
{
          CHARGE_CURRENT_50  = 0x00,
	CHARGE_CURRENT_100 = 0x01,
	CHARGE_CURRENT_150 = 0x02,
	CHARGE_CURRENT_200 = 0x03,
	CHARGE_CURRENT_250 = 0x04,
	CHARGE_CURRENT_300 = 0x05,
	CHARGE_CURRENT_400 = 0x06,
	CHARGE_CURRENT_500 = 0x07,
}charge_current_t;


/*!
 *  \brief
 *    ���״̬����  
 *  \li CHARGE_STOP :                ���ֹͣ 
 *  \li CHARGE_ING :                    ����� 
 *  \li CHARGE_PAUSE :             �����ͣ
 *  \li CHARGE_FULL :                 �������  
 *  \li CHARGE_DETECT_VOL : ����м���ѹ�׶�  
 */
typedef enum
{
    CHARGE_STOP =                                    0x00,
    CHARGE_CHARGING =                         0x01,
    CHARGE_PAUSE =                                 0x02,
    CHARGE_FULL =                                    0x04,
    CHARGE_MONITOR =                            0x08,
    CHARGE_FAILED_OVERCHARGE = 0x10,
    CHARGE_FAILED_OVERLOAD =       0x20,
    CHARGE_FAILED_BATTERY =           0x40,
    CHARGE_NO_DC5V =                            0x80,
    CHARGE_NO_BATTERY =                   0x100,
    CHARGE_TEMP_STATUS =                 0x200,
}charge_status_t;

//typedef enum 
//{
//    BAT_MODE_REAL = 0x00,
//	BAT_MODE_SHOW = 0x01
//}bat_value_mode_t;

//typedef enum 
//{
//    CHARGE_TIME_LIMIT_ENABLE = 0x1,
//	CHARGE_TIME_LIMIT_DISABLE = 0x0,
//}charge_time_limit_t;

/*!
 *  \brief
 *    ��������Ϣ�ṹ�嶨��   
 * 
 */
struct charge_param_s
{
    /*! ��������ѹ����Ϊ�����ж�����ı�׼���Ƽ���ΪLI425��4.25v��������LI430 failed*/
    int     max_voltage;
  
    /*! ����������MA050~MA500������MA500 failed���Ƽ�MA300 ������MA300ϵͳ��300ma��Ϊ��������*/
    int     max_current;
    
    /*set the time delay to avoid the invalid full ,the unit is minute*/
    int     added_time;  
};

/*!
 * \brief  
 *      charge_param_t,struct charge_param_s .
 */

typedef struct charge_param_s charge_param_t;

/*! \endcond*/

#endif  /* __CHARGE_H__ */

