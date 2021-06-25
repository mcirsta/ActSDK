/*******************************************************************************
 *                              USDK(1100)
 *                            Module: camera
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *        2009-5-26 17:9   1.0             build this file
 *******************************************************************************/
/*!
 * \file     camera_main.c
 * \brief    概述
 * \author
 * \par      GENERAL DESCRIPTION:
 *               camera主程序
 * \par      EXTERNALIZED FUNCTIONS:
 *               用到外面的模块
 * \version 1.0
 * \date  2009-5-26
 *******************************************************************************/
#include "include_psp.h"
#include "include_case.h"
#include "camera_main.h"
#include "camera_scene_dsc.h"
#include "camera_scene_dv.h"
#include "camera_scene_pc.h"
#include "camera_define.h"

/* application initial function */

static bool _lib_init(app_camera_t *data);
static bool _lib_deinit(app_camera_t *data);

static bool _app_init(app_camera_t *data);
static bool _app_deinit(app_camera_t *data);


app_camera_t* g_app_camera = NULL;
bool g_option_flag = false;
bool g_setpath = true;
static app_camera_t s_app_data;


static bool _lib_init(app_camera_t *data)
{

    data->applib_lib = dlopen("applib.so", RTLD_LAZY);
    if (data->applib_lib == NULL)
    {
        print_err("open applib err! \n");
        goto _lib_init_err;
    }

    data->commonui_lib = dlopen("commonui.so", RTLD_LAZY);
    if (data->commonui_lib == NULL)
    {
        print_err("open commonui err! \n");
        goto _lib_init_err;
    }

    data->fusion_lib = dlopen("fusion.so", RTLD_LAZY);
    if (data->fusion_lib == NULL)
    {
        print_err("open fusion err! \n");
        goto _lib_init_err;
    }

    data->gui_lib = dlopen("gui.so", RTLD_LAZY);
    if (data->gui_lib == NULL)
    {
        print_err("open gui lib err! \n");
        goto _lib_init_err;
    }

    data->sty_lib = dlopen("style.so", RTLD_LAZY);
    if (data->sty_lib == NULL)
    {
        print_err("open style lib err! \n");
        goto _lib_init_err;
    }

    data->config_lib = dlopen("apconfig.so", RTLD_LAZY);
    if (data->config_lib == NULL)
    {
        print_err("open apconfig lib err!");
        goto _lib_init_err;
    }

    data->mnavi_lib = dlopen("mnavi.so", RTLD_LAZY);
    if (data->mnavi_lib == NULL)
    {
        print_err("load mnavi.so failed.\n");
        goto _lib_init_err;
    }

    return true;

    _lib_init_err:
    _lib_deinit(data);
    return false;
}

static bool _lib_deinit(app_camera_t *data)
{
    if (data->mnavi_lib != NULL)
    {
        dlclose(data->mnavi_lib);
        data->mnavi_lib = NULL;
    }
    
    if (data->sty_lib != NULL)
    {
        dlclose(data->sty_lib);
        data->sty_lib = NULL;
    }

    if (data->config_lib != NULL)
    {
        dlclose(data->config_lib);
        data->config_lib = NULL;
    }

    if (data->gui_lib != NULL)
    {
        dlclose(data->gui_lib);
        data->gui_lib = NULL;
    }

    if (data->applib_lib != NULL)
    {
        dlclose(data->applib_lib);
        data->applib_lib = NULL;
    }

    if (data->commonui_lib != NULL)
    {
        dlclose(data->commonui_lib);
        data->commonui_lib = NULL;
    }

    if (data->fusion_lib != NULL)
    {
        dlclose(data->fusion_lib);
        data->fusion_lib = NULL;
    }

    return true;
}


static bool _app_init(app_camera_t *data)
{
    char common_file[PATH_MAX];
    char app_file[PATH_MAX];

    /*获取commonui 的 style 资源文件名 */
    snprintf(common_file, sizeof(common_file), "%s/%s.sty", sys_get_common_theme_res_dir(), "commonui");
    data->common_style = open_style_file(common_file);
    if (NULL == data->common_style)
    {
        print_err("open common open_style_file err! \n");
        goto common_sty_err;
    }

    /*获取应用的 style 资源文件名*/
    snprintf(app_file, sizeof(app_file), "%s/%s.sty", sys_get_app_theme_res_dir(), "camera");
    data->app_style = open_style_file(app_file);
    if (NULL == data->app_style)
    {
        print_err("open app open_style_file err! \n");
        goto app_sty_err;
    }
    return true;

    app_sty_err: close_style_file(data->common_style);
    common_sty_err: return false;
}

/* application exit function */
static bool _app_deinit(app_camera_t *data)
{
    if(data->common_style != NULL)
    {
        close_style_file(data->common_style);
        data->common_style = NULL;
    }

    if(data->app_style!= NULL)
    {
        close_style_file(data->app_style);
        data->app_style= NULL;
    }
    
    if (g_camera_scene_pc != NULL)
    {
        free(g_camera_scene_pc);
        g_camera_scene_pc = NULL;
    }
    
    if (g_camera_scene_dsc!= NULL)
    {
        free(g_camera_scene_dsc);
        g_camera_scene_dsc= NULL;
    }

    if (g_camera_scene_dv!= NULL)
    {
        free(g_camera_scene_dv);
        g_camera_scene_dv= NULL;
    }

    g_app_camera = NULL;
    
    return true;
}

static const clk_domain_t s_clk_domain = {(unsigned int)CLK_LOAD_MIN, (unsigned int)CLK_LOAD_MAX};

