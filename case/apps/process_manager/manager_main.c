/********************************************************************************
 *                              uS213f
 *                        Module: process  manager
 *                    Copyright(c) 2003-2008 Actions Semiconductor
 *                             All Rights Reserved.
 *
 *  History:
 *  <author>      <time>           <version >            <desc>
 *  nevin.bi      2009-11-03           35                            add comments.
 *********************************************************************************/

#include <include_psp.h>
#include <include_case.h>

typedef struct app_var_s
{
    void* applib_lib;
}app_var_t;

/*
 * when send sync message  to kill a process, we use a semaphore for sync between the wait and msg threads.
 */
struct quit_app_info_t
{
    sem_t quit_sem;
    char quit_app_name[MSG_CONTENT_SIZE];
    bool notify_invoker;
} quit_app_info;

/*
 * Every running process is invoked by another process except the manager.app,
 * here we use the invoke_list register this relationship. When the invokee exits, we
 * notify its invoker.
 */
struct invoke_list_t
{
    struct list_head list;  /* the list structure*/
    char* invoker;             /* the invoker */
    char* invokee;          /* the invokee */
    bool resume;                /* if true, when the invokee exit, the invoker will be recreated if not alive. */
} invoke_list;

/*
 * music_mutex_list:  the applications which conflict with the music engine
 * radio_mutex_list:   the applications which conflict with the radio engine
 */
struct mutex_list_t
{
    struct list_head list;
    char* app_name;
} music_mutex_list, radio_mutex_list;

/*
 * tmp variable  used in functions kill_user_apps().
 */
struct app_list_t
{
    struct list_head list;
    char* app_name;
} app_list;


/*
 * To exec a process, we should know its executable file's fullpath. Here we
 * register this information everytime we create a new process. So when
 * we need to recreate if we can't find it by its process name in this list.
 */
struct app_path_info_t
{
    struct list_head list;
    char* app_name;
    char* app_path;
}app_path_info_list;


/*
typedef enum
{
    CAUSE_USB,
    CAUSE_POWEROFF,
    CAUSE_TVIN,
}app_kill_cause_e;
*/

/*************************************************************/
static bool _init(void);

static void _deinit(void);

static void _scaleup_pm_freq(void);

static void _scaledown_pm_freq(void);

static void _start_watchdog(void);

static void _stop_watchdog(void);

static void _check_app_softwatchdogs(void * param);

static void _manager_creat_msg_thread(void);

static void*  _manager_msg_loop(void *data);

static void _manager_creat_wait_thread(void);

static void* _manager_wait_loop(void *data);

static void _manager_child_exit_cb(const char* exit_app_name);

static bool _notify_invoker(struct invoke_list_t * tmp, const char* exit_app_name);

static bool  _notify_launcher(const char* exit_app_name);

static int _msg_loop_cb(msg_apps_t *msg);

static bool _exec_app(char* content,const char* sender_name,bool resume);

static void _kill_user_apps(const char* sender,app_type_e app_type);

static bool _is_app_not_kill(const char* app_name);

static void _parse_argv(char*);

static void _reset_argv(void);

static void _print_argv(void);

static void _add_path_info(const char*);

static char*  _get_path_info(const char*);

static void _get_sys_configs(void);


static void _parse_mutex(struct mutex_list_t* list, char* text);

static void _check_mutex(char* app_name);



/*************************************************************/

static app_var_t app_var;

/*
#define NO_KILLS_USB 5
const char*     no_kills_usb[NO_KILLS_USB]=
    {APP_NAME_MANAGER,APP_NAME_MSG_SERV,APP_NAME_CONFIG,APP_NAME_LAUNCHER,APP_NAME_USB};

#define NO_KILLS_POWEROFF 3
const char*     no_kills_poweroff[NO_KILLS_POWEROFF]=
    {APP_NAME_MANAGER,APP_NAME_MSG_SERV,APP_NAME_CONFIG};

#define NO_KILLS_TVIN   4
const char*     no_kills_tvin[NO_KILLS_TVIN]=
    {APP_NAME_MANAGER,APP_NAME_MSG_SERV,APP_NAME_CONFIG,APP_NAME_LAUNCHER};
*/

