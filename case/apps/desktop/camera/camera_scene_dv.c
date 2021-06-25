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
 * \file    camera_scene_dv.c
 * \brief    camera-dv场景
 * \author   Albert_lyu
 * \version 1.0
 * \date  2009-5-26
 *******************************************************************************/
#include "camera_scene_dv.h"
#include "camera_dv_ui.h"
#include "camera_dv_config.h"
#include "camera_dv_control.h"
#include "camera_sty.h"
#include "camera_dv_widget_option.h"
#include "camera_define.h"
#include "camera_main.h"
#include "camera_dv_option_save_path.h"
#include "camera_dv_option_free_space.h"
#include "camera_dv_option.h"

static char s_use_free_number_saved_buf[ARRAY_LENGTH_TIME] = "aa:59:59/99:59:59 ";
static cam_dialog_e s_dialog_str_type = 0;
static bool s_dv_deinited = false;
static bool s_shortcut_playback = false;
static bool s_do_stoping = false;
static cam_dialog_e s_dialog_recstopped = CAM_DIALOG_TAG_ERR; 
static bool s_dv_quiting = false;

camera_scene_dv_t* g_camera_scene_dv = NULL;

#if (DV_ICON_STATUS2SDRAM == 1)
gui_bitmap_info_t *sdram_buf_icon_info[3] ;
picregion_frame_t sdram_buf_icon_frame[3] ;
static bool _dv_ui_load_icon2sdram(void);
static bool _dv_ui_unload_icon2sdram(void);
#endif /* DV_ICON_STATUS2SDRAM */
static bool  _dv_get_tvout_info(unsigned int *tvout_info);
static bool _dv_is_tvout_on(void);
static void _dv_recording_event(void *data);
static int _dv_quit(void);
static bool _dv_init(void);
static bool _dv_deinit(void);
static int _dv_proc_gui(camera_scene_dv_t *data, gui_wm_message_t *pmsg);
static void _dv_callback(gui_wm_message_t *pmsg);
static int _dv_sys_proc(msg_apps_t *msg);
static bool _dv_open_theme(void);
static bool _dv_close_theme(void);
static bool _dv_open_str_res(void);
static bool _dv_close_str_res(void);
static bool _dv_set_default_font(void);
static bool _dv_unset_default_font(void);
static bool _dv_ui_load_style(camera_scene_dv_t *data);
static bool _dv_init_blendig(resgroup_resource_t* blending_area, camera_blending_section_t *section);
static bool _dv_ui_blending(void);
/* blending area use by Albert_lyu 2009-06-24 9:52:54 begin */
static bool _dv_get_dialog_init(cam_dialog_e type, dialog_init_t *dialog_init);
static void _dv_dialog_callback(dialog_t *dialog, widget_sig_dialog_e signal);
static bool _dv_dialog_blending_create_show(void);
static bool _dv_dialog_blending_delete(void);
//bool camera_dv_dialog_create(cam_dialog_e type);
//bool camera_dv_dialog_delete(void);

static void _dv_volumebar_callback(volumebar_t *data, int signal);
static bool _dv_volumebar_blending_create_show(void);
static bool _dv_volumebar_blending_delete(void);
static bool _dv_volumebar_create(void);
//bool camera_dv_option_blending_create_show(void);
static void _dv_widget_option_callback(menuview_t *menuview, menuview_sig_e signal);

//bool camera_dv_adjust_blending_create_show(void);
//bool camera_dv_adjust_blending_delete(void);
//bool camera_dv_option_blending_delete(void);
static bool _dv_msgbox_blending_create_show(void);
static void _dv_msgbox_callback(msgbox_t *msgbox, msgbox_sig_e signal);
static bool _dv_msgbox_blending_create_show(void);
static bool _dv_msgbox_blending_delete(void);
//bool camera_dv_msgbox_create(void);
//bool camera_dv_msgbox_delete(void);

/* blending area use by Albert_lyu 2009-06-24 end*/
static void _dv_change_record_action(mmm_dv_status_t dv_status, change_rec_even_e even_type);
static void _dv_change_record_stop(void);

int dv_record_ing = 0;

struct timeval tv[50];

static bool  _dv_get_tvout_info(unsigned int *tvout_info)
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

