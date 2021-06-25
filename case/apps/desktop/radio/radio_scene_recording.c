/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : radio_scene_recording.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      cheason    2009-04-23          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     radio_scene_recording.c
 * \brief    routine of radio scene recording
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/04/23
 *******************************************************************************/

#include "radio_scene_recording.h"
#include "radio_ui_recording.h"
#include "radio_sty.h"

/* global variable define */
scene_recording_t* g_scene_recording = NULL;


static bool _scene_recording_get_bg_bitmap(void);
static bool _scene_recording_delete_bg_bitmap(void);
static bool _scene_recording_set_default_font(void);
static bool _scene_recording_unset_default_font(void);
static void mount_dsp_enc(void);
static bool _set_path(void);
static void _volumebar_callback(volumebar_t *data, volumebar_sig_e signal);
static void _headbar_callback(headbar_t *headbar, int signal);

/*function for exit recording when disk room is not enough*/

static void _msgbox_callback(msgbox_t * msgbox, msgbox_sig_e signal);
static bool _check_diskinfo(void *data);
static bool _recording_proc_sys(msg_apps_t *msg);
static bool _recording_paint(scene_recording_t *data);
static void _recording_load_resource(scene_recording_t* data);
static bool _scene_recording_rotate(void);
static bool _recording_open_theme(void);
static bool _recording_close_theme(void);
static bool _recording_open_str_res(void);
static bool _recording_close_str_res(void);
static void _scene_recording_fusion_set_first_frame(scene_t *scene);
static void _scene_recording_fusion_set_last_frame(scene_t *scene);
static bool _recording_init(fusion_enter_e fenter);
static bool _recording_deinit(void);
static bool _recording_deal_msg(scene_recording_t * data, gui_wm_message_t *pmsg);
static void _recording_callback(gui_wm_message_t *pmsg);
static void _dialog_callback(dialog_t *dialog, widget_sig_dialog_e signal);
static void _stop_recording(void);

/******************************************************************************/
/*!                    
 * \par  Description:   get background   
*******************************************************************************/
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
        g_scene_recording->bg_bitmap_info = sty_create_bitmap((int) g_scene_recording->fm_rec_bg->pic_id);
    }

    if (NULL == g_scene_recording->bg_bitmap_info)
    {
        print_err("get backgroud bitmap failed!");
        return false;
    }

    return true;
}

/******************************************************************************/
/*!                    
 * \par  Description:     delete background
*******************************************************************************/
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

/******************************************************************************/
/*!                    
 * \par  Description:     set default font
*******************************************************************************/
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

/******************************************************************************/
/*!                    
 * \par  Description:     unset default font
*******************************************************************************/
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

/******************************************************************************/
/*!                    
 * \par  Description:     load configuration
*******************************************************************************/
static bool _load_config(void)
{
    int value;

    /*录音质量*/
    if (get_config(CFG_FM_RECORD_QUALITY, (char *) &value, sizeof(value)) == -1)
    {
        return false;
    }
    else
    {
        g_scene_recording->quality = value;
    }

    /*录音增益*/
    if (get_config(CFG_FM_RECORD_GAIN, (char *) &value, sizeof(value)) == -1)
    {
        return false;
    }
    else
    {
        g_scene_recording->gain = value;
        print_dbg("gain : %d", value);
    }

    /*录音文件保存路径*/
    if (get_config(CFG_FM_RECORD_PATH, (char *) &value, sizeof(value)) == -1)
    {
        return false;
    }
    else
    {
        g_scene_recording->path = value;
    }

    return true;
}


