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
 * \file     uoc_drv.h
 * \brief    this file defines and declares the related data structure and APIs
 *           for the applications
 * \author   Gao Jing
 * \version 1.0
 * \date  2008-12-31 15:36:57
 *******************************************************************************/
#ifndef __USDK1100_UOC_DRV_H__
#define __USDK1100_UOC_DRV_H__

/*!
 *    ���USB IDpinֵ���ݴ����������豸����A�豸��B�豸\n
 */
#define UOC_IOCTL_GET_ID_PIN           (UOC_IOCLT_BASE)

/*!
 *    ���B_SESS_VLD��ѹ״̬���ݴ��ж�USB�����Ƿ񱻰γ�\n
 */
#define UOC_IOCTL_CHK_BSESS_VLD           (UOC_IOCLT_BASE+1)
/*!
 *   ��⵱ǰ���ӵ���USB���������USB Host��PC��\n
 */
#define UOC_IOCTL_CHK_CHARGER_CONNECTED           (UOC_IOCLT_BASE+2)
/*!
 *   ��ȡUSBӲ��״̬����ǰ��������ת״̬\n
 */
#define UOC_IOCTL_GET_OTG_STATUS           (UOC_IOCLT_BASE+3)
/*!
 *   ���USB״̬����ǰ�Ƿ��ڿɷ���SRP������״̬��\n
 */
#define UOC_IOCTL_CAN_DO_SRP           (UOC_IOCLT_BASE+4)
/*!
 *   ���USB״̬����ǰ�Ƿ��ڿɷ���HNP������״̬��\n
 */
#define UOC_IOCTL_CAN_DO_HNP         (UOC_IOCLT_BASE+5)

/*!
 *   ���USB�������Ƿ�֧�ַ���HNP����\n
 */
#define UOC_IOCTL_CHK_HNP_SUPPORTED           (UOC_IOCLT_BASE+6)

/*!
 *  ���USB��������HNP���������Ƿ��Ѿ���ʹ�ܺ�����\n
 */
#define UOC_IOCTL_CHK_HNP_ENABLED           (UOC_IOCLT_BASE+7)

/*!
 *    Ҫ��uoc��������SRP����\n
 */
#define UOC_IOCTL_DO_SRP           (UOC_IOCLT_BASE+8)
/*!
 *    Ҫ��uoc����ֹͣSRP����\n
 */
#define UOC_IOCTL_STOP_SRP           (UOC_IOCLT_BASE+9)

/*!
 *    Ҫ��uoc��������HNP����\n
 */
#define UOC_IOCTL_DO_HNP         (UOC_IOCLT_BASE+10)

/*!
 *  \brief
 *  VBUS��ѹ״̬
 *  ��app��UOC_IOCTL_CHK_BSESS_VLDΪ�����ֵ���uoc_ioctlʱ�ķ���ֵ
 *  \li VBUS_OFF : VBUS��ѹ�ڼ������֮�£���usb�������ӣ�
 *  \li VBUS_ON  : VBUS��ѹ�ڼ������֮�ϣ���usb�������ӣ�
 */
typedef enum
{
    /*!  VBUS��ѹ�ڼ������֮��*/
    BSESS_OFF,
    /*!  VBUS��ѹ�ڼ������֮��*/
    VBUS_ON
} UOC_BSESS_VLD_T;

/*!
 *  \brief
 *  ������ǰ����С����ϵUSB�������USB������PC)
 *  ��app��UOC_IOCTL_CHK_CHARGER_CONNECTEDΪ�����ֵ���uoc_ioctlʱ�ķ���ֵ
 *  \li HOST : ��ǰ���ӵ���USB������PC)
 *  \li CHARGER : ��ǰ���ӵ���USB�����
 */
typedef enum
{
    /*! ��ǰ���ӵ���USB������PC)*/
    HOST,
    /*! ��ǰ���ӵ���USB�����*/
    CHARGER
} UOC_CONNECTION_TYPE_T;

#endif  /* __USDK1100_UOC_DRV_H__ */
