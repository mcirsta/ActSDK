/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : usb_scene_upgrade.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *    zhaowenjie    2009-2-25          v1.0               build this file
 ********************************************************************************/
/*!
* \file     usb_scene_upgrade.c
* \brief    fw upgrade scene
* \author   zhaowenjie
*
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#include "usb.h"
#include "usb_scene_upgrade.h"
#include "usb_scene_upgrade_ui.h"

#define UPGRADE_DRIVER "upgrade.ko"
#define UPGRADE_TASK_PRIORITY 29

#define HUNDRED_PERCENT 100

#define READ_STATUS_INTERVAL   300

//  #define USB_UPGRADE_FW_FILE "/mnt/disk0/us213f.upg"
#define USB_UPGRADE_FW_FILE UDISK_ROOT"/us213f.upg"

/* config 可配置 */
static char s_usb_upgrade_fw_file[ PATH_MAX  ];

static int s_timer = NULL_TIMER;

static scene_upgrade_ui_t* s_upgrade_ui = NULL;
static int s_window;

int s_fd = -1;

void enter_scene_upgrade(void);
static bool _scene_upgrade_init(void);
static bool _check_upgrade_fw_file( char* file, const char* default_file );
static void _scene_upgrade_deinit(void);
static bool _show_scene( scene_t* scene_resource );

static bool _upgrade_install_driver(void);
static bool _upgrade_remove_driver( void );

static bool _upgrade_fw(void);
static int _proc_timer( void * param );
static void _reboot_system( void );
static bool _delete_upgrade_fw_file( void );
char* get_upgrade_fw_file(void);

static void _proc_gui_msg(gui_wm_message_t *pmsg);

static scene_upgrade_t s_scene_upgrade_api =
{
    enter_scene_upgrade,

    get_upgrade_fw_file
};

void enter_scene_upgrade(void)
{
    bool ret;
    msg_apps_t  msg;

    print_info(" ************   enter  scene_upgrade  ************* ");

    /* 场景初始化 */
    ret = _scene_upgrade_init();
    if( ret == false )
    {
        print_err("upgrade scene init error! \n");
        goto ERROR_EXIT;
    }

    _upgrade_fw();

    register_sys_dispatcher( NULL );

    while( get_msg(&msg) == true )
    {
        dispatch_msg(&msg);
    }

    unregister_sys_dispatcher();

    _delete_upgrade_fw_file();

    ERROR_EXIT:

    /* 场景退出 */
    _scene_upgrade_deinit();

    g_usb_main->set_current_scene( SCENE_EXIT );

    print_info(" ************  exit  scene_upgrade  ************* ");
}


/* scene initial function */
static bool _scene_upgrade_init(void)
{
    bool ret;
    scene_t* scene_resource;
    fusion_enter_e fenter;
    fusion_effect_t eff;

    ret = _check_upgrade_fw_file( s_usb_upgrade_fw_file, USB_UPGRADE_FW_FILE );
    if ( ret == false )
    {
        return false;
    }

    scene_resource = s_upgrade_ui->load_style_resource();
    if ( scene_resource == NULL )
    {
        print_err("s_upgrade_ui->load_style_resource();");
        return false;
    }

    fenter = FUSION_FORWARD;
    fusion_get_config( FUSION_TYPE_COMMON, fenter, &eff );
    fusion_set_effect( &eff );
    fusion_set_first_frame( NULL );

    /* 显示基本画面 */
    ret = _show_scene( scene_resource );
    if ( ret == false )
    {
        /* 释放fusion的内存，否则会内存泄漏 */
        fusion_display( NULL );
        return false;
    }

    ret = _upgrade_install_driver();
    if(ret == false )
    {
        print_err("_upgrade_install_driver error! \n");
        /* 释放fusion的内存，否则会内存泄漏 */
        fusion_display( NULL );
        return false;
    }

    
    fusion_set_last_frame( NULL );
    fusion_display( NULL);

    return true;
}

static bool _check_upgrade_fw_file( char* file, const char* default_file )
{
    unsigned int len;
    FILE* pfile = NULL;

    len = strlen( file );
    if ( len == 0 )
    {
        /* 如果没有配置值，则取默认值 */
        strcpy( file, default_file );
    }

    /* 打开文件 */
    pfile = fopen( file, "r" );
    if (pfile == NULL)
    {
        /* 如果u盘没有固件, 直接返回 */
        return false;
    }

    print_info(" upgrade_fw_file = %s ......... \n", file );

    fclose(pfile);
    return true;
}

static bool _upgrade_fw(void)
{
    int tmp = 0;

    /*! 升级状态初始化 */

    /* 打开字符设备文件 */
    s_fd = open( DEVICE_UPGRADE, O_RDWR, 0);
    if ( s_fd == WRONG_DEVICE_FD )
    {
        print_err(" open upg err %d\n", s_fd );
        return false;
    }

    /* 初始化 */
    tmp = ioctl( s_fd, UPG_CMD_INIT, 0);
    if ( tmp < 0 )
    {
        print_err("upg init err %d\n", tmp);
        return false;
    }

    /* 检查固件，驱动中会创建检查任务，因该任务比较耗时，如此方能同时读取检查状态 */
    tmp = ioctl( s_fd, UPG_CMD_CHECK_FW, 0);
    if ( tmp < 0 )
    {
        print_err("upg check fw err %d\n", tmp);
        return false;
    }

    s_timer = set_timer( READ_STATUS_INTERVAL, _proc_timer, NULL );

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
    s_upgrade_ui->paint_upgrade_initial_ui( hdc );

    return true;
}

