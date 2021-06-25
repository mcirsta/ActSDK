/*******************************************************************************
 *                              USDK(1100)
 *                            Module: camera_dv
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *        2009-5-26 15:38   1.0             build this file
 *******************************************************************************/
/*!
 * \file     camera_dv_control.c
 * \brief    概述
 * \author
 * \par      GENERAL DESCRIPTION:
 *               dv中间件相关控制
 * \par      EXTERNALIZED FUNCTIONS:
 *               用到外面的模块
 * \version 1.0
 * \date  2009-5-26
 *******************************************************************************/
#include "camera_dv_control.h"

#include "camera_dv_config.h"
#include "camera_dv_ui.h"
#include "camera_scene_dv.h"
#include "camera_define.h"
#include "camera_dv_option_save_path.h"
#include "camera_dv_option_free_space.h"

#undef NULL
#define NULL 0

static void *s_fpmmmdv = NULL;
static unsigned char s_cur_sdram;
static long s_dv_insmod_vin = -1;
static long s_dv_insmod_dspenc = -1;

char dv_format[] = "avi";
char dv_video_format[] = "xvid";
char dv_audio_format[] = "mp3";
mmm_capture_area_t dv_captrueArea;
mmm_video_args_t dv_video_args =
{ 22, MMM_VIDDEO_CBR_LEVEL2 };

void * camera_dv_mhandle = NULL;

char dv_filepath[PATH_MAX] ;//qac del it= sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_VIDEO_CAMERA);
char dv_filename[PATH_MAX] = "20090429231718.avi";

void camera_dv_get_cur_sensor_items(int *offset, int *max_scale)
{

    /*! scale up to MMM_RES640*320 */
    if (s_cur_sdram > 8)
    {
        *offset = 0;
        *max_scale = 3;
    }
    else
    {
        print_dbg("get sdram is 8m\n");
        *offset = 1;
        *max_scale = 2;
    }
}

