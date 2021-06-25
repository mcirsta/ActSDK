/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : usb_scene_charge.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *    zhaowenjie    2009-2-25          v1.0               build this file
 ********************************************************************************/
/*!
* \file     usb_scene_charge.c
* \brief    charge scene
* \author   zhaowenjie
*
* \version 1.0
* \date  2009/06/04
*******************************************************************************/

#include "usb.h"
#include "usb_scene_charge.h"
#include "usb_scene_charge_ui.h"

#define CHARGE_TIMER_INTERVAL 500

static scene_t* s_scene_resource = NULL;

static int s_window = 0;
static msgbox_t* s_msgbox = NULL;
static scene_charge_ui_t* s_charge_ui = NULL;

/* 是否需要充电画面 */
static bool s_need_charge_ui = true;

static int s_timer = NULL_TIMER;
static unsigned int s_timer_interval = CHARGE_TIMER_INTERVAL;

void enter_scene_charge(void);
static bool _scene_charge_init(void);
static void _scene_charge_deinit(void);

static bool _show_scene( scene_t* scene_resource );
static int _proc_timer( void * param );

static void _proc_gui_msg(gui_wm_message_t *pmsg);
static void _proc_key( key_event_t *msg );

static int _proc_sys_msg( msg_apps_t *msg );
static void _scene_rotate(void);

void set_need_charge_ui( bool need );

static bool _check_battery_exists( void );

static void _handle_charge_status( charge_status_t status );

static void _msgbox_callback(msgbox_t * msgbox, msgbox_sig_e signal );

static scene_charge_t s_scene_charge_api =
{
    enter_scene_charge,

    set_need_charge_ui
};

void enter_scene_charge(void)
{
    bool ret;
    msg_apps_t  msg;

    print_info(" ************   enter  scene charge  ************* ");

    if ( s_need_charge_ui == false )
    {
        g_usb_main->set_current_scene( SCENE_EXIT );
        return;
    }

    /* 场景初始化 */
    ret = _scene_charge_init();
    if( ret == false )
    {
        print_err("");
        g_usb_main->set_current_scene( SCENE_EXIT );

        goto ERROR_EXIT;
    }

    /* 场景消息循环 */
    register_sys_dispatcher( _proc_sys_msg );

    while( get_msg(&msg) == true)
    {
        dispatch_msg(&msg);
    }

    unregister_sys_dispatcher();

    ERROR_EXIT:

    /* 场景退出 */
    _scene_charge_deinit();


    print_info(" ************  exit  scene charge  ************* ");
}

/* scene initial function */
static bool _scene_charge_init(void)
{
    bool ret;
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

    s_scene_resource = s_charge_ui->load_style_resource( angle );
    if ( s_scene_resource == NULL )
    {
        print_err("");
        return false;
    }

    ret = _check_battery_exists();
    if ( ret == false )
    {
        print_err( "battery not exists ! \n" );
        return false;
    }

    fenter = FUSION_FORWARD;
    fusion_get_config( FUSION_TYPE_APP_CHANGE, fenter, &eff );
    fusion_set_effect(&eff);
    fusion_set_first_frame( NULL );
    
    ret = _show_scene( s_scene_resource );
    if ( ret == false)
    {
        print_err("");

        /* 释放fusion的内存，否则会内存泄漏 */
        fusion_display( NULL );
        return false;
    }

    s_timer = set_timer( s_timer_interval, _proc_timer, NULL );

    
    fusion_set_last_frame( NULL );
    fusion_display( NULL);

    clear_key_msg_queue( -1, -1 );

    return true;
}

static bool _show_scene( scene_t* scene_resource )
{
    int hdc;
    
    /* 创建窗口 */
    s_window = gui_wm_create_window( scene_resource->x, scene_resource->y, scene_resource->width,
                                    scene_resource->height, WM_CREATE_SHOW, _proc_gui_msg, 0 );

    gui_wm_set_focus( s_window );

    /* paint all */
    hdc = gui_dc_get( (unsigned short)s_window );
    s_charge_ui->set_charge_hdc( hdc );
    s_charge_ui->paint_charge_initial_ui();
    s_charge_ui->update_battery_status( CHARGE_CHARGING );

    s_msgbox = msgbox_create( _msgbox_callback );
    if ( s_msgbox == NULL )
    {
        print_err(" msgbox_create NULL .......");
        return false;
    }

    return true;
}

static int _proc_timer( void * param )
{
    msg_apps_t msg;
    msg_reply_t reply;
    charge_status_t status = CHARGE_CHARGING;

    memset( &msg, 0, sizeof( msg ) );
    memset( &reply, 0, sizeof( reply ) );
	
    msg.type = MSG_GET_CHARGE_STATUS_SYNC;
    send_sync_msg( APP_NAME_MSG_SERV, &msg, &reply, NULL);
	
    if ( reply.type == MSG_REPLY_FAILED )
    {
        print_err(" MSG_GET_CHARGE_STATUS_SYNC  failed ......  ");
    }
    else
    {
        memcpy( &status, reply.content, sizeof( charge_status_t ) );
    }
	
    print_dbg(" charget status ===== %d ", status );
	
    _handle_charge_status( status );
    
    s_charge_ui->update_battery_status( status );

    return 0;
}

