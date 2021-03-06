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
 * \defgroup   common_menuview    menu 菜单数据结构和接口函数
 */

#ifndef __MENU_H__
#define __MENU_H__

#include "include_case.h"

/*!
 *  \brief 
 *  menu 菜单项的信号回调函数类型
 */
typedef int (*item_cb_t)( void *, int );

/*!
 *  \brief 
 *  menu 菜单项数据结构定义
 */
typedef struct menuitem_s
{
    /*! 未选中行图标ID? */
    int unsel_icon_id;
    
    /*! 选中行图标ID */
    int sel_icon_id;
    
    /*! 菜单项静态文本ID */
    int text_id;
    
    /*! 菜单项是否有下一级标志 */
    bool has_child;
    
    /*! 菜单项的回调函数 */
    item_cb_t callback;
    
    /*! 菜单项动态文本 */
    char *text;
    
    /*! 菜单项的详细信息1 */
    char *info1;
    
    /*! 菜单项的详细信息2 */
    char *info2;
    
    /*! 动态图片数据指针 */
    gui_bitmap_info_t *bitmap;
    
}menuitem_t;

/*!
 *  \brief 
 *  menu 菜单的信号回调函数类型
 */
typedef int (*menu_cb_t)( void *, int );

/*!
 *  \brief 
 *  menu 菜单数据结构定义
 */
typedef struct menu_s
{
    /*! 当前屏幕显示起始项的索引 (取值: 0 ~ N-1) */
    int offset;
    
    /*! 当前选中菜单项的索引 (取值: 0 ~ N-1) */
    int current;

    /*! 菜单项的总数 */
    int total;
    
    /*! 菜单在全局所有项中的偏移 (取值: 0 ~ N-1) */
    int global_offset;
    
    /*! 全局所有项的总数 */
    int global_size;

    /*! 菜单的回调函数 */
    menu_cb_t callback;    

    /*! 内部私有菜单项数据结构, 仅供内部实现使用 */
    struct menuitem_inner_s *head;

}menu_t;


/* 菜单项数据 接口函数 */

/******************************************************************************/
/*!                    
* \par  Description:
*     创建一个空的菜单项  
* \ingroup common_menuview
* \param [in] 无
* \return  成功返回一个 菜单项 指针, 失败返回NULL
* \note 菜单项创建后自动清空内容
*******************************************************************************/
menuitem_t* menuitem_create( void );

/******************************************************************************/
/*!                    
* \par  Description:
*     删除一个菜单项 
* \ingroup common_menuview
* \param [in] item: 菜单项指针
* \return  成功返回true, 失败返回false  
*******************************************************************************/
bool menuitem_delete( menuitem_t *item );


/* 菜单数据 基本接口函数 */

/******************************************************************************/
/*!                    
* \par  Description:
*     创建一个空的菜单
* \ingroup common_menuview
* \param [in] 无
* \return  成功返回一个 菜单 指针, 失败返回NULL
*******************************************************************************/
menu_t* menu_create( void );

/******************************************************************************/
/*!                    
* \par  Description:
*     删除一个菜单项
* \ingroup common_menuview
* \param [in] menu: 菜单指针
* \return  成功返回true, 失败返回false      
*******************************************************************************/
bool menu_delete( menu_t *menu );

/******************************************************************************/
/*!                    
* \par  Description:
*     menu 设置当前项的索引
* \ingroup common_menuview
* \param [in] menu: 菜单指针
* \param [in] current: 当前项的索引 (取值: 0 ~ N-1)
* \return  成功返回true, 失败返回false 
*******************************************************************************/
bool menu_set_cur_index( menu_t *menu, int current );

/******************************************************************************/
/*!                    
* \par  Description:
*     menu 获取当前项的索引
* \ingroup common_menuview
* \param [in] menu: 菜单指针   
* \return  成功返回 当前项的索引 (取值: 0 ~ N-1), 失败返回 -1
*******************************************************************************/
int menu_get_cur_index( menu_t *menu );

/******************************************************************************/
/*!                    
* \par  Description:
*     menu 设置显示起始行的索引
* \ingroup common_menuview
* \param [in] menu: 菜单指针
* \param [in] offset: 显示起始行的索引 (取值: 0 ~ N-1)
* \return  成功返回true, 失败返回false      
*******************************************************************************/
bool menu_set_offset( menu_t *menu, int offset );

/******************************************************************************/
/*!                    
* \par  Description:
*     menu 获取显示起始行的索引
* \ingroup common_menuview
* \param [in] menu: 菜单指针   
* \return  成功返回 显示起始行的索引 (取值: 0 ~ N-1), 失败返回 -1
*******************************************************************************/
int menu_get_offset( menu_t *menu );

/******************************************************************************/
/*!                    
* \par  Description:
*     menu 获取菜单项的总数
* \ingroup common_menuview
* \param [in] menu: 菜单指针   
* \return  成功返回 菜单项的总数, 失败返回 -1
*******************************************************************************/
int menu_get_total( menu_t *menu );

/******************************************************************************/
/*!                    
* \par  Description:
*     menu 获取当前菜单项的数据
* \ingroup common_menuview
* \param [in] menu: 菜单指针   
* \return  成功返回 当前菜单项的数据指针, 失败返回 NULL
*******************************************************************************/
menuitem_t* menu_get_cur_item( menu_t *menu );