bool camera_dv_set_parameter_preview(void)
{

    int result = 0;

    result = mmm_ve_cmd(camera_dv_mhandle, MMM_SET_FILE_FMT, (unsigned int) dv_format);
    MMM_RET_JUDGE(result,0,"MMM_SET_FILE_FMT !");
    result = mmm_ve_cmd(camera_dv_mhandle, MMM_SET_AUDIO_FMT, (unsigned int) dv_audio_format);
    MMM_RET_JUDGE(result,0,"MMM_SET_AUDIO_FMT !");
    result = mmm_ve_cmd(camera_dv_mhandle, MMM_SET_VIDEO_FMT, (unsigned int) dv_video_format);
    MMM_RET_JUDGE(result,0,"MMM_SET_VIDEO_FMT !");
    result = mmm_ve_cmd(camera_dv_mhandle, MMM_SET_WHITE_BALANCE,
            (unsigned int) (g_camera_dv_cfgs[CAM_DV_CFG_TYPE_WHITE_BALANCE].value));
    MMM_RET_JUDGE(result,0,"MMM_SET_WHITE_BALANCE !");

    /*明亮度的ui显示用-3 ~ 3 ，设个中间件和config.txt的则用0 ~ 6*/
    result = mmm_ve_cmd(camera_dv_mhandle, MMM_SET_BRIGHTNESS,
            (unsigned int) (g_camera_dv_cfgs[CAM_DV_CFG_TYPE_BRIGHTNESS].value) +3 );
    MMM_RET_JUDGE(result,0,"MMM_SET_BRIGHTNESS !");
    result = mmm_ve_cmd(camera_dv_mhandle, MMM_SET_EXPOSURE_MODE,
            (unsigned int) (g_camera_dv_cfgs[CAM_DV_CFG_TYPE_EXPOSURE_MODE].value));
    MMM_RET_JUDGE(result,0,"MMM_SET_EXPOSURE_MODE !");
    result = mmm_ve_cmd(camera_dv_mhandle, MMM_SET_SPECIAL_EFFECT,
            (unsigned int) (g_camera_dv_cfgs[CAM_DV_CFG_TYPE_SPECIAL_EFFECT].value));
    MMM_RET_JUDGE(result,0,"MMM_SET_SPECIAL_EFFECT !");

    /*饱和度的ui显示用-3 ~ 3 ，设个中间件和config.txt的则用0 ~ 6*/
    result = mmm_ve_cmd(camera_dv_mhandle, MMM_SET_CONTRAST, 3);
#if 0
    /*! 目前没有使用对比度 */
    (unsigned int)(g_camera_dv_cfgs[CAM_DV_CFG_TYPE_CONTRAST].value)
    MMM_RET_JUDGE(result,0,"MMM_SET_CONTRAST !");
#endif

    /*! res,path,info统一在blending后显示 */
    {
        /*config保存的是对应menu中的第几项(0--n)这里需要转换成对应的支持分辨率("WVGA,VGA
         ..........")去显示出来*/
        mmm_res_constant_t temp_res;
        temp_res = camera_dv_ui_resolution_to_buf(g_camera_dv_cfgs[CAM_DV_CFG_TYPE_RESOLUTION].value);

        result = mmm_ve_cmd(camera_dv_mhandle, MMM_SET_RESOLUTION, temp_res);
        MMM_RET_JUDGE(result,0,"MMM_SET_RESOLUTION !");

        /*spec 要求qvga以下分辨率要22帧*/
        if (temp_res > MMM_RES320X240)
        {
            dv_video_args.frame_rate = (unsigned int)g_camera_dv_cfgs[CAM_DV_CFG_TYPE_REC_FRAME_RATE_MIN].value;
        }
        else
        {
            dv_video_args.frame_rate = (unsigned int)g_camera_dv_cfgs[CAM_DV_CFG_TYPE_REC_FRAME_RATE_MAX].value;
        }

        dv_video_args.bitrate_level = camera_dv_ui_quality_to_bitrate(
        g_camera_dv_cfgs[CAM_DV_CFG_TYPE_RECORD_QUALITY].value);

        result = mmm_ve_cmd(camera_dv_mhandle, MMM_SET_VIDEO_PARAM, (unsigned int) (&dv_video_args));
        MMM_RET_JUDGE(result,0,"MMM_SET_VIDEO_PARAM !");
    }
//    result = (int)camera_dv_check_path();
    result = mmm_ve_cmd(camera_dv_mhandle, MMM_GET_RECORDER_STATUS,
            (unsigned int) &(g_camera_scene_dv->dv_config.dv_status));
    MMM_RET_JUDGE(result,0,"MMM_GET_RECORDER_STATUS !");
    assert(g_camera_scene_dv->dv_config.dv_status.status == MMM_RECORDER_STOPPED);

    g_camera_scene_dv->dv_config.dv_status.time = 0;
    if (g_camera_scene_dv->dv_config.dv_status.status != MMM_RECORDER_STOPPED)
    {
        result = mmm_ve_cmd(camera_dv_mhandle, MMM_STOP_VIDEO, (unsigned int) NULL);
        MMM_RET_JUDGE(result,0,"MMM_STOP_VIDEO !");

    }

    if(camera_dv_check_path((hotplugin_dev_type_e)g_camera_dv_cfgs[CAM_DV_CFG_TYPE_SAVE_PATH].value) == true)
    {
        result = mmm_ve_cmd(camera_dv_mhandle, MMM_SET_SAVE_PATH, (unsigned int) &dv_filepath);
        MMM_RET_JUDGE(result,0,"MMM_SET_SAVE_PATH !");
        result = mmm_ve_cmd(camera_dv_mhandle, MMM_GET_DISK_INFO,
                (unsigned int) &(g_camera_scene_dv->dv_config.cfg_freeinfo));
        MMM_RET_JUDGE(result,0,"MMM_GET_DISK_INFO !");
        camera_dv_ui_free_space_to_buf();
    }

    return (bool) result;
}

