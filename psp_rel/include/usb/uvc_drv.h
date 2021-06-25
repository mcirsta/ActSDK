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
#ifndef __USDK1100_UVC_DRV_H__
#define __USDK1100_UVC_DRV_H__

/*!

 *    ioctl CMD 命令字定义 \n

 */

/*!

 *    停止uvc驱动运行，停止pc camera图像传输\n

 */

#define UVC_IOCTL_DO_DISCONNECT           (UVC_IOCLT_BASE)

//uvc驱动工作线程优先级时所使用的优先级
#define	UVC_TASK_PRIO 		4

/*!
 *   Actions UVC PID \n
 */
#define UVC_PRODUCT_ID   0x3300  //default valuse
/*!
 *   Actions UVC device release number \n
 */
#define UVC_PRODUCT_RELEASE   0x1100  //default valuse
/*!
 *    UVC设备序列号数组的长度\n
 */
#define UVC_SERIAL_LEN  33

/*!
 *  \brief
 *  该结构体用来供app设定要求底层UVC驱动以指定的参数来运行\n
 *  uvc功能对应App需填充该结构体信息，并以此为模块参数来加载uvc.ko
 */
struct uvc_runtime_arg
{

    /*! app要求指定的uvc驱动内部创建的thread运行优先级*/
    int task_priority;
    /*! app要求msc驱动挂载和上报给pc的实u盘分区数量*/
    unsigned short VID;
    /*! 该usb设备的产品编号PID*/
    unsigned short PID;
    /*!该usb设备的产品批次号码*/
    unsigned short release;
    /*!该usb设备的生产厂商名称*/
    unsigned char usb_vendor[8];
    /*! 该usb设备的产品名称*/
    unsigned char usb_product[16];
    /*!由app指定并传下的 序列号*/
    unsigned char serial[MSC_SERIAL_LEN];
};

#endif  /* __USDK1100_UVC_DRV_H__ */

