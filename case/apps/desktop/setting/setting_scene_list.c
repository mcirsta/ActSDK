/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 * file : setting_scene_list.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-5-4             v1.0             build this file
 ********************************************************************************/

#include "setting_scene_list.h"
#include "setting_list_widget_menu.h"

static bool _scene_list_init(void);
static bool _scene_list_deinit(void);
static void _scene_list_quit(void);

static void _scene_list_callback(gui_wm_message_t *pmsg);
static int  _scene_list_proc_sys(msg_apps_t *msg);
static bool _scene_list_proc_gui(gui_wm_message_t* pmsg);
static bool _scene_list_proc_key(key_event_t *key_event);

static bool _scene_list_open_style_files(void);
static bool _scene_list_close_style_files(void);
static bool _scene_list_open_theme(void);
static bool _scene_list_close_theme(void);
static bool _scene_list_open_str_res(void);
static bool _scene_list_close_str_res(void);
static bool _scene_list_set_default_font(void);
static bool _scene_list_unset_default_font(void);
static bool _scene_list_load_resources(void);
static bool _scene_list_get_bg_bitmap(void);
static bool _scene_list_delete_bg_bitmap(void);
static bool _scene_list_rotate(void);
static bool _scene_list_reload_theme(void);
static bool _scene_list_reload_widget_style(void);
static bool _scene_list_paint_bg(void);
static bool _scene_list_paint(void);


static bool _create_volumebar(void);
static void _volumebar_cb( volumebar_t *volumebar, volumebar_sig_e signal );
static bool _delete_volumebar(void);

static bool _create_headbar(headbar_type_e type);
static void _headbar_cb(headbar_t *headbar, widget_sig_headbar_e signal);
static bool _delete_headbar(void);

static bool _create_widget_menu(void);
static void _widget_menu_cb( menuview_t * menuview, menu_sig_e signal);
static bool _delete_widget_menu(void);

static bool _create_msgbox(void);
static void _msgbox_cb( msgbox_t * msgbox, msgbox_sig_e signal);
static bool _delete_msgbox(void);


static void _scene_list_fusion_set_first_frame(void);
static void _scene_list_fusion_set_last_frame(void);

scene_list_t *g_scene_list = NULL;

static bool _scene_list_init(void)
{
    fusion_effect_t fusion_effect = {0};

    fusion_get_config(FUSION_TYPE_APP_CHANGE, FUSION_FORWARD, &fusion_effect);
    fusion_set_effect(&fusion_effect);

    /* open style files*/
    if(_scene_list_open_style_files() == false)
    {
        goto open_style_files_err;
    }

    /* open theme resouce */
    if(_scene_list_open_theme() == false)
    {
        goto open_theme_err;
    }

    /* open string resouce file */
    if(_scene_list_open_str_res() == false)
    {
        goto open_str_err;
    }

    if(_scene_list_set_default_font() == false)
    {
        goto set_font_err;
    }

    if(_scene_list_load_resources() == false)
    {
        goto load_resource_err;
    }

    /* create scene window */
    g_scene_list->win_id = gui_wm_create_window(g_scene_list->scene->x, g_scene_list->scene->y, \
            g_scene_list->scene->width, g_scene_list->scene->height,
            WM_CREATE_SHOW, _scene_list_callback, 0);
    if (g_scene_list->win_id == 0)
    {
        print_err("call gui_wm_create_window failed");
        goto create_window_err;
    }

    gui_wm_set_focus(g_scene_list->win_id);

    _scene_list_fusion_set_first_frame();

    _scene_list_paint();

    /* load config */
    setting_cfg_init(g_setting_cfgs, SET_CFG_TYPE_MAX);
    if(setting_cfg_read_all(g_setting_cfgs, SET_CFG_TYPE_MAX) == false)
    {
        print_err("read config failed!");
        goto read_cfg_err;
    }

    /* create widgets */
    if(_create_headbar(HEADBAR_TYPE_NORMAL) == false)
    {
        goto create_headbar_err;
    }

    if(_create_widget_menu() == false)
    {
        goto create_widget_menu_err;
    }

    if(_create_msgbox() == false)
    {
        goto create_msgbox_err;
    }

    _scene_list_fusion_set_last_frame();

    fusion_display( NULL);

    clear_key_msg_queue(-1,-1);

    return true;

    /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< goto! <<<<<<<<<<<<<<<< goto! <<<<<<<< goto! <<<< goto! << */
    create_msgbox_err:
    _delete_widget_menu();

    /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< goto! <<<<<<<<<<<<<<<< goto! <<<<<<<< goto! <<<< goto! << */
    create_widget_menu_err:
    _delete_headbar();

    /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< goto! <<<<<<<<<<<<<<<< goto! <<<<<<<< goto! <<<< goto! << */
    create_headbar_err:
    
    /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< goto! <<<<<<<<<<<<<<<< goto! <<<<<<<< goto! <<<< goto! << */
    read_cfg_err:
    gui_wm_delete_window(g_scene_list->win_id);

    /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< goto! <<<<<<<<<<<<<<<< goto! <<<<<<<< goto! <<<< goto! << */
    create_window_err:
    _scene_list_unset_default_font();

    /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< goto! <<<<<<<<<<<<<<<< goto! <<<<<<<< goto! <<<< goto! << */
    load_resource_err:

    /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< goto! <<<<<<<<<<<<<<<< goto! <<<<<<<< goto! <<<< goto! << */
    set_font_err:
    _scene_list_close_str_res();

    /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< goto! <<<<<<<<<<<<<<<< goto! <<<<<<<< goto! <<<< goto! << */
    open_str_err:
    _scene_list_close_theme();

    /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< goto! <<<<<<<<<<<<<<<< goto! <<<<<<<< goto! <<<< goto! << */
    open_theme_err:
    _scene_list_close_style_files();

    /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< goto! <<<<<<<<<<<<<<<< goto! <<<<<<<< goto! <<<< goto! << */
    open_style_files_err:

    return false;

}

