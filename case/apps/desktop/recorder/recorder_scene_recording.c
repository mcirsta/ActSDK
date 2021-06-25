#include "recorder_scene_recording.h"
#include "recorder_ui_recording.h"
#include "recorder_sty.h"

scene_recording_t *g_scene_recording = NULL;
static char s_format[5] =
{ 0 };

static char recorder_ext[CFG_MAX_BUFF_LENGTH] =
{ 0 };

static const clk_domain_t s_clk_domain =
{ CLK_LOAD_MIN, CLK_LOAD_MAX };

static bool _scene_recording_get_bg_bitmap(void);
static bool _scene_recording_delete_bg_bitmap(void);
static bool _scene_recording_set_default_font(void);
static bool _scene_recording_unset_default_font(void);
static bool _read_config(void);
static bool _write_config(void);
static void _mode_map_to_source(int mode);
static void _quality_map_to_bitrate(char *format, int quality);
static void _gain_map_to_volt(int mode, int gain);
static bool _get_filename(void);
static bool _create_headbar(void);
static bool _create_msgbox(void);
static void _volumebar_callback(volumebar_t *data, volumebar_sig_e signal);
static void _headbar_callback(headbar_t *headbar, int signal);
static void _option_callback(menuview_t *data, int signal);
static void _dialog_callback(dialog_t *dialog, widget_sig_dialog_e signal);
static void _msgbox_callback(msgbox_t * msgbox, msgbox_sig_e signal);
static bool _check_diskinfo(void *data);
static bool _scene_recording_rotate(void);
static bool _recording_proc_sys(msg_apps_t *msg);
static bool _recording_paint(scene_recording_t *data);
static bool _recording_open_theme(void);
static bool _recording_close_theme(void);
static bool _recording_open_str_res(void);
static bool _recording_close_str_res(void);
static void _recording_load_resource(scene_recording_t* data);
static bool _recording_init(scene_recording_t* data);
static bool _recording_deinit(scene_recording_t* data);
static bool _recording_deal_msg(scene_recording_t * data, gui_wm_message_t *pmsg);
static void _recording_callback(gui_wm_message_t *pmsg);

/*卸载dsp编码库*/
void mount_dsp_enc(void)
{
    insmod("dspenc.ko", 0);
    sys_mknod(DEVICE_DSP_AE, S_IFCHR, makedev(MAJOR_DSP_AE, 0));
}

/*卸载dsp解码库*/
void mount_dsp_dec(void)
{
    insmod("dspdec.ko", 0);
    sys_mknod(DEVICE_DSP_AD, S_IFCHR, makedev(MAJOR_DSP_AD, 0));
}

static bool _scene_recording_get_bg_bitmap(void)
{
    if (g_scene_recording->bg_bitmap_info != NULL)
    {
        return false;
    }

    g_scene_recording->bg_bitmap_info = get_user_bg_pic(BG_RECORDER);
    if (NULL == g_scene_recording->bg_bitmap_info)
    {
        /*获取用户自定义背景图失败，使用ui-editor上添加的默认图*/
        g_scene_recording->bg_bitmap_info = sty_create_bitmap((int) g_scene_recording->rec_bg->pic_id);
    }

    if (NULL == g_scene_recording->bg_bitmap_info)
    {
        print_err("get backgroud bitmap failed!");
        return false;
    }

    return true;
}

/*删除背景图*/
static bool _scene_recording_delete_bg_bitmap(void)
{
    if (g_scene_recording->bg_bitmap_info == NULL)
    {
        return false;
    }

    sty_delete_bitmap(g_scene_recording->bg_bitmap_info);
    g_scene_recording->bg_bitmap_info = NULL;

    return true;
}

static bool _scene_recording_set_default_font(void)
{
    /* crate font */
    if (g_scene_recording->fontdes > 0)
    {
        return false;
    }

    g_scene_recording->fontdes = gui_create_font((unsigned char *) sys_get_default_font_file(), 16);
    if (g_scene_recording->fontdes < 0)
    {
        print_err("call gui_create_font failed");
        return false;
    }

    if (gui_dc_set_default_fontface(g_scene_recording->fontdes) < 0)
    {
        print_err("gui_dc_set_default_fontface failed");
        return false;
    }

    return true;
}

static bool _scene_recording_unset_default_font(void)
{
    /* crate font */
    if (g_scene_recording->fontdes <= 0)
    {
        return false;
    }

    gui_destroy_font(g_scene_recording->fontdes);
    g_scene_recording->fontdes = 0;

    return true;
}

#if 0
static bool _check_mic_path(void)
{
    DIR *fp;
    DIR *fp1;
    int result;

    fp = opendir(UDISK_RECORDER);
    if(fp == NULL)
    {
        result = mkdir(UDISK_RECORDER, 0x777);
        if(result < 0)
        {
            return false;
        }
    }
    else
    {
        closedir(fp);
    }

    fp1 = opendir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_RECORDER_MIC));
    if(fp1 == NULL)
    {
        result = mkdir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_RECORDER_MIC), 0x777);
        if(result < 0)
        {
            return false;
        }
    }
    else
    {
        closedir(fp1);
    }

    return true;

}

static bool _check_linein_path(void)
{
    DIR *fp;
    DIR *fp1;
    int result;

    fp = opendir(UDISK_RECORDER);
    if(fp == NULL)
    {
        result = mkdir(UDISK_RECORDER, 0x777);
        if(result < 0)
        {
            return false;
        }
    }
    else
    {
        closedir(fp);
    }

    fp1 = opendir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_RECORDER_LINEIN));
    if(fp1 == NULL)
    {
        result = mkdir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_RECORDER_LINEIN), 0x777);
        if(result < 0)
        {
            return false;
        }
    }
    else
    {
        closedir(fp1);
    }

    return true;

}
#endif

static bool _read_config(void)
{
    int value;

    /*录音支持文件格式,中间以"|"分隔*/
    recorder_ext[0] = '|';
    if (get_config(CFG_APP_SUPPORT_EXT_RECORDER, recorder_ext + 1, CFG_MAX_BUFF_LENGTH - 2) == -1)
    {
        return false;
    }
    strncat(recorder_ext, "|", 1);
    g_scene_recording->support_format = recorder_ext;

    /*LINE_IN录音支持*/
    if (get_config(CFG_RECORDER_LINEIN_SUPPORT, (char *) &value, sizeof(value)) == -1)
    {
        return false;
    }
    else
    {
        g_scene_recording->line_in_support = value;
    }

    /*录音文件格式*/
    if (get_config(CFG_RECORDER_FORMAT, s_format, 5) == -1)
    {
        return false;
    }
    else
    {
        g_scene_recording->format = s_format;
    }

    /*录音质量*/
    if (get_config(CFG_RECORDER_QUALITY, (char *) &value, sizeof(value)) == -1)
    {
        return false;
    }
    else
    {
        g_scene_recording->quality = value;
    }

    /*录音音源*/
    if (get_config(CFG_RECORDER_MODE, (char *) &value, sizeof(value)) == -1)
    {
        return false;
    }
    else
    {
        g_scene_recording->mode = value;
    }

    /*录音增益*/
    if (get_config(CFG_RECORDER_GAIN, (char *) &value, sizeof(value)) == -1)
    {
        return false;
    }
    else
    {
        g_scene_recording->gain = value;
    }

    /*录音文件保存路径*/
    if (get_config(CFG_RECORDER_PATH, (char *) &value, sizeof(value)) == -1)
    {
        return false;
    }
    else
    {
        g_scene_recording->path = value;
    }

    return true;
}

static bool _write_config(void)
{
    if (set_config(CFG_RECORDER_FORMAT, g_scene_recording->format, 5) == -1)
    {
        return false;
    }
    if (set_config(CFG_RECORDER_QUALITY, (const char *) &g_scene_recording->quality, 
        sizeof(g_scene_recording->quality)) == -1)
    {
        return false;
    }
    if (set_config(CFG_RECORDER_MODE, (const char *) &g_scene_recording->mode, sizeof(g_scene_recording->mode)) == -1)
    {
        return false;
    }
    if (set_config(CFG_RECORDER_GAIN, (const char *) &g_scene_recording->gain, sizeof(g_scene_recording->gain)) == -1)
    {
        return false;
    }
    if (set_config(CFG_RECORDER_PATH, (const char *) &g_scene_recording->path, sizeof(g_scene_recording->path)) == -1)
    {
        return false;
    }

    return true;
}


/*根据录音模式设置音源*/
static void _mode_map_to_source(int mode)
{
    if (mode == MIC)
    {
        g_scene_recording->param.source.mic = 1;
        g_scene_recording->param.source.fm = 0;
        g_scene_recording->param.source.line_in = 0;
    }
    if (mode == LINE_IN)
    {
        g_scene_recording->param.source.mic = 0;
        /*LINE_IN和FM硬件复用，故LINE_IN时设置为FM*/
        g_scene_recording->param.source.fm = 1;
        g_scene_recording->param.source.line_in = 0;
    }
}

