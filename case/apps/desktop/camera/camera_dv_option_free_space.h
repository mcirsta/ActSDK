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
 * \file    camera_dv_option_free_space.h
 * \brief    camera_dv_opttion_free_space headfile
 * \author   Albert_lyu
 * \version 1.0
 * \date  2009-5-13
 *******************************************************************************/
#ifndef __CAMERA_DV_OPTION_FREE_SPACE_H__
#define __CAMERA_DV_OPTION_FREE_SPACE_H__
#include "include_case.h"
#include "include_psp.h"

/* �������˵� */
bool camera_dv_option_free_space_enter(menuview_t *menuview);

/* �˳����˵� */
bool camera_dv_option_free_space_exit(menuview_t *menuview);

/* ��ʼ���˵����� */
menu_t* camera_dv_option_free_space_init(void);

/* ɾ���˵����� */
int camera_dv_option_free_space_deinit(void);

/* ���豸�仯��ʱ��ˢ��·���б� */
bool camera_dv_option_free_space_refresh(void);


#endif   /* __CAMERA_DV_OPTION_FREE_SPACE_H__ */

