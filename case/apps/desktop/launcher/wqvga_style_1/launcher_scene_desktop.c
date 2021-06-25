/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : launcher_scene_desktop.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      chencm    2009-04-22          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     launcher_scene_desktop.c
 * \brief    main menu scene
 * \author   chencm
 *
 * \version 1.0
 * \date  2009/04/22
 *******************************************************************************/

#include <include_case.h>
#include <include_psp.h>
#include "./../launcher.h"
#include "launcher_sty.h"
#include "launcher_desktop_ui.h"

/* global variable define */
scene_desktop_t* g_scene_desktop = NULL;
/*set scene CPU CLK: Min=48Mhz,Max=168Mhz*/
static const clk_domain_t s_clk_domain =
{ CLK_LOAD_MIN, CLK_LOAD_HIGH_0 };

static bool _launch_app(appinfo_list_t * desktop_info);

/* internal static scene function declaration */
static bool _create_headbar(headbar_type_e type);
static void _headbar_callback(headbar_t *headbar, int signal);
static void _delete_headbar(void);

static bool _create_volumebar(void);
static void _volumebar_callback(volumebar_t *data, volumebar_sig_e signal);
static void _delete_volumebar(void);

static bool _create_menuview_widget(void);
static void _menuview_widget_callback(menuview_t *data, menuview_sig_e signal);
static bool _delete_menuview_widget(void);

static bool _create_msgbox(void);
static void _msgbox_cb(msgbox_t * msgbox, msgbox_sig_e signal);
static bool _delete_msgbox(void);

static bool _scene_desktop_open_theme(void);
static bool _scene_desktop_close_theme(void);
static bool _scene_desktop_open_str_res(void);
static bool _scene_desktop_close_str_res(void);
static bool _scene_desktop_set_default_font(void);
static bool _scene_desktop_unset_default_font(void);
static bool _scene_desktop_rotate(void);
static void _scene_desktop_paint(scene_desktop_t *data);
static void _scene_desktop_quit(void);
static bool _scene_desktop_init(scene_desktop_t * data);
static bool _scene_desktop_deinit(scene_desktop_t * data);
static bool _scene_desktop_proc_gui(scene_desktop_t *data, gui_wm_message_t *pmsg);
static void _scene_desktop_callback(gui_wm_message_t * pmsg);
static int _scene_desktop_proc_sys(msg_apps_t *msg);

static bool s_exiting_scene = false;
static bool _launch_app(appinfo_list_t * desktop_info)
{
    msg_apps_t app_msg;
    msg_reply_t reply;
    char app_dir[FULL_FILENAME_LEN] =
    { '\0' };

    if (NULL == desktop_info)
    {
        print_warning("input paramter(desktop_info) is a NULL pointer!");
        return false;
    }

    if (NULL == desktop_info->value[DKEY_EXEC])
    {
        print_warning("can not launch a NULL executable name!");
        return false;
    }

    print_info("launch %s", desktop_info->value[DKEY_EXEC]);
    //enter_background_mode();

    app_msg.type = MSG_CREAT_APP_SYNC;
    strcpy(app_dir, desktop_info->filename);
    dirname(app_dir);
    if ((strlen(app_dir) + strlen(desktop_info->value[DKEY_EXEC])) < sizeof(app_msg.content))
    {
        snprintf(app_msg.content, sizeof(app_msg.content), "%s/%s", app_dir, desktop_info->value[DKEY_EXEC]);
    }
    else
    {
        print_err();
    }
    //strncpy(app_msg.content, desktop_info->value[DKEY_EXEC], FULL_FILENAME_LEN);
    //    if(desktop_info->arg != NULL)
    //    {
    //        strcat(app_msg.content, " ");
    //        strcat(app_msg.content, desktop_info->arg);
    //    }
    send_sync_msg("manager.app", &app_msg, &reply, NULL);

    if (MSG_REPLY_SUCCESS == reply.type)
    {
        print_info("Launch %s APP successed!", app_msg.content);

        //print_dbg("exit launcher");
        //g_scene_launch.ret_type = SCENE_LAUNCH_EXIT;
        // exit_msg_loop();

        return true;
    }
    else
    {
        print_err("Launch %s APP failed!", app_msg.content);

        //enter_foreground_mode();
        //g_scene_launch->exit_type = SCENE_DESKTOP;
        //exit_msg_loop();

        return false;
    }
}


