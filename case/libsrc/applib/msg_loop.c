/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : msg_loop.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-3-24             v1.0             build this file
 ********************************************************************************/


#include <include_case.h>
#include "applib_common.h"
#include "applib_config.h"

#define USE_SYS_CALL
#define MQ_MAXMSG    (5)
#define MQ_MSGSIZE   (sizeof(private_msg_t)+2)
#define GLOBAL_MQ_MANAGE_SHM_NAME    "/shm/global_mq_manage"

/*!
 * \brief
 * Open the message queue for receive messages
 */
static bool _open_msg_queue(void);

/*!
 * \brief
 * Close the message queue
 */
static bool _close_msg_queue(void);

/*!
 * \brief
 * Initialize synchronous message reply shared memory 
 */
static bool _init_sync_msg_reply_shm(void);

/*!
 * \brief
 * Release synchronous message reply shared memory 
 */
static bool _deinit_sync_msg_reply_shm(void);

/*!
 * \brief
 * Initialize global msg queue manage shared memory data 
 */
static bool _init_global_mq_manage_shm(void);

/*!
 * \brief
 * Release global msg queue manage shared memory data 
 */
static bool _deinit_global_mq_manage_shm(void);

/*!
 * \brief
 *  process global msg queue 
 */
static void _process_global_msg_queue( void );

/*!
 * \brief
 */
static bool _is_connected_to_global_msg_queue(const char * mq_name);

/*!
 * \brief
 */
static bool _is_global_msg_queue_created(const char * mq_name);

/*!
 * \brief
 * Global msg queue info 
 */
typedef struct
{
    char mq_name[MAX_NAME_LEN];
    //int ref_cnt; 
}global_mq_t;

/*!
 * \brief
 * Shared memory data structure for global msg queue manage
 */
typedef struct
{
    unsigned char used_map[MAX_GLOBAL_MQ];
    global_mq_t mq[MAX_GLOBAL_MQ];
}global_mq_shm_t;

/*!
 * \brief
 * Private message structure
 */
typedef struct
{
    bool sync;
    char sem_name[MAX_NAME_LEN];
    char shm_name[MAX_NAME_LEN];
    msg_apps_t apps_msg;
}private_msg_t;

/*!
 * \brief
 * Private synhronous message reply shared memory structure 
 */
typedef struct
{
    char shm_name[MAX_NAME_LEN];
    int shm_fd;
    msg_reply_t * shm_addr;
}sync_msg_reply_shm_t;

static sync_msg_reply_shm_t s_sync_msg_reply_shm ;

static sys_msg_proc  s_sys_proc = NULL;

static private_msg_t s_cur_received_pmsg;

static bool s_need_deal_sync_msg = false;
static bool s_do_msg_loop = true;

static global_mq_shm_t * s_global_mq_shm = NULL;
static int s_global_mq_shm_fd = -1;

static sem_t thread_mutex; 

bool applib_register_app(const char * app_name)
{
    int i;
    app_info_t app_info = {0};
    struct mq_attr mqattr;
    mqd_t mqd;

    print_dbg();

    if(NULL == app_name)
    {
        print_warning("input parameter(app_name) is NULL pointer!");
        return false;
    }

    app_running_list_init();

    if(true == is_app_exist(app_name))
    {
        print_warning("%s is already in app running list!", app_name);
        return false;
    }

    app_info.pid = -1;
    strncpy(app_info.name, app_name, sizeof(app_info.name));
    sprintf(app_info.mq_name, "/proc/%s/mq", app_info.name);
    app_info.mqdes = (mqd_t)-1;
    app_info.state = APP_STATE_INACTIVE;
    app_info.soft_watchdog_forbidden = false;
    app_info.soft_watchdog_timeout_expires = 0;
    app_info.screensaver_forbidden = false;
    app_info.auto_standby_forbidden = false;
    app_info.timed_shutdown_forbidden = false;
    app_info.timer_valid = true;

    memset(app_info.msg_grabed, -1, sizeof(app_info.msg_grabed));
    memset(app_info.msg_filtered, -1, sizeof(app_info.msg_filtered));

    for(i = 0; i < MAX_GLOBAL_MQ; i++)
    {
        memset(app_info.global_mq[i].mq_name, 0, sizeof(app_info.global_mq[i].mq_name));
        app_info.global_mq[i].mqdes = (mqd_t)-1;
        app_info.global_mq[i].proc = NULL;
    }

    app_info.filter_all_msg = false;

    /* creat message queue for app_name */
    mqattr.mq_flags = 0;
    mqattr.mq_maxmsg =  MQ_MAXMSG;
    mqattr.mq_msgsize = MQ_MSGSIZE;
    mqattr.mq_curmsgs = 0;

    print_dbg("creat message queue: %s", app_info.mq_name);

    mqd = mq_open(app_info.mq_name, O_CREAT | O_EXCL, 0666, &(mqattr));
    if((mqd_t)-1 == mqd)
    {
        print_err("creat msg queue %s failed!", app_info.mq_name);
        return false;
    }

    print_dbg("close msg queue: %s",app_info.mq_name);

    if(mq_close(mqd) < 0)
    {
        print_err("close msg queue %s failed!", app_info.mq_name);
        return false;
    }

    app_running_list_add(&app_info);
    return true;
}

