/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : sys_settings.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-3-24             v1.0             build this file 
 ********************************************************************************/

#include <include_case.h>
#include "applib_common.h"

static int s_rtc_dev_fd = 0;
static rtc_date_t s_min_date = {2000,1,1,0,0};
static rtc_date_t s_max_date = {2037,12,31,0,0};

static bool _reset_alarm(void);

void sys_forbid_screensaver(bool forbid)
{
    msg_apps_t msg = {0};
    msg_reply_t reply = {0};

    app_info_t new_info = *g_this_app_info; 

    if(!is_applib_inited())
    {
        return ;
    }

    new_info.screensaver_forbidden = forbid; 
    app_running_list_set_this_app_info(&new_info);  
    //print_app_running_list();


    if(forbid == true)
    {
        /*
         * when forbid is true, it means that we should
         * exit the screensaver at once.
         */
        msg.type = MSG_EXIT_SCREENSAVER_SYNC;

        send_sync_msg(APP_NAME_MSG_SERV, &msg, &reply, NULL);

        if(reply.type != MSG_REPLY_SUCCESS)
        {
            print_warning("exit screensaver failed!");
        }


    }
    else
    {
        /*
         * when forbid is false, it means that we should
         * reset the screensaver timer.
         * Even there are other applications forbid the timer
         * now, resetting the timer is still safe.
         */
        msg.type = MSG_RESET_SCREENSAVER_TIMER_SYNC;

        send_sync_msg(APP_NAME_MSG_SERV, &msg, &reply, NULL);

        if(reply.type != MSG_REPLY_SUCCESS)
        {
            print_warning("reset the screensaver timer failed!");
        }

    }
}


bool sys_set_screensaver_idle_time(unsigned int time)
{

    msg_apps_t msg = {0};
    msg_reply_t reply = {0};
    system_settings_t sys_set = {0}; 

    if(!is_applib_inited())
    {
        return false;
    }

    msg.type = MSG_SET_SCREENSAVER_TIME_SYNC;
    sys_set.screensaver_time = time; 
    memcpy(msg.content, &sys_set, sizeof(sys_set));

    send_sync_msg(APP_NAME_MSG_SERV, &msg, &reply, NULL);

    if(reply.type == MSG_REPLY_SUCCESS)
    {
        return true;
    }
    else
    {
        print_err("set auto standby idle time failed!");
        return false;
    }
}

bool sys_is_screensaver_forbidden(void)
{
    app_running_list_t * list; 

    if(!is_applib_inited())
    {
        return false;
    }

    list = (app_running_list_t *)app_running_list_get_list();

    for(; list != NULL; list = list->next)
    {
        if(true == list->app_info->screensaver_forbidden)
        {
            return true ;
        }
    }

    return false;
}


void sys_forbid_auto_standby(bool forbid)
{
    msg_apps_t msg = {0};
    msg_reply_t reply = {0};
    bool last_stat;
    bool cur_stat;
    int fd;
    int value;

    app_info_t new_info = *g_this_app_info; 

    if(!is_applib_inited())
    {
        return ;
    }

    last_stat = sys_is_auto_standby_forbidden();

    new_info.auto_standby_forbidden = forbid; 
    app_running_list_set_this_app_info(&new_info);  
    //print_app_running_list();

    cur_stat = sys_is_auto_standby_forbidden();

    if(cur_stat != last_stat)
    {
        fd = open(DEVICE_PM, O_RDWR);
        if (fd < 0)
        {
            print_err("open %s failed!", DEVICE_PM);
        }
        else
        {
            if(cur_stat == true)
            {
                value = 1;
                if(ioctl(fd, SET_APP_STATUS, (int)&value) != 0) 
                {
                    print_err("ioctl(SET_APP_STATUS) failed!");
                }
            }
            else
            {
                value = 0;
                if(ioctl(fd, SET_APP_STATUS, (int)&value) != 0) 
                {
                    print_err("ioctl(SET_APP_STATUS) failed!");
                }
            }
            close(fd);
        }
    }

    /*
     * when forbid is false, it means that we should
     * reset the auto standby timer.
     * Even there are other applications forbid the timer
     * now, resetting the timer is still safe.
     */
    if(forbid == false)
    {
        msg.type = MSG_RESET_AUTO_STANDBY_TIMER_SYNC;

        send_sync_msg(APP_NAME_MSG_SERV, &msg, &reply, NULL);

        if(reply.type != MSG_REPLY_SUCCESS)
        {
            print_warning("reset the auto standby timer failed!");
        }
    }
}

