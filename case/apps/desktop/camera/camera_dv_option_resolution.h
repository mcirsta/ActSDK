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
 * \file    camera_dv_option_resolution.h
 * \brief    camera_dv_opttion_resolution headfile
 * \author   Albert_lyu
 * \version 1.0
 * \date  2009-5-13
 *******************************************************************************/
#ifndef __CAMERA_DV_OPTION_RESOLUTION_H__
#define __CAMERA_DV_OPTION_RESOLUTION_H__
#include "include_case.h"
#include "include_psp.h"

/* 进入主菜单 */
bool camera_dv_option_resolution_enter(menuview_t *menuview);

/* 退出主菜单 */
bool camera_dv_option_resolution_exit(menuview_t *menuview);

/* 初始化菜单数据 */
menu_t* camera_dv_option_resolution_init(void);

/* 删除菜单数据 */
int camera_dv_option_resolution_deinit(void);

#endif   /* __CAMERA_DV_OPTION_RESOLUTION_H__ */

