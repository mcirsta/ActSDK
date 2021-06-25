/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : ebook_main.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *    zhaowenjie    2009-2-25          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     ebook_main.c
 * \brief    ebook application
 * \author   zhaowenjie
 *
 * \version 1.0
 * \date  2009/02/25
 *******************************************************************************/

#include "include_case.h"


#include "style.h"
#include "ebook.h"
#include "ebook_config.h"
#include "jbook.h"

static const clk_domain_t s_clk_domain = { (unsigned int)CLK_LOAD_MIN, (unsigned int)CLK_LOAD_HIGH_0 };


static void* s_applib_lib;
static void* s_commonui_lib;
static void* s_fusion_lib;

static void* s_gui_lib;
static void* s_apconfig_lib;

static void* s_sqlite_lib;
static void* s_mnavi_lib;
static void* s_playlist_lib;

static void* s_style_lib;

static int s_fontdesc;

static char s_ebook_sty_file[PATH_MAX];
static char s_commonui_sty_file[PATH_MAX];

static style_t* s_ebook_style;
static style_t* s_common_style;
static scene_t* s_common_scene = NULL; /* COMMON WIDGET */
static scene_t* s_util_scene = NULL;

static picture_resource_t* s_headbar_picture = NULL;
static gui_bitmap_info_t* s_background_bitmap = NULL;

static string_resource_t* s_no_disk_string;
static picture_resource_t* s_dot_picture = NULL;
static string_resource_t* s_ebook_string = NULL;
static string_resource_t* s_sure_delete_string = NULL;
static string_resource_t* s_deleting_string = NULL;
static string_resource_t* s_not_found_string = NULL;
static string_resource_t* s_file_open_fail_string = NULL;

static scene_e s_current_scene; /* 当前scene */
static scene_e s_last_scene; /* 前一个scene */

static bool _ebook_init(void);
static bool _load_libs(void);
static bool _unload_libs(void);
static bool _load_style_resource(void);
static bool _ebook_deinit(void);
static void _enter_scene_schedule(void);

static ebook_main_t* _create_ebook_main(void);

style_t* get_style(void);

void set_current_scene(scene_e scene);
scene_e get_current_scene(void);
void go_back_last_scene(void);
scene_e get_last_scene(void);

unsigned short get_headbar_pid(void);
gui_bitmap_info_t* get_background_picture(void);
void update_background_picture(void);

bool check_file_exists( char* file_path );

string_resource_t* get_no_disk_string(void);
picture_resource_t* get_dot_picture(void);
string_resource_t* get_ebook_string(void);
string_resource_t* get_sure_delete_string(void);
string_resource_t* get_deleting_string(void);
string_resource_t* get_not_found_string(void);
string_resource_t* get_file_open_fail_string(void);

bool remove_file( char* file );

static bool _open_theme(void);
static bool _close_theme(void);
static bool _open_str_res(void);
static bool _close_str_res(void);

bool change_common_ui_scene( void );

static ebook_main_t s_ebook_main_api =
{
    get_style,
    change_common_ui_scene,

    set_current_scene,
    get_current_scene,
    go_back_last_scene,
    get_last_scene,

    get_headbar_pid,
    get_background_picture,
    update_background_picture,
    
    get_no_disk_string,
    get_dot_picture,
    get_ebook_string,
    get_sure_delete_string,
    get_deleting_string,
    get_not_found_string,
    get_file_open_fail_string,

    check_file_exists,
    remove_file
};

/* application initial function */
static bool _ebook_init(void)
{
    bool ret;

    sys_pm_set_clk_domain( &s_clk_domain );

    /* 创建窗口的默认字体 */
    s_fontdesc = gui_create_font( sys_get_default_font_file(), 16 );
    if( s_fontdesc < 0 )
    {
        print_err("");
        return false;
    }
    gui_dc_set_default_fontface( s_fontdesc );


    g_sty_dc = get_sty_dc();
    g_style_api = get_style_api();
    
    ret = _load_style_resource();
    if ( ret == false )
    {
        return false;
    }

    return true;
}

