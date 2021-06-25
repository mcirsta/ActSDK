/*******************************************************************************
 *                              NOYA1100
 *                            Module: app
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-07-28          1.0             build this file 
*******************************************************************************/
/*!
 * \file     animation_decoder_scene.c
 * \brief    
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2009/07/28
*******************************************************************************/

#include "include_psp.h"
#include "include_case.h"

/*本应用的头文件*/
#include "anim_off_sty.h"
#include "animation_poweroff.h"
#include "animation_decoder_scene.h"




/* extern function, 函数声明 */
static bool _open_theme( decoder_scene_t *decoder_scene );
static bool _close_theme( decoder_scene_t *decoder_scene );

static bool _dec_open(void);
static void _dec_close(void);

static void _update_fb(void);
static int _proc_timer_paint(void *param);
static void _decode_cb(void);

static bool _init( decoder_scene_t *decoder_scene );
static bool _deinit( decoder_scene_t *decoder_scene );
static void _callback(gui_wm_message_t *pmsg);




/* define variable, 变量定义 */
/* image decoder handle */
static void *s_id_handle = NULL;
static void *s_codec_lib = NULL;
static mmm_img_info_t s_img_info = {0, 0, 0, 0, 0, 0, 0, 0};
static bool s_is_gif = false;
static int s_timer_id = -1;



#if 0
#endif
/******************************************************************************/
/*!                    
* \par  Description:
*       decoder场景界面加载风格, 仅使用场景的横竖大小, 并未加在任何资源
* \param[in]    decoder_scene_t *: 场景指针
* \retval       true:成功; false:失败
*******************************************************************************/
bool decoder_load_style( decoder_scene_t *decoder_scene )
{
    bool result = true;
    
    /* load theme 资源文件 */
    result = _open_theme( decoder_scene );
    if( false == result )
    {
        return false;
    }

    return true;
}


static bool _open_theme( decoder_scene_t *decoder_scene )
{
    bool result = true;
    scene_t *scene = NULL;
    rotation_e angle;
    char filename[PATH_MAX] = {0};
    int fd = -1;

    // 取视图角度
    angle = sty_get_view_rotation();
    // 设置屏幕方向 
    sty_set_screen_rotation(angle);
    
    result = animation_cfg_read_filename( angle, filename );
    if ( false == result )
    {
        result = animation_cfg_read_def_filename( angle, filename );
        if ( true == result )
        {
            snprintf(animation_vars.file_fullpath, sizeof(animation_vars.file_fullpath), \
                "%s/%s", sys_get_app_theme_res_dir(), filename);
        }
    }
    else
    {
        snprintf(animation_vars.file_fullpath, sizeof(animation_vars.file_fullpath), \
                "%s/%s", sys_get_app_theme_res_dir(), filename);
        //当前文件打开失败, 则读取默认文件
        fd = open(animation_vars.file_fullpath, O_RDONLY);
        if ( fd < 0 )
        {
            result = animation_cfg_read_def_filename( angle, filename );
            if ( true == result )
            {
                snprintf(animation_vars.file_fullpath, sizeof(animation_vars.file_fullpath), \
                        "%s/%s", sys_get_app_theme_res_dir(), filename);
            }
        }
        if ( fd >= 0 )
        {
            close(fd);
        }
    }
    
    // 载入应用的 style 场景
    if ( (angle == ROTATION_0) || (angle == ROTATION_180) )
    {
        scene = load_scene( animation_vars.app_style, ANIMATION_POWEROFF_SIDELONG_SCENE );
    }
    else// if ( (angle == ROTATION_90) || (angle == ROTATION_270) )
    {
        scene = load_scene( animation_vars.app_style, ANIMATION_POWEROFF_UPRIGHT_SCENE );
    }

    
    if ( scene == NULL )
    {
        print_err("load_scene fail\n\n");
        return false;   
    }
    sty_set_app_scene( scene );
    decoder_scene->app_scene.scene = scene;
    
    return true;

}

static bool _close_theme( decoder_scene_t *decoder_scene )
{
    //退出场景
    free_scene( decoder_scene->app_scene.scene );
    decoder_scene->app_scene.scene = NULL;

    return true;
}


