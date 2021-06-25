/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_main.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_main.c
* \brief    music_demo application
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/
    
#include "music.h"

app_music_t* g_app_music = NULL;
static app_music_t s_app_music;

static const clk_domain_t s_clk_domain = {CLK_LOAD_MIN, CLK_LOAD_HIGH_0};

static bool _lib_init( app_music_t *app_music);
static bool _lib_deinit( app_music_t *app_music);

static bool _app_init( app_music_t *app_music );
static bool _app_deinit( app_music_t *app_music );

static bool _lib_init( app_music_t *app_music)
{
    app_music->applib_lib = dlopen("applib.so", RTLD_LAZY);
    if (app_music->applib_lib == NULL)
    {
        printf("open applib lib err! \n");
        goto _lib_init_err;
    }

    app_music->commonui_lib = dlopen("commonui.so", RTLD_LAZY);
    if (app_music->commonui_lib == NULL)
    {
        printf("open commonui lib err! \n");
        goto _lib_init_err;
    }

    app_music->fusion_lib = dlopen("fusion.so", RTLD_LAZY);
    if (app_music->fusion_lib == NULL)
    {
        printf("open fusion lib err! \n");
        goto _lib_init_err;
    }

    app_music->gui_lib = dlopen("gui.so", RTLD_LAZY);
    if (app_music->gui_lib == NULL)
    {
        printf("open gui lib err! \n");
        goto _lib_init_err;
    }

    app_music->sty_lib = dlopen("style.so", RTLD_LAZY);
    if (app_music->sty_lib == NULL)
    {
        printf("open style lib err! \n");
        goto _lib_init_err;
    }

    app_music->config_lib = dlopen("apconfig.so", RTLD_LAZY);
    if (app_music->config_lib == NULL)
    {
        print_err("open apconfig lib err!");
        goto _lib_init_err;
    }
    
    return true;

    _lib_init_err:
    _lib_deinit(app_music);

    return false;
}

static bool _lib_deinit( app_music_t *app_music)
{ 
    if ( app_music->gui_lib != NULL )
    {
        dlclose(app_music->gui_lib);
        app_music->gui_lib = NULL;
    }

    if ( app_music->sty_lib != NULL )
    {
        dlclose(app_music->sty_lib);
        app_music->sty_lib = NULL;
    }

    if ( app_music->sqlite_lib != NULL )
    {
        dlclose(app_music->sqlite_lib);
        app_music->sqlite_lib = NULL;
    }

    if ( app_music->mnavi_lib != NULL )
    {
        dlclose(app_music->mnavi_lib);
        app_music->mnavi_lib = NULL;
    }

    if ( app_music->plist_lib != NULL )
    {
        dlclose(app_music->plist_lib);
        app_music->plist_lib = NULL;
    }

    if ( app_music->config_lib != NULL )
    {
        dlclose(app_music->config_lib);
        app_music->config_lib = NULL;
    }

    if ( app_music->codec_lib != NULL )
    {
        dlclose(app_music->codec_lib);
        app_music->codec_lib = NULL;
    }

    if (app_music->fusion_lib != NULL)
    {
        dlclose(app_music->fusion_lib);
        app_music->fusion_lib = NULL;
    }

    if (app_music->commonui_lib != NULL)
    {
        dlclose(app_music->commonui_lib);
        app_music->commonui_lib = NULL;
    }

    if (app_music->applib_lib != NULL)
    {
        dlclose(app_music->applib_lib);
        app_music->applib_lib = NULL;
    }

    return true;
}

/* application initial function */
static bool _app_init( app_music_t *app_music )
{
    char file[PATH_MAX];
    
    //禁止按键音
    sys_set_key_tone_enable(0);
    
    // 获取commonui 的 style 资源文件名 
    sprintf(file, "%s/%s.sty", sys_get_common_theme_res_dir(), "commonui");
    g_common_style = open_style_file( file );
    if (g_common_style == NULL)
    {
        print_err("open common style file err!");
        goto comsty_err;
    }

    // 获取应用的 style 资源文件名
    sprintf(file, "%s/%s.sty", sys_get_app_theme_res_dir(), "music");
    g_music_style = open_style_file( file );
    if (g_music_style == NULL)
    {
        print_err("open music style file err!");
        goto musicsty_err;
    }
        
    return true;

    musicsty_err:
    close_style_file( g_common_style );
    
    comsty_err:
    
    return false;
}


/* application exit function */
static bool _app_deinit( app_music_t *app_music )
{
    //close style file
    close_style_file( g_common_style );
    g_common_style = NULL;
    close_style_file( g_music_style );
    g_music_style = NULL;
    
    //delete dirlist
    dirlist_delete( g_dirlist );
    g_dirlist = NULL;
    
    //exit music engine
    music_close_engine();

    //album image engine
    album_close_engine();
    
    //exit playlist
    plistclass_exit(MUSIC);
    
    return true;
}