static bool _scene_list_deinit(void)
{
    bool ret = true;

    /* delete widgets */
    if(_delete_volumebar() == false)
    {
        print_warning("delete volumebar failed!");
        ret = false;
    }

    if(_delete_widget_menu() == false)
    {
        print_warning("delete widget menu failed!");
        ret = false;
    }

    if(_delete_headbar() == false)
    {
        print_warning("delete headbar failed!");
        ret = false;
    }

    if(_delete_msgbox() == false)
    {
        print_warning("delete msgbox failed!");
        ret = false;
    }

    /* delete scene window */
    if(gui_wm_delete_window(g_scene_list->win_id) < 0)
    {
        print_warning("delete window failed!");
        ret = false;
    }

    /* destroy font */
    _scene_list_unset_default_font();

    /* close theme resource file */
    if(_scene_list_close_theme() == false)
    {
        print_warning("close theme failed!");
        ret = false;
    }

    /* close style files */
    if(_scene_list_close_style_files() == false)
    {
        print_warning("close style files failed!");
        ret = false;
    }

    /* close string resource file */
    if(_scene_list_close_str_res() == false)
    {
        print_warning("close string resource failed!");
        ret = false;
    }

    /* delete backgroud bitmap */
    if(_scene_list_delete_bg_bitmap() == false)
    {
        print_warning("delete backgroud bitmap failed!");
        ret = false;
    }

    /* save config */
    if(setting_cfg_write_all(g_setting_cfgs, SET_CFG_TYPE_MAX) == false )
    {
        print_warning("write config failed!");
        ret = false;
    }

    setting_cfg_deinit(g_setting_cfgs, SET_CFG_TYPE_MAX);

    g_scene_list->exit_type = SCENE_EXIT;

    return ret;
}

static void _scene_list_quit(void)
{
    exit_msg_loop();
}

