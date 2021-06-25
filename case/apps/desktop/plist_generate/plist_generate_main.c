/*******************************************************************************
 *                              NOYA1100
 *                            Module: app
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-05-25          1.0             build this file 
*******************************************************************************/
/*!
 * \file     plist_generate_main.c
 * \brief    
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2009/05/25
*******************************************************************************/

#include "include_case.h"


/*本应用的头文件*/
#include "pl_gener_sty.h"
#include "plist_generate.h"
#include "plist_generate_scene.h"



/* extern function, 函数声明 */
static bool _lib_init( void );
static void _lib_deinit( void );
static bool _gui_init(void);
static bool _gui_deinit( void );
static bool _init(void);
static bool _deinit(void);

static bool _style_init(void);
static void _style_deinit(void);

/* define variable, 变量定义 */
plist_generate_vars_t plist_generate_vars;

static const clk_domain_t s_clk_domain = {CLK_LOAD_MIN, CLK_LOAD_HIGH_0};


#if 0
#endif
/*
* 应用运行时依赖的系统资源, 每个应用仅需进入时打开一次
* apps open system resource
*/
static bool _lib_init( void )
{
    plist_generate_vars.applib_lib = dlopen("applib.so", RTLD_LAZY);
    if(plist_generate_vars.applib_lib == NULL)
    {
        print_err("open applib.so err! \n");
        goto _LIB_INIT_ERR;
    }

    plist_generate_vars.commonui_lib = dlopen("commonui.so", RTLD_LAZY);
    if(plist_generate_vars.commonui_lib == NULL)
    {
        print_err("open commonui.so err! \n");
        goto  _LIB_INIT_ERR;
    }

    plist_generate_vars.apconfig_lib = dlopen("apconfig.so", RTLD_LAZY);
    if(plist_generate_vars.apconfig_lib == NULL)
    {
        print_err("open apconfig.so err! \n");
        goto  _LIB_INIT_ERR;
    }
    
    plist_generate_vars.gui_lib = dlopen("gui.so", RTLD_LAZY);
    if(plist_generate_vars.gui_lib == NULL)
    {
        print_err("open gui.so err! \n");
        goto  _LIB_INIT_ERR;
    }

    plist_generate_vars.sty_lib = dlopen("style.so", RTLD_LAZY);
    if(plist_generate_vars.sty_lib == NULL)
    {
        print_err("open style.so err! \n");
        goto  _LIB_INIT_ERR;
    }
    
    plist_generate_vars.sqlite_lib = dlopen("sqlite.so", RTLD_LAZY);
    if(plist_generate_vars.sqlite_lib == NULL)
    {
        print_err("open sqlite.so err! \n");
        goto  _LIB_INIT_ERR;
    }
    plist_generate_vars.mnavi_lib = dlopen("mnavi.so", RTLD_LAZY);
    if(plist_generate_vars.mnavi_lib == NULL)
    {
        print_err("open mnavi.so err! \n");
        goto  _LIB_INIT_ERR;
    }
    plist_generate_vars.playlist_lib = dlopen("playlist.so", RTLD_LAZY);
    if(plist_generate_vars.playlist_lib == NULL)
    {
        print_err("open playlist.so err! \n");
        goto  _LIB_INIT_ERR;
    }

    return true;

    _LIB_INIT_ERR:
    _lib_deinit();
    return false;
}


static void _lib_deinit( void )
{
    //卸载动态库
    if(plist_generate_vars.playlist_lib  != NULL)
    {
        dlclose( plist_generate_vars.playlist_lib );
        plist_generate_vars.playlist_lib = NULL;
    }
    if( plist_generate_vars.mnavi_lib != NULL)
    {
        dlclose( plist_generate_vars.mnavi_lib );
        plist_generate_vars.mnavi_lib = NULL;
    }
    if( plist_generate_vars.sqlite_lib != NULL)
    {
        dlclose( plist_generate_vars.sqlite_lib );
        plist_generate_vars.sqlite_lib = NULL;
    }
    if(plist_generate_vars.sty_lib != NULL)
    {
        dlclose( plist_generate_vars.sty_lib );
        plist_generate_vars.sty_lib = NULL;
    }
    if( plist_generate_vars.gui_lib != NULL)
    {
        dlclose( plist_generate_vars.gui_lib );
        plist_generate_vars.gui_lib = NULL;
    }
    if(plist_generate_vars.apconfig_lib != NULL)
    {
        dlclose( plist_generate_vars.apconfig_lib );
        plist_generate_vars.apconfig_lib = NULL;
    }

    if(plist_generate_vars.applib_lib != NULL)
    {
        dlclose( plist_generate_vars.applib_lib );
        plist_generate_vars.applib_lib = NULL;
    }
    if(plist_generate_vars.commonui_lib != NULL)
    {
        dlclose( plist_generate_vars.commonui_lib );
        plist_generate_vars.commonui_lib = NULL;
    }
}


