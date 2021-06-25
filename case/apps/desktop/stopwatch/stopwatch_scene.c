/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : stopwatch_main.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *     dupengcheng    2009-04-28          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     stopwatch_scene.c
 * \brief    stopwatch application
 * \author   dupengcheng
 *
 * \version 1.0
 * \date  2009/04/14
 *******************************************************************************/
#include "stopwatch_scene.h"
#include "stopwatch_ui.h"
#include "stopwatch_ctl.h"

extern int cur_label_idx;
extern int status;
extern watchvalue_t watchs[];
extern watchvalue_t curwatchvalue;
extern int timerEnabled;
extern struct timeval startTicks;
extern style_t* app_style;
extern style_t* commonui_style;


scene_stopwatch_t* g_scene_stopwatch = NULL;
int num_x[8];
bool rotate_change_status;

static bool _scene_watch_open_theme(void);
static bool _scene_watch_close_theme(void);
static bool _scene_watch_open_str_res(void);
static bool _scene_watch_close_str_res(void);
static bool _watch_scene_init(void);
static bool _watch_scene_deinit(void);
static void _watch_scene_quit(void);
static void _watch_scene_callback(gui_wm_message_t *pmsg);
static void _scene_proc_gui(gui_wm_message_t* pmsg);
static bool _scene_proc_key(key_event_t * key_event);
static int _proc_sys_msg(msg_apps_t *msg);
static bool _scene_watch_load_resources(void);

static void _headbar_cb(headbar_t *headbar, int signal);
static bool _create_headbar(void);
static bool _delete_headbar(void);

static bool _create_msgbox(void);
static void _msgbox_cb(msgbox_t * msgbox, msgbox_sig_e signal);
static bool _delete_msgbox(void);

static bool _create_volumebar(void);
static void _volumebar_cb(volumebar_t *volumebar, volumebar_sig_e signal);
static bool _delete_volumebar(void);

static bool _scene_watch_rotate(void);
static bool _get_num_x(void);

static bool _scene_set_default_font(void);
static bool _scene_unset_default_font(void);

static void _scene_fusion_set_first_frame(scene_t *scene);
static void _scene_fusion_set_last_frame(scene_t *scene);

//int stopwatch_scene(fusion_enter_e fenter);
//void watch_scene_init_paint(scene_stopwatch_t *my_scene);
//bool is_upright_disp(void);
//void option_fusion_set_first_frame(menuview_t *mv);
//void option_fusion_set_last_frame(menuview_t *mv);


static bool _scene_set_default_font(void)
{
    /* crate font */
    if (g_scene_stopwatch->fontdes > 0)
    {
        return false;
    }

    g_scene_stopwatch->fontdes = gui_create_font(sys_get_default_font_file(), 16);
    if (g_scene_stopwatch->fontdes < 0)
    {
        //print_err("call gui_create_font failed");
        return false;
    }

    if (gui_dc_set_default_fontface(g_scene_stopwatch->fontdes) < 0)
    {
        //print_err("gui_dc_set_default_fontface failed");
        return false;
    }

    return true;
}

static bool _scene_unset_default_font(void)
{
    /* crate font */
    if (g_scene_stopwatch->fontdes <= 0)
    {
        return false;
    }

    gui_destroy_font(g_scene_stopwatch->fontdes);
    g_scene_stopwatch->fontdes = 0;

    return true;
}

/*判断当前显示方向，竖屏返回true*/
bool is_upright_disp(void)
{
    rotation_e angle;
    bool result = false;

    angle = sty_get_view_rotation();
    if ((angle == ROTATION_90) || (angle == ROTATION_270))
    {
        result = true;
    }
    return result;
}

