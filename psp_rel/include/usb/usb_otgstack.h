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
 *    ioctl CMD 命令字定义 \n
 *
 */

/*!
 *    供app调用以动态挂载磁盘分区\n

 */
#define MSC_IOCTL_SET_DISK           (MSC_IOCLT_BASE)

/*!
 *    供app调用以设置磁盘的可读属性\n

 */
#define MSC_IOCTL_SET_RO           (MSC_IOCLT_BASE+1)

/*!
 *    供app调用以获取当前msc驱动所处状态\n

 */
#define MSC_IOCTL_GET_STATUS         (MSC_IOCLT_BASE+2)

/*!
 *    供app调用以通知msc驱动画面刷新显示完成\n

 */
#define MSC_IOCTL_SHOW_DOWN             (MSC_IOCLT_BASE+3)

/*!
 *    供app调用以退出u盘\n

 */
#define MSC_IOCTL_DO_DISCONNECT             (MSC_IOCLT_BASE+4)

/*!
 *    供app调用以检测u盘是否已处于断开状态\n

 */
#define MSC_IOCTL_DETECT_DISCON             (MSC_IOCLT_BASE+5)

/*!
 *    供app调用以检测u盘是否有写操作发生过\n

 */
#define MSC_IOCTL_WRITE_HAPPENED             (MSC_IOCLT_BASE+6)

//msc驱动工作线程优先级时所使用的优先级
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
 *    U盘分区的设备节点路径（字符串）的最大长度\n
 */
#define UDEV_NAME_LEN  32
/*!
 *    允许挂载和上报给PC的U盘盘符最大数量\n
 */
#define MAX_LUNS	8

/*!
 *    U盘序列号数组的长度\n
 */
#define MSC_SERIAL_LEN  33

/*!
 *  \brief
 *  U盘所采用的协议类型，当前我们的MSC功能实现中采用8070I。
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
 *  U盘功能下的实际介质类型，当前我们的MSC功能实现中采用nand flash、card或autorun。
 */
typedef enum
{
    /*! 空磁盘*/
    NOMEDIA_DEF = 0x0,
    /*! 系统磁盘  */
    SYSDISK_DEF = 0x73797320,
    /*!  NAND FLASH磁盘  */
    NANDDISK_DEF = 0x6e616e64,
    /*! NOR FLASH磁盘 */
    NORDISK_DEF = 0x6e6f7220,
    /*! 插卡磁盘  */
    CARDDISK_DEF = 0x63617264,
    /*! HD磁盘  */
    HARDDISK_DEF = 0x68642020,
    /*! USB主机磁盘  */
    USBDISK_DEF = 0x686f7374,
    /*! SDRAM磁盘  */
    SDRAMDISK_DEF = 0x7364726d,
    /*!  autorun 磁盘  */
    AUTORUN_DEF = 0x6175746f,
} UDISK_MEDIA_T;

/*!
 *  \brief
 *  U盘区的文件系统类型，当前我们的MSC功能中多为fat32或fat16。
 */
typedef enum
{
    /*! 没有格式化或格式不可认*/
    FS_UNKNOWN = 0,
    /*! FAT 12格式的分区，系统未支持*/
    FS_FAT12 = 12,
    /*! 可用FAT 16格式的分区*/
    FS_FAT16 = 16,
    /*! 可用FAT 32格式的分区*/
    FS_FAT32 = 32,
    /*! 加密分区,还没有截密，不可用*/
    FS_HOLD = 255,
} FS_TYPE_T;

/*!
 *  \brief
 *  该结构体用来供app表述和填充必要的U盘分区信息，并传递至内核供MSC驱动使用。
 */
typedef struct
{
    /*! 该u盘区的设备节点路径（字符串）*/
    char udev_name[UDEV_NAME_LEN];
    /*! 该u盘区的实际介质类型*/
    UDISK_MEDIA_T medium_type;
    /*! 该u盘区所采用的协议类型*/
    UDISK_PROTOCOL_T udev_protocol;
    /*! 该u盘区的文件系统类型*/
    FS_TYPE_T udev_fstype;
    /*! 由app指定该分区在最多MAX_LUNS个u盘分区中的位置*/
    unsigned int lun_index;
    /*! 该u盘区是否允许动态插拔*/
    unsigned char removable :1;
    /*! 该u盘区是否只读*/
    unsigned char readonly :1;
    /*! 该u盘区是否被允许stall*/
    unsigned char can_stall :1;
    /*! 该结构体及对应的u盘区是否被使能和可用*/
    unsigned char enabled :1;
    hidden_disk_info_t hd_info;
} udisk_info_t;

/*!
 *  \brief
 *  该结构体用来供app设定要求底层MSC驱动挂载和上报给PC的u盘区数量，及各u盘区的相关信息。\n
 *  Udisk对应App需填充该结构体信息，并以此为模块参数来加载msc.ko
 */
struct udisk_param
{
    /*! app要求指定的msc驱动内部创建的thread运行优先级*/
    int task_priority;
    /*! app要求msc驱动挂载和上报给pc的实u盘分区数量*/
    int nluns;
    /*! 该usb设备的生产厂商编号VID*/
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
    /*! 各待挂载和上报的各u盘分区的参数信息，详见udisk_info struct定义*/
    udisk_info_t disklist[MAX_LUNS];
};

/*!
 *  \brief
 *  UDISK的状态定义
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
 *  UDISK功能当前的主状态
 */
typedef enum
{
    /*! 默认状态*/
    UDISK_M_STATUS_UNKNOWN = 0,
    /*! 状态有效，可以访问*/
    UDISK_M_STATUS_ALLOW_ACCESS = 1,
    /*! 状态无效，禁止访问*/
    UDISK_M_STATUS_PREVENT_ACCESS = 2,
} UDISK_MAIN_STATUS_T;

#define	UDISK_M_STATUS_REMOVABLE  (1<<3)

/*!
 *  \brief
 *  UDISK功能当前的子状态
 */
typedef enum
{
    /*! 空闲状态*/
    UDISK_S_STATUS_IDLE = 0,
    /*! 读状态*/
    UDISK_S_STATUS_READ,
    /*! 写状态*/
    UDISK_S_STATUS_WRITE,
    /*! 升级状态*/
    UDISK_S_STATUS_UPDATE,
    /*! 连接断开状态*/
    UDISK_S_STATUS_DISCON,
    /*! 遇到致命错误*/
    UDISK_S_STATUS_ERROR,
    /*! HDS满*/
    UDISK_S_STATUS_HDS_FULL,
} UDISK_SUB_STATUS_T;

#define	GET_MAIN_STATUS(status) 			((status) & 0x07)
#define	SET_MAIN_STATUS(status,value)		(((status) & 0xf8) |((value) & 0x07))
#define	GET_SUB_STATUS(status)  				(((status) & 0xf0) >> 4)
#define	SET_SUB_STATUS(status,value)			(((status) & 0x0f) |(((value) & 0x0f) << 4))
#define	SET_ALLOW_REMOVE_STATUS(status)		((status) & ~UDISK_M_STATUS_REMOVABLE)
#define	SET_PREVENT_REMOVE_STATUS(status)	((status) | UDISK_M_STATUS_REMOVABLE)

#endif  /* __USDK1100_USB_OTGSTACK_H__ */
