/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : launcher_scene_launch.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      chencm    2009-04-22          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     launcher_scene_launch.c
 * \brief    main menu scene
 * \author   chencm
 *
 * \version 1.0
 * \date  2009/04/22
 *******************************************************************************/

#include "launcher_scene_launch.h"
#include "launcher.h"

/* global variable define */
static scene_launch_t* g_scene_launch = NULL;
bool language_changed = false;
bool app_exited = false;

static bool poweroff_flag = false;

static int _scene_launch_proc_sys(msg_apps_t *msg);
static bool _launch_app(appinfo_list_t * desktop_info);
//static int scene_launch_quit();

/* scene quit */
//static int scene_launch_quit()
//{
//    exit_msg_loop();
//    g_scene_desktop.exit_type = SCENE_LAUNCH;
//}


static int _scene_launch_proc_sys(msg_apps_t *msg)
{
    int value = 0;
    msg_apps_t m = {0};

    print_dbg("msg.type:%d\n", msg->type);
    switch (msg->type)
    {
        case MSG_ACTIVE_SYNC:
        {
            msg_reply_t t;
            t.type = MSG_REPLY_SUCCESS;
            reply_sync_msg(&t);
            app_exited = false;
        }
        break;

        case MSG_POWER_OFF:
        {
            poweroff_flag = true;
        }
        break;

        case MSG_SET_APP_FOREGROUND: //add case for app exit msg from process manager
        {
            //enter_foreground_mode();
            if(get_config(CFG_NEED_GEN_PLIST, (char *)&value, sizeof(value)) < 0)
            {
                print_err("get_config(CFG_NEED_GEN_PLIST) failed!");
                value = 0;
            }

            if( (value == 1) && (poweroff_flag == false) )
            {
                m.type = MSG_CREAT_APP;
                strcpy(m.content, APP_NAME_PL_GEN);
                send_async_msg(APP_NAME_MANAGER, &m);
            }
            else
            {
                debug_time = 0;
                //DUMP_TIME();
                app_exited = true;
                g_scene_launch->exit_type = SCENE_DESKTOP;
                exit_msg_loop();
            }
        }
        break;

        case MSG_LANGUAGE_CHANGED:
        {
            language_changed = true;
        }
        break;

        case MSG_APP_QUIT:
        {
            exit_msg_loop();
            g_scene_launch->exit_type = SCENE_EXIT;
        }
        break;

        default:

        break;
    }
    return 0;
}

/*
 APP Launch function
 */
static bool _launch_app(appinfo_list_t * desktop_info)
{
    msg_apps_t app_msg;
    msg_reply_t reply;
    char app_dir[FULL_FILENAME_LEN] =
    { '\0' };

    if (NULL == desktop_info)
    {
        print_warning("input paramter(desktop_info) is a NULL pointer!");
        return false;
    }

    if (NULL == desktop_info->value[DKEY_EXEC])
    {
        print_warning("can not launch a NULL executable name!");
        return false;
    }

    print_info("launch %s", desktop_info->value[DKEY_EXEC]);
    //enter_background_mode();

    app_msg.type = MSG_CREAT_APP_SYNC;
    strcpy(app_dir, desktop_info->filename);
    dirname(app_dir);
    if ((strlen(app_dir) + strlen(desktop_info->value[DKEY_EXEC])) < sizeof(app_msg.content))
    {
        snprintf(app_msg.content, sizeof(app_msg.content), "%s/%s", app_dir, desktop_info->value[DKEY_EXEC]);
    }
    else
    {
        print_err();
    }
    //strncpy(app_msg.content, desktop_info->value[DKEY_EXEC], FULL_FILENAME_LEN);
    //    if(desktop_info->arg != NULL)
    //    {
    //        strcat(app_msg.content, " ");
    //        strcat(app_msg.content, desktop_info->arg);
    //    }
    send_sync_msg("manager.app", &app_msg, &reply, NULL);

    if (MSG_REPLY_SUCCESS == reply.type)
    {
        print_info("Launch %s APP successed!", app_msg.content);

        //print_dbg("exit launcher");
        //g_scene_launch.ret_type = SCENE_LAUNCH_EXIT;
        // exit_msg_loop();

        return true;
    }
    else
    {
        print_err("Launch %s APP failed!", app_msg.content);

        print_dbg("go to main menu");

        //enter_foreground_mode();
        g_scene_launch->exit_type = SCENE_DESKTOP;
        exit_msg_loop();

        return false;
    }
}

/* scene entry function */
int launcher_scene_launch(scene_launch_t * data)
{
    msg_apps_t msg;

    print_dbg(">>> enter scene");

    g_scene_launch = data;

    if (NULL != data->launch_app)
    {
        _launch_app(data->launch_app);
    }

    /* 场景消息循环 */
    register_sys_dispatcher(_scene_launch_proc_sys);
    while (get_msg(&msg) == true)
    {
        dispatch_msg(&msg);
    }

    unregister_sys_dispatcher();

    print_dbg(">>> exit scene");

    return g_scene_launch->exit_type;
}