/*取得当前显示方向下的数字的X坐标*/
static bool _get_num_x(void)
{
    num_x[0] = g_scene_stopwatch->num_pic->x;
    num_x[1] = g_scene_stopwatch->position1_pic->x;
    num_x[2] = g_scene_stopwatch->position2_pic->x;
    num_x[3] = g_scene_stopwatch->position3_pic->x;
    num_x[4] = g_scene_stopwatch->position4_pic->x;
    num_x[5] = g_scene_stopwatch->position5_pic->x;
    num_x[6] = g_scene_stopwatch->position6_pic->x;
    num_x[7] = g_scene_stopwatch->position7_pic->x;

    return true;
}

/* scene init paint function */
void watch_scene_init_paint(scene_stopwatch_t *my_scene)
{
    picregion_frame_t picregion_frame;
    int i;

    _get_num_x(); //取得数字的X坐标

    /* draw static main window bg */
    stopwatch_draw_bg(my_scene);

    sty_draw_picture_res(my_scene->scene_common.handle_gui_dc, my_scene->show_area);

    /* draw watchs's pictures */
    stopwatch_draw_frame(my_scene, 0, 1);
    stopwatch_draw_frame(my_scene, 1, 0);
    stopwatch_draw_frame(my_scene, 2, 0);
    stopwatch_draw_frame(my_scene, 3, 0);

    if (is_upright_disp() == true)
    {
        stopwatch_draw_frame(my_scene, 4, 0);
        stopwatch_draw_frame(my_scene, 5, 0);
    }

    /* draw watch head pictures  画8个数字*/
    get_picregion_frame((picregion_resource_t*)my_scene->num_pic, 0, &picregion_frame);
    for (i = 0; i < 8; i++)
    {
        sty_draw_picture_id(my_scene->scene_common.handle_gui_dc, 
                            (int)picregion_frame.pic_id, num_x[i], picregion_frame.y);
    }

    /* 画三个点：2个':'和1个"." */
    sty_draw_picture_res(my_scene->scene_common.handle_gui_dc, my_scene->dot2_pic);
    sty_draw_picture_res(my_scene->scene_common.handle_gui_dc, my_scene->dot2_2_pic);
    sty_draw_picture_res(my_scene->scene_common.handle_gui_dc, my_scene->dot1_pic);

}

static void _headbar_cb(headbar_t *headbar, int signal)
{
    switch (signal)
    {
        case KEY_RETURN | KEY_TYPE_DOWN:
        break;
        
        default:
        break;
    }

}

static bool _create_headbar(void)
{
    headbar_init_t headbar_init;
    string_resource_t *str_res;
    picture_resource_t *pic_res;

    str_res = (string_resource_t *) get_resgroup_child(g_scene_stopwatch->headbar_resgroup, STR_TITLE);
    pic_res = (picture_resource_t *) get_resgroup_child(g_scene_stopwatch->headbar_resgroup, PIC_ICON);

    if ((str_res == NULL) || (pic_res == NULL))
    {
        return false;
    }
    
    headbar_init.type = HEADBAR_TYPE_NORMAL;
    headbar_init.str_id_title = str_res->str_id;
    headbar_init.pic_id_head = pic_res->pic_id;

    /* init type and callback function */
    g_scene_stopwatch->headbar = headbar_create(&headbar_init, _headbar_cb);
    if (NULL == g_scene_stopwatch->headbar)
    {
        //print_err("create headbar failed!");
        return false;
    }

    headbar_paint(g_scene_stopwatch->headbar);

    return true;
}

static bool _delete_headbar(void)
{
    bool result = true;

    result = headbar_delete(g_scene_stopwatch->headbar);
    g_scene_stopwatch->headbar = NULL;

    return result;
}

static bool _create_msgbox(void)
{
    g_scene_stopwatch->msgbox = msgbox_create(_msgbox_cb);
    if (NULL == g_scene_stopwatch->msgbox)
    {
        //print_err("create msgbox failed!");
        return false;
    }

    return true;
}

static void _msgbox_cb(msgbox_t * msgbox, msgbox_sig_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_MSGBOX_SHOW:
        break;
        
        case WIDGET_SIG_MSGBOX_QUIT:
        break;
        
        default:
        break;
    }
}

