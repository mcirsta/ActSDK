#include "include_case.h"

#include "recorder_scene_recording.h"

static bool _lib_init( void );
static bool _lib_deinit( void );

app_t app_data;

static bool _lib_init( void )
{
    app_data.applib_lib = dlopen("applib.so", RTLD_LAZY);
    if (app_data.applib_lib == NULL)
    {
        printf("open applib lib err! \n");
        goto _lib_init_err;
    }

    app_data.commonui_lib = dlopen("commonui.so", RTLD_LAZY);
    if (app_data.commonui_lib == NULL)
    {
        printf("open commonui lib err! \n");
        goto _lib_init_err;
    }

    app_data.fusion_lib = dlopen("fusion.so", RTLD_LAZY);
    if (app_data.fusion_lib == NULL)
    {
        printf("open fusion lib err! \n");
        goto _lib_init_err;
    }

    app_data.gui_lib = dlopen("gui.so", RTLD_LAZY);
    if (app_data.gui_lib == NULL)
    {
        printf("open gui lib err! \n");
        goto _lib_init_err;
    }

    app_data.sty_lib = dlopen("style.so", RTLD_LAZY);
    if (app_data.sty_lib == NULL)
    {
        printf("open style lib err! \n");
        goto _lib_init_err;
    }

    app_data.config_lib = dlopen("apconfig.so", RTLD_LAZY);
    if (app_data.config_lib == NULL)
    {
        print_err("open apconfig lib err!");
        goto _lib_init_err;
    }

    return true;

    _lib_init_err:
    _lib_deinit();

    return false;
}

static bool _lib_deinit( void )
{
    if (app_data.config_lib != NULL)
    {
        dlclose(app_data.config_lib);
        app_data.config_lib = NULL;
    }

    if (app_data.sty_lib != NULL)
    {
        dlclose(app_data.sty_lib);
        app_data.sty_lib = NULL;
    }

    if (app_data.gui_lib != NULL)
    {
        dlclose(app_data.gui_lib);
        app_data.gui_lib = NULL;
    }

    if (app_data.fusion_lib != NULL)
    {
        dlclose(app_data.fusion_lib);
        app_data.fusion_lib = NULL;
    }

    if (app_data.commonui_lib != NULL)
    {
        dlclose(app_data.commonui_lib);
        app_data.commonui_lib = NULL;
    }

    if (app_data.applib_lib != NULL)
    {
        dlclose(app_data.applib_lib);
        app_data.applib_lib = NULL;
    }

    return true;
}

/* application initial function */
static bool app_init(app_t *data)
{
    char file[PATH_MAX];

    /*打开style文件*/
    sprintf(file, "%s/%s.sty", sys_get_common_theme_res_dir(), "commonui");
    print_dbg("file = %s",file);
    data->common_style = open_style_file(file);
    if (NULL == data->common_style)
    {
        print_err("open open_style_file err! \n");
        goto COMMONUUI_ERR;
    }
    
    sprintf(file, "%s/%s.sty", sys_get_app_theme_res_dir(), "recorder");
    data->recorder_style = open_style_file(file);
    if (NULL == data->recorder_style)
    {
        print_err("open open_style_file err! \n");
        goto RECORDER_ERR;
    }
    
    memset(&data->scene_record_data, 0, sizeof(scene_recording_t));

    return true;

    RECORDER_ERR: 
    close_style_file(data->common_style);
    
    COMMONUUI_ERR: 
    return false;
}

/* application exit function */
static bool app_deinit(app_t *data)
{
    /*关闭style文件*/
    close_style_file(data->common_style);
    close_style_file(data->recorder_style);

    return true;
}

int main(int argc, const char *argv[])
{
    int ret = 0;

    print_info("record enter\n");

    if (_lib_init() == false)
    {
        print_err("lib init error!");
        return -1;
    }

    /* 应用库初始化 */
    applib_init(argc, argv, APP_TYPE_GUI);
    /*进入recorder，禁止按键音*/
    sys_set_key_tone_enable(0);

    /* 应用初始化 */
    if (app_init(&app_data) == false)
    {
        printf("app init error! \n");
        return -1;
    }
    else
    {
    }

    /*初始化应用第一个场景*/
    app_data.next_scene = SCENE_RECORDING;

    /* 进入场景 */
    while (app_data.next_scene != SCENE_EXIT)
    {
        ret = recorder_scene_recording(&(app_data.scene_record_data));
        if (ret == SCENE_RECORDING_NEXT)
        {
            print_dbg("ret = %d\n", ret);
            app_data.next_scene = SCENE_EXIT;
            print_dbg("next scene = %d\n", app_data.next_scene);
        }
    }

    /* 应用退出 */
    if (app_deinit(&app_data) == false)
    {
        printf("app exit error! \n");
        return -1;
    }
    else
    {
        print_dbg("app exit done! \n");
    }
    /*使能按键音*/
    sys_set_key_tone_enable(1);
    /* 应用库退出 */
    applib_quit();

    _lib_deinit();

    print_dbg(">>>>>>>>>>>>>>>>>>>>>> app return: %d\n", ret);

    return 0;
}
