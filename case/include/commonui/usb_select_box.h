/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : usb_select_box.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      herb hu   2009-07-14          v1.0               build this file 
 ********************************************************************************/
/*!
 * \file     usb_select_box.h
 * \brief    usb_select_box widget interface
 * \author   herb hu 
 *               
 * \version 1.0
 * \date  2009/07/14
 *******************************************************************************/

/*! 
 * \defgroup   common_usb_select_box     usb_select_box�ؼ��ӿں���
 */

#ifndef __USB_SELECT_BOX_H__
#define __USB_SELECT_BOX_H__


#include "include_case.h"

/*!
 * \par usb select box ��ʾ���� 
 * \li USB_SELECT_BOX_MTP ����MTPѡ����
 * \li USB_SELECT_DATA_TRANS �������ݴ���ѡ����
 */
typedef enum
{
    USB_SELECT_BOX_MTP,
    USB_SELECT_BOX_DATA_TRANS,
    USB_SELECT_BOX_ADAPTER,
}usb_select_box_type_e;

/*!
 *  \par usb select box �ؼ������ź�����
 *  \li WIDGET_SIG_SELECT_MTP: ѡ����MTP�豸ѡ���� 
 *  \li WIDGET_SIG_SELECT_DATA_TRANS: ѡ�������ݴ���ѡ���� 
 *  \li WIDGET_SIG_SELECT_CHARGE: ѡ���˳��ѡ���� 
 *  \li WIDGET_SIG_SELECT_CHARGE_PLAY: ѡ���˺�̨���ѡ���� 
 */
typedef enum
{
    WIDGET_SIG_USB_SEL_BOX_QUIT,
    WIDGET_SIG_USB_SEL_BOX_MTP,
    WIDGET_SIG_USB_SEL_BOX_DATA_TRANS,
    WIDGET_SIG_USB_SEL_BOX_CHARGE,
    WIDGET_SIG_USB_SEL_BOX_CHARGE_PLAY,
}usb_select_box_sig_e;

/*!
 *  \brief 
 *  usb select box �ؼ�����ṹ����
 */
typedef struct usb_select_box_s
{
    /*! ��ʾ���� */
    usb_select_box_type_e type;
    
    /*! ����ID */
    int canvas_id;
    
    /*! �źŻص����� */
    void (*callback)( struct usb_select_box_s *, usb_select_box_sig_e );

    /*! �ڲ�˽�����ݽṹָ�룬���� usb select box �ؼ��ڲ�ʵ��ʹ�� */
    struct usb_select_box_private_s * private; 

}usb_select_box_t;


/*!
 *  \brief 
 *  usb select box �ؼ����źŻص���������
 */
typedef void (*usb_select_box_cb_t)( usb_select_box_t *, usb_select_box_sig_e );

/* usb select box �ؼ������ӿ� */

/******************************************************************************/
/*!                    
* \par  Description:
*     ����һ��usb select box�ؼ�  
* \ingroup common_usb_select_box
* \param [in] type: ��ʾ����
* \param [in] callback: ���� usb select box �źŵĻص����� 
* \return  �ɹ�����һ�� usb select box ���, ʧ�ܷ���NULL      
*******************************************************************************/
usb_select_box_t* usb_select_box_create( usb_select_box_type_e type, usb_select_box_cb_t callback );

/******************************************************************************/
/*!                    
* \par  Description:
*     ɾ��һ��usb select box�ؼ�  
* \ingroup common_usb_select_box
* \param [in] usb select box: usb select box�ؼ���� 
* \return  �ɹ�����true, ʧ�ܷ���false      
*******************************************************************************/
bool usb_select_box_delete( usb_select_box_t *usb_select_box );

/******************************************************************************/
/*!                    
* \par  Description:
*     usb select box ��ͼ�ӿ�  
* \ingroup common_usb_select_box
* \param [in] usb select box: usb select box�ؼ���� 
* \return  �ɹ�����true, ʧ�ܷ���false      
* \note �˺�������usb select box�ؼ�������Ԫ�أ�һ��������Ҫ�������usb select box�ؼ�
*******************************************************************************/
bool usb_select_box_paint( usb_select_box_t *usb_select_box );

/******************************************************************************/
/*!                    
* \par  Description:
*     usb select box �ؼ�GUI��Ϣ����������������Ҫ��GUI��Ϣ���ݸ��˺���
* \ingroup common_usb_select_box
* \param [in] usb select box: usb select box�ؼ���� 
* \param [in] pmsg: GUI��Ϣ 
* \return  �ɹ�����true, ʧ�ܷ���false      
* \note �ؼ��ڲ����¼�����ʱ,�ȵ��õ�ǰ�˵���Ļص�, �ٵ��ÿؼ����źŻص�����
*******************************************************************************/
bool usb_select_box_proc_gui( usb_select_box_t *usb_select_box, gui_wm_message_t *pmsg );

/******************************************************************************/
/*!                    
* \par  Description:
*     usb select box �ؼ�����style��Դ  
* \ingroup common_usb_select_box
* \param [in] usb select box: usb select box�ؼ���� 
* \return  �ɹ�����true, ʧ�ܷ���false      
* \note �ⲿ���ý����ڶ�̬�л���ʾ���ʱ
*******************************************************************************/
bool usb_select_box_load_style( usb_select_box_t *usb_select_box );

#endif

