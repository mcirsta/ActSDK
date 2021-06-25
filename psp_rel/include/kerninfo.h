/********************************************************************************
 *                              USDK1100
 *                            Module: KERNINFO
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       hmwei     2009-06-12 16:26     1.0             build this file
 *       hmwei     2009-06-25 16:26     1.1             ioctl�ӿ�����MATCH_MOUNT_DEV������
 *       hmwei     2009-06-30 10:14     1.2             ������Ӧ�ýӿ�����������put_msg/get_msg�ӿ���
 *       hmwei     2009-06-03 10:14     1.3             ioctl�ӿ�����GET_DEV_INFO������,��ɾ��MATCH_MOUNT_DEV������
 ********************************************************************************/
/*!
 * \file     kerninfo.h
 * \brief    ϵͳ������Ϣ���úͻ�ȡ
 * \author   hmwei
 * \version 1.3
 * \date  2009/6/25
 *******************************************************************************/
#ifndef __KERNINFO_H__
#define __KERNINFO_H__

#define SYS_MSG_USB_BASE        (0x30)
#define SYS_MSG_USB_MONITOR     (SYS_MSG_USB_BASE+0)
#define SYS_MSG_USB_HOST        (SYS_MSG_USB_BASE+8)
/* �ṩ��usb_monitor ��*/
#define SYS_MSG_USB_A_IN        (SYS_MSG_USB_MONITOR+0)
#define SYS_MSG_USB_A_OUT       (SYS_MSG_USB_MONITOR+1)
#define SYS_MSG_USB_B_IN        (SYS_MSG_USB_MONITOR+2)
#define SYS_MSG_USB_B_OUT       (SYS_MSG_USB_MONITOR+3)
/* �ṩ��usb host ��*/
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
 *  sysinfo�ַ��豸ioctl������
 *  \li PUT_SYS_MSG : ����ϵͳ��Ϣ
 *  \li GET_SYS_MSG : ��ȡϵͳ��Ϣ
 *  \li GET_DEV_INFO : ��ȡ�豸����Ϣ(�Ƿ����/�Ƿ����)
 */
enum
{
    PUT_SYS_MSG,
    GET_SYS_MSG,
    GET_DEV_INFO
};

/*!
 *  \brief
 *  �豸ID
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
 *  �豸����״̬
 *  \li DEV_IN : �豸����
 *  \li DEV_READY : �豸��֧��
 *  \li DEV_ERR : �豸��֧��
 *  \li DEV_OUT : �豸�γ�
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
 *  �豸����״̬
 *  \li DEV_ACCESS_OK : �豸�ɷ���
 *  \li DEV_ACCESS_ERR : �豸���ܷ���
 */
enum
{
    DEV_ACCESS_OK,
    DEV_ACCESS_ERR
};
/*!
 * \brief
 *      �豸��Ϣ���ݽṹ
 */
typedef struct
{
    /*! ���ڱ�־*/
    unsigned int exist_flags;
    /*! ���ʱ�־*/
    unsigned int access_flags;
} kerninfo_dev_info_t;

/*!
 * \brief
 *      ��ȡ�豸��Ϣ��������Ӧ�����ݽṹ
 */
typedef struct
{
    /*! �豸Id*/
    unsigned int dev_id;
    /*! �豸��Ϣ����buf*/
    kerninfo_dev_info_t *dev_info;
} get_dev_info_arg_t;

#ifndef FUCNTION_SYMBOL //ͨ������ָ�뷽ʽ���ýӿ�ʱ��������FUCNTION_SYMBOL�궨�壬�����������ظ��������
extern int kerninfo_put_msg(unsigned int msg);
extern int kerninfo_get_msg(unsigned int *p_msg);

#endif /*FUCNTION_SYMBOL*/

#endif /*_ASSEMBLER_*/
#endif /*__KERNINFO_H__*/

