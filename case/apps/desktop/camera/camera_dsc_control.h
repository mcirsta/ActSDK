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
 * \file    camera_dsc_control.h
 * \brief    �м����װͷ�ļ�
 * \author   Albert_lyu
 * \version 1.0
 * \date  2009-5-27
 *******************************************************************************/
#ifndef __CAMERA_DSC_CONTROL_H__
#define __CAMERA_DSC_CONTROL_H__
#include "include_case.h"
#include "include_psp.h"
 #include "camera_define.h"

/*! ��Ƭ�����ļ�·�� */
extern char dsc_filepath[PATH_MAX];
extern char dsc_filename[PATH_MAX];

extern char user_name[];
extern char machine_name[];

extern mmm_camera_exif_t exifAttr;
extern menu_t *g_camera_dsc_option_save_path;

/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_set_parameter_preview-- ����Ԥ��ǰsensor�Ȳ����趨
 * \param[in]     none
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dsc
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dsc_set_parameter_preview(void);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_set_parameter_snapshot-- ��������ǰsensor�Ȳ����趨
 * \param[in]     none
 * \param[in]     none
 * \param[in]     none
 * \param[out]   none
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dsc
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dsc_set_parameter_snapshot(void);
/******************************************************************************/
/*!                                                                             
 * \par  Description:                                                           
 *	  camera_dsc_get_mmm_handle-- ��ȡdsc�м����ָ��                                      
 * \param[in]     none                                                      
 * \param[out]   �м��ָ��                                                          
 * \return       the result      �м��ָ��
 * \retval                                                              
 * \retval         
 * \ingroup      camera_dsc                                                    
 * \par          exmaple code  none                                                 
 * \code                                                                        
 *                                               
 * \endcode                                                                     
 * \note                                                                        
*******************************************************************************/
extern void* camera_dsc_get_mmm_handle(void);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_mmm_open-- ����dsc��ʼ��
 * \param[in]     none
 * \return       the result      ���󷵻�ֵ
 * \retval
 * \retval
 * \ingroup      camera_dsc
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern int camera_dsc_mmm_open(void);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_mmm_close-- �˳�dsc
 * \param[in]     none
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dsc
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dsc_mmm_close(void);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_ctr_start_preview-- dsc��ʼԤ��
 * \param[in]     none
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dsc
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dsc_ctr_start_preview(void);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_ctr_stop_preview-- dscֹͣԤ��
 * \param[in]     none
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dsc
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dsc_ctr_stop_preview(void);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_snapshot-- ����
 * \param[in]     none
 * \param[in]     none
 * \param[in]     none
 * \param[out]   none
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dsc
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dsc_snapshot(void);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_ctr_blending_area_create-- ����OSD��DSC�źŻ������
 * \param[in]     ����ṹ��
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dsc
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dsc_ctr_blending_area_create(mmm_enc_blending_create_t *blending_area);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_ctr_blending_area_delete-- ɾ��OSD��DSC�źŻ������
 * \param[in]     ����ṹ��
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dsc
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dsc_ctr_blending_area_delete(int blending_handle);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_ctr_blending_area_show-- ��ʾOSD��DSC�źŻ������
 * \param[in]     ����ṹ��
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dsc
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dsc_ctr_blending_area_show(int *handle_array);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_ctr_blending_area_hide-- ����OSD��DSC�źŻ������
 * \param[in]     ����ṹ��
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dsc
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dsc_ctr_blending_area_hide(int *handle_array);

/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_ctr_zoom-- ���ֱ佹����
 * \param[in]     none
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dsc
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dsc_ctr_zoom(void);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_get_cur_sensor_items-- ��ȡ��ǰsensor֧�ֱַ���λ�����зֱ��ʵ�ƫ�ƺ�֧������
 * \param[in]     ƫ��ֵ��ָ��
 * \param[in]     �������
 * \param[out]   none
 * \return
 * \retval
 * \retval
 * \ingroup      camera_dsc
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern void camera_dsc_get_cur_sensor_items(int *offset, int *max_scale);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_ctr_set_face_detect-- �������
 * \param[in]     onoff
 * \param[out]   none
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dsc
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dsc_ctr_set_face_detect(unsigned int param);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_ctr_set_brightness-- ����������
 * \param[in]     ��ֵ
 * \param[out]   none
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dsc
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dsc_ctr_set_brightness(unsigned int param);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_ctr_set_exposure_mode-- �����ع�ģʽ
 * \param[in]     ѡ��
 * \param[out]   none
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dsc
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dsc_ctr_set_exposure_mode(unsigned int param);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_ctr_set_flashlight-- ���������
 * \param[in]     ѡ��ֵ
 * \param[out]   none
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dsc
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dsc_ctr_set_flashlight(unsigned int param);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_ctr_get_disk_info-- ��ȡʣ��ռ�
 * \param[in]     ʣ��ռ�ṹ��
 * \param[out]   none
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dsc
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dsc_ctr_get_disk_info(mmm_disk_info_t *freeinfo);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_ctr_set_resolution-- ���÷ֱ���
 * \param[in]     �ֱ���
 * \param[out]   none
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dsc
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dsc_ctr_set_resolution(mmm_res_constant_t temp_res);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_ctr_set_save_path-- ����·��
 * \param[in]     ·��
 * \param[out]   none
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dsc
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dsc_ctr_set_save_path(char *filepath);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_ctr_set_special_effect-- ������Ч
 * \param[in]     ��Чֵ
 * \param[out]   none
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dsc
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dsc_ctr_set_special_effect(unsigned int param);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_ctr_set_photo_sticker-- ��ͷ��
 * \param[in]     ��ͷ���ṹ��
 * \param[out]   none
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dsc
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dsc_ctr_set_photo_sticker(mmm_photo_sticker_t* sticker_para);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_ctr_set_white_balance-- ���ð�ƽ��
 * \param[in]     ѡ��ֵ
 * \param[out]   none
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dsc
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dsc_ctr_set_white_balance(unsigned int param);

/******************************************************************************/
/*!                                                                             
 * \par  Description:                                                           
 *	  camera_dsc_mmm_opening_mobile_storage_out-- ��card����u�̵�����ʱ��
 *\����Ĭ��·��Ϊ�ڴ沢����Ϣ����ʾ����                                      
 * \param[in]     none                                                   
 * \param[out]   none                                                           
 * \return       the result      true �ɹ���false ʧ��                                                
 * \retval                                                              
 * \retval         
 * \ingroup      camera_dsc_mmm_opening_mobile_storage_out                                                    
 * \par          exmaple code  none                                                 
 * \code                                                                        
 *                                               
 * \endcode                                                                     
 * \note                                                                        
*******************************************************************************/
extern bool camera_dsc_mmm_opening_mobile_storage_out(void);
#endif   /* __CAMERA_DSC_CONTROL_H__ */