static void _quality_map_to_bitrate(char *format, int quality)
{
    g_scene_recording->param.bpp = 16;/*精度16bits*/

    if (0 == strcmp(format, "wav"))/*wav参数配置*/
    {
        switch (quality)
        {
            case 0:
            g_scene_recording->param.channel = 1;
            g_scene_recording->param.sample_rate = 8000;
            g_scene_recording->param.bitrate = 32;
            break;

            case 1:
            g_scene_recording->param.channel = 1;
            g_scene_recording->param.sample_rate = 16000;
            g_scene_recording->param.bitrate = 64;
            break;

            case 2:
            g_scene_recording->param.channel = 2;
            g_scene_recording->param.sample_rate = 24000;
            g_scene_recording->param.bitrate = 192;
            break;

            default:
            break;
        }
    }
    else if (0 == strcmp(format, "mp3"))/*mp3参数配置*/
    {
        g_scene_recording->param.channel = 2;/*双声道*/
        switch (quality)
        {
            case 0:
            g_scene_recording->param.sample_rate = 16000;
            g_scene_recording->param.bitrate = 64;
            break;

            case 1:
            g_scene_recording->param.sample_rate = 32000;
            g_scene_recording->param.bitrate = 128;
            break;

            case 2:
            g_scene_recording->param.sample_rate = 32000;
            g_scene_recording->param.bitrate = 256;
            break;

            default:
            break;
        }
    }
    else/*wma参数配置*/
    {
        g_scene_recording->param.channel = 2;/*双声道*/
        g_scene_recording->param.sample_rate = 44100;/*采样频率*/
        switch (quality)
        {
            case 0:
            g_scene_recording->param.bitrate = 64;
            break;

            case 1:
            g_scene_recording->param.bitrate = 128;
            break;

            case 2:
            g_scene_recording->param.bitrate = 256;
            break;

            default:
            break;
        }
    }
}

static void _gain_map_to_volt(int mode, int gain)
{
    if (0 == mode)
    {
        g_scene_recording->param.voice_volt.input_volt = 10;/*mic输入电压以10mv为基准*/
        switch (gain)
        {
            case 1:
            g_scene_recording->param.voice_volt.output_volt = 14;
            break;
            case 2:
            g_scene_recording->param.voice_volt.output_volt = 141;
            break;
            case 3:
            g_scene_recording->param.voice_volt.output_volt = 282;
            break;
            case 4:
            g_scene_recording->param.voice_volt.output_volt = 424;
            break;
            case 5:
            g_scene_recording->param.voice_volt.output_volt = 565;
            break;
            default:
            break;
        }
    }
    else
    {
        g_scene_recording->param.voice_volt.input_volt = 500;/*line in输入电压以500mv为基准*/
        switch (gain)
        {
            case 1:
            g_scene_recording->param.voice_volt.output_volt = 500;
            break;
            case 2:
            g_scene_recording->param.voice_volt.output_volt = 594;
            break;
            case 3:
            g_scene_recording->param.voice_volt.output_volt = 706;
            break;
            case 4:
            g_scene_recording->param.voice_volt.output_volt = 839;
            break;
            case 5:
            g_scene_recording->param.voice_volt.output_volt = 998;
            break;
            default:
            break;
        }
    }
}
#if 0
static bool _get_main_disk(void)
{

    dialog_init_t dialog_init;
    //检查本地存储器在不在
    g_scene_recording->main_disk = sys_get_main_disk();
    if (HOTPLUGIN_DEV_TYPE_NONE == g_scene_recording->main_disk)
    {
        //提示请插入磁盘
        //退出recorder应用
        if (NULL != g_scene_recording->rec_dialog_diskins)
        {
            // initial dialog data
            dialog_init.head = DIALOG_HEAD_WARN;
            dialog_init.str_id = g_scene_recording->rec_dialog_diskins->str_id;

            dialog_modal(&dialog_init);
        }
        /*stop monitor*/
        if (false == stop_monitor(g_scene_recording->mode))
        {
            print_warning("stop monitor err");
        }

        /*  应用退出*/
        g_scene_recording->exit_type = SCENE_RECORDING_NEXT;
        //_recording_quit();
        exit_msg_loop();
        print_dbg("no avriable dis,recorder app quit\n");
        return false;
    }

    return true;
}

/*
 //设置到默认目录，只有在本地存储器存在时使用
 */
void set_path_to_main_disk(int mode, char *file_path)
{
    hotplugin_dev_info_t card_info;
    hotplugin_dev_info_t external_info;
    if (g_scene_recording->mode == MIC)
    {
        if (HOTPLUGIN_DEV_TYPE_LOCAL == g_scene_recording->main_disk)
        {
            sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_RECORDER_MIC), UDISK_ROOT);
            file_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_RECORDER_MIC);
        }
        else if (HOTPLUGIN_DEV_TYPE_CARD == g_scene_recording->main_disk)//主目录是卡
        {
            sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_CARD, &card_info);
            if (card_info.access == HOTPLUGIN_DEV_ACCESS_OK)
            {
                sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CRAD,DIR_RECORDER_MIC), CARD_ROOT);
                file_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CRAD,DIR_RECORDER_MIC);
            }
            else
            {
                sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_RECORDER_MIC), UDISK_ROOT);
                file_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_RECORDER_MIC);
            }
        }
        else//主目录是外部存储器
        {
            sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_UHOST, &external_info);
            if (external_info.access == HOTPLUGIN_DEV_ACCESS_OK)
            {
                sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_UHOST,DIR_RECORDER_MIC), EXTERNAL_ROOT);
                file_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_UHOST,DIR_RECORDER_MIC);
            }
            else
            {
                sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_RECORDER_MIC), UDISK_ROOT);
                file_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_RECORDER_MIC);
            }
        }
    }
    else// if (g_scene_recording->mode == LINE_IN)
    {
        if (HOTPLUGIN_DEV_TYPE_LOCAL == g_scene_recording->main_disk)
        {
            sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_RECORDER_LINEIN), UDISK_ROOT);
            file_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_RECORDER_LINEIN);
        }
        else if (HOTPLUGIN_DEV_TYPE_CARD == g_scene_recording->main_disk)//主目录是卡
        {
            sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_CARD, &card_info);
            if (card_info.access == HOTPLUGIN_DEV_ACCESS_OK)
            {
                sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CARD,DIR_RECORDER_LINEIN), CARD_ROOT);
                file_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CARD,DIR_RECORDER_LINEIN);
            }
            else
            {
                sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_RECORDER_LINEIN), UDISK_ROOT);
                file_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_RECORDER_LINEIN);
            }
        }
        else//主目录是外部存储器
        {
            sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_UHOST, &external_info);
            if (external_info.access == HOTPLUGIN_DEV_ACCESS_OK)
            {
                sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_UHOST,DIR_RECORDER_MIC), EXTERNAL_ROOT);
                file_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_UHOST,DIR_RECORDER_LINEIN);
            }
            else
            {
                sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_RECORDER_MIC), UDISK_ROOT);
                file_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_RECORDER_LINEIN);
            }
        }
    }
}

#endif

/*主盘为内置存储器时的路径设置函数*/
static int _main_disk_local_set_path( void )
{
    const char *file_path = NULL;
    hotplugin_dev_info_t card_info;
    hotplugin_dev_info_t external_info;
    int result;
    
    if (g_scene_recording->mode == MIC)
    {
        if (g_scene_recording->path == INTERNAL_MEMORY)
        {
            sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_RECORDER_MIC), UDISK_ROOT);
            file_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_RECORDER_MIC);
        }
        else if (g_scene_recording->path == CARD_MEMORY)
        {
            sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_CARD, &card_info);
            if (card_info.access == HOTPLUGIN_DEV_ACCESS_OK)
            {
                sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CARD,DIR_RECORDER_MIC), CARD_ROOT);
                file_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CARD,DIR_RECORDER_MIC);
            }
            else
            {
//                set_path_to_main_disk(g_scene_recording->mode,file_path);
                /*卡不在，则设置到内置存储器*/
                sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_RECORDER_MIC), UDISK_ROOT);
                file_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_RECORDER_MIC);
                g_scene_recording->path = INTERNAL_MEMORY;
            }
        }
        else/*g_scene_recording->path == EXTERNAL_MENMORY*/
        {
            sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_UHOST, &external_info);
            if (external_info.access == HOTPLUGIN_DEV_ACCESS_OK)
            {
                sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_UHOST,DIR_RECORDER_MIC), EXTERNAL_ROOT);
                file_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_UHOST,DIR_RECORDER_MIC);
            }
            else
            {
//                set_path_to_main_disk(g_scene_recording->mode,file_path);
                /*外部存储器不在，则设置到内部存储器*/
                sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_RECORDER_MIC), UDISK_ROOT);
                file_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_RECORDER_MIC);
                g_scene_recording->path = INTERNAL_MEMORY;

            }
        }
    }
    else if (g_scene_recording->mode == LINE_IN)
    {
        if (g_scene_recording->path == INTERNAL_MEMORY)
        {
            sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_RECORDER_LINEIN), UDISK_ROOT);
            file_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_RECORDER_LINEIN);
        }
        else if (g_scene_recording->path == CARD_MEMORY)
        {
            sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_CARD, &card_info);
            if (card_info.access == HOTPLUGIN_DEV_ACCESS_OK)
            {
                sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CARD,DIR_RECORDER_LINEIN), CARD_ROOT);
                file_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CARD,DIR_RECORDER_LINEIN);
            }
            else
            {
//                set_path_to_main_disk(g_scene_recording->mode,file_path);
                /*卡不在，则设置到内置存储器*/
                sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_RECORDER_MIC), UDISK_ROOT);
                file_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_RECORDER_LINEIN);
                g_scene_recording->path = INTERNAL_MEMORY;
            }
        }
        else/*g_scene_recording->path == EXTERNAL_MENMORY*/
        {
            sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_UHOST, &external_info);
            if (external_info.access == HOTPLUGIN_DEV_ACCESS_OK)
            {
                sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_UHOST,DIR_RECORDER_LINEIN), EXTERNAL_ROOT);
                file_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_UHOST,DIR_RECORDER_LINEIN);
            }
            else
            {
//                set_path_to_main_disk(g_scene_recording->mode,file_path);
                /*外部存储器不在，则设置到内部存储器*/
                sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_RECORDER_MIC), UDISK_ROOT);
                file_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_RECORDER_LINEIN);
                g_scene_recording->path = INTERNAL_MEMORY;
            }
        }
    }
    else
    {
        print_err("recorder mode err\n");
    }

    result = mmm_ae_cmd(g_scene_recording->ae_handle, MMM_SET_SAVE_PATH, (unsigned int) file_path);
    if (result < 0)
    {
        print_warning("path set failed\n");
        return 0;
    }
    return 1;
}