static bool _upgrade_install_driver( void )
{
    long tmp = 0;

    /*! 给驱动的参数，包括固件路径，检查最大lba */
    struct upg_param_t upg_param 
    #pragma __PRQA_IGNORE_START__
    =
    {
        .pb_fwpath = "",
        .b_is_check_maxlba = 1
    }
    #pragma __PRQA_IGNORE_END__
    ;

    /* 给驱动的参数赋值，如驱动中任务的优先级 */
    upg_param.pb_fwpath = s_usb_upgrade_fw_file;
    upg_param.task_priority = UPGRADE_TASK_PRIORITY;

    /* 加载驱动 */
    tmp = insmod( UPGRADE_DRIVER, &upg_param);
    if (tmp != 0 )
    {
        print_err("upgrade.ko install err %d\n", tmp);
        return false;
    }

    return true;
}

static int _proc_timer( void * param )
{
    static bool s_checking_status = true;
    bool reboot = false;

    int tmp;
    unsigned short percent;

    struct upg_status_t upg_status 
    #pragma __PRQA_IGNORE_START__
    =
    {
        .e_state = UPG_STATE_DEFAULT,
        .d_prg = 0,
        .d_errtype = 0
    }
    #pragma __PRQA_IGNORE_END__
    ;
    /* 读取状态 */
    tmp = ioctl( s_fd, UPG_CMD_READ_STATUS, (int)(&upg_status));
    if ( tmp < 0 )
    {
        print_err(" upg UPG_CMD_READ_STATUS err %d\n", tmp);
    }

    if ( s_checking_status == true ) /* 正在检查固件 */
    {
        if ( upg_status.e_state == UPG_STATE_ERROR )
        {
            print_dbg(" check usb fw err %d\n", tmp);
            goto EXIT_UPGRADE;
        }

        if ( upg_status.e_state == UPG_STATE_CHECK_OK )
        {
            /* 检查完毕，开始升级 */
            print_dbg(" check fw succeed !!!!!!!!!!!!!!!");

            s_checking_status = false;

            tmp = ioctl( s_fd, UPG_CMD_WRITE_FW, 0);
            if (tmp < 0 )
            {
                print_err(" UPG_CMD_WRITE_FW err %d\n", tmp);
                goto EXIT_UPGRADE;
            }
        }

        percent = (unsigned short)upg_status.d_prg;

        print_dbg(" checking fw percent = %d\n", percent );
    }
    else  /* 升级固件 */
    {
        if ( upg_status.e_state == UPG_STATE_ERROR )
        {
            print_err("upg writing fw err %d\n", tmp);

            goto EXIT_UPGRADE;
        }

        if ( upg_status.e_state == UPG_STATE_WRITE_OK )
        {
            /* 写完毕 */
            print_dbg(" upgrade succeed !!!!!!!!!!!!!!!");
            s_upgrade_ui->update_process_bar( HUNDRED_PERCENT );

            reboot = true;
            goto EXIT_UPGRADE;
        }

        percent = (unsigned short)upg_status.d_prg;

        print_dbg(" upgrading fw percent = %d\n", percent );

        s_upgrade_ui->update_process_bar( percent );
    }

    return 0;


    EXIT_UPGRADE:

    exit_msg_loop();
    g_usb_main->set_current_scene( SCENE_EXIT );

    if ( reboot == true )
    {
        _reboot_system();
    }

    return 0;
}

static void _reboot_system( void )
{
    int wd_fd;
    int ret;
    int time_out = 1000;  /*1秒*/
  
    wd_fd = open( DEVICE_WD, O_RDWR, 0);

    if( wd_fd < 0 )
    {
        print_err("Error: open watchdog device\n\n");
    }

    /* 设置后，1秒内若没有再调用此函数，则会重启*/

    ret = ioctl(wd_fd, WDD_CMD_SET_TIME, time_out);
    if ( ret < 0 )
    {
        print_err("ioctl(wd_fd, WDD_CMD_SET_TIME, time_out);....");
    }

    sleep(5);
    close( wd_fd );
}

static bool _upgrade_remove_driver( void )
{
    rmmod( UPGRADE_DRIVER, 0);

    return true;
}

static void _proc_gui_msg(gui_wm_message_t *pmsg)
{
}

static void _scene_upgrade_deinit(void)
{
    if ( s_timer != NULL_TIMER )
    {
        kill_timer( s_timer );
        s_timer = NULL_TIMER;
    }

    if ( s_fd != -1 )
    {
        close( s_fd );
    }

    s_upgrade_ui->free_style_resource();

    /* 删除窗体 */
    if ( s_window != 0 )
    {
        gui_wm_delete_window( s_window );
    }

    _upgrade_remove_driver();
}

static bool _delete_upgrade_fw_file( void )
{
    remove( s_usb_upgrade_fw_file );

    return true;
}

char* get_upgrade_fw_file(void)
{
    return s_usb_upgrade_fw_file;
}

scene_upgrade_t* create_scene_upgrade(void)
{
    s_upgrade_ui = create_scene_upgrade_ui();

    return &s_scene_upgrade_api;
}
