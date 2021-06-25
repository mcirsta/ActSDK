/********************************************************************************
 *                              USDK(1100)
 *                             Module: app common
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : canvas.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-04-04          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     canvas.h
* \brief   canvas operation
* \author   songzhining
*               
* \version 1.0
* \date  2009/04/04
*******************************************************************************/

#ifndef __CANVAS_H__
#define __CANVAS_H__

#include "include_case.h"

#include "style_dc.h"

/*! 
 * \defgroup   applib_canvas   画布接口 
 */

/*!
 *  \brief
 *  画布模式
 *  \li OPACITY : 不透明画布(独立内存区域)，背景是黑色的(每点2字节)
 *  \li TRANSPARENT : 透明画布(独立内存区域)，背景是透明的(每点3字节)
 */ 
typedef enum 
{
    OPACITY = 0,
    TRANSPARENT = 1,
}canvas_mode_e;

/*!
 *  \brief
 *  画布信息
 */ 
typedef struct
{
    /*! 画布模式*/
    canvas_mode_e mode;
    
    /*! 画布区域*/
    sty_rect_t rect;    

    /*! 数据地址 */
	const unsigned char *pdata;
    
}canvas_info_t;

/*!
 *  \brief
 *  出错返回值
 */ 
#define ERR_NOCANVAS    (-2)    //不存在canvas
#define ERR_NOFOCUS     (-3)    //存在canvas但没有焦点canvas


/************************************************************************************/
/*!
 * \par  Description:
 *     创建画布，默认为显示状态
 * \ingroup applib_canvas
 * \param[in]     rect   画布区域
 * \param[in]     mode   画布模式
 * \return          成功返回 画布句柄, 失败返回-1
 * \par          exmaple code
 * \code
 *                rect.x = 0;
 *                rect.y = 0;
 *                rect.width = 240;
 *                rect.height = 320; 
 *                canvas_id = canvas_create( &rect, TRANSPARENT );	//创建透明画布
 * \endcode
 ************************************************************************************/
int canvas_create( sty_rect_t *rect, canvas_mode_e mode );


/************************************************************************************/
/*!
 * \par  Description:
 *     删除画布
 * \ingroup applib_canvas
 * \param[in]    canvas_id   画布句柄
 * \return       the result
 * \retval       true    设置成功
 * \retval       false    设置失败
 * \par          exmaple code
 * \code
 *                result = canvas_delete( canvas_id );	//删除画布
 * \endcode
 ************************************************************************************/
bool canvas_delete( int canvas_id );


/************************************************************************************/
/*!
 * \par  Description:
 *     显示画布
 * \ingroup applib_canvas
 * \param[in]     canvas_id   画布句柄
 * \return       the result
 * \retval       true    设置成功
 * \retval       false    设置失败
 * \par          exmaple code
 * \code
 *                result = canvas_show( canvas_id );	//显示画布
 * \endcode
 ************************************************************************************/
bool canvas_show( int canvas_id );


/************************************************************************************/
/*!
 * \par  Description:
 *     隐藏画布
 * \ingroup applib_canvas
 * \param[in]     canvas_id   画布句柄
 * \return       the result
 * \retval       true    设置成功
 * \retval       false    设置失败
 * \par          exmaple code
 * \code
 *                result = canvas_hide( canvas_id );	//隐藏画布
 * \endcode
 ************************************************************************************/
bool canvas_hide( int canvas_id );


/************************************************************************************/
/*!
 * \par  Description:
 *     改变画布区域
 * \ingroup applib_canvas
 * \param[in]     canvas_id   画布句柄
 * \param[in]     rect   画布区域
 * \return       the result
 * \retval       true    设置成功
 * \retval       false    设置失败
 ************************************************************************************/
bool canvas_set_region( int canvas_id, sty_rect_t *rect );


/************************************************************************************/
/*!
 * \par  Description:
 *     画布擦除指定区域(恢复成背景: 黑色/透明)
 * \ingroup applib_canvas
 * \param[in]     canvas_id   画布句柄
 * \param[in]     rect   擦除区域
 * \return       the result
 * \retval       true    设置成功
 * \retval       false    设置失败
 * \par          exmaple code
 * \code
 *                rect.x = 0;
 *                rect.y = 0;
 *                rect.width = 240;
 *                rect.height = 320; 
 *                result = canvas_erase( canvas_id, &rect );	//画布擦除指定区域
 * \endcode
 ************************************************************************************/
bool canvas_erase( int canvas_id, sty_rect_t *rect );


/************************************************************************************/
/*!
 * \par  Description:
 *     画布擦除全部区域(恢复成背景: 黑色/透明)
 * \ingroup applib_canvas
 * \param[in]     canvas_id   画布句柄
 * \return       the result
 * \retval       true    设置成功
 * \retval       false    设置失败
 * \par          exmaple code
 * \code
 *                result = canvas_erase( canvas_id );	//画布擦除全部区域
 * \endcode
 ************************************************************************************/
