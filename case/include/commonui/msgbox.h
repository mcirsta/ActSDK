/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : msgbox.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-06-02             v1.0             build this file 
 ********************************************************************************/
/*!
 * \file     msgbox.h
 * \brief    message box widget interface
 * \author   herb hu 
 *               
 * \version 1.0
 * \date  2009/06/02
 *******************************************************************************/

/*! 
 * \defgroup   common_message_box    message box�ؼ��ӿں���
 */


#ifndef __MSGBOX_H__
#define __MSGBOX_H__


#include "include_case.h"
#include "include_psp.h"

/*!
 *  \par message box�ؼ������ź�����
 *  \li SIG_MSGBOX_SHOW         : ��ʾ��Ϣ�� 
 *  \li SIG_MSGBOX_QUIT         : �˳���Ϣ�� 
 *  \li SIG_MSGBOX_USB_LAUNCHED : ������USBӦ�� 
 *  \li SIG_MSGBOX_TVIN_LAUNCHED : ������TVINӦ�� 
 *  \li WIDGET_SIG_MSGBOX_CAMERA_LAUNCHED: ������CAMERAӦ��
 *  \li WIDGET_SIG_MSGBOX_PHOTO_LAUNCHED: ������PHOTOӦ��
 *  \li WIDGET_SIG_MSGBOX_PL_GEN_LAUNCHED: ������pl_gen Ӧ��
 *  \li SIG_MSGBOX_SHOW_LOW_POWER: ��ʾ�͵���ʾ�� 
 *  \li SIG_MSGBOX_QUIT_LOW_POWER: �˳��͵���ʾ�� 
 *  \li SIG_MSGBOX_SHOW_LOW_POWER_UHOST: ��ʾUHOST֧�ֵ͵���ʾ�� 
 *  \li SIG_MSGBOX_QUIT_LOW_POWER: �˳�UHOST֧�ֵ͵���ʾ�� 
 */
typedef enum
{
    WIDGET_SIG_MSGBOX_SHOW,
    WIDGET_SIG_MSGBOX_QUIT,
    WIDGET_SIG_MSGBOX_USB_LAUNCHED,
    WIDGET_SIG_MSGBOX_TVIN_LAUNCHED,
    WIDGET_SIG_MSGBOX_CAMERA_LAUNCHED,
    WIDGET_SIG_MSGBOX_PHOTO_LAUNCHED,
    WIDGET_SIG_MSGBOX_PL_GEN_LAUNCHED,
    WIDGET_SIG_MSGBOX_SHOW_LOW_POWER,
    WIDGET_SIG_MSGBOX_QUIT_LOW_POWER,
    WIDGET_SIG_MSGBOX_SHOW_LOW_POWER_UHOST,
    WIDGET_SIG_MSGBOX_QUIT_LOW_POWER_UHOST,
}msgbox_sig_e;

/*!
 *  \brief 
 *  message box �ؼ�����ṹ����
 */
typedef struct msgbox_s 
{
    /*! ����ID */
    int canvas_id;

    /*! �źŻص����� */
    void ( *callback)(struct msgbox_s * msgbox, msgbox_sig_e);

    /*! �ڲ�˽�����ݽṹָ�룬����message box �ؼ��ڲ�ʵ��ʹ�� */
    struct msgbox_private_data_s * data; 

}msgbox_t;

/*!
 *  \brief 
 *  message box �ؼ����źŻص���������
 */
typedef void (* msgbox_cb_t)(msgbox_t * msgbox, msgbox_sig_e);

/******************************************************************************/
/*!                    
 * \par  Description:
 *     message box �ؼ�GUI��Ϣ����������������Ҫ��GUI��Ϣ���ݸ��˺���
 * \ingroup common_message_box
 * \param [in] msgbox: message box�ؼ���� 
 * \param [in] pmsg: GUI��Ϣ 
 * \return  �ɹ�����true, ʧ�ܷ���false      
 *******************************************************************************/
bool msgbox_proc_gui(msgbox_t * msgbox, gui_wm_message_t *pmsg );

/******************************************************************************/
/*!                    
 * \par  Description:
 *     message box�ؼ�ϵͳ��Ϣ����������������Ҫ��ϵͳ��Ϣ���ݸ��˺���
 * \ingroup common_message_box
 * \param [in] msgbox: message box�ؼ���� 
 * \param [in] pmsg: ϵͳ��Ϣ 
 * \return  �ɹ�����true, ʧ�ܷ���false      
 *******************************************************************************/
bool msgbox_proc_sys(msgbox_t * msgbox, msg_apps_t *pmsg );


/******************************************************************************/
/*!                    
 * \par  Description:
 *     ����һ��message box�ؼ�  
 * \ingroup common_message_box
 * \param [in] callback: ���� message box �źŵĻص����� 
 * \return  �ɹ�����һ�� message box �ؼ����, ʧ�ܷ���NULL      
 *******************************************************************************/
msgbox_t * msgbox_create(msgbox_cb_t callback );


/******************************************************************************/
/*!                    
 * \par  Description:
 *     message box ��ͼ�ӿ�  
 * \ingroup common_message_box
 * \param [in] msgbox: message box�ؼ���� 
 * \return  �ɹ�����true, ʧ�ܷ���false      
 * \note �˺�������message box �ؼ�������Ԫ�أ�һ��������Ҫ�������message box�ؼ�
 *******************************************************************************/
bool msgbox_paint( msgbox_t * msgbox );

/******************************************************************************/
/*!                    
 * \par  Description:
 *     ɾ��һ��message box�ؼ�  
 * \ingroup common_message_box
 * \param [in] msgbox: message box�ؼ���� 
 * \return  �ɹ�����true, ʧ�ܷ���false      
 *******************************************************************************/
bool msgbox_delete( msgbox_t * msgbox );

/******************************************************************************/
/*!                    
 * \par  Description:
 *     ����style��Դ  
 * \ingroup common_message_box
 * \param [in] msgbox: message box�ؼ���� 
 * \return  �ɹ�����true, ʧ�ܷ���false      
 * \note �����ڶ�̬�л���ʾ���ʱ
 *******************************************************************************/
bool msgbox_load_style( msgbox_t * msgbox);

#endif /* #ifndef __MSGBOX_H__ */ 

