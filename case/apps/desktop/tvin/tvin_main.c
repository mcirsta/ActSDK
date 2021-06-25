/********************************************************************************
 *                              USDK(1100)
 *                             Module: tvin
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : tvin_main.c
 * History:
 *      <author>       <time>        <version >           <desc>
 *      Jerry Fu       2009-04-14      v1.0           build this file
 ********************************************************************************/
/*!
* \file     tvin_main.c
* \brief    tvin application entry
* \author   Jerry Fu
*
* \version 1.0
* \date  2009/04/14
*******************************************************************************/
#include "tvin.h"

#define GSENSOR_OPERATE 0

static const clk_domain_t s_clk_domain = {CLK_LOAD_MIN, CLK_LOAD_MAX};

#if GSENSOR_OPERATE == 1
static bool s_is_gsensor_opened = false;

/*打开或关闭gsensor*/
static bool _open_or_close_gsensor(bool on)
{
    msg_apps_t msg = {0};
    msg_reply_t reply = {0};

    msg.type = MSG_SET_GSENSOR_ON_SYNC;
    memcpy(msg.content, (void *)on, sizeof(on));
    send_sync_msg(APP_NAME_MSG_SERV, &msg, &reply, NULL);
 
    if(reply.type == MSG_REPLY_SUCCESS)
    {
        return true;
    }
    else
    {
        print_err("%s gsensor failed!", (on == true)? "open":"close");          
        return false;
    }
}

/* gsensor是否已打开*/
static bool _is_gsensor_opened(void)
{
    msg_apps_t msg = {0};
    msg_reply_t reply = {0};

    bool on;
    msg.type = MSG_GET_GSENSOR_ON_SYNC;
    send_sync_msg(APP_NAME_MSG_SERV, &msg, &reply, NULL);

    if(reply.type == MSG_REPLY_SUCCESS)
    {
        memcpy(&on, reply.content, sizeof(on));
        return on;
    }
    else
    {
        print_err("get gsensor state failed!");
        return false;
    }
}
#endif

static tvin_appdata_t s_app_data;

static bool _lib_init( void );
static bool _lib_deinit( void );

static bool _lib_init( void )
{
    s_app_data.applib_lib = dlopen("applib.so", RTLD_NOW);
    if (s_app_data.applib_lib == NULL)
    {
        print_err("open applib lib err!");
        goto _lib_init_err;
    }

    s_app_data.commonui_lib = dlopen("commonui.so", RTLD_NOW);
    if (s_app_data.commonui_lib == NULL)
    {
        print_err("open commonui lib err!");
        goto _lib_init_err;
    }

    s_app_data.fusion_lib = dlopen("fusion.so", RTLD_NOW);
    if (s_app_data.fusion_lib == NULL)
    {
        print_err("open fusion lib err!");
        goto _lib_init_err;
    }

    s_app_data.config_lib = dlopen("apconfig.so", RTLD_NOW);
    if (s_app_data.config_lib == NULL)
    {
        print_err("open apconfig lib err!");
        goto _lib_init_err;
    }

    s_app_data.gui_lib = dlopen("gui.so", RTLD_NOW);
    if (s_app_data.gui_lib == NULL)
    {
        print_err("open gui lib err!");
        goto _lib_init_err;
    }

    s_app_data.sty_lib = dlopen("style.so", RTLD_NOW);
    if (s_app_data.sty_lib == NULL)
    {
        print_err("open style lib err!");
        goto _lib_init_err;
    }

    return true;

    _lib_init_err:
    _lib_deinit();
    return false;
}

static bool _lib_deinit( void )
{
    if (s_app_data.sty_lib != NULL)
    {
        dlclose(s_app_data.sty_lib);
        s_app_data.sty_lib = NULL;
    }

    if (s_app_data.gui_lib != NULL)
    {
        dlclose(s_app_data.gui_lib);
        s_app_data.gui_lib = NULL;
    }

    if (s_app_data.config_lib != NULL)
    {
        dlclose(s_app_data.config_lib);
        s_app_data.config_lib = NULL;
    }

    if (s_app_data.fusion_lib != NULL)
    {
        dlclose(s_app_data.fusion_lib);
        s_app_data.fusion_lib = NULL;
    }

    if (s_app_data.commonui_lib != NULL)
    {
        dlclose(s_app_data.commonui_lib);
        s_app_data.commonui_lib = NULL;
    }

    if (s_app_data.applib_lib != NULL)
    {
        dlclose(s_app_data.applib_lib);
        s_app_data.applib_lib = NULL;
    }

    return true;
}