/******************************************************************************/
/*!                    
 * \par  Description:     set file path
*******************************************************************************/
static bool _set_path(void)
{
    int result;
    const char *file_path = NULL;
    hotplugin_dev_info_t card_info;
    hotplugin_dev_info_t external_info;
    dialog_init_t dialog_init;

    if (HOTPLUGIN_DEV_TYPE_LOCAL == app_data.main_disk)
    {
        if (g_scene_recording->path == 0)
        {
            sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_RADIO_RECORD), UDISK_ROOT);
            file_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_RADIO_RECORD);
        }
        else if (g_scene_recording->path == 1)
        {
            sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_CARD, &card_info);
            if (card_info.access == HOTPLUGIN_DEV_ACCESS_OK)
            {
                sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CARD,DIR_RADIO_RECORD), CARD_ROOT);
                file_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CARD,DIR_RADIO_RECORD);
            }
            else
            {
                g_scene_recording->path = 0;
                sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_RADIO_RECORD), UDISK_ROOT);
                file_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_RADIO_RECORD);
            }
        }
        else
        {
            sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_UHOST, &external_info);
            if (external_info.access == HOTPLUGIN_DEV_ACCESS_OK)
            {
                sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_UHOST,DIR_RADIO_RECORD), EXTERNAL_ROOT);
                file_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_UHOST,DIR_RADIO_RECORD);
            }
            else
            {
                g_scene_recording->path = 0;
                sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_RADIO_RECORD), UDISK_ROOT);
                file_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_RADIO_RECORD);
            }
        }
    }
    else
    {
        if (g_scene_recording->path == 0)
        {
            g_scene_recording->path = 1;
        }
        sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_CARD, &card_info);
        sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_UHOST, &external_info);
        if ((card_info.access != HOTPLUGIN_DEV_ACCESS_OK) && (external_info.access != HOTPLUGIN_DEV_ACCESS_OK))
        {
            print_err("no available disk\n");
            /*提示请插入磁盘*/

            /*没有磁盘，返回FM播放场景*/
            if (NULL == g_scene_recording->dialog)
            {
                /* initial dialog data*/
                dialog_init.head = DIALOG_HEAD_WARN;
                dialog_init.str_id = g_scene_recording->rec_dialog_diskins->str_id;

                dialog_modal(&dialog_init);
            }
            print_err("g_scene_recording->dialog = %d",g_scene_recording->dialog);
            return false;
        }
        else
        {
            if (g_scene_recording->path == 1)
            {
                if (card_info.access == HOTPLUGIN_DEV_ACCESS_OK)
                {
                    sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CARD,DIR_RADIO_RECORD), CARD_ROOT);
                    file_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CARD,DIR_RADIO_RECORD);
                }
                else
                {
                    sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_UHOST,DIR_RADIO_RECORD), EXTERNAL_ROOT);
                    file_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_UHOST,DIR_RADIO_RECORD);
                }
            }
            else if (g_scene_recording->path == 2)/*external memory*/
            {
                if (external_info.access == HOTPLUGIN_DEV_ACCESS_OK)
                {
                    sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_UHOST,DIR_RADIO_RECORD), EXTERNAL_ROOT);
                    file_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_UHOST,DIR_RADIO_RECORD);
                }
                else
                {
                    sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CARD,DIR_RADIO_RECORD), CARD_ROOT);
                    file_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CARD,DIR_RADIO_RECORD);
                }
            }
            else
            {
                print_dbg("path err\n");
            }
        }
    }

    result = mmm_ae_cmd(g_scene_recording->ae_handle, MMM_SET_SAVE_PATH, (unsigned int) file_path);
    if (result < 0)
    {
        print_err("path set failed\n");
        return false;
    }

    return true;
}

/******************************************************************************/
/*!                    
 * \par  Description:     get bitrate
*******************************************************************************/
static void _quality_map_to_bitrate(int quality)
{
    g_scene_recording->param.bpp = 16;/*采样精度16bits*/
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

/******************************************************************************/
/*!                    
 * \par  Description:     set voltage
*******************************************************************************/
static void _gain_map_to_volt(int gain)
{
    g_scene_recording->param.voice_volt.input_volt = 220;/*FM输入电压以10mv为基准*/
    switch (gain)
    {
        case 1:
        g_scene_recording->param.voice_volt.output_volt = 220;
        break;
        case 2:
        g_scene_recording->param.voice_volt.output_volt = 261;
        break;
        case 3:
        g_scene_recording->param.voice_volt.output_volt = 311;
        break;
        case 4:
        g_scene_recording->param.voice_volt.output_volt = 369;
        break;
        case 5:
        g_scene_recording->param.voice_volt.output_volt = 439;
        break;
        default:
        break;
    }
}

/*
 static void mount_ada(void)
 {
 insmod("ada.ko",0);
 sys_mknod("/dev/ada", S_IFCHR, makedev(MAJOR_ADA, 0));
 }
 */
 /******************************************************************************/
/*!                    
 * \par  Description:     mount dsp
*******************************************************************************/
static void mount_dsp_enc(void)
{
    insmod("dspenc.ko", 0);
    sys_mknod(DEVICE_DSP_AE, S_IFCHR, makedev(MAJOR_DSP_AE, 0));
}

/******************************************************************************/
/*!                    
 * \par  Description:     create headbar
*******************************************************************************/
static bool _create_headbar(void)
{
    headbar_init_t headbar_init;

    headbar_init.type = 0;
    headbar_init.pic_id_head = g_scene_recording->fm_rec_hbar_pic->pic_id;
    headbar_init.str_id_title = g_scene_recording->fm_rec_hbar_str->str_id;

    g_scene_recording->headbar = headbar_create(&headbar_init, (headbar_cb_t) _headbar_callback);
    if (NULL == g_scene_recording->headbar)
    {
        print_err("headbar_create failed\n\n");
        return false;
    }
    headbar_paint(g_scene_recording->headbar);

    return true;
}

/******************************************************************************/
/*!                    
 * \par  Description:     create msgbox
*******************************************************************************/
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

/******************************************************************************/
/*!                    
 * \par  Description:     volumer callback
*******************************************************************************/
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
}

