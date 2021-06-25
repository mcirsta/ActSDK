/********************************************************************************
*                              USDK(1100)
*                             Module: TVIN
*                 Copyright(c) 2003-2009 Actions Semiconductor,
*                            All Rights Reserved.
* File : tvin_control.c
* History:
*      <author>    <time>           <version >             <desc>
*      Jerry Fu    2009-4-28             v1.0               build this file
********************************************************************************/
/*!
* \file     tvin_control.c
* \brief    tvin control operation
* \author   Jerry Fu
*
* \version 1.0
* \date  2009/04/28
*******************************************************************************/

#include "tvin_control.h"
#include "tvin_record_ui.h"

#undef NULL
#define NULL 0
#define     TIMEOUT_VAL           (1)
/************************************************************************
*
*                      局部函数原型声明
*
*************************************************************************/


/************************************************************************
*
*                      局部函数原型
*
*************************************************************************/
rtc_time_t time;
rtc_date_t date;

static void *fpmmmdv = NULL;
static void * tvin_mmm_handle = NULL;
static int tvin_fd = 0;

/************************************************************************************/
/*!
 * \para  Description:
 *  读取rtc的时间和日期
 *
 * \param[out]   date_time 读取的时间和日期放在该地址内
 * \return       成功: true   失败: false 
 ************************************************************************************/
bool get_rtc_date_time(rtc_date_time_t *date_time)
{    
    int dev_fd;

    if(NULL == date_time)
    {
        return false;
    }

    dev_fd = open(DEVICE_RTC, O_RDWR);
    if (dev_fd < 0)
    {
        print_err("open %s failed!", DEVICE_RTC);
        goto get_rtc_date_time_err;
    }

    if(ioctl(dev_fd, RTC_GET_DATE_TIME , (int)date_time) != 0)
    {
        print_err("get rtc date and time failed!");
        goto get_rtc_date_time_err;
    }
    close(dev_fd);
    return true;
    
    get_rtc_date_time_err:
    close(dev_fd);
    return false;
}

bool  set_tvin_mode(int signal_mode)
{        
    if(tvin_fd != 0)
    {                
        if(ZONE_PALD == signal_mode)
        {
            ioctl(tvin_fd, VI_IOSET_RESOLUTION, VI_TVIN_RES_PAL352X288);
        }
        else
        {
            ioctl(tvin_fd, VI_IOSET_RESOLUTION, VI_TVIN_RES_NTSC352X240);
        }
        return true;
    }
    else
    {
        print_err("open video_in fd error\n");
        return false;
    }
}

bool tvin_install_videoin(void)
{
    int video_in_type;
    int result = 0;
    /* 加载video in驱动 */
    video_in_type = VI_CVBS_TVIN_MODE;/* video in加载参数 */
    result = insmod("video_in.ko", (void *)&video_in_type);
    print_dbg("insmod video_in result=%x", result);
    if(result != 0)
    {
        print_err("insmod video_in error\n");
        return false;  
    }    
    /* 创建设备节点 */
    result = sys_mknod(DEVICE_VIDEO_IN, S_IFCHR, makedev(MAJOR_VIDEO_IN, 0));

    tvin_fd = open(DEVICE_VIDEO_IN, O_RDWR);
    if(tvin_fd < 0)
    {
        print_err("open video_in error\n");
        return false;
    }
    
    return true;
}


void  tvin_unstall_videoin(void)
{ 
    if(NULL != tvin_fd)
    {
        close(tvin_fd);
        tvin_fd = 0;
    }     
    rmmod("video_in.ko",0);   
}

bool  is_tvin_cable_insert(void)
{
    int result;
    result = ioctl(tvin_fd, VI_IOGET_TVIN_SIGNAL, NULL);
    print_dbg("result=%d\n",result);
    if(result < 0)
    {
        return false;
    }
    else
    {
        return true;
    }    
}

/************************************************************************************/
/*!
 * \para  Description:
 *  加载video_in驱动程序，以及DSP的编码驱动
 *
 * \param[in]    void 
 * \return       成功: true   失败: false 
 ************************************************************************************/
bool tvin_mmm_init(void)
{
#if  TEST_MMMM_DV    
    int result = 0;         
            
    /* 加载DSP驱动 */
    result = insmod("dspenc.ko", (void *)NULL);
    if(result != 0)
    {
        print_err("insmod dspenc error\n");
        return false;
    }
        
    /* 创建设备节点 */
    result = sys_mknod(DEVICE_DSP_AE, S_IFCHR, makedev(MAJOR_DSP_AE, 0));


    fpmmmdv = dlopen("mmm_dv.so", RTLD_NOW);
    if(NULL == fpmmmdv)
    {
        goto open_dv_so_err;
    }

    tvin_mmm_handle = mmm_ve_open(NULL);
    if(NULL == tvin_mmm_handle)
    {
        goto open_video_in_err;
    }
    
    return true;

    open_video_in_err:
    dlclose(fpmmmdv);

    open_dv_so_err:
    rmmod("dspenc.ko", 0);
         
    return false;

#endif
}

/************************************************************************************/
/*!
 * \para  Description:
 *  卸载和TVIN相关的驱动程序
 *
 * \param[in]    void
 * \return       none
 ************************************************************************************/
void tvin_mmm_Close(void)
{
#if  TEST_MMMM_DV
    int result = 0;
    print_dbg();
    if(NULL != tvin_mmm_handle)
    {
        result= mmm_ve_close(tvin_mmm_handle,NULL);
        tvin_mmm_handle = NULL;
    }

    print_dbg();
    if(NULL != fpmmmdv)
    {
        dlclose(fpmmmdv);
        fpmmmdv = NULL;
    }

    print_dbg();
    result = rmmod("dspenc.ko",0);
    print_dbg("%d",result);                   
#endif    
}

/************************************************************************************/
/*!
 * \para  Description:
 *  设置录像质量
 *
 * \param[in]    void
 * \return       none
 ************************************************************************************/
bool  set_record_quality(void)
{
    int result;
    tvin_scene_record_t *prec;
    mmm_video_args_t  video_args;

    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return false;
    }
      
    video_args.frame_rate = 20;
    switch(prec->tvin_config.record_quality)
    {
    case RECORD_QUALITY_LOW:
        video_args.bitrate_level = MMM_VIDDEO_CBR_LEVEL0;
        break;

    case RECORD_QUALITY_MEDIUM:
        video_args.bitrate_level = MMM_VIDDEO_CBR_LEVEL1;
        break;

    case RECORD_QUALITY_HIGH:
        video_args.bitrate_level = MMM_VIDDEO_CBR_LEVEL2;
        break;

    case RECORD_QUALITY_AUTO:
        video_args.bitrate_level = MMM_VIDDEO_VBR;
        break;

    case RECORD_QUALITY_INVALID:
        break;

    default:
        video_args.bitrate_level = MMM_VIDDEO_CBR_LEVEL0;
        break;
    }
      
    result = mmm_ve_cmd( tvin_mmm_handle, MMM_SET_VIDEO_PARAM, (unsigned int)&video_args);
    if(result != 0)
    {
        print_err("MMM_SET_VIDEO_PARAM failed\n");
        return false;
    }
    return true;
}


static bool _set_record_audio(void)
{
    int result = 0;
    mmm_audio_param_t  audio_param;

/* 对音频编码参数进行设置 */
    memset(&audio_param, 0, sizeof(audio_param));

    /* 设置音源从mic进入，tvin模式时需要询问驱动声音从哪个口进入linein、mic还是fm */
    audio_param.source.fm = 1;
    /* 设置增益 */ 

    audio_param.voice_volt.input_volt = 100;

    audio_param.voice_volt.output_volt = 150;
    
    /* 其它编码参数 */
    audio_param.bpp = 16;
    audio_param.channel = 2;
    audio_param.sample_rate = 16000;
    audio_param.bitrate = 64;
    audio_param.encode_mode = 0;   
    result = mmm_ve_cmd(tvin_mmm_handle, MMM_SET_AUDIO_PARAM, (unsigned int)&audio_param);
    if (result != 0)
    {
        return false;
    }

    result = mmm_ve_cmd(tvin_mmm_handle, MMM_START_MONITOR, NULL);
    if (result != 0)
    {
        return false;
    }    
    return true;
}

