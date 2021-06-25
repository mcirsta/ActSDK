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
 * \file     usb_hidden_disk.h
 * \brief    this file defines and declares the related data structure and APIs
 *           for the applications
 * \author   Gao Jing
 * \version 1.0
 * \date  2008-12-31 15:36:57
 *******************************************************************************/
#ifndef __USDK1100_USB_HIDDEN_DISK_H__
#define __USDK1100_USB_HIDDEN_DISK_H__

/*! \cond USB_MSC_API*/

/*!
 *    供app调用以获知当前是否有usb线连入小机\n
 */
#define HD_MAX_PWD_LEN      32

/*!
 *    供app调用以获知当前是否有usb线连入小机\n
 */
#define PRG_BROWSE      (1<<0)
/*!
 *    供app调用以获知当前是否有usb线连入小机\n
 */
#define PRG_READ        (1<<1)
/*!
 *    供app调用以获知当前是否有usb线连入小机\n
 */
#define PRG_WRITE       (1<<2)
/*!
 *    供app调用以获知当前是否有usb线连入小机\n
 */
#define PRG_ADISK       (1<<3)

/*!
 *    供app调用以获知当前是否有usb线连入小机\n
 */
#define ATTR_HD                 (1<<0)
/*!
 *    供app调用以获知当前是否有usb线连入小机\n
 */
#define ATTR_PWD_SUPPORT        (1<<1)
/*!
 *    供app调用以获知当前是否有usb线连入小机\n
 */
#define ATTR_DISABLE            (1<<2)
/*!
 *    供app调用以获知当前是否有usb线连入小机\n
 */
#define ATTR_PWD_COMFIRMED      (1<<7)

/*!
 *  \brief
 *  该结构体用来供app填充隐藏盘相关信息，并传递至内核供MSC驱动使用。
 */
typedef struct
{
    /*! 该隐藏盘的密码*/
    unsigned char pwd[HD_MAX_PWD_LEN];
    /*! 该隐藏盘所支持的操作属性*/
    unsigned char attr;
    /*! 该隐藏盘所允许的访问权限（通过PC端工具途径）*/
    unsigned char privilege;
} hidden_disk_info_t;

enum attr_type
{
    HIDDEN_DISK_PRIVILEGE = 0,
    HIDDEN_DISK_ATTR,
    HIDDEN_DISK_PWD
};

/*! \endcond */
#endif  /* __USDK1100_USB_HIDDEN_DISK_H__ */
