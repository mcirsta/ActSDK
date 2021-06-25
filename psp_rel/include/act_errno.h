/********************************************************************************
 *                              USDK(1100)
 *                            Module: ERRNO
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       fanqh     2008-12-30 17:25     1.0             build this file
 ********************************************************************************/
/*!
 * \file     act_errno.h
 * \brief    errno定义头文件
 * \author   fanqh
 * \version 1.0
 * \date  2008-12-30 17:25
 *******************************************************************************/
#ifndef __ACT_ERRNO_H__
#define __ACT_ERRNO_H__

#include "libc/errno.h"

#define   SYS_ERRONO_BASE							2000

#define   GUI_ERRONO_BASE							3000
/*! 参数取值不合法*/
#define   GUI_CONTEXT_INVALID_VALUE					(GUI_ERRONO_BASE+1)
/*!	使用到的全局变量未初始化*/
#define   GUI_CONTEXT_VARIABLE_UNINIT				(GUI_ERRONO_BASE+2)
/*! 字体文件打开失败*/
#define   GUI_FONT_OPEN_FONTLIB_FAILED				(GUI_ERRONO_BASE+3)
/*! 字体文件关闭失败*/
#define   GUI_FONT_CLOSE_FONTLIB_FAILED				(GUI_ERRONO_BASE+4)
/*! 所选择的字体未打开*/
#define	  GUI_FONT_SELECTED_UNINIT					(GUI_ERRONO_BASE+5)
/*! 所选择的字号不支持*/
#define	  GUI_FONT_UNSUPPORTED_FONTSIZE				(GUI_ERRONO_BASE+6)
/*! 字符串指针为空*/
#define	  GUI_TEXT_STR_NULL_POINTER					(GUI_ERRONO_BASE+7)
/*! 编码转换错误*/
#define   GUI_TEXT_ENC_CONVERT_ERROR				(GUI_ERRONO_BASE+8)
/*! 从字库读取数据出错*/
#define	  GUI_TEXT_GET_DATA_FROM_FONTLIB_ERROR		(GUI_ERRONO_BASE+9)
/*! 从缓存读取数据出错*/
#define   GUI_TEXT_GET_DATA_FROM_CACHE_ERROR		(GUI_ERRONO_BASE+10)
/*! 打开资源文件失败*/
#define   GUI_RES_OPEN_FILE_FAILED					(GUI_ERRONO_BASE+11)
/*! 读取出的数据非法*/
#define	  GUI_RES_INVALID_VALUE						(GUI_ERRONO_BASE+12)
/*! 函数参数不合法*/
#define   GUI_RES_INVALID_PARAMETER					(GUI_ERRONO_BASE+13)
/*! framebuffer设备未打开*/
#define   GUI_FB_DEVICE_UNOPENED					(GUI_ERRONO_BASE+14)
/*! framebuffer地址非法*/
#define   GUI_FB_INVALID_FBADDR						(GUI_ERRONO_BASE+15)
/*! 执行ioctl命令出错*/
#define   GUI_FB_CMD_EXEC_ERROR						(GUI_ERRONO_BASE+16)
/*! 窗口管理函数参数不正确*/
#define   GUI_WM_INVALID_PARAMETER					(GUI_ERRONO_BASE+17)
/*! 窗口句柄为0或者不存在*/
#define   GUI_WM_INVALID_HANDLE						(GUI_ERRONO_BASE+18)
/*! 当前焦点窗口句柄为0*/
#define   GUI_WM_NO_FOCUS_WIN_EXIST   				(GUI_ERRONO_BASE+19)
/*! 函数参数指针不合法*/
#define   GUI_PARAMETER_NULL_POINTER 				(GUI_ERRONO_BASE+20)
/*! 参数值不合法*/
#define   GUI_PARAMETER_INVALIDE_VALUE 				(GUI_ERRONO_BASE+21)
/*! 变量值不合法*/
#define   GUI_VARIABLE_INVALID_VALUE				(GUI_ERRONO_BASE+22)
/*! 分配空间失败*/
#define   GUI_MALLOC_FAILED							(GUI_ERRONO_BASE+23)
/*! DC句柄未初始化*/
#define   GUI_GDI_DC_UNINITIALIZED					(GUI_ERRONO_BASE+24)
/*! DC属性值不合法*/
#define   GUI_GDI_DC_VALUE_INVALID					(GUI_ERRONO_BASE+25)

#define   FS_ERRONO_BASE								4000

#define   FLASH_ERRONO_BASE						5000

#define   AOTG_UOC_ERRONO_BASE				6000

