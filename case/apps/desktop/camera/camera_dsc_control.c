/*******************************************************************************
 *                              USDK(1100)
 *                            Module: camera_dsc
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *        2009-5-26 15:38   1.0             build this file
 *******************************************************************************/
/*!
 * \file     camera_dsc_control.c
 * \brief    概述
 * \author
 * \par      GENERAL DESCRIPTION:
 *               dsc中间件相关控制
 * \par      EXTERNALIZED FUNCTIONS:
 *               用到外面的模块
 * \version 1.0
 * \date  2009-5-26
 *******************************************************************************/
#include "camera_dsc_control.h"

#include "camera_define.h"
#include "camera_dsc_config.h"
#include "camera_dsc_ui.h"
#include "camera_scene_dsc.h"
#include "camera_dsc_control_pcm_data.h"
#include "camera_dsc_option_save_path.h"
#include "camera_dsc_option_free_space.h"
// TODO:savepath交叉包含


#undef NULL
#define NULL 0

/*
 s_ 静态变量
 g_全局变量
 _静态函数
 */
static void *s_fp_mmm_dsc = NULL;
static char s_dsc_format[] = "jpg";
static long s_dsc_insmod_vin = -1;
static bool s_snapshot_ret = false;

static mmm_capture_area_t s_dsc_captrueArea;
static mmm_photo_data_t s_photo_data =
{ 0, NULL, 0, 0 };
static mmm_res_constant_t s_support_max_res;

void * camera_dsc_mhandle = NULL;
char dsc_filepath[PATH_MAX] ;
//之前用= UDISK_PHOTO_CAMERA;为了避开qac检查删除了初始化
char dsc_filename[PATH_MAX] = "20090429231718.jpg";
mmm_camera_exif_t exifAttr; 

void camera_dsc_get_cur_sensor_items(int *offset, int *max_scale)
{

    /*! scale up to MMM_RES4672X3504 */
    if (s_support_max_res >= MMM_RES2048X1536)
    {
        *offset = 0;
        *max_scale = 7;
    }
    /*! MMM_RES3840X2880 */
    else if (s_support_max_res == MMM_RES1600X1200)
    {
        *offset = 2;
        *max_scale = 6;
    }
    /*! MMM_RES2560X1920  //spec上的1280*960是错的 */
    else if (s_support_max_res == MMM_RES1280X1024)
    {
        *offset = 5;
        *max_scale = 4;
    }
    /*! MMM_RES2048X1536 */
    else if (s_support_max_res == MMM_RES640X480)
    {
        *offset = 7;
        *max_scale = 4;
    }
    /*! 如果错误,default .3M */
    else //default .3M
    {
        print_err("get sensor max res err\n");
        *offset = 7;
        *max_scale = 4;
    }
}

bool camera_dsc_set_parameter_preview(void)
{

    int result = 0;
    result = mmm_ie_cmd(camera_dsc_mhandle, MMM_SET_FLASHLIGHT, \
        (unsigned int) (g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_FLASH_LAMP].value));
    MMM_RET_JUDGE(result,0,"MMM_SET_FLASHLIGHT !");

    result = mmm_ie_cmd(camera_dsc_mhandle, MMM_SET_FILE_FMT, (unsigned int) s_dsc_format);
    MMM_RET_JUDGE(result,0,"MMM_SET_FILE_FMT !");
    result = mmm_ie_cmd(camera_dsc_mhandle, MMM_SET_WHITE_BALANCE,
            (unsigned int) (g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_WHITE_BALANCE].value));
    MMM_RET_JUDGE(result,0,"MMM_SET_WHITE_BALANCE !");

    /*明亮度的ui显示用-3 ~ 3 ，设个中间件和config.txt的则用0 ~ 6*/
    result = mmm_ie_cmd(camera_dsc_mhandle, MMM_SET_BRIGHTNESS,
            (unsigned int) (g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_BRIGHTNESS].value + 3));
    MMM_RET_JUDGE(result,0,"MMM_SET_BRIGHTNESS !");
    result = mmm_ie_cmd(camera_dsc_mhandle, MMM_SET_EXPOSURE_MODE,
            (unsigned int) (g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_EXPOSURE_MODE].value));
    MMM_RET_JUDGE(result,0,"MMM_SET_EXPOSURE_MODE !");
    result = mmm_ie_cmd(camera_dsc_mhandle, MMM_SET_SPECIAL_EFFECT,
            (unsigned int) (g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SPECIAL_EFFECT].value));
    MMM_RET_JUDGE(result,0,"MMM_SET_SPECIAL_EFFECT !");

    /*饱和度的ui显示用-3 ~ 3 ，设个中间件和config.txt的则用0 ~ 6*/
    result = mmm_ie_cmd(camera_dsc_mhandle, MMM_SET_CONTRAST, 3);
