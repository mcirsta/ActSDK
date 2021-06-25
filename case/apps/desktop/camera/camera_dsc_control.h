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
 * \brief    中间件封装头文件
 * \author   Albert_lyu
 * \version 1.0
 * \date  2009-5-27
 *******************************************************************************/
#ifndef __CAMERA_DSC_CONTROL_H__
#define __CAMERA_DSC_CONTROL_H__
#include "include_case.h"
#include "include_psp.h"
 #include "camera_define.h"

/*! 照片保存文件路径 */
extern char dsc_filepath[PATH_MAX];
extern char dsc_filename[PATH_MAX];

extern char user_name[];
extern char machine_name[];

extern mmm_camera_exif_t exifAttr;
extern menu_t *g_camera_dsc_option_save_path;

/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_set_parameter_preview-- 进入预览前sensor等参数设定
 * \param[in]     none
 * \return       the result      true 成功，false 失败
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
 *	  camera_dsc_set_parameter_snapshot-- 进入拍照前sensor等参数设定
 * \param[in]     none
 * \param[in]     none
 * \param[in]     none
 * \param[out]   none
 * \return       the result      true 成功，false 失败
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
 *	  camera_dsc_get_mmm_handle-- 获取dsc中间件打开指针                                      
 * \param[in]     none                                                      
 * \param[out]   中间件指针                                                          
 * \return       the result      中间件指针
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
 *	  camera_dsc_mmm_open-- 进入dsc初始化
 * \param[in]     none
 * \return       the result      错误返回值
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
 *	  camera_dsc_mmm_close-- 退出dsc
 * \param[in]     none
 * \return       the result      true 成功，false 失败
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
 *	  camera_dsc_ctr_start_preview-- dsc开始预览
 * \param[in]     none
 * \return       the result      true 成功，false 失败
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
 *	  camera_dsc_ctr_stop_preview-- dsc停止预览
 * \param[in]     none
 * \return       the result      true 成功，false 失败
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
 *	  camera_dsc_snapshot-- 拍照
 * \param[in]     none
 * \param[in]     none
 * \param[in]     none
 * \param[out]   none
 * \return       the result      true 成功，false 失败
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
 *	  camera_dsc_ctr_blending_area_create-- 创建OSD和DSC信号混合区域
 * \param[in]     区域结构体
 * \return       the result      true 成功，false 失败
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
 *	  camera_dsc_ctr_blending_area_delete-- 删除OSD和DSC信号混合区域
 * \param[in]     区域结构体
 * \return       the result      true 成功，false 失败
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
 *	  camera_dsc_ctr_blending_area_show-- 显示OSD和DSC信号混合区域
 * \param[in]     区域结构体
 * \return       the result      true 成功，false 失败
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
 *	  camera_dsc_ctr_blending_area_hide-- 隐藏OSD和DSC信号混合区域
 * \param[in]     区域结构体
 * \return       the result      true 成功，false 失败
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
 *	  camera_dsc_ctr_zoom-- 数字变焦缩放
 * \param[in]     none
 * \return       the result      true 成功，false 失败
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
 *	  camera_dsc_get_cur_sensor_items-- 获取当前sensor支持分辨率位于所有分辨率的偏移和支持项数
 * \param[in]     偏移值得指针
 * \param[in]     最大项数
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
 *	  camera_dsc_ctr_set_face_detect-- 人脸侦测
 * \param[in]     onoff
 * \param[out]   none
 * \return       the result      true 成功，false 失败
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
 *	  camera_dsc_ctr_set_brightness-- 设置明亮度
 * \param[in]     数值
 * \param[out]   none
 * \return       the result      true 成功，false 失败
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
 *	  camera_dsc_ctr_set_exposure_mode-- 设置曝光模式
 * \param[in]     选项
 * \param[out]   none
 * \return       the result      true 成功，false 失败
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
 *	  camera_dsc_ctr_set_flashlight-- 设置闪光灯
 * \param[in]     选项值
 * \param[out]   none
 * \return       the result      true 成功，false 失败
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
 *	  camera_dsc_ctr_get_disk_info-- 获取剩余空间
 * \param[in]     剩余空间结构体
 * \param[out]   none
 * \return       the result      true 成功，false 失败
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
 *	  camera_dsc_ctr_set_resolution-- 设置分辨率
 * \param[in]     分辨率
 * \param[out]   none
 * \return       the result      true 成功，false 失败
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
 *	  camera_dsc_ctr_set_save_path-- 设置路径
 * \param[in]     路径
 * \param[out]   none
 * \return       the result      true 成功，false 失败
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
 *	  camera_dsc_ctr_set_special_effect-- 设置特效
 * \param[in]     特效值
 * \param[out]   none
 * \return       the result      true 成功，false 失败
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
 *	  camera_dsc_ctr_set_photo_sticker-- 大头贴
 * \param[in]     大头贴结构体
 * \param[out]   none
 * \return       the result      true 成功，false 失败
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
 *	  camera_dsc_ctr_set_white_balance-- 设置白平衡
 * \param[in]     选项值
 * \param[out]   none
 * \return       the result      true 成功，false 失败
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
 *	  camera_dsc_mmm_opening_mobile_storage_out-- 当card或者u盘弹出的时候，
 *\更改默认路径为内存并将信息并显示出来                                      
 * \param[in]     none                                                   
 * \param[out]   none                                                           
 * \return       the result      true 成功，false 失败                                                
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

