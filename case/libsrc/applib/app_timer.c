/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : app_timer.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-3-24             v1.0             build this file 
 ********************************************************************************/

#include "applib_common.h"

enum timer_type_e
{
    TIMER_TYPE_NORMAL,
    TIMER_TYPE_SINGLE_SHOT,
};

enum timer_state_e
{
    TIMER_STATE_RUNNING,
    TIMER_STATE_STOPED,
    TIMER_STATE_KILLED
};

/*!
 * \brief
 * private timer structure
 */
struct timer_t 
{			
    /*! id for timer */
    int id;	    
    /*! timeout value, in msecs */    
    unsigned int timeout;	
    /*! timer expiration value */
    unsigned int timeout_expires;	
    /*! callback function param */
    void * param;	
    /*! callback function */
    timer_proc func_proc;	
    /*! timer type */    
    enum timer_type_e type;	
    /*! timer state */    
    enum timer_state_e state;	
    struct timer_t *next;
};

static struct timer_t *p_app_timer_list = NULL;	
static struct timer_t *p_inner_timer_list = NULL;	

static void print_timer_list(struct timer_t * plist);

static inline int __set_timer(struct timer_t **pplist, enum timer_type_e type, \
                              unsigned int timeout, timer_proc func_proc, void *param);
static inline bool __modify_timer(struct timer_t * plist, int timer_id, unsigned int timeout);
static inline bool __stop_timer(struct timer_t * plist, int timer_id);
static inline bool __restart_timer(struct timer_t * plist, int timer_id);
static inline bool __kill_timer(struct timer_t * plist, int timer_id);
static inline void __kill_timers(struct timer_t * plist);
static inline void __handle_timers(struct timer_t * * pplist);
static inline void __delete_timers(struct timer_t * * pplist);



int set_timer(unsigned int timeout, timer_proc func_proc, void * param)
{
    if(!is_applib_inited())
    {
        return -1;
    }

    return __set_timer(&p_app_timer_list, TIMER_TYPE_NORMAL, timeout, func_proc, param);
}

int set_inner_timer(unsigned int timeout, timer_proc func_proc, void * param)
{
    if(!is_applib_inited())
    {
        return -1;
    }

    return __set_timer(&p_inner_timer_list, TIMER_TYPE_NORMAL, timeout, func_proc, param);
}

int set_single_shot_timer(unsigned int timeout, timer_proc func_proc, void * param)
{
    if(!is_applib_inited())
    {
        return -1;
    }

    return __set_timer(&p_app_timer_list, TIMER_TYPE_SINGLE_SHOT, timeout, func_proc, param);
}

int set_inner_single_shot_timer(unsigned int timeout, timer_proc func_proc, void * param)
{
    if(!is_applib_inited())
    {
        return -1;
    }

    return __set_timer(&p_inner_timer_list, TIMER_TYPE_SINGLE_SHOT, timeout, func_proc, param);
}

bool modify_timer(int timer_id, unsigned int timeout)
{
    if(!is_applib_inited())
    {
        return false;
    }

    return __modify_timer(p_app_timer_list, timer_id, timeout);
}

bool modify_inner_timer(int timer_id, unsigned int timeout)
{
    if(!is_applib_inited())
    {
        return false;
    }

    return __modify_timer(p_inner_timer_list, timer_id, timeout);
}

bool stop_timer(int timer_id)
{
    if(!is_applib_inited())
    {
        return false;
    }

    return __stop_timer(p_app_timer_list, timer_id);
}

bool stop_inner_timer(int timer_id)
{
    if(!is_applib_inited())
    {
        return false;
    }

    return __stop_timer(p_inner_timer_list, timer_id);
}

bool restart_timer(int timer_id)
{
    if(!is_applib_inited())
    {
        return false;
    }

    return __restart_timer(p_app_timer_list, timer_id);
}

bool restart_inner_timer(int timer_id)
{
    if(!is_applib_inited())
    {
        return false;
    }

    return __restart_timer(p_inner_timer_list, timer_id);
}