bool camera_dv_mmm_opening_mobile_storage_out(void)
{
    hotplugin_dev_type_e main_disk = sys_get_main_disk();
    bool result = false;
    if ((camera_dv_mhandle != NULL) && \
        (g_camera_dv_cfgs[CAM_DV_CFG_TYPE_SAVE_PATH].value != main_disk))
    {
        //这里savepath应该始终不会等于maindisk
        result = camera_dv_check_path(main_disk);

        //这里dv_filepath已被设为main_disk
        //给主盘路径，不管g_setpath是否失败
        //不管主盘是否为空，是否为失败，出来都设为主盘
        //main_disk只代表盘插入了，不代表成功
        //是否要放到check path中?
        g_camera_dv_cfgs[CAM_DV_CFG_TYPE_SAVE_PATH].value =  (int)main_disk;

        printf("card out dv save path change to =%s", dv_filepath);

        if((g_camera_dv_cfgs[CAM_DV_CFG_TYPE_SAVE_PATH].value != CAM_DV_STORAGE_NONE) && \
            (g_setpath != false))
        {
            camera_dv_ctr_set_save_path((char *) &dv_filepath);
            camera_dv_ctr_get_disk_info(&(g_camera_scene_dv->dv_config.cfg_freeinfo));
        }
        /*无盘或者不能建路径，paint中会判断none的情况，清除button区域*/
        camera_dv_change_icon(CAM_ICON_STORE_TYPE, (unsigned int)g_camera_dv_cfgs[CAM_DV_CFG_TYPE_SAVE_PATH].value);

    }

    return result;

}
void* camera_dv_get_mmm_handle(void)
{
    return camera_dv_mhandle;
}
int camera_dv_mmm_open(void)
{
    //bool ret;
    int fd = -1;
    int video_in_type = VI_DV_MODE;
    s_cur_sdram = get_sdram_cap();
    if (s_cur_sdram <= 8)
    {
        video_in_type = VI_SMALL_MEM_MODE;
    }
    printf("use sdram = (%d)m \n",s_cur_sdram);

    s_dv_insmod_vin = insmod(("video_in.ko"),(void *)(&video_in_type));
    if(s_dv_insmod_vin != 0)
    {
        s_dv_insmod_vin = -1;
        print_err("video_in.ko ninsmod err%d errnumber=%d\n",s_dv_insmod_vin,errno);
        goto insmod_err;
    }

    s_dv_insmod_dspenc= insmod(("dspenc.ko"),(void *)0);
    if(s_dv_insmod_dspenc != 0)
    {
        s_dv_insmod_dspenc = -1;
        print_err("dspenc.ko ninsmod err%d errnumber=%d\n",s_dv_insmod_dspenc,errno);
        goto insmod_err;
    }

    #if 0
    ret = camera_dv_check_path();
    if (ret == false)
    {
        print_err("camera_dv_check_path err\n");
    }
    #endif
    s_fpmmmdv = dlopen("mmm_dv.so", RTLD_NOW);
    print_info("s_fpmmmdv=%d\n",s_fpmmmdv);

    if (s_fpmmmdv == NULL)
    {
        goto dv_open_err;
    }
#if (DBG_REMNANT_MEMORY == 1)
    unsigned int u;
    u = os_mem_query ();
    printf("-----into camera----os_mem_query_remnant=%x\n",u);
    print_mem();
    printf("-----show all module mem use info--(begin)---\n\n",u);
    dump_all_module_mem_info();
    printf("-----show all module mem use info---(end)--\n\n",u);
#endif /* DBG_REMNANT_MEMORY */

    camera_dv_mhandle = mmm_ve_open(NULL);
    if (camera_dv_mhandle == NULL)
    {
        fd = 0 - errno;
        print_err("ve open ,errno=(%d)\n",fd);
        goto ve_open_err;
    }
    return 0;
    insmod_err: dv_open_err: ve_open_err: camera_dv_mmm_close();
    return fd;


}

