/*******************************************************************************
 *                              NOYA1100
 *                            Module: video
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-09-02          1.0             build this file 
*******************************************************************************/
/*!
 * \file     video_scene_swf.c
 * \brief    
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2009/09/02
*******************************************************************************/

#include "include_psp.h"
#include "include_case.h"

/* include file of this application, 本应用的头文件 */
#include "include_video.h"


/* extern function, 函数声明 */
static bool _get_battery_voltage( charge_voltage_t *vol );

static void _dialog_cb( dialog_t *dialog, widget_sig_dialog_e signal );
static void _dialog_cb_setplay_false( dialog_t *dialog, widget_sig_dialog_e signal );
static int  _scene_swf_proc_timer( void *param );
static bool _open_theme( video_scene_swf_t *scene_swf );
static bool _close_theme( video_scene_swf_t *scene_swf );
static bool _open_str_res( void );
static bool _close_str_res( void );

static bool _scene_swf_init( video_scene_swf_t *scene_swf );
static bool _scene_swf_deinit( video_scene_swf_t *scene_swf );
static int  _scene_swf_proc_key( video_scene_swf_t *scene_swf, key_event_t *key_event );
static int  _scene_swf_proc_gui( video_scene_swf_t *scene_swf, gui_wm_message_t *pmsg );
static void _scene_swf_cb( gui_wm_message_t *pmsg );


static const clk_domain_t s_clk_domain = {CLK_LOAD_MIN, CLK_LOAD_MAX};

/* define variable, 变量定义 */
/* 用于:创建普通dialog时需要等待其显示3秒后再继续执行timer中的动作, 此时应禁止进入timer中断 */
static int s_enter_timer = 1;
/*此变量用于记录不能播放的文件index，用于在dialog的回调函数中从nplist中删除对应文件,并设置下一个文件*/
static int s_index_for_setplay_false = 0;
/*此变量用于记录当前播放时文件切换方式，便于获取下一个要播放的文件的index*/
static video_play_switch_mode_e s_playing_switch_mode = VIDEO_AUTO_PLAY;


#if 0
#endif
/******************************************************************************/
/*!                    
* \par  Description:
*       swf界面甩歌功能, 切换到下一曲, 相当于next键功能
* \param[in]    
*   注意: 
*       非swf界面, 非play状态, 直接返回
*******************************************************************************/
void video_scene_swf_shaked( video_scene_swf_t *scene_swf )
{
    bool result = true;
    int index_cur = -1;
    
    if ( NULL == scene_swf )
    {
        return;
    }
    if ( scene_swf->app_scene.win_id <= 0 )
    {
        return;
    }


    index_cur = video_get_next_play_index( LOOP_SWITCH_FORCELOOP );
    result = video_nplist_setplay_swf_by_index( index_cur );
    if ( false == result )
    {
        swf_control( V_CMD_STOP, NULL );

        s_enter_timer = 0;
        /*将不能播放的文件的inde记录，便于dialog的callback处理*/
        s_index_for_setplay_false = index_cur;
        s_playing_switch_mode = VIDEO_FORCE_PLAY_NEXT;

        
        video_dialog_create( VIDEO_DIALOG_OPEN_FAIL, _dialog_cb_setplay_false);        
    }
}

static bool _get_battery_voltage( charge_voltage_t *vol )
{
#if VIDEO_CFG_DETECT_VOL_THRESHOLD_ENABLE > 0
    bool result = true;
    /* 用于:获取电池电量 */
    hw_status_t     hw_status;
    struct timespec timeout;
    msg_apps_t      msg;
    msg_reply_t     reply;
    
    timeout.tv_sec = 1;
    timeout.tv_nsec = 0;
    
    msg.type = MSG_GET_HW_STATUS_SYNC;
    result = send_sync_msg(APP_NAME_MSG_SERV, &msg, &reply, &timeout);
    if (false == result)
    {
        print_warning("send_sync_msg failed");
        return false;
    }
    if ( MSG_REPLY_SUCCESS != reply.type )
    {
        return false;
    }
    memcpy(&hw_status, reply.content, sizeof(hw_status_t));
    if ( (PLUG_IN == hw_status.usb) || (true == hw_status.adapter) )
    {
        *vol = LI420;
    }
    else
    {
        *vol = hw_status.voltage;
    }
    
    return true;
#else
    *vol = LI420;
    return false;
#endif
}

