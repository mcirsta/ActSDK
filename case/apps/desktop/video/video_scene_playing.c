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

/* include file of this application, ��Ӧ�õ�ͷ�ļ� */
#include "include_video.h"
#include "video_playing_option.h"

/* extern function, �������� */
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

/* define variable, �������� */
/* ����:������ͨdialogʱ��Ҫ�ȴ�����ʾ3����ټ���ִ��timer�еĶ���, ��ʱӦ��ֹ����timer�ж� */
int g_enter_timer = 1;
/*�˱������ڼ�¼���ܲ��ŵ��ļ�index��������dialog�Ļص������д�nplist��ɾ����Ӧ�ļ�,��������һ���ļ�*/
static int s_index_for_setplay_false = 0;
/*�˱������ڼ�¼��ǰ����ʱ�ļ��л���ʽ�����ڻ�ȡ��һ��Ҫ���ŵ��ļ���index*/
static video_play_switch_mode_e s_playing_switch_mode = VIDEO_AUTO_PLAY;

#if 0
#endif
/******************************************************************************/
/*!                    
* \par  Description:
*       playing����˦�蹦��, �л�����һ��, �൱��next������
* \param[in]    
*   ע��: 
*       ��playing����, ��play״̬, ֱ�ӷ���
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
    //�����ʱ���ż�, ��С�ڴ�ʱ���ż�ʱ, ����¼��recent_list
    if ( scene_playing->status.cur_time > VIDEO_CFG_PLAYING_PREV_KEY_PLAY_CURRENT_TIME )
    {
        video_recent_list_add_item( video_vars.plist.np_index, scene_playing->status.cur_time );
    }
#else
    //���û��ʱ���ż�, ȫ����¼��recent_list
    video_recent_list_add_item( video_vars.plist.np_index, scene_playing->status.cur_time );
#endif

    index_cur = video_get_next_play_index( LOOP_SWITCH_FORCELOOP );
    result = video_nplist_setplay_by_index( index_cur, NULL );
    //shaofeng ����ˢ���л��ļ����ʱ��
    video_control(V_CMD_GET_PLAYER_STATUS, (void *)&scene_playing->status);
    scene_playing->cur_time = scene_playing->status.cur_time;
    _scene_playing_paint( scene_playing );
    
    if ( false == result )
    {
        video_control( V_CMD_STOP, NULL );

        g_enter_timer = 0;
        /*�����ܲ��ŵ��ļ���inde��¼������dialog��callback����*/
        s_index_for_setplay_false = index_cur;
        s_playing_switch_mode = VIDEO_FORCE_PLAY_NEXT;
        video_dialog_create( VIDEO_DIALOG_OPEN_FAIL, _dialog_cb_setplay_false);
    }
}

/******************************************************************************/
/*!                    
* \par  Description:
*       playing����ɾ����ǰ�����ļ�����������һ���ļ�������ʧ������ʾ���ļ�ʧ��
* \param[in]    
*******************************************************************************/
void video_scene_playing_delete_file( void )
{
    bool result = true;
    int file_total_num;
    int index_next = 0;
    
      
    //ɾ��np�ļ�, ������һ��
    result = video_delete_npfile( video_vars.plist.np_fullpath , &index_next );
    if ( false == result )
    {
        print_err("delete file error!\n");
        //shaofeng dialog
    }
    
    file_total_num = video_plist_get_item_num( video_vars.nplist );      
    if( file_total_num == 0 )//ֻ��һ���ļ���ɾ��֮��ֱ�ӷ���,��timer�л��˳�����
    {
        return;
    }
    
    //��һ��Ҫ���ŵ��ļ���indexΪ-1����ʾ��ǰɾ�����ǲ����б�����һ��
    if( index_next == -1 )
    {
        if( VIDEO_PLAYMOE_SHUFFLE == video_vars.play_param.playmode )
        {
            //���ģʽ�����һ�ף����³�ʼ��shuffle�����һ��
            index_next = rand() % file_total_num;
        }
        else
        {
            //˳��ģʽ�����һ�ף����³�ʼ��loop����һ��
            index_next = 0;
        }
        video_init_play_mode( video_vars.play_param.playmode, index_next, file_total_num );             
    }

    result = video_nplist_setplay_by_index( index_next, NULL );
    if ( false == result )
    {
        video_control( V_CMD_STOP, NULL );

        g_enter_timer = 0;
        /*�����ܲ��ŵ��ļ���inde��¼������dialog��callback����*/
        s_index_for_setplay_false = index_next;
        video_dialog_create( VIDEO_DIALOG_OPEN_FAIL, _dialog_cb_setplay_false);
    }
}

