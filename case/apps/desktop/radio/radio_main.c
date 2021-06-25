/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : radio_main.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *     cheason    2009-04-23          v1.0               build this file
 ********************************************************************************/
/*!
* \file     radio_main.c
* \brief    radio application main entry
* \author   cheason
*
* \version 1.0
* \date  2009/04/23
*******************************************************************************/

#include "radio_app.h"
#include "radio_scene_playing.h"
#include "radio_scene_recording.h"


radio_app_t app_data;

static bool _app_init(radio_app_t *data);
static bool _app_deinit(radio_app_t *data);

static bool _lib_init( void );
static bool _lib_deinit( void );


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

    app_data.sqlite_lib = dlopen("sqlite.so", RTLD_LAZY);
    if (app_data.sqlite_lib == NULL)
    {
        print_err("load sqlite.so failed.\n");
        goto _lib_init_err;
    }

    app_data.mnavi_lib = dlopen("mnavi.so", RTLD_LAZY);
    if (app_data.mnavi_lib == NULL)
    {
        print_err("load mnavi.so failed.\n");
        goto _lib_init_err;
    }

    return true;

    _lib_init_err:
    _lib_deinit();

    return false;
}

static bool _lib_deinit( void )
{
    if (app_data.sqlite_lib != NULL)
    {
        dlclose(app_data.sqlite_lib);
        app_data.sqlite_lib = NULL;
    }

    if (app_data.mnavi_lib != NULL)
    {
        dlclose(app_data.mnavi_lib);
        app_data.mnavi_lib = NULL;
    }

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
static bool _app_init(radio_app_t *data)
{
    
    char file[512];
    const char * theme_dir = NULL;
    
    theme_dir = sys_get_common_theme_res_dir();
    sprintf(file, "%s/%s.sty", theme_dir, "commonui");
    data->commonui_style=open_style_file(file);
    if ( NULL == data->commonui_style )
    {
        print_err("open open_style_file err! \n");
        return false;
    }
    
    theme_dir=sys_get_app_theme_res_dir();
    sprintf(file, "%s/%s.sty", theme_dir, "radio");

    data->app_style = open_style_file( file);
    if ( NULL == data->app_style )
    {
        print_err("open open_style_file err! \n");
        return false;
    }
    
    return true;
}


/* application exit function */
static bool _app_deinit(radio_app_t *data)
{
    if(g_scene_playing != NULL)
    {
        free(g_scene_playing);
        g_scene_playing = NULL;
    }
    close_style_file( data->commonui_style );
    close_style_file( data->app_style );
    
    return true;
}


/* application main function */
int main(int argc, const char *argv[])
{
   
    bool ret;
    fusion_enter_e fenter;

    print_dbg("enter radio application\n");

    ret = _lib_init();
    if(ret == false)
    {
        print_err("lib init error!");
        return -1;
    }

    /* 应用库初始化 */
    applib_init(argc, argv, APP_TYPE_GUI);

    /* 应用初始化 */
    ret = _app_init(&app_data);
    if(ret == false)
    {
        print_err("app init error! \n");
        goto _app_init_err;  
    }
    else
    {
    }

    /*初始化应用第一个场景（列表）*/
    fenter = FUSION_FORWARD;
    app_data.next_scene = RADIO_SCENE_PLAYING_FORWARD;

    /* 进入场景 */
    while (app_data.next_scene != RADIO_SCENE_EXIT)
    {
        app_data.main_disk = sys_get_main_disk();
        
        switch( app_data.next_scene )
        {
        case RADIO_SCENE_PLAYING_FORWARD:
            app_data.next_scene = radio_scene_playing(fenter);
            if(app_data.next_scene == RADIO_SCENE_RECORDING_FORWARD)
            {
                fenter = FUSION_FORWARD;
            }
            break;

        case RADIO_SCENE_PLAYING_BACKWARD:
            app_data.next_scene = radio_scene_playing(fenter);
            if(app_data.next_scene == RADIO_SCENE_RECORDING_FORWARD)
            {
                fenter = FUSION_FORWARD;
            }
            break;            

        case RADIO_SCENE_RECORDING_FORWARD:
            app_data.next_scene = radio_scene_recording(fenter);
            if(app_data.next_scene == RADIO_SCENE_PLAYING_BACKWARD)
            {
                fenter = FUSION_BACKWARD;
            }
            break;

        default:
            app_data.next_scene = RADIO_SCENE_EXIT;
            break;
        }
    }

    /* 应用退出 */
    ret = _app_deinit(&app_data);
    if(ret == false)
    {
        print_err("app exit error! \n");
    }
    else
    {
    }

    _app_init_err:
    /* 应用库退出 */
    applib_quit();

    _lib_deinit();

    print_dbg("radio app exit: %d\n", ret);

    return 0;
}

