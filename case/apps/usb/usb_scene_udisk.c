/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : usb_scene_udisk.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *    zhaowenjie    2009-2-25          v1.0               build this file
 ********************************************************************************/
/*!
* \file     usb_scene_udisk.c
* \brief    udisk scene
* \author   zhaowenjie
*
* \version 1.0
* \date  2009/02/25 
*******************************************************************************/

#include "usb.h"
#include "usb_scene_udisk.h"
#include "usb_scene_udisk_ui.h"

#define UDISK_TIMER_INTERVAL 300


static udisk_info_t s_a_disk 
#pragma __PRQA_IGNORE_START__
=
{
    .udev_name = DEVICE_FLASH "a",
    .medium_type = NANDDISK_DEF,
    .udev_protocol = USB_SC_8070, // 分区状态。
    .udev_fstype = FS_FAT32,   // 分区格式类型。
    .lun_index = 2,
    .removable = 0,
    .readonly = 0,
    .can_stall = 1,
    .enabled = 1,
}
#pragma __PRQA_IGNORE_END__
;

static udisk_info_t s_u_disk 
#pragma __PRQA_IGNORE_START__
=
{
    .udev_name = DEVICE_FLASH "u",
    .medium_type = NANDDISK_DEF,
    .udev_protocol = USB_SC_8070, // 分区状态。
    .udev_fstype = FS_FAT16,   // 分区格式类型。
    .lun_index = 0,
    .removable = 0,
    .readonly = 0,
    .can_stall = 1,
    .enabled = 1,
}
#pragma __PRQA_IGNORE_END__
;

static udisk_info_t s_card_disk 
#pragma __PRQA_IGNORE_START__
=
{
    .udev_name = DEVICE_CARD,
    .medium_type = CARDDISK_DEF,
    .udev_protocol = USB_SC_8070, // 分区状态。
    .udev_fstype = FS_FAT32,   // 分区格式类型。
    .lun_index = 1,
    .removable = 0,
    .readonly = 0,
    .can_stall = 1,
    .enabled = 0,
}
#pragma __PRQA_IGNORE_END__
;


static struct udisk_param  s_usb_udisk_param 
#pragma __PRQA_IGNORE_START__
=
{
    /*1100
      .nluns =1,
      .disklist[0] ={
      .Unit =1,       // 分区号。
      .UnitStatus = 16, // 分区状态。
      .FatType = 4,    // 分区格式类型。
      .StartAddr = 0,  // 分区起始地址.
      .Length =800000,     // 分区总长度.
      },
      .rolist={0},
      */
    .task_priority = 2,
    .nluns = 0,
    .VID = USB_VENDOR_ID,
    .PID = MSC_PRODUCT_ID,
    .release = MSC_PRODUCT_RELEASE,
    .usb_vendor = "actions",
    .usb_product = "media-player",
    .serial = "1234567887654321",
//	    .disklist[0] =
//	    {
//	        .udev_name = DEVICE_FLASH "u",
//	        .medium_type = NANDDISK_DEF,
//	        .udev_protocol = USB_SC_8070, // 分区状态。
//	        .udev_fstype = FS_FAT16,   // 分区格式类型。
//	        .lun_index = 0,
//	        .removable = 0,
//	        .readonly = 0,
//	        .can_stall = 1,
//	        .enabled = 1,
//	    },
//	
//	    .disklist[1] =
//	    {
//	        .udev_name = DEVICE_CARD,
//	        .medium_type = CARDDISK_DEF,
//	        .udev_protocol = USB_SC_8070, // 分区状态。
//	        .udev_fstype = FS_FAT32,   // 分区格式类型。
//	        .lun_index = 1,
//	        .removable = 0,
//	        .readonly = 0,
//	        .can_stall = 1,
//	        .enabled = 0,           /* 设备的介质是否已经存在 */
//	    },

}
#pragma __PRQA_IGNORE_END__
;

