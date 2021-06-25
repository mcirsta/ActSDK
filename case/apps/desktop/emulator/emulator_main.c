/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : test_main.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *     dupengcheng    2009-04-28          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     emulator_main.c
 * \brief    test_scene application
 * \author   dupengcheng
 *
 * \version 1.0
 * \date  2009/04/14
 *******************************************************************************/
#include "emulator_scene.h"
#include "em_engine.h"

extern char romFullPath[];

/*game的种类: 0=NES[default],1=GBC/GB,2=SFC,3=MD,4=G-Game*/
int gameType = 0;
char game_kind[128];
int start_byFileNavi = 0;//游戏是否由浏览器启动 1：由浏览器启动 0：正常启动

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

static bool app_init(app_t *data);
static bool app_deinit(app_t *data);
static void _load_style(void);
static void _unload_style(void);

static void _load_style(void)
{
    char file[PATH_MAX];

    /* 获取commonui 的 style 资源文件名 */
    sprintf(file, "%s/%s.sty", sys_get_common_theme_res_dir(), "commonui");
    commonui_style = open_style_file(file );

    /*获取应用的 style 资源文件名*/
    sprintf(file, "%s/%s.sty", sys_get_app_theme_res_dir(), "emulator");
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
        print_err("open applib err! \n");
        goto _lib_init_err;
    }

    data->commonui_lib = dlopen("commonui.so", RTLD_LAZY);
    if (data->commonui_lib == NULL)
    {
        print_err("open commonui  lib err! \n");
        goto _lib_init_err;
    }

    data->fusion_lib = dlopen("fusion.so", RTLD_LAZY);
    if (data->fusion_lib == NULL)
    {
        print_err("open fusion  lib err! \n");
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

static bool _lib_deinit(app_t *data)
{
    if (data->sty_lib != NULL)
    {
        dlclose(data->sty_lib);
        data->sty_lib = NULL;
    }

    if (data->config_lib != NULL)
    {
        dlclose(data->config_lib);
        data->config_lib = NULL;
    }

    if (data->gui_lib != NULL)
    {
        dlclose(data->gui_lib);
        data->gui_lib = NULL;
    }

    if (data->applib_lib != NULL)
    {
        dlclose(data->applib_lib);
        data->applib_lib = NULL;
    }

    if (data->commonui_lib != NULL)
    {
        dlclose(data->commonui_lib);
        data->commonui_lib = NULL;
    }

    if (data->fusion_lib != NULL)
    {
        dlclose(data->fusion_lib);
        data->fusion_lib = NULL;
    }

    return true;
}

static bool app_init(app_t *data)
{
    /* 加载声音、图像驱动 */
    em_mount_drv();
    return true;
}

/* application exit function */
static bool app_deinit(app_t *data)
{
    /* 卸载声音、图像驱动 */
    em_umount_drv();
    return true;
}

/* 应用频率调整 */
static const clk_domain_t s_clk_domain =
{ CLK_LOAD_MIN, CLK_LOAD_HIGH_0};
/* application main function */
int main(int argc, char *argv[])
{
    app_t app_data;
    int ret = 0;
    
    memset(&app_data,0,sizeof(app_t));

    _lib_init(&app_data);

    /* 应用库初始化 */
    applib_init(argc, argv, APP_TYPE_GUI);
    /* 应用调频 */
    sys_pm_set_clk_domain(&s_clk_domain);

    //print_info("\n\nargc=%d \nargv[0]=%s \nargv[1]=%s \nargv[2]=%s\n\n", argc, argv[0], argv[1], argv[2]);
    /*判断游戏种类:NES,GBC,SFC,MD,G-Game*/
    print_dbg("argc=%d, argv[1]=%s\n", argc, argv[1]);
    memset(game_kind, 0, sizeof(game_kind));
    if ((strcmp(argv[1], "NES") == 0) || (strcmp(argv[1], "GBC") == 0) || (strcmp(argv[1], "SFC") == 0) || (strcmp(
            argv[1], "MD") == 0) || (strcmp(argv[1], "G-Game") == 0))
    {
        strcpy(game_kind, argv[1]);
        gameType = 1; 
        if (strcmp(game_kind, "GBC") == 0)
        {
            strcpy(game_kind, "gbc|gb");
            gameType = 3; 
        }
        if (strcmp(game_kind, "SFC") == 0)
        {
            strcpy(game_kind, "smc|sfc");
            gameType = 2; 
        }
        if (strcmp(game_kind, "MD") == 0)
        {
            strcpy(game_kind, "smd|mdx|bin");
            gameType = 4; 
        }
    }
    else if (strcmp(argv[1], "playing") == 0)
    {
        start_byFileNavi = 1;
        strcpy(romFullPath, argv[2]);
    }
    else
    {
        goto applib_init_quit;
    }


    /* 应用初始化 */
    if (app_init(&app_data) == false)
    {
        print_err("app init error! \n");
        goto applib_init_quit;
    }

    /*load style 资源文件*/
    _load_style();
    
    
    ret = emulator_scene();

    mq_unlink(EMULATOR_SIMCOMMON_FIFO);
    mq_unlink(EMULATOR_SIMCOMMON_SYN);
    mq_unlink(EMULATOR_SIMCOMMON_KEYMSG);
    
    _unload_style();

    /* 应用退出 */
    if (app_deinit(&app_data) == false)
    {
        print_err("app exit error! \n");
        goto applib_init_quit;
    }
    
        /* 应用库退出 */
    applib_init_quit:

    /* 清除频率设置 */
    sys_pm_clear_clk_domain(&s_clk_domain);
    /* 应用库退出 */
    applib_quit();

    _lib_deinit(&app_data);
    return 0;
}