bool camera_dv_mmm_close(void)
{
    int result = 0;

    if (camera_dv_mhandle != NULL)
    {
        result = mmm_ve_close(camera_dv_mhandle, NULL);
        MMM_RET_JUDGE(result,0,"mmm_ve_close ERR");
        camera_dv_mhandle = NULL;
    }
    if (s_fpmmmdv != NULL)
    {
        dlclose(s_fpmmmdv);
        s_fpmmmdv = NULL;
    }
    if(s_dv_insmod_vin == 0)  //todo :check change >> -1?
    {
        s_dv_insmod_vin = -1;
        result = rmmod("video_in.ko", 0);
        if (result != 0)
        {
            print_err("video_in rmmod ret=%d errnumber=%d \n", result,errno);
        }
    }
    if(s_dv_insmod_dspenc == 0)
    {
        s_dv_insmod_dspenc = -1;
        result = rmmod("dspenc.ko", 0);
        if (result != 0)
        {
            print_err("dspenc rmmod ret=%d errnumber=%d \n", result,errno);
        }
    }
    return true;
}

bool camera_dv_ctr_start_preview(void)
{
    int result;

    print_dbg();
    result = (int) camera_dv_set_parameter_preview();

    SET_CAM_RECT(dv_captrueArea, 0, 0, (unsigned int)g_camera_scene_dv->scene->width, \
    (unsigned int)g_camera_scene_dv->scene->height);
    result = mmm_ve_cmd(camera_dv_mhandle, MMM_SET_CAPTURE_AREA, (unsigned int) (&dv_captrueArea));
    MMM_RET_JUDGE(result, 0 ,"MMM_SET_CAPTURE_AREA !");
    result = mmm_ve_cmd(camera_dv_mhandle, MMM_START_CAPTURE, NULL);
    MMM_RET_JUDGE(result,0,"MMM_START_CAPTURE !");

    return (bool) result;
}

bool camera_dv_ctr_stop_preview(void)
{
    int result;

    /*退出之前清一下dialog避免残余*/
    camera_dv_dialog_delete();

    if (camera_dv_mhandle != NULL)
    {
        SET_CAM_RECT(dv_captrueArea, 0, 0, (unsigned int)g_camera_scene_dv->scene->width, 0);
        result = mmm_ve_cmd(camera_dv_mhandle, MMM_SET_CAPTURE_AREA, (unsigned int) (&dv_captrueArea));
        MMM_RET_JUDGE(result,0,"MMM_SET_CAPTURE_AREA !");
        result = mmm_ve_cmd(camera_dv_mhandle, MMM_STOP_CAPTURE, NULL);
        MMM_RET_JUDGE(result,0,"MMM_STOP_CAPTURE !");
    }
    else
    {
        result = false;
    }
    return (bool) result;
}

