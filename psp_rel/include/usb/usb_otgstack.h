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
#ifndef __USDK1100_USB_OTGSTACK_H__
#define __USDK1100_USB_OTGSTACK_H__

/*! \cond USB_MSC_API*/

/*!
 *    ioctl CMD �����ֶ��� \n
 *
 */

/*!
 *    ��app�����Զ�̬���ش��̷���\n

 */
#define MSC_IOCTL_SET_DISK           (MSC_IOCLT_BASE)

/*!
 *    ��app���������ô��̵Ŀɶ�����\n

 */
#define MSC_IOCTL_SET_RO           (MSC_IOCLT_BASE+1)

/*!
 *    ��app�����Ի�ȡ��ǰmsc��������״̬\n

 */
#define MSC_IOCTL_GET_STATUS         (MSC_IOCLT_BASE+2)

/*!
 *    ��app������֪ͨmsc��������ˢ����ʾ���\n

 */
#define MSC_IOCTL_SHOW_DOWN             (MSC_IOCLT_BASE+3)

/*!
 *    ��app�������˳�u��\n

 */
#define MSC_IOCTL_DO_DISCONNECT             (MSC_IOCLT_BASE+4)

/*!
 *    ��app�����Լ��u���Ƿ��Ѵ��ڶϿ�״̬\n

 */
#define MSC_IOCTL_DETECT_DISCON             (MSC_IOCLT_BASE+5)

/*!
 *    ��app�����Լ��u���Ƿ���д����������\n

 */
#define MSC_IOCTL_WRITE_HAPPENED             (MSC_IOCLT_BASE+6)

//msc���������߳����ȼ�ʱ��ʹ�õ����ȼ�
#define	MSC_TASK_PRIO 		4

/*!
 *   Actions USB VID \n
 */
#define USB_VENDOR_ID    0x10d6 // Actions
/*!
 *   Actions MSC PID \n
 */
#define MSC_PRODUCT_ID   0x1101  //default valuse
/*!
 *   Actions MSC device release number \n
 */
#define MSC_PRODUCT_RELEASE   0x0100  //default valuse
/*!
 *    U�̷������豸�ڵ�·�����ַ���������󳤶�\n
 */
#define UDEV_NAME_LEN  32
/*!
 *    ������غ��ϱ���PC��U���̷��������\n
 */
#define MAX_LUNS	8

/*!
 *    U�����к�����ĳ���\n
 */
#define MSC_SERIAL_LEN  33

/*!
 *  \brief
 *  U�������õ�Э�����ͣ���ǰ���ǵ�MSC����ʵ���в���8070I��
 */
typedef enum
{
    /*! Reduced Block Commands (flash)  */
    USB_SC_RBC = 0x01,
    /*! SFF-8020i, MMC-2, ATAPI (CD-ROM)  */
    USB_SC_8020 = 0x02,
    /*! QIC-157 (tape)  */
    USB_SC_QIC = 0x03,
    /*! UFI (floppy)  */
    USB_SC_UFI = 0x04,
    /*! SFF-8070i (removable) */
    USB_SC_8070 = 0x05,
    /*! Transparent SCSI  */
    USB_SC_SCSI = 0x06,
} UDISK_PROTOCOL_T;

/*!
 *  \brief
 *  U�̹����µ�ʵ�ʽ������ͣ���ǰ���ǵ�MSC����ʵ���в���nand flash��card��autorun��
 */
typedef enum
{
    /*! �մ���*/
    NOMEDIA_DEF = 0x0,
    /*! ϵͳ����  */
    SYSDISK_DEF = 0x73797320,
    /*!  NAND FLASH����  */
    NANDDISK_DEF = 0x6e616e64,
    /*! NOR FLASH���� */
    NORDISK_DEF = 0x6e6f7220,
    /*! �忨����  */
    CARDDISK_DEF = 0x63617264,
    /*! HD����  */
    HARDDISK_DEF = 0x68642020,
    /*! USB��������  */
    USBDISK_DEF = 0x686f7374,
    /*! SDRAM����  */
    SDRAMDISK_DEF = 0x7364726d,
    /*!  autorun ����  */
    AUTORUN_DEF = 0x6175746f,
} UDISK_MEDIA_T;

/*!
 *  \brief
 *  U�������ļ�ϵͳ���ͣ���ǰ���ǵ�MSC�����ж�Ϊfat32��fat16��
 */