#if 0
    /*! 目前没有使用对比度 */
    (unsigned int)(g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_CONTRAST].value)
    MMM_RET_JUDGE(result,0,"MMM_SET_CONTRAST !");
#endif


    /*! 分辨率,路径和剩余张数在blending后一次性显示 */
    {
        mmm_res_constant_t temp_res;
        /*! 获取最大分辨率以自动识别不同sensor */
        result = mmm_ie_cmd(camera_dsc_mhandle, MMM_GET_MAX_RESOLUTION, (unsigned int) &s_support_max_res);
        MMM_RET_JUDGE(result,0,"MMM_GET_MAX_RESOLUTION !");
        print_dbg("support max res=%x \n", s_support_max_res);

        /*config保存的是对应menu中的第几项(0--n)这里需要转换成对应的支持分辨率("WVGA,VGA
        ..........")去显示出来*/
        temp_res = camera_dsc_ui_resolution_to_buf(\
        (g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_RESOLUTION].value));
        result = mmm_ie_cmd(camera_dsc_mhandle, MMM_SET_RESOLUTION, temp_res);
        MMM_RET_JUDGE(result,0,"MMM_SET_RESOLUTION !");
    }

    if(camera_dsc_check_path((hotplugin_dev_type_e)g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value) == true)
    {
        result = mmm_ie_cmd(camera_dsc_mhandle, MMM_SET_SAVE_PATH, (unsigned int) dsc_filepath);
        MMM_RET_JUDGE(result,0,"!!!!!!!MMM_SET_SAVE_PATH !!!!!!!");

        print_info("--->prview bf read space=0x%x,free=%d\n",g_camera_scene_dsc->dsc_config.cfg_freeinfo.info,\
            g_camera_scene_dsc->dsc_config.cfg_freeinfo.space_left);

        result = mmm_ie_cmd(camera_dsc_mhandle, MMM_GET_DISK_INFO,
                (unsigned int) &(g_camera_scene_dsc->dsc_config.cfg_freeinfo));
        MMM_RET_JUDGE(result,0,"MMM_GET_DISK_INFO !");
        print_info("<---prview af read space=0x%x,free=%d\n",g_camera_scene_dsc->dsc_config.cfg_freeinfo.info,\
            g_camera_scene_dsc->dsc_config.cfg_freeinfo.space_left);

    }

    /*! 中间件第一次人脸检测都是0，如果非0才设 */
    if (g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_FACE_IDENTIFY].value != 0)
    {
        result = mmm_ie_cmd(camera_dsc_mhandle, MMM_ENABLE_FACE_DETECT,
            (unsigned int) (g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_FACE_IDENTIFY].value));
        MMM_RET_JUDGE(result,0,"MMM_ENABLE_FACE_DETECT !");
    }

    return (bool) result;
}

/* 将磁盘文件写入小机内存 */
int read_file_to_sdram(unsigned char *dst, char *filename, unsigned int len)
{
    FILE *fp = NULL;
    unsigned int read_len = 0;

    /* 打开文件 */
    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        print_err("open file %s failed\n", filename);
        return -1;
    }

    /* 读文件到内存buf中 */
    read_len = fread((void *) dst, 1, len, fp);
    if (read_len != len)
    {
        print_err("read file error!\n");
    }

    /* 关闭文件 */
    fclose(fp);
    fp = NULL;

    return 0;
}
bool camera_dsc_mmm_opening_mobile_storage_out(void)
{
    hotplugin_dev_type_e main_disk = sys_get_main_disk();
    bool result = false;
    if ((camera_dsc_mhandle != NULL) && \
        (g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value != main_disk))
    {
        //这里savepath应该始终不会等于maindisk
        result = camera_dsc_check_path(main_disk);

        //这里dsc_filepath已被设为main_disk
        //给主盘路径，不管g_setpath是否失败
        //不管主盘是否为空，是否为失败，出来都设为主盘
        //main_disk只代表盘插入了，不代表成功
        //是否要放到check path中?
        g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value =  (int)main_disk;

        printf("card out dsc save path change to =%s", dsc_filepath);

        if((g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value != CAM_DSC_STORAGE_NONE) && \
            (g_setpath != false))
        {
            camera_dsc_ctr_set_save_path((char *) &dsc_filepath);
            camera_dsc_ctr_get_disk_info(&(g_camera_scene_dsc->dsc_config.cfg_freeinfo));
        }
        /*无盘或者不能建路径，paint中会判断none的情况，清除button区域*/
        camera_dsc_change_icon(CAM_ICON_STORE_TYPE, (unsigned int)g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value);

    }

    return result;

}

void* camera_dsc_get_mmm_handle(void)
{
    return camera_dsc_mhandle;
}

