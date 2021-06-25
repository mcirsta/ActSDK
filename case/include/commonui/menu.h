/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : menu.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-04-21         v1.0               build this file 
 ********************************************************************************/
/*!
 * \file     menu.h
 * \brief    menu data operation
 * \author   songzhining
 *               
 * \version 1.0
 * \date  2009/04/21
 *******************************************************************************/

/*! 
 * \defgroup   common_menuview    menu �˵����ݽṹ�ͽӿں���
 */

#ifndef __MENU_H__
#define __MENU_H__

#include "include_case.h"

/*!
 *  \brief 
 *  menu �˵�����źŻص���������
 */
typedef int (*item_cb_t)( void *, int );

/*!
 *  \brief 
 *  menu �˵������ݽṹ����
 */
typedef struct menuitem_s
{
    /*! δѡ����ͼ��ID� */
    int unsel_icon_id;
    
    /*! ѡ����ͼ��ID */
    int sel_icon_id;
    
    /*! �˵��̬�ı�ID */
    int text_id;
    
    /*! �˵����Ƿ�����һ����־ */
    bool has_child;
    
    /*! �˵���Ļص����� */
    item_cb_t callback;
    
    /*! �˵��̬�ı� */
    char *text;
    
    /*! �˵������ϸ��Ϣ1 */
    char *info1;
    
    /*! �˵������ϸ��Ϣ2 */
    char *info2;
    
    /*! ��̬ͼƬ����ָ�� */
    gui_bitmap_info_t *bitmap;
    
}menuitem_t;

/*!
 *  \brief 
 *  menu �˵����źŻص���������
 */
typedef int (*menu_cb_t)( void *, int );

/*!
 *  \brief 
 *  menu �˵����ݽṹ����
 */
typedef struct menu_s
{
    /*! ��ǰ��Ļ��ʾ��ʼ������� (ȡֵ: 0 ~ N-1) */
    int offset;
    
    /*! ��ǰѡ�в˵�������� (ȡֵ: 0 ~ N-1) */
    int current;

    /*! �˵�������� */
    int total;
    
    /*! �˵���ȫ���������е�ƫ�� (ȡֵ: 0 ~ N-1) */
    int global_offset;
    
    /*! ȫ������������� */
    int global_size;

    /*! �˵��Ļص����� */
    menu_cb_t callback;    

    /*! �ڲ�˽�в˵������ݽṹ, �����ڲ�ʵ��ʹ�� */
    struct menuitem_inner_s *head;

}menu_t;


/* �˵������� �ӿں��� */

/******************************************************************************/
/*!                    
* \par  Description:
*     ����һ���յĲ˵���  
* \ingroup common_menuview
* \param [in] ��
* \return  �ɹ�����һ�� �˵��� ָ��, ʧ�ܷ���NULL
* \note �˵�������Զ��������
*******************************************************************************/
menuitem_t* menuitem_create( void );

/******************************************************************************/
/*!                    
* \par  Description:
*     ɾ��һ���˵��� 
* \ingroup common_menuview
* \param [in] item: �˵���ָ��
* \return  �ɹ�����true, ʧ�ܷ���false  
*******************************************************************************/
bool menuitem_delete( menuitem_t *item );


/* �˵����� �����ӿں��� */

/******************************************************************************/
/*!                    
* \par  Description:
*     ����һ���յĲ˵�
* \ingroup common_menuview
* \param [in] ��
* \return  �ɹ�����һ�� �˵� ָ��, ʧ�ܷ���NULL
*******************************************************************************/
menu_t* menu_create( void );

/******************************************************************************/
/*!                    
* \par  Description:
*     ɾ��һ���˵���
* \ingroup common_menuview
* \param [in] menu: �˵�ָ��
* \return  �ɹ�����true, ʧ�ܷ���false      
*******************************************************************************/
bool menu_delete( menu_t *menu );

/******************************************************************************/
/*!                    
* \par  Description:
*     menu ���õ�ǰ�������
* \ingroup common_menuview
* \param [in] menu: �˵�ָ��
* \param [in] current: ��ǰ������� (ȡֵ: 0 ~ N-1)
* \return  �ɹ�����true, ʧ�ܷ���false 
*******************************************************************************/
bool menu_set_cur_index( menu_t *menu, int current );