bool canvas_erase_all( int canvas_id );


/************************************************************************************/
/*!
 * \par  Description:
 *     设置画布显示的透明度(alpha值)
 * \ingroup applib_canvas
 * \param[in]     canvas_id   画布句柄
 * \param[in]     alpha   画布显示的alpha值(0~255)
 * \return       the result
 * \retval       true    设置成功
 * \retval       false    设置失败
 * \par          exmaple code
 * \code
 *                result = canvas_set_alpha( canvas_id, 10 );	//设置画布显示的透明度
 * \endcode
 ************************************************************************************/
bool canvas_set_alpha( int canvas_id, int alpha );


/************************************************************************************/
/*!
 * \par  Description:
 *     获取顶层可见带焦点画布
 * \ingroup applib_canvas
 * \return          成功返回 画布句柄, 失败返回小于0的出错返回值
 * \par          exmaple code
 * \code
 *                canvas_id = canvas_get_top();	// 获取顶层带焦点画布
 * \endcode
 ************************************************************************************/
int canvas_get_top( void );


/************************************************************************************/
/*!
 * \par  Description:
 *     设置为顶层画布
 * \ingroup applib_canvas
 * \param[in]     canvas_id   画布句柄
 * \return       the result
 * \retval       true    设置成功
 * \retval       false    设置失败
 ************************************************************************************/
bool canvas_set_top( int canvas_id );


/************************************************************************************/
/*!
 * \par  Description:
 *     获取画布信息
 * \ingroup applib_canvas
 * \param[in]     canvas_id   画布句柄(为0时表示GUI第0层)
 * \param[in]     info   画布信息指针
 * \return       the result
 * \retval       true    成功
 * \retval       false    失败
 * \par          exmaple code
 * \code
 *                result = canvas_get_info( canvas_id, &info );
 *                result = canvas_get_info( 0, &info ); //获取第0层
 * \endcode
 ************************************************************************************/
bool canvas_get_info( int canvas_id, canvas_info_t *info );

/************************************************************************************/
/*!
 * \par  Description:
 *     获取顶层带焦点画布(blending时不关心canvas是否可见)
 * \ingroup applib_canvas
 * \return          成功返回 画布句柄, 失败返回小于0的出错返回值
 * \par          exmaple code
 * \code
 *                canvas_id = canvas_get_blending_top();	// 获取顶层画布
 * \endcode
 ************************************************************************************/
int canvas_get_blending_top( void );

/************************************************************************************/
/*!
 * \par  Description:
 *     设置画布为YUV blending模式
 * \ingroup applib_canvas
 * \param[in]    is_blending   1表示YUV blending模式
 * \return       the result
 * \retval       true    设置成功
 * \retval       false    设置失败
 * \note        blending模式: 分配物理连续空间(dma使用);  设置GUI为YUV模式
 ************************************************************************************/
bool canvas_set_blending_mode( bool is_blending );

/************************************************************************************/
/*!
 * \par  Description:
 *     禁止画布焦点
 * \ingroup applib_canvas
 * \param[in]     canvas_id   画布句柄
 * \return       the result
 * \retval       true    设置成功
 * \retval       false    设置失败
 ************************************************************************************/
bool canvas_disable_focus( int canvas_id );

/************************************************************************************/
/*!
 * \par  Description:
 *     使能画布焦点
 * \ingroup applib_canvas
 * \param[in]     canvas_id   画布句柄
 * \return       the result
 * \retval       true    设置成功
 * \retval       false    设置失败
 ************************************************************************************/
bool canvas_enable_focus( int canvas_id );


/************************************************************************************/
/*!
 * \par  Description:
 *     获取当前焦点画布
 * \param[in]     None
 * \return          成功返回 画布句柄, 失败返回-1
 * \par          exmaple code
 * \code
 *                canvas_id = canvas_get_focus(pmsg);    // 获取当前焦点画布
 * \endcode
 * \note        在KEY DOWN时才切换焦点: 保证完整按键序列传递给1个对象
 ************************************************************************************/
int canvas_get_focus( gui_wm_message_t *pmsg );

/************************************************************************************/
/*!
 * \par  Description:
 *     获取当前焦点画布(blending时不关心canvas是否可见)
 * \param[in]     None
 * \return          成功返回 画布句柄, 失败返回-1
 * \par          exmaple code
 * \code
 *                canvas_id = canvas_get_blending_focus(pmsg);    // 获取当前焦点画布
 * \endcode
 * \note        在KEY DOWN时才切换焦点: 保证完整按键序列传递给1个对象
 ************************************************************************************/
int canvas_get_blending_focus( gui_wm_message_t *pmsg );

#endif