void sys_forbid_timed_shutdown(bool forbid)
{
    msg_apps_t msg = {0};
    msg_reply_t reply = {0};
    bool cur_stat;

    app_info_t new_info = *g_this_app_info;

    if(!is_applib_inited())
    {
        return ;
    }

    new_info.timed_shutdown_forbidden = forbid;
    app_running_list_set_this_app_info(&new_info);

    /*
     * when timed_shutdown isn't forbidden in every process, we should notify the msg_server.
     */
    cur_stat = sys_is_timed_shutdown_forbidden();
    if(cur_stat == false)
    {
        msg.type = MSG_RESET_TIMED_SHUTDOWN_SYNC;

        send_sync_msg(APP_NAME_MSG_SERV, &msg, &reply, NULL);

        if(reply.type != MSG_REPLY_SUCCESS)
        {
            print_warning("reset the auto standby timer failed!");
        }
    }
    
    if(cur_stat == true)
    {
        filter_msg(MSG_POWER_OFF);
        filter_msg(MSG_SHORTCUT_RETURN);
    }
    else
    {
        unfilter_msg(MSG_POWER_OFF);
        unfilter_msg(MSG_SHORTCUT_RETURN);
    }
}


bool sys_set_auto_standby_idle_time(unsigned int time)
{
    msg_apps_t msg = {0};
    msg_reply_t reply = {0};
    system_settings_t sys_set = {0}; 

    if(!is_applib_inited())
    {
        return false;
    }

    msg.type = MSG_SET_AUTO_STANDBY_TIME_SYNC;
    sys_set.auto_standby_time = time; 
    memcpy(msg.content, &sys_set, sizeof(sys_set));

    send_sync_msg(APP_NAME_MSG_SERV, &msg, &reply, NULL);

    if(reply.type == MSG_REPLY_SUCCESS)
    {
        return true;
    }
    else
    {
        print_err("set auto standby idle time failed!");
        return false;
    }
}

bool sys_set_timed_shutdown_time(unsigned int time)
{
    msg_apps_t msg = {0};
    msg_reply_t reply = {0};
    system_settings_t sys_set = {0}; 

    if(!is_applib_inited())
    {
        return false;
    }

    msg.type = MSG_SET_TIMED_STUTDOWN_TIME_SYNC;
    sys_set.timed_shutdown_time = time; 
    memcpy(msg.content, &sys_set, sizeof(sys_set));

    send_sync_msg(APP_NAME_MSG_SERV, &msg, &reply, NULL);

    if(reply.type == MSG_REPLY_SUCCESS)
    {
        return true;
    }
    else
    {
        print_err("set auto standby idle time failed!");
        return false;
    }
}

bool sys_is_auto_standby_forbidden(void)
{
    app_running_list_t * list; 

    if(!is_applib_inited())
    {
        return false;
    }

    list = (app_running_list_t *)app_running_list_get_list();

    for(; list != NULL; list = list->next)
    {
        if(true == list->app_info->auto_standby_forbidden)
        {
            return true ;
        }
    }

    return false;
}

bool sys_is_timed_shutdown_forbidden(void)
{
    app_running_list_t * list;

    if(!is_applib_inited())
    {
        return false;
    }

    list = (app_running_list_t *)app_running_list_get_list();

    for(; list != NULL; list = list->next)
    {
        if(true == list->app_info->timed_shutdown_forbidden)
        {
            return true ;
        }
    }

    return false;
}

