/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       shaofeng   2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     dialog.h
* \brief    dialog widget
* \author   shaofeng
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/
/*! 
 * \defgroup   common_dialog    dialog接口函数
 */

#ifndef __DIALOG_H__
#define __DIALOG_H__


#include "include_case.h"



/*!
*\brief
	dialog控件头类型 枚举
	注意此处顺序与ui-editor的head pic region的frame的对应关系
*/
typedef enum
{
    /* query 询问型dialog, 有button */
    DIALOG_HEAD_QUERY = 0,
    /* 其余都为clip 提示框 */
    DIALOG_HEAD_SUCCESS,
    DIALOG_HEAD_FAIL,
    DIALOG_HEAD_INFO,
    DIALOG_HEAD_WARN,
    DIALOG_HEAD_LOWBAT,
    DIALOG_HEAD_CARDIN,
    DIALOG_HEAD_CARDOUT,
    DIALOG_HEAD_KEYLOCK,
    DIALOG_HEAD_MAX,
}dialog_head_e;


/*!
*\brief
	dialog控件回调信号类型 枚举
*/
typedef enum
{
    WIDGET_SIG_DIALOG_OK = 0,
    WIDGET_SIG_DIALOG_CANCAL,
}widget_sig_dialog_e;


/*!
*\brief
	dialog控件初始化 结构
*/
typedef struct
{
    /*! refer to: dialog_head_e */
    dialog_head_e head;
    /*! id of the string to show, app provide it to dialog */
    unsigned short str_id;
    /*! default select button,only used in DIALOG_HEAD_QUERY. 0 for yes ,1 for no */

    int def_sel;


}dialog_init_t;
    

/*!
*\brief
	dialog控件 结构
*/
typedef struct dialog_s
{
    /*! refer to: dialog_head_e */
	dialog_head_e head;
    /*! id of the string to show, app provide it to dialog */
	unsigned short str_id;
    
    /*! 当前选中的button. 0:yes选中, 1:no选中. 从0开始 */
    int cur_sel;
    /*! 总共button个数. 从1开始. 为0没有button,则为clip */
    int total_button;
    
    /*! 控件回调函数 */
    void (*callback)(struct dialog_s *, widget_sig_dialog_e);
    
    /*! 图层id */
    int canvas_id;
    
    /*! 控件私有结构体变量 */
	struct dialog_private_s *private;
}dialog_t;


/*!
*\brief
	dialog控件回调函数类型
*/
typedef void (*dialog_cb_t)(dialog_t *, widget_sig_dialog_e);



/******************************************************************************/
/*!                    
* \par  Description:
*      从common scene中获取控件各资源
* \param[in]    dialog_t *: 控件指针
* \retval       true: 获取style成功. false: 获取style失败
* \ingroup      common_dialog
*******************************************************************************/
bool dialog_load_style( dialog_t *dialog );

/******************************************************************************/
/*!                    
* \par  Description:
*      绘制控件
* \param[in]    dialog_t *: 控件指针
* \retval       always be true
* \ingroup      common_dialog
*******************************************************************************/
bool dialog_paint( dialog_t *dialog );

/******************************************************************************/
/*!                    
* \par  Description:
*      控件处理gui消息
* \param[in]    dialog_t *: 控件指针
* \param[in]    gui_wm_message_t *: gui消息
* \retval       always be 1
* \ingroup      common_dialog
*******************************************************************************/
int dialog_proc_gui( dialog_t *dialog, gui_wm_message_t *pmsg);

/******************************************************************************/
/*!                    
* \par  Description:
*      创建控件
* \param[in]    dialog_init_t *: 控件初始化结构体指针
* \param[in]    dialog_cb_t: 控件回调函数
* \retval       dialog_t *: 控件结构体指针. 为NULL表示创建失败
* \ingroup      common_dialog
*******************************************************************************/
dialog_t* dialog_create( dialog_init_t *init, dialog_cb_t callback );

/******************************************************************************/
/*!                    
* \par  Description:
*      删除控件
* \param[in]    dialog_t *, 控件指针
* \retval       true: 删除成功. false: 删除失败
* \ingroup      common_dialog
*******************************************************************************/
bool dialog_delete( dialog_t *dialog );


/******************************************************************************/
/*!                    
* \par  Description:
*      模态对话框. 从此函数进入, 也从此函数返回
* \param[in]    unsigned short, dialog显示字符串id
* \retval       -1: err, 其他为用户选择结果, 请参照:widget_sig_dialog_e
* \ingroup      common_dialog
*******************************************************************************/
int dialog_modal( dialog_init_t *dialog_init );

/******************************************************************************/
/*!                    
* \par  Description:
*      模态对话框, 获取资源, 用于旋转
* \param[in]    
* \retval       true: 获取style成功. false: 获取style失败
* \ingroup      common_dialog
*******************************************************************************/
bool dialog_modal_load_style( void );

/******************************************************************************/
/*!                    
* \par  Description:
*      绘制模态对话框
* \param[in]    
* \retval       always be true
* \ingroup      common_dialog
*******************************************************************************/
bool dialog_modal_paint( void );


#endif  /* __DIALOG_H__ */