static int s_msc_device_fd = WRONG_DEVICE_FD;

static scene_udisk_ui_t* s_udisk_ui = NULL;

static int s_window = 0;
static msgbox_t* s_msgbox = NULL;

static int s_timer = NULL_TIMER;

static bool s_show_manufacturer_disk = false;
static bool s_show_u_disk = false;
static bool s_show_card_disk = false;

static int s_last_udisk_status = -1;

static bool s_stop_rotate = false;

static bool s_disk_has_change = false;

static bool s_happen_card_insert = false;

void enter_scene_udisk(void);
static bool _scene_udisk_init(void);
static void _scene_udisk_deinit(void);
static bool _show_scene( scene_t* scene_resource );

static bool _udisk_install_driver(void);
static void _udisk_remove_driver(void);

static void _proc_gui_msg(gui_wm_message_t *pmsg);
//	static void _proc_key( key_event_t *msg );
static int _proc_timer( void * param );
static void _check_usb_line(void);

static int _proc_sys_msg( msg_apps_t *msg );
static void _scene_rotate( void );

static udisk_status_e _get_udisk_status_frame( UDISK_MAIN_STATUS_T main_status, UDISK_SUB_STATUS_T sub_status );

static void _set_show_disks( void );
static void _try_mount_card(void);

void set_show_manufacturer_disk(void);
void set_show_u_disk(void);
void set_show_card_disk(void);
static void _stop_charge( bool stop_charge );

static void _check_disk_change(void);
bool get_disk_change( void );

bool check_card_insert(void);


void set_pid( int p );
void set_vid( int v );
void set_release( int r );
void set_vendor( char* string );
void set_product( char* string );
void set_serial( char* string );



static void _msgbox_callback(msgbox_t * msgbox, msgbox_sig_e signal );

static scene_udisk_t s_scene_udisk_api =
{
    set_pid,
    set_vid,
    set_release,
    set_vendor,
    set_product,
    set_serial,
    
    enter_scene_udisk,

    set_show_manufacturer_disk,
    set_show_u_disk,
    set_show_card_disk,

    get_disk_change,
    check_card_insert
};

void enter_scene_udisk(void)
{
    bool ret;
    msg_apps_t  msg;

    print_info(" ************   enter  scene_udisk  ************* ");

    /* 场景初始化 */
    ret = _scene_udisk_init();
    if( ret == false )
    {
        print_err("mainmenu scene init error! \n");
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
    _scene_udisk_deinit();


    print_info(" ************  exit  scene_udisk  ************* ");
}

/* scene initial function */
static bool _scene_udisk_init(void)
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

    scene_resource = s_udisk_ui->load_style_resource( angle );
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

    ret = _udisk_install_driver();
    if ( ret == false)
    {
        print_err("");

        /* 释放fusion的内存，否则会内存泄漏 */
        fusion_display( NULL);
        return false;
    }

    s_timer = set_timer( UDISK_TIMER_INTERVAL, _proc_timer, NULL );

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
    s_udisk_ui->set_udisk_hdc( hdc );
   
    s_udisk_ui->paint_udisk_initial_ui();

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
//	    key_event_t *kmsg;

    top = canvas_get_focus(pmsg);

    if ( ( s_msgbox != NULL ) 
            && ( s_msgbox->canvas_id == top ) )
    {
        msgbox_proc_gui( s_msgbox, pmsg );
    }
//	    else if ( pmsg->msgid == WM_MSG_KEY )
//	    {
//	        kmsg = (key_event_t *)(pmsg->data.p); /*get key msg*/
//	        _proc_key( kmsg );
//	    }
//	    else
//	    {
//	    }
}

