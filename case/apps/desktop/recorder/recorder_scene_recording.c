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

/*ж��dsp�����*/
void mount_dsp_enc(void)
{
    insmod("dspenc.ko", 0);
    sys_mknod(DEVICE_DSP_AE, S_IFCHR, makedev(MAJOR_DSP_AE, 0));
}

/*ж��dsp�����*/
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
        /*��ȡ�û��Զ��屳��ͼʧ�ܣ�ʹ��ui-editor����ӵ�Ĭ��ͼ*/
        g_scene_recording->bg_bitmap_info = sty_create_bitmap((int) g_scene_recording->rec_bg->pic_id);
    }

    if (NULL == g_scene_recording->bg_bitmap_info)
    {
        print_err("get backgroud bitmap failed!");
        return false;
    }

    return true;
}

/*ɾ������ͼ*/
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

    /*¼��֧���ļ���ʽ,�м���"|"�ָ�*/
    recorder_ext[0] = '|';
    if (get_config(CFG_APP_SUPPORT_EXT_RECORDER, recorder_ext + 1, CFG_MAX_BUFF_LENGTH - 2) == -1)
    {
        return false;
    }
    strncat(recorder_ext, "|", 1);
    g_scene_recording->support_format = recorder_ext;

    /*LINE_IN¼��֧��*/
    if (get_config(CFG_RECORDER_LINEIN_SUPPORT, (char *) &value, sizeof(value)) == -1)
    {
        return false;
    }
    else
    {
        g_scene_recording->line_in_support = value;
    }

    /*¼���ļ���ʽ*/
    if (get_config(CFG_RECORDER_FORMAT, s_format, 5) == -1)
    {
        return false;
    }
    else
    {
        g_scene_recording->format = s_format;
    }

    /*¼������*/
    if (get_config(CFG_RECORDER_QUALITY, (char *) &value, sizeof(value)) == -1)
    {
        return false;
    }
    else
    {
        g_scene_recording->quality = value;
    }

    /*¼����Դ*/
    if (get_config(CFG_RECORDER_MODE, (char *) &value, sizeof(value)) == -1)
    {
        return false;
    }
    else
    {
        g_scene_recording->mode = value;
    }

    /*¼������*/
    if (get_config(CFG_RECORDER_GAIN, (char *) &value, sizeof(value)) == -1)
    {
        return false;
    }
    else
    {
        g_scene_recording->gain = value;
    }

    /*¼���ļ�����·��*/
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


/*����¼��ģʽ������Դ*/
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
        /*LINE_IN��FMӲ�����ã���LINE_INʱ����ΪFM*/
        g_scene_recording->param.source.fm = 1;
        g_scene_recording->param.source.line_in = 0;
    }
}