static void _menuview_widget_callback(menuview_t *data, menuview_sig_e signal)
{
    fusion_effect_t effect;

    switch (signal)
    {

        case WIDGET_SIG_MENUVIEW_UP:
        {
            g_scene_desktop->cur_desktop = g_scene_desktop->cur_desktop->prev;
        }
        break;

        case WIDGET_SIG_MENUVIEW_DOWN:
        {
            g_scene_desktop->cur_desktop = g_scene_desktop->cur_desktop->next;
        }
        break;

        case WIDGET_SIG_MENUVIEW_OK:
        { // quit desktop scene, rhen launch child app
            _scene_desktop_quit();
        }
        break;

        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            // return to parent menu
            if (g_scene_desktop->cur_desktop_list->parent != NULL)
            {
                fusion_get_config(FUSION_TYPE_COMMON, FUSION_BACKWARD, &effect);
                fusion_set_effect(&effect);
                fusion_set_first_frame(NULL);

                /* return parent */
                g_scene_desktop->cur_desktop = g_scene_desktop->cur_desktop_list->parent;
                g_scene_desktop->cur_desktop_list = g_scene_desktop->last_desktop_list;

                //delete menuview common
                _delete_menuview_widget();

                _delete_headbar();
                _create_headbar(HEADBAR_TYPE_SIMPLE);
                g_scene_desktop->display_mode = DEFAULT;
                _scene_desktop_paint(g_scene_desktop);

                fusion_set_last_frame(NULL);
                fusion_display(NULL);
            }
        }
        break;

        default:
        break;
    }
}

static bool _create_menuview_widget(void)
{
    appinfo_list_t *p = NULL;
    menuitem_t child_name =
    { 0 };


    g_scene_desktop->menuview = menuview_create(MENUVIEW_MENULIST_TEXT, (menuview_cb_t) _menuview_widget_callback);
    if (g_scene_desktop->menuview == NULL)
    {
        print_err("create menuview common error");
        return false;
    }
    else
    {
        g_scene_desktop->menu = menu_create();
        if (NULL == g_scene_desktop->menu)
        {
            print_err("create menu failed!");
            return false;
        }

        //init menu items
        p = g_scene_desktop->cur_desktop_list;
        do
        {
            child_name.text = p->value[DKEY_NAME_LANG];
            menu_append_item(g_scene_desktop->menu, &child_name);
            p = p->next;
        } while (p != g_scene_desktop->cur_desktop_list);

        menuview_set_menu(g_scene_desktop->menuview, g_scene_desktop->menu);
        menu_set_cur_index(g_scene_desktop->menu, g_scene_desktop->cur_desktop->app_index);

        show_desktop_bg();
        menuview_paint(g_scene_desktop->menuview);

    }

    return true;
}

static bool _delete_menuview_widget(void)
{
    //delete menu
    if (g_scene_desktop->menu != NULL)
    {
        menu_delete(g_scene_desktop->menu);
        g_scene_desktop->menu = NULL;
    }
    //delete menuview common
    if (g_scene_desktop->menuview != NULL)
    {
        menuview_delete(g_scene_desktop->menuview);
        g_scene_desktop->menuview = NULL;
    }
    return true;
}

static bool _create_msgbox(void)
{
    g_scene_desktop->msgbox = msgbox_create(_msgbox_cb);
    if (NULL == g_scene_desktop->msgbox)
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
        {
            print_dbg("show message box");
        }
        break;
        case WIDGET_SIG_MSGBOX_QUIT:
        {
            print_dbg("quit message box");
        }
        break;

        case WIDGET_SIG_MSGBOX_USB_LAUNCHED:
        case WIDGET_SIG_MSGBOX_TVIN_LAUNCHED:
        case WIDGET_SIG_MSGBOX_CAMERA_LAUNCHED:
        case WIDGET_SIG_MSGBOX_PHOTO_LAUNCHED:
        case WIDGET_SIG_MSGBOX_PL_GEN_LAUNCHED:
        {
            s_exiting_scene = true;
            exit_msg_loop();
            g_scene_desktop->exit_type = SCENE_LAUNCH;

//            //����U��ǰ��Ҫ��ж�ص�font��Դ�������˳�U�̺�font���ʧЧ �����ַ�����ʾ���󡣽����BUG00028845
//            _scene_desktop_unset_default_font();
//            enter_background_mode();
        }
        break;

        default:
        break;
    }
}

