/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : sys_info.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-07-06             v1.0             build this file 
 ********************************************************************************/
/*!
 * \file     sys_info.h
 * \brief    
 * \author   Herb Hu 
 * \version 1.0
 * \date  2009/03/24
 *******************************************************************************/


#ifndef __SYS_INFO_H__
#define __SYS_INFO_H__

#include <libc.h>

/*! 
 * \defgroup   applib_system_info   系统热插拔设备信息查询接口 
 */

/*!
 * \par  热插拔设备类型 
 *  \li  HOTPLUGIN_DEV_TYPE_LOCAL: 本地磁盘 
 *  \li  HOTPLUGIN_DEV_TYPE_CARD:  卡设备 
 *  \li  HOTPLUGIN_DEV_TYPE_UHOST: USB host 功能时外接U盘设备
 */
typedef enum 
{
    HOTPLUGIN_DEV_TYPE_NONE = 0,
    HOTPLUGIN_DEV_TYPE_LOCAL,
    HOTPLUGIN_DEV_TYPE_CARD,
    HOTPLUGIN_DEV_TYPE_UHOST,
}hotplugin_dev_type_e;

/*!
 * \par  设备插入状态 
 *  \li  HOTPLUGIN_DEV_IN:     设备已插入 
 *  \li  HOTPLUGIN_DEV_READY:  设备已准备好
 *  \li  HOTPLUGIN_DEV_ERR:    设备出错
 *  \li  HOTPLUGIN_DEV_OUT:    设备已拔出
 */
typedef enum
{
	HOTPLUGIN_DEV_IN = 0,
	HOTPLUGIN_DEV_READY,
	HOTPLUGIN_DEV_ERR,
	HOTPLUGIN_DEV_OUT,
}hotplugin_dev_insert_state_e;

/*!
 * \par  设备访问状态 
 *  \li  HOTPLUGIN_DEV_ACCESS_OK:   设备可以通过文件系统进行访问 
 *  \li  HOTPLUGIN_DEV_ACCESS_ERR:  设备不可以通过文件系统进行访问, 通常是因为该设备没有格式化 
 */
typedef enum
{
	HOTPLUGIN_DEV_ACCESS_OK = 0,
	HOTPLUGIN_DEV_ACCESS_ERR,
}hotplugin_dev_access_state_e;

/*!
 *  \brief 
 *  热插拔设备信息结构定义
 */
typedef struct
{
	 /*! 插入状态*/
    hotplugin_dev_insert_state_e insert;
	 /*! 访问状态*/
	hotplugin_dev_access_state_e access;
}hotplugin_dev_info_t;

/*!
 * \par  按照操作类型区分的电源电压门限类型定义 
 *  \li  POWER_THRESHOLD_GENERAL      : 普通操作类型     
 *  \li  POWER_THRESHOLD_FILE_OP      : 文件操作 
 *  \li  POWER_THRESHOLD_TVIN_REC     : 视频录制 
 *  \li  POWER_THRESHOLD_DC_REC       : 拍照 
 *  \li  POWER_THRESHOLD_DV_REC       : 录像 
 *  \li  POWER_THRESHOLD_VOL_REC      : 录音 
 *  \li  POWER_THRESHOLD_UHOST_SUPORT : usb host 功能支持 
 */
typedef enum 
{
    POWER_THRESHOLD_GENERAL = 0,
    POWER_THRESHOLD_FILE_OP,
    POWER_THRESHOLD_TVIN_REC,
    POWER_THRESHOLD_DC_REC,
    POWER_THRESHOLD_DV_REC,
    POWER_THRESHOLD_VOL_REC,
    POWER_THRESHOLD_UHOST_SUPPORT,
}power_threshold_type_e;

/******************************************************************************/
/*!                    
 * \par  Description:
 *      获取热插拔设备相关信息 
 * \ingroup applib_system_info
 * \param[in] dev -- 设备类型
 * \param[out] info -- 设备信息
 * \return  成功返回true, 失败返回false      
 *******************************************************************************/
extern bool sys_get_hotplugin_dev_info(hotplugin_dev_type_e dev, hotplugin_dev_info_t * info);

/******************************************************************************/
/*!                    
 * \par  Description:
 *     检查当前电源电压是否在某个操作类型的安全门限之上，从而保证操作的安全进行
 * \ingroup applib_system_info
 * \param[in] type -- 操作类型
 * \return  在安全门限之上返回true, 否则返回false      
 *******************************************************************************/
extern bool sys_power_threshold_check(power_threshold_type_e type);

/******************************************************************************/
/*!                    
 * \par  Description:
 *    获取当前主盘 
 * \ingroup applib_system_info
 * \return  如果主盘存在，返回当前主盘，否则返回HOTPLUGIN_DEV_TYPE_NONE      
 *******************************************************************************/
extern hotplugin_dev_type_e sys_get_main_disk(void);

/******************************************************************************/
/*!                    
 * \par  Description:
 *    设置当前主盘 
 * \ingroup applib_system_info
 * \param[in] type -- 磁盘类型
 * \return  成功返回true，否则返回false      
 * \note 仅供msg_serv等管理程序使用
 *******************************************************************************/
extern bool sys_set_main_disk(hotplugin_dev_type_e type);


#endif /*#ifndef __SYS_INFO_H__*/