bool sys_set_lcd_backlight(unsigned int val)
{
    msg_apps_t msg = {0};
    msg_reply_t reply = {0};
    system_settings_t sys_set = {0}; 

    if(!is_applib_inited())
    {
        return false;
    }

    msg.type = MSG_SET_LCD_BACKLIGHT_SYNC;
    sys_set.lcd_backlight = val; 
    memcpy(msg.content, &sys_set, sizeof(sys_set));

    send_sync_msg(APP_NAME_MSG_SERV, &msg, &reply, NULL);

    if(reply.type == MSG_REPLY_SUCCESS)
    {
        return true;
    }
    else
    {
        print_err("set lcd backlight failed!");
        return false;
    }

}


bool sys_set_rtc_date_time(rtc_time_t *time, rtc_date_t *date)
{
    rtc_date_t tmp_date; 

    if( (time == NULL) && (date == NULL) )
    {
        return false;
    }

    if(s_rtc_dev_fd == 0)
    {
        if(sys_open_rtc_dev() == false)
        {
            return false;
        }
    }

    if( (time != NULL) && (ioctl(s_rtc_dev_fd, RTC_SET_TIME , (int)time) != 0 ) )
    {
        print_err("set rtc time failed!");
        return false;
    }

    if( date != NULL)
    {
        tmp_date = *date;

        if(tmp_date.year < s_min_date.year)
        {
            tmp_date = s_max_date;
        }

        if(tmp_date.year > s_max_date.year)
        {
            tmp_date = s_min_date;
        }

        if(ioctl(s_rtc_dev_fd, RTC_SET_DATE , (int)&tmp_date) != 0 )
        {
            print_err("set rtc date failed!");
            return false;
        }
    }
    
    _reset_alarm();

    return true;
}

bool sys_get_rtc_date_time(rtc_time_t *time, rtc_date_t *date)
{
    rtc_date_time_t date_time;

    if( (time == NULL) && (date == NULL) )
    {
        return false;
    }

    if(s_rtc_dev_fd == 0)
    {
        if(sys_open_rtc_dev() == false)
        {
            return false;
        }
    }

    if(ioctl(s_rtc_dev_fd, RTC_GET_DATE_TIME, (int)(&date_time)) != 0)
    {
        print_err("get rtc date and time failed!");
        return false;
    }
    else
    {
        if(date_time.date.year < s_min_date.year)
        {
            date_time.date = s_min_date;
            sys_set_rtc_date_time(&date_time.time, &date_time.date);
        }

        if(date_time.date.year > s_max_date.year)
        {
            date_time.date = s_min_date;
            sys_set_rtc_date_time(&date_time.time, &date_time.date);
        }

        if(time != NULL)
        {
            *time = date_time.time;
        }

        if(date != NULL)
        {
            *date = date_time.date;
        }

        return true;
    }
}

bool sys_set_rtc_drm_date_time(rtc_time_t *drm_time, rtc_date_t *drm_date)
{
    rtc_date_time_t date_time;

    if( (drm_time == NULL) && (drm_date == NULL) )
    {
        return false;
    }

    if(s_rtc_dev_fd == 0)
    {
        if(sys_open_rtc_dev() == false)
        {
            return false;
        }
    }

    if(ioctl(s_rtc_dev_fd, RTC_GET_DRM_DATE_TIME, (int)(&date_time)) != 0)
    {
        print_err("get drm rtc date and time failed!");
        return false;
    }
    else
    {
        if(drm_time != NULL)
        {
            date_time.time = *drm_time;
        }

        if(drm_date != NULL)
        {
            date_time.date = *drm_date;
        }

        if(ioctl(s_rtc_dev_fd, RTC_SET_DRM_DATE_TIME, (int)(&date_time)) != 0)
        {
            print_err("set drm rtc date and time failed!");
            return false;
        }

        return true;
    }
}

