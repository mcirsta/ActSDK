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

 *    ioctl CMD �����ֶ��� \n

 */

/*!
 *    ��app�����Ի�֪��ǰ�Ƿ���A���Ƿ񱻰γ�����ѯ����������enum aplug_status����\n
 */
#define HOTPLUG_IOCTL_GET_APLUG_STATUS           (HOTPLUG_IOCLT_BASE)
/*!
 *    ��app��ѯ�ⲿusb�洢�豸�Ĺ����������ѯ����������enum disk_status����\n
 */
#define HOTPLUG_IOCTL_GET_DISK_STATUS           (HOTPLUG_IOCLT_BASE+1)

enum aplug_status
{
    APLUG_OUT = 0x00, //A�߰γ�
    APLUG_IN = 0x01,
//A�߲���
};

enum disk_status
{
    DISK_OUT = 0x00, //�ⲿusb�洢�豸��δ����
    DISK_IN = 0x01, //�ⲿusb�洢�豸�Ѳ���
    DISK_READY = 0x02, //�ⲿusb�洢�豸�����ö�ٲ��ɱ�uhost֧�֣��ѿɶ�д����
    DISK_ERROR = 0x03,
//�ⲿusb�洢�豸��ö�ٹ����г������ܱ�uhost��֧�֣�����ʹ��
};

#endif  /* __USDK1100_USTOR_RELATED_H__ */

