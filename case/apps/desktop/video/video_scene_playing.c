/*******************************************************************************
 *                              NOYA1100
 *                            Module: video
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-04-01          1.0             build this file 
 *      dengjihai   2009-11-20          1.1
*******************************************************************************/
/*!
 * \file     video_scene_playing.c
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
#include "video_playing_option.h"

/* extern function, 函数声明 */
static bool _get_battery_voltage( charge_voltage_t *vol );

static void _dialog_cb( dialog_t *dialog, widget_sig_dialog_e signal );
static void _dialog_cb_setplay_false( dialog_t *dialog, widget_sig_dialog_e signal );

static int  _scene_playing_proc_timer( void *param );

static void _option_callback(menuview_t *menuview, int signal);
static bool _open_theme( video_scene_playing_t *scene_playing );
static bool _close_theme( video_scene_playing_t *scene_playing );
static bool _open_str_res( void );
static bool _close_str_res( void );

static bool _playing_ui_blending_create( void );
static bool _playing_ui_blending_delete( void );
static bool _playing_ui_show( void );
static bool _playing_ui_hide( void );

static bool _scene_playing_ui_init( video_scene_playing_t *scene_playing );
static bool _scene_playing_init( video_scene_playing_t *scene_playing );
static bool _scene_playing_deinit( video_scene_playing_t *scene_playing );
static void _scene_playing_paint( video_scene_playing_t *scene_playing );
static int  _scene_playing_proc_key( video_scene_playing_t *scene_playing, key_event_t *key_event );
static int  _scene_playing_proc_gui( video_scene_playing_t *scene_playing, gui_wm_message_t *pmsg );
static void _scene_playing_cb( gui_wm_message_t *pmsg );


static const clk_domain_t s_clk_domain = {CLK_LOAD_MIN, CLK_LOAD_MAX};

/* define variable, 变量定义 */
/* 用于:创建普通dialog时需要等待其显示3秒后再继续执行timer中的动作, 此时应禁止进入timer中断 */
int g_enter_timer = 1;
/*此变量用于记录不能播放的文件index，用于在dialog的回调函数中从nplist中删除对应文件,并设置下一个文件*/
static int s_index_for_setplay_false = 0;
/*此变量用于记录当前播放时文件切换方式，便于获取下一个要播放的文件的index*/
static video_play_switch_mode_e s_playing_switch_mode = VIDEO_AUTO_PLAY;

