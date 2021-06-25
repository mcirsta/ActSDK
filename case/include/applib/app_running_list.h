/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 * file : app_running_list.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-3-24             v1.0             build this file
 ********************************************************************************/
/*!
 * \file     app_running_list.h
 * \brief    应用进程信息列表管理模块
 * \author   Herb Hu
 * \version 1.0
 * \date  2009/03/24
 *******************************************************************************/



#ifndef __APP_RUNNING_LIST_H__
#define __APP_RUNNING_LIST_H__

/*! 
 * \defgroup   applib_app_running_list    应用进程信息列表管理
 */

#include <libc.h>

#define MAX_GRAB_MSG   (32)
#define MAX_FILTER_MSG (32)
#define MAX_GLOBAL_MQ  (32)
#define MAX_NAME_LEN   (64)


/*!
 * \par  应用进程活动状态 
 *  \li  APP_STATE_INACTIVE: 没有活动 
 *  \li  APP_STATE_ACTIVE: 活动 
 */
typedef enum {
    APP_STATE_INACTIVE, 
    APP_STATE_ACTIVE
}app_state_e;

/*!
 * \par  应用进程类型 
 *  \li  APP_TYPE_GUI: GUI进程(前台进程, 可以接受GUI消息) 
 *  \li  APP_TYPE_CONSOLE: CONSOLE进程(后台进程, 不可以接受GUI消息) 
 */
typedef enum 
{
    APP_TYPE_GUI, 
    APP_TYPE_CONSOLE
}app_type_e;

/*!
 * \brief
 * 系统消息回调函数类型定义
 */
typedef int (* sys_msg_proc)(msg_apps_t *msg);

/*!
 * \brief
 *  进程连接的全局消息队列信息 
 */
typedef struct 
{
    char mq_name[MAX_NAME_LEN];
    int mqdes;
    sys_msg_proc proc;
}global_mq_data_t;

/*!
 * \brief
 *  描述一个应用进程的相关信息
 */
typedef struct
{
    /*! 进程ID */
    pid_t pid;
    /*! 进程名 */
    char name[32];
    /*! 进程专有消息队列名 */
    char mq_name[MAX_NAME_LEN];
    /*! 进程专有消息队列描述符 */
    mqd_t mqdes;
    /*! 进程类型 */
    app_type_e type;
    /*! 进程活动状态 */
    app_state_e state;
    /*! 软看门狗下次超时的时间 */
    unsigned long soft_watchdog_timeout_expires;
    /*! 是否禁止看门狗 */
    bool soft_watchdog_forbidden;
    /*! 是否禁止屏保 */
    bool screensaver_forbidden;
    /*! 是否禁止省电关机 */
    bool auto_standby_forbidden;
    /*! 是否禁止睡眠关机 */
    bool timed_shutdown_forbidden;
    /*! 被grab的msg*/
    int msg_grabed[MAX_GRAB_MSG];
    /*! 定时器是否有效*/
    bool timer_valid;
    /*! 被filter的msg*/
    int msg_filtered[MAX_FILTER_MSG];
    /*! 是否filter所有的msg*/
    bool filter_all_msg;
    /*! 连接的全局消息队列 */
    global_mq_data_t global_mq[MAX_GLOBAL_MQ];
}app_info_t;

/*!
 * \brief
 * 应用进程信息列表结点定义
 */
typedef struct app_list
{
    const app_info_t * app_info;
    struct app_list * next;
}app_running_list_t;

/******************************************************************************/
/*!
 * \par  Description:
 *     初始化进程信息列表
 * \par 细节 
 *     连接到存储进程信息列表的共享内存, 如果该共享内存不存在，则创建之
 * \ingroup applib_app_running_list
 * \return 成功返回true, 失败返回false  
 *******************************************************************************/
extern bool app_running_list_init(void);

/******************************************************************************/
/*!
 * \par  Description:
 *     注销进程信息列表
 * \par 细节 
 *     断开到存储进程信息列表的共享内存的连接
 * \ingroup applib_app_running_list
 * \return 成功返回true, 失败返回false  
 *******************************************************************************/
extern bool app_running_list_deinit(void);

