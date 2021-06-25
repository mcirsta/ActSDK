/********************************************************************************
 *                              USDK1100
 *                            Module: USB STACK
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>               <version >             <desc>
 *       GaoJing   2008-12-31 15:37:14     1.0              build this file
 ********************************************************************************/
/*!
 * \file     usb_otgstack.h
 * \brief    this file defines and declares the related data structure and APIs
 *           for the applications
 * \author   Gao Jing
 * \version 1.0
 * \date  2008-12-31 15:36:57
 *******************************************************************************/
#ifndef __USDK1100_USTOR_RELATED_H__
#define __USDK1100_USTOR_RELATED_H__

/*!

 *    ioctl CMD 命令字定义 \n

 */

/*!
 *    供app调用以获知当前是否有A线是否被拔出，查询结果类型详见enum aplug_status定义\n
 */
#define HOTPLUG_IOCTL_GET_APLUG_STATUS           (HOTPLUG_IOCLT_BASE)
/*!
 *    供app查询外部usb存储设备的挂载情况，查询结果类型详见enum disk_status定义\n
 */
#define HOTPLUG_IOCTL_GET_DISK_STATUS           (HOTPLUG_IOCLT_BASE+1)

enum aplug_status
{
    APLUG_OUT = 0x00, //A线拔出
    APLUG_IN = 0x01,
//A线插入
};

enum disk_status
{
    DISK_OUT = 0x00, //外部usb存储设备尚未插入
    DISK_IN = 0x01, //外部usb存储设备已插入
    DISK_READY = 0x02, //外部usb存储设备已完成枚举并可被uhost支持，已可读写访问
    DISK_ERROR = 0x03,
//外部usb存储设备在枚举过程中出错，不能被uhost所支持，不能使用
};

#endif  /* __USDK1100_USTOR_RELATED_H__ */

