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
 *    检测USB IDpin值，据此区分联入设备的是A设备或B设备\n
 */
#define UOC_IOCTL_GET_ID_PIN           (UOC_IOCLT_BASE)

/*!
 *    检测B_SESS_VLD电压状态，据此判断USB电缆是否被拔除\n
 */
#define UOC_IOCTL_CHK_BSESS_VLD           (UOC_IOCLT_BASE+1)
/*!
 *   检测当前连接的是USB充电器还是USB Host（PC）\n
 */
#define UOC_IOCTL_CHK_CHARGER_CONNECTED           (UOC_IOCLT_BASE+2)
/*!
 *   获取USB硬件状态机当前所处的跳转状态\n
 */
#define UOC_IOCTL_GET_OTG_STATUS           (UOC_IOCLT_BASE+3)
/*!
 *   检测USB状态机当前是否处在可发起SRP操作的状态下\n
 */
#define UOC_IOCTL_CAN_DO_SRP           (UOC_IOCLT_BASE+4)
/*!
 *   检测USB状态机当前是否处在可发起HNP操作的状态下\n
 */
#define UOC_IOCTL_CAN_DO_HNP         (UOC_IOCLT_BASE+5)

/*!
 *   检测USB控制器是否支持发起HNP操作\n
 */
#define UOC_IOCTL_CHK_HNP_SUPPORTED           (UOC_IOCLT_BASE+6)

/*!
 *  检测USB控制器的HNP操作功能是否已经被使能和允许\n
 */
#define UOC_IOCTL_CHK_HNP_ENABLED           (UOC_IOCLT_BASE+7)

/*!
 *    要求uoc驱动发起SRP操作\n
 */
#define UOC_IOCTL_DO_SRP           (UOC_IOCLT_BASE+8)
/*!
 *    要求uoc驱动停止SRP操作\n
 */
#define UOC_IOCTL_STOP_SRP           (UOC_IOCLT_BASE+9)

/*!
 *    要求uoc驱动发起HNP操作\n
 */
#define UOC_IOCTL_DO_HNP         (UOC_IOCLT_BASE+10)

/*!
 *  \brief
 *  VBUS电压状态
 *  当app以UOC_IOCTL_CHK_BSESS_VLD为命令字调用uoc_ioctl时的返回值
 *  \li VBUS_OFF : VBUS电压在检测门限之下（无usb电缆连接）
 *  \li VBUS_ON  : VBUS电压在检测门限之上（有usb电缆连接）
 */
typedef enum
{
    /*!  VBUS电压在检测门限之下*/
    BSESS_OFF,
    /*!  VBUS电压在检测门限之上*/
    VBUS_ON
} UOC_BSESS_VLD_T;

/*!
 *  \brief
 *  表明当前连接小机的系USB充电器或USB主机（PC)
 *  当app以UOC_IOCTL_CHK_CHARGER_CONNECTED为命令字调用uoc_ioctl时的返回值
 *  \li HOST : 当前连接的是USB主机（PC)
 *  \li CHARGER : 当前连接的是USB充电器
 */
typedef enum
{
    /*! 当前连接的是USB主机（PC)*/
    HOST,
    /*! 当前连接的是USB充电器*/
    CHARGER
} UOC_CONNECTION_TYPE_T;

#endif  /* __USDK1100_UOC_DRV_H__ */
