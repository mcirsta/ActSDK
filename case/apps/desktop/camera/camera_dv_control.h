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
 * \brief    中间件封装头文件
 * \author   Albert_lyu
 * \version 1.0
 * \date  2009-5-27
 *******************************************************************************/
#ifndef __CAMERA_DV_CONTROL_H__
#define __CAMERA_DV_CONTROL_H__
#include "include_psp.h"
#include "include_case.h"
#include "camera_define.h"


/*! 照片保存文件路径 */
extern char dv_filepath[PATH_MAX];
/*! 录像码率 */
extern mmm_video_args_t dv_video_args;

extern char dv_filename[PATH_MAX];
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_set_parameter_preview-- 进入预览前sensor等参数设定
 * \param[in]     none
 * \return       the result      true 成功，false 失败
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
 *	  camera_dv_get_mmm_handle-- 获取dv中间件打开指针                                      
 * \param[in]     none                                                      
 * \param[out]   中间件指针                                                          
 * \return       the result      中间件指针
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
 *	  camera_dv_mmm_open-- 进入dv初始化
 * \param[in]     none
 * \return       the result      返回值int
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
 *	  camera_dv_mmm_close-- 退出dv
 * \param[in]     none
 * \return       the result      true 成功，false 失败
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
 *	  camera_dv_ctr_start_preview-- dv开始预览
 * \param[in]     none
 * \return       the result      true 成功，false 失败
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
 *	  camera_dv_ctr_stop_preview-- dv停止预览
 * \param[in]     none
 * \return       the result      true 成功，false 失败
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
 *	camera_dv_get_record_status  -- 获取中间件当前录制状态
 * \param[in]     none
 * \param[out]   dv_status
 * \return       the result   成功与否
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
 *	  camera_dv_record_stop-- 停止录像
 * \param[in]     none
 * \param[out]   none
 * \return       the result      true 成功，false 失败
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
 *	  camera_dv_record_resume-- 恢复继续录制
 * \param[in]     none
 * \param[out]   none
 * \return       the result      true 成功，false 失败
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
 *	  -- 暂停录像
 * \param[in]     none
 * \param[out]   none
 * \return       the result      true 成功，false 失败
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
 *	  -- 开始录像
 * \param[in]     none
 * \param[out]   none
 * \return       the result      true 成功，false 失败
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
 *	  camera_dv_ctr_blending_area_create-- 创建OSD和DV信号混合区域
 * \param[in]     区域结构体
 * \return       the result      true 成功，false 失败
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
 *	  camera_dv_ctr_blending_area_delete-- 删除OSD和DV信号混合区域
 * \param[in]     区域结构体
 * \return       the result      true 成功，false 失败
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
 *	  camera_dv_ctr_blending_area_show-- 显示OSD和DV信号混合区域
 * \param[in]     区域结构体
 * \return       the result      true 成功，false 失败
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
 *	  camera_dv_ctr_blending_area_hide-- 隐藏OSD和DV信号混合区域
 * \param[in]     区域结构体
 * \return       the result      true 成功，false 失败
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
 *	  camera_dv_ctr_zoom-- 数字变焦缩放
 * \param[in]     none
 * \return       the result      true 成功，false 失败
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
 *	  camera_dv_ctr_set_brightness-- 设置明亮度
 * \param[in]     明亮度值
 * \param[out]   none
 * \return       the result      true 成功，false 失败
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
 *	  camera_dv_ctr_set_white_balance-- 设置白平衡
 * \param[in]     白平衡参数(枚举值)
 * \param[out]   none
 * \return       the result      true 成功，false 失败
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
 *	  camera_dv_ctr_set_exposure_mode-- 设置曝光模式
 * \param[in]     曝光模式参数(枚举值)
 * \param[out]   none
 * \return       the result      true 成功，false 失败
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
 *	  camera_dv_ctr_set_special_effect--设置 特效参数
 * \param[in]     特效参数(枚举值)
 * \param[out]   none
 * \return       the result      true 成功，false 失败
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
 *	  camera_dv_ctr_set_save_path-- 设置保存路径
 * \param[in]     路径
 * \param[out]   none
 * \return       the result      true 成功，false 失败
 * \retval
 * \retval
 * \ingroup      camera_dv
 * \par          exmaple code(这里因为ui的关系，改变路径后都要获取容量，更新剩余)
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
 *	  camera_dv_ctr_set_resolution-- 设置分辨率
 * \param[in]     分辨率枚举值
 * \param[out]   none
 * \return       the result      true 成功，false 失败
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
 *	  camera_dv_ctr_get_disk_info-- 获取磁盘信息
 * \param[in]     磁盘参数结构
 * \param[in]     none
 * \param[in]     none
 * \param[out]   none
 * \return       the result      true 成功，false 失败
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
 *	  camera_dv_ctr_record_gain-- 设置录音增益
 * \param[in]     增益
 * \param[out]   none
 * \return       the result      true 成功，false 失败
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
 *	  camera_dv_ctr_set_record_quality-- 设置录像质量
 * \param[in]     参数
 * \param[out]   none
 * \return       the result      true 成功，false 失败
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
 *	  camera_dv_ctr_set_activity_detect-- 设置动静检测
 * \param[in]     是否设置
 * \param[out]   none
 * \return       the result      true 成功，false 失败
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
 *	  camera_dv_get_cur_sensor_items-- 获取当前支持的分辨率有几个和在总表中的偏移                                      
 * \param[in]     偏移                                                     
 * \param[in]     相对刻度                                       
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
 *	  camera_dv_mmm_opening_mobile_storage_out-- 当card或者u盘弹出的时候，
 *\更改默认路径为内存并将信息并显示出来                                      
 * \param[in]     none                                                    
 * \param[out]   none                                                           
 * \return       the result      true 成功，false 失败                                                
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

