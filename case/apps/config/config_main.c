/********************************************************************************
 *                              uS213f
 *                        Module: system configuration program
 *                    Copyright(c) 2003-2008 Actions Semiconductor
 *                             All Rights Reserved.
 *
 *  History:
 *  <author>      <time>           <version >            <desc>
 *  nevin.bi      2009-11-03           35                            add comments.
 *********************************************************************************/

#include <include_case.h>
#include <include_psp.h>

#define VOLUME_MIN_VOL   0
#define VOLUME_MAX_VOL   31

#define FONT_PRE_INSTALL_ENABLE 0

typedef struct app_var_s
{
    void* applib_lib;
}app_var_t;


/*
 * In system boot up & power off process, config.app invoke two series of processes.
 * config.app use the invoke_stage_e flag the right process invoking stage.
 */
typedef enum invoke_stage_s
{
    STAGE_INIT,
    STAGE_ANIMON,
    STAGE_PLGENR,
    STAGE_BOOT_FINISH,
    STAGE_LAUNCHER,
    STAGE_MSGSERVER,
    STAGE_ANIMOFF,
    STAGE_SHUTDOWN_FINISH,
} invoke_stage_e;

/*************************************************************************/
static bool _init(void);

static void _deinit(void);

static void _get_sys_configs(void);

static void _set_upgrade_flag(void);

static void _restore_system_setting(void);

static void _store_system_setting(void);

static int _get_vol(void);

static void _install_preload_char_dev_kmodule(void);

static void _install_other_char_dev_kmodule(void);

static void _uninstall_char_dev_kmodule(void);

static void _install_block_dev_kmodule(void);

static void _uninstall_block_dev_kmodule(void);

static void _mount_flash_disk(void);

static void _install_application_module(void);

static void _uninstall_application_module(void);

static int _config_msg_cb(msg_apps_t *msg);

static void _config_sramoc(void);

static bool _drv_op_lcd_open_dev(void);

static bool _drv_op_lcd_set_backlight(unsigned int val);

static bool _drv_op_lcd_enter_standby(void);

static bool _drv_op_lcd_close_dev(void);

static bool _is_block_dev_formated(char *dev);

static bool _set_default_font(void);

static bool _unset_default_font(void);

static void _wait_no_key_press(void);

/*************************************************************************/

static app_var_t app_var;

static int s_dev_fd = -1;

static int s_fb_dev_fd = -1;

static const clk_domain_t s_clk_domain_boot = { CLK_LOAD_HIGH_0, CLK_LOAD_HIGH_0};

static const clk_domain_t s_clk_domain_running = { CLK_LOAD_MIN, CLK_LOAD_LOW_1 };

static invoke_stage_e invoke_stage = STAGE_INIT;

static bool upgrade_flag = false;

static bool plist_flag = false;

static bool poweron_enable = false;

static bool poweroff_enable = false;

//static void* gui_handle = NULL;
static void* apconfig_handle = NULL;
//static void* sqlite_handle = NULL;
//static void* plist_handle = NULL;
//static void* style_handle = NULL;

static int s_fontdes = -1;

/* 从ada 驱动卸载到关机至少需要保证2s(此时间为实验值)，才能消除关机时的啪声*/
static  int s_ada_sleep_time = 2;
/*************************************************************************/

static  bool _init(void)
{
    app_var.applib_lib = dlopen("applib.so",RTLD_NOW);
    if(app_var.applib_lib == NULL)
    {
        return false;
    }

    return true;
}

static void _deinit(void)
{
    if(app_var.applib_lib != NULL)
    {
        dlclose(app_var.applib_lib);
        app_var.applib_lib = NULL;
    }
}