/******************************************************************************/
/*!                    
* \par  Description:
*     menu ��ȡ��ǰ�������
* \ingroup common_menuview
* \param [in] menu: �˵�ָ��   
* \return  �ɹ����� ��ǰ������� (ȡֵ: 0 ~ N-1), ʧ�ܷ��� -1
*******************************************************************************/
int menu_get_cur_index( menu_t *menu );

/******************************************************************************/
/*!                    
* \par  Description:
*     menu ������ʾ��ʼ�е�����
* \ingroup common_menuview
* \param [in] menu: �˵�ָ��
* \param [in] offset: ��ʾ��ʼ�е����� (ȡֵ: 0 ~ N-1)
* \return  �ɹ�����true, ʧ�ܷ���false      
*******************************************************************************/
bool menu_set_offset( menu_t *menu, int offset );

/******************************************************************************/
/*!                    
* \par  Description:
*     menu ��ȡ��ʾ��ʼ�е�����
* \ingroup common_menuview
* \param [in] menu: �˵�ָ��   
* \return  �ɹ����� ��ʾ��ʼ�е����� (ȡֵ: 0 ~ N-1), ʧ�ܷ��� -1
*******************************************************************************/
int menu_get_offset( menu_t *menu );

/******************************************************************************/
/*!                    
* \par  Description:
*     menu ��ȡ�˵��������
* \ingroup common_menuview
* \param [in] menu: �˵�ָ��   
* \return  �ɹ����� �˵��������, ʧ�ܷ��� -1
*******************************************************************************/
int menu_get_total( menu_t *menu );

/******************************************************************************/
/*!                    
* \par  Description:
*     menu ��ȡ��ǰ�˵��������
* \ingroup common_menuview
* \param [in] menu: �˵�ָ��   
* \return  �ɹ����� ��ǰ�˵��������ָ��, ʧ�ܷ��� NULL
*******************************************************************************/
menuitem_t* menu_get_cur_item( menu_t *menu );

/******************************************************************************/
/*!                    
* \par  Description:
*     menu ��ȡָ���˵��������
* \ingroup common_menuview
* \param [in] menu: �˵�ָ�� 
* \param [in] index: �˵������� (ȡֵ: 0 ~ N-1)   
* \return  �ɹ����� �˵��������ָ��, ʧ�ܷ��� NULL
*******************************************************************************/
menuitem_t* menu_get_item( menu_t *menu, int cur_index );

/******************************************************************************/
/*!                    
* \par  Description:
*     menu ����ָ���˵��������
* \ingroup common_menuview
* \param [in] menu: �˵�ָ�� 
* \param [in] index: �˵������� (ȡֵ: 0 ~ N-1)   
* \param [in] item: �˵�������ָ��
* \return  �ɹ����� �˵��������ָ��, ʧ�ܷ��� NULL
*******************************************************************************/
bool menu_set_item( menu_t *menu, int cur_index, menuitem_t *item );

/******************************************************************************/
/*!                    
* \par  Description:
*     menu ��ָ��λ�ò���˵���
* \ingroup common_menuview
* \param [in] menu: �˵�ָ��
* \param [in] index: �˵������� (ȡֵ: 0 ~ N)
* \param [in] item: �˵�������ָ��
* \return  �ɹ�����true, ʧ�ܷ���false      
*******************************************************************************/
bool menu_insert_item( menu_t *menu, int cur_index, menuitem_t *item );

/******************************************************************************/
/*!                    
* \par  Description:
*     menu ���˵���ʼλ�ò���˵���
* \ingroup common_menuview
* \param [in] menu: �˵�ָ��
* \param [in] item: �˵�������ָ��
* \return  �ɹ�����true, ʧ�ܷ���false      
*******************************************************************************/
bool menu_prepend_item( menu_t *menu, menuitem_t *item );

/******************************************************************************/
/*!                    
* \par  Description:
*     menu ���˵�ĩβλ��׷�Ӳ˵���
* \ingroup common_menuview
* \param [in] menu: �˵�ָ��
* \param [in] item: �˵�������ָ��
* \return  �ɹ�����true, ʧ�ܷ���false      
*******************************************************************************/
bool menu_append_item( menu_t *menu, menuitem_t *item );