static bool _load_libs(void)
{

    s_applib_lib = dlopen("applib.so", RTLD_LAZY );
    if (s_applib_lib == NULL)
    {
        print_err(" open applib err! \n");
        goto _load_libs_err;
    }

    s_commonui_lib = dlopen("commonui.so", RTLD_LAZY );
    if (s_commonui_lib == NULL)
    {
        print_err(" open commonui err! \n");
        goto _load_libs_err;
    }

    s_fusion_lib = dlopen("fusion.so", RTLD_LAZY );
    if (s_fusion_lib == NULL)
    {
        print_err(" open fusion err! \n");
        goto _load_libs_err;
    }

    s_apconfig_lib = dlopen("apconfig.so", RTLD_LAZY );
    if (s_apconfig_lib == NULL)
    {
        print_err(" open s_apconfig_lib err! \n");
        goto _load_libs_err;
    }

    s_sqlite_lib = dlopen("sqlite.so", RTLD_LAZY );
    if (s_sqlite_lib == NULL)
    {
        print_err(" open s_sqlite_lib err! \n");
        goto _load_libs_err;
    }

    s_mnavi_lib = dlopen("mnavi.so", RTLD_LAZY );
    if (s_mnavi_lib == NULL)
    {
        print_err(" open s_mnavi_lib err! \n");
        goto _load_libs_err;
    }

    s_playlist_lib = dlopen("playlist.so", RTLD_LAZY );
    if (s_playlist_lib == NULL)
    {
        print_err(" open s_playlist_lib err! \n");
        goto _load_libs_err;
    }

    s_style_lib = dlopen("style.so", RTLD_LAZY );
    if ( s_style_lib == NULL)
    {
        print_err("open style lib err! \n");
        goto _load_libs_err;
    }

    s_gui_lib = dlopen("gui.so", RTLD_LAZY );
    if (s_gui_lib == NULL)
    {
        print_err("open gui lib err! \n");
        goto _load_libs_err;
    }
    return true;

    _load_libs_err:
    _unload_libs();
    return false;
}

static bool _unload_libs(void)
{

    /* 卸载动态库 */
    if(s_applib_lib != NULL)
    {
        dlclose(s_applib_lib);
        s_applib_lib = NULL;
    }
    if(s_commonui_lib != NULL)
    {
        dlclose(s_commonui_lib);
        s_commonui_lib = NULL;
    }
    if(s_fusion_lib != NULL)
    {
        dlclose(s_fusion_lib);
        s_fusion_lib = NULL;
    }
    if(s_gui_lib != NULL)
    {
        dlclose(s_gui_lib);
        s_gui_lib = NULL;
    }
    if(s_apconfig_lib != NULL)
    {
        dlclose(s_apconfig_lib);
        s_apconfig_lib = NULL;
    }
    if(s_sqlite_lib != NULL)
    {
        dlclose(s_sqlite_lib);
        s_sqlite_lib = NULL;
    }
    if(s_mnavi_lib != NULL)
    {
        dlclose(s_mnavi_lib);
        s_mnavi_lib = NULL;
    }
    if(s_playlist_lib != NULL)
    {
        dlclose(s_playlist_lib);
        s_playlist_lib = NULL;
    }
    if(s_style_lib  != NULL)
    {
        dlclose(s_style_lib);
        s_style_lib = NULL;
    }
    return true;
}

static bool _load_style_resource(void)
{
    bool ret;
    
    /* 装载资源文件 */
    ret = _open_theme();
    if ( ret == false )
    {
        return false;
    }
    
    ret = _open_str_res();
    if ( ret == false )
    {
        return false;
    }

    s_ebook_style = g_style_api->open_style_file( s_ebook_sty_file );

    /* 取出公用资源的scene */
    s_util_scene = g_style_api->load_scene( s_ebook_style, UTIL_SCENE );
    if (s_util_scene == NULL)
    {
        print_err(" load s_util_scene failure \n");
        return false;
    }
    else
    {
        g_sty_dc->sty_set_app_scene( s_util_scene );
    
        s_headbar_picture = (picture_resource_t *) g_style_api->get_scene_child(s_util_scene, PIC_HEADBAR );
        s_dot_picture = (picture_resource_t *) g_style_api->get_scene_child(s_util_scene, PIC_OPT_DOT );

        s_sure_delete_string = (string_resource_t*) g_style_api->get_scene_child(s_util_scene, STR_SURE_DELETE );
        s_ebook_string = (string_resource_t*) g_style_api->get_scene_child(s_util_scene, STR_EBOOK );
        s_deleting_string = (string_resource_t*) g_style_api->get_scene_child(s_util_scene, STR_DELETING );
        s_not_found_string = (string_resource_t*) g_style_api->get_scene_child(s_util_scene, STR_NOT_FOUND );
        s_file_open_fail_string = (string_resource_t*) g_style_api->get_scene_child(s_util_scene, STR_FILE_OPEN_FAIL );
        s_no_disk_string = (string_resource_t*) g_style_api->get_scene_child(s_util_scene, STR_NO_DISK );

        if ((s_headbar_picture == NULL) || (s_dot_picture == NULL) 
            || ( s_ebook_string == NULL) || (s_sure_delete_string == NULL) 
            || ( s_deleting_string == NULL) || ( s_not_found_string == NULL)
            || ( s_file_open_fail_string == NULL ) || ( s_no_disk_string == NULL ) )
        {
            print_err(" load util resources failure \n");
            return false;
        }
    }

    /* 设置语言编码 */
    g_sty_dc->sty_set_text_encoding(GUI_UTF8_ENC);

    return true;
}


