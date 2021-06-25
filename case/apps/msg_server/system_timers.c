/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : system_timers.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-3-24             v1.0             build this file 
 ********************************************************************************/

#include "include_case.h"
#include "include_msg_server.h"

static const app_info_t * s_cur_gui_app = NULL;

typedef enum
{
    SYS_TIMER_ATTRIBUTE_NORMAL,
    SYS_TIMER_ATTRIBUTE_IDLE,
}sys_timer_attribute_e;

typedef struct
{
    int timer_id;
    /* unit: milliseconds */
    unsigned int timeout;
    timer_proc  proc;
    sys_timer_type_e type;
    sys_timer_attribute_e attr;
    const char * name;
}sys_timer_t;

static int _scrsaver_timeout_proc(void * param);
static int _standby_timeout_proc(void * param);
static int _shutdown_timeout_proc(void * param);

static void _brocast_shutdown_msg(void);

static bool s_is_in_screensaver = false;
static unsigned int s_cur_lcd_backlight;

/*
 * standby定时器时间到时，如果正处于充电状态。则修改
 * standby定时器间隔为wait_charge_finish_interval，继续检测。
 */
static bool s_standby_wait_charge_finish = false;
static unsigned int wait_charge_finish_interval = 3000;

/*
 * shutdown定时器是否超时标志
 */
static bool s_shutdown_timer_timeouted = false;

static sys_timer_t sys_timers[3] = 
{ 
    {
        -1, 
        0, 
        _scrsaver_timeout_proc, 
        SYS_TIMER_TYPE_SCREENSAVER,
        SYS_TIMER_ATTRIBUTE_IDLE,
        "screensaver"
    },

    {
        -1,
        0, 
        _standby_timeout_proc, 
        SYS_TIMER_TYPE_STANDBY,
        SYS_TIMER_ATTRIBUTE_IDLE,
        "standby"
    },

    {
        -1, 
        0, 
        _shutdown_timeout_proc, 
        SYS_TIMER_TYPE_SHUTDOWN,
        SYS_TIMER_ATTRIBUTE_NORMAL,
        "shutdown"
    }
};


void load_sys_timers_config(void)
{
    print_dbg();

    sys_timers[SYS_TIMER_TYPE_SCREENSAVER].timeout = (unsigned int)g_msg_server_cfgs[CFG_TYPE_BACKLIGHT].value*1000;
    sys_timers[SYS_TIMER_TYPE_STANDBY].timeout = (unsigned int)g_msg_server_cfgs[CFG_TYPE_IDLE_TIME].value*1000;
    sys_timers[SYS_TIMER_TYPE_SHUTDOWN].timeout = (unsigned int)g_msg_server_cfgs[CFG_TYPE_SLEEP_TIME].value*1000;

    if(sys_timers[SYS_TIMER_TYPE_SCREENSAVER].timer_id > 0)
    {
        stop_timer(sys_timers[SYS_TIMER_TYPE_SCREENSAVER].timer_id);
    }

    if(sys_timers[SYS_TIMER_TYPE_STANDBY].timer_id > 0)
    {
        stop_timer(sys_timers[SYS_TIMER_TYPE_STANDBY].timer_id);
    }

    if(sys_timers[SYS_TIMER_TYPE_SHUTDOWN].timer_id > 0)
    {
        stop_timer(sys_timers[SYS_TIMER_TYPE_SHUTDOWN].timer_id);
    }
}

void start_sys_timers(bool start)
{
    print_dbg("%s", (start == true) ? "start sys timers!":"stop sys timers");

    start_sys_timer(SYS_TIMER_TYPE_SCREENSAVER, start);
    start_sys_timer(SYS_TIMER_TYPE_STANDBY, start);
    start_sys_timer(SYS_TIMER_TYPE_SHUTDOWN, start);
}

void start_sys_idle_timers(bool start)
{
    print_dbg("%s", (start == true) ? "start idle timers!":"stop idle timers");
    start_sys_timer(SYS_TIMER_TYPE_SCREENSAVER, start);

    if(s_standby_wait_charge_finish == true)
    {
        s_standby_wait_charge_finish = false;
        sys_timers[SYS_TIMER_TYPE_STANDBY].timeout = (unsigned int)g_msg_server_cfgs[CFG_TYPE_IDLE_TIME].value*1000;
    }
    start_sys_timer(SYS_TIMER_TYPE_STANDBY, start);
}

