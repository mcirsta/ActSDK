/*******************************************************************************
 *                              NOYA1100
 *                            Module: video
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-04-01          1.0             build this file 
*******************************************************************************/
/*!
 * \file     video_main.c
 * \brief    
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2009/04/01
*******************************************************************************/

#include "include_psp.h"
#include "include_case.h"

/* include file of this application, 本应用的头文件 */
#include "include_video.h"


/* extern function, 函数声明 */
static bool _system_init( void );
static bool _system_deinit( void );
static bool _lib_init( void );
static bool _lib_deinit( void );
static bool _gui_init( void );
static bool _gui_deinit( void );
static bool _video_init( void );
static bool _video_deinit( void );


/* define variable, 变量定义 */
video_vars_t video_vars;


static bool _lib_init( void )
{
    video_vars.applib_lib = dlopen("applib.so", RTLD_LAZY);
    if(video_vars.applib_lib == NULL)
    {
        print_err("open applib.so err! \n");
        goto _lib_init_err;
    }

    video_vars.commonui_lib = dlopen("commonui.so", RTLD_LAZY);
    if(video_vars.commonui_lib == NULL)
    {
        print_err("open commonui.so err! \n");
        goto _lib_init_err;
    }

    video_vars.fusion_lib = dlopen("fusion.so", RTLD_LAZY);
    if(video_vars.fusion_lib == NULL)
    {
        print_err("open fusion.so err! \n");
        goto _lib_init_err;
    }

    video_vars.apconfig_lib = dlopen("apconfig.so", RTLD_LAZY);
    if(video_vars.apconfig_lib == NULL)
    {
        print_err("open apconfig.so err! \n");
        goto _lib_init_err;
    }

    video_vars.sqlite_lib = dlopen("sqlite.so", RTLD_LAZY);
    if(video_vars.sqlite_lib == NULL)
    {
        print_err("open sqlite.so err! \n");
        goto _lib_init_err;
    }

    video_vars.mnavi_lib = dlopen("mnavi.so", RTLD_LAZY);
    if(video_vars.mnavi_lib == NULL)
    {
        print_err("open mnavi.so err! \n");
        goto _lib_init_err;
    }

    video_vars.playlist_lib = dlopen("playlist.so", RTLD_LAZY);
    if(video_vars.playlist_lib == NULL)
    {
        print_err("open playlist.so err! \n");
        goto _lib_init_err;
    }
    
    video_vars.gui_lib = dlopen("gui.so", RTLD_LAZY);
    if(video_vars.gui_lib == NULL)
    {
        print_err("open gui.so err! \n");
        goto _lib_init_err;
    }

    video_vars.sty_lib = dlopen("style.so", RTLD_LAZY);
    if(video_vars.sty_lib == NULL)
    {
        print_err("open style.so err! \n");
        goto _lib_init_err;
    }
    
    return true;

    _lib_init_err:
    _lib_deinit();

    return false;
}

static bool _lib_deinit( void )
{
    if(video_vars.sty_lib != NULL)
    {
        dlclose( video_vars.sty_lib );
        video_vars.sty_lib = NULL;
    }

    if(video_vars.gui_lib != NULL)
    {
        dlclose( video_vars.gui_lib );
        video_vars.gui_lib = NULL;
    }

    if(video_vars.playlist_lib != NULL)
    {
        dlclose( video_vars.playlist_lib );
        video_vars.playlist_lib = NULL;
    }

    if(video_vars.mnavi_lib != NULL)
    {
        dlclose( video_vars.mnavi_lib );
        video_vars.mnavi_lib = NULL;
    }

    if(video_vars.sqlite_lib != NULL)
    {
        dlclose( video_vars.sqlite_lib );
        video_vars.sqlite_lib = NULL;
    }

    if(video_vars.apconfig_lib != NULL)
    {
        dlclose( video_vars.apconfig_lib );
        video_vars.apconfig_lib = NULL;
    }

    if(video_vars.fusion_lib != NULL)
    {
        dlclose( video_vars.fusion_lib );
        video_vars.fusion_lib = NULL;
    }

    if(video_vars.commonui_lib != NULL)
    {
        dlclose( video_vars.commonui_lib );
        video_vars.commonui_lib = NULL;
    }

    if(video_vars.applib_lib != NULL)
    {
        dlclose( video_vars.applib_lib );
        video_vars.applib_lib = NULL;
    }

    return true;
}