/* application exit function */
static bool _ebook_deinit(void)
{
    bool ret;

    if ( s_background_bitmap != NULL )
    {
        g_sty_dc->sty_delete_bitmap( s_background_bitmap );
        s_background_bitmap = NULL;
    }

    g_style_api->free_scene(s_util_scene);
    s_util_scene = NULL;
    s_headbar_picture = NULL;
    s_dot_picture = NULL;
    s_sure_delete_string = NULL;
    s_ebook_string = NULL;
    s_deleting_string = NULL;
    s_not_found_string = NULL;
    s_file_open_fail_string = NULL;
    s_no_disk_string = NULL;

    _close_theme();
    _close_str_res();

    g_style_api->close_style_file( s_common_style );
    g_style_api->close_style_file( s_ebook_style );

    g_style_api = NULL;
    g_sty_dc = NULL;

    g_scene_mainmenu = NULL;

    if ( g_scene_dirlist != NULL )
    {
        g_scene_dirlist->remove_dirlist();
        g_scene_dirlist = NULL;
    }
    
    if ( g_scene_bookmark != NULL )
    {
        ret = g_scene_bookmark->quit_bookmark();
        if (ret == false)
        {
            print_err(" ebook exit error! \n");
            return false;
        }
        g_scene_bookmark = NULL;
    }

    if ( g_scene_read != NULL )
    {
        g_scene_read->close_reading_book();
        g_scene_read = NULL;
    }
    
    g_ebook_main = NULL;

    gui_destroy_font( s_fontdesc );

    sys_pm_clear_clk_domain(&s_clk_domain);

    return true;
}

/* ebook main function */
int main(int argc, const char *argv[])
{
    bool ret = true;
    hotplugin_dev_type_e main_disk ; 
    dialog_init_t dialog_init;

    print_info("ebook start .....................\n");

    _load_libs();
    /* 应用库初始化 */
    applib_init(argc, argv, APP_TYPE_GUI);

    /* 应用初始化 */
    ret = _ebook_init();
    if (ret == false)
    {
        print_err("app init error! \n");
        goto ERROR_EXIT;
    }

    main_disk = sys_get_main_disk();
    if( main_disk == HOTPLUGIN_DEV_TYPE_NONE )
    {
        /* 模态提示框 : 没有任何存储器 */
        dialog_init.head = DIALOG_HEAD_WARN;
        dialog_init.str_id = s_no_disk_string->str_id;
        dialog_modal( &dialog_init );

        goto ERROR_EXIT;
    }
    

    g_ebook_main = _create_ebook_main();
    g_scene_mainmenu = create_scene_mainmenu();
    g_scene_dirlist = create_scene_dirlist();
    g_scene_bookmark = create_scene_bookmark();

//	    ret = g_scene_bookmark->open_bookmark();
//	    if (ret == false)
//	    {
//	        print_err(" open_bookmark error! \n");
//	        s_current_scene = SCENE_EXIT;
//	    }

    g_scene_read = create_scene_read();

    get_configs();

    /* 进入场景调度器 */
    if ( argc > 2 )
    {
        char* temp = NULL;
        char separator = '/';
    
        /* browser 从参数传入要阅读的文件 
           格式为 : ebook.app playing   "xxxx.txt" */
        g_scene_read->set_file_full_path( argv[ 2 ] );

        /* 取得最后一个分隔符的位置 */
        temp = strrchr( argv[ 2 ], (int)separator );
        if ( temp != NULL )
        {
//              print_dbg(" file name ==== %s ", temp );
            g_scene_read->set_file_name( (const char*)(temp + 1) );
        }

        /* 直接进入阅读场景，退出场景后，直接退出应用 */
        s_last_scene = SCENE_EXIT;
        s_current_scene = SCENE_READ;
    }
    else
    {
        s_last_scene = SCENE_EXIT;
        s_current_scene = SCENE_MAIN_MENU;
    }

    /* 进入场景调度 */
    _enter_scene_schedule();

    set_configs();

    ERROR_EXIT:

    /* 应用退出 */
    ret = _ebook_deinit();
    if (ret == false)
    {
        print_dbg("ebook deinit error! \n");
    }

    /* 应用库退出 */
    applib_quit();

    _unload_libs();
    print_dbg(">>>>>>>>>>>>>>>>>>>>>> ebook exit = %d\n", ret);

    return true;
}