static bool  _stop_record_audio(void)
{
    int result = 0;
    mmm_audio_param_t  audio_param;

/* 对音频编码参数进行设置 */
    memset(&audio_param, 0, sizeof(audio_param));

    /* 设置音源从mic进入，tvin模式时需要询问驱动声音从哪个口进入linein、mic还是fm */
    audio_param.source.fm = 0;
    /* 设置增益 */ 

    audio_param.voice_volt.input_volt = 100;

    audio_param.voice_volt.output_volt = 150;
    
    /* 其它编码参数 */
    audio_param.bpp = 16;
    audio_param.channel = 2;
    audio_param.sample_rate = 16000;
    audio_param.bitrate = 64;
    audio_param.encode_mode = 0;   

    result = mmm_ve_cmd(tvin_mmm_handle, MMM_STOP_MONITOR, NULL);
    if (result != 0)
    {
        return false;
    }    

    result = mmm_ve_cmd(tvin_mmm_handle, MMM_SET_AUDIO_PARAM, (unsigned int)&audio_param);
    if (result != 0)
    {
        return false;
    }    
    return true;
}

/************************************************************************************/
/*!
 * \para  Description:
 *  设置TVIN的相关参数，启动预览
 *
 * \param[in]    void
 * \return       none
 ************************************************************************************/
bool  tvin_start_preview(void)
{  
#if  TEST_MMMM_DV
    int result = 0;
    tvin_scene_record_t *prec;
    mmm_capture_area_t  capture_area;
    mmm_disk_info_t disk_info;

    print_dbg();
    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return false;
    }
    
    //设置录制文件格式
    result = mmm_ve_cmd(tvin_mmm_handle, MMM_SET_FILE_FMT,(unsigned int)"avi");
    if(result != 0)
    {
        print_err("MMM_SET_FILE_FMT failed\n");
        return false;
    }
   
    //设置音频数据编码格式
    result = mmm_ve_cmd(tvin_mmm_handle, MMM_SET_AUDIO_FMT, (unsigned int)"mp3");
    if(result != 0)
    {
        print_err("MMM_SET_AUDIO_FMT failed\n");
        return false;
    }

    //设置视频数据编码格式
    result = mmm_ve_cmd(tvin_mmm_handle, MMM_SET_VIDEO_FMT, (unsigned int)"xvid");
    if(result != 0)
    {
        print_err("MMM_SET_VIDEO_FMT failed\n");
        return false;
    }
    tvin_set_record_path(prec->tvin_config.save_path);   
        
    //设置分辨率
    result = mmm_ve_cmd(tvin_mmm_handle, MMM_SET_RESOLUTION, (unsigned int)MMM_RES320X240);
    if(result != 0)
    {
        print_err("MMM_SET_RESOLUTION failed\n");
        return false;
    }

    if(false == set_record_quality())
    {
        return false;
    }
    
    //设置视屏显示区域    
    capture_area.xpos = 0x00;
    capture_area.ypos = 0x00;
    capture_area.width = (unsigned int)prec->scene->width;
    capture_area.height = (unsigned int)prec->scene->height;
    result = mmm_ve_cmd(tvin_mmm_handle, MMM_SET_CAPTURE_AREA, (unsigned int)&capture_area);
    if(result != 0)
    {
        print_err("MMM_SET_CAPTURE_AREA failed\n");
        return false;
    }
            
    //发送START_CAPTURE命令开始取景
    result = mmm_ve_cmd(tvin_mmm_handle, MMM_START_CAPTURE, NULL);
    if(result != 0)
    {
        print_err("MMM_START_CAPTURE failed\n");
        return false;
    }

    gui_set_display_mode(GUI_DISPLAY_MODE_YUV);
    
    if(true == tvin_get_disk_info(&disk_info))
    {
        prec->record_remain_time = disk_info.info;
        prec->total_time = disk_info.info;
        prec->cur_record_time = 0;
    }

    _set_record_audio();
    
#endif    
    return true;
}

/************************************************************************************/
/*!
 * \para  Description:
 *  停止预览
 *
 * \param[in]    void
 * \return       none
 ************************************************************************************/
bool tvin_stop_preview(void)
{       
#if  TEST_MMMM_DV
    int result;
    mmm_capture_area_t  capture_area;    
    result = mmm_ve_cmd( tvin_mmm_handle, MMM_STOP_CAPTURE, NULL);    
    if(result != 0)
    {
        print_err("MMM_STOP_CAPTURE failed\n");
        return false;
    }
    
    capture_area.xpos = 0x00;
    capture_area.ypos = 0x00;
    capture_area.width = 0x00;
    capture_area.height = 0x00;
    result = mmm_ve_cmd(tvin_mmm_handle, MMM_SET_CAPTURE_AREA, (unsigned int)&capture_area);        
    if(result != 0)
    {
        print_err("MMM_SET_CAPTURE_AREA failed\n");
        return false;
    }

    _stop_record_audio();
#endif   
    return true;
}

/************************************************************************************/
/*!
 * \para  Description:
 *  创建blending区域
 *
 * \param[in]    blending_area
 * \return       none
 ************************************************************************************/
static bool _blending_area_create(mmm_enc_blending_create_t *blending_area)
{
#if  TEST_MMMM_DV
    int result; 
    result = mmm_ve_cmd(tvin_mmm_handle,MMM_CREATE_BLENDING_AREA,  (unsigned int)blending_area);
    if(result != 0)
    {
        print_err("MMM_SHOW_BLENDING_AREA failed\n");
        return false;
    }
#endif
    return true;
}

/************************************************************************************/
/*!
 * \para  Description:
 *  删除blending区域
 *
 * \param[in]    blending_area
 * \return       none
 ************************************************************************************/
static bool  _blending_area_delete(mmm_enc_blending_create_t *blending_area)
{
#if  TEST_MMMM_DV
    int result ;
    result = mmm_ve_cmd(tvin_mmm_handle,MMM_DESTROY_BLENDING_AREA,  (unsigned int)(blending_area->handle));
    if(result != 0)
    {
        print_err("MMM_SHOW_BLENDING_AREA failed\n");
        return false;
    }
#endif
    return true;
}

/************************************************************************************/
/*!
 * \para  Description:
 *  创建blending区域
 *
 * \param[in]    handle_array  该数组中存放blending区域的句柄
 * \return       none
 ************************************************************************************/
static bool  _blending_area_show(int *handle_array)
{
#if  TEST_MMMM_DV
    int result;
    result = mmm_ve_cmd(tvin_mmm_handle,MMM_SHOW_BLENDING_AREA,  (unsigned int)handle_array);
    if(result != 0)
    {
        print_err("MMM_SHOW_BLENDING_AREA failed\n");
        return false;
    }
#endif
    return true;
}

/************************************************************************************/
/*!
 * \para  Description:
 *  隐藏blending区域
 *
 * \param[in]    handle_array  该数组中存放blending区域的句柄
 * \return       none
 ************************************************************************************/
static bool _blending_area_hide(int *handle_array)
{
#if  TEST_MMMM_DV
    int result ;
    result = mmm_ve_cmd(tvin_mmm_handle,MMM_HIDE_BLENDING_AREA,  (unsigned int)handle_array);
    if(result != 0)
    {
        print_err("hide blending area failed\n");
        return false;
    }
#endif
    return true;
}

/************************************************************************************/
/*!
 * \para  Description:
 *  读取录制的状态
 *
 * \param[out]    dv_status
 * \return       none
 ************************************************************************************/
bool tvin_get_recorder_status(mmm_dv_status_t *dv_status)
{
#if  TEST_MMMM_DV
    int result;
    result = mmm_ve_cmd(tvin_mmm_handle, MMM_GET_RECORDER_STATUS, (unsigned int)dv_status);
    if(result != 0)
    {
        print_err("get recorder_status error\n");
        return false;
    }    
#endif
    return true;
}

/************************************************************************************/
/*!
 * \para  Description:
 *  读取磁盘信息
 *
 * \param[out]   disk_info 
 * \return       none
 ************************************************************************************/
bool tvin_get_disk_info(mmm_disk_info_t *disk_info)
{
#if  TEST_MMMM_DV
    int result;
    result = mmm_ve_cmd(tvin_mmm_handle, MMM_GET_DISK_INFO, (unsigned int)disk_info);    
    if(result != 0)
    {
        print_err("get disk_info error\n");
        return false;
    }
#endif
    return true;
}