#if 0
#endif
/******************************************************************************/
/*!                    
* \par  Description:
*       playing界面甩歌功能, 切换到下一曲, 相当于next键功能
* \param[in]    
*   注意: 
*       非playing界面, 非play状态, 直接返回
*******************************************************************************/
void video_scene_playing_shaked( video_scene_playing_t *scene_playing )
{
    bool result = true;
    int index_cur = -1;
    
    if ( NULL == scene_playing )
    {
        return;
    }
    if ( scene_playing->app_scene.win_id <= 0 )
    {
        return;
    }
    if ( MMM_PLAYER_PLAYING != scene_playing->status.player_status )
    {
        return;
    }
    
    video_control(V_CMD_GET_PLAYER_STATUS, &scene_playing->status);
    
#if VIDEO_CFG_PLAYING_PREV_KEY_PLAY_CURRENT_TIME > 0
    //如果有时间门槛, 则小于此时间门槛时, 不记录到recent_list
    if ( scene_playing->status.cur_time > VIDEO_CFG_PLAYING_PREV_KEY_PLAY_CURRENT_TIME )
    {
        video_recent_list_add_item( video_vars.plist.np_index, scene_playing->status.cur_time );
    }
#else
    //如果没有时间门槛, 全部记录到recent_list
    video_recent_list_add_item( video_vars.plist.np_index, scene_playing->status.cur_time );
#endif

    index_cur = video_get_next_play_index( LOOP_SWITCH_FORCELOOP );
    result = video_nplist_setplay_by_index( index_cur, NULL );
    //shaofeng 立即刷新切换文件后的时间
    video_control(V_CMD_GET_PLAYER_STATUS, (void *)&scene_playing->status);
    scene_playing->cur_time = scene_playing->status.cur_time;
    _scene_playing_paint( scene_playing );
    
    if ( false == result )
    {
        video_control( V_CMD_STOP, NULL );

        g_enter_timer = 0;
        /*将不能播放的文件的inde记录，便于dialog的callback处理*/
        s_index_for_setplay_false = index_cur;
        s_playing_switch_mode = VIDEO_FORCE_PLAY_NEXT;
        video_dialog_create( VIDEO_DIALOG_OPEN_FAIL, _dialog_cb_setplay_false);
    }
}

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
        print_err("delete file error!\n");
        //shaofeng dialog
    }
    
    file_total_num = video_plist_get_item_num( video_vars.nplist );      
    if( file_total_num == 0 )//只有一个文件，删除之后直接返回,在timer中会退出播放
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

    result = video_nplist_setplay_by_index( index_next, NULL );
    if ( false == result )
    {
        video_control( V_CMD_STOP, NULL );

        g_enter_timer = 0;
        /*将不能播放的文件的inde记录，便于dialog的callback处理*/
        s_index_for_setplay_false = index_next;
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
*       playing界面弹出的dialog的回调函数
* \param[in]    
*   注意: 
*       此类dialog弹出后, 不再需要进入timer查询video状态. 在创建dialog前, 需要设置g_enter_timer = 0.
*       此类dialog消失后, 需要进入timer. 设置g_enter_timer = 1.
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
    
    g_enter_timer = 1;
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

/*
需要分情况处理的情形:
1.当前播放的是最后一首
2.下一个要播放的文件不支持

不同处理方式:
1.自动播放和手动播放时处理方式不同
2.循环模式不同时处理方式不同
*/
static void _dialog_cb_setplay_false( dialog_t *dialog, widget_sig_dialog_e signal )
{    
    bool result;
    int index_next = s_index_for_setplay_false;
    
    switch ( signal ) 
    {
    case WIDGET_SIG_DIALOG_OK:
        
    case WIDGET_SIG_DIALOG_CANCAL:
        video_dialog_delete();
        print_dbg("set index %d faile.\nswitch mode = %d \n",s_index_for_setplay_false, s_playing_switch_mode);

        /*从np_list中删除并更新loop参数*/
        result = video_nplist_del_item( s_index_for_setplay_false, s_playing_switch_mode, &index_next );
        //删除记录失败，loop不会有更新，返回的next_index不是有效值，需要获取下一个要播放的index
        if( false == result )
        {
            if( VIDEO_AUTO_PLAY == s_playing_switch_mode )
            {
                index_next = video_get_next_play_index( LOOP_SWITCH_NORMAL);
            }
            else if( VIDEO_FORCE_PLAY_NEXT == s_playing_switch_mode )
            {
                index_next = video_get_next_play_index( LOOP_SWITCH_FORCELOOP);
            }
            else
            {
                index_next = video_get_prev_play_index( LOOP_SWITCH_FORCELOOP);                
            }
        }
        //只有自动播放时到最后一首index_next才会返回-1
        if( -1 == index_next )
        {
            print_dbg("index is -1!!!\n");
            //退出前将 index_cur==0文件 置为np信息
            video_nplist_set_to_first();
            
            //非loop模式播放结束, 退出.. (如:sequence模式播放到最后一曲)
            video_vars.scene_playing.app_scene.scene_type = SCENE_LIST;
            exit_msg_loop();
            return;
            
        }
        result = video_nplist_setplay_by_index( index_next, NULL );            
        if ( false == result )
        {
            video_control( V_CMD_STOP, NULL );

            g_enter_timer = 0;
            /*将不能播放的文件的inde记录，便于dialog的callback处理*/
            s_index_for_setplay_false = index_next;
            video_dialog_create( VIDEO_DIALOG_OPEN_FAIL, _dialog_cb_setplay_false);
        }
        else
        {
            g_enter_timer = 1;
        }
        
        //shaofeng 立即刷新切换文件后的时间
        video_control(V_CMD_GET_PLAYER_STATUS, (void *)&video_vars.scene_playing.status);
        video_vars.scene_playing.cur_time = video_vars.scene_playing.status.cur_time;
        _scene_playing_paint( &video_vars.scene_playing );


        break;
    default:
        break;
    }
}
/******************************************************************************/
/*!                    
* \par  Description:
*       playing界面的timer中断处理函数
* \param[in]    void *: video_scene_playing_t *, 场景指针
*   实现的功能: 
*       获取当前播放时间和播放状态, 更新显示
*       根据当前播放状态为stop时, 播放上一个\下一个文件
*       累计playing界面的ui显示时间, 自动隐藏ui
*   注意: 
*       当文件正常播放到文件尾为stop状态, 当文件快进快退到文件头尾时, 也为stop状态
*       需要增加变量记录不同的stop状态操作, 实现对应的功能
*******************************************************************************/
int _scene_playing_proc_timer( void *param )
{
    static int cnt_get_bat_vol = 0;
    /* 获取当前电池电压, 是否低于播放video的门限 */
    charge_voltage_t battery_vol;
    /* 快退到文件头, 从0播放此文件的标志变量 */
    static bool is_fb_to_play_cur = false;
    /* 快退到文件头, 从文件尾播放此文件的标志变量 */
    static bool is_fb_to_play_prev = false;
    volatile bool result = 0;
    volatile int index_cur = 0;
    mmm_video_tag_t tag;
    char fullpath[PATH_MAX];
    video_scene_playing_t *scene_playing = (video_scene_playing_t *)param;
    
//    print_dbg("s_enter_timer:%d", g_enter_timer);
    //如果不允许进入timer, 则直接退出. 用于dialog提示时, 在dialog的cb中设置为1
    if ( 0 == g_enter_timer )
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
                g_enter_timer = 0;
                video_dialog_create( VIDEO_DIALOG_LOW_BAT ,_dialog_cb);
                /*创建dialog之后马上刷新屏幕*/
                gui_screen_update();
                
                /*马上要退出场景，延时以便可以显示出提示框*/
                sleep(1);
                
                video_vars.flag_save_tag = true;
                scene_playing->app_scene.scene_type = SCENE_LIST;
                exit_msg_loop();
                return 1;
            }
        }
    }
    
    //获得cur_time和status, 以更新显示
    video_control(V_CMD_GET_PLAYER_STATUS, (void *)&scene_playing->status);
    scene_playing->cur_time = scene_playing->status.cur_time;
    if ( MMM_PLAYER_PLAYING == scene_playing->status.player_status )
    {
        scene_playing->show.status_frame = VIDEO_PLAY;
    }
    else if ( MMM_PLAYER_PAUSED == scene_playing->status.player_status )
    {
        scene_playing->show.status_frame = VIDEO_PAUSE;
    }
    else if ( MMM_PLAYER_FFB == scene_playing->status.player_status )
    {
        if ( FFB_KEY_STATUS_FB == scene_playing->ffb_key_status )
        {
            scene_playing->show.status_frame = VIDEO_FB_HL;
        }
        else if ( FFB_KEY_STATUS_FF == scene_playing->ffb_key_status )
        {
            scene_playing->show.status_frame = VIDEO_FF_HL;
        }
        else
        {
        }
    }
    else
    {
    }
    
    /* 对播放到文件尾而处于stop状态的处理: 处理快进快退到文件头尾, 寻找下一个,, */
    if (MMM_PLAYER_STOPPED == scene_playing->status.player_status)
    {

        /*如果播放到文件尾，从最近播放文件中删除。
        注:最近播放列表记录的是最近播放而没有播放完的文件*/
        video_recent_list_delete_item( video_vars.plist.np_index );
        
#if VIDEO_CFG_FFB_END_THEN_START_NEXT == 0
        //如果是快进快退到文件头尾而处于stop状态: 方案0: 停在文件头尾
        //且: 长按快退到文件头, 松手后, 播放当前index
        if ( FFB_KEY_STATUS_FB == scene_playing->ffb_key_status )
        {
            is_fb_to_play_cur = true;
            goto SCENE_PLAYINT_PAINT;
        }
        else if ( FFB_KEY_STATUS_FF == scene_playing->ffb_key_status )
        {
            goto SCENE_PLAYINT_PAINT;
        }
        else
        {
        }
#else
        //如果是快进快退到文件头而处于stop状态: 方案1: seek到文件尾, 继续快快退下一个
        if ( FFB_KEY_STATUS_FB == scene_playing->ffb_key_status )
        {
            is_fb_to_play_prev = true;
        }
#endif /* end #if VIDEO_CFG_FFB_END_THEN_START_NEXT == 0 */
        //fb到文件头, 从时间0处开始播放
        if ( true == is_fb_to_play_cur )
        {
            is_fb_to_play_cur = false;
            
            tag.time = 0;
            index_cur = video_vars.plist.np_index;
            result = video_nplist_setplay_by_index( index_cur, &tag );
        }
        //fb到文件头, 播放上一个文件, 并且seek到上一个的文件末尾
        else if ( true == is_fb_to_play_prev )
        {
            is_fb_to_play_prev = false;
            
            index_cur = video_get_prev_play_index( LOOP_SWITCH_FORCELOOP );
            result = video_playlist_get_fullpath_by_index( video_vars.nplist, index_cur, fullpath );
            if ( true == result )
            {
                video_control_get_total_time_by_fullpath( fullpath, &scene_playing->total_time );
                tag.time = scene_playing->total_time;
                result = video_nplist_setplay_by_index( index_cur, &tag );
            }
        }
        //正常播放
        else
        {
            index_cur = video_get_next_play_index( LOOP_SWITCH_NORMAL );
            if ( -1 == index_cur )
            {
                //退出前将 index_cur==0文件 置为np信息
                video_nplist_set_to_first();
                
                //非loop模式播放结束, 退出.. (如:sequence模式播放到最后一曲)
                scene_playing->app_scene.scene_type = SCENE_LIST;
                exit_msg_loop();
                goto SCENE_PLAYINT_PAINT;
            }
            
            result = video_nplist_setplay_by_index( index_cur, NULL );
        }
        
        if ( false == result )
        {
            video_control( V_CMD_STOP, NULL );
            
            g_enter_timer = 0;
            s_playing_switch_mode = VIDEO_AUTO_PLAY;            
            s_index_for_setplay_false = index_cur;
            video_dialog_create( VIDEO_DIALOG_OPEN_FAIL, _dialog_cb_setplay_false );
        }
         
        //从pause状态开始ffb, 则达到文件头尾后, 恢复paused状态
        if ( MMM_PLAYER_PAUSED == scene_playing->before_ffb_player_status )
        {
            scene_playing->before_ffb_player_status = MMM_PLAYER_ERROR;
            video_control(V_CMD_PAUSE, NULL);
            scene_playing->show.status_frame = VIDEO_PAUSE;
        }
        
#if VIDEO_CFG_FFB_END_THEN_START_NEXT > 0
        //如果是快进快退到文件头尾而处于stop状态: 方案1: play后, 继续快进快退下一个
        if ( FFB_KEY_STATUS_FB == scene_playing->ffb_key_status )
        {
            video_control(V_CMD_FAST_BACKWARD, &video_vars.play_param.ffb_step);
        }
        else if ( FFB_KEY_STATUS_FF == scene_playing->ffb_key_status )
        {
            video_control(V_CMD_FAST_FORWARD, &video_vars.play_param.ffb_step);
        }
        else
        {
        }
#endif
    }
    /* 对出错的处理: 提示, nplist删除 */
    else if(MMM_PLAYER_ERROR == scene_playing->status.player_status)
    {
        video_control( V_CMD_STOP, NULL );//处于err状态, 需要先stop
        s_playing_switch_mode = VIDEO_AUTO_PLAY;
        g_enter_timer = 0;
        s_index_for_setplay_false = video_vars.plist.np_index;
        video_dialog_create( VIDEO_DIALOG_DECODE_ERR ,_dialog_cb_setplay_false );
        
        /*从最近播放而没有播放完的文件列表中删除*/
        video_recent_list_delete_item( video_vars.plist.np_index );
    }
    else
    {
    }
    
    //计算是否隐藏playing界面的 status ui
    if ( 1 == video_vars.blending.status_is_show )
    {
        video_vars.blending.auto_hide_time++;
        if ( VIDEO_AUTO_HIDE_TIME == video_vars.blending.auto_hide_time )
        {
            _playing_ui_hide();
        }
/* 暂停状态进度条不消失 */
#if VIDEO_CFG_PLAYING_UI_SHOW_ALLTIME_WHEN_PAUSED > 0
        if (MMM_PLAYER_PAUSED == scene_playing->status.player_status)
        {
            video_vars.blending.auto_hide_time = 0;
        }
#endif
    }
    
    SCENE_PLAYINT_PAINT:
    //如果ui处于显示状态, 则刷新到屏幕. 否则处于隐藏状态, 则只刷新数据, 但不刷新到屏幕
    if ( 1 == video_vars.blending.status_is_show )
    {
        _scene_playing_paint( scene_playing );
    }
    
    return 1;
}



