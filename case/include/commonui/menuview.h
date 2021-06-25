/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : menuview.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
 * \file     menuview.h
 * \brief    menuview widget interface
 * \author   songzhining
 *               
 * \version 1.0
 * \date  2009/02/25
 *******************************************************************************/

/*! 
 * \defgroup   common_menuview    menuview�ؼ��ӿں���
 */

#ifndef __MENUVIEW_H__
#define __MENUVIEW_H__


#include "include_case.h"

#include "menu.h"

/*!
 * \par menuview ��ʾ���� 
 *  \li MENUVIEW_OPTION: �Ӳ˵�--������ʾ
 *  \li MENUVIEW_OPTION_INFO : �Ӳ˵���ϸ��Ϣ--˫����ʾ
 *  \li MENUVIEW_MENULIST_TEXT : �˵��б�--������ʾ(����ͼ��)
 *  \li MENUVIEW_MENULIST_ICON : �˵��б�--������ʾ(��ͼ��)
 *  \li MENUVIEW_FILELIST_TEXT : �ļ��б�(����ͼ��), ��ǰ��: ˫����ʾ, �ǵ�ǰ��: ������ʾ
 *  \li MENUVIEW_FILELIST_ICON : �ļ��б�(��ͼ��), ��ǰ��: ˫����ʾ, �ǵ�ǰ��: ������ʾ
 *  \li MENUVIEW_PICLIST_MUSIC : �����б�: ˫����ʾ
 *  \li MENUVIEW_PICLIST_VIDEO : ��Ƶ�б�: ˫����ʾ
 *  \li MENUVIEW_PICLIST_PICTURE : ͼƬ�б�: ˫����ʾ
 *  \li MENUVIEW_PICLIST_EBOOK : �������б�: ˫����ʾ
 *  \li MENUVIEW_PICLIST_INFO : ������ϸ��Ϣ--˫����ʾ
 *  \li MENUVIEW_MAX : ����������Ŀ
 */
typedef enum
{
    MENUVIEW_OPTION,
    MENUVIEW_OPTION_INFO,
    MENUVIEW_MENULIST_TEXT,
    MENUVIEW_MENULIST_ICON,
    MENUVIEW_FILELIST_TEXT,
    MENUVIEW_FILELIST_ICON,
    MENUVIEW_PICLIST_MUSIC,
    MENUVIEW_PICLIST_VIDEO,
    MENUVIEW_PICLIST_PICTURE,
    MENUVIEW_PICLIST_EBOOK,
    MENUVIEW_PICLIST_INFO,
    MENUVIEW_MAX,
    
}menuview_type_e;

/*!
 *  \par menuview �ؼ������ź�����
 *  \li WIDGET_SIG_MENUVIEW_UP : ����������һ��
 *  \li WIDGET_SIG_MENUVIEW_DOWN : ����������һ��
 *  \li WIDGET_SIG_MENUVIEW_OK : �˵�ȷ��
 *  \li WIDGET_SIG_MENUVIEW_QUIT : �˵��˳�
 *  \li WIDGET_SIG_MENUVIEW_EXCEED_FIRST : ������ʼ�˵���һ��
 *  \li WIDGET_SIG_MENUVIEW_EXCEED_LAST : �������˵���һ��
 *  \li WIDGET_SIG_MENUVIEW_UPDATE_ALL : ��Ҫ�ⲿ����ȫ������
 *  \li WIDGET_SIG_MENUVIEW_SHORT_MENU : MENU�̰�,�絯���Ӳ˵�
 *  \li WIDGET_SIG_MENUVIEW_LONG_MENU : MENU����,��ר��coverflow�л�
 *  \li WIDGET_SIG_MENUVIEW_DELETE : ɾ��������
 */
typedef enum
{
    WIDGET_SIG_MENUVIEW_UP,
    WIDGET_SIG_MENUVIEW_DOWN,
    WIDGET_SIG_MENUVIEW_OK,
    WIDGET_SIG_MENUVIEW_QUIT,
    WIDGET_SIG_MENUVIEW_EXCEED_FIRST,
    WIDGET_SIG_MENUVIEW_EXCEED_LAST,
    WIDGET_SIG_MENUVIEW_UPDATE_ALL,
    WIDGET_SIG_MENUVIEW_SHORT_MENU,
    WIDGET_SIG_MENUVIEW_LONG_MENU,
    WIDGET_SIG_MENUVIEW_DELETE,
    
}menuview_sig_e;

