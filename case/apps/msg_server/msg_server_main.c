/*
 *******************************************************************************
 *                       NOYA1100
 *                (c) Copyright 2002-2008, Actions Co,Ld.
 *                        All Right Reserved
 * FileName: msg_sever_main.c
 * Description:
 * History:
 *       <author>    <time>          <version >      <desc>
 *       shaofeng    2009/03/11      1.0             建立该文件。
 *******************************************************************************
 */
#include "include_psp.h"
#include "include_case.h"
#include "include_msg_server.h"

#include "cfg_voltage_charge.h"




/* extern function, 函数声明 */
static bool _init_timers(void);
static bool _start_times(void);
static bool _init(void);
static bool _deinit(void);

static bool _lib_init(void);
static void _lib_deinit(void);

static void _set_key_rise_clk_domain( void );
static void _set_key_rise_clk_domain( void );

static int _msg_key_server(void *param);
static int _msg_keylock_server(void *param);
static int _charge_server(void *param);
static inline int _msg_low_voltage_server(void);
static int _msg_gsensor_server( void *param );
static int _sys_event_server(void *param);

static int _proc_sys_msg(msg_apps_t *msg);

static bool _get_alarm_clock(alarm_get_clock_param_t * param);



static void *s_applib_lib = 0;
static void *s_config_lib = 0;
static void *s_act_mem_lib = 0;

static bool is_console;

static hw_status_t s_hw_status;

//按键升频的升频值
static const clk_domain_t s_clk_domain = {RISE_CLK_DOMAIN_WHEN_KEY_DOWN, RISE_CLK_DOMAIN_WHEN_KEY_DOWN};
//按键是否已经升频的标志, 用于避免多次设置升频, 或者多次清除升频
static int s_key_has_rise_clk = 0;


typedef struct
{
    int key;
    int keylock;
    int charge;
    int sys_evt;
    int g_sensor;
    int rise_clk;
}timers_id_t;

static timers_id_t s_server_times_id;

#define TVIN_MODE_NO    0x01
#define ALARM_MODE_NO   0x02

#if 0
#endif
/******************************************************************************/
/*!
 * \par  Description:
 *       初始化应用需要的所有timer
 * \param[in]
 * \retval
 *******************************************************************************/
static bool _init_timers( void )
{
    s_server_times_id.key = set_timer(TIME_DETECT_KEY, (timer_proc)_msg_key_server, NULL);
    if (s_server_times_id.key <= 0)
    {
        return false;
    }
    stop_timer(s_server_times_id.key);

    s_server_times_id.keylock = set_timer(TIME_DETECT_KEYLOCK, (timer_proc)_msg_keylock_server, NULL);
    if (s_server_times_id.keylock <= 0)
    {
        return false;
    }
    stop_timer(s_server_times_id.keylock);

    s_server_times_id.charge = set_timer(TIME_DETECT_CHARGE, (timer_proc)_charge_server, NULL);
    if (s_server_times_id.charge <= 0)
    {
        return false;
    }
    stop_timer(s_server_times_id.charge);

    s_server_times_id.g_sensor = set_timer(TIME_DETECT_GSENSOR, (timer_proc)_msg_gsensor_server, NULL);
    if (s_server_times_id.g_sensor <= 0)
    {
        return false;
    }
    stop_timer(s_server_times_id.g_sensor);

    s_server_times_id.sys_evt = set_timer(TIME_DETECT_SYS_EVT, (timer_proc)_sys_event_server, NULL);
    if (s_server_times_id.sys_evt <= 0)
    {
        return false;
    }
    stop_timer(s_server_times_id.sys_evt);

    /* start current system timers, screensaver timer, idle timer etc. */
    load_sys_timers_config();
    start_sys_timers(true);
    start_sys_timers(false);
}

static bool _start_times(void)
{
    restart_timer(s_server_times_id.key);
    restart_timer(s_server_times_id.keylock);
    restart_timer(s_server_times_id.charge);
    restart_timer(s_server_times_id.sys_evt);

    //set_single_shot_timer(100, _start_sys_evt_server, NULL);

    if(drv_op_gsensor_is_dev_opened() == true)
    {
        restart_timer(s_server_times_id.g_sensor);
    }

    /* start current system timers, screensaver timer, idle timer etc. */
    start_sys_timers(true);

    return true;
}

static bool _init( void )
{
    /* init msg server's config module and read all configurations */
    msg_server_cfg_init(g_msg_server_cfgs, CFG_TYPE_MAX);
    msg_server_cfg_read_all(g_msg_server_cfgs, CFG_TYPE_MAX);

    /* sleep time need to be set off when power on */
    g_msg_server_cfgs[CFG_TYPE_SLEEP_TIME].value = 0;
    g_msg_server_cfgs[CFG_TYPE_SLEEP_TIME].changed = true;
    msg_server_cfg_write(&g_msg_server_cfgs[CFG_TYPE_SLEEP_TIME]);

    msg_queue_init();

    key_device_open();
    key_para_init();


    usb_monitor_device_open();

    charge_device_open();
    charge_para_init();

    /* set current backlight */
    drv_op_lcd_open_dev();
    drv_op_lcd_set_backlight((unsigned int)g_msg_server_cfgs[CFG_TYPE_BRIGHTNESS].value);
    drv_op_lcd_close_dev();

    init_g_sensor_axes_trans_matrix();

    if( (strcasecmp(g_msg_server_cfgs[CFG_TYPE_SCR_MODE].text, "Auto") == 0)
            ||(strcasecmp(g_msg_server_cfgs[CFG_TYPE_GSENSOR_SHAKE_SUPPORT].text, "on") == 0))
    {
        drv_op_gsensor_open_dev();
        drv_op_gsensor_enable();
    }

    sys_evt_monitor_open_dev();

    ada_device_open();/* it should be here, to reserve time interval between ada insmod*/
    ada_para_init();

    _init_timers();
    if( is_console == false )
    {
        _start_times();
    }

    return true;

}