bool sys_get_rtc_drm_date_time(rtc_time_t *drm_time, rtc_date_t *drm_date)
{
    rtc_date_time_t date_time;

    if( (drm_time == NULL) && (drm_date == NULL) )
    {
        return false;
    }

    if(s_rtc_dev_fd == 0)
    {
        if(sys_open_rtc_dev() == false)
        {
            return false;
        }
    }

    if(ioctl(s_rtc_dev_fd, RTC_GET_DRM_DATE_TIME, (int)(&date_time)) != 0)
    {
        print_err("get drm rtc date and time failed!");
        return false;
    }
    else
    {
        if(drm_time != NULL)
        {
            *drm_time = date_time.time;
        }

        if(drm_date != NULL)
        {
            *drm_date = date_time.date;
        }

        return true;
    }
}

bool sys_rtc_change_to_stdsec(rtc_time_t *time, rtc_date_t *date, unsigned long * stdsec)
{
    rtc_date_time_t date_time;

    if( (time == NULL) || (date == NULL) || (stdsec == NULL))
    {
        return false;
    }

    if(s_rtc_dev_fd == 0)
    {
        if(sys_open_rtc_dev() == false)
        {
            return false;
        }
    }

    if(ioctl(s_rtc_dev_fd, RTC_CHANGE_TO_STDSEC, (int)(&date_time)) != 0)
    {
        print_err("change to standard second failed!");
        return false;
    }
    else
    {
        *stdsec = date_time.stdsec;
        return true;
    }
}

bool sys_open_rtc_dev(void)
{
    int dev_fd; 

    if(s_rtc_dev_fd > 0)
    {
        print_warning("%s is already opened!", DEVICE_RTC);
        return false;
    }

    dev_fd = open(DEVICE_RTC, O_RDWR);

    if (dev_fd < 0)
    {
        print_err("open %s failed!", DEVICE_RTC);
        return false;
    }

    s_rtc_dev_fd = dev_fd;

    return true;
}


bool sys_close_rtc_dev(void)
{
    if(s_rtc_dev_fd == 0)
    {
        print_warning("%s has not opened", DEVICE_RTC);
        return false;
    }


    if (close(s_rtc_dev_fd) < 0)
    {
        print_err("close %s failed!", DEVICE_RTC);
        return false;
    }

    s_rtc_dev_fd = 0;

    return true;
}

bool sys_set_key_tone_enable( int flag_enable )
{
    bool result = true;
    msg_apps_t msg;

    memset( &msg, 0x00, sizeof(msg) );
    msg.type = MSG_SET_KEY_TONE_ENABLE;
    memcpy( msg.content, &flag_enable, sizeof(flag_enable) );
    result = send_async_msg( APP_NAME_MSG_SERV, &msg);

    return result;
}

bool sys_enable_sys_evt_detect(bool on)
{
    msg_apps_t msg = {0};
    msg_reply_t reply = {0};

    msg.type = MSG_SET_SYS_EVT_DETECT_ON_SYNC;
    memcpy(msg.content, (void *)&on, sizeof(on));
    send_sync_msg(APP_NAME_MSG_SERV, &msg, &reply, NULL);
                 
    if(reply.type == MSG_REPLY_SUCCESS)
    {
        if(on == true)
        {
            release_msg(MSG_SHORTCUT_RETURN);
        }
        else
        {
            grab_msg(MSG_SHORTCUT_RETURN);
        }
        return true;
    }
    else
    {
        print_err("%s system event detect failed!", (on == true) ? "enable":"disable");
        return false;
    }
}

static bool  _reset_alarm(void)
{
    int fd = 0;

    fd = open(DEVICE_ALARM, O_RDWR);
    if(fd < 0)
    {
        print_err("open %s error!", DEVICE_ALARM);
        return false;
    }

    if(ioctl(fd, ALARM_RESET, 0) != 0)
    {
        print_err("reset alarm error");
        close(fd);
        return false;
    }

    close(fd);
    return true;
}