bool camera_dv_get_record_status(mmm_dv_status_t* dv_status)
{
    int result;
    result = mmm_ve_cmd(camera_dv_mhandle, MMM_GET_RECORDER_STATUS, (unsigned int) dv_status);
    MMM_RET_JUDGE(result,0,"MMM_GET_RECORDER_STATUS !");
    //        g_camera_scene_dv->dv_config.dv_status.status = dv_status->status;

    return (bool) result;
}
bool camera_dv_record_stop(void)
{
    int result;
    print_info("bf stop dv rec\n");
    result = mmm_ve_cmd(camera_dv_mhandle, MMM_STOP_VIDEO, (unsigned int) NULL);
    MMM_RET_JUDGE(result,0,"MMM_STOP_VIDEO !");

    file_num++;

    #if (SHORTCUT_PLAYBACK_DV == 1)
    {
        size_t n_file = strlen(dv_filename);
        size_t n_path = strlen(dv_filepath);
        memset(g_camera_dv_cfgs[CAM_DV_CFG_PLAYBACK_ROOT].text,0,\
            strlen(g_camera_dv_cfgs[CAM_DV_CFG_PLAYBACK_ROOT].text));
        strncpy(g_camera_dv_cfgs[CAM_DV_CFG_PLAYBACK_ROOT].text, (const char*)(&dv_filepath),n_path);
        printf("dv toppath=(%s)\n",g_camera_dv_cfgs[CAM_DV_CFG_PLAYBACK_ROOT].text);
        g_camera_dv_cfgs[CAM_DV_CFG_PLAYBACK_ROOT].changed = true;
        camera_dv_cfg_write(&g_camera_dv_cfgs[CAM_DV_CFG_PLAYBACK_ROOT]);

        memset(g_camera_dv_cfgs[CAM_DV_CFG_PLAYBACK_FULLNAME].text,0,\
            strlen(g_camera_dv_cfgs[CAM_DV_CFG_PLAYBACK_FULLNAME].text));//CFG_TEXT_LEN);
        strncpy(g_camera_dv_cfgs[CAM_DV_CFG_PLAYBACK_FULLNAME].text, (const char*)(&dv_filepath),n_path);
        strncat(g_camera_dv_cfgs[CAM_DV_CFG_PLAYBACK_FULLNAME].text, (const char*)(&dv_filename),n_file);
        printf("dv fullname=(%s)\n",g_camera_dv_cfgs[CAM_DV_CFG_PLAYBACK_FULLNAME].text);
        g_camera_dv_cfgs[CAM_DV_CFG_PLAYBACK_FULLNAME].changed = true;
        camera_dv_cfg_write(&g_camera_dv_cfgs[CAM_DV_CFG_PLAYBACK_FULLNAME]);

        #if 0
        strncpy(g_setting_cfgs[SET_CFG_TYPE_SEARCH_FOLDER].text, "off", CFG_TEXT_LEN);
        g_setting_cfgs[SET_CFG_TYPE_SEARCH_FOLDER].changed = true;
        setting_cfg_write(&g_setting_cfgs[SET_CFG_TYPE_SEARCH_FOLDER]);
        #endif
    }
    #endif /* SHORTCUT_PLAYBACK_DV */

    clear_key_msg_queue(-1,-1);
    print_info("af stop dv rec\n");

    return (bool) result;
}
bool camera_dv_record_resume(void)
{
    int result;
    result = mmm_ve_cmd(camera_dv_mhandle, MMM_RESUME_VIDEO, (unsigned int) NULL);
    MMM_RET_JUDGE(result,0,"MMM_RESUME_VIDEO !");
    return (bool) result;
}
bool camera_dv_record_pause(void)
{
    int result;
    result = mmm_ve_cmd(camera_dv_mhandle, MMM_PAUSE_VIDEO, (unsigned int) NULL);
    MMM_RET_JUDGE(result,0,"MMM_PAUSE_VIDEO !");
    return (bool) result;
}