static void _dialog_callback_exit(dialog_t *dialog, widget_sig_dialog_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_DIALOG_OK:  
        case WIDGET_SIG_DIALOG_CANCAL:  
        if (g_scene_recording->dialog != NULL)
        {
            dialog_delete(g_scene_recording->dialog);
            g_scene_recording->dialog = NULL;
        }       
        g_scene_recording->next_scene = RADIO_SCENE_PLAYING_BACKWARD;
        
        exit_msg_loop();
        break;

        default:
        break;
    }
}


static void _dialog_callback(dialog_t *dialog, widget_sig_dialog_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_DIALOG_OK:
        
        _stop_recording();
        break;

        case WIDGET_SIG_DIALOG_CANCAL:
        /*! delete dialog */
        if (g_scene_recording->dialog != NULL)
        {
            dialog_delete(g_scene_recording->dialog);
            g_scene_recording->dialog = NULL;
        }
        break;

        default:
        break;
    }
}

/******************************************************************************/
/*!                    
 * \par  Description:     msgbox callback
*******************************************************************************/
static void _msgbox_callback(msgbox_t * msgbox, msgbox_sig_e signal)
{
    
    switch (signal)
    {
        case WIDGET_SIG_MSGBOX_SHOW:
        print_dbg("show message box");
        break;
        case WIDGET_SIG_MSGBOX_QUIT:
        print_dbg("quit message box");
        break;

        case WIDGET_SIG_MSGBOX_QUIT_LOW_POWER:
   /*    printf("QUIT_LOW_POWER box\n\n\n");
        dialog_init.head = DIALOG_HEAD_INFO;
        dialog_init.str_id = g_scene_recording->rec_dialog_lowbat->str_id;

        g_scene_recording->dialog = dialog_create(&dialog_init, _dialog_callback_exit);
        if (NULL == g_scene_recording->dialog)
        {
            print_err("dialog_create failed\n\n");
        }
        dialog_paint(g_scene_recording->dialog);*/
        _stop_recording();
        g_scene_recording->next_scene = RADIO_SCENE_PLAYING_BACKWARD;
       
        exit_msg_loop();
        
        

        break;

        default:
        break;
    }
}

/******************************************************************************/
/*!                    
 * \par  Description:     check disk 
*******************************************************************************/
static bool _check_diskinfo(void *data)
{
    int result;
    dialog_init_t dialog_init;
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

    result = mmm_ae_cmd(param->ae_handle, MMM_GET_DISK_INFO, (unsigned int) &param->disk_info);
    if (result < 0)
    {
        print_err("disk info get error");

        return false;

    }

    /*! 磁盘空间不足报警*/
    if ((param->disk_info.info < RESERVE_TIME) && (NULL == param->dialog))
    {
        /*!启动dialog 报警提示*/

        /* initial dialog data*/
        if (param->disk_info_timer_id != -1)
        {
            stop_timer(param->disk_info_timer_id);
        }
        dialog_init.head = DIALOG_HEAD_WARN;
        dialog_init.str_id = param->fm_rec_dialog_diskinfo->str_id;

        /* dialog init and callback function */
        if (param->disk_info_timer_id != -1)
        {
            if(WIDGET_SIG_DIALOG_CANCAL == dialog_modal(&dialog_init))
            {
                _stop_recording();
            }
        }
        else
        {
            dialog_modal(&dialog_init);
            g_scene_recording->next_scene = RADIO_SCENE_PLAYING_BACKWARD;       
            
        }


        return false;
    }
    else
    {
        return true;
    }
}

