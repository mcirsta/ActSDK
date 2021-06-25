/*******************************************************************************
 *                              USDK(1100)
 *                            Module: camera
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *    Albert_lyu 2009-5-26  1.0             build this file
 *******************************************************************************/
/*!
 * \file    camera_dv_config.h
 * \brief    camera_dv_config
 * \author   Albert_lyu
 * \version 1.0
 * \date  2009-5-26
 *******************************************************************************/
#ifndef __CAMERA_DV_CONFIG_H__
#define __CAMERA_DV_CONFIG_H__
#include "include_case.h"
#include "include_psp.h"
#include "camera_define.h"  


/*!
 *  \brief
 *  设定值的类型
 */
typedef enum
{
    /*! config value type is string*/
    DV_CFG_VALUE_TYPE_STRING = 0,

    /*! config value type is integer*/
    DV_CFG_VALUE_TYPE_INTEGER,
    /*! 错误枚举值 */
    DV_CFG_VALUE_TYPE_ERR = 0xFF,
} dv_cfg_value_type_e;

/*!
 *  \brief
 *  config内容
 */
typedef struct
{
    /*! config key name */
    char key[32];

    /*! value type */
    dv_cfg_value_type_e value_type;

    /*! to store string type value */
    char* text;

    /*! to store integer type value */
    int value;

    /*! need to write back to config file ?*/
    bool changed;
} camera_dv_cfg_t;
/*!
 *  \brief
 *  config选项是否可见
 */
typedef enum
{
    /*! 回放 */
    CAM_DV_CFG_VISIBLE_TYPE_PLAYBACK = 0,
    /*! 白平衡 */
    CAM_DV_CFG_VISIBLE_TYPE_WHITE_BALANCE,
    /*! 明亮度 */
    CAM_DV_CFG_VISIBLE_TYPE_BRIGHTNESS,
    /*! 曝光模式 */
    CAM_DV_CFG_VISIBLE_TYPE_EXPOSURE_MODE,
    /*! 特效 */
    CAM_DV_CFG_VISIBLE_TYPE_SPECIAL_EFFECT,
    /*! 保存路径 */
    CAM_DV_CFG_VISIBLE_TYPE_SAVE_PATH,
    /*! 分辨率 */
    CAM_DV_CFG_VISIBLE_TYPE_RESOLUTION,
    /*! 剩余空间 */
    CFG_DV_VISIBLE_TYPE_FREE_SPACE,    
    /*! 录像质量*/
    CAM_DV_CFG_VISIBLE_TYPE_RECORD_QUALITY,
    /*! 录音增益 */
    CAM_DV_CFG_VISIBLE_TYPE_RECORD_GAIN,
#if (DV_ACTIVITY_DETECT == 1)
    /*! 动静检测 */
    CAM_DV_CFG_VISIBLE_TYPE_ACTIVITY_DETECT,
#endif
    /*! 总数 */
    CAM_DV_CFG_VISIBLE_TYPE_TOTAL,
    /*! 错误枚举 */
    CAM_DV_CFG_VISIBLE_TYPE_ERR = 0xFF,

} camera_dv_option_visible_cfg_type_e;
/*!
 *  \brief
 *  config选项
 */
typedef enum
{
    /*! 回放 */
    CAM_DV_CFG_TYPE_PLAYBACK = 0,
    /*! 白平衡 */
    CAM_DV_CFG_TYPE_WHITE_BALANCE,
    /*! 明亮度 */
    CAM_DV_CFG_TYPE_BRIGHTNESS,
    /*! 曝光模式 */
    CAM_DV_CFG_TYPE_EXPOSURE_MODE,
    /*! 特效 */
    CAM_DV_CFG_TYPE_SPECIAL_EFFECT,
    /*! 保存路径 */
    CAM_DV_CFG_TYPE_SAVE_PATH,
    /*! 分辨率 */
    CAM_DV_CFG_TYPE_RESOLUTION,
    /*! 录像质量*/
    CAM_DV_CFG_TYPE_RECORD_QUALITY,
    /*! 录音增益 */
    CAM_DV_CFG_TYPE_RECORD_GAIN,
#if (DV_ACTIVITY_DETECT == 1)
    /*! 动静检测 */
    CAM_DV_CFG_TYPE_ACTIVITY_DETECT,
#endif
    /*! 录制小尺寸用较大分辨率 */
    CAM_DV_CFG_TYPE_REC_FRAME_RATE_MAX,
    /*! 录制大尺寸用较小分辨率*/
    CAM_DV_CFG_TYPE_REC_FRAME_RATE_MIN,
#if (SHORTCUT_PLAYBACK_DV == 1)
    /*! 预览顶层路径 */
    CAM_DV_CFG_PLAYBACK_ROOT,
    /*! 预览全路径文件名 */
    CAM_DV_CFG_PLAYBACK_FULLNAME,    
#endif /* SHORTCUT_PLAYBACK_DV */

    /*! 总数 */
    CAM_DV_CFG_TYPE_TOTAL,
    /*! 错误枚举 */
    CAM_DV_CFG_TYPE_ERR = 0xFF,

} camera_dv_cfg_type_e;