static void _enter_scene_schedule(void)
{
    while (s_current_scene != SCENE_EXIT)
    {
        switch (s_current_scene)
        {
            case SCENE_MAIN_MENU:
            g_scene_mainmenu->enter_scene_mainmenu();
            break;

            case SCENE_READ:
            g_scene_read->enter_scene_read();
            break;

            case SCENE_DIRECTORY_LIST:
            g_scene_dirlist->enter_scene_dirlist();
            break;

            case SCENE_BOOKMARK_LIST:
            g_scene_bookmark->enter_scene_bookmark();
            break;

            default:

            s_current_scene = SCENE_EXIT;
            break;
        }
    }
}

style_t* get_style(void)
{
    return s_ebook_style;
}

void set_current_scene(scene_e scene)
{
    s_last_scene = s_current_scene;
    s_current_scene = scene;
}

scene_e get_current_scene(void)
{
    return s_current_scene;
}

scene_e get_last_scene(void)
{
    return s_last_scene;
}

void go_back_last_scene(void)
{
    scene_e current = s_current_scene;

    s_current_scene = s_last_scene;
    s_last_scene = current;
}

unsigned short get_headbar_pid(void)
{
    return s_headbar_picture->pic_id;
}

gui_bitmap_info_t* get_background_picture(void)
{
    if ( s_background_bitmap == NULL )
    {
        update_background_picture();
    }

    return s_background_bitmap;
}

void update_background_picture(void)
{
    picture_resource_t* picture = NULL;
    rotation_e angle;

    if ( s_background_bitmap != NULL )
    {
        g_sty_dc->sty_delete_bitmap( s_background_bitmap );
        s_background_bitmap = NULL;
    }

    s_background_bitmap = get_user_bg_pic( BG_EBOOK );

    if ( s_background_bitmap == NULL )
    {
        angle = g_sty_dc->sty_get_view_rotation();
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

        s_background_bitmap = g_sty_dc->sty_create_bitmap( (int)picture->pic_id );
    }
}

picture_resource_t* get_dot_picture(void)
{
    return s_dot_picture;
}

string_resource_t* get_sure_delete_string(void)
{
    return s_sure_delete_string;
}

string_resource_t* get_ebook_string(void)
{
    return s_ebook_string;
}

string_resource_t* get_deleting_string(void)
{
    return s_deleting_string;
}

string_resource_t* get_not_found_string(void)
{
    return s_not_found_string;
}

string_resource_t* get_file_open_fail_string(void)
{
    return s_file_open_fail_string;
}

string_resource_t* get_no_disk_string(void)
{
    return s_no_disk_string;
}

bool check_file_exists( char* file_path )
{
    file_format_e file_format;
    char* string = NULL;
    FILE* file = NULL;

    /* 判断文件的格式 */
    string = strstr( file_path, LOW_TXT_FILE_FORMAT );
    if ( string != NULL )
    {
        file_format = TXT_FILE;
    }
    else
    {
        string = strstr( file_path, TXT_FILE_FORMAT );
        if ( string != NULL )
        {
            file_format = TXT_FILE;
        }
        else
        {
            file_format = JAR_FILE;
        }
    }
       

    if ( file_format == TXT_FILE )
    {
        file = fopen( file_path, READ_ONLY );
    }
    else
    {
        file = jbook_open( file_path );
    }

    if( file == NULL )
    {
        dialog_init_t init;
                
        init.head = DIALOG_HEAD_FAIL;
        init.str_id = s_file_open_fail_string->str_id;

        dialog_modal( &init );
    
        return false;
    }

    if ( file_format == TXT_FILE )
    {
        fclose( file );
    }
    else
    {
        jbook_close( file );
    }

    return true;
}

