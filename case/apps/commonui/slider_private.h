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
 * \brief    slider使用私有数据 use private date _h
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
 *      slider_private_data_t - slider内部私有数据结构定义
 */
typedef struct slider_private_data_s
{
    /*! 背景图片 */
    int bg_pic;
    /*! 资源组 */
    resgroup_resource_t *slider_resgroup;
    /*! 背景图片 */
    picture_resource_t *pic_bg;
    /*! 高亮图片 */
    picture_resource_t *pic_cur;
    /*! 背景图片缓存*/
    gui_bitmap_info_t *bmp_bg;    
    /*! 高亮图片缓存*/
    gui_bitmap_info_t *bmp_cur;    
} slider_private_data_t;

#endif   /* _SLIDER_PRIVATE_H_ */