#if 0
#endif
/*
* 应用运行时依赖的系统资源, 每个应用仅需进入时打开一次
* apps open system resource
*/
static bool _system_init( void )
{
    char file[PATH_MAX];
    
    insmod("dspdec.ko", 0);
    sys_mknod(DEVICE_DSP_AD, S_IFCHR, makedev(MAJOR_DSP_AD, 0));
    
    // 获取app 的 style 资源文件名 
    sprintf(file, "%s/%s.sty", sys_get_app_theme_res_dir(), "video");
    video_vars.app_style = open_style_file( file );
    if ( NULL == video_vars.app_style )
    {
        print_err("open open_style_file err! \n");
        return false;
    }
    
    // 获取commonui 的 style 资源文件名 
    sprintf(file, "%s/%s.sty", sys_get_common_theme_res_dir(), "commonui");
    video_vars.commonui_style = open_style_file( file );
    if ( NULL == video_vars.commonui_style )
    {
        print_err("open open_style_file err! \n");
        return false;
    }
    
    return true;
}

static bool _system_deinit( void )
{
    close_style_file( video_vars.commonui_style );
    close_style_file( video_vars.app_style );
    
    rmmod("dspdec.ko", 0);

    return true;
}


static bool _gui_init( void )
{
    int tmp = -1;

    //创建字体
    video_vars.fontdesc = gui_create_font( sys_get_default_font_file(), 16 );
    if( video_vars.fontdesc < 0 )
    {
        print_err("gui_create_font err\n\n"); 
        return false;
    }

    tmp = gui_dc_set_default_fontface( video_vars.fontdesc );
    if ( tmp < 0 )
    {
        print_err("gui_dc_set_default_fontface err\n\n"); 
        return false;
    }

    // 设置语言模式
    gui_set_language_mode( sys_get_cur_language_id() );

    return true;
}


static bool _gui_deinit( void )
{
    //删除字体
    gui_destroy_font( video_vars.fontdesc );
    video_vars.fontdesc = -1;

    return true;
}


/* application initial function */
static bool _video_init( void )
{
    bool result = true;

    result = _system_init();
    if ( false == result )
    {
        print_err("video_system_init err");
        return false;
    }
    
    result = _gui_init();
    if ( false == result )
    {
        print_err("video_gui_init err");
        return false;
    }
//shaofeng    sys_check_media_dir( UDISK_VIDEO );

    //shaofeng 位置?
    result = video_cfg_read();
    
    return result;
}

/* application exit function */
static bool _video_deinit( void )
{
    bool result = true;

    print_dbg();
    
    //释放dirlist 资源
    dirlist_delete( video_vars.dirlist );
    video_vars.dirlist = NULL;
    
    video_playlist_deinit();
    video_cfg_write();
    
    video_mmm_deinit();
    video_mmm_swf_deinit();
    
    _gui_deinit();
    
    _system_deinit();
    
    return result;
}


/******************************************************************************/
/*!                    
* \par  Description:
*       设置video的横竖屏方向. 除设置sty外, 还需要设置mmm
* \param[in]    rotation_e: 方向数据
* \retval       
*******************************************************************************/
bool video_set_scene_direction( rotation_e rotation )
{
#if 1
    sty_set_screen_rotation( rotation );
    
    //线程解码中间, 防止V_CMD_SET_SCREEN_MODE失败, 先设置stop
    video_control( V_CMD_STOP, NULL );
    video_control( V_CMD_SET_SCREEN_MODE, &rotation );
    
    return true;
#endif
}


