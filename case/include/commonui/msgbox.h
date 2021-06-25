/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : msgbox.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-06-02             v1.0             build this file 
 ********************************************************************************/
/*!
 * \file     msgbox.h
 * \brief    message box widget interface
 * \author   herb hu 
 *               
 * \version 1.0
 * \date  2009/06/02
 *******************************************************************************/

/*! 
 * \defgroup   common_message_box    message box控件接口函数
 */


#ifndef __MSGBOX_H__
#define __MSGBOX_H__


#include "include_case.h"
#include "include_psp.h"

/*!
 *  \par message box控件对外信号类型
 *  \li SIG_MSGBOX_SHOW         : 显示消息框 
 *  \li SIG_MSGBOX_QUIT         : 退出消息框 
 *  \li SIG_MSGBOX_USB_LAUNCHED : 启动了USB应用 
 *  \li SIG_MSGBOX_TVIN_LAUNCHED : 启动了TVIN应用 
 *  \li WIDGET_SIG_MSGBOX_CAMERA_LAUNCHED: 启动了CAMERA应用
 *  \li WIDGET_SIG_MSGBOX_PHOTO_LAUNCHED: 启动了PHOTO应用
 *  \li WIDGET_SIG_MSGBOX_PL_GEN_LAUNCHED: 启动了pl_gen 应用
 *  \li SIG_MSGBOX_SHOW_LOW_POWER: 显示低电提示框 
 *  \li SIG_MSGBOX_QUIT_LOW_POWER: 退出低电提示框 
 *  \li SIG_MSGBOX_SHOW_LOW_POWER_UHOST: 显示UHOST支持低电提示框 
 *  \li SIG_MSGBOX_QUIT_LOW_POWER: 退出UHOST支持低电提示框 
 */
typedef enum
{
    WIDGET_SIG_MSGBOX_SHOW,
    WIDGET_SIG_MSGBOX_QUIT,
    WIDGET_SIG_MSGBOX_USB_LAUNCHED,
    WIDGET_SIG_MSGBOX_TVIN_LAUNCHED,
    WIDGET_SIG_MSGBOX_CAMERA_LAUNCHED,
    WIDGET_SIG_MSGBOX_PHOTO_LAUNCHED,
    WIDGET_SIG_MSGBOX_PL_GEN_LAUNCHED,
    WIDGET_SIG_MSGBOX_SHOW_LOW_POWER,
    WIDGET_SIG_MSGBOX_QUIT_LOW_POWER,
    WIDGET_SIG_MSGBOX_SHOW_LOW_POWER_UHOST,
    WIDGET_SIG_MSGBOX_QUIT_LOW_POWER_UHOST,
}msgbox_sig_e;

/*!
 *  \brief 
 *  message box 控件句柄结构定义
 */
typedef struct msgbox_s 
{
    /*! 画布ID */
    int canvas_id;

    /*! 信号回调函数 */
    void ( *callback)(struct msgbox_s * msgbox, msgbox_sig_e);

    /*! 内部私有数据结构指针，仅供message box 控件内部实现使用 */
    struct msgbox_private_data_s * data; 

}msgbox_t;

/*!
 *  \brief 
 *  message box 控件的信号回调函数类型
 */
typedef void (* msgbox_cb_t)(msgbox_t * msgbox, msgbox_sig_e);

/******************************************************************************/
/*!                    
 * \par  Description:
 *     message box 控件GUI消息处理函数，调用者需要将GUI消息传递给此函数
 * \ingroup common_message_box
 * \param [in] msgbox: message box控件句柄 
 * \param [in] pmsg: GUI消息 
 * \return  成功返回true, 失败返回false      
 *******************************************************************************/
bool msgbox_proc_gui(msgbox_t * msgbox, gui_wm_message_t *pmsg );

/******************************************************************************/
/*!                    
 * \par  Description:
 *     message box控件系统消息处理函数，调用者需要将系统消息传递给此函数
 * \ingroup common_message_box
 * \param [in] msgbox: message box控件句柄 
 * \param [in] pmsg: 系统消息 
 * \return  成功返回true, 失败返回false      
 *******************************************************************************/
bool msgbox_proc_sys(msgbox_t * msgbox, msg_apps_t *pmsg );


/******************************************************************************/
/*!                    
 * \par  Description:
 *     创建一个message box控件  
 * \ingroup common_message_box
 * \param [in] callback: 接受 message box 信号的回调函数 
 * \return  成功返回一个 message box 控件句柄, 失败返回NULL      
 *******************************************************************************/
msgbox_t * msgbox_create(msgbox_cb_t callback );


/******************************************************************************/
/*!                    
 * \par  Description:
 *     message box 绘图接口  
 * \ingroup common_message_box
 * \param [in] msgbox: message box控件句柄 
 * \return  成功返回true, 失败返回false      
 * \note 此函数绘制message box 控件的所有元素，一般用于需要整体更新message box控件
 *******************************************************************************/
bool msgbox_paint( msgbox_t * msgbox );

/******************************************************************************/
/*!                    
 * \par  Description:
 *     删除一个message box控件  
 * \ingroup common_message_box
 * \param [in] msgbox: message box控件句柄 
 * \return  成功返回true, 失败返回false      
 *******************************************************************************/
bool msgbox_delete( msgbox_t * msgbox );

/******************************************************************************/
/*!                    
 * \par  Description:
 *     加载style资源  
 * \ingroup common_message_box
 * \param [in] msgbox: message box控件句柄 
 * \return  成功返回true, 失败返回false      
 * \note 仅用于动态切换显示风格时
 *******************************************************************************/
bool msgbox_load_style( msgbox_t * msgbox);

#endif /* #ifndef __MSGBOX_H__ */ 