static void _scene_list_callback(gui_wm_message_t *pmsg)
{
    int top = canvas_get_focus(pmsg);

    if(_scene_list_proc_gui(pmsg) == true)
    {
        /* 场景处理过后，不需要传递给控件 */
        return ;
    }


    //dispatch msg
    if ( (g_scene_list->menu != NULL) && (top == g_scene_list->menu->canvas_id) )   //menuview process
    {
        setting_list_widget_menu_proc_gui(g_scene_list->menu, pmsg );
    }

    if( (g_scene_list->adjust != NULL) && (top == g_scene_list->adjust->canvas_id) )
    {
        adjust_proc_gui(g_scene_list->adjust, pmsg);
    }

    if( (g_scene_list->softkeyboard != NULL) && (top == g_scene_list->softkeyboard->canvas_id) )
    {
        softkeyboard_proc_gui(g_scene_list->softkeyboard, pmsg);
    }

    if( (g_scene_list->dialog != NULL) && (top == (g_scene_list->dialog->canvas_id)) )
    {
        dialog_proc_gui(g_scene_list->dialog, pmsg);
    }

    if( (g_scene_list->volumebar != NULL) && (top == g_scene_list->volumebar->canvas_id) )
    {
        volumebar_proc_gui(g_scene_list->volumebar, pmsg);
    }

    if( (g_scene_list->progress_box != NULL) && (top == g_scene_list->progress_box->canvas_id) )
    {
        progress_box_proc_gui(g_scene_list->progress_box, pmsg);
    }

    if( (g_scene_list->loadingbox != NULL) && (top == g_scene_list->loadingbox->canvas_id) )
    {
        loadingbox_proc_gui(g_scene_list->loadingbox, pmsg);
    }

    if( (g_scene_list->msgbox != NULL) && (top == g_scene_list->msgbox->canvas_id) )
    {
        /* 处理默认系统消息(低电提示、卡插入和拔出提示、USB插入功能选择提示等)的msgbox控件需要接受GUI消息 */
        msgbox_proc_gui(g_scene_list->msgbox, pmsg);
    }

}

static int _scene_list_proc_sys(msg_apps_t *msg)
{
    msg_reply_t reply;

    print_info("msgtype:%d, content[0]:0x%x", msg->type, msg->content[0]);
    switch (msg->type)
    {
        case MSG_ACTIVE_SYNC:
        {
            reply.type = MSG_REPLY_SUCCESS;
            reply_sync_msg(&reply);
            break;
        }

        case MSG_APP_QUIT:
        {
            g_scene_list->exit_type = SCENE_EXIT;
            _scene_list_quit();
            break;
        }

        case MSG_SET_APP_FOREGROUND:
        {
            enter_foreground_mode();
            gui_wm_set_focus(g_scene_list->win_id);
            _scene_list_paint();

            break;
        }

        case MSG_GSENSOR_ROTATION_CHANGED:
        {
            print_info("received msg: MSG_GSENSOR_ROTATION_CHANGED");
            _scene_list_rotate();
            break;
        }

        case MSG_RESET_DEFAULT_CFG:
        {
            char last_scr_mode[128] = {0};
            char last_tvout_mode[128] = {0};

            strcpy(last_scr_mode, g_setting_cfgs[SET_CFG_TYPE_SCR_MODE].text);
            strcpy(last_tvout_mode, g_setting_cfgs[SET_CFG_TYPE_TVOUT].text);

            setting_cfg_read_all(g_setting_cfgs, SET_CFG_TYPE_MAX);
            strcpy(g_setting_cfgs[SET_CFG_TYPE_TVOUT].text, last_tvout_mode);

            fusion_update_config();

            _scene_list_close_str_res();
            _scene_list_open_str_res();

            _scene_list_unset_default_font();
            _scene_list_set_default_font();

            setting_list_widget_menu_update_minor_info(g_scene_list->menu);

            if( (strcmp(last_scr_mode, g_setting_cfgs[SET_CFG_TYPE_SCR_MODE].text) != 0 )
                    && (strcmp(g_setting_cfgs[SET_CFG_TYPE_SCR_MODE].text, "Auto") !=0 ) )
            {
                _scene_list_rotate();
            }

            _scene_list_reload_theme();
            _scene_list_paint();

            break;
        }
        default:
        {
            if(g_scene_list->msgbox != NULL)
            {
                /* 控件msgbox用来处理默认系统消息(低电提示、卡插入和拔出提示、USB插入功能选择提示等) */
                msgbox_proc_sys(g_scene_list->msgbox, msg);
            }
            break;
        }
    }

    return 0;
}