/*!
 *  \brief
 *  选项参数
 */
typedef enum
{
    /*! 回放 */
    CAM_DV_PARA_PLAYBACK = 0,
    /*! 白平衡 */
    CAM_DV_PARA_WHITE_BALANCE,
    /*! 明亮度 */
    CAM_DV_PARA_BRIGHTNESS,
    /*! 曝光模式 */
    CAM_DV_PARA_EXPOSURE_MODE,
    /*! 特效 */
    CAM_DV_PARA_SPECIAL_EFFECT,
    /*! 保存路径 */
    CAM_DV_PARA_SAVE_PATH,
    /*! 分辨率 */
    CAM_DV_PARA_RESOLUTION,
    /*! 剩余空间 */
    CAM_DV_PARA_FREE_SPACE,
    /*! 录像质量 */
    CAM_DV_PARA_RECORD_QUALITY,
    /*! 录音增益 */
    CAM_DV_PARA_RECORD_GAIN,
    /*数码变焦缩放级别*/
    CAM_DV_PARA_ZOOM_LEVEL,
#if (DV_ACTIVITY_DETECT == 1)
    /*! 动静检测 */
    CAM_DV_PARA_ACTIVITY_DETECT,
#endif
    /*! 错误枚举值 */
    CAM_DV_PARA_ERR = 0xFF,
} cam_dv_parameter_e;

extern cam_dv_parameter_e dv_parameter;
extern camera_dv_cfg_t g_camera_dv_cfgs[];
extern camera_dv_cfg_t g_camera_dv_option_visible_cfgs[];
/******************************************************************************/
/*!                                                                             
 * \par  Description:                                                           
 *	  -- 初始化配置                                      
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
bool camera_dv_init_all_cfg(void);
/******************************************************************************/
/*!                                                                             
 * \par  Description:                                                           
 *	  -- 配置退出                                      
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
bool camera_dv_deinit_all_cfg(void);
/******************************************************************************/
/*!                                                                             
 * \par  Description:                                                           
 *	  camera_dv_cfg_init-- 配置中字符串分配空间                                      
 * \param[in]     cfg[]                                                      
 * \param[int]   count                                                           
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
extern bool camera_dv_cfg_init(camera_dv_cfg_t cfgs[], int count);
/******************************************************************************/
/*!                                                                             
 * \par  Description:                                                           
 *	  camera_dv_cfg_deinit-- 配置中字符串分配的空间释放                                      
 * \param[in]     cfg                                                     
 * \param[in]     count                                                          
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
extern bool camera_dv_cfg_deinit(camera_dv_cfg_t cfgs[], int count);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_cfg_read-- 读取单个设置项
 * \param[in]     设置项
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
extern bool camera_dv_cfg_read(camera_dv_cfg_t * cfg);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_cfg_read_all-- 读取所有设置项
 * \param[in]     设置项
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
extern bool camera_dv_cfg_read_all(camera_dv_cfg_t cfgs[], int count);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_cfg_write-- 写单个指定设置项
 * \param[in]     设置项
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
extern bool camera_dv_cfg_write(camera_dv_cfg_t * cfg);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_cfg_write_all-- 写所有设置项
 * \param[in]     设置项
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
extern bool camera_dv_cfg_write_all(camera_dv_cfg_t cfgs[], int count);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_save_parameter-- 菜单选项参数保存
 * \param[in]     选项
 * \param[in]     保存值
 * \return       the result      none
 * \retval
 * \retval
 * \ingroup      camera_dv
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern void camera_dv_save_parameter(cam_dv_parameter_e fun, int parameter);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_cfg_after_read_value_regulate-- 读取配置值后数值校正
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
 * \note           主要config配置不支持负数
 *******************************************************************************/
extern bool camera_dv_cfg_after_read_value_regulate(void);
/******************************************************************************/
/*!
 * \par  Description:
 *	  -- 写入配置前数值校正
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
 * \note           主要config配置不支持负数
 *******************************************************************************/
extern bool camera_dv_cfg_before_write_value_regulate(void);
/******************************************************************************/
/*!                                                                             
 * \par  Description:                                                           
 *	  camera_dv_check_path-- 检测设置的路径是否存在，如果不存在录制目录则创建                                      
 * \param[in]     none                                                         
 * \return       the result      true 成功，false 失败                                                
 * \retval                                                              
 * \retval         
 * \ingroup      camera_dv_check_path                                                    
 * \par          exmaple code  none                                                 
 * \code                                                                        
 *                                               
 * \endcode                                                                     
 * \note                                                                        
*******************************************************************************/
extern bool camera_dv_check_path(hotplugin_dev_type_e path);
#endif /* #ifndef __CAMERA_DV_CONFIG_H__ */
