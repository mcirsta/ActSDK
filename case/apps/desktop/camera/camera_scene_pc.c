/*******************************************************************************
 *                              USDK(1100)
 *                            Module: camera
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *    Albert_lyu 2009-5-26  1.0             build this file
 *******************************************************************************/
/*!
 * \file    camera_scene_pc.c
 * \brief    camera-pc场景
 * \author   Albert_lyu
 * \version 1.0
 * \date  2009-5-26
 *******************************************************************************/
#include "camera_scene_pc.h"
#include "camera_private.h"
#include "camera_sty.h"
#include "camera_define.h"
#include "camera_main.h"

/* global variable define */
camera_scene_pc_t* g_camera_scene_pc = NULL;
static int s_umonitor_device_fd = -1;
static int s_last_uoc_status = -1;
static USB_CONNECTION_T s_usb_status = PLUG_OUT;
static bool s_insert_usb_dialog = false;
static int s_ins_video_in = -1;
//static int s_ins_uoc = -1;
static int s_ins_uvc = -1;
static int s_ins_udc4uvc = -1;

static bool  _pc_get_tvout_info(unsigned int *tvout_info);
static bool _pc_is_tvout_on(void);
static int _pc_quit(void);
static bool _pc_paint(camera_scene_pc_t *data);
static bool _pc_ui_load_style(camera_scene_pc_t *data);
static bool _pc_init(void);
static bool _pc_mmm_init(void);

static bool _pc_deinit(void);
static int _pc_proc_gui(camera_scene_pc_t *data, gui_wm_message_t *pmsg);
static void _pc_callback(gui_wm_message_t *pmsg);
static int _pc_sys_proc(msg_apps_t *msg);
static bool _pc_open_theme(void);
static bool _pc_close_theme(void);
static bool _pc_open_str_res(void);
static bool _pc_close_str_res(void);
static bool _pc_set_default_font(void);
static bool _pc_unset_default_font(void);
static int _pc_mmm_open(void);
static bool _pc_mmm_close(void);

static bool _create_msgbox(void);
static bool _delete_msgbox(void);
static void _msgbox_cb(msgbox_t * msgbox, msgbox_sig_e signal);
static bool _pc_get_dialog_init(cam_dialog_e type, dialog_init_t *dialog_init);
static void _pc_dialog_callback(dialog_t *dialog, widget_sig_dialog_e signal);
static bool _pc_dialog_create(cam_dialog_e type);
static bool _pc_dialog_delete(void);
static void _pc_usb_check_event(void *data);
static bool _pc_get_usb( void );
static void _stop_charge( bool stop_charge );

static void _stop_charge( bool stop_charge )
{
    msg_apps_t msg;// = {0};
    int start_charge;

    memset(&msg,0,sizeof(msg_apps_t));
    print_dbg( " send msg_server.app stop_charge  == %d",  stop_charge  );

    if ( stop_charge == true )
    {
        start_charge = 0;
    }
    else
    {
        start_charge = 1;
    }

    msg.type = MSG_SET_CHARGE_START_SYNC;
//	    sprintf( msg.content, "%d", start_charge );
    memcpy( msg.content, &start_charge, sizeof( start_charge ) );
    send_sync_msg( APP_NAME_MSG_SERV, &msg, NULL, NULL );
}

static void _pc_usb_check_event(void *data)
{
    int ret;
    UOC_BSESS_VLD_T usb_status;
    bool stop_charge = false;

    /* 查询 usb 的状态 */
    ret = ioctl( s_umonitor_device_fd, UOC_IOCTL_CHK_BSESS_VLD, (int)&usb_status );
    if ( ret < 0 )
    {
        print_err(" ioctl error ! \n");
        return ;
    }

    print_dbg(" read status = 0x%x \n", usb_status );

    if ( usb_status == s_last_uoc_status )
    {
        return ;
    }
    s_last_uoc_status = usb_status;


    if ( usb_status == BSESS_OFF ) /* 拔线 */
    {
        print_dbg(" usb line was removed ! \n");
        kill_timer(g_camera_scene_pc->pc_even_id);
        g_camera_scene_pc->pc_even_id = 0;
        exit_msg_loop();
        g_camera_scene_pc->exit_type = CAM_SCENE_EXIT;
    }

    stop_charge = false;
    _stop_charge( stop_charge );

    return ;
}

static bool _pc_get_usb( void )
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
        print_err("send_sync_msg failed");
        return false;
    }
    memcpy(&hw_status, reply.content, sizeof(hw_status_t));

    if ( PLUG_OUT == hw_status.usb )
    {
        print_dbg("no usb line !\n");
    }
    s_usb_status = hw_status.usb;
    return true;
}
static bool  _pc_get_tvout_info(unsigned int *tvout_info)
{
    int fd;
    int ret;

    fd = open(DEVICE_FB, O_RDWR);
    if(fd < 0)
    {
        print_err("open DEVICE_FB error\n");
        return false;
    }

    ret = ioctl(fd, FBIOGET_DISPLAYER, (int)tvout_info);
    if(ret < 0)
    {
        print_err("get FBIOGET_DISPLAYER failed\n");
        close(fd);
        return false;
    }
    close(fd);
    print_dbg("fb displayer=%x\n",*tvout_info);
    return true;
}