static bool _set_record_para(int num)
{
    rtc_time_t *ptime = NULL;    
    tvin_scene_record_t *prec = NULL;
    mmm_disk_info_t  disk_info;
    
    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return false;
    }

    switch(num)
    {
    case 0:
    case 1:
    case 2:
        ptime = &(prec->tvin_config.single_record[num].record_time);
        break;

    case 3:
    case 4:
    case 5:
        ptime = &(prec->tvin_config.repeat_record[num-3].record_time);
        break;

    default:
        break;
    }
    if(true == tvin_get_disk_info(&disk_info))
    {
        prec->record_remain_time = disk_info.info;
        prec->total_time = disk_info.info;
        prec->cur_record_time = 0;
    }
    //定时录像
    if((num>=0) && (num<6))
    {
        prec->alarm_len_time = (unsigned int)((ptime->hour * 3600) + (ptime->minute * 60));  
    }
    else
    {
        prec->alarm_len_time = 0;
    }
    return true;
}


/************************************************************************************/
/*!
 * \para  Description:
 *  设置tvin的录制文件的保存路径
 *
 * \param[in]    保存路径的索引值
 * \return       成功: 0   设置路径出错: -1   没有磁盘: -2 
 ************************************************************************************/
int tvin_set_record_path(tvin_save_path_e  save_path)
{
    tvin_scene_record_t *prec;
    hotplugin_dev_info_t  dev_info;
    hotplugin_dev_type_e main_disk;
    char set_path[32];
    tvin_save_path_e temp_save_path;
    int result = 0; 
       
    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return -1;
    }
    
    switch(save_path)
    {
    case INTERNAL_MEMORY:
        if(false == sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_LOCAL, &dev_info))
        {
            goto handle_path_set_error;
        }
        
        if(HOTPLUGIN_DEV_ACCESS_OK != dev_info.access)
        {
            goto handle_path_set_error;
        }
        
        if(false == sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_TVIN), UDISK_ROOT))
        {
            ;//goto handle_path_set_error;
        }
        snprintf(set_path, 32, sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_TVIN));        
        break;

    case CARD_MEMORY:
        if(false == sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_CARD, &dev_info))
        {
            goto handle_path_set_error;
        }
        
        if(HOTPLUGIN_DEV_ACCESS_OK != dev_info.access)
        {
            goto handle_path_set_error;
        }

        if(false == sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CARD,DIR_TVIN), CARD_ROOT))
        {
            ;//goto handle_path_set_error;
        }
        
        snprintf(set_path, 32, sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CARD,DIR_TVIN));        
        break;

    case EXTERNAL_MEMORY:
        if(false == sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_UHOST, &dev_info))
        {
            goto handle_path_set_error;
        }
       
        if(HOTPLUGIN_DEV_ACCESS_OK != dev_info.access)
        {
            goto handle_path_set_error;
        }

        if(false == sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_UHOST,DIR_TVIN), EXTERNAL_ROOT))
        {
            ;//goto handle_path_set_error;
        }
        snprintf(set_path, 32, sys_get_media_dir(HOTPLUGIN_DEV_TYPE_UHOST,DIR_TVIN));
        break;
    
    default:
        goto handle_path_set_error;
    }
           
    //设置保存路径
    result = mmm_ve_cmd(tvin_mmm_handle, MMM_SET_SAVE_PATH, (unsigned int)set_path);
    if(result < 0)
    {
        print_err("set save path error\n");
        return -1;
    }    
    return 0;

    handle_path_set_error:
    main_disk = sys_get_main_disk();
    if(main_disk == HOTPLUGIN_DEV_TYPE_LOCAL)
    {
        /* 主盘是 本地磁盘*/
        if(NULL == sys_get_main_disk_dir(DIR_TVIN))
        {
            ;//return false;
        }
        snprintf(set_path, 32, sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_TVIN));
        temp_save_path = INTERNAL_MEMORY;
    }
    else if(main_disk == HOTPLUGIN_DEV_TYPE_CARD)
    {
        if(NULL == sys_get_main_disk_dir(DIR_TVIN))
        {
            ;//return false;
        }
        snprintf(set_path, 32, sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CARD,DIR_TVIN));
        temp_save_path = CARD_MEMORY;
    }
    else if(main_disk == HOTPLUGIN_DEV_TYPE_UHOST)
    {
        if(NULL == sys_get_main_disk_dir(DIR_TVIN))
        {
            ;//return false;
        }
        snprintf(set_path, 32, sys_get_media_dir(HOTPLUGIN_DEV_TYPE_UHOST,DIR_TVIN));
        temp_save_path = EXTERNAL_MEMORY;
    } 
    else
    {
         /*没有盘*/
        temp_save_path = INVALID_MEMORY;
        print_err("has no main disk");
        return -2;//no disk!!
    }
    result = mmm_ve_cmd(tvin_mmm_handle, MMM_SET_SAVE_PATH, (unsigned int)set_path);
    if(result < 0)
    {
        print_err("set save path error\n");
        return -1;
    }

    prec->tvin_config.save_path = temp_save_path;
    g_tvin_cfgs[TVIN_CFG_SAVEPATH].value = temp_save_path;
    g_tvin_cfgs[TVIN_CFG_SAVEPATH].changed = true;
    return 0;
    
}
/************************************************************************************/
/*!
 * \para  Description:
 *  根据当前的时间和日期组成要录制的文件名
 *
 * \param[in]    void
 * \return       none
 ************************************************************************************/
static bool _set_record_file(void)
{
#if  TEST_MMMM_DV
    int result;
    rtc_date_time_t  date_time;
    char rec_file_name[PATH_MAX];
    string_resource_t  *str_dialog_info;

    tvin_scene_record_t *prec;
    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return false;
    }

    //设置保存路径,正常返回0，设置失败返回-1，没有磁盘返回-2
    result = tvin_set_record_path(prec->tvin_config.save_path);
    if(-2 == result)
    {
        /*no disk*/
        str_dialog_info = (string_resource_t *)get_scene_child(prec->scene, STR_DISKINS);
        if(str_dialog_info != NULL)
        {
            if (true == tvin_create_dialog(str_dialog_info->str_id, DIALOG_HEAD_WARN))
            {
                print_dbg();
                dialog_paint(prec->dialog);
            } 
        }
        return false;
    }
    else if(-1 == result)
    {
        print_err("set path err");
        return false;
    }
    else
    {
        ;//
    }
    
    //获取当前的系统时间，以此来命名要保存的文件名
    
    if(false == get_rtc_date_time(&date_time))
    {
        return false;
    }
    
    snprintf(rec_file_name, PATH_MAX, "%.4d%.2d%.2d%.2d%.2d%.2d.avi",\
        date_time.date.year, date_time.date.month,date_time.date.day,\
        date_time.time.hour, date_time.time.minute, date_time.time.second);
        
    result = mmm_ve_cmd( tvin_mmm_handle, MMM_SET_FILE_NAME, (unsigned int)rec_file_name);
    if(result != 0) 
    {
        print_err("set file name error\n");
        return false;
    }
    return true;
#endif
}



/************************************************************************************/
/*!
 * \para  Description:
 *  开始录制文件
 *
 * \param[out]    dv_status
 * \return       none
 ************************************************************************************/
bool tvin_start_record_file(int num)
{ 
#if  TEST_MMMM_DV    
    int result; 
    bool  bret;
    mmm_disk_info_t disk_info;
    tvin_scene_record_t  *prec;
    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return false;
    }
    
    bret = _set_record_file();
    if(false == bret)
    {
        return false;
    }

    if(false == _set_record_para(num))
    {
        return false;
    }

    //如果磁盘已满，则提示"空间满"
    if(true == tvin_get_disk_info(&disk_info))
    {
        if(disk_info.info == 0)
        {
            if (true == tvin_create_dialog(prec->str_disk_full->str_id, DIALOG_HEAD_WARN))
            {
                dialog_paint(prec->dialog);
            }  
        }
    }
            
    result = mmm_ve_cmd(tvin_mmm_handle, MMM_START_VIDEO, NULL);
    if(result != 0)
    {
        print_err("start record error\n");
        return false;
    }
    prec->saved_x = prec->xstart_pos;
    prec->pbar_bg_is_show = false;
    sys_forbid_auto_standby(true);
    return true;
#endif
}