/******************************************************************************/
/*!                    
 * \par  Description:     load resource
*******************************************************************************/
static void _recording_load_resource(scene_recording_t* data)
{
    data->fm_rec_bg = (picture_resource_t *) get_scene_child(data->scene, PIC_BG);
    data->fm_rec_word_status = (picregion_resource_t *) get_scene_child(data->scene, PIC_WORD_STATUS);
    data->fm_rec_filename = (string_resource_t *) get_scene_child(data->scene, STR_FILENAME);
    data->fm_rec_pbbg = (picture_resource_t *) get_scene_child(data->scene, PIC_PBBG);
    data->fm_rec_pb = (picture_resource_t *) get_scene_child(data->scene, PIC_PB);
    data->fm_rec_curtime = (string_resource_t *) get_scene_child(data->scene, STR_CURTIME);
    data->fm_rec_totaltime = (string_resource_t *) get_scene_child(data->scene, STR_TOTALTIME);
    data->fm_rec_color_status = (picregion_resource_t *) get_scene_child(data->scene, PIC_COLOR_STATUS);
    data->fm_rec_tape = (picture_resource_t *) get_scene_child(data->scene, PIC_TAPE);
    data->fm_rec_vol_bg = (picture_resource_t *) get_scene_child(data->scene, PIC_VOL_BG);
    data->fm_rec_vol1 = (picture_resource_t *) get_scene_child(data->scene, PIC_VOL1);
    data->fm_rec_vol2 = (picture_resource_t *) get_scene_child(data->scene, PIC_VOL2);
    data->fm_rec_vol3 = (picture_resource_t *) get_scene_child(data->scene, PIC_VOL3);
    data->fm_rec_vol4 = (picture_resource_t *) get_scene_child(data->scene, PIC_VOL4);
    data->fm_rec_vol5 = (picture_resource_t *) get_scene_child(data->scene, PIC_VOL5);

    /*load headbar resource*/
    data->fm_rec_hbar = (resgroup_resource_t *) get_scene_child(data->scene, WIDGET_HEADBAR_RESGROUP);
    data->fm_rec_hbar_pic = (picture_resource_t *) get_resgroup_child(data->fm_rec_hbar, PIC_HBAR);
    data->fm_rec_hbar_str = (string_resource_t *) get_resgroup_child(data->fm_rec_hbar, STR_HBAR);

    /* load dialog initial resource */
    data->fm_rec_dialog = (resgroup_resource_t *) get_scene_child(data->scene, WIDGET_DIALOG_RESGROUP);
    data->fm_rec_dialog_stoprec = (string_resource_t *) get_resgroup_child(data->fm_rec_dialog, STR_DIALOG_STOPREC);
    data->fm_rec_dialog_diskinfo = (string_resource_t *) get_resgroup_child(data->fm_rec_dialog, STR_DIALOG_DISKINFO);
    data->rec_dialog_lowbat = (string_resource_t *) get_resgroup_child(data->fm_rec_dialog, STR_DIALOG_LOWBAT);
    /*rec_dialog_savefile*/
    data->rec_dialog_savefile = (string_resource_t *) get_resgroup_child(data->fm_rec_dialog, STR_DIALOG_SAVEFILE);
    data->rec_dialog_diskins = (string_resource_t *) get_resgroup_child(data->fm_rec_dialog, STR_DIALOG_DISKINS);
    data->rec_dialog_nodisk = (string_resource_t *) get_resgroup_child(data->fm_rec_dialog, STR_DIALOG_NODISK);

    if (data->bg_bitmap_info != NULL)
    {
        _scene_recording_delete_bg_bitmap();
    }

    _scene_recording_get_bg_bitmap();
}

/******************************************************************************/
/*!                    
 * \par  Description:     scene rotate
*******************************************************************************/
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

    g_scene_recording->bg = need_paint;
    g_scene_recording->word_status = need_paint;
    g_scene_recording->filename = need_paint;
    g_scene_recording->pbbg = need_paint;
    g_scene_recording->totaltime = need_paint;
    g_scene_recording->color_status = need_paint;
    g_scene_recording->tape = need_paint;

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

    

    fusion_set_last_frame(NULL);
    fusion_display(NULL);

    return true;
}