/******************************************************************************/
/*!                    
* \par  Description:
*     menu ���˵�ĩβλ��׷��N���˵���
* \ingroup common_menuview
* \param [in] menu: �˵�ָ��
* \param [in] item: �˵�������ָ��
* \param [in] num: �˵������
* \return  �ɹ�����true, ʧ�ܷ���false      
*******************************************************************************/
bool menu_append_items( menu_t *menu, menuitem_t *items, int num );

/******************************************************************************/
/*!                    
* \par  Description:
*     menu ɾ��ָ��λ�õĲ˵���
* \ingroup common_menuview
* \param [in] menu: �˵�ָ��
* \param [in] index: �˵������� (ȡֵ: 0 ~ N-1)
* \return  �ɹ�����true, ʧ�ܷ���false      
*******************************************************************************/
bool menu_remove_item( menu_t *menu, int cur_index );

/******************************************************************************/
/*!                    
* \par  Description:
*     menu ɾ����ʼһ��˵���
* \ingroup common_menuview
* \param [in] menu: �˵�ָ��
* \return  �ɹ�����true, ʧ�ܷ���false      
*******************************************************************************/
bool menu_remove_first_item( menu_t *menu );

/******************************************************************************/
/*!                    
* \par  Description:
*     menu ɾ�����һ��˵���
* \ingroup common_menuview
* \param [in] menu: �˵�ָ��
* \return  �ɹ�����true, ʧ�ܷ���false      
*******************************************************************************/
bool menu_remove_last_item( menu_t *menu );

/******************************************************************************/
/*!                    
* \par  Description:
*     menu ɾ�����в˵���
* \ingroup common_menuview
* \param [in] menu: �˵�ָ��
* \return  �ɹ�����true, ʧ�ܷ���false      
*******************************************************************************/
bool menu_remove_all_items( menu_t *menu );

/* menu �߼����ݽӿں��� */

/******************************************************************************/
/*!                    
* \par  Description:
*     menu ���ò˵�������ȫ�ֲ˵��е�ƫ����
* \ingroup common_menuview
* \param [in] menu: �˵�ָ��
* \param [in] offset: ƫ���� (ȡֵ: 0 ~ N-1)
* \return  �ɹ�����true, ʧ�ܷ���false      
* \note ��ȫ�ֲ˵����ݹ���ֻ�����벿������ʱʹ�øýӿ�, ��browser
*******************************************************************************/
bool menu_set_global_offset( menu_t *menu, int offset );

/******************************************************************************/
/*!                    
* \par  Description:
*     menu ��ȡ�˵�������ȫ�ֲ˵��е�ƫ����
* \ingroup common_menuview
* \param [in] menu: �˵�ָ��   
* \return  �ɹ����� �˵�������ȫ�ֲ˵��е�ƫ���� (ȡֵ: 0 ~ N-1), ʧ�ܷ��� -1
* \note ��ȫ�ֲ˵����ݹ���ֻ�����벿������ʱʹ�øýӿ�, ��browser
*******************************************************************************/
int menu_get_global_offset( menu_t *menu );

/******************************************************************************/
/*!                    
* \par  Description:
*     menu ����ȫ�ֲ˵��еĴ�С
* \ingroup common_menuview
* \param [in] menu: �˵�ָ��
* \param [in] size: ȫ�ֲ˵��еĴ�С
* \return  �ɹ�����true, ʧ�ܷ���false      
* \note ��ȫ�ֲ˵����ݹ���ֻ�����벿������ʱʹ�øýӿ�, ��browser
*******************************************************************************/
bool menu_set_global_size( menu_t *menu, int size );

/******************************************************************************/
/*!                    
* \par  Description:
*     menu ��ȡȫ�ֲ˵��еĴ�С
* \ingroup common_menuview
* \param [in] menu: �˵�ָ��   
* \return  �ɹ����� ȫ�ֲ˵��еĴ�С, ʧ�ܷ��� -1
* \note ��ȫ�ֲ˵����ݹ���ֻ�����벿������ʱʹ�øýӿ�, ��browser
*******************************************************************************/
int menu_get_global_size( menu_t *menu );

#endif