void  tvin_alarmin_proc(int alarm_clock)
{
    int i;
    tvin_scene_record_t  *prec = NULL;
    alarm_query_param_t  query_clock_param;    

    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return;
    }
    
    query_clock_param.in.mode_no = 1;         
    query_clock_param.in.clock_no = alarm_clock;
     
    if(true == tvin_query_alarm_clock(&query_clock_param))
    {
        prec->wake_up = query_clock_param.out.wakeup;                
    }
    else
    {
        prec->wake_up = 0;
    }

    for(i=0; i<6; i++)
    {
        if(alarm_clock == prec->tvin_config.alarm_clknum[i])
        {
            break;
        }
    }

    if(i != 6)
    {                    
        if(true == tvin_start_record_file(i))
        {
            prec->tvin_record_sta = TVIN_RECORD_PLAY;
            prec->record_status_is_show = false;           
        }		
    }             
}
static void _deal_stop_record(void)
{
    int result;
    result = mmm_ve_cmd(tvin_mmm_handle, MMM_STOP_VIDEO, NULL);
    if(result != 0)
    {
        print_err("stop record err!");
    }
    sys_enable_sys_evt_detect(true);
    sys_forbid_auto_standby(false);
}

static bool _create_stop_record_thread( void *proc, void *param )
{
    pthread_t tid;
    pthread_attr_t pthread_attr;
    struct sched_param self_param;
    int policy;
    int result = -1;

    /*
     * process manger's msg loop should be activated as long as there is
     * a msg in the process manager's msg queue
     */
    pthread_attr_init(&pthread_attr);
    pthread_attr_setinheritsched(&pthread_attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setdetachstate(&pthread_attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setscope(&pthread_attr, PTHREAD_SCOPE_PROCESS);
    
    //创建比主进程低一级的线程, 保证主进程先运行
    pthread_getschedparam( pthread_self(), &policy, &self_param );
    self_param.sched_priority = self_param.sched_priority - 2;
    pthread_attr_setschedparam(&pthread_attr, &self_param);
    
    print_dbg(" manager_msg_loop  thread's priority is: %d\n", self_param.sched_priority );

    result = pthread_create( &tid, &pthread_attr, proc, param );
    pthread_attr_destroy( &pthread_attr );
    if( result != 0 )
    {
        print_err("can’t create thread, result:%d\n", result);
        return false;
    }
    
    return true;
}

/************************************************************************************/
/*!
 * \para  Description:
 *  停止录制文件,并且要检查一下是否需要关机
 *  因为调用MMM_STOP_VIDEO时，中间件会作保存文件的动作
 *  且在文件保存完之后才退出，录制大文件时耗时较长
 *  所以采用子线程的方式执行这动作
 * \param[out]   void
 * \return       none
 ************************************************************************************/
void tvin_stop_record_file(void)
{
#if  TEST_MMMM_DV
    bool result;
    tvin_scene_record_t  *prec;
    msg_apps_t  msg;    
    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return;
    }
    //停止以及保存文件过程中不允许被系统消息打断
    sys_enable_sys_evt_detect(false);
    result = _create_stop_record_thread(_deal_stop_record, NULL);
    if(result == false)
    {
        print_err("stop and save file err!");
        sys_enable_sys_evt_detect(true);
    }
    if(1 == prec->wake_up)
    {
        msg.type = MSG_POWER_OFF;
        broadcast_msg(&msg);
    }
#endif
}

/************************************************************************************/
/*!
 * \para  Description:
 *  退出该场景以前，需要判断当前是否在录像，如果在录像
 *  需要停止录像并保存录像文件，然后退出。如果没有在录像直接退出。
 *
 * \param[out]   void
 * \return       none
 ************************************************************************************/
void tvin_return_check_recorder(void)
{    
    tvin_scene_record_t *prec;
    mmm_dv_status_t record_status;
    string_resource_t  *str_dialog_info;
    
    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return;
    }        
    tvin_get_recorder_status(&record_status);

    if((record_status.status == MMM_RECORDER_RECORDING) ||\
        (record_status.status == MMM_RECORDER_PAUSED))      
    {        
        mmm_ve_cmd(tvin_mmm_handle, MMM_STOP_VIDEO, NULL);
        prec->tvin_record_sta = TVIN_RECORD_STOP;
        prec->record_status_is_show = false;
        tvin_scene_record_paint(prec);
        str_dialog_info = (string_resource_t *)get_resgroup_child(prec->group, STR_SAVE_RECORD_FILE);
        if(str_dialog_info != NULL)
        {
            if (true == tvin_create_dialog(str_dialog_info->str_id, DIALOG_HEAD_INFO))
            {
                dialog_paint(prec->dialog);
            }
        }
    }
    else
    {
        exit_msg_loop();
    }
}

/************************************************************************************/
/*!
 * \para  Description:
 *  暂停录制文件
 *
 * \param[out]   void
 * \return       none
 ************************************************************************************/
bool tvin_pause_record(void)
{
#if  TEST_MMMM_DV
    int result;
    result = mmm_ve_cmd(tvin_mmm_handle, MMM_PAUSE_VIDEO, NULL);
    if(result != 0)
    {
        print_err("MMM_PAUSE_VIDEO failed\n");
        return false;
    }   
#endif
    return true;
}

/************************************************************************************/
/*!
 * \para  Description:
 *  继续录制文件
 *
 * \param[out]   void
 * \return       none
 ************************************************************************************/
bool tvin_resume_record(void)
{
#if  TEST_MMMM_DV
    int result;
    result = mmm_ve_cmd(tvin_mmm_handle, MMM_RESUME_VIDEO, NULL);
    if(result != 0)
    {
        print_err("MMM_RESUME_VIDEO failed\n");
        return false;
    }
#endif
    return true;
}

/************************************************************************************/
/*!
 * \para  Description:
 *  创建UI界面的blending区域
 *
 * \param[in]    void
 * \return       成功:true  失败:false
 ************************************************************************************/
