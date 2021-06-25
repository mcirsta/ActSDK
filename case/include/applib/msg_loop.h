/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 * file : msg_loop.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-3-24             v1.0             build this file
 ********************************************************************************/
/*!
 * \file     msg_loop.h
 * \brief    应用消息循环
 * \author   Herb Hu
 * \version 1.0
 * \date  2009/03/24
 *******************************************************************************/



#ifndef __MSG_LOOP_H__
#define __MSG_LOOP_H__

#include <app_msg.h>
#include "app_running_list.h"

/*! 
 * \defgroup   applib_msg_loop  应用消息循环、通信接口模块 
 */

/******************************************************************************/
/*!
 * \par  Description:
 *      为新应用创建和初始化专有消息队列，并在进程管理信息列表中为其创建一个条目
 * \ingroup applib_msg_loop
 * \param[in] app_name -- 应用进程名
 * \return 成功返回true, 失败返回false
 * \note 仅供 manager进程在启动一个应用前调用
 *******************************************************************************/
extern bool applib_register_app(const char * app_name);

/******************************************************************************/
/*!
 * \par  Description:
 *     应用退出时回收其专有消息队列资源, 并在进程信息列表中删除其相关条目
 * \ingroup applib_msg_loop
 * \param[in] app_name -- 应用进程名
 * \return 成功返回true, 失败返回false
 * \note 仅供 manager进程在检测到一个应用进程退出后调用
 *******************************************************************************/
extern bool applib_unregister_app(const char * app_name);

/******************************************************************************/
/*!
 * \par  Description:
 *    连接管理进程信息列表的共享内存区，打开本进程专有消息队列
 * \ingroup applib_msg_loop
 * \param[in] argc -- 由main()函数传入的argc参数
 * \param[in] argv -- 由main()函数传入的argv参数
 * \param[in] type -- 指定进程类型(console进程还是gui进程)
 * \return 成功返回true, 失败返回false
 *******************************************************************************/
extern bool applib_init(int argc , const char * argv[], app_type_e type);

/******************************************************************************/
/*!
 * \par  Description:
 *    断开管理进程消息列表的共享内存区，关闭本进程专有消息队列
 * \ingroup applib_msg_loop
 * \return 成功返回true, 失败返回false
 *******************************************************************************/
extern bool applib_quit(void);

/******************************************************************************/
/*!
 * \par  Description:
 *    获取本进程的系统消息。
 * \ingroup applib_msg_loop
 * \param[out] msg -- 存放获取到的系统消息
 * \return 收到消息返回true, 如果有调用exit_msg_loop()则返回false
 * \note 只有收到消息或调用了exit_msg_loop(), get_msg()才会返回
 *******************************************************************************/
extern bool get_msg(msg_apps_t *msg);

/******************************************************************************/
/*!
 * \par  Description:
 *    给指定进程发送同步消息并挂起本进程，直到对方收到消息并完成处理
 * \ingroup applib_msg_loop
 * \param[in] app_name -- 目标进程名
 * \param[in] msg -- 待发送消息体结构指针
 * \param[out] reply -- 存放回执信息的结构体指针
 * \param[in] timeout -- 指定超时等待时间, 若timeout = NULL, 表示一直挂起直到对方完成消息处理
 * \return 成功返回true, 失败返回false
 * \note 不可以给自己发送同步消息
 *******************************************************************************/
extern bool send_sync_msg(const char * app_name, const msg_apps_t *msg, msg_reply_t * reply, struct timespec *timeout);

/******************************************************************************/
/*!
 * \par  Description:
 *    用于收到同步信息后，向对方返回回执信息
 * \ingroup applib_msg_loop
 * \param[in] reply -- 待返回的回执信息结构体指针
 * \return 成功返回true, 失败返回false
 *******************************************************************************/
extern bool reply_sync_msg(const msg_reply_t * reply);

/******************************************************************************/
/*!
 * \par  Description:
 *    给指定进程发送异步消息，完成后立即返回，无需等待对方完成消息处理
 * \ingroup applib_msg_loop
 * \param[in] app_name -- 目标进程名
 * \param[in] msg -- 待发送消息体结构指针
 * \return 成功返回true,  失败返回false
 *******************************************************************************/
extern bool send_async_msg(const char * app_name, const msg_apps_t *msg);

/******************************************************************************/
/*!
 * \par  Description:
 *    发送广播消息，完成后立即返回，无需等待对方完成消息处理
 * \ingroup applib_msg_loop
 * \param[in] msg -- 待发送消息体结构指针
 * \return 成功返回true, 失败返回false
 *******************************************************************************/
