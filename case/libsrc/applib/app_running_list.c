/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : app_running_list.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-3-24             v1.0             build this file 
 ********************************************************************************/

#include "applib_common.h"

#define APP_RUNNING_LIST_SHM_NAME    "/shm/app_running_list"

#define USE_SYS_CALL

#define MAX_RUNNING_APP      10


/*!
 * \brief
 * Private structure placed in shared memory to 
 * store application info list. 
 */
typedef struct
{
    /*! amount of items */
    unsigned int num;

    /*! mutex semaphore for hole talbe */
    //sem_t table_mutex; 

    /*! mutex semaphore for every item */
    //sem_t item_mutexs[MAX_RUNNING_APP];

    /*! app running list */
    app_running_list_t list[MAX_RUNNING_APP];

    /*! item used map */
    unsigned char used_map[MAX_RUNNING_APP];

    /*! every item stored a application process's information */
    app_info_t items[MAX_RUNNING_APP];

}shm_app_data_t;


static int s_shm_fd = -1;
static shm_app_data_t * s_shm_app_data = NULL;
static app_running_list_t * s_app_list = NULL;

static bool initialized = false;
static inline bool is_inited(void);

/*!
 * \brief 
 */
static app_running_list_t * find_by_name(const char * app_name);

/*!
 * \brief 
 */
static  app_running_list_t * find_by_pid(pid_t pid);


bool is_app_exist(const char *app_name)
{
    app_running_list_t * app;

    print_dbg();

    if(is_inited() == false)
    {
        return false;
    }

    app = find_by_name(app_name);
    if(NULL == app)
    {
        return false;
    }

    return true;
}

bool get_app_type(const char *app_name, app_type_e * type)
{
    app_running_list_t * app;

    print_dbg();

    if(is_inited() == false)
    {
        return false;
    }

    app = find_by_name(app_name);
    if(NULL == app)
    {
        return false;
    }

    *type = (app->app_info)->type;

    return true;
}

pid_t get_app_pid(const char * app_name)
{
    app_running_list_t * app;

    print_dbg();

    if(is_inited() == false)
    {
        return -1;
    }

    app = find_by_name(app_name);
    if(NULL == app)
    {
        return -1;
    }

    return (app->app_info)->pid;
}

const char * get_app_name(pid_t pid)
{
    app_running_list_t * app;

    print_dbg();

    if(is_inited() == false)
    {
        return NULL;
    }

    app = find_by_pid(pid);
    if(NULL == app)
    {
        return NULL;
    }

    return (app->app_info)->name;
}

const app_info_t * app_running_list_get_app_info(const char * app_name)
{
    app_running_list_t * app;

    print_dbg();

    if(is_inited() == false)
    {
        return NULL;
    }

    app = find_by_name(app_name);
    if(NULL == app)
    {
        return NULL;
    }

    print_dbg();

    return app->app_info;
}

const app_info_t * app_running_list_get_this_app_info(void)
{
    app_running_list_t * this_app = NULL;

    print_dbg();

    if(is_inited() == false)
    {
        return NULL;
    }

    this_app = find_by_pid(getpid());
    if(NULL == this_app)
    {
        return NULL;
    }

    return this_app->app_info;
}

bool app_running_list_set_app_info(const char * app_name, const app_info_t * app_info)
{
    app_running_list_t * app;

    print_dbg();

    if(is_inited() == false)
    {
        return false;
    }

    if(NULL == app_name)
    {
        print_warning("input parameter(app_name) is a NULL pointer!");
        return false;
    }

    if(NULL == app_info)
    {
        print_warning("input parameter(app_info) is a NULL pointer!");
        return false;
    }

    os_sched_lock();
    //sem_wait(&(s_shm_app_data->table_mutex));

    app = find_by_name(app_name);
    if(NULL == app)
    {
        os_sched_unlock();
        //sem_post(&(s_shm_app_data->table_mutex));
        return false;
    }

    memcpy((app_info_t *)app->app_info, app_info, sizeof(app_info_t));

    //sem_post(&(s_shm_app_data->table_mutex));
    os_sched_unlock();

    return true;
}