bool  tvin_ui_blending_create(void)
{
#if  TEST_MMMM_DV
    sty_rect_t srect;
    tvin_scene_record_t *prec;
    canvas_info_t canvas_info = {0};

    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return false;
    }
    
    srect.x = prec->pic_record_status_bg->x;
    srect.y = prec->pic_record_status_bg->y;
    srect.width = prec->pic_record_status_bg->width;
    srect.height = prec->pic_record_status_bg->height;
    prec->blending.button_canvas_id = canvas_create( &srect, TRANSPARENT);
    
    if ( -1 == prec->blending.button_canvas_id )
    {
        print_warning("canvas_create fail\n\n");
        return false;
    }
    //tvin record界面的canvas为提高显示效率, 将canvas设为隐藏
    canvas_hide( prec->blending.button_canvas_id );
    //获得canvas的blending_buf, 用于创建blending
    canvas_get_info( prec->blending.button_canvas_id, &canvas_info );

    prec->blending.show_blending_button = 0;
    prec->blending.button_region.blend_param.color_key   = TVIN_DEFAULT_BLENDING_COLOR_KEY;
    prec->blending.button_region.blend_param.alpha       = TVIN_DEFAULT_BLENDING_ALPHA;
    prec->blending.button_region.blend_param.area.x      = canvas_info.rect.x;
    prec->blending.button_region.blend_param.area.y      = canvas_info.rect.y;
    prec->blending.button_region.blend_param.area.w      = canvas_info.rect.width;
    prec->blending.button_region.blend_param.area.h      = canvas_info.rect.height;
    prec->blending.button_region.blend_param.blending_buf = (unsigned char *)canvas_info.pdata;
    prec->blending.button_region.handle = -1;

    _blending_area_create( &prec->blending.button_region);

        
    //创建str区域的图层
    srect.x = prec->str_record_time->x;
    srect.y = prec->str_record_time->y;
    srect.width = prec->str_record_time->width;
    srect.height= prec->str_record_time->height;
    
    prec->blending.recordtime_canvas_id = canvas_create( &srect, TRANSPARENT );
    if ( -1 == prec->blending.recordtime_canvas_id )
    {
        print_warning("canvas_create fail\n\n");
        return false;
    }
    //playing界面的canvas为提高显示效率, 将canvas设为隐藏
    canvas_hide( prec->blending.recordtime_canvas_id );
    //获得canvas的blending_buf, 用于创建blending
    canvas_get_info( prec->blending.recordtime_canvas_id, &canvas_info );


    /* recordtime blend区域初始化, volumebar blending area init*/
    prec->blending.show_blending_rectime = 0;
    prec->blending.recordtime_region.blend_param.color_key   = TVIN_DEFAULT_BLENDING_COLOR_KEY;
    prec->blending.recordtime_region.blend_param.alpha       = TVIN_DEFAULT_BLENDING_ALPHA;
    prec->blending.recordtime_region.blend_param.area.x      = canvas_info.rect.x;
    prec->blending.recordtime_region.blend_param.area.y      = canvas_info.rect.y;
    prec->blending.recordtime_region.blend_param.area.w      = canvas_info.rect.width;
    prec->blending.recordtime_region.blend_param.area.h      = canvas_info.rect.height;
    prec->blending.recordtime_region.blend_param.blending_buf = (unsigned char *)canvas_info.pdata;
    prec->blending.recordtime_region.handle = -1;

    _blending_area_create( &prec->blending.recordtime_region);
 
    srect.x = prec->str_remain_time->x;
    srect.y = prec->str_remain_time->y;
    srect.width = prec->str_remain_time->width;
    srect.height= prec->str_remain_time->height;
    
    prec->blending.remaintime_canvas_id = canvas_create( &srect, TRANSPARENT );
    if ( -1 == prec->blending.remaintime_canvas_id )
    {
        print_warning("canvas_create fail\n\n");
        return false;
    }
    //playing界面的canvas为提高显示效率, 将canvas设为隐藏
    canvas_hide( prec->blending.remaintime_canvas_id );
    //获得canvas的blending_buf, 用于创建blending
    canvas_get_info( prec->blending.remaintime_canvas_id, &canvas_info );


    /* recordtime blend区域初始化, volumebar blending area init*/
    prec->blending.show_blending_remaintime = 0;
    prec->blending.remain_time_region.blend_param.color_key   = TVIN_DEFAULT_BLENDING_COLOR_KEY;
    prec->blending.remain_time_region.blend_param.alpha       = TVIN_DEFAULT_BLENDING_ALPHA;
    prec->blending.remain_time_region.blend_param.area.x      = canvas_info.rect.x;
    prec->blending.remain_time_region.blend_param.area.y      = canvas_info.rect.y;
    prec->blending.remain_time_region.blend_param.area.w      = canvas_info.rect.width;
    prec->blending.remain_time_region.blend_param.area.h      = canvas_info.rect.height;
    prec->blending.remain_time_region.blend_param.blending_buf = (unsigned char *)canvas_info.pdata;
    
    prec->blending.remain_time_region.handle = -1;

    _blending_area_create( &prec->blending.remain_time_region);
   
    srect.x = prec->pic_pbar_bg->x;
    srect.y = prec->pic_pbar_bg->y;
    srect.width = prec->pic_pbar_bg->width;
    srect.height= prec->pic_pbar_bg->height;
    
    prec->blending.pbar_canvas_id = canvas_create( &srect, TRANSPARENT );
    if ( -1 == prec->blending.pbar_canvas_id )
    {
        print_warning("canvas_create fail\n\n");
        return false;
    }
    //playing界面的canvas为提高显示效率, 将canvas设为隐藏
    canvas_hide( prec->blending.pbar_canvas_id );
    //获得canvas的blending_buf, 用于创建blending
    canvas_get_info( prec->blending.pbar_canvas_id, &canvas_info );


    /* recordtime blend区域初始化, volumebar blending area init*/
    prec->blending.show_blending_pbar= 0;
    prec->blending.pbar_region.blend_param.color_key   = TVIN_DEFAULT_BLENDING_COLOR_KEY;
    prec->blending.pbar_region.blend_param.alpha       = TVIN_DEFAULT_BLENDING_ALPHA;
    prec->blending.pbar_region.blend_param.area.x      = canvas_info.rect.x;
    prec->blending.pbar_region.blend_param.area.y      = canvas_info.rect.y;
    prec->blending.pbar_region.blend_param.area.w      = canvas_info.rect.width;
    prec->blending.pbar_region.blend_param.area.h      = canvas_info.rect.height;
    prec->blending.pbar_region.blend_param.blending_buf = (unsigned char *)canvas_info.pdata;
    
    prec->blending.pbar_region.handle = -1;

    _blending_area_create( &prec->blending.pbar_region);  

#endif
    return true;
}


/************************************************************************************/
/*!
 * \para  Description:
 *  删除UI界面的blending区域
 *
 * \param[in]    void
 * \return       成功:true  失败:false
 ************************************************************************************/
bool tvin_ui_blending_delete(void)
{
#if  TEST_MMMM_DV
    tvin_scene_record_t *prec;
    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return false;
    }
            
    if( -1 != prec->blending.button_region.handle )
    {
        _blending_area_delete( &prec->blending.button_region);
    }
    canvas_delete( prec->blending.button_canvas_id );
    
    if( -1 != prec->blending.recordtime_region.handle )
    {
        _blending_area_delete( &prec->blending.recordtime_region);
    }
    canvas_delete( prec->blending.recordtime_canvas_id);
    
    if( -1 != prec->blending.remain_time_region.handle )
    {
        _blending_area_delete( &prec->blending.remain_time_region);
    }
    canvas_delete( prec->blending.remaintime_canvas_id);
    
    if( -1 != prec->blending.pbar_region.handle )
    {
        _blending_area_delete( &prec->blending.pbar_region);
    }
    canvas_delete( prec->blending.pbar_canvas_id);
    
#endif      
    return true;
}

/************************************************************************************/
/*!
 * \para  Description:
 *  创建headbar控件的blending区域
 *
 * \param[in]    void
 * \return       成功:true  失败:false
 ************************************************************************************/
static bool _headbar_blending_create( void )
{  
#if  TEST_MMMM_DV
    tvin_scene_record_t *prec;
    canvas_info_t canvas_info = {0};

    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return false;
    }
              
    //playing界面的canvas为提高显示效率, 将canvas设为隐藏
    canvas_hide( prec->headbar->canvas_id );
    //获得canvas的blending_buf, 用于创建blending
    canvas_get_info( prec->headbar->canvas_id, &canvas_info );
    
    /* 创建dialog blend区域, create dialog blending area*/
    prec->blending.show_blending_hbar = false;
    prec->blending.hbar_region.blend_param.color_key   = TVIN_DEFAULT_BLENDING_COLOR_KEY;
    prec->blending.hbar_region.blend_param.alpha       = TVIN_DEFAULT_BLENDING_ALPHA;
    prec->blending.hbar_region.blend_param.area.x      = canvas_info.rect.x;
    prec->blending.hbar_region.blend_param.area.y      = canvas_info.rect.y;
    prec->blending.hbar_region.blend_param.area.w      = canvas_info.rect.width;
    prec->blending.hbar_region.blend_param.area.h      = canvas_info.rect.height;
    prec->blending.hbar_region.blend_param.blending_buf = (unsigned char *)canvas_info.pdata;
    prec->blending.hbar_region.handle = -1;
    
    _blending_area_create(&prec->blending.hbar_region); 
#endif
    return true;
}

/************************************************************************************/
/*!
 * \para  Description:
 *  删除headbar控件的blending区域
 *
 * \param[in]    void
 * \return       成功:true  失败:false
 ************************************************************************************/
static bool _headbar_blending_delete(void)
{
#if  TEST_MMMM_DV
    tvin_scene_record_t *prec;
    prec = fetch_tvin_record();    
    if(NULL == prec)
    {
        return false;
    }
    
    if( -1 != prec->blending.hbar_region.handle )
    {
        _blending_area_delete( &prec->blending.hbar_region);
    } 
#endif
    return true;
}

/************************************************************************************/
/*!
 * \para  Description:
 *  创建option控件的blending区域
 *
 * \param[in]    void
 * \return       成功:true  失败:false
 ************************************************************************************/
