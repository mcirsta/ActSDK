/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : loadingbox.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
 * \file     loadingbox.h
 * \brief    loadingbox widget interface
 * \author   songzhining
 *               
 * \version 1.0
 * \date  2009/02/25
 *******************************************************************************/

/*! 
 * \defgroup   common_loadingbox    loadingbox控件接口函数
 */

#ifndef __LOADINGBOX_H__
#define __LOADINGBOX_H__


#include "include_case.h"

/*!
 * \par loadingbox 显示类型 
 *  \li LOADINGBOX_SEARCHING: 搜索中
 *  \li LOADINGBOX_WAITING: 请等待
 *  \li LOADINGBOX_MAX : 所有种类数目
 */
typedef enum
{
    LOADINGBOX_SEARCHING,
    LOADINGBOX_WAITING,
    LOADINGBOX_MAX,
    
}loadingbox_type_e;

/*!
 *  \par loadingbox 控件对外信号类型
 *  \li WIDGET_SIG_LOADINGBOX_OK : 确定
 *  \li WIDGET_SIG_LOADINGBOX_QUIT : 退出
 */
typedef enum
{
    WIDGET_SIG_LOADINGBOX_OK,
    WIDGET_SIG_LOADINGBOX_QUIT,
    
}loadingbox_sig_e;

/*!
 *  \brief 
 *  loadingbox 控件句柄结构定义
 */
typedef struct loadingbox_s
{
    /*! 列表显示类型 */
    loadingbox_type_e type;
    
    /*! 画布ID */
    int canvas_id;
    
    /*! 信号回调函数 */
    void (*callback)( struct loadingbox_s *, loadingbox_sig_e );

    /*! 内部私有数据结构指针，仅供 loadingbox 控件内部实现使用 */
    struct loadingbox_private_s * private; 

}loadingbox_t;


/*!
 *  \brief 
 *  loadingbox 控件的信号回调函数类型
 */
typedef void (*loadingbox_cb_t)( loadingbox_t *, loadingbox_sig_e );

/* loadingbox 控件基本接口 */

/******************************************************************************/
/*!                    
* \par  Description:
*     创建一个loadingbox控件  
* \ingroup common_loadingbox
* \param [in] type: 列表显示类型
* \param [in] callback: 接受 loadingbox 信号的回调函数 
* \return  成功返回一个 loadingbox 句柄, 失败返回NULL      
*******************************************************************************/
loadingbox_t* loadingbox_create( loadingbox_type_e type, loadingbox_cb_t callback );

/******************************************************************************/
/*!                    
* \par  Description:
*     删除一个loadingbox控件  
* \ingroup common_loadingbox
* \param [in] loadingbox: loadingbox控件句柄 
* \return  成功返回true, 失败返回false      
*******************************************************************************/
bool loadingbox_delete( loadingbox_t *loadingbox );

/******************************************************************************/
/*!                    
* \par  Description:
*     loadingbox 绘图接口  
* \ingroup common_loadingbox
* \param [in] loadingbox: loadingbox控件句柄 
* \return  成功返回true, 失败返回false      
* \note 此函数绘制loadingbox控件的所有元素，一般用于需要整体更新loadingbox控件
*******************************************************************************/
bool loadingbox_paint( loadingbox_t *loadingbox );

/******************************************************************************/
/*!                    
* \par  Description:
*     loadingbox 控件GUI消息处理函数，调用者需要将GUI消息传递给此函数
* \ingroup common_loadingbox
* \param [in] loadingbox: loadingbox控件句柄 
* \param [in] pmsg: GUI消息 
* \return  成功返回true, 失败返回false      
* \note 控件内部有事件发生时,先调用当前菜单项的回调, 再调用控件的信号回调函数
*******************************************************************************/
bool loadingbox_proc_gui( loadingbox_t *loadingbox, gui_wm_message_t *pmsg );

/******************************************************************************/
/*!                    
* \par  Description:
*     loadingbox 控件加载style资源  
* \ingroup common_loadingbox
* \param [in] loadingbox: loadingbox控件句柄 
* \return  成功返回true, 失败返回false      
* \note 外部调用仅用于动态切换显示风格时
*******************************************************************************/
bool loadingbox_load_style( loadingbox_t *loadingbox );

#endif

