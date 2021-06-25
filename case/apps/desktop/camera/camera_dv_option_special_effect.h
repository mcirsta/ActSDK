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
 * \file    camera_dv_option_special_effect.h
 * \brief    camera_dv_opttion_special_effect headfile
 * \author   Albert_lyu
 * \version 1.0
 * \date  2009-5-13
 *******************************************************************************/
#ifndef __CAMERA_DV_OPTION_SPECIAL_EFFECT_H__
#define __CAMERA_DV_OPTION_SPECIAL_EFFECT_H__
#include "include_case.h"
#include "include_psp.h"


/* 进入主菜单 */
bool camera_dv_option_special_effect_enter(menuview_t *menuview);

/* 退出主菜单 */
bool camera_dv_option_special_effect_exit(menuview_t *menuview);

/* 初始化菜单数据 */
menu_t* camera_dv_option_special_effect_init(void);

/* 删除菜单数据 */
int camera_dv_option_special_effect_deinit(void);

#endif   /* __CAMERA_DV_OPTION_SPECIAL_EFFECT_H__ */
