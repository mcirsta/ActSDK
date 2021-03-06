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
 * \defgroup   common_menuview    menuview控件接口函数
 */

#ifndef __MENUVIEW_H__
#define __MENUVIEW_H__


#include "include_case.h"

#include "menu.h"

/*!
 * \par menuview 显示类型 
 *  \li MENUVIEW_OPTION: 子菜单--单行显示
 *  \li MENUVIEW_OPTION_INFO : 子菜单详细信息--双行显示
 *  \li MENUVIEW_MENULIST_TEXT : 菜单列表--单行显示(不带图标)
 *  \li MENUVIEW_MENULIST_ICON : 菜单列表--单行显示(带图标)
 *  \li MENUVIEW_FILELIST_TEXT : 文件列表(不带图标), 当前行: 双行显示, 非当前行: 单行显示
 *  \li MENUVIEW_FILELIST_ICON : 文件列表(带图标), 当前行: 双行显示, 非当前行: 单行显示
 *  \li MENUVIEW_PICLIST_MUSIC : 音乐列表: 双行显示
 *  \li MENUVIEW_PICLIST_VIDEO : 视频列表: 双行显示
 *  \li MENUVIEW_PICLIST_PICTURE : 图片列表: 双行显示
 *  \li MENUVIEW_PICLIST_EBOOK : 电子书列表: 双行显示
 *  \li MENUVIEW_PICLIST_INFO : 设置详细信息--双行显示
 *  \li MENUVIEW_MAX : 所有种类数目
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
 *  \par menuview 控件对外信号类型
 *  \li WIDGET_SIG_MENUVIEW_UP : 高亮条上移一行
 *  \li WIDGET_SIG_MENUVIEW_DOWN : 高亮条下移一行
 *  \li WIDGET_SIG_MENUVIEW_OK : 菜单确定
 *  \li WIDGET_SIG_MENUVIEW_QUIT : 菜单退出
 *  \li WIDGET_SIG_MENUVIEW_EXCEED_FIRST : 超出起始菜单项一项
 *  \li WIDGET_SIG_MENUVIEW_EXCEED_LAST : 超出最后菜单项一项
 *  \li WIDGET_SIG_MENUVIEW_UPDATE_ALL : 需要外部更新全部数据
 *  \li WIDGET_SIG_MENUVIEW_SHORT_MENU : MENU短按,如弹出子菜单
 *  \li WIDGET_SIG_MENUVIEW_LONG_MENU : MENU长按,如专辑coverflow切换
 *  \li WIDGET_SIG_MENUVIEW_DELETE : 删除键按下
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
 *  menuview 控件句柄结构定义
 */
typedef struct menuview_s
{
    /*! 列表显示类型 */
    menuview_type_e type;
    
    /*! 画布ID */
    int canvas_id;
    
    /*! 信号回调函数 */
    void (*callback)( struct menuview_s *, menuview_sig_e );

    /*! 菜单数据指针 */
    menu_t* menu;

    /*! 内部私有数据结构指针，仅供 menuview 控件内部实现使用 */
    struct menuview_private_s * private; 

}menuview_t;


/*!
 *  \brief 
 *  menuview 控件的信号回调函数类型
 */
typedef void (*menuview_cb_t)( menuview_t *, menuview_sig_e );

/* menuview 控件基本接口 */

/******************************************************************************/
/*!                    
* \par  Description:
*     创建一个menuview控件  
* \ingroup common_menuview
* \param [in] type: 列表显示类型
* \param [in] callback: 接受 menuview 信号的回调函数 
* \return  成功返回一个 menuview 句柄, 失败返回NULL      
*******************************************************************************/
menuview_t* menuview_create( menuview_type_e type, menuview_cb_t callback );

/******************************************************************************/
/*!                    
* \par  Description:
*     删除一个menuview控件  
* \ingroup common_menuview
* \param [in] menuview: menuview控件句柄 
* \return  成功返回true, 失败返回false      
*******************************************************************************/
bool menuview_delete( menuview_t *menuview );

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview 绘图接口  
* \ingroup common_menuview
* \param [in] menuview: menuview控件句柄 
* \return  成功返回true, 失败返回false      
* \note 此函数绘制menuview控件的所有元素，一般用于需要整体更新menuview控件
*******************************************************************************/
bool menuview_paint( menuview_t *menuview );

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview 控件GUI消息处理函数，调用者需要将GUI消息传递给此函数
* \ingroup common_menuview
* \param [in] menuview: menuview控件句柄 
* \param [in] pmsg: GUI消息 
* \return  成功返回true, 失败返回false      
* \note 控件内部有事件发生时,先调用当前菜单项的回调, 再调用控件的信号回调函数
*******************************************************************************/
bool menuview_proc_gui( menuview_t *menuview, gui_wm_message_t *pmsg );

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview 控件加载style资源  
* \ingroup common_menuview
* \param [in] menuview: menuview控件句柄 
* \param [in] type: 列表显示类型
* \return  成功返回true, 失败返回false      
* \note 外部调用仅用于动态切换显示风格时
*******************************************************************************/
bool menuview_load_style( menuview_t *menuview, menuview_type_e type );