static bool _delete_msgbox(void)
{
    msgbox_delete(g_scene_stopwatch->msgbox);
    g_scene_stopwatch->msgbox = NULL;
    return true;
}

static bool _create_volumebar(void)
{
    if (g_scene_stopwatch->volumebar != NULL)
    {
        //print_warning("volumebar is already exist!");
        return false;
    }

    g_scene_stopwatch->volumebar = volumebar_create(0, (volumebar_cb_t) _volumebar_cb);
    if (g_scene_stopwatch->volumebar == NULL)
    {
        //print_err("create volumebar error");
    }
    else
    {
        volumebar_paint(g_scene_stopwatch->volumebar);
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
    if (g_scene_stopwatch->volumebar != NULL)
    {
        volumebar_delete(g_scene_stopwatch->volumebar);
        g_scene_stopwatch->volumebar = NULL;
    }

    return true;
}

static bool _scene_proc_key(key_event_t * key_event)
{
    bool result = false;

    switch (key_event->val | key_event->type)
    {
        case KEY_MENU | KEY_TYPE_DOWN:
        watch_menu(g_scene_stopwatch);
        clear_key_msg_queue(-1, -1);
        result = true;
        break;

        case KEY_OK | KEY_TYPE_DOWN:
        startCurWatch(g_scene_stopwatch);
        clear_key_msg_queue(-1, -1);
        result = true;
        break;

        case KEY_NEXT | KEY_TYPE_DOWN:
        startNextWatch(g_scene_stopwatch);
        clear_key_msg_queue(-1, -1);
        result = true;
        break;

        case KEY_VOL | KEY_TYPE_DOWN:
        case KEY_VSUB | KEY_TYPE_DOWN:
        case KEY_VADD | KEY_TYPE_DOWN:
        _create_volumebar();
        result = true;
        break;

        case KEY_RETURN | KEY_TYPE_SHORT_UP:
        case KEY_RETURN | KEY_TYPE_LONG_UP:
        case KEY_RETURN | KEY_TYPE_HOLD_UP:
        _watch_scene_quit();
        clear_key_msg_queue(-1, -1);
        result = true;
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
static void _watch_scene_callback(gui_wm_message_t *pmsg)
{
    int top;

    /* top canvas process */
    top = canvas_get_focus(pmsg);

    if ((NULL != g_scene_stopwatch->dialog) && (top == g_scene_stopwatch->dialog->canvas_id))
    {
        dialog_proc_gui(g_scene_stopwatch->dialog, pmsg);
    }
    else if ((NULL != g_scene_stopwatch->volumebar) && (top == g_scene_stopwatch->volumebar->canvas_id))
    {
        volumebar_proc_gui(g_scene_stopwatch->volumebar, pmsg);
    }
    else if ((g_scene_stopwatch->msgbox != NULL) && (top == g_scene_stopwatch->msgbox->canvas_id))
    {
        /* 处理默认系统消息(低电提示、卡插入和拔出提示、USB插入功能选择提示等)的msgbox控件需要接受GUI消息 */
        msgbox_proc_gui(g_scene_stopwatch->msgbox, pmsg);
    }
    else
    {
        _scene_proc_gui(pmsg);
    }
    //clear_key_msg_queue(-1, -1);
}

static bool _scene_watch_load_resources(void)
{
    scene_stopwatch_t *my_scene = g_scene_stopwatch;

    my_scene->bg_pic = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PIC_BG );
    my_scene->show_area = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PIC_AREA );
    my_scene->cover_pic = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PIC_COVER );
    my_scene->watch1.pic_res = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PICREGION1 );
    my_scene->watch2.pic_res = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PICREGION2 );
    my_scene->watch3.pic_res = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PICREGION3 );
    my_scene->watch4.pic_res = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PICREGION4 );
    my_scene->watch5.pic_res = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PICREGION5 );
    my_scene->watch6.pic_res = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PICREGION6 );

    my_scene->watch1.str_res = (string_resource_t *) get_scene_child(my_scene->scene_common.scene, STRING1);
    my_scene->watch2.str_res = (string_resource_t *) get_scene_child(my_scene->scene_common.scene, STRING2);
    my_scene->watch3.str_res = (string_resource_t *) get_scene_child(my_scene->scene_common.scene, STRING3);
    my_scene->watch4.str_res = (string_resource_t *) get_scene_child(my_scene->scene_common.scene, STRING4);
    my_scene->watch5.str_res = (string_resource_t *) get_scene_child(my_scene->scene_common.scene, STRING5);
    my_scene->watch6.str_res = (string_resource_t *) get_scene_child(my_scene->scene_common.scene, STRING6);

    my_scene->watch1.str_id_res = (string_resource_t *) get_scene_child(my_scene->scene_common.scene, STR_IDX1);
    my_scene->watch2.str_id_res = (string_resource_t *) get_scene_child(my_scene->scene_common.scene, STR_IDX2);
    my_scene->watch3.str_id_res = (string_resource_t *) get_scene_child(my_scene->scene_common.scene, STR_IDX3);
    my_scene->watch4.str_id_res = (string_resource_t *) get_scene_child(my_scene->scene_common.scene, STR_IDX4);
    my_scene->watch5.str_id_res = (string_resource_t *) get_scene_child(my_scene->scene_common.scene, STR_IDX5);
    my_scene->watch6.str_id_res = (string_resource_t *) get_scene_child(my_scene->scene_common.scene, STR_IDX6);

    my_scene->num_pic = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PICREGION_SHOW_NUM);
    my_scene->position1_pic = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PIC_POSITION1);
    my_scene->position2_pic = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PIC_POSITION2);
    my_scene->position3_pic = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PIC_POSITION3);
    my_scene->position4_pic = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PIC_POSITION4);
    my_scene->position5_pic = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PIC_POSITION5);
    my_scene->position6_pic = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PIC_POSITION6);
    my_scene->position7_pic = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PIC_POSITION7);

    my_scene->dot2_pic = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PIC_DOT2);
    my_scene->dot2_2_pic = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PIC_DOT2_2);
    my_scene->dot1_pic = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PIC_DOT1);

    my_scene->headbar_resgroup=(resgroup_resource_t*)get_scene_child(my_scene->scene_common.scene,RESGROUP_HEADBAR);
    my_scene->restart_str = (string_resource_t *) get_scene_child(my_scene->scene_common.scene, STR_RESTART);
    my_scene->nodisk_str = (string_resource_t *) get_scene_child(my_scene->scene_common.scene, STR_NODISK);    
    
    return true;
}