bool app_running_list_set_this_app_info(const app_info_t * app_info)
{
    app_running_list_t * this_app = NULL;
    print_dbg();

    if(is_inited() == false)
    {
        return false;
    }

    if(NULL == app_info)
    {
        print_warning("input parameter(app_info) is a NULL pointer!");
        return false;
    }

    this_app = find_by_pid(getpid());
    if(NULL == this_app)
    {
        print_warning("%d is not in app running list!", getpid());
        return false;
    }

    os_sched_lock();
    //sem_wait(&(s_shm_app_data->table_mutex));
    memcpy((app_info_t *)this_app->app_info, app_info, sizeof(app_info_t));
    //sem_post(&(s_shm_app_data->table_mutex));
    os_sched_unlock();

    return true;
}

const app_running_list_t * app_running_list_get_list(void)
{
    print_dbg();

    if(is_inited() == false)
    {
        return NULL;
    }
    else
    {
        return s_app_list;
    }
}

bool app_running_list_add(const app_info_t * appinfo)
{
    int i,j;

    print_dbg();

    if(is_inited() == false)
    {
        return false;
    }

    if(NULL == appinfo)
    {
        print_warning(" input parameter(appinfo) is a NULL pointer!");
        return false;
    }

    os_sched_lock();
    //sem_wait(&(s_shm_app_data->table_mutex));

    if(s_shm_app_data->num > MAX_RUNNING_APP)
    {
        print_err("app running list is full!");
        os_sched_unlock();
        //sem_post(&(s_shm_app_data->table_mutex));

        return false;
    }

    /* find a unused item space */
    for(i  = 0; i < MAX_RUNNING_APP; i++)
    {
        if( 0 == s_shm_app_data->used_map[i])
        {
            break;
        }
    }

    if(i >= MAX_RUNNING_APP)
    {
        print_err("can not find a unused item!");
        os_sched_unlock();
        return false;
    }

    memcpy(&(s_shm_app_data->items[i]), appinfo, sizeof(app_info_t));
    s_shm_app_data->num++;
    s_shm_app_data->used_map[i] = 1;

    /* rebuild app running list */
    for(i  = 0, j = 0 ; i < MAX_RUNNING_APP; i++)
    {
        if( 1 == s_shm_app_data->used_map[i])
        {
            s_shm_app_data->list[j].app_info = &(s_shm_app_data->items[i]);
            s_shm_app_data->list[j].next = NULL;

            if(j > 0)
            {
                s_shm_app_data->list[j - 1].next = &(s_shm_app_data->list[j]);
            }

            j++;
        }
    }

    if(NULL == s_app_list)
    {
        s_app_list = s_shm_app_data->list;
    }

    //print_app_running_list();

    os_sched_unlock();
    //sem_post(&(s_shm_app_data->table_mutex));
    return true;
}

bool app_running_list_delete(const char * app_name)
{
    int i,j;

    print_dbg();

    if(is_inited() == false)
    {
        return false;
    }

    if(NULL == app_name)
    {
        print_warning(" input parameter(app_name) is a NULL pointer!");
        return false;
    }

    os_sched_lock();
    //sem_wait(&(s_shm_app_data->table_mutex));

    for(i  = 0; i < MAX_RUNNING_APP; i++)
    {
        if( 0 == s_shm_app_data->used_map[i])
        {
            continue;
        }

        if(strcmp(s_shm_app_data->items[i].name, app_name) == 0)
        {
            break;
        }
    }

    if(i >= MAX_RUNNING_APP)
    {
        print_warning("%s is not in app running list!", app_name);
        //sem_post(&(s_shm_app_data->table_mutex));
        os_sched_unlock();

        return false;
    }

    s_shm_app_data->used_map[i] = 0;
    s_shm_app_data->num--;

    /* rebuild app running list */
    for(i  = 0, j = 0 ; i < MAX_RUNNING_APP; i++)
    {
        if( 1 == s_shm_app_data->used_map[i])
        {
            s_shm_app_data->list[j].app_info = &(s_shm_app_data->items[i]);
            s_shm_app_data->list[j].next = NULL;

            if(j > 0)
            {
                s_shm_app_data->list[j - 1].next = &(s_shm_app_data->list[j]);
            }

            j++;
        }
    }

    //print_app_running_list();

    os_sched_unlock();
    //sem_post(&(s_shm_app_data->table_mutex));

    return false;
}

