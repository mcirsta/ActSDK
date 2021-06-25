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
 * \defgroup   music    srsuser_widget控件接口函数
 */

#ifndef __SRSUSER_H__
#define __SRSUSER_H__


#include "include_case.h"


#include "music_sty.h"



/*!
 * \par srsuser_widget 最大EQ参数
 */
#define SRSUSER_MAX  7

/*!
 * \par srsuser_widget 显示类型 
 */
typedef enum
{
    SRSUSER_TYPE_NULL,
    
}srsuser_widget_type_e;

/*!
 *  \par srsuser_widget 控件对外信号类型
 *  \li WIDGET_SIG_SRSUSER_OK :确定
 *  \li WIDGET_SIG_SRSUSER_QUIT : 退出
 *  \li WIDGET_SIG_SRSUSER_CHANGED : EQ修改
 */
typedef enum
{
    WIDGET_SIG_SRSUSER_OK,
    WIDGET_SIG_SRSUSER_QUIT,
    WIDGET_SIG_SRSUSER_CHANGED,
    
}srsuser_widget_sig_e;

/*!
 *  \par srsuser_widget 控件EQ资源类型
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
 *  srsuser_widget 控件句柄结构定义
 */
typedef struct srsuser_widget_s
{
    /*! 显示类型 */
    srsuser_widget_type_e type;
    
    /*! 背景图 */
    gui_bitmap_info_t *bmp_app_bg;
    gui_bitmap_info_t *bmp_bg;
    
    /*! 画布ID */
    int canvas_id;
    int hdc;
    
    /*! 信号回调函数 */
    void (*callback)( struct srsuser_widget_s *, srsuser_widget_sig_e );

    /*! EQ 数据 */
    int srs_usereq[SRSUSER_MAX];

    /*! 当前操作偏移 */
    int eq_offset;

    /*! 内部私有数据结构指针，仅供 srsuser_widget 控件内部实现使用 */
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
 *  srsuser_widget 控件的信号回调函数类型
 */
typedef void (*srsuser_widget_cb_t)( srsuser_widget_t *, srsuser_widget_sig_e );

/* srsuser_widget 控件基本接口 */

/******************************************************************************/
/*!                    
* \par  Description:
*     创建一个srsuser_widget控件  
* \ingroup music
* \param [in] srsuser: user eq数据
* \param [in] bmp_bg: 背景图
* \param [in] callback: 接受 srsuser_widget 信号的回调函数 
* \return  成功返回一个 srsuser_widget 句柄, 失败返回NULL      
*******************************************************************************/
srsuser_widget_t* srsuser_widget_create( int *srs_usereq, gui_bitmap_info_t *bmp_bg, srsuser_widget_cb_t callback );

/******************************************************************************/
/*!                    
* \par  Description:
*     删除一个srsuser_widget控件  
* \ingroup music
* \param [in] srsuser_widget: srsuser_widget控件句柄 
* \return  成功返回true, 失败返回false      
*******************************************************************************/
bool srsuser_widget_delete( srsuser_widget_t *srsuser_widget );

/******************************************************************************/
/*!                    
* \par  Description:
*     srsuser_widget 绘图接口  
* \ingroup music
* \param [in] srsuser_widget: srsuser_widget控件句柄 
* \return  成功返回true, 失败返回false      
* \note 此函数绘制srsuser_widget控件的所有元素，一般用于需要整体更新srsuser_widget控件
*******************************************************************************/
bool srsuser_widget_paint( srsuser_widget_t *srsuser_widget );

/******************************************************************************/
/*!                    
* \par  Description:
*     srsuser_widget 控件GUI消息处理函数，调用者需要将GUI消息传递给此函数
* \ingroup music
* \param [in] srsuser_widget: srsuser_widget控件句柄 
* \param [in] pmsg: GUI消息 
* \return  成功返回true, 失败返回false      
* \note 控件内部有事件发生时,先调用当前菜单项的回调, 再调用控件的信号回调函数
*******************************************************************************/
bool srsuser_widget_proc_gui( srsuser_widget_t *srsuser_widget, gui_wm_message_t *pmsg );

/******************************************************************************/
/*!                    
* \par  Description:
*     srsuser_widget 控件加载style资源  
* \ingroup music
* \param [in] srsuser_widget: srsuser_widget控件句柄 
* \return  成功返回true, 失败返回false      
* \note 外部调用仅用于动态切换显示风格时
*******************************************************************************/
bool srsuser_widget_load_style( srsuser_widget_t *srsuser_widget );

/******************************************************************************/
/*!                    
* \par  Description:
*     srsuser_widget 控件设置菜单数据
* \ingroup music
* \param [in] srsuser_widget: srsuser_widget控件句柄 
* \param [in] srsuser: user eq数据
* \return  成功返回true, 失败返回false      
* \note srsuser_widget 只负责显示和输入处理, 菜单数据必须由外部设置和保存
*******************************************************************************/
bool srsuser_widget_set_value( srsuser_widget_t *srsuser_widget, int *srs_usereq );

/******************************************************************************/
/*!                    
* \par  Description:
*     srsuser_widget 控件获取菜单数据
* \ingroup music
* \param [in] srsuser_widget: srsuser_widget控件句柄     
* \param [out] srsuser: user eq数据
* \return  成功返回true, 失败返回false      
*******************************************************************************/
bool srsuser_widget_get_value( srsuser_widget_t *srsuser_widget, int *srs_usereq );

#endif

