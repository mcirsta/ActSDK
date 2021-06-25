/********************************************************************************
 *                              USDK(1100)
 *                             Module: app common
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : canvas.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-04-04          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     canvas.h
* \brief   canvas operation
* \author   songzhining
*               
* \version 1.0
* \date  2009/04/04
*******************************************************************************/

#ifndef __CANVAS_H__
#define __CANVAS_H__

#include "include_case.h"

#include "style_dc.h"

/*! 
 * \defgroup   applib_canvas   �����ӿ� 
 */

/*!
 *  \brief
 *  ����ģʽ
 *  \li OPACITY : ��͸������(�����ڴ�����)�������Ǻ�ɫ��(ÿ��2�ֽ�)
 *  \li TRANSPARENT : ͸������(�����ڴ�����)��������͸����(ÿ��3�ֽ�)
 */ 
typedef enum 
{
    OPACITY = 0,
    TRANSPARENT = 1,
}canvas_mode_e;

/*!
 *  \brief
 *  ������Ϣ
 */ 
typedef struct
{
    /*! ����ģʽ*/
    canvas_mode_e mode;
    
    /*! ��������*/
    sty_rect_t rect;    

    /*! ���ݵ�ַ */
	const unsigned char *pdata;
    
}canvas_info_t;

/*!
 *  \brief
 *  ������ֵ
 */ 
#define ERR_NOCANVAS    (-2)    //������canvas
#define ERR_NOFOCUS     (-3)    //����canvas��û�н���canvas


/************************************************************************************/
/*!
 * \par  Description:
 *     ����������Ĭ��Ϊ��ʾ״̬
 * \ingroup applib_canvas
 * \param[in]     rect   ��������
 * \param[in]     mode   ����ģʽ
 * \return          �ɹ����� �������, ʧ�ܷ���-1
 * \par          exmaple code
 * \code
 *                rect.x = 0;
 *                rect.y = 0;
 *                rect.width = 240;
 *                rect.height = 320; 
 *                canvas_id = canvas_create( &rect, TRANSPARENT );	//����͸������
 * \endcode
 ************************************************************************************/
int canvas_create( sty_rect_t *rect, canvas_mode_e mode );


/************************************************************************************/
/*!
 * \par  Description:
 *     ɾ������
 * \ingroup applib_canvas
 * \param[in]    canvas_id   �������
 * \return       the result
 * \retval       true    ���óɹ�
 * \retval       false    ����ʧ��
 * \par          exmaple code
 * \code
 *                result = canvas_delete( canvas_id );	//ɾ������
 * \endcode
 ************************************************************************************/
bool canvas_delete( int canvas_id );


/************************************************************************************/
/*!
 * \par  Description:
 *     ��ʾ����
 * \ingroup applib_canvas
 * \param[in]     canvas_id   �������
 * \return       the result
 * \retval       true    ���óɹ�
 * \retval       false    ����ʧ��
 * \par          exmaple code
 * \code
 *                result = canvas_show( canvas_id );	//��ʾ����
 * \endcode
 ************************************************************************************/
bool canvas_show( int canvas_id );


/************************************************************************************/
/*!
 * \par  Description:
 *     ���ػ���
 * \ingroup applib_canvas
 * \param[in]     canvas_id   �������
 * \return       the result
 * \retval       true    ���óɹ�
 * \retval       false    ����ʧ��
 * \par          exmaple code
 * \code
 *                result = canvas_hide( canvas_id );	//���ػ���
 * \endcode
 ************************************************************************************/
bool canvas_hide( int canvas_id );


/************************************************************************************/
/*!
 * \par  Description:
 *     �ı仭������
 * \ingroup applib_canvas
 * \param[in]     canvas_id   �������
 * \param[in]     rect   ��������
 * \return       the result
 * \retval       true    ���óɹ�
 * \retval       false    ����ʧ��
 ************************************************************************************/
bool canvas_set_region( int canvas_id, sty_rect_t *rect );


/************************************************************************************/
/*!
 * \par  Description:
 *     ��������ָ������(�ָ��ɱ���: ��ɫ/͸��)
 * \ingroup applib_canvas
 * \param[in]     canvas_id   �������
 * \param[in]     rect   ��������
 * \return       the result
 * \retval       true    ���óɹ�
 * \retval       false    ����ʧ��
 * \par          exmaple code
 * \code
 *                rect.x = 0;
 *                rect.y = 0;
 *                rect.width = 240;
 *                rect.height = 320; 
 *                result = canvas_erase( canvas_id, &rect );	//��������ָ������
 * \endcode
 ************************************************************************************/
bool canvas_erase( int canvas_id, sty_rect_t *rect );


/************************************************************************************/
/*!
 * \par  Description:
 *     ��������ȫ������(�ָ��ɱ���: ��ɫ/͸��)
 * \ingroup applib_canvas
 * \param[in]     canvas_id   �������
 * \return       the result
 * \retval       true    ���óɹ�
 * \retval       false    ����ʧ��
 * \par          exmaple code
 * \code
 *                result = canvas_erase( canvas_id );	//��������ȫ������
 * \endcode
 ************************************************************************************/