static void _quality_map_to_bitrate(char *format, int quality)
{
    g_scene_recording->param.bpp = 16;/*����16bits*/

    if (0 == strcmp(format, "wav"))/*wav��������*/
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
    else if (0 == strcmp(format, "mp3"))/*mp3��������*/
    {
        g_scene_recording->param.channel = 2;/*˫����*/
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
    else/*wma��������*/
    {
        g_scene_recording->param.channel = 2;/*˫����*/
        g_scene_recording->param.sample_rate = 44100;/*����Ƶ��*/
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
        g_scene_recording->param.voice_volt.input_volt = 10;/*mic�����ѹ��10mvΪ��׼*/
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
        g_scene_recording->param.voice_volt.input_volt = 500;/*line in�����ѹ��500mvΪ��׼*/
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
    //��鱾�ش洢���ڲ���
    g_scene_recording->main_disk = sys_get_main_disk();
    if (HOTPLUGIN_DEV_TYPE_NONE == g_scene_recording->main_disk)
    {
        //��ʾ��������
        //�˳�recorderӦ��
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

        /*  Ӧ���˳�*/
        g_scene_recording->exit_type = SCENE_RECORDING_NEXT;
        //_recording_quit();
        exit_msg_loop();
        print_dbg("no avriable dis,recorder app quit\n");
        return false;
    }

    return true;
}

/*
 //���õ�Ĭ��Ŀ¼��ֻ���ڱ��ش洢������ʱʹ��
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
        else if (HOTPLUGIN_DEV_TYPE_CARD == g_scene_recording->main_disk)//��Ŀ¼�ǿ�
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
        else//��Ŀ¼���ⲿ�洢��
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
        else if (HOTPLUGIN_DEV_TYPE_CARD == g_scene_recording->main_disk)//��Ŀ¼�ǿ�
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
        else//��Ŀ¼���ⲿ�洢��
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

/*����Ϊ���ô洢��ʱ��·�����ú���*/
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
                /*�����ڣ������õ����ô洢��*/
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
                /*�ⲿ�洢�����ڣ������õ��ڲ��洢��*/
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
                /*�����ڣ������õ����ô洢��*/
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
                /*�ⲿ�洢�����ڣ������õ��ڲ��洢��*/
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


/*����Ϊ�������ⲿ�洢��ʱ��·�����ú���*/
static int _main_disk_other_set_path( void )
{
    const char *file_path = NULL;
    hotplugin_dev_info_t card_info;
    hotplugin_dev_info_t external_info;
    int result;
    /*���ô洢�����ڣ�����洢·��Ϊ���ô洢�����ȸ��Ĵ洢·��*/
    if (g_scene_recording->path == INTERNAL_MEMORY)
    {
        g_scene_recording->path = CARD_MEMORY;
    }
    /*�����ƶ��洢��״̬*/
    sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_CARD, &card_info);
    sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_UHOST, &external_info);
    /*���û�п��ô洢����ֱ�ӷ���*/
    if ((card_info.access != HOTPLUGIN_DEV_ACCESS_OK) && (external_info.access != HOTPLUGIN_DEV_ACCESS_OK))
    {
        return -1;
    }
    /*��ʱ������һ�����õĴ洢�����������ⲿ�洢��*/
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
                /*�������ã������õ��ⲿ�洢��*/
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
                /*�ⲿ�洢�������ã������õ���*/
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
                /*�������ã������õ��ⲿ�洢��*/
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
                /*�ⲿ�洢�������ã������õ���*/
                sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CARD,DIR_RECORDER_LINEIN), CARD_ROOT);
                file_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CARD,DIR_RECORDER_LINEIN);
                g_scene_recording->path = CARD_MEMORY;
            }
        }
    }
    /*�����м��������ò���*/
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
*       ����¼�������ļ�·��
* \param[in]  ��   
* \param[out] ��
*   ע��: 
* ����ֵ:0 :����·��ʧ�� -1:û�п���·�� 1:����·���ɹ�
*******************************************************************************/

int set_path(void)
{
    int result;    

    print_dbg("g_scene_recording->path == %d\n",g_scene_recording->path);

    /*�����֧��LINE_IN�����޸�¼��ģʽ*/
    if (0 == g_scene_recording->line_in_support)
    {
        g_scene_recording->mode = MIC;
    }

    if (HOTPLUGIN_DEV_TYPE_LOCAL == g_scene_recording->main_disk)
    {
        /*���ش洢���ڣ����������Ϊ���صĴ洢����·�����ú���*/
        result = _main_disk_local_set_path();
    }
    else
    {
        /*���̲�Ϊ���ش洢������������Ϊ�����洢����·�����ú���*/
        result = _main_disk_other_set_path();
    }
    return result;
}

