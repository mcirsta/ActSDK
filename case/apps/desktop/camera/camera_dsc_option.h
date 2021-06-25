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
 * \brief    menu�˵�ѡ��ͷ�ļ�
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
 *  �˵���
 */
{
    /*! �ط� */
    CAM_DSC_OPT_PLAYBACK = 0,
    /*! ��ƽ�� */
    CAM_DSC_OPT_WHITE_BALANCE,
    /*! ������ */
    CAM_DSC_OPT_BRIGHTNESS,
    /*! �ع�ģʽ */
    CAM_DSC_OPT_EXPOSURE_MODE,
    /*! ����� */
    CAM_DSC_OPT_FLASH_LAMP,
    /*! ������� */
    CAM_DSC_OPT_FACE_IDENTIFY,
    /*! ��ͷ�� */
    CAM_DSC_OPT_STICKER_PHOTO,
    /*! ��Ч */
    CAM_DSC_OPT_SPECIAL_EFFECT,
    /*! ����·�� */
    CAM_DSC_OPT_SAVE_PATH,
    /*! �ֱ��� */
    CAM_DSC_OPT_RESOLUTION,
    /*! ʣ��ռ� */
    CAM_DSC_OPT_FREE_SPACE,
    /*! ����ʱ�� */
    CAM_DSC_OPT_SELF_TIMER,
    /*! ����ʱ�� */
    CAM_DSC_OPT_CONT_SHOOTING,
    /*! ������ */
    CAM_DSC_OPT_SHUTTER_SOUND,
    /*! ��Ƭ�ϴ�ӡ���� */
    CAM_DSC_OPT_DATE_STAMP,
    /*! ����ö��ֵ */
    CAM_DSC_OPT_ERR = 0xFF,
} cam_dsc_option_e;

extern picture_resource_t *g_cam_dsc_opt_dot;

/******************************************************************************/
/*!                                                                             
 * \par  Description:                                                           
 *	 camera_dsc_playback-- ����¼�ƻط�                                      
 * \param[in]     none                                                      
 * \param[out]   none                                                           
 * \return       the result      true �ɹ���false ʧ��                                                
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
 *	  camera_dsc_option_enter-- �������˵�
 * \param[in]     menuview�ṹ
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
extern bool camera_dsc_option_enter(menuview_t *menuview, fusion_enter_e fenter );
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_option_exit-- �˳����˵�
 * \param[in]     menuview�ṹ
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
extern bool camera_dsc_option_exit(menuview_t *menuview);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dsc_option_init-- ��ʼ���˵�����
 * \param[in]     none
 * \return       the result      menu�ṹ
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
 *	  camera_dsc_option_deinit-- ɾ���˵�����
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
extern int camera_dsc_option_deinit(void);

#endif   /* __CAMERA_DSC_OPTION_H__ */