static void _get_sys_configs(void)
{
    int ret = -1;
    int v = 0;

    ret = get_config(CFG_UPGRADE_FLAG, (char*) &v, 4);
    if ((ret != -1) && (v == 1))
    {
        upgrade_flag = true;
    }

    ret = get_config(CFG_NEED_GEN_PLIST, (char*) &v, 4);
    if ((ret != -1) && (v == 1))
    {
        plist_flag = true;
    }

    ret = get_config(CFG_AMINATION_POWERON_ENABLE, (char*) &v, 4);
    if ((ret != -1) && (v == 1))
    {
        poweron_enable = true;
    }

    ret = get_config(CFG_AMINATION_POWEROFF_ENABLE, (char*) &v, 4);
    if ((ret != -1) && (v == 1))
    {
        poweroff_enable = true;
    }

    ret = get_config(CFG_USER_FONT_VALID, (char*) &v, 4);
    if ((ret != -1) && (v == 1))
    {
        _mount_flash_disk();
    }
}

static void _set_upgrade_flag(void)
{
    if (upgrade_flag == true)
    {
        int v = 0;
        set_config(CFG_UPGRADE_FLAG, (char*) &v, 4);
    }
}

static void _restore_system_setting(void)
{
    /*
     * tvout默认为关闭状态
     */
    set_config(CFG_TVOUT, "Off", 3);

    _set_default_font();

    /*
     * 主盘默认为NONE
     */
    sys_set_main_disk(HOTPLUGIN_DEV_TYPE_NONE);
}

static void _store_system_setting(void)
{
    int vol;
    vol = _get_vol();
    set_config(CFG_VOICE_VOLUM, (char*) &vol, 4);
}

static int _get_vol(void)
{
    int vol = 0;
    int fd = 0;
    int ret;

    fd = open("/dev/ada", O_RDWR);

    ret = ioctl(fd, DACIO_GET_VOLUME, (int) &vol);

    if (vol < VOLUME_MIN_VOL)
    {
        vol = VOLUME_MIN_VOL;
    }
    else
    {
        if (vol > VOLUME_MAX_VOL)
        {
            vol = VOLUME_MAX_VOL;
        }
    }

    close(fd);

    return vol;
}

static void _install_preload_char_dev_kmodule(void)
{
    /* 由于spi nor会和g_sensor复用clk和data线,可能导致g_sensor的寄存器被错写,
        因此需要最先加载,以保存原寄存器值 */
    insmod("gsensor.ko", 0);
    sys_mknod(DEVICE_GSENSOR, S_IFCHR, makedev(MAJOR_GSENSOR, 0));

    insmod("pm.ko", 0);
    sys_mknod(DEVICE_PM, S_IFCHR, makedev(MAJOR_PM, 0));

    insmod("lcd.ko", 0);
    sys_mknod(DEVICE_LCD, S_IFCHR, makedev(MAJOR_LCD, 0));

    insmod("fb_drv.ko", 0);
    sys_mknod(DEVICE_FB, S_IFCHR, makedev(MAJOR_FB, 0));

    insmod("fft.ko", 0);
    sys_mknod(DEVICE_FFT, S_IFCHR, makedev(MAJOR_FFT, 0));

    s_fb_dev_fd = open(DEVICE_FB, O_RDWR);
    if (s_fb_dev_fd < 0)
    {
        print_err("open %s failed!", DEVICE_FB);
    }
}