int main(int argc, char *argv[])
{
    int ret = 0;
    bool result = false;

#if (DBG_REMNANT_MEMORY == 1)
    unsigned int u;
    u = os_mem_query ();
    printf("-----into camera----os_mem_query_remnant=%x\n",u);
    print_mem();
    printf("-----show all module mem use info--(begin)---\n\n",u);
    dump_all_module_mem_info();
    printf("-----show all module mem use info---(end)--\n\n",u);
#endif /* DBG_REMNANT_MEMORY */

    printf("enter camera main\n");

    result = _lib_init(&s_app_data);
    if(result == false)
    {
        print_err("_lib_init failed.");
        return -1;
    }

    result = applib_init(argc, (const char **)argv, APP_TYPE_GUI);
    if (result == false)
    {
        print_err("applib init error!");
        goto applib_init_err;
    }

    sys_pm_set_clk_domain(&s_clk_domain);

    result = _app_init(&s_app_data);
    if (result == false)
    {
        print_err("app init error! \n");
        _app_deinit(&s_app_data);
        goto app_init_err;
    }
    else
    {
        g_app_camera= (app_camera_t*)&s_app_data;
    }

    if (argc == 2)
    {
        /*! argv[0] 代表本应用名，从argv[1]开始是启动参数 */
        if (!strcasecmp(argv[1], CAMERA_ARGV_PCCAM))
        {
            /*! pc camera */
            s_app_data.next_scene = CAM_SCENE_PC;
        }
        else
        if (!strcasecmp(argv[1], APPLICATION_ARGV_RESUME))
        {
            /*!应用调用 回放后再返回应用进入这里*/
            int temp_scene = 0;
            /*需要回放的时候，设配置值拍照=1，录制=2，
            在回放结束后argv启动camera的时候读配置来判断启动场景*/
            if (get_config(CFG_CAM_PLAYBACK_SCENE, (char*)&temp_scene, sizeof(temp_scene)) < 0)
            {
                print_err("get config fail, errno = 0x%x", errno);
                s_app_data.next_scene = CAM_SCENE_DSC;                
            }  
            else
            {
                g_option_flag = true;
                
                if (temp_scene == 2)
                {
                    s_app_data.next_scene = CAM_SCENE_DV;                    
                }
                else
                {
                    s_app_data.next_scene = CAM_SCENE_DSC;                    
                }
            }
        }
        else
        {
            int temp_scene = 0;
            /*出厂可定制默认进入camera中选为拍照还是录制，设配置值拍照=1，录制=2*/
            if (get_config(CFG_CAM_DEFAULT_SCENE, (char*)&temp_scene, sizeof(temp_scene)) < 0)
            {
                print_err("get config fail, errno = 0x%x", errno);
                s_app_data.next_scene = CAM_SCENE_DSC;   
            } 

            if (temp_scene == 2)
            {
                s_app_data.next_scene = CAM_SCENE_DV;                    
            }
            else
            {
                s_app_data.next_scene = CAM_SCENE_DSC;                    
            }            
        }
    }
    else
    {
        int temp_scene = 0;
        /*出厂可定制默认进入camera中选为拍照还是录制，设配置值拍照=1，录制=2*/
        if (get_config(CFG_CAM_DEFAULT_SCENE, (char*)&temp_scene, sizeof(temp_scene)) < 0)
        {
            print_err("get config fail, errno = 0x%x", errno);
            s_app_data.next_scene = CAM_SCENE_DSC;   
        } 

        if (temp_scene == 2)
        {
            s_app_data.next_scene = CAM_SCENE_DV;                    
        }
        else
        {
            s_app_data.next_scene = CAM_SCENE_DSC;                    
        }            
    }    
    sys_forbid_screensaver(true);
    sys_forbid_auto_standby(false);
    sys_set_key_tone_enable(0);

    /* 进入场景 */
    while (s_app_data.next_scene != CAM_SCENE_EXIT)
    {
        switch (s_app_data.next_scene)
        {
            case CAM_SCENE_DSC:
            ret = camera_scene_dsc();
            s_app_data.next_scene = ret;
            break;
            case CAM_SCENE_DV:
            ret = camera_scene_dv();
            s_app_data.next_scene = ret;
            break;
            case CAM_SCENE_PC:
            sys_forbid_auto_standby(true);
            sys_forbid_timed_shutdown(true);
            ret = camera_scene_pc();
            default:
            s_app_data.next_scene = CAM_SCENE_EXIT;
            break;
        }
    }
    result = _app_deinit(&s_app_data);
    sys_set_key_tone_enable(1);
    sys_forbid_auto_standby(false);
    sys_forbid_timed_shutdown(false);
    sys_forbid_screensaver(false);
    if (result == false)
    {
        print_err("app deinit error! \n");
        //return -1;
    }

    sys_pm_clear_clk_domain(&s_clk_domain);
    /* 应用库退出 */
    result = applib_quit();
    if (result == false)
    {
        print_err("applib quit error! \n");
        //return -1;
    }
    _lib_deinit(&s_app_data);
#if (DBG_TIME_OPTIMIZE == 1)
        {
            struct timeval tv;
            do_gettimeofday(&tv);
            print_err(" camera applib quit ---- time now-----  tv_sec = %ld   tv_usec = %ld -----",tv.tv_sec,tv.tv_usec);
        }
#endif /* DBG_TIME_OPTIMIZE */
//DBG_TIME();

    print_info(">>>>>>>>>>>>>>>>>>>>>> app return: %d\n", ret);

    return 0;

    app_init_err: applib_quit();
    applib_init_err:
    _lib_deinit(&s_app_data);
    return -1;

}