bool kill_timer(int timer_id)
{
    if(!is_applib_inited())
    {
        return false;
    }

    return __kill_timer(p_app_timer_list, timer_id);
}

bool kill_inner_timer(int timer_id)
{
    if(!is_applib_inited())
    {
        return false;
    }

    return __kill_timer(p_inner_timer_list, timer_id);
}

void kill_timers(void)
{
    if(!is_applib_inited())
    {
        return ;
    }

    __kill_timers(p_app_timer_list);
}

void kill_inner_timers(void)
{
    if(!is_applib_inited())
    {
        return ;
    }

    __kill_timers(p_inner_timer_list);
}

void delete_timers(void)
{
    if(!is_applib_inited())
    {
        return ;
    }

    __delete_timers(&p_app_timer_list);
}

void delete_inner_timers(void)
{
    if(!is_applib_inited())
    {
        return ;
    }

    __delete_timers(&p_inner_timer_list);
}

void handle_timers(void)
{
    __handle_timers(&p_inner_timer_list);

    if(g_this_app_info->timer_valid == true)
    {
        __handle_timers(&p_app_timer_list);
    }

}

unsigned int get_tick_time_ms(void)
{
    /*
       struct timeval tm;
       unsigned int curtime = 0;	
       do_gettimeofday(&tm);

       curtime = tm.tv_sec*1000 + tm.tv_usec/1000;
       return curtime;
       */
    return get_ab_timer();
}

static inline int __set_timer(struct timer_t **pplist, enum timer_type_e type, \
                              unsigned int timeout, timer_proc func_proc, void *param)
{
    static int timer_id = 1;
    struct timer_t *tm = NULL;


    if(!func_proc)
    {
        print_warning("timer callback function pointer could not be NULL!");
        return -1;
    }

    if(0 == timeout)
    {
        print_warning("timeout value could not be set to zero!");
        return -1;
    }

    tm = (struct timer_t *) malloc ( sizeof(struct timer_t) );

    if( tm == NULL )
    {
        print_err("malloc failed!");
        return -1;
    }

    tm->id = timer_id;
    timer_id++;
    tm->timeout = timeout;
    tm->timeout_expires = get_tick_time_ms() + timeout;
    tm->param = param;
    tm->func_proc = func_proc;
    tm->type = type;
    tm->state = TIMER_STATE_RUNNING;
    tm->next = *pplist;
    *pplist = tm;

    /*
       printf("inner timer list:\n");
       print_timer_list(p_inner_timer_list);
       printf("app timer list:\n");
       print_timer_list(p_app_timer_list);
       */

    return tm->id;
}

static inline bool __modify_timer(struct timer_t * plist, int timer_id, unsigned int timeout)
{
    struct timer_t *tm;

    if(0 == timeout)
    {
        print_warning("timeout value could not be set to zero!");
        return 0;
    }

    for ( tm = plist; tm != NULL; tm = tm->next ) 
    {
        if( tm->id == timer_id) 
        {
            tm->timeout = timeout;
            tm->timeout_expires = get_tick_time_ms() + timeout;

            /*
               printf("inner timer list:\n");
               print_timer_list(p_inner_timer_list);
               printf("app timer list:\n");
               print_timer_list(p_app_timer_list);
               */

            return true;
        }
    }

    return false;
}

static inline bool __stop_timer(struct timer_t * plist, int timer_id)
{
    struct timer_t *tm;

    for ( tm = plist; tm != NULL; tm = tm->next ) 
    {
        if( tm->id == timer_id) 
        {
            tm->state = TIMER_STATE_STOPED;

            /*
               printf("inner timer list:\n");
               print_timer_list(p_inner_timer_list);
               printf("app timer list:\n");
               print_timer_list(p_app_timer_list);
               */

            return true;
        }
    }

    return false;
}