/******************************************************************************/
/*!                    
* \par  Description:
*       处理进程间的系统消息函数
* \param[in]    msg_apps_t *: 消息指针
* \retval       
*******************************************************************************/
int video_proc_sys_msg( msg_apps_t *msg )
{
    msg_reply_t reply;

//    print_dbg("msgcontent:%s", msg->content); //注意content内容超长越界
    print_info("msgtype:%d, content[0]:0x%x", msg->type, msg->content[0]);
    memset(&reply, 0x00, sizeof(reply));
    
    switch(msg->type)
    {
    case MSG_APP_QUIT:
    case MSG_MAIN_DISK_CHANGED:
        video_vars.flag_save_tag = true;
        video_vars.scene_list.app_scene.scene_type = SCENE_EXIT;
        video_vars.scene_playing.app_scene.scene_type = SCENE_EXIT;
        video_vars.scene_swf.app_scene.scene_type = SCENE_EXIT;
        video_vars.next_scene = SCENE_EXIT;
        exit_msg_loop();
        break;
        
    case MSG_GSENSOR_ROTATION_CHANGED:
        video_scene_list_rotate( &video_vars.scene_list );
//        video_scene_playing_rotate( &video_vars.scene_playing );
        break;
        
    case MSG_GSENSOR_SHAKED:
        video_scene_playing_shaked( &video_vars.scene_playing );
        video_scene_swf_shaked( &video_vars.scene_swf );
        break;
        
    default:
        dirlist_proc_sys( video_vars.dirlist, msg ); /* 内部会处理卡，U盘拔插消息. */
        msgbox_proc_sys( video_vars.msgbox, msg );
        break;
    }
//shaofeng    clear_sys_msg_queue( msg->type );
    return 1;
}