static bool _delete_msgbox(void)
{
    if (g_scene_desktop->msgbox != NULL)
    {
        msgbox_delete(g_scene_desktop->msgbox);
        g_scene_desktop->msgbox = NULL;
    }

    return true;
}

static bool _create_headbar(headbar_type_e type)
{
    headbar_init_t headbar_init = { 0 };
    headbar_init.type = type;

    if((type == HEADBAR_TYPE_NORMAL) && ((g_scene_desktop->on_headbar_icon) != NULL))
    {
        headbar_init.pic_id_head = g_scene_desktop->on_headbar_icon->pic_id;
    }

    g_scene_desktop->headbar = headbar_create(&headbar_init, (headbar_cb_t) _headbar_callback);

    if (NULL == g_scene_desktop->headbar)
    {
        print_err("headbar_create failed\n\n");
        return false;
    }

    headbar_paint(g_scene_desktop->headbar);

    return true;
}

static void _headbar_callback(headbar_t *headbar, int signal)
{
    print_dbg("signal:0x%x", signal);
}

static void _delete_headbar(void)
{
    if(g_scene_desktop->headbar != NULL)
    {
        headbar_delete(g_scene_desktop->headbar);
        g_scene_desktop->headbar = NULL;
    }
}

static bool _create_volumebar(void)
{
    g_scene_desktop->volumebar = volumebar_create(0, (volumebar_cb_t) _volumebar_callback);
    if (NULL == g_scene_desktop->volumebar)
    {
        print_err("create volumebar common error");
        return false;
    }
    else
    {
        volumebar_paint(g_scene_desktop->volumebar);
    }
    return true;
}

static void _volumebar_callback(volumebar_t *data, volumebar_sig_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_VOLUMEBAR_CHANGED:
        {
            print_dbg("SIG_VOL_DEC: %s---%d---\n", __FILE__, __LINE__);
        }
        break;

        case WIDGET_SIG_VOLUMEBAR_QUIT:
        {
            _delete_volumebar();
        }
        break;

        default:
        break;
    }
}

static void _delete_volumebar(void)
{
    if (g_scene_desktop->volumebar != NULL)
    {
        volumebar_delete(g_scene_desktop->volumebar);
        g_scene_desktop->volumebar = NULL;
    }
}



static bool _scene_desktop_rotate(void)
{
    fusion_effect_t effect;

    fusion_get_config(FUSION_TYPE_ROTATE, FUSION_FORWARD, &effect);
    fusion_set_effect(&effect);
    fusion_set_first_frame(NULL);

    _scene_desktop_close_theme();
    _scene_desktop_open_theme();

    desktop_release_resource();
    if ((!desktop_load_style()) || (!desktop_buffer_resource()))
    {
        print_err("desktop scene load resource false!");
    }

    gui_wm_set_size(g_scene_desktop->win_id, g_scene_desktop->scene->width, g_scene_desktop->scene->height);

    g_scene_desktop->display_mode = DEFAULT;

    if (g_scene_desktop->headbar != NULL)
    {
        headbar_load_style(g_scene_desktop->headbar);
        headbar_paint(g_scene_desktop->headbar);
    }

    if (g_scene_desktop->menuview != NULL)
    {
        menuview_load_style(g_scene_desktop->menuview, MENUVIEW_MENULIST_TEXT);
        menuview_paint(g_scene_desktop->menuview);
    }

    if (g_scene_desktop->volumebar != NULL)
    {
        volumebar_load_style(g_scene_desktop->volumebar);
        volumebar_paint(g_scene_desktop->volumebar);
    }

    if (g_scene_desktop->msgbox != NULL)
    {
        msgbox_load_style(g_scene_desktop->msgbox);
        msgbox_paint(g_scene_desktop->msgbox);
    }

    _scene_desktop_paint(g_scene_desktop);

    fusion_set_last_frame(NULL);
    fusion_display(NULL);

    return true;

}

