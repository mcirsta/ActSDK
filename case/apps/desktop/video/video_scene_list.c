/*******************************************************************************
 *                              NOYA1100
 *                            Module: video
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-05-15          1.0             build this file 
*******************************************************************************/
/*!
 * \file     video_scene_list.c
 * \brief    
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2009/05/15
*******************************************************************************/

#include "include_psp.h"
#include "include_case.h"

/* include file of this application, 本应用的头文件 */
#include "include_video.h"


/* extern function, 函数声明 */
static bool _open_theme( video_scene_list_t *scene_list );
static bool _close_theme( video_scene_list_t *scene_list );
static bool _open_str_res( void );
static bool _close_str_res( void );
static bool _scene_list_init( video_scene_list_t *scene_list );
static bool _scene_list_deinit( video_scene_list_t *scene_list );
static void _scene_list_paint( video_scene_list_t *scene_list );
static bool _scene_list_proc_gui( video_scene_list_t *scene_list, gui_wm_message_t *pmsg );
static void _scene_list_cb(gui_wm_message_t *pmsg);


static const clk_domain_t s_clk_domain = {CLK_LOAD_MIN, CLK_LOAD_HIGH_0};
static bool s_rotate_process_enter = true;

#if 0
#endif
bool video_scene_list_load_style( video_scene_list_t *scene_list )
{
    scene_t *scene = NULL;
    
    /* load theme 资源文件 */
    if(_open_theme( scene_list ) == false)
    {
        return false;
    }

    /* load 字符串资源文件 */
    if(_open_str_res() == false)
    {
        return false;
    }
    
    //load pic and pic region from scene or resource group
    scene_list->bg_bitmap_info = get_user_bg_pic(BG_VIDEO);
    if ( NULL == scene_list->bg_bitmap_info )
    {
        scene = scene_list->app_scene.scene;
        scene_list->pic_bg = ( picture_resource_t * )get_scene_child( scene, PIC_BG );
        if ( NULL == scene_list->pic_bg )
        {
            return false;
        }
        
        scene_list->bg_bitmap_info = sty_create_bitmap( (int)scene_list->pic_bg->pic_id );
        if ( NULL == scene_list->bg_bitmap_info )
        {
            return false;
        }
    }
    
    return true;
}


bool video_scene_list_rotate( video_scene_list_t *scene_list )
{
    fusion_effect_t fusion_effect = {0};
    /*list场景已经deinit，则记录横竖屏切换标识并返回*/
    if (scene_list->app_scene.win_id < 0)
    {
        /*横竖屏有切换，但是没有load 资源和更新dirlist，标记为false。
        用于从播放场景返回后横竖屏发生变化时刷新dirlist*/
        s_rotate_process_enter = false;
        return false;
    }
    
    fusion_get_config(FUSION_TYPE_ROTATE, FUSION_FORWARD, &fusion_effect);
    fusion_set_effect(&fusion_effect);
    /* 设置此界面之前的数据, 作为特效第一帧 */
    fusion_set_first_frame( NULL );
    
    //unload所有显示资源
    _close_theme( scene_list );
    _close_str_res();
    sty_delete_bitmap( scene_list->bg_bitmap_info );
    scene_list->bg_bitmap_info = NULL;
    //重新load所有显示资源
    video_scene_list_load_style( scene_list );

    gui_wm_set_size( scene_list->app_scene.win_id, \
        scene_list->app_scene.scene->width, scene_list->app_scene.scene->height );

    _scene_list_paint( scene_list );

    if ( video_vars.menuview != NULL )
    {
        video_list_widget_menu_load_style( video_vars.menuview );
        video_list_widget_menu_paint( video_vars.menuview );
    }

    if ( video_vars.dirlist != NULL )
    {
        dirlist_load_style( video_vars.dirlist );
        dirlist_paint( video_vars.dirlist );
    }

    if ( video_vars.headbar != NULL )
    {
        headbar_load_style( video_vars.headbar );
        headbar_paint( video_vars.headbar );
    }

    if( video_vars.adjust != NULL )
    {
        adjust_load_style( video_vars.adjust );
        adjust_paint( video_vars.adjust );
    }
    
    if( video_vars.loadingbox != NULL )
    {
        loadingbox_load_style( video_vars.loadingbox );
        loadingbox_paint( video_vars.loadingbox );
    }
    
    if( video_vars.softkeyboard != NULL )
    {
        softkeyboard_load_style( video_vars.softkeyboard );
        softkeyboard_paint( video_vars.softkeyboard );
    }
    
    if( video_vars.dialog != NULL )
    {
        dialog_load_style( video_vars.dialog );
        dialog_paint( video_vars.dialog );
    }
    dialog_modal_load_style();
    dialog_modal_paint();

    if( video_vars.vbar != NULL )
    {
        volumebar_load_style( video_vars.vbar );
        volumebar_paint( video_vars.vbar );
    }

    if( video_vars.msgbox != NULL )
    {
        msgbox_load_style( video_vars.msgbox );
        msgbox_paint( video_vars.msgbox );
    }

    /* 设置此界面的数据, 作为特效最后一帧 */
    fusion_set_last_frame(NULL);
    /* 显示特效 */
    fusion_display( NULL );
    
    s_rotate_process_enter = true;
    return true;
}

