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
 * \file    camera_scene_dsc.c
 * \brief    camera-dsc场景
 * \author   Albert_lyu
 * \version 1.0
 * \date  2009-5-26
 *******************************************************************************/
#include "camera_scene_dsc.h"
#include "camera_dsc_ui.h"
#include "camera_dsc_config.h"
#include "camera_dsc_control.h"
#include "camera_sty.h"
#include "camera_dsc_widget_option.h"
#include "camera_dsc_option_sticker_photo.h"
#include "camera_dsc_control_pcm_data.h"
#include "camera_define.h"
#include "camera_main.h"
#include "camera_dsc_control.h"
#include "camera_dsc_option_save_path.h"
#include "camera_dsc_option_free_space.h"
#include "camera_dsc_option.h"

static unsigned int s_icon_flicker_ing = 0;
static unsigned int s_cap_selftimer_ing = 0;
static unsigned int s_cap_conttimer_ing = 0;
static char s_free_number_saved_buf[ARRAY_LENGTH_TIME] = "abcdefghijk";//100:59:59
static char s_resolution_saved_buf[ARRAY_LENGTH_RESOLUTION] = "123456789";
static cam_dialog_e s_dialog_str_type = 0;
static bool s_dsc_deinited = false;
static bool s_shortcut_playback = false;
camera_scene_dsc_t* g_camera_scene_dsc = NULL;
bool g_do_snapshotting = false;
bool g_dsc_quiting = false;

static bool  _dsc_get_tvout_info(unsigned int *tvout_info);
static bool _dsc_is_tvout_on(void);
static void _dsc_icon_flicker_event(void *data);
static void _dsc_selftimer_event(void *data);
static void _dsc_conttimer_event(void *data);
static int _dsc_quit(void);
static bool _dsc_init(void);
static bool _dsc_deinit(void);
static int _dsc_proc_gui(camera_scene_dsc_t *data, gui_wm_message_t *pmsg);
static void _dsc_callback(gui_wm_message_t *pmsg);
static int _dsc_sys_proc(msg_apps_t *msg);
static bool _dsc_open_theme(void);
static bool _dsc_close_theme(void);
static bool _dsc_open_str_res(void);
static bool _dsc_close_str_res(void);
static bool _dsc_set_default_font(void);
static bool _dsc_unset_default_font(void);

static bool _dsc_ui_load_style(camera_scene_dsc_t *data);
static bool _dsc_init_blendig(resgroup_resource_t* blending_area, camera_blending_section_t *section);

static bool _dsc_ui_blending(void);
/* blending area use by Albert_lyu 2009-06-24 9:52:54 begin */
static bool _dsc_get_dialog_init(cam_dialog_e type, dialog_init_t *dialog_init);
static void _dsc_dialog_callback(dialog_t *dialog, widget_sig_dialog_e signal);
static bool _dsc_dialog_blending_create_show(void);
static bool _dsc_dialog_blending_delete(void);
//bool camera_dsc_dialog_create(cam_dialog_e type);
//bool camera_dsc_dialog_delete(void);
static void _dsc_volumebar_callback(volumebar_t *data, int signal);
static bool _dsc_volumebar_blending_create_show(void);
static bool _dsc_volumebar_blending_delete(void);
static bool _dsc_volumebar_create(void);
#if 0//nouse
static bool _dsc_volumebar_delete( void ); //nouse
#endif
//bool camera_dsc_option_blending_create_show(void);
static void _dsc_widget_option_callback(menuview_t *menuview, menuview_sig_e signal);
//bool camera_dsc_adjust_blending_create_show(void);
//bool camera_dsc_adjust_blending_delete(void);
//bool camera_dsc_option_blending_delete(void);
static bool _dsc_msgbox_blending_create_show(void);
static void _dsc_msgbox_callback(msgbox_t *msgbox, msgbox_sig_e signal);
static bool _dsc_msgbox_blending_create_show(void);
static bool _dsc_msgbox_blending_delete(void);
//bool camera_dsc_msgbox_create(void);
//bool camera_dsc_msgbox_delete(void);

/* blending area use by Albert_lyu 2009-06-24 end*/

static bool  _dsc_get_tvout_info(unsigned int *tvout_info)
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