int camera_dsc_mmm_open(void)
{
    #if 0
    long result2;
    #endif
    #if 0
    int fd;
    #else
    int ret = -1;
    #endif
    int video_in_type = VI_DC_MODE;
    static unsigned char s_cur_sdram;

    s_cur_sdram = get_sdram_cap();

    if (s_cur_sdram <= 8)
    {
        video_in_type = VI_SMALL_MEM_MODE;
    }
    printf("use sdram = (%d)m \n",s_cur_sdram);

    s_dsc_insmod_vin = insmod(("video_in.ko"),(void *)(&video_in_type));
    if(s_dsc_insmod_vin != 0)
    {
        s_dsc_insmod_vin = -1;
        print_err("video_in.ko ninsmod err%d errnumber=%d\n",s_dsc_insmod_vin,errno);
        goto insmod_err;
    }
    /*之前这里创建节点移到config.c中，open驱动判断移到中间件中*/
    s_fp_mmm_dsc = dlopen("mmm_dc.so", RTLD_NOW);
    print_info("s_fpmmmdv=%d\n",s_fpmmmdv);
    if (s_fp_mmm_dsc == NULL)
    {
        goto dsc_open_err;
    }

    camera_dsc_mhandle = mmm_ie_open(NULL);
    print_dbg("camera_dsc_mhandle=%x cmd=%x\n",camera_dsc_mhandle,mmm_ie_cmd);
    if (camera_dsc_mhandle == NULL)
    {
        ret = 0 - errno;
        print_err("ie open ,errno=(%d)\n",ret);
        goto ie_open_err;
    }
    print_dbg("%d\n", ret);
    return 0;

    insmod_err: dsc_open_err: ie_open_err: camera_dsc_mmm_close();
    return ret;
}

bool camera_dsc_mmm_close(void)
{
    int result = 0;

    if (camera_dsc_mhandle != NULL)
    {
        result = mmm_ie_close(camera_dsc_mhandle, NULL);
        MMM_RET_JUDGE(result,0,"mmm_ie_close ");
        camera_dsc_mhandle = NULL;
    }
    if (s_fp_mmm_dsc != NULL)
    {
        dlclose(s_fp_mmm_dsc);
        s_fp_mmm_dsc = NULL;
    }

    if (s_dsc_insmod_vin == 0)
    {
        s_dsc_insmod_vin = -1;
        result = rmmod("video_in.ko", 0);
        if (result != 0)
        {
            print_err("video_in rmmod ret=%d errnumber=%d \n", result,errno);
        }
    }
    return true;
}

bool camera_dsc_ctr_start_preview(void)
{
    int result;

    result = (int) camera_dsc_set_parameter_preview();

    SET_CAM_RECT(s_dsc_captrueArea, 0, 0, (unsigned int)g_camera_scene_dsc->scene->width, \
    (unsigned int)g_camera_scene_dsc->scene->height);
    result = mmm_ie_cmd(camera_dsc_mhandle, MMM_SET_CAPTURE_AREA, (unsigned int) (&s_dsc_captrueArea));
    MMM_RET_JUDGE(result, 0 ,"MMM_SET_CAPTURE_AREA !");
    result = mmm_ie_cmd(camera_dsc_mhandle, MMM_START_CAPTURE, NULL);
    MMM_RET_JUDGE(result,0,"MMM_START_CAPTURE !");

    print_dbg("%d\n", result);
    return (bool) result;
}

bool camera_dsc_ctr_stop_preview(void)
{
    int result;

    if (camera_dsc_mhandle != NULL)
    {
        SET_CAM_RECT(s_dsc_captrueArea, 0, 0, (unsigned int)g_camera_scene_dsc->scene->width, 0);
        result = mmm_ie_cmd(camera_dsc_mhandle, MMM_SET_CAPTURE_AREA, (unsigned int) (&s_dsc_captrueArea));
        MMM_RET_JUDGE(result,0,"MMM_SET_CAPTURE_AREA !");
        result = mmm_ie_cmd(camera_dsc_mhandle, MMM_STOP_CAPTURE, NULL);
        MMM_RET_JUDGE(result,0,"MMM_STOP_CAPTURE !");
    }
    else
    {
        result = false;
    }
    print_dbg("%d\n", result);
    return (bool) result;
}

