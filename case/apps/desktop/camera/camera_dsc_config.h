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
 *  �趨ֵ������
 */
typedef enum
{
    /*! config value type is string*/
    DSC_CFG_VALUE_TYPE_STRING = 0,

    /*! config value type is integer*/
    DSC_CFG_VALUE_TYPE_INTEGER,
    /*! ����ö��ֵ */
    DSC_CFG_VALUE_TYPE_ERR = 0xFF,
} dsc_cfg_value_type_e;

/*!
 *  \brief
 *  config����
 */
typedef struct
{
    /*! config key name */
    char key[32];

    /*! value type */
    dsc_cfg_value_type_e value_type;

    /*! to store string type value */
    #if 0
    //�����Ϊ��̬�����ִ��ռ䣬ע���ͷ�
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
 *  configѡ��
 */
typedef enum
{  
    /*! �ط� */
    CAM_DSC_CFG_VISIBLE_TYPE_PLAYBACK = 0,
    /*! ��ƽ�� */
    CAM_DSC_CFG_VISIBLE_TYPE_WHITE_BALANCE,
    /*! ������ */
    CAM_DSC_CFG_VISIBLE_TYPE_BRIGHTNESS,
    /*! �ع�ģʽ */
    CAM_DSC_CFG_VISIBLE_TYPE_EXPOSURE_MODE,
    /*! ����� */
    CAM_DSC_CFG_VISIBLE_TYPE_FLASH_LAMP,
    /*! ������� */
    CAM_DSC_CFG_VISIBLE_TYPE_FACE_IDENTIFY,
    /*! ��ͷ�� */
    CAM_DSC_CFG_VISIBLE_TYPE_STICKERPHOTO,
    /*! ��Ч */
    CAM_DSC_CFG_VISIBLE_TYPE_SPECIAL_EFFECT,
    /*! ����·�� */
    CAM_DSC_CFG_VISIBLE_TYPE_SAVE_PATH,
    /*! �ֱ��� */
    CAM_DSC_CFG_VISIBLE_TYPE_RESOLUTION,
    /*! ʣ��ռ� */
    CAM_DSC_CFG_VISIBLE_TYPE_FREE_SPACE,    
    /*! ����ʱ�� */
    CAM_DSC_CFG_VISIBLE_TYPE_SELF_TIMER,
    /*! ����ʱ�� */
    CAM_DSC_CFG_VISIBLE_TYPE_CONT_SHOOTING,
    /*! ������ */
    CAM_DSC_CFG_VISIBLE_TYPE_SHUTTER_SOUND,
    /*! ��Ƭ�����ڱ�� */
    CAM_DSC_CFG_VISIBLE_TYPE_DATE_STAMP,
    /*! ���� */
    CAM_DSC_CFG_VISIBLE_TYPE_TOTAL,
    /*! ����ö�� */
    CAM_DSC_CFG_VISIBLE_TYPE_ERR = 0xFF,

} camera_dsc_option_visible_cfg_type_e;


/*!
 *  \brief
 *  configѡ��
 */
typedef enum
{  
    /*! �ط� */
    CAM_DSC_CFG_TYPE_PLAYBACK = 0,
    /*! ��ƽ�� */
    CAM_DSC_CFG_TYPE_WHITE_BALANCE,
    /*! ������ */
    CAM_DSC_CFG_TYPE_BRIGHTNESS,
    /*! �ع�ģʽ */
    CAM_DSC_CFG_TYPE_EXPOSURE_MODE,
    /*! ����� */
    CAM_DSC_CFG_TYPE_FLASH_LAMP,
    /*! ������� */
    CAM_DSC_CFG_TYPE_FACE_IDENTIFY,
    /*! ��ͷ�� */
    CAM_DSC_CFG_TYPE_STICKERPHOTO,
    /*! ��Ч */
    CAM_DSC_CFG_TYPE_SPECIAL_EFFECT,
    /*! ����·�� */
    CAM_DSC_CFG_TYPE_SAVE_PATH,
    /*! �ֱ��� */
    CAM_DSC_CFG_TYPE_RESOLUTION,
#if 0
    /*! ����ʱ�� */
    CAM_DSC_CFG_TYPE_SELF_TIMER,
    /*! ����ʱ�� */
    CAM_DSC_CFG_TYPE_CONT_SHOOTING,
#endif
    /*! ������ */
    CAM_DSC_CFG_TYPE_SHUTTER_SOUND,
    /*! ��Ƭ�����ڱ�� */
    CAM_DSC_CFG_TYPE_DATE_STAMP,
#if (SHORTCUT_PLAYBACK_DSC == 1)
    /*! Ԥ������·�� */
    CAM_DSC_CFG_PLAYBACK_ROOT,
    /*! Ԥ��ȫ·���ļ��� */
    CAM_DSC_CFG_PLAYBACK_FULLNAME,    
#endif /* SHORTCUT_PLAYBACK_DSC */

    /*! ���� */
    CAM_DSC_CFG_TYPE_TOTAL,
    /*! ����ö�� */
    CAM_DSC_CFG_TYPE_ERR = 0xFF,

} camera_dsc_cfg_type_e;

/*!
 *  \brief
 *  ѡ�����
 */
typedef enum
{
    /*! �ط� */
    CAM_DSC_PARA_PLAYBACK = 0,
    /*! ��ƽ�� */
    CAM_DSC_PARA_WHITE_BALANCE,
    /*! ������ */
    CAM_DSC_PARA_BRIGHTNESS,
    /*! �ع�ģʽ */
    CAM_DSC_PARA_EXPOSURE_MODE,
    /*! ����� */
    CAM_DSC_PARA_FLASH_LAMP,
    /*! ������� */
    CAM_DSC_PARA_FACE_IDENTIFY,
    /*! ��ͷ�� */
    CAM_DSC_PARA_STICKER_PHOTO,
    /*! ��Ч */
    CAM_DSC_PARA_SPECIAL_EFFECT,
    /*! ����·�� */
    CAM_DSC_PARA_SAVE_PATH,
    /*! �ֱ��� */
    CAM_DSC_PARA_RESOLUTION,
    /*! ʣ��ռ� */
    CAM_DSC_PARA_FREE_SPACE,
    /*! ����ʱ�� */
    CAM_DSC_PARA_SELF_TIMER,
    /*! ����ʱ�� */
    CAM_DSC_PARA_CONT_SHOOTING,
    /*! ������ */
    CAM_DSC_PARA_SHUTTER_SOUND,
    /*! ��Ƭ�ϴ�ӡ���� */
    CAM_DSC_PARA_DATE_STAMP,
    /*����佹���ż���*/
    CAM_DSC_PARA_ZOOM_LEVEL,
    /*! ����ö��ֵ */
    CAM_DSC_PARA_ERR = 0xFF,
} cam_dsc_parameter_e;

extern camera_dsc_cfg_t g_camera_dsc_cfgs[];
extern camera_dsc_cfg_t g_camera_dsc_option_visible_cfgs[];
extern camera_dsc_cfg_t g_camera_dsc_sticker_cfgs[];
extern camera_dsc_cfg_t g_camera_dsc_shutter_sound_cfgs[];
/******************************************************************************/
/*!                                                                             
 * \par  Description:                                                           
 *	  camera_dsc_init_all_cfg-- ��ʼ��dsc��������                                      
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
bool camera_dsc_init_all_cfg(void);
/******************************************************************************/
/*!                                                                             
 * \par  Description:                                                           
 *	  camera_dsc_deinit_all_cfg-- �����˳�                                      
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
bool camera_dsc_deinit_all_cfg(void);
/******************************************************************************/
/*!                                                                             
 * \par  Description:                                                           
 *	  camera_dsc_cfg_init-- �������ַ�������ռ�                                      
 * \param[in]     cfg[]                                                      
 * \param[int]   count                                                           
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
extern bool camera_dsc_cfg_init(camera_dsc_cfg_t cfgs[], int count);
/******************************************************************************/
/*!                                                                             
 * \par  Description:                                                           
 *	  camera_dsc_cfg_deinit-- �������ַ�������Ŀռ��ͷ�                                      
 * \param[in]     cfg                                                     
 * \param[in]     count                                                          
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
extern bool camera_dsc_cfg_deinit(camera_dsc_cfg_t cfgs[], int count);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_cfg_read-- ��ȡ����������
 * \param[in]     ������
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
extern bool camera_dsc_cfg_read(camera_dsc_cfg_t * cfg);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_cfg_read_all-- ��ȡ����������
 * \param[in]     ������
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
extern bool camera_dsc_cfg_read_all(camera_dsc_cfg_t cfgs[], int count);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_cfg_write-- д����ָ��������
 * \param[in]     ������
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
extern bool camera_dsc_cfg_write(camera_dsc_cfg_t * cfg);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_cfg_write_all-- д����������
 * \param[in]     ������
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
extern bool camera_dsc_cfg_write_all(camera_dsc_cfg_t cfgs[], int count);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_save_parameter-- �˵�ѡ���������
 * \param[in]     ѡ��
 * \param[in]     ����ֵ
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
 *	  camera_dsc_cfg_after_read_value_regulate-- ��ȡ����ֵ����ֵУ��
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
 * \note           ��Ҫconfig���ò�֧�ָ���
 *******************************************************************************/
extern bool camera_dsc_cfg_after_read_value_regulate(void);
/******************************************************************************/
/*!
 * \par  Description:
 *	  -- д������ǰ��ֵУ��
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
 * \note           ��Ҫconfig���ò�֧�ָ���
 *******************************************************************************/
extern bool camera_dsc_cfg_before_write_value_regulate(void);

/******************************************************************************/
/*!                                                                             
 * \par  Description:                                                           
 *	  camera_dsc_check_path-- ������õ�·���Ƿ���ڣ����������¼��Ŀ¼�򴴽�                                      
 * \param[in]     none                                                         
 * \return       the result      true �ɹ���false ʧ��                                                
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