static bool _deinit(void)
{
    start_sys_timers(false);
    kill_timers();

    key_device_close();
    usb_monitor_device_close();
    charge_device_close();
    ada_device_close();

    drv_op_gsensor_disable();
    drv_op_gsensor_close_dev();

    sys_evt_monitor_close_dev();
    msg_queue_deinit();

    msg_server_cfg_deinit(g_msg_server_cfgs, CFG_TYPE_MAX);

    return true;
}

static bool _lib_init(void)
{
    s_applib_lib = dlopen("applib.so", RTLD_LAZY);
    if (s_applib_lib == NULL)
    {
        print_err("open applib lib err!");
        goto _LIB_INIT_ERR;
    }

    s_config_lib = dlopen("apconfig.so", RTLD_LAZY);
    if (s_config_lib == NULL)
    {
        print_err("open apconfig lib err!");
        goto _LIB_INIT_ERR;
    }
    s_act_mem_lib = dlopen("act_mem.so", RTLD_LAZY);
    if (s_act_mem_lib == NULL)
    {
        print_err("open s_act_mem_lib err!");
        goto _LIB_INIT_ERR;
    }
    return true;

    _LIB_INIT_ERR:
    if (s_applib_lib != NULL)
    {
        dlclose(s_applib_lib);
        s_applib_lib = NULL;
    }
    if (s_config_lib != NULL)
    {
        dlclose(s_config_lib);
        s_config_lib = NULL;
    }
    if (s_act_mem_lib != NULL)
    {
        dlclose(s_act_mem_lib);
        s_act_mem_lib = NULL;
    }
    return false;
}