static bool _pc_is_tvout_on(void)
{
    int tvout_info;
    bool ret = false;
    if(true == _pc_get_tvout_info(&tvout_info))
    {
        print_dbg("get tvout info is: %x\n",tvout_info);
        if((tvout_info & TV_PAL_DISPLAYER) || (tvout_info & TV_NTSC_DISPLAYER))
        {
            ret = true;  /*假如TVOUT已经打开了*/
        }
        else
        {
            ret = false; /*TVOUT没有打开*/
        }
    }
    else
    {
        /*假如读取TVOUT的信息出错了，当作TVOUT已经打开了*/
        ret = true;
    }
    return ret;
}
#define B_DIALOG
static bool _pc_get_dialog_init(cam_dialog_e type, dialog_init_t *dialog_init)
{
    dialog_init_t init;
    scene_t *scene;
    resgroup_resource_t *resgroup = NULL;
    string_resource_t *str_res = NULL;

    if (NULL == dialog_init)
    {
        print_err("param err");
        return false;
    }
    //get scene
    scene = sty_get_app_scene();
    resgroup = (resgroup_resource_t *) get_scene_child(scene, PC_STRGROUP_DIALOG);

    switch (type)
    {
        case CAM_DIALOG_NOSPACE:
        str_res = (string_resource_t *) get_resgroup_child(resgroup, STR_NOSPACE);
        init.head = DIALOG_HEAD_WARN;
        break;

        case CAM_DIALOG_NOPOWER:
        str_res = (string_resource_t *) get_resgroup_child(resgroup, STR_NOPOWER);
        init.head = DIALOG_HEAD_LOWBAT;
        break;

       case CAM_DIALOG_NOMEMORY:
        str_res = (string_resource_t *) get_resgroup_child(resgroup, STR_ACCESS_MEMORY_FAIL);
        init.head = DIALOG_HEAD_WARN;
        break;

        case CAM_DIALOG_OPEN_FAIL:
        str_res = (string_resource_t *) get_resgroup_child(resgroup, STR_OPFAIL);
        init.head = DIALOG_HEAD_WARN;
        break;

        case CAM_DIALOG_SAVEFILE:
        str_res = (string_resource_t *) get_resgroup_child(resgroup, STR_SAVEFILE);
        init.head = DIALOG_HEAD_INFO;
        break;

        case CAM_DIALOG_SAVE_FAIL:
        str_res = (string_resource_t *) get_resgroup_child(resgroup, STR_SAVE_FAIL);
        init.head = DIALOG_HEAD_WARN;
        break;

        case CAM_DIALOG_INIT_SENSOR_FAIL:
        str_res = (string_resource_t *) get_resgroup_child(resgroup, STR_INIT_FAIL);
        init.head = DIALOG_HEAD_WARN;
        break;


        case CAM_DIALOG_TVOUT_OPENED:
        str_res = (string_resource_t *) get_resgroup_child(resgroup, STR_TV_OUT_OPENED);
        init.head = DIALOG_HEAD_WARN;
        break;

        case CAM_DIALOG_PLUG_IN_USB:
        str_res = (string_resource_t *) get_resgroup_child(resgroup, STR_PLUG_IN_USB);
        init.head = DIALOG_HEAD_INFO;
        break;

        case CAM_DIALOG_NODISK:
        str_res = (string_resource_t *) get_resgroup_child(resgroup, STR_NODISK);
        init.head = DIALOG_HEAD_WARN;
        break;

        case CAM_DIALOG_DISKINSERT:
        str_res = (string_resource_t *) get_resgroup_child(resgroup, STR_DISKINSERT);
        init.head = DIALOG_HEAD_WARN;
        break;

        default:
        print_err();
        str_res = (string_resource_t *) get_resgroup_child(resgroup, STR_OPFAIL);
        init.head = DIALOG_HEAD_WARN;
        
        break;
    }
    if (NULL != str_res)
    {
        init.str_id = str_res->str_id;
    }
    else
    {
        print_err("str err\n");        
        return false;
    }

    *dialog_init = init;

    return true;
}

static void _pc_dialog_callback(dialog_t *dialog, widget_sig_dialog_e signal)
{
    print_dbg("signal:0x%x", signal);
    switch (signal)
    {

        case WIDGET_SIG_DIALOG_OK:
        _pc_dialog_delete();
        if (g_camera_scene_pc->need_exit == true)
        {
            g_camera_scene_pc->exit_type = CAM_SCENE_EXIT;
            _pc_quit();
        }          
        break;

        case WIDGET_SIG_DIALOG_CANCAL:
        if (g_camera_scene_pc->need_exit == true)
        {
            _pc_dialog_delete();            
            g_camera_scene_pc->exit_type = CAM_SCENE_EXIT;
            _pc_quit();
            
        }
        break;

        default:
        break;
    }
}