/******************************************************************************/
/*!                    
* \par  Description:
*       打开decoder库和中间件
* \retval       true:成功; false:失败
*******************************************************************************/
static bool _dec_open(void)
{
    print_dbg();
    if (NULL == s_codec_lib)
    {
        s_codec_lib = dlopen("a_codec.so", RTLD_LAZY);
        if (s_codec_lib == NULL)
        {
            print_err("open codec err!\n");
            return false;
        }
    }

    if (NULL == s_id_handle)
    {
        s_id_handle = mmm_id_open(NULL);
        if (s_id_handle == NULL)
        {
            print_err("open mmm err!\n");
            return false;
        }
    }
    return true;
}

static void _dec_close(void)
{
    print_dbg();
    if (NULL != s_id_handle)
    {
        mmm_id_close(s_id_handle, NULL);
        s_id_handle = NULL;
    }
    
    if (NULL != s_codec_lib)
    {
        dlclose(s_codec_lib);
        s_codec_lib = NULL;
    }
}

/******************************************************************************/
/*!                    
* \par  Description:
*       刷新数据到屏幕, 解码方式的刷屏脱离了gui机制, 需要直接刷屏
*******************************************************************************/
static void _update_fb(void)
{
    int fd = open(DEVICE_FB, O_RDWR, 0);
    ioctl(fd, FBIOSET_UPDATE_FB, 0);
    close(fd);
}