/******************************************************************************/
/*!                    
* \par  Description:
*       应用入口函数. application main function
* \param[in]    
* \retval       
*   注意: 
*       根据启动参数进入不同场景
*       共list playing两个场景
*******************************************************************************/
int main( int argc, const char *argv[] )
{
    bool result = true;
    bool playlist_init_result = true;
    bool start_by_other = false;
    scene_type_e tmp_scene_type;
    int index_cur;
    
    print_dbg("start video application");

    result = _lib_init();
    if ( false == result )
    {
        print_err("lib init err");
        return -1;
    }

    /* 应用库初始化 */
    result = applib_init(argc, argv, APP_TYPE_GUI);
    if(false == result)
    {
        return -1;
    }

    sys_set_key_tone_enable( 0 );
    
    /* 应用初始化 */
    result = _video_init();
    if(false == result)
    {
        print_err("video init error!\n");
        goto VIDEO_INIT_ERR;
    }
    
    //从其它应用启动, 读取1 2参数, 根据参数启动
    if ( argc > 2 )
    {
        if ( (NULL == argv[1]) || (NULL == argv[2]) )
        {
            goto VIDEO_INIT_ERR;
        }
        start_by_other = true;
        
        print_dbg("argv[1]:%s, argv[2]:%s\n", argv[1], argv[2]);
        if( 0 == strcmp(argv[1], VIDEO_ARGV_PLAYING) )
        {
            /* 为优化显示速度, 将playlist初始化放在scene_list和各启动处 */
            /*playlist_init_result记录playlist初始化结果，用于在播放场景提示*/
            playlist_init_result = video_playlist_init(); 
            
            strcpy( video_vars.plist.np_fullpath, argv[2] );
            //找到文件后缀, 并与swf比较, 选择进入相应的播放场景
            result = is_swf_ext( video_vars.plist.np_fullpath );
            if ( true == result )
            {
                video_vars.next_scene = SCENE_SWF;
                if( false == playlist_init_result )
                {
                    /*播放列表初始化失败，在播放场景弹提示并退出*/
                    goto VIDEO_PLAYLIST_INIT_ERR;
                }
/*                
                //清除nplist, 仅添加此swf文件到nplist, 并设置np_index用于播放
                plist_clear( video_vars.nplist );
                video_nplist_add_file( video_vars.plist.np_fullpath );
*/
                /*使用文件选择器播放*/
                video_vars.flag_fsel_mode = true;
                video_nplist_init( video_vars.plist.np_fullpath );

                video_vars.plist.np_index = 0;
            }
            else
            {
                video_vars.next_scene = SCENE_PLAYING;
                if( false == playlist_init_result )
                {
                    /*播放列表初始化失败，在播放场景弹提示并退出*/
                    goto VIDEO_PLAYLIST_INIT_ERR;
                }
                memset( &video_vars.plist.np_tag, 0x00, sizeof(video_vars.plist.np_tag) );
/*                
                //将外部传入的文件全路径, 添加到nplist, 并设置np_index用于播放
                video_nplist_add_file( video_vars.plist.np_fullpath );
*/                
                /*使用文件选择器播放*/
                video_vars.flag_fsel_mode = true;
                video_nplist_init( video_vars.plist.np_fullpath );
                              
                if ( true == video_nplist_get_index_by_fullpath( video_vars.plist.np_fullpath, &index_cur) )
                {
                    video_vars.plist.np_index = index_cur;
                }
                else
                {
                    print_err("video_nplist_get_index_by_fullpath false\n");
                    video_vars.plist.np_index = -1;
                }
               
            }
        }
        else if( 0 == strcmp(argv[1], VIDEO_ARGV_DV_DIRLIST) )
        {
            /* 为优化显示速度, 将playlist初始化放在scene_list和各启动处 */
            video_playlist_init();
            
            video_vars.start_mode = VIDEO_START_MODE_DV_DIRLIST;
            video_vars.next_scene = SCENE_LIST;
            strcpy( video_vars.dv_top_path, argv[2] );
            strcpy( video_vars.dv_init_path, argv[3] );
        }
        else if( 0 == strcmp(argv[1], VIDEO_ARGV_LIST) )
        {
            video_vars.next_scene = SCENE_LIST;
        }
        else
        {
        }
    }
    //从launch启动, 正常启动
    else
    {
        start_by_other = false;
        //初始化应用第一个场景
        video_vars.next_scene = SCENE_LIST;
    }
    
    VIDEO_PLAYLIST_INIT_ERR:
    
    /* 进入场景 */  
    while ( video_vars.next_scene != SCENE_EXIT )
    {
        switch( video_vars.next_scene )
        {
        case SCENE_LIST:
            tmp_scene_type = video_scene_list( &video_vars.scene_list );
            /*场景会给video_vars.next_scene赋值，此处不做QAC检查*/
#pragma __PRQA_IGNORE_START__
            /* 外部设置当前应用退出  */
            if ( SCENE_EXIT == video_vars.next_scene )
            {
                break;
            }
#pragma __PRQA_IGNORE_END__

            video_vars.next_scene = tmp_scene_type;
            if ( SCENE_RETURN == tmp_scene_type)
            {
                video_vars.next_scene = SCENE_EXIT;
            }
            break;
            
        case SCENE_PLAYING:
            tmp_scene_type = video_scene_playing( &video_vars.scene_playing );

            /*场景会给video_vars.next_scene赋值，此处不做QAC检查*/
#pragma __PRQA_IGNORE_START__
            /* 外部设置当前应用退出  */
            if ( SCENE_EXIT == video_vars.next_scene )
            {
                break;
            }
#pragma __PRQA_IGNORE_END__
           
            //如果从list进入, err则返回list.. 如果从其他应用进入, err, 则应退出
            video_vars.next_scene = tmp_scene_type;
            if ( SCENE_ERR == tmp_scene_type )
            {
                video_vars.next_scene = SCENE_LIST;
            }
            else if ( SCENE_RETURN == tmp_scene_type )
            {
                video_vars.next_scene = SCENE_LIST;
            }
            else
            {
            }
            
            if ( (true == start_by_other) && (VIDEO_START_MODE_DV_DIRLIST != video_vars.start_mode) )
            {
                video_vars.next_scene = SCENE_EXIT;
            }
            break;

        case SCENE_SWF:
            tmp_scene_type = video_scene_swf( &video_vars.scene_swf );

            /*场景会给video_vars.next_scene赋值，此处不做QAC检查*/
#pragma __PRQA_IGNORE_START__
            /* 外部设置当前应用退出 */
            if ( SCENE_EXIT == video_vars.next_scene )
            {
                break;
            }
#pragma __PRQA_IGNORE_END__
            
            //如果从swf进入, err则返回list.
            video_vars.next_scene = tmp_scene_type;
            if ( SCENE_ERR == tmp_scene_type )
            {
                video_vars.next_scene = SCENE_LIST;
            }
            else if ( SCENE_RETURN == tmp_scene_type )
            {
                video_vars.next_scene = SCENE_LIST;
            }
            else
            {
            }

            if ( (true == start_by_other) && (VIDEO_START_MODE_DV_DIRLIST != video_vars.start_mode) )
            {
                video_vars.next_scene = SCENE_EXIT;
            }
            break;
            
        default:
            print_warning(" next_scene:%d outof range. apps will exit!!\n\n", video_vars.next_scene);
            video_vars.next_scene = SCENE_EXIT;
            break;
        }
    }
    
    /* 应用退出 */
    VIDEO_INIT_ERR:
    _video_deinit();
    
    sys_set_key_tone_enable( 1 );

    /* 应用库退出 */
    applib_quit();
    _lib_deinit();
    print_dbg("end video application");
    
    return 0;
}

