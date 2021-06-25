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
 *  �趨ֵ������
 */
typedef enum
{
    /*! config value type is string*/
    DV_CFG_VALUE_TYPE_STRING = 0,

    /*! config value type is integer*/
    DV_CFG_VALUE_TYPE_INTEGER,
    /*! ����ö��ֵ */
    DV_CFG_VALUE_TYPE_ERR = 0xFF,
} dv_cfg_value_type_e;

/*!
 *  \brief
 *  config����
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
 *  configѡ���Ƿ�ɼ�
 */
typedef enum
{
    /*! �ط� */
    CAM_DV_CFG_VISIBLE_TYPE_PLAYBACK = 0,
    /*! ��ƽ�� */
    CAM_DV_CFG_VISIBLE_TYPE_WHITE_BALANCE,
    /*! ������ */
    CAM_DV_CFG_VISIBLE_TYPE_BRIGHTNESS,
    /*! �ع�ģʽ */
    CAM_DV_CFG_VISIBLE_TYPE_EXPOSURE_MODE,
    /*! ��Ч */
    CAM_DV_CFG_VISIBLE_TYPE_SPECIAL_EFFECT,
    /*! ����·�� */
    CAM_DV_CFG_VISIBLE_TYPE_SAVE_PATH,
    /*! �ֱ��� */
    CAM_DV_CFG_VISIBLE_TYPE_RESOLUTION,
    /*! ʣ��ռ� */
    CFG_DV_VISIBLE_TYPE_FREE_SPACE,    
    /*! ¼������*/
    CAM_DV_CFG_VISIBLE_TYPE_RECORD_QUALITY,
    /*! ¼������ */
    CAM_DV_CFG_VISIBLE_TYPE_RECORD_GAIN,
#if (DV_ACTIVITY_DETECT == 1)
    /*! ������� */
    CAM_DV_CFG_VISIBLE_TYPE_ACTIVITY_DETECT,
#endif
    /*! ���� */
    CAM_DV_CFG_VISIBLE_TYPE_TOTAL,
    /*! ����ö�� */
    CAM_DV_CFG_VISIBLE_TYPE_ERR = 0xFF,

} camera_dv_option_visible_cfg_type_e;
/*!
 *  \brief
 *  configѡ��
 */
typedef enum
{
    /*! �ط� */
    CAM_DV_CFG_TYPE_PLAYBACK = 0,
    /*! ��ƽ�� */
    CAM_DV_CFG_TYPE_WHITE_BALANCE,
    /*! ������ */
    CAM_DV_CFG_TYPE_BRIGHTNESS,
    /*! �ع�ģʽ */
    CAM_DV_CFG_TYPE_EXPOSURE_MODE,
    /*! ��Ч */
    CAM_DV_CFG_TYPE_SPECIAL_EFFECT,
    /*! ����·�� */
    CAM_DV_CFG_TYPE_SAVE_PATH,
    /*! �ֱ��� */
    CAM_DV_CFG_TYPE_RESOLUTION,
    /*! ¼������*/
    CAM_DV_CFG_TYPE_RECORD_QUALITY,
    /*! ¼������ */
    CAM_DV_CFG_TYPE_RECORD_GAIN,
#if (DV_ACTIVITY_DETECT == 1)
    /*! ������� */
    CAM_DV_CFG_TYPE_ACTIVITY_DETECT,
#endif
    /*! ¼��С�ߴ��ýϴ�ֱ��� */
    CAM_DV_CFG_TYPE_REC_FRAME_RATE_MAX,
    /*! ¼�ƴ�ߴ��ý�С�ֱ���*/
    CAM_DV_CFG_TYPE_REC_FRAME_RATE_MIN,
#if (SHORTCUT_PLAYBACK_DV == 1)
    /*! Ԥ������·�� */
    CAM_DV_CFG_PLAYBACK_ROOT,
    /*! Ԥ��ȫ·���ļ��� */
    CAM_DV_CFG_PLAYBACK_FULLNAME,    
#endif /* SHORTCUT_PLAYBACK_DV */

    /*! ���� */
    CAM_DV_CFG_TYPE_TOTAL,
    /*! ����ö�� */
    CAM_DV_CFG_TYPE_ERR = 0xFF,

} camera_dv_cfg_type_e;