static bool _scene_list_proc_gui(gui_wm_message_t* pmsg)
{
    bool result = false;
    switch (pmsg->msgid)
    {
        case WM_MSG_CREATE:
        {
            print_dbg("WM_MSG_CREATE");
            break;
        }
        case WM_MSG_KEY:
        {
            result = _scene_list_proc_key((key_event_t *)(pmsg->data.p));
            break;
        }
        default:
        {
            gui_wm_default_callback(pmsg);
            break;
        }
    }

    return result;
}

static bool _scene_list_proc_key(key_event_t * key_event)
{
    bool result = false;
    switch ( key_event->val | key_event->type )
    {
        case KEY_VOL | KEY_TYPE_DOWN:
        case KEY_VSUB | KEY_TYPE_DOWN:
        case KEY_VADD | KEY_TYPE_DOWN:
        {
            if(g_scene_list->volumebar == NULL)
            {
                _create_volumebar();
                result = true;
            }
            break;
        }

        default:
        {
            break;
        }
    }

    return result;
}

static bool _scene_list_open_style_files(void)
{
    char file[512] = {0};

    // 获取commonui 的 style 资源文件名
    sprintf(file, "%s/%s.sty", sys_get_common_theme_res_dir(), "commonui");
    g_scene_list->commonui_style = open_style_file(file);
    if(g_scene_list->commonui_style == NULL)
    {
        print_err("open style file %s failed!",file);
        goto open_com_style_err;
    }

    // 获取app 的 style 资源文件名
    sprintf(file, "%s/%s.sty", sys_get_app_theme_res_dir(), "setting");
    g_scene_list->app_style = open_style_file(file);
    if(g_scene_list->app_style == NULL)
    {
        print_err("open style file %s failed!",file);
        goto open_app_style_err;
    }

    // 获取commonui 的图片资源文件名
    sprintf(file, "%s/%s.res", sys_get_common_theme_res_dir(), "commonui");
    if(sty_open_common_pic_res(file) == false)
    {
        print_err("open %s failed!", file);
        goto open_com_res_err;
    }

    // 获取app 的图片资源文件名
    sprintf(file, "%s/%s.res", sys_get_app_theme_res_dir(), "setting");
    if(sty_open_app_pic_res(file) == false)
    {
        print_err("open %s failed!", file);
        goto open_app_res_err;
    }

    return true;

    /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< goto! <<<<<<<<<<<<<<<< goto! <<<<<<<< goto! <<<< goto! << */
    open_app_res_err:
    sty_close_common_pic_res();

    /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< goto! <<<<<<<<<<<<<<<< goto! <<<<<<<< goto! <<<< goto! << */
    open_com_res_err:
    close_style_file(g_scene_list->app_style);
    g_scene_list->app_style = NULL;

    /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< goto! <<<<<<<<<<<<<<<< goto! <<<<<<<< goto! <<<< goto! << */
    open_app_style_err:
    close_style_file(g_scene_list->commonui_style);
    g_scene_list->commonui_style = NULL;

    /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< goto! <<<<<<<<<<<<<<<< goto! <<<<<<<< goto! <<<< goto! << */
    open_com_style_err:

    return false;
}

static bool _scene_list_close_style_files(void)
{
    if(g_scene_list->app_style != NULL)
    {
        close_style_file(g_scene_list->app_style);
    }

    if(g_scene_list->commonui_style != NULL)
    {
        close_style_file(g_scene_list->commonui_style);
    }

    if(sty_close_common_pic_res() == false)
    {
        print_err("close common picture resource error!");
        return false;
    }

    if(sty_close_app_pic_res() == false)
    {
        print_err("close app picture resource error!");
        return false;
    }

    return true;
}