bool camera_dsc_set_parameter_snapshot(void)
{
    /*一般在拍照前设定，也可能在设置推出后?*/
    int result;
    rtc_time_t time;
    rtc_date_t date;
    char temp_filename[PATH_MAX];
    FILE *f_temp;
    static char saved_date[16] = {0};
    static char str_date[16];
    int tmp;

    sys_get_rtc_date_time(&time, &date);
    snprintf(str_date,sizeof(str_date), "%04d%02d%02d", date.year, date.month,date.day);
    tmp = strcmp(str_date, saved_date);
    
    //当单个文件夹文件数大于FILE_MAX或者日期发生了变化，则重新找路径
    if((file_num >= FILE_MAX) || (tmp!=0))
    {
        camera_dsc_check_path((hotplugin_dev_type_e)g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value);
    }
    result = mmm_ie_cmd(camera_dsc_mhandle, MMM_SET_SAVE_PATH, (unsigned int) &dsc_filepath);
    MMM_RET_JUDGE(result,0,"MMM_SET_SAVE_PATH !");

    memset(saved_date, 0x00, sizeof(saved_date));
    strcpy(saved_date, str_date);

    /*! 参数设置：文件名 */
    do
    {
        sys_get_rtc_date_time(&time, &date);
        snprintf(dsc_filename,sizeof(dsc_filename), "%02d%02d%02d.jpg", \
            time.hour, time.minute,time.second);
        snprintf(temp_filename,sizeof(temp_filename), "%s%s", dsc_filepath, dsc_filename);

        f_temp = fopen(temp_filename, "r");
        if (f_temp != NULL)
        {
            //todo 需要一个特定文件测试一下
            fclose(f_temp);
            f_temp = NULL;
            result = -1;

        }
        else
        {
            print_info("*****fopen null,is new file\n");
            result = 0;
        }
    }while (result == -1);

    result = mmm_ie_cmd(camera_dsc_mhandle, MMM_SET_FILE_NAME, (unsigned int) dsc_filename);
    if (result != 0)
    {
        print_err("set filename fail  filename = %s\n",dsc_filename);
    }
    MMM_RET_JUDGE(result,0,"MMM_SET_FILE_NAME !");

    /*! 参数设置：扩展信息 */
    exifAttr.time.year = date.year;
    exifAttr.time.month = date.month;
    exifAttr.time.day = date.day;
    exifAttr.time.hour = time.hour;
    exifAttr.time.minute = time.minute;
    exifAttr.time.second = time.second;
    //snprintf(exifAttr.machine,sizeof(exifAttr.machine), "chip of atj213x");
    exifAttr.owner = user_name;
    exifAttr.machine = machine_name;
    print_dbg("real owner=%s\n", exifAttr.owner);
    print_dbg("real machine=%s\n", exifAttr.machine);
    /*! 在_dsc_init中获取config值 */
    //snprintf(exifAttr.owner,sizeof(exifAttr.machine), "albert");
    result = mmm_ie_cmd(camera_dsc_mhandle, MMM_SET_EXIF_INFO, (unsigned int) (&exifAttr));
    MMM_RET_JUDGE(result,0,"MMM_SET_EXIF_INFO !");

    /*! 参数设置：照片上打印日期时间 */
    {
        char temp_str[ARRAY_LENGTH_TIME] = "2009-06-08 15:36:00 ";
        char_pix_info_t ppixinfo;

        if (g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_DATE_STAMP].value == 0)
        {
            s_photo_data.buf = NULL;
            s_photo_data.height = 0;
            s_photo_data.width = 0;
            s_photo_data.photo_data_flag = 0;
            result = mmm_ie_cmd(camera_dsc_mhandle, MMM_PHOTO_DATE, (unsigned int) (&s_photo_data));
            MMM_RET_JUDGE(result,0,"MMM_PHOTO_DATE !");
            /*! 如果不打印直接返回 */
            return (bool) result;
        }
        else if (g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_DATE_STAMP].value == 1)
        {
            snprintf(temp_str, sizeof(temp_str), "%04d-%02d-%02d", date.year, date.month, date.day);
        }
        else if (g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_DATE_STAMP].value == 2)
        {
            snprintf(temp_str, sizeof(temp_str), "%04d-%02d-%02d %02d:%02d:%02d", date.year, date.month, date.day, \
                time.hour, time.minute, time.second);
        }
        else
        {
            assert((g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_DATE_STAMP].value >= 0) &&
                    (g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_DATE_STAMP].value <= 2));
        }
        /*! 在改变分辨率之前就会计算好g_date_stamp_h的高度，
         算法规定 不能超过resolution的w,h 1/3*/
        ppixinfo.pixbuflen = (int)( ((((unsigned int)g_date_stamp_h * \
        (unsigned int)g_date_stamp_h) * 3) * (sizeof (temp_str)))/2);
        /*! gui接口需要把获取的字符点阵放到pixbuf中，在拍照后记得释放 */
        ppixinfo.pixbuf = malloc((unsigned int)ppixinfo.pixbuflen);
        if (ppixinfo.pixbuf == NULL)
        {
            print_err("malloc date stamp pixbuffer\n");
        }
        else
        {
            memset(ppixinfo.pixbuf, 0, (unsigned int)ppixinfo.pixbuflen);
            gui_get_string_pixel_buf(temp_str, g_date_stamp_h, &ppixinfo, GUI_UTF8_ENC);
            s_photo_data.buf = ppixinfo.pixbuf;
            s_photo_data.height = (unsigned int)ppixinfo.height;
            s_photo_data.width = (unsigned int)ppixinfo.bytes_per_line;
            s_photo_data.photo_data_flag = 1;
            result = mmm_ie_cmd(camera_dsc_mhandle, MMM_PHOTO_DATE, (unsigned int) (&s_photo_data));
            MMM_RET_JUDGE(result,0,"MMM_PHOTO_DATE !");
        }
    }

    return (bool) result;
}