/******************************************************************************/
/*!                    
* \par  Description:
*       playing界面加载风格
* \param[in]    video_scene_playing_t *: 场景指针
* \retval       true:成功; false:失败
*   实现的功能: 
*       加载commonui图片字符串资源
*       加载本场景的图片字符串资源
*       将本场景的各元素从资源中依次获取出来, 用于后续显示
*******************************************************************************/
bool video_scene_playing_load_style( video_scene_playing_t *scene_playing )
{
    scene_t* scene;
    
    /* load theme 资源文件 */
    if(_open_theme( scene_playing ) == false)
    {
        return false;
    }

    /* load 字符串资源文件 */
    if(_open_str_res() == false)
    {
        return false;
    }
    
    scene = scene_playing->app_scene.scene;
    //load pic and pic region from scene or resource group
    scene_playing->pic_pbar_bg       = ( picture_resource_t * )get_scene_child( scene, PIC_PBAR_BG );
    scene_playing->pic_pbar          = ( picture_resource_t * )get_scene_child( scene, PIC_PBAR );
    scene_playing->pic_pbar_hl       = ( picture_resource_t * )get_scene_child( scene, PIC_PBAR_HL );
    scene_playing->pic_select_status = ( picture_resource_t * )get_scene_child( scene, PIC_SELECT);    
    scene_playing->resgroup_pbar_highlight_rect      = ( resgroup_resource_t * ) \
        get_scene_child( scene, RESGROUP_PBAR_HIGHLIGHT_RECT );
    if ( (NULL != scene_playing->resgroup_pbar_highlight_rect) && (NULL != scene_playing->pic_pbar_bg) )
    {
        scene_playing->pbar_length = scene_playing->resgroup_pbar_highlight_rect->width;
        scene_playing->pbar_xstart = scene_playing->resgroup_pbar_highlight_rect->absolute_x \
             - scene_playing->pic_pbar_bg->x;
    }
    else
    {
        scene_playing->pbar_length = 0;
        scene_playing->pbar_xstart = 0;
    }

    scene_playing->pic_status_bg    = ( picture_resource_t * )get_scene_child( scene, PIC_STATUS_BG );
    scene_playing->reg_status       = ( picregion_resource_t * )get_scene_child( scene, REG_STATUS );
    
    //load string from resource group
    scene_playing->str_time_left    = ( string_resource_t * )get_scene_child( scene, STR_TIME_LEFT );
    scene_playing->str_time_right   = ( string_resource_t * )get_scene_child( scene, STR_TIME_RIGHT );
    
    return true;
}

bool proc_displaymode_update( void )
{
    video_control(V_CMD_GET_PLAYER_STATUS, (void *)&video_vars.scene_playing.status);
    print_dbg("player status:%d", video_vars.scene_playing.status.player_status);
    if (MMM_PLAYER_PAUSED == video_vars.scene_playing.status.player_status)
    {       
        //resume cmd
        video_control(V_CMD_RESUME, NULL);
        
        sys_forbid_screensaver( true );
        sys_forbid_auto_standby( true );
    }
    return true;
}

static void _option_callback(menuview_t *menuview, int signal)
{

}


static bool _open_theme( video_scene_playing_t *scene_playing )
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
    scene_playing->app_scene.scene = scene;
    
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