static bool _scene_list_open_theme(void)
{
    rotation_e angle;

    // 取视图角度
    angle = sty_get_view_rotation();

    // 设置屏幕方向
    sty_set_screen_rotation(angle);

    if ( (angle == ROTATION_0) || (angle == ROTATION_180) )
    {
        if(g_scene_list->commonui_style != NULL)
        {
            g_scene_list->common_scene = load_scene(g_scene_list->commonui_style, COMMONUI_SIDELONG_SCENE );
        }

        if(g_scene_list->app_style != NULL)
        {
            g_scene_list->scene = load_scene(g_scene_list->app_style, LIST_SIDELONG_SCENE);
        }
    }
    else
    {
        if(g_scene_list->commonui_style != NULL)
        {
            g_scene_list->common_scene = load_scene(g_scene_list->commonui_style, COMMONUI_UPRIGHT_SCENE );
        }

        if(g_scene_list->app_style != NULL)
        {
            g_scene_list->scene = load_scene(g_scene_list->app_style, LIST_UPRIGHT_SCENE);
        }
    }

    if ( (g_scene_list->common_scene == NULL) || (g_scene_list->scene == NULL) )
    {
        print_err("load commonui and application scene failed");
        return false;
    }
    else
    {
        sty_set_common_scene(g_scene_list->common_scene);
        sty_set_app_scene(g_scene_list->scene);
    }

    return true;
}

static bool _scene_list_close_theme(void)
{

    free_scene(g_scene_list->common_scene);
    g_scene_list->common_scene = NULL;

    free_scene(g_scene_list->scene);
    g_scene_list->scene = NULL;

    return true;
}

static bool _scene_list_open_str_res(void)
{
    char file[512];

    // 获取commonui 字符串资源文件名
    sprintf(file, "%s/%s_%s.str", sys_get_common_string_res_dir(), "commonui", sys_get_cur_language_codes());
    print_dbg("load %s",file);
    // 打开 commonui 字符串资源文件
    if(sty_open_common_str_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }

    // 获取setting应用字符串资源文件名
    sprintf(file, "%s/%s_%s.str", sys_get_app_string_res_dir(), "setting", sys_get_cur_language_codes());
    print_dbg("load %s",file);
    // 打开应用的字符串资源文件
    if(sty_open_app_str_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }

    // 设置语言编码
    sty_set_text_encoding(GUI_UTF8_ENC);
    gui_set_language_mode(sys_get_cur_language_id());

    return true;
}

static bool _scene_list_close_str_res(void)
{
    if(sty_close_common_str_res() == false)
    {
        print_err("close common string resource error!");
        return false;
    }

    if(sty_close_app_str_res() == false)
    {
        print_err("close app string resource error!");
        return false;
    }

    return true;
}

static bool _scene_list_get_bg_bitmap(void)
{
    if(g_scene_list->bg_bitmap_info != NULL)
    {
        return false;
    }

    g_scene_list->bg_bitmap_info = get_user_bg_pic(BG_SETTING);
    if(NULL == g_scene_list->bg_bitmap_info)
    {
        /*获取用户自定义背景图失败，使用ui-editor上添加的默认图*/
        g_scene_list->bg_bitmap_info = sty_create_bitmap((int)g_scene_list->bg_pic->pic_id); 
    }

    if(NULL == g_scene_list->bg_bitmap_info)
    {
        print_err("get backgroud bitmap failed!");
        return false;
    }

    return true;
}

static bool _scene_list_delete_bg_bitmap(void)
{
    if(g_scene_list->bg_bitmap_info == NULL)
    {
        return false;
    }

    sty_delete_bitmap(g_scene_list->bg_bitmap_info);
    g_scene_list->bg_bitmap_info = NULL;

    return true;
}

static bool _scene_list_rotate(void)
{
    fusion_effect_t fusion_effect = {0}; 
    fusion_get_config(FUSION_TYPE_ROTATE, FUSION_FORWARD, &fusion_effect);
    fusion_set_effect(&fusion_effect);
    fusion_set_first_frame(NULL);

    _scene_list_close_theme();
    _scene_list_open_theme();
    _scene_list_load_resources();

    gui_wm_set_size(g_scene_list->win_id, g_scene_list->scene->width, g_scene_list->scene->height);

    _scene_list_reload_widget_style();
    _scene_list_paint();

    fusion_set_last_frame(NULL);
    fusion_display( NULL);

    return true;

}

static bool _scene_list_reload_theme(void)
{
    _scene_list_close_theme();
    _scene_list_close_style_files();

    _scene_list_open_style_files();
    _scene_list_open_theme();
    _scene_list_load_resources();

    gui_wm_set_size(g_scene_list->win_id, g_scene_list->scene->width, g_scene_list->scene->height);

    _scene_list_reload_widget_style();

    return true;
}