static bool _dsc_create_thread(void *proc, void *param)
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
    pthread_getschedparam(pthread_self(), &policy, &self_param);
    self_param.sched_priority = self_param.sched_priority - 2;
    pthread_attr_setschedparam(&pthread_attr, &self_param);

    print_dbg("self thread's priority is: %d\n", self_param.sched_priority );

    result = pthread_create(&tid, &pthread_attr, proc, param);
    pthread_attr_destroy(&pthread_attr);
    if (result != 0)
    {
        print_err("can’t create thread, result:%d\n", result);
        return false;
    }
    return true;
}

static void  _dsc_do_snapshot_done( void )
{
    int result;
    bool err;
    err = s_snapshot_ret ;
    /*! 恢复取景h= LCD HEIGHT */
    SET_CAM_RECT(s_dsc_captrueArea, 0, 0, (unsigned int)g_camera_scene_dsc->scene->width, \
    (unsigned int)g_camera_scene_dsc->scene->height);

    result = mmm_ie_cmd(camera_dsc_mhandle, MMM_SET_CAPTURE_AREA, (unsigned int) (&s_dsc_captrueArea));
    MMM_RET_JUDGE(result,0,"MMM_SET_CAPTURE_AREA !");

    if(err == false)
    {
        print_err("snapshot err,restart preview\n");
        result = mmm_ie_cmd(camera_dsc_mhandle, MMM_START_CAPTURE, NULL);
        MMM_RET_JUDGE(result,0,"MMM_START_CAPTURE !");
    }
    clear_key_msg_queue(-1,-1);


    /*! 在回放前需要写配置最后拍照文件名
    和路径供回放应用列表高亮是使用 */

    #if (SHORTCUT_PLAYBACK_DSC == 1)
    {
        size_t n_file = strlen(dsc_filename);
        size_t n_path = strlen(dsc_filepath);
        memset(g_camera_dsc_cfgs[CAM_DSC_CFG_PLAYBACK_ROOT].text,0,\
            strlen(g_camera_dsc_cfgs[CAM_DSC_CFG_PLAYBACK_ROOT].text));
        strncpy(g_camera_dsc_cfgs[CAM_DSC_CFG_PLAYBACK_ROOT].text, (const char*)(&dsc_filepath),n_path);
        printf("dsc toppath=(%s)\n",g_camera_dsc_cfgs[CAM_DSC_CFG_PLAYBACK_ROOT].text);
        g_camera_dsc_cfgs[CAM_DSC_CFG_PLAYBACK_ROOT].changed = true;
        camera_dsc_cfg_write(&g_camera_dsc_cfgs[CAM_DSC_CFG_PLAYBACK_ROOT]);

        memset(g_camera_dsc_cfgs[CAM_DSC_CFG_PLAYBACK_FULLNAME].text,0,\
            strlen(g_camera_dsc_cfgs[CAM_DSC_CFG_PLAYBACK_FULLNAME].text));//CFG_TEXT_LEN);
        strncpy(g_camera_dsc_cfgs[CAM_DSC_CFG_PLAYBACK_FULLNAME].text, (const char*)(&dsc_filepath),n_path);
        strncat(g_camera_dsc_cfgs[CAM_DSC_CFG_PLAYBACK_FULLNAME].text, (const char*)(&dsc_filename),n_file);
        printf("dsc fullname=(%s)\n",g_camera_dsc_cfgs[CAM_DSC_CFG_PLAYBACK_FULLNAME].text);
        g_camera_dsc_cfgs[CAM_DSC_CFG_PLAYBACK_FULLNAME].changed = true;
        camera_dsc_cfg_write(&g_camera_dsc_cfgs[CAM_DSC_CFG_PLAYBACK_FULLNAME]);
    }
    #endif /* SHORTCUT_PLAYBACK_DSC */

    camera_dsc_dialog_delete();

    if(err == false)
    {
        camera_dsc_dialog_create(CAM_DIALOG_SAVE_FAIL);
    }
    else
    {
        file_num++;   //当前文件夹照片数量加1
    }

    /*! 这个是打印日期到照片上的buf如果没有释放会有问题 */
    if (s_photo_data.buf != NULL)
    {
        free(s_photo_data.buf);
    }

    /*! 获取剩余并改变icon显示 */
    camera_dsc_ctr_get_disk_info(&(g_camera_scene_dsc->dsc_config.cfg_freeinfo));
    camera_dsc_change_icon(CAM_ICON_FREE_NUMBER, (unsigned int) (g_camera_scene_dsc->dsc_config.cfg_freeinfo.info));

    if (g_camera_scene_dsc->dsc_config.cfg_freeinfo.info <= CAM_THRESHOLD_SPACE)
    {
        camera_dsc_warning_nospace();
    }
    printf("********* snapshot name=%s%s********\n", dsc_filepath, dsc_filename);
    //ABTenter_foreground_mode();

    camera_dsc_conttimer_restart();
    sys_forbid_auto_standby(false);
    g_do_snapshotting = false;

    if(g_dsc_quiting == true)
    {
        camera_dsc_quit_do();
    }
    
    
}
static void* _dsc_do_snapshot(void* arg )
{
    int result = -1;

    #if 0
    /*! 拍前设置屏幕h=0 */
    SET_CAM_RECT(s_dsc_captrueArea, 0, 0, (unsigned int)g_camera_scene_dsc->scene->width, 0);
    result = mmm_ie_cmd(camera_dsc_mhandle, MMM_SET_CAPTURE_AREA, (unsigned int) (&s_dsc_captrueArea));
    MMM_RET_JUDGE(result,0,"MMM_SET_CAPTURE_AREA !");
    #endif

    s_snapshot_ret = false;
    result = mmm_ie_cmd(camera_dsc_mhandle, MMM_SNAPSHOT, (unsigned int) NULL);
    print_info("af call mmm_snapshot \n");
    MMM_RET_JUDGE(result,0,"MMM_SNAPSHOT !");
    s_snapshot_ret = (bool)result;

    set_single_shot_timer( 1, (timer_proc) _dsc_do_snapshot_done, NULL );
    return (void*)0;
}
bool camera_dsc_snapshot(void)
{
    int result = 0;
    g_do_snapshotting = true;
    /*测试累计拍照张数，发布时删除*/

    #if 0
    camera_dsc_dialog_create(CAM_DIALOG_SAVEFILE);
    gui_screen_update();

    /*! 拍前设置屏幕h=0 */
    SET_CAM_RECT(s_dsc_captrueArea, 0, 0, (unsigned int)g_camera_scene_dsc->scene->width, 0);
    result = mmm_ie_cmd(camera_dsc_mhandle, MMM_SET_CAPTURE_AREA, (unsigned int) (&s_dsc_captrueArea));
    MMM_RET_JUDGE(result,0,"MMM_SET_CAPTURE_AREA !");
    #endif

    /*! 播放拍照音 */
    shutter_sound_play();

    /*! 检查路径，文件名，拥有者，打印日期 */
    result = (int) camera_dsc_set_parameter_snapshot();

    /*! 拍照 */
    print_info("bf call mmm_snapshot \n");
    #if 0
    result = mmm_ie_cmd(camera_dsc_mhandle, MMM_SNAPSHOT, (unsigned int) NULL);
    print_info("af call mmm_snapshot \n");
    MMM_RET_JUDGE(result,0,"MMM_SNAPSHOT !");
    /*! 拍照时间过久清理按键消息 */
    clear_key_msg_queue(-1,-1);

    err = (bool)result;
    #else
    camera_dsc_dialog_create(CAM_DIALOG_SAVEFILE);
    gui_screen_update();
    //enter_background_mode();

    #if 0
    /*! 拍前设置屏幕h=0 */
    SET_CAM_RECT(s_dsc_captrueArea, 0, 0, (unsigned int)g_camera_scene_dsc->scene->width, 0);
    result = mmm_ie_cmd(camera_dsc_mhandle, MMM_SET_CAPTURE_AREA, (unsigned int) (&s_dsc_captrueArea));
    MMM_RET_JUDGE(result,0,"MMM_SET_CAPTURE_AREA !");
    #endif

    _dsc_create_thread(_dsc_do_snapshot, NULL );
    #endif
    #if 0

    err = s_snapshot_ret ;
    clear_key_msg_queue(-1,-1);
    /*! 恢复取景h= LCD HEIGHT */
    SET_CAM_RECT(s_dsc_captrueArea, 0, 0, (unsigned int)g_camera_scene_dsc->scene->width, \
    (unsigned int)g_camera_scene_dsc->scene->height);

    result = mmm_ie_cmd(camera_dsc_mhandle, MMM_SET_CAPTURE_AREA, (unsigned int) (&s_dsc_captrueArea));
    MMM_RET_JUDGE(result,0,"MMM_SET_CAPTURE_AREA !");

    if(err == false)
    {
        print_err("snapshot err,restart preview\n");
        result = mmm_ie_cmd(camera_dsc_mhandle, MMM_START_CAPTURE, NULL);
        MMM_RET_JUDGE(result,0,"MMM_START_CAPTURE !");
    }

    /*! 在回放前需要写配置最后拍照文件名
    和路径供回放应用列表高亮是使用 */

    #if (SHORTCUT_PLAYBACK_DSC == 1)
    {
        size_t n_file = strlen(dsc_filename);
        size_t n_path = strlen(dsc_filepath);
        memset(g_camera_dsc_cfgs[CAM_DSC_CFG_PLAYBACK_ROOT].text,0,\
            strlen(g_camera_dsc_cfgs[CAM_DSC_CFG_PLAYBACK_ROOT].text));
        strncpy(g_camera_dsc_cfgs[CAM_DSC_CFG_PLAYBACK_ROOT].text, (const char*)(&dsc_filepath),n_path);
        printf("dsc toppath=(%s)\n",g_camera_dsc_cfgs[CAM_DSC_CFG_PLAYBACK_ROOT].text);
        g_camera_dsc_cfgs[CAM_DSC_CFG_PLAYBACK_ROOT].changed = true;
        camera_dsc_cfg_write(&g_camera_dsc_cfgs[CAM_DSC_CFG_PLAYBACK_ROOT]);

        memset(g_camera_dsc_cfgs[CAM_DSC_CFG_PLAYBACK_FULLNAME].text,0,\
            strlen(g_camera_dsc_cfgs[CAM_DSC_CFG_PLAYBACK_FULLNAME].text));//CFG_TEXT_LEN);
        strncpy(g_camera_dsc_cfgs[CAM_DSC_CFG_PLAYBACK_FULLNAME].text, (const char*)(&dsc_filepath),n_path);
        strncat(g_camera_dsc_cfgs[CAM_DSC_CFG_PLAYBACK_FULLNAME].text, (const char*)(&dsc_filename),n_file);
        printf("dsc fullname=(%s)\n",g_camera_dsc_cfgs[CAM_DSC_CFG_PLAYBACK_FULLNAME].text);
        g_camera_dsc_cfgs[CAM_DSC_CFG_PLAYBACK_FULLNAME].changed = true;
        camera_dsc_cfg_write(&g_camera_dsc_cfgs[CAM_DSC_CFG_PLAYBACK_FULLNAME]);
    }
    #endif /* SHORTCUT_PLAYBACK_DSC */

    camera_dsc_dialog_delete();

    if(err == false)
    {
        camera_dsc_dialog_create(CAM_DIALOG_SAVE_FAIL);
    }

    /*! 这个是打印日期到照片上的buf如果没有释放会有问题 */
    if (s_photo_data.buf != NULL)
    {
        free(s_photo_data.buf);
    }

    /*! 获取剩余并改变icon显示 */
    camera_dsc_ctr_get_disk_info(&(g_camera_scene_dsc->dsc_config.cfg_freeinfo));
    camera_dsc_change_icon(CAM_ICON_FREE_NUMBER, (unsigned int) (g_camera_scene_dsc->dsc_config.cfg_freeinfo.info));

    if (g_camera_scene_dsc->dsc_config.cfg_freeinfo.info <= CAM_THRESHOLD_SPACE)
    {
        camera_dsc_warning_nospace();
    }
    printf("********* snapshot name=%s%s********\n", dsc_filepath, dsc_filename);
    sys_forbid_auto_standby(false);
    #endif
    return (bool) result;
}