#define   MMM_ERRONO_BASE					7000
/*! 打开文件失败 */
#define MMM_ERR_OPEN_FILE               (MMM_ERRONO_BASE+1)
/*! 文件格式不支持 */
#define MMM_ERR_FILE_NOT_SUPPORT  (MMM_ERRONO_BASE+2)
/*! 文件被损坏 */
#define MMM_ERR_FILE_DAMAGED         (MMM_ERRONO_BASE+3)
/*! 解码出错 */
#define MMM_ERR_DECODER_ERROR      (MMM_ERRONO_BASE+4)
/*! 编码出错 */
#define MMM_ERR_ENCODER_ERROR       (MMM_ERRONO_BASE+5)
/*! 回显数据格式不支持 */
#define MMM_ERR_VO_NOT_SUPPORT      (MMM_ERRONO_BASE+6)
/*! 此格式视频不支持录音 */
#define MMM_ERR_ENCORDER_FORMAT_NOT_SUPPORT    (MMM_ERRONO_BASE+7)
/*! 磁盘空间不够 */
#define MMM_ERR_ENCORDER_NO_SPACE   (MMM_ERRONO_BASE+8)
/*! 此格式视频不支持练唱 */
#define MMM_ERR_VOCALIZE_FORMAT_NOT_SUPPORT    (MMM_ERRONO_BASE+9)
/*! 无license，播放次数已满或是播放时间已经到期 */
#define MMM_ERR_NO_LICENSE              (MMM_ERRONO_BASE+10)
/*! DRM时钟错误 */
#define MMM_ERR_SECURE_CLOCK         (MMM_ERRONO_BASE+11)
/*! DRM信息错误 */
#define MMM_ERR_LICENSE_INFO         (MMM_ERRONO_BASE+12)
/*! 无效操作，注：但编解码处于某种状态时，应用层发送的某些命令会无效，错误号会相应的变为无效操作 */
#define MMM_ERR_INVALUABLE_OPERATION    (MMM_ERRONO_BASE+13)
/*! 其他错误 */
#define MMM_ERR_OTHER                   (MMM_ERRONO_BASE+14)

#define   AOTG_UDC_ERRONO_BASE				8000

#define   UDC4UVC_ERRONO_BASE					9000

#define   FSG_ERRONO_BASE							10000

#define   VRAM_ERRONO_BASE							11000

#define   MMM_DCA_ERRONO_BASE					12000

#define   MMM_DCT_ERRONO_BASE					13000

#define   KSC_ERRONO_BASE							14000

#define   MMM_MTP_ERRONO_BASE					15000

#define   UVC_ERRONO_BASE 							16000

#define   MSMTP_ERRONO_BASE						17000

#define   MTPLST_ERRONO_BASE 			  			18000

#define   STUB_ERRONO_BASE							19000

#define   FONTDRV_ERRONO_BASE					20000

#define   TVOUT_ERRONO_BASE						21000

#define   UPGRADE_ERRONO_BASE					22000
/*! 待升级固件不存在 */
#define UPGRADE_FW_NOT_EXIST        (UPGRADE_ERRONO_BASE+1)

/*! 缓冲区空间不够 */
#define UPGRADE_BUF_NOT_ENOUGH      (UPGRADE_ERRONO_BASE+2)

/*! 待升级固件的头部标志错误 */
#define UPGRADE_FW_HEADFLAG_ERR     (UPGRADE_ERRONO_BASE+3)

#define   DWL_ERRONO_BASE							23000

#define   PP_ERRONO_BASE								24000

#define   PLAYLIST_ERRONO_BASE                          25000
/*!分配内存失败*/
#define PLIST_NOMEM               (PLAYLIST_ERRONO_BASE+1)

/*!传入非法参数*/
#define PLIST_INVALIDPARAM           (PLAYLIST_ERRONO_BASE+2)

/*!指定的表结构为空.当要打开一个列表，而此列表又不存在时，会根据指定的结构创建列表，如果此结构为空，则返回此代码。*/
#define PLIST_EMPTYLISTFIELD      (PLAYLIST_ERRONO_BASE+3)

/*!使用失效的列表句柄*/
#define PLIST_INVALIDHANDLE       (PLAYLIST_ERRONO_BASE+4)

/*!不同的播放列表结构*/
#define PLIST_DIFFSTRUCT          (PLAYLIST_ERRONO_BASE+5)

/*!索引位置不在列表中*/
#define PLIST_INVALIDEINDEX       (PLAYLIST_ERRONO_BASE+6)

/*!列表中无文件名字段,某些场合要求列表必须含有文件全路径字段*/
#define PLIST_NONAMEFIELD         (PLAYLIST_ERRONO_BASE+7)

/*!使用fopen 的时候发生错误*/
#define PLIST_FOPENFAIL           (PLAYLIST_ERRONO_BASE+8)

