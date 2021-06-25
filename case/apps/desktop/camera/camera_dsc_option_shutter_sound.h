/*******************************************************************************
 *                              USDK(1100)
 *                            Module: camera
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *    Albert_lyu 2009-5-13  1.0             build this file
 *******************************************************************************/
/*!
 * \file    camera_dsc_option_shutter_sound.h
 * \brief    camera_dsc_opttion_shutter_sound headfile
 * \author   Albert_lyu
 * \version 1.0
 * \date  2009-5-13
 *******************************************************************************/
#ifndef __CAMERA_DSC_OPTION_SHUTTER_SOUND_H__
#define __CAMERA_DSC_OPTION_SHUTTER_SOUND_H__
#include "include_case.h"
#include "include_psp.h"

/* �������˵� */
bool camera_dsc_option_shutter_sound_enter(menuview_t *menuview);

/* �˳����˵� */
bool camera_dsc_option_shutter_sound_exit(menuview_t *menuview);

/* ��ʼ���˵����� */
menu_t* camera_dsc_option_shutter_sound_init(void);

/* ɾ���˵����� */
int camera_dsc_option_shutter_sound_deinit(void);

#endif   /* __CAMERA_DSC_OPTION_SHUTTER_SOUND_H__ */