//shaofeng 作用??
static void _list_callback(menuview_t *menuview, int signal)
{
}



static bool _open_theme( video_scene_list_t *scene_list )
{
    scene_t *scene = NULL;
    rotation_e angle;
    char file[PATH_MAX];

    // 取视图角度
    angle = sty_get_view_rotation();
    // 设置屏幕方向 
    sty_set_screen_rotation(angle);

    // 载入commonui的 style 场景 
    if ( (angle == ROTATION_0) || (angle == ROTATION_180) )
    {
        video_vars.commonui_scene = load_scene( video_vars.commonui_style, COMMONUI_SIDELONG_SCENE );
    }
    else //if ( (angle == ROTATION_90) || (angle == ROTATION_270) )
    {
        video_vars.commonui_scene = load_scene( video_vars.commonui_style, COMMONUI_UPRIGHT_SCENE );
    }
    
    if ( NULL == video_vars.commonui_scene )
    {
        print_err("load commonui scene err");
        return false;
    }
    
    // 设置 commonui 的 style 场景
    sty_set_common_scene( video_vars.commonui_scene );

    // 获取commonui 的图片资源文件名 
    sprintf(file, "%s/%s.res", sys_get_common_theme_res_dir(), "commonui");
    // 打开 commonui 图片资源文件 
    if( false == sty_open_common_pic_res(file) )
    {
        print_err("open %s failed!", file);
        return false;
    }
    
    
    // 载入应用的 style 场景
    if ( (angle == ROTATION_0) || (angle == ROTATION_180) )
    {
        scene = load_scene( video_vars.app_style, VIDEO_LIST_SIDELONG_SCENE );
    }
    else //if ( (angle == ROTATION_90) || (angle == ROTATION_270) )
    {
        scene = load_scene( video_vars.app_style, VIDEO_LIST_UPRIGHT_SCENE );
    }
    
    if ( scene == NULL )
    {
        print_err("load_scene fail\n\n");
        return false;   
    }
    sty_set_app_scene( scene );
    scene_list->app_scene.scene = scene;
    
    // 获取图片资源文件名
    sprintf(file, "%s/%s.res", sys_get_app_theme_res_dir(), "video");
    // 打开应用的图片资源文件
    if( false == sty_open_app_pic_res(file) )
    {
        print_err("open %s failed!", file);
        return false;
    }
    
    return true;
}

static bool _close_theme( video_scene_list_t *scene_list )
{

    //退出场景
    free_scene( video_vars.commonui_scene );
    video_vars.commonui_scene = NULL;
    free_scene( scene_list->app_scene.scene );
    scene_list->app_scene.scene = NULL;

    if( sty_close_common_pic_res() == false )
    {
        print_err("close common picture resource error!");
        return false;
    }

    if( sty_close_app_pic_res() == false )
    {
        print_err("close app picture resource error!");
        return false;
    }
    
    return true;
}

static bool _open_str_res( void )
{
    char file[PATH_MAX];  

    // 获取commonui 字符串资源文件名 
    sprintf(file, "%s/%s_%s.str", sys_get_common_string_res_dir(), "commonui", sys_get_cur_language_codes());
    // 打开 commonui 字符串资源文件 
    if( false == sty_open_common_str_res(file) )
    {
        print_err("open %s failed!", file);
        return false;
    }

    // 获取应用字符串资源文件名
    sprintf(file, "%s/%s_%s.str", sys_get_app_string_res_dir(), "video", sys_get_cur_language_codes());
    // 打开应用的字符串资源文件 
    if( false == sty_open_app_str_res(file) )
    {
        print_err("open %s failed!", file);
        return false;
    }

    // 设置语言编码
    sty_set_text_encoding(GUI_UTF8_ENC);
    
    return true;
}