static void _lib_deinit(void)
{
    if (s_applib_lib != NULL)
    {
        dlclose(s_applib_lib);
        s_applib_lib = NULL;
    }
    if (s_config_lib != NULL)
    {
        dlclose(s_config_lib);
        s_config_lib = NULL;
    }
    if (s_act_mem_lib != NULL)
    {
        dlclose(s_act_mem_lib);
        s_act_mem_lib = NULL;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *       设置按键升频升频函数
 *       增加标志位控制, 避免多次设置了升频
 * \param[in]
 * \retval
 *******************************************************************************/
static void _set_key_rise_clk_domain( void )
{
    if ( 1 == s_key_has_rise_clk )
    {
        return;
    }
    s_key_has_rise_clk = 1;
    sys_pm_set_clk_domain(&s_clk_domain);
}

/******************************************************************************/
/*!
 * \par  Description:
 *       清除按键升频升频函数
 *       增加标志位控制, 避免多次清除了升频
 * \param[in]
 * \retval
 *******************************************************************************/
static void _clear_key_rise_clk_domain( void )
{
    if ( 0 == s_key_has_rise_clk )
    {
        return;
    }
    s_key_has_rise_clk = 0;
    sys_pm_clear_clk_domain(&s_clk_domain);
}

/******************************************************************************/
/*!
 * \par  Description:
 *       升频timer处理函数
 *       用于到达升频时间后, 清除升频设置, 使系统恢复到正常频率
 * \param[in]
 * \retval
 *******************************************************************************/
static int _proc_singel_timer_rise_clk( void *param )
{
    _clear_key_rise_clk_domain();
    return 1;
}

/******************************************************************************/
/*!
 * \par  Description:
 *       按键处理函数
 *       read key value from key driver,
 *       play key tone, detect weather keylock, search map key, search shortcut key,
 *       and finally send key message to ucGUI
 * \param[in]
 * \retval
 *******************************************************************************/
static int _msg_key_server( void *param )
{
    static bool is_just_exit_screensaver = false;
    static unsigned int buffer_keyval[10];
    bool result = true;
    input_gui_msg_t input_msg;
    int i;
    int key_num;

    memset(&buffer_keyval, 0x00, sizeof(buffer_keyval));
    memset(&input_msg, 0x00, sizeof(input_msg));
    input_msg.type = INPUT_MSG_KEY;

    key_num = key_get_no_jitter_keyval( sizeof(buffer_keyval)/sizeof(unsigned int), buffer_keyval );
    for( i=0; i<key_num; i++ )
    {
        //运行到这里已经完成了去抖, 如果不稳定则之前已经返回
        //发送给应用稳定的键值, 并完成按键音映射等功能
        result = process_key_event( buffer_keyval[i], &input_msg );
        if ( true == result )
        {
            if ( 0 != (input_msg.data.kmsg.type & KEY_TYPE_DOWN) )
            {
                if ( s_server_times_id.rise_clk <= 0 )
                {
                    _set_key_rise_clk_domain();

                    s_server_times_id.rise_clk = set_single_shot_timer( \
                            TIME_CONTINUE_RISE_CLK_FROM_KEY_DOWN, \
                            (timer_proc)_proc_singel_timer_rise_clk, NULL );
                }
                else
                {
                    _set_key_rise_clk_domain();

                    restart_timer( s_server_times_id.rise_clk );
                }
            }

#if CFG_KEY_TONE_PLAY_WHEN_HOLD > 0
            /* set key tone play, when key is down long hold */
            if ( 0 != (input_msg.data.kmsg.type & \
                        (KEY_TYPE_DOWN | KEY_TYPE_HOLD)) )
            {
                //power key and hold status, donot play tone
                if ( (CFG_POWER_KEYVAL_WHEN_HOLD_NOT_PLAY_KEY_TONE == input_msg.data.kmsg.val) && \
                        (KEY_TYPE_HOLD == input_msg.data.kmsg.type) )
                {
                }
                else
                {
                    ada_set_key_tone_play();
                }
            }
#else
            if ( 0 != (input_msg.data.kmsg.type & KEY_TYPE_DOWN) )
            {
                ada_set_key_tone_play();
            }
#endif
            else
            {
            }

            /* restart system idle times when user press or release a key */
            start_sys_idle_timers(true);

            /* 如果处于screensaver, 收到按键直接退出, 并设置is_just_exit_screensaver */
            if(is_in_screensaver() == true)
            {
                exit_screensaver();
                is_just_exit_screensaver = true;
                /* ignore the key msg when exit screensaver */
                continue;
            }
            /* 如果is_just_exit_screensaver, 只有再收到down才是有效按键, 否则不发送此按键 */
            else if ( true == is_just_exit_screensaver )
            {
                if ( KEY_TYPE_DOWN == input_msg.data.kmsg.type )
                {
                    is_just_exit_screensaver = false;
                }
                else
                {
                    continue;
                }
            }
            else
            {
            }

            /* send lock message, when keylock status and key is down */
            if ( true == s_hw_status.keylock )
            {
                result = false;
                if (KEY_TYPE_DOWN == input_msg.data.kmsg.type)
                {
                    post_broadcase_msg(MSG_KEYLOCK);
                }
            }
            else
            {
                /* key val is valid, mapping it, and send */
                key_mapping( &input_msg );
                if ( KEY_TYPE_HOLD_UP == input_msg.data.kmsg.type )
                {
                    result = post_key_msg(mq_key_fd, &input_msg, 1);
                }
                else
                {
                    result = post_key_msg(mq_key_fd, &input_msg, 0);
                }
                if ( false == result )
                {
                    print_warning("post msg failed\n");
                }

                /* the key value which has been mapped, search it's shortcut and send message */
                key_shortcut( &input_msg );
            }
        }
    }
    return (int)result;
}


/******************************************************************************/
/*!
 * \par  Description:
 *       按键锁检测函数
 *       detect weather is lock, and send lock or unlock message, when lock switch changed
 * \param[in]
 * \retval
 * 注意
 *******************************************************************************/
static int _msg_keylock_server( void *param )
{
    static int saved_keylock_status = -1;

    key_get_keylock_status( (int *)&s_hw_status.keylock );

    /* lock status changed, we send lock or unlock message */
    if (saved_keylock_status != (int)s_hw_status.keylock)
    {
        /* unlock */
        if (false == s_hw_status.keylock)
        {
            post_broadcase_msg(MSG_KEYUNLOCK);
        }
        /* lock */
        else
        {
            post_broadcase_msg(MSG_KEYLOCK);
        }
    }

    saved_keylock_status = (int)s_hw_status.keylock;

    return saved_keylock_status;
}


/******************************************************************************/
/*!
 * \par  Description:
 *       充电服务函数
 * \param[in]
 * \retval
 * 注意
 *       此函数需要后台0.5秒调用一次, 即使不在充电状态
 *******************************************************************************/
static int _charge_server( void *param )
{
    charge_set_process();
    _msg_low_voltage_server();

    return 1;
}


//shaofeng 插拔usb时, 电量检测问题. 用show的平滑电量??
/******************************************************************************/
/*!
 * \par  Description:
 *       低电检测函数. 根据电量发送低电或者低电关机消息
 *       get battery voltage, send low voltage when battery weaker,
 *       and send poweroff message when much battery weaker
 * \param[in]
 * \retval
 * 注意
 *       此函数0.5秒调用一次
 *       usb插入状态不再进行检测
 *******************************************************************************/
static inline int _msg_low_voltage_server( void )
{
#if CFG_ENABLE_LOW_VOLTAGE_DETECT > 0
    bool result = true;
    charge_voltage_t voltage;
    static int cnt_detect_low_vol = 0;
    static int cnt_broadcase_low_vol = 0;
    static int cnt_power_off = 0;
    static int sum_voltage = 0;
    static int average_voltage = 0;
    bool adapter_is_on = false;

    /* if usb line b is connected, we will not detect low power */
    if ((PLUG_IN == s_hw_status.usb) &&(USB_LINE_B == s_hw_status.usb_linetype))
    {
        cnt_detect_low_vol = 0;
        cnt_broadcase_low_vol = 0;
        cnt_power_off = 0;
        sum_voltage = 0;
        return false;
    }

    /* if adapter is connected, we will not detect low power */
    result = charge_get_adapter( &adapter_is_on );
    if (true == result)
    {
        if ( true == adapter_is_on )
        {
            return false;
        }
    }

    result = charge_get_battery_voltage( &voltage );
    if (false == result)
    {
        return false;
    }

    cnt_detect_low_vol++;
    sum_voltage += voltage;
    /* 达到 检测电池电压时间, 计算平均电压, 如果电压低于关机电压, 发送关机消息 */
    if ( (TIME_DETECT_LOW_VOLTAGE/TIME_DETECT_CHARGE) == cnt_detect_low_vol)
    {
        cnt_detect_low_vol = 0;

        if (0 != CNT_DETECT_LOW_VOLTAGE)
        {
            average_voltage = sum_voltage / CNT_DETECT_LOW_VOLTAGE;
        }

        /* or when low voltage less than VOL_POWER_OFF, we poweroff */
        if (average_voltage <= VOL_POWER_OFF)
        {
            post_broadcase_msg( MSG_POWER_OFF );
        }

        sum_voltage = 0;
    }/* end if (TIME_DETECT_LOW_VOLTAGE/TIME_DETECT_CHARGE == cnt_detect_low_vol) */

    cnt_broadcase_low_vol++;
    /* 达到 发送低电提示的时间, 根据平均电压, 如果电压低, 发送提示消息 */
    if ( (TIME_BROADCAST_MSG_LOW_VOLTAGE/TIME_DETECT_CHARGE) == cnt_broadcase_low_vol)
    {
        cnt_broadcase_low_vol = 0;

        if (average_voltage <= VOL_LOW_VOLTAGE)
        {
            post_broadcase_msg( MSG_LOW_VOLTAGE );

            cnt_power_off++;
            /* when low voltage has CNT_LOW_VOLTAGE_TO_POWER_OFF times, we poweroff */
            if (CNT_LOW_VOLTAGE_TO_POWER_OFF == cnt_power_off)
            {
                cnt_power_off = 0;
                post_broadcase_msg( MSG_POWER_OFF );
            }
        }
        else
        {
            cnt_power_off = 0;
        }
    }/* end if (TIME_BROADCAST_MSG_LOW_VOLTAGE/TIME_DETECT_CHARGE == cnt_broadcase_low_vol) */

    return result;
#else
    return 1;
#endif
}

static int _msg_gsensor_server( void *param )
{
    gsensor_data_t gs_data;
    if(drv_op_gsensor_read(&gs_data) == true)
    {
        process_gsensor_event(&gs_data);
    }
    else
    {
        print_warning("read gsensor data failed!");
    }

    return 0;

}

static bool _get_alarm_clock(alarm_get_clock_param_t * param)
{
    int fd = 0;

    if(NULL == param)
    {
        print_err("input parameter(param) is a NULL pointer");
        return false;
    }

    fd = open(DEVICE_ALARM, O_RDWR);
    if(fd < 0)
    {
        print_err("open %s failed!", DEVICE_ALARM);
        return false;
    }

    if(ioctl(fd, ALARM_GET_CLOCK, (int)param) != 0 )
    {
        print_err("ioctl(ALARM_GET_CLOCK) error!");
        close(fd);
        return false;
    }

    close(fd);

    print_dbg("get clock :(%d,%d)", param->mode_no,param->clock_no);

    return true;
}

/******************************************************************************/
/*!
 * \par  Description:
 *       系统事件处理函数
 *       主要包括: usb adaptor card 等外设的插拔
 * \param[in]
 * \retval
 *******************************************************************************/
static int _sys_event_server(void *param)
{
    bool result = true;
    bool ret = true;
    charge_status_t charge_sts = CHARGE_STOP;
    bool need_send_msg = false;
    bool need_exit_screensaver = false;
    msg_apps_t msg = {0};
    sys_evt_e  evt;
    USB_CONNECTOR_T usb_connect_type;
    int (* card_init)(void) = kernel_sym("card_init");
    bool main_disk_changed = false;
    bool need_gen_plist = false;

    hotplugin_dev_info_t uhost_info;
    hotplugin_dev_info_t card_info;

    hotplugin_dev_type_e main_disk;

    evt =  sys_evt_monitor_get_evt();

    switch ( evt )
    {
        case SYS_EVT_USB_A_IN:
        {
            if (insmod("ucore.ko", 0 ) < 0 )
            {
                print_err("insmod %s failed!", "ucore.ko");
                return -1;
            }
            if (insmod("uhc.ko", 0 ) < 0 )
            {
                print_err("insmod %s failed!", "uhc.ko");
                return -1;
            }
            break;
        }
        case SYS_EVT_USB_A_OUT:
        {
            if (rmmod( "uhc.ko", 0 ) < 0 )
            {
                print_err("rmmod %s failed!", "uhc.ko");
                return -1;
            }
            if (rmmod( "ucore.ko", 0 ) < 0 )
            {
                print_err("rmmod %s failed!", "ucore.ko");
                return -1;
            }
            break;
        }
        case SYS_EVT_USB_HOST_DISK_IN:
        {
            if (insmod( "usb_stor.ko", 0 ) < 0 )
            {
                print_err("insmod %s failed!", "usb_stor.ko");
                return -1;
            }
            break;
        }
        case SYS_EVT_USB_HOST_DISK_READY:
        {

            need_exit_screensaver = true;

            if( (mkdir(EXTERNAL_ROOT, 0777) < 0)
                    && (errno != EEXIST))
            {
                print_err("mkdir %s error", EXTERNAL_ROOT);
            }

            if(sys_mount(DEVICE_USTOR, EXTERNAL_ROOT, "fat", 0, NULL) < 0)
            {
                print_err("mount %s to %s failed!", DEVICE_USTOR, EXTERNAL_ROOT);
                msg.type = MSG_USB_HOST_DISK_IN;
                send_async_msg_to_global_mq(GLOBAL_SYS_MQ_FOR_FOREGROUND_APP, &msg);

                msg.type = MSG_USB_HOST_DISK_INIT_FAILED;
                send_async_msg_to_global_mq(GLOBAL_SYS_MQ_FOR_FOREGROUND_APP, &msg);
            }
            else
            {
                msg.type = MSG_USB_HOST_DISK_IN;
                send_async_msg_to_global_mq(GLOBAL_SYS_MQ_FOR_FOREGROUND_APP, &msg);

                main_disk = sys_get_main_disk();

                if(main_disk == HOTPLUGIN_DEV_TYPE_NONE)
                {
                    sys_set_main_disk(HOTPLUGIN_DEV_TYPE_UHOST);
                    sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_UHOST,DIR_ALL), EXTERNAL_ROOT);
                    main_disk_changed = true;
                    need_gen_plist = true;
                }
                s_hw_status.usb = PLUG_IN;
                s_hw_status.usb_linetype = USB_LINE_A;
            }
            break;
        }
        case SYS_EVT_USB_HOST_DISK_OUT:
        {
            msg.type = MSG_USB_HOST_DISK_OUT;
            need_send_msg = true;
            need_exit_screensaver = true;

            s_hw_status.usb = PLUG_OUT;
            s_hw_status.usb_linetype = USB_LINE_NONE;

            if(sys_umount(EXTERNAL_ROOT,0) < 0)
            {
                print_err("unmount %s failed!", EXTERNAL_ROOT);
            }


            if (rmmod( "usb_stor.ko", 0 ) < 0 )
            {
                print_err("rmmod %s failed!", "usb_stor.ko");
                return -1;
            }
            main_disk = sys_get_main_disk();

            if(main_disk == HOTPLUGIN_DEV_TYPE_UHOST)
            {
                sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_CARD, &card_info);
                if(card_info.access == HOTPLUGIN_DEV_ACCESS_OK)
                {
                    sys_set_main_disk(HOTPLUGIN_DEV_TYPE_CARD);
                    sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CARD,DIR_ALL), CARD_ROOT);
                    main_disk_changed = true;
                    need_gen_plist = true;
                }
                else
                {
                    sys_set_main_disk(HOTPLUGIN_DEV_TYPE_NONE);
                    main_disk_changed = true;
                }
            }

            break;
        }
        case SYS_EVT_USB_B_IN:
        {
            // 获取当前的充电状态,由于下面的启动充电会改变当前
            // 充电状态，因此这里先查询当前状态
            ret = charge_get_status(&charge_sts);

            //启动充电
            charge_set_start(1);
            s_hw_status.usb = PLUG_IN;
            s_hw_status.usb_linetype = USB_LINE_B;

            result = usb_get_connect_type(&usb_connect_type);
            if (true == result)
            {
                if (USB_HOST == usb_connect_type)
                {
                    msg.type = MSG_USB_B_IN;
                }
                else
                {
                    if (ret == true)
                    {
                        if (charge_sts == CHARGE_STOP)
                        {
                            msg.type = MSG_USB_ADAPTOR_IN;
                        }
                        else
                        {
                            break;
                        }
                    }
                }
            }
            else
            {
                break;
            }
            need_send_msg = true;
            need_exit_screensaver = true;
            break;
        }
        case SYS_EVT_USB_B_OUT:
        {
            //停止充电
            //charge_set_start( 0 );
            s_hw_status.usb = PLUG_OUT;
            s_hw_status.usb_linetype = USB_LINE_NONE;

            msg.type = MSG_USB_B_OUT;
            need_send_msg = true;
            need_exit_screensaver = true;
            break;
        }
        case SYS_EVT_ROUND_POWER_ADAPTER_IN:
        {
            ret = charge_get_status(&charge_sts);
            if(ret == true)
                    {
            if (charge_sts == CHARGE_STOP)
            {
                  //启动充电
                charge_set_start( 1 );
                  msg.type = MSG_USB_ADAPTOR_IN;
                need_send_msg = true;
                need_exit_screensaver = true;
            }
          }
            break;
        }
        case SYS_EVT_POWER_ADAPTER_OUT:
        {
              //停止充电
            charge_set_start( 0 );
            need_send_msg = true;
            need_exit_screensaver = true;
            break;
        }

        case SYS_EVT_CARD_IN:
        {
            if(card_init != NULL)
            {
                card_init();
            }
            break;
        }
        case SYS_EVT_CARD_ERR:
        {
            msg.type = MSG_CARD_IN;
            send_async_msg_to_global_mq(GLOBAL_SYS_MQ_FOR_FOREGROUND_APP, &msg);

            msg.type = MSG_CARD_INIT_FAILED;
            send_async_msg_to_global_mq(GLOBAL_SYS_MQ_FOR_FOREGROUND_APP, &msg);
            break;
        }
        case SYS_EVT_CARD_READY:
        {
            need_exit_screensaver = true;

            if( (mkdir(CARD_ROOT, 0777) < 0)
                    && (errno != EEXIST))
            {
                print_err("mkdir %s error", CARD_ROOT);
            }

            if(sys_mount(DEVICE_CARD, CARD_ROOT, "fat", 0, NULL) < 0)
            {
                print_err("mount %s to %s failed!", DEVICE_CARD, CARD_ROOT);

                msg.type = MSG_CARD_IN;
                send_async_msg_to_global_mq(GLOBAL_SYS_MQ_FOR_FOREGROUND_APP, &msg);

                msg.type = MSG_CARD_MOUNT_FAILED;
                send_async_msg_to_global_mq(GLOBAL_SYS_MQ_FOR_FOREGROUND_APP, &msg);
            }
            else
            {
                s_hw_status.card = true;

                msg.type = MSG_CARD_IN;
                send_async_msg_to_global_mq(GLOBAL_SYS_MQ_FOR_FOREGROUND_APP, &msg);

                main_disk = sys_get_main_disk();

                if(main_disk == HOTPLUGIN_DEV_TYPE_NONE)
                {
                    sys_set_main_disk(HOTPLUGIN_DEV_TYPE_CARD);
                    sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CARD,DIR_ALL), CARD_ROOT);
                    main_disk_changed = true;
                    need_gen_plist = true;
                }
            }
            break;
        }
        case SYS_EVT_CARD_OUT:
        {
            s_hw_status.card = false;

            msg.type = MSG_CARD_OUT;
            need_send_msg = true;
            need_exit_screensaver = true;

            if(sys_umount(CARD_ROOT,0) < 0)
            {
                print_err("unmount %s failed!", CARD_ROOT);
            }

            main_disk = sys_get_main_disk();

            if(main_disk == HOTPLUGIN_DEV_TYPE_CARD)
            {
                sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_UHOST, &uhost_info);
                if(uhost_info.access == HOTPLUGIN_DEV_ACCESS_OK)
                {
                    sys_set_main_disk(HOTPLUGIN_DEV_TYPE_UHOST);
                    sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_UHOST,DIR_ALL), EXTERNAL_ROOT);
                    main_disk_changed = true;
                    need_gen_plist = true;
                }
                else
                {
                    sys_set_main_disk(HOTPLUGIN_DEV_TYPE_NONE);
                    main_disk_changed = true;
                }
            }
            break;
        }
        case SYS_EVT_ALARM_IN:
        {
            alarm_get_clock_param_t alarm_param;
            if(( _get_alarm_clock(&alarm_param) == true ) && (alarm_param.mode_no == TVIN_MODE_NO))
            {
                msg.type = MSG_TVIN_ALARM_IN;
                memcpy(msg.content, &alarm_param.clock_no, sizeof(alarm_param.clock_no));
                need_send_msg = true;
                need_exit_screensaver = true;
            }

            break;
        }

        default:
        return -1;
    }

    if ( true == need_send_msg )
    {
        send_async_msg_to_global_mq(GLOBAL_SYS_MQ_FOR_FOREGROUND_APP, &msg);
    }

    if ( true == need_exit_screensaver )
    {
        //复位屏保和省电定时器, 以重新计时
        start_sys_idle_timers( true );

        //退出屏保
        exit_screensaver();
    }

    if(main_disk_changed == true)
    {
        if(need_gen_plist == true)
        {
            msg.type = MSG_NEED_GEN_PLIST;
            send_async_msg_to_global_mq(GLOBAL_SYS_MQ_FOR_FOREGROUND_APP, &msg);
        }

        msg.type = MSG_MAIN_DISK_CHANGED;
        send_async_msg_to_global_mq(GLOBAL_SYS_MQ_FOR_FOREGROUND_APP, &msg);
    }
    return 0;
}