bool camera_dv_record_start(void)
{
    int result;
    rtc_time_t time;
    rtc_date_t date;
    char temp_filename[PATH_MAX];
    FILE *f_temp;
    bool err = true;


    if(file_num >= FILE_MAX)
    {
        camera_dv_check_path((hotplugin_dev_type_e)g_camera_dv_cfgs[CAM_DV_CFG_TYPE_SAVE_PATH].value);
    }
    result = mmm_ve_cmd(camera_dv_mhandle, MMM_SET_SAVE_PATH, (unsigned int) &dv_filepath);
    MMM_RET_JUDGE(result,0,"MMM_SET_SAVE_PATH !");
    /*基本上是预览，改变分辨率，摄制后都会获取剩余容量，
     所以摄制前不取容量*/
    sys_get_rtc_date_time(&time, &date);
    snprintf(dv_filename, sizeof(dv_filename), "%02d%02d%02d.avi", \
        time.hour, time.minute,time.second);
    print_info("filename=%s", dv_filename);

    f_temp = fopen(temp_filename, "r");
    if (f_temp != NULL)
    {//todo 需要一个特定文件测试一下
        fclose(f_temp);
        f_temp = NULL;
        time.second += 1;
        snprintf(dv_filename, sizeof(dv_filename), "%02d%02d%02d.avi", \
            time.hour, time.minute,time.second);
    }

    result = mmm_ve_cmd(camera_dv_mhandle, MMM_SET_FILE_NAME, (unsigned int) dv_filename);
    MMM_RET_JUDGE(result,0,"MMM_SET_FILE_NAME !");

#if (DV_ACTIVITY_DETECT == 1)
    print_info("start rec video activty detect =%d\n",g_camera_dv_cfgs[CAM_DV_CFG_TYPE_ACTIVITY_DETECT].value);
    result = mmm_ve_cmd(camera_dv_mhandle, MMM_ENABLE_VIDEO_MONITOR, \
        (unsigned int) g_camera_dv_cfgs[CAM_DV_CFG_TYPE_ACTIVITY_DETECT].value);
    MMM_RET_JUDGE(result,0,"MMM_ENABLE_VIDEO_MONITOR !");
#endif

    result = mmm_ve_cmd(camera_dv_mhandle, MMM_START_VIDEO, (unsigned int) NULL);
    MMM_RET_JUDGE(result,0,"MMM_START_VIDEO !");
    err = (bool)result;

    if(err == false)
    {
        print_err("dv rec err,restart preview\n");
        result = mmm_ve_cmd(camera_dv_mhandle, MMM_START_CAPTURE, NULL);
        MMM_RET_JUDGE(result,0,"MMM_START_CAPTURE !");
    }

    printf("*********dv name=%s%s********\n", dv_filepath, dv_filename);
    return (bool) result;
}

bool camera_dv_ctr_blending_area_create(mmm_enc_blending_create_t *blending_area)
{
    int result;

    result = mmm_ve_cmd(camera_dv_mhandle, MMM_CREATE_BLENDING_AREA, (unsigned int) blending_area);
    MMM_RET_JUDGE(result,0,"MMM_CREATE_BLENDING_AREA !");

    return (bool) result;
}

bool camera_dv_ctr_blending_area_delete(int blending_handle)
{
    int result;
    result = mmm_ve_cmd(camera_dv_mhandle, MMM_DESTROY_BLENDING_AREA, (unsigned int) blending_handle);
    MMM_RET_JUDGE(result,0,"MMM_DESTROY_BLENDING_AREA !");

    return (bool) result;
}
bool camera_dv_ctr_blending_area_show(int *handle_array)
{
    int result;

    print_dbg("");
    result = mmm_ve_cmd(camera_dv_mhandle, MMM_SHOW_BLENDING_AREA, (unsigned int) handle_array);
    MMM_RET_JUDGE(result,0,"MMM_SHOW_BLENDING_AREA !");

    return (bool) result;
}
bool camera_dv_ctr_blending_area_hide(int *handle_array)
{
    int result;

    result = mmm_ve_cmd(camera_dv_mhandle, MMM_HIDE_BLENDING_AREA, (unsigned int) handle_array);
    MMM_RET_JUDGE(result,0,"MMM_HIDE_BLENDING_AREA !");

    return (bool) result;
}

bool camera_dv_ctr_zoom(void)
{
    int result;

    mmm_zoom_level_t temp =
    { 10, 10 };
    temp.denominator = g_camera_scene_dv->dv_config.cfg_zoom_level.denominator;
    temp.numerator = g_camera_scene_dv->dv_config.cfg_zoom_level.numerator;

    temp.numerator += 10;
    if ((temp.numerator > 30) || (temp.numerator < 10))
    {
        temp.numerator = 10;
    }
    result = mmm_ve_cmd(camera_dv_mhandle, MMM_SET_ZOOM_LEVEL, (unsigned int) (&temp));
    MMM_RET_JUDGE(result,0,"MMM_SET_ZOOM_LEVEL !");
    //camera_dv_save_parameter(cam_dv_parameter_e fun, int parameter)
    g_camera_scene_dv->dv_config.cfg_zoom_level.denominator = temp.denominator;
    g_camera_scene_dv->dv_config.cfg_zoom_level.numerator = temp.numerator;

    return (bool) result;
}