// 考虑用inline或者define代替
/*
 1，inline 不能在头文件出现，要在调用前
 2，不一定成功，要长度短小
 3，不能有if for switch等
 */
bool camera_dsc_ctr_blending_area_create(mmm_enc_blending_create_t *blending_area)
{
    int result;
    result = mmm_ie_cmd(camera_dsc_mhandle, MMM_CREATE_BLENDING_AREA, (unsigned int) blending_area);
    MMM_RET_JUDGE(result,0,"MMM_CREATE_BLENDING_AREA !");
    return (bool) result;
}

bool camera_dsc_ctr_blending_area_delete(int blending_handle)
{
    int result;
    result = mmm_ie_cmd(camera_dsc_mhandle, MMM_DESTROY_BLENDING_AREA, (unsigned int) blending_handle);
    MMM_RET_JUDGE(result,0,"MMM_DESTROY_BLENDING_AREA !");
    return (bool) result;
}

bool camera_dsc_ctr_blending_area_show(int *handle_array)
{
    int result;
    print_dbg("");
    result = mmm_ie_cmd(camera_dsc_mhandle, MMM_SHOW_BLENDING_AREA, (unsigned int) handle_array);
    MMM_RET_JUDGE(result,0,"MMM_SHOW_BLENDING_AREA !");
    return (bool) result;
}

