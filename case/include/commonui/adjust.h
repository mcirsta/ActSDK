/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : adjust.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Jerry Fu    2009-4-14          v1.0             build this file 
 ********************************************************************************/
/*!
 * \file     adjust.h
 * \brief    adjust widget interface
 * \author   Jerry Fu 
 *               
 * \version 1.0
 * \date  2009/04/14
 *******************************************************************************/

/*! 
 * \defgroup   common_adjust    adjust控件接口函数
 */
 
 
#ifndef      __ADJUST_H__
#define      __ADJUST_H__

#include "include_case.h"

#define    MAX_DISP_STRING    (16)

/*!
 *  \par  adjust 控件的风格
 *  \li ADJ_STYLE_TEXTDIGIT :调节数字或者文本信息的数据框
 *  \li ADJ_STYLE_SWITCH :具有ON、OFF特性的数据调节框
 *  \li ADJ_STYLE_TIME :调整时间值的数据调节框
 *  \li ADJ_STYLE_DATE :调整时日期值的数据调节框
 *  \li ADJ_STYLE_RECLEN :调整录制时间的数据调节框
 */
typedef  enum
{
    ADJ_STYLE_TEXTDIGIT,
    ADJ_STYLE_SWITCH,
    ADJ_STYLE_TIME,
    ADJ_STYLE_DATE,
    ADJ_STYLE_RECLEN,
}adjust_style_e;


/*!
 * \par  adjust控件回调函数的信号
 *  \li WIDGET_SIG_ADJUST_OK : 输入结果有效
 *  \li WIDGET_SIG_ADJUST_CANCEL :输入结果取消
 *  \li WIDGET_SIG_ADJUST_SWITCH :发生开关动作
 *  \li WIDGET_SIG_ADJUST_UP :数值向上调整
 *  \li WIDGET_SIG_ADJUST_DOWN: 数值向下调整
 */
typedef  enum
{
    WIDGET_SIG_ADJUST_OK,
    WIDGET_SIG_ADJUST_CANCEL,    
    WIDGET_SIG_ADJUST_SWITCHON,
    WIDGET_SIG_ADJUST_SWITCHOFF,
    WIDGET_SIG_ADJUST_UP,
    WIDGET_SIG_ADJUST_DOWN,
}adjust_signal_e;


/*!
 *  \par  具有开关风格的数据调节框的开关状态
 *  \li ADJUST_ON : 处于打开状态 
 *  \li ADJUST_OFF : 处于关闭状态
 */
typedef  enum
{
    ADJUST_ON,
    ADJUST_OFF,
}adj_switch_mode_e;

/*!
 *  \par  具有开关风格的数据调节框的开关状态
 *  \li TIME_MODE_24 : 24小时制式 
 *  \li TIME_FORMAT_12 : 12小时制式
 */
typedef  enum
{
    TIME_MODE_24,
    TIME_MODE_12,
}adj_time_mode_e;


typedef   struct
{
    /*! 调节框Title的字符串资源ID号 */           
    int  str_title_id;

    /*! 调节单位的字符串资源ID号 */  
    int  str_unit_id;

    /*! 表示单位的字符串 */
    char str_unit[8];

    /*! 调到最小值时要显示的字符串资源ID号 */  
    int  str_min_id;

    /*! 调到最大值时要显示的字符串资源ID号 */ 
    int  str_max_id;
    
    /*! 显示调节内容的字符串资源ID号*/
    int  str_disptext_id[MAX_DISP_STRING];

    /*! 用户传入的示字符串 */
    char *str_disp[16];
    
    /*! 调整内容的上限 */
    int max_val;
    
    /*! 调整内容的下限 */
    int min_val;
    
    /*! 当前的计数 */
    int cur_val;
    
    /*! 调整的步长 */
    int step;                  
}adjust_textdigit_init_t;


typedef   struct
{
    /*! 调节框Title的字符串资源ID号 */           
    int  str_title_id;            
    /*! 具有开关属性的数据调节框的状态 */
    adj_switch_mode_e  on_off_status;
    
}adjust_switch_init_t;


/*!
 *  \brief 
 *  设置调节时间的adjust控件的结构体
 */
typedef  struct
{
    /*! 调节时间的TITLE*/
    int str_title_id; 
    
    /*! 保存系统时间 */
    rtc_time_t sys_time;                
}adjust_time_init_t;


/*!
 *  \brief 
 *  设置调节日期的adjust控件的结构体
 */
typedef  struct
{
    /*! 调节日期的TITLE */
    int str_title_id;

    /*! 保存系统日期 */
    rtc_date_t sys_date;        
}adjust_date_init_t;

/*!
 *  \brief 
 *  描述adjust控件的结构体
 */
