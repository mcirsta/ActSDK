/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : browser_main.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *    binannan    2009-05-25          v1.0               build this file
 ********************************************************************************/
#include "include_case.h"
#include "include_psp.h"

#include "browser.h"
#include "browser_scene_browsing.h"

/*
 * 局部静态函数列表
 */
/********************************************************************/
static bool _app_init(app_browser_t *data);

static bool _app_deinit(app_browser_t *data);


static bool _lib_init(app_browser_t *data);

static bool _lib_deinit(app_browser_t *data);

/********************************************************************/
app_browser_t* g_app_browser = NULL;

bool g_resume_boot = false;

static app_browser_t s_app_data;

static clk_domain_t s_browser_clk = {CLK_LOAD_LOW_1,CLK_LOAD_HIGH_0};

/********************************************************************/

static bool _app_init(app_browser_t *data)
{
    char file[PATH_MAX];

    sys_pm_set_clk_domain(&s_browser_clk);

    sprintf(file, "%s/%s.sty", sys_get_common_theme_res_dir(), "commonui");

    data->commonui_style = open_style_file(file );

    sprintf(file, "%s/%s.sty", sys_get_app_theme_res_dir(), "browser");

    data->browser_style = open_style_file(file );

    return true;


}

/* application exit function */
bool _app_deinit(app_browser_t *data)
{
    sys_pm_clear_clk_domain(&s_browser_clk);

    close_style_file(data->commonui_style);
    close_style_file(data->browser_style);


    return true;
}

static bool _lib_init(app_browser_t *data)
{
    data->applib_lib = dlopen("applib.so", RTLD_LAZY);
    if (data->applib_lib == NULL)
    {
        print_err("open applib err! \n");
        goto _lib_init_err;
    }

    data->commonui_lib = dlopen("commonui.so", RTLD_LAZY);
    if (data->commonui_lib == NULL)
    {
        print_err("open commonui err! \n");
        goto _lib_init_err;
    }

    data->fusion_lib = dlopen("fusion.so", RTLD_LAZY);
    if (data->fusion_lib == NULL)
    {
        print_err("open fusion err! \n");
        goto _lib_init_err;
    }

    data->gui_lib = dlopen("gui.so", RTLD_LAZY);
    if (data->gui_lib == NULL)
    {
        print_err("open gui lib err! \n");
        goto _lib_init_err;
    }

    data->sty_lib = dlopen("style.so", RTLD_LAZY);
    if (data->sty_lib == NULL)
    {
        print_err("open style lib err! \n");
        goto _lib_init_err;
    }
    data->apconfig_lib = dlopen("apconfig.so", RTLD_LAZY);
    if (data->apconfig_lib == NULL)
    {
        print_err("open apconfig lib err! \n");
        goto _lib_init_err;
    }

    return true;

    _lib_init_err:
    _lib_deinit(data);
    return false;
}

static bool _lib_deinit(app_browser_t *data)
{
    if(data->applib_lib != NULL)
    {
        dlclose(data->applib_lib);
        data->applib_lib = NULL;
    }
    if(data->commonui_lib != NULL)
    {
        dlclose(data->commonui_lib);
        data->commonui_lib = NULL;
    }
    if(data->fusion_lib != NULL)
    {
        dlclose(data->fusion_lib);
        data->fusion_lib = NULL;
    }

    if(data->apconfig_lib != NULL)
    {
        dlclose(data->apconfig_lib);
        data->apconfig_lib = NULL;
    }
    if(data->sty_lib != NULL)
    {
        dlclose(data->sty_lib);
        data->sty_lib = NULL;
    }
    if(data->gui_lib != NULL)
    {
        dlclose(data->gui_lib);
        data->gui_lib = NULL;
    }
    return true;
}

int main(int argc, const char *argv[])
{
    bool ret = false;

    fusion_enter_e enter_type = FUSION_FORWARD;


    if(argc ==2 )
    {
        if(strcmp(argv[1],"resume") == 0)
        {
            g_resume_boot = true;
        }
    }

    ret = _lib_init(&s_app_data);
    if(ret ==false)
    {
        print_err("lib init error!");
        goto lib_init_err;
    }

    /* 应用库初始化 */
    ret = applib_init(argc, argv, APP_TYPE_GUI);
    if(ret ==false)
    {
        print_err("applib init error!");
        goto applib_init_err;
    }

    /* 应用初始化 */
    ret = _app_init(&s_app_data);
    if (ret == false)
    {
        print_err("app init error!\n");
        goto app_init_err;
    }
    else
    {
        g_app_browser = &s_app_data;
    }

    //初始化应用第一个场景
    s_app_data.next_scene = SCENE_BROWSING;

    /* 进入场景调度循环 */
    while (s_app_data.next_scene != SCENE_EXIT)
    {
        switch (s_app_data.next_scene)
        {
            case SCENE_BROWSING:
            {
                s_app_data.next_scene = browser_scene_browsing(enter_type);
            }
            break;

            default:
            {
                s_app_data.next_scene = SCENE_EXIT;
            }
            break;
        }
    }
    /* 应用析构 */
    ret = _app_deinit(&s_app_data);
    if (ret == false)
    {
        print_err("app exit error! \n");
    }
    /* 应用库退出 */
    app_init_err:
    applib_quit();

    applib_init_err:
    _lib_deinit(&s_app_data);

    lib_init_err:
    return 0;
}
