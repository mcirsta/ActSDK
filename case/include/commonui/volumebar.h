/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : volumebar.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    wangsheng    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     volumebar.h
* \brief    volumebar widget
* \author   wangsheng
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/
/*! 
  * \defgroup   common_volumebar    volumebar接口函数
  */


#ifndef __VOLUMEBAR_H__
#define __VOLUMEBAR_H__


#include "include_case.h"


/*! define volumebar data */
typedef struct volumebar_s
{
    int cur_value;   //!< the current volume value
    int min_value;   //!< the min volume value
    int max_value;   //!< the max volume value

    void (*callback)( struct volumebar_s *, int );//!< callback function

    /*! resource */
    int canvas_id;             //!< canvas id
    int bg_pic;			//!< background picture
    int type;			//!< display type	
    int timer_id;               //!< timer id

    struct volumebar_private_data_s *private_data;//!< volumebar private data
}volumebar_t;


/*! define widget signals */
typedef enum
{
    /*! 音量改变*/
    WIDGET_SIG_VOLUMEBAR_CHANGED,
    /*! volumebar退出*/
    WIDGET_SIG_VOLUMEBAR_QUIT,
}volumebar_sig_e;

/******************************************************************************/
/*!                    
  * \par  Description:
  *      volumebar回调函数
  * \param[in]    data -- volumebar的数据结构
                          signal -- volumebar信号
  * \retval      无。
  * \par example code
  * \code
  * \endcode
*******************************************************************************/
typedef void (*volumebar_cb_t)(volumebar_t *data, volumebar_sig_e signal);


/******************************************************************************/
/*!                    
* \par  Description:
*      volumebar入口函数
* \param[in]    data -- volumebar的数据结构
                        pmsg -- gui消息
* \retval      true 成功，false 失败
* \ingroup      common_volumebar
* \par example code
* \code
*
* \endcode
* \note
*******************************************************************************/
int volumebar_proc_gui( volumebar_t *data, gui_wm_message_t *pmsg);

/*! volumebar external interface */

/******************************************************************************/
/*!                    
* \par  Description:
*      volumebar图层创建函数
* \param[in]    type -- volumebar显示类型
                       callback -- volumebar回调函数
* \retval      存储volumebar所有数据
* \ingroup      common_volumebar
* \par example code
* \code

* \endcode
* \note
*******************************************************************************/
volumebar_t* volumebar_create( int type, volumebar_cb_t callback );

/******************************************************************************/
/*!                    
* \par  Description:
*      volumebar图层删除函数
* \param[in]    data -- volumebar的数据结构
* \retval      true 成功，false 失败
* \ingroup      common_volumebar
* \par example code
* \code

* \endcode
* \note
*******************************************************************************/
bool volumebar_delete( volumebar_t *data );

/******************************************************************************/
/*!                    
* \par  Description:
*      volumebar画图函数
* \param[in]    data -- volumebar的数据结构
* \retval      always be true 
* \ingroup      common_volumebar
* \par example code
* \code

* \endcode
* \note
*******************************************************************************/
bool volumebar_paint( volumebar_t *data );

/******************************************************************************/
/*!                    
* \par  Description:
*      volumebar资源载入函数
* \param[in]    data -- volumebar的数据结构
* \retval      true 成功，false 失败
* \ingroup      common_volumebar
* \par example code
* \code

* \endcode
* \note
*******************************************************************************/
bool volumebar_load_style( volumebar_t *data );

#endif