static bool _pc_dialog_create(cam_dialog_e type)
{
    bool result = true;
    dialog_init_t init;

    memset(&init, 0x00, sizeof(init));
    result = _pc_get_dialog_init(type, &init);
    if (false == result)
    {
        return false;
    }

    g_camera_scene_pc->dialog = dialog_create(&init, (dialog_cb_t) _pc_dialog_callback);
    if (NULL == g_camera_scene_pc->dialog)
    {
        print_err("dialog_create fail\n\n");
        return false;
    }

    dialog_paint(g_camera_scene_pc->dialog);

    return true;
}

static bool _pc_dialog_delete(void)
{
    if (NULL != g_camera_scene_pc->dialog)
    {
        dialog_delete(g_camera_scene_pc->dialog);
    }
    g_camera_scene_pc->dialog = NULL;

    return true;
}

#undef B_DIALOG

static bool _create_msgbox(void)
{
    g_camera_scene_pc->msgbox = msgbox_create(_msgbox_cb);
    if (NULL == g_camera_scene_pc->msgbox)
    {
        print_err("create msgbox failed!");
        return false;
    }

    return true;
}

static void _msgbox_cb(msgbox_t * msgbox, msgbox_sig_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_MSGBOX_SHOW:
        print_dbg("show message box");
        break;
        case WIDGET_SIG_MSGBOX_QUIT:
        print_dbg("quit message box");
        break;
        default:
        break;
    }
}

static bool _delete_msgbox(void)
{
    if (g_camera_scene_pc->msgbox != NULL)
    {
        msgbox_delete(g_camera_scene_pc->msgbox);
        g_camera_scene_pc->msgbox = NULL;
    }

    return true;
}

static bool _pc_paint(camera_scene_pc_t *data)
{
    int hdc = gui_dc_get(data->win_id);

    sty_draw_bitmap(hdc, data ->bg_bitmap_info, 0, 0);
    sty_draw_picture_res(hdc,data->icon_pccam);

    return true;
}

/* scene quit */
static int _pc_quit(void)
{
    if(g_camera_scene_pc->pc_even_id != 0)
    {
        kill_timer(g_camera_scene_pc->pc_even_id);
        g_camera_scene_pc->pc_even_id = 0;
    }
    exit_msg_loop();
    g_camera_scene_pc->exit_type = CAM_SCENE_EXIT;
    return 0;
}

static bool _pc_ui_load_style(camera_scene_pc_t *data)
{
    scene_t* scene; //!< commonui scene

    //get root resource
    // scene = sty_get_app_scene();
    scene = data->scene;
    if (scene == NULL)
    {
        print_err("cann't find app scene!");
        return false;
    }
    print_dbg();
    data->pc_bg = (picture_resource_t *) get_scene_child(scene, PC_PIC_BG);
    data->icon_pccam = (picture_resource_t *) get_scene_child(scene, PC_PIC_PCCAM);

    return true;
}

static bool _pc_open_theme(void)
{
    rotation_e angle;
    char common_file[PATH_MAX];
    char app_file[PATH_MAX];

    angle = sty_get_view_rotation();

    sty_set_screen_rotation(angle);

    if ((angle == ROTATION_0) || (angle == ROTATION_180))
    {
        g_camera_scene_pc->common_scene = load_scene(g_app_camera->common_style, COMMONUI_SIDELONG_SCENE);
        g_camera_scene_pc->scene = load_scene(g_app_camera->app_style, CAMERA_PC_SIDELONG_SCENE);
    }
    else 
    {
        g_camera_scene_pc->common_scene = load_scene(g_app_camera->common_style, COMMONUI_UPRIGHT_SCENE);
        g_camera_scene_pc->scene = load_scene(g_app_camera->app_style, CAMERA_PC_UPRIGHT_SCENE);
    }
    if ((g_camera_scene_pc->common_scene == NULL) || (g_camera_scene_pc->scene == NULL))
    {
        print_err("load commonui and application scene failed");
        return false;
    }
    else
    {
        sty_set_common_scene(g_camera_scene_pc->common_scene);
        sty_set_app_scene(g_camera_scene_pc->scene);
    }

    // 获取commonui 的图片资源文件名
    snprintf(common_file, sizeof(common_file), "%s/%s.res", sys_get_common_theme_res_dir(), "commonui");
    if (sty_open_common_pic_res(common_file) == false)
    {
        print_err("open %s failed!", common_file);
        return false;
    }

    // 获取camera 的图片资源文件名
    snprintf(app_file, sizeof(app_file), "%s/%s.res", sys_get_app_theme_res_dir(), "camera");
    if (sty_open_app_pic_res(app_file) == false)
    {
        print_err("open %s failed!", app_file);
        return false;
    }

    return true;

}