static bool _get_battery_voltage( charge_voltage_t *vol )
{
#if VIDEO_CFG_DETECT_VOL_THRESHOLD_ENABLE > 0
    bool result = true;
    /* ����:��ȡ��ص��� */
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
*       playing���浯����dialog�Ļص�����
* \param[in]    
*   ע��: 
*       ����dialog������, ������Ҫ����timer��ѯvideo״̬. �ڴ���dialogǰ, ��Ҫ����g_enter_timer = 0.
*       ����dialog��ʧ��, ��Ҫ����timer. ����g_enter_timer = 1.
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
��Ҫ��������������:
1.��ǰ���ŵ������һ��
2.��һ��Ҫ���ŵ��ļ���֧��

��ͬ����ʽ:
1.�Զ����ź��ֶ�����ʱ����ʽ��ͬ
2.ѭ��ģʽ��ͬʱ����ʽ��ͬ
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

        /*��np_list��ɾ��������loop����*/
        result = video_nplist_del_item( s_index_for_setplay_false, s_playing_switch_mode, &index_next );
        //ɾ����¼ʧ�ܣ�loop�����и��£����ص�next_index������Чֵ����Ҫ��ȡ��һ��Ҫ���ŵ�index
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
        //ֻ���Զ�����ʱ�����һ��index_next�Ż᷵��-1
        if( -1 == index_next )
        {
            print_dbg("index is -1!!!\n");
            //�˳�ǰ�� index_cur==0�ļ� ��Ϊnp��Ϣ
            video_nplist_set_to_first();
            
            //��loopģʽ���Ž���, �˳�.. (��:sequenceģʽ���ŵ����һ��)
            video_vars.scene_playing.app_scene.scene_type = SCENE_LIST;
            exit_msg_loop();
            return;
            
        }
        result = video_nplist_setplay_by_index( index_next, NULL );            
        if ( false == result )
        {
            video_control( V_CMD_STOP, NULL );

            g_enter_timer = 0;
            /*�����ܲ��ŵ��ļ���inde��¼������dialog��callback����*/
            s_index_for_setplay_false = index_next;
            video_dialog_create( VIDEO_DIALOG_OPEN_FAIL, _dialog_cb_setplay_false);
        }
        else
        {
            g_enter_timer = 1;
        }
        
        //shaofeng ����ˢ���л��ļ����ʱ��
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
*       playing�����timer�жϴ�����
* \param[in]    void *: video_scene_playing_t *, ����ָ��
*   ʵ�ֵĹ���: 
*       ��ȡ��ǰ����ʱ��Ͳ���״̬, ������ʾ
*       ���ݵ�ǰ����״̬Ϊstopʱ, ������һ��\��һ���ļ�
*       �ۼ�playing�����ui��ʾʱ��, �Զ�����ui
*   ע��: 
*       ���ļ��������ŵ��ļ�βΪstop״̬, ���ļ�������˵��ļ�ͷβʱ, ҲΪstop״̬
*       ��Ҫ���ӱ�����¼��ͬ��stop״̬����, ʵ�ֶ�Ӧ�Ĺ���
*******************************************************************************/
int _scene_playing_proc_timer( void *param )
{
    static int cnt_get_bat_vol = 0;
    /* ��ȡ��ǰ��ص�ѹ, �Ƿ���ڲ���video������ */
    charge_voltage_t battery_vol;
    /* ���˵��ļ�ͷ, ��0���Ŵ��ļ��ı�־���� */
    static bool is_fb_to_play_cur = false;
    /* ���˵��ļ�ͷ, ���ļ�β���Ŵ��ļ��ı�־���� */
    static bool is_fb_to_play_prev = false;
    volatile bool result = 0;
    volatile int index_cur = 0;
    mmm_video_tag_t tag;
    char fullpath[PATH_MAX];
    video_scene_playing_t *scene_playing = (video_scene_playing_t *)param;
    
//    print_dbg("s_enter_timer:%d", g_enter_timer);
    //������������timer, ��ֱ���˳�. ����dialog��ʾʱ, ��dialog��cb������Ϊ1
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
                /*����dialog֮������ˢ����Ļ*/
                gui_screen_update();
                
                /*����Ҫ�˳���������ʱ�Ա������ʾ����ʾ��*/
                sleep(1);
                
                video_vars.flag_save_tag = true;
                scene_playing->app_scene.scene_type = SCENE_LIST;
                exit_msg_loop();
                return 1;
            }
        }
    }
    
    //���cur_time��status, �Ը�����ʾ
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
    
    /* �Բ��ŵ��ļ�β������stop״̬�Ĵ���: ���������˵��ļ�ͷβ, Ѱ����һ��,, */
    if (MMM_PLAYER_STOPPED == scene_playing->status.player_status)
    {

        /*������ŵ��ļ�β������������ļ���ɾ����
        ע:��������б��¼����������Ŷ�û�в�������ļ�*/
        video_recent_list_delete_item( video_vars.plist.np_index );
        
#if VIDEO_CFG_FFB_END_THEN_START_NEXT == 0
        //����ǿ�����˵��ļ�ͷβ������stop״̬: ����0: ͣ���ļ�ͷβ
        //��: �������˵��ļ�ͷ, ���ֺ�, ���ŵ�ǰindex
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
        //����ǿ�����˵��ļ�ͷ������stop״̬: ����1: seek���ļ�β, �����������һ��
        if ( FFB_KEY_STATUS_FB == scene_playing->ffb_key_status )
        {
            is_fb_to_play_prev = true;
        }
#endif /* end #if VIDEO_CFG_FFB_END_THEN_START_NEXT == 0 */
        //fb���ļ�ͷ, ��ʱ��0����ʼ����
        if ( true == is_fb_to_play_cur )
        {
            is_fb_to_play_cur = false;
            
            tag.time = 0;
            index_cur = video_vars.plist.np_index;
            result = video_nplist_setplay_by_index( index_cur, &tag );
        }
        //fb���ļ�ͷ, ������һ���ļ�, ����seek����һ�����ļ�ĩβ
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
        //��������
        else
        {
            index_cur = video_get_next_play_index( LOOP_SWITCH_NORMAL );
            if ( -1 == index_cur )
            {
                //�˳�ǰ�� index_cur==0�ļ� ��Ϊnp��Ϣ
                video_nplist_set_to_first();
                
                //��loopģʽ���Ž���, �˳�.. (��:sequenceģʽ���ŵ����һ��)
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
         
        //��pause״̬��ʼffb, ��ﵽ�ļ�ͷβ��, �ָ�paused״̬
        if ( MMM_PLAYER_PAUSED == scene_playing->before_ffb_player_status )
        {
            scene_playing->before_ffb_player_status = MMM_PLAYER_ERROR;
            video_control(V_CMD_PAUSE, NULL);
            scene_playing->show.status_frame = VIDEO_PAUSE;
        }
        
#if VIDEO_CFG_FFB_END_THEN_START_NEXT > 0
        //����ǿ�����˵��ļ�ͷβ������stop״̬: ����1: play��, �������������һ��
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
    /* �Գ���Ĵ���: ��ʾ, nplistɾ�� */
    else if(MMM_PLAYER_ERROR == scene_playing->status.player_status)
    {
        video_control( V_CMD_STOP, NULL );//����err״̬, ��Ҫ��stop
        s_playing_switch_mode = VIDEO_AUTO_PLAY;
        g_enter_timer = 0;
        s_index_for_setplay_false = video_vars.plist.np_index;
        video_dialog_create( VIDEO_DIALOG_DECODE_ERR ,_dialog_cb_setplay_false );
        
        /*��������Ŷ�û�в�������ļ��б���ɾ��*/
        video_recent_list_delete_item( video_vars.plist.np_index );
    }
    else
    {
    }
    
    //�����Ƿ�����playing����� status ui
    if ( 1 == video_vars.blending.status_is_show )
    {
        video_vars.blending.auto_hide_time++;
        if ( VIDEO_AUTO_HIDE_TIME == video_vars.blending.auto_hide_time )
        {
            _playing_ui_hide();
        }
/* ��ͣ״̬����������ʧ */
#if VIDEO_CFG_PLAYING_UI_SHOW_ALLTIME_WHEN_PAUSED > 0
        if (MMM_PLAYER_PAUSED == scene_playing->status.player_status)
        {
            video_vars.blending.auto_hide_time = 0;
        }
#endif
    }
    
    SCENE_PLAYINT_PAINT:
    //���ui������ʾ״̬, ��ˢ�µ���Ļ. ����������״̬, ��ֻˢ������, ����ˢ�µ���Ļ
    if ( 1 == video_vars.blending.status_is_show )
    {
        _scene_playing_paint( scene_playing );
    }
    
    return 1;
}



/******************************************************************************/
/*!                    
* \par  Description:
*       playing������ط��
* \param[in]    video_scene_playing_t *: ����ָ��
* \retval       true:�ɹ�; false:ʧ��
*   ʵ�ֵĹ���: 
*       ����commonuiͼƬ�ַ�����Դ
*       ���ر�������ͼƬ�ַ�����Դ
*       ���������ĸ�Ԫ�ش���Դ�����λ�ȡ����, ���ں�����ʾ
*******************************************************************************/
bool video_scene_playing_load_style( video_scene_playing_t *scene_playing )
{
    scene_t* scene;
    
    /* load theme ��Դ�ļ� */
    if(_open_theme( scene_playing ) == false)
    {
        return false;
    }

    /* load �ַ�����Դ�ļ� */
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

    // ȡ��ͼ�Ƕ�
    angle = sty_get_view_rotation();
    // ������Ļ���� 
    sty_set_screen_rotation(angle);

    video_vars.commonui_scene = load_scene( video_vars.commonui_style, COMMONUI_SIDELONG_SCENE );
    if ( NULL == video_vars.commonui_scene )
    {
        print_err("load commonui scene err");
        return false;
    }
    // ���� commonui �� style ����
    sty_set_common_scene( video_vars.commonui_scene );

    // ��ȡcommonui ��ͼƬ��Դ�ļ��� 
    sprintf(file, "%s/%s.res", sys_get_common_theme_res_dir(), "commonui");
    // �� commonui ͼƬ��Դ�ļ� 
    if( false == sty_open_common_pic_res(file) )
    {
        print_err("open %s failed!", file);
        return false;
    }
    
    
    // ����Ӧ�õ� style ����
    scene = load_scene( video_vars.app_style, VIDEO_PLAYING_SCENE );
    if ( scene == NULL )
    {
        print_err("load_scene fail\n\n");
        return false;   
    }
    sty_set_app_scene( scene );
    scene_playing->app_scene.scene = scene;
    
    // ��ȡͼƬ��Դ�ļ���
    sprintf(file, "%s/%s.res", sys_get_app_theme_res_dir(), "video");
    // ��Ӧ�õ�ͼƬ��Դ�ļ�
    if( false == sty_open_app_pic_res(file) )
    {
        print_err("open %s failed!", file);
        return false;
    }
    
    return true;
}

static bool _close_theme( video_scene_playing_t *scene_playing )
{

    //�˳�����
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

    // ��ȡcommonui �ַ�����Դ�ļ��� 
    sprintf(file, "%s/%s_%s.str", sys_get_common_string_res_dir(), "commonui", sys_get_cur_language_codes());
    // �� commonui �ַ�����Դ�ļ� 
    if( false == sty_open_common_str_res(file) )
    {
        print_err("open %s failed!", file);
        return false;
    }

    // ��ȡӦ���ַ�����Դ�ļ���
    sprintf(file, "%s/%s_%s.str", sys_get_app_string_res_dir(), "video", sys_get_cur_language_codes());
    // ��Ӧ�õ��ַ�����Դ�ļ� 
    if( false == sty_open_app_str_res(file) )
    {
        print_err("open %s failed!", file);
        return false;
    }

    // �������Ա���
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

//shaofeng ������ ui_create����:blending��canvas
/******************************************************************************/
/*!                    
* \par  Description:
*       ����ui�����commonui���������Ԫ�ص�canvas��blending����.
*       playingԪ�ص�canvas��blending���Զ�Ϊhide״̬, ��Ҫ��ʾʱ, �ٽ�blending����Ϊshow״̬
*       ����Ԫ�ص�canvas����Ϊshow״̬, ��������ͣʱ�޷�ˢ��
* \param[in]    
* \retval       bool: true:�����ɹ�; false:����ʧ��
* ע��: blending������������궼��8�ı���!
*******************************************************************************/
static bool _playing_ui_blending_create( void )
{
    bool result = true;
    sty_rect_t srect;
    canvas_info_t canvas_info = {0};
    
#if 1
    //����pbar�����ͼ��
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
    //playing�����canvasΪ�����ʾЧ��, ��canvas��Ϊ����
    canvas_hide( video_vars.blending.pbar_canvas_id );
    //���canvas��blending_buf, ���ڴ���blending
    canvas_get_info( video_vars.blending.pbar_canvas_id, &canvas_info );
    
    /* ����blend����, cretae pbar blending area*/
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
    //����str�����ͼ��
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
    //playing�����canvasΪ�����ʾЧ��, ��canvas��Ϊ����
    canvas_hide( video_vars.blending.time_left_canvas_id );
    //���canvas��blending_buf, ���ڴ���blending
    canvas_get_info( video_vars.blending.time_left_canvas_id, &canvas_info );
    
    /* time left��blend�����ʼ��, time left blending area init*/
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
    //����str�����ͼ��
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
    //playing�����canvasΪ�����ʾЧ��, ��canvas��Ϊ����
    canvas_hide( video_vars.blending.time_right_canvas_id );
    //���canvas��blending_buf, ���ڴ���blending
    canvas_get_info( video_vars.blending.time_right_canvas_id, &canvas_info );
    
    /* time_right��blend�����ʼ��, time_right blending area init*/
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
    //����status �����ͼ��
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
    //playing�����canvasΪ�����ʾЧ��, ��canvas��Ϊ����
    canvas_hide( video_vars.blending.status_canvas_id );
    //���canvas��blending_buf, ���ڴ���blending
    canvas_get_info( video_vars.blending.status_canvas_id, &canvas_info );
    
    /* ״̬ͼ��blend�����ʼ��, status blending area init*/
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
*       ��ʾplaying�����uiԪ��, ��:��blending����Ϊshow״̬
*       ��uiԪ��, uiͼ��, ui��blending�����Ѿ�����, �˺�������������ʾ
*       ��uiԪ�ذ���: headbar, status, pbar, left time, right time
* \param[in]    
* \retval       true:�ɹ�; false:ʧ��
*******************************************************************************/
static bool _playing_ui_show( void )
{
    bool result = true;
    
    //�Ѿ�����ʾ��, ֱ�ӷ���
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
    
    //�Ѿ������ص�
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
*       playing����ui״̬��ʼ��. �����ڽ��볡��ʱ��ʼ��һ��
* \param[in]    video_scene_playing_t *: ����ָ��
* \retval       true:�ɹ�; false:ʧ��
*   ʵ�ֵĹ���: 
*       ���ø�Ԫ�س�ʼ��ʾ״̬
*       ��ȡ��Ԫ�س�ʼֵ, ��Ҫ�ǵ�ǰ����ʱ��
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
    
    //��ʼ��cur time, �Գ�ʼ��ui����
    if ( 0 == video_vars.plist.np_tag.time )
    {
        //tagʱ��Ϊ0, ˵�����Ǵ�taglist����, ��Ҫ��������Ƿ񲥷Ź�
        result = video_recent_list_get_tag_by_fullpath( video_vars.plist.np_fullpath, &time_recent_list );
        if ( true == result )
        {
            video_vars.plist.np_tag.time = time_recent_list;
        }
    }
    scene_playing->cur_time = video_vars.plist.np_tag.time;
    
    /* ��ʼ��saved_show, ��֤��һ��show��saved_show��ͬ, ��һ��ִ��draw���� */
    memset(&scene_playing->saved_show, -1, sizeof(scene_playing->saved_show));
    
    return result;
}


/******************************************************************************/
/*!                    
* \par  Description:
*       scene initial function. ������ʼ��. 
* \param[in]    video_scene_playing_t *: ����ָ��
* \retval       true:�ɹ�; false:ʧ��
*   ʵ�ֵĹ���: 
*       ������Դ. ��ʼ����Դ. ��Ҫ��window, gui dc, blending��Դ
*       ����np_index��tag��������
*******************************************************************************/
static bool _scene_playing_init( video_scene_playing_t *scene_playing )
{
    bool result = true;
    scene_t *scene = NULL;
    charge_voltage_t battery_vol;

    print_dbg("enter scene playing");        
    
    //������ʼ��
    memset( scene_playing, 0x00, sizeof(video_scene_playing_t) );
    video_vars.flag_save_tag = false;
    g_enter_timer = 1;
    
    //mmm��swf����
    video_mmm_swf_deinit();
    //����playing, ��Ҫmmm
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
    
    //���볡��
    result = video_scene_playing_load_style( scene_playing );
    if (false == result)
    {
        print_err("load_style fail\n\n");
        return false;
    }
    scene = scene_playing->app_scene.scene;
    
    //��������
    print_dbg("playwin xywh:%d,%d,%d,%d", scene->x, scene->y, scene->width, scene->height);
    scene_playing->app_scene.win_id = gui_wm_create_window(scene->x, scene->y, scene->width, scene->height, \
                                        WM_CREATE_SHOW, _scene_playing_cb, 0);
    if( scene_playing->app_scene.win_id <= 0 )
    {
        print_err("gui_wm_create_window failed\n\n");
        return false;
    }
    
    //��ȡgui dc���
    scene_playing->app_scene.handle_gui_dc = gui_dc_get( (unsigned short)scene_playing->app_scene.win_id );
    if (scene_playing->app_scene.handle_gui_dc <= 0)
    {
        print_err("gui_dc_get fail\n\n");
        return false;
    }
    
    //���ý���
    gui_wm_set_focus( scene_playing->app_scene.win_id );
    
    //һ����loadstyle�ʹ�������֮��, ������Ļ����. �˳������κλ�ͼ����һ������֮��
    video_set_scene_direction( ROTATION_0 );
    //���ú�������, ���gui��buffer, ����֮ǰ�������в���
    gui_dc_clear( scene_playing->app_scene.handle_gui_dc );

    //ע��:������ʾ��Ӧ����������Ļ����֮����ʾ��������벥��ǰ����Ļ�����б仯��
    //������ʾλ�ò���ȷ�򱳾���ʾ����

    //���벥��ǰ, ������
    result = _get_battery_voltage( &battery_vol );
    if ( true == result )
    {
        if ( battery_vol <= VOL_THRESHOLD_PLAY_VIDEO )
        {
            //û�н��벥�Ž���, ��Ҫ��ʾʱ, һ��������NON_YUV. ������
            gui_set_display_mode( GUI_DISPLAY_MODE_NON_YUV );
            video_dialog_modal( VIDEO_DIALOG_LOW_BAT );
            return false;
        }
    }
    
    canvas_set_blending_mode( true );
    
    //init common and playing ui. 
    //ע��: ����ʾ���ֺ�ʱ�϶�, ����playǰ׼����. ����play֮���һ��ʱ�����ʾ����
    video_msgbox_create();
    
    video_headbar_create();
    
    _playing_ui_blending_create();
    video_set_display_mode( video_vars.play_param.displaymode );


    //ʹ��np_index��tag��ʼ����
    if ( 0 == video_vars.plist.np_tag.time )
    {
        _scene_playing_ui_init( scene_playing );
        
        //tagʱ��Ϊ0, ˵��û��tag��Ϣ, ����NULL, �ȴ���tag.time = 0Ч�ʸ�
        result = video_nplist_setplay_by_index( video_vars.plist.np_index, NULL );
    }
    else//tagʱ�䲻Ϊ0, Ϊ: ��taglist����, ����Ϊrecent list�е��ļ�
    {
        _scene_playing_ui_init( scene_playing );
        //��tag_list���ļ�������tag��Ϣ����tagʱ��㿪ʼ����
        result = video_nplist_setplay_by_index( video_vars.plist.np_index, &video_vars.plist.np_tag );
    }
    
    //������ļ���ʧ��, �����np. ���ܴ�np dirlist taglist����playing
    if ( false == result )
    {
        video_control( V_CMD_STOP, NULL );
        //û�н��벥�Ž���, ��Ҫ��ʾʱ, һ��������NON_YUV. ������
        gui_set_display_mode( GUI_DISPLAY_MODE_NON_YUV );
        video_dialog_modal( VIDEO_DIALOG_OPEN_FAIL );
        //��ʧ�ܵ��ļ�, ͬʱ�����taglist, ����ɾ��tag�ļ���, �ٴ�taglist���벥�ŵ����
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
    //ȫ����ʼ���ɹ�, �������timer, ��ʱget video����״̬ ��Ϣ
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

    /*������ڲ����б�û���ļ�������ʾ���ڲ����б�*/
    if( 0 == video_plist_get_item_num( video_vars.nplist ) )
    {
        video_vars.flag_has_np_file = false;
        print_dbg("no file in nplist\n");

    }
    
    //�����Ҫ����tag, np��Ϣ�Ѿ��Զ�����, ������np��ӵ�recent_list
    if ( true == video_vars.flag_save_tag )
    {
        //�����˳�ʱ������ͳһstop, �����tag��Ϣ. �������������Ҳ��stop, �����豣��tag��Ϣ
        video_control( V_CMD_STOP, &video_vars.plist.np_tag );
        video_recent_list_add_item( video_vars.plist.np_index, video_vars.plist.np_tag.time );
    }
    else
    {
        //��ֹͣ���ţ���������ʾ����
        video_control( V_CMD_STOP, NULL );
    }    
    
    //���video��yuv���ݵ���ʾ����, ������ʾrgb����
    video_control(V_CMD_SET_VIDEO_AREA, &video_area);

    _playing_ui_blending_delete();
    
    video_control_set_mem_mode( MMM_MEM_THUMBNAIL_MODE );

    //deinit commonui    
    video_headbar_delete();
    video_msgbox_delete();
    video_dialog_delete();
    video_vbar_delete();
    video_adjust_delete();
    
    //ɾ�� option
    video_playing_widget_option_delete( video_vars.menuview );
    video_vars.menuview = NULL;
    
    canvas_set_blending_mode( false );
    
    //��ȡ��ͼ�Ƕ�
    angle = sty_get_view_rotation();
    video_set_scene_direction( angle );
    //���ú�������, ���gui��buffer, ����֮ǰ�������в���
    gui_dc_clear( scene_playing->app_scene.handle_gui_dc );

    //kill timer
    if ( -1 != scene_playing->timer_id )
    {
        kill_timer( scene_playing->timer_id );
        scene_playing->timer_id = -1;
    }
    
    //ɾ������
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
    
    /* ������ */
    video_playing_draw_pbar_bg( scene_playing, scene_playing->show.pbar_bg );
    
    /* ���ݵ�ǰ����ʱ�����ʱ��, �������������, �������� */
    tmp = video_playing_time_to_pbar_coordinate( scene_playing->cur_time, scene_playing->total_time, &x_coordinate );
    if (true == tmp)
    {
        video_playing_draw_pbar( scene_playing, scene_playing->show.pbar, x_coordinate );
    }
    
    /* ���ݵ�ǰplay pause ff fb�Ȳ���״̬, ��Ӧ�Ļ�play pause ff fbͼ�� */
    video_playing_draw_status_bg( scene_playing, scene_playing->show.status_bg );
    video_playing_draw_status( scene_playing, scene_playing->show.status, scene_playing->show.status_frame );
    
    /* ��������ʾ��ʱ�� */
    video_playing_draw_time_left( scene_playing, scene_playing->show.time_left, scene_playing->cur_time );
    video_playing_draw_time_right( scene_playing, scene_playing->show.time_right, scene_playing->total_time );
}

static void _scene_playing_cancel_ffb(video_scene_playing_t *scene_playing)
{
    print_dbg("player status:%d", scene_playing->status.player_status);
    video_control(V_CMD_CANCEL_FFB, NULL);
    scene_playing->ffb_key_status = FFB_KEY_STATUS_OTHER;

    //���û��ffb���ļ�ͷβ, ��ָ�ffb֮ǰ״̬. ����timer������һ��
    video_control(V_CMD_GET_PLAYER_STATUS, &scene_playing->status);
    if ( MMM_PLAYER_STOPPED != scene_playing->status.player_status)
    {
        //�ָ�ffb֮ǰ��¼��״̬
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
*       �����ص�֮ǰ���쳣�������:
*       1.�ο�/�ⲿ�洢�����������ļ�ʧ��
*       2.��������в���USB
* \param[in]    void *: video_scene_playing_t *, ����ָ��
*   ʵ�ֵĹ���: 
*       1.�ο�/�ⲿ�洢�������������ļ�ʧ�ܵ���ʾ���ܰ�return����list
*       2.�������ʱ����USB�����Ի����̧�𰴼���ȡ���������
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
                    //�˳�ǰ�� index_cur==0�ļ� ��Ϊnp��Ϣ
                    video_nplist_set_to_first();
                    
                    //����list����
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
    
    //playing ui��ʾʱ, �յ�����ʱ, ����hide time
    if ( 1 == video_vars.blending.pbar_is_show )
    {
        video_vars.blending.auto_hide_time = 0;
    }
    //playing ui����ʱ, ˢ�²�������ʾ����
    else
    {
        //return ������ʾui, ֱ���˳�
        if ( KEY_RETURN != key_event->val )
        {
            headbar_paint( video_vars.headbar );
            _scene_playing_paint( scene_playing );
            _playing_ui_show();
        }
//1: playing ui����ʱ, ����, �����ui, ����ִ�а�������, ����������������Ч;
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
        
        //��¼ffb֮ǰ��״̬, ����ȡ��ffb֮��, �ָ�����״̬
        video_control(V_CMD_GET_PLAYER_STATUS, &scene_playing->status);
        scene_playing->before_ffb_player_status = scene_playing->status.player_status;
        
        video_control(V_CMD_FAST_BACKWARD, &video_vars.play_param.ffb_step);
        scene_playing->ffb_key_status = FFB_KEY_STATUS_FB;
        break;
    case KEY_NEXT | KEY_TYPE_LONG:
        scene_playing->show.status_frame = VIDEO_FF_HL;
        _scene_playing_paint( scene_playing );

        //��¼ffb֮ǰ��״̬, ����ȡ��ffb֮��, �ָ�����״̬
        video_control(V_CMD_GET_PLAYER_STATUS, &scene_playing->status);
        scene_playing->before_ffb_player_status = scene_playing->status.player_status;
        
        video_control(V_CMD_FAST_FORWARD, &video_vars.play_param.ffb_step);
        scene_playing->ffb_key_status = FFB_KEY_STATUS_FF;
        break;
        
    //�̰�, �򲥷���һ�����ߴ�ͷ���ű��ļ�
    case KEY_PREV | KEY_TYPE_SHORT_UP:
        
        s_playing_switch_mode = VIDEO_FORCE_PLAY_PREV;
        //the previcon hightlight, then play prev file
        scene_playing->show.status_frame = VIDEO_PREV_HL;
        _scene_playing_paint( scene_playing );
        
        scene_playing->show.status_frame = VIDEO_PLAY;
        _scene_playing_paint( scene_playing );
        
        //����δ������ɵ��ļ� ��ӵ�recent list
        video_control(V_CMD_GET_PLAYER_STATUS, &scene_playing->status);
#if VIDEO_CFG_PLAYING_PREV_KEY_PLAY_CURRENT_TIME > 0
        //�����ʱ���ż�, ��С�ڴ�ʱ���ż�ʱ, ����¼��recent_list
        if ( scene_playing->status.cur_time > VIDEO_CFG_PLAYING_PREV_KEY_PLAY_CURRENT_TIME )
        {
            video_recent_list_add_item( video_vars.plist.np_index, scene_playing->status.cur_time );
        }
#else
        //���û��ʱ���ż�, ȫ����¼��recent_list
        video_recent_list_add_item( video_vars.plist.np_index, scene_playing->status.cur_time );
#endif
        
#if VIDEO_CFG_PLAYING_PREV_KEY_PLAY_CURRENT_AGAIN > 0
        //��prev��, ���ʱ�����VIDEO_CFG_PLAYING_PREV_KEY_PLAY_CURRENT_TIME, ��ͷ���ŵ�ǰ��Ŀ, �����ǲ�����һ��; 
        if ( scene_playing->status.cur_time > VIDEO_CFG_PLAYING_PREV_KEY_PLAY_CURRENT_TIME )
        {
            mmm_video_tag_t video_tag = {0};
            
            index_cur = video_vars.plist.np_index;
            result = video_nplist_setplay_by_index( index_cur, &video_tag );
        }
        else
        {
            //��prev��������һ��; 
            index_cur = video_get_prev_play_index( LOOP_SWITCH_FORCELOOP );
            result = video_nplist_setplay_by_index( index_cur, NULL );
        }
#else
        //��prev��������һ��; 
        index_cur = video_get_prev_play_index( LOOP_SWITCH_FORCELOOP );
        result = video_nplist_setplay_by_index( index_cur, NULL );
#endif /* end #if VIDEO_CFG_PLAYING_PREV_KEY_PLAY_CURRENT_AGAIN > 0 */

        //shaofeng ����ˢ���л��ļ����ʱ��
        video_control(V_CMD_GET_PLAYER_STATUS, (void *)&scene_playing->status);
        scene_playing->cur_time = scene_playing->status.cur_time;
        _scene_playing_paint( scene_playing );

        /* �ļ���ʧ��, ��ʾ, ��plistɾ��ʧ���ļ�, ����������һ�� */
        if ( false == result )
        {
            video_control( V_CMD_STOP, NULL );
            g_enter_timer = 0;
            /*�����ܲ��ŵ��ļ���inde��¼������dialog��callback����*/
            s_index_for_setplay_false = index_cur;
            video_dialog_create( VIDEO_DIALOG_OPEN_FAIL, _dialog_cb_setplay_false);
        }

        //clear key
        clear_key_msg_queue(-1,-1);
        break;
    //�ǳ���, ��ȡ���������
    case KEY_PREV | KEY_TYPE_LONG_UP:
    case KEY_PREV | KEY_TYPE_HOLD_UP:
        _scene_playing_cancel_ffb( scene_playing );
        break;
        
    //�Ƕ̰�, �򲥷���һ��
    case KEY_NEXT | KEY_TYPE_SHORT_UP:
        
        s_playing_switch_mode = VIDEO_FORCE_PLAY_NEXT;
        //the nexticon hightlight, then play prev file
        scene_playing->show.status_frame = VIDEO_NEXT_HL;
        _scene_playing_paint( scene_playing );
        
        scene_playing->show.status_frame = VIDEO_PLAY;
        _scene_playing_paint( scene_playing );
        
        //����δ������ɵ��ļ� ��ӵ�recent list
        video_control(V_CMD_GET_PLAYER_STATUS, &scene_playing->status);
#if VIDEO_CFG_PLAYING_PREV_KEY_PLAY_CURRENT_TIME > 0
        //�����ʱ���ż�, ��С�ڴ�ʱ���ż�ʱ, ����¼��recent_list
        if ( scene_playing->status.cur_time > VIDEO_CFG_PLAYING_PREV_KEY_PLAY_CURRENT_TIME )
        {
            video_recent_list_add_item( video_vars.plist.np_index, scene_playing->status.cur_time );
        }
#else
        //���û��ʱ���ż�, ȫ����¼��recent_list
        video_recent_list_add_item( video_vars.plist.np_index, scene_playing->status.cur_time );
#endif   

        index_cur = video_get_next_play_index( LOOP_SWITCH_FORCELOOP );
        result = video_nplist_setplay_by_index( index_cur, NULL );
        //shaofeng ����ˢ���л��ļ����ʱ��
        video_control(V_CMD_GET_PLAYER_STATUS, (void *)&scene_playing->status);
        scene_playing->cur_time = scene_playing->status.cur_time;
        _scene_playing_paint( scene_playing );
        
        if ( false == result )
        {
            video_control( V_CMD_STOP, NULL );
            g_enter_timer = 0;
            /*�����ܲ��ŵ��ļ���inde��¼������dialog��callback����*/           
            s_index_for_setplay_false = index_cur;
            video_dialog_create( VIDEO_DIALOG_OPEN_FAIL, _dialog_cb_setplay_false);
        }
        
        //clear key
        clear_key_msg_queue(-1,-1);
        break;
        
    //�ǳ���, ��ȡ���������
    case KEY_NEXT | KEY_TYPE_LONG_UP:
    case KEY_NEXT | KEY_TYPE_HOLD_UP:
        _scene_playing_cancel_ffb( scene_playing );
        break;

    case KEY_MENU | KEY_TYPE_DOWN:
        video_playing_widget_option_create( (menuview_cb_t)_option_callback );
        break;

    //ɾ���ļ���ݼ�    
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
*       �����ص�����, Ҳ�����ڻص�����. һ������ֻ��һ��ȫ������
*       ���䴰��cb��gui��Ϣ: ��ȡtop���ͼ��, ����Ϣ���͸�top.
*       menuview��scene�����²�
* \param[in]    
* \retval       
*   ע��: ���ؼ��� ����playing scene�Ĺ�ϵ
*       playingʹ�� canvas_get_blending_top��ȡtop
*******************************************************************************/
static void _scene_playing_cb(gui_wm_message_t *pmsg)
{ 
    int top;
        
    /* �յ�down������ż���������, ����ֱ���˳� */
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
    
    //canvas_get_topֻѰ��show״̬��top. canvas_get_blending_focus���ҵ����е�canvas. 
    //����blending�����canvas����hide״̬, ֻ��ʹ��blending�����scene�ŵ��ô˽ӿ�
    top = canvas_get_blending_focus( pmsg );
    
//    print_dbg("top canvas:%d", top);

    //�Ƚ����쳣�¼�����
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
*       ������ں���. scene entry function
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
    //��ֹ����, ʡ��
    sys_forbid_screensaver( true );
    sys_forbid_auto_standby( true);
    
    /* ������ʼ�� */
    result = _scene_playing_init( scene_playing );
    if(false == result)
    {
        print_err("scene init error!\n");
        goto VIDEO_SCENE_PLAYING_ERR_INIT;
    }
    //�������sceneǰ��������Ϣ
    clear_sys_msg_queue( -1 );
    clear_key_msg_queue( -1, -1 );
    
    register_sys_dispatcher( (sys_msg_proc)video_proc_sys_msg );

    while(true == get_msg(&msg))
    {
        dispatch_msg(&msg);
    }

    unregister_sys_dispatcher();

    next_scene = scene_playing->app_scene.scene_type;
    /* �����˳� */
    VIDEO_SCENE_PLAYING_ERR_INIT:
    //�ָ�����, ʡ��
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