/******************************************************************************/
/*!                    
* \par  Description:
*       显示图片功能放到定时器事件处理函数, 并发送解码下一帧命令
*******************************************************************************/
static int _proc_timer_paint(void *param)
{
    mmm_image_status_t status;
    int x, y, ret;
    
    //更新解码数据到屏幕
    x = (int)((s_img_info.rect_width - s_img_info.img_width) >> 1);
    y = (int)((s_img_info.rect_height - s_img_info.img_height) >> 1);
    gui_dc_draw_bitmap_ext(animation_vars.decoder_scene.app_scene.handle_gui_dc, \
            s_img_info.buf, x, y,
            (int)s_img_info.img_width,  (int)s_img_info.img_height,
            (int)s_img_info.bpp);
    
    _update_fb();
            
    ret = mmm_id_cmd(s_id_handle, MMM_GET_DECODER_STATUS, (unsigned int) &status);
    //如果是gif 并且没有解码结束, 继续发送解码命令. 否则退出应用
    if ((true == s_is_gif) && (0 == status.end_of_file))
    {
        mmm_id_cmd(s_id_handle, MMM_IMG_DECODE, (unsigned int)&s_img_info); 
    }
    else
    {
        exit_msg_loop();  /* 调用退出当前场景函数 */
    }
    
    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*       中间件解码回调函数, 用于解码完成后告知ap, ap使用timer完成画图刷新操作
*       函数里面不能发送解码命令, 否则死锁, 解码命令也放到timer中完成
*       此函数为另一线程, 不能进行画图操作
*******************************************************************************/
static void _decode_cb(void)
{
    mmm_image_status_t status;
    int ret;

    ret = mmm_id_cmd(s_id_handle, MMM_GET_DECODER_STATUS, (unsigned int) &status);

    if ((0 == ret) && (MMM_IMAGE_DECODER_FREE == status.status))
    {
        if ( s_timer_id <= 0 )
        {
            s_timer_id = set_single_shot_timer(1, _proc_timer_paint, 0);
        }
        else
        {
            restart_timer( s_timer_id );
        }
    }
    else
    {
        mmm_id_cmd(s_id_handle, MMM_ABORT_DECODE, 0);
        exit_msg_loop();
    }
}

/******************************************************************************/
/*!                    
* \par  Description:
*       scene callback function
*       分配cb的gui消息. 此AP不处理gui消息
*******************************************************************************/
static void _callback(gui_wm_message_t *pmsg)
{ 
    gui_wm_default_callback(pmsg);
}


/******************************************************************************/
/*!                    
* \par  Description:
*       scene initial function
*       申请资源. 初始化资源
*******************************************************************************/
static bool _init( decoder_scene_t *decoder_scene )
{
    bool result = true;
    scene_t *scene = NULL;

    memset( decoder_scene, 0x00, sizeof(decoder_scene_t) );
    
    result = _dec_open();
    if( false == result )
    {
        return false;
    }
    
    /*** 申请资源, 如有任何失败直接返回退出 ***/
    //载入场景
    result = decoder_load_style( decoder_scene );
    if (false == result)
    {
        print_err("load_style fail\n\n");
        return false;
    }
    scene = decoder_scene->app_scene.scene;
    
    //创建窗口
    print_dbg("playwin xywh:%d,%d,%d,%d", scene->x, scene->y, scene->width, scene->height);
    decoder_scene->app_scene.win_id = gui_wm_create_window(scene->x, scene->y, scene->width, scene->height, \
                                        WM_CREATE_SHOW, _callback, 0);
    if( decoder_scene->app_scene.win_id < 0 )
    {
        print_err("gui_wm_create_window failed\n\n");
        return false;
    }
    
    //获取gui dc句柄
    decoder_scene->app_scene.handle_gui_dc = gui_dc_get( decoder_scene->app_scene.win_id );
    if (decoder_scene->app_scene.handle_gui_dc <= 0)
    {
        print_err("gui_dc_get fail\n\n");
        return false;
    }
    
    /*** 申请资源成功, 开始初始化 ***/
    //设置焦点
    gui_wm_set_focus( decoder_scene->app_scene.win_id );
    
    return result;
}


/* scene exit function */
static bool _deinit( decoder_scene_t *decoder_scene )
{
    bool result = true;

    print_dbg();
    
    if ( -1 != s_timer_id )
    {
        kill_timer( s_timer_id );
        s_timer_id = -1;
    }
    
    //删除窗体
    if ( decoder_scene->app_scene.win_id > 0 )
    {
        gui_wm_delete_window( decoder_scene->app_scene.win_id );
        decoder_scene->app_scene.win_id = -1;
    }
    
    if( _close_theme( decoder_scene ) == false )
    {
        result = false;
    }
    
    _dec_close();
    
    return result;
}


/* scene entry function */
bool animation_decoder_scene( decoder_scene_t *decoder_scene )
{
    bool result = true;
    int tmp = -1;
    msg_apps_t  msg;
    mmm_img_file_info_t file_info;

    result = _init( decoder_scene );
    if ( false == result )
    {
        goto ANIMATION_DECODER_SCENE_ERR_INIT;
    }
        
    //清空屏幕
    gui_dc_clear( decoder_scene->app_scene.handle_gui_dc );
    
    s_img_info.rect_width = (unsigned int)decoder_scene->app_scene.scene->width;
    s_img_info.rect_height = (unsigned int)decoder_scene->app_scene.scene->height;
    
    mmm_id_cmd(s_id_handle, MMM_ABORT_DECODE, 0);
    tmp = mmm_id_cmd( s_id_handle, MMM_SET_FILE, (unsigned int)(animation_vars.file_fullpath) );
    if( tmp != 0 )
    {
        print_err("set file fail:%s", animation_vars.file_fullpath);
        goto ANIMATION_DECODER_SCENE_ERR_INIT;
    }
    
    tmp = mmm_id_cmd(s_id_handle, MMM_SET_CALLBACK, (unsigned int)_decode_cb);
    if( tmp != 0 )
    {
        print_err("set callback fail");
        goto ANIMATION_DECODER_SCENE_ERR_INIT;
    }
    tmp = mmm_id_cmd(s_id_handle, MMM_GET_MEDIA_INFO, (unsigned int)&file_info);
    if( tmp != 0 )
    {
        print_err("get media info fail");
        goto ANIMATION_DECODER_SCENE_ERR_INIT;
    }

    s_is_gif = false;
    if(!strcmp(file_info.file_format, "gif"))
    {
        s_is_gif = true;
    }

    s_img_info.bpp = 2;
    s_img_info.len = (s_img_info.rect_width * s_img_info.rect_height) * s_img_info.bpp;
    if(NULL != s_img_info.buf)
    {
        free(s_img_info.buf);
    }
    s_img_info.buf = (unsigned char *)malloc(s_img_info.len);
    
    //发送开始解码命令, 真正开始解码
    if(mmm_id_cmd(s_id_handle, MMM_IMG_DECODE, (unsigned int)&s_img_info) != 0)
    {
        print_err("decode fail");
    }
    
    register_sys_dispatcher( (sys_msg_proc)proc_sys_msg );

    while( true == get_msg(&msg) )
    {
        dispatch_msg(&msg);
    }

    unregister_sys_dispatcher();
    
    ANIMATION_DECODER_SCENE_ERR_INIT:
    _deinit( decoder_scene );

    if(NULL != s_img_info.buf)
    {
        free(s_img_info.buf);
        s_img_info.buf = NULL;
    }
    
    return true;
}