bool remove_file( char* file )
{
    int ret;
    ret = remove( file );
    if ( ret != -1 )
    {
        FSEL_HANDLE fsel;
        fsel = g_scene_dirlist->get_fsel_handle();
    
        /* 更新文件选择器的数据库 */
        if( fsel != NULL)
        {
            fsel_update( fsel , file, FSEL_DELDATA );
        }
        return true;
    }

    return false;
}

static ebook_main_t* _create_ebook_main(void)
{
    return &s_ebook_main_api;
}













static bool _open_theme(void)
{    
    char file[512];

    /* 获取 commonui 的 style 资源文件名 */ 
    sprintf( s_commonui_sty_file, "%s/%s.sty", sys_get_common_theme_res_dir(), "commonui");
   
    s_common_style = g_style_api->open_style_file( s_commonui_sty_file );

    change_common_ui_scene();
    
    /* 获取应用的 style 资源文件名 */ 
    sprintf(s_ebook_sty_file, "%s/%s.sty", sys_get_app_theme_res_dir(), "ebook");

    /* 获取 commonui 的图片资源文件名 */ 
    sprintf(file, "%s/%s.res", sys_get_common_theme_res_dir(), "commonui");

    /* 打开 commonui 图片资源文件 */ 
    if (g_sty_dc->sty_open_common_pic_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }

    /* 获取 ebook 的图片资源文件名 */ 
    sprintf(file, "%s/%s.res", sys_get_app_theme_res_dir(), "ebook");

    /* 打开 ebook 的图片资源文件 */ 
    if (g_sty_dc->sty_open_app_pic_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }

    return true;
}

bool change_common_ui_scene( void )
{
    rotation_e angle;

    /* 取视图角度 */ 
    angle = g_sty_dc->sty_get_view_rotation();
    
    if ( s_common_scene != NULL )
    {
        g_style_api->free_scene(s_common_scene);
        s_common_scene = NULL;
    }

    /* 载入 commonui的 style 场景 */ 
    if ( ( angle == ROTATION_0 ) || ( angle == ROTATION_180 ))
    {
        s_common_scene = g_style_api->load_scene( s_common_style, COMMONUI_SIDELONG_SCENE);
    }
    else /* if ( ( angle == ROTATION_90 ) || ( angle == ROTATION_270 ) ) */
    {
        s_common_scene = g_style_api->load_scene( s_common_style, COMMONUI_UPRIGHT_SCENE);
    }

    if (s_common_scene == NULL)
    {
        print_err("load commonui scene failed");
        return false;
    }

    /* 设置 commonui 的 style 场景 */ 
    g_sty_dc->sty_set_common_scene(s_common_scene);
   
    return true;
}

static bool _close_theme(void)
{

    g_style_api->free_scene(s_common_scene);
    s_common_scene = NULL;

    if (g_sty_dc->sty_close_common_pic_res() == false)
    {
        print_err("close common picture resource error!");
        return false;
    }

    if (g_sty_dc->sty_close_app_pic_res() == false)
    {
        print_err("close app picture resource error!");
        return false;
    }

    return true;
}

static bool _open_str_res(void)
{
    char file[512];

    /* 获取commonui 字符串资源文件名 */ 
    sprintf(file, "%s/%s_%s.str", sys_get_common_string_res_dir(), "commonui", sys_get_cur_language_codes());

    /* 打开 commonui 字符串资源文件 */ 
    if (g_sty_dc->sty_open_common_str_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }

    /* 获取 应用字符串资源文件名 */ 
    sprintf(file, "%s/%s_%s.str", sys_get_app_string_res_dir(), "ebook", sys_get_cur_language_codes());

    /* 打开应用的字符串资源文件 */ 
    if (g_sty_dc->sty_open_app_str_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }

    gui_set_language_mode( sys_get_cur_language_id());

    return true;
}

static bool _close_str_res(void)
{
    if (g_sty_dc->sty_close_common_str_res() == false)
    {
        print_err("close common string resource error!");
        return false;
    }

    if (g_sty_dc->sty_close_app_str_res() == false)
    {
        print_err("close app string resource error!");
        return false;
    }

    return true;
}