static void _install_other_char_dev_kmodule(void)
{
    umonitor_arg_t umonitor_arg;

    insmod("key.ko", 0);
    sys_mknod(DEVICE_KEY, S_IFCHR, makedev(MAJOR_KEY, 0));

    insmod("charge.ko", 0);
    sys_mknod(DEVICE_CHARGER, S_IFCHR, makedev(MAJOR_CHARGER, 0));

    insmod("fm.ko", 0);
    sys_mknod(DEVICE_FM, S_IFCHR, makedev(MAJOR_FM, 0));

    insmod("rtc.ko", 0);
    sys_mknod(DEVICE_RTC, S_IFCHR, makedev(MAJOR_RTC, 0));

    insmod("kerninfo.ko", 0);
    sys_mknod(DEVICE_KERNINFO, S_IFCHR, makedev(MAJOR_KERNINFO, 0));

    insmod("tvout.ko", 0);
    sys_mknod(DEVICE_TVOUT, S_IFCHR, makedev(MAJOR_TVOUT, 0));

    insmod("alarm.ko", 0);
    sys_mknod("/dev/alarm", S_IFCHR, makedev(MAJOR_ALARM, 0));

    insmod("ada.ko", 0);
    sys_mknod(DEVICE_ADA, S_IFCHR, makedev(MAJOR_ADA, 0));

    insmod("watchdog.ko", 0);
    sys_mknod(DEVICE_WD, S_IFCHR, makedev(MAJOR_WD, 0));

    umonitor_arg.vbus_threshold = VBUS_47;
    umonitor_arg.usbtimer = 500;
    umonitor_arg.chktimer = 100;
    umonitor_arg.use_vbus = 1;
    umonitor_arg.use_bvld = 1;
    umonitor_arg.use_id = 1;
    umonitor_arg.use_dp = 0;

    insmod("umonitor.ko", &umonitor_arg);
    sys_mknod(DEVICE_UMONITOR, S_IFCHR, makedev(MAJOR_UMONITOR, 0));

    /* 创建 升级设备 文件结点 */
    sys_mknod(DEVICE_UPGRADE, S_IFCHR, makedev(MAJOR_UPGRADE, 0));

    /* 创建 usb设备 节点 */
    sys_mknod(DEVICE_MSC, S_IFCHR, makedev(MAJOR_MSC, 0));

    /* 创建 UOC 设备节点 */
//    sys_mknod(DEVICE_UOC, S_IFCHR, makedev(MAJOR_UOC, 0));

    /* 创建usb hotplug设备节点 */
    sys_mknod(DEVICE_HOTPLUG, S_IFCHR, makedev(MAJOR_HOTPLUG, 0));

    /* 创建 MTP 相关节点 */
    sys_mknod(DEVICE_MTPDRM, S_IFCHR, makedev(MAJOR_MTPDRM,0));
    sys_mknod(DEVICE_TUNNEL, S_IFCHR, makedev(MAJOR_TUNNEL,0));

    /* 创建videoin节点,ko加载参数不一样，所有insmod放在不同应用中*/
    sys_mknod(DEVICE_VIDEO_IN, S_IFCHR, makedev(MAJOR_VIDEO_IN, 0));

    /* 创建 DSP 相关节点 */
    sys_mknod(DEVICE_DSP_AE, S_IFCHR, makedev(MAJOR_DSP_AE, 0));

    /* 创建 PCCAM应用相关节点 */
    sys_mknod(DEVICE_UDC, S_IFCHR, makedev(MAJOR_UDC4UVC, 0));
    sys_mknod(DEVICE_UVC, S_IFCHR, makedev(MAJOR_UVC, 0));

    sys_mknod(DEVICE_DSP_AD, S_IFCHR, makedev(MAJOR_DSP_AD, 0));
    
    sys_mknod(DEVICE_ADRM, S_IFCHR, makedev(MAJOR_ADRM, 0));
    /*  调试的时候需要将watchdog关闭，否则一旦单步，系统会立刻重启 */
    /*
     insmod("watchdog.ko", 0);
     sys_mknod(DEVICE_WD, S_IFCHR, makedev(MAJOR_WD, 0));
     */
}

static void _uninstall_char_dev_kmodule(void)
{
    if(s_fb_dev_fd > 0)
    {
        close(s_fb_dev_fd);
    }
    rmmod("umonitor.ko", 0);
    rmmod("alarm.ko", 0);
    rmmod("tvout.ko", 0);
    rmmod("kerninfo.ko", 0);
    rmmod("gsensor.ko", 0);
    rmmod("rtc.ko", 0);
    rmmod("fm.ko", 0);
    rmmod("charge.ko", 0);
    rmmod("key.ko", 0);
    rmmod("fb_drv.ko", 0);
    rmmod("lcd.ko", 0);
    rmmod("fft.ko", 0);
    rmmod("watchdog.ko", 0);
    if(s_ada_sleep_time > 0)
    {
        /* 睡眠s_ada_sleep_time时间。sleep()调用依赖于pm.ko，因此需要在pm.ko驱动卸载前调用 */
        sleep((unsigned int)s_ada_sleep_time);
    }
    rmmod("pm.ko", 0);
}