/* scene initial function */
static bool _watch_scene_init(void)
{
    scene_stopwatch_t *my_scene;
    int i;
//    hotplugin_dev_type_e main_disk ; 
    dialog_init_t dialog_init;

    /* 分配场景数据空间 */
    if (g_scene_stopwatch == NULL)
    {
        g_scene_stopwatch = malloc(sizeof(scene_stopwatch_t));

        if (g_scene_stopwatch == NULL)
        {
            //print_err();
            return false;
        }
        else
        {
            memset(g_scene_stopwatch, 0, sizeof(scene_stopwatch_t)); //clear
            my_scene = g_scene_stopwatch;
        }
    }
    else
    {
        my_scene = g_scene_stopwatch;
    }

    /* load theme 资源文件 */
    if (_scene_watch_open_theme() == false)
    {
        return false;
    }

    /* load 字符串资源文件 */
    if (_scene_watch_open_str_res() == false)
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
    _scene_watch_load_resources();

    /* 初始化变量 */
    cur_label_idx = 0;
    status = STATE_STOP;
    timerEnabled = 0;
    memset(&startTicks, 0, sizeof(struct timeval));
    memset(&curwatchvalue, 0, sizeof(watchvalue_t));
    for (i = 0; i < MAX_WATCH; i++)
    {
        memset(&watchs[i], 0, sizeof(watchvalue_t));
    }

    /* 设置字体风格,颜色,大小 */
    gui_dc_set_text_mode(my_scene->scene_common.handle_gui_dc, GUI_DRAW_MODE_TRANS);
    gui_dc_set_font_size(my_scene->scene_common.handle_gui_dc, 20);
    gui_dc_set_pen_size(my_scene->scene_common.handle_gui_dc, 2);

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

    g_scene_stopwatch->scene_common.win_id = gui_wm_create_window(g_scene_stopwatch->scene_common.scene->x,
            g_scene_stopwatch->scene_common.scene->y, g_scene_stopwatch->scene_common.scene->width,
            g_scene_stopwatch->scene_common.scene->height, WM_CREATE_SHOW, _watch_scene_callback, 0);
    if (g_scene_stopwatch->scene_common.win_id == 0)
    {
        //print_err("call gui_wm_create_window failed");
        return false;
    }

    /* 设置焦点 */
    gui_wm_set_focus(g_scene_stopwatch->scene_common.win_id);
    g_scene_stopwatch->scene_common.handle_gui_dc = gui_dc_get((unsigned short)g_scene_stopwatch->scene_common.win_id);

    g_scene_stopwatch->bg_bitmap_info = get_user_bg_pic(BG_STOPWATCH);
    if (NULL == g_scene_stopwatch->bg_bitmap_info)
    {
        /*获取用户自定义背景图失败，使用ui-editor上添加的默认图*/
        g_scene_stopwatch->bg_bitmap_info = sty_create_bitmap((int) g_scene_stopwatch->bg_pic->pic_id);
    }
    if (NULL == g_scene_stopwatch->bg_bitmap_info)
    {
        //print_err("bg_bitmap_info is NULL. \n");
        return false;
    }

    _scene_fusion_set_first_frame(g_scene_stopwatch->scene_common.scene);
    
    watch_scene_init_paint(g_scene_stopwatch);

    if (_create_headbar() == false)
    {
        //print_err("create headbar failed!");
        return false;
    }

    if (_create_msgbox() == false)
    {
        //print_err("create msgbox failed!");
        return false;
    }

    return true;
}