/*主盘为卡或者外部存储器时的路径设置函数*/
static int _main_disk_other_set_path( void )
{
    const char *file_path = NULL;
    hotplugin_dev_info_t card_info;
    hotplugin_dev_info_t external_info;
    int result;
    /*内置存储器不在，如果存储路径为内置存储器，先更改存储路径*/
    if (g_scene_recording->path == INTERNAL_MEMORY)
    {
        g_scene_recording->path = CARD_MEMORY;
    }
    /*更新移动存储器状态*/
    sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_CARD, &card_info);
    sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_UHOST, &external_info);
    /*如果没有可用存储器，直接返回*/
    if ((card_info.access != HOTPLUGIN_DEV_ACCESS_OK) && (external_info.access != HOTPLUGIN_DEV_ACCESS_OK))
    {
        return -1;
    }
    /*此时至少有一个可用的存储器，卡或者外部存储器*/
    if (g_scene_recording->mode == MIC)
    {
        if (g_scene_recording->path == CARD_MEMORY)
        {
            if (card_info.access == HOTPLUGIN_DEV_ACCESS_OK)
            {
                sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CARD,DIR_RECORDER_MIC), CARD_ROOT);
                file_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CARD,DIR_RECORDER_MIC);
            }
            else
            {
                /*卡不可用，则设置到外部存储器*/
                sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_UHOST,DIR_RECORDER_MIC), EXTERNAL_ROOT);
                file_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_UHOST,DIR_RECORDER_MIC);
                g_scene_recording->path = EXTERNAL_MEMORY;
            }
        }
        else/*external memory*/
        {
            if (external_info.access == HOTPLUGIN_DEV_ACCESS_OK)
            {
                sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_UHOST,DIR_RECORDER_MIC), EXTERNAL_ROOT);
                file_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_UHOST,DIR_RECORDER_MIC);
            }
            else
            {
                /*外部存储器不可用，则设置到卡*/
                sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CARD,DIR_RECORDER_MIC), CARD_ROOT);
                file_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CARD,DIR_RECORDER_MIC);
                g_scene_recording->path = CARD_MEMORY;
            }
        }
    }
    else/*LINE_IN*/
    {
        if (g_scene_recording->path == CARD_MEMORY)
        {
            if (card_info.access == HOTPLUGIN_DEV_ACCESS_OK)
            {
                sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CARD,DIR_RECORDER_LINEIN), CARD_ROOT);
                file_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CARD,DIR_RECORDER_LINEIN);
            }
            else
            {
                /*卡不可用，则设置到外部存储器*/
                sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_UHOST,DIR_RECORDER_LINEIN), EXTERNAL_ROOT);
                file_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_UHOST,DIR_RECORDER_LINEIN);
                g_scene_recording->path = EXTERNAL_MEMORY;
            }
        }
        else/*external memory*/
        {
            if (external_info.access == HOTPLUGIN_DEV_ACCESS_OK)
            {
                sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_UHOST,DIR_RECORDER_LINEIN), EXTERNAL_ROOT);
                file_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_UHOST,DIR_RECORDER_LINEIN);
            }
            else
            {
                /*外部存储器不可用，则设置到卡*/
                sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CARD,DIR_RECORDER_LINEIN), CARD_ROOT);
                file_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CARD,DIR_RECORDER_LINEIN);
                g_scene_recording->path = CARD_MEMORY;
            }
        }
    }
    /*调用中间件命令，设置参数*/
    result = mmm_ae_cmd(g_scene_recording->ae_handle, MMM_SET_SAVE_PATH, (unsigned int) file_path);
    if (result < 0)
    {
        print_warning("path set failed\n");
        return 0;
    }


    return 1;
}


/******************************************************************************/
/*!                    
* \par  Description:
*       设置录音保持文件路径
* \param[in]  无   
* \param[out] 无
*   注意: 
* 返回值:0 :设置路径失败 -1:没有可用路径 1:设置路径成功
*******************************************************************************/

int set_path(void)
{
    int result;    

    print_dbg("g_scene_recording->path == %d\n",g_scene_recording->path);

    /*如果不支持LINE_IN，则修改录音模式*/
    if (0 == g_scene_recording->line_in_support)
    {
        g_scene_recording->mode = MIC;
    }

    if (HOTPLUGIN_DEV_TYPE_LOCAL == g_scene_recording->main_disk)
    {
        /*本地存储器在，则调用主盘为本地的存储器的路径设置函数*/
        result = _main_disk_local_set_path();
    }
    else
    {
        /*主盘不为本地存储器，调用主盘为其他存储器的路径设置函数*/
        result = _main_disk_other_set_path();
    }
    return result;
}

/*退出recorder时关闭音源*/
bool static _close_source(void)
{
    int result;
    g_scene_recording->param.source.mic = 0;
    g_scene_recording->param.source.fm = 0;
    g_scene_recording->param.source.line_in = 0;
    /*设置录音参数*/
    result = mmm_ae_cmd(g_scene_recording->ae_handle, MMM_SET_AUDIO_PARAM, (unsigned int) &g_scene_recording->param);
    if (result < 0)
    {
        print_warning("param set failed\n");
        return false;
    }
    return true;
}

bool set_param(void)
{
    int result;

    /*配置音源*/
    _mode_map_to_source(g_scene_recording->mode);
    /*配置比特率，声道，采样精度，采样频率*/
    _quality_map_to_bitrate(g_scene_recording->format, g_scene_recording->quality);
    /*配置输入电压，输出电压*/
    _gain_map_to_volt(g_scene_recording->mode, g_scene_recording->gain);

    /*设置录音参数*/
    result = mmm_ae_cmd(g_scene_recording->ae_handle, MMM_SET_AUDIO_PARAM, (unsigned int) &g_scene_recording->param);
    if (result < 0)
    {
        print_warning("param set failed\n");
        return false;
    }

    return true;
}

bool set_format(void)
{
    int result;

    /*设置录音格式*/
    result = mmm_ae_cmd(g_scene_recording->ae_handle, MMM_SET_FILE_FMT, (unsigned int) g_scene_recording->format);
    if (result < 0)
    {
        print_warning("file mode set failed\n");
        return false;
    }

    return true;
}

/*打开监听，只有在模式为LINE_IN时有效*/
bool start_monitor(int mode)
{
    int result;

    if (mode == LINE_IN)
    { 
        /*打开监听，监听方式为模拟监听*/
        result = mmm_ae_cmd(g_scene_recording->ae_handle, MMM_START_MONITOR, MMM_ANALOGE_MODE);// MMM_DIGITAL_MODE
        if (result < 0)
        {
            print_warning("start monitor err");
            return false;
        }
    }

    return true;
}

/*关闭监听,只有在模式为LINE_IN时有效*/
bool stop_monitor(int mode)
{
    int result;

    if (mode == LINE_IN)
    {
        result = mmm_ae_cmd(g_scene_recording->ae_handle, MMM_STOP_MONITOR, 0);
        if (result < 0)
        {
            print_warning("stop monitor err");
            return false;
        }
    }

    return true;
}

static bool _get_filename(void)
{
    rtc_time_t time;
    rtc_date_t date;

    bool ret;

    /*! 获取日期时间*/
    ret = sys_get_rtc_date_time(&time, &date);
    if (ret == false)
    {
        print_warning("get rtc date time error");
        return false;
    }

    /*! 保存文件名*/
    sprintf(g_scene_recording->file_name_buf, "%04d%02d%02d%02d%02d%02d", date.year, date.month, date.day, time.hour,
            time.minute, time.second);
    return true;
}

