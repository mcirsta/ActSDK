/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : progress_box.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    herb hu      2009-07-27          v1.0               build this file 
 ********************************************************************************/
/*!
 * \file     progress_box.h
 * \brief    progress_box widget interface
 * \author   herb hu 
 *               
 * \version 1.0
 * \date  2009/07/27
 *******************************************************************************/

/*! 
 * \defgroup   common_progress_box    progress_box �ؼ��ӿں���
 */

#ifndef __PROGRESS_BOX_H__
#define __PROGRESS_BOX_H__

#include "include_case.h"

/*!
*\brief
	progress box �ؼ���ʼ���ṹ
*/
typedef struct
{
    /*! ���ȴ����б����ַ�����Դid */
	int text_str_id;
    /*! ���ȴ�����ʱ�����ַ�����Դid */
	int complete_text_str_id;
    /*! �ܽ��� */
	unsigned int total_step;
    /*! ��ǰ���� */
	unsigned int cur_step;
}progress_box_init_t;

/*!
 *  \par progress box�ؼ������ź�����
 *  \li WIDGET_SIG_PROGRESS_BOX_COMPLETE: ������� 
 */
typedef enum
{
    WIDGET_SIG_PROGRESS_BOX_COMPLETE,
}progress_box_sig_e;

/*!
 *  \brief 
 *  progress box �ؼ�����ṹ����
 */
typedef struct progress_box_s
{
    /*! ����ID */
    int canvas_id;
    
    /*! �źŻص����� */
    void (*callback)( struct progress_box_s *, progress_box_sig_e );

    /*! �ڲ�˽�����ݽṹָ�룬���� progress box �ؼ��ڲ�ʵ��ʹ�� */
    struct progress_box_private_s * private; 

}progress_box_t;


/*!
 *  \brief 
 *  progress_box �ؼ����źŻص���������
 */
typedef void (*progress_box_cb_t)( progress_box_t *, progress_box_sig_e );

/* progress_box �ؼ������ӿ� */

/******************************************************************************/
/*!                    
* \par  Description:
*     ����һ��progress_box�ؼ�  
* \ingroup common_progress_box
* \param [in] init: ��ʼ������ 
* \param [in] callback: ���� progress_box �źŵĻص����� 
* \return  �ɹ�����һ�� progress_box ���, ʧ�ܷ���NULL      
*******************************************************************************/
progress_box_t* progress_box_create( progress_box_init_t * init, progress_box_cb_t callback );

/******************************************************************************/
/*!                    
* \par  Description:
*     ɾ��һ��progress_box�ؼ�  
* \ingroup common_progress_box
* \param [in] progress_box: progress_box�ؼ���� 
* \return  �ɹ�����true, ʧ�ܷ���false      
*******************************************************************************/
bool progress_box_delete( progress_box_t *progress_box );

/******************************************************************************/
/*!                    
* \par  Description:
*     progress_box ��ͼ�ӿ�  
* \ingroup common_progress_box
* \param [in] progress_box: progress_box�ؼ���� 
* \return  �ɹ�����true, ʧ�ܷ���false      
* \note �˺�������progress_box�ؼ�������Ԫ�أ�һ��������Ҫ�������progress_box�ؼ�
*******************************************************************************/
bool progress_box_paint( progress_box_t *progress_box );

/******************************************************************************/
/*!                    
* \par  Description:
*     progress_box �ؼ�GUI��Ϣ����������������Ҫ��GUI��Ϣ���ݸ��˺���
* \ingroup common_progress_box
* \param [in] progress_box: progress_box�ؼ���� 
* \param [in] pmsg: GUI��Ϣ 
* \return  �ɹ�����true, ʧ�ܷ���false      
* \note �ؼ��ڲ����¼�����ʱ,�ȵ��õ�ǰ�˵���Ļص�, �ٵ��ÿؼ����źŻص�����
*******************************************************************************/
bool progress_box_proc_gui( progress_box_t *progress_box, gui_wm_message_t *pmsg );

/******************************************************************************/
/*!                    
* \par  Description:
*     progress_box �ؼ�����style��Դ  
* \ingroup common_progress_box
* \param [in] progress_box: progress_box�ؼ���� 
* \return  �ɹ�����true, ʧ�ܷ���false      
* \note �ⲿ���ý����ڶ�̬�л���ʾ���ʱ
*******************************************************************************/
bool progress_box_load_style( progress_box_t *progress_box );

/******************************************************************************/
/*!                    
* \par  Description:
*     ����progress_box �ؼ���ǰ����
* \ingroup common_progress_box
* \param [in] progress_box: progress_box�ؼ���� 
* \param [in] step: ��ǰ���� 
* \return  �ɹ�����true, ʧ�ܷ���false      
*******************************************************************************/
bool progress_box_set_cur_step( progress_box_t *progress_box, unsigned int step);

/******************************************************************************/
/*!                    
* \par  Description:
*     ����progress_box �ؼ���ǰ��ʾ�ı� 
* \ingroup common_progress_box
* \param [in] progress_box: progress_box�ؼ���� 
* \param [in] text_str_id: ��ǰ��ʾ�ı�id 
* \return  �ɹ�����true, ʧ�ܷ���false      
*******************************************************************************/
bool progress_box_set_text( progress_box_t *progress_box, int text_str_id);

#endif

