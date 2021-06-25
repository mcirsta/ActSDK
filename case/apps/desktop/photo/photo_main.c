/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_main.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *       carrie     2009-4-24          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_main.c
 * \brief    photo application
 * \author   carrie
 *
 * \version 1.0
 * \date  2009/04/24
 *******************************************************************************/

#include "photo.h"

#include "photo_scene_list.h"
#include "photo_scene_preview.h"
#include "photo_scene_playing.h"
#include "photo_playlist.h"
#include "photo_dec_ctrl.h"

/* scene type define */
typedef enum
{
    SCENE_LIST = 0, /* 列表场景 */
    SCENE_PREVIEW,  /* 预览场景 */
    SCENE_PLAYING,  /* 全屏播放场景 */
    SCENE_EXIT,     /* 退出应用 */
} app_scene_t;

typedef struct app_s
{
    /* system resource */
    void *applib_lib;
    void *commonui_lib;
    void *fusion_lib;
    void *gui_lib;
    void *sty_lib;
    void *config_lib;
    void *sqlite_lib;
    void *mnavi_lib;
    void *playlist_lib;

    app_scene_t next_scene; /* 下一个场景 */

} app_t;

static int _app_init(app_t *app);
static int _app_deinit(app_t *app);

static const clk_domain_t s_clk_domain = {CLK_LOAD_MIN, CLK_LOAD_HIGH_0};

static bool _lib_init(app_t *app);
static bool _lib_deinit(app_t *app);

static bool _lib_init(app_t *app)
{
    app->applib_lib = dlopen("applib.so", RTLD_LAZY);
    if (app->applib_lib == NULL)
    {
        printf("open applib lib err! \n");
        goto _lib_init_err;
    }

    app->commonui_lib = dlopen("commonui.so", RTLD_LAZY);
    if (app->commonui_lib == NULL)
    {
        printf("open commonui lib err! \n");
        goto _lib_init_err;
    }

    app->fusion_lib = dlopen("fusion.so", RTLD_LAZY);
    if (app->fusion_lib == NULL)
    {
        printf("open fusion lib err! \n");
        goto _lib_init_err;
    }

    app->gui_lib = dlopen("gui.so", RTLD_LAZY);
    if (app->gui_lib == NULL)
    {
        printf("open gui lib err! \n");
        goto _lib_init_err;
    }

    app->sty_lib = dlopen("style.so", RTLD_LAZY);
    if (app->sty_lib == NULL)
    {
        printf("open style lib err! \n");
        goto _lib_init_err;
    }

    app->config_lib = dlopen("apconfig.so", RTLD_LAZY);
    if (app->config_lib == NULL)
    {
        print_err("open apconfig lib err!");
        goto _lib_init_err;
    }

    app->sqlite_lib = dlopen("sqlite.so", RTLD_LAZY);
    if (app->sqlite_lib == NULL)
    {
        print_err("load sqlite.so failed.\n");
        goto _lib_init_err;
    }

    app->mnavi_lib = dlopen("mnavi.so", RTLD_LAZY);
    if (app->mnavi_lib == NULL)
    {
        print_err("load mnavi.so failed.\n");
        goto _lib_init_err;
    }

    app->playlist_lib = dlopen("playlist.so", RTLD_LAZY);
    if (app->playlist_lib == NULL)
    {
        print_err("load playlist.so failed.\n");
        goto _lib_init_err;
    }

    return true;

    _lib_init_err:
    _lib_deinit(app);

    return false;
}

static bool _lib_deinit(app_t *app)
{ 
    if (app->playlist_lib != NULL)
    {
        dlclose(app->playlist_lib);
        app->playlist_lib = NULL;
    }

    if (app->sqlite_lib != NULL)
    {
        dlclose(app->sqlite_lib);
        app->sqlite_lib = NULL;
    }

    if (app->mnavi_lib != NULL)
    {
        dlclose(app->mnavi_lib);
        app->mnavi_lib = NULL;
    }

    if (app->config_lib != NULL)
    {
        dlclose(app->config_lib);
        app->config_lib = NULL;
    }

    if (app->sty_lib != NULL)
    {
        dlclose(app->sty_lib);
        app->sty_lib = NULL;
    }

    if (app->gui_lib != NULL)
    {
        dlclose(app->gui_lib);
        app->gui_lib = NULL;
    }

    if (app->fusion_lib != NULL)
    {
        dlclose(app->fusion_lib);
        app->fusion_lib = NULL;
    }

    if (app->commonui_lib != NULL)
    {
        dlclose(app->commonui_lib);
        app->commonui_lib = NULL;
    }

    if (app->applib_lib != NULL)
    {
        dlclose(app->applib_lib);
        app->applib_lib = NULL;
    }

    return true;
}