void print_app_running_list(void)
{
    app_running_list_t * p ;

    if(is_inited() == false)
    {
        return ;
    }

    printf("\npid(%d) running app list:\n", getpid());
    printf("pid\tname\t\t\tmsg queue\t\t\tmqdes\t\tstate\t\ttype\n");

    for(p = s_app_list ; p != NULL ; p = p->next)
    {
        printf("%-7d%-25s%-32s%#-16x%-15s%-15s\n", p->app_info->pid,
                p->app_info->name,
                p->app_info->mq_name,
                p->app_info->mqdes,
                (p->app_info->state == APP_STATE_ACTIVE) ? "active":"inactive",
                (p->app_info->type == APP_TYPE_GUI) ? "gui":"console");
    }
}

bool app_running_list_init(void)
{
    bool create = false;
    int fd = -1;

    print_dbg();

    if(true == initialized)
    {
        print_warning(" app running list has already initialized!");
        return false;
    }

    print_dbg("open shared memory: %s", APP_RUNNING_LIST_SHM_NAME);
    fd = shm_open(APP_RUNNING_LIST_SHM_NAME, O_RDWR, 0666);
    if(fd < 0)
    {
        /*
           if(ENOENT != errno)
           {
           print_err("critical error! errno:%d",errno);
           return false;
           }
           */

        print_info("create shared memory: %s",APP_RUNNING_LIST_SHM_NAME);
        fd = shm_open(APP_RUNNING_LIST_SHM_NAME, O_RDWR | O_CREAT, 0666);
        if(fd < 0)
        {
            print_err("create shared memory %s failed!", APP_RUNNING_LIST_SHM_NAME);
            return false;
        }

#ifdef USE_SYS_CALL
        sys_ftruncate((unsigned int)fd, sizeof(shm_app_data_t)); 
#else
        ftruncate(fd, sizeof(shm_app_data_t)); 
#endif
        create = true;
    }

#ifdef USE_SYS_CALL
    s_shm_app_data = (shm_app_data_t *)sys_mmap((unsigned long)NULL, sizeof(shm_app_data_t), \
            PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
#else
    s_shm_app_data = (shm_app_data_t *)mmap(NULL, sizeof(shm_app_data_t), \
            PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
#endif


    if((void *)-1 == (void *)s_shm_app_data)
    {
        print_err("map to %s failed!", APP_RUNNING_LIST_SHM_NAME);
        close(fd);
        fd = -1;
        s_shm_app_data = NULL;
        s_app_list = NULL;
        return false;
    }

    if(create == true)
    {
        memset(s_shm_app_data, 0 , sizeof(shm_app_data_t));

        //sem_init(&(s_shm_app_data->table_mutex), PTHREAD_PROCESS_SHARED , 1);     /* init talbe mutext semaphore. */
    }

    if(0 == s_shm_app_data->num)
    {
        s_app_list = NULL;
    }
    else
    {
        s_app_list = s_shm_app_data->list;
    }

    s_shm_fd = fd ;

    initialized = true;
    return true;
}

bool app_running_list_deinit(void)
{
    print_dbg();

    if(false == initialized)
    {
        print_warning("app running list has not initialized!");
        return false;
    }

    if(s_shm_fd > 0)
    {
        close(s_shm_fd);
        s_shm_fd = -1;
    }

    s_app_list = NULL; 
    s_shm_app_data = NULL;

    initialized = false;

    return true;
}

static  app_running_list_t * find_by_name(const char * app_name)
{
    app_running_list_t * p ;

    print_dbg();

    if(NULL == app_name)
    {
        print_warning("input parameter(app_name) is a NULL pointer!", app_name);
        return NULL;
    }

    for(p = s_app_list ; p != NULL ; p = p->next)
    {
        if(strcmp((p->app_info)->name, app_name) == 0)
        {
            return p;
        }
    }

    print_warning("%s is not in app running list", app_name);
    return NULL;
}

static  app_running_list_t * find_by_pid(pid_t pid)
{
    app_running_list_t * p ;

    print_dbg();

    for(p = s_app_list ; p != NULL ; p = p->next)
    {
        if((p->app_info)->pid == pid)
        {
            return p;
        }
    }

    print_warning("%d is not in app running list", pid);
    return NULL;
}


static inline bool is_inited(void)
{
    if(!initialized)
    {
        print_warning("app running list has not initialized!");
        return false;
    }

    return true;
}
