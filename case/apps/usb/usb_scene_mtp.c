/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : usb_scene_mtp.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *    zhaowenjie    2009-2-25          v1.0               build this file
 ********************************************************************************/
/*!
* \file     usb_scene_mtp.c
* \brief    udisk scene
* \author   zhaowenjie
*
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#include "usb.h"
#include "usb_scene_mtp.h"
#include "usb_scene_mtp_ui.h"
#include "usb_mtp_thread.h"

#define MTP_TIMER_INTERVAL 300

static mtp_vendor_info_t s_mtp_param 
#pragma __PRQA_IGNORE_START__
=
{
    .mtp_pid = MTP_PRODUCT_ID_FEFAULT,
    .mtp_vid = MTP_VENDOR_ID_FEFAULT,
    .mtp_ext = MTP_EXTERN_ID_FEFAULT,
    
    .dev_manufacturer = MTP_DEVICE_MANUFACTURER_DEFAULT,
    .mtp_dev_model = MTP_DEVINFO_MODEL_DEFAULT,
    .mtp_dev_version = MTP_DEVINFO_VERSION_DEFAULT,
    .mtp_dev_serial = MTP_DEVINFO_SERIAL_DEFAULT,
    
    .mtp_store_inner_desc = MTP_STORE_INTERNAL_DESC_DEFAULT,
    .mtp_store_portable_desc = MTP_STORE_PORTABLE_DESC_DEFAULT,

    .mtp_dev_ext_desc0 = MTP_STORE_EXT_DESC0_DEFAULT,
    .mtp_dev_ext_desc1 = MTP_STORE_EXT_DESC1_DEFAULT,
}
#pragma __PRQA_IGNORE_END__
;

static scene_mtp_ui_t* s_mtp_ui = NULL;
static mtp_thread_t* s_mtp_thread = NULL;

static int s_window = 0;
static msgbox_t* s_msgbox = NULL;

static int s_last_mtp_status = 0;

static int s_timer = NULL_TIMER;

void enter_scene_mtp(void);
static bool _scene_mtp_init(void);
static void _scene_mtp_deinit(void);
static bool _show_scene( scene_t* scene_resource );

static bool _mtp_install_driver(void);
static void _mtp_remove_driver(void);

static void _proc_gui_msg(gui_wm_message_t *pmsg);
static void _proc_key( key_event_t *msg );
static int _proc_timer( void * param );

static int _proc_sys_msg( msg_apps_t *msg );
static void _scene_rotate( void );
static mtp_status_e _get_mtp_status_frame( int mtp_status );

static void _set_charge_percent(void);

bool get_mtp_change( void );

void set_mtp_pid( int p );
void set_mtp_vid( int v );
void set_mtp_ext( int e );
void set_mtp_dev_manufacturer( char* string );
void set_mtp_dev_model( char* string );
void set_mtp_dev_version( char* string );
void set_mtp_dev_serial( char* string );
void set_mtp_store_inner_desc( char* string );
void set_mtp_store_portable_desc( char* string );
void set_mtp_dev_ext_desc0( char* string );
void set_mtp_dev_ext_desc1( char* string );


static void _msgbox_callback(msgbox_t * msgbox, msgbox_sig_e signal );

static scene_mtp_t s_scene_mtp_api =
{
    set_mtp_pid,
    set_mtp_vid,
    set_mtp_ext,
    
    set_mtp_dev_manufacturer,
    set_mtp_dev_model,
    set_mtp_dev_version,
    set_mtp_dev_serial,

    set_mtp_store_inner_desc,
    set_mtp_store_portable_desc,

    set_mtp_dev_ext_desc0,
    set_mtp_dev_ext_desc1,
    

    enter_scene_mtp,

    get_mtp_change
};

void enter_scene_mtp(void)
{
    bool ret;
    msg_apps_t  msg;

    print_info(" ************   enter  scene_mtp  ************* ");

    /* 场景初始化 */
    ret = _scene_mtp_init();
    if( ret == false )
    {
        print_err("mtp scene init error! \n");
        g_usb_main->set_current_scene( SCENE_EXIT );

        goto ERROR_EXIT;
    }

    /* 场景消息循环 */
    register_sys_dispatcher( _proc_sys_msg );

    while( get_msg(&msg) == true )
    {
        dispatch_msg(&msg);
    }

    unregister_sys_dispatcher();

    ERROR_EXIT:

    /* 场景退出 */
    _scene_mtp_deinit();

    print_info(" ************  exit  scene_mtp  ************* ");
}

