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
 * \brief    ������ģ�����õ���gpio�������ͳһ��һ���ļ��У�������gpio������ʱ�����޸�.
 *           ��������ͳһʹ�ô��ļ��ж���ĺ꣬�������gpio�����ĺ��붨�嵽���ļ���
 * \author   yangrui
 * \version 1.0
 * \date  2009-11-25
 *******************************************************************************/

#ifndef _GPIO_CONFIG_H_
#define _GPIO_CONFIG_H_

#include <kmod_calls.h>

/***���ڼ�������������gpio��*/
#define SPEAKER_GPIO_INEN    GPIO_AINEN
#define SPEAKER_GPIO_OUTEN   GPIO_AOUTEN
#define SPEAKER_GPIO_DATA    GPIO_ADAT
#define SPEAKER_GPIO_NUM     26

/***���ڼ��linein�ߵ�gpio��*/
#define LINEIN_GPIO_INEN    GPIO_BINEN
#define LINEIN_GPIO_OUTEN   GPIO_BOUTEN
#define LINEIN_GPIO_DATA    GPIO_BDAT
#define LINEIN_GPIO_NUM     31

#endif

