/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_playing_widget_usereq.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
 * \file     music_playing_widget_usereq.h
 * \brief    usereq_widget widget interface
 * \author   songzhining
 *               
 * \version 1.0
 * \date  2009/02/25
 *******************************************************************************/

/*! 
 * \defgroup   music    usereq_widget�ؼ��ӿں���
 */

#ifndef __USEREQ_H__
#define __USEREQ_H__


#include "include_case.h"


#include "music_sty.h"


/*!
 * \par usereq_widget ���EQ����
 */
#define USEREQ_MAX  5

/*!
 * \par usereq_widget EQ���������ֵ
 */
#define USEREQ_VAL_MAX  (+6)

/*!
 * \par usereq_widget ��������Сֵ
 */
#define USEREQ_VAL_MIN  (-6)

/*!
 * \par usereq_widget ��ʾ���� 
 */
typedef enum
{
    USEREQ_TYPE_NULL,
    
}usereq_widget_type_e;

/*!
 *  \par usereq_widget �ؼ������ź�����
 *  \li WIDGET_SIG_USEREQ_OK :ȷ��
 *  \li WIDGET_SIG_USEREQ_QUIT : �˳�
 *  \li WIDGET_SIG_USEREQ_CHANGED : EQ�޸�
 */
typedef enum
{
    WIDGET_SIG_USEREQ_OK,
    WIDGET_SIG_USEREQ_QUIT,
    WIDGET_SIG_USEREQ_CHANGED,
    
}usereq_widget_sig_e;

/*!
 *  \par usereq_widget �ؼ�EQ��Դ����
 */
typedef struct
{
    picture_resource_t *pic_bar_bg;
    picture_resource_t *pic_btn_unsel;
    picture_resource_t *pic_btn_sel;
    picture_resource_t *pic_slider_sel;
    picture_resource_t *pic_slider_unsel;

}usereq_res_t;

/*!
 *  \brief 
 *  usereq_widget �ؼ�����ṹ����
 */
typedef struct usereq_widget_s
{
    /*! ��ʾ���� */
    usereq_widget_type_e type;
    
    /*! ����ͼ */
    gui_bitmap_info_t *bmp_app_bg;
    gui_bitmap_info_t *bmp_bg;
    
    /*! ����ID */
    int canvas_id;
    int hdc;
    
    /*! �źŻص����� */
    void (*callback)( struct usereq_widget_s *, usereq_widget_sig_e );

    /*! EQ ���� */
    int user_eq[USEREQ_MAX];

    /*! ��ǰ����ƫ�� */
    int eq_offset;

    /*! �ڲ�˽�����ݽṹָ�룬���� usereq_widget �ؼ��ڲ�ʵ��ʹ�� */
    resgroup_resource_t *group;
	resgroup_resource_t *resgroup[USEREQ_MAX];
    
	picture_resource_t *pic_bg;
	string_resource_t *str_title;
	picture_resource_t *pic_max;
	picture_resource_t *pic_mid;
	picture_resource_t *pic_min;

    usereq_res_t eq_resgroup[USEREQ_MAX];    

}usereq_widget_t;


/*!
 *  \brief 
 *  usereq_widget �ؼ����źŻص���������
 */
typedef void (*usereq_widget_cb_t)( usereq_widget_t *, usereq_widget_sig_e );

/* usereq_widget �ؼ������ӿ� */

/******************************************************************************/
/*!                    
* \par  Description:
*     ����һ��usereq_widget�ؼ�  
* \ingroup music
* \param [in] usereq: user eq����
* \param [in] bmp_bg: ����ͼ
* \param [in] callback: ���� usereq_widget �źŵĻص����� 
* \return  �ɹ�����һ�� usereq_widget ���, ʧ�ܷ���NULL      
*******************************************************************************/
usereq_widget_t* usereq_widget_create( int *user_eq, gui_bitmap_info_t *bmp_bg, usereq_widget_cb_t callback );

/******************************************************************************/
/*!                    
* \par  Description:
*     ɾ��һ��usereq_widget�ؼ�  
* \ingroup music
* \param [in] usereq_widget: usereq_widget�ؼ���� 
* \return  �ɹ�����true, ʧ�ܷ���false      
*******************************************************************************/
bool usereq_widget_delete( usereq_widget_t *usereq_widget );

/******************************************************************************/
/*!                    
* \par  Description:
*     usereq_widget ��ͼ�ӿ�  
* \ingroup music
* \param [in] usereq_widget: usereq_widget�ؼ���� 
* \return  �ɹ�����true, ʧ�ܷ���false      
* \note �˺�������usereq_widget�ؼ�������Ԫ�أ�һ��������Ҫ�������usereq_widget�ؼ�
*******************************************************************************/
bool usereq_widget_paint( usereq_widget_t *usereq_widget );

/******************************************************************************/
/*!                    
* \par  Description:
*     usereq_widget �ؼ�GUI��Ϣ����������������Ҫ��GUI��Ϣ���ݸ��˺���
* \ingroup music
* \param [in] usereq_widget: usereq_widget�ؼ���� 
* \param [in] pmsg: GUI��Ϣ 
* \return  �ɹ�����true, ʧ�ܷ���false      
* \note �ؼ��ڲ����¼�����ʱ,�ȵ��õ�ǰ�˵���Ļص�, �ٵ��ÿؼ����źŻص�����
*******************************************************************************/
bool usereq_widget_proc_gui( usereq_widget_t *usereq_widget, gui_wm_message_t *pmsg );

/******************************************************************************/
/*!                    
* \par  Description:
*     usereq_widget �ؼ�����style��Դ  
* \ingroup music
* \param [in] usereq_widget: usereq_widget�ؼ���� 
* \return  �ɹ�����true, ʧ�ܷ���false      
* \note �ⲿ���ý����ڶ�̬�л���ʾ���ʱ
*******************************************************************************/
bool usereq_widget_load_style( usereq_widget_t *usereq_widget );

/******************************************************************************/
/*!                    
* \par  Description:
*     usereq_widget �ؼ����ò˵�����
* \ingroup music
* \param [in] usereq_widget: usereq_widget�ؼ���� 
* \param [in] usereq: user eq����
* \return  �ɹ�����true, ʧ�ܷ���false      
* \note usereq_widget ֻ������ʾ�����봦��, �˵����ݱ������ⲿ���úͱ���
*******************************************************************************/
bool usereq_widget_set_value( usereq_widget_t *usereq_widget, int *user_eq );

/******************************************************************************/
/*!                    
* \par  Description:
*     usereq_widget �ؼ���ȡ�˵�����
* \ingroup music
* \param [in] usereq_widget: usereq_widget�ؼ����     
* \param [out] usereq: user eq����
* \return  �ɹ�����true, ʧ�ܷ���false      
*******************************************************************************/
bool usereq_widget_get_value( usereq_widget_t *usereq_widget, int *user_eq );

#endif