/* scene paint function */
static void _scene_desktop_paint(scene_desktop_t *data)
{

    //current list is child list,so draw it by menuview
    if (g_scene_desktop->cur_desktop_list->parent != NULL)
    {
        if (g_scene_desktop->menuview != NULL)
        {
            show_desktop_bg();
            menuview_paint(g_scene_desktop->menuview);
        }
        else
        {
            _delete_headbar();
            //get current app icon (will be displayed on the NORMAL type headbar)
            g_scene_desktop->on_headbar_icon = desktop_get_headbar_appicon(data->cur_desktop_list->parent);
            _create_headbar(HEADBAR_TYPE_NORMAL);
            _create_menuview_widget();
        }
    }
    else
    {
        switch (data->display_mode)
        {
            case PREV_ANIMATION:
            case NEXT_ANIMATION:
            {
                show_next_desktop(data);
            }
            break;

            case DISPLAY_CHILD:
            { //TODO: ���б������ʾ��Ӧ��menuview�ؼ�
                show_child_desktop(data);
            }
            break;

            case DEFAULT: //��ʾ��ʼ״̬�Ľ���
            {
                show_current_desktop(data);
            }
            break;

            default:
            break;
        }
    }
}

/* if define EXIT_DESKTOP, then the desktop scene should be exit before launch APP */
#define EXIT_DESKTOP
/* scene quit */
static void _scene_desktop_quit(void)
{
    s_exiting_scene = true;
#ifdef EXIT_DESKTOP
    exit_msg_loop();
    g_scene_desktop->exit_type = SCENE_LAUNCH_APP;
#else
    //����U��ǰ��Ҫ��ж�ص�font��Դ�������˳�U�̺�font���ʧЧ �����ַ�����ʾ���󡣽����BUG00028845
    _scene_desktop_unset_default_font();

    enter_background_mode();
    _launch_app(g_scene_desktop->cur_desktop);
#endif
}

static bool _scene_desktop_open_theme(void)
{
    rotation_e angle;
    char common_file[FULL_FILENAME_LEN], app_file[FULL_FILENAME_LEN];

    angle = sty_get_view_rotation(); // ȡ��ͼ�Ƕ�
    sty_set_screen_rotation(angle); // ������Ļ����

    //    // ����commonui �� Desktop������style��Դ
    //    // ��ȡcommonui �� style��Դ �ļ���
    //    sprintf(common_file, "%s/%s.sty", sys_get_common_theme_res_dir(), "commonui");
    //    sprintf(app_file, "%s/%s.sty", sys_get_app_theme_res_dir(), "launcher"); // ��ȡӦ�õ� .sty ��Դ �ļ���
    //
    //    g_scene_desktop->app_style = open_style_file(app_file);
    //    g_scene_desktop->common_style = open_style_file(common_file);

    if ((angle == ROTATION_0) || (angle == ROTATION_180))
    {
        g_scene_desktop->common_scene = load_scene(g_scene_desktop->common_style, COMMONUI_SIDELONG_SCENE);
        g_scene_desktop->scene = load_scene(g_scene_desktop->app_style, DESKTOP_SIDELONG_SCENE);
    }
    else //else if ((angle == ROTATION_90) || (angle == ROTATION_270))
    {
        g_scene_desktop->common_scene = load_scene(g_scene_desktop->common_style, COMMONUI_UPRIGHT_SCENE);
        g_scene_desktop->scene = load_scene(g_scene_desktop->app_style, DESKTOP_UPRIGHT_SCENE);
    }

    if ((g_scene_desktop->common_scene == NULL) || (g_scene_desktop->scene == NULL))
    {
        print_err("load desktop scene failed");
        return false;
    }
    else
    {
        sty_set_common_scene(g_scene_desktop->common_scene); // ���� commonui��style��Դ��Ϊsty_get_common_scene()��׼��
        sty_set_app_scene(g_scene_desktop->scene); // ����Ӧ�õ� style ��Դ,Ϊsty_get_app_scene()��׼��
    }

    // ����commonui ��ͼƬ��Դ
    sprintf(common_file, "%s/%s.res", sys_get_common_theme_res_dir(), "commonui"); //��ȡcommonui ��ͼƬ��Դ�ļ���
    if (sty_open_common_pic_res(common_file) == false) // �� commonui ͼƬ��Դ�ļ�
    {
        print_err("open %s failed!", common_file);
        return false;
    }

    // ����Ӧ�õ�ͼƬ��Դ
    sprintf(app_file, "%s/%s.res", sys_get_app_theme_res_dir(), "launcher"); //��ȡӦ�õ�ͼƬ��Դ�ļ���
    if (sty_open_app_pic_res(app_file) == false) // ��Ӧ�õ�ͼƬ��Դ�ļ�
    {
        print_err("open %s failed!", app_file);
        return false;
    }

    g_scene_desktop->on_headbar_icon = (picture_resource_t *) get_scene_child(g_scene_desktop->scene, PIC_HBAR_TOOL);
    return true;
}

