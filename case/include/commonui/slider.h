/*******************************************************************************
*                              USDK(1100)
*                            Module: common
*                 Copyright(c) 2003-2009 Actions Semiconductor,
*                            All Rights Reserved. 
*
* History:         
*      <author>    <time>           <version >             <desc>
*    Albert_lyu 2009-4-23  1.0             build this file 
*******************************************************************************/
/*!
* \file    slider.h
* \brief    slider所用头文件 
* \author   Albert_lyu
* \version 1.0
* \date  2009-4-23
*******************************************************************************/
/*! 

 * \defgroup   common_slider    slider接口函数

 */


#ifndef _SLIDER_H_
#define _SLIDER_H_

#include "include_case.h"

typedef enum
{
    /*! slider类型1-list  */
    W_SB_TYPE_LIST    = 1, 
    /*! slider类型2-option */
    W_SB_TYPE_OPTION,
    /*! slider类型总数 */
    W_SB_TYPE_TOTAL
}slider_type_e;

/***
 slider随着页面的第一条变化而移动

 sliderbar use example:

 01,	xxxx
 02,	xxxx
 ___________
 |03,	xxxx     |<----page_index = 3;				-----
 |04,	xxxx     |									    ^
 |05,	xxxx     |
 |06,	xxxx     |
 |**,	xxxx     | <-----select item ;cur_item = 7
 |08,	xxxx     |									item_perpage=8
 |09,	xxxx     |
 |10,	xxxx     |									    v
 __________|									------
 11,	xxxx
 12,	xxxx 	<--------total = 12

 cur_item = 07,total_item =12,item_perpage=8,page_index=3;
 ***/

/*!
 * \brief
 *      --slider init use patameter    需要传入的参数
 */
typedef struct slider_init_s
{
    /*! total of list menu  列表总行数 */
    int total_item;
    /*! 每页行数 */
    int item_perpage;
    /*! 当前页首项基于总数的索引 */
    int page_index;
}slider_init_t;

typedef struct slide_s
{
    /*! 传入参数 */
    slider_init_t init;
    /*! 画布id */
    int canvas_id;
    /*! slider类型 */
    int type;
    /*!callback*/
    void (*callback)(struct slide_s *, int);
    /* 内部私有数据结构指针，仅供slider控件内部实现使用 */	
    struct slider_private_data_s * data;
    
    /*! 外部画布 */
    bool extern_canvas;
    /*! x偏移 */
    int xoffset;
    /*! y偏移 */
    int yoffset;
    
}slider_t;

//static int slider_timer_proc(void *param);
/******************************************************************************/
/*!
 * \par  Description:
 *	  slider_set_parameter-- 设置传入参数
 * \param[in]       slider_t* slider:控件句柄 
 * \param[in]       slider_init_t* parameter:需传入的参数
 * \param[out]   none
 * \return         成功返回true, 失败返回false     
 * \retval
 * \retval
 * \ingroup      common_slider
 * \par          exmaple code      none
 * \code
 *
 * \endcode
 * \note
 ************************************************ *******************************/
int slider_set_parameter(slider_t*slider, slider_init_t* parameter);
typedef void (*slider_cb_t)(slider_t *slider, int signal);
/******************************************************************************/
/*!
 * \par  Description:
 *	  slider_proc_gui-- 控件GUI消息处理函数，调用者需要将GUI消息传递给此函数
 * \param[in]       slider_t *slider:控件句柄 
 * \param[in]       gui_wm_message_t *pmsg: GUI消息 
 * \param[in]
 * \param[out]   none
 * \return       成功返回true, 失败返回false     
 * \retval
 * \retval
 * \ingroup      common_slider
 * \par          exmaple code   none
 * \code
 *
 * \endcode
 * \note
 ************************************************ *******************************/
int slider_proc_gui(slider_t *slider, gui_wm_message_t *pmsg);
/******************************************************************************/
/*!
 * \par  Description:
 *	  slider_create-- 创建slider
 * \param[in]              int type:指定要创建的类型
 * \param[in]              slider_cb_t callback:接受 slider 信号的回调函数 
 * \param[in]
 * \param[out]   none
 * \return        成功返回true, 失败返回false     
 * \retval
 * \retval
 * \ingroup      common_slider
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *******************************************************************************/
slider_t* slider_create(int type, slider_cb_t callback, int canvas_id);
/******************************************************************************/
/*!
 * \par  Description:
 *	  slider_delete-- 删除slider控件
 * \param[in]       slider_t *slider:控件句柄 
 * \param[in]
 * \param[in]
 * \param[out]   none
 * \return         成功返回true, 失败返回false     
 * \retval
 * \retval
 * \ingroup      common_slider
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 ************************************************** *****************************/
bool slider_delete(slider_t *slider);
/******************************************************************************/
/*!
 * \par  Description:
 *	  slider_paint-- 重绘slider控件
 * \param[in]     slider_t *slider:控件句柄 
 * \param[in]     none
 * \param[in]     none
 * \param[out]   none
 * \return         成功返回true, 失败返回false     
 * \retval
 * \retval
 * \ingroup      common_slider
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 *************************************************** ****************************/
bool slider_paint(slider_t *slider);
/******************************************************************************/
/*!
 * \par  Description:
 *	  slider_switch_type_test-- 测试不同风格切换
 * \param[in]            int type:指定切换类型
 * \param[in]           slider_t *slider:控件句柄 
 * \param[in]
 * \param[out]   none
 * \return         成功返回true, 失败返回false     
 * \retval
 * \retval
 * \ingroup      common_slider
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 ****************************************************** *************************/
bool slider_switch_type_test(int type, slider_t *slider);

/******************************************************************************/
/*!
 * \par  Description:
 *	  slider_load_style-- 装载slider所需要的资源
 * \param[in]     none
 * \param[in]     none
 * \param[in]     none
 * \param[out]   none
 * \return       the result
 * \retval
 * \retval
 * \ingroup      common_slider
 * \par          exmaple code  none
 * \code
 *
 * \endcode
 * \note
 **************************************************** ***************************/
int slider_load_style(slider_t *slider);


#endif   /* _SLIDER_H_ */