/******************************************************************************/
/*!                    
* \par  Description:
*       swf界面弹出的dialog的回调函数
* \param[in]    
*   注意: 
*       此类dialog弹出后, 不再需要进入timer查询video状态. 在创建dialog前, 需要设置s_enter_timer = 0.
*       此类dialog消失后, 需要进入timer. 设置s_enter_timer = 1.
*******************************************************************************/
static void _dialog_cb( dialog_t *dialog, widget_sig_dialog_e signal )
{
    switch ( signal ) 
    {
    case WIDGET_SIG_DIALOG_OK:
        video_dialog_delete();
        break;
        
    case WIDGET_SIG_DIALOG_CANCAL:
        video_dialog_delete();
        break;
    default:
        break;
    }
    
    s_enter_timer = 1;
}

static void _dialog_cb_setplay_false( dialog_t *dialog, widget_sig_dialog_e signal )
{  
    bool result;
    int index_next = s_index_for_setplay_false;
    
    switch ( signal ) 
    {
    case WIDGET_SIG_DIALOG_OK:
        
    case WIDGET_SIG_DIALOG_CANCAL:
        video_dialog_delete();

        /*从np_list中删除并更新loop参数*/
        result = video_nplist_del_item( s_index_for_setplay_false, s_playing_switch_mode, &index_next );
        //删除记录失败，loop不会有更新，返回的next_index不是有效值，需要获取下一个要播放的index
        if( false == result )
        {
            if( VIDEO_AUTO_PLAY == s_playing_switch_mode )
            {
                index_next = video_get_next_play_index( LOOP_SWITCH_NORMAL );
            }
            else if( VIDEO_FORCE_PLAY_NEXT == s_playing_switch_mode )
            {
                index_next = video_get_next_play_index( LOOP_SWITCH_FORCELOOP );
            }
            else
            {
                index_next = video_get_prev_play_index( LOOP_SWITCH_FORCELOOP );                
            }
        }
        
        //只有自动播放时到最后一首index_next才会返回-1
        if( -1 == index_next )
        {
            //退出前将 index_cur==0文件 置为np信息
            video_nplist_set_to_first();
            
            //非loop模式播放结束, 退出.. (如:sequence模式播放到最后一曲)
            video_vars.scene_swf.app_scene.scene_type = SCENE_LIST;
            exit_msg_loop();
            return;        
        }
        
        result = video_nplist_setplay_swf_by_index( index_next );            
        if ( false == result )
        {
            swf_control( V_CMD_STOP, NULL );

            s_enter_timer = 0;
            /*将不能播放的文件的inde记录，便于dialog的callback处理*/
            s_index_for_setplay_false = index_next;
            video_dialog_create( VIDEO_DIALOG_OPEN_FAIL, _dialog_cb_setplay_false);        
        }
        else
        {
            s_enter_timer = 1;   
        }
            
        break;
        
    default:
        break;
    }
}