/*! 各个菜单对应 */

bool camera_dv_ctr_set_brightness(unsigned int param)
{
    int result;

    /*明亮度的ui显示用-3 ~ 3 ，设个中间件和config.txt的则用0 ~ 6*/
    result = mmm_ve_cmd(camera_dv_mhandle, MMM_SET_BRIGHTNESS, (unsigned int) param + 3);
    MMM_RET_JUDGE(result,0,"MMM_SET_BRIGHTNESS !");
    return (bool) result;
}

bool camera_dv_ctr_set_white_balance(unsigned int param)
{
    int result;
    result = mmm_ve_cmd(camera_dv_mhandle, MMM_SET_WHITE_BALANCE, (unsigned int) param);
    MMM_RET_JUDGE(result,0,"MMM_SET_WHITE_BALANCE !");
    return (bool) result;
}

bool camera_dv_ctr_set_exposure_mode(unsigned int param)
{
    int result;
    result = mmm_ve_cmd(camera_dv_mhandle, MMM_SET_EXPOSURE_MODE, (unsigned int) param);
    MMM_RET_JUDGE(result,0,"MMM_SET_EXPOSURE_MODE !");
    return (bool) result;
}

bool camera_dv_ctr_set_special_effect(unsigned int param)
{
    int result;
    result = mmm_ve_cmd(camera_dv_mhandle, MMM_SET_SPECIAL_EFFECT, (unsigned int) param);
    MMM_RET_JUDGE(result,0,"MMM_SET_SPECIAL_EFFECT !");
    return (bool) result;
}

bool camera_dv_ctr_set_save_path(char *filepath)
{
    int result;
    result = mmm_ve_cmd(camera_dv_mhandle, MMM_SET_SAVE_PATH, (unsigned int) filepath);
    MMM_RET_JUDGE(result,0,"MMM_SET_SAVE_PATH !");
    return (bool) result;
}

bool camera_dv_ctr_set_resolution(mmm_res_constant_t temp_res)
{
    int result;
    result = mmm_ve_cmd(camera_dv_mhandle, MMM_SET_RESOLUTION, (unsigned int) temp_res);
    MMM_RET_JUDGE(result,0,"MMM_SET_RESOLUTION !");
    return (bool) result;
}

bool camera_dv_ctr_get_disk_info(mmm_disk_info_t *freeinfo)
{
    int result;
    result = mmm_ve_cmd(camera_dv_mhandle, MMM_GET_DISK_INFO, (unsigned int) freeinfo);
    MMM_RET_JUDGE(result,0,"MMM_GET_DISK_INFO !");
    return (bool) result;
}

bool camera_dv_ctr_record_gain(mmm_audio_param_t* param)
{
    int result;
    result = mmm_ve_cmd(camera_dv_mhandle, MMM_SET_AUDIO_PARAM, (unsigned int) param);
    MMM_RET_JUDGE(result,0,"MMM_SET_AUDIO_PARAM !");
    return (bool) result;
}

bool camera_dv_ctr_set_record_quality(mmm_video_args_t* param)
{
    int result;
    result = mmm_ve_cmd(camera_dv_mhandle, MMM_SET_VIDEO_PARAM, (unsigned int) param);
    MMM_RET_JUDGE(result,0,"MMM_SET_WHITE_BALANCE !");
    return (bool) result;
}

bool camera_dv_ctr_set_activity_detect(unsigned int param)
{
    int result;
    result = mmm_ve_cmd(camera_dv_mhandle, MMM_ENABLE_VIDEO_MONITOR, (unsigned int) param);
    MMM_RET_JUDGE(result,0,"MMM_ENABLE_VIDEO_MONITOR !");
    return (bool) result;
}

