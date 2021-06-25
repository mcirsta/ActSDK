/*
 *********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : calendar_scene.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *     dupengcheng    2009-04-28          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     calendar_scene.c
 * \brief    calendar 场景相关
 * \author   dupengcheng
 *
 * \version 1.0
 * \date  2009/04/14
 *******************************************************************************/
#include "calendar_scene.h"
#include "calendar_play.h"
#include "calendar_ui.h"


extern style_t* app_style;
extern style_t* commonui_style;
scene_calendar_t* g_scene_calendar = NULL;


static void _calendar_scene_quit(void);
static int _proc_sys_msg(msg_apps_t *msg);
static bool _calendar_scene_init(void);
static bool _calendar_scene_deinit(void);

static void _scene_proc_gui(gui_wm_message_t* pmsg);
static bool _scene_proc_key(key_event_t *key_event);

static bool _calendar_scene_init_paint(scene_calendar_t *my_scene);
static bool _calendar_rotate_paint(scene_calendar_t *my_scene);

static bool _create_headbar(void);
static bool _delete_headbar(void);
static void _headbar_cb(headbar_t *headbar, int signal);

static void _calendar_scene_callback(gui_wm_message_t *pmsg);

static bool _scene_calendar_open_theme(void);
static bool _scene_calendar_close_theme(void);
static bool _scene_calendar_open_str_res(void);
static bool _scene_calendar_close_str_res(void);
static bool _scene_calendar_load_resources(void);
static bool _scene_calendar_rotate(void);

static bool _create_msgbox(void);
static void _msgbox_cb(msgbox_t * msgbox, msgbox_sig_e signal);
static bool _delete_msgbox(void);

static bool _create_volumebar(void);
static void _volumebar_cb(volumebar_t *volumebar, volumebar_sig_e signal);
static bool _delete_volumebar(void);

static bool _scene_set_default_font(void);
static bool _scene_unset_default_font(void);

static void _scene_fusion_set_first_frame(scene_t *scene);
static void _scene_fusion_set_last_frame(scene_t *scene);



static bool _scene_set_default_font(void)
{
    /* crate font */
    if (g_scene_calendar->fontdes > 0)
    {
        return false;
    }

    g_scene_calendar->fontdes = gui_create_font(sys_get_default_font_file(), 16);
    if (g_scene_calendar->fontdes < 0)
    {
        print_err("call gui_create_font failed");
        return false;
    }

    if (gui_dc_set_default_fontface(g_scene_calendar->fontdes) < 0)
    {
        print_err("gui_dc_set_default_fontface failed");
        return false;
    }

    return true;
}

static bool _scene_unset_default_font(void)
{
    /* crate font */
    if (g_scene_calendar->fontdes <= 0)
    {
        return false;
    }

    gui_destroy_font(g_scene_calendar->fontdes);
    g_scene_calendar->fontdes = 0;

    return true;
}

/* scene paint function */
static bool _calendar_scene_init_paint(scene_calendar_t *my_scene)
{
    /* draw static main window bg */
    //sty_draw_picture_res( my_scene->scene_common.handle_gui_dc, my_scene->bg_pic);
    sty_draw_bitmap(my_scene->scene_common.handle_gui_dc, my_scene->bg_bitmap_info, 0, 0);
    sty_draw_picture_res(my_scene->scene_common.handle_gui_dc, my_scene->bigarea_pic);
    sty_draw_picture_res(my_scene->scene_common.handle_gui_dc, my_scene->area_pic);
    sty_draw_picture_res(my_scene->scene_common.handle_gui_dc, my_scene->week_pic);

    sty_draw_picture_res(my_scene->scene_common.handle_gui_dc, my_scene->left_pic);
    sty_draw_picture_res(my_scene->scene_common.handle_gui_dc, my_scene->right_pic);
    sty_draw_picture_res(my_scene->scene_common.handle_gui_dc, my_scene->line1_pic);
    sty_draw_picture_res(my_scene->scene_common.handle_gui_dc, my_scene->line2_pic);

    /*得到界面中各个日期方格的坐标，用gui_rect_t表示*/
    get_date_panes_coordinate();

    /* 画当前的日期及其相关信息 */
    calendar_load(my_scene);

    //print_info("scene first paint end");
    return true;
}