//	static void _proc_key( key_event_t *msg )
//	{
//	    key_event_t *kmsg;
//	    kmsg = msg;
//	    switch ( kmsg->val | kmsg->type )
//	    {
//	        case KEY_RETURN | KEY_TYPE_SHORT_UP:
//	        {
//	            exit_msg_loop();
//	            g_usb_main->set_current_scene( SCENE_EXIT );
//	            break;
//	        }
//	
//	        default:
//	        {
//	            break;
//	        }
//	    }
//	}

static bool _udisk_install_driver(void)
{
    int ret;

    _check_usb_line();

    print_dbg(" installing udisk drivers........ ");

    ret = insmod( UDC_DRIVER, 0 );
    if ( ret < 0 )
    {
        print_err(" udc dirver installed failed ........ ");
        return false;
    }
    print_dbg(" udc dirver was installed ........ ");

    _set_show_disks();

    ret = insmod( MSC_DRIVER, &s_usb_udisk_param );
    if ( ret < 0 )
    {
        print_err(" msc dirver installed failed ........ ");
        return false;
    }
    print_dbg(" msc dirver was installed ........ ");

    /* 打开设备 */
    s_msc_device_fd = open( DEVICE_MSC, O_RDWR, 0);
    if ( s_msc_device_fd == WRONG_DEVICE_FD )
    {
        print_dbg(" open udisk_device_fd == %d", s_msc_device_fd);
        return false;
    }
    print_info(" open udisk_device_fd == %d", s_msc_device_fd );

    print_dbg(" installing udisk drivers succeed ........ ");

    _check_usb_line();

    return true;
}

static void _udisk_remove_driver(void)
{
    print_dbg(" removing udisk drivers........ ");

    if ( s_msc_device_fd != WRONG_DEVICE_FD )
    {
        close( s_msc_device_fd );
    }

    rmmod( MSC_DRIVER, 0);
    
    rmmod( UDC_DRIVER, 0);
       
    print_dbg(" udisk drivers are removed ! ");
}


static int _proc_timer( void * param )
{
    int ret;
    volatile int usb_status = 0;
    UDISK_MAIN_STATUS_T main_status;
    UDISK_SUB_STATUS_T sub_status = 0;

    bool stop_charge = false;

    udisk_status_e udisk_status = UDISK_STATUS_IDLE;

    /* 查询 usb 的状态 */
    ret = ioctl( s_msc_device_fd, MSC_IOCTL_GET_STATUS, (int)&usb_status );
    if ( ret < 0 )
    {
        print_err(" ioctl error ! ");
        return 0;
    }

//      print_dbg(" read status = 0x%x ", usb_status );

    if ( usb_status == 0 )
    {
        /* 未知状态 */
        volatile int status = 0;
        int monitor_device_fd = WRONG_DEVICE_FD;
        
        monitor_device_fd = open( DEVICE_UMONITOR, O_RDWR, 0);
        if ( monitor_device_fd == WRONG_DEVICE_FD )
        {
            print_err(" open monitor_device_fd == %d", monitor_device_fd );
            goto ERROR;
        }
        
        ret = ioctl( monitor_device_fd, UOC_IOCTL_CHK_BSESS_VLD, (int)&status );
        if ( ret < 0 )
        {
            print_err(" ioctl error ! ");

            close( monitor_device_fd );
            goto ERROR;
        }

        close( monitor_device_fd );

        if ( status == 1 )
        {
            /* 线还在 */
            return 0;            
        }

        /* 线被拔掉 */
        ERROR :

//        close( uoc_device_fd );
        exit_msg_loop();
        g_usb_main->set_current_scene( SCENE_EXIT );
        return 0;
    }

    if ( usb_status == s_last_udisk_status )
    {
        return 0;
    }
    s_last_udisk_status = usb_status;

    main_status = usb_status & 0x07;

    #pragma __PRQA_IGNORE_START__
    sub_status  = ( usb_status >> 4 ) & 0x0F;
    
//      print_dbg("status = 0x%x ,   main = %d, sub = %d", usb_status, main_status, sub_status );

    if ( sub_status == UDISK_S_STATUS_DISCON ) /* 拔线 */
    {
        print_dbg(" usb line was removed ! ");

        exit_msg_loop();
        g_usb_main->set_current_scene( SCENE_EXIT );
    }

    #pragma __PRQA_IGNORE_END__

    udisk_status = _get_udisk_status_frame( main_status, sub_status );

    if ( udisk_status == UDISK_STATUS_UPGRADE )
    {
        s_udisk_ui->paint_upgrade_strings();
        /* 升级时不能读取flash上的东西，故此时不能做旋转 */
        s_stop_rotate = true;

        stop_charge = true;
    }
    else
    {
        s_udisk_ui->paint_status_frame( udisk_status );

        if ( udisk_status == UDISK_STATUS_BUSY )
        {
            stop_charge = true;
        }
        else /* idle */
        {
            stop_charge = false;
        }
    }

    _stop_charge( stop_charge );
    
    return 0;
}