static bool _scene_desktop_close_theme(void)
{

    free_scene(g_scene_desktop->common_scene);
    g_scene_desktop->common_scene = NULL;

    free_scene(g_scene_desktop->scene);
    g_scene_desktop->scene = NULL;

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

static bool _scene_desktop_open_str_res(void)
{
    char file[FULL_FILENAME_LEN];

    // ��ȡcommonui �ַ�����Դ�ļ���
    sprintf(file, "%s/%s_%s.str", sys_get_common_string_res_dir(), "commonui", sys_get_cur_language_codes());
    print_dbg("load %s", file);
    // �� commonui �ַ�����Դ�ļ�
    if (sty_open_common_str_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }

    // ��ȡsettingӦ���ַ�����Դ�ļ���
    sprintf(file, "%s/%s_%s.str", sys_get_app_string_res_dir(), "launcher", sys_get_cur_language_codes());
    print_dbg("load %s", file);
    // ��Ӧ�õ��ַ�����Դ�ļ�
    if (sty_open_app_str_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }

    // �������Ա���
    sty_set_text_encoding(GUI_UTF8_ENC);
    gui_set_language_mode( sys_get_cur_language_id());

    return true;
}

static bool _scene_desktop_close_str_res(void)
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

static bool _scene_desktop_set_default_font(void)
{

    /* crate font */
    if (g_scene_desktop->fontdes > 0)
    {
        return false;
    }

    g_scene_desktop->fontdes = gui_create_font(sys_get_default_font_file(), 16);
    if (g_scene_desktop->fontdes < 0)
    {
        print_err("call gui_create_font failed");
        return false;
    }

    if (gui_dc_set_default_fontface(g_scene_desktop->fontdes) < 0)
    {
        print_err("gui_dc_set_default_fontface failed");
        return false;
    }

    return true;
}

static bool _scene_desktop_unset_default_font(void)
{
    /* crate font */
    if (g_scene_desktop->fontdes <= 0)
    {
        return false;
    }

    gui_destroy_font(g_scene_desktop->fontdes);
    g_scene_desktop->fontdes = 0;

    return true;
}

/* scene initial function */
static bool _scene_desktop_init(scene_desktop_t * data)
{
    fusion_effect_t effect;
    rotation_e angle;
    char common_file[FULL_FILENAME_LEN], app_file[FULL_FILENAME_LEN];

    //DUMP_TIME();
    //set scene CPU CLK before enter scene
    sys_pm_set_clk_domain(&s_clk_domain);

    //  �ŵ� desktop�д�,�Ա��˳��������ͷ� gui.so����ʡ�ڴ�ռ䡣
    data->gui_lib = dlopen("gui.so", RTLD_LAZY);
    if (NULL == data->gui_lib)
    {
        print_err("open gui lib err!");
        return false;
    }

    g_scene_desktop = data;

    // ��ȡcommonui ��Ӧ�ó�����style��Դ���
    sprintf(common_file, "%s/%s.sty", sys_get_common_theme_res_dir(), "commonui"); // ��ȡcommonui �� style��Դ �ļ���
    g_scene_desktop->common_style = open_style_file(common_file);

    sprintf(app_file, "%s/%s.sty", sys_get_app_theme_res_dir(), "launcher"); // ��ȡӦ�õ� .sty ��Դ �ļ���
    g_scene_desktop->app_style = open_style_file(app_file);

    //���볡��
    /* load theme ��Դ�ļ� */
    if (_scene_desktop_open_theme() == false)
    {
        return false;
    }

    /* load �ַ�����Դ�ļ� */
    if (_scene_desktop_open_str_res() == false)
    {
        return false;
    }

    //��������
    if (_scene_desktop_set_default_font() == false)
    {
        return false;
    }

    //������Ļ���  set screen size(width & height )
    //gui_set_dimension(g_scene_desktop->scene->width, g_scene_desktop->scene->height);

    //Ӧ���˳� ����desktop����Ҫ����Ч
    if (g_scene_desktop->app_exited == true)
    {
        fusion_get_config(FUSION_TYPE_APP_CHANGE, FUSION_BACKWARD, &effect);
        fusion_set_effect(&effect);
        fusion_set_first_frame(NULL);
    }

    //��������
    g_scene_desktop->win_id = gui_wm_create_window(g_scene_desktop->scene->x, g_scene_desktop->scene->y,
            g_scene_desktop->scene->width, g_scene_desktop->scene->height, WM_CREATE_SHOW, _scene_desktop_callback, 0);
    if (g_scene_desktop->win_id == -1)
    {
        return false;
    }

    //��ȡdc
    g_scene_desktop->hdc = gui_dc_get(g_scene_desktop->win_id);
    //���ý���
    gui_wm_set_focus(g_scene_desktop->win_id);

    //load desktop style resource and buffer resource data from the desktop scene
    //    if ((!desktop_load_style()) || (!desktop_buffer_resource()))
    //    {
    //        print_err("desktop scene load resource false!");
    //    }
    if (!desktop_load_style())
    {
        print_err("desktop scene load resource false!");
    }
    //DUMP_TIME();
    if(_create_headbar(HEADBAR_TYPE_SIMPLE) == false)
    {
        print_err("headbar_create failed\n\n");
        return false;
    }
    //DUMP_TIME();
    headbar_paint(g_scene_desktop->headbar);

    if (_create_msgbox() == false)
    {
        goto create_msgbox_err;
    }

    //paint scene
    g_scene_desktop->display_mode = DEFAULT;
    _scene_desktop_paint(g_scene_desktop); //draw static window bg

    //Ӧ���˳� ����desktop����Ҫ����Ч
    if (g_scene_desktop->app_exited == true)
    {
        fusion_set_last_frame(NULL);
        fusion_display(NULL);
    }

    // if load launcher by console(base argv[1]) then desktop scene is background.
    if (g_scene_desktop->is_console == true)
    {
        enter_background_mode(); //GUI�е���̨
        g_scene_desktop->is_console = false; //just first time need to detect
    }
    else
    {
        enter_foreground_mode(); //GUI�л�ǰ̨

        gui_screen_update();
    }
    //DUMP_TIME();

    if (!desktop_buffer_resource())
    {
        print_err("desktop scene load resource false!");
    }

    clear_key_msg_queue(-1, -1); //clear msg buffer.
    //DUMP_TIME();
    s_exiting_scene = false;
    return true;
    create_msgbox_err: return false;
}

/* scene exit function */
static bool _scene_desktop_deinit(scene_desktop_t * data)
{
    bool ret;

    //GUI�л�����̨���Ա�ͬʱ������UIӦ��
    enter_background_mode();
    //ɾ������
    gui_wm_delete_window(data->win_id);
    data->win_id = 0;

    //ɾ��headbar
    if (data->headbar != NULL)
    {
        ret = headbar_delete(data->headbar);
        print_dbg("%d", ret);
        data->headbar = NULL;
    }

    //release  buffer of the desktop scene picture
    desktop_release_resource();

    //ɾ��volumebar
    if (data->volumebar != NULL)
    {
        volumebar_delete(data->volumebar);
        data->volumebar = NULL;
    }

    _delete_menuview_widget();

    _delete_msgbox();

    //�˳�����
    free_scene(data->scene);
    data->scene = NULL;

    // �ر�commonui ��Ӧ�ó�����style��Դ���
    close_style_file(g_scene_desktop->common_style);
    close_style_file(g_scene_desktop->app_style);

    /* destroy font */
    _scene_desktop_unset_default_font();

    if (_scene_desktop_close_str_res() == false)
    {
        print_err("close desktop str resource false!");
        return false;
    }

    if (_scene_desktop_close_theme() == false)
    {
        print_err("close desktop theme resource false!");
        return false;
    }

    dlclose(data->gui_lib);

    //clear CPU CLK before return scene
    sys_pm_clear_clk_domain(&s_clk_domain);

    return true;
}

/* scene msg process */
static bool _scene_desktop_proc_gui(scene_desktop_t *data, gui_wm_message_t *pmsg)
{
    fusion_effect_t effect;
    key_event_t *kmsg;
    bool ret = false;    

    switch (pmsg->msgid)
    {
        case WM_MSG_CREATE:
        {
        }
        break;

        case WM_MSG_PAINT:
        {
            print_dbg("WM_MSG_PAINT");
        }
        break;

        case WM_MSG_KEY:
        {
            kmsg = (key_event_t *) (pmsg->data.p); //get key msg
            switch (kmsg->val | kmsg->type)
            {
                case KEY_PREV | KEY_TYPE_DOWN:
                case KEY_PREV | KEY_TYPE_HOLD:
                {
                    if (NULL != g_scene_desktop->menuview)
                    {
                        return false;
                    }
                    
                    print_dbg("prev item");
                    data->cur_desktop = data->cur_desktop->prev;
                    data->focus_app_id = data->cur_desktop->app_index;
                    if (kmsg->type == KEY_TYPE_HOLD)
                    {
                        data->display_mode = DEFAULT;
                    }
                    else
                    {
                        data->display_mode = PREV_ANIMATION;
                    }
                    _scene_desktop_paint(data);
                    clear_key_msg_queue(-1, -1);//need to clear msg between show screen.
                }
                break;
                case KEY_NEXT | KEY_TYPE_DOWN:
                case KEY_NEXT | KEY_TYPE_HOLD:
                {
                    if (NULL != g_scene_desktop->menuview)
                    {
                        return false;    
                    }

                    print_dbg("next item");
                    data->cur_desktop = data->cur_desktop->next;
                    data->focus_app_id = data->cur_desktop->app_index;
                    if (kmsg->type == KEY_TYPE_HOLD)
                    {
                        data->display_mode = DEFAULT;
                    }
                    else
                    {
                        data->display_mode = NEXT_ANIMATION;
                    }
                    _scene_desktop_paint(data);
                    clear_key_msg_queue(-1, -1);//need to clear msg between show screen.
                }
                break;

                case KEY_OK | KEY_TYPE_SHORT_UP:
                {
                    if (NULL != g_scene_desktop->menuview)
                    {
                        return false;    
                    }
                    
                    if ((!strcasecmp(DESKTOP_TYPE_DIR, data->cur_desktop->value[DKEY_TYPE])) && (NULL
                            != data->cur_desktop->child)) //have child item
                    {
                        /* enter sub menu */
                        data->last_desktop_list = data->cur_desktop_list;
                        data->cur_desktop_list = data->cur_desktop->child;
                        data->cur_desktop = data->cur_desktop_list;
                        data->display_mode = DISPLAY_CHILD;

                        fusion_get_config(FUSION_TYPE_COMMON, FUSION_FORWARD, &effect);
                        fusion_set_effect(&effect);
                        fusion_set_first_frame(NULL);

                        if (g_scene_desktop->menuview != NULL)
                        {
                            show_desktop_bg();
                            menuview_paint(g_scene_desktop->menuview);
                        }
                        else
                        {
                            _create_menuview_widget();
                        }
                        _delete_headbar();
                        //get current app icon (will be displayed on the NORMAL type headbar)
                        g_scene_desktop->on_headbar_icon = desktop_get_headbar_appicon(data->cur_desktop_list->parent);
                        _create_headbar(HEADBAR_TYPE_NORMAL);

                        fusion_set_last_frame(NULL);
                        fusion_display(NULL);

                    }
                    else
                    {
                        /* quit main menu scene and launch app */
                        _scene_desktop_quit();
                    }
                }
                break;

                case KEY_VOL | KEY_TYPE_DOWN:
                case KEY_VSUB | KEY_TYPE_DOWN:
                case KEY_VADD | KEY_TYPE_DOWN:
                {
                    if (NULL == g_scene_desktop->volumebar)
                    {
                        _create_volumebar();
                        ret = true;
                    }
                    else    //if volume bar has exited then return false, let the key to volumebar proceed.
                    {
                        ret = false;    
                    }
                           
                }
                break;
                default:
                {
                    ret = false;
                }
                break;
            }
        }
        break;

        default:
        {
            gui_wm_default_callback(pmsg);
        }
        break;
    }

    return ret;
}

/* scene callback function */
static void _scene_desktop_callback(gui_wm_message_t *pmsg)
{
    int top;

    if(s_exiting_scene == true)
    {
        return;
    }

    top = canvas_get_focus(pmsg);

    //dispatch msg
    
    //ret = _scene_desktop_proc_gui(g_scene_desktop, pmsg);
    //if (ret == true)
    //{
    //    return;
    //}
    
    if ((g_scene_desktop->volumebar != NULL) && (top == g_scene_desktop->volumebar->canvas_id)) //volumebar process
    {
        volumebar_proc_gui(g_scene_desktop->volumebar, pmsg);
    }
    else if ((g_scene_desktop->menuview != NULL) && (top == g_scene_desktop->menuview->canvas_id))
    {
        menuview_proc_gui(g_scene_desktop->menuview, pmsg);
        _scene_desktop_proc_gui(g_scene_desktop, pmsg); //respond the vol key when menuview screen.
    }
    else if ((g_scene_desktop->msgbox != NULL) && (top == g_scene_desktop->msgbox->canvas_id))
    {
        /* ����Ĭ��ϵͳ��Ϣ(�͵���ʾ��������Ͱγ���ʾ��USB���빦��ѡ����ʾ��)��msgbox�ؼ���Ҫ����GUI��Ϣ */
        msgbox_proc_gui(g_scene_desktop->msgbox, pmsg);
    }
    else
    {
        _scene_desktop_proc_gui(g_scene_desktop, pmsg);
    }

}

static int _scene_desktop_proc_sys(msg_apps_t *msg)
{
    msg_reply_t t;

    print_dbg("msg.type:%d\n", msg->type);

    switch (msg->type)
    {
        case MSG_ACTIVE_SYNC:
        {
            t.type = MSG_REPLY_SUCCESS;
            reply_sync_msg(&t);
        }
        break;

        case MSG_GSENSOR_ROTATION_CHANGED:
        {
            print_info("received msg: MSG_GSENSOR_ROTATION_CHANGED");
            _scene_desktop_rotate();
        }
        break;

        case MSG_SET_APP_FOREGROUND_SYNC:
        {
            //fusion_set_first_frame(NULL);

            //��������font�������U�̳�����font���ʧЧ�������BUG00028845
            _scene_desktop_set_default_font();

            //GUI�л�ǰ̨
            enter_foreground_mode();//paint scene
            //���ý���
            gui_wm_set_focus(g_scene_desktop->win_id);

            g_scene_desktop->display_mode = DEFAULT;
            _scene_desktop_paint(g_scene_desktop); //draw static window bg

            t.type = MSG_REPLY_SUCCESS;
            reply_sync_msg(&t);

            //fusion_set_last_frame(NULL);
            //fusion_get_config(FUSION_TYPE_APP_CHANGE, FUSION_BACKWARD, &effect);
            //fusion_display(&effect, NULL);
        }
        break;

        case MSG_APP_QUIT:
        {
            s_exiting_scene = true;
            exit_msg_loop();
            g_scene_desktop->exit_type = SCENE_EXIT;
        }
        break;

        default:
        {
            if (g_scene_desktop->msgbox != NULL)
            {
                /* �ؼ�msgbox��������Ĭ��ϵͳ��Ϣ(�͵���ʾ��������Ͱγ���ʾ��USB���빦��ѡ����ʾ��) */
                msgbox_proc_sys(g_scene_desktop->msgbox, msg);
            }
        }
        break;
    }

    return 0;
}

/* scene entry function */
int launcher_scene_desktop(scene_desktop_t * scene_data)
{
    msg_apps_t msg;

    print_dbg(">>> enter scene");

    /* ������ʼ�� */
    if (_scene_desktop_init(scene_data) == false)
    {
        print_err("scene init error!");
        return -1;
    }
    else
    {
    }
    /* ������Ϣѭ�� */
    register_sys_dispatcher(_scene_desktop_proc_sys);

    while (get_msg(&msg) == true)
    {
        dispatch_msg(&msg);
    }

    //DUMP_TIME();
    unregister_sys_dispatcher();

    /* �����˳� */
    if (_scene_desktop_deinit(scene_data) == false)
    {
        print_err("scene exit error!");
        return -1;
    }
    else
    {
    }
    //DUMP_TIME();
    print_dbg("<<< exit scene");

    return g_scene_desktop->exit_type;
}