/*!
 *  \brief 
 *  menuview �ؼ�����ṹ����
 */
typedef struct menuview_s
{
    /*! �б���ʾ���� */
    menuview_type_e type;
    
    /*! ����ID */
    int canvas_id;
    
    /*! �źŻص����� */
    void (*callback)( struct menuview_s *, menuview_sig_e );

    /*! �˵�����ָ�� */
    menu_t* menu;

    /*! �ڲ�˽�����ݽṹָ�룬���� menuview �ؼ��ڲ�ʵ��ʹ�� */
    struct menuview_private_s * private; 

}menuview_t;


/*!
 *  \brief 
 *  menuview �ؼ����źŻص���������
 */
typedef void (*menuview_cb_t)( menuview_t *, menuview_sig_e );

/* menuview �ؼ������ӿ� */

/******************************************************************************/
/*!                    
* \par  Description:
*     ����һ��menuview�ؼ�  
* \ingroup common_menuview
* \param [in] type: �б���ʾ����
* \param [in] callback: ���� menuview �źŵĻص����� 
* \return  �ɹ�����һ�� menuview ���, ʧ�ܷ���NULL      
*******************************************************************************/
menuview_t* menuview_create( menuview_type_e type, menuview_cb_t callback );

/******************************************************************************/
/*!                    
* \par  Description:
*     ɾ��һ��menuview�ؼ�  
* \ingroup common_menuview
* \param [in] menuview: menuview�ؼ���� 
* \return  �ɹ�����true, ʧ�ܷ���false      
*******************************************************************************/
bool menuview_delete( menuview_t *menuview );

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview ��ͼ�ӿ�  
* \ingroup common_menuview
* \param [in] menuview: menuview�ؼ���� 
* \return  �ɹ�����true, ʧ�ܷ���false      
* \note �˺�������menuview�ؼ�������Ԫ�أ�һ��������Ҫ�������menuview�ؼ�
*******************************************************************************/
bool menuview_paint( menuview_t *menuview );

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview �ؼ�GUI��Ϣ����������������Ҫ��GUI��Ϣ���ݸ��˺���
* \ingroup common_menuview
* \param [in] menuview: menuview�ؼ���� 
* \param [in] pmsg: GUI��Ϣ 
* \return  �ɹ�����true, ʧ�ܷ���false      
* \note �ؼ��ڲ����¼�����ʱ,�ȵ��õ�ǰ�˵���Ļص�, �ٵ��ÿؼ����źŻص�����
*******************************************************************************/
bool menuview_proc_gui( menuview_t *menuview, gui_wm_message_t *pmsg );

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview �ؼ�����style��Դ  
* \ingroup common_menuview
* \param [in] menuview: menuview�ؼ���� 
* \param [in] type: �б���ʾ����
* \return  �ɹ�����true, ʧ�ܷ���false      
* \note �ⲿ���ý����ڶ�̬�л���ʾ���ʱ
*******************************************************************************/
bool menuview_load_style( menuview_t *menuview, menuview_type_e type );

/******************************************************************************/
/*!                    
* \par  Description:
*     ��ʾmenuview�ؼ�  
* \ingroup common_menuview
* \param [in] menuview: menuview�ؼ���� 
* \return  �ɹ�����true, ʧ�ܷ���false      
*******************************************************************************/
bool menuview_show( menuview_t *menuview );

