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
 * \file    camera_dsc_config.h
 * \brief    camera_dsc_config
 * \author   Albert_lyu
 * \version 1.0
 * \date  2009-5-26
 *******************************************************************************/
#ifndef __CAMERA_DSC_CONFIG_H__
#define __CAMERA_DSC_CONFIG_H__
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
    DSC_CFG_VALUE_TYPE_STRING = 0,

    /*! config value type is integer*/
    DSC_CFG_VALUE_TYPE_INTEGER,
    /*! 错误枚举值 */
    DSC_CFG_VALUE_TYPE_ERR = 0xFF,
} dsc_cfg_value_type_e;

/*!
 *  \brief
 *  config内容
 */
typedef struct
{
    /*! config key name */
    char key[32];

    /*! value type */
    dsc_cfg_value_type_e value_type;

    /*! to store string type value */
    #if 0
    //这里改为动态分配字串空间，注意释放
    char text[CFG_TEXT_LEN];
    #else
    char* text;
    #endif
    /*! to store integer type value */
    int value;

    /*! need to write back to config file ?*/
    bool changed;
} camera_dsc_cfg_t;
//camera_dsc_sticker_cfg_type_e
//camera_dsc_shutter_sound_cfg_type_e

/*!
 *  \brief
 *  config选项
 */
typedef enum
{  
    /*! 回放 */
    CAM_DSC_CFG_VISIBLE_TYPE_PLAYBACK = 0,
    /*! 白平衡 */
    CAM_DSC_CFG_VISIBLE_TYPE_WHITE_BALANCE,
    /*! 明亮度 */
    CAM_DSC_CFG_VISIBLE_TYPE_BRIGHTNESS,
    /*! 曝光模式 */
    CAM_DSC_CFG_VISIBLE_TYPE_EXPOSURE_MODE,
    /*! 闪光灯 */
    CAM_DSC_CFG_VISIBLE_TYPE_FLASH_LAMP,
    /*! 人脸检测 */
    CAM_DSC_CFG_VISIBLE_TYPE_FACE_IDENTIFY,
    /*! 大头贴 */
    CAM_DSC_CFG_VISIBLE_TYPE_STICKERPHOTO,
    /*! 特效 */
    CAM_DSC_CFG_VISIBLE_TYPE_SPECIAL_EFFECT,
    /*! 保存路径 */
    CAM_DSC_CFG_VISIBLE_TYPE_SAVE_PATH,
    /*! 分辨率 */
    CAM_DSC_CFG_VISIBLE_TYPE_RESOLUTION,
    /*! 剩余空间 */
    CAM_DSC_CFG_VISIBLE_TYPE_FREE_SPACE,    
    /*! 自拍时间 */
    CAM_DSC_CFG_VISIBLE_TYPE_SELF_TIMER,
    /*! 连拍时间 */
    CAM_DSC_CFG_VISIBLE_TYPE_CONT_SHOOTING,
    /*! 拍照音 */
    CAM_DSC_CFG_VISIBLE_TYPE_SHUTTER_SOUND,
    /*! 照片上日期标记 */
    CAM_DSC_CFG_VISIBLE_TYPE_DATE_STAMP,
    /*! 总数 */
    CAM_DSC_CFG_VISIBLE_TYPE_TOTAL,
    /*! 错误枚举 */
    CAM_DSC_CFG_VISIBLE_TYPE_ERR = 0xFF,

} camera_dsc_option_visible_cfg_type_e;


/*!
 *  \brief
 *  config选项
 */
typedef enum
{  
    /*! 回放 */
    CAM_DSC_CFG_TYPE_PLAYBACK = 0,
    /*! 白平衡 */
    CAM_DSC_CFG_TYPE_WHITE_BALANCE,
    /*! 明亮度 */
    CAM_DSC_CFG_TYPE_BRIGHTNESS,
    /*! 曝光模式 */
    CAM_DSC_CFG_TYPE_EXPOSURE_MODE,
    /*! 闪光灯 */
    CAM_DSC_CFG_TYPE_FLASH_LAMP,
    /*! 人脸检测 */
    CAM_DSC_CFG_TYPE_FACE_IDENTIFY,
    /*! 大头贴 */
    CAM_DSC_CFG_TYPE_STICKERPHOTO,
    /*! 特效 */
    CAM_DSC_CFG_TYPE_SPECIAL_EFFECT,
    /*! 保存路径 */
    CAM_DSC_CFG_TYPE_SAVE_PATH,
    /*! 分辨率 */
    CAM_DSC_CFG_TYPE_RESOLUTION,
#if 0
    /*! 自拍时间 */
    CAM_DSC_CFG_TYPE_SELF_TIMER,
    /*! 连拍时间 */
    CAM_DSC_CFG_TYPE_CONT_SHOOTING,
#endif
    /*! 拍照音 */
    CAM_DSC_CFG_TYPE_SHUTTER_SOUND,
    /*! 照片上日期标记 */
    CAM_DSC_CFG_TYPE_DATE_STAMP,
#if (SHORTCUT_PLAYBACK_DSC == 1)
    /*! 预览顶层路径 */
    CAM_DSC_CFG_PLAYBACK_ROOT,
    /*! 预览全路径文件名 */
    CAM_DSC_CFG_PLAYBACK_FULLNAME,    
#endif /* SHORTCUT_PLAYBACK_DSC */

    /*! 总数 */
    CAM_DSC_CFG_TYPE_TOTAL,
    /*! 错误枚举 */
    CAM_DSC_CFG_TYPE_ERR = 0xFF,

} camera_dsc_cfg_type_e;