static bool _dsc_is_tvout_on(void)
{
    int tvout_info;
    bool ret = false;
    if(true == _dsc_get_tvout_info(&tvout_info))
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


static void _dsc_icon_flicker_event(void *data)
{
    if (s_icon_flicker_ing == 0)
    {
        return;
    }

    if (((s_icon_flicker_ing) % 2) == 1)
    {
        s_icon_flicker_ing++;
        camera_dsc_change_icon(CAM_ICON_SELFTIME, 5);
        return;
    }
    else
    {
        s_icon_flicker_ing++;
        if (s_cap_selftimer_ing != 0)
        {
            camera_dsc_change_icon(CAM_ICON_SELFTIME, g_camera_scene_dsc->dsc_config.cfg_adj_self_time);
        }
        else if (s_cap_conttimer_ing != 0)
        {
            camera_dsc_change_icon(CAM_ICON_SELFTIME, g_camera_scene_dsc->dsc_config.cfg_adj_cont_shooting);
        }
        else
        {
            print_err();
        }
        return;
    }
}
static void _dsc_selftimer_event(void *data)
{
    /*! 自拍时候闪烁用 */
    s_icon_flicker_ing = 0;
    kill_timer(g_camera_scene_dsc->dsc_icon_flicker_id);
    g_camera_scene_dsc->dsc_icon_flicker_id = 0;

    s_cap_selftimer_ing = 1;
    camera_dsc_change_icon(CAM_ICON_SELFTIME, 0);
    camera_dsc_save_parameter(CAM_DSC_PARA_SELF_TIMER, 0);

    kill_timer(g_camera_scene_dsc->dsc_selftimer_id);
    g_camera_scene_dsc->dsc_selftimer_id = 0;
    camera_dsc_snapshot();
    s_cap_selftimer_ing = 0;
    /*! 自拍结束，自拍时间自动清零 */
    g_camera_scene_dsc->dsc_config.cfg_adj_self_time = 0;
}

bool camera_dsc_conttimer_restart(void)
{
    if(s_cap_conttimer_ing == 1)
    {
        if (g_camera_scene_dsc->dsc_config.cfg_freeinfo.info <= CAM_THRESHOLD_SPACE)
        {
            /*! 空间不够连拍结束*/
            camera_dsc_kill_timer(g_camera_scene_dsc);
        }
        else
        {
            sys_forbid_auto_standby(true);
            restart_timer(g_camera_scene_dsc->dsc_conttimer_id);
            restart_timer(g_camera_scene_dsc->dsc_icon_flicker_id);
        }
        return true;
    } 
    return false;
}
static void _dsc_conttimer_event(void *data)
{
    /*! 连拍时候闪烁用 */
    stop_timer(g_camera_scene_dsc->dsc_icon_flicker_id);

    s_cap_conttimer_ing = 1;
    camera_dsc_change_icon(CAM_ICON_SELFTIME, g_camera_scene_dsc->dsc_config.cfg_adj_cont_shooting);

    stop_timer(g_camera_scene_dsc->dsc_conttimer_id);
    camera_dsc_snapshot();
    #if 0
    if (g_camera_scene_dsc->dsc_config.cfg_freeinfo.info <= CAM_THRESHOLD_SPACE)
    {
        /*! 空间不够连拍结束*/
        camera_dsc_kill_timer((camera_scene_dsc_t *)data);
    }
    else
    {
        sys_forbid_auto_standby(true);
        restart_timer(g_camera_scene_dsc->dsc_conttimer_id);
        restart_timer(g_camera_scene_dsc->dsc_icon_flicker_id);
    }
    #endif
}

/*!==========blending处理=========== */
#define B_DIALOG
static bool _dsc_get_dialog_init(cam_dialog_e type, dialog_init_t *dialog_init)
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
    resgroup = (resgroup_resource_t *) get_scene_child(scene, DSC_STRGROUP_DIALOG);

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

static void _dsc_dialog_callback(dialog_t *dialog, widget_sig_dialog_e signal)
{
    print_dbg("signal:0x%x", signal);
    switch (signal)
    {
        case WIDGET_SIG_DIALOG_OK:
        camera_dsc_dialog_delete();
        if (g_camera_scene_dsc->need_exit == true)
        {
            g_camera_scene_dsc->exit_type = CAM_SCENE_EXIT;
            _dsc_quit();
        }
        break;

        case WIDGET_SIG_DIALOG_CANCAL:
        if(g_do_snapshotting == true)
        {
            break;
        }            
        //timer 3s over will go here
        camera_dsc_dialog_delete();
        if (g_camera_scene_dsc->need_exit == true)
        {
            g_camera_scene_dsc->exit_type = CAM_SCENE_EXIT;
            _dsc_quit();
        }
        break;
        default:
        break;
    }
}

static bool _dsc_dialog_blending_create_show(void)
{
    bool result = true;
    canvas_info_t canvas_info =
    { 0 };

    if (camera_dsc_get_mmm_handle() == NULL)
    {
        return true;
    }
    canvas_hide(g_camera_scene_dsc->dialog->canvas_id);
    canvas_get_info(g_camera_scene_dsc->dialog->canvas_id, &canvas_info);

    g_camera_scene_dsc->blending.dialog_section.show = false;
    g_camera_scene_dsc->blending.dialog_section.change_show_status = true;
    g_camera_scene_dsc->blending.dialog_section.creat.blend_param.color_key = 0;
    g_camera_scene_dsc->blending.dialog_section.creat.blend_param.alpha = CAM_DEFAULT_BLENDING_ALPHA;
    g_camera_scene_dsc->blending.dialog_section.creat.blend_param.area.x = canvas_info.rect.x;
    g_camera_scene_dsc->blending.dialog_section.creat.blend_param.area.y = canvas_info.rect.y;
    g_camera_scene_dsc->blending.dialog_section.creat.blend_param.area.w = canvas_info.rect.width;
    g_camera_scene_dsc->blending.dialog_section.creat.blend_param.area.h = canvas_info.rect.height;
    g_camera_scene_dsc->blending.dialog_section.creat.blend_param.blending_buf = (unsigned char *) canvas_info.pdata;
    g_camera_scene_dsc->blending.dialog_section.creat.handle = -1;
    result = camera_dsc_ctr_blending_area_create(&g_camera_scene_dsc->blending.dialog_section.creat);
    if ((false == result) || (g_camera_scene_dsc->blending.dialog_section.creat.handle < 0))
    {
        print_err("send AREA_CREATE err\n\n");
        return false;
    }

    {
        int show_array[MAX_BLENDING_AREA + 1];
        memset(show_array, BLENDING_HANLE_NULL, sizeof(show_array));
        show_array[0] = g_camera_scene_dsc->blending.dialog_section.creat.handle;
        result = camera_dsc_ctr_blending_area_show(show_array);
    }

    return true;
}

static bool _dsc_dialog_blending_delete(void)
{
    bool result = true;

    if (camera_dsc_get_mmm_handle() == NULL)
    {
        return true;
    }
    if (-1 != g_camera_scene_dsc->blending.dialog_section.creat.handle)
    {
        result = camera_dsc_ctr_blending_area_delete((int) g_camera_scene_dsc->blending.dialog_section.creat.handle);
        g_camera_scene_dsc->blending.dialog_section.creat.handle = -1;
    }
    //canvas_delete(g_camera_scene_dsc->blending.dialog_section.canvas_id);
    return result;
}

bool camera_dsc_dialog_create(cam_dialog_e type)
{
    bool result = true;
    dialog_init_t init;

    if (NULL != g_camera_scene_dsc->dialog)
    {
        camera_dsc_dialog_delete();
    }
    memset(&init, 0x00, sizeof(init));
    result = _dsc_get_dialog_init(type, &init);
    if (false == result)
    {
        return false;
    }

    g_camera_scene_dsc->dialog = dialog_create(&init, (dialog_cb_t) _dsc_dialog_callback);
    if (NULL == g_camera_scene_dsc->dialog)
    {
        print_err("dialog_create fail\n\n");
        return false;
    }

    _dsc_dialog_blending_create_show();

    dialog_paint(g_camera_scene_dsc->dialog);

    return true;
}

bool camera_dsc_dialog_delete(void)
{
    _dsc_dialog_blending_delete();

    if (NULL != g_camera_scene_dsc->dialog)
    {
        dialog_delete(g_camera_scene_dsc->dialog);
    }
    g_camera_scene_dsc->dialog = NULL;
    s_dialog_str_type = 0;

    return true;
}

#if (USE_MODAL_DIALOG == 1)
bool camera_dsc_dialog_modal( cam_dialog_e type )
{
    bool ret = false;
    int result = -1;
    dialog_init_t init;

    memset( &init, 0x00, sizeof(init) );
    result = _dsc_get_dialog_init( type, &init );
    if ( false == result )
    {
        return ret;
    }

    _dsc_dialog_blending_create_show();

    result = dialog_modal( &init );

    _dsc_dialog_blending_delete();

    if (result != -1)
    {
        ret  = true;
    }
    return ret;
}
#endif /* USE_MODAL_DIALOG */

#undef B_DIALOG
#define B_VOLBAR
static void _dsc_volumebar_callback(volumebar_t *data, int signal)
{
    switch (signal)
    {
        case WIDGET_SIG_VOLUMEBAR_CHANGED:
        print_dbg();
        break;

        case WIDGET_SIG_VOLUMEBAR_QUIT:
        print_dbg();
        _dsc_volumebar_blending_delete();
        //如果vbar从内部timer删除是否有问题
        volumebar_delete(data);
        g_camera_scene_dsc->vbar = NULL;
        break;

        default:
        break;
    }
}

static bool _dsc_volumebar_blending_create_show(void)
{
    bool result = true;
    canvas_info_t canvas_info =
    { 0 };

    canvas_hide(g_camera_scene_dsc->vbar->canvas_id);
    canvas_get_info(g_camera_scene_dsc->vbar->canvas_id, &canvas_info);

    g_camera_scene_dsc->blending.volbar_section.show = false;
    g_camera_scene_dsc->blending.volbar_section.change_show_status = true;
    g_camera_scene_dsc->blending.volbar_section.creat.blend_param.color_key = 0;
    g_camera_scene_dsc->blending.volbar_section.creat.blend_param.alpha = CAM_DEFAULT_BLENDING_ALPHA;
    g_camera_scene_dsc->blending.volbar_section.creat.blend_param.area.x = canvas_info.rect.x;
    g_camera_scene_dsc->blending.volbar_section.creat.blend_param.area.y = canvas_info.rect.y;
    g_camera_scene_dsc->blending.volbar_section.creat.blend_param.area.w = canvas_info.rect.width;
    g_camera_scene_dsc->blending.volbar_section.creat.blend_param.area.h = canvas_info.rect.height;
    g_camera_scene_dsc->blending.volbar_section.creat.blend_param.blending_buf = (unsigned char *) canvas_info.pdata;
    g_camera_scene_dsc->blending.volbar_section.creat.handle = -1;
    result = camera_dsc_ctr_blending_area_create(&g_camera_scene_dsc->blending.volbar_section.creat);
    if ((false == result) || (g_camera_scene_dsc->blending.volbar_section.creat.handle < 0))
    {
        print_err("send AREA_CREATE err\n\n");
        return false;
    }

    {
        int show_array[MAX_BLENDING_AREA + 1];
        memset(show_array, BLENDING_HANLE_NULL, sizeof(show_array));
        show_array[0] = g_camera_scene_dsc->blending.volbar_section.creat.handle;
        result = camera_dsc_ctr_blending_area_show(show_array);
    }

    return true;
}

static bool _dsc_volumebar_blending_delete(void)
{
    bool result = true;

    if (camera_dsc_get_mmm_handle() == NULL)
    {
        return true;
    }
    if (-1 != g_camera_scene_dsc->blending.volbar_section.creat.handle)
    {
        result = camera_dsc_ctr_blending_area_delete((int) g_camera_scene_dsc->blending.volbar_section.creat.handle);
        g_camera_scene_dsc->blending.volbar_section.creat.handle = -1;
    }

    return result;
}

static bool _dsc_volumebar_create(void)
{
    g_camera_scene_dsc->vbar = volumebar_create(0, (volumebar_cb_t) _dsc_volumebar_callback);
    if (NULL == g_camera_scene_dsc->vbar)
    {
        print_err("volumebar_create fail");
        return false;
    }

    _dsc_volumebar_blending_create_show();

    volumebar_paint(g_camera_scene_dsc->vbar);

    return true;
}
static bool _dsc_volumebar_delete(void)
{
    _dsc_volumebar_blending_delete();
    if (g_camera_scene_dsc->vbar != NULL)
    {
        volumebar_delete(g_camera_scene_dsc->vbar);
    }
    g_camera_scene_dsc->vbar = NULL;
    return true;
}
#undef B_VOLBAR
#define   B_OPTION
/*! option和adjust都只在option.c中用到，为了统一都放在这里 */
static void _dsc_widget_option_callback(menuview_t *menuview, menuview_sig_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_QUIT:
        camera_dsc_widget_option_delete(g_camera_scene_dsc->opt);
        g_camera_scene_dsc->opt = NULL;
        break;

        default:
        break;
    }
}

bool camera_dsc_option_blending_create_show(void)
{
    bool result;
    canvas_info_t canvas_info =
    { 0 };

    canvas_hide(g_camera_scene_dsc->opt->canvas_id);
    canvas_get_info(g_camera_scene_dsc->opt->canvas_id, &canvas_info);
    g_camera_scene_dsc->blending.menu_section.show = false;
    g_camera_scene_dsc->blending.menu_section.change_show_status = true;
    g_camera_scene_dsc->blending.menu_section.creat.blend_param.color_key = 0;
    g_camera_scene_dsc->blending.menu_section.creat.blend_param.alpha = CAM_DEFAULT_BLENDING_ALPHA;
    g_camera_scene_dsc->blending.menu_section.creat.blend_param.area.x = canvas_info.rect.x;
    g_camera_scene_dsc->blending.menu_section.creat.blend_param.area.y = canvas_info.rect.y;
    g_camera_scene_dsc->blending.menu_section.creat.blend_param.area.w = canvas_info.rect.width;
    g_camera_scene_dsc->blending.menu_section.creat.blend_param.area.h = canvas_info.rect.height;
    g_camera_scene_dsc->blending.menu_section.creat.blend_param.blending_buf = (unsigned char *) canvas_info.pdata;
    g_camera_scene_dsc->blending.menu_section.creat.handle = -1;
    assert(canvas_info.rect.x %8 == 0);
    assert(canvas_info.rect.y %8 == 0);
    assert(canvas_info.rect.width %8 == 0);
    assert(canvas_info.rect.height %8 == 0);
    result = camera_dsc_ctr_blending_area_create(&g_camera_scene_dsc->blending.menu_section.creat);
    if ((false == result) || (g_camera_scene_dsc->blending.menu_section.creat.handle < 0))
    {
        print_err("send AREA_CREATE err\n\n");
        return false;
    }

    {
        int show_array[MAX_BLENDING_AREA + 1];
        memset(show_array, BLENDING_HANLE_NULL, sizeof(show_array));
        show_array[0] = g_camera_scene_dsc->blending.menu_section.creat.handle;
        result = camera_dsc_ctr_blending_area_show(show_array);
    }

    return true;
}

