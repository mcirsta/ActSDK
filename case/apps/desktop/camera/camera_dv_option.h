/*******************************************************************************
 *                              USDK(1100)
 *                            Module: camera_dv
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *    Albert_lyu 2009-5-26  1.0             build this file
 *******************************************************************************/
/*!
 * \file    camera_dv_option.h
 * \brief    menu菜单选项头文件
 * \author   Albert_lyu
 * \version 1.0
 * \date  2009-5-26
 *******************************************************************************/
#ifndef __CAMERA_DV_OPTION_H__
#define __CAMERA_DV_OPTION_H__
#include "include_case.h"
#include "include_psp.h"
#include "camera_define.h"

typedef enum
/*!
 *  \brief
 *  菜单项
 */
{
    /*! 回放 */
    CAM_DV_OPT_PLAYBACK = 0,
    /*! 白平衡 */
    CAM_DV_OPT_WHITE_BALANCE,
    /*! 明亮度 */
    CAM_DV_OPT_BRIGHTNESS,
    /*! 曝光模式 */
    CAM_DV_OPT_EXPOSURE_MODE,
    /*! 特效 */
    CAM_DV_OPT_SPECIAL_EFFECT,
    /*! 保存路径 */
    CAM_DV_OPT_SAVE_PATH,
    /*! 分辨率 */
    CAM_DV_OPT_RESOLUTION,
    /*! 剩余空间 */
    CAM_DV_OPT_FREE_SPACE,
    /*! 录像质量 */
    CAM_DV_OPT_RECORD_QUALITY,
    /*! 录音增益 */
    CAM_DV_OPT_RECORD_GAIN,
#if (DV_ACTIVITY_DETECT == 1)
    /*! 动静检测 */
    CAM_DV_OPT_ACTIVITY_DETECT,
#endif
    /*! 错误枚举值 */
    CAM_DV_OPT_ERR = 0xFF,
} cam_dv_option_e;

extern picture_resource_t *g_cam_dv_opt_dot;
/******************************************************************************/
/*!                                                                             
 * \par  Description:                                                           
 *	 camera_dv_playback-- 进入录制回放                                      
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
bool camera_dv_playback(void);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_option_enter-- 进入主菜单
 * \param[in]     menuview结构
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
bool camera_dv_option_enter(menuview_t *menuview, fusion_enter_e fenter );
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_option_exit-- 退出主菜单
 * \param[in]     menuview结构
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
bool camera_dv_option_exit(menuview_t *menuview);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_option_init-- 初始化菜单数据
 * \param[in]     none
 * \return       the result      menu结构
 * \retval
 * \retval
 * \ingroup      camera_dv
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
menu_t* camera_dv_option_init(void);

/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_option_deinit-- 删除菜单数据
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
int camera_dv_option_deinit(void);

#endif   /* __CAMERA_DV_OPTION_H__ */