static void _check_usb_line(void)
{
    bool result = true;
    hw_status_t     hw_status;
    struct timespec timeout;
    msg_apps_t      msg;
    msg_reply_t     reply;
    
    memset( &hw_status, 0x00, sizeof(hw_status) );
    
    timeout.tv_sec = 1;
    timeout.tv_nsec = 0;

    msg.type = MSG_GET_HW_STATUS_SYNC;
    result = send_sync_msg(APP_NAME_MSG_SERV, &msg, &reply, &timeout);
    if ( false == result )
    {
        print_warning("send_sync_msg failed");
        return;
    }
    memcpy(&hw_status, reply.content, sizeof(hw_status_t));

    if ( PLUG_OUT == hw_status.usb )
    {
        print_dbg(" usb line absent ! ");
    
        exit_msg_loop();
        g_usb_main->set_current_scene( SCENE_EXIT );
    }
}


static void _scene_udisk_deinit(void)
{
    if ( s_timer != NULL_TIMER )
    {
        kill_timer( s_timer);
        s_timer = NULL_TIMER;
    }

    s_udisk_ui->free_style_resource();

    _check_disk_change();

    _udisk_remove_driver();

    _try_mount_card();

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

    /* delete actplaylist.db for test */
//      remove("/mnt/disk0/actplaylist.db");
//      remove("/mnt/diska/fsel.db");
}