/*call by Camera_dsc_option.c Camera_dsc_widget_option.c */
bool camera_dsc_option_blending_delete(void)
{
    bool result = true;

    if (camera_dsc_get_mmm_handle() == NULL)
    {
        return true;
    }

    if (-1 != g_camera_scene_dsc->blending.menu_section.creat.handle)
    {
        result = camera_dsc_ctr_blending_area_delete((int) g_camera_scene_dsc->blending.menu_section.creat.handle);
        g_camera_scene_dsc->blending.menu_section.creat.handle = -1;
    }

    return result;
}
static bool _dsc_option_delete(void)
{
    if (g_camera_scene_dsc->opt != NULL)
    {
        /*! 删除menu的数据 */
        camera_dsc_widget_option_deinit();
        /*! 删除menu的视图 */
        camera_dsc_widget_option_delete(g_camera_scene_dsc->opt);
        g_camera_scene_dsc->opt = NULL;
    }
    return true;
}
#undef B_OPTION
#define B_ADJUST
/*call by Camera_dsc_option.c*/
bool camera_dsc_adjust_blending_create_show(void)
{
    bool result;
    canvas_info_t canvas_info =
    { 0 };

    print_info("bf creat blending adjust....1\n");
    canvas_hide(g_camera_scene_dsc->adjust->canvas_id);
    canvas_get_info(g_camera_scene_dsc->adjust->canvas_id, &canvas_info);

    g_camera_scene_dsc->blending.adjust_section.show = false;
    g_camera_scene_dsc->blending.adjust_section.change_show_status = true;
    g_camera_scene_dsc->blending.adjust_section.creat.blend_param.color_key = 0;
    g_camera_scene_dsc->blending.adjust_section.creat.blend_param.alpha = CAM_DEFAULT_BLENDING_ALPHA;
    g_camera_scene_dsc->blending.adjust_section.creat.blend_param.area.x = canvas_info.rect.x;
    g_camera_scene_dsc->blending.adjust_section.creat.blend_param.area.y = canvas_info.rect.y;
    g_camera_scene_dsc->blending.adjust_section.creat.blend_param.area.w = canvas_info.rect.width;
    g_camera_scene_dsc->blending.adjust_section.creat.blend_param.area.h = canvas_info.rect.height;
    g_camera_scene_dsc->blending.adjust_section.creat.blend_param.blending_buf = (unsigned char *) canvas_info.pdata;
    g_camera_scene_dsc->blending.adjust_section.creat.handle = -1;
    assert(canvas_info.rect.x %8 == 0);
    assert(canvas_info.rect.y %8 == 0);
    assert(canvas_info.rect.width %8 == 0);
    assert(canvas_info.rect.height %8 == 0);
    result = camera_dsc_ctr_blending_area_create(&g_camera_scene_dsc->blending.adjust_section.creat);
    if ((false == result) || (g_camera_scene_dsc->blending.adjust_section.creat.handle < 0))
    {
        print_err("send AREA_CREATE err\n\n");
        return false;
    }

    {
        int show_array[MAX_BLENDING_AREA + 1];
        memset(show_array, BLENDING_HANLE_NULL, sizeof(show_array));
        show_array[0] = g_camera_scene_dsc->blending.adjust_section.creat.handle;
        result = camera_dsc_ctr_blending_area_show(show_array);
    }

    return true;
}
/*call by Camera_dsc_option.c*/
bool camera_dsc_adjust_blending_delete(void)
{
    bool result = true;

    if (camera_dsc_get_mmm_handle() == NULL)
    {
        return true;
    }

    if (-1 != g_camera_scene_dsc->blending.adjust_section.creat.handle)
    {
        result = camera_dsc_ctr_blending_area_delete((int) g_camera_scene_dsc->blending.adjust_section.creat.handle);
        g_camera_scene_dsc->blending.adjust_section.creat.handle = -1;
    }

    return result;
}
static bool _dsc_adjust_delete(void)
{
    camera_dsc_adjust_blending_delete();
    if (g_camera_scene_dsc->adjust != NULL)
    {
        adjust_delete(g_camera_scene_dsc->adjust);
        g_camera_scene_dsc->adjust = NULL;
    }
    return true;
}
/*!==========blending处理=========== */
#undef B_ADJUST
#define B_MSGBOX