extern bool broadcast_msg(const msg_apps_t *msg);

/******************************************************************************/
/*!
 * \par  Description:
 *    抓住消息，阻止其他应用进程接受此消息
 * \ingroup applib_msg_loop
 * \param[in] msg_type -- 需要抓住的消息类型 
 * \return 成功返回true, 失败返回false
 * \note 此接口只会影响广播消息，目前最多只支持同时grab 32个消息
 *******************************************************************************/
extern bool grab_msg(msg_apps_type_e msg_type);

/******************************************************************************/
/*!
 * \par  Description:
 *    释放grab的消息，使得其他应用进程也可以接受此消息
 * \ingroup applib_msg_loop
 * \param[in] msg_type -- 需要释放的消息类型 
 * \return 成功返回true, 失败返回false
 *******************************************************************************/
extern bool release_msg(msg_apps_type_e msg_type);

/******************************************************************************/
/*!
 * \par  Description:
 *    过滤消息，使得本应用进程不再接受此消息
 * \ingroup applib_msg_loop
 * \param[in] msg_type -- 需要抓住的消息类型 
 * \return 成功返回true, 失败返回false
 * \note 此接口对同步、异步和广播消息均有效，目前最多只支持同时filter 32个消息
 *******************************************************************************/
extern bool filter_msg(msg_apps_type_e msg_type);

/******************************************************************************/
/*!
 * \par  Description:
 *    解除某个消息的过滤，使得本应用进程可以接受此消息
 * \ingroup applib_msg_loop
 * \param[in] msg_type -- 需要释放的消息类型 
 * \return 成功返回true, 失败返回false
 *******************************************************************************/
extern bool unfilter_msg(msg_apps_type_e msg_type);

/******************************************************************************/
/*!
 * \par  Description:
 *    过滤所有的系统消息，使得本应用进程不再接受任何系统消息
 * \ingroup applib_msg_loop
 * \return 成功返回true, 失败返回false
 * \note 此接口对同步、异步和广播消息均有效
 *******************************************************************************/
extern bool filter_all_msg(void);

/******************************************************************************/
/*!
 * \par  Description:
 *    解除所有系统消息的过滤，使得本应用进程可以接受所有系统消息
 * \ingroup applib_msg_loop
 * \return 成功返回true, 失败返回false
 *******************************************************************************/
extern bool unfilter_all_msg(void);

/******************************************************************************/
/*!
 * \par  Description:
 *    给指定全局消息队列发送异步消息
 * \ingroup applib_msg_loop
 * \param[in] mq_name -- 全局消息队列名 
 * \param[in] msg -- 待发送消息体结构指针
 * \return 成功返回true,  失败返回false
 *******************************************************************************/
extern bool send_async_msg_to_global_mq(const char * mq_name, const msg_apps_t *msg);

/******************************************************************************/
/*!
 * \par  Description:
 *    分派系统消息到应用进程注册的系统消息处理函数，如未注册则执行默认处理函数
 * \ingroup applib_msg_loop
 * \param[in] msg -- 系统消息结构体指针
 *******************************************************************************/
extern void dispatch_msg(msg_apps_t *msg);

/******************************************************************************/
/*!
 * \par  Description:
 *    注册应用进程当前的系统消息处理函数。
 * \ingroup applib_msg_loop
 * \param[in] proc -- 系统消息回调函数指针 
 * \return 成功返回true，失败返回false
 *******************************************************************************/
extern bool register_sys_dispatcher(sys_msg_proc proc);

/******************************************************************************/
/*!
 * \par  Description:
 *    获取应用进程当前的系统消息处理函数。
 * \ingroup applib_msg_loop
 * \return 成功返回当前进程注册的系统消息处理函数指针，失败返回NULL
 *******************************************************************************/
extern sys_msg_proc get_sys_dispatcher(void);

/******************************************************************************/
/*!
 * \par  Description:
 *    注销应用进程当前的系统消息处理函数。
 * \ingroup applib_msg_loop
 * \return 成功返回true，失败返回false
 *******************************************************************************/
extern bool unregister_sys_dispatcher(void);

/******************************************************************************/
/*!                    
 * \par  Description:
 *     退出消息循环  
 * \ingroup applib_msg_loop
 * \return void 
 * \note 调用此函数后，get_msg()会返回false, 从而退出while(get_msg())消息循环结构 
 *******************************************************************************/
extern void exit_msg_loop(void);