/*
 * when kill user applications, these should be not listed.
 */
#define NO_KILLS_SYSTEM 4
const char*     no_kills_system[NO_KILLS_SYSTEM]=
    {APP_NAME_MANAGER,APP_NAME_MSG_SERV,APP_NAME_CONFIG,APP_NAME_LAUNCHER};

/*
 * container of the parsed process's argument list used in create new process.
 */
char* args[10] = { 0 };

/*
 * to speedup the process boot time, when call exec, we shift up the system's running frequency.
 */
static const clk_domain_t s_clk_domain_create_app = {CLK_LOAD_MIN, CLK_LOAD_HIGH_0};

/*
 * the config.app will notify manager.app the system bootup process finished.
 */
static bool bootup_finish = false;

static int freq_timer_id = -1;

static int wd_timer_id = -1;

static unsigned int watchdog_time = 5000;

static int wd_fd = -1;

/*
 * the shortcut applications can be called in every ui applications, when these
 * shortcuts start, its invoker has exited ahead. So when these shortcuts exit,
 * we should instead notify the launcher.
 * Here we use the remind_launcher to mark the shortcut application has been called.
 */
static bool remind_launcher = false;

/*************************************************************/
static  bool _init(void)
{
    app_var.applib_lib = dlopen("applib.so",RTLD_NOW);
    if(app_var.applib_lib == NULL)
    {
        return false;
    }

    return true;
}

static void _deinit(void)
{
    if(app_var.applib_lib != NULL)
    {
        dlclose(app_var.applib_lib);
        app_var.applib_lib = NULL;
    }
}

static void _scaleup_pm_freq(void)
{
    if(freq_timer_id == -1)
    {
        bool b = sys_pm_set_clk_domain(&s_clk_domain_create_app);
        if(b  == true)
        {
            print_dbg("call sys_pm_set_clk_domain success!");
            freq_timer_id = set_single_shot_timer(2000,(timer_proc)_scaledown_pm_freq,NULL);
        }
        else
        {
            print_err("call sys_pm_set_clk_domain failed!");
        }
    }
    else
    {
        modify_timer(freq_timer_id,2000);
    }
}


static void _scaledown_pm_freq(void)
{
    if(freq_timer_id != -1)
    {
        sys_pm_clear_clk_domain(&s_clk_domain_create_app);
        kill_timer(freq_timer_id);
        freq_timer_id = -1;
    }
}

static void _start_watchdog(void)
{
    if(watchdog_time > 0)
    {
        wd_fd = open( DEVICE_WD, O_RDWR, 0);
        if( wd_fd < 0 )
        {
            print_err("Error: open watchdog device\n\n");
        }
        else
        {
            wd_timer_id = set_timer(watchdog_time / 2, (timer_proc)_check_app_softwatchdogs, NULL);

            ioctl(wd_fd, WDD_CMD_SET_TIME,watchdog_time);
        }
    }
}

static void _stop_watchdog(void)
{
    if(wd_fd > 0)
    {
        close(wd_fd);
        wd_fd = -1;

        if (wd_timer_id != -1)
        {
            stop_timer(wd_timer_id);
            wd_timer_id = -1;
        }
    }
}


static void _check_app_softwatchdogs(void * param)
{
    check_soft_watchdogs();
    ioctl(wd_fd, WDD_CMD_SET_TIME,watchdog_time);
}