#if 0
/******************************************************************************/
/*!                    
* \par  Description:
*       playing界面删除当前播放文件，并播放下一个文件，播放失败则提示打开文件失败
* \param[in]    
*******************************************************************************/
void video_scene_playing_delete_file( void )
{
    bool result = true;
    int file_total_num;
    int index_next = 0;
    
      
    //删除np文件, 播放下一曲
    result = video_delete_npfile( video_vars.plist.np_fullpath , &index_next );
    if ( false == result )
    {
        //shaofeng dialog
    }
    
    file_total_num = video_plist_get_item_num( video_vars.nplist );      
    if( file_total_num == 0 )//只有一个文件，删除之后直接返回
    {
        return;
    }
    
    //下一个要播放的文件的index为-1，表示当前删除的是播放列表的最后一首
    if( index_next == -1 )
    {
        if( VIDEO_PLAYMOE_SHUFFLE == video_vars.play_param.playmode )
        {
            //随机模式到最后一首，重新初始化shuffle到随机一首
            index_next = rand() % file_total_num;
        }
        else
        {
            //顺序模式到最后一首，重新初始化loop到第一首
            index_next = 0;
        }
        video_init_play_mode( video_vars.play_param.playmode, index_next, file_total_num );             
    }

    result = video_nplist_setplay_swf_by_index( index_next );
    if ( false == result )
    {
        video_control( V_CMD_STOP, NULL );

        g_enter_timer = 0;
        /*将不能播放的文件的inde记录，便于dialog的callback处理*/
        s_index_for_setplay_false = index_next;
        video_dialog_create( VIDEO_DIALOG_OPEN_FAIL, _dialog_cb_setplay_false);
    }
}

static void _dialog_cb_delete_file( dialog_t *dialog, widget_sig_dialog_e signal )
{    
    switch ( signal ) 
    {
    case WIDGET_SIG_DIALOG_OK:
        video_dialog_delete();
        video_scene_playing_delete_file();
        break;

    case WIDGET_SIG_DIALOG_CANCAL:
        video_dialog_delete();
        break;
    default:
        break;
    }
}
#endif