static void _install_block_dev_kmodule(void)
{
    //  insmod("fs.ko", 0); /* 装载内核文件系统 */

    /*创建设备文件。从设备号意义：支持8个磁盘，每个磁盘有8个分区*/
    sys_mknod(DEVICE_FLASH "u", S_IFBLK, makedev(MAJOR_FLASH, 8*8));

    insmod("card.ko", 0);
    sys_mknod(DEVICE_CARD, S_IFBLK, makedev(MAJOR_CARD, 0));

    sys_mknod(DEVICE_USTOR, S_IFBLK, makedev(MAJOR_USTOR, 0));
}

static void _uninstall_block_dev_kmodule(void)
{
    rmmod("card.ko", 0);
}

static void _mount_flash_disk(void)
{
    static bool already_mounted = false;
    int result;
    hotplugin_dev_type_e main_disk;

    if (already_mounted == false)
    {
        _install_block_dev_kmodule();
        main_disk = sys_get_main_disk();
        if (main_disk == HOTPLUGIN_DEV_TYPE_LOCAL)
        {
            if (_is_block_dev_formated(DEVICE_FLASH "u") == false)
            {
                result = format(DEVICE_FLASH "u", "fat32");
                assert(result == 0);
            }

            sys_mkdir(UDISK_ROOT, 0); /* 创建磁盘挂载目录 */
            /* 将flash第0个磁盘第0个分区挂载到 /mnt/disk0目录中 */
            if(sys_mount(DEVICE_FLASH "u", UDISK_ROOT, "fat", 0 * 8 + 0, NULL) < 0)
            {
                print_err("mount %s to %s error! try to format it ", DEVICE_FLASH "u", UDISK_ROOT);
                result = format(DEVICE_FLASH "u", "fat32");
                assert(result == 0);
                result = sys_mount(DEVICE_FLASH "u", UDISK_ROOT, "fat", 0 * 8 + 0, NULL);
                assert(result == 0);
            }

        }

        already_mounted = true;
    }

}

static void _install_application_module(void)
{
    
//    if (gui_handle == NULL)
//    {
//        gui_handle = dlopen("gui.so", RTLD_LAZY);
//        if (gui_handle == NULL)
//        {
//            print_err("dlopen gui.so failed.");
//        }
//    }
//
//    if (sqlite_handle == NULL)
//    {
//        sqlite_handle = dlopen("sqlite.so", RTLD_LAZY);
//        if (sqlite_handle == NULL)
//        {
//            print_err("dlopen sqlite.so failed.");
//        }
//    }
//
//    if (plist_handle == NULL)
//    {
//        plist_handle = dlopen("playlist.so", RTLD_LAZY);
//        if (plist_handle == NULL)
//        {
//            print_err("dlopen playlist.so failed.");
//        }
//    }
//
//    if (style_handle == NULL)
//    {
//        style_handle = dlopen("style.so", RTLD_LAZY);
//        if (style_handle == NULL)
//        {
//            print_err("dlopen style.so failed.");
//        }
//    }

}

static void _uninstall_application_module(void)
{
//    if (gui_handle != NULL)
//    {
//        dlclose(gui_handle);
//    }
//
    if (apconfig_handle != NULL)
    {
        dlclose(apconfig_handle);
    }
//
//    if (sqlite_handle != NULL)
//    {
//        dlclose(sqlite_handle);
//    }
//
//    if (plist_handle != NULL)
//    {
//        dlclose(plist_handle);
//    }
//
//    if (style_handle != NULL)
//    {
//        dlclose(style_handle);
//    }
}