bool tvin_option_blending_create_show( void )
{
#if  TEST_MMMM_DV
    tvin_scene_record_t *prec;
    canvas_info_t canvas_info = {0};

    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return false;
    }
              
    //playing界面的canvas为提高显示效率, 将canvas设为隐藏
    canvas_hide( prec->pRecMView->canvas_id );
    //获得canvas的blending_buf, 用于创建blending
    canvas_get_info( prec->pRecMView->canvas_id, &canvas_info );
    
    /* 创建dialog blend区域, create dialog blending area*/
    prec->blending.show_blending_option = 0;
    prec->blending.option_region.blend_param.color_key   = TVIN_DEFAULT_BLENDING_COLOR_KEY;
    prec->blending.option_region.blend_param.alpha       = TVIN_DEFAULT_BLENDING_ALPHA;
    prec->blending.option_region.blend_param.area.x      = canvas_info.rect.x;
    prec->blending.option_region.blend_param.area.y      = canvas_info.rect.y;
    prec->blending.option_region.blend_param.area.w      = canvas_info.rect.width;
    prec->blending.option_region.blend_param.area.h      = canvas_info.rect.height;
    prec->blending.option_region.blend_param.blending_buf = (unsigned char *)canvas_info.pdata;
    prec->blending.option_region.handle = -1;
    
    _blending_area_create(&prec->blending.option_region);
    
    tvin_blending_show_spec(TVIN_BLENDING_OPTION);
#endif
    return true;
}

/************************************************************************************/
/*!
 * \para  Description:
 *  删除option控件的blending区域
 *
 * \param[in]    void
 * \return       成功:true  失败:false
 ************************************************************************************/
bool tvin_option_blending_delete(void)
{
#if  TEST_MMMM_DV
    tvin_scene_record_t *prec;
    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return false;
    }
    
    if(-1 != prec->blending.option_region.handle)
    {
        _blending_area_delete( &prec->blending.option_region);
    } 
#endif
    return true;
}

/************************************************************************************/
/*!
 * \para  Description:
 *  创建dialog控件的blending区域
 *
 * \param[in]    void
 * \return       成功:true  失败:false
 ************************************************************************************/
static bool _dialog_blending_create(void)
{
#if  TEST_MMMM_DV
    tvin_scene_record_t *prec;
    canvas_info_t canvas_info = {0};

    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return false;
    }
              
    //playing界面的canvas为提高显示效率, 将canvas设为隐藏
    canvas_hide( prec->dialog->canvas_id );
    //获得canvas的blending_buf, 用于创建blending
    canvas_get_info( prec->dialog->canvas_id, &canvas_info );
    
    /* 创建dialog blend区域, create dialog blending area*/
    prec->blending.show_blending_dialog = 0;
    prec->blending.dialog_region.blend_param.color_key   = TVIN_DEFAULT_BLENDING_COLOR_KEY;
    prec->blending.dialog_region.blend_param.alpha       = TVIN_DEFAULT_BLENDING_ALPHA;
    prec->blending.dialog_region.blend_param.area.x      = canvas_info.rect.x;
    prec->blending.dialog_region.blend_param.area.y      = canvas_info.rect.y;
    prec->blending.dialog_region.blend_param.area.w      = canvas_info.rect.width;
    prec->blending.dialog_region.blend_param.area.h      = canvas_info.rect.height;
    prec->blending.dialog_region.blend_param.blending_buf = (unsigned char *)canvas_info.pdata;
    prec->blending.dialog_region.handle = -1;

    _blending_area_create(&prec->blending.dialog_region);

    tvin_blending_show_spec(TVIN_BLENDING_DIALOG);   
#endif
    return true;
}

/************************************************************************************/
/*!
 * \para  Description:
 *  删除dialog控件的blending区域
 *
 * \param[in]    void
 * \return       成功:true  失败:false
 ************************************************************************************/
static bool _dialog_blending_delete(void)
{
#if  TEST_MMMM_DV
    tvin_scene_record_t *prec;
    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return false;
    }
    
    if(-1 != prec->blending.dialog_region.handle)
    {
        _blending_area_delete( &prec->blending.dialog_region);
    } 
#endif
    return true;
}


/************************************************************************************/
/*!
 * \para  Description:
 *  创建dialog控件的blending区域
 *
 * \param[in]    void
 * \return       成功:true  失败:false
 ************************************************************************************/
bool tvin_adjust_blending_create(void)
{
#if  TEST_MMMM_DV
    tvin_scene_record_t *prec;
    canvas_info_t canvas_info = {0};

    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return false;
    }
              
    //playing界面的canvas为提高显示效率, 将canvas设为隐藏
    canvas_hide( prec->adjust->canvas_id );
    //获得canvas的blending_buf, 用于创建blending
    canvas_get_info( prec->adjust->canvas_id, &canvas_info );
    
    /* 创建dialog blend区域, create dialog blending area*/
    prec->blending.show_blending_adjust= 0;
    prec->blending.adjust_region.blend_param.color_key   = TVIN_DEFAULT_BLENDING_COLOR_KEY;
    prec->blending.adjust_region.blend_param.alpha       = TVIN_DEFAULT_BLENDING_ALPHA;
    prec->blending.adjust_region.blend_param.area.x      = canvas_info.rect.x;
    prec->blending.adjust_region.blend_param.area.y      = canvas_info.rect.y;
    prec->blending.adjust_region.blend_param.area.w      = canvas_info.rect.width;
    prec->blending.adjust_region.blend_param.area.h      = canvas_info.rect.height;
    prec->blending.adjust_region.blend_param.blending_buf = (unsigned char *)canvas_info.pdata;
    prec->blending.adjust_region.handle = -1;

    _blending_area_create(&prec->blending.adjust_region);

    tvin_blending_show_spec(TVIN_BLENDING_DIALOG);
#endif
    return true;
}

/************************************************************************************/
/*!
 * \para  Description:
 *  删除dialog控件的blending区域
 *
 * \param[in]    void
 * \return       成功:true  失败:false
 ************************************************************************************/
bool tvin_adjust_blending_delete(void)
{
#if  TEST_MMMM_DV
    tvin_scene_record_t *prec;
    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return false;
    }
    
    if(-1 != prec->blending.adjust_region.handle)
    {
        _blending_area_delete( &prec->blending.adjust_region);
    } 
#endif
    return true;
}


bool tvin_msgbox_blending_create_show(void)
{
#if  TEST_MMMM_DV
    tvin_scene_record_t *prec;
    canvas_info_t canvas_info = {0};

    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return false;
    }
              
    //playing界面的canvas为提高显示效率, 将canvas设为隐藏
    canvas_hide( prec->msgbox->canvas_id );
    //获得canvas的blending_buf, 用于创建blending
    canvas_get_info( prec->msgbox->canvas_id, &canvas_info );
    
    /* 创建dialog blend区域, create dialog blending area*/
    prec->blending.show_blending_msgbox = 0;
    prec->blending.msgbox_region.blend_param.color_key   = TVIN_DEFAULT_BLENDING_COLOR_KEY;
    prec->blending.msgbox_region.blend_param.alpha       = TVIN_DEFAULT_BLENDING_ALPHA;
    prec->blending.msgbox_region.blend_param.area.x      = canvas_info.rect.x;
    prec->blending.msgbox_region.blend_param.area.y      = canvas_info.rect.y;
    prec->blending.msgbox_region.blend_param.area.w      = canvas_info.rect.width;
    prec->blending.msgbox_region.blend_param.area.h      = canvas_info.rect.height;
    prec->blending.msgbox_region.blend_param.blending_buf = (unsigned char *)canvas_info.pdata;
    prec->blending.msgbox_region.handle = -1;

    _blending_area_create(&prec->blending.msgbox_region);

    tvin_blending_show_spec(TVIN_BLENDING_MSGBOX);
#endif
    return true;
}


bool tvin_msgbox_blending_delete(void)
{
#if  TEST_MMMM_DV
    tvin_scene_record_t *prec;
    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return false;
    }
    
    if(-1 != prec->blending.msgbox_region.handle)
    {
        _blending_area_delete( &prec->blending.msgbox_region);
    } 
#endif
    return true;
}

