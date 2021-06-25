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
 *    ioctl CMD 命令字定义 \n  
 * 
 */
#define CHARGE_BASE                                        0x4800
#define CHARGE_IOSET_START                       (CHARGE_BASE+1)        /*开始充电命令*/
#define CHARGE_IOSET_STOP                          (CHARGE_BASE+2)        /*停止充电命令*/
#define CHARGE_IOSET_PARAM                      (CHARGE_BASE+3)        /*设置充电参数*/
#define CHARGE_IOGET_PARAM                      (CHARGE_BASE+4)       /*获取充电参数*/
#define CHARGE_IOSET_PROCESS                 (CHARGE_BASE+5)        /*开始充电命令*/
#define CHARGE_IOGET_BATTERY                  (CHARGE_BASE+6)         /*获取当前电量*/
#define CHARGE_IOGET_BATTERY_SHOW   (CHARGE_BASE+7)   /*获取当前电量,经过了平滑处理，给显示图标用*/
#define CHARGE_IOGET_DET_ADAPTER       (CHARGE_BASE+8)        /*检测圆头充电器是否连接，USB VBUS由usb模块负责检测*/
#define CHARGE_IOGET_DET_SPK                   (CHARGE_BASE+9)        /*检测扬声器是否连接*/
#define CHARGE_IOGET_STATE                        (CHARGE_BASE+10)       /*获取当前充电状态*/

/*!
 *  \brief
 *  电压检测值定义（Vref=1.522V），对应PMU_LRADC -- BATADC6 的值。
 *  if  Vref is changed,the below value should  be changed two .
 *
 *  \li LI280 : 2.80v 
 *  \li LI285 : 2.85v 
 *  \li LI290 : 2.90v 
 *  \li LI295 : 2.95v
 *  \li LI300 : 3.00v
 *  \li …………
 *  \li …………
 *  \li …………
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
 *  充电电流值定义，对应PMU_CHG -- CURRENT 的值。
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
 *    充电状态定义  
 *  \li CHARGE_STOP :                充电停止 
 *  \li CHARGE_ING :                    充电中 
 *  \li CHARGE_PAUSE :             充电暂停
 *  \li CHARGE_FULL :                 充电已满  
 *  \li CHARGE_DETECT_VOL : 充电中检测电压阶段  
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
 *    充电参数信息结构体定义   
 * 
 */
struct charge_param_s
{
    /*! 充电的最大电压，作为流程判断满电的标准，推荐设为LI425（4.25v），大于LI430 failed*/
    int     max_voltage;
  
    /*! 最大充电电流，MA050~MA500，大于MA500 failed，推荐MA300 ，大于MA300系统以300ma作为最大充电电流*/
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