static bool _pc_close_theme(void)
{
    if(g_camera_scene_pc->common_scene != NULL)
    {
        free_scene(g_camera_scene_pc->common_scene);
        g_camera_scene_pc->common_scene = NULL;
    }

    if(g_camera_scene_pc->scene != NULL)
    {
        free_scene(g_camera_scene_pc->scene);
        g_camera_scene_pc->scene = NULL;
    }

    if (sty_close_common_pic_res() == false)
    {
        print_err("close common picture resource error!");
        return false;
    }

    if (sty_close_app_pic_res() == false)
    {
        print_err("close app picture resource error!");
        return false;
    }

    return true;
}

static bool _pc_open_str_res(void)
{
    char file[PATH_MAX];

    /* 获取commonui 字符串资源文件名 */
    snprintf(file, sizeof(file), "%s/%s_%s.str", sys_get_common_string_res_dir(),\
     "commonui", sys_get_cur_language_codes());
    print_dbg("load %s", file);
    /* 打开 commonui 字符串资源文件 */
    if (sty_open_common_str_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }

    /* 获取setting应用字符串资源文件名*/
    snprintf(file, sizeof(file), "%s/%s_%s.str", sys_get_app_string_res_dir(), "camera", sys_get_cur_language_codes());
    print_dbg("load %s", file);
    /* 打开应用的字符串资源文件 */
    if (sty_open_app_str_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }

    /* 设置语言编码*/
    sty_set_text_encoding(GUI_UTF8_ENC);
    gui_set_language_mode(sys_get_cur_language_id());
    return true;
}

static bool _pc_close_str_res(void)
{
    if (sty_close_common_str_res() == false)
    {
        print_err("close common string resource error!");
        return false;
    }

    if (sty_close_app_str_res() == false)
    {
        print_err("close app string resource error!");
        return false;
    }

    return true;
}

static void _set_monitor_mode( USB_MODE_T mode )
{
    int ret;
    int device_fd = -1;
    
    USB_MODE_T arg;
    arg = mode;

    device_fd = open( DEVICE_UMONITOR, O_RDWR, 0);
    if ( device_fd == -1 )
    {
        print_err(" open DEVICE_UMONITOR fd == %d", device_fd );
        return;
    }

    ret = ioctl( device_fd, UMONITOR_IOCTL_SET_USB_MODE, (int)&arg );
    if ( ret < 0 )
    {
        print_err(" ioctl error ! ");
    }

    close( device_fd );
}

