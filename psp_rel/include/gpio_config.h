/********************************************************************************
 *                              NOYA
 *                            Module: soft image layer
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       yangrui    2009-11-25          1.0             build this file
 ********************************************************************************/
/*!
 * \file     gpio_config.h
 * \brief    将驱动模块所用到的gpio分配情况统一到一个文件中，在遇到gpio分配变更时易于修改.
 *           驱动中请统一使用此文件中定义的宏，新增针对gpio操作的宏请定义到此文件中
 * \author   yangrui
 * \version 1.0
 * \date  2009-11-25
 *******************************************************************************/

#ifndef _GPIO_CONFIG_H_
#define _GPIO_CONFIG_H_

#include <kmod_calls.h>

/***用于检测喇叭与耳机的gpio口*/
#define SPEAKER_GPIO_INEN    GPIO_AINEN
#define SPEAKER_GPIO_OUTEN   GPIO_AOUTEN
#define SPEAKER_GPIO_DATA    GPIO_ADAT
#define SPEAKER_GPIO_NUM     26

/***用于检测linein线的gpio口*/
#define LINEIN_GPIO_INEN    GPIO_BINEN
#define LINEIN_GPIO_OUTEN   GPIO_BOUTEN
#define LINEIN_GPIO_DATA    GPIO_BDAT
#define LINEIN_GPIO_NUM     31

#endif