/******************************************************************************/
/*!
 * \par  Description:
 *       处理进程间的系统消息函数
 * \param[in]    msg_apps_t *: 消息指针
 * \retval
 *******************************************************************************/
static int _proc_sys_msg(msg_apps_t *msg)
{
    static int need_set_key_tone_on = 0;
    msg_reply_t reply = {0};
    system_settings_t sys_set;
    unsigned int nouse;
    bool tmp;
    bool result;

    switch(msg->type)
    {
        case MSG_SET_APP_FOREGROUND_SYNC:
        {
            if( is_console == true )
            {
                _start_times();
                is_console = false;
            }

            reply.type = MSG_REPLY_SUCCESS;
            reply_sync_msg((const msg_reply_t *)&reply);

            break;
        }

        case MSG_ACTIVE_SYNC:
        {
            reply.type = MSG_REPLY_SUCCESS;
            reply_sync_msg((const msg_reply_t *)&reply);

            break;
        }

        case MSG_SET_KEY_TONE_ON:
        {
            int flag_on;
            memcpy(&flag_on, msg->content, sizeof(flag_on));

            //如果是setting中设置打开按键音, 但当前处于禁止状态; 则不打开, 待使能时再打开
            if ( (1 == flag_on) && (0 == key_tone_param.enable) )
            {
                need_set_key_tone_on = 1;
                break;
            }
            tmp = ada_set_key_tone_on( flag_on );

            break;
        }

        case MSG_SET_KEY_TONE_ENABLE:
        {
            int flag_enable;

            memcpy(&flag_enable, msg->content, sizeof(flag_enable));
            tmp = ada_set_key_tone_enable(flag_enable);
            //重新使能按键音, 需要重新设置按键音参数
            if ( 1 == flag_enable )
            {
                //处于禁止状态时打开了按键音, 则在使能时才真正打开按键音
                if ( 1 == need_set_key_tone_on )
                {
                    need_set_key_tone_on = 0;
                    ada_set_key_tone_on( 1 );
                }
                //如果已经打开了按键音, 仅需要重新设置按键音参数
                else
                {
                    ada_set_key_tone_param();
                }
            }

            break;
        }

        case MSG_SET_CHARGE_PARAM_SYNC:
        {
            charge_param_t param;

            memcpy(&param, msg->content, sizeof(param));
            charge_set_param( &param );
            reply.type = MSG_REPLY_SUCCESS;
            reply_sync_msg((const msg_reply_t *)&reply);

            break;
        }

        case MSG_SET_CHARGE_START_SYNC:
        {
            int start;

            memcpy(&start, msg->content, sizeof(start));
            charge_set_start( start );
            reply.type = MSG_REPLY_SUCCESS;
            reply_sync_msg((const msg_reply_t *)&reply);

            break;
        }

        case MSG_GET_CHARGE_STATUS_SYNC:
        {
            charge_status_t status;

            charge_get_status( (charge_status_t *)&status );
            memcpy(reply.content, &status, sizeof(status));
            reply.type = MSG_REPLY_SUCCESS;
            reply_sync_msg((const msg_reply_t *)&reply);

            break;
        }

        case MSG_SET_KEY_MAPPING_SYNC:
        {
            key_map_t key_map;

            memcpy(&key_map, msg->content, sizeof(key_map));
            key_mapping_set_status( key_map.key_val_src, key_map.enable );

            break;
        }

        case MSG_SET_KEY_SERVER_ENABLE_SYNC:
        {
            int enable_key_timer = 0;
            memcpy(&enable_key_timer, msg->content, sizeof(enable_key_timer));

            //清除驱动中的按键
            while(1)
            {
                result = key_get_drv_keyval( &nouse );
                if ( false == result )
                {
                    break;
                }
            }

            if ( 0 == enable_key_timer )
            {
                stop_timer( s_server_times_id.key );
            }
            else
            {
                restart_timer( s_server_times_id.key );
            }

            break;
        }

        case MSG_SET_VOLUME_SYNC:
        {
            int volume;

            memcpy(&volume, msg->content, sizeof(volume));
            tmp = ada_set_volume(volume);

            reply.type = MSG_REPLY_SUCCESS;
            memcpy(reply.content, &tmp, sizeof(tmp));
            reply_sync_msg((const msg_reply_t *)&reply);
            break;
        }

        case MSG_GET_VOLUME_SYNC:
        {
            int volume;

            ada_get_volume(&volume);
            memcpy(reply.content, &volume, sizeof(volume));
            reply.type = MSG_REPLY_SUCCESS;
            reply_sync_msg((const msg_reply_t *)&reply);
            break;
        }

        case MSG_GET_HW_STATUS_SYNC:
        {
            key_get_keylock_status( (int *)&s_hw_status.keylock );

            charge_get_adapter( &s_hw_status.adapter );
            charge_get_status( &s_hw_status.charge );
            charge_get_battery_voltage( &s_hw_status.voltage );
            charge_get_battery_voltage_show( &s_hw_status.voltage_show );

            memcpy(reply.content, &s_hw_status, sizeof(s_hw_status));
            reply.type = MSG_REPLY_SUCCESS;
            reply_sync_msg((const msg_reply_t *)&reply);
            break;
        }

        case MSG_SET_SCREENSAVER_TIME_SYNC:
        {
            memcpy(&sys_set, msg->content, sizeof(sys_set));
            print_info("set screensaver time:%d", sys_set.screensaver_time);
            set_sys_timer(SYS_TIMER_TYPE_SCREENSAVER, sys_set.screensaver_time);
            reply.type = MSG_REPLY_SUCCESS;
            reply_sync_msg((const msg_reply_t *)&reply);

            break;
        }

        case MSG_SET_AUTO_STANDBY_TIME_SYNC:
        {
            memcpy(&sys_set, msg->content, sizeof(sys_set));
            print_info("set auto standby time:%d", sys_set.auto_standby_time);
            set_sys_timer(SYS_TIMER_TYPE_STANDBY, sys_set.auto_standby_time);
            reply.type = MSG_REPLY_SUCCESS;
            reply_sync_msg((const msg_reply_t *)&reply);

            break;
        }

        case MSG_SET_TIMED_STUTDOWN_TIME_SYNC:
        {
            memcpy(&sys_set, msg->content, sizeof(sys_set));
            print_info("set timed shutdown time:%d", sys_set.timed_shutdown_time);
            set_sys_timer(SYS_TIMER_TYPE_SHUTDOWN, sys_set.timed_shutdown_time);
            reply.type = MSG_REPLY_SUCCESS;
            reply_sync_msg((const msg_reply_t *)&reply);
            break;
        }

        case MSG_SET_LCD_BACKLIGHT_SYNC:
        {
            memcpy(&sys_set, msg->content, sizeof(sys_set));
            print_info("set lcd backlight:%d", sys_set.lcd_backlight);
            drv_op_lcd_open_dev();
            result = drv_op_lcd_set_backlight(sys_set.lcd_backlight);
            drv_op_lcd_close_dev();

            if(result == true)
            {
                reply.type = MSG_REPLY_SUCCESS;
            }
            else
            {
                reply.type = MSG_REPLY_FAILED;
            }
            reply_sync_msg((const msg_reply_t *)&reply);

            break;
        }

        case MSG_SET_GSENSOR_ON:
        {
            bool on;
            memcpy(&on, msg->content, sizeof(on));

            if(on == true)
            {
                drv_op_gsensor_open_dev();
                drv_op_gsensor_enable();
                restart_timer(s_server_times_id.g_sensor);
            }
            else
            {
                drv_op_gsensor_disable();
                drv_op_gsensor_close_dev();
                stop_timer(s_server_times_id.g_sensor);
            }

            break;
        }

        case MSG_RESET_GSENSOR_SHAKE:
        {
            msg_server_cfg_read(&g_msg_server_cfgs[CFG_TYPE_GSENSOR_SHAKE_SUPPORT]);

            if(strcasecmp(g_msg_server_cfgs[CFG_TYPE_GSENSOR_SHAKE_SUPPORT].text, "on") == 0)
            {
                if(drv_op_gsensor_is_dev_opened() == false)
                {
                    drv_op_gsensor_open_dev();
                    drv_op_gsensor_enable();
                    restart_timer(s_server_times_id.g_sensor);
                }
            }
            else
            {
                if(strcasecmp(g_msg_server_cfgs[CFG_TYPE_SCR_MODE].text, "Auto") != 0)
                {
                    drv_op_gsensor_disable();
                    drv_op_gsensor_close_dev();
                    stop_timer(s_server_times_id.g_sensor);
                }
            }

            break;
        }

        case MSG_RESET_GSENSOR_ROTATION:
        {
            msg_server_cfg_read(&g_msg_server_cfgs[CFG_TYPE_SCR_MODE]);

            if(strcasecmp(g_msg_server_cfgs[CFG_TYPE_SCR_MODE].text, "Auto") == 0)
            {
                if(drv_op_gsensor_is_dev_opened() == false)
                {
                    drv_op_gsensor_open_dev();
                    drv_op_gsensor_enable();
                    restart_timer(s_server_times_id.g_sensor);
                }
            }
            else
            {
                if(strcasecmp(g_msg_server_cfgs[CFG_TYPE_GSENSOR_SHAKE_SUPPORT].text, "on") != 0)
                {
                    drv_op_gsensor_disable();
                    drv_op_gsensor_close_dev();
                    stop_timer(s_server_times_id.g_sensor);
                }
            }

            break;
        }

        case MSG_GET_GSENSOR_ROTATION_SYNC:
        {
            rotation_e rotation;

            rotation = gsensor_get_rotation();
            memcpy(reply.content, &rotation, sizeof(rotation));
            reply.type = MSG_REPLY_SUCCESS;
            reply_sync_msg(&reply);
            break;
        }

        case MSG_RESET_DEFAULT_CFG:
        {
            /* reload configurations */
            msg_server_cfg_read_all(g_msg_server_cfgs, CFG_TYPE_MAX);

            /* reset current system timers running state, screensaver timer, idle timer etc. */
            load_sys_timers_config();
            start_sys_timers(true);

            /* reset current backlight */
            drv_op_lcd_open_dev();
            drv_op_lcd_set_backlight((unsigned int)g_msg_server_cfgs[CFG_TYPE_BRIGHTNESS].value);
            drv_op_lcd_close_dev();

            /* reset current gsensor running state */
            if( (strcasecmp(g_msg_server_cfgs[CFG_TYPE_SCR_MODE].text, "Auto") == 0)
                    ||(strcasecmp(g_msg_server_cfgs[CFG_TYPE_GSENSOR_SHAKE_SUPPORT].text, "on") == 0))
            {
                drv_op_gsensor_open_dev();
                drv_op_gsensor_enable();
                restart_timer(s_server_times_id.g_sensor);
            }
            else
            {
                drv_op_gsensor_disable();
                drv_op_gsensor_close_dev();
                stop_timer(s_server_times_id.g_sensor);
            }

            /* reset current key tone */
            ada_para_init();

            break;
        }

        case MSG_RESET_SYS_IDLE_TIMERS_SYNC:
        {
            start_sys_idle_timers(true);
            reply.type = MSG_REPLY_SUCCESS;
            reply_sync_msg(&reply);

            break;
        }

        case MSG_RESET_AUTO_STANDBY_TIMER_SYNC:
        {
            start_sys_timer(SYS_TIMER_TYPE_STANDBY, true);
            reply.type = MSG_REPLY_SUCCESS;
            reply_sync_msg(&reply);

            break;
        }

        case MSG_RESET_SCREENSAVER_TIMER_SYNC:
        {
            start_sys_timer(SYS_TIMER_TYPE_SCREENSAVER, true);
            reply.type = MSG_REPLY_SUCCESS;
            reply_sync_msg(&reply);

            break;
        }

        case MSG_RESET_TIMED_SHUTDOWN_SYNC:
        {
            check_shutdown_timer();
            reply.type = MSG_REPLY_SUCCESS;
            reply_sync_msg(&reply);

            break;
        }

        case MSG_EXIT_SCREENSAVER_SYNC:
        {
            if(is_in_screensaver() == true)
            {
                exit_screensaver();
            }

            reply.type = MSG_REPLY_SUCCESS;
            reply_sync_msg(&reply);

            break;
        }

        case MSG_ENTER_SCREENSAVER_SYNC:
        {
            if(is_in_screensaver() == false)
            {
                enter_screensaver();
            }

            reply.type = MSG_REPLY_SUCCESS;
            reply_sync_msg(&reply);

            break;
        }

        case MSG_SET_SYS_EVT_DETECT_ON_SYNC:
        {
            bool on;
            memcpy(&on, msg->content, sizeof(on));

            if(on == true)
            {
                if (s_server_times_id.sys_evt > 0)
                {
                    restart_timer(s_server_times_id.sys_evt);
                }
            }
            else
            {
                if (s_server_times_id.sys_evt > 0)
                {
                    stop_timer(s_server_times_id.sys_evt);
                }
            }

            reply.type = MSG_REPLY_SUCCESS;
            reply_sync_msg(&reply);

            break;
        }

        case MSG_APP_QUIT:
        {
            exit_msg_loop();
            print_dbg("received MSG_APP_QUIT, quit running!");
            break;
        }

        default:
        {
            break;
        }
    }

    return 1;
}


