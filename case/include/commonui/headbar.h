/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-4-17          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     headbar.h
* \brief    
* \author   shaofeng
*               
* \version 1.0
* \date  2009/04/17
*******************************************************************************/
/*! 
 * \defgroup   common_headbar    headbar�ӿں���
 */

#ifndef __HEADBAR_H__
#define __HEADBAR_H__


#include "include_case.h"

/*!
*\brief
	headbar�ؼ����� ö��
*/
typedef enum
{
    HEADBAR_TYPE_NORMAL = 0,
    HEADBAR_TYPE_SIMPLE,
    HEADBAR_TYPE_YUV,
    HEADBAR_TYPE_MAX,
}headbar_type_e;


/*!
*\brief
	headbar�ؼ��ص��ź����� ö��
*/
typedef enum
{
    WIDGET_SIG_HEADBAR_NULL = 0,
}widget_sig_headbar_e;


/*!
*\brief
	headbar��ʾ��ʱ���ʽ
*/
typedef enum
{
    /*! ����ʾ12���Ƶ�hh:mm am/pm */
    TIME_FORMAT_12 = 12,
    /*! ����ʾ24���Ƶ�hh:mm */
    TIME_FORMAT_24 = 24,
    /*! ��ʾ12���Ƶ�yyyy/mm/dd hh:mm am/pm */
    TIME_FORMAT_112 = 112,
    /*! ��ʾ24���Ƶ�yyyy/mm/dd hh:mm */
    TIME_FORMAT_124 = 124,
    /*! ��ʾ12���Ƶ�yyyy-mm-dd hh:mm am/pm */
    TIME_FORMAT_212 = 212,
    /*! ��ʾ24���Ƶ�yyyy-mm-dd hh:mm */
    TIME_FORMAT_224 = 224,
}time_format_e;


/*!
*\brief
	headbar�ؼ���ʼ�� �ṹ
*/
typedef struct
{
    /*! refer to: headbar_type_e */
	headbar_type_e type;
    /*! head picture id */
	unsigned short pic_id_head;
    /*! id of string title to show, app provide it to headbar */
	unsigned short str_id_title;
}headbar_init_t;


/*!
*\brief
	headbar�ؼ� �ṹ
*/
typedef struct headbar_s
{
    /*! refer to: headbar_type_e */
	headbar_type_e type;
    /*! head picture id */
	unsigned short pic_id_head;
    /*! id of string title to show, app provide it to headbar, ��str_buffer_title���� */
	unsigned short str_id_title;
    /*! pointer of string title, ��str_id_title���� */
	char *str_buffer_title;
    
    /*! �ؼ��ص����� */
    void (*callback)(struct headbar_s *, widget_sig_headbar_e);
    
    /*! ͼ��id */
    int canvas_id;
    
    /*! �ؼ�˽�нṹ����� */
    struct headbar_private_s *private;
}headbar_t;


/*!
*\brief
	headbar�ؼ��ص���������
*/
typedef void (*headbar_cb_t)(headbar_t *, widget_sig_headbar_e);



/******************************************************************************/
/*!                    
* \par  Description:
*      ��common scene�л�ȡ�ؼ�����Դ
* \param[in]    headbar_t *: �ؼ�ָ��
* \retval       true: ��ȡstyle�ɹ�. false: ��ȡstyleʧ��
* \ingroup      common_headbar
*******************************************************************************/
bool headbar_load_style( headbar_t *headbar );

/******************************************************************************/
/*!                    
* \par  Description:
*      ����title��Ҫ��ʾ���ַ���
* \param[in]    headbar_t *: �ؼ�ָ��
* \param[in]    unsigned short: ��Ҫ��ʾ�ַ�����id
* \retval       always be true
* \ingroup      common_headbar
*******************************************************************************/
bool headbar_set_title_id( headbar_t *headbar, unsigned short id );

/******************************************************************************/
/*!                    
* \par  Description:
*      ����title��Ҫ��ʾ���ַ���
* \param[in]    headbar_t *: �ؼ�ָ��
* \param[in]    char *: ��Ҫ��ʾ�ַ�����ָ��
* \retval       always be true
* \ingroup      common_headbar
*******************************************************************************/
bool headbar_set_title_buffer( headbar_t *headbar, char *buffer );

/******************************************************************************/
/*!                    
* \par  Description:
*      ����time��ʾ��ʱ����ʽ
* \param[in]    headbar_t *: �ؼ�ָ��
* \param[in]    time_format_e: ��Ҫ��ʾʱ����ʽ
* \retval       
* \ingroup      common_headbar
*******************************************************************************/
bool headbar_set_time_format( headbar_t *headbar, time_format_e time_format );

/******************************************************************************/
/*!                    
* \par  Description:
*      ���ƿؼ�
* \param[in]    headbar_t *: �ؼ�ָ��
* \retval       always be true
* \ingroup      common_headbar
*******************************************************************************/
bool headbar_paint( headbar_t *headbar );

/******************************************************************************/
/*!                    
* \par  Description:
*      ȫ�����»��ƿؼ�
* \param[in]    headbar_t *: �ؼ�ָ��
* \retval       always be true
* \ingroup      common_headbar
*******************************************************************************/
bool headbar_repaint( headbar_t *headbar );

/******************************************************************************/
/*!                    
* \par  Description:
*      �ؼ�����gui��Ϣ
* \param[in]    headbar_t *: �ؼ�ָ��
* \param[in]    gui_wm_message_t *: gui��Ϣ
* \retval       always be 1
* \ingroup      common_headbar
*******************************************************************************/
int headbar_proc_gui( headbar_t *data, gui_wm_message_t *pmsg);

/******************************************************************************/
/*!                    
* \par  Description:
*      �����ؼ�
* \param[in]    headbarg_init_t *: �ؼ���ʼ���ṹ��ָ��
* \param[in]    headbar_cb_t: �ؼ��ص�����
* \retval       headbar_t *: �ؼ��ṹ��ָ��. ΪNULL��ʾ����ʧ��
* \ingroup      common_headbar
*******************************************************************************/
headbar_t* headbar_create( headbar_init_t *init, headbar_cb_t callback );

/******************************************************************************/
/*!                    
* \par  Description:
*      ɾ���ؼ�
* \param[in]    headbar_t *, �ؼ�ָ��
* \retval       true: ɾ���ɹ�. false: ɾ��ʧ��
* \ingroup      common_headbar
*******************************************************************************/
bool headbar_delete( headbar_t *headbar );


#endif  /* __HEADBAR_H__ */