void start_sys_timer(sys_timer_type_e timer, bool start)
{
    print_dbg();

    if(true == start)
    {
        if(sys_timers[timer].timeout == 0)
        {
            if( sys_timers[timer].timer_id > 0 )
            {
                stop_timer(sys_timers[timer].timer_id);
            }

            return;
        }

        if( sys_timers[timer].timer_id > 0 )
        {
            restart_timer(sys_timers[timer].timer_id);
        }
        else
        {
            sys_timers[timer].timer_id = set_single_shot_timer(sys_timers[timer].timeout, sys_timers[timer].proc, NULL);
        }
    }
    else
    {
        if(sys_timers[timer].timer_id > 0)
        {
            stop_timer(sys_timers[timer].timer_id);
        }
        else
        {
            print_warning("%s timer has not started!", sys_timers[timer].name);
        }

    }
}


void set_sys_timer(sys_timer_type_e timer, unsigned int timeout)
{
    print_dbg();

    sys_timers[timer].timeout = timeout*1000;

    if(sys_timers[timer].timeout > 0)
    {
        if(sys_timers[timer].timer_id > 0)
        {
            modify_timer(sys_timers[timer].timer_id, sys_timers[timer].timeout);
        }
        else
        {
            start_sys_timer(timer, true);
        }
    }
    else
    {
        if(sys_timers[timer].timer_id > 0)
        {
            start_sys_timer(timer, false);
        }
        else
        {
        }
    }

}

inline unsigned int get_sys_timer_timeout(sys_timer_type_e timer)
{
    return sys_timers[timer].timeout;
}

static int _scrsaver_timeout_proc(void * param)
{
    if(false == sys_is_screensaver_forbidden())
    {
        enter_screensaver();
    }

    return 0;
}

static int _standby_timeout_proc(void * param)
{
    if(false == sys_is_auto_standby_forbidden())
    {
        bool ret;
        charge_status_t sts;
        ret = charge_get_status(&sts);
        if(ret == true)
        {
            if (sts == CHARGE_CHARGING)
            {
                sys_timers[SYS_TIMER_TYPE_STANDBY].timeout = wait_charge_finish_interval;
                start_sys_timer(SYS_TIMER_TYPE_STANDBY, true);
                s_standby_wait_charge_finish = true;
            }
            else
            {
                _brocast_shutdown_msg();
            }
        }
        else
        {
            start_sys_timer(SYS_TIMER_TYPE_STANDBY, true);
        }
    }

    return 0;
}

static int _shutdown_timeout_proc(void * param)
{
    bool forbid;

    forbid = sys_is_timed_shutdown_forbidden();
    if(forbid == true)
    {
        s_shutdown_timer_timeouted = true;
        return -1;
    }
    else
    {
        _brocast_shutdown_msg();
        return 0;
    }
}

static void _brocast_shutdown_msg(void)
{
    msg_apps_t msg = {0};
    start_sys_timers(false);
    /* todo: shutdown */
    print_info("power off!");

    msg.type = MSG_POWER_OFF;
    broadcast_msg(&msg);
}



void check_shutdown_timer(void)
{
    if(s_shutdown_timer_timeouted == true)
    {
        _shutdown_timeout_proc(NULL);
    }
}


bool enter_screensaver(void)
{
    app_running_list_t * list;
    app_info_t app_info;

    if(s_is_in_screensaver == true)
    {
        print_warning("already in screensaver!");
        return false;
    }

    print_info("enter screensaver!");

    drv_op_lcd_open_dev();
    drv_op_lcd_get_backlight(&s_cur_lcd_backlight);

    /* turn off lcd backlight*/
    drv_op_lcd_set_backlight(0);
    drv_op_lcd_enter_standby();
    drv_op_lcd_close_dev();

    list = (app_running_list_t *)app_running_list_get_list();
    for(; list != NULL; list = list->next)
    {
        if(list->app_info->type == APP_TYPE_GUI)
        {
            s_cur_gui_app = list->app_info;
            break;
        }
    }

    if(s_cur_gui_app != NULL)
    {
        app_info = *s_cur_gui_app;
        app_info.type = APP_TYPE_CONSOLE;
        app_info.timer_valid = false;
        app_running_list_set_app_info(app_info.name, &app_info);
    }

    s_is_in_screensaver = true;

    return true;
}


bool exit_screensaver(void)
{
    app_info_t app_info;

    if(s_is_in_screensaver == false)
    {
        print_warning("not in screensaver!");
        return false;
    }

    print_info("exit screensaver!");

    if(s_cur_gui_app != NULL)
    {
        app_info = *s_cur_gui_app;
        app_info.type = APP_TYPE_GUI;
        app_info.timer_valid = true;
        app_running_list_set_app_info(app_info.name, &app_info);
        s_cur_gui_app = NULL;
    }

    drv_op_lcd_open_dev();
    drv_op_lcd_exit_standby();
    sty_lcd_update();
    usleep(200000);

    /* restore lcd backlight*/
    drv_op_lcd_set_backlight(s_cur_lcd_backlight);
    drv_op_lcd_close_dev();

    s_is_in_screensaver = false;

    return true;
}

bool is_in_screensaver(void)
{
    return s_is_in_screensaver;
}

