/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_playing_widget_srsuser.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
 * \file     music_playing_widget_srsuser.h
 * \brief    srsuser widget interface
 * \author   songzhining
 *               
 * \version 1.0
 * \date  2009/02/25
 *******************************************************************************/

/*! 
 * \defgroup   music    srsuser_widget�ؼ��ӿں���
 */

#ifndef __SRSUSER_H__
#define __SRSUSER_H__


#include "include_case.h"


#include "music_sty.h"



/*!
 * \par srsuser_widget ���EQ����
 */
#define SRSUSER_MAX  7

/*!
 * \par srsuser_widget ��ʾ���� 
 */
typedef enum
{
    SRSUSER_TYPE_NULL,
    
}srsuser_widget_type_e;

/*!
 *  \par srsuser_widget �ؼ������ź�����
 *  \li WIDGET_SIG_SRSUSER_OK :ȷ��
 *  \li WIDGET_SIG_SRSUSER_QUIT : �˳�
 *  \li WIDGET_SIG_SRSUSER_CHANGED : EQ�޸�
 */
typedef enum
{
    WIDGET_SIG_SRSUSER_OK,
    WIDGET_SIG_SRSUSER_QUIT,
    WIDGET_SIG_SRSUSER_CHANGED,
    
}srsuser_widget_sig_e;

/*!
 *  \par srsuser_widget �ؼ�EQ��Դ����
 */
typedef struct
{
    picture_resource_t *pic_bar_bg;
    picture_resource_t *pic_btn_unsel;
    picture_resource_t *pic_btn_sel;
    picture_resource_t *pic_slider_sel;
    picture_resource_t *pic_slider_unsel;
	string_resource_t *str_btn_unsel;
	string_resource_t *str_btn_sel;
    picture_resource_t *pic_val_unsel;
    picture_resource_t *pic_val_sel;
	string_resource_t *str_val_unsel;
	string_resource_t *str_val_sel;

}srsuser_res_t;

/*!
 *  \brief 
 *  srsuser_widget �ؼ�����ṹ����
 */
typedef struct srsuser_widget_s
{
    /*! ��ʾ���� */
    srsuser_widget_type_e type;
    
    /*! ����ͼ */
    gui_bitmap_info_t *bmp_app_bg;
    gui_bitmap_info_t *bmp_bg;
    
    /*! ����ID */
    int canvas_id;
    int hdc;
    
    /*! �źŻص����� */
    void (*callback)( struct srsuser_widget_s *, srsuser_widget_sig_e );

    /*! EQ ���� */
    int srs_usereq[SRSUSER_MAX];

    /*! ��ǰ����ƫ�� */
    int eq_offset;

    /*! �ڲ�˽�����ݽṹָ�룬���� srsuser_widget �ؼ��ڲ�ʵ��ʹ�� */
    resgroup_resource_t *group;
	resgroup_resource_t *resgroup[SRSUSER_MAX];
    
	picture_resource_t *pic_bg;
	string_resource_t *str_title;
	picture_resource_t *pic_val_0;
	picture_resource_t *pic_val_10;
	picture_resource_t *pic_val_60;
	picture_resource_t *pic_val_100;
	picture_resource_t *pic_val_150;

    srsuser_res_t eq_resgroup[SRSUSER_MAX];    

}srsuser_widget_t;


/*!
 *  \brief 
 *  srsuser_widget �ؼ����źŻص���������
 */
typedef void (*srsuser_widget_cb_t)( srsuser_widget_t *, srsuser_widget_sig_e );

/* srsuser_widget �ؼ������ӿ� */

/******************************************************************************/
/*!                    
* \par  Description:
*     ����һ��srsuser_widget�ؼ�  
* \ingroup music
* \param [in] srsuser: user eq����
* \param [in] bmp_bg: ����ͼ
* \param [in] callback: ���� srsuser_widget �źŵĻص����� 
* \return  �ɹ�����һ�� srsuser_widget ���, ʧ�ܷ���NULL      
*******************************************************************************/
srsuser_widget_t* srsuser_widget_create( int *srs_usereq, gui_bitmap_info_t *bmp_bg, srsuser_widget_cb_t callback );

/******************************************************************************/
/*!                    
* \par  Description:
*     ɾ��һ��srsuser_widget�ؼ�  
* \ingroup music
* \param [in] srsuser_widget: srsuser_widget�ؼ���� 
* \return  �ɹ�����true, ʧ�ܷ���false      
*******************************************************************************/
bool srsuser_widget_delete( srsuser_widget_t *srsuser_widget );

/******************************************************************************/
/*!                    
* \par  Description:
*     srsuser_widget ��ͼ�ӿ�  
* \ingroup music
* \param [in] srsuser_widget: srsuser_widget�ؼ���� 
* \return  �ɹ�����true, ʧ�ܷ���false      
* \note �˺�������srsuser_widget�ؼ�������Ԫ�أ�һ��������Ҫ�������srsuser_widget�ؼ�
*******************************************************************************/
bool srsuser_widget_paint( srsuser_widget_t *srsuser_widget );

/******************************************************************************/
/*!                    
* \par  Description:
*     srsuser_widget �ؼ�GUI��Ϣ����������������Ҫ��GUI��Ϣ���ݸ��˺���
* \ingroup music
* \param [in] srsuser_widget: srsuser_widget�ؼ���� 
* \param [in] pmsg: GUI��Ϣ 
* \return  �ɹ�����true, ʧ�ܷ���false      
* \note �ؼ��ڲ����¼�����ʱ,�ȵ��õ�ǰ�˵���Ļص�, �ٵ��ÿؼ����źŻص�����
*******************************************************************************/
bool srsuser_widget_proc_gui( srsuser_widget_t *srsuser_widget, gui_wm_message_t *pmsg );

/******************************************************************************/
/*!                    
* \par  Description:
*     srsuser_widget �ؼ�����style��Դ  
* \ingroup music
* \param [in] srsuser_widget: srsuser_widget�ؼ���� 
* \return  �ɹ�����true, ʧ�ܷ���false      
* \note �ⲿ���ý����ڶ�̬�л���ʾ���ʱ
*******************************************************************************/
bool srsuser_widget_load_style( srsuser_widget_t *srsuser_widget );

/******************************************************************************/
/*!                    
* \par  Description:
*     srsuser_widget �ؼ����ò˵�����
* \ingroup music
* \param [in] srsuser_widget: srsuser_widget�ؼ���� 
* \param [in] srsuser: user eq����
* \return  �ɹ�����true, ʧ�ܷ���false      
* \note srsuser_widget ֻ������ʾ�����봦��, �˵����ݱ������ⲿ���úͱ���
*******************************************************************************/
bool srsuser_widget_set_value( srsuser_widget_t *srsuser_widget, int *srs_usereq );

/******************************************************************************/
/*!                    
* \par  Description:
*     srsuser_widget �ؼ���ȡ�˵�����
* \ingroup music
* \param [in] srsuser_widget: srsuser_widget�ؼ����     
* \param [out] srsuser: user eq����
* \return  �ɹ�����true, ʧ�ܷ���false      
*******************************************************************************/
bool srsuser_widget_get_value( srsuser_widget_t *srsuser_widget, int *srs_usereq );

#endif

