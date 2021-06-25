/*******************************************************************************
 *                              NOYA1100
 *                            Module: app
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 * History:
 *      <author>    <time>           <version >             <desc>
 *      zhaowenjie    2009-05-26          1.0             build this file
*******************************************************************************/
/*!
 * \file     usb_main.c
 * \brief
 * \author   zhaowenjie
 * \par      GENERAL DESCRIPTION:
 *              负责程序进入的初始化，退出的清理和场景的调度。
 *
 * \par      EXTERNALIZED FUNCTIONS:
 *
 * \version 1.0
 * \date  2009/05/26
*******************************************************************************/

#include "include_psp.h"
#include "include_case.h"
#include "usb.h"
#include "usb_config.h"

static const clk_domain_t s_clk_domain = {(unsigned int)CLK_LOAD_HIGH_0, (unsigned int)CLK_LOAD_HIGH_0};
static const clk_domain_t s_charge_clk_domain = {(unsigned int)CLK_LOAD_LOW_1, (unsigned int)CLK_LOAD_LOW_1};
static bool s_is_charge_clk = false;

static bool s_receive_quit_msg = false;

static char s_usb_sty_file[PATH_MAX ];
static style_t* s_common_style = NULL;
static style_t* s_usb_style = NULL;
static scene_t* s_common_scene = NULL; /* COMMON WIDGET */

static scene_e s_current_scene = SCENE_EXIT; /* 当前场景 */

static char s_font_path[ PATH_MAX ];

static scene_t* s_util_scene = NULL;
static picture_resource_t* s_background_picture = NULL;


static void* s_applib_lib = NULL;
static void* s_commonui_lib = NULL;
static void* s_fusion_lib = NULL;

static void* s_sqlite_lib = NULL;
static void* s_gui_lib = NULL;
static void* s_apconfig_lib = NULL;
static void* s_style_lib = NULL;
static void* s_playlist_lib = NULL;
static int s_fontdesc;

static bool _system_init( void );
static void _system_deinit( void );

static bool _lib_init(void);
static void _lib_deinit(void);

bool _gui_init(void);
void _gui_deinit(void);
static bool _init(void);
static bool _deinit(void);

static void _enter_scene_schedule(void);
void set_current_scene( scene_e scene );

static bool _load_style_resource(void);

char* get_font_path(void);

static style_t* get_style(void);
picture_resource_t* get_background_picture(void);
void update_background_picture(void);

void set_receive_quit_msg( bool quit );

static void _generate_playlist(void);

static bool _open_theme(void);
static bool _close_theme(void);
static bool _open_str_res(void);
static bool _close_str_res(void);

bool _send_msg_to_cofig( msg_apps_type_e type );

static void _check_media_dir(void);

static usb_main_t s_usb_main_api =
{
    get_font_path,
    set_current_scene,

    get_style,
    get_background_picture,
    update_background_picture,
    set_receive_quit_msg,
};



/*
* 应用运行时依赖的系统资源, 每个应用仅需进入时打开一次
* apps open system resource
*/
static bool _system_init( void )
{
    if ( s_current_scene == SCENE_UCHARGE )
    {
        sys_pm_set_clk_domain( &s_charge_clk_domain );
        s_is_charge_clk = true;
    }
    else
    {
        sys_pm_set_clk_domain( &s_clk_domain );
        s_is_charge_clk = false;
    }
    
    return true;
}


static void _system_deinit( void )
{
    if ( s_is_charge_clk == true )
    {
        sys_pm_clear_clk_domain( &s_charge_clk_domain );
    }
    else
    {
        sys_pm_clear_clk_domain( &s_clk_domain );
    }



}