static bool _scene_watch_open_theme(void)
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
        g_scene_stopwatch->scene_common.common_scene = load_scene(commonui_style, COMMONUI_SIDELONG_SCENE);

        g_scene_stopwatch->scene_common.scene = load_scene(app_style, STOPWATCH_SIDELONG_SCENE);
    }
    else //if ((angle == ROTATION_90) || (angle == ROTATION_270))
    {
        g_scene_stopwatch->scene_common.common_scene = load_scene(commonui_style, COMMONUI_UPRIGHT_SCENE);

        g_scene_stopwatch->scene_common.scene = load_scene(app_style, STOPWATCH_UPRIGHT_SCENE);
    }

    if ((g_scene_stopwatch->scene_common.common_scene == NULL) || (g_scene_stopwatch->scene_common.scene == NULL))
    {
        //print_err("load commonui scene failed");
        return false;
    }
    else
    {
        sty_set_common_scene(g_scene_stopwatch->scene_common.common_scene);
        sty_set_app_scene(g_scene_stopwatch->scene_common.scene);
    }

    /* 获取commonui 的图片资源文件名 */
    sprintf(file, "%s/%s.res", sys_get_common_theme_res_dir(), "commonui");
    if (sty_open_common_pic_res(file) == false)
    {
        //print_err("open %s failed!", file);
        return false;
    }

    /* 获取commonui 的图片资源文件名 */
    sprintf(file, "%s/%s.res", sys_get_app_theme_res_dir(), "watch");
    if (sty_open_app_pic_res(file) == false)
    {
        //print_err("open %s failed!", file);
        return false;
    }

    return true;
}

