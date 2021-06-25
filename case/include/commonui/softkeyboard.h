/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : softkeyboard.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-4-14             v1.0             build this file 
 ********************************************************************************/
/*!
 * \file     softkeyboard.h
 * \brief    softkeyboard widget interface
 * \author   herb hu 
 *               
 * \version 1.0
 * \date  2009/04/14
 *******************************************************************************/

/*! 
 * \defgroup   common_softkeyboard    softkeyboard�ؼ��ӿں���
 */


#ifndef __SOFT_KEYBOARD_H__
#define __SOFT_KEYBOARD_H__


#include "include_case.h"

/*!
 * \par  Ĭ�����뷨���� 
 *  \li IME_TYPE_EN : Ӣ�����뷨 
 *  \li IME_TYPE_PINYIN : ����ƴ�����뷨 
 *  \li IME_TYPE_NUM : �������뷨 
 */
typedef enum
{
    IME_TYPE_EN,
    IME_TYPE_PINYIN,
    IME_TYPE_NUM
}IME_type_e;

/*!
 *  \par softkeyboard �ؼ������ź�����
 *  \li SIG_SKB_OK : ������� 
 *  \li SIG_SKB_CANCEL : ȡ������ 
 */
typedef enum
{
    WIDGET_SIG_SKB_OK,
    WIDGET_SIG_SKB_CANCEL,
}softkeyboard_sig_e;

/*!
 *  \brief 
 *  softkeyboard �ؼ�����ṹ����
 */
typedef struct softkeyboard_s 
{
    /*! ����ID */
    int canvas_id;

    /*! ��ͼDC */
    int hdc;

    /*! �źŻص����� */
    void ( *callback)(struct softkeyboard_s * keyboard, softkeyboard_sig_e);

    /*! �ڲ�˽�����ݽṹָ�룬����softkeyboard �ؼ��ڲ�ʵ��ʹ�� */
    struct softkey_private_data_s * data; 

}softkeyboard_t;

/*!
 *  \brief 
 *  softkeyboard �ؼ����źŻص���������
 */
typedef void (* softkeyboard_cb_t)(softkeyboard_t * keyboard, softkeyboard_sig_e);

/******************************************************************************/
/*!                    
 * \par  Description:
 *     softkeyboard �ؼ�GUI��Ϣ����������������Ҫ��GUI��Ϣ���ݸ��˺���
 * \ingroup common_softkeyboard
 * \param [in] keyboard: softkeyboard�ؼ���� 
 * \param [in] pmsg: GUI��Ϣ 
 * \return  �ɹ�����true, ʧ�ܷ���false      
 *******************************************************************************/

bool softkeyboard_proc_gui(softkeyboard_t * keyboard, gui_wm_message_t *pmsg );


/******************************************************************************/
/*!                    
 * \par  Description:
 *     ����һ��softkeyboard�ؼ�  
 * \ingroup common_softkeyboard
 * \param [in] ime: Ĭ�����뷨���� 
 * \param [in] callback: ���� softkeyboard �źŵĻص����� 
 * \return  �ɹ�����һ�� softkeyboard ���, ʧ�ܷ���NULL      
 *******************************************************************************/

softkeyboard_t * softkeyboard_create( IME_type_e ime, softkeyboard_cb_t callback );


/******************************************************************************/
/*!                    
 * \par  Description:
 *     softkeyboard ��ͼ�ӿ�  
 * \ingroup common_softkeyboard
 * \param [in] keyboard: softkeyboard�ؼ���� 
 * \return  �ɹ�����true, ʧ�ܷ���false      
 * \note �˺�������softkeyboard�ؼ�������Ԫ�أ�һ��������Ҫ�������softkeyboard�ؼ�
 *******************************************************************************/

bool softkeyboard_paint( softkeyboard_t * keyboard );

/******************************************************************************/
/*!                    
 * \par  Description:
 *     ɾ��һ��softkeyboard�ؼ�  
 * \ingroup common_softkeyboard
 * \param [in] keyboard: softkeyboard�ؼ���� 
 * \return  �ɹ�����true, ʧ�ܷ���false      
 *******************************************************************************/

bool softkeyboard_delete( softkeyboard_t * keyboard );


/******************************************************************************/
/*!                    
 * \par  Description:
 *     ��ȡ�û������ı���Ϣ(unicode �ַ���) 
 * \ingroup common_softkeyboard
 * \param [in] keyboard: softkeyboard�ؼ���� 
 * \param [in] utf8_text: ��Ž���ַ���(utf8�����ַ�)�Ļ�����
 * \param [in] len:  ��������С 
 * \return �ɹ�����true, ʧ�ܷ���false;       
 * \note 1.���������յ� SIG_SKB_DONE �źź�ͨ���˽ӿڻ�ȡ�û������ı�\n
 *       2.�����������ȷ�����㹻��С�Ļ�����(�����ַ���������־)���������ʧ��\n
 *       3.�õ��óɹ�ʱ���Զ����ַ���ĩβ���㣬����Ϊ�ַ����Ľ�����־\n
 *******************************************************************************/

bool softkeyboard_get_text( softkeyboard_t * keyboard , char * utf8_text, unsigned int len);


/******************************************************************************/
/*!                    
 * \par  Description:
 *     Ԥ���û������ı�   
 * \ingroup common_softkeyboard
 * \param [in] keyboard: softkeyboard�ؼ���� 
 * \param [in] utf8_text: utf8�����ַ��� 
 * \return  �ɹ�����true, ʧ�ܷ���false      
 *******************************************************************************/

bool softkeyboard_set_text( softkeyboard_t * keyboard , const char * utf8_text);


/******************************************************************************/
/*!                    
 * \par  Description:
 *     ����û������ı�  
 * \ingroup common_softkeyboard
 * \param [in] keyboard: softkeyboard�ؼ���� 
 * \return  �ɹ�����true, ʧ�ܷ���false      
 *******************************************************************************/

bool softkeyboard_clear( softkeyboard_t * keyboard);


/******************************************************************************/
/*!                    
 * \par  Description:
 *     ����style��Դ  
 * \ingroup common_softkeyboard
 * \param [in] keyboard: softkeyboard�ؼ���� 
 * \return  �ɹ�����true, ʧ�ܷ���false      
 * \note �����ڶ�̬�л���ʾ���ʱ
 *******************************************************************************/

bool softkeyboard_load_style( softkeyboard_t * keyboard);

#endif /* #ifndef __SOFT_KEYBOARD_H__ */ 