/* paint when rotate */
static bool _calendar_rotate_paint(scene_calendar_t *my_scene)
{
    /* draw static main window bg */
    sty_draw_bitmap(my_scene->scene_common.handle_gui_dc, my_scene->bg_bitmap_info, 0, 0);
    sty_draw_picture_res(my_scene->scene_common.handle_gui_dc, my_scene->bigarea_pic);
    sty_draw_picture_res(my_scene->scene_common.handle_gui_dc, my_scene->area_pic);
    sty_draw_picture_res(my_scene->scene_common.handle_gui_dc, my_scene->week_pic);

    sty_draw_picture_res(my_scene->scene_common.handle_gui_dc, my_scene->left_pic);
    sty_draw_picture_res(my_scene->scene_common.handle_gui_dc, my_scene->right_pic);
    sty_draw_picture_res(my_scene->scene_common.handle_gui_dc, my_scene->line1_pic);
    sty_draw_picture_res(my_scene->scene_common.handle_gui_dc, my_scene->line2_pic);

    /*得到界面中各个日期方格的坐标，用gui_rect_t表示*/
    get_date_panes_coordinate();

    /* 重画所有 */
    rePaintWindowArea(my_scene);

    //print_info("scene first paint end");
    return true;
}


static void _headbar_cb(headbar_t *headbar, int signal)
{
    switch (signal)
    {
        case KEY_RETURN | KEY_TYPE_DOWN:
        //print_dbg();
        break;
        default:
        break;
    }
}

static bool _create_headbar(void)
{
    headbar_init_t headbar_init;

    headbar_init.type = HEADBAR_TYPE_NORMAL;
    headbar_init.str_id_title = g_scene_calendar->title_str->str_id;
    headbar_init.pic_id_head = g_scene_calendar->icon_pic->pic_id;

    /* init type and callback function */
    g_scene_calendar->headbar = headbar_create(&headbar_init, _headbar_cb);
    if (NULL == g_scene_calendar->headbar)
    {
        print_err("create headbar failed!");
        return false;
    }

    headbar_paint(g_scene_calendar->headbar);

    return true;
}

static bool _delete_headbar(void)
{
    bool result = true;

    result = headbar_delete(g_scene_calendar->headbar);
    g_scene_calendar->headbar = NULL;

    return result;
}

static bool _create_msgbox(void)
{
    g_scene_calendar->msgbox = msgbox_create(_msgbox_cb);
    if (NULL == g_scene_calendar->msgbox)
    {
        print_err("create msgbox failed!");
        return false;
    }

    return true;
}

static void _msgbox_cb(msgbox_t * msgbox, msgbox_sig_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_MSGBOX_SHOW:
        //print_dbg("show message box");
        break;
        case WIDGET_SIG_MSGBOX_QUIT:
        //print_dbg("quit message box");
        break;
        default:
        break;
    }
}

static bool _delete_msgbox(void)
{
    msgbox_delete(g_scene_calendar->msgbox);
    g_scene_calendar->msgbox = NULL;
    return true;
}

static bool _create_volumebar(void)
{
    if (g_scene_calendar->volumebar != NULL)
    {
        //print_warning("volumebar is already exist!");
        return false;
    }

    g_scene_calendar->volumebar = volumebar_create(0, (volumebar_cb_t) _volumebar_cb);
    if (g_scene_calendar->volumebar == NULL)
    {
        print_err("create volumebar error");
    }
    else
    {
        volumebar_paint(g_scene_calendar->volumebar);
    }
    return true;
}