static bool _dv_is_tvout_on(void)
{
    int tvout_info;
    bool ret = false;
    if(true == _dv_get_tvout_info(&tvout_info))
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



static void _dv_recording_event(void *data)
{
    static int s_count_time= 0;
    mmm_dv_status_t temp_status;
    camera_dv_get_record_status(&temp_status);
    print_info("moving=%d,recing=%d,time = %d\n", temp_status.monitor_status, temp_status.status, temp_status.time);

    if(temp_status.status == MMM_RECORDER_ERROR)
    {
        //camera_dv_dialog_create(CAM_DIALOG_OPEN_FAIL);
        //printf("card out dv status err\n");
        /*! 卡拔出本身有提示，这里就不提示直接停止 */
        s_dialog_recstopped = CAM_DIALOG_SAVE_FAIL;        
        _dv_change_record_stop();
        #if 0
        camera_dv_dialog_create(CAM_DIALOG_SAVE_FAIL);
        gui_screen_update();
        #endif
        return;
    }

#if (DV_ACTIVITY_DETECT == 1)
    if (g_camera_dv_cfgs[CAM_DV_CFG_TYPE_ACTIVITY_DETECT].value == CAM_DV_ACT_DETECT_ON)
    {
        if(((temp_status.monitor_status == MMM_MOVING) && (temp_status.status == MMM_RECORDER_PAUSED)) ||
                ((temp_status.monitor_status == MMM_STILL) && (temp_status.status == MMM_RECORDER_RECORDING)))
        {
            s_count_time ++;
            if (s_count_time > 2)
            {
                s_count_time = 0;
                print_info("timer dvr even change moving=%d recing=%d\n",temp_status.monitor_status,temp_status.status);
                _dv_change_record_action(temp_status,CHANGE_REC_EVEN_TIMER);
            }
        }
        else
        {
            s_count_time = 0;
        }
    }
    //TODO 不要动静检测的时候，是否要停止timer
    //如果有动静检测key不能暂停录像
#endif
    camera_dv_ctr_get_disk_info(&(g_camera_scene_dv->dv_config.cfg_freeinfo)/*&temp_freeinfo*/);
    //TODO 要增加判断磁盘不够的情况
    if (g_camera_scene_dv->dv_config.cfg_freeinfo.info <= CAM_THRESHOLD_SPACE)
    {
        //杀掉timer
        s_dialog_recstopped = CAM_DIALOG_NOSPACE;
        _dv_change_record_stop();
        //警示框3s，磁盘不够
        //dialog注意先后顺序保证create和delete是成对出现
        #if 0
        camera_dv_dialog_create(CAM_DIALOG_NOSPACE);
        #endif
        return;
    }

    if (g_camera_scene_dv->dv_config.dv_status.time != temp_status.time)
    {
        //g_camera_scene_dv->dv_config.cfg_freeinfo.info = temp_freeinfo.info;
        //g_camera_scene_dv->dv_config.cfg_freeinfo.space_left = temp_freeinfo.space_left;
        g_camera_scene_dv->dv_config.dv_status.time = temp_status.time;
        print_info("get free PCS=%d,MB=%d\n", g_camera_scene_dv->dv_config.cfg_freeinfo.info,
                g_camera_scene_dv->dv_config.cfg_freeinfo.space_left);
        camera_dv_change_icon(CAM_ICON_USE_FREE_NUMBER, 0);
        print_info("show free space over.\n");
    }

}
/*!==========blending处理=========== */
#define B_DIALOG
static bool _dv_get_dialog_init(cam_dialog_e type, dialog_init_t *dialog_init)
{
    dialog_init_t init;
    scene_t *scene;
    resgroup_resource_t *resgroup = NULL;
    string_resource_t *str_res = NULL;

    s_dialog_str_type = type;
    if (NULL == dialog_init)
    {
        print_err("param err");
        return false;
    }
    //get scene
    scene = sty_get_app_scene();
    resgroup = (resgroup_resource_t *) get_scene_child(scene, DV_STRGROUP_DIALOG);

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

        case CAM_DIALOG_SETPATH_FAIL:
        str_res = (string_resource_t *) get_resgroup_child(resgroup, STR_SETPATH_FAIL);
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

static void _dv_dialog_callback(dialog_t *dialog, widget_sig_dialog_e signal)
{
    print_dbg("signal:0x%x", signal);
    switch (signal)
    {
        case WIDGET_SIG_DIALOG_OK:
        camera_dv_dialog_delete();
        if (g_camera_scene_dv->need_exit == true)
        {
            g_camera_scene_dv->exit_type = CAM_SCENE_EXIT;
            _dv_quit();
        }
        break;

        case WIDGET_SIG_DIALOG_CANCAL:
        if(s_do_stoping == true)
        {
            break;
        }
        camera_dv_dialog_delete();
        if (g_camera_scene_dv->need_exit == true)
        {
            g_camera_scene_dv->exit_type = CAM_SCENE_EXIT;
            _dv_quit();
        }
        break;
        default:
        break;
    }
}

static bool _dv_dialog_blending_create_show(void)
{
    bool result = true;
    canvas_info_t canvas_info =
    { 0 };
    if (camera_dv_get_mmm_handle() == NULL)
    {
        return true;
    }
    canvas_hide(g_camera_scene_dv->dialog->canvas_id);
    canvas_get_info(g_camera_scene_dv->dialog->canvas_id, &canvas_info);

    g_camera_scene_dv->blending.dialog_section.show = false;
    g_camera_scene_dv->blending.dialog_section.change_show_status = true;
    g_camera_scene_dv->blending.dialog_section.creat.blend_param.color_key = 0;
    g_camera_scene_dv->blending.dialog_section.creat.blend_param.alpha = CAM_DEFAULT_BLENDING_ALPHA;
    g_camera_scene_dv->blending.dialog_section.creat.blend_param.area.x = canvas_info.rect.x;
    g_camera_scene_dv->blending.dialog_section.creat.blend_param.area.y = canvas_info.rect.y;
    g_camera_scene_dv->blending.dialog_section.creat.blend_param.area.w = canvas_info.rect.width;
    g_camera_scene_dv->blending.dialog_section.creat.blend_param.area.h = canvas_info.rect.height;
    g_camera_scene_dv->blending.dialog_section.creat.blend_param.blending_buf = (unsigned char *) canvas_info.pdata;
    g_camera_scene_dv->blending.dialog_section.creat.handle = -1;
    result = camera_dv_ctr_blending_area_create(&g_camera_scene_dv->blending.dialog_section.creat);
    if ((false == result) || (g_camera_scene_dv->blending.dialog_section.creat.handle < 0))
    {
        print_err("send AREA_CREATE err\n\n");
        return false;
    }

    {
        int show_array[MAX_BLENDING_AREA + 1];
        memset(show_array, BLENDING_HANLE_NULL, sizeof(show_array));
        show_array[0] = g_camera_scene_dv->blending.dialog_section.creat.handle;
        result = camera_dv_ctr_blending_area_show(show_array);
    }

    return true;
}

static bool _dv_dialog_blending_delete(void)
{
    /*! 这里所有blending销毁都默认返回true */
    bool result = true;

    if (camera_dv_get_mmm_handle() == NULL)
    {
        return true;
    }

    if (-1 != g_camera_scene_dv->blending.dialog_section.creat.handle)
    {
        result = camera_dv_ctr_blending_area_delete((int) g_camera_scene_dv->blending.dialog_section.creat.handle);
        g_camera_scene_dv->blending.dialog_section.creat.handle = -1;
    }
    //canvas_delete(g_camera_scene_dv->blending.dialog_section.canvas_id);
    return result;
}

bool camera_dv_dialog_create(cam_dialog_e type)
{
    bool result = true;
    dialog_init_t init;

    if (NULL != g_camera_scene_dv->dialog)
    {
        camera_dv_dialog_delete();
    }
    memset(&init, 0x00, sizeof(init));
    result = _dv_get_dialog_init(type, &init);
    if (false == result)
    {
        return false;
    }

    g_camera_scene_dv->dialog = dialog_create(&init, (dialog_cb_t) _dv_dialog_callback);
    if (NULL == g_camera_scene_dv->dialog)
    {
        print_err("dialog_create fail\n\n");
        return false;
    }

    _dv_dialog_blending_create_show();

    dialog_paint(g_camera_scene_dv->dialog);

    return true;
}

bool camera_dv_dialog_delete(void)
{
    _dv_dialog_blending_delete();

    if (NULL != g_camera_scene_dv->dialog)
    {
        dialog_delete(g_camera_scene_dv->dialog);
    }
    g_camera_scene_dv->dialog = NULL;
    s_dialog_str_type = 0;
    return true;
}

#if (USE_MODAL_DIALOG == 1)
int camera_dv_dialog_modal( cam_dialog_e type )
{
    int result = -1;
    dialog_init_t init;

    memset( &init, 0x00, sizeof(init) );
    result = _dv_get_dialog_init( type, &init );
    if ( false == result )
    {
        return -1;
    }

    _dv_dialog_blending_create_show();

    result = dialog_modal( &init );

    _dv_dialog_blending_delete();

    return result;
}
#endif /* USE_MODAL_DIALOG */

#undef B_DIALOG
#define B_VOLBAR
static void _dv_volumebar_callback(volumebar_t *data, int signal)
{
    switch (signal)
    {
        case WIDGET_SIG_VOLUMEBAR_CHANGED:
        print_dbg();
        break;

        case WIDGET_SIG_VOLUMEBAR_QUIT:
        print_dbg();
        _dv_volumebar_blending_delete();
        //如果vbar从内部timer删除是否有问题
        volumebar_delete(data);
        g_camera_scene_dv->vbar = NULL;
        break;

        default:
        break;
    }
}

static bool _dv_volumebar_blending_create_show(void)
{
    bool result = true;
    canvas_info_t canvas_info =
    { 0 };

    canvas_hide(g_camera_scene_dv->vbar->canvas_id);
    canvas_get_info(g_camera_scene_dv->vbar->canvas_id, &canvas_info);

    g_camera_scene_dv->blending.volbar_section.show = false;
    g_camera_scene_dv->blending.volbar_section.change_show_status = true;
    g_camera_scene_dv->blending.volbar_section.creat.blend_param.color_key = 0;
    g_camera_scene_dv->blending.volbar_section.creat.blend_param.alpha = CAM_DEFAULT_BLENDING_ALPHA;
    g_camera_scene_dv->blending.volbar_section.creat.blend_param.area.x = canvas_info.rect.x;
    g_camera_scene_dv->blending.volbar_section.creat.blend_param.area.y = canvas_info.rect.y;
    g_camera_scene_dv->blending.volbar_section.creat.blend_param.area.w = canvas_info.rect.width;
    g_camera_scene_dv->blending.volbar_section.creat.blend_param.area.h = canvas_info.rect.height;
    g_camera_scene_dv->blending.volbar_section.creat.blend_param.blending_buf = (unsigned char *) canvas_info.pdata;
    g_camera_scene_dv->blending.volbar_section.creat.handle = -1;
    result = camera_dv_ctr_blending_area_create(&g_camera_scene_dv->blending.volbar_section.creat);
    if ((false == result) || (g_camera_scene_dv->blending.volbar_section.creat.handle < 0))
    {
        print_err("send AREA_CREATE err\n\n");
        return false;
    }

    {
        int show_array[MAX_BLENDING_AREA + 1];
        memset(show_array, BLENDING_HANLE_NULL, sizeof(show_array));
        show_array[0] = g_camera_scene_dv->blending.volbar_section.creat.handle;
        result = camera_dv_ctr_blending_area_show(show_array);
    }

    return true;
}

static bool _dv_volumebar_blending_delete(void)
{
    bool result = true;

    if (camera_dv_get_mmm_handle() == NULL)
    {
        return true;
    }

    if (-1 != g_camera_scene_dv->blending.volbar_section.creat.handle)
    {
        result = camera_dv_ctr_blending_area_delete((int) g_camera_scene_dv->blending.volbar_section.creat.handle);
        g_camera_scene_dv->blending.volbar_section.creat.handle = -1;
    }

    return result;
}

static bool _dv_volumebar_create(void)
{
    g_camera_scene_dv->vbar = volumebar_create(0, (volumebar_cb_t) _dv_volumebar_callback);
    if (NULL == g_camera_scene_dv->vbar)
    {
        print_err("volumebar_create fail");
        return false;
    }

    _dv_volumebar_blending_create_show();

    volumebar_paint(g_camera_scene_dv->vbar);

    return true;
}

static bool _dv_volumebar_delete(void)
{
    _dv_volumebar_blending_delete();
    if (g_camera_scene_dv->vbar != NULL)
    {
        volumebar_delete(g_camera_scene_dv->vbar);
    }
    g_camera_scene_dv->vbar = NULL;

    return true;
}

#undef B_VOLBAR
#define B_OPTION
/*! option和adjust都只在option.c中用到，为了统一都放在这里 */
static void _dv_widget_option_callback(menuview_t *menuview, menuview_sig_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_QUIT:
        camera_dv_widget_option_delete(g_camera_scene_dv->opt);
        g_camera_scene_dv->opt = NULL;
        break;

        default:
        break;
    }
}

bool camera_dv_option_blending_create_show(void)
{
    bool result;
    canvas_info_t canvas_info =
    { 0 };

    canvas_hide(g_camera_scene_dv->opt->canvas_id);
    canvas_get_info(g_camera_scene_dv->opt->canvas_id, &canvas_info);
    g_camera_scene_dv->blending.menu_section.show = false;
    g_camera_scene_dv->blending.menu_section.change_show_status = true;
    g_camera_scene_dv->blending.menu_section.creat.blend_param.color_key = 0;
    g_camera_scene_dv->blending.menu_section.creat.blend_param.alpha = CAM_DEFAULT_BLENDING_ALPHA;
    g_camera_scene_dv->blending.menu_section.creat.blend_param.area.x = canvas_info.rect.x;
    g_camera_scene_dv->blending.menu_section.creat.blend_param.area.y = canvas_info.rect.y;
    g_camera_scene_dv->blending.menu_section.creat.blend_param.area.w = canvas_info.rect.width;
    g_camera_scene_dv->blending.menu_section.creat.blend_param.area.h = canvas_info.rect.height;
    g_camera_scene_dv->blending.menu_section.creat.blend_param.blending_buf = (unsigned char *) canvas_info.pdata;
    g_camera_scene_dv->blending.menu_section.creat.handle = -1;
    assert(canvas_info.rect.x %8 == 0);
    assert(canvas_info.rect.y %8 == 0);
    assert(canvas_info.rect.width %8 == 0);
    assert(canvas_info.rect.height %8 == 0);
    result = camera_dv_ctr_blending_area_create(&g_camera_scene_dv->blending.menu_section.creat);
    if ((false == result) || (g_camera_scene_dv->blending.menu_section.creat.handle < 0))
    {
        print_err("send AREA_CREATE err\n\n");
        return false;
    }

    {
        int show_array[MAX_BLENDING_AREA + 1];
        memset(show_array, BLENDING_HANLE_NULL, sizeof(show_array));
        show_array[0] = g_camera_scene_dv->blending.menu_section.creat.handle;
        result = camera_dv_ctr_blending_area_show(show_array);
    }

    return true;
}

/*call by Camera_dv_option.c Camera_dv_widget_option.c */
bool camera_dv_option_blending_delete(void)
{
    bool result = true;

    if (camera_dv_get_mmm_handle() == NULL)
    {
        return true;
    }

    if (-1 != g_camera_scene_dv->blending.menu_section.creat.handle)
    {
        result = camera_dv_ctr_blending_area_delete((int) g_camera_scene_dv->blending.menu_section.creat.handle);
        g_camera_scene_dv->blending.menu_section.creat.handle =-1;
    }

    return result;
}

static bool _dv_option_delete(void)
{
    if (g_camera_scene_dv->opt != NULL)
    {
        /*! 删除menu的数据 */
        camera_dv_widget_option_deinit();
        /*! 删除menu的视图 */
        camera_dv_widget_option_delete(g_camera_scene_dv->opt);
        g_camera_scene_dv->opt = NULL;
    }
    return true;
}

#undef  B_OPTION
#define B_ADJUST
/*call by Camera_dv_option.c*/
bool camera_dv_adjust_blending_create_show(void)
{
    bool result;
    canvas_info_t canvas_info =
    { 0 };

    canvas_hide(g_camera_scene_dv->adjust->canvas_id);
    canvas_get_info(g_camera_scene_dv->adjust->canvas_id, &canvas_info);

    g_camera_scene_dv->blending.adjust_section.show = false;
    g_camera_scene_dv->blending.adjust_section.change_show_status = true;
    g_camera_scene_dv->blending.adjust_section.creat.blend_param.color_key = 0;
    g_camera_scene_dv->blending.adjust_section.creat.blend_param.alpha = CAM_DEFAULT_BLENDING_ALPHA;
    g_camera_scene_dv->blending.adjust_section.creat.blend_param.area.x = canvas_info.rect.x;
    g_camera_scene_dv->blending.adjust_section.creat.blend_param.area.y = canvas_info.rect.y;
    g_camera_scene_dv->blending.adjust_section.creat.blend_param.area.w = canvas_info.rect.width;
    g_camera_scene_dv->blending.adjust_section.creat.blend_param.area.h = canvas_info.rect.height;
    g_camera_scene_dv->blending.adjust_section.creat.blend_param.blending_buf = (unsigned char *) canvas_info.pdata;
    g_camera_scene_dv->blending.adjust_section.creat.handle = -1;
    assert(canvas_info.rect.x %8 == 0);
    assert(canvas_info.rect.y %8 == 0);
    assert(canvas_info.rect.width %8 == 0);
    assert(canvas_info.rect.height %8 == 0);
    result = camera_dv_ctr_blending_area_create(&g_camera_scene_dv->blending.adjust_section.creat);
    if ((false == result) || (g_camera_scene_dv->blending.adjust_section.creat.handle < 0))
    {
        print_err("send AREA_CREATE err\n\n");
        return false;
    }

    {
        int show_array[MAX_BLENDING_AREA + 1];
        memset(show_array, BLENDING_HANLE_NULL, sizeof(show_array));
        show_array[0] = g_camera_scene_dv->blending.adjust_section.creat.handle;
        result = camera_dv_ctr_blending_area_show(show_array);
    }

    return true;
}
/*call by Camera_dv_option.c*/
bool camera_dv_adjust_blending_delete(void)
{
    bool result = true;

    if (camera_dv_get_mmm_handle() == NULL)
    {
        return true;
    }

    if (-1 != g_camera_scene_dv->blending.adjust_section.creat.handle)
    {
        result = camera_dv_ctr_blending_area_delete((int) g_camera_scene_dv->blending.adjust_section.creat.handle);
        g_camera_scene_dv->blending.adjust_section.creat.handle = -1;
    }

    return result;
}

static bool _dv_adjust_delete(void)
{
    camera_dv_adjust_blending_delete();
    if (g_camera_scene_dv->adjust != NULL)
    {
        adjust_delete(g_camera_scene_dv->adjust);
        g_camera_scene_dv->adjust = NULL;
    }

    return true;
}

/*!==========blending处理=========== */
#undef B_ADJUST
#define B_MSGBOX
static void _dv_msgbox_callback(msgbox_t *msgbox, msgbox_sig_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_MSGBOX_SHOW:
        _dv_msgbox_blending_create_show();
        break;

        case WIDGET_SIG_MSGBOX_QUIT:
        _dv_msgbox_blending_delete();
        break;

        case WIDGET_SIG_MSGBOX_QUIT_LOW_POWER:
        {
            mmm_dv_status_t dv_status= {0};

            if(s_do_stoping == true)
            {
                break;
            }
            camera_dv_get_record_status(&dv_status);
            if (dv_status.status != MMM_RECORDER_STOPPED)
            {
                s_dialog_recstopped = CAM_DIALOG_TAG_ERR;                
                _dv_change_record_stop();
            }
        }
        default:
        break;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *       创建commonui的blending区域.
 *       msgbox创建之后不需要立即显示, 收到show消息后需要, 所以需要:create and show
 *       commonui的canvas属性为show状态, 否则在暂停时无法刷新
 * \param[in]
 * \retval       bool: true:创建成功; false:创建失败
 * 注意: 非playing界面, 此函数直接返回, 不创建操作blending
 *******************************************************************************/
static bool _dv_msgbox_blending_create_show(void)
{
    bool result;
    canvas_info_t canvas_info =
    { 0 };

    canvas_hide(g_camera_scene_dv->msgbox->canvas_id);
    canvas_get_info(g_camera_scene_dv->msgbox->canvas_id, &canvas_info);

    g_camera_scene_dv->blending.msgbox_section.show = false;
    g_camera_scene_dv->blending.msgbox_section.change_show_status = true;
    g_camera_scene_dv->blending.msgbox_section.creat.blend_param.color_key = 0;
    g_camera_scene_dv->blending.msgbox_section.creat.blend_param.alpha = CAM_DEFAULT_BLENDING_ALPHA;
    g_camera_scene_dv->blending.msgbox_section.creat.blend_param.area.x = canvas_info.rect.x;
    g_camera_scene_dv->blending.msgbox_section.creat.blend_param.area.y = canvas_info.rect.y;
    g_camera_scene_dv->blending.msgbox_section.creat.blend_param.area.w = canvas_info.rect.width;
    g_camera_scene_dv->blending.msgbox_section.creat.blend_param.area.h = canvas_info.rect.height;
    g_camera_scene_dv->blending.msgbox_section.creat.blend_param.blending_buf = (unsigned char *) canvas_info.pdata;
    g_camera_scene_dv->blending.msgbox_section.creat.handle = -1;
    assert(canvas_info.rect.x %8 == 0);
    assert(canvas_info.rect.y %8 == 0);
    assert(canvas_info.rect.width %8 == 0);
    assert(canvas_info.rect.height %8 == 0);
    result = camera_dv_ctr_blending_area_create(&g_camera_scene_dv->blending.msgbox_section.creat);
    if ((false == result) || (g_camera_scene_dv->blending.msgbox_section.creat.handle < 0))
    {
        print_err("send AREA_CREATE err\n\n");
        return false;
    }

    {
        int show_array[MAX_BLENDING_AREA + 1];
        memset(show_array, BLENDING_HANLE_NULL, sizeof(show_array));
        show_array[0] = g_camera_scene_dv->blending.msgbox_section.creat.handle;
        result = camera_dv_ctr_blending_area_show(show_array);
    }

    return true;
}

static bool _dv_msgbox_blending_delete(void)
{
    bool result = true;

    if (camera_dv_get_mmm_handle() == NULL)
    {
        return true;
    }

    if (-1 != g_camera_scene_dv->blending.msgbox_section.creat.handle)
    {
        result = camera_dv_ctr_blending_area_delete((int) g_camera_scene_dv->blending.msgbox_section.creat.handle);
        g_camera_scene_dv->blending.msgbox_section.creat.handle = -1;
    }

    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *       创建 digit msgbox, 完成变速调节
 *       根据当前scene自动完成横竖风格的创建
 *       根据当前是否是playing界面, 自动完成blending的操作
 * \retval       bool: true:创建成功; false:创建失败
 *******************************************************************************/
bool camera_dv_msgbox_create(void)
{
    g_camera_scene_dv->msgbox = msgbox_create(_dv_msgbox_callback);
    if (NULL == g_camera_scene_dv->msgbox)
    {
        print_err("msgbox_create fail");
        return false;
    }

    return true;
}

bool camera_dv_msgbox_delete(void)
{
    if(    g_camera_scene_dv->msgbox != NULL)
    {
        msgbox_delete(g_camera_scene_dv->msgbox);
        g_camera_scene_dv->msgbox = NULL;
        return true;
    }
    else
    {
        return false;
    }
}

#undef B_MSGBOX

void camera_dv_warning_nospace(void)
{
    camera_dv_dialog_create(CAM_DIALOG_NOSPACE);
    print_info("no enough space\n ");
}
void camera_dv_kill_timer(camera_scene_dv_t *data)
{
    if (data->dv_recordtimer_id != 0)
    {
        kill_timer(data->dv_recordtimer_id);
        data->dv_recordtimer_id = 0;
        dv_record_ing = 0;
    }

}

bool camera_dv_ui_init(camera_dv_ui_t *dv_ui)
{
    dv_ui->blending_area_left_bg.saved_show = -1;
    dv_ui->blending_area_left_bg.show = 1;

    dv_ui->blending_area_botton_bg.saved_show = -1;
    dv_ui->blending_area_botton_bg.show = 1;

    dv_ui->dv_icon.saved_show = -1;
    dv_ui->dv_icon.show = 1;
    dv_ui->dv_icon.saved_frame = -1;
    dv_ui->dv_icon.frame = 0;

    dv_ui->brightness.saved_show = -1;
    dv_ui->brightness.show = 1;
    dv_ui->brightness.saved_frame = -1;
    dv_ui->brightness.frame = (char)(g_camera_dv_cfgs[CAM_DV_CFG_TYPE_BRIGHTNESS].value + 3);

    dv_ui->special_effect.saved_show = -1;
    dv_ui->special_effect.show = 1;
    dv_ui->special_effect.saved_frame = -1;
    dv_ui->special_effect.frame = (char)g_camera_dv_cfgs[CAM_DV_CFG_TYPE_SPECIAL_EFFECT].value;

    dv_ui->white_balance.saved_show = -1;
    dv_ui->white_balance.show = 1;
    dv_ui->white_balance.saved_frame = -1;
    dv_ui->white_balance.frame = (char)g_camera_dv_cfgs[CAM_DV_CFG_TYPE_WHITE_BALANCE].value;

    dv_ui->record_quality.saved_show = -1;
    dv_ui->record_quality.show = 1;
    dv_ui->record_quality.saved_frame = -1;
    dv_ui->record_quality.frame = (char)g_camera_dv_cfgs[CAM_DV_CFG_TYPE_RECORD_QUALITY].value;

    dv_ui->exposure_mode.saved_show = -1;
    dv_ui->exposure_mode.show = 1;
    dv_ui->exposure_mode.saved_frame = -1;
    dv_ui->exposure_mode.frame = (char)g_camera_dv_cfgs[CAM_DV_CFG_TYPE_EXPOSURE_MODE].value;

    dv_ui->store_type.saved_show = -1;
    dv_ui->store_type.show = 1;
    dv_ui->store_type.saved_frame = -1;
    dv_ui->store_type.frame = (char)g_camera_dv_cfgs[CAM_DV_CFG_TYPE_SAVE_PATH].value;
    if(dv_ui->store_type.frame > 0)
    {
        dv_ui->store_type.frame -= 1;
    }
    dv_ui->rec_status.saved_show = -1;
    dv_ui->rec_status.show = 1;


    dv_ui->rec_status.saved_frame = -1;
    dv_ui->rec_status.frame = 2;

    dv_ui->use_free_number.saved_show = -1;
    dv_ui->use_free_number.show = 1;
    dv_ui->use_free_number.saved_buf = (char *) s_use_free_number_saved_buf;
    dv_ui->use_free_number.buf = (char *) g_use_free_number_buf;

    dv_ui->blending_area_focus_bg.saved_show = -1;
    dv_ui->blending_area_focus_bg.show = 1;

    dv_ui->focus.saved_show = -1;
    dv_ui->focus.show = 1;

    memset(dv_ui->use_free_number.saved_buf, 0, sizeof(dv_ui->use_free_number.saved_buf));
    /*所有这里没有设的和sensor相关的会在preview中获取，这里将在preview之后才调用*/

    return true;

}
void camera_scene_dv_paint(camera_scene_dv_t *data)
{
    int hdc = gui_dc_get(data->win_id);
    camera_dv_ui_t *p = &(data->dv_ui_icon);
    int canvas_id;
    static int s_pre_savepath = -1;
    //static int s_pre_pathstatus = -1;

    print_dbg("%s----in\n", __FUNCTION__);

    hdc = gui_dc_get(g_camera_scene_dv->blending.left_section.canvas_id);
    canvas_id = g_camera_scene_dv->blending.left_section.canvas_id;
    camera_dv_ui_draw_picregion(hdc, &(p->dv_icon), canvas_id);
    camera_dv_ui_draw_picregion(hdc, &(p->brightness), canvas_id);
    camera_dv_ui_draw_picregion(hdc, &(p->special_effect), canvas_id);
    camera_dv_ui_draw_picregion(hdc, &(p->white_balance), canvas_id);
    camera_dv_ui_draw_picregion(hdc, &(p->record_quality), canvas_id);
    camera_dv_ui_draw_picregion(hdc, &(p->exposure_mode), canvas_id);

    if ((g_camera_dv_cfgs[CAM_DV_CFG_TYPE_SAVE_PATH].value != 0) && \
        (g_setpath != false))
    {

        camera_dv_ui_draw_picregion(hdc, &(p->store_type), canvas_id);
    }
    else
    {
        // TODO:recheck here ,will re clear
        print_err();
/*        if((pre_savepath != g_camera_dv_cfgs[CAM_DV_CFG_TYPE_SAVE_PATH].value) && \
            (g_setpath != false))
*/
        /*if((s_pre_savepath != g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value) || \
            (s_pre_pathstatus != (int)g_setpath))*/

        {
            sty_rect_t srect;
            //data->dv_ui_icon.store_type.picregion_res->x;
            srect.x = p->store_type.picregion_res->x;
            srect.y = p->store_type.picregion_res->y;
            srect.width = p->store_type.picregion_res->width;
            srect.height = p->store_type.picregion_res->height;
            canvas_erase(canvas_id, &srect);
            print_err();
        }
    }

    hdc = gui_dc_get(g_camera_scene_dv->blending.bottom_section.canvas_id);
    canvas_id = g_camera_scene_dv->blending.bottom_section.canvas_id;

    if ((g_camera_dv_cfgs[CAM_DV_CFG_TYPE_SAVE_PATH].value != 0) && \
        (g_setpath != false))
    {
        #if (DV_ICON_STATUS2SDRAM == 1)
        camera_dv_ui_draw_sdram_status(hdc, &(p->rec_status), canvas_id);
        #else
        camera_dv_ui_draw_picregion_status(hdc, &(p->rec_status), canvas_id);
        #endif
        camera_dv_ui_draw_string(hdc, &(p->use_free_number), canvas_id);
    }
    else
    {
        // TODO:recheck here ,will re clear
        print_err();
        //if(s_pre_savepath != g_camera_dv_cfgs[CAM_DV_CFG_TYPE_SAVE_PATH].value)
        {
            //camera_dv_ui_draw_pic(hdc, &(p->blending_area_botton_bg));
            canvas_erase_all(canvas_id);//, &srect);
            print_err();
        }
    }
    s_pre_savepath = g_camera_dv_cfgs[CAM_DV_CFG_TYPE_SAVE_PATH].value;


    hdc = gui_dc_get(g_camera_scene_dv->blending.focus_section.canvas_id);
    canvas_id = g_camera_scene_dv->blending.focus_section.canvas_id;

    camera_dv_ui_draw_pic(hdc, &(p->focus));
}

/* scene quit */
static int _dv_quit(void)
{
    exit_msg_loop();
    return 0;
}

/*
 * 创建各blending区域, 但都处于hide状态.
 * 需要显示时, 再将其设置为show状态
 */
static bool _dv_init_blendig(resgroup_resource_t* blending_area, camera_blending_section_t *section)
{
    bool result = true;
    canvas_info_t canvas_info = { 0 };
    sty_rect_t srect = { 0 };

    srect.x = blending_area->x;
    srect.y = blending_area->y;
    srect.width = blending_area->width;
    srect.height = blending_area->height;
    section->canvas_id = canvas_create( &srect, TRANSPARENT);
    if ( -1 == section->canvas_id )
    {
        print_err("canvas_create fail\n\n");
        return false;
    }
    else
    {
        print_info("canvas_create ok\n\n");
    }
    canvas_hide( section->canvas_id );
    canvas_get_info( section->canvas_id, &canvas_info );
    section->show = false;
    section->change_show_status = true;
    section->creat.blend_param.color_key = (0);
    section->creat.blend_param.alpha     = CAM_DEFAULT_BLENDING_ALPHA;
    section->creat.blend_param.area.x    = canvas_info.rect.x;
    section->creat.blend_param.area.y    = canvas_info.rect.y;
    section->creat.blend_param.area.w    = canvas_info.rect.width;
    section->creat.blend_param.area.h    = canvas_info.rect.height;
    section->creat.blend_param.blending_buf = (unsigned char *)canvas_info.pdata;
    section->creat.handle = -1;
    assert(canvas_info.rect.x %8 == 0);
    assert(canvas_info.rect.y %8 == 0);
    assert(canvas_info.rect.width %8 == 0);
    assert(canvas_info.rect.height %8 == 0);
    result = camera_dv_ctr_blending_area_create( (mmm_enc_blending_create_t *)(&section->creat));

    return result;
}

bool camera_dv_blending_init(void)
{
    bool result = true;
    resgroup_resource_t* blending_area;
    camera_blending_section_t* section;

    blending_area = g_camera_scene_dv->dv_ui_icon.blending_area_left;
    section = &(g_camera_scene_dv->blending.left_section);
    result = _dv_init_blendig(blending_area, section);

    blending_area = g_camera_scene_dv->dv_ui_icon.blending_area_botton;
    section = &(g_camera_scene_dv->blending.bottom_section);
    result = _dv_init_blendig(blending_area, section);

    blending_area = g_camera_scene_dv->dv_ui_icon.blending_area_focus;
    section = &(g_camera_scene_dv->blending.focus_section);
    result = _dv_init_blendig(blending_area, section);

    return result;
}

/*
 * 删除各blending区域, 释放资源
 */
bool camera_dv_blending_deinit(void)
{
    bool result = true;

    if ( -1 != g_camera_scene_dv->blending.left_section.creat.handle)
    {
        result = camera_dv_ctr_blending_area_delete((int) g_camera_scene_dv->blending.left_section.creat.handle);
        g_camera_scene_dv->blending.left_section.creat.handle = -1;
    }
    if(-1 != g_camera_scene_dv->blending.left_section.canvas_id)
    {
        canvas_delete(g_camera_scene_dv->blending.left_section.canvas_id);
        g_camera_scene_dv->blending.left_section.canvas_id = -1;
    }

    if ( -1 != g_camera_scene_dv->blending.bottom_section.creat.handle)
    {
        result = camera_dv_ctr_blending_area_delete((int) g_camera_scene_dv->blending.bottom_section.creat.handle);
        g_camera_scene_dv->blending.bottom_section.creat.handle = -1;
    }
    if (-1 != g_camera_scene_dv->blending.bottom_section.canvas_id)
    {
        canvas_delete(g_camera_scene_dv->blending.bottom_section.canvas_id);
        g_camera_scene_dv->blending.bottom_section.canvas_id = -1;
    }

    if ( -1 != g_camera_scene_dv->blending.focus_section.creat.handle)
    {
        result = camera_dv_ctr_blending_area_delete((int) g_camera_scene_dv->blending.focus_section.creat.handle);
        g_camera_scene_dv->blending.focus_section.creat.handle = -1;
    }
    if(-1 != g_camera_scene_dv->blending.focus_section.canvas_id)
    {
        canvas_delete(g_camera_scene_dv->blending.focus_section.canvas_id);
        g_camera_scene_dv->blending.focus_section.canvas_id = -1;
    }

    /*可能创建的blending区域统一删除*/
    camera_dv_dialog_delete();
    _dv_option_delete();
    _dv_adjust_delete();
    _dv_volumebar_delete();
    _dv_msgbox_blending_delete();

    return result;
}

static bool _dv_ui_blending(void)
{
    bool result;
    int show_array[MAX_BLENDING_AREA + 1];
    int hide_array[MAX_BLENDING_AREA + 1];
    int show = -1;
    int hide = -1;
    int i = 0;
    camera_blending_section_t* p[] =
    {
        &g_camera_scene_dv->blending.left_section, &g_camera_scene_dv->blending.bottom_section,
        &g_camera_scene_dv->blending.focus_section,
    };
    memset(show_array, BLENDING_HANLE_NULL, sizeof(show_array));
    memset(hide_array, BLENDING_HANLE_NULL, sizeof(hide_array));
    for (i = 0; i < 3; i++)
    {
        if (true == p[i]->change_show_status)
        {
            p[i]->change_show_status = false;
            if (true == p[i]->show)
            {
                ++show;
                show_array[show] = p[i]->creat.handle;
            }
            else
            {
                ++hide;
                hide_array[hide] = p[i]->creat.handle;
            }
        }
    }
    if (hide > -1)//注意顺序先隐藏再显示
    {
        result = camera_dv_ctr_blending_area_hide(hide_array);
        if (false == result)
        {
            print_err("send hide err\n\n");
            return false;
        }
    }

    if (show > -1)
    {
        result = camera_dv_ctr_blending_area_show(show_array);
        if (false == result)
        {
            print_err("send show err\n\n");
            return false;
        }
    }
    return true;
}

#if (DV_ICON_STATUS2SDRAM == 1)
/*
bmp = sty_create_bitmap( BG );	//载入RES图片资源到bitmap
result = sty_draw_bitmap( hdc, bmp, 0, 0 );   //绘制bitmap
result =  sty_delete_bitmap( bmp );   //删除bitmap
*/
static bool _dv_ui_load_icon2sdram(void)
{
    int i;
    for(i = 0; i < 3; i++)
    {
        sdram_buf_icon_info[i] = sty_create_bitmap((int)(sdram_buf_icon_frame[i].pic_id));
    }
    return true;
}
static bool _dv_ui_unload_icon2sdram(void)
{
    bool result;
    int i;
    for(i = 0; i < 3; i++)
    {
        result = sty_delete_bitmap(sdram_buf_icon_info[i]);
    }
    return true;
}
#endif /* DV_ICON_STATUS2SDRAM */

static bool _dv_ui_load_style(camera_scene_dv_t *data)
{
    scene_t* scene;
    resgroup_resource_t *group;

    // scene = sty_get_app_scene();
    scene = data->scene;
    if (scene == NULL)
    {
        print_err("cann't find app scene!");
        return false;
    }

    /*! 左边区域 */
    group = (resgroup_resource_t *) get_scene_child(scene, DV_GROUP_BLENDING_AREA_LEFT);
    data->dv_ui_icon.blending_area_left = group;

    data->dv_ui_icon.blending_area_left_bg.pic_res = (picture_resource_t *) get_resgroup_child(group,
            DV_PIC_BLENDING_LEFT);
    data->dv_ui_icon.dv_icon.picregion_res = (picregion_resource_t *) get_resgroup_child(group, DV_REG_DC);
    data->dv_ui_icon.brightness.picregion_res = (picregion_resource_t *) get_resgroup_child(group, DV_REG_BRIGHTNESS);
    data->dv_ui_icon.special_effect.picregion_res = (picregion_resource_t *) get_resgroup_child(group,
            DV_REG_SPECIAL_EFFECT);
    data->dv_ui_icon.white_balance.picregion_res = (picregion_resource_t *) get_resgroup_child(group,
            DV_REG_WHITE_BALANCE);
    data->dv_ui_icon.record_quality.picregion_res = (picregion_resource_t *) get_resgroup_child(group,
            DV_REG_RECORD_QUALITY);
    data->dv_ui_icon.exposure_mode.picregion_res = (picregion_resource_t *) get_resgroup_child(group,
            DV_REG_EXPOSURE_MODE);
    data->dv_ui_icon.store_type.picregion_res = (picregion_resource_t *) get_resgroup_child(group, DV_REG_STORE_TYPE);

    /*! 底部区域 */
    group = (resgroup_resource_t *) get_scene_child(scene, DV_GROUP_BLENDING_AREA_BOTTON);
    data->dv_ui_icon.blending_area_botton = group;
    data->dv_ui_icon.blending_area_botton_bg.pic_res = (picture_resource_t *) get_resgroup_child(group,
            DV_PIC_BLENDING_BOTTON);
    data->dv_ui_icon.rec_status.picregion_res = (picregion_resource_t *) get_resgroup_child(group, DV_REG_REC_STATUS);
    #if (DV_ICON_STATUS2SDRAM == 1)
    {
        int i = 0;
        for (i = 0; i < 3; i++)
        {
            get_picregion_frame(data->dv_ui_icon.rec_status.picregion_res,\
             (const unsigned short)i, &sdram_buf_icon_frame[i]);
        }
    }
    #endif /* DV_ICON_STATUS2SDRAM */
    data->dv_ui_icon.use_free_number.str_res = (string_resource_t *) get_resgroup_child(group, DV_STR_USE_FREE_NUMBER);

    /*! 焦点区域 */
    group = (resgroup_resource_t *) get_scene_child(scene, DV_GROUP_BLENDING_AREA_FOCUS);
    data->dv_ui_icon.blending_area_focus = group;
    data->dv_ui_icon.blending_area_focus_bg.pic_res = (picture_resource_t *) get_resgroup_child(group,
            DV_PIC_BLENDING_FOCUS);
    data->dv_ui_icon.focus.pic_res = (picture_resource_t *) get_resgroup_child(group, DV_PIC_FOCUS);

    return true;
}

static bool _dv_open_theme(void)
{
    rotation_e angle;
    char common_file[PATH_MAX];
    char app_file[PATH_MAX];

    /* 取视图角度*/
    angle = sty_get_view_rotation();
    if (angle == ROTATION_90)
    {
        angle = ROTATION_0;
    }

    if (angle == ROTATION_270)
    {
        angle = ROTATION_180;
    }

    /* 设置屏幕方向 */
    sty_set_screen_rotation(angle);

    /* 载入commonui的 style 场景 */
    g_camera_scene_dv->common_scene = load_scene(g_app_camera->common_style, COMMONUI_SIDELONG_SCENE);

    if (g_camera_scene_dv->common_scene == NULL)
    {
        print_err("load commonui scene failed");
        return false;
    }
    else
    {
        /* 设置 commonui 的 style 场景*/
        sty_set_common_scene(g_camera_scene_dv->common_scene);
    }

    /* 载入应用的 style 场景*/
    g_camera_scene_dv->scene = load_scene(g_app_camera->app_style, CAMERA_DV_SCENE);
    if (g_camera_scene_dv->scene == NULL)
    {
        print_err("load scene CAMERA_DV_SCENE failed");
        return false;
    }
    else
    {
        /* 设置应用的 style 场景 */
        sty_set_app_scene(g_camera_scene_dv->scene);
    }

    /* 获取commonui 的图片资源文件名 */
    snprintf(common_file, sizeof(common_file), "%s/%s.res", sys_get_common_theme_res_dir(), "commonui");

    /* 打开 commonui 图片资源文件 */
    if (sty_open_common_pic_res(common_file) == false)
    {
        print_err("open %s failed!", common_file);
        return false;
    }

    /* 获取commonui 的图片资源文件名*/
    snprintf(app_file, sizeof(app_file), "%s/%s.res", sys_get_app_theme_res_dir(), "camera");

    /* 打开应用的图片资源文件*/
    if (sty_open_app_pic_res(app_file) == false)
    {
        print_err("open %s failed!", app_file);
        return false;
    }

    return true;

}

static bool _dv_close_theme(void)
{
    if(g_camera_scene_dv->common_scene != NULL)
    {
        free_scene(g_camera_scene_dv->common_scene);
        g_camera_scene_dv->common_scene = NULL;
    }

    if(g_camera_scene_dv->scene != NULL)
    {
        free_scene(g_camera_scene_dv->scene);
        g_camera_scene_dv->scene = NULL;
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

static bool _dv_open_str_res(void)
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

static bool _dv_close_str_res(void)
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

static bool _dv_set_default_font(void)
{
    /* crate font */
    if (g_camera_scene_dv->fontdes > 0)
    {
        return false;
    }

    g_camera_scene_dv->fontdes = gui_create_font((unsigned char*)sys_get_default_font_file(), 16);
    if (g_camera_scene_dv->fontdes < 0)
    {
        print_err("call gui_create_font failed\n");
        return false;
    }

    if (gui_dc_set_default_fontface(g_camera_scene_dv->fontdes) < 0)
    {
        print_err("gui_dc_set_default_fontface failed\n");
        return false;
    }

    return true;
}

static bool _dv_unset_default_font(void)
{
    /* crate font */
    if (g_camera_scene_dv->fontdes <= 0)
    {
        return false;
    }

    gui_destroy_font(g_camera_scene_dv->fontdes);
    g_camera_scene_dv->fontdes = 0;

    return true;
}

/* scene initial function */
static bool _dv_init(void)
{
    bool result = false;
    camera_scene_dv_t *data;
    cam_dialog_e dialog_str_type_temp = CAM_DIALOG_OPEN_FAIL ;
    int ret = 0;

    s_dv_quiting = false;
    
    canvas_set_blending_mode(true);
    /*! 分配场景数据空间 */
    if (g_camera_scene_dv == NULL)
    {
        g_camera_scene_dv = malloc(sizeof(camera_scene_dv_t));

        if (g_camera_scene_dv == NULL)
        {
            print_err();
            return false;
        }
        else
        {
            memset(g_camera_scene_dv, 0, sizeof(camera_scene_dv_t));
            g_camera_scene_dv->blending.left_section.creat.handle = -1;
            g_camera_scene_dv->blending.bottom_section.creat.handle = -1;
            g_camera_scene_dv->blending.focus_section.creat.handle = -1;
            g_camera_scene_dv->blending.menu_section.creat.handle = -1;
            g_camera_scene_dv->blending.volbar_section.creat.handle = -1;
            g_camera_scene_dv->blending.dialog_section.creat.handle = -1;
            g_camera_scene_dv->blending.adjust_section.creat.handle = -1;
            g_camera_scene_dv->blending.msgbox_section.creat.handle = -1;
            data = g_camera_scene_dv;
        }
    }
    else
    {
        data = g_camera_scene_dv;
    }

    data->need_exit = FALSE;
    data->exit_type = CAM_SCENE_EXIT;
    if (_dv_open_theme() == false)
    {
        print_err("_dv_open_theme failed\n");
        goto open_theme_err;
    }

    if (_dv_open_str_res() == false)
    {
        print_err("_dv_open_str_res failed\n");
        goto open_str_err;
    }

    if (_dv_set_default_font() == false)
    {
        print_err("call _dv_set_default_font failed\n");
        goto create_font_err;
    }

    /* 获取背景图资源 */
    //data->dv_bg = (picture_resource_t *) get_scene_child(data->scene, DV_PIC_BG );

    /* 设置GUI宽高 */
    gui_set_dimension(data->scene->width, data->scene->height);
    if (_dv_ui_load_style(data) == false)
    {
        print_err("_dv_ui_load_style failed\n");
        goto load_resource_err;
    }
    #if (DV_ICON_STATUS2SDRAM == 1)
    if (_dv_ui_load_icon2sdram() == false)
    {
        print_err("_dv_ui_load_icon2sdram failed\n");
        goto load_icon2sdram_err;
    }
    #endif /* DV_ICON_STATUS2SDRAM */
    data->win_id = gui_wm_create_window(data->scene->x, data->scene->y, data->scene->width, data->scene->height,
            WM_CREATE_SHOW, _dv_callback, 0);
    if (data->win_id == 0)
    {
        print_err("call gui_wm_create_window failed");
        goto create_window_err;
    }

    data->hdc = gui_dc_get(data->win_id);
    ///*目前gui做法，对0层不能有任何操作,否则死机*/gui_dc_clear(data->hdc);

    gui_wm_set_focus(data->win_id);

    if(_dv_is_tvout_on() == true)
    {
        print_err("TV OUT is on,can't into CAMERA!\n");
        dialog_str_type_temp = CAM_DIALOG_TVOUT_OPENED;
        goto camera_open_err;
    }

    /*! 1,取配置 */
    if (camera_dv_init_all_cfg() == false)
    {
        print_err("camera_dv_cfg_read_all failed\n");
        goto read_cfg_err;
    }


#if (DBG_REMNANT_MEMORY == 1)
{
    unsigned int u;
    u = os_mem_query ();
    printf("-----into camera----os_mem_query_remnant=%x\n",u);
    print_mem();
    printf("-----show all module mem use info--(begin)---\n\n",u);
    dump_all_module_mem_info();
    printf("-----show all module mem use info---(end)--\n\n",u);
}
#endif /* DBG_REMNANT_MEMORY */



    /*! 3,设置参数 */
    ret = camera_dv_mmm_open();
    if (ret < 0)
    {
        print_err("err ret=(%d)\n", ret);
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
        print_err("camera_dv_mmm_open\n");
        goto camera_open_err;
    }
    //init config
    if (camera_dv_msgbox_create() == false)
    {
        print_err("camera_dv_msgbox_create failed\n");
        goto create_msgbox_err;
    }
    /*! 4,设置全局参数 */
    {
        g_camera_scene_dv->dv_config.cfg_zoom_level.denominator = 10;
        g_camera_scene_dv->dv_config.cfg_zoom_level.numerator = 10;
    }

    if(g_camera_dv_cfgs[CAM_DV_CFG_TYPE_SAVE_PATH].value == CAM_DV_STORAGE_NONE)
    {
        hotplugin_dev_type_e main_disk = sys_get_main_disk();
        if(main_disk != HOTPLUGIN_DEV_TYPE_NONE)
        {
            g_camera_dv_cfgs[CAM_DV_CFG_TYPE_SAVE_PATH].value = (int)main_disk;
            //result = camera_dv_check_path(main_disk);
        }
        else
        {
            camera_dv_dialog_create(CAM_DIALOG_NODISK);
        }
    }
    else
    {
        //result = camera_dv_check_path((hotplugin_dev_type_e)g_camera_dv_cfgs[CAM_DV_CFG_TYPE_SAVE_PATH].value);
    }


    /*! 5,开始预览 */
    if (camera_dv_ctr_start_preview() == false)
    {
        print_err("camera_dv_ctr_start_preview failed\n");
        goto start_preview_err;
    }
    //paint all
    /*! 6,初始化场景有那个blending（common单独处理） */
    result = camera_dv_blending_init();
    if (result == false)
    {
        print_err("camera_dv_blending_init err\n");
        goto create_blending_err;
    }
    camera_dv_ui_init(&data->dv_ui_icon);
    g_camera_scene_dv->blending.left_section.change_show_status = true;
    g_camera_scene_dv->blending.left_section.show = true;
    g_camera_scene_dv->blending.bottom_section.change_show_status = true;
    g_camera_scene_dv->blending.bottom_section.show = true;
    g_camera_scene_dv->blending.focus_section.change_show_status = true;
    g_camera_scene_dv->blending.focus_section.show = true;
    camera_scene_dv_paint(data);

    if (_dv_ui_blending() == false)
    {
        print_err("_dv_ui_blending failed\n");
        goto show_blending_err;
    }

    if (g_option_flag == true)
    {
        g_option_flag = false;
        if (data->opt == NULL)
        {
            print_dbg();
            s_shortcut_playback = false;
            camera_dv_widget_option_create(_dv_widget_option_callback);
            if (data->opt == NULL)
            {
                print_err();
                //return false;
            }
        }
    }

    clear_key_msg_queue(-1, -1);

    return true;

    show_blending_err: camera_dv_blending_deinit();

    create_blending_err: camera_dv_ctr_stop_preview();

    start_preview_err: camera_dv_msgbox_delete();

    create_msgbox_err: camera_dv_mmm_close();

    camera_open_err: canvas_set_blending_mode(false);
    gui_dc_clear(g_camera_scene_dv->hdc );

    camera_dv_dialog_create(dialog_str_type_temp);
    data->need_exit = true;
    return true;

    read_cfg_err: gui_wm_delete_window(data->win_id);
    data->win_id = 0;

    create_window_err:

    #if (DV_ICON_STATUS2SDRAM == 1)
    _dv_ui_unload_icon2sdram();
    load_icon2sdram_err:
    #endif /* DV_ICON_STATUS2SDRAM */

    load_resource_err: _dv_unset_default_font();

    create_font_err: _dv_close_str_res();

    open_str_err: _dv_close_theme();

    open_theme_err: return false;

}

/* scene exit function */
static bool _dv_deinit(void)
{
    camera_scene_dv_t *data = g_camera_scene_dv;

    s_shortcut_playback = false;

    if (s_dv_deinited == true)
    {
        return true;
    }

    camera_dv_ctr_stop_preview();

    camera_dv_msgbox_delete();

    camera_dv_blending_deinit();

    camera_dv_deinit_all_cfg();

    camera_dv_mmm_close();

    canvas_set_blending_mode(false);

    if (data->win_id != 0)
    {
        gui_wm_delete_window(data->win_id);
        data->win_id = (int) NULL;
    }

    #if (DV_ICON_STATUS2SDRAM == 1)
    _dv_ui_unload_icon2sdram();
    #endif /* DV_ICON_STATUS2SDRAM */

    _dv_unset_default_font();

    if (_dv_close_theme() == false)
    {
        return false;
    }

    if (_dv_close_str_res() == false)
    {
        return false;
    }

    return true;

}


static bool _dv_create_thread(void *proc, void *param)
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
static void  _dv_do_stop_done( void )
{   
    if (s_dialog_recstopped != CAM_DIALOG_SAVE_FAIL)
    {
        camera_dv_dialog_delete();
    }
    sys_forbid_auto_standby(false);
    //enter_foreground_mode();
    
    if (s_dialog_recstopped == CAM_DIALOG_NOSPACE)
    {
        camera_dv_dialog_create(CAM_DIALOG_NOSPACE);
    }
    #if 0
    if (s_dialog_recstopped == CAM_DIALOG_SAVE_FAIL)
    {
        camera_dv_dialog_create(CAM_DIALOG_SAVE_FAIL);
    }
    #endif
    camera_dv_change_icon(CAM_ICON_REC_STATUS, CAM_DV_STATUS_STOP);
    g_camera_scene_dv->dv_config.dv_status.time = 0;
    camera_dv_ctr_get_disk_info(&(g_camera_scene_dv->dv_config.cfg_freeinfo));
    camera_dv_change_icon(CAM_ICON_USE_FREE_NUMBER, 0);
    s_dialog_recstopped = CAM_DIALOG_TAG_ERR;
    s_do_stoping = false;
    if (s_dv_quiting == true)
    {
        exit_msg_loop();
        g_camera_scene_dv->exit_type = CAM_SCENE_EXIT;
    }
}
static void* _dv_do_stop(void* arg )
{
    s_dialog_recstopped = camera_dv_record_stop();

    #if 0
    {
        mmm_dv_status_t dv_status = {0};
        //while (dv_status.status != MMM_RECORDER_STOPPED)
        {
            //usleep(1000* 300 );
            camera_dv_get_record_status(&dv_status);
            print_err("stop over status==%d\n",dv_status.status);
        }
    }
    #endif
    //camera_dv_change_icon(CAM_ICON_REC_STATUS,CAM_DV_STATUS_STOP);
    set_single_shot_timer( 1, (timer_proc) _dv_do_stop_done, NULL );
    return (void*)0;
}

static void _dv_change_record_stop(void)
{
    if (s_do_stoping == true)
    {
        return;
    }
    s_do_stoping = true;
    
     //TODO 待修改中间件后改icon显示顺序
    camera_dv_kill_timer(g_camera_scene_dv);
    /*! 保存提示,立即显示到停止后显示消失 */


    /*! 停止录像正在录制时间要清零 */
    print_info("save file\n ");
    /*! 查看显示和动作的顺序安排 */


    if(s_dialog_recstopped == CAM_DIALOG_SAVE_FAIL)
    {
        camera_dv_dialog_create(CAM_DIALOG_SAVE_FAIL);
    }
    else
    {
        camera_dv_dialog_create(CAM_DIALOG_SAVEFILE);
    }
    gui_screen_update();
    //camera_dv_change_icon(CAM_ICON_REC_STATUS, CAM_DV_STATUS_STOP);
    //g_camera_scene_dv->dv_config.dv_status.time = 0;
    //camera_dv_change_icon(CAM_ICON_USE_FREE_NUMBER, 0);
    
    _dv_create_thread(_dv_do_stop, NULL );

    #if 0
    camera_dv_record_stop();
    {
        mmm_dv_status_t dv_status = {0};
        while (dv_status.status != MMM_RECORDER_STOPPED)
        {
            usleep(1000* 300 );
            camera_dv_get_record_status(&dv_status);
            print_dbg("stop over status==%d\n",dv_status.status);
        }
    }

    //camera_dv_change_icon(CAM_ICON_REC_STATUS,CAM_DV_STATUS_STOP);

    camera_dv_dialog_delete();
    sys_forbid_auto_standby(false);
    #endif

}
static void _dv_change_record_action(mmm_dv_status_t dv_status, change_rec_even_e even_type)
{
    bool result = false;
    switch (dv_status.status)
    {
        case MMM_RECORDER_STOPPED:

        //result = camera_dv_check_path();
        result = camera_dv_check_path((hotplugin_dev_type_e)g_camera_dv_cfgs[CAM_DV_CFG_TYPE_SAVE_PATH].value);
        // TODO:result must use bool
        if(result == false)
        {
            //camera_dv_dialog_create(CAM_DIALOG_DISKINSERT);
            break;
        }


        if (g_camera_scene_dv->dv_config.cfg_freeinfo.info <= CAM_THRESHOLD_SPACE)
        {
            camera_dv_kill_timer(g_camera_scene_dv);
            camera_dv_warning_nospace();
        }

        /*! 如果电压低于门限，禁止录像，但仍可预览 */
        else if( sys_power_threshold_check(POWER_THRESHOLD_DV_REC) == false)
        {
            camera_dv_kill_timer(g_camera_scene_dv);
            camera_dv_dialog_create(CAM_DIALOG_NOPOWER);
            gui_screen_update();
        }

        else
        {
            /*! 启动timer，改变显示图标 */
            if (dv_record_ing == 0)
            {
                dv_record_ing = 1;
                g_camera_scene_dv->dv_recordtimer_id = set_timer(500, (timer_proc) _dv_recording_event,
                        (void *) g_camera_scene_dv);
            }
            camera_dv_change_icon(CAM_ICON_REC_STATUS, CAM_DV_STATUS_RECING);
            camera_dv_change_icon(CAM_ICON_USE_FREE_NUMBER, 0);
            //录制中不关机
            sys_forbid_auto_standby(true);
            camera_dv_record_start();
        }

        break;
        case MMM_RECORDER_PAUSED:
        camera_dv_change_icon(CAM_ICON_REC_STATUS, CAM_DV_STATUS_RECING);
        print_info("resume...\n");
        camera_dv_record_resume();
        #if 0
        if(g_camera_dv_cfgs[CAM_DV_CFG_TYPE_ACTIVITY_DETECT].value == 0)
        {
            sys_forbid_auto_standby(true);
        }        
        #endif
        print_info("resume. over...\n");
        break;
        case MMM_RECORDER_RECORDING:
        print_info("pause. over...\n");
        camera_dv_record_pause();
        print_info("pause...\n");
        #if 0
        if(g_camera_dv_cfgs[CAM_DV_CFG_TYPE_ACTIVITY_DETECT].value == 0)
        {
            sys_forbid_auto_standby(false);
        }
        #endif
        camera_dv_change_icon(CAM_ICON_REC_STATUS, CAM_DV_STATUS_PAUSE);
        break;
        case MMM_RECORDER_ERROR:
        print_err();
        break;
        default:
        break;
    }
}

static int _dv_proc_gui(camera_scene_dv_t *data, gui_wm_message_t *pmsg)
{
    int ret = 0;
    key_event_t *kmsg;

    if ((s_do_stoping == true) || (s_dv_quiting == true))
    {
        return 0;
    }
    
    switch (pmsg->msgid)
    {
        case WM_MSG_CREATE:
        break;

        case WM_MSG_KEY:
        kmsg = (key_event_t *) (pmsg->data.p);
        if (data->opt != NULL)
        {
            gui_wm_default_callback(pmsg);
            break;
        }

        switch (kmsg->val | kmsg->type)
        {
            case KEY_VSUB | KEY_TYPE_DOWN:
            case KEY_VADD | KEY_TYPE_DOWN:
            {
                if (data->vbar == NULL)
                {
                    /*! 截获按键 */
                    ret = 1;
                    print_dbg();
                    _dv_volumebar_create();
                    print_dbg();
                }
            }
            break;

            case KEY_PREV | KEY_TYPE_DOWN:
            {
                mmm_dv_status_t dv_status = {0};

                print_dbg();
                camera_dv_get_record_status(&dv_status);
                if (dv_status.status == MMM_RECORDER_STOPPED)
                {
                    data->exit_type = CAM_SCENE_DSC;
                    data->need_exit = false;
                    _dv_quit();
                }
            }
            clear_key_msg_queue(-1, -1);
            break;

#if (SHORTCUT_PLAYBACK_DV == 1)
            case KEY_PLAYBACK | KEY_TYPE_DOWN:
            {
                print_err("playbackDV shot in\n");
                if(camera_dv_playback() == false)
                {
                    break;
                }
                s_shortcut_playback = true;
                clear_key_msg_queue(-1, -1);
            }
            break;
#endif /* SHORTCUT_PLAYBACK_DV */

#if (SHORTCUT_CAMERA_DV == 1)
            case KEY_CAMERA | KEY_TYPE_DOWN:
                print_err("cameradv shot in\n");
#endif /* SHORTCUT_CAMERA_DV */

            case KEY_OK | KEY_TYPE_SHORT_UP:
            {
                mmm_dv_status_t dv_status= {0};

                camera_dv_get_record_status(&dv_status);
                _dv_change_record_action(dv_status, CHANGE_REC_EVEN_KEY);
            }
            clear_key_msg_queue(-1, -1);
            break;

            case KEY_NEXT | KEY_TYPE_DOWN:
            {
                mmm_dv_status_t dv_status= {0};

                camera_dv_get_record_status(&dv_status);
                if (dv_status.status == MMM_RECORDER_STOPPED)
                {
                    camera_dv_ctr_zoom();
                }
            }
            break;

            case KEY_MENU | KEY_TYPE_DOWN:
            {
                mmm_dv_status_t dv_status= {0};

                print_dbg();
                camera_dv_get_record_status(&dv_status);
                if (dv_status.status != MMM_RECORDER_STOPPED)
                {
                    break;
                }
                if (data->opt == NULL)
                {
                    s_shortcut_playback = false;
                    camera_dv_widget_option_create(_dv_widget_option_callback);
                    print_dbg();
                    if (data->opt == NULL)
                    {
                        print_err();
                        return false;
                    }
                    else
                    {
                    }
                    print_dbg();
                }

            }
            break;

            case KEY_RETURN | KEY_TYPE_DOWN:
            {
                mmm_dv_status_t dv_status= {0};

                print_dbg();
                camera_dv_get_record_status(&dv_status);
                if (dv_status.status == MMM_RECORDER_STOPPED)
                {
                    if (data->opt == NULL)
                    {
                        data->exit_type = CAM_SCENE_EXIT;
                        _dv_quit();
                    }
                }
                else
                {
                    s_dialog_recstopped = CAM_DIALOG_TAG_ERR;                    
                    _dv_change_record_stop();
                    break;
                }
            }
            clear_key_msg_queue(-1, -1);
            break;

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
static void _dv_callback(gui_wm_message_t *pmsg)
{
    int top;

    print_dbg();

    //top = canvas_get_focus(pmsg);
    top = canvas_get_blending_focus(pmsg);

    //dispatch msg
    if ((g_camera_scene_dv->vbar != NULL) && (top == g_camera_scene_dv->vbar->canvas_id)) //volumebar process
    {
        volumebar_proc_gui(g_camera_scene_dv->vbar, pmsg);
    }
    else if ((g_camera_scene_dv->dialog != NULL) && (top == g_camera_scene_dv->dialog->canvas_id))
    {
        dialog_proc_gui(g_camera_scene_dv->dialog, pmsg);
    }
    else if ((g_camera_scene_dv->opt != NULL) && (top == g_camera_scene_dv->opt->canvas_id)) //option process
    {
        menuview_proc_gui(g_camera_scene_dv->opt, pmsg);
    }
    else if ((g_camera_scene_dv->adjust != NULL) && (top == g_camera_scene_dv->adjust->canvas_id)) //adjust
    {
        adjust_proc_gui(g_camera_scene_dv->adjust, pmsg);
    }
    else if ((g_camera_scene_dv->msgbox != NULL) && (top == g_camera_scene_dv->msgbox->canvas_id))
    {
        /* 处理默认系统消息(低电提示、卡插入和拔出提示、USB插入功能选择提示等)的msgbox控件需要接受GUI消息 */
        msgbox_proc_gui(g_camera_scene_dv->msgbox, pmsg);
    }

    else
    {
        _dv_proc_gui(g_camera_scene_dv, pmsg);
    }

}
static int _dv_sys_proc(msg_apps_t *msg)
{
    msg_reply_t t;

    print_dbg("msg.type:%d\n", msg->type);
    switch (msg->type)
    {
        case MSG_ACTIVE_SYNC:
        t.type = MSG_REPLY_SUCCESS;
        reply_sync_msg(&t);
        break;
        #if 0   //回放改为了退出应用不会用到这里
        case MSG_SET_APP_FOREGROUND:
        print_info("MSG_SET_APP_FOREGROUND\n");
        enter_foreground_mode();
        gui_wm_set_focus(g_camera_scene_dv->win_id);
        /* 设置屏幕方向 */
        sty_set_screen_rotation(0);
        g_camera_scene_dv->hdc = gui_dc_get(g_camera_scene_dv->win_id);
        ///*目前gui做法，对0层不能有任何操作,否则死机*/gui_dc_clear(g_camera_scene_dv->hdc);
        {
            bool result;
            int ret = 0;
            cam_dialog_e dialog_str_type_temp = CAM_DIALOG_OPEN_FAIL ;

            canvas_set_blending_mode(true);

            #if (DBG_REMNANT_MEMORY == 1)
            {
                unsigned int u;
                u = os_mem_query ();
                printf("-----into camera----os_mem_query_remnant=%x\n",u);
                print_mem();
                printf("-----show all module mem use info--(begin)---\n\n",u);
                dump_all_module_mem_info();
                printf("-----show all module mem use info---(end)--\n\n",u);
            }
            #endif /* DBG_REMNANT_MEMORY */

            ret = camera_dv_mmm_open();
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
                print_err("camera_dv_mmm_open\n");
                goto cb_camera_open_err;
            }

            if (camera_dv_msgbox_create() == false)
            {
                print_err("camera_dv_msgbox_create failed\n");
                goto cb_create_msgbox_err;
            }

            {
                g_camera_scene_dv->dv_config.cfg_zoom_level.denominator = 10;
                g_camera_scene_dv->dv_config.cfg_zoom_level.numerator = 10;
            }

            if (camera_dv_ctr_start_preview() == false)
            {
                print_err("camera_dv_ctr_start_preview failed\n");
                goto cb_start_preview_err;
            }

            result = camera_dv_blending_init();
            if (result == false)
            {
                print_err("camera_dv_blending_init err\n");
                goto cb_create_blending_err;
            }
            camera_dv_ui_init(&g_camera_scene_dv->dv_ui_icon);
            g_camera_scene_dv->blending.left_section.change_show_status = true;
            g_camera_scene_dv->blending.left_section.show = true;
            g_camera_scene_dv->blending.bottom_section.change_show_status = true;
            g_camera_scene_dv->blending.bottom_section.show = true;
            g_camera_scene_dv->blending.focus_section.change_show_status = true;
            g_camera_scene_dv->blending.focus_section.show = true;

            camera_scene_dv_paint(g_camera_scene_dv);

            if (_dv_ui_blending() == false)
            {
                print_err("_dv_ui_blending failed\n");
                goto cb_show_blending_err;
            }

            if (s_shortcut_playback == false)
            {
                camera_dv_widget_option_create(_dv_widget_option_callback);
            }
            else
            {
                s_shortcut_playback = false;
            }

            print_dbg();
            if (g_camera_scene_dv->opt == NULL)
            {
                print_err();
                //return false;
            }
            clear_key_msg_queue(-1, -1);

            return 0;


            cb_show_blending_err: camera_dv_blending_deinit();

            cb_create_blending_err: camera_dv_ctr_stop_preview();

            cb_start_preview_err: camera_dv_msgbox_delete();

            cb_create_msgbox_err: camera_dv_mmm_close();

            cb_camera_open_err: canvas_set_blending_mode(false);
            camera_dv_dialog_create(dialog_str_type_temp);
            g_camera_scene_dv->need_exit = true;
            return 0;
            #if 0
            gui_screen_update();
            sleep(3);
            camera_dv_dialog_delete();

            //cb_read_cfg_err:
            gui_wm_delete_window(g_camera_scene_dv->win_id);
            g_camera_scene_dv->win_id = 0;

            //cb_create_window_err:

            #if (DV_ICON_STATUS2SDRAM == 1)
            _dv_ui_unload_icon2sdram();
            //cb_load_icon2sdram_err:
            #endif /* DV_ICON_STATUS2SDRAM */

            //cb_load_resource_err:
            _dv_unset_default_font();

            //cb_create_font_err:
            _dv_close_str_res();

            //cb_open_str_err:
            _dv_close_theme();

            //cb_open_theme_err:
            s_dv_deinited = true;
            exit_msg_loop();
            g_camera_scene_dv->exit_type = CAM_SCENE_EXIT;
            #endif

        }
        //break; //语句不可达
        #endif

#if ((SHORTCUT_CAMERA_DSC == 1) || (SHORTCUT_CAMERA_DV == 1))
        case MSG_SHORTCUT_CAMERA:
        break;
#endif /* SHORTCUT_CAMERA_DSC */

#if ((SHORTCUT_PLAYBACK_DSC == 1) || (SHORTCUT_PLAYBACK_DV == 1))
        case MSG_SHORTCUT_PLAYBACK:
        break;
#endif /* SHORTCUT_PLAYBACK_DSC */

        case MSG_APP_QUIT:
        {
            bool do_quit_flag = false;
            s_dv_quiting = true;
            if(s_do_stoping == true)
            {
                break;
            }
            if(camera_dv_get_mmm_handle() != NULL)
            {
                mmm_dv_status_t dv_status= {0};
                camera_dv_get_record_status(&dv_status);
                if (dv_status.status != MMM_RECORDER_STOPPED)
                {
                    s_dialog_recstopped = CAM_DIALOG_TAG_ERR;                    
                    _dv_change_record_stop();
                    do_quit_flag = true;
                }
            }
            if (do_quit_flag != true)
            {
                exit_msg_loop();
                g_camera_scene_dv->exit_type = CAM_SCENE_EXIT;
            }

        }

        break;


        default:
        {

            /*main_disk会在确认后才发card_out等消息,所有在这之后获取主盘是可以的*/
            if (msg->type == MSG_USB_HOST_DISK_OUT)
            {
                if(g_camera_dv_cfgs[CAM_DV_CFG_TYPE_SAVE_PATH].value == CAM_DV_STORAGE_EXTERNAL)
                {
                    if (s_do_stoping == true)
                    {
                        //if saving eject out card
                        s_dialog_recstopped = CAM_DIALOG_SAVE_FAIL;
                    }
                    if(dv_record_ing !=0)
                    {
                        s_dialog_recstopped = CAM_DIALOG_SAVE_FAIL;
                        _dv_change_record_stop();
                        #if 0
                        camera_dv_dialog_create(CAM_DIALOG_SAVE_FAIL);
                        gui_screen_update();
                        #endif
                    }
                    if(camera_dv_mmm_opening_mobile_storage_out() == false)
                    {
                        print_err("dv card out err\n");
                    }
                }
            }
            else if (msg->type == MSG_CARD_OUT)
            {
                //printf("card out\n");
                if(g_camera_dv_cfgs[CAM_DV_CFG_TYPE_SAVE_PATH].value == CAM_DV_STORAGE_CARD)
                {
                    if (s_do_stoping == true)
                    {
                        //if saving eject out card
                        s_dialog_recstopped = CAM_DIALOG_SAVE_FAIL;
                    }
                    if(dv_record_ing !=0)
                    {
                        s_dialog_recstopped = CAM_DIALOG_SAVE_FAIL;                       
                        _dv_change_record_stop();
                        #if 0
                        camera_dv_dialog_create(CAM_DIALOG_SAVE_FAIL);
                        gui_screen_update();
                        #endif
                    }
                    if(camera_dv_mmm_opening_mobile_storage_out() == false)
                    {
                        print_err("dv card out err\n");
                    }
                }
            }
            else
            {
            }
            if ((msg->type == MSG_CARD_IN) || (msg->type == MSG_USB_HOST_DISK_IN) || \
                (msg->type == MSG_USB_HOST_DISK_OUT) || (msg->type == MSG_CARD_OUT))
            {
                if(g_camera_scene_dv->opt != NULL)
                {
                    camera_dv_option_savepath_refresh();
                    camera_dv_option_free_space_refresh();
                }
            }
            else
            {
            }

            if (g_camera_scene_dv->msgbox != NULL)
            {
                if(msg->type == MSG_LOW_VOLTAGE)
                {
                    if(g_camera_scene_dv->dialog != NULL)/*自己的低电弹出框 exist*/
                    {
                        if(s_dialog_str_type == CAM_DIALOG_NOPOWER)
                        {
                            break;
                        }
                    }
                }
                /* 控件msgbox用来处理默认系统消息(低电提示、卡插入和拔出提示、USB插入功能选择提示等) */
                msgbox_proc_sys(g_camera_scene_dv->msgbox, msg);
            }
            break;
        }
    }
    return 0;
}

/* scene entry function */
int camera_scene_dv(void)
{
    bool ret;
    msg_apps_t msg;
    app_cam_scene_type_e exit_type_temp;

    ret = _dv_init();
    if (ret == false)
    {
        print_err("scene init error! \n");
        return -1;
    }

    register_sys_dispatcher(_dv_sys_proc);
    while ((get_msg(&msg)) == true)
    {
        dispatch_msg(&msg);
    }

    unregister_sys_dispatcher();

    ret = _dv_deinit();
    s_dv_deinited = false;
    exit_type_temp = g_camera_scene_dv->exit_type;
    free(g_camera_scene_dv);
    g_camera_scene_dv = NULL;

    if (ret == false)
    {
        print_err("scene exit error! \n");
        return -1;
    }

    return exit_type_temp;
}

