/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : loadingbox.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
 * \file     loadingbox.h
 * \brief    loadingbox widget interface
 * \author   songzhining
 *               
 * \version 1.0
 * \date  2009/02/25
 *******************************************************************************/

/*! 
 * \defgroup   common_loadingbox    loadingbox�ؼ��ӿں���
 */

#ifndef __LOADINGBOX_H__
#define __LOADINGBOX_H__


#include "include_case.h"

/*!
 * \par loadingbox ��ʾ���� 
 *  \li LOADINGBOX_SEARCHING: ������
 *  \li LOADINGBOX_WAITING: ��ȴ�
 *  \li LOADINGBOX_MAX : ����������Ŀ
 */
typedef enum
{
    LOADINGBOX_SEARCHING,
    LOADINGBOX_WAITING,
    LOADINGBOX_MAX,
    
}loadingbox_type_e;

/*!
 *  \par loadingbox �ؼ������ź�����
 *  \li WIDGET_SIG_LOADINGBOX_OK : ȷ��
 *  \li WIDGET_SIG_LOADINGBOX_QUIT : �˳�
 */
typedef enum
{
    WIDGET_SIG_LOADINGBOX_OK,
    WIDGET_SIG_LOADINGBOX_QUIT,
    
}loadingbox_sig_e;

/*!
 *  \brief 
 *  loadingbox �ؼ�����ṹ����
 */
typedef struct loadingbox_s
{
    /*! �б���ʾ���� */
    loadingbox_type_e type;
    
    /*! ����ID */
    int canvas_id;
    
    /*! �źŻص����� */
    void (*callback)( struct loadingbox_s *, loadingbox_sig_e );

    /*! �ڲ�˽�����ݽṹָ�룬���� loadingbox �ؼ��ڲ�ʵ��ʹ�� */
    struct loadingbox_private_s * private; 

}loadingbox_t;


/*!
 *  \brief 
 *  loadingbox �ؼ����źŻص���������
 */
typedef void (*loadingbox_cb_t)( loadingbox_t *, loadingbox_sig_e );

/* loadingbox �ؼ������ӿ� */

/******************************************************************************/
/*!                    
* \par  Description:
*     ����һ��loadingbox�ؼ�  
* \ingroup common_loadingbox
* \param [in] type: �б���ʾ����
* \param [in] callback: ���� loadingbox �źŵĻص����� 
* \return  �ɹ�����һ�� loadingbox ���, ʧ�ܷ���NULL      
*******************************************************************************/
loadingbox_t* loadingbox_create( loadingbox_type_e type, loadingbox_cb_t callback );

/******************************************************************************/
/*!                    
* \par  Description:
*     ɾ��һ��loadingbox�ؼ�  
* \ingroup common_loadingbox
* \param [in] loadingbox: loadingbox�ؼ���� 
* \return  �ɹ�����true, ʧ�ܷ���false      
*******************************************************************************/
bool loadingbox_delete( loadingbox_t *loadingbox );

/******************************************************************************/
/*!                    
* \par  Description:
*     loadingbox ��ͼ�ӿ�  
* \ingroup common_loadingbox
* \param [in] loadingbox: loadingbox�ؼ���� 
* \return  �ɹ�����true, ʧ�ܷ���false      
* \note �˺�������loadingbox�ؼ�������Ԫ�أ�һ��������Ҫ�������loadingbox�ؼ�
*******************************************************************************/
bool loadingbox_paint( loadingbox_t *loadingbox );

/******************************************************************************/
/*!                    
* \par  Description:
*     loadingbox �ؼ�GUI��Ϣ����������������Ҫ��GUI��Ϣ���ݸ��˺���
* \ingroup common_loadingbox
* \param [in] loadingbox: loadingbox�ؼ���� 
* \param [in] pmsg: GUI��Ϣ 
* \return  �ɹ�����true, ʧ�ܷ���false      
* \note �ؼ��ڲ����¼�����ʱ,�ȵ��õ�ǰ�˵���Ļص�, �ٵ��ÿؼ����źŻص�����
*******************************************************************************/
bool loadingbox_proc_gui( loadingbox_t *loadingbox, gui_wm_message_t *pmsg );

/******************************************************************************/
/*!                    
* \par  Description:
*     loadingbox �ؼ�����style��Դ  
* \ingroup common_loadingbox
* \param [in] loadingbox: loadingbox�ؼ���� 
* \return  �ɹ�����true, ʧ�ܷ���false      
* \note �ⲿ���ý����ڶ�̬�л���ʾ���ʱ
*******************************************************************************/
bool loadingbox_load_style( loadingbox_t *loadingbox );

#endif