static void _volumebar_cb(volumebar_t *volumebar, volumebar_sig_e signal)
{
    switch (signal)
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

static bool _delete_volumebar(void)
{
    if (g_scene_calendar->volumebar != NULL)
    {
        volumebar_delete(g_scene_calendar->volumebar);
        g_scene_calendar->volumebar = NULL;
    }

    return true;
}

static bool _scene_proc_key(key_event_t * key_event)
{
    bool result = false;

    switch (key_event->val | key_event->type)
    {
        case KEY_VOL | KEY_TYPE_DOWN:
        case KEY_VSUB | KEY_TYPE_DOWN:
        case KEY_VADD | KEY_TYPE_DOWN:
        _create_volumebar();
        result = true;
        break;

        case KEY_OK | KEY_TYPE_DOWN:
        case KEY_MENU | KEY_TYPE_DOWN:
        case KEY_NEXT | KEY_TYPE_DOWN:
        case KEY_NEXT | KEY_TYPE_LONG:        
        case KEY_NEXT | KEY_TYPE_HOLD:        
        case KEY_PREV | KEY_TYPE_DOWN:
        case KEY_PREV | KEY_TYPE_LONG:        
        case KEY_PREV | KEY_TYPE_HOLD:        
        calendar_key_mamage(g_scene_calendar, key_event);
        result = true;
        clear_key_msg_queue(-1, -1);
        break;

        case KEY_RETURN | KEY_TYPE_SHORT_UP:
        case KEY_RETURN | KEY_TYPE_LONG_UP:
        case KEY_RETURN | KEY_TYPE_HOLD_UP:
        _calendar_scene_quit();
        result = true;
        clear_key_msg_queue(-1, -1);
        break;

        default:
        break;
    }
    return result;
}

static void _scene_proc_gui(gui_wm_message_t* pmsg)
{
    switch (pmsg->msgid)
    {
        case WM_MSG_CREATE:
        //print_dbg("WM_MSG_CREATE");
        break;
        case WM_MSG_KEY:
        _scene_proc_key((key_event_t *) (pmsg->data.p));
        break;
        default:
        gui_wm_default_callback(pmsg);
        break;
    }
}

/* scene callback function */
static void _calendar_scene_callback(gui_wm_message_t *pmsg)
{
    int top;

    top = canvas_get_focus(pmsg);

    if ((g_scene_calendar->msgbox != NULL) && (top == g_scene_calendar->msgbox->canvas_id))
    {
        /* 处理默认系统消息(低电提示、卡插入和拔出提示、USB插入功能选择提示等)的msgbox控件需要接受GUI消息 */
        msgbox_proc_gui(g_scene_calendar->msgbox, pmsg);
    }
    else if ((NULL != g_scene_calendar->volumebar) && (top == g_scene_calendar->volumebar->canvas_id))
    {
        volumebar_proc_gui(g_scene_calendar->volumebar, pmsg);
    }
    else
    {
        _scene_proc_gui(pmsg);
    }
    //clear_key_msg_queue(-1, -1);
}

/* 获取背景图资源 */
static bool _scene_calendar_load_resources(void)
{
    scene_calendar_t *my_scene = g_scene_calendar;

    my_scene->bg_pic = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PIC_BG );
    my_scene->icon_pic = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PIC_ICON );
    my_scene->area_pic = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PIC_AREA );
    my_scene->week_pic = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PIC_WEEK );
    my_scene->bigarea_pic = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PIC_BIGAREA );

    my_scene->left_pic = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PIC_LEFT);
    my_scene->right_pic = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PIC_RIGHT);
    my_scene->line1_pic = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PIC_LINE1);
    my_scene->line2_pic = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PIC_LINE2);

    my_scene->numArryW_pic = (picregion_resource_t *) get_scene_child(my_scene->scene_common.scene, PICREG_NUM_W);
    my_scene->numArryY_pic = (picregion_resource_t *) get_scene_child(my_scene->scene_common.scene, PICREG_NUM_Y);

    my_scene->shield_pic = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PIC_SHIELD);
    my_scene->info_str = (string_resource_t *) get_scene_child(my_scene->scene_common.scene, STR_DATEINFO);
    my_scene->title_str = (string_resource_t *) get_scene_child(my_scene->scene_common.scene, STR_TITLE);
    my_scene->color_str = (string_resource_t *) get_scene_child(my_scene->scene_common.scene, STR_SEL_COLOR);
    
    my_scene->position1_pic = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PIC_POSITION1);
    my_scene->position2_pic = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PIC_POSITION2);
    my_scene->position3_pic = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PIC_POSITION3);
    my_scene->position4_pic = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PIC_POSITION4);
    my_scene->position5_pic = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PIC_POSITION5);
    my_scene->position6_pic = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PIC_POSITION6);
    my_scene->position7_pic = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PIC_POSITION7);
    my_scene->position8_pic = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PIC_POSITION8);

    my_scene->block_pic = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PIC_BLOCK);
    my_scene->nodisk_str = (string_resource_t *) get_scene_child(my_scene->scene_common.scene, STR_NODISK);
    
    return true;
}