static bool _close_str_res( void )
{
    if( false == sty_close_common_str_res() )
    {
        print_err("close common string resource error!");
        return false;
    }

    if( false == sty_close_app_str_res() )
    {
        print_err("close app string resource error!");
        return false;
    } 

    return true;
}


/*
* scene initial function
* 申请资源. 初始化资源
*/
static bool _scene_list_init( video_scene_list_t *scene_list )
{
    bool result = true;
    scene_t *scene = NULL;
    hotplugin_dev_type_e main_disk; 

    print_dbg("enter scene list");
    memset( scene_list, 0x00, sizeof(video_scene_list_t) );
    //载入场景
    result = video_scene_list_load_style( scene_list );
    if (false == result)
    {
        print_err("load_style fail\n\n");
        return false;
    }
    //主盘检查
    main_disk = sys_get_main_disk();
    if(HOTPLUGIN_DEV_TYPE_NONE == main_disk)
    {
        //提示无磁盘
        video_dialog_modal( VIDEO_DIALOG_NODISK);
        return false;
    }
    
    scene = scene_list->app_scene.scene;
    
    //创建窗口
    print_dbg("playwin xywh:%d,%d,%d,%d", scene->x, scene->y, scene->width, scene->height);
    scene_list->app_scene.win_id = gui_wm_create_window(scene->x, scene->y, scene->width, scene->height, \
                                        WM_CREATE_SHOW, _scene_list_cb, 0);
    if( scene_list->app_scene.win_id <= 0 )
    {
        print_err("gui_wm_create_window failed\n\n");
        return false;
    }
    
    //获取gui dc句柄
    scene_list->app_scene.handle_gui_dc = gui_dc_get( (unsigned short)scene_list->app_scene.win_id );
    if (scene_list->app_scene.handle_gui_dc <= 0)
    {
        print_err("gui_dc_get fail\n\n");
        return false;
    }
    
    /*** 申请资源成功, 开始初始化 ***/
    //设置焦点
    gui_wm_set_focus( scene_list->app_scene.win_id );
    
    video_headbar_create();
    headbar_paint( video_vars.headbar );
    
    video_msgbox_create();
    
    //shaofeng shaofeng注意位置, 对特效和退回到list的影响
    _scene_list_paint( scene_list );
    //第一次进入dirlist, 创建dirlist; 否则dirlist的入口注册给了menu
    if ( (VIDEO_START_MODE_DV_DIRLIST == video_vars.start_mode) && (NULL==video_vars.dirlist) )
    {
        //进入dv dirlist
        video_list_dirlist_create( video_vars.menuview, FUSION_FORWARD );
    }
    else
    {
        //进入mainmenu
        video_list_widget_menu_create( (menuview_cb_t)_list_callback );
    }
    
    /*将界面先立即刷新到屏幕, 之后再初始化plist */
    gui_screen_update();
    
    /* 为优化显示速度, 将playlist初始化放在scene_list和各启动处 */
    result = video_playlist_init();

    if(false == s_rotate_process_enter)
    {
        /*横竖屏有切换，但是没有更新dirlist，此时需更新，主要用于从playing场景返回*/
        if ( video_vars.dirlist != NULL )
        {
            dirlist_load_style( video_vars.dirlist );
            dirlist_paint( video_vars.dirlist );
        }
        s_rotate_process_enter = true;
    }
    
    return result;
}


/* scene exit function */
static bool _scene_list_deinit( video_scene_list_t *scene_list )
{
    bool result = true;

    print_dbg();
    
    dirlist_hide( video_vars.dirlist );
    //deinit commonui    
    video_headbar_delete();
    video_msgbox_delete();
    video_dialog_delete();
    video_vbar_delete();
    
    //shaofeng 保存数据
    video_vars.menu = menuview_get_menu( video_vars.menuview );
    video_list_widget_menu_delete( video_vars.menuview );
    video_vars.menuview = NULL;
    
    //删除窗体
    if ( scene_list->app_scene.win_id > 0 )
    {
        gui_wm_delete_window( scene_list->app_scene.win_id );
        scene_list->app_scene.win_id = -1;
    }
    
    result = _close_theme( scene_list );
    result = _close_str_res();
    
    sty_delete_bitmap( scene_list->bg_bitmap_info );
    scene_list->bg_bitmap_info = NULL;
    
    return result;
}


/* scene paint function */
static void _scene_list_paint( video_scene_list_t *scene_list )
{
    sty_draw_bitmap( scene_list->app_scene.handle_gui_dc, scene_list->bg_bitmap_info, 0, 0 );
}