static bool _scene_watch_close_theme(void)
{
    free_scene(g_scene_stopwatch->scene_common.scene);
    g_scene_stopwatch->scene_common.scene = NULL;

    free_scene(g_scene_stopwatch->scene_common.common_scene);
    g_scene_stopwatch->scene_common.common_scene = NULL;

    if (sty_close_common_pic_res() == false)
    {
        //print_err("close common picture resource error!");
        return false;
    }

    if (sty_close_app_pic_res() == false)
    {
        //print_err("close app picture resource error!");
        return false;
    }

    return true;
}

static bool _scene_watch_open_str_res(void)
{
    char file[512];

    /* 获取commonui 字符串资源文件名 */
    sprintf(file, "%s/%s_%s.str", sys_get_common_string_res_dir(), "commonui", sys_get_cur_language_codes());
    print_dbg("load %s", file);
    /* 打开 commonui 字符串资源文件 */
    if (sty_open_common_str_res(file) == false)
    {
        //print_err("open %s failed!", file);
        return false;
    }

    /* 获取setting应用字符串资源文件名 */
    sprintf(file, "%s/%s_%s.str", sys_get_app_string_res_dir(), "watch", sys_get_cur_language_codes());
    
    /* 打开应用的字符串资源文件 */
    if (sty_open_app_str_res(file) == false)
    {
        //print_err("open %s failed!", file);
        return false;
    }

    /* 设置语言编码 */
    sty_set_text_encoding(GUI_UTF8_ENC);
    gui_set_language_mode(sys_get_cur_language_id());
}

static bool _scene_watch_close_str_res(void)
{
    if (sty_close_common_str_res() == false)
    {
        //print_err("close common string resource error!");
        return false;
    }

    if (sty_close_app_str_res() == false)
    {
        //print_err("close app string resource error!");
        return false;
    }

    return true;
}