static void _manager_creat_msg_thread(void)
{
    pthread_t tid;
    pthread_attr_t manager_attr;
    struct sched_param manager_msg_param;

    /*
     * process manger's msg loop should be activated as long as there is
     * a msg in the process manager's msg queue
     */
    manager_msg_param.sched_priority = sched_get_priority_max(PTHREAD_SCOPE_SYSTEM) - 2;
    print_dbg(" manager_msg_loop  thread's priority is: %d\n",manager_msg_param.sched_priority );
    pthread_attr_init(&manager_attr);
    pthread_attr_setinheritsched(&manager_attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setdetachstate(&manager_attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setscope(&manager_attr, PTHREAD_SCOPE_SYSTEM);
    pthread_attr_setschedparam(&manager_attr, &manager_msg_param);

    pthread_create(&tid, &manager_attr, _manager_msg_loop, NULL );

    pthread_attr_destroy(&manager_attr);
}

static void*  _manager_msg_loop(void *data)
{
    msg_apps_t msg;

    INIT_LIST_HEAD(&invoke_list.list);

    INIT_LIST_HEAD(&app_list.list);

    INIT_LIST_HEAD(&app_path_info_list.list);

    register_sys_dispatcher(_msg_loop_cb);
    printf(" manager process: enter message receive loop...\n");

    msg.type = MSG_CREAT_APP;
    strncpy(msg.content, APP_NAME_CONFIG, MSG_CONTENT_SIZE);

    send_async_msg(APP_NAME_MANAGER, &msg);

    while (get_msg(&msg) == true)
    {
        dispatch_msg(&msg);
    }

    unregister_sys_dispatcher();
}

static void _manager_creat_wait_thread(void)
{

    pthread_t tid;
    pthread_attr_t manager_attr;
    struct sched_param manager_wait_param;

    /*
     * wait-thread should run at the max priority in system scope, because
     * we want every process be processed immediately after its exit for the resource recycling.
     */
    manager_wait_param.sched_priority = sched_get_priority_max(PTHREAD_SCOPE_SYSTEM);
    print_dbg(" manager_wait_loop  thread's priority is: %d\n",manager_wait_param.sched_priority );
    pthread_attr_init(&manager_attr);
    pthread_attr_setinheritsched(&manager_attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setdetachstate(&manager_attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setscope(&manager_attr, PTHREAD_SCOPE_SYSTEM);
    pthread_attr_setschedparam(&manager_attr, &manager_wait_param);

    pthread_create(&tid, &manager_attr, _manager_wait_loop, NULL );

    pthread_attr_destroy(&manager_attr);
}

static void* _manager_wait_loop(void* data)
{
    const char* arg[1] = { APP_NAME_MANAGER };

    char* default_path = "PATH=/mnt/remotedisk/:/mnt/diska/apps/:/mnt/sdisk/";
    putenv(default_path);

    _init();

    applib_init(1, arg, APP_TYPE_CONSOLE);

    sem_init(&quit_app_info.quit_sem, PTHREAD_PROCESS_PRIVATE, 0);

    while (1)
    {
        pid_t exit_pid = -1;
        exit_pid = waitpid(-1, NULL, 0);

        if (exit_pid != -1)
        {
            const char* exit_app_name = get_app_name(exit_pid);
            if (exit_app_name != NULL)
            {
                _manager_child_exit_cb(exit_app_name);
            }
            else
            {
                print_dbg("foreign process %d exit. <<<<<<<<<<<<<<<<<<<<<<<<",exit_pid);
            }
        }
        else
        {
            if (errno == ECHILD)
            {
                print_dbg(" there is no child i can waiting for...<<<<<<<<<<<<<<<<<<<<");
            }
            sleep(1);
        }
    }
    //_deinit();          //waitloop线程永远不会退出，因此此句永远不会执行到。
}

static void _manager_child_exit_cb(const char* exit_app_name)
{
    struct invoke_list_t *tmp;
    struct list_head *pos, *q;

    bool b_notify_invoker = true;

    bool b_invoker_notified = false;

    int cmp_quit_app = strncmp(exit_app_name, quit_app_info.quit_app_name, MSG_CONTENT_SIZE);

    if (cmp_quit_app == 0)
    {
        if (quit_app_info.notify_invoker == false)
        {
            b_notify_invoker = false;
            quit_app_info.notify_invoker = true;
        }
        sem_post(&quit_app_info.quit_sem);
        strncpy(quit_app_info.quit_app_name, "non_exist.app", MSG_CONTENT_SIZE);
    }

    applib_unregister_app(exit_app_name);
    print_dbg("child process %s exit. <<<<<<<<<<<<<<<<<<<<<<<<",exit_app_name);

    /*
     * iterate the invoke_list, find the matched invoker-invokee pair,
     * send notify message when necessary.
     */
    list_for_each_safe(pos,q,&(invoke_list.list))
    {
        tmp = list_entry(pos,struct invoke_list_t,list);

        print_dbg("tmp's invoker=%s, invokdee = %s\n",tmp->invoker,tmp->invokee);

        if (strcmp(tmp->invokee, exit_app_name) == 0)
        {
            print_dbg("above equals.\n");

            if (b_notify_invoker == true)
            {
                b_invoker_notified = _notify_invoker(tmp,exit_app_name);
            }
            list_del(pos);
            free(tmp->invokee);
            free(tmp->invoker);
            free(tmp);
            break;
        }
    }

    if (b_invoker_notified == true)
    {
        /*
         * if we have already notify the invoker, skip the launcher invoke process.
         */
        return;
    }

    /*
     *  the engine can run without the exist of the ui applications. So when they exit,
     *  we will not notify their launcher.
     */
    if ((strcmp(exit_app_name, basename(APP_NAME_MUSIC_ENGINE)) == 0) ||
            (strcmp(exit_app_name, basename( APP_NAME_RADIO_ENGINE)) == 0))
    {
        return;
    }

    if (remind_launcher == true)
    {
        _notify_launcher(exit_app_name);
        remind_launcher = false;
    }
    else
    {
        print_err();
    }
}

static bool _notify_invoker(struct invoke_list_t * tmp, const char* exit_app_name)
{
    msg_apps_t msg;

    bool b_invoker_notified = false;

    bool app_exist = is_app_exist(tmp->invoker);
    if (app_exist == true)
    {
        msg.type = MSG_SET_APP_FOREGROUND;
        memset(msg.content, 0, sizeof(msg.content));
        strncpy(msg.content, exit_app_name, MSG_CONTENT_SIZE);
        send_async_msg(tmp->invoker, &msg);

        if ((remind_launcher == true) && (strcmp(tmp->invoker, basename(APP_NAME_LAUNCHER)) == 0))
        {
            remind_launcher = false;
        }
        b_invoker_notified = true;
    }
    else
    {
        if (tmp->resume == true)
        {
            char* app_path = _get_path_info(tmp->invoker);
            if (app_path != NULL)
            {
                memset(msg.content, 0, sizeof(msg.content));
                strcpy(msg.content, app_path);
                strcat(msg.content, " "APPLICATION_ARGV_RESUME);
                _exec_app(msg.content, basename(APP_NAME_LAUNCHER), false);
            }

            remind_launcher = true;
            b_invoker_notified = true;
        }
    }

    return b_invoker_notified;
}

static bool  _notify_launcher(const char* exit_app_name)
{
    msg_apps_t msg;

    bool launcher_exist = is_app_exist(basename(APP_NAME_LAUNCHER));
    if (launcher_exist == true)
    {
        print_dbg();
        msg.type = MSG_SET_APP_FOREGROUND;
        memset(msg.content, 0, sizeof(msg.content));
        strncpy(msg.content, exit_app_name, MSG_CONTENT_SIZE);
        send_async_msg(APP_NAME_LAUNCHER, &msg);
        return true;
    }
    else
    {
        return false;
    }
}

static int _msg_loop_cb(msg_apps_t *msg)
{

    bool app_exist = false;

    print_dbg("\n in manager's message loop:\n");
    print_dbg("msg.type = %d\t", msg->type);

    switch (msg->type)
    {
        case MSG_CREAT_APP:
        {
            _exec_app(msg->content,get_app_name(msg->sender_pid),false);
        }
        break;

        case MSG_CREAT_APP_SYNC:
        {
            msg_reply_t reply2;
            bool result = _exec_app(msg->content,get_app_name(msg->sender_pid),false);

            if (result == true)
            {
                reply2.type = MSG_REPLY_SUCCESS;
            }
            else
            {
                reply2.type = MSG_REPLY_FAILED;
            }
            reply_sync_msg(&reply2);
        }
        break;

        case MSG_KILL_APP_SYNC:
        {
            app_exist = is_app_exist(basename(msg->content));
            if (app_exist == true)
            {
                msg_apps_t m;
                msg_reply_t reply;

                m.type = MSG_APP_QUIT;
                memset(m.content, 0, MSG_CONTENT_SIZE);
                send_async_msg(msg->content, &m);
                strncpy(quit_app_info.quit_app_name, basename(msg->content), MSG_CONTENT_SIZE);
                quit_app_info.notify_invoker = true;
                sem_wait(&(quit_app_info.quit_sem));
                // TODO: use sem_timedwait instead of sem_wait, and use the
                // appropriate reply msg accordingly
                reply.type = MSG_REPLY_SUCCESS;
                reply_sync_msg(&reply);
            }
            else
            {
                msg_reply_t reply;
                reply.type = MSG_REPLY_FAILED;
                reply_sync_msg(&reply);
            }
        }
        break;

        case MSG_KILL_APP:
        {
            app_exist = is_app_exist(basename(msg->content));
            if (app_exist == true)
            {
                msg_apps_t m;
                m.type = MSG_APP_QUIT;
                memset(m.content, 0, MSG_CONTENT_SIZE);
                send_async_msg(msg->content, &m);
            }
        }
        break;

            //        case MSG_MENGINE_GET_STATE_SYNC:
            //        {
            //            msg_reply_t reply;
            //            reply.type = MSG_REPLY_MENGINE_GET_STATE_PLAYING;
            //            strncpy(reply.content,"MSG_REPLY_MENGINE_GET_STATE_PLAYING",MSG_CONTENT_SIZE);
            //            reply_sync_msg(&reply);
            //        }
            //      break;

        case MSG_KILL_USER_APPS:
        {
            remind_launcher = true;
            _kill_user_apps(NULL,APP_TYPE_GUI);
        }
        break;

        case MSG_KILL_USER_APPS_SYNC:
        {
            msg_reply_t reply;
            const char* sender_name = get_app_name(msg->sender_pid);
            _kill_user_apps(sender_name,APP_TYPE_GUI);

            reply.type = MSG_REPLY_SUCCESS;
            reply_sync_msg(&reply);
        }
        break;

        case MSG_KILL_USER_APPS_CREAT_APP:
        {
            remind_launcher = false;
            _kill_user_apps(NULL,APP_TYPE_GUI);
            _exec_app(msg->content,get_app_name(msg->sender_pid),false);
            remind_launcher = true;
        }
        break;

        case MSG_KILL_ALL_USER_APPS_SYNC:
        {
            msg_reply_t reply;
            const char* sender_name = get_app_name(msg->sender_pid);
            _kill_user_apps(sender_name,APP_TYPE_GUI);
            _kill_user_apps(sender_name,APP_TYPE_CONSOLE);

            reply.type = MSG_REPLY_SUCCESS;
            reply_sync_msg(&reply);
        }
        break;

        case MSG_CREAT_APP_RESUME:
        {
            msg_apps_t m;
            const char* name;
            char sender_name[128];

            remind_launcher = false;
            memset(sender_name, 0, 128);
            name = get_app_name(msg->sender_pid);
            if(name != NULL)
            {
                strncpy(sender_name, name, 128);

                m.type = MSG_APP_QUIT;
                memset(m.content, 0, MSG_CONTENT_SIZE);
                send_async_msg(sender_name, &m);
                strncpy(quit_app_info.quit_app_name, sender_name, MSG_CONTENT_SIZE);
                quit_app_info.notify_invoker = false;
                sem_wait(&(quit_app_info.quit_sem));

                _exec_app(msg->content, sender_name, true);
            }
        }
        break;

        case MSG_BOOTUP_FINISH:
        {
            bootup_finish = true;
            _get_sys_configs();

            _start_watchdog();
        }
        break;

        case MSG_STOP_HARD_WATCHDG_SYNC:
        {
            msg_reply_t reply;
            _stop_watchdog();
            reply.type = MSG_REPLY_SUCCESS;
            reply_sync_msg(&reply);
        }
        break;

        case MSG_RESTART_HARD_WATCHDG_SYNC:
        {
            msg_reply_t reply;
            _start_watchdog();
            reply.type = MSG_REPLY_SUCCESS;
            reply_sync_msg(&reply);
        }
        break;

        default:
        break;
    }
    return 0;
}

static bool _exec_app(char* content,const char* sender_name,bool resume)
{
    bool result = false;

    bool app_register_success = false;

    if (bootup_finish == true)
    {
        _scaleup_pm_freq();
    }

    _reset_argv();
    _parse_argv(content);
    _print_argv();

    _check_mutex(basename(args[0]));

    app_register_success = applib_register_app(basename(args[0]));
    if (app_register_success == true)
    {
        int ret = execvp(args[0], args);
        if (ret == 0)
        {
            app_info_t new_info = *app_running_list_get_app_info(basename(args[0]));
            new_info.state = APP_STATE_ACTIVE;
            app_running_list_set_app_info(basename(args[0]), &new_info);

            //const char* sender_name = get_app_name(msg->sender_pid);
            if (sender_name != NULL)
            {
                struct invoke_list_t* tmp = (struct invoke_list_t*) malloc(sizeof(struct invoke_list_t));

                tmp->invoker = strdup(sender_name);
                tmp->invokee = strdup(basename(args[0]));
                if (resume == true)
                {
                    tmp->resume = true;
                }
                else
                {
                    tmp->resume = false;
                }
                list_add(&(tmp->list), &(invoke_list.list));
                print_dbg("new invoke added: invoker=%s invokee=%s",tmp->invoker,tmp->invokee);

                _add_path_info(args[0]);
            }

            result = true;
        }
        else
        {
            applib_unregister_app(basename(args[0]));
        }
    }
    else // app_register_success = false
    {
        result = false;
    }
    return result;
}

static void _kill_user_apps(const char* sender, app_type_e app_type)
{
    msg_apps_t m;
    const app_running_list_t* p = NULL;
    struct list_head *pos, *q;

    p = app_running_list_get_list();
    for (; p != NULL; p = p->next)
    {
        if ((sender != NULL) && (strcmp(p->app_info->name, sender) == 0))
        {
            continue;
        }

        if(p->app_info->type !=  app_type)
        {
            continue;
        }

        if(_is_app_not_kill(p->app_info->name) == false)
        {
            struct app_list_t* t = (struct app_list_t*) malloc(sizeof(struct app_list_t));
            if( t != NULL)
            {
                t->app_name = strdup(p->app_info->name);

                list_add(&(t->list), &(app_list.list));

                list_for_each_safe(pos,q,&(invoke_list.list))
                {
                    struct invoke_list_t* l = list_entry(pos,struct invoke_list_t,list);

                    if(l->resume == true)
                    {
                        l->resume = false;
                    }

                    if ((strcmp(p->app_info->name, l->invokee) == 0) || (strcmp(p->app_info->name, l->invoker) == 0))
                    {
                        free(l->invokee);
                        free(l->invoker);
                        list_del(pos);
                        free(l);
                    }

                }
            }
        }
    }

    list_for_each_safe(pos,q,&(app_list.list))
    {
        struct app_list_t * tmp = list_entry(pos,struct app_list_t,list);

        m.type = MSG_APP_QUIT;
        memset(m.content, 0, MSG_CONTENT_SIZE);
        send_async_msg(tmp->app_name, &m);
        strncpy(quit_app_info.quit_app_name, tmp->app_name, MSG_CONTENT_SIZE);
        quit_app_info.notify_invoker = false;
        print_dbg("waiting for <%s> to exit. <<<<<<<<<<<<<<<<<<<<<<<<<",tmp->app_name);
        sem_wait(&(quit_app_info.quit_sem));

        list_del(pos);
        free(tmp->app_name);
        free(tmp);
    }
}

static bool _is_app_not_kill(const char* app_name)
{
    int i;
    for (i = 0; i < NO_KILLS_SYSTEM; i++)
    {
        if (strcmp(app_name, basename(no_kills_system[i])) == 0)
        {
            return true;
        }
    }
    return false;
}


static void _parse_argv(char* s)
{
    int i = 0;
    char* p = NULL;
    char* ss = s;
    size_t len = 0;

    while ((*ss !='\0') && (isspace(*ss) != 0))
    {
        ss++;
    }

    p = ss;

    while (p != NULL)
    {
        if (*ss == '"')
        {
            ss++;
            p = strchr(ss, '"');
            if (p != NULL)
            {
                args[i] = (char*) malloc((size_t)(p - ss + 1));
                memset(args[i], 0, (size_t)(p - ss + 1));
                strncpy(args[i], ss, (size_t)(p - ss));

                p++;

                while ((*p !='\0') && (isspace(*p) != 0))
                {
                    p++;
                }
                ss = p;
                i++;
                continue;
            }
        }

        p = strchr(ss, ' ');
        if ((p != NULL ) && (i < 8))
        {
            args[i] = (char*) malloc((size_t)(p - ss + 1));
            memset(args[i], 0,(size_t)( p - ss + 1));
            strncpy(args[i], ss, (size_t)(p - ss));

            while ((*p !='\0') && (isspace(*p) != 0))
            {
                p++;
            }
            ss = p;
            i++;
        }
    }
    len = strlen(ss);
    if (len > 0)
    {
        args[i] = (char*) malloc(len + 1);
        memset(args[i], 0, len + 1);
        strncpy(args[i], ss, len);
    }

}

static void _reset_argv(void)
{
    int i;
    for (i = 0; i < 10; i++)
    {
        if (args[i] != NULL)
        {
            free(args[i]);
            args[i] = NULL;
        }
    }

}

static void _print_argv(void)
{
    int i;
    for (i = 0; i < 10; i++)
    {
        if (args[i] != NULL)
        {
            printf("argv[%d]=%s\n", i, args[i]);
        }
    }
}

static void _add_path_info(const char* app_fullpath_name)
{
    struct app_path_info_t * tmp;
    struct list_head *pos, *q;
    struct app_path_info_t* t;

    list_for_each_safe(pos,q,&(app_path_info_list.list))
    {
        tmp = list_entry(pos,struct app_path_info_t,list);

        if(strcmp(tmp->app_path,app_fullpath_name) ==0)
        {
            return;
        }
    }

    t = (struct app_path_info_t*) malloc(sizeof(struct app_path_info_t));
    if( t != NULL)
    {
        t->app_name = strdup(basename(app_fullpath_name));
        t->app_path =strdup(app_fullpath_name);

        list_add(&(t->list), &(app_path_info_list.list));

        print_dbg(" --- add to app_path_info_list: %s - %s",t->app_name,t->app_path);
    }
}

static char*  _get_path_info(const char* app_name)
{
    struct app_path_info_t * tmp;
    struct list_head *pos, *q;

    list_for_each_safe(pos,q,&(app_path_info_list.list))
    {
        tmp = list_entry(pos,struct app_path_info_t,list);

        if(strcmp(tmp->app_name,app_name) ==0)
        {
            return tmp->app_path;
        }
    }

    return NULL;
}


static void _get_sys_configs(void)
{
    int ret = -1;
    unsigned int v = 0;
    char text[CFG_MAX_BUFF_LENGTH];

    void * p_apconfig = dlopen("apconfig.so", RTLD_NOW);
    if (p_apconfig == NULL)
    {
        print_err("load apconfig failed.");
        return;
    }
    else
    {
        ret = get_config(CFG_DEF_WATCHDOG_TIME, (char*) &v, 4);
        if (ret != -1)
        {
            watchdog_time = v;
        }

        memset(text, 0, CFG_MAX_BUFF_LENGTH);
        get_config(CFG_MUSIC_MUTEX_APPS, text, CFG_MAX_BUFF_LENGTH);
        _parse_mutex(&music_mutex_list, text);

        memset(text, 0, CFG_MAX_BUFF_LENGTH);
        get_config(CFG_RADIO_MUTEX_APPS, text, CFG_MAX_BUFF_LENGTH);
        _parse_mutex(&radio_mutex_list, text);
        dlclose(p_apconfig);
    }
}

static void _parse_mutex(struct mutex_list_t* list, char* text)
{
    char* p = text;
    char* q = text;
    struct mutex_list_t* tmp = NULL;

    while ((q != NULL))
    {
        p = strchr(q, ';');
        if (p == NULL)
        {
            break;
        }

        tmp = (struct mutex_list_t*) malloc(sizeof(struct mutex_list_t));

        if (tmp != NULL)
        {
            tmp->app_name = (char*) malloc((size_t) (p - q + 1));
            if (tmp->app_name != NULL)
            {
                memset(tmp->app_name, 0, (size_t) (p - q + 1));
                strncpy(tmp->app_name, q, (size_t) (p - q));

                list_add(&(tmp->list), &(list->list));
            }
            else
            {
                free(tmp);
            }
        }
        q = p + 1;
    }
}

static void _check_mutex(char* app_name)
{
    msg_apps_t m;

    struct list_head* pos;
    struct mutex_list_t* t;

    list_for_each(pos,&music_mutex_list.list)
    {
        t = list_entry(pos, struct mutex_list_t, list);
        if(strcmp(t->app_name,app_name) == 0)
        {
            if (is_app_exist(basename(APP_NAME_MUSIC_ENGINE)) == true)
            {
                m.type = MSG_APP_QUIT;
                memset(m.content, 0, MSG_CONTENT_SIZE);
                send_async_msg(APP_NAME_MUSIC_ENGINE, &m);
                strncpy(quit_app_info.quit_app_name, basename(APP_NAME_MUSIC_ENGINE), MSG_CONTENT_SIZE);
                quit_app_info.notify_invoker = false;
                sem_wait(&(quit_app_info.quit_sem));
                return;
            }
        }
    }

    list_for_each(pos,&radio_mutex_list.list)
    {
        t = list_entry(pos, struct mutex_list_t, list);
        if(strcmp(t->app_name,app_name) == 0)
        {
            if (is_app_exist(basename(APP_NAME_RADIO_ENGINE)) == true)
            {
                m.type = MSG_APP_QUIT;
                memset(m.content, 0, MSG_CONTENT_SIZE);
                send_async_msg(APP_NAME_RADIO_ENGINE, &m);
                strncpy(quit_app_info.quit_app_name, basename(APP_NAME_RADIO_ENGINE), MSG_CONTENT_SIZE);
                quit_app_info.notify_invoker = false;
                sem_wait(&(quit_app_info.quit_sem));
                return;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    INIT_LIST_HEAD(&music_mutex_list.list);
    INIT_LIST_HEAD(&radio_mutex_list.list);

    /*
     * arrange two individual threads for child waiting & msg receiving
     */
    _manager_creat_wait_thread(); /* child waiting daemon thread */
    _manager_creat_msg_thread(); /* msg receiving daemon thread */

    pthread_detach(pthread_self());

    pthread_exit(0);
}