static bool _lib_init(void)
{
    s_applib_lib = dlopen("applib.so", RTLD_LAZY );
    if( s_applib_lib == NULL)
    {
        print_err("open applib.so err! \n");
        goto _lib_init_err;
    }

    s_commonui_lib = dlopen("commonui.so", RTLD_LAZY );
    if( s_commonui_lib == NULL)
    {
        print_err("open applib.so err! \n");
        goto _lib_init_err;
    }

    s_fusion_lib = dlopen("fusion.so", RTLD_LAZY );
    if( s_fusion_lib == NULL)
    {
        print_err("open fusion.so err! \n");
        goto _lib_init_err;
    }

    s_gui_lib = dlopen("gui.so", RTLD_LAZY );
    if( s_gui_lib == NULL)
    {
        print_err("open gui.so err! \n");
        goto _lib_init_err;
    }

    s_apconfig_lib = dlopen("apconfig.so", RTLD_LAZY );
    if (s_apconfig_lib == NULL)
    {
        print_err(" open s_apconfig_lib err! \n");
        goto _lib_init_err;
    }

    s_style_lib = dlopen("style.so", RTLD_LAZY );
    if( s_style_lib == NULL)
    {
        print_err("open style.so err! \n");
        goto _lib_init_err;
    }

    s_sqlite_lib = dlopen("sqlite.so", RTLD_LAZY );
    if (s_sqlite_lib == NULL)
    {
        print_err(" open s_sqlite_lib err! \n");
        goto _lib_init_err;
    }

    s_playlist_lib = dlopen("playlist.so", RTLD_LAZY );
    if (s_playlist_lib == NULL)
    {
        print_err(" open s_playlist_lib err! \n");
        goto _lib_init_err;
    }
    return true;

    _lib_init_err:
    _lib_deinit();

    return false;
}

static void _lib_deinit(void)
{
    /* 卸载动态库 */

    if (s_applib_lib != NULL)
    {
        dlclose(s_applib_lib);
        s_applib_lib = NULL;
    }

    if (s_commonui_lib != NULL)
    {
        dlclose(s_commonui_lib);
        s_commonui_lib = NULL;
    }

    if (s_fusion_lib != NULL)
    {
        dlclose(s_fusion_lib);
        s_fusion_lib = NULL;
    }

    if (s_gui_lib != NULL)
    {
        dlclose(s_gui_lib);
        s_gui_lib = NULL;
    }

    if (s_apconfig_lib != NULL)
    {
        dlclose(s_apconfig_lib);
        s_apconfig_lib = NULL;
    }

    if (s_style_lib != NULL)
    {
        dlclose(s_style_lib);
        s_style_lib = NULL;
    }

    if (s_sqlite_lib != NULL)
    {
        dlclose(s_sqlite_lib);
        s_sqlite_lib = NULL;
    }

    if (s_playlist_lib != NULL)
    {
        dlclose(s_playlist_lib);
        s_playlist_lib = NULL;
    }

}

bool _gui_init(void)
{
    print_dbg(" s_font_path == %s ", s_font_path );

    /* 创建字体 */
    s_fontdesc = gui_create_font( s_font_path, 16);
    if( s_fontdesc < 0 )
    {
        print_err("");
        return false;
    }
    gui_dc_set_default_fontface( s_fontdesc );

    sty_set_text_encoding(GUI_UTF8_ENC);

    return true;
}


/* application initial function */
static bool _init(void)
{
    bool result = true;

    result = _system_init();
    if ( false == result )
    {
        print_err("_system_init err");
        return false;
    }

    result = _load_style_resource();

    /* 禁止省电关机 */
    sys_forbid_auto_standby( true );
    /* 禁止睡眠关机 */
    sys_forbid_timed_shutdown(true);
    /* 禁止屏保背光 */
    sys_forbid_screensaver( true );

    return result;
}

static bool _load_style_resource(void)
{
    g_style_api = get_style_api();

    _open_theme();
    _open_str_res();

    /* 取出公用资源的scene */
    s_util_scene = g_style_api->load_scene( get_style(), UTIL_SCENE );
    if (s_util_scene == NULL)
    {
        print_err(" load s_util_scene failure \n");
        return false;
    }

    return true;
}

/* application deinit function */
static bool _deinit(void)
{
    bool result = true;

    _close_theme();
    _close_str_res();

    g_style_api->free_scene( s_util_scene );
    s_util_scene = NULL;

    g_style_api->close_style_file( s_common_style );
    g_style_api->close_style_file( s_usb_style );

    _gui_deinit();
    _system_deinit();

    g_style_api = NULL;
    g_usb_main = NULL;
    
    sys_forbid_auto_standby( false );
    sys_forbid_timed_shutdown(false);
    sys_forbid_screensaver( false );

    return result;
}


