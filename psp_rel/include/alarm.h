/********************************************************************************
 *                        USDK130
 *                     	common sub
 *
 *                (c) Copyright 2002, Actions Co,Ld.
 *                        All Right Reserved
 *
 * File: alarm.h
 * By  : wurui
 * Version: 1> v1.00     first version     2009-05-29 8:28
 *******************************************************************************
 */
/*!
 * \file     alarm.h
 * \brief    定时提醒设备驱动的头文件
 * \author   wurui
 * \version 1.0
 * \date  2009/05/29
 *******************************************************************************/
#ifndef __ALARM_H__
#define __ALARM_H__

#include "kmod_calls.h"
#include "syscalls.h"

#define FUCNTION_SYMBOL
#include "rtc.h"

/*!
 *  \brief
 *  屏的模式
 *  \li ALARM_ONCE : 指定日期和时间
 *  \li UPRIGHT_MODE : 指定周期和时间
 */
typedef enum
{
    ALARM_ONCE,
    ALARM_WEEKLY
} alarm_type_t;

/*!
 * \brief
 *      alarm_info_t,计时器信息结构 .
 */
typedef struct
{
    /*! clock的类型，可指定日期或指定循环周期*/
    alarm_type_t type;

    /*! 定时时间*/
    rtc_time_t time;

    /*! 定时日期*/
    struct
    {
        /*! 在type==ALARM_WEEKLY类型时有效，定时周期*/
        char wday[7];
        /*! 在type==ALARM_ONCE类型时有效，定时日期*/
        rtc_date_t date;
    } special_date;
} alarm_info_t;

/*!
 * \brief
 *      alarm_new_in_t,创建新定时器的输入参数结构.
 */
typedef struct
{
    /*! 定时器的初始属性*/
    alarm_info_t alarm_clock;
    /*! 要在哪一路mode上建立定时器*/
    int mode_no;
} alarm_new_in_t;

/*!
 * \brief
 *      alarm_new_out_t,创建新定时器的输出参数结构.
 */
typedef struct
{
    /*! 新建立的定时器的序列号*/
    int clock_no;
} alarm_new_out_t;

/*!
 * \brief
 *      alarm_new_param_t,创建新定时器的参数结构.
 */
typedef struct
{
    /*! 创建新定时器的输入参数结构*/
    alarm_new_in_t in;
    /*! 创建新定时器的输出参数结构*/
    alarm_new_out_t out;
} alarm_new_param_t;

/*!
 * \brief
 *      alarm_query_in_t,定时器信息查询的输入参数结构;
 *      alarm_delete_param_t,定时器删除的参数结构.
 */
typedef struct
{
    /*! 定时器所属的模式号*/
    int mode_no;
    /*! 定时器的定时器号*/
    int clock_no;
} alarm_query_in_t, alarm_delete_param_t;

/*!
 * \brief
 *      alarm_query_out_t,定时器信息查询的输出参数结构
 */
typedef struct
{
    /*! 定时器的定时信息*/
    alarm_info_t alarm_clock;
    /*! 本次开机是否为该定时器唤醒的结果,0x01表示是因为该定时器的到达导致自动开机,0x00表示不是*/
    int wakeup;
} alarm_query_out_t;

/*!
 * \brief
 *      alarm_modify_param_t,定时器信息修改的参数结构
 */
typedef struct
{
    /*! 定时器所属的模式号*/
    int mode_no;
    /*! 定时器的定时器号*/
    int clock_no;
    /*! 定时器的新信息*/
    alarm_info_t alarm_info;
} alarm_modify_param_t;

/*!
 * \brief
 *      alarm_query_param_t,定时器信息查询的参数结构
 */
typedef struct
{
    /*! 定时器信息查询的输入参数结构*/
    alarm_query_in_t in;
    /*! 定时器信息查询的输出参数结构*/
    alarm_query_out_t out;
} alarm_query_param_t;

/*!
 * \brief
 *      alarm_get_clock_param_t,已生效定时器的索引信息获取结构
 */
typedef struct
{
    /*! 已生效定时器所属的模式号*/
    int mode_no;
    /*! 已生效定时器的定时器号*/
    int clock_no;
} alarm_queue_node_t, alarm_get_clock_param_t;

/*!
 *  \brief
 *  定时器设备的ioctl操作命令字
 *  \li ALARM_NEW_CLOCK : 创建定时器
 *  \li ALARM_MODIDY_CLOCK : 修改定时器内容
 *  \li ALARM_DELETE_CLOCK : 删除定时器
 *  \li ALARM_GET_CLOCK : 获取已生效定时器索引
 *  \li ALARM_QUERY_CLOCK : 查询定时器内容
 *  \li ALARM_RESET : 在修改系统时间后，重新排列系统定时
 *  \li ALARM_DELETE_MODE : 删除指定模式下的所有定时
 *  \li ALARM_DELETE_ALL : 删除所有定时
 */
enum
{
    ALARM_NEW_CLOCK,
    ALARM_MODIDY_CLOCK,
    ALARM_DELETE_CLOCK,
    ALARM_GET_CLOCK,
    ALARM_QUERY_CLOCK,
    ALARM_RESET,
    ALARM_DELETE_MODE,
    ALARM_DELETE_ALL,
};

/* 在standby程序中调用的静态库函数，用于检查和保存开机是否为定时到达引起
 返回值为1表示本次开机因为定时器唤醒引起，返回值为0则为其它原因*/
int alarm_check_wakeup_reason(void);

#endif /*__ALARM_H__*/