static int _proc_sys_msg( msg_apps_t *msg )
{
    msg_reply_t t;
    int ret;
    
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

        case MSG_USB_B_OUT:
        {
            print_dbg(" receive  MSG_USB_B_OUT");

            if ( s_msgbox != NULL )
            {
                msgbox_proc_sys( s_msgbox, msg );
            }

            exit_msg_loop();
            g_usb_main->set_current_scene( SCENE_EXIT );

            break;
        }

        case MSG_CARD_OUT:
        {
            udisk_info_t card_info;

            if ( s_msgbox != NULL )
            {
                msgbox_proc_sys( s_msgbox, msg );
            }
            
            memset( &card_info, 0, sizeof( udisk_info_t ) );
            
            card_info.lun_index = s_card_disk.lun_index;
            print_dbg(" receive  MSG_CARD_OUT ;;;; card_info.lun_index = %d ", card_info.lun_index );
            
            /* 卸载 card 盘 */
            ret = ioctl( s_msc_device_fd, MSC_IOCTL_SET_DISK, (int)&card_info );
            if ( ret < 0 )
            {
                print_err(" ioctl error ! ");
                return 0;
            }            

            s_happen_card_insert = false;
            
            break;
        }

        case MSG_CARD_IN:
        {
            print_dbg(" receive  MSG_CARD_IN lun_index = %d ", s_card_disk.lun_index );

            if ( s_msgbox != NULL )
            {
                msgbox_proc_sys( s_msgbox, msg );
            }

            /* 卡插入时，msg_server 都会执行 mount(card) 操作。 */
            if( sys_umount(CARD_ROOT,0) < 0 ) 
            {
                print_err("unmount %s failed!", CARD_ROOT);
            }

            s_card_disk.enabled = 1;
            
            /* 重新挂载 card 盘 */
            ret = ioctl( s_msc_device_fd, MSC_IOCTL_SET_DISK, (int)&s_card_disk );
            if ( ret < 0 )
            {
                print_err(" ioctl error ! ");
                return 0;
            }         

            s_happen_card_insert = true;
            
            break;
        }
        
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
    UDISK_MAIN_STATUS_T main_status;
    UDISK_SUB_STATUS_T sub_status = 0;
    udisk_status_e udisk_status;
    fusion_effect_t effect;

    if ( s_stop_rotate == true )
    {
        return;
    }

    fusion_get_config( FUSION_TYPE_ROTATE, FUSION_FORWARD, &effect );
    fusion_set_effect( &effect );
    fusion_set_first_frame( NULL );
    
    angle = sty_get_view_rotation();
    sty_set_screen_rotation( angle );

    /* 更换资源 */
    scene_resource = s_udisk_ui->load_style_resource( angle );

    gui_wm_set_size( s_window, scene_resource->width, scene_resource->height );

    g_usb_main->update_background_picture();
    s_udisk_ui->paint_udisk_initial_ui();

    main_status = s_last_udisk_status & 0x07;

    #pragma __PRQA_IGNORE_START__
    sub_status  = ( s_last_udisk_status >> 4 ) & 0x0F;
    #pragma __PRQA_IGNORE_END__

    udisk_status = _get_udisk_status_frame( main_status, sub_status );
    s_udisk_ui->paint_status_frame( udisk_status );

    fusion_set_last_frame( NULL );
    fusion_display( NULL );
}


static void _set_show_disks( void )
{
    int luns;
    luns = 0; /* u 盘是一定挂载的 */

    if ( s_show_manufacturer_disk == true )
    {        
        memset( &(s_usb_udisk_param.disklist[ luns ]), 0, sizeof( udisk_info_t ) );
        memcpy( &(s_usb_udisk_param.disklist[ luns ]), &s_a_disk, sizeof( udisk_info_t ) );

        s_a_disk.lun_index = (unsigned int)luns;
        luns++;
    }

    if ( s_show_u_disk == true )
    {        
        memset( &(s_usb_udisk_param.disklist[ luns ]), 0, sizeof( udisk_info_t ) );
        memcpy( &(s_usb_udisk_param.disklist[ luns ]), &s_u_disk, sizeof( udisk_info_t ) );

        s_u_disk.lun_index = (unsigned int)luns;
        luns++;
    }

    if ( s_show_card_disk == true )
    {   
        hotplugin_dev_info_t card_info;

        memset( &(s_usb_udisk_param.disklist[ luns ]), 0, sizeof( udisk_info_t ) );
        memcpy( &(s_usb_udisk_param.disklist[ luns ]), &s_card_disk, sizeof( udisk_info_t ) );
        
        sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_CARD, &card_info);
        if( card_info.insert == HOTPLUGIN_DEV_READY )
        {              
            /* 设备已插入且可用，可以挂载到PC */
            s_usb_udisk_param.disklist[ luns ].enabled = 1;

            print_info( "sys_umount(CARD_ROOT,0) .......\n" );
            if( sys_umount(CARD_ROOT,0) < 0 ) 
            {
                print_err("unmount %s failed!", CARD_ROOT);
            }
        }

        s_card_disk.lun_index = (unsigned int)luns;
        luns++;
    }

    s_usb_udisk_param.nluns = luns;
}

