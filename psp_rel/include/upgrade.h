/********************************************************************************
 *                              usdk130
 *                            Module: UPGRADE
 *                 Copyright(c) 2001-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       huanghe     2008-08-31      1.0                    build this file
 *       huanghe     2009-01-04      2.0                    sdk代码整改
 ********************************************************************************/
/*!
 * \file			upgrade.h
 * \brief		upgrade引用外部的变量类型和提供的对外接口
 * \author		黄河
 * \par Copyright(c) 2001-2008 Actions Semiconductor, All Rights Reserved.
 *
 * \version		2.0
 * \date			2009/1/4
 *******************************************************************************/

#ifndef  __UPGRADE_H__
#define  __UPGRADE_H__

/*! \cond USB_UPGRADE_API*/

/*!	升级运行状态
 *  \li UPG_STATE_DEFAULT，开始之前的默认状态
 *  \li UPG_STATE_INIT，初始状态
 *  \li UPG_STATE_CHECK_FW，检查fw正确性
 *  \li UPG_STATE_CHECK_OK，fw检查完毕
 *  \li UPG_STATE_WRITE_FW，写入fw
 *  \li UPG_STATE_WRITE_OK，fw写入完毕
 *  \li UPG_STATE_EXIT，退出当此操作
 * 	\li UPG_STATE_ERROR，出错
 */
enum upg_state_t
{
    UPG_STATE_DEFAULT = 0,
    UPG_STATE_INIT,
    UPG_STATE_CHECK_FW,
    UPG_STATE_CHECK_OK,
    UPG_STATE_WRITE_FW,
    UPG_STATE_WRITE_OK,
    UPG_STATE_EXIT,
    UPG_STATE_ERROR
};

/*! 无错 */
#define NO_ERR                  (0)

/*! 基数 */
#define BASE_ERR                (100)

/*! 固件不存在 */
#define FW_NOT_EXIST_ERR        (BASE_ERR+1)

/*! 内存不够 */
#define BUF_MALLOC_ERR          (BASE_ERR+2)

/*! 固件头的标志错误 */
#define FW_HEAD_FLAG_ERR        (BASE_ERR+3)

/*! dsp初始化错误 */
#define DSP_INIT_ERR            (BASE_ERR+4)

/*! 固件头数据解密出错 */
#define FW_HEAD_DATA_ERR        (BASE_ERR+5)

/*! 固件长度不对 */
#define FW_LENGTH_ERR           (BASE_ERR+6)

/*! 固件尾部标志错误 */
#define FW_TAIL_FLAG_ERR        (BASE_ERR+7)

/*! 固件尾部校验和错误 */
#define FW_TAIL_CHECKSUM_ERR    (BASE_ERR+8)

/*! 固件的校验和错误 */
#define FW_CHECKSUM_ERR         (BASE_ERR+9)

/*! afi头的标志为错误 */
#define AFI_HEAD_FLAG_ERR       (BASE_ERR+10)

/*! afi头的目录项错误 */
#define AFI_HEAD_DIR_ERR        (BASE_ERR+11)

/*! afu头的错误 */
#define AFI_HEAD_CHECKSUM_ERR   (BASE_ERR+12)

/*! lfi头的标志位错误 */
#define LFI_HEAD_FLAG_ERR       (BASE_ERR+13)

/*! lfi头的校验和错误 */
#define LFI_HEAD_CHECKSUM_ERR   (BASE_ERR+14)

/*! 新固件写入时将覆盖旧固件的u盘区 */
#define UDISK_OVERLAP_ERR       (BASE_ERR+15)

/*! mbrc的标志位错误 */
#define MBRC_FLAG_ERR           (BASE_ERR+16)

/*! mbrc1的校验和错误 */
#define MBRC_1st_CHECKSUM_ERR   (BASE_ERR+17)

/*! mbrc2的校验和错误 */
#define MBRC_2nd_CHECKSUM_ERR   (BASE_ERR+18)

/*! brec的标志位错误 */
#define BREC_FLAG_ERR           (BASE_ERR+19)

/*! brec的校验和错误 */
#define BREC_CHECKSUM_ERR       (BASE_ERR+20)

/*! storage逻辑写错 */
#define STORAGE_LOG_WRITE_ERR   (BASE_ERR+21)

/*! storage逻辑读错 */
#define STORAGE_LOG_READ_ERR    (BASE_ERR+22)

/*! storage物理写错 */
#define STORAGE_PHY_WRITE_ERR   (BASE_ERR+23)

/*! storage物理读错 */
#define STORAGE_PHY_READ_ERR	(BASE_ERR+24)

/*! strage访问出错 */
#define STORAGE_OTHER_ERR       (BASE_ERR+25)

/*! 固件读错 */
#define FW_READ_ERR             (BASE_ERR+26)

/*! 固件定位错 */
#define FW_SEEK_ERR             (BASE_ERR+27)

/*! 超时 */
#define TIMEOUT_ERR             (BASE_ERR+28)

/*! 状态错误 */
#define STATE_ERR               (BASE_ERR+29)

/*! 创建检查固件任务错 */
#define CREATE_CHECK_TASK_ERR   (BASE_ERR+30)

/*! 创建写固件任务错 */
#define CREATE_WRITE_TASK_ERR   (BASE_ERR+31)

/*! 内核导出符号错误 */
#define KERNEL_SYM_ERR          (BASE_ERR+32)

/*! 未知错误 */
#define UNKNOWN_ERR             (BASE_ERR+100)

/*!
 * \brief	升级运行状态的结构体，用于应用和驱动间传送状态
 */
struct upg_status_t
{
    /*! 升级运行状态 */
    enum upg_state_t e_state;

    /*! 升级进度值，0～100，由驱动写入 */
    unsigned int d_prg;

    /*! 升级错误类型 */
    int d_errtype;
};

/*! this enumeration is defined for ioctrl command of upgrade driver
 *  \li UPG_CMD_INIT : 初始化
 *  \li UPG_CMD_CHECK_FW : 检查固件是否正确
 *  \li UPGRADE_WRITE_FW : 写入固件
 *  \li UPG_CMD_EXIT : 退出
 *  \li UPGRADE_READ_STATUS : 读取升级状态
 *  \li UPG_CMD_MODIFY_PARAM : 修改升级参数
 */
enum upg_ioctl_cmd_t
{
    UPG_CMD_INIT,
    UPG_CMD_CHECK_FW,
    UPG_CMD_WRITE_FW,
    UPG_CMD_EXIT,
    UPG_CMD_READ_STATUS,
    UPG_CMD_MODIFY_PARAM
};

/*!
 * \brief	应用传给驱动的参数
 */
struct upg_param_t
{
    /*! 自动升级驱动内部任务的优先级 */
    int task_priority;

    /*! 固件路径 */
    unsigned char *pb_fwpath;

    /*! 是否检查将要写的最大lba，以防止覆盖待升级的固件所在fs */
    unsigned char b_is_check_maxlba;
};

/*! \endcond*/

#endif /*__UPGRADE_H__ */