/* scene initial function */
static bool _calendar_scene_init(void)
{
    scene_calendar_t *my_scene;
//    hotplugin_dev_type_e main_disk ; 
    dialog_init_t dialog_init;
    
    //分配场景数据空间
    if (g_scene_calendar == NULL)
    {
        g_scene_calendar = malloc(sizeof(scene_calendar_t));

        if (g_scene_calendar == NULL)
        {
            print_err();
            return false;
        }
        else
        {
            memset(g_scene_calendar, 0, sizeof(scene_calendar_t)); //clear
            my_scene = g_scene_calendar;
        }
    }
    else
    {
        my_scene = g_scene_calendar;
    }

    /* load theme 资源文件 */
    if (_scene_calendar_open_theme() == false)
    {
        return false;
    }

    /* load 字符串资源文件 */
    if (_scene_calendar_open_str_res() == false)
    {
        return false;
    }

    /* 创建字体 */
    if (_scene_set_default_font() == false)
    {
        //print_warning("set font failed!");
        return false;
    }

    /* 获取背景图资源 */
    _scene_calendar_load_resources();

//    //判断主盘是否存在
//    main_disk = sys_get_main_disk();
//    if(main_disk == HOTPLUGIN_DEV_TYPE_NONE)
//    {
//        //模态提示框
//        if (my_scene->nodisk_str != NULL)
//        {
//            dialog_init.head = DIALOG_HEAD_WARN;
//            dialog_init.str_id = my_scene->nodisk_str->str_id;
//            dialog_modal( &dialog_init );
//        }
//        return false;
//    }
    
    g_scene_calendar->scene_common.win_id = gui_wm_create_window(g_scene_calendar->scene_common.scene->x,
            g_scene_calendar->scene_common.scene->y, g_scene_calendar->scene_common.scene->width,
            g_scene_calendar->scene_common.scene->height, WM_CREATE_SHOW, _calendar_scene_callback, 0);
    if (g_scene_calendar->scene_common.win_id == 0)
    {
        print_err("call gui_wm_create_window failed");
        return false;
    }

    //设置焦点
    gui_wm_set_focus(g_scene_calendar->scene_common.win_id);
    g_scene_calendar->scene_common.handle_gui_dc = gui_dc_get((unsigned short)g_scene_calendar->scene_common.win_id);

    g_scene_calendar->bg_bitmap_info = get_user_bg_pic(BG_CALENDAR);
    if (NULL == g_scene_calendar->bg_bitmap_info)
    {
        g_scene_calendar->bg_bitmap_info = sty_create_bitmap((int) g_scene_calendar->bg_pic->pic_id);
    }
    if (NULL == g_scene_calendar->bg_bitmap_info)
    {
        print_err("bg_bitmap_info is NULL. \n");
        return false;
    }

    _scene_fusion_set_first_frame(g_scene_calendar->scene_common.scene);
    
    _calendar_scene_init_paint(g_scene_calendar);

    if (_create_headbar() == false)
    {
        print_err("create headbar failed!");
        return false;
    }

    if (_create_msgbox() == false)
    {
        print_err("create msgbox failed!");
        return false;
    }

    return true;
}