static bool  _volumebar_blending_createshow(void)
{
#if  TEST_MMMM_DV
    tvin_scene_record_t *prec;
    canvas_info_t canvas_info = {0};

    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return false;
    }
              
    //playing界面的canvas为提高显示效率, 将canvas设为隐藏
    canvas_hide( prec->volume_bar->canvas_id );
    //获得canvas的blending_buf, 用于创建blending
    canvas_get_info( prec->volume_bar->canvas_id, &canvas_info );
    
    /* 创建dialog blend区域, create dialog blending area*/
    prec->blending.show_blending_volumebar = 0;
    prec->blending.volumebar_region.blend_param.color_key   = TVIN_DEFAULT_BLENDING_COLOR_KEY;
    prec->blending.volumebar_region.blend_param.alpha       = TVIN_DEFAULT_BLENDING_ALPHA;
    prec->blending.volumebar_region.blend_param.area.x      = canvas_info.rect.x;
    prec->blending.volumebar_region.blend_param.area.y      = canvas_info.rect.y;
    prec->blending.volumebar_region.blend_param.area.w      = canvas_info.rect.width;
    prec->blending.volumebar_region.blend_param.area.h      = canvas_info.rect.height;
    prec->blending.volumebar_region.blend_param.blending_buf = (unsigned char *)canvas_info.pdata;
    prec->blending.volumebar_region.handle = -1;

    _blending_area_create(&prec->blending.volumebar_region);

    tvin_blending_show_spec(TVIN_BLENDING_VOLUMEBAR);
#endif
    return true;
}



static bool _volumebar_blending_delete(void)
{
#if  TEST_MMMM_DV
    tvin_scene_record_t *prec;
    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return false;
    }
    
    if(-1 != prec->blending.volumebar_region.handle)
    {
        _blending_area_delete( &prec->blending.volumebar_region);
    } 
#endif
    return true;
}

/******************************************************************************/
/*!                    
* \para  Description:
*      显示指定的blending区域
* \param[in]    index  要显示blending区域的序号
* \retval       void
* \ingroup      tvin
*******************************************************************************/
void tvin_blending_show_spec(tvin_blending_area_e blending_index)
{
#if  TEST_MMMM_DV
    tvin_scene_record_t *prec;
    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return;
    }
    
    memset(&prec->blending.blending_handle, BLENDING_HANLE_NULL, sizeof(prec->blending.blending_handle));
    switch(blending_index)
    {
    case TVIN_BLENDING_HEADBAR:
        if(prec->blending.show_blending_hbar == true)
        {
            return;
        }
        prec->blending.blending_handle[0] = prec->blending.hbar_region.handle;
        prec->blending.show_blending_hbar = true;
        break;

    case TVIN_BLENDING_BUTTON:
        if(prec->blending.show_blending_button == true)
        {
            return;
        }
        prec->blending.blending_handle[0] = prec->blending.button_region.handle;
        prec->blending.show_blending_button = true;
        break;
        
    case TVIN_BLENDING_RECTIME:
        if(prec->blending.show_blending_rectime == true)
        {
            return;
        }
        prec->blending.blending_handle[0] = prec->blending.recordtime_region.handle;
        prec->blending.show_blending_rectime = true;
        break;

    case TVIN_BLENDING_REMAIN_TIME:
        if(prec->blending.show_blending_remaintime == true)
        {
            return;
        }
        prec->blending.blending_handle[0] = prec->blending.remain_time_region.handle;
        prec->blending.show_blending_remaintime = true;
        break;

    case TVIN_BLENDING_PBAR:
        if(prec->blending.show_blending_pbar == true)
        {
            return;
        }
        prec->blending.blending_handle[0] = prec->blending.pbar_region.handle;
        prec->blending.show_blending_pbar = true;
        break;

    case TVIN_BLENDING_OPTION:
        if(prec->blending.show_blending_option == true)
        {
            return;
        }
        prec->blending.blending_handle[0] = prec->blending.option_region.handle;
        prec->blending.show_blending_option = true;
        break;

    case TVIN_BLENDING_DIALOG:
        if(prec->blending.show_blending_dialog == true)
        {
            return;
        }
        prec->blending.blending_handle[0] = prec->blending.dialog_region.handle;
        prec->blending.show_blending_dialog = true;
        break;
        
    case TVIN_BLENDING_ADJUST:
        if(prec->blending.show_blending_adjust == true)
        {
            return;
        }
        prec->blending.blending_handle[0] = prec->blending.adjust_region.handle;
        prec->blending.show_blending_adjust= true;
        break;

    case TVIN_BLENDING_MSGBOX:
        if(prec->blending.show_blending_msgbox == true)
        {
            return;
        }
        prec->blending.blending_handle[0] = prec->blending.msgbox_region.handle;
        prec->blending.show_blending_msgbox = true;
        break;

    case TVIN_BLENDING_VOLUMEBAR:
        if(prec->blending.show_blending_volumebar == true)
        {
            return;
        }
        prec->blending.blending_handle[0] = prec->blending.volumebar_region.handle;
        prec->blending.show_blending_volumebar = true;
        break;
    
    default:
        break;
    }

    _blending_area_show(prec->blending.blending_handle);
#endif
}

/******************************************************************************/
/*!                    
* \para  Description:
*      隐藏指定的blending区域
* \param[in]    index  blending区域的序号
* \retval       void
* \ingroup      tvin
*******************************************************************************/
void tvin_blending_hide_spec(tvin_blending_area_e  blending_index)
{
#if  TEST_MMMM_DV
    tvin_scene_record_t *prec;
    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return;
    }
    
    memset(&prec->blending.blending_handle, BLENDING_HANLE_NULL, sizeof(prec->blending.blending_handle));
    switch(blending_index)
    {
    case TVIN_BLENDING_HEADBAR:
        if(prec->blending.show_blending_hbar == false)
        {
            return;
        }
        prec->blending.blending_handle[0] = prec->blending.hbar_region.handle;
        prec->blending.show_blending_hbar = false;
        break;

    case TVIN_BLENDING_BUTTON:
        if(prec->blending.show_blending_button == false)
        {
            return;
        }
        prec->blending.blending_handle[0] = prec->blending.button_region.handle;
        prec->blending.show_blending_button = false;
        break;
        
    case TVIN_BLENDING_RECTIME:
        if(prec->blending.show_blending_rectime == false)
        {
            return;
        }
        prec->blending.blending_handle[0] = prec->blending.recordtime_region.handle;
        prec->blending.show_blending_rectime = false;
        break;

    case TVIN_BLENDING_REMAIN_TIME:
        if(prec->blending.show_blending_remaintime == false)
        {
            return;
        }
        prec->blending.blending_handle[0] = prec->blending.remain_time_region.handle;
        prec->blending.show_blending_remaintime = false;
        break;

    case TVIN_BLENDING_PBAR:
        if(prec->blending.show_blending_pbar == false)
        {
            return;
        }
        prec->blending.blending_handle[0] = prec->blending.pbar_region.handle;
        prec->blending.show_blending_pbar = false;
        break;

    case TVIN_BLENDING_OPTION:
        if(prec->blending.show_blending_option == false)
        {
            return;
        }
        prec->blending.blending_handle[0] = prec->blending.option_region.handle;
        prec->blending.show_blending_option = false;
        break;

    case TVIN_BLENDING_DIALOG:
        if(prec->blending.show_blending_dialog == false)
        {
            return;
        }
        prec->blending.blending_handle[0] = prec->blending.dialog_region.handle;
        prec->blending.show_blending_dialog = false;
        break;

    case TVIN_BLENDING_ADJUST:
        if(prec->blending.show_blending_adjust == false)
        {
            return;
        }
        prec->blending.blending_handle[0] = prec->blending.adjust_region.handle;
        prec->blending.show_blending_adjust = false;
        break;

    case TVIN_BLENDING_MSGBOX:
        if(prec->blending.show_blending_msgbox == false)
        {
            return;
        }
        prec->blending.blending_handle[0] = prec->blending.msgbox_region.handle;
        prec->blending.show_blending_msgbox = false;
        break;

    case TVIN_BLENDING_VOLUMEBAR:
        if(prec->blending.show_blending_volumebar == false)
        {
            return;
        }
        prec->blending.blending_handle[0] = prec->blending.volumebar_region.handle;
        prec->blending.show_blending_volumebar = false;
        break;
           
    default:
        break;
    }

    _blending_area_hide(prec->blending.blending_handle);
#endif
}