static bool _scene_list_set_default_font(void)
{
    int value = 0;
    /* crate font */
    if(g_scene_list->fontdes > 0)
    {
        return false;
    }

    g_scene_list->fontdes = gui_create_font(sys_get_default_font_file(), 16);
    if(g_scene_list->fontdes < 0)
    {
        print_err("call gui_create_font failed");

        /* reuse */
        value = 0;
        if(set_config(CFG_USER_FONT_VALID,(char *)&value, sizeof(value)) < 0 )
        {
            print_warning("set_config (CFG_USER_FONT_VALID) failed!");
        }

        return false;
    }

    if(gui_dc_set_default_fontface(g_scene_list->fontdes) < 0) 
    {
        print_err("gui_dc_set_default_fontface failed");
        return false;
    }

    return true;
}

static bool _scene_list_unset_default_font(void)
{
    /* crate font */
    if(g_scene_list->fontdes <= 0)
    {
        return false;
    }

    gui_destroy_font(g_scene_list->fontdes);
    g_scene_list->fontdes = 0; 

    return true;
}

static bool _scene_list_load_resources(void)
{
    /* get scene backgroud picture resouce */
    g_scene_list->bg_pic = (picture_resource_t *) get_scene_child(g_scene_list->scene, PIC_BG );

    /* get on headbar icon picture resource for setting */
    g_scene_list->on_headbar_icon = (picture_resource_t *) get_scene_child(g_scene_list->scene, PIC_ICON );

    /* get on headbar title string resource for setting */
    g_scene_list->on_headbar_title = (string_resource_t *) get_scene_child(g_scene_list->scene, STR_TITLE );

    if( (g_scene_list->bg_pic == NULL) 
            || (g_scene_list->on_headbar_icon == NULL) 
            || (g_scene_list->on_headbar_title == NULL))
    {
        print_err("load resouces failed!");
        return false;
    }

    if(g_scene_list->bg_bitmap_info != NULL)
    {
        _scene_list_delete_bg_bitmap();
    }

    if(_scene_list_get_bg_bitmap() == false)
    {
        return false;
    }

    return true;
}

static bool _scene_list_reload_widget_style(void)
{
    if ( g_scene_list->menu != NULL)
    {
        setting_list_widget_menu_load_style(g_scene_list->menu);
    }

    if ( g_scene_list->headbar != NULL)
    {
        headbar_load_style(g_scene_list->headbar);
    }

    if(g_scene_list->adjust != NULL)
    {
        adjust_load_style(g_scene_list->adjust);
    }

    if(g_scene_list->softkeyboard != NULL)
    {
        softkeyboard_load_style(g_scene_list->softkeyboard);
    }

    if(g_scene_list->dialog != NULL)
    {
        dialog_load_style(g_scene_list->dialog);
    }

    if(g_scene_list->volumebar != NULL)
    {
        volumebar_load_style(g_scene_list->volumebar);
    }

    if(g_scene_list->msgbox != NULL)
    {
        msgbox_load_style(g_scene_list->msgbox);
    }

    if(g_scene_list->progress_box != NULL)
    {
        progress_box_load_style(g_scene_list->progress_box);
    }

    if(g_scene_list->loadingbox != NULL)
    {
        loadingbox_load_style(g_scene_list->loadingbox);
    }

    dialog_modal_load_style();

    return true;
}

static bool _scene_list_paint_bg(void)
{
    int hdc = gui_dc_get(g_scene_list->win_id);
    sty_draw_bitmap(hdc, g_scene_list->bg_bitmap_info, 0, 0);
    return true;
}

