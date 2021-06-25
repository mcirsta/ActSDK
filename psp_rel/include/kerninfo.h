/********************************************************************************
 *                              USDK1100
 *                            Module: KERNINFO
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2009-06-12 16:26     1.0             build this file
 *       hmwei     2009-06-25 16:26     1.1             ioctl接口增加MATCH_MOUNT_DEV命令字
 *       hmwei     2009-06-30 10:14     1.2             避免与应用接口重名，更改put_msg/get_msg接口名
 *       hmwei     2009-06-03 10:14     1.3             ioctl接口增加GET_DEV_INFO命令字,并删除MATCH_MOUNT_DEV命令字
 ********************************************************************************/
/*!
 * \file     kerninfo.h
 * \brief    系统各类信息设置和获取
 * \author   hmwei
 * \version 1.3
 * \date  2009/6/25
 *******************************************************************************/
#ifndef __KERNINFO_H__
#define __KERNINFO_H__

#define SYS_MSG_USB_BASE        (0x30)
#define SYS_MSG_USB_MONITOR     (SYS_MSG_USB_BASE+0)
#define SYS_MSG_USB_HOST        (SYS_MSG_USB_BASE+8)
/* 提供给usb_monitor 用*/
#define SYS_MSG_USB_A_IN        (SYS_MSG_USB_MONITOR+0)
#define SYS_MSG_USB_A_OUT       (SYS_MSG_USB_MONITOR+1)
#define SYS_MSG_USB_B_IN        (SYS_MSG_USB_MONITOR+2)
#define SYS_MSG_USB_B_OUT       (SYS_MSG_USB_MONITOR+3)
/* 提供给usb host 用*/
#define SYS_MSG_ADD_DEV_MSC     (SYS_MSG_USB_HOST+0)
#define SYS_MSG_REMOVE_DEV_MSC  (SYS_MSG_USB_HOST+1)
#define SYS_MSG_DEV_MSC_READY   (SYS_MSG_USB_HOST+2)
#define SYS_MSG_DEV_MSC_ERR     (SYS_MSG_USB_HOST+3)
#define SYS_MSG_ADD_DEV_HID     (SYS_MSG_USB_HOST+4)
#define SYS_MSG_REMOVE_DEV_HID  (SYS_MSG_USB_HOST+5)
#define SYS_MSG_REMOVE_DEV_ALL  (SYS_MSG_USB_HOST+0x20-1)

#define SYS_POWER_ADAPTER_BASE 0x70
#define SYS_ROUND_POWER_ADAPTER_IN    (SYS_POWER_ADAPTER_BASE+0)
#define SYS_POWER_ADAPTER_OUT    (SYS_POWER_ADAPTER_BASE+1)

#define SYS_MSG_CARD_BASE   0x60
#define SYS_MSG_CARD_IN     (SYS_MSG_CARD_BASE+0)
#define SYS_MSG_CARD_OUT    (SYS_MSG_CARD_BASE+1)
#define SYS_MSG_CARD_READY  (SYS_MSG_CARD_BASE+2)
#define SYS_MSG_CARD_ERR    (SYS_MSG_CARD_BASE+3)

#define SYS_MSG_ALARM_BASE 0x50
#define SYS_MSG_ALARM_IN    (SYS_MSG_ALARM_BASE+0)

#ifndef _ASSEMBLER_

/*!
 *  \brief
 *  sysinfo字符设备ioctl命令字
 *  \li PUT_SYS_MSG : 发送系统消息
 *  \li GET_SYS_MSG : 获取系统消息
 *  \li GET_DEV_INFO : 获取设备的信息(是否存在/是否可用)
 */
enum
{
    PUT_SYS_MSG,
    GET_SYS_MSG,
    GET_DEV_INFO
};

/*!
 *  \brief
 *  设备ID
 *  \li DEV_ID_CARD : SD/MMC CARD
 *  \li DEV_ID_USB_HOST : USB HOST DEV
 *  \li DEV_ID_LOCAL : LOCAL DISK
 */
enum
{
    DEV_ID_CARD,
    DEV_ID_USB_HOST,
    DEV_ID_LOCAL,
    DEV_ID_MAX
};

/*!
 *  \brief
 *  设备存在状态
 *  \li DEV_IN : 设备插入
 *  \li DEV_READY : 设备可支持
 *  \li DEV_ERR : 设备不支持
 *  \li DEV_OUT : 设备拔出
 */
enum
{
    DEV_OUT,
    DEV_IN,
    DEV_READY,
    DEV_ERR
};

/*!
 *  \brief
 *  设备访问状态
 *  \li DEV_ACCESS_OK : 设备可访问
 *  \li DEV_ACCESS_ERR : 设备不能访问
 */
enum
{
    DEV_ACCESS_OK,
    DEV_ACCESS_ERR
};
/*!
 * \brief
 *      设备信息数据结构
 */
typedef struct
{
    /*! 存在标志*/
    unsigned int exist_flags;
    /*! 访问标志*/
    unsigned int access_flags;
} kerninfo_dev_info_t;

/*!
 * \brief
 *      获取设备信息命令字相应的数据结构
 */
typedef struct
{
    /*! 设备Id*/
    unsigned int dev_id;
    /*! 设备信息储存buf*/
    kerninfo_dev_info_t *dev_info;
} get_dev_info_arg_t;

#ifndef FUCNTION_SYMBOL //通过函数指针方式调用接口时，需增加FUCNTION_SYMBOL宏定义，否则编译会有重复定义错误
extern int kerninfo_put_msg(unsigned int msg);
extern int kerninfo_get_msg(unsigned int *p_msg);

#endif /*FUCNTION_SYMBOL*/

#endif /*_ASSEMBLER_*/
#endif /*__KERNINFO_H__*/