/* scene msg process */
static bool _scene_list_proc_gui( video_scene_list_t *scene_list, gui_wm_message_t *pmsg )
{
    key_event_t *kmsg;
    bool result = true;
    
    switch (pmsg->msgid) 
    {
    case WM_MSG_KEY:
        kmsg = (key_event_t *) (pmsg->data.p);

        switch (kmsg->val | kmsg->type)
        {
        case KEY_VOL | KEY_TYPE_DOWN:
        case KEY_VSUB | KEY_TYPE_DOWN:
        case KEY_VADD | KEY_TYPE_DOWN:
            if( NULL == video_vars.vbar )
            {
                video_vbar_create();
                return false;
            }
            break;
        default:
            break;
        }
        break;
         
    default:
        gui_wm_default_callback(pmsg);
        break;
    }

    return result;
}


/******************************************************************************/
/*!                    
* \par  Description:
*       场景回调函数, 也即窗口回调函数. 一个场景只有一个全屏窗口
*       分配窗口cb的gui消息: 获取top层的图层, 将消息发送给top.
*       menuview和scene在最下层
* \param[in]    
* \retval       
*   注意: 各控件间 及与playing scene的关系
*******************************************************************************/
static void _scene_list_cb(gui_wm_message_t *pmsg)
{ 
    int top;
    bool ret;
    
    /* 收到down按键后才继续处理按键, 否则直接退出 */
    if ( WM_MSG_KEY == pmsg->msgid )
    {
        if ( KEY_TYPE_DOWN == ((key_event_t *)(pmsg->data.p))->type )
        {
            video_vars.scene_list.is_key_valid = 1;
        }
    }
    if ( 1 != video_vars.scene_list.is_key_valid )
    {
        return;
    }
    
    ret = _scene_list_proc_gui( &video_vars.scene_list, pmsg );
    if( false == ret )
    {
        return;
    }

    top = canvas_get_top();
    print_dbg("top canvas:%d", top);
    
    if ( (NULL != video_vars.msgbox) && (top == video_vars.msgbox->canvas_id) )
    {
        msgbox_proc_gui( video_vars.msgbox, pmsg );
    }
    else if ( (NULL != video_vars.dialog) && (top == video_vars.dialog->canvas_id) )
    {
        dialog_proc_gui( video_vars.dialog, pmsg );
    }
    else if ( (NULL != video_vars.vbar) && (top == video_vars.vbar->canvas_id) )
    {
        volumebar_proc_gui( video_vars.vbar, pmsg );
    }
    else if ( (NULL != video_vars.softkeyboard) && (top == video_vars.softkeyboard->canvas_id) )
    {
        softkeyboard_proc_gui( video_vars.softkeyboard, pmsg );
    }
    else if ( (NULL != video_vars.dirlist) && (top == video_vars.dirlist->canvas_id) )
    {
        dirlist_proc_gui( video_vars.dirlist, pmsg );
    }
    else
    {
        menuview_proc_gui( video_vars.menuview, pmsg );
    }
}



/******************************************************************************/
/*!                    
* \par  Description:
*       场景入口函数. scene entry function
* \param[in]    
* \retval       
*******************************************************************************/
scene_type_e video_scene_list( video_scene_list_t *scene_list )
{
    scene_type_e next_scene = SCENE_ERR;
    bool result;
    msg_apps_t  msg;

    print_dbg();
    sys_pm_set_clk_domain(&s_clk_domain);
    /* 场景初始化 */
    result = _scene_list_init( scene_list );
    if(false == result)
    {
        print_err("scene init error!\n");
        goto VIDEO_SCENE_LIST_ERR_INIT;
    }
    //清除进入scene前的所有消息
    clear_sys_msg_queue( -1 );
    clear_key_msg_queue( -1, -1 );
    
    register_sys_dispatcher( (sys_msg_proc)video_proc_sys_msg );

    while(true == get_msg(&msg))
    {
        dispatch_msg(&msg);
    }

    unregister_sys_dispatcher();

    next_scene = scene_list->app_scene.scene_type;
    /* 场景退出 */
    VIDEO_SCENE_LIST_ERR_INIT:
    result = _scene_list_deinit( scene_list );
    if(false == result)
    {
        print_err("scene deinit error!\n");
        next_scene = SCENE_EXIT;
    }
    clear_sys_msg_queue( -1 );
    clear_key_msg_queue( -1, -1 );
    
    sys_pm_clear_clk_domain(&s_clk_domain);
    
    return next_scene;
}