static void _handle_charge_status( charge_status_t status )
{
    bool exit_app = false;

    switch( status )
    {
        case CHARGE_FULL :
        {
            string_resource_t* string;
            dialog_init_t dialog_init;

            if ( s_timer != NULL_TIMER )
            {
                kill_timer( s_timer );
                s_timer = NULL_TIMER;
            }

            string = ( string_resource_t* )g_style_api->get_scene_child( s_scene_resource, STR_CHARGE_FULL );
            dialog_init.head = DIALOG_HEAD_INFO;
            dialog_init.str_id = string->str_id;
            dialog_modal( &dialog_init );
            
            exit_app = true;
            break;
        }
    
        case CHARGE_STOP :
        {
            exit_app = true;
            break;
        }
    
        case CHARGE_FAILED_OVERCHARGE :
        {
            exit_app = true;
            break;
        }

        case CHARGE_FAILED_OVERLOAD :
        {
            exit_app = true;
            break;
        }

        case CHARGE_FAILED_BATTERY :
        {
            exit_app = true;
            break;
        }

        case CHARGE_NO_BATTERY :
        {
            string_resource_t* string;
            dialog_init_t dialog_init;

            if ( s_timer != NULL_TIMER )
            {
                kill_timer( s_timer );
                s_timer = NULL_TIMER;
            }
            
            string = ( string_resource_t* )g_style_api->get_scene_child( s_scene_resource, STR_BATTERY_CUT );
	
            dialog_init.head = DIALOG_HEAD_FAIL;
            dialog_init.str_id = string->str_id;
            dialog_modal( &dialog_init );
           
            exit_app = true;
            break;
        }

        case CHARGE_NO_DC5V :
        {
            exit_app = true;
            break;
        }

        default :
        {
            break;
        }
    }

    if ( exit_app == true )
    {    
        exit_msg_loop();
        g_usb_main->set_current_scene( SCENE_EXIT );
    }
}

static void _scene_charge_deinit(void)
{
    print_dbg("_scene_charge_deinit........");

    if ( s_timer != NULL_TIMER )
    {
        kill_timer( s_timer );
        s_timer = NULL_TIMER;
    }

    s_charge_ui->free_style_resource();

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
            exit_msg_loop();
            g_usb_main->set_current_scene( SCENE_EXIT );
            break;
        }
        
        default:
        {
            break;
        }
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

        case MSG_APP_QUIT:
        {
            print_dbg(" receive  MSG_APP_QUIT");

            g_usb_main->set_receive_quit_msg( true );

            exit_msg_loop();
            g_usb_main->set_current_scene( SCENE_EXIT );
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

static void _scene_rotate(void)
{
    rotation_e angle;
    fusion_effect_t effect;

    fusion_get_config( FUSION_TYPE_ROTATE, FUSION_FORWARD, &effect );
    fusion_set_effect( &effect );
    fusion_set_first_frame( NULL );
    
    angle = sty_get_view_rotation();
    sty_set_screen_rotation( angle );

    /* 更换资源 */
    s_scene_resource = s_charge_ui->load_style_resource( angle );

    gui_wm_set_size( s_window, s_scene_resource->width, s_scene_resource->height );

    g_usb_main->update_background_picture();
    s_charge_ui->paint_charge_initial_ui();

    fusion_set_last_frame( NULL );
    fusion_display( NULL );
}

static bool _check_battery_exists( void )
{
    msg_apps_t msg;
    msg_reply_t reply;
    charge_status_t status = CHARGE_CHARGING;

    memset( &msg, 0, sizeof( msg ) );
    memset( &reply, 0, sizeof( reply ) );
	
    msg.type = MSG_GET_CHARGE_STATUS_SYNC;
    send_sync_msg( APP_NAME_MSG_SERV, &msg, &reply, NULL);
	
    if ( reply.type == MSG_REPLY_FAILED )
    {
        return true;
    }

    memcpy( &status, reply.content, sizeof( charge_status_t ) );
    if ( status == CHARGE_NO_BATTERY )
    {
        string_resource_t* string;
        dialog_init_t dialog_init;
        
        string = ( string_resource_t* )g_style_api->get_scene_child( s_scene_resource, STR_NO_BATTERY );

        dialog_init.head = DIALOG_HEAD_FAIL;
        dialog_init.str_id = string->str_id;
        dialog_modal( &dialog_init );

        return false;
    }

    return true;
}

void set_need_charge_ui( bool need )
{
    s_need_charge_ui = need;
}

static void _msgbox_callback(msgbox_t * msgbox, msgbox_sig_e signal )
{    
}

scene_charge_t* create_scene_charge(void)
{
    s_charge_ui = create_scene_charge_ui();

    return &s_scene_charge_api;
}