/*****************************************************************************
*停止录音函数:
*完成功能:
*1.获取录音状态，主要是已录时间信息
*2.发送停止录音命令
*3.停止刷新能量信息，已录时间，磁盘空间检查的定时器
*4.设置录音状态
*5.重绘UI
*6.如果当前录音格式是wma格式，还需降频处理
*****************************************************************************/
static void _stop_recording(void)
{
    int result;

    /* read rec time*/
    result = mmm_ae_cmd(g_scene_recording->ae_handle, MMM_GET_RECORDER_STATUS,
            (unsigned int) &g_scene_recording->recording_status);
    if (result < 0)
    {
        print_warning("<<get rec time failed\n\n");
    }

    /* stop recording */
    result = mmm_ae_cmd(g_scene_recording->ae_handle, MMM_STOP_RECORDING, 0);
    if (result < 0)
    {
        print_err("stop recording err\n");
    }

    /* kill timers */
    if (g_scene_recording->vol_energe_timer_id != -1)
    {
        kill_timer(g_scene_recording->vol_energe_timer_id);
        g_scene_recording->vol_energe_timer_id = -1;
    }
    if (g_scene_recording->disk_info_timer_id != -1)
    {
        kill_timer(g_scene_recording->disk_info_timer_id);
        g_scene_recording->disk_info_timer_id = -1;
    }
    if (g_scene_recording->pb_curtime_timer_id != -1)
    {
        kill_timer(g_scene_recording->pb_curtime_timer_id);
        g_scene_recording->pb_curtime_timer_id = -1;
    }

    /* enter standby status and repaint*/
    g_scene_recording->recorder_status = STANDBY;

    g_scene_recording->bg = NOT_NEED_PAINT;
    g_scene_recording->pbbg = NOT_NEED_PAINT;
    g_scene_recording->tape = NOT_NEED_PAINT;

    g_scene_recording->word_status = NEED_PAINT;
    g_scene_recording->filename = NEED_PAINT;
    g_scene_recording->totaltime = NEED_PAINT;
    g_scene_recording->color_status = NEED_PAINT;

    _recording_paint(g_scene_recording);

    if (strcmp(g_scene_recording->format, "wma") == 0)
    {
        sys_pm_clear_clk_domain(&s_clk_domain);
    }

    sys_forbid_auto_standby(false);
}

int set_mmm(void)
{
    bool ret;
    int i_setpath;

    /*设置文件保存路径*/
    i_setpath = set_path();
    if (-1 == i_setpath)
    {
        return -1;
    }
    else if (0 == i_setpath)
    {
        return 0;
    }
    else
    {
    }

    /*设置录音格式*/
    ret = set_format();
    if (ret == false)
    {
        return 0;
    }

    /*设置录音参数*/
    ret = set_param();
    if (ret == false)
    {
        return 0;
    }

    return 1;
}

static bool _create_headbar(void)
{
    headbar_init_t headbar_init;

    headbar_init.type = 0;
    headbar_init.pic_id_head = g_scene_recording->rec_hbar_pic->pic_id;
    headbar_init.str_id_title = g_scene_recording->rec_hbar_str->str_id;

    g_scene_recording->headbar = headbar_create(&headbar_init, (headbar_cb_t) _headbar_callback);
    if (NULL == g_scene_recording->headbar)
    {
        print_warning("headbar_create failed\n\n");
        return false;
    }
    headbar_paint(g_scene_recording->headbar);

    return true;
}

static bool _create_msgbox(void)
{
    g_scene_recording->msgbox = msgbox_create(_msgbox_callback);
    if (NULL == g_scene_recording->msgbox)
    {
        print_err("create msgbox failed!");
        return false;
    }

    return true;
}

static void _volumebar_callback(volumebar_t *data, volumebar_sig_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_VOLUMEBAR_CHANGED:
        print_dbg("SIG_VOL_DEC: %s---%d---\n", __FILE__, __LINE__);
        break;

        case WIDGET_SIG_VOLUMEBAR_QUIT:
        print_dbg("SIG_VOL_DONE: %s---%d---\n", __FILE__, __LINE__);
        volumebar_delete(data);
        g_scene_recording->vbar = NULL;
        break;

        default:
        break;
    }
}

static void _headbar_callback(headbar_t *headbar, int signal)
{
    print_dbg("signal:0x%x", signal);
}

static void _option_callback(menuview_t *data, int signal)
{
    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_QUIT:
        recorder_recording_widget_option_delete(g_scene_recording->option);
        g_scene_recording->option = NULL;
        break;

        default:
        break;
    }
}

static void _dialog_callback(dialog_t *dialog, widget_sig_dialog_e signal)
{
    dialog_init_t dialog_init_savefile;

    switch (signal)
    {
        case WIDGET_SIG_DIALOG_OK:
        print_dbg("delete dialog\n");
        /*! delete dialog */
        if (g_scene_recording->dialog != NULL)
        {
            dialog_delete(g_scene_recording->dialog);
            g_scene_recording->dialog = NULL;
        }
        /*! delete dialog */
        if (g_scene_recording->dialog_query != NULL)
        {
            dialog_delete(g_scene_recording->dialog_query);
            g_scene_recording->dialog_query = NULL;
        }

        /*提示用户，正在保存文件*/
        dialog_init_savefile.head = DIALOG_HEAD_INFO;
        dialog_init_savefile.str_id = g_scene_recording->rec_dialog_savefile->str_id;

        /* dialog init and callback function */
        g_scene_recording->dialog = dialog_create(&dialog_init_savefile, _dialog_callback);
        if (NULL == g_scene_recording->dialog)
        {
            print_err("dialog_create failed\n\n");
        }
        dialog_paint(g_scene_recording->dialog);
        /*停止录音*/
        _stop_recording();
        break;

        case WIDGET_SIG_DIALOG_CANCAL:
        /*! delete dialog */
        if (g_scene_recording->dialog != NULL)
        {
            dialog_delete(g_scene_recording->dialog);
            g_scene_recording->dialog = NULL;
        }
        if (g_scene_recording->dialog_query != NULL)
        {
            dialog_delete(g_scene_recording->dialog_query);
            g_scene_recording->dialog_query = NULL;
        }
        break;

        default:
        break;
    }
}

static void _msgbox_callback(msgbox_t * msgbox, msgbox_sig_e signal)
{
    dialog_init_t dialog_init;

    switch (signal)
    {
        case WIDGET_SIG_MSGBOX_SHOW:
        print_dbg("show message box");
        break;

        case WIDGET_SIG_MSGBOX_QUIT:
        print_dbg("quit message box");
        break;

        case WIDGET_SIG_MSGBOX_QUIT_LOW_POWER:
        if ((g_scene_recording->recorder_status == RECORDING) || (g_scene_recording->recorder_status == PAUSE))
        {
            /*提示用户，正在保存文件*/
            dialog_init.head = DIALOG_HEAD_INFO;
            dialog_init.str_id = g_scene_recording->rec_dialog_savefile->str_id;

            g_scene_recording->dialog = dialog_create(&dialog_init, _dialog_callback);
            if (NULL == g_scene_recording->dialog)
            {
                print_err("dialog_create failed\n\n");
            }
            dialog_paint(g_scene_recording->dialog);

            /*停止录音*/
            _stop_recording();
        }
        print_dbg("quit low power message box");
        break;
        default:
        break;
    }
}


static bool _check_diskinfo(void *data)
{
    int result;
    dialog_init_t dialog_init_disk;
    scene_recording_t * param = (scene_recording_t *) data;

    /*获取中间件状态*/
    result = mmm_ae_cmd(g_scene_recording->ae_handle, MMM_GET_RECORDER_STATUS,
            (unsigned int) &g_scene_recording->recording_status);
    if (result < 0)
    {
        print_warning("<<get rec time failed\n\n");
    }
    /*中间件出错状态，停止录音*/
    if( MMM_RECORDER_ERROR == g_scene_recording->recording_status.status )
    {
        print_err("recorder status error!\n");
        _stop_recording();
        return false;
    }
    /*获取磁盘空间信息*/
    result = mmm_ae_cmd(param->ae_handle, MMM_GET_DISK_INFO, (unsigned int) &param->disk_info);
    if (result < 0)
    {
        print_warning("disk info get error");
        return false;
    }

    /*! 磁盘空间满报警*/
    if ( 0 == param->disk_info.info )
    {

        /*录制过程中，空间不足，提示文件正在保存，回到standby状态再提示磁盘满*/
        if (param->recorder_status == RECORDING)
        {
            print_dbg("\n time left:%d\n cur_time:%d\n totle_time:%d\n",\
                param->disk_info.info,param->recording_status.time,param->total_time_info);
            if(param->recording_status.time >= param->total_time_info)
            {
//                _stop_recording();
            
                dialog_init_disk.head = DIALOG_HEAD_INFO;
                dialog_init_disk.str_id = param->rec_dialog_savefile->str_id;
  
//                dialog_modal(&dialog_init_disk);
                param->dialog = dialog_create(&dialog_init_disk, _dialog_callback);
                if (NULL == param->dialog)
                {
                    print_err("dialog_create failed\n\n");
                    return false;
                }
                dialog_paint(param->dialog);
                gui_screen_update(); 
                sleep(1);
                _stop_recording();
                sleep(1);
                dialog_delete(param->dialog);
                param->dialog = NULL;

                dialog_init_disk.head = DIALOG_HEAD_WARN;
                dialog_init_disk.str_id = param->rec_dialog_diskinfo->str_id;
                param->dialog = dialog_create(&dialog_init_disk, _dialog_callback);
                if (NULL == param->dialog)
                {
                    print_err("dialog_create failed\n\n");
                    return false;
                }

                /* dialog paint */
                dialog_paint(param->dialog);
            }
        }
        else
        {
            /*!启动dialog 报警提示*/

            /* initial dialog data*/
            dialog_init_disk.head = DIALOG_HEAD_WARN;
            dialog_init_disk.str_id = param->rec_dialog_diskinfo->str_id;
            /* dialog init and callback function */
            param->dialog = dialog_create(&dialog_init_disk, _dialog_callback);
            if (NULL == param->dialog)
            {
                print_err("dialog_create failed\n\n");
                return false;
            }

            /* dialog paint */
            dialog_paint(param->dialog);
        }
        return false;
    }
    else
    {
        return true;
    }
}

