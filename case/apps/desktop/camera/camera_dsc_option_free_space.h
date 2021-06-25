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
 * \file    camera_dsc_option_free_space.h
 * \brief    camera_dsc_opttion_free_space headfile
 * \author   Albert_lyu
 * \version 1.0
 * \date  2009-5-13
 *******************************************************************************/
#ifndef __CAMERA_DSC_OPTION_FREE_SPACE_H__
#define __CAMERA_DSC_OPTION_FREE_SPACE_H__
#include "include_case.h"
#include "include_psp.h"

/* 进入主菜单 */
bool camera_dsc_option_free_space_enter(menuview_t *menuview);

/* 退出主菜单 */
bool camera_dsc_option_free_space_exit(menuview_t *menuview);

/* 初始化菜单数据 */
menu_t* camera_dsc_option_free_space_init(void);

/* 删除菜单数据 */
int camera_dsc_option_free_space_deinit(void);

/* 在设备变化的时候刷新路径列表 */
bool camera_dsc_option_free_space_refresh(void);

#endif   /* __CAMERA_DSC_OPTION_FREE_SPACE_H__ */

