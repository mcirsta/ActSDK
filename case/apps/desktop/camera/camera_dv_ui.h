/*******************************************************************************
 *                              USDK(1100)
 *                            Module: camera_dv
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *    Albert_lyu 2009-5-27  1.0             build this file
 *******************************************************************************/
/*!
 * \file    camera_dv_ui.h
 * \brief    dv�����ͼ���ͷ�ļ�
 * \author   Albert_lyu
 * \version 1.0
 * \date  2009-5-27
 *******************************************************************************/
#ifndef __CAMERA_DV_UI_H__
#define __CAMERA_DV_UI_H__
#include "include_case.h"
#include "include_psp.h"

#include "camera_main.h"
#include "camera_define.h"

extern char g_use_free_number_buf[ARRAY_LENGTH_TIME];
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_ui_free_space_to_buf-- ʣ��ռ���ֵת����buf
 * \param[in]     none
 * \return       the result
 * \retval
 * \retval
 * \ingroup      camera_dv
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
void camera_dv_ui_free_space_to_buf(void);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_ui_resolution_to_buf-- �ֱ���ת����buf
 * \param[in]     �˵��е�����--���Ƿֱ���
 * \return       the result      ������Ӧ�ķֱ���ö��ֵ(mmm_encode)
 * \retval
 * \retval
 * \ingroup      camera_dv
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
mmm_res_constant_t camera_dv_ui_resolution_to_buf(int value);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_ui_quality_to_bitrate-- �������¼��Ʒ�ʵȼ�ת��Ϊ����
 * \param[in]     �ȼ�
 * \param[out]   none
 * \return       the result   ����
 * \retval
 * \retval
 * \ingroup      camera_dv
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
mmm_video_bitrate_level_t camera_dv_ui_quality_to_bitrate(int value);

/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_change_icon-- dv�ı䵱ǰͼ��
 * \param[in]     ����
 * \param[in]     ֡��
 * \return       the result  none
 * \retval
 * \retval
 * \ingroup      camera_dv
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
void camera_dv_change_icon(cam_icon_e icon, unsigned int frame);

void camera_dv_ui_draw_slider_bg(int hdc, ui_pic_t *ui_pic);
void camera_dv_ui_draw_pic(int hdc, ui_pic_t *ui_pic);
void camera_dv_ui_draw_picregion(int hdc, ui_picregion_t *ui_picregion, int canvas_id);
void camera_dv_ui_draw_picregion_status(int hdc, ui_picregion_t *ui_picregion, int canvas_id);
#if (DV_ICON_STATUS2SDRAM == 1)
void camera_dv_ui_draw_sdram_status(int hdc, ui_picregion_t *ui_picregion, int canvas_id);
#endif /* DV_ICON_STATUS2SDRAM */

void camera_dv_ui_draw_string(int hdc, ui_string_t *ui_string, int canvas_id);

#endif   /* __CAMERA_DV_UI_H__ */