static bool _scene_recording_rotate(void)
{
    fusion_effect_t eff;

    fusion_get_config(FUSION_TYPE_ROTATE, FUSION_FORWARD, &eff);
    fusion_set_effect(&eff);

    fusion_set_first_frame(NULL);

    _recording_close_theme();

    _recording_open_theme();
    _recording_load_resource(g_scene_recording);

    gui_wm_set_size(g_scene_recording->win_id, g_scene_recording->scene->width, g_scene_recording->scene->height);

    g_scene_recording->bg = NEED_PAINT;
    g_scene_recording->word_status = NEED_PAINT;
    g_scene_recording->filename = NEED_PAINT;
    g_scene_recording->pbbg = NEED_PAINT;
    g_scene_recording->totaltime = NEED_PAINT;
    g_scene_recording->color_status = NEED_PAINT;
    g_scene_recording->tape = NEED_PAINT;

    _recording_paint(g_scene_recording);

    if (g_scene_recording->headbar != NULL)
    {
        headbar_load_style(g_scene_recording->headbar);
        headbar_paint(g_scene_recording->headbar);
    }

    if (g_scene_recording->adjust != NULL)
    {
        adjust_load_style(g_scene_recording->adjust);
        adjust_paint(g_scene_recording->adjust);
    }

    if (g_scene_recording->dialog != NULL)
    {
        dialog_load_style(g_scene_recording->dialog);
        dialog_paint(g_scene_recording->dialog);
    }

    if (g_scene_recording->dialog_query != NULL)
    {
        dialog_load_style(g_scene_recording->dialog_query);
        dialog_paint(g_scene_recording->dialog_query);
    }

    if (g_scene_recording->vbar != NULL)
    {
        volumebar_load_style(g_scene_recording->vbar);
        volumebar_paint(g_scene_recording->vbar);
    }

    if (g_scene_recording->msgbox != NULL)
    {
        msgbox_load_style(g_scene_recording->msgbox);
        msgbox_paint(g_scene_recording->msgbox);
    }

    if (g_scene_recording->option != NULL)
    {
        menuview_load_style(g_scene_recording->option, MENUVIEW_OPTION);
        menuview_paint(g_scene_recording->option);
    }

    fusion_set_last_frame(NULL);
    fusion_display(NULL);

    return true;
}

static bool _recording_proc_sys(msg_apps_t *msg)
{
    msg_reply_t t;
    int result;
    dialog_init_t dialog_init;

    switch (msg->type)
    {
        case MSG_ACTIVE_SYNC:
        t.type = MSG_REPLY_SUCCESS;
        reply_sync_msg(&t);
        break;

        case MSG_GSENSOR_ROTATION_CHANGED:
        _scene_recording_rotate();
        break;

        case MSG_APP_QUIT:
        if (g_scene_recording->recorder_status != STANDBY)
        {
            if (NULL != g_scene_recording->dialog)
            {
                dialog_delete(g_scene_recording->dialog);
                g_scene_recording->dialog = NULL;
            }

            /*停止录音*/
            _stop_recording();

            /*提示用户，正在保存文件*/
            dialog_init.head = DIALOG_HEAD_INFO;
            dialog_init.str_id = g_scene_recording->rec_dialog_savefile->str_id;
            dialog_modal( &dialog_init );
        }

        /*stop monitor*/
        if (false == stop_monitor(g_scene_recording->mode))
        {
            print_warning("stop monitor err");
        }

        /*  应用退出*/
        g_scene_recording->exit_type = SCENE_RECORDING_NEXT;

        exit_msg_loop();
        print_dbg("scene quit\n");

        break;

        case MSG_CARD_OUT:           
        /*先弹提示框*/
        if (g_scene_recording->msgbox != NULL)
        {
            msgbox_proc_sys(g_scene_recording->msgbox, msg);
        }

        /*更新存储路径菜单项*/
        update_savepath();

        /*存储路径处理*/
        if (g_scene_recording->path == CARD_MEMORY)
        {
            if (HOTPLUGIN_DEV_TYPE_LOCAL == g_scene_recording->main_disk)/*本地存储器在*/
            {
                g_scene_recording->path = INTERNAL_MEMORY;
            }
            else
            {
                g_scene_recording->path = EXTERNAL_MEMORY;
            }

            if (g_scene_recording->recorder_status == STANDBY)
            {
                /*设置存储路径，以便刷新可录时间*/
                if (0 == set_path())
                {
                    print_warning("set path err");
                }
                /*重绘可录时间*/
                g_scene_recording->totaltime = NEED_PAINT;
                recorder_draw_totaltime(g_scene_recording);
            }
            else
            {

                /* stop recording */
                result = mmm_ae_cmd(g_scene_recording->ae_handle, MMM_STOP_RECORDING, 0);
                print_dbg("stop record result: %d\n", result);
                if (result < 0)
                {
                    print_err("stop recording err\n");
                }

                /* kill timers */
                if (g_scene_recording->vol_energe_timer_id != -1)
                {
                    kill_timer(g_scene_recording->vol_energe_timer_id);
                    g_scene_recording->vol_energe_timer_id = -1;
                }
                if (g_scene_recording->disk_info_timer_id != -1)
                {
                    kill_timer(g_scene_recording->disk_info_timer_id);
                    g_scene_recording->disk_info_timer_id = -1;
                }
                if (g_scene_recording->pb_curtime_timer_id != -1)
                {
                    kill_timer(g_scene_recording->pb_curtime_timer_id);
                    g_scene_recording->pb_curtime_timer_id = -1;
                }

                /* enter standby status and repaint*/
                g_scene_recording->recorder_status = STANDBY;

                g_scene_recording->bg = NOT_NEED_PAINT;
                g_scene_recording->pbbg = NOT_NEED_PAINT;
                g_scene_recording->tape = NOT_NEED_PAINT;

                g_scene_recording->word_status = NEED_PAINT;
                g_scene_recording->filename = NEED_PAINT;
                g_scene_recording->totaltime = NEED_PAINT;
                g_scene_recording->color_status = NEED_PAINT;

                if (0 == set_path())
                {
                    print_warning("set path err");
                }

                _recording_paint(g_scene_recording);

                if (strcmp(g_scene_recording->format, "wma") == 0)
                {
                    sys_pm_clear_clk_domain(&s_clk_domain);
                }

                sys_forbid_auto_standby(false);
            }
        }
        break;
        case MSG_USB_HOST_DISK_OUT:
        /*先弹提示框*/
        if (g_scene_recording->msgbox != NULL)
        {
            msgbox_proc_sys(g_scene_recording->msgbox, msg);
        }

        /*更新存储路径菜单项*/
        update_savepath();

        /*存储路径处理*/
        if (g_scene_recording->path == EXTERNAL_MEMORY)
        {
            if (HOTPLUGIN_DEV_TYPE_LOCAL == g_scene_recording->main_disk)/*本地存储器在*/
            {
                g_scene_recording->path = INTERNAL_MEMORY;
            }
            else
            {
                g_scene_recording->path = CARD_MEMORY;
            }

            if (g_scene_recording->recorder_status == STANDBY)
            {
                if (0 == set_path())
                {
                    print_warning("set path err");
                }

                g_scene_recording->totaltime = NEED_PAINT;
                recorder_draw_totaltime(g_scene_recording);
            }
            else
            {

                /* stop recording */
                result = mmm_ae_cmd(g_scene_recording->ae_handle, MMM_STOP_RECORDING, 0);
                print_dbg("stop record result: %d\n", result);
                if (result < 0)
                {
                    print_err("stop recording err\n");
                }

                /* kill timers */
                if (g_scene_recording->vol_energe_timer_id != -1)
                {
                    kill_timer(g_scene_recording->vol_energe_timer_id);
                    g_scene_recording->vol_energe_timer_id = -1;
                }
                if (g_scene_recording->disk_info_timer_id != -1)
                {
                    kill_timer(g_scene_recording->disk_info_timer_id);
                    g_scene_recording->disk_info_timer_id = -1;
                }
                if (g_scene_recording->pb_curtime_timer_id != -1)
                {
                    kill_timer(g_scene_recording->pb_curtime_timer_id);
                    g_scene_recording->pb_curtime_timer_id = -1;
                }

                /* enter standby status and repaint*/
                g_scene_recording->recorder_status = STANDBY;

                g_scene_recording->bg = NOT_NEED_PAINT;
                g_scene_recording->pbbg = NOT_NEED_PAINT;
                g_scene_recording->tape = NOT_NEED_PAINT;

                g_scene_recording->word_status = NEED_PAINT;
                g_scene_recording->filename = NEED_PAINT;
                g_scene_recording->totaltime = NEED_PAINT;
                g_scene_recording->color_status = NEED_PAINT;

                if (0 == set_path())
                {
                    print_warning("set path err");
                }

                _recording_paint(g_scene_recording);

                if (strcmp(g_scene_recording->format, "wma") == 0)
                {
                    sys_pm_clear_clk_domain(&s_clk_domain);
                }

                sys_forbid_auto_standby(false);
            }
        }
        break;

        case MSG_USB_HOST_DISK_IN:
        case MSG_CARD_IN:
        /*先弹提示框*/
        if (g_scene_recording->msgbox != NULL)
        {
            msgbox_proc_sys(g_scene_recording->msgbox, msg);
        }
        /*更新存储路径菜单项*/
        update_savepath();

        break;
        default:
        if (msg->type == MSG_LOW_VOLTAGE)
        {
            if ((g_scene_recording->dialog != NULL) && (g_scene_recording->dialog->str_id
                    == g_scene_recording->rec_dialog_lowbat->str_id))
            {
                break;
            }
        }

        if (g_scene_recording->msgbox != NULL)
        {
            msgbox_proc_sys(g_scene_recording->msgbox, msg);
        }
        break;
    }
    return true;
}

