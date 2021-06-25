/*******************************************************************************
 *                              USDK(1100)
 *                            Module: common
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *    Albert_lyu 2009-4-22  1.0             build this file
 *******************************************************************************/
/*!
 * \file    slider_private.h
 * \brief    sliderʹ��˽������ use private date _h
 * \author   Albert_lyu
 * \version 1.0
 * \date  2009-4-22
 *******************************************************************************/
#ifndef _SLIDER_PRIVATE_H_
#define _SLIDER_PRIVATE_H_
#include "include_case.h"

#include "commonui_sty.h"

/*!
 *  \brief
 *      slider_private_data_t - slider�ڲ�˽�����ݽṹ����
 */
typedef struct slider_private_data_s
{
    /*! ����ͼƬ */
    int bg_pic;
    /*! ��Դ�� */
    resgroup_resource_t *slider_resgroup;
    /*! ����ͼƬ */
    picture_resource_t *pic_bg;
    /*! ����ͼƬ */
    picture_resource_t *pic_cur;
    /*! ����ͼƬ����*/
    gui_bitmap_info_t *bmp_bg;    
    /*! ����ͼƬ����*/
    gui_bitmap_info_t *bmp_cur;    
} slider_private_data_t;

#endif   /* _SLIDER_PRIVATE_H_ */