bool canvas_erase_all( int canvas_id );


/************************************************************************************/
/*!
 * \par  Description:
 *     ���û�����ʾ��͸����(alphaֵ)
 * \ingroup applib_canvas
 * \param[in]     canvas_id   �������
 * \param[in]     alpha   ������ʾ��alphaֵ(0~255)
 * \return       the result
 * \retval       true    ���óɹ�
 * \retval       false    ����ʧ��
 * \par          exmaple code
 * \code
 *                result = canvas_set_alpha( canvas_id, 10 );	//���û�����ʾ��͸����
 * \endcode
 ************************************************************************************/
bool canvas_set_alpha( int canvas_id, int alpha );


/************************************************************************************/
/*!
 * \par  Description:
 *     ��ȡ����ɼ������㻭��
 * \ingroup applib_canvas
 * \return          �ɹ����� �������, ʧ�ܷ���С��0�ĳ�����ֵ
 * \par          exmaple code
 * \code
 *                canvas_id = canvas_get_top();	// ��ȡ��������㻭��
 * \endcode
 ************************************************************************************/
int canvas_get_top( void );


/************************************************************************************/
/*!
 * \par  Description:
 *     ����Ϊ���㻭��
 * \ingroup applib_canvas
 * \param[in]     canvas_id   �������
 * \return       the result
 * \retval       true    ���óɹ�
 * \retval       false    ����ʧ��
 ************************************************************************************/
bool canvas_set_top( int canvas_id );


/************************************************************************************/
/*!
 * \par  Description:
 *     ��ȡ������Ϣ
 * \ingroup applib_canvas
 * \param[in]     canvas_id   �������(Ϊ0ʱ��ʾGUI��0��)
 * \param[in]     info   ������Ϣָ��
 * \return       the result
 * \retval       true    �ɹ�
 * \retval       false    ʧ��
 * \par          exmaple code
 * \code
 *                result = canvas_get_info( canvas_id, &info );
 *                result = canvas_get_info( 0, &info ); //��ȡ��0��
 * \endcode
 ************************************************************************************/
bool canvas_get_info( int canvas_id, canvas_info_t *info );

/************************************************************************************/
/*!
 * \par  Description:
 *     ��ȡ��������㻭��(blendingʱ������canvas�Ƿ�ɼ�)
 * \ingroup applib_canvas
 * \return          �ɹ����� �������, ʧ�ܷ���С��0�ĳ�����ֵ
 * \par          exmaple code
 * \code
 *                canvas_id = canvas_get_blending_top();	// ��ȡ���㻭��
 * \endcode
 ************************************************************************************/
int canvas_get_blending_top( void );

/************************************************************************************/
/*!
 * \par  Description:
 *     ���û���ΪYUV blendingģʽ
 * \ingroup applib_canvas
 * \param[in]    is_blending   1��ʾYUV blendingģʽ
 * \return       the result
 * \retval       true    ���óɹ�
 * \retval       false    ����ʧ��
 * \note        blendingģʽ: �������������ռ�(dmaʹ��);  ����GUIΪYUVģʽ
 ************************************************************************************/
bool canvas_set_blending_mode( bool is_blending );

/************************************************************************************/
/*!
 * \par  Description:
 *     ��ֹ��������
 * \ingroup applib_canvas
 * \param[in]     canvas_id   �������
 * \return       the result
 * \retval       true    ���óɹ�
 * \retval       false    ����ʧ��
 ************************************************************************************/
bool canvas_disable_focus( int canvas_id );

/************************************************************************************/
/*!
 * \par  Description:
 *     ʹ�ܻ�������
 * \ingroup applib_canvas
 * \param[in]     canvas_id   �������
 * \return       the result
 * \retval       true    ���óɹ�
 * \retval       false    ����ʧ��
 ************************************************************************************/
bool canvas_enable_focus( int canvas_id );


/************************************************************************************/
/*!
 * \par  Description:
 *     ��ȡ��ǰ���㻭��
 * \param[in]     None
 * \return          �ɹ����� �������, ʧ�ܷ���-1
 * \par          exmaple code
 * \code
 *                canvas_id = canvas_get_focus(pmsg);    // ��ȡ��ǰ���㻭��
 * \endcode
 * \note        ��KEY DOWNʱ���л�����: ��֤�����������д��ݸ�1������
 ************************************************************************************/
int canvas_get_focus( gui_wm_message_t *pmsg );

/************************************************************************************/
/*!
 * \par  Description:
 *     ��ȡ��ǰ���㻭��(blendingʱ������canvas�Ƿ�ɼ�)
 * \param[in]     None
 * \return          �ɹ����� �������, ʧ�ܷ���-1
 * \par          exmaple code
 * \code
 *                canvas_id = canvas_get_blending_focus(pmsg);    // ��ȡ��ǰ���㻭��
 * \endcode
 * \note        ��KEY DOWNʱ���л�����: ��֤�����������д��ݸ�1������
 ************************************************************************************/
int canvas_get_blending_focus( gui_wm_message_t *pmsg );

#endif