/*�˳�recorderʱ�ر���Դ*/
bool static _close_source(void)
{
    int result;
    g_scene_recording->param.source.mic = 0;
    g_scene_recording->param.source.fm = 0;
    g_scene_recording->param.source.line_in = 0;
    /*����¼������*/
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

    /*������Դ*/
    _mode_map_to_source(g_scene_recording->mode);
    /*���ñ����ʣ��������������ȣ�����Ƶ��*/
    _quality_map_to_bitrate(g_scene_recording->format, g_scene_recording->quality);
    /*���������ѹ�������ѹ*/
    _gain_map_to_volt(g_scene_recording->mode, g_scene_recording->gain);

    /*����¼������*/
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

    /*����¼����ʽ*/
    result = mmm_ae_cmd(g_scene_recording->ae_handle, MMM_SET_FILE_FMT, (unsigned int) g_scene_recording->format);
    if (result < 0)
    {
        print_warning("file mode set failed\n");
        return false;
    }

    return true;
}

/*�򿪼�����ֻ����ģʽΪLINE_INʱ��Ч*/
bool start_monitor(int mode)
{
    int result;

    if (mode == LINE_IN)
    { 
        /*�򿪼�����������ʽΪģ�����*/
        result = mmm_ae_cmd(g_scene_recording->ae_handle, MMM_START_MONITOR, MMM_ANALOGE_MODE);// MMM_DIGITAL_MODE
        if (result < 0)
        {
            print_warning("start monitor err");
            return false;
        }
    }

    return true;
}

/*�رռ���,ֻ����ģʽΪLINE_INʱ��Ч*/
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

    /*! ��ȡ����ʱ��*/
    ret = sys_get_rtc_date_time(&time, &date);
    if (ret == false)
    {
        print_warning("get rtc date time error");
        return false;
    }

    /*! �����ļ���*/
    sprintf(g_scene_recording->file_name_buf, "%04d%02d%02d%02d%02d%02d", date.year, date.month, date.day, time.hour,
            time.minute, time.second);
    return true;
}