/* scene initial function */
static bool _scene_mtp_init(void)
{
    bool ret;
    scene_t* scene_resource;
    rotation_e angle;
    fusion_enter_e fenter;
    fusion_effect_t eff;

    angle = sty_get_view_rotation();
    ret = sty_set_screen_rotation( angle );
    if ( ret == false )
    {
        print_err( "sty_set_screen_rotation failure \n" );
        return false;
    }

    scene_resource = s_mtp_ui->load_style_resource( angle );
    if ( scene_resource == NULL )
    {
        print_err("");
        return false;
    }

    fenter = FUSION_FORWARD;
    fusion_get_config( FUSION_TYPE_APP_CHANGE, fenter, &eff );
    fusion_set_effect( &eff );
    fusion_set_first_frame( NULL );
    
    ret = _show_scene( scene_resource );
    if ( ret == false)
    {
        print_err("");

        /* 释放fusion的内存，否则会内存泄漏 */
        fusion_display( NULL);
        return false;
    }

    ret = _mtp_install_driver();
    if ( ret == false)
    {
        print_err("");

        /* 释放fusion的内存，否则会内存泄漏 */
        fusion_display( NULL );
        return false;
    }

    fusion_set_last_frame( NULL );
    fusion_display( NULL );

    s_mtp_thread->start_mtp_thread();

    /* 要等 mtp 运行起来后才查状态 */
    while ( s_mtp_thread->is_running() == false )
    {
        usleep( MTP_TIMER_INTERVAL * 1000 );
    }

    s_timer = set_timer( MTP_TIMER_INTERVAL, _proc_timer, NULL );

    clear_key_msg_queue( -1, -1 );

    return true;
}

static bool _show_scene( scene_t* scene_resource )
{
    bool ret = true;
    int hdc;

    ret = sty_set_screen_rotation( sty_get_view_rotation() );
    if ( ret == false )
    {
        print_err( "sty_set_screen_rotation failure \n" );
        return false;
    }

    /* 创建窗口 */
    s_window = gui_wm_create_window( scene_resource->x, scene_resource->y, scene_resource->width,
                                    scene_resource->height, WM_CREATE_SHOW, _proc_gui_msg, 0 );

    gui_wm_set_focus( s_window );

    /* paint all */
    hdc = gui_dc_get( (unsigned short)s_window );

    s_mtp_ui->set_mtp_hdc( hdc );
    s_mtp_ui->paint_mtp_initial_ui();

    s_msgbox = msgbox_create( _msgbox_callback );
    if ( s_msgbox == NULL )
    {
        print_err(" msgbox_create NULL .......");
        return false;
    }

    return true;
}


static void _proc_gui_msg(gui_wm_message_t *pmsg)
{
    int top;
    key_event_t *kmsg;

    top = canvas_get_focus(pmsg);

    if ( ( s_msgbox != NULL ) 
            && ( s_msgbox->canvas_id == top ) )
    {
        msgbox_proc_gui( s_msgbox, pmsg );
        return;
    }

    if ( pmsg->msgid == WM_MSG_KEY )
    {
        kmsg = (key_event_t *)(pmsg->data.p); /*get key msg*/
        _proc_key( kmsg );
    }
    
}

static void _proc_key( key_event_t *msg )
{
    key_event_t *kmsg;
    kmsg = msg;
    switch ( kmsg->val | kmsg->type )
    {
        case KEY_RETURN | KEY_TYPE_SHORT_UP:
        {
            s_mtp_thread->stop_mtp_thread();
            break;
        }

        default:
        {
            break;
        }
    }
}