/* application main function */
int main(int argc, const char* argv[] )
{
    int value = 1;
    int old_value = 1;
    int ret;
    bool result = true;
    bool check_upgrade = false;
    
    bool consider_generate_plist = false;
    bool need_generate_plist = false;
    
    bool check_dir = false;
    bool need_umount = false;
    bool is_mtp = false;
    //bool config_set_font = false;
    
    msg_apps_t msg;
    msg_reply_t reply;

    _lib_init();
    print_info(" ************   usb app start  ************* ");
    /* 应用库初始化 */
    applib_init( argc, argv, APP_TYPE_GUI );
	
    /* 创建场景 */
    g_usb_main = create_usb_main();
    g_scene_udisk = create_scene_udisk();
    g_scene_mtp = create_scene_mtp();
    g_scene_charge = create_scene_charge();
    g_scene_upgrade = create_scene_upgrade();
	
    if ( argc == 1 )
    {
        print_dbg(" msg_box input args error !!! ");
    }
    else
    {   
        if ( strcmp( argv[ 1 ], USB_ARGV_UDISK) == 0 )
        {        
            s_current_scene = SCENE_UDISK;
	
            check_upgrade = true;
            consider_generate_plist = true;
            check_dir = true;
            need_umount = true;
        }
        else if ( strcmp( argv[ 1 ], USB_ARGV_MTP ) == 0 )
        {
            s_current_scene = SCENE_UMTP;
            
            consider_generate_plist = true;
            check_dir = true;
            is_mtp = true;
        }
        else if ( strcmp( argv[ 1 ], USB_ARGV_CHARGE_PLAY ) == 0 )
        {
            s_current_scene = SCENE_UCHARGE;
            g_scene_charge->set_need_charge_ui( false );
	
            result = false;
        }
        else if ( strcmp( argv[ 1 ], USB_ARGV_CHARGE ) == 0 )
        {        
            s_current_scene = SCENE_UCHARGE;
            g_scene_charge->set_need_charge_ui( true );
        }
        else
        {
        }
    }
	
    if ( result == true )
    {
        print_info("  MSG_KILL_USER_APPS_SYNC  start ......  ");
        enter_background_mode();
	
        memset( &msg, 0, sizeof( msg ) );
        memset( &reply, 0, sizeof( reply ) );
    
        msg.type = MSG_KILL_USER_APPS_SYNC;
        send_sync_msg(APP_NAME_MANAGER, &msg, &reply, NULL);
    
        if ( reply.type == MSG_REPLY_FAILED )
        {
            print_err(" MSG_KILL_USER_APPS_SYNC  failed ......  ");
            goto ERROR_EXIT;
        }
    
        print_info("  MSG_KILL_USER_APPS_SYNC  end ......  ");
        enter_foreground_mode();
    }
	
	
    /* 应用初始化 */
    result = _init();
    if(false == result)
    {
        print_err("usb init error!\n");
        goto ERROR_EXIT;
    }

    /* nor flash 方案 msg_box 需要在usb退出的时候，调用 pl_gener.app */
    ret = get_config( CFG_NEED_GEN_PLIST, (char*)&old_value , INT_LENGTH );
    if ( ret >= 0 )
    {
        if ( old_value == 1 )
        {
            need_generate_plist = true;
        }
    }
	
    /* 预防拔线断电 */
    if ( consider_generate_plist == true )
    {
        value = 1;
        set_config( CFG_NEED_GEN_PLIST, (char*)&value , 4 );
        sync_config();
    }
	
    get_configs();
    result = _gui_init();
    if ( false == result )
    {
        print_err("_gui_init err");
        goto ERROR_EXIT;
    }
    
    if ( need_umount == true )
    {
        sys_umount( UDISK_ROOT, 0 );
    }
	
    /* 进入场景调度器 */
    _enter_scene_schedule();

    print_info(" exit scene schedule ......... ");
    
    if ( need_umount == true )
    {    
        hotplugin_dev_info_t card_info;
        hotplugin_dev_type_e main_disk ;
        /* 将flash第0个磁盘第0个分区挂载到 /mnt/disk0目录中 */
        sys_mount(DEVICE_FLASH "u", UDISK_ROOT, "fat", 0 * 8 + 0, NULL);

        sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_CARD, &card_info);
        if( card_info.access == HOTPLUGIN_DEV_ACCESS_OK)
        {              
            main_disk = sys_get_main_disk();

            if(main_disk == HOTPLUGIN_DEV_TYPE_NONE)
            {
                sys_set_main_disk(HOTPLUGIN_DEV_TYPE_CARD);
            }
        }
        else
        {
            main_disk = sys_get_main_disk();

            if( main_disk == HOTPLUGIN_DEV_TYPE_CARD )
            {
                sys_set_main_disk(HOTPLUGIN_DEV_TYPE_NONE);
            }
        }
    }
    
    /* 查询是否有升级文件 */
    if ( check_upgrade == true )
    {
        g_scene_upgrade->enter_scene_upgrade();
    }
	
    if ( check_dir == true )
    {
        /* 检查主盘上各个文件夹是否存在 */
        _check_media_dir();
    }
    
    if ( consider_generate_plist == true )
    {
        if ( is_mtp == true )
        {
            if ( g_scene_mtp->get_mtp_change() == true )
            {
                char ext_music[PATH_MAX];

                plist_set_rootdir(sys_get_main_disk_dir(DIR_SYSTEM_PLAYLIST));

                /* read ext config */
                ret = get_config(CFG_APP_SUPPORT_EXT_MUSIC, (char*) ext_music, sizeof(ext_music));
                if ( ret != -1)
                {
                    plist_set_filetype_ext(MUSIC, ext_music);
                }

                /* 同步 MTP 数据到 playlist */
                plist_init();

                plist_sync_mtp(SYNC_FROM_MTP, NULL);

                plist_quit();

                need_generate_plist = true;
            }
            else
            {
                value = 0;
                set_config( CFG_NEED_GEN_PLIST, (char*)&old_value , 4 );
            }
        }
        else /* udisk */
        {
            if ( ( g_scene_udisk->get_disk_change() == true )
                    || ( g_scene_udisk->check_card_insert() == true ) )
            {
                need_generate_plist = true;
            }
            else
            {
                value = 0;
                set_config( CFG_NEED_GEN_PLIST, (char*)&old_value , 4 );
            }
        }
        
    }

    /* 关机退出时，不启动 gen_plist.app  */
    if ( s_receive_quit_msg == false)
    {
        if ( need_generate_plist == true )
        {
            _generate_playlist();
        }
    }
	
    /* 应用退出 */
    ERROR_EXIT:
	
    _deinit();
	
    /* 应用库退出 */
    applib_quit();

    _lib_deinit();
	
    print_info(" ************  usb app exit  ************* ");
	
    return 0;
}