/*****************************************************************************
*ֹͣ¼������:
*��ɹ���:
*1.��ȡ¼��״̬����Ҫ����¼ʱ����Ϣ
*2.����ֹͣ¼������
*3.ֹͣˢ��������Ϣ����¼ʱ�䣬���̿ռ���Ķ�ʱ��
*4.����¼��״̬
*5.�ػ�UI
*6.�����ǰ¼����ʽ��wma��ʽ�����轵Ƶ����
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

    /*�����ļ�����·��*/
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

    /*����¼����ʽ*/
    ret = set_format();
    if (ret == false)
    {
        return 0;
    }

    /*����¼������*/
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

        /*��ʾ�û������ڱ����ļ�*/
        dialog_init_savefile.head = DIALOG_HEAD_INFO;
        dialog_init_savefile.str_id = g_scene_recording->rec_dialog_savefile->str_id;

        /* dialog init and callback function */
        g_scene_recording->dialog = dialog_create(&dialog_init_savefile, _dialog_callback);
        if (NULL == g_scene_recording->dialog)
        {
            print_err("dialog_create failed\n\n");
        }
        dialog_paint(g_scene_recording->dialog);
        /*ֹͣ¼��*/
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
            /*��ʾ�û������ڱ����ļ�*/
            dialog_init.head = DIALOG_HEAD_INFO;
            dialog_init.str_id = g_scene_recording->rec_dialog_savefile->str_id;

            g_scene_recording->dialog = dialog_create(&dialog_init, _dialog_callback);
            if (NULL == g_scene_recording->dialog)
            {
                print_err("dialog_create failed\n\n");
            }
            dialog_paint(g_scene_recording->dialog);

            /*ֹͣ¼��*/
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

    /*��ȡ�м��״̬*/
    result = mmm_ae_cmd(g_scene_recording->ae_handle, MMM_GET_RECORDER_STATUS,
            (unsigned int) &g_scene_recording->recording_status);
    if (result < 0)
    {
        print_warning("<<get rec time failed\n\n");
    }
    /*�м������״̬��ֹͣ¼��*/
    if( MMM_RECORDER_ERROR == g_scene_recording->recording_status.status )
    {
        print_err("recorder status error!\n");
        _stop_recording();
        return false;
    }
    /*��ȡ���̿ռ���Ϣ*/
    result = mmm_ae_cmd(param->ae_handle, MMM_GET_DISK_INFO, (unsigned int) &param->disk_info);
    if (result < 0)
    {
        print_warning("disk info get error");
        return false;
    }

    /*! ���̿ռ�������*/
    if ( 0 == param->disk_info.info )
    {

        /*¼�ƹ����У��ռ䲻�㣬��ʾ�ļ����ڱ��棬�ص�standby״̬����ʾ������*/
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
            /*!����dialog ������ʾ*/

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

            /*ֹͣ¼��*/
            _stop_recording();

            /*��ʾ�û������ڱ����ļ�*/
            dialog_init.head = DIALOG_HEAD_INFO;
            dialog_init.str_id = g_scene_recording->rec_dialog_savefile->str_id;
            dialog_modal( &dialog_init );
        }

        /*stop monitor*/
        if (false == stop_monitor(g_scene_recording->mode))
        {
            print_warning("stop monitor err");
        }

        /*  Ӧ���˳�*/
        g_scene_recording->exit_type = SCENE_RECORDING_NEXT;

        exit_msg_loop();
        print_dbg("scene quit\n");

        break;

        case MSG_CARD_OUT:           
        /*�ȵ���ʾ��*/
        if (g_scene_recording->msgbox != NULL)
        {
            msgbox_proc_sys(g_scene_recording->msgbox, msg);
        }

        /*���´洢·���˵���*/
        update_savepath();

        /*�洢·������*/
        if (g_scene_recording->path == CARD_MEMORY)
        {
            if (HOTPLUGIN_DEV_TYPE_LOCAL == g_scene_recording->main_disk)/*���ش洢����*/
            {
                g_scene_recording->path = INTERNAL_MEMORY;
            }
            else
            {
                g_scene_recording->path = EXTERNAL_MEMORY;
            }

            if (g_scene_recording->recorder_status == STANDBY)
            {
                /*���ô洢·�����Ա�ˢ�¿�¼ʱ��*/
                if (0 == set_path())
                {
                    print_warning("set path err");
                }
                /*�ػ��¼ʱ��*/
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
        /*�ȵ���ʾ��*/
        if (g_scene_recording->msgbox != NULL)
        {
            msgbox_proc_sys(g_scene_recording->msgbox, msg);
        }

        /*���´洢·���˵���*/
        update_savepath();

        /*�洢·������*/
        if (g_scene_recording->path == EXTERNAL_MEMORY)
        {
            if (HOTPLUGIN_DEV_TYPE_LOCAL == g_scene_recording->main_disk)/*���ش洢����*/
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
        /*�ȵ���ʾ��*/
        if (g_scene_recording->msgbox != NULL)
        {
            msgbox_proc_sys(g_scene_recording->msgbox, msg);
        }
        /*���´洢·���˵���*/
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

/*��¼��״̬��������¼ʱ�䣬��¼ʱ�䣬����������������¼��״̬ͼ���*/
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
 *       ������OK������
 * \param[in]    scene_recording_t *: ����ָ��
 * \retval       true:�ɹ�; false:ʧ��
 *   ʵ�ֵĹ���:
 *       ���ݵ�ǰ¼��״̬������Ӧ��OK��������Ӧ����
 *       standby->recording����recording <-> pause
 *******************************************************************************/
static bool _recording_deal_OK_up(scene_recording_t * data)
{
    int result;
    dialog_init_t dialog_init;

    if (data->recorder_status == STANDBY)
    {
        /*¼��֮ǰ�����͵�ѹ���,��ѹ���򵯿���ʾ��ѹ��*/
        if (sys_power_threshold_check(POWER_THRESHOLD_VOL_REC) == true)
        {
            if (strcmp(g_scene_recording->format, "wma") == 0)
            {
                sys_pm_set_clk_domain(&s_clk_domain);
            }

            /*����¼����ز���*/
            result = set_mmm();
            if (result < 0)
            {
                /*��ʾ��������*/
                /*�˳�recorderӦ��*/
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

                /*  Ӧ���˳�*/
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

            /*�����̿ռ�*/
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

            /*��ʼ¼��*/
            result = mmm_ae_cmd(data->ae_handle, MMM_START_RECORDING, (unsigned int) data->file_name_buf);
            if (result < 0)
            {
                print_err("start recording error");
                return false;
            }

            /*pbhl, curtime, voice energe ��timer�����ƻ�ͼ*/
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
            /*�����������*/
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

                /*  Ӧ���˳�*/
                data->exit_type = SCENE_RECORDING_NEXT;
                exit_msg_loop();
                print_dbg("scene quit\n");
            }
            else
            {
                print_warning("unknow recorder status\n");
            }
            /*�����������*/
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
                /*����option�˵�*/
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
                /*��ʾ�ļ����ڱ���*/
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
            /*�����������*/
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

    /* ȡ��ͼ�Ƕ�*/
    angle = sty_get_view_rotation();

    /* ������Ļ����*/
    sty_set_screen_rotation(angle);

    /* ����commonui�� style ����*/
    if ((angle == ROTATION_0) || (angle == ROTATION_180))
    {
        /* ��ȡcommonui �� style ��Դ�ļ���*/
        g_scene_recording->common_scene = load_scene(app_data.common_style, COMMONUI_SIDELONG_SCENE);

        /* ��ȡӦ�õ� style ��Դ�ļ���*/
        g_scene_recording->scene = load_scene(app_data.recorder_style, RECORDING_SIDELONG_SCENE);
    }
    else
    /*if ( (angle == ROTATION_90) || (angle == ROTATION_270) )*/
    {
        /* ��ȡcommonui �� style ��Դ�ļ���*/
        g_scene_recording->common_scene = load_scene(app_data.common_style, COMMONUI_UPRIGHT_SCENE);

        /* ��ȡӦ�õ� style ��Դ�ļ���*/
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

    /* ��ȡcommonui ��ͼƬ��Դ�ļ���*/
    sprintf(file, "%s/%s.res", sys_get_common_theme_res_dir(), "commonui");

    /* �� commonui ͼƬ��Դ�ļ�*/
    if (sty_open_common_pic_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }

    /* ��ȡcommonui ��ͼƬ��Դ�ļ���*/
    sprintf(file, "%s/%s.res", sys_get_app_theme_res_dir(), "recorder");

    /* ��Ӧ�õ�ͼƬ��Դ�ļ�*/
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

    /* ��ȡcommonui �ַ�����Դ�ļ���*/
    sprintf(file, "%s/%s_%s.str", sys_get_common_string_res_dir(), "commonui", sys_get_cur_language_codes());
    print_dbg("load %s", file);
    /* �� commonui �ַ�����Դ�ļ�*/
    if (sty_open_common_str_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }

    /* ��ȡsettingӦ���ַ�����Դ�ļ���*/
    sprintf(file, "%s/%s_%s.str", sys_get_app_string_res_dir(), "recorder", sys_get_cur_language_codes());
    print_dbg("load %s", file);
    /* ��Ӧ�õ��ַ�����Դ�ļ�*/
    if (sty_open_app_str_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }

    /* �������Ա���*/
    sty_set_text_encoding(GUI_UTF8_ENC);

    /* ��������ģʽ*/
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
 * ���õ�ǰ����ȫ��������Ϊ��Ч�ĵ�һ֡
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
 * ���õ�ǰ����ȫ��������Ϊ��Ч�����һ֡
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

    /* load theme ��Դ�ļ� */
    if (_recording_open_theme() == false)
    {
        return false;
    }

    /* load �ַ�����Դ�ļ� */
    if (_recording_open_str_res() == false)
    {
        return false;
    }

    /*��������*/
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
        /*��ʾ�޴洢��*/
        /*�˳�recorderӦ��*/
        if (NULL != data->rec_dialog_nodisk)
        {
            dialog_init.head = DIALOG_HEAD_WARN;
            dialog_init.str_id = data->rec_dialog_nodisk->str_id;

            dialog_modal(&dialog_init);
        }

        /*  Ӧ���˳�*/
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

    /*���ý���*/
    gui_wm_set_focus(data->win_id);
    /*��������*/
    _read_config();

    if (strcmp(data->format, "wma") == 0)
    {
        mount_dsp_dec();

        /*�����м��*/
        data->fptr = dlopen("a_record.so", RTLD_LAZY);
    }
    else
    {
        mount_dsp_enc();

        /*�����м��*/
        data->fptr = dlopen("a_codec.so", RTLD_LAZY);
    }
    /*���м��*/
    data->ae_handle = mmm_ae_open(NULL);
    if (data->ae_handle == NULL)
    {
        print_err("mmm open failed\n");
        return false;
    }

    /*����¼�������ȼ�*/
    /*�����ȼ�Ϊ5��ֵ��Χ[0 - 31]*/
    ret = mmm_ae_cmd(data->ae_handle, MMM_SET_ENERGY_LEVEL, 5);
    if (ret < 0)
    {
        print_warning("energy level set failed\n");
        return false;
    }

    /*�˴�����¼����ز�����Ϊ�˼����̣���total time*/
    ret = set_mmm();
    if ( ret <= 0 )
    {
        print_err("set mmm error!!!\n");

        return false;

    }

    /*�򿪼���*/
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

    /*�����������*/
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

    /*ɾ��headbar*/
    if (data->headbar != NULL)
    {
        headbar_delete(data->headbar);
        data->headbar = NULL;
    }

    /*ɾ��volumebar*/
    if (data->vbar != NULL)
    {
        volumebar_delete(data->vbar);
        data->vbar = NULL;
    }

    print_dbg("delete dialog");

    /*ɾ��dialog*/
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

    /*ɾ��adjust*/
    if (data->adjust != NULL)
    {
        adjust_delete(data->adjust);
        data->adjust = NULL;
    }

    /*ɾ��option*/
    if (data->option != NULL)
    {
        menuview_delete(data->option);
        data->option = NULL;
    }

    /*ɾ��msgbox*/
    if (data->msgbox != NULL)
    {
        msgbox_delete(data->msgbox);
        data->msgbox = NULL;
    }

    print_dbg();
    /*! ɾ������*/
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

    /*�ر���Դ*/
    if (false == _close_source())
    {
        print_warning("close_source error\n");
        return false;
    }

    /*! ж���м��*/
    ret = mmm_ae_close(data->ae_handle, NULL);
    if (ret != 0)
    {
        print_err("ae close : %d\n", ret);
        return false;
    }

    dlclose(data->fptr);

    /*! MP3��wav��ʽʱж��dspenc��wma��ʽʱж��dspdec*/
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

    /* ������ʼ�� */
    if (false == _recording_init(data))
    {
        printf("scene init error! \n");
        goto RECORDING_INIT_ERR;
    }

    printf("--: %s---%d--enter\n", __FILE__, __LINE__);

    /* ������Ϣѭ�� */
    register_sys_dispatcher((sys_msg_proc) _recording_proc_sys);

    while (get_msg(&msg) == true)
    {
        dispatch_msg(&msg);
    }

    unregister_sys_dispatcher();

    RECORDING_INIT_ERR:
    /* �����˳� */
    if (false == _recording_deinit(data))
    {
        printf("scene exit error! \n");
        return -1;
    }

    print_dbg("record exit\n");
    print_dbg("exit type: %d\n", data->exit_type);

    return data->exit_type;
}