bool applib_unregister_app(const char * app_name)
{
    const app_info_t * app_info = NULL;
    mqd_t mqd;
    struct mq_attr attr;
    int i;
    private_msg_t msg;
    unsigned msg_prio;

    int shm_fd;
    sem_t *sem;
    msg_reply_t * shm_addr = NULL;
    msg_reply_t reply = {0};

    print_dbg();

    if(NULL == app_name)
    {
        print_warning("input parameter(app_name) is NULL pointer!");
        return false;
    }

    app_info = app_running_list_get_app_info(app_name);

    if(NULL == app_info)
    {
        return false;
    }

    mqd = mq_open(app_info->mq_name, O_RDWR | O_NONBLOCK);
    if((mqd_t)-1 == mqd)
    {
        print_err("open msg queue %s failed!", app_info->mq_name);
    }
    else
    {
        mq_getattr(mqd, &attr); 
        for(i = 0; i < attr.mq_curmsgs; i++)
        {
            mq_receive(mqd, (char *)&msg, sizeof(msg), &msg_prio);
            /* 处理应用app_name 未处理的同步消息 */
            if(msg.sync == true)
            {
                reply.type = MSG_REPLY_NO_REPLY;

                print_dbg("open shared memory: %s", msg.shm_name);
                shm_fd = shm_open(msg.shm_name, O_RDWR, 0666);
                if(shm_fd < 0)
                {
                    print_err("open shared memory %s failed", msg.shm_name);
                    goto post_sem;
                }

#ifdef USE_SYS_CALL
                shm_addr = (msg_reply_t *)sys_mmap((unsigned long)NULL, sizeof(msg_reply_t), PROT_READ | PROT_WRITE, \
                        MAP_SHARED, shm_fd, 0);
#else
                shm_addr = (msg_reply_t *)mmap(NULL, sizeof(msg_reply_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
#endif

                if((void *)-1 == (void *)shm_addr)
                {
                    print_err("map to %s failed!", msg.shm_name);
                    close(shm_fd);
                    goto post_sem;
                }

                memcpy(shm_addr, &reply, sizeof(msg_reply_t));
                close(shm_fd);

                /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< goto! <<<<<<<<<<<<<<<< goto! <<<<<<<< goto! <<<< goto! << */
                post_sem:

                print_dbg("open named semaphore: %s", msg.sem_name);
                sem = sem_open(msg.sem_name, 0);
                if(sem != SEM_FAILED)
                {
                    print_dbg("post named semaphore: %s ", msg.sem_name);
                    sem_post(sem);
                    sem_close(sem);
                }
                else
                {
                    print_err("opened named semaphore: %s failed!", msg.sem_name);
                } 
            }

        }

        mq_close(mqd);
    }

    print_dbg("unlink %s", app_info->mq_name);
    if(mq_unlink(app_info->mq_name) < 0)
    {
        print_err("unlink msg queue %s failed!", app_info->mq_name);

        app_running_list_delete(app_name);
        return false;
    }

    app_running_list_delete(app_name);

    return true;
}

bool applib_init(int argc, const char *argv[], app_type_e type)
{
    unsigned int value = 0;
    const app_info_t * cur_app_info;
    const app_running_list_t *  app_running_list;
    app_info_t new_info;
    void * p_apconfig = NULL;

    if(true == g_applib_inited)
    {
        print_warning("applib has initialized already!");
        return false;
    }

    if(argc < 1)
    {
        print_warning("must input a app name to applib_init() via argc and argv[]");
        print_warning("applib initialized failed!");
        return false;
    }

    app_running_list_init();
    app_running_list  = app_running_list_get_list();
    if(app_running_list == NULL)
    {
        applib_register_app(basename(argv[0]));
    }

    cur_app_info = app_running_list_get_app_info(basename(argv[0]));

    if(NULL == cur_app_info)
    {
        goto get_cur_app_info_err;
    }

    /* fill in pid and app type */
    new_info = *cur_app_info;
    new_info.pid = getpid();
    new_info.type = type;
    app_running_list_set_app_info(cur_app_info->name, &new_info);

    g_this_app_info = app_running_list_get_this_app_info();

    if(_open_msg_queue() == false)
    {
        goto open_msg_queue_err;
    }

    if(_init_sync_msg_reply_shm() == false)
    {
        goto init_sync_msg_reply_shm_err;
    }

    if(_init_global_mq_manage_shm() == false)
    {
        goto init_global_mq_manage_err;
    }

    if(applib_dir_cfg_init() == false)
    {
        goto applib_dir_cfg_init_err;
    }

    //print_app_running_list();
    g_applib_inited = true;

    p_apconfig = dlopen("apconfig.so", RTLD_LAZY);

    if (p_apconfig == NULL)
    {
        print_err("load apconfig failed");
    }

    if(p_apconfig != NULL) 
    {
        if(get_config(CFG_DEF_WATCHDOG_TIME, (char *)&value, sizeof(value)) < 0)
        {
            print_err("get_config(CFG_DEF_WATCHDOG_TIME) failed!");
            value = 5000;
        }

        dlclose(p_apconfig);
    }
    else
    {
        value = 5000;
    }

    if(value > 0)
    {
        set_soft_watchdog_timeout(value);
    }

    sem_init(&thread_mutex, PTHREAD_PROCESS_PRIVATE , 1);     

    return true;

    /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< goto! <<<<<<<<<<<<<<<< goto! <<<<<<<< goto! <<<< goto! << */
    applib_dir_cfg_init_err:
    _deinit_global_mq_manage_shm();

    /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< goto! <<<<<<<<<<<<<<<< goto! <<<<<<<< goto! <<<< goto! << */
    init_global_mq_manage_err:
    _deinit_sync_msg_reply_shm();

    /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< goto! <<<<<<<<<<<<<<<< goto! <<<<<<<< goto! <<<< goto! << */
    init_sync_msg_reply_shm_err:
    _close_msg_queue();    

    /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< goto! <<<<<<<<<<<<<<<< goto! <<<<<<<< goto! <<<< goto! << */
    open_msg_queue_err:

    /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< goto! <<<<<<<<<<<<<<<< goto! <<<<<<<< goto! <<<< goto! << */
    get_cur_app_info_err:
    app_running_list_deinit();

    print_err("applib init failed!");
    return false;

}

bool applib_quit(void)
{
    print_dbg();

    if(false == g_applib_inited)
    {
        print_warning("applib has not initialized!");
        return false;
    }

    _close_msg_queue();

    disconnect_all_global_msg_queue();

    unregister_sys_dispatcher();

    kill_timers();
    kill_inner_timers();
    delete_timers();
    delete_inner_timers();

    sys_close_rtc_dev();

    app_running_list_deinit();
    g_this_app_info = NULL;

    _deinit_sync_msg_reply_shm();
    sem_destroy(&thread_mutex);     

    applib_dir_cfg_deinit();
    _deinit_global_mq_manage_shm();

    g_applib_inited = false;

    return true;
}

bool get_msg(msg_apps_t *msg)
{
    int ret;
    unsigned msg_prio;

    if(!is_applib_inited())
    {
        return false;
    }

    while(1)
    {
        if(s_do_msg_loop == false)
        {
            /* exit message loop */
            s_do_msg_loop = true;

            return false;
        }

        if(APP_TYPE_GUI == g_this_app_info->type)
        {
            gui_exec();
        }

        _process_global_msg_queue();

        handle_timers();

        if(s_sys_proc == NULL)
        {
            usleep(20000);
            continue;
        }

        ret = (int)mq_receive(g_this_app_info->mqdes, (char *)&s_cur_received_pmsg, \
                sizeof(private_msg_t), &msg_prio);

        if(ret < 0)
        {
            if(EAGAIN != errno)
            {
                print_err("receive msg error!");
                print_err("mq name:%s, mqd:%d", g_this_app_info->mq_name, g_this_app_info->mqdes);
            }

            usleep(20000);
        }
        else
        {
            memcpy(msg, &(s_cur_received_pmsg.apps_msg), sizeof(msg_apps_t));

            if(s_cur_received_pmsg.sync == true)
            {
                s_need_deal_sync_msg = true;
            }

            return true;
        }
    }
}

static void _process_global_msg_queue( void )
{
    int ret;
    int i;
    unsigned msg_prio;
    msg_apps_t msg = {0};

    /* read connected global msg queue */
    for(i = 0; i < MAX_GLOBAL_MQ; i++)
    {
        if((g_this_app_info->global_mq[i].mqdes != (mqd_t)-1)
                && ((g_this_app_info->global_mq[i].proc != NULL) || (s_sys_proc != NULL) ))
        {
            ret = (int)mq_receive(g_this_app_info->global_mq[i].mqdes, (char *)&s_cur_received_pmsg, \
                    sizeof(private_msg_t), &msg_prio);

            if(ret < 0)
            {
                if(EAGAIN != errno)
                {
                    print_err("receive msg error!");
                    print_err("mq name:%s, mqd:%d", g_this_app_info->global_mq[i].mq_name, \
                            g_this_app_info->global_mq[i].mq_name);
                }
            }
            else
            {
                memcpy(&msg, &(s_cur_received_pmsg.apps_msg), sizeof(msg_apps_t));

                if(s_cur_received_pmsg.sync == true)
                {
                    s_need_deal_sync_msg = true;
                }

                print_dbg("receive: %d", msg.type);

                if(g_this_app_info->global_mq[i].proc != NULL)
                {
                    g_this_app_info->global_mq[i].proc(&msg);
                }
                else
                {
                    s_sys_proc(&msg);
                }
            }
        }
    }
}

bool send_sync_msg(const char * app_name, const msg_apps_t *msg, msg_reply_t * reply, struct timespec *timeout)
{
    int i;
    mqd_t mqd;
    const app_info_t * target_app_info;
    sem_t *sem;
    bool retvalue = true;
    char * base_app_name = NULL;
    private_msg_t cur_send_pmsg = {0};

    print_dbg("send sync msg to %s, msg type:%d",app_name, msg->type);

    if(!is_applib_inited())
    {
        return false;
    }

    sem_wait(&thread_mutex);

    if(NULL == app_name)
    {
        print_warning("input parameter(app_name) is a NULL pointer!");

        sem_post(&thread_mutex);
        return false;
    }

    base_app_name = basename(app_name);

    if(strcmp(g_this_app_info->name, base_app_name) == 0)
    {
        print_warning("can not send sync msg to self!");
        errno = EDEADLK;

        sem_post(&thread_mutex);
        return false;
    }

    target_app_info = app_running_list_get_app_info(base_app_name);
    if(NULL == target_app_info)
    {
        sem_post(&thread_mutex);
        return false;
    }

    if(target_app_info->filter_all_msg == true)
    {
        print_warning("%s filtered all msg!", base_app_name);
        sem_post(&thread_mutex);
        return false;
    }

    for(i = 0; i < MAX_FILTER_MSG; i++)
    {
        if(target_app_info->msg_filtered[i] == msg->type)
        {
            print_warning("%s filtered msg :%d!", base_app_name, msg->type);
            sem_post(&thread_mutex);
            return false;
        }
    }

    memcpy(&(cur_send_pmsg.apps_msg), msg, sizeof(msg_apps_t));

    /* fill in sender pid */
    cur_send_pmsg.apps_msg.sender_pid = getpid();

    cur_send_pmsg.sync = true;

    sprintf(cur_send_pmsg.sem_name, "/proc/%s/sync_sem_%d", g_this_app_info->name, msg->type);

    print_dbg("open msg queue: %s", target_app_info->mq_name);
    mqd = mq_open(target_app_info->mq_name, O_WRONLY | O_NONBLOCK);

    if((mqd_t)-1 == mqd)
    {
        print_err("open msg queue %s failed!", target_app_info->mq_name);

        sem_post(&thread_mutex);
        return false;
    }

    print_dbg("creat named semaphore: %s", cur_send_pmsg.sem_name);
    sem = sem_open(cur_send_pmsg.sem_name, O_CREAT | O_EXCL, 0666, 0);
    if(SEM_FAILED == sem)
    {
        print_err("create semaphore %s failed!", cur_send_pmsg.sem_name);

        print_dbg("close msg queue: %s",target_app_info->mq_name);
        mq_close(mqd);

        sem_post(&thread_mutex);
        return false;
    }

    strncpy(cur_send_pmsg.shm_name, s_sync_msg_reply_shm.shm_name, 32);

    if(mq_send(mqd, (const char *)&cur_send_pmsg, sizeof(cur_send_pmsg), MQ_PRIO_MAX - 2) < 0)
    {
        print_err("send sync msg to %s failed!",base_app_name);
        print_dbg("close msg queue: %s",target_app_info->mq_name);
        mq_close(mqd);

        print_dbg("unlink named semaphore : %s", cur_send_pmsg.sem_name);
        sem_close(sem);
        sem_unlink(cur_send_pmsg.sem_name);

        sem_post(&thread_mutex);
        return false;
    }

    if(timeout != NULL)
    {
        print_dbg("wait named semaphore : %s with timeout ", cur_send_pmsg.sem_name);
        if(sem_timedwait(sem, timeout) < 0)
        {
            if(ETIMEDOUT == errno)
            {
                print_err("%d,time out!", msg->type);
            }
            else
            {
                print_err("occurs on sem_timedwait()!");
            }

            retvalue = false;
        }
    }
    else
    {
        print_dbg("wait named semaphore: %s ", cur_send_pmsg.sem_name);
        if(sem_wait(sem) < 0)
        {
            print_err("occurs on sem_wait()!");

            retvalue = false;
        }
    }

    if(retvalue && reply)
    {
        memcpy(reply, s_sync_msg_reply_shm.shm_addr, sizeof(msg_reply_t));
    }

    print_dbg("close msg queue: %s",target_app_info->mq_name);
    mq_close(mqd);

    print_dbg("unlink named semaphore : %s", cur_send_pmsg.sem_name);
    sem_close(sem);
    sem_unlink(cur_send_pmsg.sem_name);

    sem_post(&thread_mutex);
    return retvalue;
}

bool send_async_msg(const char * app_name, const msg_apps_t *msg)
{
    int i;
    mqd_t mqd;
    const app_info_t * target_app_info;
    char * base_app_name = NULL;
    unsigned int prio = 0;
    private_msg_t cur_send_pmsg = {0};

    print_dbg("send async msg to %s, msg type:%d",app_name, msg->type);

    if(!is_applib_inited())
    {
        return false;
    }

    base_app_name =  basename(app_name);

    target_app_info = app_running_list_get_app_info(base_app_name);
    if(target_app_info == NULL)
    {
        return false;
    }

    if(target_app_info->filter_all_msg == true)
    {
        print_warning("%s filtered all msg!", base_app_name);
        return false;
    }

    for(i = 0; i < MAX_FILTER_MSG; i++)
    {
        if(target_app_info->msg_filtered[i] == msg->type)
        {
            print_warning("%s filtered msg :%d!", base_app_name, msg->type);
            return false;
        }
    }

    if(msg->type == MSG_APP_QUIT)
    {
        prio = MQ_PRIO_MAX;
    }

    memcpy(&(cur_send_pmsg.apps_msg), msg, sizeof(msg_apps_t));

    /* fill in sender pid */
    cur_send_pmsg.apps_msg.sender_pid = getpid();

    cur_send_pmsg.sync = false;

    print_dbg("open msg queue: %s", target_app_info->mq_name);
    mqd = mq_open(target_app_info->mq_name, O_WRONLY | O_NONBLOCK);
    if((mqd_t)-1 == mqd)
    {
        print_err("open msg queue %s failed!", target_app_info->mq_name);

        return false;
    }

    if(mq_send(mqd, (const char *)&cur_send_pmsg, sizeof(cur_send_pmsg), prio) < 0)
    {
        print_err("send async msg to %s failed!", base_app_name);
        print_dbg("close msg queue: %s",target_app_info->mq_name);
        mq_close(mqd);

        return false;
    }

    print_dbg("close msg queue: %s",target_app_info->mq_name);
    mq_close(mqd);

    return true;
}

bool broadcast_msg(const msg_apps_t *msg)
{
    app_running_list_t * list;
    const app_info_t * grab_msg_owner = NULL;
    int i;

    print_dbg();

    if(!is_applib_inited())
    {
        return false;
    }

    list = (app_running_list_t *)app_running_list_get_list();
    for(; list != NULL; list = list->next)
    {
        for(i = 0; i < MAX_GRAB_MSG; i++)
        {
            if(list->app_info->msg_grabed[i] == msg->type)
            {
                grab_msg_owner = list->app_info;
                break;
            }
        }
    }

    if(grab_msg_owner != NULL)
    {
        send_async_msg(grab_msg_owner->name, msg);
    }
    else
    {
        list = (app_running_list_t *)app_running_list_get_list();
        for(; list != NULL; list = list->next)
        {
            send_async_msg(list->app_info->name, msg);
        }
    }

    return true;
}

bool grab_msg(msg_apps_type_e msg_type)
{
    int i;
    app_info_t app_info;

    print_dbg();

    if(!is_applib_inited())
    {
        return false;
    }

    release_msg(msg_type);

    for(i = 0; i < MAX_GRAB_MSG; i++)
    {
        if(g_this_app_info->msg_grabed[i] == -1)
        {
            app_info = *g_this_app_info;
            app_info.msg_grabed[i] = msg_type;
            app_running_list_set_app_info(app_info.name, &app_info);
            return true;
        }
    }

    return false;
}

bool release_msg(msg_apps_type_e msg_type)
{
    app_running_list_t * list;
    int i;
    app_info_t app_info;

    print_dbg();

    if(!is_applib_inited())
    {
        return false;
    }

    list = (app_running_list_t *)app_running_list_get_list();
    for(; list != NULL; list = list->next)
    {
        for(i = 0; i < MAX_GRAB_MSG; i++)
        {
            if(list->app_info->msg_grabed[i] == msg_type)
            {
                app_info = *list->app_info;
                app_info.msg_grabed[i] = -1;
                app_running_list_set_app_info(app_info.name, &app_info);
            }
        }
    }

    return true;
}

bool filter_msg(msg_apps_type_e msg_type)
{
    int i;
    app_info_t app_info;

    print_dbg();

    if(!is_applib_inited())
    {
        return false;
    }

    unfilter_msg(msg_type);

    for(i = 0; i < MAX_FILTER_MSG; i++)
    {
        if(g_this_app_info->msg_filtered[i] == -1)
        {
            app_info = *g_this_app_info;
            app_info.msg_filtered[i] = msg_type;
            app_running_list_set_app_info(app_info.name, &app_info);
            return true;
        }
    }

    return false;
}

bool unfilter_msg(msg_apps_type_e msg_type)
{
    int i;
    app_info_t app_info;

    print_dbg();

    if(!is_applib_inited())
    {
        return false;
    }

    for(i = 0; i < MAX_FILTER_MSG; i++)
    {
        if(g_this_app_info->msg_filtered[i] == msg_type)
        {
            app_info = *g_this_app_info;
            app_info.msg_filtered[i] = -1;
            app_running_list_set_app_info(app_info.name, &app_info);
            return true;
        }
    }

    return false;
}

bool filter_all_msg(void)
{
    app_info_t app_info;

    print_dbg();

    if(!is_applib_inited())
    {
        return false;
    }

    app_info = *g_this_app_info;
    app_info.filter_all_msg  = true;
    app_running_list_set_app_info(app_info.name, &app_info);

    return true;
}

bool unfilter_all_msg(void)
{
    app_info_t app_info;

    print_dbg();

    if(!is_applib_inited())
    {
        return false;
    }

    app_info = *g_this_app_info;
    app_info.filter_all_msg  = false;
    memset(app_info.msg_filtered, -1, sizeof(app_info.msg_filtered));
    app_running_list_set_app_info(app_info.name, &app_info);
    
    return true;
}

bool send_async_msg_to_global_mq(const char * mq_name, const msg_apps_t *msg)
{
    mqd_t mqd;
    unsigned int prio = 0;
    private_msg_t cur_send_pmsg = {0};

    print_dbg("send async msg to %s, msg type:%d", mq_name, msg->type);

    if(!is_applib_inited())
    {
        return false;
    }

    if(_is_global_msg_queue_created(mq_name) == false)
    {
        print_err("%s has not created!", mq_name);
        return false;
    }

    if(msg->type == MSG_APP_QUIT)
    {
        prio = MQ_PRIO_MAX;
    }

    memcpy(&(cur_send_pmsg.apps_msg), msg, sizeof(msg_apps_t));

    /* fill in sender pid */
    cur_send_pmsg.apps_msg.sender_pid = getpid();
    cur_send_pmsg.sync = false;

    print_dbg("open msg queue: %s", mq_name);
    mqd = mq_open(mq_name, O_WRONLY | O_NONBLOCK);
    if((mqd_t)-1 == mqd)
    {
        print_err("open msg queue %s failed!", mq_name);
        return false;
    }

    if(mq_send(mqd, (const char *)&cur_send_pmsg, sizeof(cur_send_pmsg), prio) < 0)
    {
        print_err("send async msg to %s failed!", mq_name);
        print_dbg("close msg queue: %s", mq_name);
        mq_close(mqd);

        return false;
    }

    print_dbg("close msg queue: %s", mq_name);
    mq_close(mqd);

    return true;
}

void dispatch_msg(msg_apps_t *msg)
{
    print_dbg();

    if(!is_applib_inited())
    {
        return ;
    }

    if(s_sys_proc != NULL)
    {
        (*s_sys_proc)(msg);
    }

    if(s_need_deal_sync_msg == true)
    {
        msg_reply_t reply;
        reply.type = MSG_REPLY_NO_REPLY;
        reply_sync_msg(&reply);
    }
}

bool reply_sync_msg(const msg_reply_t * reply)
{
    sem_t *sem;

    print_dbg();

    if(!is_applib_inited())
    {
        return false;
    }

    if(s_need_deal_sync_msg == false)
    {
        print_warning("current msg is not a sync msg");

        return false;
    }

    print_dbg("open named semaphore: %s", s_cur_received_pmsg.sem_name);

    sem = sem_open(s_cur_received_pmsg.sem_name, 0);
    if(sem != SEM_FAILED)
    {
        print_dbg("post named semaphore: %s ", s_cur_received_pmsg.sem_name);

        if(reply != NULL)
        {
            int shm_fd;
            msg_reply_t * shm_addr = NULL;

            print_dbg("open shared memory: %s", s_cur_received_pmsg.shm_name);
            shm_fd = shm_open(s_cur_received_pmsg.shm_name, O_RDWR, 0666);
            if(shm_fd < 0)
            {
                print_err("open shared memory %s failed", s_cur_received_pmsg.shm_name);

                sem_post(sem);
                sem_close(sem);
                s_need_deal_sync_msg = false;
                return false;
            }

#ifdef USE_SYS_CALL
            shm_addr = (msg_reply_t *)sys_mmap((unsigned long)NULL, sizeof(msg_reply_t), PROT_READ | PROT_WRITE, \
                    MAP_SHARED, shm_fd, 0);
#else
            shm_addr = (msg_reply_t *)mmap(NULL, sizeof(msg_reply_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
#endif

            if((void *)-1 == (void *)shm_addr)
            {
                print_err("map to %s failed!", s_cur_received_pmsg.shm_name);
                close(shm_fd);

                sem_post(sem);
                sem_close(sem);
                s_need_deal_sync_msg = false;
                return false;
            }

            memcpy(shm_addr, reply, sizeof(msg_reply_t));
            close(shm_fd);
        }

        sem_post(sem);
        sem_close(sem);
    }
    else
    {
        print_err("opened named semaphore: %s failed!", s_cur_received_pmsg.sem_name);
    }

    s_need_deal_sync_msg = false;

    return true;
}

bool register_sys_dispatcher(sys_msg_proc proc)
{
    print_dbg();

    if(!is_applib_inited())
    {
        return false;
    }

    s_sys_proc = proc;

    return true;
}

sys_msg_proc get_sys_dispatcher( void )
{
    print_dbg();

    if(!is_applib_inited())
    {
        return NULL;
    }

    return s_sys_proc;
}

bool unregister_sys_dispatcher(void)
{
    print_dbg();

    if(!is_applib_inited())
    {
        return false;
    }

    s_sys_proc = NULL;

    return true;
}

void exit_msg_loop(void)
{
    if(!is_applib_inited())
    {
        return ;
    }

    if(APP_TYPE_GUI == g_this_app_info->type)
    {
        gui_screen_update();
    }

    s_do_msg_loop = false;
}

static bool _open_msg_queue(void)
{
    mqd_t mqd;
    app_info_t new_info;

    print_dbg("open msg queue: %s", g_this_app_info->mq_name);
    mqd = mq_open(g_this_app_info->mq_name, O_RDONLY | O_NONBLOCK);

    if((mqd_t)-1 == mqd)
    {
        print_err("open msg queue %s failed!", g_this_app_info->mq_name);
        return false;
    }

    /* modify curent app info */
    new_info = *g_this_app_info;
    new_info.mqdes = mqd;
    app_running_list_set_this_app_info(&new_info);

    return true;
}

static bool _close_msg_queue(void)
{
    app_info_t new_info = *g_this_app_info;

    print_dbg();

    if(g_this_app_info->mqdes != (mqd_t)-1)
    {
        print_dbg("close msg queue: %s",g_this_app_info->mq_name);
        if(mq_close(g_this_app_info->mqdes) < 0)
        {
            print_err("close msg queue %s failed!", g_this_app_info->mq_name);
            return false;
        }

        /* modify curent app info */
        new_info.mqdes = (mqd_t)-1;
        app_running_list_set_this_app_info(&new_info);

        return true;
    }

    return false;
}


static bool _init_sync_msg_reply_shm(void)
{
    int shm_fd;
    void * shm_addr = NULL;

    print_dbg();
    sprintf(s_sync_msg_reply_shm.shm_name, "/proc/%s/sync_shm", g_this_app_info->name);

    print_dbg("open shared memory: %s", s_sync_msg_reply_shm.shm_name);

    shm_fd = shm_open(s_sync_msg_reply_shm.shm_name, O_RDWR, 0666);
    if(shm_fd < 0)
    {
        /*
           if(ENOENT != errno)
           {
           print_err("critical error! errno:%d",errno);
           return false;
           }
           */

        print_info("create shared memory: %s",s_sync_msg_reply_shm.shm_name);
        shm_fd = shm_open(s_sync_msg_reply_shm.shm_name, O_RDWR | O_CREAT, 0666);
        if(-1 == shm_fd)
        {
            print_err("create shared memory %s failed!", s_sync_msg_reply_shm.shm_name);
            return false;
        }

#ifdef USE_SYS_CALL
        sys_ftruncate((unsigned int)shm_fd, sizeof(msg_reply_t)); 
#else
        ftruncate(shm_fd, sizeof(msg_reply_t)); 
#endif
    }

#ifdef USE_SYS_CALL
    shm_addr = (msg_reply_t *)sys_mmap((unsigned long)NULL, sizeof(msg_reply_t), \
            PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
#else
    shm_addr = (msg_reply_t *)mmap(NULL, sizeof(msg_reply_t), \
            PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
#endif


    if((void *)-1 == shm_addr)
    {
        print_err("map to %s failed!", s_sync_msg_reply_shm.shm_name);
        close(shm_fd);
        return false;
    }

    s_sync_msg_reply_shm.shm_addr = shm_addr;
    s_sync_msg_reply_shm.shm_fd = shm_fd;

    return true;
}


static bool _deinit_sync_msg_reply_shm(void)
{
    print_dbg();

    if(s_sync_msg_reply_shm.shm_fd > 0)
    {
#ifdef USE_SYS_CALL
        sys_munmap((unsigned long)s_sync_msg_reply_shm.shm_addr, sizeof(msg_reply_t));
#else
        munmap(s_sync_msg_reply_shm.shm_addr, sizeof(msg_reply_t));
#endif
        close(s_sync_msg_reply_shm.shm_fd);
        s_sync_msg_reply_shm.shm_fd = -1;
        shm_unlink(s_sync_msg_reply_shm.shm_name);
    }

    s_sync_msg_reply_shm.shm_addr = NULL;

    return true;
}

bool clear_sys_msg_queue(int sys_msg_type)
{
    mqd_t mqd;
    struct mq_attr attr;
    int i;
    private_msg_t msg;
    unsigned msg_prio;

    if(!is_applib_inited())
    {
        return false;
    }

    os_sched_lock();
    print_dbg("open msg queue: %s", g_this_app_info->mq_name);

    mqd = mq_open(g_this_app_info->mq_name, O_RDWR | O_NONBLOCK);

    if((mqd_t)-1 == mqd)
    {
        print_err("open msg queue %s failed!", g_this_app_info->mq_name);
        os_sched_unlock();
        return false;
    }

    mq_getattr(mqd, &attr); 

    if(-1 == sys_msg_type)
    {
        for(i = 0; i < attr.mq_curmsgs; i++)
        {
            mq_receive(mqd, (char *)&msg, sizeof(msg), &msg_prio);

            /* 保留同步消息 */
            if(msg.sync == true)
            {
                mq_send(mqd, (const char *)&msg, sizeof(msg), 0);
            }

        }
    }
    else 
    {
        for(i = 0; i < attr.mq_curmsgs; i++)
        {
            mq_receive(mqd, (char *)&msg, sizeof(msg), &msg_prio);

            /* 保留同步消息 */
            if(msg.sync == true)
            {
                mq_send(mqd, (const char *)&msg, sizeof(msg), 0);
            }

            /* 保留type类型为非sys_msg_type的消息 */
            if(sys_msg_type != msg.apps_msg.type)
            {
                mq_send(mqd, (const char *)&msg, sizeof(msg), 0);
            }
        }
    }

    print_dbg("close msg queue: %s",g_this_app_info->mq_name);
    if(mq_close(mqd) < 0)
    {
        print_err("close msg queue %s failed!", g_this_app_info->mq_name);
        os_sched_unlock();
        return false;
    }

    os_sched_unlock();
    return true;
}

bool clear_key_msg_queue(int key_val, int key_type)
{
    mqd_t mqd;
    struct mq_attr attr;
    int i;
    input_gui_msg_t msg;
    unsigned msg_prio;

    if(!is_applib_inited())
    {
        return false;
    }

    os_sched_lock();
    print_dbg("open msg queue: %s", GUI_KEY_FIFO);

    mqd = mq_open(GUI_KEY_FIFO, O_RDWR | O_NONBLOCK);

    if((mqd_t)-1 == mqd)
    {
        print_err("open msg queue %s failed!", GUI_KEY_FIFO);
        os_sched_unlock();
        return false;
    }

    mq_getattr(mqd, &attr); 

    if(-1 == key_val)
    {
        for(i = 0; i < attr.mq_curmsgs; i++)
        {
            mq_receive(mqd, (char *)&msg, sizeof(msg), &msg_prio);

            /* 保留非按键消息 */
            if( msg.type != INPUT_MSG_KEY )
            {
                mq_send(mqd, (const char *)&msg, sizeof(msg), 0);
            }
            else
            {
                if(key_type != -1)
                {
                    /* 保留按键状态为非key_type的按键消息 */
                    if(msg.data.kmsg.type != key_type)
                    {
                        mq_send(mqd, (const char *)&msg, sizeof(msg), 0);
                    }
                }
            }
        }
    }
    else 
    {
        for(i = 0; i < attr.mq_curmsgs; i++)
        {
            mq_receive(mqd, (char *)&msg, sizeof(msg), &msg_prio);

            /* 保留非按键消息 */
            if( msg.type != INPUT_MSG_KEY )
            {
                mq_send(mqd, (const char *)&msg, sizeof(msg), 0);
            }
            else
            {
                /* 保留按键值为非key_val的按键消息 */
                if( msg.data.kmsg.val != key_val )
                {
                    mq_send(mqd, (const char *)&msg, sizeof(msg), 0);
                }

                if(key_type != -1)
                {
                    /* 保留按键状态为非key_type的按键消息 */
                    if( key_type != msg.data.kmsg.type)
                    {
                        mq_send(mqd, (const char *)&msg, sizeof(msg), 0);
                    }
                }
            }
        }
    }

    print_dbg("close msg queue: %s", GUI_KEY_FIFO);
    if(mq_close(mqd) < 0)
    {
        print_err("close msg queue %s failed!", GUI_KEY_FIFO);
        os_sched_unlock();
        return false;
    }

    os_sched_unlock();
    return true;
}

bool enter_background_mode(void)
{
    app_info_t new_info = *g_this_app_info; 

    if(!is_applib_inited())
    {
        return false;
    }

    /* modify app type */
    new_info.type = APP_TYPE_CONSOLE; 
    return app_running_list_set_this_app_info(&new_info);
}

bool enter_foreground_mode(void)
{
    app_info_t new_info = *g_this_app_info; 

    if(!is_applib_inited())
    {
        return false;
    }

    /* modify app type */
    new_info.type = APP_TYPE_GUI; 
    return app_running_list_set_this_app_info(&new_info);
}


bool create_global_msg_queue(const char * mq_name)
{
    int i;
    struct mq_attr mqattr;
    mqd_t mqd;

    if(mq_name == NULL)
    {
        return false;
    }

    if(!is_applib_inited())
    {
        return false;
    }

    os_sched_lock();

    /* find a unused item space */
    for(i  = 0; i < MAX_GLOBAL_MQ; i++)
    {
        if( 0 == s_global_mq_shm->used_map[i])
        {
            break;
        }
    }

    if(i >= MAX_GLOBAL_MQ)
    {
        print_err("can not find a unused item!");
        os_sched_unlock();
        return false;
    }

    strncpy(s_global_mq_shm->mq[i].mq_name, mq_name, MAX_NAME_LEN - 1);
    s_global_mq_shm->mq[i].mq_name[MAX_NAME_LEN - 1] = '\0';

    /* creat message queue for app_name */
    mqattr.mq_flags = 0;
    mqattr.mq_maxmsg =  MQ_MAXMSG;
    mqattr.mq_msgsize = MQ_MSGSIZE;
    mqattr.mq_curmsgs = 0;

    print_dbg("creat message queue: %s", s_global_mq_shm->mq[i].mq_name);
    mqd = mq_open(s_global_mq_shm->mq[i].mq_name, O_CREAT | O_EXCL, 0666, &(mqattr));
    if((mqd_t)-1 == mqd)
    {
        print_err("creat msg queue %s failed!", s_global_mq_shm->mq[i].mq_name);
        os_sched_unlock();
        return false;
    }

    print_dbg("close msg queue: %s",s_global_mq_shm->mq[i].mq_name);
    if(mq_close(mqd) < 0)
    {
        print_err("close msg queue %s failed!", s_global_mq_shm->mq[i].mq_name);
        os_sched_unlock();
        return false;
    }

    s_global_mq_shm->used_map[i] = 1;

    os_sched_unlock();

    return true;
}


static bool _is_global_msg_queue_created(const char * mq_name)
{
    int i;
    /* find mq_name in global msg queue array */
    for(i  = 0; i < MAX_GLOBAL_MQ; i++)
    {
        if( (1 == s_global_mq_shm->used_map[i])
                && (strcmp(s_global_mq_shm->mq[i].mq_name, mq_name) == 0) )
        {
            break;
        }
    }

    if(i >= MAX_GLOBAL_MQ)
    {
        return false;
    }

    return true;
}

bool delete_global_msg_queue(const char * mq_name)
{
    int i;

    if(!is_applib_inited())
    {
        return false;
    }

    if(mq_name == NULL)
    {
        return false;
    }

    if(!is_applib_inited())
    {
        return false;
    }

    os_sched_lock();

    /* find mq_name in global msg queue array */
    for(i  = 0; i < MAX_GLOBAL_MQ; i++)
    {
        if( (1 == s_global_mq_shm->used_map[i])
           && (strcmp(s_global_mq_shm->mq[i].mq_name, mq_name) == 0) )
        {
            break;
        }
    }

    if(i >= MAX_GLOBAL_MQ)
    {
        print_err("can not find %s", mq_name);
        os_sched_unlock();
        return false;
    } 

    if(mq_unlink(s_global_mq_shm->mq[i].mq_name) < 0)
    {
        print_err("unlink msg queue %s failed!", s_global_mq_shm->mq[i].mq_name);
        os_sched_unlock();
        return false;
    }

    s_global_mq_shm->used_map[i] = 0;
    os_sched_unlock();

    return true;
}

bool connect_global_msg_queue(const char * mq_name, sys_msg_proc proc)
{
    int i;
    app_info_t app_info;
    mqd_t mqd;

    if(!is_applib_inited())
    {
        return false;
    }

    if(mq_name == NULL)
    {
        return false;
    }

    if(_is_connected_to_global_msg_queue(mq_name) == true)
    {
        return true;
    }

    for(i = 0; i < MAX_GLOBAL_MQ; i++)
    {
        if(strlen(g_this_app_info->global_mq[i].mq_name) == 0)
        {
            print_dbg("open msg queue: %s", mq_name);
            mqd = mq_open(mq_name, O_RDONLY | O_NONBLOCK);

            if((mqd_t)-1 == mqd)
            {
                print_err("open msg queue %s failed!", mq_name);
                return false;
            }

            /* modify curent app info */
            app_info = *g_this_app_info;
            strcpy(app_info.global_mq[i].mq_name, mq_name);
            app_info.global_mq[i].mqdes = mqd;
            app_info.global_mq[i].proc = proc;

            app_running_list_set_this_app_info(&app_info);

            print_dbg(" connect %s ", mq_name);

            return true;
        }
    }

    print_err("only allowed to connect %d global msg queue", MAX_GLOBAL_MQ);
    return false;
}

static bool _is_connected_to_global_msg_queue(const char * mq_name)
{
    int i;
    for(i = 0; i < MAX_GLOBAL_MQ; i++)
    {
        if(strcmp(g_this_app_info->global_mq[i].mq_name, mq_name) == 0)
        {
            return true;
        }
    }

    return false;
}

bool disconnect_global_msg_queue(const char * mq_name)
{
    int i;
    app_info_t app_info;

    if(!is_applib_inited())
    {
        return false;
    }

    if(mq_name == NULL)
    {
        return false;
    }

    if(_is_connected_to_global_msg_queue(mq_name) == false)
    {
        return true;
    }

    for(i = 0; i < MAX_GLOBAL_MQ; i++)
    {
        if(strcmp(g_this_app_info->global_mq[i].mq_name, mq_name) == 0)
        {
            if(g_this_app_info->global_mq[i].mqdes != (mqd_t)-1)
            {
                print_dbg("close msg queue: %s", g_this_app_info->global_mq[i].mq_name);
                if(mq_close(g_this_app_info->global_mq[i].mqdes) < 0)
                {
                    print_err("close msg queue %s failed!", g_this_app_info->global_mq[i].mq_name);
                    return false;
                }
            }

            /* modify curent app info */
            app_info = *g_this_app_info;
            memset(app_info.global_mq[i].mq_name, 0, sizeof(app_info.global_mq[i].mq_name)); 
            app_info.global_mq[i].mqdes = (mqd_t)-1; 
            app_info.global_mq[i].proc = NULL; 

            app_running_list_set_app_info(app_info.name, &app_info);

            print_dbg(" disconnect %s ", mq_name);

            return true;
        }
    }

    print_warning(" has not connected to %s", mq_name);
    return true;
}

bool disconnect_all_global_msg_queue(void)
{
    int i;
    app_info_t app_info;

    if(!is_applib_inited())
    {
        return false;
    }

    for(i = 0; i < MAX_GLOBAL_MQ; i++)
    {
        if(g_this_app_info->global_mq[i].mqdes != (mqd_t)-1)
        {
            print_dbg("close msg queue: %s", g_this_app_info->global_mq[i].mq_name);
            if(mq_close(g_this_app_info->global_mq[i].mqdes) < 0)
            {
                print_err("close msg queue %s failed!", g_this_app_info->global_mq[i].mq_name);
            }
        }

        /* modify curent app info */
        app_info = *g_this_app_info;
        memset(app_info.global_mq[i].mq_name, 0, sizeof(app_info.global_mq[i].mq_name)); 
        app_info.global_mq[i].mqdes = (mqd_t)-1; 
        app_info.global_mq[i].proc = NULL; 

        app_running_list_set_app_info(app_info.name, &app_info);

        print_dbg(" disconnect %s ", g_this_app_info->global_mq[i].mq_name);

//        return true;
    }

    return true;
}

static bool _init_global_mq_manage_shm(void)
{
    bool create = false;
    int shm_fd;
    global_mq_shm_t * shm_addr = NULL;

    print_dbg("open shared memory: %s", GLOBAL_MQ_MANAGE_SHM_NAME);

    shm_fd = shm_open(GLOBAL_MQ_MANAGE_SHM_NAME, O_RDWR, 0666);
    if(shm_fd < 0)
    {
        /*
           if(ENOENT != errno)
           {
           print_err("critical error! errno:%d",errno);
           return false;
           }
           */

        print_info("create shared memory: %s",GLOBAL_MQ_MANAGE_SHM_NAME);
        shm_fd = shm_open(GLOBAL_MQ_MANAGE_SHM_NAME, O_RDWR | O_CREAT, 0666);
        if(-1 == shm_fd)
        {
            print_err("create shared memory %s failed!", GLOBAL_MQ_MANAGE_SHM_NAME);
            return false;
        }

#ifdef USE_SYS_CALL
        sys_ftruncate((unsigned int)shm_fd, sizeof(global_mq_shm_t)); 
#else
        ftruncate(shm_fd, sizeof(global_mq_shm_t)); 
#endif
        create = true;
    }

#ifdef USE_SYS_CALL
    shm_addr = (global_mq_shm_t *)sys_mmap((unsigned long)NULL, sizeof(global_mq_shm_t), \
            PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
#else
    shm_addr = (global_mq_shm_t *)mmap(NULL, sizeof(global_mq_shm_t), \
            PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
#endif


    if((void *)-1 == shm_addr)
    {
        print_err("map to %s failed!", GLOBAL_MQ_MANAGE_SHM_NAME);
        close(shm_fd);
        s_global_mq_shm = NULL;
        s_global_mq_shm_fd = -1;
        return false;
    }

    s_global_mq_shm = shm_addr;
    s_global_mq_shm_fd = shm_fd;

    if(create == true)
    {
        memset(s_global_mq_shm, 0, sizeof(global_mq_shm_t));
    }

    return true;
}


static bool _deinit_global_mq_manage_shm(void)
{
    if(s_global_mq_shm_fd > 0)
    {
        close(s_global_mq_shm_fd);
        s_global_mq_shm_fd = -1;
    }

    s_global_mq_shm = NULL; 

    return true;
}
