/*
*******************************************************************************
*                       NOYA1100
*                (c) Copyright 2002-2008, Actions Co,Ld. 
*                        All Right Reserved 
* FileName: msg_server.h
* Description:
* History: 
*       <author>    <time>          <version >      <desc>
*       shaofeng    2009/03/11      1.0             建立该文件。
*******************************************************************************
*/ 

#ifndef __MSG_SERVER_H__
#define __MSG_SERVER_H__


#include "include_case.h"



#define GUI_KEY_FIFO  "/tmp/ucgui/.gui_key_fifo"
#define SYS_MSG_FIFO  "/tmp/ucgui/.sys_msg_fifo"



/*!
*\brief
    输入给gui的消息类型 */
typedef enum
{
    /*! 按键类型的消息 */
    INPUT_MSG_KEY,
    /*! 触摸屏类型的消息 */
    INPUT_MSG_TOUCH,
}input_msg_type_e;

/*!
*\brief
    输入给gui的消息结构体 */
typedef struct
{
    input_msg_type_e type;
    union
    {
        key_event_t kmsg;
        touch_event_t tmsg;
    }data;
}input_gui_msg_t;

/*!
 * \brief
 * 插入USB线的类型
 */
typedef enum
{
    /*! USB未连接 */
    USB_LINE_NONE,
    /*! USB A线*/
    USB_LINE_A,
    /*! USB B线*/
    USB_LINE_B,
}usb_line_type_e;

/*!
*\brief
    硬件各状态结构体, msg_server将检测的硬件状态记录在当前结构体中 */
typedef struct
{
    /*! 是否锁 */
    bool keylock;
    /*! 是否有卡插入 */
    bool card;
    /*! 是否有圆口充电器插入 */
    bool adapter;
    /*! 是否有usb插入, 包括usb口连接到pc和充电器以及U盘 */
    USB_CONNECTION_T usb;
    /*! 当前插入的USB线类型 */
    usb_line_type_e usb_linetype;
    /*! 当前的充电状态 */
    charge_status_t charge;
    /*! 直接获取电池的电压 */
    charge_voltage_t voltage;
    /*! 用于headbar显示的电压, 经过了平滑处理 */
    charge_voltage_t voltage_show;
}hw_status_t;



#endif /* __MSG_SERVER_H__ */

