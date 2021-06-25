/********************************************************************************
 *                              USDK1100
 *                            Module: USB MONITOR
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>               <version >             <desc>
 *       GaoJing   2008-12-31 15:37:14     1.0              build this file
 ********************************************************************************/
/*!
 * \file     usb_monitor.h
 * \brief    this file defines and declares the related data structure and APIs
 *           for the applications
 * \author   Gao Jing
 * \version 1.0
 * \date  2008-12-31 15:36:57
 *******************************************************************************/
#ifndef __USDK1100_USB_MONITOR_H__
#define __USDK1100_USB_MONITOR_H__

/*!
 *    ioctl CMD 命令字定义 \n
 *
 */

/*!
 *    供app调用以获知当前是否有usb线连入小机\n
 */
#define UMONITOR_IOCTL_GET_USB_CON           (UMONITOR_BASE)
/*!
 *    供app设置USB为插线或拔线状态，并由monitor消息通知GUI\n
 */
#define UMONITOR_IOCTL_SET_USB_CON           (UMONITOR_BASE+1)
/*!
 *    使能、禁止usb monitor进行插拔线检测\n
 */
#define UMONITOR_IOCTL_ENABLE_DETECT         (UMONITOR_BASE+2)
/*!
 *    供app调用以获知当前monitor插拔线检测功能是否处于使能状态\n
 */
#define UMONITOR_IOCTL_IS_WORKING             (UMONITOR_BASE+3)
/*!
 *    供app调用以获知当前连入小机的是充电器还是PC\n
 */
#define UMONITOR_IOCTL_IS_CHARGER             (UMONITOR_BASE+4)
/*!
 *    上层告知Monitor AP所在的模式，Udisk还是充电模式或其他\n
 */
#define UMONITOR_IOCTL_SET_USB_MODE             (UMONITOR_BASE+5)

/*!
 *  \brief
 *  区分充电器/usb host（PC）接口的结果类型定义。
 */
typedef enum
{
    /*! 当前连入的系usb host（PC）\n */
    USB_HOST = 0x00,
    /*! 当前连入的系usb charger\n */
    USB_CHARGER = 0x01,

} USB_CONNECTOR_T;

/*!
 *  \brief
 *  插拔线检测使能/禁止命令值定义，对应UMONITOR_IOCTL_ENABLE_DETECT命令（ioctl）。
 */
typedef enum
{
    /*! 停止插拔线检测，直至被app再次使能 */
    DISABLE_DETECT = 0x00,
    /*! 使能插拔线检测，直至被app再次禁止*/
    ENABLE_DETECT = 0x01,

} USB_DETECTION_T;

/*!
 *  \brief
 *  USB状态设置值定义，对应UMONITOR_IOCTL_SET_USB_CON命令（ioctl）。
 */
typedef enum
{
    /*! 拔除usb线*/
    PLUG_OUT = 0x00,
    /*! 插入usb线*/
    PLUG_IN = 0x01,
} USB_CONNECTION_T;

/*!
 *  \brief
 *  USB插拔线时VBUS检测门限电压可供选配的电压值。
 */
typedef enum
{
    /*! VBUS检测门限电压设为4.7v\n */
    VBUS_47 = 0x1,
    /*! VBUS检测门限电压设为4.4v\n */
    VBUS_44 = 0x2,
    /*! VBUS检测门限电压设为4.0v\n */
    VBUS_40 = 0x3,
    /*! VBUS检测门限电压设为3.5v\n */
    VBUS_35 = 0x4,
} VBUS_VOLTAGE_T;

/*!
 *  \brief
 *  usb monitor驱动加载时所需的参数结构体
 *
 *  此结构体参数目的是使monitor进行插拔线检测时的定时器运行间隔，依据的唤醒源，检测电压档位均可配置。
 *	方便满足客户可能的个案定制需求和二次开发
 *
 */
typedef struct
{
    /*! 利用vubs电压进行检测时的电压门限档位设定(1:4.7v, 2:4.4v, 3:4.0v, 4:3.5v) */
    VBUS_VOLTAGE_T vbus_threshold;
    /*! 插拔线主定时器usb_timer运行间隔(毫秒，建议500ms) */
    unsigned int usbtimer :16;
    /*! check_again定时器运行间隔(毫秒，建议100ms)*/
    unsigned int chktimer :16;
    /*! 标志位，表明是否采用vbus作为检测的唤醒源*/
    unsigned int use_vbus :1;
    /*! 标志位，表明是否采用b_see_vld作为检测的唤醒源*/
    unsigned int use_bvld :1;
    /*! 标志位，表明是否采用ID pin作为检测的唤醒源 */
    unsigned int use_id :1;
    /*! 标志位，表明是否采用D+作为检测的唤醒源*/
    unsigned int use_dp :1;
} umonitor_arg_t;

/*!
 *  \brief
 *  区分USB 模式的类型定义。
 */
typedef enum
{
    /*! 当前作为charger\n */
    USB_MODE_CHARGE = 0,
    /*! B线插入后，当前可能作为Udisk或是MTP\n */
    USB_MODE_UDISK_MTP,
    /*! 当前作为Udisk\n */
    USB_MODE_SENSOR,
    USB_MODE_UDISK,
    /*! 当前作为MTP\n */
    USB_MODE_MTP,
    /*! 当前为默认模式\n */
    USB_MODE_DEFAULT,
} USB_MODE_T;

/*!
 *  \brief
 *  区分USB 端口模式的类型定义。
 */
typedef enum
{
    /*! Default模式\n */
    USB_PORT_MODE_DEFAULT = 0,
    /*! B线插入的模式\n */
    USB_PORT_MODE_B,
    /*! A线插入的模式\n */
    USB_PORT_MODE_A,
} USB_PORT_MODE_T;

#endif  /* __USDK1100_USB_MONITOR_H__ */
