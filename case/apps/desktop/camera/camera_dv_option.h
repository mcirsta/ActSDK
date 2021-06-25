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
 * \brief    menu�˵�ѡ��ͷ�ļ�
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
 *  �˵���
 */
{
    /*! �ط� */
    CAM_DV_OPT_PLAYBACK = 0,
    /*! ��ƽ�� */
    CAM_DV_OPT_WHITE_BALANCE,
    /*! ������ */
    CAM_DV_OPT_BRIGHTNESS,
    /*! �ع�ģʽ */
    CAM_DV_OPT_EXPOSURE_MODE,
    /*! ��Ч */
    CAM_DV_OPT_SPECIAL_EFFECT,
    /*! ����·�� */
    CAM_DV_OPT_SAVE_PATH,
    /*! �ֱ��� */
    CAM_DV_OPT_RESOLUTION,
    /*! ʣ��ռ� */
    CAM_DV_OPT_FREE_SPACE,
    /*! ¼������ */
    CAM_DV_OPT_RECORD_QUALITY,
    /*! ¼������ */
    CAM_DV_OPT_RECORD_GAIN,
#if (DV_ACTIVITY_DETECT == 1)
    /*! ������� */
    CAM_DV_OPT_ACTIVITY_DETECT,
#endif
    /*! ����ö��ֵ */
    CAM_DV_OPT_ERR = 0xFF,
} cam_dv_option_e;

extern picture_resource_t *g_cam_dv_opt_dot;
/******************************************************************************/
/*!                                                                             
 * \par  Description:                                                           
 *	 camera_dv_playback-- ����¼�ƻط�                                      
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
bool camera_dv_playback(void);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_option_enter-- �������˵�
 * \param[in]     menuview�ṹ
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
bool camera_dv_option_enter(menuview_t *menuview, fusion_enter_e fenter );
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_option_exit-- �˳����˵�
 * \param[in]     menuview�ṹ
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
bool camera_dv_option_exit(menuview_t *menuview);
/******************************************************************************/
/*!
 * \par  Description:
 *	  camera_dv_option_init-- ��ʼ���˵�����
 * \param[in]     none
 * \return       the result      menu�ṹ
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
 *	  camera_dv_option_deinit-- ɾ���˵�����
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
int camera_dv_option_deinit(void);

#endif   /* __CAMERA_DV_OPTION_H__ */