typedef  struct  adjust_s
{   
    /*! 画布ID */
    int canvas_id;
    
    /*! 绘图DC */
    int hdc;

    /*! 包含adjust控件各元素的资源组指针 */
    resgroup_resource_t *adjust_resgroup;
        
    /*! adjust控件的风格 */
    adjust_style_e  adjust_style;
            
    /*! 定义初始化的数据 */
    
    adjust_time_init_t  adjust_time;
    adjust_date_init_t  adjust_date;
    adjust_textdigit_init_t adjust_digit; 
    adjust_switch_init_t    adjust_switch;

    adj_time_mode_e  time_mode;
        
    /*! adjust控件的回调函数 */
    void ( *callback)(struct adjust_s * pAdjust, int signal);
    
    /*! 内部私有数据结构指针，仅供adjust 控件内部实现使用 */
    struct adjust_private_s * pAdjPrivate;    
}adjust_t;


/*!
 *  \brief 
 *  定义adjust 控件的信号回调函数类型
 */
typedef void (*adjust_cb_t)(adjust_t *pAdjust, adjust_signal_e signal);

/******************************************************************************/
/*!                    
* \par  Description:
*     adjust 控件GUI消息处理函数，调用者需要将GUI消息传递给此函数
* \ingroup common_adjust
* \param [in] data: adjust控件的句柄
* \param [in] pmsg: GUI消息
* \return  成功返回true, 失败返回false
*******************************************************************************/
bool adjust_proc_gui( adjust_t *pAdjust, gui_wm_message_t *pmsg );

/******************************************************************************/
/*!                    
* \par  Description:
*     创建一个adjust控件  
* \ingroup common_adjust
* \param [in] adjust_para: adjust控件的输入参数 
* \param [in] direction: LCD显示的方向，横屏或者竖屏
* \param [in] adjust_style: 要创建的adjust控件的风格
* \param [in] callback: 接受adjust信号的回调函数
* \return  成功返回一个 adjust 句柄, 失败返回NULL      
*******************************************************************************/
adjust_t* adjust_create(void *adjust_para,adjust_style_e adjust_style,adjust_cb_t callback );

/******************************************************************************/
/*!                    
* \par  Description:
*      从common scene中获取控件各资源
* \param[in]    adjust_t *: 控件指针
* \retval       true: 获取style成功. false: 获取style失败
* \ingroup      common_adjust
*******************************************************************************/
bool adjust_load_style( adjust_t *pAdjust );

/******************************************************************************/
/*!                    
* \par  Description:
*     删除一个adjust控件  
* \ingroup common_adjust
* \param [in] keyboard: adjust控件句柄 
* \return  成功返回true, 失败返回false      
*******************************************************************************/
bool  adjust_delete( adjust_t *data );

/******************************************************************************/
/*!                    
* \par  Description:
*     adjust控件绘图接口  
* \ingroup common_adjust
* \param [in] data: adjust控件句柄 
* \return  成功返回true, 失败返回false      
* \note 此函数绘制adjust控件的所有元素
*******************************************************************************/
bool  adjust_paint(adjust_t *data);

/******************************************************************************/
/*!                    
* \par  Description:
*     从adjust控件读取当前的调整值  
* \ingroup common_adjust
* \param [out] cur_val: 读取的调整值放在这里
* \return  成功返回true, 失败返回false      
*******************************************************************************/
bool adjust_get_current_val(int *cur_val);


/******************************************************************************/
/*!                    
* \par  Description:
*     从adjust控件读取当前的调整值  
* \ingroup common_adjust
* \param [out] cur_val: 读取的调整值放在这里
* \return  成功返回true, 失败返回false      
*******************************************************************************/
bool adjust_get_current_index(int *cur_index);


/******************************************************************************/
/*!                    
* \par  Description:
*     从adjust读取开关状态  
* \ingroup common_adjust
* \param [out] on_off_sta: 从这里返回adjust控件调整的开关状态
* \return  成功返回true, 失败返回false      
*******************************************************************************/
bool adjust_get_switch_status(adj_switch_mode_e *on_off_sta);

/******************************************************************************/
/*!                    
* \par  Description:
*     读取adjust控件当前的设定值
* \ingroup common_adjust
* \param [out] set_time: 当前调整的时间，在adjust控件上按下"OK"以后生效
* \return  成功返回true, 失败返回false      
*******************************************************************************/
bool adjust_get_setting_time(rtc_time_t *set_time);

/******************************************************************************/
/*!                    
* \par  Description:
*     读取adjust控件当前的设定值
* \ingroup common_adjust
* \param [out] set_date: 当前调整的日期，在adjust控件上按下"OK"以后生效
* \return  成功返回true, 失败返回false      
*******************************************************************************/
bool adjust_get_setting_date(rtc_date_t *set_date);


/******************************************************************************/
/*!                    
* \par  Description:
*     读取adjust控件当前的设定值
* \ingroup common_adjust
* \param [out] record_length: 当前设置的录像长度，在adjust控件上按下"OK"以后生效
* \return  成功返回true, 失败返回false      
*******************************************************************************/
bool adjust_get_recordtime(rtc_time_t *record_length);


#endif