/******************************************************************************/
/*!                    
 * \par  Description:     process system message
*******************************************************************************/
static bool _recording_proc_sys(msg_apps_t *msg)
{
    msg_reply_t t;
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
        case MSG_MAIN_DISK_CHANGED:
        print_dbg("MSG_MAIN_DISK_CHANGED\n");
        break;

        case MSG_APP_QUIT:

        /*! delete dialog */
        if (g_scene_recording->dialog != NULL)
        {
            dialog_delete(g_scene_recording->dialog);
            g_scene_recording->dialog = NULL;
        }

        /*提示用户，正在保存文件*/
        dialog_init.head = DIALOG_HEAD_INFO;
        dialog_init.str_id = g_scene_recording->rec_dialog_savefile->str_id;
        dialog_modal(&dialog_init);
        _stop_recording();
#if 0
        g_scene_recording->dialog = dialog_create(&dialog_init, _dialog_callback_exit);
        if (NULL == g_scene_recording->dialog)
        {
            print_err("dialog_create failed\n\n");
        }
        dialog_paint(g_scene_recording->dialog);

        /* read rec time*/
        result = mmm_ae_cmd(g_scene_recording->ae_handle, MMM_GET_RECORDER_STATUS,
                (unsigned int) &g_scene_recording->recording_status);
        if (result < 0)
        {
            print_err("get rec time failed\n");
        }
        print_dbg("rec time: %d\n", g_scene_recording->recording_status.time);

        /* stop recording */
        result = mmm_ae_cmd(g_scene_recording->ae_handle, MMM_STOP_RECORDING, 0);
        print_dbg("stop record result: %d\n", result);
        if (result < 0)
        {
            print_err("stop recording err\n");
        }
        print_dbg("stop recording\n");
#endif
        
        g_scene_recording->next_scene = RADIO_SCENE_EXIT;
        break;

        case MSG_CARD_OUT:
        if (g_scene_recording->msgbox != NULL)
        {
            msgbox_proc_sys(g_scene_recording->msgbox, msg);
        }
        if (g_scene_recording->path == 1)
        {
            g_scene_recording->path = 0;
            
            _stop_recording();           
        }
        break;

        case MSG_USB_HOST_DISK_OUT:
        if (g_scene_recording->msgbox != NULL)
        {
            msgbox_proc_sys(g_scene_recording->msgbox, msg);
        }
        if (g_scene_recording->path == 2)
        {
            g_scene_recording->path = 0;
           
            _stop_recording();
        }
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

/* scene quit operation function */
/*
 static bool _recording_quit()
 {
 msg_apps_t m;
 m.type = MSG_QUIT;

 return send_async_msg(RADIO_APP_NAME,&m);
 }
 */
/******************************************************************************/
/*!                    
 * \par  Description:     stop recording
*******************************************************************************/
static void _stop_recording(void)
{
    int result;
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

    /* read rec time*/
    result = mmm_ae_cmd(g_scene_recording->ae_handle, MMM_GET_RECORDER_STATUS,
            (unsigned int) &g_scene_recording->recording_status);
    if (result < 0)
    {
        print_err("get rec time failed\n");
    }
    print_dbg("rec time: %d\n", g_scene_recording->recording_status.time);

    /* stop recording */
    result = mmm_ae_cmd(g_scene_recording->ae_handle, MMM_STOP_RECORDING, 0);
    print_dbg("stop record result: %d\n", result);
    if (result < 0)
    {
        print_err("stop recording err\n");
    }
    print_dbg("stop recording\n");

    /*quit scene*/
    g_scene_recording->next_scene = RADIO_SCENE_PLAYING_BACKWARD;
    exit_msg_loop();
 
    return;
}
/******************************************************************************/
/*!                    
 * \par  Description:     recording scene paint
*******************************************************************************/
static bool _recording_paint(scene_recording_t *data)
{
    fm_recorder_draw_bg(data);
    fm_recorder_draw_word_status(data);
    fm_recorder_draw_filename(data);
    fm_recorder_draw_pbbg(data);
    fm_recorder_draw_totaltime(data);
    fm_recorder_draw_pbhl_curtime(data);
    fm_recorder_draw_color_status(data);
    fm_recorder_draw_tape(data);
    fm_recorder_draw_vol(data);

    return true;
}

/******************************************************************************/
/*!                    
 * \par  Description:     deal message
*******************************************************************************/
static bool _recording_deal_msg(scene_recording_t * data, gui_wm_message_t *pmsg)
{
    int result;
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
            case KEY_OK | KEY_TYPE_SHORT_UP:
            if (data->recorder_status == recording)
            {
                data->recorder_status = pause;

                data->word_status = need_paint;
                data->color_status = need_paint;

                data->bg = not_need_paint;
                data->filename = not_need_paint;
                data->pbbg = not_need_paint;
                data->totaltime = not_need_paint;
                data->tape = not_need_paint;

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
            else if (data->recorder_status == pause)
            {
                data->recorder_status = recording;

                data->word_status = need_paint;
                data->color_status = need_paint;

                data->bg = not_need_paint;
                data->filename = not_need_paint;
                data->pbbg = not_need_paint;
                data->totaltime = not_need_paint;
                data->tape = not_need_paint;

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
                print_err("unknow recorder status\n");
            }
            break;

            case KEY_RETURN | KEY_TYPE_SHORT_UP:
            case KEY_RETURN | KEY_TYPE_LONG_UP:
            case KEY_RETURN | KEY_TYPE_HOLD_UP:
            /* initial dialog data*/
            dialog_init.head = DIALOG_HEAD_QUERY;
            dialog_init.str_id = data->fm_rec_dialog_stoprec->str_id;
            dialog_init.def_sel = WIDGET_SIG_DIALOG_CANCAL;

            /* dialog init and callback function */

            if (data->dialog != NULL)
            {
                dialog_delete(data->dialog);
                data->dialog = NULL;
            } 
         
            data->dialog = dialog_create( &dialog_init, _dialog_callback);
            dialog_paint(data->dialog);
           
            /*            
            if(WIDGET_SIG_DIALOG_OK == dialog_modal(&dialog_init))
            {
                _stop_recording();
            }
            */             
            break;

            case KEY_VOL | KEY_TYPE_DOWN:
            case KEY_VSUB | KEY_TYPE_DOWN:
            case KEY_VADD | KEY_TYPE_DOWN:
           

            data->vbar = volumebar_create(0, _volumebar_callback);

            if (data->vbar == NULL)
            {
                
                return false;
            }
            else
            {
                volumebar_paint(data->vbar);
            }
           
            break;

            case KEY_MENU | KEY_TYPE_DOWN:
          
            /*提示正在保存文件*/
            dialog_init.head = DIALOG_HEAD_INFO;
            dialog_init.str_id = g_scene_recording->rec_dialog_savefile->str_id;
#if 0
            dialog_modal(&dialog_init);
            _stop_recording();
#endif
            g_scene_recording->dialog = dialog_create(&dialog_init, _dialog_callback_exit);
            if (NULL == g_scene_recording->dialog)
            {
                print_err("dialog_create failed\n");
            }
            dialog_paint(g_scene_recording->dialog);

            /*停止当前录音时间和磁盘空间检查*/
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

            /* read rec time*/
            result = mmm_ae_cmd(g_scene_recording->ae_handle, MMM_GET_RECORDER_STATUS,
                    (unsigned int) &g_scene_recording->recording_status);
            if (result < 0)
            {
                print_err("get rec time failed\n");
            }
            print_dbg("rec time: %d\n", g_scene_recording->recording_status.time);

            /* stop recording */
            result = mmm_ae_cmd(g_scene_recording->ae_handle, MMM_STOP_RECORDING, 0);
            if (result < 0)
            {
                print_err("stop recording err\n");
            }
            print_dbg("stop recording\n");

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

/******************************************************************************/
/*!                    
 * \par  Description:     scene callback
*******************************************************************************/
static void _recording_callback(gui_wm_message_t *pmsg)
{
    int top;

    top = canvas_get_focus(pmsg);

    /*dispatch msg*/
    if ((g_scene_recording->vbar != NULL) && (top == g_scene_recording->vbar->canvas_id)) /*volumebar process*/
    {
        volumebar_proc_gui(g_scene_recording->vbar, pmsg);
    }
    else if ((g_scene_recording->dialog != NULL) && (top == g_scene_recording->dialog->canvas_id))
    {
        dialog_proc_gui(g_scene_recording->dialog, pmsg);
    }
    else if ((g_scene_recording->adjust != NULL) && (top == g_scene_recording->adjust->canvas_id))
    {
        adjust_proc_gui(g_scene_recording->adjust, pmsg);
    }
    else if ((g_scene_recording->msgbox != NULL) && (top == g_scene_recording->msgbox->canvas_id))
    {
        msgbox_proc_gui(g_scene_recording->msgbox, pmsg);
    }
    else
    {
        _recording_deal_msg(g_scene_recording, pmsg);
    }
}

/******************************************************************************/
/*!                    
 * \par  Description:     open theme
*******************************************************************************/
static bool _recording_open_theme(void)
{
    rotation_e angle;
    char file[512];

    /* 取视图角度*/
    angle = sty_get_view_rotation();

    /* 设置屏幕方向*/
    sty_set_screen_rotation(angle);

    /* 载入commonui的 style 场景*/
    if ((angle == ROTATION_0) || (angle == ROTATION_180))
    {
        /* 获取commonui 的 style 资源文件名*/
        sprintf(file, "%s/%s.sty", sys_get_common_theme_res_dir(), "commonui");
        g_scene_recording->common_scene = load_scene(app_data.commonui_style, COMMONUI_SIDELONG_SCENE);

        /* 获取应用的 style 资源文件名*/
        sprintf(file, "%s/%s.sty", sys_get_app_theme_res_dir(), "radio");
        g_scene_recording->scene = load_scene(app_data.app_style, RECORDING_SIDELONG_SCENE);
    }
    else
  
    {
        /* 获取commonui 的 style 资源文件名*/
        sprintf(file, "%s/%s.sty", sys_get_common_theme_res_dir(), "commonui");
        g_scene_recording->common_scene = load_scene(app_data.commonui_style, COMMONUI_UPRIGHT_SCENE);

        /* 获取应用的 style 资源文件名*/
        sprintf(file, "%s/%s.sty", sys_get_app_theme_res_dir(), "radio");
        g_scene_recording->scene = load_scene(app_data.app_style, RECORDING_UPRIGHT_SCENE);
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
    sprintf(file, "%s/%s.res", sys_get_app_theme_res_dir(), "radio");

    /* 打开应用的图片资源文件*/
    if (sty_open_app_pic_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }

    return true;

}

/******************************************************************************/
/*!                    
 * \par  Description:     close theme
*******************************************************************************/
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

/******************************************************************************/
/*!                    
 * \par  Description:     open string resource
*******************************************************************************/
static bool _recording_open_str_res(void)
{
    char file[512];

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
    sprintf(file, "%s/%s_%s.str", sys_get_app_string_res_dir(), "radio", sys_get_cur_language_codes());
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

/******************************************************************************/
/*!                    
 * \par  Description:     close string resource
*******************************************************************************/
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

/******************************************************************************/
/*!                    
 * \par  Description:     recording init
*******************************************************************************/
static bool _recording_init(fusion_enter_e fenter)
{
    int result = 0;
    bool ret;
    scene_recording_t *data;
  
    fusion_effect_t eff;
    dialog_init_t dialog_init;
    rtc_time_t l_time;
    rtc_date_t l_date;

    

    /*分配场景数据空间*/
    if (g_scene_recording == NULL)
    {
        g_scene_recording = malloc(sizeof(scene_recording_t));
        if (g_scene_recording == NULL)
        {
            print_err("malloc failure\n");
            return false;
        }
        else
        {
            memset(g_scene_recording, 0, sizeof(scene_recording_t)); 
            data = g_scene_recording;
        }
    }
    else
    {
        data = g_scene_recording;
    }

    

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

    /*load FM recorder resource*/
    _recording_load_resource(data);

    /*! init resource status */
    data->bg = need_paint;
    data->word_status = need_paint;
    data->filename = need_paint;
    data->pbbg = need_paint;
    data->totaltime = need_paint;
    data->color_status = need_paint;
    data->tape = need_paint;

    /*设置横竖屏方向*/
    

    data->win_id = gui_wm_create_window(data->scene->x, data->scene->y, data->scene->width, data->scene->height,
            WM_CREATE_SHOW, _recording_callback, 0);
    if (data->win_id <= 0)
    {
        print_err("gui create window failed");
        return false;
    }

    data->hdc = gui_dc_get((unsigned short) data->win_id);

    /*设置焦点*/
    gui_wm_set_focus(data->win_id);

    /*绘制headbar提前，避免在进入场景之前弹框没有headbar*/
    if (false == _create_headbar())
    {
        return false;
    }

    /*mount_ada();*/
    mount_dsp_enc();

    /*加载中间件*/
    data->fptr = dlopen("a_codec.so", RTLD_LAZY);

    /*打开中间件*/
    data->ae_handle = mmm_ae_open(NULL);
    if (data->ae_handle == NULL)
    {
        print_err("mmm open failed\n");
        return false;
    }

    _load_config();

    _quality_map_to_bitrate(data->quality);
    _gain_map_to_volt(data->gain);

    /*设置音源为FM*/
    data->param.source.mic = 0;
    data->param.source.fm = 1;
    data->param.source.line_in = 0;

    /*设置录音格式*/
    result = mmm_ae_cmd(data->ae_handle, MMM_SET_FILE_FMT, (unsigned int) "mp3");
    if (result < 0)
    {
        print_err("file mode set failed\n");
        return false;
    }

    /*设置录音参数*/
    result = mmm_ae_cmd(data->ae_handle, MMM_SET_AUDIO_PARAM, (unsigned int) &data->param);
    if (result < 0)
    {
        print_err("param set failed\n");
        return false;
    }


    /*设置文件保存路径*/
    if (false == _set_path())
    {
        print_err("set path error!\n");
        return false;
    }


    data->recorder_status = recording;
    if (sys_power_threshold_check(POWER_THRESHOLD_VOL_REC) != true)

    {

        /* initial dialog data*/
        dialog_init.head = DIALOG_HEAD_LOWBAT;
        dialog_init.str_id = data->rec_dialog_lowbat->str_id;

        /* dialog init and callback function*/
        dialog_modal(&dialog_init); 
        g_scene_recording->next_scene = RADIO_SCENE_PLAYING_BACKWARD;
       

        return false;
    }

    /*检查磁盘空间*/
    ret = _check_diskinfo(data);
    if (ret == false)
    {
        return false;
    }
    else
    {
        /*! 获取日期时间*/
        ret = sys_get_rtc_date_time(&l_time, &l_date);
        if (ret == false)
        {
            print_err("get rtc date time error");
            return false;
        }

        /*! 保存文件名*/
        sprintf(data->file_name_buf, "%04d%02d%02d%02d%02d%02d", l_date.year, l_date.month, l_date.day, l_time.hour,
                l_time.minute, l_time.second);

        /*pbhl, curtime, voice energe 用timer来控制绘图*/
        result = mmm_ae_cmd(data->ae_handle, MMM_START_RECORDING, (unsigned int) data->file_name_buf);
        if (result < 0)
        {
            print_err("start recording error");
            return false;
        }

        print_info("rec start result : %d\n", result);

        data->disk_info_timer_id = set_timer(300, (timer_proc) _check_diskinfo, (void *) data);
        data->vol_energe_timer_id = set_timer(1000, (timer_proc) fm_recorder_draw_vol, (void *) data);
        data->pb_curtime_timer_id = set_timer(300, (timer_proc) fm_recorder_draw_pbhl_curtime, (void *) data);

        
    }
    fusion_get_config(FUSION_TYPE_COMMON, fenter, &eff);
    fusion_set_effect(&eff);

    _scene_recording_fusion_set_first_frame(data->scene);
    /*
     if(false == _create_headbar())
     {
     return false;
     }
     */
    if (false == _create_msgbox())
    {
        return false;
    }

    _recording_paint(data);

    _scene_recording_fusion_set_last_frame(data->scene);
 
    fusion_display(NULL);

    sys_forbid_auto_standby(true);
    clear_key_msg_queue(-1, -1);

    return true;
}

/******************************************************************************/
/*!                    
 * \par  Description:     recording deinit
*******************************************************************************/
static bool _recording_deinit(void)
{
    int ret;
    scene_recording_t *data = g_scene_recording;

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

    /*删除adjust*/
    if (data->adjust != NULL)
    {
        adjust_delete(data->adjust);
        data->adjust = NULL;
    }

    /*删除msgbox*/
    if (data->msgbox != NULL)
    {
        msgbox_delete(data->msgbox);
        data->msgbox = NULL;
    }

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

    if (NULL == data->ae_handle)
    {
        return true;
    }
    /*! 卸载中间件*/
    ret = mmm_ae_close(data->ae_handle, NULL);
    if (ret != 0)
    {
        print_err("ae close : %d\n", ret);
        return false;
    }

    /*! 卸载dspenc*/
    ret = rmmod("dspenc.ko", 0);
    if (ret != 0)
    {
        print_err("dspenc rmmod : %d \n", ret);
        return false;
    }

    /*! 卸载ada*/
    /*
     ret = rmmod("ada.ko",0);
     print_err("ada rmmod : %d\n", ret);
     */

    dlclose(data->fptr);

    free(g_scene_recording);
    g_scene_recording = NULL;

    print_dbg("deinit done\n");

    sys_forbid_auto_standby(false);

    return true;
}
/******************************************************************************/
/*!                    
 * \par  Description:     recording scene
*******************************************************************************/
int radio_scene_recording(fusion_enter_e fenter)
{
    bool bret;
    int next_scene;
    msg_apps_t msg;

    /* 场景初始化 */
    bret = _recording_init(fenter);
    if (bret == false)
    {
        goto record_exit_lable;
    }
    else
    {
        print_dbg("--: %s---%d---\n", __FILE__, __LINE__);
    }

    /* 场景消息循环 */
    register_sys_dispatcher((sys_msg_proc) _recording_proc_sys);

    while (get_msg(&msg) == true)
    {
        dispatch_msg(&msg);
    }

    unregister_sys_dispatcher();
    record_exit_lable:
   
    next_scene = g_scene_recording->next_scene;

    /* 场景退出 */
    bret = _recording_deinit();
    if (bret == false)
    {
        print_err("scene exit error! \n");
        return -1;
    }
    print_dbg("record exit\n");

    return next_scene;
}