/*!
 *  \brief
 *  选项参数
 */
typedef enum
{
    /*! 回放 */
    CAM_DSC_PARA_PLAYBACK = 0,
    /*! 白平衡 */
    CAM_DSC_PARA_WHITE_BALANCE,
    /*! 明亮度 */
    CAM_DSC_PARA_BRIGHTNESS,
    /*! 曝光模式 */
    CAM_DSC_PARA_EXPOSURE_MODE,
    /*! 闪光灯 */
    CAM_DSC_PARA_FLASH_LAMP,
    /*! 人脸检测 */
    CAM_DSC_PARA_FACE_IDENTIFY,
    /*! 大头贴 */
    CAM_DSC_PARA_STICKER_PHOTO,
    /*! 特效 */
    CAM_DSC_PARA_SPECIAL_EFFECT,
    /*! 保存路径 */
    CAM_DSC_PARA_SAVE_PATH,
    /*! 分辨率 */
    CAM_DSC_PARA_RESOLUTION,
    /*! 剩余空间 */
    CAM_DSC_PARA_FREE_SPACE,
    /*! 自拍时间 */
    CAM_DSC_PARA_SELF_TIMER,
    /*! 连怕时间 */
    CAM_DSC_PARA_CONT_SHOOTING,
    /*! 拍照声 */
    CAM_DSC_PARA_SHUTTER_SOUND,
    /*! 照片上打印日期 */
    CAM_DSC_PARA_DATE_STAMP,
    /*数码变焦缩放级别*/
    CAM_DSC_PARA_ZOOM_LEVEL,
    /*! 错误枚举值 */
    CAM_DSC_PARA_ERR = 0xFF,
} cam_dsc_parameter_e;

extern camera_dsc_cfg_t g_camera_dsc_cfgs[];
extern camera_dsc_cfg_t g_camera_dsc_option_visible_cfgs[];
extern camera_dsc_cfg_t g_camera_dsc_sticker_cfgs[];
extern camera_dsc_cfg_t g_camera_dsc_shutter_sound_cfgs[];
/******************************************************************************/
/*!                                                                             
 * \par  Description:                                                           
 *	  camera_dsc_init_all_cfg-- 初始化dsc所有配置                                      
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
bool camera_dsc_init_all_cfg(void);
/******************************************************************************/
/*!                                                                             
 * \par  Description:                                                           
 *	  camera_dsc_deinit_all_cfg-- 配置退出                                      
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
bool camera_dsc_deinit_all_cfg(void);
/******************************************************************************/
/*!                                                                             
 * \par  Description:                                                           
 *	  camera_dsc_cfg_init-- 配置中字符串分配空间                                      
 * \param[in]     cfg[]                                                      
 * \param[int]   count                                                           
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
extern bool camera_dsc_cfg_init(camera_dsc_cfg_t cfgs[], int count);
/******************************************************************************/
/*!                                                                             
 * \par  Description:                                                           
 *	  camera_dsc_cfg_deinit-- 配置中字符串分配的空间释放                                      
 * \param[in]     cfg                                                     
 * \param[in]     count                                                          
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
extern bool camera_dsc_cfg_deinit(camera_dsc_cfg_t cfgs[], int count);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_cfg_read-- 读取单个设置项
 * \param[in]     设置项
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
extern bool camera_dsc_cfg_read(camera_dsc_cfg_t * cfg);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_cfg_read_all-- 读取所有设置项
 * \param[in]     设置项
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
extern bool camera_dsc_cfg_read_all(camera_dsc_cfg_t cfgs[], int count);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_cfg_write-- 写单个指定设置项
 * \param[in]     设置项
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
extern bool camera_dsc_cfg_write(camera_dsc_cfg_t * cfg);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_cfg_write_all-- 写所有设置项
 * \param[in]     设置项
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
extern bool camera_dsc_cfg_write_all(camera_dsc_cfg_t cfgs[], int count);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_save_parameter-- 菜单选项参数保存
 * \param[in]     选项
 * \param[in]     保存值
 * \return       the result      none
 * \retval
 * \retval
 * \ingroup      camera_dsc
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
extern void camera_dsc_save_parameter(cam_dsc_parameter_e fun, int parameter);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_cfg_after_read_value_regulate-- 读取配置值后数值校正
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
 * \note           主要config配置不支持负数
 *******************************************************************************/
extern bool camera_dsc_cfg_after_read_value_regulate(void);
/******************************************************************************/
/*!
 * \par  Description:
 *	  -- 写入配置前数值校正
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
 * \note           主要config配置不支持负数
 *******************************************************************************/
extern bool camera_dsc_cfg_before_write_value_regulate(void);

/******************************************************************************/
/*!                                                                             
 * \par  Description:                                                           
 *	  camera_dsc_check_path-- 检测设置的路径是否存在，如果不存在录制目录则创建                                      
 * \param[in]     none                                                         
 * \return       the result      true 成功，false 失败                                                
 * \retval                                                              
 * \retval         
 * \ingroup      camera_dsc_check_path                                                    
 * \par          exmaple code  none                                                 
 * \code                                                                        
 *                                               
 * \endcode                                                                     
 * \note                                                                        
*******************************************************************************/
extern bool camera_dsc_check_path(hotplugin_dev_type_e path);
#endif /* #ifndef __CAMERA_DSC_CONFIG_H__ */

