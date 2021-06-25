/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : usb_select_box.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      herb hu   2009-07-14          v1.0               build this file 
 ********************************************************************************/
/*!
 * \file     usb_select_box.h
 * \brief    usb_select_box widget interface
 * \author   herb hu 
 *               
 * \version 1.0
 * \date  2009/07/14
 *******************************************************************************/

/*! 
 * \defgroup   common_usb_select_box     usb_select_box控件接口函数
 */

#ifndef __USB_SELECT_BOX_H__
#define __USB_SELECT_BOX_H__


#include "include_case.h"

/*!
 * \par usb select box 显示类型 
 * \li USB_SELECT_BOX_MTP 具有MTP选择项
 * \li USB_SELECT_DATA_TRANS 具有数据传输选择项
 */
typedef enum
{
    USB_SELECT_BOX_MTP,
    USB_SELECT_BOX_DATA_TRANS,
    USB_SELECT_BOX_ADAPTER,
}usb_select_box_type_e;

/*!
 *  \par usb select box 控件对外信号类型
 *  \li WIDGET_SIG_SELECT_MTP: 选择了MTP设备选择项 
 *  \li WIDGET_SIG_SELECT_DATA_TRANS: 选择了数据传输选择项 
 *  \li WIDGET_SIG_SELECT_CHARGE: 选择了充电选择项 
 *  \li WIDGET_SIG_SELECT_CHARGE_PLAY: 选择了后台充电选择项 
 */
typedef enum
{
    WIDGET_SIG_USB_SEL_BOX_QUIT,
    WIDGET_SIG_USB_SEL_BOX_MTP,
    WIDGET_SIG_USB_SEL_BOX_DATA_TRANS,
    WIDGET_SIG_USB_SEL_BOX_CHARGE,
    WIDGET_SIG_USB_SEL_BOX_CHARGE_PLAY,
}usb_select_box_sig_e;

/*!
 *  \brief 
 *  usb select box 控件句柄结构定义
 */
typedef struct usb_select_box_s
{
    /*! 显示类型 */
    usb_select_box_type_e type;
    
    /*! 画布ID */
    int canvas_id;
    
    /*! 信号回调函数 */
    void (*callback)( struct usb_select_box_s *, usb_select_box_sig_e );

    /*! 内部私有数据结构指针，仅供 usb select box 控件内部实现使用 */
    struct usb_select_box_private_s * private; 

}usb_select_box_t;


/*!
 *  \brief 
 *  usb select box 控件的信号回调函数类型
 */
typedef void (*usb_select_box_cb_t)( usb_select_box_t *, usb_select_box_sig_e );

/* usb select box 控件基本接口 */

/******************************************************************************/
/*!                    
* \par  Description:
*     创建一个usb select box控件  
* \ingroup common_usb_select_box
* \param [in] type: 显示类型
* \param [in] callback: 接受 usb select box 信号的回调函数 
* \return  成功返回一个 usb select box 句柄, 失败返回NULL      
*******************************************************************************/
usb_select_box_t* usb_select_box_create( usb_select_box_type_e type, usb_select_box_cb_t callback );

/******************************************************************************/
/*!                    
* \par  Description:
*     删除一个usb select box控件  
* \ingroup common_usb_select_box
* \param [in] usb select box: usb select box控件句柄 
* \return  成功返回true, 失败返回false      
*******************************************************************************/
bool usb_select_box_delete( usb_select_box_t *usb_select_box );

/******************************************************************************/
/*!                    
* \par  Description:
*     usb select box 绘图接口  
* \ingroup common_usb_select_box
* \param [in] usb select box: usb select box控件句柄 
* \return  成功返回true, 失败返回false      
* \note 此函数绘制usb select box控件的所有元素，一般用于需要整体更新usb select box控件
*******************************************************************************/
bool usb_select_box_paint( usb_select_box_t *usb_select_box );

/******************************************************************************/
/*!                    
* \par  Description:
*     usb select box 控件GUI消息处理函数，调用者需要将GUI消息传递给此函数
* \ingroup common_usb_select_box
* \param [in] usb select box: usb select box控件句柄 
* \param [in] pmsg: GUI消息 
* \return  成功返回true, 失败返回false      
* \note 控件内部有事件发生时,先调用当前菜单项的回调, 再调用控件的信号回调函数
*******************************************************************************/
bool usb_select_box_proc_gui( usb_select_box_t *usb_select_box, gui_wm_message_t *pmsg );

/******************************************************************************/
/*!                    
* \par  Description:
*     usb select box 控件加载style资源  
* \ingroup common_usb_select_box
* \param [in] usb select box: usb select box控件句柄 
* \return  成功返回true, 失败返回false      
* \note 外部调用仅用于动态切换显示风格时
*******************************************************************************/
bool usb_select_box_load_style( usb_select_box_t *usb_select_box );

#endif