/******************************************************************************/
/*!                    
* \par  Description:
*       swf界面的timer中断处理函数
* \param[in]    void *: video_scene_swf_t *, 场景指针
*   实现的功能: 
*       获取当前播放时间和播放状态, 更新显示
*       根据当前播放状态为stop时, 播放上一个\下一个文件
*       累计swf界面的ui显示时间, 自动隐藏ui
*   注意: 
*       当文件正常播放到文件尾为stop状态, 当文件快进快退到文件头尾时, 也为stop状态
*       需要增加变量记录不同的stop状态操作, 实现对应的功能
*******************************************************************************/
static int _scene_swf_proc_timer( void *param )
{
    static int cnt_get_bat_vol = 0;
    /* 获取当前电池电压, 是否低于播放video的门限 */
    charge_voltage_t battery_vol;
    volatile bool result = true;
    int index_cur;
    video_scene_swf_t *scene_swf = (video_scene_swf_t *)param;


    rotation_e angle;

    
//    print_dbg("s_enter_timer:%d", s_enter_timer);
    //如果不允许进入timer, 则直接退出. 用于dialog提示时, 在dialog的cb中设置为1
    if ( 0 == s_enter_timer )
    {
        return 0;
    }
    
    cnt_get_bat_vol++;
    if ( cnt_get_bat_vol >= VIDEO_DETECT_BAT_VOL_TIME )
    {
        cnt_get_bat_vol = 0;
        result = _get_battery_voltage( &battery_vol );
        if ( true == result )
        {
            if ( battery_vol <= VOL_THRESHOLD_PLAY_VIDEO )
            {
                s_enter_timer = 0;
                video_dialog_modal( VIDEO_DIALOG_LOW_BAT );
                 
                scene_swf->app_scene.scene_type = SCENE_LIST;
                exit_msg_loop();
                return 0;
            }
        }
    }

    //获得解码状态，以便出错时提示
    swf_control(V_CMD_GET_PLAYER_STATUS, (void *)&scene_swf->player_status);
    if( MMM_PLAYER_STOPPED == scene_swf->player_status )
    {
        s_playing_switch_mode = VIDEO_AUTO_PLAY;
        index_cur = video_get_next_play_index( LOOP_SWITCH_NORMAL );
       
        if ( -1 == index_cur )
        {
            //退出前将 index_cur==0文件 置为np信息
            video_nplist_set_to_first();
            
            //非loop模式播放结束, 退出.. (如:sequence模式播放到最后一曲)
            scene_swf->app_scene.scene_type = SCENE_LIST;
            exit_msg_loop();
            return 1;
        }
        
        result = video_nplist_setplay_swf_by_index( index_cur );

        if ( false == result )
        {
            video_control( V_CMD_STOP, NULL );
            
            s_enter_timer = 0;
            s_playing_switch_mode = VIDEO_AUTO_PLAY;

            /*将不能播放的文件的inde记录，便于dialog的callback处理*/
            s_index_for_setplay_false = index_cur;
            /*由于中间件会根据播放文件属性设置屏幕方向，因此此处提示需要重新设置屏幕方向，否则花屏*/
            sty_set_screen_rotation( ROTATION_0 );

            video_dialog_create( VIDEO_DIALOG_OPEN_FAIL, _dialog_cb_setplay_false );
        }

    }
        
    if(MMM_PLAYER_ERROR == scene_swf->player_status)
    {
        swf_control( V_CMD_STOP, NULL );//处于err状态, 需要先stop
        s_enter_timer = 0;
        
#if VIDEO_SWF_PLAYMODE == 1  
        /*由于中间件会根据播放文件属性设置屏幕方向，因此此处提示需要重新设置屏幕方向，否则花屏*/
        sty_set_screen_rotation( ROTATION_0 );
/*
        //获取视图角度
        angle = sty_get_view_rotation();
        sty_set_screen_rotation( angle );
*/
        s_playing_switch_mode = VIDEO_AUTO_PLAY;
        s_index_for_setplay_false = video_vars.plist.np_index; 
        /*出错则播放下一个文件，并将当前文件从plist中删除*/
        video_dialog_create( VIDEO_DIALOG_DECODE_ERR, _dialog_cb_setplay_false ); 
        /*从最近播放而没有播放完的文件列表中删除*/
        video_recent_list_delete_item( video_vars.plist.np_index );

        
#else 
        /*由于中间件会根据播放文件属性设置屏幕方向，因此此处提示需要重新设置屏幕方向，否则花屏*/
        sty_set_screen_rotation( ROTATION_0 );
        /*播放失败退出场景*/  
        video_dialog_modal( VIDEO_DIALOG_DECODE_ERR );
        /*从最近播放而没有播放完的文件列表中删除*/
        video_recent_list_delete_item( video_vars.plist.np_index );
        video_nplist_del_item( video_vars.plist.np_index , VIDEO_AUTO_PLAY, NULL );
       
        scene_swf->app_scene.scene_type = SCENE_LIST;
        exit_msg_loop();
#endif      
    }

    return 1;
}


/******************************************************************************/
/*!                    
* \par  Description:
*       swf界面加载风格
* \param[in]    video_scene_swf_t *: 场景指针
* \retval       true:成功; false:失败
*   实现的功能: 
*       加载commonui图片字符串资源
*       加载本场景的图片字符串资源
*       将本场景的各元素从资源中依次获取出来, 用于后续显示
*******************************************************************************/
bool video_scene_swf_load_style( video_scene_swf_t *scene_swf )
{
    scene_t* scene;
    
    /* load theme 资源文件 */
    if(_open_theme( scene_swf ) == false)
    {
        return false;
    }

    /* load 字符串资源文件 */
    if(_open_str_res() == false)
    {
        return false;
    }
    
    scene = scene_swf->app_scene.scene;
    
    return true;
}