/******************************************************************************/
/*!                    
* \par  Description:
*     显示menuview控件  
* \ingroup common_menuview
* \param [in] menuview: menuview控件句柄 
* \return  成功返回true, 失败返回false      
*******************************************************************************/
bool menuview_show( menuview_t *menuview );

/******************************************************************************/
/*!                    
* \par  Description:
*     隐藏menuview控件  
* \ingroup common_menuview
* \param [in] menuview: menuview控件句柄 
* \return  成功返回true, 失败返回false      
*******************************************************************************/
bool menuview_hide( menuview_t *menuview );

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview 控件设置菜单数据
* \ingroup common_menuview
* \param [in] menuview: menuview控件句柄 
* \param [in] menu: 菜单数据指针
* \return  成功返回true, 失败返回false      
* \note menuview 只负责显示和输入处理, 菜单数据必须由外部设置和保存
*******************************************************************************/
bool menuview_set_menu( menuview_t *menuview, menu_t* menu );

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview 控件获取菜单数据
* \ingroup common_menuview
* \param [in] menuview: menuview控件句柄     
* \return  成功返回一个 菜单数据 指针, 失败返回NULL   
*******************************************************************************/
menu_t* menuview_get_menu( menuview_t *menuview );

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview 控件设置显示行数
* \ingroup common_menuview
* \param [in] menuview: menuview控件句柄 
* \param [in] lines: 显示行数
* \return  成功返回true, 失败返回false      
* \note menuview 创建时读取style资源时自动计算行数, 一般无需调用
*******************************************************************************/
bool menuview_set_lines( menuview_t *menuview, int lines );

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview 控件获取显示行数
* \ingroup common_menuview
* \param [in] menuview: menuview控件句柄     
* \return  成功返回一个 显示行数, 失败返回 -1  ?
*******************************************************************************/
int menuview_get_lines( menuview_t *menuview );


/* menuview 控件高级接口 */

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview 控件获取动态图片的宽度
* \ingroup common_menuview
* \param [in] menuview: menuview控件句柄     
* \return  成功返回一个 动态图片的宽度, 失败返回 -1  
* \note menuview 中动态图片由外部另起线程来解码, 如music的专辑图片或video预览图,
*       解码前先获取图片尺寸
*******************************************************************************/
int menuview_get_bitmap_width( menuview_t *menuview );

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview 控件获取动态图片的高度
* \ingroup common_menuview
* \param [in] menuview: menuview控件句柄     
* \return  成功返回一个 动态图片的高度, 失败返回 -1  
* \note menuview 中动态图片由外部另起线程来解码, 如music的专辑图片或video预览图,
*       解码前先获取图片尺寸
*******************************************************************************/
int menuview_get_bitmap_height( menuview_t *menuview );

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview 控件绘制一张动态图片
* \ingroup common_menuview
* \param [in] menuview: menuview控件句柄  
* \param [in] cur_index: menuview控件列表项的索引
* \return  成功返回一个 动态图片的宽度, 失败返回 -1  
* \note menuview 中动态图片由外部另起线程来解码, 如music的专辑图片或video预览图,
*       解码前先获取图片尺寸, 解码结束后调用该接口来绘制动态图片
*******************************************************************************/
bool menuview_paint_bitmap( menuview_t *menuview, int cur_index );

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview 控件设置预加载行数
* \ingroup common_menuview
* \param [in] menuview: menuview控件句柄 
* \param [in] lines: 预加载行数
* \return  成功返回true, 失败返回false      
*******************************************************************************/
bool menuview_set_prefetch_num( menuview_t *menuview, int lines );

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview 控件获取预加载行数
* \ingroup common_menuview
* \param [in] menuview: menuview控件句柄     
* \return  成功返回预加载行数, 失败返回 -1  
*******************************************************************************/
int menuview_get_prefetch_num( menuview_t *menuview );

/******************************************************************************/
/*!                    
* \par  Description:
*     menuview 控件获取预加载处理的起始行
* \ingroup common_menuview
* \param [in] menuview: menuview控件句柄     
* \return  成功返回预加载处理的起始行, 失败返回 -1  
*******************************************************************************/
int menuview_get_prefetch_start( menuview_t *menuview );

#endif