static bool _scene_list_paint(void)
{
    _scene_list_paint_bg();

    if ( g_scene_list->menu != NULL)
    {
        setting_list_widget_menu_paint(g_scene_list->menu);
    }

    if ( g_scene_list->headbar != NULL)
    {
        headbar_paint(g_scene_list->headbar);
    }

    if(g_scene_list->adjust != NULL)
    {
        adjust_paint(g_scene_list->adjust);
    }

    if(g_scene_list->softkeyboard != NULL)
    {
        softkeyboard_paint(g_scene_list->softkeyboard);
    }

    if(g_scene_list->dialog != NULL)
    {
        dialog_paint(g_scene_list->dialog);
    }

    if(g_scene_list->volumebar != NULL)
    {
        volumebar_paint(g_scene_list->volumebar);
    }

    if(g_scene_list->msgbox != NULL)
    {
        msgbox_paint(g_scene_list->msgbox);
    }

    if(g_scene_list->progress_box != NULL)
    {
        progress_box_paint(g_scene_list->progress_box);
    }

    if(g_scene_list->loadingbox != NULL)
    {
        loadingbox_paint(g_scene_list->loadingbox);
    }

    dialog_modal_paint();

    return true;
}

static bool _create_volumebar(void)
{
    if(g_scene_list->volumebar != NULL)
    {
        print_warning("volumebar is already exist!");
        return false;
    }

    g_scene_list->volumebar = volumebar_create(0, (volumebar_cb_t) _volumebar_cb);
    if( g_scene_list->volumebar == NULL )
    {
        print_err("create volumebar error");
    }
    else
    {
        volumebar_paint(g_scene_list->volumebar);
    } 
    return true;
}

static void _volumebar_cb( volumebar_t *volumebar, volumebar_sig_e signal )
{
    switch( signal )
    {
        case WIDGET_SIG_VOLUMEBAR_CHANGED:
        {
            break;
        }

        case WIDGET_SIG_VOLUMEBAR_QUIT:
        {
            _delete_volumebar();
            break;
        }

        default:
        {
            break;
        }
    }
}

static bool _delete_volumebar(void)
{
    if(g_scene_list->volumebar != NULL)
    {
        volumebar_delete(g_scene_list->volumebar);
        g_scene_list->volumebar = NULL;
    }

    return true;
}

static bool _create_headbar(headbar_type_e type)
{
    headbar_init_t headbar_init;

    if(g_scene_list->headbar != NULL)
    {
        print_warning("headbar is already exist!");
        return false;
    }

    headbar_init.type = type;
    headbar_init.pic_id_head = g_scene_list->on_headbar_icon->pic_id;
    headbar_init.str_id_title = g_scene_list->on_headbar_title->str_id;

    /* init type and callback function */
    g_scene_list->headbar = headbar_create(&headbar_init, _headbar_cb);
    if (NULL == g_scene_list->headbar)
    {
        print_err("create headbar failed!");
        return false;
    }

    headbar_paint(g_scene_list->headbar);

    return true;

}

static void _headbar_cb(headbar_t * headbar, widget_sig_headbar_e signal)
{
    /*do nothing*/
}

static bool _delete_headbar(void)
{
    if(g_scene_list->headbar != NULL)
    {
        headbar_delete(g_scene_list->headbar);
        g_scene_list->headbar = NULL;
    }

    return true;
}

static bool _create_widget_menu(void)
{
    if(g_scene_list->menu != NULL)
    {
        print_warning("menu widget is already exist!");
        return false;
    }

    /* create  menu widget */
    g_scene_list->menu = setting_list_widget_menu_create((menu_cb_t)_widget_menu_cb);

    if(NULL == g_scene_list->menu)
    {
        print_err("create widget menu failed!");
        return false;
    }

    /* enter main mainu */
    setting_list_menu_main_enter(g_scene_list->menu, FUSION_FORWARD);

    return true;
}