bool camera_dsc_ctr_blending_area_hide(int *handle_array)
{
    int result;
    result = mmm_ie_cmd(camera_dsc_mhandle, MMM_HIDE_BLENDING_AREA, (unsigned int) handle_array);
    MMM_RET_JUDGE(result,0,"MMM_HIDE_BLENDING_AREA !");
    return (bool) result;
}

bool camera_dsc_ctr_zoom(void)
{
    int result;
    mmm_zoom_level_t temp =
    { 10, 10 };
    temp.denominator = g_camera_scene_dsc->dsc_config.cfg_zoom_level.denominator;
    temp.numerator = g_camera_scene_dsc->dsc_config.cfg_zoom_level.numerator;

    temp.numerator += 10;
    if ((temp.numerator > 30) || (temp.numerator < 10))
    {
        temp.numerator = 10;
    }
    result = mmm_ie_cmd(camera_dsc_mhandle, MMM_SET_ZOOM_LEVEL, (unsigned int) &temp);
    MMM_RET_JUDGE(result,0,"MMM_SET_ZOOM_LEVEL !");
    //camera_dsc_save_parameter(cam_dsc_parameter_e fun, int parameter)
    //放大倍数不保存到配置
    g_camera_scene_dsc->dsc_config.cfg_zoom_level.denominator = temp.denominator;
    g_camera_scene_dsc->dsc_config.cfg_zoom_level.numerator = temp.numerator;

    camera_dsc_change_icon(CAM_ICON_FOCI_SLIDER, 0);

    return (bool) result;
}

