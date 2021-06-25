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
 * \file    camera_dsc_option_sticker_photo.h
 * \brief    camera_dsc_opttion_sticker_photo headfile
 * \author   Albert_lyu
 * \version 1.0
 * \date  2009-5-13
 *******************************************************************************/
#ifndef __CAMERA_DSC_OPTION_STICKER_PHOTO_H__
#define __CAMERA_DSC_OPTION_STICKER_PHOTO_H__
#include "include_case.h"
#include "include_psp.h"

/* 进入主菜单 */
bool camera_dsc_option_sticker_photo_enter(menuview_t *menuview);

/* 退出主菜单 */
bool camera_dsc_option_sticker_photo_exit(menuview_t *menuview);

/* 初始化菜单数据 */
menu_t* camera_dsc_option_sticker_photo_init(void);

/* 删除菜单数据 */
int camera_dsc_option_sticker_photo_deinit(void);

/* 大头贴图片解码*/
bool camera_dsc_sticker_photo_decode_init(void);

/*将大头贴解码数据buf清除*/
bool camera_dsc_sticker_photo_decode_deinit(void);

/*切换大头贴*/
bool camera_dsc_sticker_photo_change(int cur_item);

#endif   /* __CAMERA_DSC_OPTION_STICKER_PHOTO_H__ */
