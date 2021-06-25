/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : software_watchdog.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-3-24             v1.0             build this file 
 ********************************************************************************/

#include "applib_common.h"

static unsigned long soft_watchdog_timeout = 2000;
static int soft_watchdog_feed_timer_id = 0 ;


static int _feed_soft_watchdog(void * param);
static int _reboot_system(void *param);

void set_soft_watchdog_timeout(unsigned int timeout)
{
    app_info_t new_info = *g_this_app_info; 

    if(!is_applib_inited())
    {
        print_warning("set soft watchdog failed!");
        return ;
    }

    soft_watchdog_timeout = timeout;

    /* modify softwate watchdog timeout expired time */
    if(0 == timeout)
    {
        new_info.soft_watchdog_timeout_expires = 0;
        app_running_list_set_this_app_info(&new_info);
        kill_inner_timer(soft_watchdog_feed_timer_id);
    }
    else
    {
        new_info.soft_watchdog_timeout_expires = get_tick_time_ms() + soft_watchdog_timeout;
        app_running_list_set_this_app_info(&new_info);
        soft_watchdog_feed_timer_id = set_inner_timer(1000, _feed_soft_watchdog, NULL);
    }

}

bool  sys_is_soft_watchdog_forbid(void)
{
    app_running_list_t * list;

    if(!is_applib_inited())
    {
        return false;
    }

    list = (app_running_list_t *)app_running_list_get_list();

    for(; list != NULL; list = list->next)
    {
        if(true == list->app_info->soft_watchdog_forbidden)
        {
            return true ;
        }
    }
    return false;
}

void sys_forbid_soft_watchdog(bool forbid)
{
    msg_apps_t msg = {0};
    msg_reply_t reply = {0};
    bool last_stat;
    bool cur_stat;
    app_running_list_t * list;

    app_info_t new_info = *g_this_app_info;

    if(!is_applib_inited())
    {
        return ;
    }

    last_stat =sys_is_soft_watchdog_forbid();

    new_info.soft_watchdog_forbidden = forbid;
    app_running_list_set_this_app_info(&new_info);

    cur_stat = sys_is_soft_watchdog_forbid();

    if((last_stat == false) && (cur_stat == true))
    {
        /*
         * 看门狗由有效变为禁止时
         */
        msg.type = MSG_STOP_HARD_WATCHDG_SYNC;

        send_sync_msg(APP_NAME_MANAGER, &msg, &reply, NULL);

        if(reply.type != MSG_REPLY_SUCCESS)
        {
            print_warning("stop hard watchdog failed!");
        }
    }
    else if((last_stat == true) && (cur_stat == false))
    {
        /*
         * 看门狗由禁止变为有效时
         */
        /*
         * 1. 重设所有应用程序的soft watchdog
         */
        list = (app_running_list_t *)app_running_list_get_list();
        for(; list != NULL; list = list->next)
        {
            new_info = *(list->app_info);
            /*
             * 仅在各个进程的soft watchdog有效时，才进行重设
             */
            if(new_info.soft_watchdog_timeout_expires  != 0)
            {
                new_info.soft_watchdog_timeout_expires = get_tick_time_ms() + soft_watchdog_timeout;
                app_running_list_set_app_info(new_info.name, &new_info);
                print_err(" now reset %s 's soft_watchdog timer. new expires=%d",
                            new_info.name,new_info.soft_watchdog_timeout_expires);
            }
        }

        /*
         * 2. 通知manager重设hard watchdog
         */
        msg.type = MSG_RESTART_HARD_WATCHDG_SYNC;

        send_sync_msg(APP_NAME_MANAGER, &msg, &reply, NULL);

        if(reply.type != MSG_REPLY_SUCCESS)
        {
            print_warning("restart hard watchdog failed!");
        }
    }
    else
    {
        /*
         * 看门狗当前设置未发生改变。
         */
    }
}

void check_soft_watchdogs(void)
{
    app_running_list_t * list;
    app_running_list_t * list_tmp;

    msg_apps_t msg = {0};

    if(!is_applib_inited())
    {
        print_warning("check soft watchdogs failed!");
        return ;
    }

    list = (app_running_list_t *)app_running_list_get_list();
    list_tmp = list;

    for(; list_tmp != NULL; list_tmp = list_tmp->next)
    {
        if(list_tmp->app_info->soft_watchdog_forbidden == true)
        {
            return;
        }
    }

    for(; list != NULL; list = list->next)
    {

        if(strcmp(list->app_info->name, basename(APP_NAME_MANAGER)) == 0)
        {
            continue;
        }

        if( (list->app_info->soft_watchdog_timeout_expires > 0 )
                && (get_tick_time_ms() >= list->app_info->soft_watchdog_timeout_expires) )
        {
            printf("\n##### software watchdog timeout #####\n");
            printf("%s(%d) not responsed!   it's expire is %d, now is %d\n",
                    list->app_info->name, list->app_info->pid,
                    list->app_info->soft_watchdog_timeout_expires,get_tick_time_ms());

#if 0
            if( (strcmp(list->app_info->name, basename(APP_NAME_CONFIG)) == 0 )
                || (strcmp(list->app_info->name, basename(APP_NAME_CONFIG)) == 0 )
                || (strcmp(list->app_info->name, basename(APP_NAME_MSG_SERV)) == 0 ) )
            {
                printf("reboot system ...\n");
                _reboot_system(NULL);
            }
#else 
            printf("reboot system ...\n");
            _reboot_system(NULL);
#endif
            printf("kill %d\n\n", list->app_info->pid);
            if(kill(list->app_info->pid, 9) < 0) // SIGKILL is 9
            {
                print_err("kill %d failed!", list->app_info->pid);
            }

            if( (strcmp(list->app_info->name, basename(APP_NAME_LAUNCHER)) == 0 ))
            {
                printf("restart launcher ...\n");
                msg.type = MSG_CREAT_APP;
                strcpy(msg.content, APP_NAME_LAUNCHER);
                send_async_msg(APP_NAME_MANAGER, &msg);
            }
        }
    }
}

static int _feed_soft_watchdog(void * param)
{
    /* modify softwate watchdog timeout expired time */

    app_info_t new_info = *g_this_app_info;
    new_info.soft_watchdog_timeout_expires = get_tick_time_ms() + soft_watchdog_timeout;
    app_running_list_set_this_app_info(&new_info);
    return 0;
}

#pragma __PRQA_IGNORE_START__
/* 特意制造死循环，实现watchdog重启系统的目的, 屏蔽QAC检查 */

static int _reboot_system(void *param)
{
    int wd_fd = -1;
    int ret;
    int time_out = 1000;  /*1秒*/
    bool loop = true;

    wd_fd = open( DEVICE_WD, O_RDWR, 0);
    if( wd_fd < 0 )
    {
        print_err("Error: open watchdog device\n\n");
    }

    /* 设置后，1秒内若没有再调用此函数，则会重启*/
    ret = ioctl(wd_fd, WDD_CMD_SET_TIME, time_out);

    while(loop == true)
    {
        loop = true;
    }

    close(wd_fd);

    return 0;
}

#pragma __PRQA_IGNORE_END__