static void _widget_menu_cb( menuview_t * menuview, menu_sig_e signal)
{
    msg_apps_t msg = {0};
    msg_reply_t reply = {0};
    struct timespec timeout;

    switch ( signal )
    {
        case WIDGET_SIG_MENU_QUIT:
        {
            _scene_list_quit();
            break;
        }
        case WIDGET_SIG_MENU_LANGUAGE_CHANGED:
        {
            _scene_list_close_str_res();
            _scene_list_open_str_res();
            if ( g_scene_list->headbar != NULL)
            {
                headbar_load_style(g_scene_list->headbar);
                headbar_paint(g_scene_list->headbar);
            }

            break;
        }

        case WIDGET_SIG_MENU_SCREEN_MODE_CHANGED:
        {
            _scene_list_rotate();
            break;
        }

        case WIDGET_SIG_MENU_FONT_CHANGED:
        {
            timeout.tv_sec = 1;
            timeout.tv_nsec = 0;
            msg.type = MSG_UNSET_DEF_FONT_SYNC;
            send_sync_msg(APP_NAME_CONFIG, &msg, &reply, &timeout);
            if(reply.type != MSG_REPLY_SUCCESS)
            {
                print_err("send sync msg to config to unset default font failed!");
            }

            _scene_list_unset_default_font();
            _scene_list_set_default_font();

            msg.type = MSG_SET_DEF_FONT_SYNC;
            send_sync_msg(APP_NAME_CONFIG, &msg, &reply, &timeout);
            if(reply.type != MSG_REPLY_SUCCESS)
            {
                print_err("send sync msg to config to reset default font failed!");
            }

            if ( g_scene_list->headbar != NULL)
            {
                headbar_load_style(g_scene_list->headbar);
                headbar_paint(g_scene_list->headbar);
            }

            if ( g_scene_list->menu != NULL)
            {
                setting_list_widget_menu_load_style(g_scene_list->menu);
                setting_list_widget_menu_paint(g_scene_list->menu);
            }
            break;
        }

        case WIDGET_SIG_MENU_THEME_CHANGED:
        {
            _scene_list_close_str_res();
            _scene_list_open_str_res();

            _scene_list_reload_theme();
            _scene_list_paint();
            break;
        }
        default:
        {
            break;
        }
    }
}

static bool _delete_widget_menu(void)
{
    if(g_scene_list->menu != NULL)
    {
        setting_list_widget_menu_delete(g_scene_list->menu );
        g_scene_list->menu = NULL;
    }

    return true;
}

static bool _create_msgbox(void)
{
    g_scene_list->msgbox = msgbox_create(_msgbox_cb);
    if (NULL == g_scene_list->msgbox)
    {
        print_err("create msgbox failed!");
        return false;
    }

    return true;
}

static void _msgbox_cb( msgbox_t * msgbox, msgbox_sig_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_MSGBOX_SHOW:
        {
            print_dbg("show message box");
            break;
        }

        case WIDGET_SIG_MSGBOX_QUIT:
        {
            print_dbg("quit message box");
            break;
        }

        default:
        {
            break;
        }
    }
}

static bool _delete_msgbox(void)
{
    if(g_scene_list->msgbox != NULL)
    {
        msgbox_delete(g_scene_list->msgbox);
        g_scene_list->msgbox = NULL;
    }

    return true;
}

static void _scene_list_fusion_set_first_frame(void)
{
    fusion_frame_t frame;

    frame.rect.x = g_scene_list->scene->x;
    frame.rect.y = g_scene_list->scene->y;
    frame.rect.width = g_scene_list->scene->width;
    frame.rect.height = g_scene_list->scene->height;
    frame.pdata = NULL;
    frame.alpha = false;

    fusion_set_first_frame(&frame);
}

static void _scene_list_fusion_set_last_frame(void)
{
    fusion_frame_t frame;

    frame.rect.x = g_scene_list->scene->x;
    frame.rect.y = g_scene_list->scene->y;
    frame.rect.width = g_scene_list->scene->width;
    frame.rect.height = g_scene_list->scene->height;
    frame.pdata = NULL;
    frame.alpha = false;

    fusion_set_last_frame(&frame);
}

int setting_scene_list(scene_list_t *scene_list)
{
    msg_apps_t  msg;

    print_dbg(">>> enter scene");

    if(NULL == scene_list)
    {
        print_warning("input parameter(scene_menu) is a NULL pointer!");
        return -1;
    }

    g_scene_list = scene_list;

    if (_scene_list_init() == false)
    {
        print_err("scene init error!\n");
        return -1;
    }

    register_sys_dispatcher(_scene_list_proc_sys);

    while (get_msg(&msg) == true)
    {
        dispatch_msg(&msg);
    }

    unregister_sys_dispatcher();

    if (_scene_list_deinit() == false)
    {
        print_err("scene menu exit error!");
        return -1;
    }

    print_dbg("<<< exit scene");

    return g_scene_list->exit_type;
}