/*画录音状态，包括已录时间，可录时间，进入条，能量条，录音状态图标等*/
static bool _recording_paint(scene_recording_t *data)
{
    recorder_draw_bg(data);
    recorder_draw_word_status(data);
    recorder_draw_filename(data);
    recorder_draw_pbbg(data);
    recorder_draw_totaltime(data);
    recorder_draw_pbhl_curtime(data);
    recorder_draw_color_status(data);
    recorder_draw_tape(data);
    recorder_draw_vol(data);

    return true;
}

/******************************************************************************/
/*!
 * \par  Description:
 *       处理邋OK键弹起
 * \param[in]    scene_recording_t *: 场景指针
 * \retval       true:成功; false:失败
 *   实现的功能:
 *       根据当前录音状态设置相应邋OK键弹起响应操作
 *       standby->recording或者recording <-> pause
 *******************************************************************************/
static bool _recording_deal_OK_up(scene_recording_t * data)
{
    int result;
    dialog_init_t dialog_init;

    if (data->recorder_status == STANDBY)
    {
        /*录音之前先做低电压检测,电压低则弹框提示电压低*/
        if (sys_power_threshold_check(POWER_THRESHOLD_VOL_REC) == true)
        {
            if (strcmp(g_scene_recording->format, "wma") == 0)
            {
                sys_pm_set_clk_domain(&s_clk_domain);
            }

            /*设置录音相关参数*/
            result = set_mmm();
            if (result < 0)
            {
                /*提示请插入磁盘*/
                /*退出recorder应用*/
                if (NULL == g_scene_recording->dialog)
                {
                    /* initial dialog data*/
                    dialog_init.head = DIALOG_HEAD_WARN;
                    dialog_init.str_id = g_scene_recording->rec_dialog_diskins->str_id;

                    dialog_modal(&dialog_init);
                }
                /*stop monitor*/
                if (false == stop_monitor(g_scene_recording->mode))
                {
                    print_warning("stop monitor err");
                }

                /*  应用退出*/
                g_scene_recording->exit_type = SCENE_RECORDING_NEXT;

                exit_msg_loop();
                print_dbg("no avriable dis,recorder app quit\n");

                return false;
            }
            else if (0 == result)
            {
                print_warning("mmm set err");
                return false;
            }
            else
            {

            }

            /*get file name*/
            if (false == _get_filename())
            {
                print_warning("mmm set err");
                return false;
            }

            /*检查磁盘空间*/
            if (false == _check_diskinfo(data))
            {
                return false;
            }

            data->word_status = NEED_PAINT;
            data->color_status = NEED_PAINT;
            data->filename = NEED_PAINT;

            data->bg = NOT_NEED_PAINT;
            data->pbbg = NOT_NEED_PAINT;
            data->totaltime = NOT_NEED_PAINT;
            data->tape = NOT_NEED_PAINT;

            data->recorder_status = RECORDING;

            /*开始录音*/
            result = mmm_ae_cmd(data->ae_handle, MMM_START_RECORDING, (unsigned int) data->file_name_buf);
            if (result < 0)
            {
                print_err("start recording error");
                return false;
            }

            /*pbhl, curtime, voice energe 用timer来控制绘图*/
            data->disk_info_timer_id = set_timer(300, (timer_proc) _check_diskinfo, (void *) data);
            data->vol_energe_timer_id = set_timer(500, (timer_proc) recorder_draw_vol, (void *) data);
            data->pb_curtime_timer_id = set_timer(250, (timer_proc) recorder_draw_pbhl_curtime, (void *) data);

            _recording_paint(data);

            sys_forbid_auto_standby(true);
            print_info("ok quit<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
        }
        else
        {
            /* initial dialog data*/
            dialog_init.head = DIALOG_HEAD_LOWBAT;
            dialog_init.str_id = data->rec_dialog_lowbat->str_id;

            /* dialog init and callback function*/
            data->dialog = dialog_create(&dialog_init, _dialog_callback);
            if (NULL == data->dialog)
            {
                print_err("dialog_create failed\n\n");
                return false;
            }
            dialog_paint(data->dialog);
        }

    }
    else if (data->recorder_status == RECORDING)
    {
        data->recorder_status = PAUSE;

        data->word_status = NEED_PAINT;
        data->color_status = NEED_PAINT;

        data->bg = NOT_NEED_PAINT;
        data->filename = NOT_NEED_PAINT;
        data->pbbg = NOT_NEED_PAINT;
        data->totaltime = NOT_NEED_PAINT;
        data->tape = NOT_NEED_PAINT;

        result = mmm_ae_cmd(data->ae_handle, MMM_PAUSE_RECORDING, 0);
        if (result < 0)
        {
            print_err("pause recording error");
            return false;
        }

        stop_timer(data->vol_energe_timer_id);
        stop_timer(data->pb_curtime_timer_id);
        stop_timer(data->disk_info_timer_id);

        _recording_paint(data);       
    }
    else if (data->recorder_status == PAUSE)
    {
        data->recorder_status = RECORDING;

        data->word_status = NEED_PAINT;
        data->color_status = NEED_PAINT;

        data->bg = NOT_NEED_PAINT;
        data->filename = NOT_NEED_PAINT;
        data->pbbg = NOT_NEED_PAINT;
        data->totaltime = NOT_NEED_PAINT;
        data->tape = NOT_NEED_PAINT;

        result = mmm_ae_cmd(data->ae_handle, MMM_RESUME_RECORDING, 0);
        if (result < 0)
        {
            print_err("resume recording error");
            return false;
        }
        print_info("resume recording result : %d", result);

        restart_timer(data->vol_energe_timer_id);
        restart_timer(data->pb_curtime_timer_id);
        restart_timer(data->disk_info_timer_id);
        print_info();

        _recording_paint(data);
    }
    else
    {
        print_warning("unknow recorder status\n");
    }

    return true;
}

static bool _recording_deal_msg(scene_recording_t * data, gui_wm_message_t *pmsg)
{
    key_event_t *kmsg;
    dialog_init_t dialog_init;

    switch (pmsg->msgid)
    {
        case WM_MSG_CREATE:
        print_info();
        break;

        case WM_MSG_PAINT:

        break;

        case WM_MSG_KEY:
        print_info("%d\n", pmsg->msgid);
        kmsg = (key_event_t *) (pmsg->data.p);

        switch (kmsg->val | kmsg->type)
        {
            case KEY_OK | KEY_TYPE_DOWN:
            if (data->ok_down_flag == 0)
            {
                data->ok_down_flag = 1;
            }
            break;

            case KEY_OK | KEY_TYPE_SHORT_UP:
            if (data->ok_down_flag == 1)
            {
                data->ok_down_flag = 0;
                if (false == _recording_deal_OK_up(data))
                {
                    return false;
                }
            }
            /*清除按键队列*/
            clear_key_msg_queue(-1, -1);
            break;

            case KEY_RETURN | KEY_TYPE_LONG_UP:
            case KEY_RETURN | KEY_TYPE_SHORT_UP:
            case KEY_RETURN | KEY_TYPE_HOLD_UP:
            if ((data->recorder_status == PAUSE) || (data->recorder_status == RECORDING))
            {
                /* initial dialog data*/
                dialog_init.head = DIALOG_HEAD_QUERY;
                dialog_init.str_id = data->rec_dialog_stoprec->str_id;

                /* dialog init and callback function */
                data->dialog_query = dialog_create(&dialog_init, _dialog_callback);
                if (NULL == data->dialog_query)
                {
                    print_err("dialog_create failed\n\n");
                    return false;
                }

                /* dialog paint */
                dialog_paint(data->dialog_query);
            }
            else if (data->recorder_status == STANDBY)
            {
                /*stop monitor*/
                if (false == stop_monitor(g_scene_recording->mode))
                {
                    print_warning("stop_monitor err\n");
                }

                /*  应用退出*/
                data->exit_type = SCENE_RECORDING_NEXT;
                exit_msg_loop();
                print_dbg("scene quit\n");
            }
            else
            {
                print_warning("unknow recorder status\n");
            }
            /*清除按键队列*/
            clear_key_msg_queue(-1, -1);

            break;

            case KEY_VOL | KEY_TYPE_DOWN:
            case KEY_VSUB | KEY_TYPE_DOWN:
            case KEY_VADD | KEY_TYPE_DOWN:
            {
                print_info();
                data->vbar = volumebar_create(0, _volumebar_callback);
                print_info("%d", data->vbar);
                if (data->vbar == NULL)
                {
                    printf("--: %s---%d---\n", __FILE__, __LINE__);
                    return false;
                }
                else
                {
                    volumebar_paint(data->vbar);
                }
            }
            break;
            case KEY_MENU | KEY_TYPE_DOWN:
            if (data->recorder_status == STANDBY)
            {
                /*创建option菜单*/
                if (data->option == NULL)
                {
                    data->option = recorder_recording_widget_option_create((menuview_cb_t) _option_callback);

                    if (data->option == NULL)
                    {
                        print_err();
                        return false;
                    }
                }
            }
            else if ((data->recorder_status == PAUSE) || (data->recorder_status == RECORDING))
            {
                /*提示文件正在保存*/
                dialog_init.head = DIALOG_HEAD_INFO;
                dialog_init.str_id = data->rec_dialog_savefile->str_id;

                /* dialog init and callback function */
                data->dialog = dialog_create(&dialog_init, _dialog_callback);
                if (NULL == data->dialog)
                {
                    print_err("dialog_create failed\n\n");
                    return false;
                }

                dialog_paint(data->dialog);

                _stop_recording();
            }
            else
            {
                print_warning("unknow recorder status\n");
            }
            /*清除按键队列*/
            clear_key_msg_queue(-1, -1);
            break;

            default:
            break;
        }
        default:
        gui_wm_default_callback(pmsg);
        break;
    }
    return true;
}

static void _recording_callback(gui_wm_message_t *pmsg)
{
    int top;

    top = canvas_get_top();
    /*dispatch msg*/
    if ((g_scene_recording->vbar != NULL) && (top == g_scene_recording->vbar->canvas_id))
    {
        volumebar_proc_gui(g_scene_recording->vbar, pmsg);
        return;
    }
    if ((g_scene_recording->option != NULL) && (top == g_scene_recording->option->canvas_id))
    {
        menuview_proc_gui(g_scene_recording->option, pmsg);
        return;
    }
    if ((g_scene_recording->dialog != NULL) && (top == g_scene_recording->dialog->canvas_id))
    {
        dialog_proc_gui(g_scene_recording->dialog, pmsg);
        return;
    }
    if ((g_scene_recording->dialog_query != NULL) && (top == g_scene_recording->dialog_query->canvas_id))
    {
        dialog_proc_gui(g_scene_recording->dialog_query, pmsg);
        return;
    }
    if ((g_scene_recording->adjust != NULL) && (top == g_scene_recording->adjust->canvas_id))
    {
        adjust_proc_gui(g_scene_recording->adjust, pmsg);
        return;
    }
    if ((g_scene_recording->msgbox != NULL) && (top == g_scene_recording->msgbox->canvas_id))
    {
        msgbox_proc_gui(g_scene_recording->msgbox, pmsg);
        return;
    }

    _recording_deal_msg(g_scene_recording, pmsg);

}

static bool _recording_open_theme(void)
{
    rotation_e angle;
    char file[PATH_MAX];

    /* 取视图角度*/
    angle = sty_get_view_rotation();

    /* 设置屏幕方向*/
    sty_set_screen_rotation(angle);

    /* 载入commonui的 style 场景*/
    if ((angle == ROTATION_0) || (angle == ROTATION_180))
    {
        /* 获取commonui 的 style 资源文件名*/
        g_scene_recording->common_scene = load_scene(app_data.common_style, COMMONUI_SIDELONG_SCENE);

        /* 获取应用的 style 资源文件名*/
        g_scene_recording->scene = load_scene(app_data.recorder_style, RECORDING_SIDELONG_SCENE);
    }
    else
    /*if ( (angle == ROTATION_90) || (angle == ROTATION_270) )*/
    {
        /* 获取commonui 的 style 资源文件名*/
        g_scene_recording->common_scene = load_scene(app_data.common_style, COMMONUI_UPRIGHT_SCENE);

        /* 获取应用的 style 资源文件名*/
        g_scene_recording->scene = load_scene(app_data.recorder_style, RECORDING_UPRIGHT_SCENE);
    }

    if ((g_scene_recording->common_scene == NULL) || (g_scene_recording->scene == NULL))
    {
        print_err("load commonui and application scene failed");
        return false;
    }
    else
    {
        sty_set_common_scene(g_scene_recording->common_scene);
        sty_set_app_scene(g_scene_recording->scene);
    }

    /* 获取commonui 的图片资源文件名*/
    sprintf(file, "%s/%s.res", sys_get_common_theme_res_dir(), "commonui");

    /* 打开 commonui 图片资源文件*/
    if (sty_open_common_pic_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }

    /* 获取commonui 的图片资源文件名*/
    sprintf(file, "%s/%s.res", sys_get_app_theme_res_dir(), "recorder");

    /* 打开应用的图片资源文件*/
    if (sty_open_app_pic_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }

    return true;

}

static bool _recording_close_theme(void)
{

    free_scene(g_scene_recording->common_scene);
    g_scene_recording->common_scene = NULL;

    free_scene(g_scene_recording->scene);
    g_scene_recording->scene = NULL;

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

static bool _recording_open_str_res(void)
{
    char file[PATH_MAX];

    /* 获取commonui 字符串资源文件名*/
    sprintf(file, "%s/%s_%s.str", sys_get_common_string_res_dir(), "commonui", sys_get_cur_language_codes());
    print_dbg("load %s", file);
    /* 打开 commonui 字符串资源文件*/
    if (sty_open_common_str_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }

    /* 获取setting应用字符串资源文件名*/
    sprintf(file, "%s/%s_%s.str", sys_get_app_string_res_dir(), "recorder", sys_get_cur_language_codes());
    print_dbg("load %s", file);
    /* 打开应用的字符串资源文件*/
    if (sty_open_app_str_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }

    /* 设置语言编码*/
    sty_set_text_encoding(GUI_UTF8_ENC);

    /* 设置语言模式*/
    gui_set_language_mode(sys_get_cur_language_id());

    return true;
}

static bool _recording_close_str_res(void)
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

/*
 * 设置当前场景全部区域作为特效的第一帧
 */
static void _scene_recording_fusion_set_first_frame(scene_t *scene)
{
    fusion_frame_t frame;

    frame.rect.x = scene->x;
    frame.rect.y = scene->y;
    frame.rect.width = scene->width;
    frame.rect.height = scene->height;
    frame.pdata = NULL;
    frame.alpha = false;

    fusion_set_first_frame(&frame);
}

/*
 * 设置当前场景全部区域作为特效的最后一帧
 */
static void _scene_recording_fusion_set_last_frame(scene_t *scene)
{
    fusion_frame_t frame;

    frame.rect.x = scene->x;
    frame.rect.y = scene->y;
    frame.rect.width = scene->width;
    frame.rect.height = scene->height;
    frame.pdata = NULL;
    frame.alpha = false;

    fusion_set_last_frame(&frame);
}

static void _recording_load_resource(scene_recording_t* data)
{
    /*load recorder resource*/
    data->rec_bg = (picture_resource_t *) get_scene_child(data->scene, PIC_BG);
    data->rec_word_status = (picregion_resource_t *) get_scene_child(data->scene, PIC_WORD_STATUS);
    data->rec_filename = (string_resource_t *) get_scene_child(data->scene, STR_FILENAME);
    data->rec_pbbg = (picture_resource_t *) get_scene_child(data->scene, PIC_PBBG);
    data->rec_pb = (picture_resource_t *) get_scene_child(data->scene, PIC_PB);
    data->rec_curtime = (string_resource_t *) get_scene_child(data->scene, STR_CURTIME);
    data->rec_totaltime = (string_resource_t *) get_scene_child(data->scene, STR_TOTALTIME);
    data->rec_color_status = (picregion_resource_t *) get_scene_child(data->scene, PIC_COLOR_STATUS);
    data->rec_tape = (picture_resource_t *) get_scene_child(data->scene, PIC_TAPE);
    data->rec_vol_bg = (picture_resource_t *) get_scene_child(data->scene, PIC_VOL_BG);
    data->rec_vol1 = (picture_resource_t *) get_scene_child(data->scene, PIC_VOL1);
    data->rec_vol2 = (picture_resource_t *) get_scene_child(data->scene, PIC_VOL2);
    data->rec_vol3 = (picture_resource_t *) get_scene_child(data->scene, PIC_VOL3);
    data->rec_vol4 = (picture_resource_t *) get_scene_child(data->scene, PIC_VOL4);
    data->rec_vol5 = (picture_resource_t *) get_scene_child(data->scene, PIC_VOL5);

    /*load option select icon*/
    data->rec_select = (picture_resource_t *) get_scene_child(data->scene, PIC_SELECT);

    /*load headbar resource*/
    data->rec_hbar = (resgroup_resource_t *) get_scene_child(data->scene, WIDGET_HEADBAR_RESGROUP);
    data->rec_hbar_pic = (picture_resource_t *) get_resgroup_child(data->rec_hbar, PIC_HBAR);
    data->rec_hbar_str = (string_resource_t *) get_resgroup_child(data->rec_hbar, STR_HBAR);

    /* load dialog initial resource */
    data->rec_dialog = (resgroup_resource_t *) get_scene_child(data->scene, WIDGET_DIALOG_RESGROUP);
    data->rec_dialog_stoprec = (string_resource_t *) get_resgroup_child(data->rec_dialog, STR_DIALOG_STOPREC);
    data->rec_dialog_diskinfo = (string_resource_t *) get_resgroup_child(data->rec_dialog, STR_DIALOG_DISKINFO);
    data->rec_dialog_savefile = (string_resource_t *) get_resgroup_child(data->rec_dialog, STR_DIALOG_SAVEFILE);
    data->rec_dialog_lowbat = (string_resource_t *) get_resgroup_child(data->rec_dialog, STR_DIALOG_LOWBAT);
    data->rec_dialog_diskins = (string_resource_t *) get_resgroup_child(data->rec_dialog, STR_DIALOG_DISKINS);
    data->rec_dialog_nodisk = (string_resource_t *) get_resgroup_child(data->rec_dialog, STR_DIALOG_NODISK);

    if (data->bg_bitmap_info != NULL)
    {
        _scene_recording_delete_bg_bitmap();
    }

    _scene_recording_get_bg_bitmap();
}

static bool _recording_init(scene_recording_t* data)
{
    int ret;
    fusion_effect_t eff;
    dialog_init_t dialog_init;

    g_scene_recording = data;

    print_info("<<<<<<<<<<<<<<<<<<<<<<<<<< scene recording init enter\n");

    /* load theme 资源文件 */
    if (_recording_open_theme() == false)
    {
        return false;
    }

    /* load 字符串资源文件 */
    if (_recording_open_str_res() == false)
    {
        return false;
    }

    /*创建字体*/
    if (_scene_recording_set_default_font() == false)
    {
        return false;
    }

    /*load recorder resource*/
    _recording_load_resource(data);

    /*! init resource status */
    data->bg = NEED_PAINT;
    data->word_status = NEED_PAINT;
    data->filename = NOT_NEED_PAINT;
    data->pbbg = NEED_PAINT;
    data->totaltime = NEED_PAINT;
    data->color_status = NEED_PAINT;
    data->tape = NEED_PAINT;
    data->recorder_status = STANDBY;

    data->main_disk = sys_get_main_disk();
    if (HOTPLUGIN_DEV_TYPE_NONE == data->main_disk)
    {
        /*提示无存储器*/
        /*退出recorder应用*/
        if (NULL != data->rec_dialog_nodisk)
        {
            dialog_init.head = DIALOG_HEAD_WARN;
            dialog_init.str_id = data->rec_dialog_nodisk->str_id;

            dialog_modal(&dialog_init);
        }

        /*  应用退出*/
        data->exit_type = SCENE_RECORDING_NEXT;
        exit_msg_loop();
        print_dbg("no avriable disk,recorder app quit\n");
        return false;
    }
    
    data->win_id = gui_wm_create_window(data->scene->x, data->scene->y, data->scene->width, data->scene->height,
            WM_CREATE_SHOW, _recording_callback, 0);
    if (data->win_id <= 0)
    {
        print_err("gui create window failed");
        return false;
    }
    data->hdc = gui_dc_get( (unsigned short)data->win_id );

    /*设置焦点*/
    gui_wm_set_focus(data->win_id);
    /*读配置项*/
    _read_config();

    if (strcmp(data->format, "wma") == 0)
    {
        mount_dsp_dec();

        /*加载中间件*/
        data->fptr = dlopen("a_record.so", RTLD_LAZY);
    }
    else
    {
        mount_dsp_enc();

        /*加载中间件*/
        data->fptr = dlopen("a_codec.so", RTLD_LAZY);
    }
    /*打开中间件*/
    data->ae_handle = mmm_ae_open(NULL);
    if (data->ae_handle == NULL)
    {
        print_err("mmm open failed\n");
        return false;
    }

    /*设置录音能量等级*/
    /*能量等级为5，值域范围[0 - 31]*/
    ret = mmm_ae_cmd(data->ae_handle, MMM_SET_ENERGY_LEVEL, 5);
    if (ret < 0)
    {
        print_warning("energy level set failed\n");
        return false;
    }

    /*此处设置录音相关参数是为了检测磁盘，画total time*/
    ret = set_mmm();
    if ( ret <= 0 )
    {
        print_err("set mmm error!!!\n");

        return false;

    }

    /*打开监听*/
    if (false == start_monitor(data->mode))
    {
        return false;
    }

    fusion_get_config(FUSION_TYPE_APP_CHANGE, FUSION_FORWARD, &eff);
    fusion_set_effect(&eff);

    _scene_recording_fusion_set_first_frame(data->scene);

    if (false == _create_headbar())
    {
        return false;
    }

    if (false == _create_msgbox())
    {
        return false;
    }

    _recording_paint(data);
    _scene_recording_fusion_set_last_frame(data->scene);
    fusion_display(NULL);

    sys_forbid_auto_standby(false);

    /*清除按键队列*/
    clear_key_msg_queue(-1, -1);
    return true;
}

static bool _recording_deinit(scene_recording_t* data)
{
    int ret;

    _write_config();

    /* kill timers */
    if (data->vol_energe_timer_id != -1)
    {
        kill_timer(data->vol_energe_timer_id);
        data->vol_energe_timer_id = -1;
    }
    if (data->disk_info_timer_id != -1)
    {
        kill_timer(data->disk_info_timer_id);
        data->disk_info_timer_id = -1;
    }
    if (data->pb_curtime_timer_id != -1)
    {
        kill_timer(data->pb_curtime_timer_id);
        data->pb_curtime_timer_id = -1;
    }

    /*删除headbar*/
    if (data->headbar != NULL)
    {
        headbar_delete(data->headbar);
        data->headbar = NULL;
    }

    /*删除volumebar*/
    if (data->vbar != NULL)
    {
        volumebar_delete(data->vbar);
        data->vbar = NULL;
    }

    print_dbg("delete dialog");

    /*删除dialog*/
    if (data->dialog != NULL)
    {
        dialog_delete(data->dialog);
        data->dialog = NULL;
    }
    if (data->dialog_query != NULL)
    {
        dialog_delete(data->dialog_query);
        data->dialog_query = NULL;
    }

    /*删除adjust*/
    if (data->adjust != NULL)
    {
        adjust_delete(data->adjust);
        data->adjust = NULL;
    }

    /*删除option*/
    if (data->option != NULL)
    {
        menuview_delete(data->option);
        data->option = NULL;
    }

    /*删除msgbox*/
    if (data->msgbox != NULL)
    {
        msgbox_delete(data->msgbox);
        data->msgbox = NULL;
    }

    print_dbg();
    /*! 删除窗体*/
    gui_wm_delete_window(data->win_id);

    /* destroy font */
    _scene_recording_unset_default_font();

    if (_recording_close_theme() == false)
    {
        return false;
    }

    if (_recording_close_str_res() == false)
    {
        return false;
    }

    _scene_recording_delete_bg_bitmap();
    print_dbg();

    if (NULL == data->ae_handle)
    {
        print_warning("ae_handleis NULL\n");
        return true;
    }

    /*关闭音源*/
    if (false == _close_source())
    {
        print_warning("close_source error\n");
        return false;
    }

    /*! 卸载中间件*/
    ret = mmm_ae_close(data->ae_handle, NULL);
    if (ret != 0)
    {
        print_err("ae close : %d\n", ret);
        return false;
    }

    dlclose(data->fptr);

    /*! MP3和wav格式时卸载dspenc，wma格式时卸载dspdec*/
    if (strcmp(data->format, "wma") == 0)
    {
        ret = (int) rmmod("dspdec.ko", 0);
        if (ret != 0)
        {
            print_err("dspdec rmmod : %d \n", ret);
            return false;
        }
    }
    else
    {
        ret = (int) rmmod("dspenc.ko", 0);
        if (ret != 0)
        {
            print_err("dspenc rmmod : %d \n", ret);
            return false;
        }
    }

    print_dbg("deinit done\n");
    return true;
}

int recorder_scene_recording(scene_recording_t* data)
{
    msg_apps_t msg;

    /* 场景初始化 */
    if (false == _recording_init(data))
    {
        printf("scene init error! \n");
        goto RECORDING_INIT_ERR;
    }

    printf("--: %s---%d--enter\n", __FILE__, __LINE__);

    /* 场景消息循环 */
    register_sys_dispatcher((sys_msg_proc) _recording_proc_sys);

    while (get_msg(&msg) == true)
    {
        dispatch_msg(&msg);
    }

    unregister_sys_dispatcher();

    RECORDING_INIT_ERR:
    /* 场景退出 */
    if (false == _recording_deinit(data))
    {
        printf("scene exit error! \n");
        return -1;
    }

    print_dbg("record exit\n");
    print_dbg("exit type: %d\n", data->exit_type);

    return data->exit_type;
}