/* application initial function */
static int _app_init(app_t *app)
{
    photo_sty_open();
    return 0;
}

/* application exit function */
static int _app_deinit(app_t *app)
{
    /* 释放全局数据 */
    photo_dirlist_delete_all();
    photo_menu_delete_all();
    photo_level_info_delete_all();
    photo_sty_close();

    photo_dec_close();
    photo_playlist_deinit();

    return 0;
}

/* application main function */
int main(int argc, const char *argv[])
{
    app_t app_data;
    int ret = 0, temp;
    app_scene_t init_scene = SCENE_LIST;
    char *initpath = NULL, *toppath = NULL;
    char *malloc_path = NULL;
    fusion_enter_e enter_type = FUSION_FORWARD;

    if( _lib_init(&app_data) == false )
    {
        print_err("lib init error!");
        return -1;
    }

    /* 应用库初始化 */
    applib_init(argc, argv, APP_TYPE_GUI);
    sys_pm_set_clk_domain(&s_clk_domain);

    /* 应用初始化 */
    ret = _app_init(&app_data);
    if (ret < 0)
    {
        print_err();
        goto _app_init_err;
    }

    if(sys_get_main_disk_dir(DIR_PHOTO) == NULL)
    {
        argc = 1;   /* 无盘，进scene list弹对话框后退出 */
    }

    /*
     * 传递路径参数，从其他应用（例如dc）进来时，直接进入preview或nowplaying
     * 使用不同的播放列表
     */
    if(argc > 2)
    {
        init_scene = SCENE_PLAYING;
        /* 如果初始化播放列表出错，则进到nowplaying里显示错误后退出 */
        if(true == photo_playlist_init(PHOTO_PLIST_SPECIAL))
        {
            photo_playlist_del_items(0, -1);

            if(!strcmp(PHOTO_ARGV_PLAYING, argv[1]))
            {
                init_scene = SCENE_PLAYING;
                set_config(CFG_PHOTO_NOWPLAY_PATH, argv[2], (int)strlen(argv[2]) + 1);
                photo_playlist_add_file_dir(argv[2]); /* 添加文件所在目录下所有文件 */
                ret = photo_playlist_get_file_index(argv[2]);
                set_config(CFG_PHOTO_CURRENT_INDEX, (const char *) &ret, sizeof(ret));
                initpath = (char *)argv[2];
                toppath = initpath;
            }
            else if(!strcmp(PHOTO_ARGV_PREVIEW, argv[1]))
            {
                assert(argc > 3);
                init_scene = SCENE_PREVIEW;
                toppath = (char *)argv[2];
                initpath = (char *)argv[3];
            }
            else
            {
                init_scene = SCENE_LIST;
            }
        }
    }
    else
    {
        init_scene = SCENE_LIST;
    }

    /* 设置当前播放的存储器类型 */
    if(initpath != NULL)
    {
        if(!memcmp(initpath, EXTERNAL_ROOT, strlen(EXTERNAL_ROOT)))
        {
            temp = PATH_EXTERNAL;
            set_config(CFG_PHOTO_PATH_TYPE, (char *)&temp, 4);
        }
        else if(!memcmp(initpath, CARD_ROOT, strlen(CARD_ROOT)))
        {
            temp = PATH_CARD;
            set_config(CFG_PHOTO_PATH_TYPE, (char *)&temp, 4);
        }
        else
        {
            temp = PATH_INTERNAL;
            set_config(CFG_PHOTO_PATH_TYPE, (const char *) &temp, 4);
        }
    }
    else
    {
        temp = PATH_INTERNAL;
        set_config(CFG_PHOTO_PATH_TYPE, (const char *) &temp, 4);
    }

    app_data.next_scene = init_scene;

    /* 进入场景 */
    while (app_data.next_scene != SCENE_EXIT)
    {
        switch (app_data.next_scene)
        {
        case SCENE_LIST:
            ret = photo_scene_list(enter_type);
            switch(ret)
            {
            case SCENE_LIST_RETURN:
            case SCENE_LIST_EXIT_APP:
                app_data.next_scene = SCENE_EXIT;
                break;

            case SCENE_LIST_NOWPLAYING_NEXT:
                if(NULL != malloc_path)
                {
                    free(malloc_path);
                }
                malloc_path = (char *)malloc(MAX_PATH_LENGTH);
                if(NULL == malloc_path)
                {
                    print_err("malloc failure!");
                    break;
                }
                if(get_config(CFG_PHOTO_NOWPLAY_PATH, malloc_path, MAX_PATH_LENGTH) > 0)
                {
                    initpath = malloc_path;
                }
                toppath = get_top_path(initpath);
                enter_type = FUSION_FORWARD;
                app_data.next_scene = SCENE_PREVIEW;
                break;

            case SCENE_LIST_DIRLIST_NEXT:
                enter_type = FUSION_FORWARD;
                app_data.next_scene = SCENE_PREVIEW;
                break;

            default:
                break;
            }
            break;

        case SCENE_PLAYING:
            ret = photo_scene_playing(enter_type);
            if((SCENE_PLAYING == init_scene) ||
                ((SCENE_PREVIEW == init_scene) && (ret == SCENE_PLAYING_STORAGE_OUT)))
            {
                app_data.next_scene = SCENE_EXIT;
                break;
            }

            print_dbg("ret = %d", ret);

            switch(ret)
            {
            case SCENE_PLAYING_NO_FILE:
            case SCENE_PLAYING_RETURN:
                if(NULL != malloc_path)
                {
                    free(malloc_path);
                }

                malloc_path = (char *)malloc(MAX_PATH_LENGTH);
                if(NULL == malloc_path)
                {
                    print_err("malloc failure!");
                    break;
                }
                initpath = NULL;
                if(get_config(CFG_PHOTO_NOWPLAY_PATH, malloc_path, MAX_PATH_LENGTH) > 0)
                {
                    initpath = malloc_path;
                }

                if(NULL == toppath)     /* 从DC/browser应用进来时,toppath不为空 */
                {
                    toppath = get_top_path(initpath);
                }

                enter_type = FUSION_BACKWARD;
                app_data.next_scene = SCENE_PREVIEW;    /* 从nowplaying返回时，高亮当前文件 */
                break;

            case SCENE_PLAYING_STORAGE_OUT:
                enter_type = FUSION_BACKWARD;
                app_data.next_scene = SCENE_LIST;
                break;

            case SCENE_PLAYING_EXIT_APP:
                app_data.next_scene = SCENE_EXIT;
                break;

            default:
                break;
            }
            break;

        case SCENE_PREVIEW:
            ret = photo_scene_preview(initpath, toppath, enter_type);
            if((SCENE_PREVIEW == init_scene) &&
                    ((ret == SCENE_PREVIEW_RETURN) || (ret == SCENE_PREVIEW_STORAGE_OUT)))
            {
                app_data.next_scene = SCENE_EXIT;
                break;
            }

            print_dbg("ret = %d", ret);
            switch(ret)
            {
            case SCENE_PREVIEW_RETURN:
                enter_type = FUSION_BACKWARD;
                initpath = NULL;
                toppath = NULL;
                app_data.next_scene = SCENE_LIST;
                break;

            case SCENE_PREVIEW_NEXT:
                print_dbg("");
                enter_type = FUSION_FORWARD;
                app_data.next_scene = SCENE_PLAYING;
                break;

            case SCENE_PREVIEW_EXIT_APP:
                app_data.next_scene = SCENE_EXIT;
                break;

            case SCENE_PREVIEW_STORAGE_OUT:
                enter_type = FUSION_BACKWARD;
                app_data.next_scene = SCENE_LIST;
                break;

            default:
                break;
            }

            break;

        default:
            app_data.next_scene = SCENE_EXIT;
            break;
        }
    }
    

    /* 应用退出 */
    ret = _app_deinit(&app_data);
    if (ret < 0)
    {
        print_warning();
        return -1;
    }
    else
    {
    }

    sys_pm_clear_clk_domain(&s_clk_domain);

    _app_init_err:

    /* 应用库退出 */
    applib_quit();

    if(NULL != malloc_path)
    {
        free(malloc_path);
    }

    _lib_deinit(&app_data);

    printf(">>>>>>>>>>>>>>>>>>>>>> app return: %d\n", ret);

    return 0;
}