static inline bool __restart_timer(struct timer_t * plist, int timer_id)
{
    struct timer_t *tm;

    for ( tm = plist; tm != NULL; tm = tm->next ) 
    {
        if( tm->id == timer_id) 
        {
            tm->state = TIMER_STATE_RUNNING;

            /* reset timer */
            tm->timeout_expires = get_tick_time_ms() + tm->timeout; 

            /*
               printf("inner timer list:\n");
               print_timer_list(p_inner_timer_list);
               printf("app timer list:\n");
               print_timer_list(p_app_timer_list);
               */

            return true;
        }
    }

    return false;
}

static inline bool __kill_timer(struct timer_t * plist, int timer_id)
{
    struct timer_t *tm;

    //printf("__kill_timer\n");

    for ( tm = plist; tm != NULL; tm = tm->next ) 
    {
        if( tm->id == timer_id) 
        {
            tm->state = TIMER_STATE_KILLED;

            /*
               printf("inner timer list:\n");
               print_timer_list(p_inner_timer_list);
               printf("app timer list:\n");
               print_timer_list(p_app_timer_list);
               */

            return true;
        }
    }

    return false;
}

static inline void __kill_timers(struct timer_t * plist)
{
    struct timer_t *tm ;

    //printf("__kill_timers\n");

    for ( tm = plist; tm != NULL; tm = tm->next ) 
    {
        tm->state = TIMER_STATE_KILLED;
    }

    /*
       printf("inner timer list:\n");
       print_timer_list(p_inner_timer_list);
       printf("app timer list:\n");
       print_timer_list(p_app_timer_list);
       */
}

static inline void __handle_timers(struct timer_t * * pplist)
{
    struct timer_t *tm = *pplist; 
    struct timer_t *ltm = NULL;
    unsigned int cur_time_ms ;

    while(tm != NULL) 
    {
        /* remove the killed timer */
        if( TIMER_STATE_KILLED == tm->state) 
        {
            if( ltm != NULL )
            {
                ltm->next = tm->next;
                free(tm);
                tm = ltm->next;
            }
            else
            {
                *pplist = tm->next;
                free(tm);
                tm = *pplist;
            }

            /*
               printf("__handle_timer\n");
               print_timer_list(*pplist);
               */

            continue;
        }

        cur_time_ms = get_tick_time_ms();

        if( cur_time_ms >= tm->timeout_expires) 
        {
            /* call timer function */
            if( (tm->func_proc) && (TIMER_STATE_RUNNING == tm->state) )
            {
                if(TIMER_TYPE_SINGLE_SHOT == tm->type)
                {
                    tm->state = TIMER_STATE_STOPED;
                }
                tm->func_proc(tm->param);

                /* reset timer */
                tm->timeout_expires = get_tick_time_ms() + tm->timeout;
            }
        }

        if( cur_time_ms <= (tm->timeout_expires - tm->timeout )) 
        {
            /* if system time flow backwards because of some reason, need to repair timer */
            tm->timeout_expires = get_tick_time_ms() + tm->timeout;
        }

        ltm = tm;
        tm = tm->next;
    }
}

static inline void __delete_timers(struct timer_t * * pplist)
{
    struct timer_t *tm = *pplist; 
    //printf("__kill_timer\n");

    while(tm != NULL) 
    {
        /* remove all timers */
        *pplist = tm->next;
        free(tm);
        tm = *pplist;
    }

    /*
       printf("inner timer list:\n");
       print_timer_list(p_inner_timer_list);
       printf("app timer list:\n");
       print_timer_list(p_app_timer_list);
       */
}

static void print_timer_list(struct timer_t * plist)
{
    struct timer_t *tm;

    printf("pid(%d) timer list:\n", getpid());
    printf("\tid\t\ttype\t\tstate\t\ttimeout\n");

    for ( tm = plist; tm != NULL; tm = tm->next ) 
    {
        printf("\t%-7d\t\t%-12s\t\t%-13s\t\t%-7d\n", tm->id,\
        (tm->type == TIMER_TYPE_NORMAL) ? "normal":"single shot", \
        (tm->state == TIMER_STATE_RUNNING) ? "running": ( (tm->state == TIMER_STATE_STOPED) ? "stopped" : "killed"),\
        tm->timeout);
    }
}