static udisk_status_e _get_udisk_status_frame( UDISK_MAIN_STATUS_T main_status, UDISK_SUB_STATUS_T sub_status )
{
    udisk_status_e udisk_status = UDISK_STATUS_IDLE;

    if ( main_status == UDISK_M_STATUS_ALLOW_ACCESS )
    {
        if ( sub_status == UDISK_S_STATUS_IDLE )
        {
            udisk_status = UDISK_STATUS_IDLE;
        }
        else if ( sub_status == UDISK_S_STATUS_READ )
        {
            udisk_status = UDISK_STATUS_BUSY;
        }
        else if ( sub_status == UDISK_S_STATUS_WRITE )
        {
            udisk_status = UDISK_STATUS_BUSY;
        }
        else if ( sub_status == UDISK_S_STATUS_UPDATE )
        {
            udisk_status = UDISK_STATUS_UPGRADE;
        }
        else
        {
        }
    }

    return udisk_status;
}

static void _stop_charge( bool stop_charge )
{
    msg_apps_t msg;
    int start_charge;

    print_dbg( " send msg_server.app stop_charge  == %d",  stop_charge  );

    if ( stop_charge == true )
    {
        start_charge = 0;
    }
    else
    {
        start_charge = 1;
    }

    memset( &msg, 0, sizeof( msg ) );
    msg.type = MSG_SET_CHARGE_START_SYNC;
//      sprintf( msg.content, "%d", start_charge );
    memcpy( msg.content, &start_charge, sizeof( start_charge ) );
    send_sync_msg( APP_NAME_MSG_SERV, &msg, NULL, NULL );
}

static void _check_disk_change(void)
{
    volatile int update = 0;
    int ret;

    ret = ioctl( s_msc_device_fd, MSC_IOCTL_WRITE_HAPPENED, (int)&update );
    if ( ret < 0 )
    {
        print_err(" ioctl error ! ");
        return;
    }

    print_dbg("  MSC_IOCTL_WRITE_HAPPENED == %d ", update );

    if ( update == 1 )
    {
        s_disk_has_change = true;
    }
}

bool get_disk_change( void )
{
    return s_disk_has_change;
}

static void _try_mount_card(void)
{
    if ( s_show_card_disk == true )
    {   
        hotplugin_dev_info_t card_info;
        sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_CARD, &card_info);
        if( card_info.insert == HOTPLUGIN_DEV_READY )
        {              
            if( sys_mount( DEVICE_CARD, CARD_ROOT, "fat", 0, NULL) < 0 ) 
            {
                print_err("mount %s to %s failed!", DEVICE_CARD, CARD_ROOT);
            }
        }
    
    }
}

bool check_card_insert(void)
{
    if ( s_happen_card_insert == true )
    {
        hotplugin_dev_type_e main_disk ;

        main_disk = sys_get_main_disk();
        if ( main_disk == HOTPLUGIN_DEV_TYPE_CARD )
        {
            /* 有发生过插入卡，且卡是主盘( nor flash 方案 ) */
            return true;
        }
    }

    return false;
}

void set_pid( int p )
{
    s_usb_udisk_param.PID = (unsigned short)p;
}

void set_vid( int v )
{
    s_usb_udisk_param.VID = (unsigned short)v;
}

void set_release( int r )
{
    s_usb_udisk_param.release = (unsigned short)r;
}

void set_vendor( char* string )
{
    memcpy( &s_usb_udisk_param.usb_vendor, string, 8 );
}

void set_product( char* string )
{
    memcpy( &s_usb_udisk_param.usb_product, string, 16 );
}

void set_serial( char* string )
{
    memcpy( &s_usb_udisk_param.serial, string, 33 );
}


void set_show_manufacturer_disk(void)
{
    s_show_manufacturer_disk = true;
}

void set_show_u_disk(void)
{
    s_show_u_disk = true;
}

void set_show_card_disk(void)
{
    s_show_card_disk = true;
}

static void _msgbox_callback(msgbox_t * msgbox, msgbox_sig_e signal )
{    
}

scene_udisk_t* create_scene_udisk(void)
{
    s_udisk_ui = create_scene_udisk_ui();

    return &s_scene_udisk_api;
}