bool app_extra_init( app_music_t *app_music )
{
    static bool app_extra_inited = false;

    //check init
    if ( app_extra_inited == false )
    {
        app_extra_inited = true;
    }
    else
    {
        return true;
    }
    
    //grab power off msg
    grab_msg(MSG_POWER_OFF);
    
    app_music->sqlite_lib = dlopen("sqlite.so", RTLD_LAZY);
    if(app_music->sqlite_lib == NULL)
    {
        print_err("open sqlite_lib err! \n");
        goto sqlite_err;
    }
    
    app_music->mnavi_lib = dlopen("mnavi.so", RTLD_LAZY);
    if(app_music->mnavi_lib == NULL)
    {
        print_err("open mnavi_lib err! \n");
        goto mnavi_err;
    }
    
    app_music->plist_lib = dlopen("playlist.so", RTLD_LAZY);
    if(app_music->plist_lib == NULL)
    {
        print_err("open plist_lib err! \n");
        goto plist_err;
    }

    app_music->codec_lib = dlopen("a_codec.so", RTLD_LAZY);
    if (app_music->codec_lib == NULL)
    {
        print_err("open codec lib err!");
        goto codec_err;
    }    
    
    //init playlist
    if( plistclass_init(MUSIC) == false )
    {
        print_err("plistclass init error! \n");
//        goto plistclass_err;
    }
    
    //release power off msg
    release_msg(MSG_POWER_OFF);        

    return true;

//    plistclass_err:
//    dlclose(app_music->codec_lib);
//    app_music->codec_lib = NULL;

    codec_err:
    dlclose(app_music->plist_lib);
    app_music->plist_lib = NULL;
    
    plist_err:
    dlclose(app_music->mnavi_lib);
    app_music->mnavi_lib = NULL;
    
    mnavi_err:
    dlclose(app_music->sqlite_lib);
    app_music->sqlite_lib = NULL;
    
    sqlite_err:
    release_msg(MSG_POWER_OFF);  
    
    return false;
}


/* application main function */
int main(int argc, const char *argv[])
{
    int ret = 0;
    bool passive_mode;    
    fusion_enter_e enter_type;

    if( _lib_init(&s_app_music) == false )
    {
        print_err("lib init error!");
        goto _lib_init_err;
    }
    else
    {
        g_app_music = &s_app_music;
    } 

    /* 应用库初始化 */
    if( applib_init(argc,argv,APP_TYPE_GUI) == false )
    {
        print_err("applib init error! \n");
        goto applib_init_err;
    }
    else
    {

    }
    
    sys_pm_set_clk_domain(&s_clk_domain);    

    /* 应用初始化 */
    if( _app_init( g_app_music ) == false )
    {
        print_err("app init error! \n");
        goto app_init_err;
    }
    else
    {
    }    

    /* 工作模式判断 */
    //初始化应用第一个场景
    if( argc >= 3 )
    {
        if( strcmp( argv[1], MUSIC_ARGV_PLAYING ) == 0 )
        {            
            //extra init
            if( app_extra_init(g_app_music) == false )
            {
                print_err("app_extra_init error! \n");
            }
            else
            {
                //run music engine
                music_open_engine( true ); 
                music_set_filepath( (char *)argv[2] );
            }
            passive_mode = true;
            s_app_music.next_scene = SCENE_PLAY;
        }
        else
        {
            passive_mode = false;
            s_app_music.next_scene = SCENE_LIST;
        }
    }
    else
    {
        passive_mode = false;
        s_app_music.next_scene = SCENE_LIST;
    }    

    enter_type = FUSION_FORWARD;
    
    /* 进入场景 */    
    while ( s_app_music.next_scene != SCENE_EXIT )
    {
        switch( s_app_music.next_scene )
        {
        case SCENE_LIST:
            ret = music_scene_list( enter_type );
            
            switch( ret )
            {
                case SCENE_LIST_NEXT:
                {
                    s_app_music.next_scene = SCENE_PLAY;
                    enter_type = FUSION_FORWARD;
                    break;
                }
                
                case SCENE_LIST_RETURN:
                {
                    s_app_music.next_scene = SCENE_EXIT; //exit
                    break;
                }

                default:
                {
                    s_app_music.next_scene = SCENE_EXIT; //exit
                    break;
                }
            }
            break;
            
        case SCENE_PLAY:
            ret = music_scene_playing( enter_type );            
            
            switch( ret )
            {
                case SCENE_PLAY_NEXT:
                {
                    s_app_music.next_scene = SCENE_PLAY;
                    break;
                }
                
                case SCENE_PLAY_RETURN:
                {
                    s_app_music.next_scene = SCENE_LIST;
                    enter_type = FUSION_BACKWARD;
                    break;
                }
                
                case SCENE_PLAY_QUIT:
                {
                    s_app_music.next_scene = SCENE_EXIT; //exit
                    break;
                }
                
                default:
                {
                    s_app_music.next_scene = SCENE_LIST; //return
                    break;
                }
            }
            
            if( passive_mode == true )
            {
                s_app_music.next_scene = SCENE_EXIT; // passive mode exit
            }
            break;
            
        default:
            s_app_music.next_scene = SCENE_EXIT;
            break;
        }
    }
    
    /* 应用退出 */
    if( _app_deinit( g_app_music ) == false )
    {
        print_err("app exit error! \n");        
        goto app_deinit_err;
    }
    else
    {
    }
    
    sys_pm_clear_clk_domain(&s_clk_domain);

    /* 应用库退出 */  
    applib_quit();

    _lib_deinit(&s_app_music);

    return 0;
    
    app_deinit_err:    
    app_init_err:
    sys_pm_clear_clk_domain(&s_clk_domain);
    applib_quit();
    
    applib_init_err:
    _lib_deinit(&s_app_music);
    _lib_init_err:

    return -1;
}
