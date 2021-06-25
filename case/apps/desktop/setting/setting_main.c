/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * file : setting_main.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-5-4             v1.0             build this file 
 ********************************************************************************/

#include <include_case.h>

#include "setting_scene_list.h"

typedef struct app_setting_s
{
    /* system resource */
    void *applib_lib;
    void *commonui_lib;
    void *fusion_lib;
    void *gui_lib;
    void *sty_lib;
    void *config_lib;

    /* 场景退出后进入的下一场景 */
    scene_status_t next_scene;

}app_setting_t;

static bool _lib_init(app_setting_t *data);
static bool _lib_deinit(app_setting_t *data);

static const clk_domain_t s_clk_domain_high = { CLK_LOAD_MIN, CLK_LOAD_HIGH_0 };

static bool _lib_init(app_setting_t *data)
{
    data->applib_lib = dlopen("applib.so", RTLD_LAZY);
    if (data->applib_lib == NULL)
    {
        print_err("open applib lib err!");
        goto _lib_init_err;
    }

    data->commonui_lib = dlopen("commonui.so", RTLD_LAZY);
    if (data->commonui_lib == NULL)
    {
        print_err("open commonui lib err!");
        goto _lib_init_err;
    }

    data->fusion_lib = dlopen("fusion.so", RTLD_LAZY);
    if (data->fusion_lib == NULL)
    {
        print_err("open fusion lib err!");
        goto _lib_init_err;
    }

    data->gui_lib = dlopen("gui.so", RTLD_LAZY);
    if (data->gui_lib == NULL)
    {
        print_err("open gui lib err!");
        goto _lib_init_err;
    }

    data->sty_lib = dlopen("style.so", RTLD_LAZY);
    if (data->sty_lib == NULL)
    {
        print_err("open style lib err!");
        goto _lib_init_err;
    }

    data->config_lib = dlopen("apconfig.so", RTLD_LAZY);
    if (data->config_lib == NULL)
    {
        print_err("open apconfig lib err!");
        goto _lib_init_err;
    }

    return true;

    /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< goto! <<<<<<<<<<<<<<<< goto! <<<<<<<< goto! <<<< goto! << */
    _lib_init_err:
    _lib_deinit(data);

    return false;
}

static bool _lib_deinit(app_setting_t *data)
{
    if(data->config_lib != NULL)
    {
        dlclose(data->config_lib);
        data->config_lib = NULL;
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

    if(data->fusion_lib != NULL)
    {
        dlclose(data->fusion_lib);
        data->fusion_lib = NULL;
    }

    if(data->commonui_lib != NULL)
    {
        dlclose(data->commonui_lib);
        data->commonui_lib = NULL;
    }

    if(data->applib_lib != NULL)
    {
        dlclose(data->applib_lib);
        data->applib_lib = NULL;
    }

    return true;
}

int main(int argc, const char *argv[])
{
    app_setting_t app_data;
    scene_list_t scene_list = {0};

    print_dbg(">>>>>>>>>>>>>>>> enter setting");

    if(_lib_init(&app_data) == false)
    {
        print_err("app init error!");
        return -1;
    }

    if(applib_init(argc, argv, APP_TYPE_GUI) == false)
    {
        print_err("applib init error!");
        goto applib_init_err;
    }

    sys_pm_set_clk_domain(&s_clk_domain_high);

    app_data.next_scene = SCENE_LIST;

    while(app_data.next_scene != SCENE_EXIT)
    {
        switch (app_data.next_scene)
        {
            case SCENE_LIST:
            {
                app_data.next_scene = setting_scene_list(&scene_list);
                break;
            }
            default:
            {
                break;
            }
        }
    }

    

    sys_pm_clear_clk_domain(&s_clk_domain_high);

    if(applib_quit() == false )
    {
        print_warning("applib quit failed!");
    }

    if (_lib_deinit(&app_data) == false)
    {
        print_warning("app deinit failed!");
    }

    print_dbg("<<<<<<<<<<<<<<< exit setting");

    return 0;

    /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< goto! <<<<<<<<<<<<<<<< goto! <<<<<<<< goto! <<<< goto! << */
    applib_init_err:

    _lib_deinit(&app_data);
 
    return -1;
}
