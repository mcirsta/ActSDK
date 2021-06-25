/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : launcher_main.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *       chencm    2009-04-22          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     launch_main.c
 * \brief
 * \author   chencm
 *
 * \version 1.0
 * \date  2009/04/22
 *******************************************************************************/

#include <include_case.h>
#include "launcher.h"
#include "launcher_scene_desktop.h"
#include "launcher_scene_launch.h"


typedef struct app_launcher_s
{
    /* system resource */
    void * fs_lib;
    void * gui_lib;
    void * sty_lib;
    void * fusion_lib;
    void * apconfig_lib;
    void * scene_desktop_lib;

    /* whole desktop info list */
    appinfo_list_t * desktop_list;

    scene_t *common_scene; //common scene

    scene_desktop_t scene_desktop_data; //desktop scene data
    scene_launch_t scene_launch_data; //launch scene data

    /* 场景退出后进入的下一场景 */
    scene_status_e next_scene;

} app_launcher_t;

static app_launcher_t app_data =
{ 0 };
int debug_time;
desktop_parser_handle dp;

/*******************************************************************/
static bool _app_init(app_launcher_t *data);
static bool _app_deinit(app_launcher_t *data);

/*******************************************************************/

static void *applib_lib = NULL;
static void *commonui_lib = NULL;

bool _applib_init(int argc, const char *argv[], app_type_e type)
{

    applib_lib = dlopen("applib.so", RTLD_NOW);
    if (applib_lib == NULL)
    {
        print_err("open applib lib err");
        goto applib_err;
    }

//    if( type == APP_TYPE_GUI )
//    {
//        commonui_lib = dlopen("commonui.so", RTLD_LAZY);
//        if (commonui_lib == NULL)
//        {
//            print_err("open commonui_lib err!");
//            goto commonui_lib_err;
//        }
//    }

    return applib_init(argc, argv, type);

//commonui_lib_err:
//    dlclose(applib_lib);
//    applib_lib = NULL;
    applib_err:
    return false;
}

bool _applib_quit(void)
{
    bool result;
    result = applib_quit();
    if((applib_lib != NULL) && (dlclose(applib_lib) < 0))
    {
        print_warning("close applib lib failed!");
        result = false;
    }

//    if((commonui_lib != NULL) && (dlclose(commonui_lib) < 0))
//    {
//        print_warning("close commonui lib failed!");
//        result = false;
//    }
//
    return result;
}

/* application initial function */
static bool _app_init(app_launcher_t *data)
{
    // open shared libraries
    /*
    data->fs_lib = dlopen("libc_fs.so", RTLD_LAZY);
    if (NULL == data->fs_lib)
    {
    print_err("open gui lib err!");
    return false;
    }
    */

    data->fusion_lib = dlopen("fusion.so", RTLD_LAZY);
    if( NULL == data->fusion_lib)
    {
        print_err("open fusion lib err!");
        return false;
    }

    data->sty_lib = dlopen("style.so", RTLD_LAZY);
    if (NULL == data->sty_lib)
    {
        print_err("open style lib err!");
        return false;
    }

    data->apconfig_lib = dlopen("apconfig.so", RTLD_LAZY);
    if (NULL == data->apconfig_lib)
    {
        print_err("open style lib err!");
        return false;
    }

    return true;
}

/* application exit function */
static bool _app_deinit(app_launcher_t *data)
{
    bool ret = false;

    /* close shared libraries */
    dlclose(data->apconfig_lib);
    dlclose(data->sty_lib);
    dlclose(data->fusion_lib);
    //dlclose(data->fs_lib);

    ret = true;
    return ret;
}

/* */
//void _get_desktop_so_name()
//{
//    int i = 0;
//    const char * cur_theme;
//
//    //get current style dir name
//    cur_theme = sys_get_cur_theme_name();
//
//    // make the current style file full name.
//    //For example: "/mnt/diska/apps/launcher/" + "qvga_style_2" + "/desktop.so"
//    strncpy(desktop_so_name, APP_NAME_LAUNCHER, sizeof(APP_NAME_LAUNCHER));
//    dirname(desktop_so_name);
//    snprintf(desktop_so_name, sizeof(desktop_so_name), "%s/%s/desktop.so", desktop_so_name, cur_theme);
//
//    //    //for test: cur_theme = desktop_so_map[1].theme_name;
//    //    for (i = 0; i < THEME_NUM; i++)
//    //    {
//    //        if (0 == strcasecmp(desktop_so_map[i].theme_name, cur_theme))
//    //        {
//    //            return desktop_so_map[i].desktop_so_name;
//    //        }
//    //    }
//    //
//    //    return desktop_so_map[0].desktop_so_name;
//}