/******************************************************************************/
/*!                    
* \par  Description:
*     menu 获取指定菜单项的数据
* \ingroup common_menuview
* \param [in] menu: 菜单指针 
* \param [in] index: 菜单项索引 (取值: 0 ~ N-1)   
* \return  成功返回 菜单项的数据指针, 失败返回 NULL
*******************************************************************************/
menuitem_t* menu_get_item( menu_t *menu, int cur_index );

/******************************************************************************/
/*!                    
* \par  Description:
*     menu 设置指定菜单项的数据
* \ingroup common_menuview
* \param [in] menu: 菜单指针 
* \param [in] index: 菜单项索引 (取值: 0 ~ N-1)   
* \param [in] item: 菜单项数据指针
* \return  成功返回 菜单项的数据指针, 失败返回 NULL
*******************************************************************************/
bool menu_set_item( menu_t *menu, int cur_index, menuitem_t *item );

/******************************************************************************/
/*!                    
* \par  Description:
*     menu 往指定位置插入菜单项
* \ingroup common_menuview
* \param [in] menu: 菜单指针
* \param [in] index: 菜单项索引 (取值: 0 ~ N)
* \param [in] item: 菜单项数据指针
* \return  成功返回true, 失败返回false      
*******************************************************************************/
bool menu_insert_item( menu_t *menu, int cur_index, menuitem_t *item );

/******************************************************************************/
/*!                    
* \par  Description:
*     menu 往菜单起始位置插入菜单项
* \ingroup common_menuview
* \param [in] menu: 菜单指针
* \param [in] item: 菜单项数据指针
* \return  成功返回true, 失败返回false      
*******************************************************************************/
bool menu_prepend_item( menu_t *menu, menuitem_t *item );

/******************************************************************************/
/*!                    
* \par  Description:
*     menu 往菜单末尾位置追加菜单项
* \ingroup common_menuview
* \param [in] menu: 菜单指针
* \param [in] item: 菜单项数据指针
* \return  成功返回true, 失败返回false      
*******************************************************************************/
bool menu_append_item( menu_t *menu, menuitem_t *item );

/******************************************************************************/
/*!                    
* \par  Description:
*     menu 往菜单末尾位置追加N个菜单项
* \ingroup common_menuview
* \param [in] menu: 菜单指针
* \param [in] item: 菜单项数据指针
* \param [in] num: 菜单项个数
* \return  成功返回true, 失败返回false      
*******************************************************************************/
bool menu_append_items( menu_t *menu, menuitem_t *items, int num );

/******************************************************************************/
/*!                    
* \par  Description:
*     menu 删除指定位置的菜单项
* \ingroup common_menuview
* \param [in] menu: 菜单指针
* \param [in] index: 菜单项索引 (取值: 0 ~ N-1)
* \return  成功返回true, 失败返回false      
*******************************************************************************/
bool menu_remove_item( menu_t *menu, int cur_index );

/******************************************************************************/
/*!                    
* \par  Description:
*     menu 删除起始一项菜单项
* \ingroup common_menuview
* \param [in] menu: 菜单指针
* \return  成功返回true, 失败返回false      
*******************************************************************************/
bool menu_remove_first_item( menu_t *menu );

/******************************************************************************/
/*!                    
* \par  Description:
*     menu 删除最后一项菜单项
* \ingroup common_menuview
* \param [in] menu: 菜单指针
* \return  成功返回true, 失败返回false      
*******************************************************************************/
bool menu_remove_last_item( menu_t *menu );

/******************************************************************************/
/*!                    
* \par  Description:
*     menu 删除所有菜单项
* \ingroup common_menuview
* \param [in] menu: 菜单指针
* \return  成功返回true, 失败返回false      
*******************************************************************************/
bool menu_remove_all_items( menu_t *menu );

/* menu 高级数据接口函数 */

/******************************************************************************/
/*!                    
* \par  Description:
*     menu 设置菜单数据在全局菜单中的偏移量
* \ingroup common_menuview
* \param [in] menu: 菜单指针
* \param [in] offset: 偏移量 (取值: 0 ~ N-1)
* \return  成功返回true, 失败返回false      
* \note 当全局菜单内容过多只能载入部分内容时使用该接口, 如browser
*******************************************************************************/
bool menu_set_global_offset( menu_t *menu, int offset );

/******************************************************************************/
/*!                    
* \par  Description:
*     menu 获取菜单数据在全局菜单中的偏移量
* \ingroup common_menuview
* \param [in] menu: 菜单指针   
* \return  成功返回 菜单数据在全局菜单中的偏移量 (取值: 0 ~ N-1), 失败返回 -1
* \note 当全局菜单内容过多只能载入部分内容时使用该接口, 如browser
*******************************************************************************/
int menu_get_global_offset( menu_t *menu );

/******************************************************************************/
/*!                    
* \par  Description:
*     menu 设置全局菜单中的大小
* \ingroup common_menuview
* \param [in] menu: 菜单指针
* \param [in] size: 全局菜单中的大小
* \return  成功返回true, 失败返回false      
* \note 当全局菜单内容过多只能载入部分内容时使用该接口, 如browser
*******************************************************************************/
bool menu_set_global_size( menu_t *menu, int size );

/******************************************************************************/
/*!                    
* \par  Description:
*     menu 获取全局菜单中的大小
* \ingroup common_menuview
* \param [in] menu: 菜单指针   
* \return  成功返回 全局菜单中的大小, 失败返回 -1
* \note 当全局菜单内容过多只能载入部分内容时使用该接口, 如browser
*******************************************************************************/
int menu_get_global_size( menu_t *menu );

#endif