/******************************************************************************/
/*!
* \par  Description:
*	  初始化tvin应用程序的数据结构
* \param[in]    none
* \param[out]   none
* \return       成功返回true, 失败返回false 
* \ingroup      tvin
*******************************************************************************/
bool app_init(void)
{
    char file[PATH_MAX];
    
    sprintf(file, "%s/%s.sty", sys_get_app_theme_res_dir(), "tvin");
    s_app_data.tvin_style = open_style_file(file);

    sprintf(file, "%s/%s.sty", sys_get_common_theme_res_dir(), "commonui");
    s_app_data.commonui_style = open_style_file(file);

    tvin_cfg_read_all(g_tvin_cfgs, TVIN_CFG_INVALID);

    return true;
}


/******************************************************************************/
/*!
* \par  Description:
*	  卸载该应用已经加载的库，释放内存
* \param[in]    应用的数据结构指针
* \param[out]   none
* \return       true: 成功  false:有错误发生
* \ingroup      tvin
*******************************************************************************/
bool app_deinit( void )
{                
    print_dbg();    
    //    if(NULL != s_app_data.pTvinRecord)
    //    {
    //        if(true == s_app_data.pTvinRecord.tvin_install)
    //        {
    //            print_dbg();
    //            tvin_mmm_Close();
    //        }        
    //    }
    tvin_unstall_videoin();
    print_dbg();
    
    
    close_style_file(s_app_data.tvin_style);
    close_style_file(s_app_data.commonui_style);
    
    if(NULL != s_app_data.palntsc_data)
    {
        free(s_app_data.palntsc_data);
    }

    if(NULL != s_app_data.pTvinRecord)
    {
        free(s_app_data.pTvinRecord);
    }
    
    return true;
}


/******************************************************************************/
/*!
* \par  Description:
*	  tvin程序的入口函数
* \param[in]    输入参数
* \param[out]   none
* \return
* \ingroup      tvin
*******************************************************************************/
int main(int argc, const char *argv[])
{
    scene_tvin_ret_e  ret;

    //dump_all_module_mem_info();

    if(_lib_init() == false)
    {
        return -1;
    }

    /* 应用库初始化 */
    applib_init(argc, argv, APP_TYPE_GUI);
    
    sys_pm_set_clk_domain(&s_clk_domain);

    /* 应用初始化 */
    if(app_init() == false)
    {
        return -1;
    }

#if GSENSOR_OPERATE == 1
    /*tvin与g-sensor有冲突, 在进入这个应用时，需要先关闭g-sensor, 退出后再打开*/
    s_is_gsensor_opened = _is_gsensor_opened();
    if(s_is_gsensor_opened == true)
    {
        /*关闭gsensor*/
        _open_or_close_gsensor(false);
    }
#endif

    s_app_data.dev_installed = tvin_install_videoin();

    if((true == s_app_data.dev_installed) && (argc > 1))
    {
        print_dbg("%d,%s,%s",argc, argv[1], argv[2]);
        sscanf(argv[2],"%d", &(s_app_data.alarm_clock));
        s_app_data.app_start_mode = APP_START_RTC;
        ret = SCENE_TVIN_RECORD;
    }		        
    else
    {
        print_dbg("start from lanuncher\n");
        s_app_data.app_start_mode = APP_START_NORMAL;
        ret = SCENE_TVIN_PALNTSC;
    }
    
    while(ret != SCENE_TVIN_EXIT )
    {
        switch(ret)
        {
        case SCENE_TVIN_PALNTSC:
            if(true == scene_palntsc(&s_app_data))
            {
                ret = s_app_data.palntsc_data->next_scene;
            }
            else
            {
                ret = SCENE_TVIN_EXIT; 
            }
            break;

        case SCENE_TVIN_RECORD:
            if(true == scene_tvin_record(&s_app_data))
            {
                ret = s_app_data.pTvinRecord->next_scene;
            }
            else
            {
                ret = SCENE_TVIN_EXIT;
            }            
            break;

        default:
            break;
        }
    }

    print_dbg("save_angle = %d\n", s_app_data.save_angle);
    sty_set_screen_rotation(s_app_data.save_angle);  // 设置屏幕方向

        
    /* 应用退出 */
    app_deinit();

#if GSENSOR_OPERATE == 1
    /*tvin与g-sensor有冲突, 在进入这个应用时，需要先关闭g-sensor, 退出后再打开*/
    if(s_is_gsensor_opened == true)
    {
        /*重打开gsensor*/
        _open_or_close_gsensor(true);
    }
#endif

    sys_pm_clear_clk_domain(&s_clk_domain);
     
    /* 应用库退出 */
    applib_quit();   
    _lib_deinit();

    //dump_all_module_mem_info();
    return 0;
}