static bool _mtp_install_driver(void)
{
    int ret;

    print_dbg(" installing mtp drivers........ ");

    ret = insmod( AMTP_DRIVER, 0 );
    if ( ret < 0 )
    {
        return false;
    }

    ret = insmod( UDC_DRIVER, 0 );
    if ( ret < 0 )
    {
        return false;
    }

    ret = insmod( TUNNEL_DRIVER, (void*)&s_mtp_param );
    if ( ret < 0 )
    {
        return false;
    }

    print_dbg(" mtp drivers install succeed  ........ ");

    return true;
}

static void _mtp_remove_driver(void)
{
    print_dbg(" removing mtp drivers........ ");

    print_dbg(" removing TUNNEL_DRIVER driver........ ");
    rmmod( TUNNEL_DRIVER, 0);

    print_dbg(" removing UDC_DRIVER driver........ ");
    rmmod( UDC_DRIVER, 0);
    
    print_dbg(" removing AMTP_DRIVER driver........ ");
    rmmod( AMTP_DRIVER, 0);

    print_dbg(" mtp drivers are removed ! ");
}

static int _proc_timer( void * param )
{
    int mtp_status;
    mtp_status_e mtp_status_frame = MTP_STATUS_IDLE;

    mtp_status = s_mtp_thread->get_mtp_status();
    if ( mtp_status < 0 )
    {
        /* 收到退出状态才表示 MTP 退出。 */
        exit_msg_loop();
        g_usb_main->set_current_scene( SCENE_EXIT );
        return 0;
    }

    _set_charge_percent();

    if ( mtp_status == s_last_mtp_status )
    {
        return 0;
    }
    s_last_mtp_status = mtp_status;

    mtp_status_frame = _get_mtp_status_frame( mtp_status );

    s_mtp_ui->paint_mtp_status_frame( mtp_status_frame );

    return 0;
}

static void _set_charge_percent(void)
{
    msg_apps_t      msg;
    msg_reply_t     reply = {0};
    hw_status_t hw_status = {0};
    struct timespec timeout;
    bool result = true;

    timeout.tv_sec = 1;
    timeout.tv_nsec = 0;

    memset( &msg, 0, sizeof( msg ) );
    msg.type = MSG_GET_HW_STATUS_SYNC;
    result = send_sync_msg( APP_NAME_MSG_SERV, &msg, &reply, &timeout);
    if (false == result)
    {
        print_warning("send_sync_msg failed");
        return;
    }

    memcpy( &hw_status, reply.content, sizeof(hw_status_t) );

    print_dbg(" hw_status.voltage = %d ", hw_status.voltage );
}

static void _scene_mtp_deinit(void)
{
    if ( s_timer != NULL_TIMER )
    {
        kill_timer( s_timer);
        s_timer = NULL_TIMER;
    }

    s_mtp_ui->free_style_resource();

    _mtp_remove_driver();

    if ( s_msgbox != NULL )
    {
        msgbox_delete( s_msgbox );
        s_msgbox = NULL;
    }

    /* 删除窗体 */
    if ( s_window != 0 )
    {
        gui_wm_delete_window( s_window );
    }
}

static int _proc_sys_msg( msg_apps_t *msg )
{
    msg_reply_t t;
    switch(msg->type)
    {
        case MSG_ACTIVE_SYNC:
        {
            /* 回复 process manager 应用创建成功。 */
            t.type = MSG_REPLY_SUCCESS;
            strncpy( t.content, USB_APP ,MSG_CONTENT_SIZE );
            reply_sync_msg(&t);
            break;
        }

        case MSG_GSENSOR_ROTATION_CHANGED:
        {
            print_dbg(" receive  MSG_GSENSOR_ROTATION_CHANGED");

            _scene_rotate();
            break;
        }

//	        case MSG_APP_QUIT:
//	        {
//	            print_dbg(" receive  MSG_APP_QUIT");
//	
//	            g_usb_main->set_receive_quit_msg( true );
//	
//	            exit_msg_loop();
//	            g_usb_main->set_current_scene( SCENE_EXIT );
//	            break;
//	        }

        //USB连接状态下不进入tvin定时录像
        case MSG_TVIN_ALARM_IN:
        {
            break;
        }
            
        default :
        {
            if ( s_msgbox != NULL )
            {
                msgbox_proc_sys( s_msgbox, msg );
            }
        }
    }
    return 0;
}

