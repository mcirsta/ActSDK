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
 *    ioctl CMD �����ֶ��� \n
 *
 */

/*!
 *    ��app�����Ի�֪��ǰ�Ƿ���usb������С��\n
 */
#define UMONITOR_IOCTL_GET_USB_CON           (UMONITOR_BASE)
/*!
 *    ��app����USBΪ���߻����״̬������monitor��Ϣ֪ͨGUI\n
 */
#define UMONITOR_IOCTL_SET_USB_CON           (UMONITOR_BASE+1)
/*!
 *    ʹ�ܡ���ֹusb monitor���в���߼��\n
 */
#define UMONITOR_IOCTL_ENABLE_DETECT         (UMONITOR_BASE+2)
/*!
 *    ��app�����Ի�֪��ǰmonitor����߼�⹦���Ƿ���ʹ��״̬\n
 */
#define UMONITOR_IOCTL_IS_WORKING             (UMONITOR_BASE+3)
/*!
 *    ��app�����Ի�֪��ǰ����С�����ǳ��������PC\n
 */
#define UMONITOR_IOCTL_IS_CHARGER             (UMONITOR_BASE+4)
/*!
 *    �ϲ��֪Monitor AP���ڵ�ģʽ��Udisk���ǳ��ģʽ������\n
 */
#define UMONITOR_IOCTL_SET_USB_MODE             (UMONITOR_BASE+5)

/*!
 *  \brief
 *  ���ֳ����/usb host��PC���ӿڵĽ�����Ͷ��塣
 */
typedef enum
{
    /*! ��ǰ�����ϵusb host��PC��\n */
    USB_HOST = 0x00,
    /*! ��ǰ�����ϵusb charger\n */
    USB_CHARGER = 0x01,

} USB_CONNECTOR_T;

/*!
 *  \brief
 *  ����߼��ʹ��/��ֹ����ֵ���壬��ӦUMONITOR_IOCTL_ENABLE_DETECT���ioctl����
 */
typedef enum
{
    /*! ֹͣ����߼�⣬ֱ����app�ٴ�ʹ�� */
    DISABLE_DETECT = 0x00,
    /*! ʹ�ܲ���߼�⣬ֱ����app�ٴν�ֹ*/
    ENABLE_DETECT = 0x01,

} USB_DETECTION_T;

/*!
 *  \brief
 *  USB״̬����ֵ���壬��ӦUMONITOR_IOCTL_SET_USB_CON���ioctl����
 */
typedef enum
{
    /*! �γ�usb��*/
    PLUG_OUT = 0x00,
    /*! ����usb��*/
    PLUG_IN = 0x01,
} USB_CONNECTION_T;

/*!
 *  \brief
 *  USB�����ʱVBUS������޵�ѹ�ɹ�ѡ��ĵ�ѹֵ��
 */
typedef enum
{
    /*! VBUS������޵�ѹ��Ϊ4.7v\n */
    VBUS_47 = 0x1,
    /*! VBUS������޵�ѹ��Ϊ4.4v\n */
    VBUS_44 = 0x2,
    /*! VBUS������޵�ѹ��Ϊ4.0v\n */
    VBUS_40 = 0x3,
    /*! VBUS������޵�ѹ��Ϊ3.5v\n */
    VBUS_35 = 0x4,
} VBUS_VOLTAGE_T;

/*!
 *  \brief
 *  usb monitor��������ʱ����Ĳ����ṹ��
 *
 *  �˽ṹ�����Ŀ����ʹmonitor���в���߼��ʱ�Ķ�ʱ�����м�������ݵĻ���Դ������ѹ��λ�������á�
 *	��������ͻ����ܵĸ�����������Ͷ��ο���
 *
 */
typedef struct
{
    /*! ����vubs��ѹ���м��ʱ�ĵ�ѹ���޵�λ�趨(1:4.7v, 2:4.4v, 3:4.0v, 4:3.5v) */
    VBUS_VOLTAGE_T vbus_threshold;
    /*! ���������ʱ��usb_timer���м��(���룬����500ms) */
    unsigned int usbtimer :16;
    /*! check_again��ʱ�����м��(���룬����100ms)*/
    unsigned int chktimer :16;
    /*! ��־λ�������Ƿ����vbus��Ϊ���Ļ���Դ*/
    unsigned int use_vbus :1;
    /*! ��־λ�������Ƿ����b_see_vld��Ϊ���Ļ���Դ*/
    unsigned int use_bvld :1;
    /*! ��־λ�������Ƿ����ID pin��Ϊ���Ļ���Դ */
    unsigned int use_id :1;
    /*! ��־λ�������Ƿ����D+��Ϊ���Ļ���Դ*/
    unsigned int use_dp :1;
} umonitor_arg_t;

/*!
 *  \brief
 *  ����USB ģʽ�����Ͷ��塣
 */
typedef enum
{
    /*! ��ǰ��Ϊcharger\n */
    USB_MODE_CHARGE = 0,
    /*! B�߲���󣬵�ǰ������ΪUdisk����MTP\n */
    USB_MODE_UDISK_MTP,
    /*! ��ǰ��ΪUdisk\n */
    USB_MODE_SENSOR,
    USB_MODE_UDISK,
    /*! ��ǰ��ΪMTP\n */
    USB_MODE_MTP,
    /*! ��ǰΪĬ��ģʽ\n */
    USB_MODE_DEFAULT,
} USB_MODE_T;

/*!
 *  \brief
 *  ����USB �˿�ģʽ�����Ͷ��塣
 */
typedef enum
{
    /*! Defaultģʽ\n */
    USB_PORT_MODE_DEFAULT = 0,
    /*! B�߲����ģʽ\n */
    USB_PORT_MODE_B,
    /*! A�߲����ģʽ\n */
    USB_PORT_MODE_A,
} USB_PORT_MODE_T;

#endif  /* __USDK1100_USB_MONITOR_H__ */
