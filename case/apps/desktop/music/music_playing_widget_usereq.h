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
 * \defgroup   music    usereq_widget控件接口函数
 */

#ifndef __USEREQ_H__
#define __USEREQ_H__


#include "include_case.h"


#include "music_sty.h"


/*!
 * \par usereq_widget 最大EQ参数
 */
#define USEREQ_MAX  5

/*!
 * \par usereq_widget EQ参数的最大值
 */
#define USEREQ_VAL_MAX  (+6)

/*!
 * \par usereq_widget 参数的最小值
 */
#define USEREQ_VAL_MIN  (-6)

/*!
 * \par usereq_widget 显示类型 
 */
typedef enum
{
    USEREQ_TYPE_NULL,
    
}usereq_widget_type_e;

/*!
 *  \par usereq_widget 控件对外信号类型
 *  \li WIDGET_SIG_USEREQ_OK :确定
 *  \li WIDGET_SIG_USEREQ_QUIT : 退出
 *  \li WIDGET_SIG_USEREQ_CHANGED : EQ修改
 */
typedef enum
{
    WIDGET_SIG_USEREQ_OK,
    WIDGET_SIG_USEREQ_QUIT,
    WIDGET_SIG_USEREQ_CHANGED,
    
}usereq_widget_sig_e;

/*!
 *  \par usereq_widget 控件EQ资源类型
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
 *  usereq_widget 控件句柄结构定义
 */
typedef struct usereq_widget_s
{
    /*! 显示类型 */
    usereq_widget_type_e type;
    
    /*! 背景图 */
    gui_bitmap_info_t *bmp_app_bg;
    gui_bitmap_info_t *bmp_bg;
    
    /*! 画布ID */
    int canvas_id;
    int hdc;
    
    /*! 信号回调函数 */
    void (*callback)( struct usereq_widget_s *, usereq_widget_sig_e );

    /*! EQ 数据 */
    int user_eq[USEREQ_MAX];

    /*! 当前操作偏移 */
    int eq_offset;

    /*! 内部私有数据结构指针，仅供 usereq_widget 控件内部实现使用 */
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
 *  usereq_widget 控件的信号回调函数类型
 */
typedef void (*usereq_widget_cb_t)( usereq_widget_t *, usereq_widget_sig_e );

/* usereq_widget 控件基本接口 */

/******************************************************************************/
/*!                    
* \par  Description:
*     创建一个usereq_widget控件  
* \ingroup music
* \param [in] usereq: user eq数据
* \param [in] bmp_bg: 背景图
* \param [in] callback: 接受 usereq_widget 信号的回调函数 
* \return  成功返回一个 usereq_widget 句柄, 失败返回NULL      
*******************************************************************************/
usereq_widget_t* usereq_widget_create( int *user_eq, gui_bitmap_info_t *bmp_bg, usereq_widget_cb_t callback );

/******************************************************************************/
/*!                    
* \par  Description:
*     删除一个usereq_widget控件  
* \ingroup music
* \param [in] usereq_widget: usereq_widget控件句柄 
* \return  成功返回true, 失败返回false      
*******************************************************************************/
bool usereq_widget_delete( usereq_widget_t *usereq_widget );

/******************************************************************************/
/*!                    
* \par  Description:
*     usereq_widget 绘图接口  
* \ingroup music
* \param [in] usereq_widget: usereq_widget控件句柄 
* \return  成功返回true, 失败返回false      
* \note 此函数绘制usereq_widget控件的所有元素，一般用于需要整体更新usereq_widget控件
*******************************************************************************/
bool usereq_widget_paint( usereq_widget_t *usereq_widget );

/******************************************************************************/
/*!                    
* \par  Description:
*     usereq_widget 控件GUI消息处理函数，调用者需要将GUI消息传递给此函数
* \ingroup music
* \param [in] usereq_widget: usereq_widget控件句柄 
* \param [in] pmsg: GUI消息 
* \return  成功返回true, 失败返回false      
* \note 控件内部有事件发生时,先调用当前菜单项的回调, 再调用控件的信号回调函数
*******************************************************************************/
bool usereq_widget_proc_gui( usereq_widget_t *usereq_widget, gui_wm_message_t *pmsg );

/******************************************************************************/
/*!                    
* \par  Description:
*     usereq_widget 控件加载style资源  
* \ingroup music
* \param [in] usereq_widget: usereq_widget控件句柄 
* \return  成功返回true, 失败返回false      
* \note 外部调用仅用于动态切换显示风格时
*******************************************************************************/
bool usereq_widget_load_style( usereq_widget_t *usereq_widget );

/******************************************************************************/
/*!                    
* \par  Description:
*     usereq_widget 控件设置菜单数据
* \ingroup music
* \param [in] usereq_widget: usereq_widget控件句柄 
* \param [in] usereq: user eq数据
* \return  成功返回true, 失败返回false      
* \note usereq_widget 只负责显示和输入处理, 菜单数据必须由外部设置和保存
*******************************************************************************/
bool usereq_widget_set_value( usereq_widget_t *usereq_widget, int *user_eq );

/******************************************************************************/
/*!                    
* \par  Description:
*     usereq_widget 控件获取菜单数据
* \ingroup music
* \param [in] usereq_widget: usereq_widget控件句柄     
* \param [out] usereq: user eq数据
* \return  成功返回true, 失败返回false      
*******************************************************************************/
bool usereq_widget_get_value( usereq_widget_t *usereq_widget, int *user_eq );

#endif