static bool _style_init(void)
{
    char file[PATH_MAX];
    // 获取commonui 的 style 资源文件名
    sprintf(file, "%s/%s.sty", sys_get_common_theme_res_dir(), "commonui");
    plist_generate_vars.com_style = open_style_file( file );
    if (plist_generate_vars.com_style == NULL)
    {
        print_err("open common style file err!");
        return false;
    }

    // 获取app 的 style 资源文件名
    sprintf(file, "%s/%s.sty", sys_get_app_theme_res_dir(), "pl_gener");
    plist_generate_vars.app_style = open_style_file( file );
    if ( NULL == plist_generate_vars.app_style )
    {
        print_err("open open_style_file err! \n");
        close_style_file( plist_generate_vars.com_style );
        plist_generate_vars.com_style=NULL;
        return false;
    }
}

static void _style_deinit(void)
{
    if(plist_generate_vars.com_style!=NULL)
    {
        close_style_file( plist_generate_vars.com_style );
        plist_generate_vars.com_style=NULL;
    }
    if(plist_generate_vars.app_style !=NULL)
    {
        close_style_file( plist_generate_vars.app_style );
        plist_generate_vars.app_style =NULL;
    }
}

static bool _gui_init(void)
{
    int tmp = -1;
    
    //创建字体
    plist_generate_vars.fontdesc = gui_create_font( sys_get_default_font_file(), 16 );
    if( plist_generate_vars.fontdesc < 0 )
    {
        print_err("gui_create_font err\n\n"); 
        return false;
    }
    
    tmp = gui_dc_set_default_fontface( plist_generate_vars.fontdesc );
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
    gui_destroy_font( plist_generate_vars.fontdesc );
    plist_generate_vars.fontdesc = -1;

    return true;
}


/* application initial function */
static bool _init(void)
{
    bool result = true;

    memset( &plist_generate_vars, 0x00, sizeof(plist_generate_vars) );
    result = _lib_init();
    if ( false == result )
    {
        print_err("_system_init err");
        return false;
    }
    
    result = _gui_init();
    if ( false == result )
    {
        print_err("_gui_init err");
        return false;
    }   
    
    return result;
}

/* application deinit function */
static bool _deinit(void)
{
    bool result = true;

    print_dbg();
    
    _gui_deinit();
    _lib_deinit();
    
    return result;
}


int plist_generate_proc_sys_msg(msg_apps_t *msg)
{
    msg_reply_t reply;

//    print_dbg("msgcontent:%s", msg->content); //注意content内容超长越界
    print_info("msgtype:%d, content[0]:0x%x", msg->type, msg->content[0]);
    memset(&reply, 0x00, sizeof(reply));
    
    return 1;
}

/* application main function */
int main(int argc, const char *argv[])
{
    bool result = true;
    int value = 0;
    
    print_dbg("start plist generate application");

    /* 应用初始化 */
    result = _init();
    if(false == result)
    {
        print_err("plist generate init error!\n");
        goto PLIST_GENERATE_INIT_ERR;
    }
    
    /* 应用库初始化 */
    result = applib_init(argc, argv, APP_TYPE_GUI);
    if ( false == result )
    {
        goto PLIST_GENERATE_APPLIB_INIT_ERR;
    }
    
    if( sys_get_main_disk_dir(DIR_ROOT) == NULL )
    {
        // 清除标志
        set_config( CFG_NEED_GEN_PLIST, (char*)&value , 4 );
    
        print_err("main disk no exist!");
        goto PLIST_GENERATE_MAINDISK_ERR;   /* 无主盘退出 */
    }

    _style_init();

    sys_pm_set_clk_domain(&s_clk_domain);
    sys_enable_sys_evt_detect(false);

    plist_gene_scene( &plist_generate_vars.scene );

    sys_enable_sys_evt_detect(true);
    sys_pm_clear_clk_domain(&s_clk_domain);

    _style_deinit();

    PLIST_GENERATE_MAINDISK_ERR:
    /* 应用库退出 */
    applib_quit();

    PLIST_GENERATE_APPLIB_INIT_ERR:
    _deinit();

    /* 应用退出 */
    PLIST_GENERATE_INIT_ERR:
    print_dbg("end plist generate application");
    
    return 0;
}