/******************************************************************************/
/*!                    
* \para  Description:
*      显示ui界面的blending区域
* \param[in]    void  
* \retval       void
* \ingroup      tvin
*******************************************************************************/
void tvin_record_blending_ui_show(void)
{
#if  TEST_MMMM_DV
    tvin_scene_record_t *prec;
    prec = fetch_tvin_record();
    if((prec->blending.show_blending_hbar) == true)
    {
        return;
    }            
    memset(&prec->blending.blending_handle, BLENDING_HANLE_NULL, sizeof(prec->blending.blending_handle)); 
    
    prec->blending.blending_handle[0] = prec->blending.pbar_region.handle;
    prec->blending.blending_handle[1] = prec->blending.button_region.handle;
    prec->blending.blending_handle[2] = prec->blending.recordtime_region.handle;
    prec->blending.blending_handle[3] = prec->blending.remain_time_region.handle;

#if   TVIN_RECORD_SHOW_HEADBAR
    prec->blending.blending_handle[4] = prec->blending.hbar_region.handle;
#endif

    _blending_area_show(prec->blending.blending_handle);

    prec->blending.show_blending_hbar = true;
    prec->blending.show_blending_button = true;
    prec->blending.show_blending_rectime = true;
    prec->blending.show_blending_remaintime = true;
    prec->blending.show_blending_pbar = true;
#endif
}


/******************************************************************************/
/*!                    
* \para  Description:
*      隐藏UI界面的blending区域
* \param[in]    void
* \retval       void
* \ingroup      tvin
*******************************************************************************/
void tvin_record_blending_ui_hide(void)
{
#if  TEST_MMMM_DV
    tvin_scene_record_t *prec;
    prec = fetch_tvin_record();
    if((prec->blending.show_blending_hbar) == false)
    {
        return;
    }            
    memset(&prec->blending.blending_handle, BLENDING_HANLE_NULL, sizeof(prec->blending.blending_handle)); 
    
    prec->blending.blending_handle[0] = prec->blending.pbar_region.handle;
    prec->blending.blending_handle[1] = prec->blending.button_region.handle;
    prec->blending.blending_handle[2] = prec->blending.recordtime_region.handle;
    prec->blending.blending_handle[3] = prec->blending.remain_time_region.handle;

#if  TVIN_RECORD_SHOW_HEADBAR
    prec->blending.blending_handle[4] = prec->blending.hbar_region.handle;
#endif

    _blending_area_hide(prec->blending.blending_handle);

    prec->blending.show_blending_hbar = false;
    prec->blending.show_blending_button = false;
    prec->blending.show_blending_rectime = false;
    prec->blending.show_blending_remaintime = false;
    prec->blending.show_blending_pbar = false;
#endif
}

/******************************************************************************/
/*!                    
* \para  Description:
*      dialog信息提示框的回调函数
* \param[in]    dialog   dialog结构体指针
* \param[in]    dialog   信号的类型
* \retval       void
* \ingroup      tvin
*******************************************************************************/
static void _tvin_dialog_cb(dialog_t *dialog, widget_sig_dialog_e signal)
{
    tvin_scene_record_t *prec;
    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return;
    }
    
    switch ( signal )
    {
    case WIDGET_SIG_DIALOG_OK:    
    case WIDGET_SIG_DIALOG_CANCAL:
        //点击了:no, 或者return, 不执行任何操作. 删除dialog               
        tvin_blending_hide_spec(TVIN_BLENDING_DIALOG);
        tvin_delete_dialog();        
        if((SCENE_TVIN_PALNTSC == prec->next_scene) || (SCENE_TVIN_EXIT == prec->next_scene))
        {                        
            exit_msg_loop();
        }                
        break;

    default:
        break;
    }
}


/******************************************************************************/
/*!                    
* \para  Description:
*      创建dialog信息提示框
* \param[in]    void
* \retval       成功: true  失败:false
* \ingroup      tvin
*******************************************************************************/
bool tvin_create_dialog(unsigned short str_id, dialog_head_e head)
{
    bool result = true;
    dialog_init_t  dialog_init;
    tvin_scene_record_t *prec;

    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return false;
    }
    
    /*避免多个dialog重叠*/
    if(NULL != prec->dialog)
    {
        tvin_delete_dialog();
    }
    
    memset(&dialog_init, 0, sizeof(dialog_init_t));

    dialog_init.head = head;
    dialog_init.str_id = str_id;

    /* init and callback function */
    prec->dialog = dialog_create(&dialog_init, _tvin_dialog_cb);
    if ( NULL == prec->dialog )
    {
        print_err("dialog_create failed\n\n");
        return false;
    }
    
    _dialog_blending_create();
    dialog_paint(prec->dialog);
    return result;
}

/******************************************************************************/
/*!                    
* \para  Description:
*      销毁dialog信息提示框
* \param[in]    void
* \retval       成功: true  失败:false
* \ingroup      tvin
*******************************************************************************/
bool tvin_delete_dialog(void)
{
    bool result = true;
    tvin_scene_record_t *prec;

    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return false;
    }

    _dialog_blending_delete();
    dialog_delete( prec->dialog);
    prec->dialog = NULL;    
    return result;
}

/******************************************************************************/
/*!                    
* \para  Description:
*      headbar回调函数
* \param[in]    void
* \retval       none
* \ingroup      tvin
*******************************************************************************/
static void _tvin_headbar_cb(headbar_t *headbar, widget_sig_headbar_e signal)
{
    print_dbg("signal:%d", signal);
    switch ( signal )
    {
    case WIDGET_SIG_HEADBAR_NULL:
        break;

    default:
        break;
    }
}

/******************************************************************************/
/*!                    
* \para  Description:
*      创建headbar
* \param[in]    void
* \retval       成功: true  失败:false
* \ingroup      tvin
*******************************************************************************/
bool tvin_create_headbar(void)
{
#if  TVIN_RECORD_SHOW_HEADBAR
    tvin_scene_record_t *prec;
    headbar_init_t hbar_init;
    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return false;
    }

    memset(&hbar_init, 0, sizeof(headbar_init_t));
    
    hbar_init.type = HEADBAR_TYPE_YUV;
    hbar_init.pic_id_head = prec->pic_hb_icon->pic_id;
    hbar_init.str_id_title = prec->str_hb_title->str_id;
            
    /* init type and callback function */
    prec->headbar = headbar_create(&hbar_init, _tvin_headbar_cb);
    if ( NULL == prec->headbar )
    {
        print_err("headbar_create failed\n\n");
        return false;
    }
    _headbar_blending_create();
#endif    
    return true;
}


/******************************************************************************/
/*!                    
* \para  Description:
*      销毁headbar
* \param[in]    void
* \retval       成功: true  失败:false
* \ingroup      tvin
*******************************************************************************/
bool tvin_delete_headbar( void)
{
    bool bret = true;
#if  TVIN_RECORD_SHOW_HEADBAR    
    tvin_scene_record_t *prec;
    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return false;
    }

    _headbar_blending_delete();
    
    bret = headbar_delete( prec->headbar);
    prec->headbar = NULL;
#endif    
    return bret;
}

/******************************************************************************/
/*!                    
* \para  Description:
*      销毁headbar
* \param[in]    void
* \retval       成功: true  失败:false
* \ingroup      tvin
*******************************************************************************/
static bool _delete_volumebar( void)
{
    bool bret = true;
#if  TVIN_RECORD_SHOW_HEADBAR    
    tvin_scene_record_t *prec;
    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return false;
    }

    _volumebar_blending_delete();
    
    bret = volumebar_delete( prec->volume_bar);
    prec->volume_bar = NULL;
#endif    
    return bret;
}

/******************************************************************************/
/*!                    
* \para  Description:
*      headbar回调函数
* \param[in]    void
* \retval       none
* \ingroup      tvin
*******************************************************************************/
static void _volumebar_cb(volumebar_t *volumebar, volumebar_sig_e signal)
{
    switch( signal )
    {
    case WIDGET_SIG_VOLUMEBAR_CHANGED:
        break;

    case WIDGET_SIG_VOLUMEBAR_QUIT:    
        _delete_volumebar();
        break;

    default:        
        break;        
    }
}

/******************************************************************************/
/*!                    
* \para  Description:
*      创建headbar
* \param[in]    void
* \retval       成功: true  失败:false
* \ingroup      tvin
*******************************************************************************/
bool tvin_create_volumebar(void)
{
#if  TVIN_RECORD_SHOW_HEADBAR
    tvin_scene_record_t *prec;
    prec = fetch_tvin_record();
    if(NULL == prec)
    {
        return false;
    }

    prec->volume_bar = volumebar_create(0, (volumebar_cb_t) _volumebar_cb);
    if( prec->volume_bar == NULL )
    {
        print_err("create volumebar error");
    }
    else
    {
        volumebar_paint(prec->volume_bar);
    } 
       
    _volumebar_blending_createshow();
#endif    
    return true;
}


