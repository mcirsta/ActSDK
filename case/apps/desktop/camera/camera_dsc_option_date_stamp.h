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
 * \file    camera_dsc_option_date_stamp.h
 * \brief    camera_dsc_opttion_date_stamp headfile
 * \author   Albert_lyu
 * \version 1.0
 * \date  2009-5-13
 *******************************************************************************/
#ifndef __CAMERA_DSC_OPTION_DATE_STAMP_H__
#define __CAMERA_DSC_OPTION_DATE_STAMP_H__
#include "include_case.h"
#include "include_psp.h"

/* 进入主菜单 */
extern bool camera_dsc_option_date_stamp_enter(menuview_t *menuview);

/* 退出主菜单 */
extern bool camera_dsc_option_date_stamp_exit(menuview_t *menuview);

/* 初始化菜单数据 */
extern menu_t* camera_dsc_option_date_stamp_init(void);

/* 删除菜单数据 */
extern int camera_dsc_option_date_stamp_deinit(void);

#endif   /* __CAMERA_DSC_OPTION_DATE_STAMP_H__ */