static void _dsc_msgbox_callback(msgbox_t *msgbox, msgbox_sig_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_MSGBOX_SHOW:
        _dsc_msgbox_blending_create_show();
        break;

        case WIDGET_SIG_MSGBOX_QUIT:
        _dsc_msgbox_blending_delete();
        break;

        case WIDGET_SIG_MSGBOX_QUIT_LOW_POWER:
         /*! 自拍连拍中低电会停止和清除自拍连拍功能 */
        if ((s_cap_conttimer_ing != 0) || (s_cap_selftimer_ing != 0))
        {
            camera_dsc_kill_timer(g_camera_scene_dsc);
        }
        break;

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
static bool _dsc_msgbox_blending_create_show(void)
{
    bool result;
    canvas_info_t canvas_info =
    { 0 };

    canvas_hide(g_camera_scene_dsc->msgbox->canvas_id);
    canvas_get_info(g_camera_scene_dsc->msgbox->canvas_id, &canvas_info);

    g_camera_scene_dsc->blending.msgbox_section.show = false;
    g_camera_scene_dsc->blending.msgbox_section.change_show_status = true;
    g_camera_scene_dsc->blending.msgbox_section.creat.blend_param.color_key = 0;
    g_camera_scene_dsc->blending.msgbox_section.creat.blend_param.alpha = CAM_DEFAULT_BLENDING_ALPHA;
    g_camera_scene_dsc->blending.msgbox_section.creat.blend_param.area.x = canvas_info.rect.x;
    g_camera_scene_dsc->blending.msgbox_section.creat.blend_param.area.y = canvas_info.rect.y;
    g_camera_scene_dsc->blending.msgbox_section.creat.blend_param.area.w = canvas_info.rect.width;
    g_camera_scene_dsc->blending.msgbox_section.creat.blend_param.area.h = canvas_info.rect.height;
    g_camera_scene_dsc->blending.msgbox_section.creat.blend_param.blending_buf = (unsigned char *) canvas_info.pdata;
    g_camera_scene_dsc->blending.msgbox_section.creat.handle = -1;
    assert(canvas_info.rect.x %8 == 0);
    assert(canvas_info.rect.y %8 == 0);
    assert(canvas_info.rect.width %8 == 0);
    assert(canvas_info.rect.height %8 == 0);
    result = camera_dsc_ctr_blending_area_create(&g_camera_scene_dsc->blending.msgbox_section.creat);
    if ((false == result) || (g_camera_scene_dsc->blending.msgbox_section.creat.handle < 0))
    {
        print_err("send AREA_CREATE err\n\n");
        return false;
    }

    {
        int show_array[MAX_BLENDING_AREA + 1];
        memset(show_array, BLENDING_HANLE_NULL, sizeof(show_array));
        show_array[0] = g_camera_scene_dsc->blending.msgbox_section.creat.handle;
        result = camera_dsc_ctr_blending_area_show(show_array);
    }

    return true;
}

static bool _dsc_msgbox_blending_delete(void)
{
    bool result = true;

    if (camera_dsc_get_mmm_handle() == NULL)
    {
        return true;
    }

    if (-1 != g_camera_scene_dsc->blending.msgbox_section.creat.handle)
    {
        result = camera_dsc_ctr_blending_area_delete((int) g_camera_scene_dsc->blending.msgbox_section.creat.handle);
        g_camera_scene_dsc->blending.msgbox_section.creat.handle = -1;
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
bool camera_dsc_msgbox_create(void)
{
    g_camera_scene_dsc->msgbox = msgbox_create(_dsc_msgbox_callback);
    if (NULL == g_camera_scene_dsc->msgbox)
    {
        print_err("msgbox_create fail");
        return false;
    }

    return true;
}

bool camera_dsc_msgbox_delete(void)
{
    if(g_camera_scene_dsc->msgbox != NULL)
    {
        msgbox_delete(g_camera_scene_dsc->msgbox);
        g_camera_scene_dsc->msgbox = NULL;
        return true;
    }
    else
    {
        return false;
    }
}
#undef B_MSGBOX

void camera_dsc_warning_nospace(void)
{
    camera_dsc_dialog_create(CAM_DIALOG_NOSPACE);
    gui_screen_update();

    print_info("no enough space\n ");
}
void camera_dsc_kill_timer(camera_scene_dsc_t *data)
{
    if (data->dsc_selftimer_id != 0)
    {
        kill_timer(data->dsc_selftimer_id);
        data->dsc_selftimer_id = 0;
        s_cap_selftimer_ing = 0;
        g_camera_scene_dsc->dsc_config.cfg_adj_self_time = 0;
    }
    if (data->dsc_conttimer_id != 0)
    {
        kill_timer(data->dsc_conttimer_id);
        data->dsc_conttimer_id = 0;
        s_cap_conttimer_ing = 0;
        g_camera_scene_dsc->dsc_config.cfg_adj_cont_shooting = 0;
    }
    if (data->dsc_icon_flicker_id != 0)
    {
        kill_timer(data->dsc_icon_flicker_id);
        data->dsc_icon_flicker_id = 0;
        s_icon_flicker_ing = 0;
        //g_camera_scene_dsc->dsc_config.cfg_adj_cont_shooting = 0;
        camera_dsc_save_parameter(CAM_DSC_PARA_SELF_TIMER, 0);
        camera_dsc_change_icon(CAM_ICON_SELFTIME, 0);
    }
}

bool camera_dsc_ui_init(camera_dsc_ui_t *dsc_ui)
{
    dsc_ui->blending_area_left_bg.saved_show = -1;
    dsc_ui->blending_area_left_bg.show = 1;

    dsc_ui->blending_area_botton_bg.saved_show = -1;
    dsc_ui->blending_area_botton_bg.show = 1;

    dsc_ui->dsc_icon.saved_show = -1;
    dsc_ui->dsc_icon.show = 1;
    dsc_ui->dsc_icon.saved_frame = -1;
    dsc_ui->dsc_icon.frame = 0;

    dsc_ui->self_time.saved_show = -1;
    dsc_ui->self_time.show = 1;
    dsc_ui->self_time.saved_frame = -1;
    dsc_ui->self_time.frame = 0;

    dsc_ui->brightness.saved_show = -1;
    dsc_ui->brightness.show = 1;
    dsc_ui->brightness.saved_frame = -1;
    dsc_ui->brightness.frame = (char)(g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_BRIGHTNESS].value + 3);

    dsc_ui->special_effect.saved_show = -1;
    dsc_ui->special_effect.show = 1;
    dsc_ui->special_effect.saved_frame = -1;
    dsc_ui->special_effect.frame = (char)g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SPECIAL_EFFECT].value;

    dsc_ui->white_balance.saved_show = -1;
    dsc_ui->white_balance.show = 1;
    dsc_ui->white_balance.saved_frame = -1;
    dsc_ui->white_balance.frame = (char)g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_WHITE_BALANCE].value;

    if (g_camera_dsc_option_visible_cfgs[CAM_DSC_CFG_VISIBLE_TYPE_FLASH_LAMP].value != 0xffff)
    {
        dsc_ui->flash_lamp.saved_show = -1;
        dsc_ui->flash_lamp.show = 1;
        dsc_ui->flash_lamp.saved_frame = -1;
        dsc_ui->flash_lamp.frame = (char)g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_FLASH_LAMP].value;
    }

    if (g_camera_dsc_option_visible_cfgs[CAM_DSC_CFG_VISIBLE_TYPE_SHUTTER_SOUND].value != 0xffff)
    {
        dsc_ui->shutter_sound.saved_show = -1;
        dsc_ui->shutter_sound.show = 1;
        dsc_ui->shutter_sound.saved_frame = -1;
        dsc_ui->shutter_sound.frame = (char)g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SHUTTER_SOUND].value;
    }
    dsc_ui->store_type.saved_show = -1;
    dsc_ui->store_type.show = 1;
    dsc_ui->store_type.saved_frame = -1;
    dsc_ui->store_type.frame = (char)g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value;
    /*因为存储类型有:cam_dsc_save_path_e 0(无图片),1,2,3,图片只有0,1,2*/
    if(dsc_ui->store_type.frame > 0)
    {
        dsc_ui->store_type.frame -= 1;
    }

    dsc_ui->resolution.saved_show = -1;
    dsc_ui->resolution.show = 1;

    dsc_ui->resolution.saved_buf = (char *) s_resolution_saved_buf;
    dsc_ui->resolution.buf = (char *) g_dsc_resolution_buf;

    dsc_ui->free_space.saved_show = -1;
    dsc_ui->free_space.show = 1;

    dsc_ui->free_space_number.saved_show = -1;
    dsc_ui->free_space_number.show = 1;
    dsc_ui->free_space_number.saved_buf = (char *) s_free_number_saved_buf;
    dsc_ui->free_space_number.buf = (char *) g_free_number_buf;

    dsc_ui->blending_area_focus_bg.saved_show = -1;
    dsc_ui->blending_area_focus_bg.show = 1;

    dsc_ui->focus.saved_show = -1;
    dsc_ui->focus.show = 1;

    dsc_ui->foci_slider.saved_show = -1;
    dsc_ui->foci_slider.show = 1;

    memset(dsc_ui->resolution.saved_buf, 0, sizeof(dsc_ui->resolution.saved_buf));
    memset(dsc_ui->free_space_number.saved_buf, 0, sizeof(dsc_ui->free_space_number.saved_buf));

    camera_dsc_ui_free_space_to_buf();
    camera_dsc_ui_resolution_to_buf(g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_RESOLUTION].value);
    return true;
}

void camera_scene_dsc_paint(camera_scene_dsc_t *data)
{
    int hdc = gui_dc_get(data->win_id);
    camera_dsc_ui_t *p = &(data->dsc_ui_icon);
    int y;
    int canvas_id;
    static int s_pre_savepath = -1;
    static int s_pre_pathstatus = -1;


    print_dbg("%s----in\n", __FUNCTION__);

    hdc = gui_dc_get(g_camera_scene_dsc->blending.left_section.canvas_id);
    canvas_id = g_camera_scene_dsc->blending.left_section.canvas_id;
    camera_dsc_ui_draw_picregion(hdc, &(p->dsc_icon), canvas_id);
    camera_dsc_ui_draw_picregion(hdc, &(p->self_time), canvas_id);
    camera_dsc_ui_draw_picregion(hdc, &(p->brightness), canvas_id);
    camera_dsc_ui_draw_picregion(hdc, &(p->special_effect), canvas_id);
    camera_dsc_ui_draw_picregion(hdc, &(p->white_balance), canvas_id);
    if (g_camera_dsc_option_visible_cfgs[CAM_DSC_CFG_VISIBLE_TYPE_FLASH_LAMP].value != 0xffff)
    {
        camera_dsc_ui_draw_picregion(hdc, &(p->flash_lamp), canvas_id);
    }
    if (g_camera_dsc_option_visible_cfgs[CAM_DSC_CFG_VISIBLE_TYPE_SHUTTER_SOUND].value != 0xffff)
    {
        camera_dsc_ui_draw_picregion(hdc, &(p->shutter_sound), canvas_id);
    }
    hdc = gui_dc_get(g_camera_scene_dsc->blending.bottom_section.canvas_id);
    canvas_id = g_camera_scene_dsc->blending.bottom_section.canvas_id;

    camera_dsc_ui_draw_string(hdc, &(p->resolution), canvas_id);

    if ((g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value != CAM_DSC_STORAGE_NONE) && \
        (g_setpath != false))
    {
        //camera_dsc_ui_draw_pic(hdc, &(p->free_space));
        camera_dsc_ui_draw_picregion(hdc, &(p->store_type), canvas_id);

        camera_dsc_ui_draw_string(hdc, &(p->free_space_number), canvas_id);
    }
    else
    {
        // TODO:recheck here ,will re clear
        /*if((s_pre_savepath != g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value) || \
            (s_pre_pathstatus != (int)g_setpath))*/
        {
            //camera_dsc_ui_draw_pic(hdc, &(p->blending_area_botton_bg));
            canvas_erase_all(canvas_id);
            //重刷新res
            camera_dsc_ui_draw_string(hdc, &(p->resolution), canvas_id);
        }
    }
    s_pre_savepath = g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value;
    s_pre_pathstatus = (int)g_setpath;

    hdc = gui_dc_get(g_camera_scene_dsc->blending.focus_section.canvas_id);
    canvas_id = g_camera_scene_dsc->blending.focus_section.canvas_id;

    camera_dsc_ui_draw_pic(hdc, &(p->focus));


    hdc = gui_dc_get(g_camera_scene_dsc->blending.slider_section.canvas_id);
    canvas_id = g_camera_scene_dsc->blending.slider_section.canvas_id;

    camera_dsc_ui_zoom_level_to_coordinate(g_camera_scene_dsc->dsc_config.cfg_zoom_level.numerator
            / g_camera_scene_dsc->dsc_config.cfg_zoom_level.denominator, 3, &y);
    camera_dsc_ui_draw_slider(hdc, &(p->foci_slider), y);

    print_dbg();
    /* 立即更新到屏幕 */
    //gui_screen_update();


}

static int _dsc_quit(void)
{
    exit_msg_loop();
    return 0;
}