/* application main function */
int main(int argc, const char *argv[])
{
    bool result = false;
    int focus_appid = 0;
    const char * cur_theme;
    char desktop_so_name[FULL_FILENAME_LEN];

//    applib_init(argc, argv, APP_TYPE_GUI); //applications libs initial
    _applib_init(argc, argv, APP_TYPE_GUI); //applications libs initial
    usleep(10000);

    result = _app_init(&app_data);
    if (result == false)
    {
        print_warning("init app failed!");
        return -1;
    }

    if (argc > 1)
    {
        // if load launcher by console then desktop scene is background.
        if (!strcasecmp(argv[1], LAUNCHER_ARGV_CONSOLE))
        {
            app_data.scene_desktop_data.is_console = true;
        }
        else
        {
            app_data.scene_desktop_data.is_console = false;
        }
    }
    else
    {
        app_data.scene_desktop_data.is_console = false;
    }

    //初始化应用第一个场景（Desktop）
    app_data.next_scene = SCENE_DESKTOP;

    //获取desktop 信息列表
    get_config(CFG_FOCUS_APPID, (char *) &focus_appid, sizeof(focus_appid));
    app_data.desktop_list = appinfo_list_create(focus_appid);

    if (NULL != app_data.desktop_list)
    {

        app_data.scene_desktop_data.cur_desktop_list = app_data.desktop_list;
        app_data.scene_desktop_data.cur_desktop = app_data.desktop_list;

        /* 进入场景 */
        while (app_data.next_scene != SCENE_EXIT)
        {
            switch (app_data.next_scene)
            {
                case SCENE_DESKTOP:
                {
                    commonui_lib = dlopen("commonui.so", RTLD_LAZY);
                    if (commonui_lib == NULL)
                    {
                        print_err("open desktop commonui err!");
                        //return false;
                        app_data.next_scene = SCENE_EXIT;
                        break;
                    }

                    //get current style dir name
                    cur_theme = sys_get_cur_theme_name();
                    // make the current style file full name.
                    // For example: "/mnt/diska/apps/launcher/" + "qvga_style_2" + "/desktop.so"
                    strncpy(desktop_so_name, APP_NAME_LAUNCHER, sizeof(APP_NAME_LAUNCHER));
                    dirname(desktop_so_name);
                    snprintf(desktop_so_name, sizeof(desktop_so_name), "%s/%s/desktop.so", desktop_so_name, cur_theme);
                    print_dbg("--Launcher: use %s Style File. \n", desktop_so_name);

                    //open current style .so file.
                    app_data.scene_desktop_lib = dlopen(desktop_so_name, RTLD_LAZY);
                    if (NULL == app_data.scene_desktop_lib)
                    {
                        print_err("open desktop lib err!");
                        //return false;
                        app_data.next_scene = SCENE_EXIT;
                        break;
                    }

                    app_data.next_scene = launcher_scene_desktop(&app_data.scene_desktop_data);
                    if (SCENE_LAUNCH_APP == app_data.next_scene)
                    {
                        app_data.scene_launch_data.launch_app = app_data.scene_desktop_data.cur_desktop;
                    }
                    else
                    {
                        app_data.scene_launch_data.launch_app = NULL;
                    }

                    dlclose(app_data.scene_desktop_lib);
                    if((commonui_lib != NULL) && (dlclose(commonui_lib) < 0))
                    {
                        print_warning("close commonui lib failed!");
                    }

                }
                break;
                case SCENE_LAUNCH:
                case SCENE_LAUNCH_APP:
                {
                    app_data.next_scene = launcher_scene_launch(&app_data.scene_launch_data);

                    //更换语言，更新当前语言下的应用名。
                    if (language_changed == true)
                    {
                        //app_data.scene_desktop_data.language_changed = language_changed;
                        appinfo_list_update_name(app_data.scene_desktop_data.cur_desktop_list);
                        language_changed = false;
                    }
                    app_data.scene_desktop_data.app_exited = app_exited;
                }
                break;

                default:
                break;
            }
        }

        focus_appid = app_data.scene_desktop_data.focus_app_id;
        set_config(CFG_FOCUS_APPID, (char *) &(focus_appid), sizeof(focus_appid));

        if (appinfo_list_delete(app_data.desktop_list) == false)
        {
            print_err("Delete appinfo_list false!");
        }
    }
    else
    {
        print_err("AppInfo_list create false! Exit launcher!");
    }
    _app_deinit(&app_data);
    _applib_quit();
    print_dbg("<<<<<<<<<<<<<<< exit launcher");
    return 0;
}
