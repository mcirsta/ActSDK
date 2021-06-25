/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : stopwatch_main.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *     dupengcheng    2009-04-28          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     stopwatch_main.c
 * \brief    stopwatch application
 * \author   dupengcheng
 *
 * \version 1.0
 * \date  2009/04/14
 *******************************************************************************/
#include "stopwatch_scene.h"

/* 应用频率调整 */
static const clk_domain_t s_clk_domain =
{ CLK_LOAD_MIN, CLK_LOAD_HIGH_0 };

typedef struct app_s
{
    /* system resource */
    void *applib_lib;
    void *commonui_lib;
    void *fusion_lib;
    void *gui_lib;
    void *sty_lib;
    void *config_lib;
} app_t;

style_t* app_style;
style_t* commonui_style;


static bool _lib_init(app_t *data);
static bool _lib_deinit(app_t *data);
static void _load_style(void);
static void _unload_style(void);

static void _load_style(void)
{
    char file[PATH_MAX];

    /* 获取commonui 的 style 资源文件名 */
    sprintf(file, "%s/%s.sty", sys_get_common_theme_res_dir(), "commonui");
    commonui_style = open_style_file(file );

    /*获取应用的 style 资源文件名*/
    sprintf(file, "%s/%s.sty", sys_get_app_theme_res_dir(), "watch");
    app_style = open_style_file(file );
}

static void _unload_style(void)
{
    close_style_file(commonui_style);
    close_style_file(app_style);  
}

static bool _lib_init(app_t *data)
{

    data->applib_lib = dlopen("applib.so", RTLD_LAZY);
    if (data->applib_lib == NULL)
    {
        print_err("open applib lib err! \n");
        goto _lib_init_err;
    }

    data->commonui_lib = dlopen("commonui.so", RTLD_LAZY);
    if (data->commonui_lib == NULL)
    {
        print_err("open commonui lib err! \n");
        goto _lib_init_err;
    }

    data->fusion_lib = dlopen("fusion.so", RTLD_LAZY);
    if (data->fusion_lib == NULL)
    {
        print_err("open fusion lib err! \n");
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

    data->config_lib = dlopen("apconfig.so", RTLD_LAZY);
    if (data->config_lib == NULL)
    {
        print_err("open apconfig lib err!");
        goto _lib_init_err;
    }

    return true;

    _lib_init_err:
    _lib_deinit(data);

    return false;
}

/* application exit function */
static bool _lib_deinit(app_t *data)
{
    if (data->config_lib != NULL)
    {
        dlclose(data->config_lib);
        data->config_lib = NULL;
    }

    if (data->sty_lib != NULL)
    {
        dlclose(data->sty_lib);
        data->sty_lib = NULL;
    }

    if (data->gui_lib != NULL)
    {
        dlclose(data->gui_lib);
        data->gui_lib = NULL;
    }

    if (data->fusion_lib != NULL)
    {
        dlclose(data->fusion_lib);
        data->fusion_lib = NULL;
    }
    if (data->commonui_lib != NULL)
    {
        dlclose(data->commonui_lib);
        data->commonui_lib = NULL;
    }

    if (data->applib_lib != NULL)
    {
        dlclose(data->applib_lib);
        data->applib_lib = NULL;
    }

    return true;
}

/* application main function */
int main(int argc, char *argv[])
{
    app_t app_data;
    fusion_enter_e enter_type;
    int ret = 0;

    /* 应用初始化 */
    if (_lib_init(&app_data) == false)
    {
        print_err("app init error! \n");
        return -1;
    }

    /* 应用库初始化 */
    applib_init(argc, argv, APP_TYPE_GUI);

    /* 应用调频 */
    sys_pm_set_clk_domain(&s_clk_domain);

    /*load style 资源文件*/
    _load_style();
    
    enter_type = FUSION_FORWARD;
    ret = stopwatch_scene(enter_type);

    _unload_style();
    
    /* 应用退出 */
    
    /* 清除频率设置 */
    sys_pm_clear_clk_domain(&s_clk_domain);

    /* 应用库退出 */
    applib_quit();
    _lib_deinit(&app_data);

    return 0;
}