static /*bool*/int _pc_mmm_open(void)
{
    int video_in_type = VI_PC_CAMERA_MODE;
    int result/*,result2*/;
    int fd;
    int uvc_device_fd = -1;

    UOC_CONNECTION_TYPE_T temp_status;
    struct uvc_runtime_arg uvc_arg
#pragma __PRQA_IGNORE_START__
            =
            { .task_priority = 2, .VID = USB_VENDOR_ID, .PID = UVC_PRODUCT_ID, .release = 0x1100,
                    .usb_vendor = "actions", .usb_product = "pc_camera", .serial = "1234567887654321", }
#pragma __PRQA_IGNORE_END__
    ;

    //MOUNT_DRV(result,"video_in.ko",&video_in_type,result2,DEVICE_VIDEO_IN,S_IFCHR,MAJOR_VIDEO_IN);
    result = insmod(("video_in.ko"),(void *)(&video_in_type));
    if(result != 0)
    {
        print_err("video_in.ko ninsmod err%d errnumber=%d\n",result,errno);
    }
    s_ins_video_in = result;

    if (result < 0)
    {
        return -1;
    }
    fd = open("/dev/video_in", O_RDWR);
    if (fd < 0)
    {
        print_err("sensor open drv result fd=%d ,errno=%d\n",fd,errno);
        print_err("open video_in err \n");
        fd = 0 - errno;
        result = rmmod("video_in.ko",0);
        s_ins_video_in = -1;
        if (result != 0)
        {
            print_err("video_in rmmod ret=%d errnumber=%d \n", result,errno);
        }
        return fd;
    }
    else
    {
        close(fd);
    }

    //result = insmod(("uoc.ko"),(void *)0);
    //if(result != 0)
    //{
    //    print_err("uoc.ko ninsmod err%d errnumber=%d\n",result,errno);
    //}
    //s_ins_uoc = result;

    //if (result < 0)
    //{
    //    goto err_rm_video_in;
    //}
    _set_monitor_mode(USB_MODE_SENSOR);
    result = insmod(("udc4uvc.ko"),(void *)0);
    if(result != 0)
    {
        print_err("udc4uvc.ko ninsmod err%d errnumber=%d\n",result,errno);
    }
    s_ins_udc4uvc = result;


    if (result < 0)
    {
        goto err_rm_video_in;
    }

    result = insmod(("uvc.ko"),(void *)(&uvc_arg));
    if(result != 0)
    {
        print_err("uvc.ko ninsmod err%d errnumber=%d\n",result,errno);
    }
    s_ins_uvc = result;

    if (result < 0)
    {
        goto err_rm_udc4uvc;
    }
    /* 打开设备 */
    sleep(2);
    s_umonitor_device_fd = open( DEVICE_UMONITOR, O_RDWR, 0);
    print_dbg("s_umonitor_device_fd=%d\n",s_umonitor_device_fd);
    if ( s_umonitor_device_fd == -1 )
    {
        print_dbg(" open s_umonitor_device_fd == %d", s_umonitor_device_fd);
        goto err_rm_uvc;
    }

    result = ioctl( s_umonitor_device_fd, UMONITOR_IOCTL_IS_CHARGER, (int)&temp_status);
    print_dbg("errno=%d\n",errno);
    print_dbg("result=%d charger?=(%d)\n",result,temp_status);
    if ( result < 0 )
    {
        print_err(" ioctl error ! \n");
        goto err_close_uoc;
    }

    if (temp_status == CHARGER)
    {
        print_err(" not pc,is charger !\n ");
        goto err_close_uoc;
    }

    return 0;

    err_close_uoc:
    if ( s_umonitor_device_fd != -1 )
    {
        close( s_umonitor_device_fd );
        s_umonitor_device_fd = -1;
    }

    err_rm_uvc:

    uvc_device_fd = open( DEVICE_UVC, O_RDWR, 0);
    print_dbg("uvc_device_fd=%d\n",uvc_device_fd);
    if ( uvc_device_fd == -1 )
    {
        print_err(" open uvc_device_fd == %d", uvc_device_fd);
    }

    //result = ioctl( uvc_device_fd, UVC_IOCTL_DO_DISCONNECT, 0);
    result = ioctl( uvc_device_fd, UVC_IOCLT_BASE, 0);

    print_dbg("result=%d \n",result);
    if ( result < 0 )
    {
        print_err(" ioctl uvc_device_fd UVC_IOCTL_DO_DISCONNECT error ! \n");
    }

    result = close(uvc_device_fd);
    print_dbg("result=%d \n",result);
    if (result < 0)
    {
        print_err("uvc_device_fd close err\n");
    }
    if (s_ins_uvc != -1)
    {
        s_ins_uvc = -1;
        result = rmmod("uvc.ko",0);
        if (result != 0)
        {
            print_err("uvc rmmod ret=%d errnumber=%d \n", result,errno);
        }
    }
    err_rm_udc4uvc:
    if (s_ins_udc4uvc != -1)
    {
        s_ins_udc4uvc = -1;
        result = rmmod("udc4uvc.ko",0);
        if (result != 0)
        {
            print_err("udc4uvc rmmod ret=%d errnumber=%d \n", result,errno);
        }
    }
    //err_rm_uoc:
    //if (s_ins_uoc != -1)
    //{
    //    s_ins_uoc = -1;
    //    result = rmmod("uoc.ko",0);
    //    if (result != 0)
    //    {
    //        print_err("uoc rmmod ret=%d errnumber=%d \n", result,errno);
    //    }
    //}
    err_rm_video_in:
    if (s_ins_video_in != -1)
    {
        s_ins_video_in = -1;
        result = rmmod("video_in.ko",0);
        if (result != 0)
        {
            print_err("video_in rmmod ret=%d errnumber=%d \n", result,errno);
        }
    }
    return -1;
}
static bool _pc_mmm_close(void)
{
    int uvc_device_fd = -1;
    int result = 0;

    if ( s_umonitor_device_fd != -1 )
    {
        close( s_umonitor_device_fd );
        s_umonitor_device_fd = -1;
    }

    uvc_device_fd = open( DEVICE_UVC, O_RDWR, 0);
    print_dbg("uvc_device_fd=%d\n",uvc_device_fd);
    if ( uvc_device_fd == -1 )
    {
        print_err(" open uvc_device_fd == %d", uvc_device_fd);
    }
    else
    {
        //result = ioctl( uvc_device_fd, UVC_IOCTL_DO_DISCONNECT, 0);
        result = ioctl( uvc_device_fd, UVC_IOCLT_BASE, 0);
        print_dbg("result=%d \n",result);
        if ( result < 0 )
        {
            print_err(" ioctl uvc_device_fd UVC_IOCTL_DO_DISCONNECT error ! \n");
        }

        result = close(uvc_device_fd);
        print_dbg("result=%d \n",result);
        if (result < 0)
        {
            print_err("uvc_device_fd close err\n");
        }
    }
    if (s_ins_uvc != -1)
    {
        s_ins_uvc = -1;
        result = rmmod("uvc.ko", 0);
        if (result != 0)
        {
            print_err("uvc rmmod ret=%d errnumber=%d \n", result,errno);
        }
    }

    if (s_ins_udc4uvc != -1)
    {
        s_ins_udc4uvc = -1;
        result = rmmod("udc4uvc.ko", 0);
        if (result != 0)
        {
            print_err("udc4uvc rmmod ret=%d errnumber=%d \n", result,errno);
        }
    }

    //if (s_ins_uoc != -1)
    //{
    //    s_ins_uoc = -1;
    //    result = rmmod("uoc.ko", 0);
    //    if (result != 0)
    //    {
    //        print_err("uoc rmmod ret=%d errnumber=%d \n", result,errno);
    //    }
    //}

    if (s_ins_video_in != -1)
    {
        s_ins_video_in = -1;
        result = rmmod("video_in.ko", 0);
        if (result != 0)
        {
            print_err("video_in rmmod ret=%d errnumber=%d \n", result,errno);
        }
    }
    release_msg(MSG_POWER_OFF);
    return true;
}
static bool _camera_pc_rotate(void)
{
    /*! 看下_scene_list_rotate */

    fusion_effect_t fusion_effect =
    { 0 };
    fusion_get_config(FUSION_TYPE_ROTATE, FUSION_FORWARD, &fusion_effect);
    fusion_set_effect(&fusion_effect);    
    fusion_set_first_frame(NULL);

    _pc_close_theme();
    _pc_open_theme();
    _pc_ui_load_style(g_camera_scene_pc);

    gui_wm_set_size(g_camera_scene_pc->win_id, g_camera_scene_pc->scene->width, g_camera_scene_pc->scene->height);

    if (NULL != g_camera_scene_pc->bg_bitmap_info)
    {
        sty_delete_bitmap(g_camera_scene_pc->bg_bitmap_info);
        g_camera_scene_pc->bg_bitmap_info = NULL;
    }
    g_camera_scene_pc->bg_bitmap_info = get_user_bg_pic(BG_CAMERA);
    if (NULL == g_camera_scene_pc->bg_bitmap_info)
    {
        g_camera_scene_pc->bg_bitmap_info = sty_create_bitmap((int) g_camera_scene_pc->pc_bg->pic_id);
    }
    if (NULL == g_camera_scene_pc->bg_bitmap_info)
    {
        print_err("create bg bitmap pic err\n");
        return false;
    }

    _pc_paint(g_camera_scene_pc);

    if (g_camera_scene_pc->msgbox != NULL)
    {
        msgbox_load_style(g_camera_scene_pc->msgbox);
        msgbox_paint(g_camera_scene_pc->msgbox);
    }

    //gui_framebuffer_update();

    fusion_set_last_frame(NULL);
    fusion_display(NULL);

    return true;

}