static void _enter_scene_schedule(void)
{
    while ( s_current_scene != SCENE_EXIT )
    {
        switch ( s_current_scene )
        {
            case SCENE_UDISK:
            {
                g_scene_udisk->enter_scene_udisk();
                break;
            }

            case SCENE_UMTP:
            {
                g_scene_mtp->enter_scene_mtp();
                break;
            }

            case SCENE_UCHARGE:
            {
                g_scene_charge->enter_scene_charge();
                break;
            }

            case SCENE_UPGRADE:
            {
                g_scene_upgrade->enter_scene_upgrade();
                break;
            }

            default:
            {
                s_current_scene = SCENE_EXIT;
                break;
            }
        }
    }
}

void set_current_scene( scene_e scene )
{
    s_current_scene = scene;
}

style_t* get_style(void)
{
    return s_usb_style;
}

static void _generate_playlist(void)
{
    msg_apps_t msg;
    memset( &msg, 0, sizeof( msg ) );

    print_dbg( " create %s ......",  APP_NAME_PL_GEN  );

    msg.type = MSG_KILL_USER_APPS_CREAT_APP;
    strncpy(msg.content, APP_NAME_PL_GEN, MSG_CONTENT_SIZE );
    send_async_msg(APP_NAME_MANAGER, &msg);

    print_dbg( " end create %s ......",  APP_NAME_PL_GEN  );
    
}


picture_resource_t* get_background_picture(void)
{
    if ( s_background_picture == NULL )
    {
        update_background_picture(); 
    }

    return s_background_picture;
}

void update_background_picture(void)
{
    picture_resource_t* picture = NULL;
    rotation_e angle;

    angle = sty_get_view_rotation();
    if ( ( angle == ROTATION_0 ) ||  ( angle == ROTATION_180 ) )
    {
        picture = (picture_resource_t *) g_style_api->get_scene_child(s_util_scene, 
                                                                PIC_SIDELONG_BG );
    }
    else
    {
        picture = (picture_resource_t *) g_style_api->get_scene_child(s_util_scene, 
                                                                PIC_BACKGROUND );
    }
    
    s_background_picture = picture;
}

void set_receive_quit_msg( bool quit )
{
    s_receive_quit_msg = quit;
}

void _gui_deinit(void)
{
    gui_destroy_font( s_fontdesc );
}

usb_main_t* create_usb_main(void)
{
    return &s_usb_main_api;
}