/******************************************************************************/
/*!
 * \par  Description:
 *     通过进程名查询一个进程是否存在于进程管理信息列表中
 * \ingroup applib_app_running_list
 * \param[in] app_name -- 进程名
 * \return 若进程存在, 返回true, 否则返回false
 *******************************************************************************/
extern bool is_app_exist(const char *app_name);

/******************************************************************************/
/*!
 * \par  Description:
 *    通过进程名查询应用进程类型(GUI进程或console进程)
 * \ingroup applib_app_running_list
 * \param[in]  app_name -- 进程名
 * \param[out] type -- 返回进程类型 
 * \return 成功返回true, 否则返回false(进程不存在)
 *******************************************************************************/
extern bool get_app_type(const char *app_name, app_type_e * type);

/******************************************************************************/
/*!
 * \par  Description:
 *     通过进程名获取该进程的pid
 * \ingroup applib_app_running_list
 * \param[in] app_name -- 进程名
 * \return 若进程存在, 返回其pid, 否则返回-1
 *******************************************************************************/
extern pid_t get_app_pid(const char * app_name);

/******************************************************************************/
/*!
 * \par  Description:
 *     通过进程pid获取该进程的进程名
 * \ingroup applib_app_running_list
 * \param[in] pid -- 进程pid
 * \return 若进程存在, 返回其进程名, 否则返回NULL
 *******************************************************************************/
extern const char * get_app_name(pid_t pid);

/******************************************************************************/
/*!
 * \par  Description:
 *    获取指定应用进程的信息
 * \ingroup applib_app_running_list
 * \param[in] app_name -- 进程名
 * \return 若进程存在, 返回描述该进程信息的结构体指针, 否则返回NULL
 *******************************************************************************/
extern const app_info_t * app_running_list_get_app_info(const char * app_name);

/******************************************************************************/
/*!
 * \par  Description:
 *    获取本应用进程的信息
 * \ingroup applib_app_running_list
 * \return 若进程存在, 返回描述该进程信息的结构体指针, 否则返回NULL
 *******************************************************************************/
extern const app_info_t * app_running_list_get_this_app_info(void);

/******************************************************************************/
/*!
 * \par  Description:
 *    修改指定应用的信息
 * \ingroup applib_app_running_list
 * \param[in] app_name -- 进程名
 * \param[in] app_info -- 新的进程信息结构体指针
 * \return 修改成功返回true, 否则返回false
 * \note 仅供manager 和 applib 库内部使用
 *******************************************************************************/
extern bool app_running_list_set_app_info(const char * app_name, const app_info_t * app_info);

/******************************************************************************/
/*!
 * \par  Description:
 *    修改本应用的信息
 * \ingroup applib_app_running_list
 * \param[in] app_info -- 新的进程信息结构体指针
 * \return 修改成功返回true, 否则返回false
 * \note 仅供manager 和 applib 库内部使用
 *******************************************************************************/
extern bool app_running_list_set_this_app_info(const app_info_t * app_info);

/******************************************************************************/
/*!
 * \par  Description:
 *     获取当前运行应用进程信息列表
 * \ingroup applib_app_running_list
 * \return 成功返回进程信息列表指针, 否则返回NULL
 * \note 仅供manager 和 applib 库内部使用
 *******************************************************************************/
extern  const app_running_list_t * app_running_list_get_list(void);

/******************************************************************************/
/*!
 * \par  Description:
 *     增加一个应用进程信息到进程信息列表
 * \ingroup applib_app_running_list
 * \param[in] appinfo -- 指向一个进程信息结构体
 * \return 成功返回true, 否则返回flase
 * \note 仅供 applib 库内部使用
 *******************************************************************************/
extern bool app_running_list_add(const app_info_t * appinfo);

/******************************************************************************/
/*!
 * \par  Description:
 *    从应用进程信息列表中删除指定应用的信息
 * \ingroup applib_app_running_list
 * \param[in] app_name -- 应用进程名
 * \return 成功返回true, 否则返回flase
 * \note 仅供 applib 库内部使用
 *******************************************************************************/
extern bool app_running_list_delete(const char * app_name);

/******************************************************************************/
/*!
 * \par  Description:
 *    打印进程信息列表
 * \ingroup applib_app_running_list
 * \return void
 *******************************************************************************/
extern void print_app_running_list(void);

#endif /*#ifndef __APP_RUNNING_LIST_H__*/