/*!
 *  \brief
 *  ѡ�����
 */
typedef enum
{
    /*! �ط� */
    CAM_DV_PARA_PLAYBACK = 0,
    /*! ��ƽ�� */
    CAM_DV_PARA_WHITE_BALANCE,
    /*! ������ */
    CAM_DV_PARA_BRIGHTNESS,
    /*! �ع�ģʽ */
    CAM_DV_PARA_EXPOSURE_MODE,
    /*! ��Ч */
    CAM_DV_PARA_SPECIAL_EFFECT,
    /*! ����·�� */
    CAM_DV_PARA_SAVE_PATH,
    /*! �ֱ��� */
    CAM_DV_PARA_RESOLUTION,
    /*! ʣ��ռ� */
    CAM_DV_PARA_FREE_SPACE,
    /*! ¼������ */
    CAM_DV_PARA_RECORD_QUALITY,
    /*! ¼������ */
    CAM_DV_PARA_RECORD_GAIN,
    /*����佹���ż���*/
    CAM_DV_PARA_ZOOM_LEVEL,
#if (DV_ACTIVITY_DETECT == 1)
    /*! ������� */
    CAM_DV_PARA_ACTIVITY_DETECT,
#endif
    /*! ����ö��ֵ */
    CAM_DV_PARA_ERR = 0xFF,
} cam_dv_parameter_e;

extern cam_dv_parameter_e dv_parameter;
extern camera_dv_cfg_t g_camera_dv_cfgs[];
extern camera_dv_cfg_t g_camera_dv_option_visible_cfgs[];
/******************************************************************************/
/*!                                                                             
 * \par  Description:                                                           
 *	  -- ��ʼ������                                      
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
bool camera_dv_init_all_cfg(void);
/******************************************************************************/
/*!                                                                             
 * \par  Description:                                                           
 *	  -- �����˳�                                      
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
bool camera_dv_deinit_all_cfg(void);
/******************************************************************************/
/*!                                                                             
 * \par  Description:                                                           
 *	  camera_dv_cfg_init-- �������ַ�������ռ�                                      
 * \param[in]     cfg[]                                                      
 * \param[int]   count                                                           
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
extern bool camera_dv_cfg_init(camera_dv_cfg_t cfgs[], int count);
/******************************************************************************/
/*!                                                                             
 * \par  Description:                                                           
 *	  camera_dv_cfg_deinit-- �������ַ�������Ŀռ��ͷ�                                      
 * \param[in]     cfg                                                     
 * \param[in]     count                                                          
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
extern bool camera_dv_cfg_deinit(camera_dv_cfg_t cfgs[], int count);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_cfg_read-- ��ȡ����������
 * \param[in]     ������
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
extern bool camera_dv_cfg_read(camera_dv_cfg_t * cfg);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_cfg_read_all-- ��ȡ����������
 * \param[in]     ������
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
extern bool camera_dv_cfg_read_all(camera_dv_cfg_t cfgs[], int count);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_cfg_write-- д����ָ��������
 * \param[in]     ������
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
extern bool camera_dv_cfg_write(camera_dv_cfg_t * cfg);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_cfg_write_all-- д����������
 * \param[in]     ������
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
extern bool camera_dv_cfg_write_all(camera_dv_cfg_t cfgs[], int count);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_save_parameter-- �˵�ѡ���������
 * \param[in]     ѡ��
 * \param[in]     ����ֵ
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
 *	  camera_dv_cfg_after_read_value_regulate-- ��ȡ����ֵ����ֵУ��
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
 * \note           ��Ҫconfig���ò�֧�ָ���
 *******************************************************************************/
extern bool camera_dv_cfg_after_read_value_regulate(void);
/******************************************************************************/
/*!
 * \par  Description:
 *	  -- д������ǰ��ֵУ��
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
 * \note           ��Ҫconfig���ò�֧�ָ���
 *******************************************************************************/
extern bool camera_dv_cfg_before_write_value_regulate(void);
/******************************************************************************/
/*!                                                                             
 * \par  Description:                                                           
 *	  camera_dv_check_path-- ������õ�·���Ƿ���ڣ����������¼��Ŀ¼�򴴽�                                      
 * \param[in]     none                                                         
 * \return       the result      true �ɹ���false ʧ��                                                
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