/*
 * 创建各blending区域, 但都处于hide状态.
 * 需要显示时, 再将其设置为show状态
 */
static bool _dsc_init_blendig(resgroup_resource_t* blending_area, camera_blending_section_t *section)
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
    result = camera_dsc_ctr_blending_area_create( (mmm_enc_blending_create_t *)(&section->creat));

    return result;
}


bool camera_dsc_blending_init(void)
{
    bool result = true;
    resgroup_resource_t* blending_area;
    camera_blending_section_t* section;

    blending_area = g_camera_scene_dsc->dsc_ui_icon.blending_area_left;
    section = &(g_camera_scene_dsc->blending.left_section);
    result = _dsc_init_blendig(blending_area, section);

    blending_area = g_camera_scene_dsc->dsc_ui_icon.blending_area_botton;
    section = &(g_camera_scene_dsc->blending.bottom_section);
    result = _dsc_init_blendig(blending_area, section);

    blending_area = g_camera_scene_dsc->dsc_ui_icon.blending_area_focus;
    section = &(g_camera_scene_dsc->blending.focus_section);
    result = _dsc_init_blendig(blending_area, section);

    blending_area = g_camera_scene_dsc->dsc_ui_icon.blending_area_foci_slider;
    section = &(g_camera_scene_dsc->blending.slider_section);
    result = _dsc_init_blendig(blending_area, section);

    return result;
}

/*
 * 删除各blending区域, 释放资源
 */
bool camera_dsc_blending_deinit(void)
{
    bool result = true;

    if ( -1 != g_camera_scene_dsc->blending.left_section.creat.handle)
    {
        result = camera_dsc_ctr_blending_area_delete((int) g_camera_scene_dsc->blending.left_section.creat.handle);
        g_camera_scene_dsc->blending.left_section.creat.handle = -1;
    }
    if (-1 != g_camera_scene_dsc->blending.left_section.canvas_id)
    {
        canvas_delete(g_camera_scene_dsc->blending.left_section.canvas_id);
        g_camera_scene_dsc->blending.left_section.canvas_id = -1;
    }

    if ( -1 != g_camera_scene_dsc->blending.bottom_section.creat.handle)
    {
        result = camera_dsc_ctr_blending_area_delete((int) g_camera_scene_dsc->blending.bottom_section.creat.handle);
        g_camera_scene_dsc->blending.bottom_section.creat.handle = -1;
    }
    if(-1 != g_camera_scene_dsc->blending.bottom_section.canvas_id)
    {
        canvas_delete(g_camera_scene_dsc->blending.bottom_section.canvas_id);
        g_camera_scene_dsc->blending.bottom_section.canvas_id = -1;
    }

    if ( -1 != g_camera_scene_dsc->blending.focus_section.creat.handle)
    {
        result = camera_dsc_ctr_blending_area_delete((int) g_camera_scene_dsc->blending.focus_section.creat.handle);
        g_camera_scene_dsc->blending.focus_section.creat.handle = -1;
    }
    if(-1 != g_camera_scene_dsc->blending.focus_section.canvas_id)
    {
        canvas_delete(g_camera_scene_dsc->blending.focus_section.canvas_id);
        g_camera_scene_dsc->blending.focus_section.canvas_id = -1;
    }

    if ( -1 != g_camera_scene_dsc->blending.slider_section.creat.handle)
    {
        result = camera_dsc_ctr_blending_area_delete((int) g_camera_scene_dsc->blending.slider_section.creat.handle);
        g_camera_scene_dsc->blending.slider_section.creat.handle = -1;
    }
    if(-1 != g_camera_scene_dsc->blending.slider_section.canvas_id)
    {
        canvas_delete(g_camera_scene_dsc->blending.slider_section.canvas_id);
        g_camera_scene_dsc->blending.slider_section.canvas_id = -1;
    }

    /*可能创建的blending区域统一删除*/
    camera_dsc_dialog_delete();
    _dsc_option_delete();
    _dsc_adjust_delete();
    _dsc_volumebar_delete();
    _dsc_msgbox_blending_delete();

    return result;
}

static bool _dsc_ui_blending(void)
{
    bool result;
    int show_array[MAX_BLENDING_AREA + 1];
    int hide_array[MAX_BLENDING_AREA + 1];
    int show = -1;
    int hide = -1;
    int i = 0;
    camera_blending_section_t* p[] =
    {
        &g_camera_scene_dsc->blending.left_section, &g_camera_scene_dsc->blending.slider_section,
        &g_camera_scene_dsc->blending.bottom_section, &g_camera_scene_dsc->blending.focus_section,
    };
    memset(show_array, BLENDING_HANLE_NULL, sizeof(show_array));
    memset(hide_array, BLENDING_HANLE_NULL, sizeof(hide_array));
    for (i = 0; i < 4; i++)
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
        result = camera_dsc_ctr_blending_area_hide(hide_array);
        if (false == result)
        {
            print_err("send hide err\n\n");
            return false;
        }
    }

    if (show > -1)
    {
        result = camera_dsc_ctr_blending_area_show(show_array);
        if (false == result)
        {
            print_err("send show err\n\n");
            return false;
        }
    }
    return true;
}

static bool _dsc_ui_load_style(camera_scene_dsc_t *data)
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
    group = (resgroup_resource_t *) get_scene_child(scene, DSC_GROUP_BLENDING_AREA_LEFT);
    data->dsc_ui_icon.blending_area_left = group;

    data->dsc_ui_icon.blending_area_left_bg.pic_res = (picture_resource_t *) get_resgroup_child(group,
            DSC_PIC_BLENDING_LEFT);
    data->dsc_ui_icon.dsc_icon.picregion_res = (picregion_resource_t *) get_resgroup_child(group, DSC_REG_DC);
    data->dsc_ui_icon.self_time.picregion_res = (picregion_resource_t *) get_resgroup_child(group, DSC_REG_SELF_TIME);
    data->dsc_ui_icon.brightness.picregion_res = (picregion_resource_t *) get_resgroup_child(group, DSC_REG_BRIGHTNESS);
    data->dsc_ui_icon.special_effect.picregion_res = (picregion_resource_t *) get_resgroup_child(group,
            DSC_REG_SPECIAL_EFFECT);
    data->dsc_ui_icon.white_balance.picregion_res = (picregion_resource_t *) get_resgroup_child(group,
            DSC_REG_WHITE_BALANCE);
    data->dsc_ui_icon.flash_lamp.picregion_res = (picregion_resource_t *) get_resgroup_child(group, DSC_REG_FLASH_LAMP);
    data->dsc_ui_icon.shutter_sound.picregion_res = (picregion_resource_t *) get_resgroup_child(group,
            DSC_REG_SHUTTER_SOUND);

    /*! 焦距条 */
    group = (resgroup_resource_t *) get_scene_child(scene, DSC_GROUP_BLENDING_AREA_FOCI_SLIDER);
    data->dsc_ui_icon.blending_area_foci_slider = group;
    data->dsc_ui_icon.foci_slider.pic_bg_res = (picture_resource_t *) get_resgroup_child(group, DSC_PIC_SLIDER_BG);
    data->dsc_ui_icon.foci_slider.pic_h_res = (picture_resource_t *) get_resgroup_child(group, DSC_PIC_SLIDER_H);
    data->dsc_ui_icon.foci_slider.max_str_res = (string_resource_t *) get_resgroup_child(group, DSC_STR_MAX);
    data->dsc_ui_icon.foci_slider.min_str_res = (string_resource_t *) get_resgroup_child(group, DSC_STR_MIN);

    /*! 底部区域 */
    group = (resgroup_resource_t *) get_scene_child(scene, DSC_GROUP_BLENDING_AREA_BOTTON);
    data->dsc_ui_icon.blending_area_botton = group;
    data->dsc_ui_icon.blending_area_botton_bg.pic_res = (picture_resource_t *) get_resgroup_child(group,
            DSC_PIC_BLENDING_BOTTON);
    data->dsc_ui_icon.resolution.str_res = (string_resource_t *) get_resgroup_child(group, DSC_STR_RESOLUTION);
//    data->dsc_ui_icon.free_space.pic_res = (picture_resource_t *) get_resgroup_child(group, DSC_PIC_FREE);
    data->dsc_ui_icon.store_type.picregion_res = (picregion_resource_t *) get_resgroup_child(group, DSC_REG_STORE_TYPE);

    data->dsc_ui_icon.free_space_number.str_res = (string_resource_t *) get_resgroup_child(group, DSC_STR_FREENUMBER);

    /*! 焦点区域 */
    group = (resgroup_resource_t *) get_scene_child(scene, DSC_GROUP_BLENDING_AREA_FOCUS);
    data->dsc_ui_icon.blending_area_focus = group;
    data->dsc_ui_icon.blending_area_focus_bg.pic_res = (picture_resource_t *) get_resgroup_child(group,
            DSC_PIC_BLENDING_FOCUS);
    data->dsc_ui_icon.focus.pic_res = (picture_resource_t *) get_resgroup_child(group, DSC_PIC_FOCUS);

    return true;
}