static bool _scene_calendar_open_theme(void)
{
    rotation_e angle;
    char file[512];

    /* 取视图角度 */
    angle = sty_get_view_rotation();

    /* 设置屏幕方向 */
    sty_set_screen_rotation(angle);

    /* 载入commonui的 style 场景 */
    if ((angle == ROTATION_0) || (angle == ROTATION_180))
    {
        g_scene_calendar->scene_common.common_scene = load_scene(commonui_style, COMMONUI_SIDELONG_SCENE);

        g_scene_calendar->scene_common.scene = load_scene(app_style, CALENDAR_SIDELONG_SCENE);
    }
    else //if (angle == ROTATION_90 || angle == ROTATION_270)
    {
        g_scene_calendar->scene_common.common_scene = load_scene(commonui_style, COMMONUI_UPRIGHT_SCENE);

        g_scene_calendar->scene_common.scene = load_scene(app_style, CALENDAR_UPRIGHT_SCENE);
    }

    if ((g_scene_calendar->scene_common.common_scene == NULL) || (g_scene_calendar->scene_common.scene == NULL))
    {
        print_err("load commonui scene failed");
        return false;
    }
    else
    {
        sty_set_common_scene(g_scene_calendar->scene_common.common_scene);
        sty_set_app_scene(g_scene_calendar->scene_common.scene);
    }

    /* 获取commonui 的图片资源文件名 */
    sprintf(file, "%s/%s.res", sys_get_common_theme_res_dir(), "commonui");
    if (sty_open_common_pic_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }

    /* 获取commonui 的图片资源文件名 */
    sprintf(file, "%s/%s.res", sys_get_app_theme_res_dir(), "calendar");
    if (sty_open_app_pic_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }

    return true;
}

