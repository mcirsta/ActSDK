/*
*******************************************************************************
*                       NOYA1100
*                (c) Copyright 2002-2008, Actions Co,Ld. 
*                        All Right Reserved 
* FileName: .h
* Description:
* History: 
*       <author>    <time>          <version >      <desc>
*       shaofeng    2009/02/28      1.0             建立该文件。
*******************************************************************************
*/ 

#ifndef __CFG_VOLTAGE_CHARGE_H__
#define __CFG_VOLTAGE_CHARGE_H__

#include "include_case.h"

//选择Actions智能充电流程, 还是标准充电流程(差异在恒压阶段是否检测实际电池电压)
//1: Actions智能充电; 0:标准充电
#define CHARGE_TYPE 1




//充电应用
    //充电参数
    #define CHARGE_FULL_VOL     
    #define CHARGE_MAX_CURRENT  




//边玩边充
    //边玩边充, 请打开此宏定义
    #define CHARGE_PLAYING  1
    
    //充电参数
    #define CHARGE_PLAYING_FULL_VOL     
    #define CHARGE_PLAYING_MAX_CURRENT  




//USB充电
    //USB 空闲充电, 请打开此宏定义
    #define CHARGE_USB_IDLE  1
    
    //空闲多长时间开始充电, 单位:秒
    #define CHARGE_USB_IDLE_TIME  10
    //充电参数
    #define CHARGE_USB_FULL_VOL     
    #define CHARGE_USB_MAX_CURRENT  




//播放视频充电
    //播放视频时充电, 请打开此宏定义
    #define CHARGE_VIDEO  1
    
    //充电参数
    #define CHARGE_VIDEO_FULL_VOL     
    #define CHARGE_VIDEO_MAX_CURRENT  




//电池显示级数, 及各级电压
    #define VOL_BATTERY_LEVELS 4
    #define VOL_BATTERY_LEVEL0 
    #define VOL_BATTERY_LEVEL1 
    #define VOL_BATTERY_LEVEL2 
    #define VOL_BATTERY_LEVEL3 




//各种电压
    //不开机电压
    #define VOL_NOT_START_UP   
    //低电提示电压, 包括:开机低电提示后关机, 和开机后的低电提示
    #define VOL_LOW_VOLTAGE             LI350
    //低电关机电压
    #define VOL_POWER_OFF               LI330
    //播放video的门限电压
    #define VOL_THRESHOLD_PLAY_VIDEO    LI360
    //开始录音的门限电压
    #define VOL_THRESHOLD_PLAY_RECORD
    //开始自动升级的门限电压
    #define VOL_THRESHOLD_UPGRADE






#endif /* __CFG_VOLTAGE_CHARGE_H__ */