/******************************************************************************/
/*!                    
* \par  Description:
*     ����menuview�ؼ�  
* \ingroup common_menuview
* \param [in] menuview: menuview�ؼ���� 
* \return  �ɹ�����true, ʧ�ܷ���false      
*******************************************************************************/
bool menuview_hide( menuview_t *menuview );

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview �ؼ����ò˵�����
* \ingroup common_menuview
* \param [in] menuview: menuview�ؼ���� 
* \param [in] menu: �˵�����ָ��
* \return  �ɹ�����true, ʧ�ܷ���false      
* \note menuview ֻ������ʾ�����봦��, �˵����ݱ������ⲿ���úͱ���
*******************************************************************************/
bool menuview_set_menu( menuview_t *menuview, menu_t* menu );

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview �ؼ���ȡ�˵�����
* \ingroup common_menuview
* \param [in] menuview: menuview�ؼ����     
* \return  �ɹ�����һ�� �˵����� ָ��, ʧ�ܷ���NULL   
*******************************************************************************/
menu_t* menuview_get_menu( menuview_t *menuview );

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview �ؼ�������ʾ����
* \ingroup common_menuview
* \param [in] menuview: menuview�ؼ���� 
* \param [in] lines: ��ʾ����
* \return  �ɹ�����true, ʧ�ܷ���false      
* \note menuview ����ʱ��ȡstyle��Դʱ�Զ���������, һ���������
*******************************************************************************/
bool menuview_set_lines( menuview_t *menuview, int lines );

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview �ؼ���ȡ��ʾ����
* \ingroup common_menuview
* \param [in] menuview: menuview�ؼ����     
* \return  �ɹ�����һ�� ��ʾ����, ʧ�ܷ��� -1  �
*******************************************************************************/
int menuview_get_lines( menuview_t *menuview );


/* menuview �ؼ��߼��ӿ� */

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview �ؼ���ȡ��̬ͼƬ�Ŀ��
* \ingroup common_menuview
* \param [in] menuview: menuview�ؼ����     
* \return  �ɹ�����һ�� ��̬ͼƬ�Ŀ��, ʧ�ܷ��� -1  
* \note menuview �ж�̬ͼƬ���ⲿ�����߳�������, ��music��ר��ͼƬ��videoԤ��ͼ,
*       ����ǰ�Ȼ�ȡͼƬ�ߴ�
*******************************************************************************/
int menuview_get_bitmap_width( menuview_t *menuview );

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview �ؼ���ȡ��̬ͼƬ�ĸ߶�
* \ingroup common_menuview
* \param [in] menuview: menuview�ؼ����     
* \return  �ɹ�����һ�� ��̬ͼƬ�ĸ߶�, ʧ�ܷ��� -1  
* \note menuview �ж�̬ͼƬ���ⲿ�����߳�������, ��music��ר��ͼƬ��videoԤ��ͼ,
*       ����ǰ�Ȼ�ȡͼƬ�ߴ�
*******************************************************************************/
int menuview_get_bitmap_height( menuview_t *menuview );

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview �ؼ�����һ�Ŷ�̬ͼƬ
* \ingroup common_menuview
* \param [in] menuview: menuview�ؼ����  
* \param [in] cur_index: menuview�ؼ��б��������
* \return  �ɹ�����һ�� ��̬ͼƬ�Ŀ��, ʧ�ܷ��� -1  
* \note menuview �ж�̬ͼƬ���ⲿ�����߳�������, ��music��ר��ͼƬ��videoԤ��ͼ,
*       ����ǰ�Ȼ�ȡͼƬ�ߴ�, �����������øýӿ������ƶ�̬ͼƬ
*******************************************************************************/
bool menuview_paint_bitmap( menuview_t *menuview, int cur_index );

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview �ؼ�����Ԥ��������
* \ingroup common_menuview
* \param [in] menuview: menuview�ؼ���� 
* \param [in] lines: Ԥ��������
* \return  �ɹ�����true, ʧ�ܷ���false      
*******************************************************************************/
bool menuview_set_prefetch_num( menuview_t *menuview, int lines );

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview �ؼ���ȡԤ��������
* \ingroup common_menuview
* \param [in] menuview: menuview�ؼ����     
* \return  �ɹ�����Ԥ��������, ʧ�ܷ��� -1  
*******************************************************************************/
int menuview_get_prefetch_num( menuview_t *menuview );

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview �ؼ���ȡԤ���ش������ʼ��
* \ingroup common_menuview
* \param [in] menuview: menuview�ؼ����     
* \return  �ɹ�����Ԥ���ش������ʼ��, ʧ�ܷ��� -1  
*******************************************************************************/
int menuview_get_prefetch_start( menuview_t *menuview );

#endif

