/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       shaofeng   2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     dialog.h
* \brief    dialog widget
* \author   shaofeng
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/
/*! 
 * \defgroup   common_dialog    dialog�ӿں���
 */

#ifndef __DIALOG_H__
#define __DIALOG_H__


#include "include_case.h"



/*!
*\brief
	dialog�ؼ�ͷ���� ö��
	ע��˴�˳����ui-editor��head pic region��frame�Ķ�Ӧ��ϵ
*/
typedef enum
{
    /* query ѯ����dialog, ��button */
    DIALOG_HEAD_QUERY = 0,
    /* ���඼Ϊclip ��ʾ�� */
    DIALOG_HEAD_SUCCESS,
    DIALOG_HEAD_FAIL,
    DIALOG_HEAD_INFO,
    DIALOG_HEAD_WARN,
    DIALOG_HEAD_LOWBAT,
    DIALOG_HEAD_CARDIN,
    DIALOG_HEAD_CARDOUT,
    DIALOG_HEAD_KEYLOCK,
    DIALOG_HEAD_MAX,
}dialog_head_e;


/*!
*\brief
	dialog�ؼ��ص��ź����� ö��
*/
typedef enum
{
    WIDGET_SIG_DIALOG_OK = 0,
    WIDGET_SIG_DIALOG_CANCAL,
}widget_sig_dialog_e;


/*!
*\brief
	dialog�ؼ���ʼ�� �ṹ
*/
typedef struct
{
    /*! refer to: dialog_head_e */
    dialog_head_e head;
    /*! id of the string to show, app provide it to dialog */
    unsigned short str_id;
    /*! default select button,only used in DIALOG_HEAD_QUERY. 0 for yes ,1 for no */

    int def_sel;


}dialog_init_t;
    

/*!
*\brief
	dialog�ؼ� �ṹ
*/
typedef struct dialog_s
{
    /*! refer to: dialog_head_e */
	dialog_head_e head;
    /*! id of the string to show, app provide it to dialog */
	unsigned short str_id;
    
    /*! ��ǰѡ�е�button. 0:yesѡ��, 1:noѡ��. ��0��ʼ */
    int cur_sel;
    /*! �ܹ�button����. ��1��ʼ. Ϊ0û��button,��Ϊclip */
    int total_button;
    
    /*! �ؼ��ص����� */
    void (*callback)(struct dialog_s *, widget_sig_dialog_e);
    
    /*! ͼ��id */
    int canvas_id;
    
    /*! �ؼ�˽�нṹ����� */
	struct dialog_private_s *private;
}dialog_t;


/*!
*\brief
	dialog�ؼ��ص���������
*/
typedef void (*dialog_cb_t)(dialog_t *, widget_sig_dialog_e);



/******************************************************************************/
/*!                    
* \par  Description:
*      ��common scene�л�ȡ�ؼ�����Դ
* \param[in]    dialog_t *: �ؼ�ָ��
* \retval       true: ��ȡstyle�ɹ�. false: ��ȡstyleʧ��
* \ingroup      common_dialog
*******************************************************************************/
bool dialog_load_style( dialog_t *dialog );

/******************************************************************************/
/*!                    
* \par  Description:
*      ���ƿؼ�
* \param[in]    dialog_t *: �ؼ�ָ��
* \retval       always be true
* \ingroup      common_dialog
*******************************************************************************/
bool dialog_paint( dialog_t *dialog );

/******************************************************************************/
/*!                    
* \par  Description:
*      �ؼ�����gui��Ϣ
* \param[in]    dialog_t *: �ؼ�ָ��
* \param[in]    gui_wm_message_t *: gui��Ϣ
* \retval       always be 1
* \ingroup      common_dialog
*******************************************************************************/
int dialog_proc_gui( dialog_t *dialog, gui_wm_message_t *pmsg);

/******************************************************************************/
/*!                    
* \par  Description:
*      �����ؼ�
* \param[in]    dialog_init_t *: �ؼ���ʼ���ṹ��ָ��
* \param[in]    dialog_cb_t: �ؼ��ص�����
* \retval       dialog_t *: �ؼ��ṹ��ָ��. ΪNULL��ʾ����ʧ��
* \ingroup      common_dialog
*******************************************************************************/
dialog_t* dialog_create( dialog_init_t *init, dialog_cb_t callback );

/******************************************************************************/
/*!                    
* \par  Description:
*      ɾ���ؼ�
* \param[in]    dialog_t *, �ؼ�ָ��
* \retval       true: ɾ���ɹ�. false: ɾ��ʧ��
* \ingroup      common_dialog
*******************************************************************************/
bool dialog_delete( dialog_t *dialog );


/******************************************************************************/
/*!                    
* \par  Description:
*      ģ̬�Ի���. �Ӵ˺�������, Ҳ�Ӵ˺�������
* \param[in]    unsigned short, dialog��ʾ�ַ���id
* \retval       -1: err, ����Ϊ�û�ѡ����, �����:widget_sig_dialog_e
* \ingroup      common_dialog
*******************************************************************************/
int dialog_modal( dialog_init_t *dialog_init );

/******************************************************************************/
/*!                    
* \par  Description:
*      ģ̬�Ի���, ��ȡ��Դ, ������ת
* \param[in]    
* \retval       true: ��ȡstyle�ɹ�. false: ��ȡstyleʧ��
* \ingroup      common_dialog
*******************************************************************************/
bool dialog_modal_load_style( void );

/******************************************************************************/
/*!                    
* \par  Description:
*      ����ģ̬�Ի���
* \param[in]    
* \retval       always be true
* \ingroup      common_dialog
*******************************************************************************/
bool dialog_modal_paint( void );


#endif  /* __DIALOG_H__ */