static int _config_msg_cb(msg_apps_t *msg)
{
    msg_apps_t m;
    msg_reply_t reply;
    bool result;
    hotplugin_dev_type_e main_disk ;
    struct sched_param param;
    int policy;

    print_dbg("msg.type:%d\n", msg->type);
    switch (msg->type)
    {
        case MSG_ACTIVE_SYNC:
        {
            reply.type = MSG_REPLY_SUCCESS;
            reply_sync_msg(&reply);
            //sleep(1);
        }
        break;
        case MSG_SET_APP_FOREGROUND:
        {
            switch (invoke_stage)
            {
                case STAGE_ANIMON:
                {
                    print_dbg(" >>>  STAGE_ANIMON  <<<") ;
                    if ((upgrade_flag == true ) || (plist_flag == true))
                    {
//                        install_other_char_dev_kmodule();
//                        mount_flash_disk();

                        /*
                         * if any other application need to know the upgrade flag,
                         * set set_upgrade_flag() call will change position.
                         */
                        if(upgrade_flag == true)
                        {
                            _set_upgrade_flag();
                        }

                        if(plist_flag == true)
                        {
                            plist_flag = false;
                        }
                        
                        main_disk = sys_get_main_disk();
                        if(main_disk == HOTPLUGIN_DEV_TYPE_LOCAL)
                        {
                            sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_ALL), UDISK_ROOT);
                            m.type = MSG_CREAT_APP_SYNC;
                            strcpy(m.content, APP_NAME_PL_GEN);
                            send_sync_msg(APP_NAME_MANAGER, &m, &reply, NULL);
                        }
                        else
                        {
                            m.type = MSG_SET_APP_FOREGROUND;
                            strcpy(m.content, APP_NAME_CONFIG);
                            send_async_msg(APP_NAME_CONFIG, &m);
                        }
                        invoke_stage = STAGE_PLGENR;
                    }
                    else
                    {
                        while(1)
                        {
                            m.type = MSG_SET_APP_FOREGROUND_SYNC;
                            strcpy(m.content, APP_NAME_LAUNCHER);
                            result = send_sync_msg(APP_NAME_LAUNCHER, &m, &reply, NULL);
                            if( (result == true) && (reply.type == MSG_REPLY_SUCCESS) )
                            {
                                break;
                            }
                            usleep(10000);
                        }

                        main_disk = sys_get_main_disk();

                        if(main_disk == HOTPLUGIN_DEV_TYPE_LOCAL)
                        {
                            sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_ALL), UDISK_ROOT);
                        }

                        while(1)
                        {
                            m.type = MSG_SET_APP_FOREGROUND_SYNC;
                            strcpy(m.content, APP_NAME_MSG_SERV);
                            result = send_sync_msg(APP_NAME_MSG_SERV, &m, &reply, NULL);
                            if( (result == true) && (reply.type == MSG_REPLY_SUCCESS) )
                            {
                                break;
                            }
                            usleep(10000);
                        }

                        _install_application_module();

                        sys_pm_clear_clk_domain(&s_clk_domain_boot);
                        sys_pm_set_clk_domain(&s_clk_domain_running);
                        m.type = MSG_BOOTUP_FINISH;
                        send_async_msg(APP_NAME_MANAGER, &m);

                        pthread_getschedparam( pthread_self(), &policy, &param);
                        param.sched_priority += 1;
                        pthread_setschedparam( pthread_self(), policy, &param);

                        invoke_stage = STAGE_BOOT_FINISH;
                    }
                }
                break;

                case STAGE_PLGENR:
                {
                    print_dbg(" >>>  STAGE_PLGENR <<<") ;
                    m.type = MSG_CREAT_APP_SYNC;
                    strncpy(m.content, APP_NAME_MSG_SERV, MSG_CONTENT_SIZE);
                    send_sync_msg(APP_NAME_MANAGER, &m, &reply, NULL);

                    m.type = MSG_CREAT_APP_SYNC;
                    strcpy(m.content, APP_NAME_LAUNCHER);
                    send_sync_msg(APP_NAME_MANAGER, &m, &reply, NULL);

                    _install_application_module();

                    sys_pm_clear_clk_domain(&s_clk_domain_boot);
                    sys_pm_set_clk_domain(&s_clk_domain_running);
                    m.type = MSG_BOOTUP_FINISH;
                    send_async_msg(APP_NAME_MANAGER, &m);
                
                    pthread_getschedparam( pthread_self(), &policy, &param);
                    param.sched_priority += 1;
                    pthread_setschedparam( pthread_self(), policy, &param);

                    invoke_stage = STAGE_BOOT_FINISH;
                }
                break;

                case STAGE_LAUNCHER:
                {
                    print_dbg(" >>>  STAGE_LAUNCHER  <<<");
                    m.type = MSG_KILL_APP;
                    memset(m.content, 0, sizeof(m.content));
                    strncpy(m.content, APP_NAME_MSG_SERV, MSG_CONTENT_SIZE);
                    send_async_msg(APP_NAME_MANAGER, &m);
                    invoke_stage = STAGE_MSGSERVER;
                }
                break;

                case STAGE_MSGSERVER:
                {
                    print_dbg(" >>>  STAGE_MSGSERVER  <<<");
                    rmmod("ada.ko",0);      // msg_server退出后，立即卸载ada驱动
                    if (poweroff_enable == true)
                    {
                        m.type = MSG_CREAT_APP;
                        memset(m.content, 0, MSG_CONTENT_SIZE);
                        strcpy(m.content, APP_NAME_ANIM_OFF);
                        send_async_msg(APP_NAME_MANAGER, &m);
                        invoke_stage = STAGE_ANIMOFF;
                    }
                    else
                    {
                        exit_msg_loop();
                        invoke_stage = STAGE_SHUTDOWN_FINISH;
                    }
                }
                break;

                case STAGE_ANIMOFF:
                {
                    print_dbg(" >>>  STAGE_ANIMOFF  <<<") ;
                    invoke_stage = STAGE_SHUTDOWN_FINISH;
                    s_ada_sleep_time -= 2;      // 关机动画持续时间2s
                    exit_msg_loop();
                }
                break;

                default:
                break;

            }
        }
        break;
        case MSG_SET_DEF_FONT_SYNC:
        {
            if(_set_default_font() == true)
            {
                reply.type = MSG_REPLY_SUCCESS;
            }
            else
            {
                reply.type = MSG_REPLY_FAILED;
            }

            reply_sync_msg(&reply);
        }
        break;
        case MSG_UNSET_DEF_FONT_SYNC:
        {
            if(_set_default_font() == true)
            {
                reply.type = MSG_REPLY_SUCCESS;
            }
            else
            {
                reply.type = MSG_REPLY_FAILED;
            }

            reply_sync_msg(&reply);
        }
        break;
        case MSG_POWER_OFF:
        {
            filter_msg(MSG_POWER_OFF);
            clear_sys_msg_queue(MSG_POWER_OFF);
            sys_enable_sys_evt_detect(false);

            m.type = MSG_KILL_ALL_USER_APPS_SYNC;
            strncpy(m.content, APP_NAME_CONFIG, MSG_CONTENT_SIZE);
            send_sync_msg(APP_NAME_MANAGER, &m, &reply, NULL);
            if (reply.type == MSG_REPLY_FAILED)
            {
                print_err();
                break;
            }

            _store_system_setting();    /* 应用程序退出后，即保存系统设置*/

            m.type = MSG_KILL_APP;
            strncpy(m.content, APP_NAME_LAUNCHER, MSG_CONTENT_SIZE);
            send_sync_msg(APP_NAME_MANAGER, &m, &reply, NULL);
            invoke_stage = STAGE_LAUNCHER;
        }
        break;
        case MSG_APP_QUIT:
        {
            exit_msg_loop();
        }
        break;
        default:
        break;
    }

    return 0;
}