/* scene exit function */
static bool _watch_scene_deinit(void)
{
    bool ret = true;

    if (NULL != g_scene_stopwatch->bg_bitmap_info)
    {
        sty_delete_bitmap(g_scene_stopwatch->bg_bitmap_info);
        g_scene_stopwatch->bg_bitmap_info = NULL;
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
    if (gui_wm_delete_window(g_scene_stopwatch->scene_common.win_id) < 0)
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

    if (_scene_watch_close_theme() == false)
    {
        ret = false;
    }

    if (_scene_watch_close_str_res() == false)
    {
        ret = false;
    }

    free(g_scene_stopwatch);

    return ret;
}

static void _watch_scene_quit(void)
{
    msg_apps_t m;
    m.type = MSG_APP_QUIT;
    send_async_msg("watch.app", &m);
}

static bool _scene_watch_rotate(void)
{
    fusion_effect_t fusion_effect =
    { 0 };
    
    fusion_get_config(FUSION_TYPE_ROTATE, FUSION_FORWARD, &fusion_effect);
    fusion_set_effect(&fusion_effect);    
    fusion_set_first_frame(NULL);

    _scene_watch_close_theme();
    _scene_watch_open_theme();
    _scene_watch_load_resources();
    _get_num_x(); //取得数字的X坐标

    gui_wm_set_size(g_scene_stopwatch->scene_common.win_id, g_scene_stopwatch->scene_common.scene->width,
            g_scene_stopwatch->scene_common.scene->height);

    if (NULL != g_scene_stopwatch->bg_bitmap_info)
    {
        sty_delete_bitmap(g_scene_stopwatch->bg_bitmap_info);
        g_scene_stopwatch->bg_bitmap_info = NULL;
    }
    g_scene_stopwatch->bg_bitmap_info = get_user_bg_pic(BG_STOPWATCH);
    if (NULL == g_scene_stopwatch->bg_bitmap_info)
    {
        g_scene_stopwatch->bg_bitmap_info = sty_create_bitmap((int) g_scene_stopwatch->bg_pic->pic_id);
    }
    if (NULL == g_scene_stopwatch->bg_bitmap_info)
    {
        return false;
    }

    paint_when_rotate();

    if (g_scene_stopwatch->headbar != NULL)
    {
        headbar_load_style(g_scene_stopwatch->headbar);
        headbar_paint(g_scene_stopwatch->headbar);
    }

    if (g_scene_stopwatch->volumebar != NULL)
    {
        volumebar_load_style(g_scene_stopwatch->volumebar);
        volumebar_paint(g_scene_stopwatch->volumebar);
    }

    if (g_scene_stopwatch->dialog != NULL)
    {
        dialog_load_style(g_scene_stopwatch->dialog);
        dialog_paint(g_scene_stopwatch->dialog);
    }

    if (g_scene_stopwatch->msgbox != NULL)
    {
        msgbox_load_style(g_scene_stopwatch->msgbox);
        msgbox_paint(g_scene_stopwatch->msgbox);
    }

    fusion_set_last_frame(NULL);
    fusion_display(NULL);

    return true;
}

static int _proc_sys_msg(msg_apps_t *msg)
{
    msg_reply_t t;
    switch (msg->type)
    {
        case MSG_ACTIVE_SYNC:
        t.type = MSG_REPLY_SUCCESS;
        reply_sync_msg(&t);
        break;

        case MSG_APP_QUIT:
        exit_msg_loop();
        g_scene_stopwatch->scene_common.exit_type = WATCH_SCENE_RETURN;
        break;

        case MSG_GSENSOR_ROTATION_CHANGED:
        rotate_change_status = true;
        _scene_watch_rotate();
        break;

        default:
        if (g_scene_stopwatch->msgbox != NULL)
        {
            /* 控件msgbox用来处理默认系统消息(低电提示、卡插入和拔出提示、USB插入功能选择提示等) */
            msgbox_proc_sys(g_scene_stopwatch->msgbox, msg);
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

///*
// * Option菜单特效第一帧
// */
//void option_fusion_set_first_frame(menuview_t *mv)
//{
//    fusion_frame_t frame;
//    canvas_info_t info;
//
//    canvas_get_info(mv->canvas_id, &info);
//
//    frame.rect.x = info.rect.x;
//    frame.rect.y = info.rect.y;
//    frame.rect.width = info.rect.width;
//    frame.rect.height = info.rect.height;
//    frame.pdata = NULL;
//    frame.alpha = false;
//    fusion_set_first_frame(&frame);
//}
//
///*
// * Option菜单特效最后一帧
// */
//void option_fusion_set_last_frame(menuview_t *mv)
//{
//    fusion_frame_t frame;
//    canvas_info_t info;
//
//    canvas_get_info(mv->canvas_id, &info);
//
//    frame.rect.x = info.rect.x;
//    frame.rect.y = info.rect.y;
//    frame.rect.width = info.rect.width;
//    frame.rect.height = info.rect.height;
//    frame.pdata = (unsigned char *)info.pdata;
//    frame.alpha = true;
//    fusion_set_last_frame(&frame);
//}


int stopwatch_scene(fusion_enter_e fenter)
{
    bool ret;
    msg_apps_t msg;

    /* 场景初始化 */
    ret = _watch_scene_init();
    if (ret == false)
    {
        goto ERROR_EXIT;	
    }
    
    _scene_fusion_set_last_frame(g_scene_stopwatch->scene_common.scene);
    fusion_display(NULL);

//    if (ret < 0)
//    {
//        return -1;
//    }


    /* 场景消息循环 */
    g_scene_stopwatch->timer_id = set_timer(1, onTimer, (void *) g_scene_stopwatch);

    /* 场景消息循环 */
    register_sys_dispatcher((sys_msg_proc) _proc_sys_msg);

    while (true == get_msg(&msg))
    {

        dispatch_msg(&msg);
    }

    unregister_sys_dispatcher();

    /* 场景退出 */  
    kill_timer(g_scene_stopwatch->timer_id);
    
    ERROR_EXIT:
    
    ret = (int)_watch_scene_deinit();
    if (ret == false)
    {
        return -1;
    }
    return 0;
}

