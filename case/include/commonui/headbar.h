/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-4-17          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     headbar.h
* \brief    
* \author   shaofeng
*               
* \version 1.0
* \date  2009/04/17
*******************************************************************************/
/*! 
 * \defgroup   common_headbar    headbar接口函数
 */

#ifndef __HEADBAR_H__
#define __HEADBAR_H__


#include "include_case.h"

/*!
*\brief
	headbar控件类型 枚举
*/
typedef enum
{
    HEADBAR_TYPE_NORMAL = 0,
    HEADBAR_TYPE_SIMPLE,
    HEADBAR_TYPE_YUV,
    HEADBAR_TYPE_MAX,
}headbar_type_e;


/*!
*\brief
	headbar控件回调信号类型 枚举
*/
typedef enum
{
    WIDGET_SIG_HEADBAR_NULL = 0,
}widget_sig_headbar_e;


/*!
*\brief
	headbar显示的时间格式
*/
typedef enum
{
    /*! 仅显示12进制的hh:mm am/pm */
    TIME_FORMAT_12 = 12,
    /*! 仅显示24进制的hh:mm */
    TIME_FORMAT_24 = 24,
    /*! 显示12进制的yyyy/mm/dd hh:mm am/pm */
    TIME_FORMAT_112 = 112,
    /*! 显示24进制的yyyy/mm/dd hh:mm */
    TIME_FORMAT_124 = 124,
    /*! 显示12进制的yyyy-mm-dd hh:mm am/pm */
    TIME_FORMAT_212 = 212,
    /*! 显示24进制的yyyy-mm-dd hh:mm */
    TIME_FORMAT_224 = 224,
}time_format_e;


/*!
*\brief
	headbar控件初始化 结构
*/
typedef struct
{
    /*! refer to: headbar_type_e */
	headbar_type_e type;
    /*! head picture id */
	unsigned short pic_id_head;
    /*! id of string title to show, app provide it to headbar */
	unsigned short str_id_title;
}headbar_init_t;


/*!
*\brief
	headbar控件 结构
*/
typedef struct headbar_s
{
    /*! refer to: headbar_type_e */
	headbar_type_e type;
    /*! head picture id */
	unsigned short pic_id_head;
    /*! id of string title to show, app provide it to headbar, 与str_buffer_title互斥 */
	unsigned short str_id_title;
    /*! pointer of string title, 与str_id_title互斥 */
	char *str_buffer_title;
    
    /*! 控件回调函数 */
    void (*callback)(struct headbar_s *, widget_sig_headbar_e);
    
    /*! 图层id */
    int canvas_id;
    
    /*! 控件私有结构体变量 */
    struct headbar_private_s *private;
}headbar_t;


/*!
*\brief
	headbar控件回调函数类型
*/
typedef void (*headbar_cb_t)(headbar_t *, widget_sig_headbar_e);



/******************************************************************************/
/*!                    
* \par  Description:
*      从common scene中获取控件各资源
* \param[in]    headbar_t *: 控件指针
* \retval       true: 获取style成功. false: 获取style失败
* \ingroup      common_headbar
*******************************************************************************/
bool headbar_load_style( headbar_t *headbar );

/******************************************************************************/
/*!                    
* \par  Description:
*      设置title需要显示的字符串
* \param[in]    headbar_t *: 控件指针
* \param[in]    unsigned short: 需要显示字符串的id
* \retval       always be true
* \ingroup      common_headbar
*******************************************************************************/
bool headbar_set_title_id( headbar_t *headbar, unsigned short id );

/******************************************************************************/
/*!                    
* \par  Description:
*      设置title需要显示的字符串
* \param[in]    headbar_t *: 控件指针
* \param[in]    char *: 需要显示字符串的指针
* \retval       always be true
* \ingroup      common_headbar
*******************************************************************************/
bool headbar_set_title_buffer( headbar_t *headbar, char *buffer );

/******************************************************************************/
/*!                    
* \par  Description:
*      设置time显示的时间制式
* \param[in]    headbar_t *: 控件指针
* \param[in]    time_format_e: 需要显示时间制式
* \retval       
* \ingroup      common_headbar
*******************************************************************************/
bool headbar_set_time_format( headbar_t *headbar, time_format_e time_format );

/******************************************************************************/
/*!                    
* \par  Description:
*      绘制控件
* \param[in]    headbar_t *: 控件指针
* \retval       always be true
* \ingroup      common_headbar
*******************************************************************************/
bool headbar_paint( headbar_t *headbar );

/******************************************************************************/
/*!                    
* \par  Description:
*      全部重新绘制控件
* \param[in]    headbar_t *: 控件指针
* \retval       always be true
* \ingroup      common_headbar
*******************************************************************************/
bool headbar_repaint( headbar_t *headbar );

/******************************************************************************/
/*!                    
* \par  Description:
*      控件处理gui消息
* \param[in]    headbar_t *: 控件指针
* \param[in]    gui_wm_message_t *: gui消息
* \retval       always be 1
* \ingroup      common_headbar
*******************************************************************************/
int headbar_proc_gui( headbar_t *data, gui_wm_message_t *pmsg);

/******************************************************************************/
/*!                    
* \par  Description:
*      创建控件
* \param[in]    headbarg_init_t *: 控件初始化结构体指针
* \param[in]    headbar_cb_t: 控件回调函数
* \retval       headbar_t *: 控件结构体指针. 为NULL表示创建失败
* \ingroup      common_headbar
*******************************************************************************/
headbar_t* headbar_create( headbar_init_t *init, headbar_cb_t callback );

/******************************************************************************/
/*!                    
* \par  Description:
*      删除控件
* \param[in]    headbar_t *, 控件指针
* \retval       true: 删除成功. false: 删除失败
* \ingroup      common_headbar
*******************************************************************************/
bool headbar_delete( headbar_t *headbar );


#endif  /* __HEADBAR_H__ */

