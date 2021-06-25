/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : progress_box.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    herb hu      2009-07-27          v1.0               build this file 
 ********************************************************************************/
/*!
 * \file     progress_box.h
 * \brief    progress_box widget interface
 * \author   herb hu 
 *               
 * \version 1.0
 * \date  2009/07/27
 *******************************************************************************/

/*! 
 * \defgroup   common_progress_box    progress_box 控件接口函数
 */

#ifndef __PROGRESS_BOX_H__
#define __PROGRESS_BOX_H__

#include "include_case.h"

/*!
*\brief
	progress box 控件初始化结构
*/
typedef struct
{
    /*! 进度处理中标题字符串资源id */
	int text_str_id;
    /*! 进度处理完时标题字符串资源id */
	int complete_text_str_id;
    /*! 总进度 */
	unsigned int total_step;
    /*! 当前进度 */
	unsigned int cur_step;
}progress_box_init_t;

/*!
 *  \par progress box控件对外信号类型
 *  \li WIDGET_SIG_PROGRESS_BOX_COMPLETE: 进度完成 
 */
typedef enum
{
    WIDGET_SIG_PROGRESS_BOX_COMPLETE,
}progress_box_sig_e;

/*!
 *  \brief 
 *  progress box 控件句柄结构定义
 */
typedef struct progress_box_s
{
    /*! 画布ID */
    int canvas_id;
    
    /*! 信号回调函数 */
    void (*callback)( struct progress_box_s *, progress_box_sig_e );

    /*! 内部私有数据结构指针，仅供 progress box 控件内部实现使用 */
    struct progress_box_private_s * private; 

}progress_box_t;


/*!
 *  \brief 
 *  progress_box 控件的信号回调函数类型
 */
typedef void (*progress_box_cb_t)( progress_box_t *, progress_box_sig_e );

/* progress_box 控件基本接口 */

/******************************************************************************/
/*!                    
* \par  Description:
*     创建一个progress_box控件  
* \ingroup common_progress_box
* \param [in] init: 初始化数据 
* \param [in] callback: 接受 progress_box 信号的回调函数 
* \return  成功返回一个 progress_box 句柄, 失败返回NULL      
*******************************************************************************/
progress_box_t* progress_box_create( progress_box_init_t * init, progress_box_cb_t callback );

/******************************************************************************/
/*!                    
* \par  Description:
*     删除一个progress_box控件  
* \ingroup common_progress_box
* \param [in] progress_box: progress_box控件句柄 
* \return  成功返回true, 失败返回false      
*******************************************************************************/
bool progress_box_delete( progress_box_t *progress_box );

/******************************************************************************/
/*!                    
* \par  Description:
*     progress_box 绘图接口  
* \ingroup common_progress_box
* \param [in] progress_box: progress_box控件句柄 
* \return  成功返回true, 失败返回false      
* \note 此函数绘制progress_box控件的所有元素，一般用于需要整体更新progress_box控件
*******************************************************************************/
bool progress_box_paint( progress_box_t *progress_box );

/******************************************************************************/
/*!                    
* \par  Description:
*     progress_box 控件GUI消息处理函数，调用者需要将GUI消息传递给此函数
* \ingroup common_progress_box
* \param [in] progress_box: progress_box控件句柄 
* \param [in] pmsg: GUI消息 
* \return  成功返回true, 失败返回false      
* \note 控件内部有事件发生时,先调用当前菜单项的回调, 再调用控件的信号回调函数
*******************************************************************************/
bool progress_box_proc_gui( progress_box_t *progress_box, gui_wm_message_t *pmsg );

/******************************************************************************/
/*!                    
* \par  Description:
*     progress_box 控件加载style资源  
* \ingroup common_progress_box
* \param [in] progress_box: progress_box控件句柄 
* \return  成功返回true, 失败返回false      
* \note 外部调用仅用于动态切换显示风格时
*******************************************************************************/
bool progress_box_load_style( progress_box_t *progress_box );

/******************************************************************************/
/*!                    
* \par  Description:
*     设置progress_box 控件当前进度
* \ingroup common_progress_box
* \param [in] progress_box: progress_box控件句柄 
* \param [in] step: 当前进度 
* \return  成功返回true, 失败返回false      
*******************************************************************************/
bool progress_box_set_cur_step( progress_box_t *progress_box, unsigned int step);

/******************************************************************************/
/*!                    
* \par  Description:
*     设置progress_box 控件当前提示文本 
* \ingroup common_progress_box
* \param [in] progress_box: progress_box控件句柄 
* \param [in] text_str_id: 当前提示文本id 
* \return  成功返回true, 失败返回false      
*******************************************************************************/
bool progress_box_set_text( progress_box_t *progress_box, int text_str_id);

#endif