static void _scene_rotate( void )
{
    rotation_e angle;
    scene_t* scene_resource;
    mtp_status_e mtp_status_frame = MTP_STATUS_IDLE;
    fusion_effect_t effect;

    fusion_get_config( FUSION_TYPE_ROTATE, FUSION_FORWARD, &effect );
    fusion_set_effect( &effect );
    fusion_set_first_frame( NULL );
    
    angle = sty_get_view_rotation();
    sty_set_screen_rotation( angle );

    /* 更换资源 */
    scene_resource = s_mtp_ui->load_style_resource( angle );

    gui_wm_set_size( s_window, scene_resource->width, scene_resource->height );

    g_usb_main->update_background_picture();
    s_mtp_ui->paint_mtp_initial_ui();

    mtp_status_frame = _get_mtp_status_frame( s_last_mtp_status );
    
    s_mtp_ui->paint_mtp_status_frame( mtp_status_frame );

    fusion_set_last_frame( NULL );
    fusion_display( NULL );
}

static mtp_status_e _get_mtp_status_frame( int mtp_status )
{
    mtp_status_e mtp_status_frame = MTP_STATUS_IDLE;

    switch ( mtp_status )
    {
        case MTP_DAEMON_RUNNING_STATUS_WAITING_COMMAND :
        {
            mtp_status_frame = MTP_STATUS_IDLE;
            break;
        }

        case MTP_DAEMON_RUNNING_STATUS_DO_COMMMAND :
        {
            mtp_status_frame = MTP_STATUS_BUSY;
            break;
        }

        case MTP_DAEMON_RUNNING_STATUS_SEND_OBJECT :
        {
            mtp_status_frame = MTP_STATUS_BUSY;
            break;
        }

        case MTP_DAEMON_RUNNING_STATUS_GET_OBJECT :
        {
            mtp_status_frame = MTP_STATUS_BUSY;
            break;
        }

        default :
        {
            mtp_status_frame = MTP_STATUS_IDLE;
            break;
        }
    }

    return mtp_status_frame;
}

bool get_mtp_change( void )
{
    if ( s_mtp_thread->get_mtp_change_count() > 0 )
    {
        return true;
    }

    return false;
}





void set_mtp_pid( int p )
{
    s_mtp_param.mtp_pid = (unsigned int)p;
}

void set_mtp_vid( int v )
{
    s_mtp_param.mtp_vid = (unsigned int)v;
}

void set_mtp_ext( int e )
{
    s_mtp_param.mtp_ext = (unsigned int)e;
}


void set_mtp_dev_manufacturer( char* string )
{
    memcpy( &s_mtp_param.dev_manufacturer, string, 60 );
}

void set_mtp_dev_model( char* string )
{
    memcpy( &s_mtp_param.mtp_dev_model, string, 60 );
}

void set_mtp_dev_version( char* string )
{
    memcpy( &s_mtp_param.mtp_dev_version, string, 60 );
}

void set_mtp_dev_serial( char* string )
{
    memcpy( &s_mtp_param.mtp_dev_serial, string, 60 );
}


void set_mtp_store_inner_desc( char* string )
{
    memcpy( &s_mtp_param.mtp_store_inner_desc, string, 40 );
}

void set_mtp_store_portable_desc( char* string )
{
    memcpy( &s_mtp_param.mtp_store_portable_desc, string, 40 );
}


void set_mtp_dev_ext_desc0( char* string )
{
    memcpy( &s_mtp_param.mtp_dev_ext_desc0, string, 60 );
}

void set_mtp_dev_ext_desc1( char* string )
{
    memcpy( &s_mtp_param.mtp_dev_ext_desc1, string, 60 );
}





static void _msgbox_callback(msgbox_t * msgbox, msgbox_sig_e signal )
{    
}

scene_mtp_t* create_scene_mtp(void)
{
    s_mtp_ui = create_scene_mtp_ui();
    s_mtp_thread = create_mtp_thread();

    return &s_scene_mtp_api;
}