/******************************************************************************/
/*!
 * \par  Description:
 *       应用入口函数. application main function
 * \param[in]
 * \retval
 *******************************************************************************/
int main(int argc, const char * argv[])
{
    bool result = true;
    msg_apps_t  msg;

    struct sched_param param;
    int policy;

    pthread_getschedparam( pthread_self(), &policy, &param);
    param.sched_priority += 1;
    pthread_setschedparam( pthread_self(), policy, &param);

    _lib_init();
    result = applib_init(argc, argv, APP_TYPE_CONSOLE);
    if( false == result )
    {
        print_err("applib init error!");
        goto APP_INIT_ERR;
    }

    if (argc > 1)
    {
        // if load launcher by console then desktop scene is background.
        if (!strcasecmp(argv[1], MSG_SERVER_ARGV_CONSOLE))
        {
            is_console = true;
        }
        else
        {
            is_console = false;
        }
    }
    else
    {
        is_console = false;
    }

    result = _init();
    if( false == result )
    {
        print_err("_init error!");
        goto APP_INIT_ERR;
    }

    //    /* 此处为了能够在初始化时先完成msg_server的加载，
    //    而不会造成msg_server的空间放到launcher后面，从而引起内存碎片*/
    //    pthread_getschedparam( pthread_self(), &policy, &param);
    //    param.sched_priority -= 3;
    //    pthread_setschedparam( pthread_self(), policy, &param);

    register_sys_dispatcher( (sys_msg_proc)_proc_sys_msg );
    create_global_msg_queue(GLOBAL_SYS_MQ_FOR_FOREGROUND_APP);

    while(true == get_msg(&msg))
    {
        dispatch_msg(&msg);
    }

    unregister_sys_dispatcher();

    delete_global_msg_queue(GLOBAL_SYS_MQ_FOR_FOREGROUND_APP);
    applib_quit();

    _deinit();

    /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< goto! <<<<<<<<<<<<<<<< goto! <<<<<<<< goto! <<<< goto! << */
    APP_INIT_ERR:

    _lib_deinit();


    return 0;
}