static bool _pc_set_default_font(void)
{
    /* crate font */
    if (g_camera_scene_pc->fontdes > 0)
    {
        return false;
    }

    g_camera_scene_pc->fontdes = gui_create_font((unsigned char*)sys_get_default_font_file(), 16);
    if (g_camera_scene_pc->fontdes < 0)
    {
        print_err("call gui_create_font failed");
        return false;
    }

    if (gui_dc_set_default_fontface(g_camera_scene_pc->fontdes) < 0)
    {
        print_err("gui_dc_set_default_fontface failed");
        return false;
    }

    return true;
}

static bool _pc_unset_default_font(void)
{
    /* crate font */
    if (g_camera_scene_pc->fontdes <= 0)
    {
        return false;
    }

    gui_destroy_font(g_camera_scene_pc->fontdes);
    g_camera_scene_pc->fontdes = 0;

    return true;
}

/* scene initial function */
static bool _pc_init(void)
{
    camera_scene_pc_t *data;
    fusion_effect_t fusion_effect =
    { 0 };

    print_dbg();

    //分配场景数据空间
    if (g_camera_scene_pc == NULL)
    {
        g_camera_scene_pc = malloc(sizeof(camera_scene_pc_t));

        if (g_camera_scene_pc == NULL)
        {
            print_err();
            return false;
        }
        else
        {
            memset(g_camera_scene_pc, 0, sizeof(camera_scene_pc_t));
            data = g_camera_scene_pc;
        }
    }
    else
    {
        data = g_camera_scene_pc;
    }

    data->need_exit = FALSE;
    data->exit_type = CAM_SCENE_EXIT;

    if (_pc_open_theme() == false)
    {
        print_err("_pc_open_theme failed\n");
        goto open_theme_err;
    }

    if (_pc_open_str_res() == false)
    {
        print_err("_pc_open_str_res failed\n");
        goto open_str_err;
    }

    if (_pc_set_default_font() == false)
    {
        print_err("call _pc_set_default_font failed\n");
        goto create_font_err;
    }

    if (_pc_ui_load_style(data) == false)
    {
        print_err("_pc_ui_load_style failed\n");
        goto load_resource_err;
    }

    //gui_set_dimension(data->scene->width, data->scene->height);


    data->win_id = gui_wm_create_window(data->scene->x, data->scene->y, data->scene->width, data->scene->height,
            WM_CREATE_SHOW, _pc_callback, 0);
    if (data->win_id == 0)
    {
        print_err("call gui_wm_create_window failed");
        goto create_window_err;
    }

    //data->hdc = gui_dc_get(data->win_id);
    //gui_dc_clear( data->hdc );


    gui_wm_set_focus(data->win_id);

    data->bg_bitmap_info = get_user_bg_pic(BG_CAMERA);
    if (NULL == data->bg_bitmap_info)
    {
        data->bg_bitmap_info = sty_create_bitmap((int) data->pc_bg->pic_id);
    }
    if (NULL == data->bg_bitmap_info)
    {
        print_err("sty_create_bitmap err\n");
        goto create_bitmap_err;
    }

    fusion_get_config(FUSION_TYPE_APP_CHANGE, FUSION_FORWARD, &fusion_effect);
    fusion_set_effect(&fusion_effect);
    scene_fusion_set_first_frame(data->scene);

    _pc_paint(data);
    gui_screen_update();

    scene_fusion_set_last_frame(data->scene);
    fusion_display(NULL);

    if(_pc_is_tvout_on() == true)
    {
        print_err("TV OUT is on,can't into CAMERA!\n");
        _pc_dialog_create(CAM_DIALOG_TVOUT_OPENED);
        data->need_exit = true;
        return true;

        //goto create_msgbox_err; //qac不可达
    }

    if (_create_msgbox() == false)
    {
        print_err("_pc_msgbox_create failed\n");
        goto create_msgbox_err;
    }

    #if 1
    if (_pc_get_usb() == false)
    {
        goto create_msgbox_err;
    }
    clear_key_msg_queue(-1, -1);
    print_dbg("usb_status=%d\n",s_usb_status);
    #endif
    if (s_usb_status == PLUG_OUT)
    {
        print_dbg("no usb line show dialog ....\n");
        _pc_dialog_create(CAM_DIALOG_PLUG_IN_USB);
        //未插入对话框要求esc可以退出，插入usb要自己消失，否则一直停留
        s_insert_usb_dialog = true;
        return true;
    }
    if (_pc_mmm_init() == false)
    {
        goto mmm_init_err;
    }
    clear_key_msg_queue(-1, -1);

    return true;

    mmm_init_err: _delete_msgbox();

    create_msgbox_err: //delete(bitmap);

    create_bitmap_err: gui_wm_delete_window(data->win_id);

    create_window_err: load_resource_err: _pc_unset_default_font();

    create_font_err: _pc_close_str_res();

    open_str_err: _pc_close_theme();

    open_theme_err:

    return false;

}
static bool _pc_mmm_init(void)
{
    cam_dialog_e dialog_str_type_temp = CAM_DIALOG_OPEN_FAIL ;
    int ret = 0;

    ret = _pc_mmm_open();
    if (ret < 0)
    {
        print_err("err ret=(%d)\n");
        if (ret == -ENOMEM)
        {
            dialog_str_type_temp = CAM_DIALOG_NOMEMORY;
        }
        else if(ret == -EIO)
        {
            dialog_str_type_temp = CAM_DIALOG_INIT_SENSOR_FAIL;
        }
        else
        {
            dialog_str_type_temp = CAM_DIALOG_OPEN_FAIL;
        }
        print_err("camera_pc_mmm_open\n");
        goto mmm_open_err;
    }
    //gui_framebuffer_update();
    g_camera_scene_pc->pc_even_id = set_timer(500,\
                        (timer_proc) _pc_usb_check_event, (void *) g_camera_scene_pc);
    clear_key_msg_queue(-1, -1);

    grab_msg(MSG_POWER_OFF);


#if (DBG_REMNANT_MEMORY == 1)
    unsigned int u;
    u = os_mem_query ();
    printf("-----into camera----os_mem_query_remnant=%x\n",u);
    print_mem();
    printf("-----show all module mem use info--(begin)---\n\n",u);
    dump_all_module_mem_info();
    printf("-----show all module mem use info---(end)--\n\n",u);
#endif /* DBG_REMNANT_MEMORY */
    
    return true;

    mmm_open_err: _pc_dialog_create(dialog_str_type_temp);
    g_camera_scene_pc->need_exit = true;
    return true;    
    //TODO:不同的错误用errno来定，最后的地方弹出msg
    #if 0
    _delete_msgbox();

    create_msgbox_err:
    //delete(bitmap);

    create_bitmap_err: gui_wm_delete_window(g_camera_scene_pc->win_id);

    create_window_err: load_resource_err: _pc_unset_default_font();

    create_font_err: _pc_close_str_res();
    open_str_err: _pc_close_theme();
    open_theme_err:
    #endif

    //qac 不可达 //return false;
}
/* scene exit function */
static bool _pc_deinit(void)
{
    camera_scene_pc_t *data = g_camera_scene_pc;

    if (data->pc_even_id != 0)
    {
        kill_timer(data->pc_even_id);
        data->pc_even_id = 0;
    }

    _pc_dialog_delete();

    if (_delete_msgbox() == false)
    {
        print_err("delete msgbox failed!");
        //ret = false;
        return false;
    }
    if (NULL != data->bg_bitmap_info)
    {
        sty_delete_bitmap(data->bg_bitmap_info);
        data->bg_bitmap_info = NULL;
    }

    _pc_mmm_close();

    if (data->win_id != 0)
    {
        gui_wm_delete_window(data->win_id);
        data->win_id = (int) NULL;
    }

    _pc_unset_default_font();

    if (_pc_close_theme() == false)
    {
        return false;
    }

    if (_pc_close_str_res() == false)
    {
        return false;
    }

    g_camera_scene_pc->exit_type = CAM_SCENE_EXIT;

    return true;

}
/* scene msg process */