/******************************************************************************/
/*!                    
 * \par  Description:
 *     从系统消息队列中清除消息  
 * \ingroup applib_msg_loop
 * \param[in] sys_msg_type -- 系统消息类型， 如果为 -1 则清除所有的系统消息(同步消息除外) 
 * \return  成功返回true, 失败返回false      
 * \note 此函数只清除本应用的系统消息队列，且不能清除同步消息 
 * \par 例子
 * \code
 * clear_sys_msg_queue(-1);              //清空所有的系统消息
 * clear_sys_msg_queue(MSG_CARD_IN);     //清除卡插入的系统消息
 * \endcode
 *******************************************************************************/
extern bool clear_sys_msg_queue(int sys_msg_type);

/******************************************************************************/
/*!                    
 * \par  Description:
 *     从按键消息队列中清除按键消息  
 * \ingroup applib_msg_loop
 * \param[in] key_val -- 按键值，如果为-1，则清除所有的按键状态为key_type的按键消息 
 * \param[in] key_type -- 按键状态，如果为-1，则清除按键值为key_val的所有状态的消息 
 * \return  成功返回true, 失败返回false      
 * \par 例子
 * \code
 *  clear_key_msg_queue(-1, -1);                   // 清空所有的按键消息
 *  clear_key_msg_queue(KEY_PREV, -1);             // 清空按键值为KEY_PREV的所有消息
 *  clear_key_msg_queue(KEY_PREV, KEY_TYPE_HOLD);  // 清空按键值为KEY_PREV的所有hold状态消息
 *  clear_key_msg_queue(-1, KEY_TYPE_HOLD);        // 清空所有状态为hold的按键消息
 * \endcode
 *******************************************************************************/
extern bool clear_key_msg_queue(int key_val, int key_type);

/******************************************************************************/
/*!                    
 * \par  Description:
 *    将当前应用进程置入后台运行(console进程，不再接受GUI消息)
 * \ingroup applib_msg_loop
 * \return  成功返回true, 失败返回false      
 *******************************************************************************/
extern bool enter_background_mode(void);

/******************************************************************************/
/*!                    
 * \par  Description:
 *    将当前应用进程置入前台运行(GUI进程，可接受GUI消息)
 * \ingroup applib_msg_loop
 * \return  成功返回true, 失败返回false      
 * \note 将一个应用进程置为前台后，还需要重设本应用的窗口焦点，方能正确收到GUI消息 
 *******************************************************************************/
extern bool enter_foreground_mode(void);

/******************************************************************************/
/*!                    
 * \par  Description:
 *   创建全局消息队列 
 * \ingroup applib_msg_loop
 * \param[in] mq_name -- 欲创建的全局消息队列名 
 * \return  成功返回true, 失败返回false      
 *******************************************************************************/
extern bool create_global_msg_queue(const char * mq_name);

/******************************************************************************/
/*!                    
 * \par  Description:
 *   删除全局消息队列 
 * \ingroup applib_msg_loop
 * \param[in] mq_name -- 欲删除的全局消息队列名 
 * \return  成功返回true, 失败返回false      
 *******************************************************************************/
extern bool delete_global_msg_queue(const char * mq_name);

/******************************************************************************/
/*!                    
 * \par  Description:
 *   连接一个全局消息队列，使得当前消息循环可以接收此全局消息队列的消息 
 * \ingroup applib_msg_loop
 * \param[in] mq_name -- 欲连接的全局消息队列名 
 * \param[in] proc --    消息处理函数 
 * \return  成功返回true, 失败返回false      
 * \note 如果proc指定为NULL，将使用默认的系统消息处理函数，即register_sys_dispatcher()
 *       所注册的系统消息处理函数。
 *******************************************************************************/
extern bool connect_global_msg_queue(const char * mq_name, sys_msg_proc proc);

/******************************************************************************/
/*!                    
 * \par  Description:
 *   解除一个全局消息队列的连接，使得当前消息循环不再接收此全局消息队列的消息 
 * \ingroup applib_msg_loop
 * \param[in] mq_name -- 欲解除连接的全局消息队列名 
 * \return  成功返回true, 失败返回false      
 *******************************************************************************/
extern bool disconnect_global_msg_queue(const char * mq_name);

/******************************************************************************/
/*!                    
 * \par  Description:
 *   解除当前进程的所有全局消息队列连接 
 * \ingroup applib_msg_loop
 * \return  成功返回true, 失败返回false      
 *******************************************************************************/
extern bool disconnect_all_global_msg_queue(void);


#endif /* #ifndef __MSG_LOOP_H__ */
