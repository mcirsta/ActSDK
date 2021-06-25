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
#ifndef __USDK1100_USB_IOCTL_H__
#define __USDK1100_USB_IOCTL_H__

/*! \cond USBMONITOR*/

/*!
 *    ioctl CMD ÃüÁî×Ö¶¨Òå \n
 *
 */
#define USB_IOCTL_BASE                   0x4800

#define UMONITOR_BASE              USB_IOCTL_BASE

#define UOC_IOCLT_BASE             (USB_IOCTL_BASE+0x20)

#define UDC_IOCLT_BASE             (USB_IOCTL_BASE+0x40)

#define UDC4UVC_IOCLT_BASE     (USB_IOCTL_BASE+0x50)

#define MSC_IOCLT_BASE             (USB_IOCTL_BASE+0x60)

#define UVC_IOCLT_BASE             (USB_IOCTL_BASE+0xa0)

#define HOTPLUG_IOCLT_BASE             (USB_IOCTL_BASE+0xb0)
#endif  /* __USDK1100_USB_IOCTL_H__ */