/*!数据库中此文件信息已经存在*/
#define PLIST_FILEEXIST           (PLAYLIST_ERRONO_BASE+9)

/*!使用文件选择器设置工作目录时出错*/
#define PLIST_FSELSETWORKDIRFAIL  (PLAYLIST_ERRONO_BASE+10)

/*!字段名全hash失败*/
#define PLIST_FIELDHASHCOLLIDE    (PLAYLIST_ERRONO_BASE+11)

/*!没有记录发生变化*/
#define PLIST_NORECORDCHANGE      (PLAYLIST_ERRONO_BASE+12)

/*!试图修改系统创建的表*/
#define PLIST_CHANGESYSTBL        (PLAYLIST_ERRONO_BASE+13)

/*!非法的列表类型*/
#define PLIST_INVALIDLISTTYPE   (PLAYLIST_ERRONO_BASE+14)

/*!试图修改不能修改的字段.当前只支持星级和文件全路径被修改*/
#define PLIST_MODIFYREADABLEFIELD  (PLAYLIST_ERRONO_BASE+15)

/*!操作列表中不存在的字段*/
#define PLIST_FIELDNOTEXIST        (PLAYLIST_ERRONO_BASE+16)

/*!按照指定的条件搜索不到此文件*/
#define PLIST_NOFILE               (PLAYLIST_ERRONO_BASE+17)

/*!路径为空字符串*/
#define PLIST_NULLPATH             (PLAYLIST_ERRONO_BASE+18)

/*!目录选择器设置过滤条件错误*/
#define PLIST_FSELSETFILTERFAIL    (PLAYLIST_ERRONO_BASE+19)

/*!打开数据库文件失败*/
#define PLIST_OPENDBFAIL           (PLAYLIST_ERRONO_BASE+20)

/*!过滤字段在列表中不存在*/
#define PLIST_NOFILTERFIELD        (PLAYLIST_ERRONO_BASE+21)

/*!非法的文件类型*/
#define PLIST_INVALIDFILETYPE      (PLAYLIST_ERRONO_BASE+22)

/*!此表未创建*/
#define PLSIT_NOTEXISTTABLE        (PLAYLIST_ERRONO_BASE+23)

/*!数据库文件被损坏*/
#define PLIST_CORRUPTDBFILE          (PLAYLIST_ERRONO_BASE+24)

#define   MLANG_ERRONO_BASE                          26000
/*! 函数参数为空指针*/
#define   MLANG_NULL_POINTER			(MLANG_ERRONO_BASE+1)
/*! 打开编码转换文件失败*/
#define   MLANG_OPEN_CODEPAGE_FAILED	(MLANG_ERRONO_BASE+2)
/*! 打开多国语言配置文件失败*/
#define   MLANG_OPEN_CONFIG_FAILED		(MLANG_ERRONO_BASE+3)
/*! 多国语言配置文件解析失败*/
#define   MLANG_PARSE_CONFIG_FAILED		(MLANG_ERRONO_BASE+4)
/*! 语言id不合法*/
#define   MLANG_INVALID_LANGUAGE_ID		(MLANG_ERRONO_BASE+5)

#define   FSEL_ERRONO_BASE                          27000

/*! 输入参数错误*/
#define  FSEL_PARA_ERROR              (FSEL_ERRONO_BASE+1)
/*! 数据库没有查询结果*/
#define  FSEL_NO_RECORD               (FSEL_ERRONO_BASE+2)
/*! 操作数据库指令执行返回不成功*/
#define  FSEL_OP_DB_ERROR             (FSEL_ERRONO_BASE+3)
/*! 内存申请失败*/
#define  FSEL_MALLOC_ERROR            (FSEL_ERRONO_BASE+4)
/*! 数据异常,包括物理上查询文件失败*/
#define  FSEL_DATA_ERROR              (FSEL_ERRONO_BASE+5)
/*! 磁盘空间满*/
#define  FSEL_DISK_FULL               (FSEL_ERRONO_BASE+6)
/*! 未设定工作目录*/
#define  FSEL_NOWORKDIR               (FSEL_ERRONO_BASE+7)

#define   APPS_ERRONO_BASE                          28000

/*! 进程退出主消息循环*/
#define  APPS_QUIT                    (APPS_ERRONO_BASE+1)

#define  APCONFIG_ERRONO_BASE                       29000
/*! 找不到KEY */
#define CONFIG_KEY_NOT_FOUND          (APCONFIG_ERRONO_BASE+1)
/*! 设置buffer的长度超过最大值 */
#define CONFIG_EXCEED_MAX_LENGTH      (APCONFIG_ERRONO_BASE+2)

#endif