typedef enum
{
    /*! û�и�ʽ�����ʽ������*/
    FS_UNKNOWN = 0,
    /*! FAT 12��ʽ�ķ�����ϵͳδ֧��*/
    FS_FAT12 = 12,
    /*! ����FAT 16��ʽ�ķ���*/
    FS_FAT16 = 16,
    /*! ����FAT 32��ʽ�ķ���*/
    FS_FAT32 = 32,
    /*! ���ܷ���,��û�н��ܣ�������*/
    FS_HOLD = 255,
} FS_TYPE_T;

/*!
 *  \brief
 *  �ýṹ��������app����������Ҫ��U�̷�����Ϣ�����������ں˹�MSC����ʹ�á�
 */
typedef struct
{
    /*! ��u�������豸�ڵ�·�����ַ�����*/
    char udev_name[UDEV_NAME_LEN];
    /*! ��u������ʵ�ʽ�������*/
    UDISK_MEDIA_T medium_type;
    /*! ��u���������õ�Э������*/
    UDISK_PROTOCOL_T udev_protocol;
    /*! ��u�������ļ�ϵͳ����*/
    FS_TYPE_T udev_fstype;
    /*! ��appָ���÷��������MAX_LUNS��u�̷����е�λ��*/
    unsigned int lun_index;
    /*! ��u�����Ƿ�����̬���*/
    unsigned char removable :1;
    /*! ��u�����Ƿ�ֻ��*/
    unsigned char readonly :1;
    /*! ��u�����Ƿ�����stall*/
    unsigned char can_stall :1;
    /*! �ýṹ�弰��Ӧ��u�����Ƿ�ʹ�ܺͿ���*/
    unsigned char enabled :1;
    hidden_disk_info_t hd_info;
} udisk_info_t;

/*!
 *  \brief
 *  �ýṹ��������app�趨Ҫ��ײ�MSC�������غ��ϱ���PC��u��������������u�����������Ϣ��\n
 *  Udisk��ӦApp�����ýṹ����Ϣ�����Դ�Ϊģ�����������msc.ko
 */
struct udisk_param
{
    /*! appҪ��ָ����msc�����ڲ�������thread�������ȼ�*/
    int task_priority;
    /*! appҪ��msc�������غ��ϱ���pc��ʵu�̷�������*/
    int nluns;
    /*! ��usb�豸���������̱��VID*/
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
    /*! �������غ��ϱ��ĸ�u�̷����Ĳ�����Ϣ�����udisk_info struct����*/
    udisk_info_t disklist[MAX_LUNS];
};

/*!
 *  \brief
 *  UDISK��״̬����
 **************************************************************
 **MSB          *              *           *              LSB**
 **	31         8*7            4*     3     * 2             0 **
 **************************************************************
 **             *              *           *                 **
 ** Reserve     * Sub-Status   * Removable * Main-Status     **
 **************************************************************
 */

/*!
 *  \brief
 *  UDISK���ܵ�ǰ����״̬
 */
typedef enum
{
    /*! Ĭ��״̬*/
    UDISK_M_STATUS_UNKNOWN = 0,
    /*! ״̬��Ч�����Է���*/
    UDISK_M_STATUS_ALLOW_ACCESS = 1,
    /*! ״̬��Ч����ֹ����*/
    UDISK_M_STATUS_PREVENT_ACCESS = 2,
} UDISK_MAIN_STATUS_T;

#define	UDISK_M_STATUS_REMOVABLE  (1<<3)

/*!
 *  \brief
 *  UDISK���ܵ�ǰ����״̬
 */
typedef enum
{
    /*! ����״̬*/
    UDISK_S_STATUS_IDLE = 0,
    /*! ��״̬*/
    UDISK_S_STATUS_READ,
    /*! д״̬*/
    UDISK_S_STATUS_WRITE,
    /*! ����״̬*/
    UDISK_S_STATUS_UPDATE,
    /*! ���ӶϿ�״̬*/
    UDISK_S_STATUS_DISCON,
    /*! ������������*/
    UDISK_S_STATUS_ERROR,
    /*! HDS��*/
    UDISK_S_STATUS_HDS_FULL,
} UDISK_SUB_STATUS_T;

#define	GET_MAIN_STATUS(status) 			((status) & 0x07)
#define	SET_MAIN_STATUS(status,value)		(((status) & 0xf8) |((value) & 0x07))
#define	GET_SUB_STATUS(status)  				(((status) & 0xf0) >> 4)
#define	SET_SUB_STATUS(status,value)			(((status) & 0x0f) |(((value) & 0x0f) << 4))
#define	SET_ALLOW_REMOVE_STATUS(status)		((status) & ~UDISK_M_STATUS_REMOVABLE)
#define	SET_PREVENT_REMOVE_STATUS(status)	((status) | UDISK_M_STATUS_REMOVABLE)

#endif  /* __USDK1100_USB_OTGSTACK_H__ */
