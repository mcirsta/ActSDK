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

 *    ioctl CMD �����ֶ��� \n

 */

/*!

 *    ֹͣuvc�������У�ֹͣpc cameraͼ����\n

 */

#define UVC_IOCTL_DO_DISCONNECT           (UVC_IOCLT_BASE)

//uvc���������߳����ȼ�ʱ��ʹ�õ����ȼ�
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
 *    UVC�豸���к�����ĳ���\n
 */
#define UVC_SERIAL_LEN  33

/*!
 *  \brief
 *  �ýṹ��������app�趨Ҫ��ײ�UVC������ָ���Ĳ���������\n
 *  uvc���ܶ�ӦApp�����ýṹ����Ϣ�����Դ�Ϊģ�����������uvc.ko
 */
struct uvc_runtime_arg
{

    /*! appҪ��ָ����uvc�����ڲ�������thread�������ȼ�*/
    int task_priority;
    /*! appҪ��msc�������غ��ϱ���pc��ʵu�̷�������*/
    unsigned short VID;
    /*! ��usb�豸�Ĳ�Ʒ���PID*/
    unsigned short PID;
    /*!��usb�豸�Ĳ�Ʒ���κ���*/
    unsigned short release;
    /*!��usb�豸��������������*/
    unsigned char usb_vendor[8];
    /*! ��usb�豸�Ĳ�Ʒ����*/
    unsigned char usb_product[16];
    /*!��appָ�������µ� ���к�*/
    unsigned char serial[MSC_SERIAL_LEN];
};

#endif  /* __USDK1100_UVC_DRV_H__ */

