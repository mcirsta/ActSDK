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

/* �������˵� */
bool camera_dsc_option_sticker_photo_enter(menuview_t *menuview);

/* �˳����˵� */
bool camera_dsc_option_sticker_photo_exit(menuview_t *menuview);

/* ��ʼ���˵����� */
menu_t* camera_dsc_option_sticker_photo_init(void);

/* ɾ���˵����� */
int camera_dsc_option_sticker_photo_deinit(void);

/* ��ͷ��ͼƬ����*/
bool camera_dsc_sticker_photo_decode_init(void);

/*����ͷ����������buf���*/
bool camera_dsc_sticker_photo_decode_deinit(void);

/*�л���ͷ��*/
bool camera_dsc_sticker_photo_change(int cur_item);

#endif   /* __CAMERA_DSC_OPTION_STICKER_PHOTO_H__ */