static int _pc_proc_gui(camera_scene_pc_t *data, gui_wm_message_t *pmsg)
{
    int ret = 0;
    key_event_t *kmsg;

    switch (pmsg->msgid)
    {
        case WM_MSG_CREATE:
        break;

        case WM_MSG_KEY:
        kmsg = (key_event_t *) (pmsg->data.p);
        switch (kmsg->val | kmsg->type)
        {

            case KEY_RETURN | KEY_TYPE_SHORT_UP:
            {
                if (s_insert_usb_dialog == true)
                {
                    print_info("pccam key return \n");
                    data->exit_type = CAM_SCENE_EXIT;
                    _pc_quit();
                }
                clear_key_msg_queue(-1,-1);
                break;
            }
            default:
            gui_wm_default_callback(pmsg);
            break;
        }
        break;

        default:
        break;
    }

    return ret;

}

/* scene callback function */
static void _pc_callback(gui_wm_message_t *pmsg)
{
    int top;

    print_dbg();

    top = canvas_get_focus(pmsg);
    if ((g_camera_scene_pc->msgbox != NULL) && (top == g_camera_scene_pc->msgbox->canvas_id))
    {
        /* 处理默认系统消息(低电提示、卡插入和拔出提示、USB插入功能选择提示等)的msgbox控件需要接受GUI消息 */
        msgbox_proc_gui(g_camera_scene_pc->msgbox, pmsg);
    }
    else if ((g_camera_scene_pc->dialog != NULL) && (top == g_camera_scene_pc->dialog->canvas_id) \
        && (g_camera_scene_pc->need_exit == true))
    {
        dialog_proc_gui(g_camera_scene_pc->dialog, pmsg);
    }    
    else
    {
        _pc_proc_gui(g_camera_scene_pc, pmsg);
    }

}
static int _pc_sys_proc(msg_apps_t *msg)
{
    msg_reply_t t;

    print_dbg("msg.type:%d\n", msg->type);
    switch (msg->type)
    {
        case MSG_ACTIVE_SYNC:
        t.type = MSG_REPLY_SUCCESS;
        reply_sync_msg(&t);
        break;

        case MSG_APP_QUIT:
        exit_msg_loop();
        g_camera_scene_pc->exit_type = CAM_SCENE_EXIT;
        break;

        case MSG_USB_B_OUT:
        print_dbg(" receive  MSG_USB_B_OUT");

        exit_msg_loop();
        g_camera_scene_pc->exit_type = CAM_SCENE_EXIT;
        break;

        case MSG_GSENSOR_ROTATION_CHANGED:
        print_info("received msg: MSG_GSENSOR_ROTATION_CHANGED");
        _camera_pc_rotate();
        break;

        case MSG_USB_B_IN:
        if(s_usb_status == PLUG_OUT)
        {
            //wait dialog...
            print_dbg("camera_pc_dialog_wait_usb_plug in\n");
            s_insert_usb_dialog = false;
            _pc_dialog_delete();
            if (_pc_mmm_init() == false)
            {
                print_err("_pc_mmm_init err\n");
                exit_msg_loop();
                g_camera_scene_pc->exit_type = CAM_SCENE_EXIT;
            }
            clear_key_msg_queue(-1, -1);
        }
        break;
        //TODO:when MSG_USB_ADAPTOR_IN choice background charge will stop here...

        case MSG_NEED_GEN_PLIST:
        break;

        case MSG_SHORTCUT_RETURN:
        break;
        default:
        {
            if ((g_camera_scene_pc->msgbox != NULL) && (MSG_USB_B_IN != msg->type))
            {
                /* 控件msgbox用来处理默认系统消息(低电提示、卡插入和拔出提示、USB插入功能选择提示等) */
                msgbox_proc_sys(g_camera_scene_pc->msgbox, msg);
            }
            break;
        }
    }
    return 0;
}

/* scene entry function */
int camera_scene_pc(void)
{
    bool result;
    msg_apps_t msg;
    int ret = 0;
    /*进pc camera时，当pc上界面没出来时，
    usb驱动有时一直处于忙状态，
    无时间片给应用运行，所以关闭wd
    退出pc camera时再打开*/
    sys_forbid_soft_watchdog(true);
    result = _pc_init();
    if (result == false)
    {
        print_err("scene init error! \n");
        ret = -1;
        goto err_return;
    }
    register_sys_dispatcher(_pc_sys_proc);
    while ((get_msg(&msg)) == true)
    {
        dispatch_msg(&msg);
    }
    unregister_sys_dispatcher();

    result = _pc_deinit();
    sys_forbid_soft_watchdog(false);
    ret = (int)(g_camera_scene_pc->exit_type);


    if (result == false)
    {
        print_err("scene exit error! \n");
        ret = -1;
        goto err_return;
    }

    err_return:
    if (g_camera_scene_pc != NULL)
    {
        free(g_camera_scene_pc);
        g_camera_scene_pc = NULL;
    }
    
    return ret;
}