static void _config_sramoc(void)
{
    act_writel(act_readl(CMU_DEVCLKEN) | (0x1 << 2) | (0x1 << 4) | (0x1 << 16), CMU_DEVCLKEN);
    act_writel((act_readl(SRAMOC_CTL) & (~(0x1 << 19))) | (0xf << 4) | (0x1 << 2), SRAMOC_CTL);
}

static bool _drv_op_lcd_open_dev(void)
{
    s_dev_fd = open(DEVICE_LCD, O_RDWR);

    if (s_dev_fd < 0)
    {
        s_dev_fd = -1;
        print_err("open %s failed!", DEVICE_LCD);
        return false;
    }

    return true;
}

static bool _drv_op_lcd_set_backlight(unsigned int val)
{
    if (-1 == s_dev_fd)
    {
        print_warning("lcd device has not opened!");
        return false;
    }

    print_info("set lcd backlight :%d", val);

    if (ioctl(s_dev_fd, DISPSET_LCD_BACKLIGHT, val) != 0)
    {
        print_err("set lcd backlight failed!");
        return false;
    }

    return true;
}

static bool _is_block_dev_formated(char *dev)
{
    unsigned char buf[512];
    int fd;
    ssize_t size;

    bool formated = true;

    fd = sys_open(dev, O_RDONLY, 0);
    if (fd < 0)
    {
        print_err("sys_open %s failed.",dev);
    }
    else
    {
        size = sys_read((unsigned int)fd, buf, 512);
        if((size <0) || ((size_t)size != sizeof(buf)))
        {
            print_err("sys_read failed");
        }

        sys_close((unsigned int)fd);

        if ((buf[510] != 0x55) || (buf[511] != 0xaa))
        {
            formated = false;
        }
    }
    return formated;
}