static bool _scene_calendar_close_theme(void)
{
    free_scene(g_scene_calendar->scene_common.scene);
    g_scene_calendar->scene_common.scene = NULL;

    free_scene(g_scene_calendar->scene_common.common_scene);
    g_scene_calendar->scene_common.common_scene = NULL;

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

static bool _scene_calendar_open_str_res(void)
{
    char file[512];

    // 获取commonui 字符串资源文件名
    sprintf(file, "%s/%s_%s.str", sys_get_common_string_res_dir(), "commonui", sys_get_cur_language_codes());
    //print_dbg("load %s", file);
    // 打开 commonui 字符串资源文件
    if (sty_open_common_str_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }

    // 获取setting应用字符串资源文件名
    sprintf(file, "%s/%s_%s.str", sys_get_app_string_res_dir(), "calendar", sys_get_cur_language_codes());
    //print_dbg("load %s", file);
    // 打开应用的字符串资源文件
    if (sty_open_app_str_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }

    // 设置语言编码
    sty_set_text_encoding(GUI_UTF8_ENC);
    //gui_set_language_mode(MLANG_ZH_CN);
    gui_set_language_mode(sys_get_cur_language_id());
}

static bool _scene_calendar_close_str_res(void)
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

/* scene exit function */
static bool _calendar_scene_deinit(void)
{
    bool ret = true;

    if (NULL != g_scene_calendar->bg_bitmap_info)
    {
        sty_delete_bitmap(g_scene_calendar->bg_bitmap_info);
        g_scene_calendar->bg_bitmap_info = NULL;
    }

    if (_delete_headbar() == false)
    {
        //print_warning("delete headbar failed!");
        ret = false;
    }

    if (_delete_volumebar() == false)
    {
        //print_warning("delete volumebar failed!");
        ret = false;
    }

    if (_delete_msgbox() == false)
    {
        //print_warning("delete msgbox failed!");
        ret = false;
    }

    /* delete scene window */
    if (gui_wm_delete_window(g_scene_calendar->scene_common.win_id) < 0)
    {
        //print_warning("delete window failed!");
        ret = false;
    }

    /* destroy font */
    if (false == _scene_unset_default_font())
    {
        //print_warning("unset font failed!");
        ret = false;
    }

    if (_scene_calendar_close_theme() == false)
    {
        ret = false;
    }

    if (_scene_calendar_close_str_res() == false)
    {
        ret = false;
    }
    
  
    free(g_scene_calendar);

    return ret;
}


static void _calendar_scene_quit(void)
{
    exit_msg_loop();
    g_scene_calendar->scene_common.exit_type = CALENDAR_SCENE_RETURN;
}


static bool _scene_calendar_rotate(void)
{
    fusion_effect_t fusion_effect =
    { 0 };
    
    fusion_get_config(FUSION_TYPE_ROTATE, FUSION_FORWARD, &fusion_effect);
    fusion_set_effect(&fusion_effect);
    fusion_set_first_frame(NULL);

    _scene_calendar_close_theme();
    _scene_calendar_open_theme();
    _scene_calendar_load_resources();

    gui_wm_set_size(g_scene_calendar->scene_common.win_id, g_scene_calendar->scene_common.scene->width,
            g_scene_calendar->scene_common.scene->height);

    if (NULL != g_scene_calendar->bg_bitmap_info)
    {
        sty_delete_bitmap(g_scene_calendar->bg_bitmap_info);
        g_scene_calendar->bg_bitmap_info = NULL;
    }
    g_scene_calendar->bg_bitmap_info = get_user_bg_pic(BG_CALENDAR);
    if (NULL == g_scene_calendar->bg_bitmap_info)
    {
        g_scene_calendar->bg_bitmap_info = sty_create_bitmap((int) g_scene_calendar->bg_pic->pic_id);
    }
    if (NULL == g_scene_calendar->bg_bitmap_info)
    {
        print_err("bg_bitmap_info is NULL. \n");
        return false;
    }

    _calendar_rotate_paint(g_scene_calendar);

    if (g_scene_calendar->headbar != NULL)
    {
        headbar_load_style(g_scene_calendar->headbar);
        headbar_paint(g_scene_calendar->headbar);
    }

    if (g_scene_calendar->volumebar != NULL)
    {
        volumebar_load_style(g_scene_calendar->volumebar);
        volumebar_paint(g_scene_calendar->volumebar);
    }

    if (g_scene_calendar->msgbox != NULL)
    {
        msgbox_load_style(g_scene_calendar->msgbox);
        msgbox_paint(g_scene_calendar->msgbox);
    }

    fusion_set_last_frame(NULL);
    fusion_display(NULL);

    return true;
}

static int _proc_sys_msg(msg_apps_t *msg)
{
    msg_reply_t t;
    //print_dbg("msg.type:%d\n", msg->type);

    switch (msg->type)
    {
        case MSG_ACTIVE_SYNC:
        t.type = MSG_REPLY_SUCCESS;
        reply_sync_msg(&t);
        break;

        case MSG_APP_QUIT:
        exit_msg_loop();
        g_scene_calendar->scene_common.exit_type = CALENDAR_SCENE_RETURN;
        break;

        case MSG_GSENSOR_ROTATION_CHANGED:
        //print_info("received msg: MSG_GSENSOR_ROTATION_CHANGED");
        _scene_calendar_rotate();
        break;

        default:
        if (g_scene_calendar->msgbox != NULL)
        {
            /* 控件msgbox用来处理默认系统消息(低电提示、卡插入和拔出提示、USB插入功能选择提示等) */
            msgbox_proc_sys(g_scene_calendar->msgbox, msg);
        }
        break;
    }
    return 0;
}

/*
 * 设置当前场景全部区域作为特效的第一帧
 */
static void _scene_fusion_set_first_frame(scene_t *scene)
{
    fusion_frame_t frame;
    fusion_effect_t effect;

    fusion_get_config(FUSION_TYPE_APP_CHANGE, FUSION_FORWARD, &effect); 
    fusion_set_effect(&effect);
    
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
static void _scene_fusion_set_last_frame(scene_t *scene)
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

int calendar_scene(fusion_enter_e fenter)
{
    bool ret;
    msg_apps_t msg;

    /* 场景初始化 */
    ret = _calendar_scene_init();
    if (ret == false)
    {
        goto ERROR_EXIT;	
    }
    
    _scene_fusion_set_last_frame(g_scene_calendar->scene_common.scene);
    fusion_display(NULL);

//    if (ret < 0)
//    {
//        print_err("scene_calendar  init error! \n");
//        return -1;
//    }
//    else
//    {
//        //print_info("init ok");
//    }

    /* 场景消息循环 */
    register_sys_dispatcher((sys_msg_proc) _proc_sys_msg);

    while (get_msg(&msg) == true)
    {

        dispatch_msg(&msg);
    }

    unregister_sys_dispatcher();

    /* 场景退出 */
    ERROR_EXIT:
    
    ret = _calendar_scene_deinit();
    if (ret == false)
    {
        print_err("scene exit error! \n");
        return -1;
    }
    return 0;
}