static bool _open_theme(void)
{
    rotation_e angle;
    char file[512];

    // 取视图角度
    angle = sty_get_view_rotation();

    // 获取commonui 的 style 资源文件名
    sprintf(file, "%s/%s.sty", sys_get_common_theme_res_dir(), "commonui");
    // 获取应用的 style 资源文件名
    sprintf( s_usb_sty_file, "%s/%s.sty", sys_get_app_theme_res_dir(), "usb");

    s_common_style = g_style_api->open_style_file( file );
    s_usb_style = g_style_api->open_style_file( s_usb_sty_file );

    // 载入commonui的 style 场景
    if ((angle == ROTATION_0) || (angle == ROTATION_180))
    {
        s_common_scene = g_style_api->load_scene( s_common_style, COMMONUI_SIDELONG_SCENE);
    }
    else /* if ( ( angle == ROTATION_90 ) || ( angle == ROTATION_270 ) ) */
    {
        s_common_scene = g_style_api->load_scene( s_common_style, COMMONUI_UPRIGHT_SCENE);
    }
	
    if ( s_common_scene == NULL)
    {
        print_err("load commonui scene failed");
        return false;
    }
    else
    {
        // 设置 commonui 的 style 场景
        sty_set_common_scene( s_common_scene );
    }

    // 获取commonui 的图片资源文件名
    sprintf(file, "%s/%s.res", sys_get_common_theme_res_dir(), "commonui");
	
    // 打开 commonui 图片资源文件
    if ( sty_open_common_pic_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }

    // 获取 usb 的图片资源文件名
    sprintf(file, "%s/%s.res", sys_get_app_theme_res_dir(), "usb");

    // 打开应用的图片资源文件
    if (sty_open_app_pic_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }

    return true;

}

static bool _close_theme(void)
{

    g_style_api->free_scene( s_common_scene );
    s_common_scene = NULL;
	
    if ( sty_close_common_pic_res() == false)
    {
        print_err("close common picture resource error!");
        return false;
    }

    if ( sty_close_app_pic_res() == false)
    {
        print_err("close app picture resource error!");
        return false;
    }

    return true;
}

static bool _open_str_res(void)
{
    char file[512];

    // 获取commonui 字符串资源文件名
    sprintf(file, "%s/%s_%s.str", sys_get_common_string_res_dir(), "commonui", sys_get_cur_language_codes());
	
    // 打开 commonui 字符串资源文件
    if ( sty_open_common_str_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }

    // 获取setting应用字符串资源文件名
    sprintf(file, "%s/%s_%s.str", sys_get_app_string_res_dir(), "usb", sys_get_cur_language_codes());

    // 打开应用的字符串资源文件
    if ( sty_open_app_str_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }

    gui_set_language_mode( sys_get_cur_language_id());

    return true;
}

static bool _close_str_res(void)
{
    if ( sty_close_common_str_res() == false)
    {
        print_err("close common string resource error!");
        return false;
    }

    if ( sty_close_app_str_res() == false)
    {
        print_err("close app string resource error!");
        return false;
    }

    return true;
}

char* get_font_path(void)
{
    return s_font_path;
}

bool _send_msg_to_cofig( msg_apps_type_e type )
{
    msg_apps_t m;
    msg_reply_t reply;

    m.type = type;
    send_sync_msg( APP_NAME_CONFIG, &m, &reply, NULL);

    if ( reply.type == MSG_REPLY_SUCCESS)
    {
        return true;
    }

    return false;
}

static void _check_media_dir(void)
{
    hotplugin_dev_type_e main_disk ;

    main_disk = sys_get_main_disk();

    if(main_disk == HOTPLUGIN_DEV_TYPE_LOCAL)
    {
        /* 主盘是 本地磁盘*/
        sys_check_media_dir( sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_ALL), UDISK_ROOT ); 

    }
    else if(main_disk == HOTPLUGIN_DEV_TYPE_CARD )
    {
        /*主盘是卡*/
        sys_check_media_dir( sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CARD,DIR_ALL), CARD_ROOT ); 

    }
    else if(main_disk == HOTPLUGIN_DEV_TYPE_UHOST )
    {
        /*主盘是uhost盘*/
        sys_check_media_dir( sys_get_media_dir(HOTPLUGIN_DEV_TYPE_UHOST,DIR_ALL), EXTERNAL_ROOT );
    }
    else
    {
        /*没有盘*/
        print_dbg("has no main disk");
    }
}