static bool _open_theme( video_scene_swf_t *scene_swf )
{
    scene_t *scene = NULL;
    rotation_e angle;
    char file[PATH_MAX];

    // 取视图角度
    angle = sty_get_view_rotation();
    // 设置屏幕方向 
    sty_set_screen_rotation(angle);

    video_vars.commonui_scene = load_scene( video_vars.commonui_style, COMMONUI_SIDELONG_SCENE );
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
    scene = load_scene( video_vars.app_style, VIDEO_PLAYING_SCENE );
    if ( scene == NULL )
    {
        print_err("load_scene fail\n\n");
        return false;   
    }
    sty_set_app_scene( scene );
    scene_swf->app_scene.scene = scene;
    
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

static bool _close_theme( video_scene_swf_t *scene_swf )
{

    //退出场景
    free_scene( video_vars.commonui_scene );
    video_vars.commonui_scene = NULL;
    free_scene( scene_swf->app_scene.scene );
    scene_swf->app_scene.scene = NULL;

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



/******************************************************************************/
/*!                    
* \par  Description:
*       scene initial function. 场景初始化. 
* \param[in]    video_scene_swf_t *: 场景指针
* \retval       true:成功; false:失败
*   实现的功能: 
*       申请资源. 初始化资源. 主要是window, gui dc, blending资源
*       根据np_index和tag启动播放
*******************************************************************************/
static bool _scene_swf_init( video_scene_swf_t *scene_swf )
{
    bool result = true;
    scene_t *scene = NULL;
    charge_voltage_t battery_vol;
    unsigned int s_cur_sdram;

    print_dbg("enter scene swf");
    
    //变量初始化
    memset( scene_swf, 0x00, sizeof(video_scene_swf_t) );
    s_enter_timer = 1;


    //载入场景
    result = video_scene_swf_load_style( scene_swf );
    if (false == result)
    {
        print_err("load_style fail\n\n");
        return false;
    }
    scene = scene_swf->app_scene.scene;
    

    //进入scene_swf时才加载swf中间件. swf与mmm互斥
    video_mmm_deinit();
    //进入dirlist会解码缩略图, 需要mmm
    result = video_mmm_swf_init();
    if ( false == result )
    {
        print_err("video_mmm_init err");
        return false;
    }
       
    //创建窗口
    print_dbg("playwin xywh:%d,%d,%d,%d", scene->x, scene->y, scene->width, scene->height);
    scene_swf->app_scene.win_id = gui_wm_create_window(scene->x, scene->y, scene->width, scene->height, \
                                        WM_CREATE_SHOW, _scene_swf_cb, 0);
    if( scene_swf->app_scene.win_id <= 0 )
    {
        print_err("gui_wm_create_window failed\n\n");
        return false;
    }
    
    //获取gui dc句柄
    scene_swf->app_scene.handle_gui_dc = gui_dc_get( (unsigned short)scene_swf->app_scene.win_id );
    if (scene_swf->app_scene.handle_gui_dc <= 0)
    {
        print_err("gui_dc_get fail\n\n");
        return false;
    }
    
    //设置焦点
    gui_wm_set_focus( scene_swf->app_scene.win_id );
    
    //一定在loadstyle和创建窗口之后, 设置屏幕方向. 此场景的任何绘图操作一定在这之后
    sty_set_screen_rotation( ROTATION_0 );
    //设置横竖屏后, 清空gui的buffer, 否则之前的数据有残留
    gui_dc_clear( scene_swf->app_scene.handle_gui_dc );

    //注意:所有提示都应该在设置屏幕方向之后提示，以免进入播放前后屏幕方向有变化，
    //导致提示位置不正确或背景显示乱码

    //获取内存大小
    s_cur_sdram = (unsigned int)get_sdram_cap();
    print_dbg("SDRAM size:%d\n",s_cur_sdram);
    //只有同时满足支持swf并且内存大于8M才能进入swf场景
    if ((0 == video_vars.swf_support) || (s_cur_sdram <= 8))
    {
        video_dialog_modal( VIDEO_DIALOG_NOT_SUPPORT );
        return false;
    }

    
    //进入播放前, 检测电量
    result = _get_battery_voltage( &battery_vol );
    if ( true == result )
    {
        if ( battery_vol <= VOL_THRESHOLD_PLAY_VIDEO )
        {
            //没有进入播放界面, 需要显示时, 一定先设置NON_YUV. 否则花屏
            video_dialog_modal( VIDEO_DIALOG_LOW_BAT );
            return false;
        }
    }

    result = video_nplist_setplay_swf_by_index( video_vars.plist.np_index );
    //点击的文件打开失败, 则清空np. 不管从np dirlist taglist进入swf
    if ( false == result )
    {
        swf_control( V_CMD_STOP, NULL );
        //没有进入播放界面, 需要显示时, 一定先设置NON_YUV. 否则花屏
        video_dialog_modal( VIDEO_DIALOG_OPEN_FAIL );
        //打开失败的文件, 同时清除其taglist, 用于删除tag文件后, 再从taglist进入播放的情况
        video_nplist_clear_npinfo();
        
        return false;
    }
    video_vars.flag_has_np_file = 1;
    video_set_play_mode( video_vars.play_param.playmode );
    
    //全部初始化成功, 最后启动timer, 定时get video播放状态 信息
    scene_swf->timer_id = set_timer(VIDEO_PLAYING_TIMER_INTERVAL, _scene_swf_proc_timer, (void *)scene_swf);
    if (scene_swf->timer_id < 0)
    {
        print_err("set_timer failed");
        return false;
    }
    
    return result;
}


/* scene exit function */
static bool _scene_swf_deinit( video_scene_swf_t *scene_swf )
{
    bool result = true;
    rotation_e angle;

    print_dbg("exit scene swf\n");

    /*如果正在播放列表没有文件，不显示正在播放列表*/
    if( 0 == video_plist_get_item_num( video_vars.nplist ) )
    {
        video_vars.flag_has_np_file = false;
        print_dbg("no file in nplist\n");
    }

    
    //在这里统一stop
    swf_control( V_CMD_STOP, NULL );
    
    //获取视图角度
    angle = sty_get_view_rotation();
    sty_set_screen_rotation( angle );
    //设置横竖屏后, 清空gui的buffer, 否则之前的数据有残留
    gui_dc_clear( scene_swf->app_scene.handle_gui_dc );

    //kill timer
    if ( -1 != scene_swf->timer_id )
    {
        kill_timer( scene_swf->timer_id );
        scene_swf->timer_id = -1;
    }
    
    //删除窗体
    if ( scene_swf->app_scene.win_id > 0 )
    {
        gui_wm_delete_window( scene_swf->app_scene.win_id );
        scene_swf->app_scene.win_id = -1;
    }
    
    result = _close_theme( scene_swf );
    result = _close_str_res();
    
    
    return result;
}


static int _scene_swf_proc_key( video_scene_swf_t *scene_swf, key_event_t *key_event )
{
    int index_cur = 0;
    bool result = 0;
    
//    print_dbg("keyval:0x%x, type:0x%x", key_event->val, key_event->type);
    
    switch ( key_event->val | key_event->type ) 
    {
    case KEY_VOL | KEY_TYPE_DOWN:
    case KEY_VSUB | KEY_TYPE_DOWN:
    case KEY_VADD | KEY_TYPE_DOWN:
        break;

    //短按, 则播放上一个或者从头播放本文件
    case KEY_PREV | KEY_TYPE_SHORT_UP:
        
        s_playing_switch_mode = VIDEO_FORCE_PLAY_PREV;
        
        //按prev键播放上一个; 
        index_cur = video_get_prev_play_index( LOOP_SWITCH_FORCELOOP );
        result = video_nplist_setplay_swf_by_index( index_cur );

        /*将不能播放的文件的inde记录，便于dialog的callback处理*/
        /*由于swf文件播放时不能判断是否播放完，另外，
        能不能播放只有在中间件播放时才能判断，故需要在此记录播放文件的index，
        便于timer在收到中间件error时从plist中删除，并播放下一个文件*/
        s_index_for_setplay_false = index_cur;

        /* 文件打开失败, 提示, 从plist删除失败文件, 继续播放下一个 */
        if ( false == result )
        {
            swf_control( V_CMD_STOP, NULL );
            s_enter_timer = 0;
            video_dialog_create( VIDEO_DIALOG_OPEN_FAIL, _dialog_cb_setplay_false);
        }

        //clear key
        clear_key_msg_queue(-1,-1);
        break;
        
    //是短按, 则播放下一个
    case KEY_NEXT | KEY_TYPE_SHORT_UP:
        
        s_playing_switch_mode = VIDEO_FORCE_PLAY_NEXT;
        
        index_cur = video_get_next_play_index( LOOP_SWITCH_FORCELOOP );            
        result = video_nplist_setplay_swf_by_index( index_cur );

        /*将不能播放的文件的inde记录，便于dialog的callback处理*/
        /*由于swf文件播放时不能判断是否播放完，另外，
        能不能播放只有在中间件播放时才能判断，故需要在此记录播放文件的index，
        便于timer在收到中间件error时从plist中删除，并播放下一个文件*/
        s_index_for_setplay_false = index_cur;
        
        if ( false == result )
        {
            swf_control( V_CMD_STOP, NULL );
            s_enter_timer = 0;
            video_dialog_create( VIDEO_DIALOG_OPEN_FAIL, _dialog_cb_setplay_false);
        }
        
        //clear key
        clear_key_msg_queue(-1,-1);
        break;
/*       
    //由于swf文件播放过程中不能弹提示创建dialog，因此，不响应删除操作
    //删除文件快捷键    
    case KEY_DELETE | KEY_TYPE_DOWN:
        s_enter_timer = 1;
        
        video_dialog_create( VIDEO_DIALOG_DELETE_FILE, _dialog_cb_delete_file );
        break;
*/
            
    case KEY_RETURN | KEY_TYPE_SHORT_UP:
        scene_swf->app_scene.scene_type = SCENE_RETURN;
        exit_msg_loop();
        break;

    default:
        break;
    }
    return 1;
}


/* scene msg process */
static int _scene_swf_proc_gui( video_scene_swf_t *scene_swf, gui_wm_message_t *pmsg )
{
    int result = 1;
    
    switch (pmsg->msgid) 
    {
    case WM_MSG_KEY:
        _scene_swf_proc_key( scene_swf, (key_event_t *)(pmsg->data.p) );
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
*   注意: 各控件间 及与swf scene的关系
*       swf使用 canvas_get_blending_top获取top
*******************************************************************************/
static void _scene_swf_cb(gui_wm_message_t *pmsg)
{ 
    /* 收到down按键后才继续处理按键, 否则直接退出 */
    if ( WM_MSG_KEY == pmsg->msgid )
    {
        if ( KEY_TYPE_DOWN == ((key_event_t *)(pmsg->data.p))->type )
        {
            video_vars.scene_swf.is_key_valid = 1;
        }
    }
    if ( 1 != video_vars.scene_swf.is_key_valid )
    {
        return;
    }
    
    _scene_swf_proc_gui( &video_vars.scene_swf, pmsg );
}



/******************************************************************************/
/*!                    
* \par  Description:
*       场景入口函数. scene entry function
* \param[in]    
* \retval       
*******************************************************************************/
scene_type_e video_scene_swf( video_scene_swf_t *scene_swf )
{
    scene_type_e next_scene = SCENE_ERR;
    bool result;
    msg_apps_t  msg;

    print_dbg();
    
    sys_pm_set_clk_domain(&s_clk_domain);
    //禁止屏保, 省电
    sys_forbid_screensaver( true );
    sys_forbid_auto_standby( true);
    
    /* 场景初始化 */
    result = _scene_swf_init( scene_swf );
    if(false == result)
    {
        print_err("scene init error!\n");
        goto VIDEO_SCENE_SWF_ERR_INIT;
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

    next_scene = scene_swf->app_scene.scene_type;
    /* 场景退出 */
    VIDEO_SCENE_SWF_ERR_INIT:
    //恢复屏保, 省电
    sys_forbid_screensaver( false );
    sys_forbid_auto_standby( false );
    
    result = _scene_swf_deinit( scene_swf );
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