static bool _dsc_open_theme(void)
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
    g_camera_scene_dsc->common_scene = load_scene(g_app_camera->common_style, COMMONUI_SIDELONG_SCENE);

    if (g_camera_scene_dsc->common_scene == NULL)
    {
        print_err("load commonui scene failed");
        return false;
    }
    else
    {
        /* 设置 commonui 的 style 场景*/
        sty_set_common_scene(g_camera_scene_dsc->common_scene);
    }

    /* 载入应用的 style 场景*/
    g_camera_scene_dsc->scene = load_scene(g_app_camera->app_style, CAMERA_DSC_SCENE);
    if (g_camera_scene_dsc->scene == NULL)
    {
        print_err("load scene CAMERA_DSC_SCENE failed");
        return false;
    }
    else
    {
        /* 设置应用的 style 场景 */
        sty_set_app_scene(g_camera_scene_dsc->scene);
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

static bool _dsc_close_theme(void)
{
    if(g_camera_scene_dsc->common_scene != NULL)
    {
        free_scene(g_camera_scene_dsc->common_scene);
        g_camera_scene_dsc->common_scene = NULL;
    }

    if(g_camera_scene_dsc->scene != NULL)
    {
        free_scene(g_camera_scene_dsc->scene);
        g_camera_scene_dsc->scene = NULL;
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

static bool _dsc_open_str_res(void)
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

static bool _dsc_close_str_res(void)
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

static bool _dsc_set_default_font(void)
{
    /* crate font */
    if (g_camera_scene_dsc->fontdes > 0)
    {
        return false;
    }

    g_camera_scene_dsc->fontdes = gui_create_font((unsigned char*)sys_get_default_font_file(), 16);
    if (g_camera_scene_dsc->fontdes < 0)
    {
        print_err("call gui_create_font failed");
        return false;
    }

    if (gui_dc_set_default_fontface(g_camera_scene_dsc->fontdes) < 0)
    {
        print_err("gui_dc_set_default_fontface failed");
        return false;
    }

    return true;
}

static bool _dsc_unset_default_font(void)
{
    /* crate font */
    if (g_camera_scene_dsc->fontdes <= 0)
    {
        return false;
    }

    gui_destroy_font(g_camera_scene_dsc->fontdes);
    g_camera_scene_dsc->fontdes = 0;

    return true;
}

/* scene initial function */
static bool _dsc_init(void)
{
    bool result = false;
    camera_scene_dsc_t *data;
    cam_dialog_e dialog_str_type_temp = CAM_DIALOG_OPEN_FAIL ;
    int ret = 0;
    canvas_set_blending_mode(true);
    g_dsc_quiting = false;
    /*! 分配场景数据空间 */
    if (g_camera_scene_dsc == NULL)
    {
        g_camera_scene_dsc = malloc(sizeof(camera_scene_dsc_t));

        if (g_camera_scene_dsc == NULL)
        {
            print_err();
            return false;
        }
        else
        {
            memset(g_camera_scene_dsc, 0, sizeof(camera_scene_dsc_t)); //clear
            g_camera_scene_dsc->blending.left_section.creat.handle = -1;
            g_camera_scene_dsc->blending.slider_section.creat.handle = -1;
            g_camera_scene_dsc->blending.bottom_section.creat.handle = -1;
            g_camera_scene_dsc->blending.focus_section.creat.handle = -1;
            g_camera_scene_dsc->blending.menu_section.creat.handle = -1;
            g_camera_scene_dsc->blending.volbar_section.creat.handle = -1;
            g_camera_scene_dsc->blending.dialog_section.creat.handle = -1;
            g_camera_scene_dsc->blending.adjust_section.creat.handle = -1;
            g_camera_scene_dsc->blending.msgbox_section.creat.handle = -1;

            data = g_camera_scene_dsc;
        }
    }
    else
    {
        data = g_camera_scene_dsc;
    }

    data->need_exit = FALSE;
    data->exit_type = CAM_SCENE_EXIT;

    if (_dsc_open_theme() == false)
    {
        print_err("_dsc_open_theme failed\n");
        goto open_theme_err;
    }

    if (_dsc_open_str_res() == false)
    {
        print_err("_dsc_open_str_res failed\n");
        goto open_str_err;
    }

    if (_dsc_set_default_font() == false)
    {
        print_err("_dsc_set_default_font failed\n");
        goto create_font_err;
    }

    /* 获取背景图资源 */
    //data->dsc_bg = (picture_resource_t *) get_scene_child(data->scene, DSC_PIC_BG );

    /* 设置GUI宽高 */
    gui_set_dimension(data->scene->width, data->scene->height);

    if (_dsc_ui_load_style(data) == false)
    {
        print_err("_dsc_ui_load_style failed\n");
        goto load_resource_err;
    }
    print_dbg("windows. w=%d.h=%d\n",data->scene->width,data->scene->height);
    data->win_id = gui_wm_create_window(data->scene->x, data->scene->y, data->scene->width, data->scene->height,
            WM_CREATE_SHOW, _dsc_callback, 0);
    if (data->win_id == 0)
    {
        print_err("call gui_wm_create_window failed\n");
        goto create_window_err;
    }

    data->hdc = gui_dc_get(data->win_id);
    ///*目前gui做法，对0层不能有任何操作,否则死机*/gui_dc_clear(data->hdc);

    gui_wm_set_focus(data->win_id);

    if(_dsc_is_tvout_on() == true)
    {
        print_err("TV OUT is on,can't into CAMERA!\n");
        dialog_str_type_temp = CAM_DIALOG_TVOUT_OPENED;
        goto camera_open_err;
    }

    /*============================== */
    /*! 1,取配置 */
    if (camera_dsc_init_all_cfg() == false)
    {
        print_err("camera_dsc_cfg_read_all failed\n");
        goto read_cfg_err;
    }

    /*! 2,解大头贴 ,解过之后才能开dsc，因为app_no*/
    if (camera_dsc_sticker_photo_decode_init() == false)
    {
        print_err("camera_dsc_sticker_photo_decode_init failed\n");
        goto decode_photo_err;
    }

    /*! 3,设置参数 */

    ret = camera_dsc_mmm_open();
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
        print_err("camera_dsc_mmm_open failded\n");
        goto camera_open_err;
    }

    if (camera_dsc_msgbox_create() == false)
    {
        print_err("camera_dsc_msgbox_create failed\n");
        goto create_msgbox_err;
    }

    if (camera_dsc_sticker_photo_change(g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_STICKERPHOTO].value) == false)
    {
        print_err("camera_dsc_sticker_photo_change failed");
        goto change_photo_err;
    }

    /*! 4,设置全局参数 */
    {
        g_camera_scene_dsc->dsc_config.cfg_adj_cont_shooting = 0;
        g_camera_scene_dsc->dsc_config.cfg_adj_self_time = 0;
        g_camera_scene_dsc->dsc_config.cfg_zoom_level.denominator = 10;
        g_camera_scene_dsc->dsc_config.cfg_zoom_level.numerator = 10;
    }

    if(g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value == CAM_DSC_STORAGE_NONE)
    {
        hotplugin_dev_type_e main_disk = sys_get_main_disk();
        if(main_disk != HOTPLUGIN_DEV_TYPE_NONE)
        {
            g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value = (int)main_disk;
            //result = camera_dsc_check_path(main_disk);
        }
        else
        {
            camera_dsc_dialog_create(CAM_DIALOG_NODISK);
        }
    }
    else
    {
        //result = camera_dsc_check_path((hotplugin_dev_type_e)g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value);
    }
    // TODO:recheck result=?
    /*! 5,开始预览 */
    if (camera_dsc_ctr_start_preview() == false)
    {
        print_err("camera_dsc_ctr_start_preview failed\n");
        goto start_preview_err;
    }

    /*! 6,初始化场景有那个blending（common单独处理） */
    result = camera_dsc_blending_init();
    if (result == false)
    {
        print_err("camera_dsc_blending_init err\n");
        goto create_blending_err;
    }

    camera_dsc_ui_init(&data->dsc_ui_icon);
    g_camera_scene_dsc->blending.left_section.change_show_status = true;
    g_camera_scene_dsc->blending.left_section.show = true;
    g_camera_scene_dsc->blending.bottom_section.change_show_status = true;
    g_camera_scene_dsc->blending.bottom_section.show = true;
    g_camera_scene_dsc->blending.slider_section.change_show_status = true;
    g_camera_scene_dsc->blending.slider_section.show = true;
    g_camera_scene_dsc->blending.focus_section.change_show_status = true;
    g_camera_scene_dsc->blending.focus_section.show = true;
    camera_scene_dsc_paint(data);

    if (_dsc_ui_blending() == false)
    {
        print_err("_dsc_ui_blending failed\n");
        goto show_blending_err;
    }

    /*! 初始化拍照音 */
    shutter_sound_init();
    if(shutter_sound_change() == false)
    {
        print_err("shutter sound init failed\n");
        goto show_blending_err;
    }
    /*============================== */
    /*! 如果保存的是空，但主盘不为空要切换为主盘 */


    /*回放修改为退出应用，回放结束的时候要回到菜单下通过这里判断*/
    if (g_option_flag == true)
    {
        g_option_flag = false;
        if (data->opt == NULL)
        {
            print_dbg();
            s_shortcut_playback = false;
            camera_dsc_widget_option_create(_dsc_widget_option_callback);
            if (data->opt == NULL)
            {
                print_err();
                //return false;
            }
        }
    }

    clear_key_msg_queue(-1, -1);
    //todo:check clear msg 时机

    return true;

    show_blending_err: camera_dsc_blending_deinit();

    create_blending_err: camera_dsc_ctr_stop_preview();

    start_preview_err: change_photo_err: camera_dsc_msgbox_delete();

    create_msgbox_err: camera_dsc_mmm_close();

    camera_open_err: canvas_set_blending_mode(false);
    gui_dc_clear(g_camera_scene_dsc->hdc );

    camera_dsc_dialog_create(dialog_str_type_temp);
    data->need_exit = true;
    return true;
    #if 0
    // TODO:语句不可达，不同的错误用errno来定，最后的地方弹出msg
    camera_dsc_sticker_photo_decode_deinit();
    #endif
    decode_photo_err: read_cfg_err: gui_wm_delete_window(data->win_id);
    data->win_id = 0;

    create_window_err: load_resource_err:
    _dsc_unset_default_font();

    create_font_err: _dsc_close_str_res();

    open_str_err: _dsc_close_theme();

    open_theme_err: return false;

}

/* scene exit function */
static bool _dsc_deinit(void)
{
    camera_scene_dsc_t *data = g_camera_scene_dsc;
    //这里的顺序是不可以随便更改的
    s_shortcut_playback = false;
    if (s_dsc_deinited == true)
    {
        return true;
    }

    camera_dsc_ctr_stop_preview();

    camera_dsc_msgbox_delete();


    camera_dsc_blending_deinit();

    shutter_sound_deinit();

    camera_dsc_deinit_all_cfg();

    camera_dsc_sticker_photo_decode_deinit();

    camera_dsc_mmm_close();

    canvas_set_blending_mode(false);


    if (data->win_id != 0)
    {
        gui_wm_delete_window(data->win_id);
        data->win_id = (int) NULL;
    }

    _dsc_unset_default_font();

    if (_dsc_close_theme() == false)
    {
        return false;
    }

    if (_dsc_close_str_res() == false)
    {
        return false;
    }

    return true;

}
/* scene msg process */


static int _dsc_proc_gui(camera_scene_dsc_t *data, gui_wm_message_t *pmsg)
{
    int ret = 0;
    key_event_t *kmsg;
    unsigned int kact;
    bool result = false;

    //printf("g_do_snapshotting = %d quit= %d\n",g_do_snapshotting,g_dsc_quiting);
    if ((g_do_snapshotting == true) || (g_dsc_quiting == true))
    {
        return true;
    }
    switch (pmsg->msgid)
    {
        case WM_MSG_CREATE:
        break;

        case WM_MSG_KEY:
        kmsg = (key_event_t *) (pmsg->data.p);
        kact = (unsigned int) (kmsg->val | kmsg->type);

        /*! menu弹出不响应按键 */
        if (data->opt != NULL)
        {
            gui_wm_default_callback(pmsg);
            break;
        }

        /*! 自拍连拍中只响应ok和esc键盘 */
        if (((s_cap_selftimer_ing != 0) || (s_cap_conttimer_ing != 0)) && \
        ((kact != (KEY_OK | KEY_TYPE_SHORT_UP)) && (kact
                != (KEY_RETURN | KEY_TYPE_DOWN))))
        {
            print_info("self time and no ok esc key.......\n");
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
                    _dsc_volumebar_create();
                    print_dbg();
                }
            }
            break;

            case KEY_PREV | KEY_TYPE_DOWN:
            {
                data->exit_type = CAM_SCENE_DV;
                data->need_exit = false;
                _dsc_quit();
            }
            break;


#if (SHORTCUT_PLAYBACK_DSC == 1)
            case KEY_PLAYBACK | KEY_TYPE_DOWN:
            {
                print_err("playback shot in\n");
                if (camera_dsc_playback() == false)
                {
                    break;
                }
                s_shortcut_playback = true;
                clear_key_msg_queue(-1, -1);
            }
            break;
#endif /* SHORTCUT_PLAYBACK_DSC */

#if (SHORTCUT_CAMERA_DSC == 1)
            case KEY_CAMERA | KEY_TYPE_DOWN:
            print_err("camera shot in\n");
#endif /* SHORTCUT_CAMERA_DSC */

            case KEY_OK | KEY_TYPE_SHORT_UP:
            {

                result = \
                    camera_dsc_check_path((hotplugin_dev_type_e)g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value);
                // TODO:result must use bool
                if(result == false)
                {
                    //检测路径已经提示这里不管了
                    //camera_dsc_dialog_create(CAM_DIALOG_DISKINSERT);
                    break;
                }

                /*! 拍前直接判断数值，拍后要读取中间件剩余空间 */
                if (g_camera_scene_dsc->dsc_config.cfg_freeinfo.info <= CAM_THRESHOLD_SPACE)
                {
                    camera_dsc_kill_timer(data);
                    camera_dsc_warning_nospace();
                    break;
                }

                /*! 如果电压低于门限，禁止拍照，但仍可预览 */
                if( sys_power_threshold_check(POWER_THRESHOLD_DC_REC) == false)
                {
                    camera_dsc_kill_timer(data);
                    camera_dsc_dialog_create(CAM_DIALOG_NOPOWER);
                    gui_screen_update();
                    break;
                }
                sys_forbid_auto_standby(true);
                /*! 连怕和自拍相互斥，只有一种存在 */
                if (g_camera_scene_dsc->dsc_config.cfg_adj_self_time != 0)
                {
                    if (s_cap_selftimer_ing == 0)
                    {
                        /*! ok按下自拍开始计时 */
                        s_cap_selftimer_ing = 1;
                        data->dsc_selftimer_id = set_timer(\
                            (unsigned int) (g_camera_scene_dsc->dsc_config.cfg_adj_self_time * 5000),\
                            (timer_proc) _dsc_selftimer_event, (void *) data);

                        /*! msg:启动2个定时器，一个闪烁一个自拍，防止累计时间误差 */
                        s_icon_flicker_ing = 1;
                        data->dsc_icon_flicker_id = set_timer(\
                        500,\
                        (timer_proc) _dsc_icon_flicker_event, (void *) data);
                    }
                    else
                    {
                        /*! 自拍中，ok会清除当次自拍时间，但不会清除自拍功能 */
                        restart_timer(data->dsc_selftimer_id);
                    }
                }
                else if (g_camera_scene_dsc->dsc_config.cfg_adj_cont_shooting != 0)
                {
                    if (s_cap_conttimer_ing == 0)
                    {
                        /*! ok按下自拍开始计时 */
                        s_cap_conttimer_ing = 1;
                        data->dsc_conttimer_id = set_timer(\
                            (unsigned int) (g_camera_scene_dsc->dsc_config.cfg_adj_cont_shooting * 5000),\
                            (timer_proc) _dsc_conttimer_event, (void *) data);

                        s_icon_flicker_ing = 1;
                        data->dsc_icon_flicker_id = set_timer(\
                        500,\
                        (timer_proc) _dsc_icon_flicker_event, (void *) data);
                    }
                    else
                    {
                        /*! 连拍中，ok会清除当次自拍时间，但不会清除自拍功能 */
                        restart_timer(data->dsc_conttimer_id);
                    }
                }
                else
                {
                    //正常拍照
                    camera_dsc_snapshot();//结束如果不足空间提示
                    return true;
                }
            }
            clear_key_msg_queue(-1, -1);
            break;

            case KEY_NEXT | KEY_TYPE_DOWN:
            {
                camera_dsc_ctr_zoom();
            }
            break;

            case KEY_MENU | KEY_TYPE_DOWN:
            {
                print_dbg();
                if (data->opt == NULL)
                {
                    s_shortcut_playback = false;
                    camera_dsc_widget_option_create(_dsc_widget_option_callback);
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
                if (data->opt == NULL)
                {
                    print_info("key esc in....\n\n");
                    /*! 自拍连拍中esc会停止和清除自拍连拍功能 */
                    if ((s_cap_conttimer_ing != 0) || (s_cap_selftimer_ing != 0))
                    {
                        camera_dsc_kill_timer(data);
                    }
                    else
                    {
                        data->exit_type = CAM_SCENE_EXIT;
                        _dsc_quit();
                    }
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
    //动作之后清理按键消息

    return ret;
}

/* scene callback function */
static void _dsc_callback(gui_wm_message_t *pmsg)
{
    int top;

    top = canvas_get_blending_focus(pmsg);
    //这里在删除不同控件的时候要把对应指针设为NULL，否则删除有可能还出现问题
    //dispatch msg
    if ((g_camera_scene_dsc->vbar != NULL) && (top == g_camera_scene_dsc->vbar->canvas_id))
    {
        volumebar_proc_gui(g_camera_scene_dsc->vbar, pmsg);
    }
    else if ((g_camera_scene_dsc->dialog != NULL) && (top == g_camera_scene_dsc->dialog->canvas_id))
    {
        dialog_proc_gui(g_camera_scene_dsc->dialog, pmsg);
    }
    else if ((g_camera_scene_dsc->opt != NULL) && (top == g_camera_scene_dsc->opt->canvas_id))
    {
        menuview_proc_gui(g_camera_scene_dsc->opt, pmsg);
    }
    else if ((g_camera_scene_dsc->adjust != NULL) && (top == g_camera_scene_dsc->adjust->canvas_id))
    {
        print_info("bf proc gui adjust....2\n");
        adjust_proc_gui(g_camera_scene_dsc->adjust, pmsg);
    }
    else if ((g_camera_scene_dsc->msgbox != NULL) && (top == g_camera_scene_dsc->msgbox->canvas_id))
    {
        /* 处理默认系统消息(低电提示、卡插入和拔出提示、USB插入功能选择提示等)的msgbox控件需要接受GUI消息 */
        msgbox_proc_gui(g_camera_scene_dsc->msgbox, pmsg);
    }
    else
    {
        _dsc_proc_gui(g_camera_scene_dsc, pmsg);
    }

}
bool camera_dsc_quit_do(void)
{
    //print_err("quit do\n");    
    if((s_cap_selftimer_ing != 0) || (s_cap_conttimer_ing != 0))
    {
        camera_dsc_kill_timer(g_camera_scene_dsc);
    }

    exit_msg_loop();
    g_camera_scene_dsc->exit_type = CAM_SCENE_EXIT;
    return true;
}
static int _dsc_sys_proc(msg_apps_t *msg)
{
    msg_reply_t t;
    print_err("msg.type:%d\n", msg->type);
    switch (msg->type)
    {
        case MSG_ACTIVE_SYNC:
        t.type = MSG_REPLY_SUCCESS;
        reply_sync_msg(&t);
        break;

        #if 0   //改用退出应用，不再用到
        case MSG_SET_APP_FOREGROUND:
        print_info("MSG_SET_APP_FOREGROUND\n");
        enter_foreground_mode();
        gui_wm_set_focus(g_camera_scene_dsc->win_id);

        /* 设置屏幕方向 */
        sty_set_screen_rotation(0);

        g_camera_scene_dsc->hdc = gui_dc_get(g_camera_scene_dsc->win_id);
        {
            bool result;
            int ret = 0;
            cam_dialog_e dialog_str_type_temp = CAM_DIALOG_OPEN_FAIL ;

            canvas_set_blending_mode(true);

            ret = camera_dsc_mmm_open();
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
                print_err("camera_dsc_mmm_open failded\n");
                goto camera_open_err;
            }

            if (camera_dsc_msgbox_create() == false)
            {
                print_err("camera_dsc_msgbox_create failed\n");
                goto create_msgbox_err;
            }

            if (camera_dsc_sticker_photo_change(g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_STICKERPHOTO].value) == false)
            {
                print_err("camera_dsc_sticker_photo_change failed");
                goto change_photo_err;
            }


            if (camera_dsc_ctr_start_preview() == false)
            {
                print_err("camera_dsc_ctr_start_preview failed\n");
                goto start_preview_err;
            }

            result = camera_dsc_blending_init();
            if (result == false)
            {
                print_err("camera_dsc_blending_init err\n");
                goto create_blending_err;
            }

            camera_dsc_ui_init(&g_camera_scene_dsc->dsc_ui_icon);
            g_camera_scene_dsc->blending.left_section.change_show_status = true;
            g_camera_scene_dsc->blending.left_section.show = true;
            g_camera_scene_dsc->blending.bottom_section.change_show_status = true;
            g_camera_scene_dsc->blending.bottom_section.show = true;
            g_camera_scene_dsc->blending.slider_section.change_show_status = true;
            g_camera_scene_dsc->blending.slider_section.show = true;
            g_camera_scene_dsc->blending.focus_section.change_show_status = true;
            g_camera_scene_dsc->blending.focus_section.show = true;
            camera_scene_dsc_paint(g_camera_scene_dsc);

            if (_dsc_ui_blending() == false)
            {
                print_err("_dsc_ui_blending failed\n");
                goto show_blending_err;
            }
            if (s_shortcut_playback == false)
            {
                camera_dsc_widget_option_create(_dsc_widget_option_callback);
            }
            else
            {
                s_shortcut_playback = false;
            }
            if (g_camera_scene_dsc->opt == NULL)
            {
                print_err();
            }

            //shutter_sound_init();
            //shutter_sound_change();
            clear_key_msg_queue(-1, -1);

            return 0;

            show_blending_err: camera_dsc_blending_deinit();

            create_blending_err: camera_dsc_ctr_stop_preview();

            start_preview_err: change_photo_err: camera_dsc_msgbox_delete();

            create_msgbox_err: camera_dsc_mmm_close();

            camera_open_err: canvas_set_blending_mode(false);
            camera_dsc_dialog_create(dialog_str_type_temp/*CAM_DIALOG_INIT_SENSOR_FAIL*/);
            g_camera_scene_dsc->need_exit = true;
            return 0;
            #if 0
            gui_screen_update();
            sleep(3);
            camera_dsc_dialog_delete();

            camera_dsc_sticker_photo_decode_deinit();
            //decode_photo_err: read_cfg_err:
            gui_wm_delete_window(g_camera_scene_dsc->win_id);
            g_camera_scene_dsc->win_id = 0;

            //create_window_err: load_resource_err:
            _dsc_unset_default_font();

            //create_font_err:
            _dsc_close_str_res();

            //open_str_err:
            _dsc_close_theme();

            //open_theme_err:
            s_dsc_deinited = true;
            exit_msg_loop();
            g_camera_scene_dsc->exit_type = CAM_SCENE_EXIT;
            #endif
        }

        //break; //不可达
        #endif

#if ((SHORTCUT_CAMERA_DSC == 1) || (SHORTCUT_CAMERA_DV == 1))
        case MSG_SHORTCUT_CAMERA:
        print_err("camera in\n");
        break;
#endif /* SHORTCUT_CAMERA_DSC */

#if ((SHORTCUT_PLAYBACK_DSC == 1) || (SHORTCUT_PLAYBACK_DV == 1))
        case MSG_SHORTCUT_PLAYBACK:
        print_err("playback in\n");
        break;
#endif /* SHORTCUT_PLAYBACK_DSC */

        case MSG_APP_QUIT:
        //print_err("quit \n");
        g_dsc_quiting = true;
        if(g_do_snapshotting == true)
        {
            break; 
        }
        //print_err("quit 2\n");

        camera_dsc_quit_do();
    
        break;

        default:
        {
            printf("default msg\n");
            /*main_disk会在确认后才发card_out等消息,所有在这之后获取主盘是可以的*/
            if (msg->type == MSG_USB_HOST_DISK_OUT)
            {
                if(g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value == CAM_DSC_STORAGE_EXTERNAL)
                {
                    if((s_cap_selftimer_ing != 0) || (s_cap_conttimer_ing != 0))
                    {
                        camera_dsc_kill_timer(g_camera_scene_dsc);
                    }
                    if(camera_dsc_mmm_opening_mobile_storage_out() == false)
                    {
                        print_err("dsc card out err\n");
                    }
                }
            }
            else if (msg->type == MSG_CARD_OUT)
            {
                if(g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SAVE_PATH].value == CAM_DSC_STORAGE_CARD)
                {
                    if((s_cap_selftimer_ing != 0) || (s_cap_conttimer_ing != 0))
                    {
                        camera_dsc_kill_timer(g_camera_scene_dsc);
                    }
                    if(camera_dsc_mmm_opening_mobile_storage_out() == false)
                    {
                        print_err("dsc card out err\n");
                    }
                }
            }
            else
            {

            }
            if ((msg->type == MSG_CARD_IN) || (msg->type == MSG_USB_HOST_DISK_IN) || \
                (msg->type == MSG_USB_HOST_DISK_OUT) || (msg->type == MSG_CARD_OUT))
            {
                if(g_camera_scene_dsc->opt != NULL)
                {
                    camera_dsc_option_savepath_refresh();
                    camera_dsc_option_free_space_refresh();
                }
            }
            else
            {
                //nor flash 时候因为会回desktop所以不用管，在nand下需要做处理改变工作盘
            }

            /* break;next will go msgbox */

            if (g_camera_scene_dsc->msgbox != NULL)
            {
                if(msg->type == MSG_LOW_VOLTAGE)
                {
                    if(g_camera_scene_dsc->dialog != NULL)/*自己的低电弹出框 exist*/
                    {
                        if(s_dialog_str_type == CAM_DIALOG_NOPOWER)
                        {
                            break;
                        }
                    }
                }
                /* 控件msgbox用来处理默认系统消息(低电提示、卡插入和
                拔出提示、USB插入功能选择提示等) */
                msgbox_proc_sys(g_camera_scene_dsc->msgbox, msg);
            }
            break;
        }
    }
    return 0;
}

/* scene entry function */
int camera_scene_dsc(void)
{
    bool ret;
    msg_apps_t msg;
    app_cam_scene_type_e exit_type_temp;

    ret = _dsc_init();
    if (ret == false)
    {
        print_err("scene init error! \n");
        return -1;
    }
    register_sys_dispatcher(_dsc_sys_proc);
    while ((get_msg(&msg)) == true)
    {
        dispatch_msg(&msg);
    }
    unregister_sys_dispatcher();

    ret = _dsc_deinit();
    s_dsc_deinited = false;
    exit_type_temp = g_camera_scene_dsc->exit_type;
    free(g_camera_scene_dsc);
    g_camera_scene_dsc = NULL;

    if (ret == false)
    {
        print_err("scene exit error! \n");
        return -1;
    }
    return exit_type_temp ;
}