static bool _drv_op_lcd_enter_standby(void)
{
    if (-1 == s_dev_fd)
    {
        print_warning("lcd device has not opened!");
        return false;
    }

    if (ioctl(s_dev_fd, DISPSET_LCD_STANDBY, ENTER) != 0)
    {
        print_err("enter lcd standby failed!");
        return false;
    }

    return true;
}

static bool _drv_op_lcd_close_dev(void)
{
    if (-1 != s_dev_fd)
    {
        if (close(s_dev_fd) != 0)
        {
            print_err("close lcd device failed!");
            return false;
        }
        else
        {
            return true;
        }
    }

    print_warning("lcd device has not opened!");
    return false;
}

static bool _set_default_font(void)
{
#if FONT_PRE_INSTALL_ENABLE == 1
    if(s_fontdes > 0)
    {
        return false;
    }

    s_fontdes = gui_create_font(sys_get_default_font_file(), 16);
    if(s_fontdes < 0)
    {
        print_warning("create font failed");

        value = 0;
        if(set_config(CFG_USER_FONT_VALID,(char *)&value, sizeof(value)) < 0 )
        {
            print_warning("set_config (CFG_USER_FONT_VALID) failed!");
            return false;
        }

        /* use system default font file try again */

        s_fontdes = gui_create_font(sys_get_default_font_file(), 16);
        if(s_fontdes < 0)
        {
            print_err("create font failed!");
            return false;
        }
    }

    if(gui_dc_set_default_fontface(s_fontdes) < 0) 
    {
        print_err("gui_dc_set_default_fontface failed");
        return false;
    }
#endif
    return true;
}

static bool _unset_default_font(void)
{
    if(s_fontdes < 0)
    {
        return false;
    }
#if FONT_PRE_INSTALL_ENABLE == 1
    gui_destroy_font(s_fontdes);
    s_fontdes = -1; 
#endif

    return true;
}

static void _wait_no_key_press(void)
{
    int fd;
    int key_val;
    int ret;
    int n;

    unsigned key_val_len = sizeof(key_val);

    fd = open(DEVICE_KEY, O_RDWR, 0);
    if (fd < 0 )
    {
        print_err("open key device failed!");
        return ;
    }

    n = 10;

    while(1)
    {
        usleep(50000);

        ret = read(fd, &key_val, sizeof(key_val_len));
        print_dbg("ret:%d, key_val:%d", ret, key_val);


        /* 没有读到按键 */
        if(ret != (int)key_val_len)
        {
            n--;
            if(n > 0)
            {
                continue;
            }
            else
            {
                /*如果连续10次读不到按键, 则认为没有按键，退出*/
                break;
            }
        }

        /* 按键抬起 */
        if(key_val == 0)
        {
            break;
        }

        n = 10;
    }

    close(fd);

    return;
}

