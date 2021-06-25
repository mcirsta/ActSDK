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
 * \file    camera_dv_control.h
 * \brief    �м����װͷ�ļ�
 * \author   Albert_lyu
 * \version 1.0
 * \date  2009-5-27
 *******************************************************************************/
#ifndef __CAMERA_DV_CONTROL_H__
#define __CAMERA_DV_CONTROL_H__
#include "include_psp.h"
#include "include_case.h"
#include "camera_define.h"


/*! ��Ƭ�����ļ�·�� */
extern char dv_filepath[PATH_MAX];
/*! ¼������ */
extern mmm_video_args_t dv_video_args;

extern char dv_filename[PATH_MAX];
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_set_parameter_preview-- ����Ԥ��ǰsensor�Ȳ����趨
 * \param[in]     none
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dv
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dv_set_parameter_preview(void);
/******************************************************************************/
/*!                                                                             
 * \par  Description:                                                           
 *	  camera_dv_get_mmm_handle-- ��ȡdv�м����ָ��                                      
 * \param[in]     none                                                      
 * \param[out]   �м��ָ��                                                          
 * \return       the result      �м��ָ��
 * \retval                                                              
 * \retval         
 * \ingroup      camera_dv                                                    
 * \par          exmaple code  none                                                 
 * \code                                                                        
 *                                               
 * \endcode                                                                     
 * \note                                                                        
*******************************************************************************/
extern void* camera_dv_get_mmm_handle(void);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_mmm_open-- ����dv��ʼ��
 * \param[in]     none
 * \return       the result      ����ֵint
 * \retval
 * \retval
 * \ingroup      camera_dv
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern int camera_dv_mmm_open(void);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_mmm_close-- �˳�dv
 * \param[in]     none
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dv
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dv_mmm_close(void);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_ctr_start_preview-- dv��ʼԤ��
 * \param[in]     none
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dv
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dv_ctr_start_preview(void);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_ctr_stop_preview-- dvֹͣԤ��
 * \param[in]     none
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dv
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dv_ctr_stop_preview(void);
/******************************************************************************/
/*!
 * \par  Description:
 *	camera_dv_get_record_status  -- ��ȡ�м����ǰ¼��״̬
 * \param[in]     none
 * \param[out]   dv_status
 * \return       the result   �ɹ����
 * \retval
 * \retval
 * \ingroup      camera_dv
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dv_get_record_status(mmm_dv_status_t* dv_status);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_record_stop-- ֹͣ¼��
 * \param[in]     none
 * \param[out]   none
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dv
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dv_record_stop(void);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_record_resume-- �ָ�����¼��
 * \param[in]     none
 * \param[out]   none
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dv
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dv_record_resume(void);
/******************************************************************************/
/*!
 * \par  Description:
 *	  -- ��ͣ¼��
 * \param[in]     none
 * \param[out]   none
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dv
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dv_record_pause(void);
/******************************************************************************/
/*!
 * \par  Description:
 *	  -- ��ʼ¼��
 * \param[in]     none
 * \param[out]   none
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dv
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dv_record_start(void);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_ctr_blending_area_create-- ����OSD��DV�źŻ������
 * \param[in]     ����ṹ��
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dv
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dv_ctr_blending_area_create(mmm_enc_blending_create_t *blending_area);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_ctr_blending_area_delete-- ɾ��OSD��DV�źŻ������
 * \param[in]     ����ṹ��
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dv
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dv_ctr_blending_area_delete(int blending_handle);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_ctr_blending_area_show-- ��ʾOSD��DV�źŻ������
 * \param[in]     ����ṹ��
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dv
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dv_ctr_blending_area_show(int *handle_array);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_ctr_blending_area_hide-- ����OSD��DV�źŻ������
 * \param[in]     ����ṹ��
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dv
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dv_ctr_blending_area_hide(int *handle_array);

/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_ctr_zoom-- ���ֱ佹����
 * \param[in]     none
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dv
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dv_ctr_zoom(void);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_ctr_set_brightness-- ����������
 * \param[in]     ������ֵ
 * \param[out]   none
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dv
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dv_ctr_set_brightness(unsigned int param);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_ctr_set_white_balance-- ���ð�ƽ��
 * \param[in]     ��ƽ�����(ö��ֵ)
 * \param[out]   none
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dv
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dv_ctr_set_white_balance(unsigned int param);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_ctr_set_exposure_mode-- �����ع�ģʽ
 * \param[in]     �ع�ģʽ����(ö��ֵ)
 * \param[out]   none
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dv
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dv_ctr_set_exposure_mode(unsigned int param);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_ctr_set_special_effect--���� ��Ч����
 * \param[in]     ��Ч����(ö��ֵ)
 * \param[out]   none
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dv
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dv_ctr_set_special_effect(unsigned int param);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_ctr_set_save_path-- ���ñ���·��
 * \param[in]     ·��
 * \param[out]   none
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dv
 * \par          exmaple code(������Ϊui�Ĺ�ϵ���ı�·����Ҫ��ȡ����������ʣ��)
 * \            camera_dv_ctr_set_save_path((char *) &dv_filepath);
 * \            camera_dv_ctr_get_disk_info(&(g_camera_scene_dv->dv_config.cfg_freeinfo));
 * \            camera_dv_change_icon(CAM_ICON_STORE_TYPE, (unsigned int)cur_item_pos);
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dv_ctr_set_save_path(char *filepath);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_ctr_set_resolution-- ���÷ֱ���
 * \param[in]     �ֱ���ö��ֵ
 * \param[out]   none
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dv
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dv_ctr_set_resolution(mmm_res_constant_t temp_res);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_ctr_get_disk_info-- ��ȡ������Ϣ
 * \param[in]     ���̲����ṹ
 * \param[in]     none
 * \param[in]     none
 * \param[out]   none
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup       camera_dv
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dv_ctr_get_disk_info(mmm_disk_info_t *freeinfo);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_ctr_record_gain-- ����¼������
 * \param[in]     ����
 * \param[out]   none
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dv
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dv_ctr_record_gain(mmm_audio_param_t* param);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_ctr_set_record_quality-- ����¼������
 * \param[in]     ����
 * \param[out]   none
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dv
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dv_ctr_set_record_quality(mmm_video_args_t* param);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_ctr_set_activity_detect-- ���ö������
 * \param[in]     �Ƿ�����
 * \param[out]   none
 * \return       the result      true �ɹ���false ʧ��
 * \retval
 * \retval
 * \ingroup      camera_dv
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern bool camera_dv_ctr_set_activity_detect(unsigned int param);
/******************************************************************************/
/*!                                                                             
 * \par  Description:                                                           
 *	  camera_dv_get_cur_sensor_items-- ��ȡ��ǰ֧�ֵķֱ����м��������ܱ��е�ƫ��                                      
 * \param[in]     ƫ��                                                     
 * \param[in]     ��Կ̶�                                       
 * \param[out]   none                                                           
 * \return      none                                          
 * \retval                                                              
 * \retval         
 * \ingroup      camera_dv_get_cur_sensor_items                                                    
 * \par          exmaple code  none                                                 
 * \code                                                                        
 *                                               
 * \endcode                                                                     
 * \note                                                                        
*******************************************************************************/
extern void camera_dv_get_cur_sensor_items(int *offset, int *max_scale);
/******************************************************************************/
/*!                                                                             
 * \par  Description:                                                           
 *	  camera_dv_mmm_opening_mobile_storage_out-- ��card����u�̵�����ʱ��
 *\����Ĭ��·��Ϊ�ڴ沢����Ϣ����ʾ����                                      
 * \param[in]     none                                                    
 * \param[out]   none                                                           
 * \return       the result      true �ɹ���false ʧ��                                                
 * \retval                                                              
 * \retval         
 * \ingroup      camera_dv_mmm_opening_mobile_storage_out                                                    
 * \par          exmaple code  none                                                 
 * \code                                                                        
 *                                               
 * \endcode                                                                     
 * \note                                                                        
*******************************************************************************/
extern bool camera_dv_mmm_opening_mobile_storage_out(void);

#endif   /* __CAMERA_DV_CONTROL_H__ */

