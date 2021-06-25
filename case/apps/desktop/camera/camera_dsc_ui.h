/*******************************************************************************
 *                              USDK(1100)
 *                            Module: camera_dsc
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *    Albert_lyu 2009-5-27  1.0             build this file
 *******************************************************************************/
/*!
 * \file    camera_dsc_ui.h
 * \brief    dsc界面绘图相关头文件
 * \author   Albert_lyu
 * \version 1.0
 * \date  2009-5-27
 *******************************************************************************/
#ifndef __CAMERA_DSC_UI_H__
#define __CAMERA_DSC_UI_H__
#include "include_case.h"
#include "include_psp.h"

#include "camera_main.h"
#include "camera_define.h"

extern int g_date_stamp_h;
extern char g_dsc_resolution_buf[ARRAY_LENGTH_RESOLUTION];
extern char g_free_number_buf[ARRAY_LENGTH_TIME];


/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_ui_free_space_to_buf-- 剩余空间数值转换到buf
 * \param[in]     none
 * \return       the result
 * \retval
 * \retval
 * \ingroup      camera_dsc
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
void camera_dsc_ui_free_space_to_buf(void);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_ui_resolution_to_buf-- 分辨率转换到buf
 * \param[in]     菜单中的索引--不是分辨率
 * \return       the result      索引对应的分辨率枚举值(mmm_encode)
 * \retval
 * \retval
 * \ingroup      camera_dsc
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
mmm_res_constant_t camera_dsc_ui_resolution_to_buf(int value);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_change_icon-- dsc改变当前图标
 * \param[in]     名称
 * \param[in]     帧数
 * \return       the result  none
 * \retval
 * \retval
 * \ingroup      camera_dsc
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
void camera_dsc_change_icon(cam_icon_e icon, unsigned int frame);

/*显示各个icon*/
void camera_dsc_ui_draw_slider_bg(int hdc, ui_pic_t *ui_pic);
bool camera_dsc_ui_zoom_level_to_coordinate(int cur_zoom_level, int total_zoom_level, int *y_coordinate);
void camera_dsc_ui_draw_slider(int hdc, ui_slider_t *ui_slider, int y);
void camera_dsc_ui_draw_pic(int hdc, ui_pic_t *ui_pic);
void camera_dsc_ui_draw_picregion(int hdc, ui_picregion_t *ui_picregion, int canvas_id);
void camera_dsc_ui_draw_string(int hdc, ui_string_t *ui_string, int canvas_id);

#endif   /* __CAMERA_DSC_UI_H__ */