int main(int argc, const char *argv[])
{
    msg_apps_t msg;
    msg_apps_t m;
    int tmp;
    msg_reply_t reply;
    char udisk_volume[PATH_MAX];
    int fd;

    struct sched_param param;
    int policy;

    pthread_getschedparam( pthread_self(), &policy, &param);
    param.sched_priority -= 1;
    pthread_setschedparam( pthread_self(), policy, &param);

    _init();

    applib_init(argc, argv, APP_TYPE_CONSOLE);

    _install_preload_char_dev_kmodule();
    sys_pm_set_clk_domain(&s_clk_domain_boot);

    if (apconfig_handle == NULL)
    {
        apconfig_handle = dlopen("apconfig.so", RTLD_NOW);
        if (apconfig_handle == NULL)
        {
            print_err("dlopen gui.so failed.");
        }
    }

    _get_sys_configs();

    _restore_system_setting();

    if(poweron_enable == true)
    {
        invoke_stage = STAGE_ANIMON;
        m.type = MSG_CREAT_APP_SYNC;
        strcpy(m.content, APP_NAME_ANIM_ON);
        send_sync_msg(APP_NAME_MANAGER, &m, &reply, NULL);
    }
    else
    {
        invoke_stage = STAGE_ANIMON;
        m.type = MSG_SET_APP_FOREGROUND;
        strcpy(m.content, APP_NAME_CONFIG);
        send_async_msg(APP_NAME_CONFIG, &m);
    }

    _install_other_char_dev_kmodule();
    _mount_flash_disk();
    
    
    /* 
     * 写U盘卷标
     * 必须放在_get_sys_configs及_mount_flash_disk之后，_set_upgrade_flag之前
     */
    if(upgrade_flag == true)
    {
        if(get_config(CFG_UDISK_VOLUME, udisk_volume, PATH_MAX) > 0)
        {
            fd = open(UDISK_ROOT, O_RDWR | O_DIRECTORY, 0);
            if( fd >= 0 )
            {
                fcntl(fd, F_SETVLM, (long)udisk_volume);
                close(fd);
            }
        }
    }

    if ((upgrade_flag == false) &&(plist_flag == false) )
    {
        m.type = MSG_CREAT_APP_SYNC;
        strncpy(m.content, APP_NAME_MSG_SERV, MSG_CONTENT_SIZE);
        strcat(m.content, " console");
        send_sync_msg(APP_NAME_MANAGER, &m, &reply, NULL);

        m.type = MSG_CREAT_APP_SYNC;
        strcpy(m.content, APP_NAME_LAUNCHER);
        strcat(m.content, " console");
        send_sync_msg(APP_NAME_MANAGER, &m, &reply, NULL);
    }


    register_sys_dispatcher(_config_msg_cb);

    while (get_msg(&msg) == true)
    {
        dispatch_msg(&msg);
    }
    _unset_default_font();

    /*
     *  we should make lcd enter the standby state first
     */
    _drv_op_lcd_open_dev();
    _drv_op_lcd_set_backlight(0);
    _drv_op_lcd_enter_standby();
    _drv_op_lcd_close_dev();

    if(sync_config() < 0)
    {
        print_err("invoke sync_config() failed!");
    }

    if(nandflash_update() < 0)
    {
        print_err("invoke nandflash_update() failed!");
    }

    _uninstall_application_module();
    _uninstall_block_dev_kmodule();

    /* 等没有按键操作(如用户长按play键后抬起)后，才关机，
     * 以防止长按play关机后，在没有释放play键时又开机*/
    _wait_no_key_press();

    sys_pm_clear_clk_domain(&s_clk_domain_running);
    _uninstall_char_dev_kmodule();

    unregister_sys_dispatcher();

    applib_quit();

    _deinit();

    //进入shutoff驱动, 进入insmod完成硬件shutoff, insmod不会退出
    {
        OS_INT_SAVE_AREA
        OS_ENTER_CRITICAL();
    }

    _config_sramoc();
    //在完全澄清shutoff死机前，请保留如下打印   
    printf("change\n cmu_devclken=%x\n",*(unsigned int *)CMU_DEVCLKEN);
    printf(" SRAMOC_CTL = %x\n",*(unsigned int *)SRAMOC_CTL);
     
    tmp = insmod("shutoff.ko", 0);
    printf("\n\n\nafter insmod shutoff, result:%d..print this is shutoff failed\n\n\n\n", tmp);
    
    return 0;
}

