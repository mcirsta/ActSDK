/*******************************************************************************
 *                              USDK(1100)
 *                            Module: camera_dsc
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *    Albert_lyu 2009-5-26  1.0             build this file
 *******************************************************************************/
/*!
 * \file    camera_dsc_option.h
 * \brief    menu菜单选项头文件
 * \author   Albert_lyu
 * \version 1.0
 * \date  2009-5-26
 *******************************************************************************/
#ifndef __CAMERA_DSC_OPTION_H__
#define __CAMERA_DSC_OPTION_H__
#include "include_case.h"
#include "include_psp.h"

typedef enum
/*!
 *  \brief
 *  菜单项
 */
{
    /*! 回放 */
    CAM_DSC_OPT_PLAYBACK = 0,
    /*! 白平衡 */
    CAM_DSC_OPT_WHITE_BALANCE,
    /*! 明亮度 */
    CAM_DSC_OPT_BRIGHTNESS,
    /*! 曝光模式 */
    CAM_DSC_OPT_EXPOSURE_MODE,
    /*! 闪光灯 */
    CAM_DSC_OPT_FLASH_LAMP,
    /*! 人脸检测 */
    CAM_DSC_OPT_FACE_IDENTIFY,
    /*! 大头贴 */
    CAM_DSC_OPT_STICKER_PHOTO,
    /*! 特效 */
    CAM_DSC_OPT_SPECIAL_EFFECT,
    /*! 保存路径 */
    CAM_DSC_OPT_SAVE_PATH,
    /*! 分辨率 */
    CAM_DSC_OPT_RESOLUTION,
    /*! 剩余空间 */
    CAM_DSC_OPT_FREE_SPACE,
    /*! 自拍时间 */
    CAM_DSC_OPT_SELF_TIMER,
    /*! 连怕时间 */
    CAM_DSC_OPT_CONT_SHOOTING,
    /*! 拍照声 */
    CAM_DSC_OPT_SHUTTER_SOUND,
    /*! 照片上打印日期 */
    CAM_DSC_OPT_DATE_STAMP,
    /*! 错误枚举值 */
    CAM_DSC_OPT_ERR = 0xFF,
} cam_dsc_option_e;

extern picture_resource_t *g_cam_dsc_opt_dot;

/******************************************************************************/
/*!                                                                             
 * \par  Description:                                                           
 *	 camera_dsc_playback-- 进入录制回放                                      
 * \param[in]     none                                                      
 * \param[out]   none                                                           
 * \return       the result      true 成功，false 失败                                                
 * \retval                                                              
 * \retval         
 * \ingroup      camera_option                                                    
 * \par          exmaple code  none                                                 
 * \code                                                                        
 *                                               
 * \endcode                                                                     
 * \note                                                                        
*******************************************************************************/
extern bool camera_dsc_playback(void);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_option_enter-- 进入主菜单
 * \param[in]     menuview结构
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
extern bool camera_dsc_option_enter(menuview_t *menuview, fusion_enter_e fenter );
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_option_exit-- 退出主菜单
 * \param[in]     menuview结构
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
extern bool camera_dsc_option_exit(menuview_t *menuview);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_option_init-- 初始化菜单数据
 * \param[in]     none
 * \return       the result      menu结构
 * \retval
 * \retval
 * \ingroup      camera_dsc
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern menu_t* camera_dsc_option_init(void);

/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_option_deinit-- 删除菜单数据
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
extern int camera_dsc_option_deinit(void);

#endif   /* __CAMERA_DSC_OPTION_H__ */
