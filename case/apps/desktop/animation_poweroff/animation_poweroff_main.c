/*******************************************************************************
 *                              NOYA1100
 *                            Module: app
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-05-09          1.0             build this file 
*******************************************************************************/
/*!
 * \file     animation_poweroff_main.c
 * \brief    
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2009/05/09
*******************************************************************************/

#include "include_psp.h"
#include "include_case.h"

/*本应用的头文件*/
#include "anim_off_sty.h"
#include "animation_poweroff.h"
#include "animation_config.h"
#include "animation_res_scene.h"
#include "animation_decoder_scene.h"



/* extern function, 函数声明 */
static bool _lib_init( void );
static bool _lib_deinit( void );
static bool _init(void);
static bool _deinit(void);



/* define variable, 变量定义 */
animation_vars_t animation_vars;


#if 0
#endif
/*
* 应用运行时依赖的系统资源, 每个应用仅需进入时打开一次
* apps open system resource
*/
static bool _lib_init( void )
{
    animation_vars.gui_lib = dlopen("gui.so", RTLD_LAZY);
    if (animation_vars.gui_lib == NULL)
    {
        print_err("open gui.so err! \n");
        goto _lib_init_err;
    }

    animation_vars.sty_lib = dlopen("style.so", RTLD_LAZY);
    if (animation_vars.sty_lib == NULL)
    {
        print_err("open style.so err! \n");
        goto _lib_init_err;
    }

    animation_vars.cfg_lib = dlopen("apconfig.so", RTLD_LAZY);
    if (animation_vars.cfg_lib == NULL)
    {
        print_err("open apconfig.so err! \n");
        goto _lib_init_err;
    }

    animation_vars.applib_lib = dlopen("applib.so", RTLD_LAZY);
    if (animation_vars.applib_lib == NULL)
    {
        print_err("open applib.so err! \n");
        goto _lib_init_err;
    }

    animation_vars.commonui_lib = dlopen("commonui.so", RTLD_LAZY);
    if (animation_vars.commonui_lib == NULL)
    {
        print_err("open commonui.so err! \n");
        goto _lib_init_err;
    }
    return true;

    _lib_init_err: _lib_deinit();
    return false;
}


static bool _lib_deinit(void)
{
    //卸载动态库
    if (animation_vars.commonui_lib != NULL)
    {
        dlclose(animation_vars.commonui_lib);
        animation_vars.commonui_lib = NULL;
    }
    if (animation_vars.applib_lib != NULL)
    {
        dlclose(animation_vars.applib_lib);
        animation_vars.applib_lib = NULL;
    }
    if (animation_vars.cfg_lib != NULL)
    {
        dlclose(animation_vars.cfg_lib);
        animation_vars.cfg_lib = NULL;
    }
    if (animation_vars.sty_lib != NULL)
    {
        dlclose(animation_vars.sty_lib);
        animation_vars.sty_lib = NULL;
    }
    if (animation_vars.gui_lib != NULL)
    {
        dlclose(animation_vars.gui_lib);
        animation_vars.gui_lib = NULL;
    }
    return true;
}


/* application initial function */
static bool _init(void)
{
    char file[PATH_MAX];
    // 获取app 的 style 资源文件名
    sprintf(file, "%s/%s.sty", sys_get_app_theme_res_dir(), "anim_off");
    animation_vars.app_style = open_style_file(file);
    if (NULL == animation_vars.app_style)
    {
        print_err("open open_style_file err! \n");
        return false;
    }
    return true;
}

/* application deinit function */
static bool _deinit(void)
{
    if(animation_vars.app_style != NULL)
    {
        close_style_file( animation_vars.app_style );
        animation_vars.app_style = NULL;
    }
    return true;
}


int proc_sys_msg(msg_apps_t *msg)
{
    msg_reply_t reply;

    memset(&reply, 0x00, sizeof(reply));
    
    return 1;
}



/* application main function */
int main(int argc, const char *argv[])
{
    bool result = true;
    
    print_dbg("start poweroff application");

    _lib_init();
    /* 应用库初始化 */
    result = applib_init(argc, argv, APP_TYPE_GUI);
    if ( false == result )
    {
        return -1;
    }
    
    //禁止屏保, 省电
    sys_forbid_screensaver( true );
    sys_forbid_auto_standby( true);
    
    /* 应用初始化 */
    result = _init();
    if(false == result)
    {
        print_err("poweroff init error!\n");
        goto ANIM_POWEROFF_INIT_ERR;
    }
    
    animation_cfg_read_start_mode();
    
    if ( ANIMATION_START_MODE_RES == animation_vars.start_mode )
    {
        animation_res_scene( &animation_vars.res_scene );
    }
    else
    {
        animation_decoder_scene( &animation_vars.decoder_scene );
    }
    
    /* 应用退出 */
    ANIM_POWEROFF_INIT_ERR:
    //恢复屏保, 省电
    sys_forbid_screensaver( false );
    sys_forbid_auto_standby( false );
    
    _deinit();

    /* 应用库退出 */
    applib_quit();

    _lib_deinit();
    print_dbg("end poweroff application");
    
    return 0;
}

