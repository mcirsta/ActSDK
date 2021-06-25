/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : softkeyboard.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-4-14             v1.0             build this file 
 ********************************************************************************/
/*!
 * \file     softkeyboard.h
 * \brief    softkeyboard widget interface
 * \author   herb hu 
 *               
 * \version 1.0
 * \date  2009/04/14
 *******************************************************************************/

/*! 
 * \defgroup   common_softkeyboard    softkeyboard控件接口函数
 */


#ifndef __SOFT_KEYBOARD_H__
#define __SOFT_KEYBOARD_H__


#include "include_case.h"

/*!
 * \par  默认输入法类型 
 *  \li IME_TYPE_EN : 英文输入法 
 *  \li IME_TYPE_PINYIN : 汉语拼音输入法 
 *  \li IME_TYPE_NUM : 数字输入法 
 */
typedef enum
{
    IME_TYPE_EN,
    IME_TYPE_PINYIN,
    IME_TYPE_NUM
}IME_type_e;

/*!
 *  \par softkeyboard 控件对外信号类型
 *  \li SIG_SKB_OK : 完成输入 
 *  \li SIG_SKB_CANCEL : 取消输入 
 */
typedef enum
{
    WIDGET_SIG_SKB_OK,
    WIDGET_SIG_SKB_CANCEL,
}softkeyboard_sig_e;

/*!
 *  \brief 
 *  softkeyboard 控件句柄结构定义
 */
typedef struct softkeyboard_s 
{
    /*! 画布ID */
    int canvas_id;

    /*! 绘图DC */
    int hdc;

    /*! 信号回调函数 */
    void ( *callback)(struct softkeyboard_s * keyboard, softkeyboard_sig_e);

    /*! 内部私有数据结构指针，仅供softkeyboard 控件内部实现使用 */
    struct softkey_private_data_s * data; 

}softkeyboard_t;

/*!
 *  \brief 
 *  softkeyboard 控件的信号回调函数类型
 */
typedef void (* softkeyboard_cb_t)(softkeyboard_t * keyboard, softkeyboard_sig_e);

/******************************************************************************/
/*!                    
 * \par  Description:
 *     softkeyboard 控件GUI消息处理函数，调用者需要将GUI消息传递给此函数
 * \ingroup common_softkeyboard
 * \param [in] keyboard: softkeyboard控件句柄 
 * \param [in] pmsg: GUI消息 
 * \return  成功返回true, 失败返回false      
 *******************************************************************************/

bool softkeyboard_proc_gui(softkeyboard_t * keyboard, gui_wm_message_t *pmsg );


/******************************************************************************/
/*!                    
 * \par  Description:
 *     创建一个softkeyboard控件  
 * \ingroup common_softkeyboard
 * \param [in] ime: 默认输入法类型 
 * \param [in] callback: 接受 softkeyboard 信号的回调函数 
 * \return  成功返回一个 softkeyboard 句柄, 失败返回NULL      
 *******************************************************************************/

softkeyboard_t * softkeyboard_create( IME_type_e ime, softkeyboard_cb_t callback );


/******************************************************************************/
/*!                    
 * \par  Description:
 *     softkeyboard 绘图接口  
 * \ingroup common_softkeyboard
 * \param [in] keyboard: softkeyboard控件句柄 
 * \return  成功返回true, 失败返回false      
 * \note 此函数绘制softkeyboard控件的所有元素，一般用于需要整体更新softkeyboard控件
 *******************************************************************************/

bool softkeyboard_paint( softkeyboard_t * keyboard );

/******************************************************************************/
/*!                    
 * \par  Description:
 *     删除一个softkeyboard控件  
 * \ingroup common_softkeyboard
 * \param [in] keyboard: softkeyboard控件句柄 
 * \return  成功返回true, 失败返回false      
 *******************************************************************************/

bool softkeyboard_delete( softkeyboard_t * keyboard );


/******************************************************************************/
/*!                    
 * \par  Description:
 *     获取用户输入文本信息(unicode 字符串) 
 * \ingroup common_softkeyboard
 * \param [in] keyboard: softkeyboard控件句柄 
 * \param [in] utf8_text: 存放结果字符串(utf8编码字符)的缓冲区
 * \param [in] len:  缓冲区大小 
 * \return 成功返回true, 失败返回false;       
 * \note 1.调用者在收到 SIG_SKB_DONE 信号后，通过此接口获取用户输入文本\n
 *       2.调用者需事先分配好足够大小的缓冲区(包括字符串结束标志)，否则调用失败\n
 *       3.该调用成功时会自动在字符串末尾补零，以作为字符串的结束标志\n
 *******************************************************************************/

bool softkeyboard_get_text( softkeyboard_t * keyboard , char * utf8_text, unsigned int len);


/******************************************************************************/
/*!                    
 * \par  Description:
 *     预设用户输入文本   
 * \ingroup common_softkeyboard
 * \param [in] keyboard: softkeyboard控件句柄 
 * \param [in] utf8_text: utf8编码字符串 
 * \return  成功返回true, 失败返回false      
 *******************************************************************************/

bool softkeyboard_set_text( softkeyboard_t * keyboard , const char * utf8_text);


/******************************************************************************/
/*!                    
 * \par  Description:
 *     清空用户输入文本  
 * \ingroup common_softkeyboard
 * \param [in] keyboard: softkeyboard控件句柄 
 * \return  成功返回true, 失败返回false      
 *******************************************************************************/

bool softkeyboard_clear( softkeyboard_t * keyboard);


/******************************************************************************/
/*!                    
 * \par  Description:
 *     加载style资源  
 * \ingroup common_softkeyboard
 * \param [in] keyboard: softkeyboard控件句柄 
 * \return  成功返回true, 失败返回false      
 * \note 仅用于动态切换显示风格时
 *******************************************************************************/

bool softkeyboard_load_style( softkeyboard_t * keyboard);

#endif /* #ifndef __SOFT_KEYBOARD_H__ */ 

