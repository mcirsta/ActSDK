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

/* include file of this application, ��Ӧ�õ�ͷ�ļ� */
#include "include_video.h"


/* extern function, �������� */
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

/* define variable, �������� */
/* ����:������ͨdialogʱ��Ҫ�ȴ�����ʾ3����ټ���ִ��timer�еĶ���, ��ʱӦ��ֹ����timer�ж� */
static int s_enter_timer = 1;
/*�˱������ڼ�¼���ܲ��ŵ��ļ�index��������dialog�Ļص������д�nplist��ɾ����Ӧ�ļ�,��������һ���ļ�*/
static int s_index_for_setplay_false = 0;
/*�˱������ڼ�¼��ǰ����ʱ�ļ��л���ʽ�����ڻ�ȡ��һ��Ҫ���ŵ��ļ���index*/
static video_play_switch_mode_e s_playing_switch_mode = VIDEO_AUTO_PLAY;


#if 0
#endif
/******************************************************************************/
/*!                    
* \par  Description:
*       swf����˦�蹦��, �л�����һ��, �൱��next������
* \param[in]    
*   ע��: 
*       ��swf����, ��play״̬, ֱ�ӷ���
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
        /*�����ܲ��ŵ��ļ���inde��¼������dialog��callback����*/
        s_index_for_setplay_false = index_cur;
        s_playing_switch_mode = VIDEO_FORCE_PLAY_NEXT;

        
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
*       swf���浯����dialog�Ļص�����
* \param[in]    
*   ע��: 
*       ����dialog������, ������Ҫ����timer��ѯvideo״̬. �ڴ���dialogǰ, ��Ҫ����s_enter_timer = 0.
*       ����dialog��ʧ��, ��Ҫ����timer. ����s_enter_timer = 1.
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

        /*��np_list��ɾ��������loop����*/
        result = video_nplist_del_item( s_index_for_setplay_false, s_playing_switch_mode, &index_next );
        //ɾ����¼ʧ�ܣ�loop�����и��£����ص�next_index������Чֵ����Ҫ��ȡ��һ��Ҫ���ŵ�index
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
        
        //ֻ���Զ�����ʱ�����һ��index_next�Ż᷵��-1
        if( -1 == index_next )
        {
            //�˳�ǰ�� index_cur==0�ļ� ��Ϊnp��Ϣ
            video_nplist_set_to_first();
            
            //��loopģʽ���Ž���, �˳�.. (��:sequenceģʽ���ŵ����һ��)
            video_vars.scene_swf.app_scene.scene_type = SCENE_LIST;
            exit_msg_loop();
            return;        
        }
        
        result = video_nplist_setplay_swf_by_index( index_next );            
        if ( false == result )
        {
            swf_control( V_CMD_STOP, NULL );

            s_enter_timer = 0;
            /*�����ܲ��ŵ��ļ���inde��¼������dialog��callback����*/
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
        //shaofeng dialog
    }
    
    file_total_num = video_plist_get_item_num( video_vars.nplist );      
    if( file_total_num == 0 )//ֻ��һ���ļ���ɾ��֮��ֱ�ӷ���
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

    result = video_nplist_setplay_swf_by_index( index_next );
    if ( false == result )
    {
        video_control( V_CMD_STOP, NULL );

        g_enter_timer = 0;
        /*�����ܲ��ŵ��ļ���inde��¼������dialog��callback����*/
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
*       swf�����timer�жϴ�����
* \param[in]    void *: video_scene_swf_t *, ����ָ��
*   ʵ�ֵĹ���: 
*       ��ȡ��ǰ����ʱ��Ͳ���״̬, ������ʾ
*       ���ݵ�ǰ����״̬Ϊstopʱ, ������һ��\��һ���ļ�
*       �ۼ�swf�����ui��ʾʱ��, �Զ�����ui
*   ע��: 
*       ���ļ��������ŵ��ļ�βΪstop״̬, ���ļ�������˵��ļ�ͷβʱ, ҲΪstop״̬
*       ��Ҫ���ӱ�����¼��ͬ��stop״̬����, ʵ�ֶ�Ӧ�Ĺ���
*******************************************************************************/
static int _scene_swf_proc_timer( void *param )
{
    static int cnt_get_bat_vol = 0;
    /* ��ȡ��ǰ��ص�ѹ, �Ƿ���ڲ���video������ */
    charge_voltage_t battery_vol;
    volatile bool result = true;
    int index_cur;
    video_scene_swf_t *scene_swf = (video_scene_swf_t *)param;


    rotation_e angle;

    
//    print_dbg("s_enter_timer:%d", s_enter_timer);
    //������������timer, ��ֱ���˳�. ����dialog��ʾʱ, ��dialog��cb������Ϊ1
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

    //��ý���״̬���Ա����ʱ��ʾ
    swf_control(V_CMD_GET_PLAYER_STATUS, (void *)&scene_swf->player_status);
    if( MMM_PLAYER_STOPPED == scene_swf->player_status )
    {
        s_playing_switch_mode = VIDEO_AUTO_PLAY;
        index_cur = video_get_next_play_index( LOOP_SWITCH_NORMAL );
       
        if ( -1 == index_cur )
        {
            //�˳�ǰ�� index_cur==0�ļ� ��Ϊnp��Ϣ
            video_nplist_set_to_first();
            
            //��loopģʽ���Ž���, �˳�.. (��:sequenceģʽ���ŵ����һ��)
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

            /*�����ܲ��ŵ��ļ���inde��¼������dialog��callback����*/
            s_index_for_setplay_false = index_cur;
            /*�����м������ݲ����ļ�����������Ļ������˴˴���ʾ��Ҫ����������Ļ���򣬷�����*/
            sty_set_screen_rotation( ROTATION_0 );

            video_dialog_create( VIDEO_DIALOG_OPEN_FAIL, _dialog_cb_setplay_false );
        }

    }
        
    if(MMM_PLAYER_ERROR == scene_swf->player_status)
    {
        swf_control( V_CMD_STOP, NULL );//����err״̬, ��Ҫ��stop
        s_enter_timer = 0;
        
#if VIDEO_SWF_PLAYMODE == 1  
        /*�����м������ݲ����ļ�����������Ļ������˴˴���ʾ��Ҫ����������Ļ���򣬷�����*/
        sty_set_screen_rotation( ROTATION_0 );
/*
        //��ȡ��ͼ�Ƕ�
        angle = sty_get_view_rotation();
        sty_set_screen_rotation( angle );
*/
        s_playing_switch_mode = VIDEO_AUTO_PLAY;
        s_index_for_setplay_false = video_vars.plist.np_index; 
        /*�����򲥷���һ���ļ���������ǰ�ļ���plist��ɾ��*/
        video_dialog_create( VIDEO_DIALOG_DECODE_ERR, _dialog_cb_setplay_false ); 
        /*��������Ŷ�û�в�������ļ��б���ɾ��*/
        video_recent_list_delete_item( video_vars.plist.np_index );

        
#else 
        /*�����м������ݲ����ļ�����������Ļ������˴˴���ʾ��Ҫ����������Ļ���򣬷�����*/
        sty_set_screen_rotation( ROTATION_0 );
        /*����ʧ���˳�����*/  
        video_dialog_modal( VIDEO_DIALOG_DECODE_ERR );
        /*��������Ŷ�û�в�������ļ��б���ɾ��*/
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
*       swf������ط��
* \param[in]    video_scene_swf_t *: ����ָ��
* \retval       true:�ɹ�; false:ʧ��
*   ʵ�ֵĹ���: 
*       ����commonuiͼƬ�ַ�����Դ
*       ���ر�������ͼƬ�ַ�����Դ
*       ���������ĸ�Ԫ�ش���Դ�����λ�ȡ����, ���ں�����ʾ
*******************************************************************************/
bool video_scene_swf_load_style( video_scene_swf_t *scene_swf )
{
    scene_t* scene;
    
    /* load theme ��Դ�ļ� */
    if(_open_theme( scene_swf ) == false)
    {
        return false;
    }

    /* load �ַ�����Դ�ļ� */
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
    scene_swf->app_scene.scene = scene;
    
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

static bool _close_theme( video_scene_swf_t *scene_swf )
{

    //�˳�����
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



/******************************************************************************/
/*!                    
* \par  Description:
*       scene initial function. ������ʼ��. 
* \param[in]    video_scene_swf_t *: ����ָ��
* \retval       true:�ɹ�; false:ʧ��
*   ʵ�ֵĹ���: 
*       ������Դ. ��ʼ����Դ. ��Ҫ��window, gui dc, blending��Դ
*       ����np_index��tag��������
*******************************************************************************/
static bool _scene_swf_init( video_scene_swf_t *scene_swf )
{
    bool result = true;
    scene_t *scene = NULL;
    charge_voltage_t battery_vol;
    unsigned int s_cur_sdram;

    print_dbg("enter scene swf");
    
    //������ʼ��
    memset( scene_swf, 0x00, sizeof(video_scene_swf_t) );
    s_enter_timer = 1;


    //���볡��
    result = video_scene_swf_load_style( scene_swf );
    if (false == result)
    {
        print_err("load_style fail\n\n");
        return false;
    }
    scene = scene_swf->app_scene.scene;
    

    //����scene_swfʱ�ż���swf�м��. swf��mmm����
    video_mmm_deinit();
    //����dirlist���������ͼ, ��Ҫmmm
    result = video_mmm_swf_init();
    if ( false == result )
    {
        print_err("video_mmm_init err");
        return false;
    }
       
    //��������
    print_dbg("playwin xywh:%d,%d,%d,%d", scene->x, scene->y, scene->width, scene->height);
    scene_swf->app_scene.win_id = gui_wm_create_window(scene->x, scene->y, scene->width, scene->height, \
                                        WM_CREATE_SHOW, _scene_swf_cb, 0);
    if( scene_swf->app_scene.win_id <= 0 )
    {
        print_err("gui_wm_create_window failed\n\n");
        return false;
    }
    
    //��ȡgui dc���
    scene_swf->app_scene.handle_gui_dc = gui_dc_get( (unsigned short)scene_swf->app_scene.win_id );
    if (scene_swf->app_scene.handle_gui_dc <= 0)
    {
        print_err("gui_dc_get fail\n\n");
        return false;
    }
    
    //���ý���
    gui_wm_set_focus( scene_swf->app_scene.win_id );
    
    //һ����loadstyle�ʹ�������֮��, ������Ļ����. �˳������κλ�ͼ����һ������֮��
    sty_set_screen_rotation( ROTATION_0 );
    //���ú�������, ���gui��buffer, ����֮ǰ�������в���
    gui_dc_clear( scene_swf->app_scene.handle_gui_dc );

    //ע��:������ʾ��Ӧ����������Ļ����֮����ʾ��������벥��ǰ����Ļ�����б仯��
    //������ʾλ�ò���ȷ�򱳾���ʾ����

    //��ȡ�ڴ��С
    s_cur_sdram = (unsigned int)get_sdram_cap();
    print_dbg("SDRAM size:%d\n",s_cur_sdram);
    //ֻ��ͬʱ����֧��swf�����ڴ����8M���ܽ���swf����
    if ((0 == video_vars.swf_support) || (s_cur_sdram <= 8))
    {
        video_dialog_modal( VIDEO_DIALOG_NOT_SUPPORT );
        return false;
    }

    
    //���벥��ǰ, ������
    result = _get_battery_voltage( &battery_vol );
    if ( true == result )
    {
        if ( battery_vol <= VOL_THRESHOLD_PLAY_VIDEO )
        {
            //û�н��벥�Ž���, ��Ҫ��ʾʱ, һ��������NON_YUV. ������
            video_dialog_modal( VIDEO_DIALOG_LOW_BAT );
            return false;
        }
    }

    result = video_nplist_setplay_swf_by_index( video_vars.plist.np_index );
    //������ļ���ʧ��, �����np. ���ܴ�np dirlist taglist����swf
    if ( false == result )
    {
        swf_control( V_CMD_STOP, NULL );
        //û�н��벥�Ž���, ��Ҫ��ʾʱ, һ��������NON_YUV. ������
        video_dialog_modal( VIDEO_DIALOG_OPEN_FAIL );
        //��ʧ�ܵ��ļ�, ͬʱ�����taglist, ����ɾ��tag�ļ���, �ٴ�taglist���벥�ŵ����
        video_nplist_clear_npinfo();
        
        return false;
    }
    video_vars.flag_has_np_file = 1;
    video_set_play_mode( video_vars.play_param.playmode );
    
    //ȫ����ʼ���ɹ�, �������timer, ��ʱget video����״̬ ��Ϣ
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

    /*������ڲ����б�û���ļ�������ʾ���ڲ����б�*/
    if( 0 == video_plist_get_item_num( video_vars.nplist ) )
    {
        video_vars.flag_has_np_file = false;
        print_dbg("no file in nplist\n");
    }

    
    //������ͳһstop
    swf_control( V_CMD_STOP, NULL );
    
    //��ȡ��ͼ�Ƕ�
    angle = sty_get_view_rotation();
    sty_set_screen_rotation( angle );
    //���ú�������, ���gui��buffer, ����֮ǰ�������в���
    gui_dc_clear( scene_swf->app_scene.handle_gui_dc );

    //kill timer
    if ( -1 != scene_swf->timer_id )
    {
        kill_timer( scene_swf->timer_id );
        scene_swf->timer_id = -1;
    }
    
    //ɾ������
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

    //�̰�, �򲥷���һ�����ߴ�ͷ���ű��ļ�
    case KEY_PREV | KEY_TYPE_SHORT_UP:
        
        s_playing_switch_mode = VIDEO_FORCE_PLAY_PREV;
        
        //��prev��������һ��; 
        index_cur = video_get_prev_play_index( LOOP_SWITCH_FORCELOOP );
        result = video_nplist_setplay_swf_by_index( index_cur );

        /*�����ܲ��ŵ��ļ���inde��¼������dialog��callback����*/
        /*����swf�ļ�����ʱ�����ж��Ƿ񲥷��꣬���⣬
        �ܲ��ܲ���ֻ�����м������ʱ�����жϣ�����Ҫ�ڴ˼�¼�����ļ���index��
        ����timer���յ��м��errorʱ��plist��ɾ������������һ���ļ�*/
        s_index_for_setplay_false = index_cur;

        /* �ļ���ʧ��, ��ʾ, ��plistɾ��ʧ���ļ�, ����������һ�� */
        if ( false == result )
        {
            swf_control( V_CMD_STOP, NULL );
            s_enter_timer = 0;
            video_dialog_create( VIDEO_DIALOG_OPEN_FAIL, _dialog_cb_setplay_false);
        }

        //clear key
        clear_key_msg_queue(-1,-1);
        break;
        
    //�Ƕ̰�, �򲥷���һ��
    case KEY_NEXT | KEY_TYPE_SHORT_UP:
        
        s_playing_switch_mode = VIDEO_FORCE_PLAY_NEXT;
        
        index_cur = video_get_next_play_index( LOOP_SWITCH_FORCELOOP );            
        result = video_nplist_setplay_swf_by_index( index_cur );

        /*�����ܲ��ŵ��ļ���inde��¼������dialog��callback����*/
        /*����swf�ļ�����ʱ�����ж��Ƿ񲥷��꣬���⣬
        �ܲ��ܲ���ֻ�����м������ʱ�����жϣ�����Ҫ�ڴ˼�¼�����ļ���index��
        ����timer���յ��м��errorʱ��plist��ɾ������������һ���ļ�*/
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
    //����swf�ļ����Ź����в��ܵ���ʾ����dialog����ˣ�����Ӧɾ������
    //ɾ���ļ���ݼ�    
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
*       �����ص�����, Ҳ�����ڻص�����. һ������ֻ��һ��ȫ������
*       ���䴰��cb��gui��Ϣ: ��ȡtop���ͼ��, ����Ϣ���͸�top.
*       menuview��scene�����²�
* \param[in]    
* \retval       
*   ע��: ���ؼ��� ����swf scene�Ĺ�ϵ
*       swfʹ�� canvas_get_blending_top��ȡtop
*******************************************************************************/
static void _scene_swf_cb(gui_wm_message_t *pmsg)
{ 
    /* �յ�down������ż���������, ����ֱ���˳� */
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
*       ������ں���. scene entry function
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
    //��ֹ����, ʡ��
    sys_forbid_screensaver( true );
    sys_forbid_auto_standby( true);
    
    /* ������ʼ�� */
    result = _scene_swf_init( scene_swf );
    if(false == result)
    {
        print_err("scene init error!\n");
        goto VIDEO_SCENE_SWF_ERR_INIT;
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

    next_scene = scene_swf->app_scene.scene_type;
    /* �����˳� */
    VIDEO_SCENE_SWF_ERR_INIT:
    //�ָ�����, ʡ��
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