static bool _close_theme( video_scene_playing_t *scene_playing )
{

    //退出场景
    free_scene( video_vars.commonui_scene );
    video_vars.commonui_scene = NULL;
    free_scene( scene_playing->app_scene.scene );
    scene_playing->app_scene.scene = NULL;

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

//shaofeng 函数名 ui_create包括:blending和canvas
/******************************************************************************/
/*!                    
* \par  Description:
*       创建ui界面除commonui外的其他各元素的canvas和blending区域.
*       playing元素的canvas和blending属性都为hide状态, 需要显示时, 再将blending设置为show状态
*       界面元素的canvas属性为show状态, 否则在暂停时无法刷新
* \param[in]    
* \retval       bool: true:创建成功; false:创建失败
* 注意: blending区域的所有坐标都是8的倍数!
*******************************************************************************/
static bool _playing_ui_blending_create( void )
{
    bool result = true;
    sty_rect_t srect;
    canvas_info_t canvas_info = {0};
    
#if 1
    //创建pbar区域的图层
    srect.x = video_vars.scene_playing.pic_pbar_bg->x;
    srect.y = video_vars.scene_playing.pic_pbar_bg->y;
    srect.width = video_vars.scene_playing.pic_pbar_bg->width;
    srect.height= video_vars.scene_playing.pic_pbar_bg->height;
    video_vars.blending.pbar_canvas_id = canvas_create( &srect, TRANSPARENT );
    if ( -1 == video_vars.blending.pbar_canvas_id )
    {
        print_warning("canvas_create fail\n\n");
        return false;
    }
    //playing界面的canvas为提高显示效率, 将canvas设为隐藏
    canvas_hide( video_vars.blending.pbar_canvas_id );
    //获得canvas的blending_buf, 用于创建blending
    canvas_get_info( video_vars.blending.pbar_canvas_id, &canvas_info );
    
    /* 创建blend区域, cretae pbar blending area*/
    video_vars.blending.pbar_is_show   = 0;
    video_vars.blending.pbar.blend_param.color_key = VIDEO_DEFAULT_BLENDING_COLOR_KEY;
    video_vars.blending.pbar.blend_param.alpha     = VIDEO_DEFAULT_BLENDING_ALPHA;
    video_vars.blending.pbar.blend_param.area.x    = canvas_info.rect.x;
    video_vars.blending.pbar.blend_param.area.y    = canvas_info.rect.y;
    video_vars.blending.pbar.blend_param.area.w    = canvas_info.rect.width;
    video_vars.blending.pbar.blend_param.area.h    = canvas_info.rect.height;
    video_vars.blending.pbar.blend_param.blending_buf = (unsigned char *)canvas_info.pdata;
    video_vars.blending.pbar.handle = -1;
    result = video_control(V_CMD_BLENDING_AREA_CREATE, &video_vars.blending.pbar);
    if ( (false == result) || (video_vars.blending.pbar.handle < 0) )
    {
        print_warning("send AREA_CREATE err\n\n");
        return false;
    }
#endif


#if 1    
    //创建str区域的图层
    srect.x = video_vars.scene_playing.str_time_left->x;
    srect.y = video_vars.scene_playing.str_time_left->y;
    srect.width = video_vars.scene_playing.str_time_left->width;
    srect.height= video_vars.scene_playing.str_time_left->height;
    video_vars.blending.time_left_canvas_id = canvas_create( &srect, TRANSPARENT );
    if ( -1 == video_vars.blending.time_left_canvas_id )
    {
        print_warning("canvas_create fail\n\n");
        return false;
    }
    //playing界面的canvas为提高显示效率, 将canvas设为隐藏
    canvas_hide( video_vars.blending.time_left_canvas_id );
    //获得canvas的blending_buf, 用于创建blending
    canvas_get_info( video_vars.blending.time_left_canvas_id, &canvas_info );
    
    /* time left的blend区域初始化, time left blending area init*/
    video_vars.blending.time_left_is_show   = 0;
    video_vars.blending.time_left.blend_param.color_key = VIDEO_DEFAULT_BLENDING_COLOR_KEY;
    video_vars.blending.time_left.blend_param.alpha     = VIDEO_DEFAULT_BLENDING_ALPHA;
    video_vars.blending.time_left.blend_param.area.x    = canvas_info.rect.x;
    video_vars.blending.time_left.blend_param.area.y    = canvas_info.rect.y;
    video_vars.blending.time_left.blend_param.area.w    = canvas_info.rect.width;
    video_vars.blending.time_left.blend_param.area.h    = canvas_info.rect.height;
    video_vars.blending.time_left.blend_param.blending_buf = (unsigned char *)canvas_info.pdata;
    video_vars.blending.time_left.handle = -1;
    result = video_control(V_CMD_BLENDING_AREA_CREATE, &video_vars.blending.time_left);
    if ( (false == result) || (video_vars.blending.time_left.handle < 0) )
    {
        print_warning("send AREA_CREATE err\n\n");
        return false;
    }
#endif


#if 1    
    //创建str区域的图层
    srect.x = video_vars.scene_playing.str_time_right->x;
    srect.y = video_vars.scene_playing.str_time_right->y;
    srect.width = video_vars.scene_playing.str_time_right->width;
    srect.height= video_vars.scene_playing.str_time_right->height;
    video_vars.blending.time_right_canvas_id = canvas_create( &srect, TRANSPARENT );
    if ( -1 == video_vars.blending.time_right_canvas_id )
    {
        print_warning("canvas_create fail\n\n");
        return false;
    }
    //playing界面的canvas为提高显示效率, 将canvas设为隐藏
    canvas_hide( video_vars.blending.time_right_canvas_id );
    //获得canvas的blending_buf, 用于创建blending
    canvas_get_info( video_vars.blending.time_right_canvas_id, &canvas_info );
    
    /* time_right的blend区域初始化, time_right blending area init*/
    video_vars.blending.time_right_is_show   = 0;
    video_vars.blending.time_right.blend_param.color_key = VIDEO_DEFAULT_BLENDING_COLOR_KEY;
    video_vars.blending.time_right.blend_param.alpha     = VIDEO_DEFAULT_BLENDING_ALPHA;
    video_vars.blending.time_right.blend_param.area.x    = canvas_info.rect.x;
    video_vars.blending.time_right.blend_param.area.y    = canvas_info.rect.y;
    video_vars.blending.time_right.blend_param.area.w    = canvas_info.rect.width;
    video_vars.blending.time_right.blend_param.area.h    = canvas_info.rect.height;
    video_vars.blending.time_right.blend_param.blending_buf = (unsigned char *)canvas_info.pdata;
    video_vars.blending.time_right.handle = -1;
    result = video_control(V_CMD_BLENDING_AREA_CREATE, &video_vars.blending.time_right);
    if ( (false == result) || (video_vars.blending.time_right.handle < 0) )
    {
        print_warning("send AREA_CREATE err\n\n");
        return false;
    }
#endif

#if 1    
    //创建status 区域的图层
    srect.x = video_vars.scene_playing.pic_status_bg->x;
    srect.y = video_vars.scene_playing.pic_status_bg->y;
    srect.width = video_vars.scene_playing.pic_status_bg->width;
    srect.height= video_vars.scene_playing.pic_status_bg->height;
    video_vars.blending.status_canvas_id = canvas_create( &srect, TRANSPARENT );
    if ( -1 == video_vars.blending.status_canvas_id )
    {
        print_warning("canvas_create fail\n\n");
        return false;
    }
    //playing界面的canvas为提高显示效率, 将canvas设为隐藏
    canvas_hide( video_vars.blending.status_canvas_id );
    //获得canvas的blending_buf, 用于创建blending
    canvas_get_info( video_vars.blending.status_canvas_id, &canvas_info );
    
    /* 状态图标blend区域初始化, status blending area init*/
    video_vars.blending.status_is_show   = 0;
    video_vars.blending.status.blend_param.color_key = VIDEO_DEFAULT_BLENDING_COLOR_KEY;
    video_vars.blending.status.blend_param.alpha     = VIDEO_DEFAULT_BLENDING_ALPHA;
    video_vars.blending.status.blend_param.area.x    = canvas_info.rect.x;
    video_vars.blending.status.blend_param.area.y    = canvas_info.rect.y;
    video_vars.blending.status.blend_param.area.w    = canvas_info.rect.width;
    video_vars.blending.status.blend_param.area.h    = canvas_info.rect.height;
    video_vars.blending.status.blend_param.blending_buf = (unsigned char *)canvas_info.pdata;
    video_vars.blending.status.handle = -1;
    result = video_control(V_CMD_BLENDING_AREA_CREATE, &video_vars.blending.status);
    if ( (false == result) || (video_vars.blending.status.handle < 0) )
    {
        print_warning("send AREA_CREATE err\n\n");
        return false;
    }
#endif
    
    return true;
}

static bool _playing_ui_blending_delete( void )
{
    bool result = true;
    
    if ( -1 != video_vars.blending.pbar.handle )
    {
        result = video_control(V_CMD_BLENDING_AREA_DESTROY, &video_vars.blending.pbar.handle);
    }
    canvas_delete( video_vars.blending.pbar_canvas_id );
    
    if ( -1 != video_vars.blending.time_left.handle )
    {
        result = video_control(V_CMD_BLENDING_AREA_DESTROY, &video_vars.blending.time_left.handle);
    }
    canvas_delete( video_vars.blending.time_left_canvas_id );
    
    if ( -1 != video_vars.blending.time_right.handle )
    {
        result = video_control(V_CMD_BLENDING_AREA_DESTROY, &video_vars.blending.time_right.handle);
    }
    canvas_delete( video_vars.blending.time_right_canvas_id );
    
    if ( -1 != video_vars.blending.status.handle )
    {
        result = video_control(V_CMD_BLENDING_AREA_DESTROY, &video_vars.blending.status.handle);
    }
    canvas_delete( video_vars.blending.status_canvas_id );
    
    return result;
}


/******************************************************************************/
/*!                    
* \par  Description:
*       显示playing界面的ui元素, 即:将blending设置为show状态
*       各ui元素, ui图层, ui的blending区域都已经创建, 此函数仅仅将其显示
*       各ui元素包括: headbar, status, pbar, left time, right time
* \param[in]    
* \retval       true:成功; false:失败
*******************************************************************************/
static bool _playing_ui_show( void )
{
    bool result = true;
    
    //已经是显示的, 直接返回
    if ( (1 == video_vars.blending.status_is_show) && (1 == video_vars.blending.pbar_is_show) )
    {
        return false;
    }
    
    memset( &video_vars.blending.blending_handle, BLENDING_HANLE_NULL, sizeof(video_vars.blending.blending_handle) );
    video_vars.blending.blending_handle[0] = video_vars.blending.headbar.handle;
    video_vars.blending.blending_handle[1] = video_vars.blending.status.handle;
    video_vars.blending.blending_handle[2] = video_vars.blending.pbar.handle;
    video_vars.blending.blending_handle[3] = video_vars.blending.time_left.handle;
    video_vars.blending.blending_handle[4] = video_vars.blending.time_right.handle;
    result = video_control(V_CMD_BLENDING_AREA_SHOW, video_vars.blending.blending_handle);

    if ( false == result )
    {
        print_warning("send AREA_SHOW err\n\n");
        return false;
    }
    
    video_vars.blending.headbar_is_show     = 1;
    video_vars.blending.status_is_show      = 1;
    video_vars.blending.pbar_is_show        = 1;
    video_vars.blending.time_left_is_show   = 1;
    video_vars.blending.time_right_is_show  = 1;
    
    video_vars.blending.auto_hide_time = 0;
    
    return true;
}

static bool _playing_ui_hide( void )
{
    bool result;
    
    //已经是隐藏的
    if ( (0 == video_vars.blending.status_is_show) && (0 == video_vars.blending.pbar_is_show) )
    {
        return false;
    }
    
    memset( &video_vars.blending.blending_handle, BLENDING_HANLE_NULL, sizeof(video_vars.blending.blending_handle) );
    video_vars.blending.blending_handle[0] = video_vars.blending.headbar.handle;
    video_vars.blending.blending_handle[1] = video_vars.blending.status.handle;
    video_vars.blending.blending_handle[2] = video_vars.blending.pbar.handle;
    video_vars.blending.blending_handle[3] = video_vars.blending.time_left.handle;
    video_vars.blending.blending_handle[4] = video_vars.blending.time_right.handle;
    
    result = video_control(V_CMD_BLENDING_AREA_HIDE, video_vars.blending.blending_handle);
    if ( false == result )
    {
        print_warning("send AREA_HIDE err\n\n");
        return false;
    }
    
    video_vars.blending.headbar_is_show     = 0;
    video_vars.blending.status_is_show      = 0;
    video_vars.blending.pbar_is_show        = 0;
    video_vars.blending.time_left_is_show   = 0;
    video_vars.blending.time_right_is_show  = 0;
    
    return true;
}



/******************************************************************************/
/*!                    
* \par  Description:
*       playing界面ui状态初始化. 仅用于进入场景时初始化一次
* \param[in]    video_scene_playing_t *: 场景指针
* \retval       true:成功; false:失败
*   实现的功能: 
*       设置各元素初始显示状态
*       获取各元素初始值, 主要是当前播放时间
*******************************************************************************/
static bool _scene_playing_ui_init( video_scene_playing_t *scene_playing )
{
    bool result = true;
    unsigned int time_recent_list;
    
    scene_playing->show.pbar_bg = 1;
    scene_playing->show.pbar = 1;
    
    scene_playing->show.status_bg = 1;
    scene_playing->show.status = 1;
    scene_playing->show.status_frame = VIDEO_PLAY;
    
    scene_playing->show.time_left = 1;
    scene_playing->show.time_right = 1;
    
    //初始化cur time, 以初始化ui界面
    if ( 0 == video_vars.plist.np_tag.time )
    {
        //tag时间为0, 说明不是从taglist进入, 需要查找最近是否播放过
        result = video_recent_list_get_tag_by_fullpath( video_vars.plist.np_fullpath, &time_recent_list );
        if ( true == result )
        {
            video_vars.plist.np_tag.time = time_recent_list;
        }
    }
    scene_playing->cur_time = video_vars.plist.np_tag.time;
    
    /* 初始化saved_show, 保证第一次show与saved_show不同, 而一定执行draw函数 */
    memset(&scene_playing->saved_show, -1, sizeof(scene_playing->saved_show));
    
    return result;
}


/******************************************************************************/
/*!                    
* \par  Description:
*       scene initial function. 场景初始化. 
* \param[in]    video_scene_playing_t *: 场景指针
* \retval       true:成功; false:失败
*   实现的功能: 
*       申请资源. 初始化资源. 主要是window, gui dc, blending资源
*       根据np_index和tag启动播放
*******************************************************************************/
static bool _scene_playing_init( video_scene_playing_t *scene_playing )
{
    bool result = true;
    scene_t *scene = NULL;
    charge_voltage_t battery_vol;

    print_dbg("enter scene playing");        
    
    //变量初始化
    memset( scene_playing, 0x00, sizeof(video_scene_playing_t) );
    video_vars.flag_save_tag = false;
    g_enter_timer = 1;
    
    //mmm与swf互斥
    video_mmm_swf_deinit();
    //进入playing, 需要mmm
    result = video_mmm_init();
    if ( false == result )
    {
        print_err("video_mmm_init err");
        return false;
    }
    result = video_control_set_mem_mode( MMM_MEM_NORMAL_MODE );
    if ( false == result )
    {
        print_err("set_mem_mode err");
        return false;
    }
    
    //载入场景
    result = video_scene_playing_load_style( scene_playing );
    if (false == result)
    {
        print_err("load_style fail\n\n");
        return false;
    }
    scene = scene_playing->app_scene.scene;
    
    //创建窗口
    print_dbg("playwin xywh:%d,%d,%d,%d", scene->x, scene->y, scene->width, scene->height);
    scene_playing->app_scene.win_id = gui_wm_create_window(scene->x, scene->y, scene->width, scene->height, \
                                        WM_CREATE_SHOW, _scene_playing_cb, 0);
    if( scene_playing->app_scene.win_id <= 0 )
    {
        print_err("gui_wm_create_window failed\n\n");
        return false;
    }
    
    //获取gui dc句柄
    scene_playing->app_scene.handle_gui_dc = gui_dc_get( (unsigned short)scene_playing->app_scene.win_id );
    if (scene_playing->app_scene.handle_gui_dc <= 0)
    {
        print_err("gui_dc_get fail\n\n");
        return false;
    }
    
    //设置焦点
    gui_wm_set_focus( scene_playing->app_scene.win_id );
    
    //一定在loadstyle和创建窗口之后, 设置屏幕方向. 此场景的任何绘图操作一定在这之后
    video_set_scene_direction( ROTATION_0 );
    //设置横竖屏后, 清空gui的buffer, 否则之前的数据有残留
    gui_dc_clear( scene_playing->app_scene.handle_gui_dc );

    //注意:所有提示都应该在设置屏幕方向之后提示，以免进入播放前后屏幕方向有变化，
    //导致提示位置不正确或背景显示乱码

    //进入播放前, 检测电量
    result = _get_battery_voltage( &battery_vol );
    if ( true == result )
    {
        if ( battery_vol <= VOL_THRESHOLD_PLAY_VIDEO )
        {
            //没有进入播放界面, 需要显示时, 一定先设置NON_YUV. 否则花屏
            gui_set_display_mode( GUI_DISPLAY_MODE_NON_YUV );
            video_dialog_modal( VIDEO_DIALOG_LOW_BAT );
            return false;
        }
    }
    
    canvas_set_blending_mode( true );
    
    //init common and playing ui. 
    //注意: 此显示部分耗时较多, 需在play前准备好. 否则play之后过一段时间才显示出来
    video_msgbox_create();
    
    video_headbar_create();
    
    _playing_ui_blending_create();
    video_set_display_mode( video_vars.play_param.displaymode );


    //使用np_index及tag开始播放
    if ( 0 == video_vars.plist.np_tag.time )
    {
        _scene_playing_ui_init( scene_playing );
        
        //tag时间为0, 说明没有tag信息, 则传入NULL, 比传入tag.time = 0效率高
        result = video_nplist_setplay_by_index( video_vars.plist.np_index, NULL );
    }
    else//tag时间不为0, 为: 从taglist进入, 或者为recent list中的文件
    {
        _scene_playing_ui_init( scene_playing );
        //是tag_list中文件，则传入tag信息，从tag时间点开始播放
        result = video_nplist_setplay_by_index( video_vars.plist.np_index, &video_vars.plist.np_tag );
    }
    
    //点击的文件打开失败, 则清空np. 不管从np dirlist taglist进入playing
    if ( false == result )
    {
        video_control( V_CMD_STOP, NULL );
        //没有进入播放界面, 需要显示时, 一定先设置NON_YUV. 否则花屏
        gui_set_display_mode( GUI_DISPLAY_MODE_NON_YUV );
        video_dialog_modal( VIDEO_DIALOG_OPEN_FAIL );
        //打开失败的文件, 同时清除其taglist, 用于删除tag文件后, 再从taglist进入播放的情况
        video_taglist_del_items_by_fullpath( video_vars.plist.np_fullpath );
        video_nplist_clear_npinfo();
        
        return false;
    }
    video_vars.flag_has_np_file = 1;
    video_set_play_mode( video_vars.play_param.playmode );
//    video_set_display_mode( video_vars.play_param.displaymode );
    
#if VIDEO_CFG_PLAYING_UI_SHOW_WHEN_START_PLAY > 0
    headbar_paint( video_vars.headbar );
    _scene_playing_paint( scene_playing );
    _playing_ui_show();
#endif
    //全部初始化成功, 最后启动timer, 定时get video播放状态 信息
    scene_playing->timer_id = set_timer(VIDEO_PLAYING_TIMER_INTERVAL, _scene_playing_proc_timer, (void *)scene_playing);
    if (scene_playing->timer_id < 0)
    {
        print_err("set_timer failed");
        return false;
    }
    
    return result;
}


/* scene exit function */
static bool _scene_playing_deinit( video_scene_playing_t *scene_playing )
{
    bool result = true;
    mmm_video_area_t video_area = {0, 0, 0, 0};
    rotation_e angle;

    print_dbg("exit scene playing\n");

    /*如果正在播放列表没有文件，不显示正在播放列表*/
    if( 0 == video_plist_get_item_num( video_vars.nplist ) )
    {
        video_vars.flag_has_np_file = false;
        print_dbg("no file in nplist\n");

    }
    
    //如果需要保存tag, np信息已经自动保存, 并将此np添加到recent_list
    if ( true == video_vars.flag_save_tag )
    {
        //正常退出时在这里统一stop, 并获得tag信息. 其他非正常情况也会stop, 但无需保留tag信息
        video_control( V_CMD_STOP, &video_vars.plist.np_tag );
        video_recent_list_add_item( video_vars.plist.np_index, video_vars.plist.np_tag.time );
    }
    else
    {
        //先停止播放，再设置显示区域
        video_control( V_CMD_STOP, NULL );
    }    
    
    //清除video的yuv数据的显示区域, 用于显示rgb数据
    video_control(V_CMD_SET_VIDEO_AREA, &video_area);

    _playing_ui_blending_delete();
    
    video_control_set_mem_mode( MMM_MEM_THUMBNAIL_MODE );

    //deinit commonui    
    video_headbar_delete();
    video_msgbox_delete();
    video_dialog_delete();
    video_vbar_delete();
    video_adjust_delete();
    
    //删除 option
    video_playing_widget_option_delete( video_vars.menuview );
    video_vars.menuview = NULL;
    
    canvas_set_blending_mode( false );
    
    //获取视图角度
    angle = sty_get_view_rotation();
    video_set_scene_direction( angle );
    //设置横竖屏后, 清空gui的buffer, 否则之前的数据有残留
    gui_dc_clear( scene_playing->app_scene.handle_gui_dc );

    //kill timer
    if ( -1 != scene_playing->timer_id )
    {
        kill_timer( scene_playing->timer_id );
        scene_playing->timer_id = -1;
    }
    
    //删除窗体
    if ( scene_playing->app_scene.win_id > 0 )
    {
        gui_wm_delete_window( scene_playing->app_scene.win_id );
        scene_playing->app_scene.win_id = -1;
    }
    
    result = _close_theme( scene_playing );
    result = _close_str_res();
    
    return result;
}


/* scene paint function */
static void _scene_playing_paint( video_scene_playing_t *scene_playing )
{
    int x_coordinate;
    bool tmp;
    
    /* 画背景 */
    video_playing_draw_pbar_bg( scene_playing, scene_playing->show.pbar_bg );
    
    /* 根据当前播放时间和总时间, 计算进度条坐标, 画进度条 */
    tmp = video_playing_time_to_pbar_coordinate( scene_playing->cur_time, scene_playing->total_time, &x_coordinate );
    if (true == tmp)
    {
        video_playing_draw_pbar( scene_playing, scene_playing->show.pbar, x_coordinate );
    }
    
    /* 根据当前play pause ff fb等播放状态, 相应的画play pause ff fb图标 */
    video_playing_draw_status_bg( scene_playing, scene_playing->show.status_bg );
    video_playing_draw_status( scene_playing, scene_playing->show.status, scene_playing->show.status_frame );
    
    /* 画左右显示的时间 */
    video_playing_draw_time_left( scene_playing, scene_playing->show.time_left, scene_playing->cur_time );
    video_playing_draw_time_right( scene_playing, scene_playing->show.time_right, scene_playing->total_time );
}

static void _scene_playing_cancel_ffb(video_scene_playing_t *scene_playing)
{
    print_dbg("player status:%d", scene_playing->status.player_status);
    video_control(V_CMD_CANCEL_FFB, NULL);
    scene_playing->ffb_key_status = FFB_KEY_STATUS_OTHER;

    //如果没有ffb到文件头尾, 则恢复ffb之前状态. 否则timer启动下一个
    video_control(V_CMD_GET_PLAYER_STATUS, &scene_playing->status);
    if ( MMM_PLAYER_STOPPED != scene_playing->status.player_status)
    {
        //恢复ffb之前记录的状态
        if ( MMM_PLAYER_PLAYING == scene_playing->before_ffb_player_status )
        {
            video_control(V_CMD_RESUME, NULL);
            scene_playing->show.status_frame = VIDEO_PLAY;
        }
        else if ( MMM_PLAYER_PAUSED == scene_playing->before_ffb_player_status )
        {
            video_control(V_CMD_PAUSE, NULL);
            scene_playing->show.status_frame = VIDEO_PAUSE;
        }
        else
        {
        }
    }
    _scene_playing_paint( scene_playing );    
}

/**********************************************************************************************                   
* \par  Description:
*       场景回调之前的异常情况处理:
*       1.拔卡/外部存储器后连续打开文件失败
*       2.快进快退中插入USB
* \param[in]    void *: video_scene_playing_t *, 场景指针
*   实现的功能: 
*       1.拔卡/外部存储器后在连续打开文件失败弹提示中能按return返回list
*       2.快进快退时插入USB弹出对话框后抬起按键能取消快进快退
***********************************************************************************************/
static void _scene_playing_pre_proc(video_scene_playing_t *scene_playing, gui_wm_message_t *pmsg )
{
    key_event_t *kmsg;
    switch (pmsg->msgid) 
    {
    case WM_MSG_KEY:
        kmsg = (key_event_t *)(pmsg->data.p);
            
        switch ( kmsg->val | kmsg->type ) 
        {
            
        case KEY_PREV | KEY_TYPE_LONG_UP:
        case KEY_PREV | KEY_TYPE_HOLD_UP:
        case KEY_NEXT | KEY_TYPE_LONG_UP:
        case KEY_NEXT | KEY_TYPE_HOLD_UP:
            {
                _scene_playing_cancel_ffb( scene_playing );
            }
            break;

        case KEY_RETURN | KEY_TYPE_DOWN:
            {
                if ( ((MMM_PLAYER_STOPPED == scene_playing->status.player_status) \
                    ||(MMM_PLAYER_ERROR == scene_playing->status.player_status)) \
                    && (NULL != video_vars.dialog) )
                {
                    video_dialog_delete();
                    //退出前将 index_cur==0文件 置为np信息
                    video_nplist_set_to_first();
                    
                    //返回list场景
                    scene_playing->app_scene.scene_type = SCENE_LIST;
                    exit_msg_loop();
                }
            }
            break;
            
        default:
            break;
        }
    default:
        break;
    }    
}

static int _scene_playing_proc_key( video_scene_playing_t *scene_playing, key_event_t *key_event )
{
    int index_cur = 0;
    bool result = 0;

//    print_dbg("keyval:0x%x, type:0x%x", key_event->val, key_event->type);
    
    //playing ui显示时, 收到按键时, 清零hide time
    if ( 1 == video_vars.blending.pbar_is_show )
    {
        video_vars.blending.auto_hide_time = 0;
    }
    //playing ui隐藏时, 刷新并将其显示出来
    else
    {
        //return 键不显示ui, 直接退出
        if ( KEY_RETURN != key_event->val )
        {
            headbar_paint( video_vars.headbar );
            _scene_playing_paint( scene_playing );
            _playing_ui_show();
        }
//1: playing ui隐藏时, 按键, 则出现ui, 但不执行按键动作, 继续操作按键才有效;
#if VIDEO_CFG_PLAYING_FIRST_KEY_IGNORE > 0
        return;
#endif
    }
    
    switch ( key_event->val | key_event->type ) 
    {
    case KEY_VOL | KEY_TYPE_DOWN:
    case KEY_VSUB | KEY_TYPE_DOWN:
    case KEY_VADD | KEY_TYPE_DOWN:
        video_vbar_create();

        break;

    case KEY_PREV | KEY_TYPE_LONG:
        scene_playing->show.status_frame = VIDEO_FB_HL;
        _scene_playing_paint( scene_playing );
        
        //记录ffb之前的状态, 用于取消ffb之后, 恢复到此状态
        video_control(V_CMD_GET_PLAYER_STATUS, &scene_playing->status);
        scene_playing->before_ffb_player_status = scene_playing->status.player_status;
        
        video_control(V_CMD_FAST_BACKWARD, &video_vars.play_param.ffb_step);
        scene_playing->ffb_key_status = FFB_KEY_STATUS_FB;
        break;
    case KEY_NEXT | KEY_TYPE_LONG:
        scene_playing->show.status_frame = VIDEO_FF_HL;
        _scene_playing_paint( scene_playing );

        //记录ffb之前的状态, 用于取消ffb之后, 恢复到此状态
        video_control(V_CMD_GET_PLAYER_STATUS, &scene_playing->status);
        scene_playing->before_ffb_player_status = scene_playing->status.player_status;
        
        video_control(V_CMD_FAST_FORWARD, &video_vars.play_param.ffb_step);
        scene_playing->ffb_key_status = FFB_KEY_STATUS_FF;
        break;
        
    //短按, 则播放上一个或者从头播放本文件
    case KEY_PREV | KEY_TYPE_SHORT_UP:
        
        s_playing_switch_mode = VIDEO_FORCE_PLAY_PREV;
        //the previcon hightlight, then play prev file
        scene_playing->show.status_frame = VIDEO_PREV_HL;
        _scene_playing_paint( scene_playing );
        
        scene_playing->show.status_frame = VIDEO_PLAY;
        _scene_playing_paint( scene_playing );
        
        //将尚未播放完成的文件 添加到recent list
        video_control(V_CMD_GET_PLAYER_STATUS, &scene_playing->status);
#if VIDEO_CFG_PLAYING_PREV_KEY_PLAY_CURRENT_TIME > 0
        //如果有时间门槛, 则小于此时间门槛时, 不记录到recent_list
        if ( scene_playing->status.cur_time > VIDEO_CFG_PLAYING_PREV_KEY_PLAY_CURRENT_TIME )
        {
            video_recent_list_add_item( video_vars.plist.np_index, scene_playing->status.cur_time );
        }
#else
        //如果没有时间门槛, 全部记录到recent_list
        video_recent_list_add_item( video_vars.plist.np_index, scene_playing->status.cur_time );
#endif
        
#if VIDEO_CFG_PLAYING_PREV_KEY_PLAY_CURRENT_AGAIN > 0
        //按prev键, 如果时间大于VIDEO_CFG_PLAYING_PREV_KEY_PLAY_CURRENT_TIME, 从头播放当前曲目, 而不是播放上一个; 
        if ( scene_playing->status.cur_time > VIDEO_CFG_PLAYING_PREV_KEY_PLAY_CURRENT_TIME )
        {
            mmm_video_tag_t video_tag = {0};
            
            index_cur = video_vars.plist.np_index;
            result = video_nplist_setplay_by_index( index_cur, &video_tag );
        }
        else
        {
            //按prev键播放上一个; 
            index_cur = video_get_prev_play_index( LOOP_SWITCH_FORCELOOP );
            result = video_nplist_setplay_by_index( index_cur, NULL );
        }
#else
        //按prev键播放上一个; 
        index_cur = video_get_prev_play_index( LOOP_SWITCH_FORCELOOP );
        result = video_nplist_setplay_by_index( index_cur, NULL );
#endif /* end #if VIDEO_CFG_PLAYING_PREV_KEY_PLAY_CURRENT_AGAIN > 0 */

        //shaofeng 立即刷新切换文件后的时间
        video_control(V_CMD_GET_PLAYER_STATUS, (void *)&scene_playing->status);
        scene_playing->cur_time = scene_playing->status.cur_time;
        _scene_playing_paint( scene_playing );

        /* 文件打开失败, 提示, 从plist删除失败文件, 继续播放下一个 */
        if ( false == result )
        {
            video_control( V_CMD_STOP, NULL );
            g_enter_timer = 0;
            /*将不能播放的文件的inde记录，便于dialog的callback处理*/
            s_index_for_setplay_false = index_cur;
            video_dialog_create( VIDEO_DIALOG_OPEN_FAIL, _dialog_cb_setplay_false);
        }

        //clear key
        clear_key_msg_queue(-1,-1);
        break;
    //是长按, 则取消快进快退
    case KEY_PREV | KEY_TYPE_LONG_UP:
    case KEY_PREV | KEY_TYPE_HOLD_UP:
        _scene_playing_cancel_ffb( scene_playing );
        break;
        
    //是短按, 则播放下一个
    case KEY_NEXT | KEY_TYPE_SHORT_UP:
        
        s_playing_switch_mode = VIDEO_FORCE_PLAY_NEXT;
        //the nexticon hightlight, then play prev file
        scene_playing->show.status_frame = VIDEO_NEXT_HL;
        _scene_playing_paint( scene_playing );
        
        scene_playing->show.status_frame = VIDEO_PLAY;
        _scene_playing_paint( scene_playing );
        
        //将尚未播放完成的文件 添加到recent list
        video_control(V_CMD_GET_PLAYER_STATUS, &scene_playing->status);
#if VIDEO_CFG_PLAYING_PREV_KEY_PLAY_CURRENT_TIME > 0
        //如果有时间门槛, 则小于此时间门槛时, 不记录到recent_list
        if ( scene_playing->status.cur_time > VIDEO_CFG_PLAYING_PREV_KEY_PLAY_CURRENT_TIME )
        {
            video_recent_list_add_item( video_vars.plist.np_index, scene_playing->status.cur_time );
        }
#else
        //如果没有时间门槛, 全部记录到recent_list
        video_recent_list_add_item( video_vars.plist.np_index, scene_playing->status.cur_time );
#endif   

        index_cur = video_get_next_play_index( LOOP_SWITCH_FORCELOOP );
        result = video_nplist_setplay_by_index( index_cur, NULL );
        //shaofeng 立即刷新切换文件后的时间
        video_control(V_CMD_GET_PLAYER_STATUS, (void *)&scene_playing->status);
        scene_playing->cur_time = scene_playing->status.cur_time;
        _scene_playing_paint( scene_playing );
        
        if ( false == result )
        {
            video_control( V_CMD_STOP, NULL );
            g_enter_timer = 0;
            /*将不能播放的文件的inde记录，便于dialog的callback处理*/           
            s_index_for_setplay_false = index_cur;
            video_dialog_create( VIDEO_DIALOG_OPEN_FAIL, _dialog_cb_setplay_false);
        }
        
        //clear key
        clear_key_msg_queue(-1,-1);
        break;
        
    //是长按, 则取消快进快退
    case KEY_NEXT | KEY_TYPE_LONG_UP:
    case KEY_NEXT | KEY_TYPE_HOLD_UP:
        _scene_playing_cancel_ffb( scene_playing );
        break;

    case KEY_MENU | KEY_TYPE_DOWN:
        video_playing_widget_option_create( (menuview_cb_t)_option_callback );
        break;

    //删除文件快捷键    
    case KEY_DELETE | KEY_TYPE_DOWN:
        g_enter_timer = 1;
        video_dialog_create( VIDEO_DIALOG_DELETE_FILE, _dialog_cb_delete_file );
        
        //clear key
        clear_key_msg_queue(-1,-1);
        break;
  
    case KEY_OK | KEY_TYPE_SHORT_UP:
    case KEY_OK | KEY_TYPE_LONG_UP:
    case KEY_OK | KEY_TYPE_HOLD_UP:
        video_control(V_CMD_GET_PLAYER_STATUS, (void *)&scene_playing->status);
        print_dbg("player status:%d", scene_playing->status.player_status);
        if (MMM_PLAYER_PLAYING == scene_playing->status.player_status)
        {
            //draw hight light play icon
            scene_playing->show.status_frame = VIDEO_PLAY_HL;
            _scene_playing_paint( scene_playing );
            gui_screen_update();
            
            //delay to highlight icon
            usleep(20000);
            
            //draw pause icon
            scene_playing->show.status_frame = VIDEO_PAUSE;
            _scene_playing_paint( scene_playing );
            gui_screen_update();
            
            //pause cmd
            video_control(V_CMD_PAUSE, NULL);
            
            sys_forbid_screensaver( false );
//            sys_forbid_auto_standby( false );
        }
        else if (MMM_PLAYER_PAUSED == scene_playing->status.player_status)
        {
            //draw hight light pause icon
            scene_playing->show.status_frame = VIDEO_PAUSE_HL;
            _scene_playing_paint( scene_playing );
            gui_screen_update();
            
            //delay to highlight icon
            usleep(30000);
            
            //draw play icon
            scene_playing->show.status_frame = VIDEO_PLAY;
            _scene_playing_paint( scene_playing );
            gui_screen_update();
            
            //resume cmd
            video_control(V_CMD_RESUME, NULL);
            
            sys_forbid_screensaver( true );
//            sys_forbid_auto_standby( true );
        }
        else
        {
            print_warning("status err:%d", scene_playing->status.player_status);
        }
        
        //clear key
        clear_key_msg_queue(-1,-1);
        break;
        
    case KEY_RETURN | KEY_TYPE_SHORT_UP:
        video_vars.flag_save_tag = true;
        scene_playing->app_scene.scene_type = SCENE_RETURN;
        exit_msg_loop();
        break;

    default:
        break;
    }
    return 1;
}


/* scene msg process */
static int _scene_playing_proc_gui( video_scene_playing_t *scene_playing, gui_wm_message_t *pmsg )
{
    int result = 1;
    switch (pmsg->msgid) 
    {
    case WM_MSG_KEY:
        _scene_playing_proc_key( scene_playing, (key_event_t *)(pmsg->data.p) );
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
*       playing使用 canvas_get_blending_top获取top
*******************************************************************************/
static void _scene_playing_cb(gui_wm_message_t *pmsg)
{ 
    int top;
        
    /* 收到down按键后才继续处理按键, 否则直接退出 */
    if ( WM_MSG_KEY == pmsg->msgid )
    {
        if ( KEY_TYPE_DOWN == ((key_event_t *)(pmsg->data.p))->type )
        {
            video_vars.scene_playing.is_key_valid = 1;
        }
    }
    if ( 1 != video_vars.scene_playing.is_key_valid )
    {
        return;
    }
    
    //canvas_get_top只寻找show状态的top. canvas_get_blending_focus会找到所有的canvas. 
    //由于blending区域的canvas都是hide状态, 只有使用blending界面的scene才调用此接口
    top = canvas_get_blending_focus( pmsg );
    
//    print_dbg("top canvas:%d", top);

    //先进行异常事件处理
    _scene_playing_pre_proc( &video_vars.scene_playing, pmsg );

    
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
    else if ( (NULL != video_vars.adjust) && (top == video_vars.adjust->canvas_id) )
    {
        adjust_proc_gui( video_vars.adjust, pmsg );
    }
    else if ( (NULL != video_vars.menuview) && (top == video_vars.menuview->canvas_id) )
    {
        menuview_proc_gui( video_vars.menuview, pmsg );
    }
    else
    {
        _scene_playing_proc_gui( &video_vars.scene_playing, pmsg );
    }
}



/******************************************************************************/
/*!                    
* \par  Description:
*       场景入口函数. scene entry function
* \param[in]    
* \retval       
*******************************************************************************/
scene_type_e video_scene_playing( video_scene_playing_t *scene_playing )
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
    result = _scene_playing_init( scene_playing );
    if(false == result)
    {
        print_err("scene init error!\n");
        goto VIDEO_SCENE_PLAYING_ERR_INIT;
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

    next_scene = scene_playing->app_scene.scene_type;
    /* 场景退出 */
    VIDEO_SCENE_PLAYING_ERR_INIT:
    //恢复屏保, 省电
    sys_forbid_screensaver( false );
    sys_forbid_auto_standby( false );
    
    result = _scene_playing_deinit( scene_playing );
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