bool camera_dsc_ctr_set_face_detect(unsigned int param)
{
    int result;
    result = mmm_ie_cmd(camera_dsc_mhandle, MMM_ENABLE_FACE_DETECT, (unsigned int) (param));
    MMM_RET_JUDGE(result,0,"MMM_ENABLE_FACE_DETECT !");
    return (bool) result;
}

bool camera_dsc_ctr_set_brightness(unsigned int param)
{
    int result;

    /*明亮度的ui显示用-3 ~ 3 ，设个中间件和config.txt的则用0 ~ 6*/
    result = mmm_ie_cmd(camera_dsc_mhandle, MMM_SET_BRIGHTNESS, (unsigned int) param + 3);
    MMM_RET_JUDGE(result,0,"MMM_SET_BRIGHTNESS !");
    return (bool) result;
}
bool camera_dsc_ctr_set_exposure_mode(unsigned int param)
{
    int result;
    result = mmm_ie_cmd(camera_dsc_mhandle, MMM_SET_EXPOSURE_MODE, (unsigned int) (param));
    MMM_RET_JUDGE(result,0,"MMM_SET_EXPOSURE_MODE !");
    return (bool) result;
}

bool camera_dsc_ctr_set_flashlight(unsigned int param)
{
    int result;
    result = mmm_ie_cmd(camera_dsc_mhandle, MMM_SET_FLASHLIGHT, (unsigned int) (param));
    MMM_RET_JUDGE(result,0,"MMM_SET_FLASHLIGHT !");
    return (bool) result;
}

bool camera_dsc_ctr_get_disk_info(mmm_disk_info_t *freeinfo)
{
    int result;
    result = mmm_ie_cmd(camera_dsc_mhandle, MMM_GET_DISK_INFO, (unsigned int) (freeinfo));
    MMM_RET_JUDGE(result,0,"MMM_GET_DISK_INFO !");
    return (bool) result;
}

bool camera_dsc_ctr_set_resolution(mmm_res_constant_t temp_res)
{
    int result;
    result = mmm_ie_cmd(camera_dsc_mhandle, MMM_SET_RESOLUTION, (unsigned int) temp_res);
    MMM_RET_JUDGE(result,0,"MMM_SET_RESOLUTION !");
    return (bool) result;
}
bool camera_dsc_ctr_set_save_path(char *filepath)
{
    int result;
    result = mmm_ie_cmd(camera_dsc_mhandle, MMM_SET_SAVE_PATH, (unsigned int) filepath);
    MMM_RET_JUDGE(result,0,"MMM_SET_SAVE_PATH !");
    return (bool) result;
}

bool camera_dsc_ctr_set_special_effect(unsigned int param)
{
    int result;
    result = mmm_ie_cmd(camera_dsc_mhandle, MMM_SET_SPECIAL_EFFECT, (unsigned int) (param));
    MMM_RET_JUDGE(result,0,"MMM_SET_SPECIAL_EFFECT !");
    return (bool) result;
}

bool camera_dsc_ctr_set_photo_sticker(mmm_photo_sticker_t* sticker_para)
{
    int result;
    result = mmm_ie_cmd(camera_dsc_mhandle, MMM_PHOTO_STICKER, (unsigned int) sticker_para);
    MMM_RET_JUDGE(result,0,"MMM_SET_STICKER_PHOTO !");
    return (bool) result;
}

bool camera_dsc_ctr_set_white_balance(unsigned int param)
{
    int result;
    result = mmm_ie_cmd(camera_dsc_mhandle, MMM_SET_WHITE_BALANCE, (unsigned int) (param));
    MMM_RET_JUDGE(result,0,"MMM_SET_WHITE_BALANCE !");
    return (bool) result;
}

