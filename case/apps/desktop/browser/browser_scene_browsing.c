/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : browser_scene_browsing.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *    binannan    2009-05-25          v1.0               build this file
 ********************************************************************************/
#include "browser_scene_browsing.h"
#include "browser_browsing_widget_option.h"
#include "file_operation.h"
#include "browser_browsing_option.h"

/*
 * 局部静态函数列表
 */
/**************************************************************************/

static void _loadingbox_widget_cb(loadingbox_t *loadingbox, loadingbox_sig_e signal);

static void _com_noop_dialog_widget_cb(dialog_t * dialog, widget_sig_dialog_e signal);

static void volumebar_cb(volumebar_t *data, volumebar_sig_e signal);

static void _com_headbar_cb(headbar_t * headbar, widget_sig_headbar_e signal);
static bool _com_create_headbar(void);
static bool _com_delete_headbar(void);

static void _com_msgbox_cb(msgbox_t * msgbox, msgbox_sig_e signal);
static bool _com_create_msgbox(void);
static bool _com_delete_msgbox(void);

static void _com_dialog_cb(dialog_t * dialog, widget_sig_dialog_e signal);
static bool _com_create_dialog(dialog_init_t * dialog_init);
static bool _com_delete_dialog(void);

static void _com_filelist_cb(filelist_t * filelist, filelist_sig_e sig);
static void _com_filelist_shortmenu_cb(filelist_t * filelist);
static bool _com_create_filelist(void);


static void _scene_fusion_set_first_frame(scene_t *scene);
static void _scene_fusion_set_last_frame(scene_t *scene);
static void _option_fusion_set_first_frame(menuview_t *mv);
static void _option_fusion_set_last_frame(menuview_t *mv);

static bool _scene_browsing_open_theme(void);
static bool _scene_browsing_close_theme(void);
static bool _scene_browsing_open_str_res(void);
static bool _scene_browsing_close_str_res(void);

static bool _scene_browsing_set_default_font(void);
static bool _scene_browsing_unset_default_font(void);

static void _scene_browsing_load_resources(void);

static void _scene_browsing_rotate(void);

static int _scene_browsing_init(void);

static int _scene_browsing_deinit(void);

static void _scene_browsing_quit(void);

static bool _scene_browsing_paint(void);

static void _scene_browsing_cb(gui_wm_message_t * pmsg);

static bool _scene_proc_gui(scene_browsing_t * data, gui_wm_message_t * pmsg);

static int _scene_proc_sys_msg(msg_apps_t * msg);

static void _scene_browsing_invoke_app(void* param);

/***************************************************************************/

scene_browsing_t *g_scene_browsing = 0;

static fusion_enter_e s_enter_type = FUSION_FORWARD;

static msg_apps_t s_msg_invoke_app;

bool g_msg_lowpower_received = false;

sem_modal_t g_sem_modal;

/*
 * indicate the work thread is active or not. If it's active, the app should not
 * exit when receiving the MSG_APP_QUIT message.
 */
bool g_thread_active = false;

/*
 * indicate the app received the MSG_APP_QUIT message.
 */
static bool msg_app_quit_received = false;

/***************************************************************************/

bool create_thread(void *proc, void *param)
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

    print_dbg(" self thread's priority is: %d\n", self_param.sched_priority );

    result = pthread_create(&tid, &pthread_attr, proc, param);
    pthread_attr_destroy(&pthread_attr);
    if (result != 0)
    {
        print_err("can’t create thread, result:%d\n", result);
        return false;
    }

    g_thread_active = true;
    return true;
}

void file_op_done(void)
{
    bool result;

    dialog_init_t dialog_init;

    if (g_file_op_success == true)
    {
        dialog_init.head = DIALOG_HEAD_SUCCESS;

        if (g_scene_browsing->filelist->content_mode == CONTENT_MOVE)
        {
            dialog_init.str_id = g_scene_browsing->str_move_succeed->str_id;
        }
        else if (g_scene_browsing->filelist->content_mode == CONTENT_COPY)
        {
            dialog_init.str_id = g_scene_browsing->str_copy_succeed->str_id;
        }
        else if (g_scene_browsing->filelist->content_mode == CONTENT_DELETE)
        {
            print_dbg();
            dialog_init.str_id = g_scene_browsing->str_delete_succeed->str_id;
        }
        else
        {
            print_err();
        }
    }
    else
    {
        dialog_init.head = DIALOG_HEAD_FAIL;

        if (g_file_op_errno == ENOSPC)
        {
            dialog_init.str_id = g_scene_browsing->str_no_space->str_id;
            g_file_op_errno = 0;
        }
        else
        {
            if (g_scene_browsing->filelist->content_mode == CONTENT_MOVE)
            {
                dialog_init.str_id = g_scene_browsing->str_move_failed->str_id;

            }
            else if (g_scene_browsing->filelist->content_mode == CONTENT_COPY)
            {
                dialog_init.str_id = g_scene_browsing->str_copy_failed->str_id;
            }
            else if (g_scene_browsing->filelist->content_mode == CONTENT_DELETE)
            {
                dialog_init.str_id = g_scene_browsing->str_delete_failed->str_id;
            }
            else
            {
                print_err();
            }
        }
    }

    sys_forbid_screensaver(false);
    sys_forbid_auto_standby(false);
    sys_forbid_timed_shutdown(false);
    sys_enable_sys_evt_detect(true);

    if ((g_scene_browsing->filelist->content_mode == CONTENT_MOVE) || (g_scene_browsing->filelist->content_mode
            == CONTENT_COPY))
    {
        browser_widget_filelist_exit_fileop_mode();
    }
    else if(g_scene_browsing->filelist->content_mode == CONTENT_DELETE)
    {
        browser_widget_filelist_exit_delete_fileop_mode();
    }
    else
    {

    }

    g_scene_browsing->filelist->content_mode = CONTENT_NORMAL;

    _com_delete_dialog();
    if (g_scene_browsing->filelist->select_mode == true)
    {
        browser_widget_filelist_toggle_select_mode();
    }

    result = com_create_noop_dialog_widget(&dialog_init);
    if (true == result)
    {
        dialog_paint(g_scene_browsing->dialog);
        gui_screen_update();
    }
    sleep(1);
    _com_delete_dialog();

    g_msg_lowpower_received = false;

    g_thread_active = false;

    if(msg_app_quit_received == true)
    {
        _scene_browsing_quit();
    }
}


bool com_create_loadingbox_widget(void)
{
    if (g_scene_browsing->loadingbox == NULL)
    {
        g_scene_browsing->loadingbox = loadingbox_create(LOADINGBOX_SEARCHING, _loadingbox_widget_cb);
        if (NULL == g_scene_browsing->loadingbox)
        {
            print_err("loadingbox_create failed\n\n");
            return false;
        }
        else
        {
            //paint
            loadingbox_paint(g_scene_browsing->loadingbox);
        }
    }
    return true;
}

static void _loadingbox_widget_cb(loadingbox_t *loadingbox, loadingbox_sig_e signal)
{
    print_dbg();
}

bool com_delete_loadingbox_widget(void)
{
    if (g_scene_browsing->loadingbox != NULL)
    {
        loadingbox_delete(g_scene_browsing->loadingbox);
        g_scene_browsing->loadingbox = NULL;
        return true;
    }
    else
    {
        return false;
    }
}

static void _com_noop_dialog_widget_cb(dialog_t * dialog, widget_sig_dialog_e signal)
{
    print_dbg("signal:%d", signal);
}

bool com_create_noop_dialog_widget(dialog_init_t * dialog_init)
{
    if (g_scene_browsing->dialog == NULL)
    {
        g_scene_browsing->dialog = dialog_create(dialog_init, _com_noop_dialog_widget_cb);
        if (g_scene_browsing->dialog == NULL)
        {
            print_err("dialog_create failed\n\n");
            return false;
        }
    }
    return true;
}

void com_create_modal_dialog_widget(void)
{
    g_sem_modal.ret = dialog_modal(&(g_sem_modal.modal));

    sem_post(&g_sem_modal.sem);
}


static void volumebar_cb(volumebar_t *data, volumebar_sig_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_VOLUMEBAR_CHANGED:
        {
            print_dbg("SIG_VOL_CHANGED: %s---%d---\n",__FILE__,__LINE__);
        }
        break;

        case WIDGET_SIG_VOLUMEBAR_QUIT:
        {
            print_dbg("SIG_VOL_DONE: %s---%d---\n",__FILE__,__LINE__);
            volumebar_delete(data);
            g_scene_browsing->vbar = NULL;
        }
        break;

        default:
        break;
    }
}

static void _com_headbar_cb(headbar_t * headbar, widget_sig_headbar_e signal)
{
    print_dbg("signal:0x%x", signal);
}

static bool _com_create_headbar(void)
{
    headbar_init_t headbar_init;

    headbar_init.type = HEADBAR_TYPE_NORMAL;

    headbar_init.str_id_title = g_scene_browsing->str_app_name->str_id;
    headbar_init.pic_id_head = g_scene_browsing->pic_title->pic_id;

    /* init type and callback function */
    g_scene_browsing->headbar = headbar_create(&headbar_init, _com_headbar_cb);
    if (NULL == g_scene_browsing->headbar)
    {
        print_err("headbar_create failed\n\n");
        return false;
    }
    else
    {
        headbar_paint(g_scene_browsing->headbar);
        return true;
    }
}

static bool _com_delete_headbar(void)
{
    if (g_scene_browsing->headbar != NULL)
    {
        headbar_delete(g_scene_browsing->headbar);
        g_scene_browsing->headbar = NULL;
    }
    return true;

}

static void _com_msgbox_cb(msgbox_t * msgbox, msgbox_sig_e signal)
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

        case WIDGET_SIG_MSGBOX_QUIT_LOW_POWER:
        {
            g_msg_lowpower_received = true;
        }
        break;

        default:
        break;
    }
}

static bool _com_create_msgbox(void)
{
    g_scene_browsing->msgbox = msgbox_create(_com_msgbox_cb);
    if (NULL == g_scene_browsing->msgbox)
    {
        print_err("create msgbox failed!");
        return false;
    }
    return true;
}

static bool _com_delete_msgbox(void)
{
    if (g_scene_browsing->msgbox != NULL)
    {
        msgbox_delete(g_scene_browsing->msgbox);
        g_scene_browsing->msgbox = NULL;
    }
    return true;
}


void _com_dialog_cb(dialog_t * dialog, widget_sig_dialog_e signal)
{
    content_mode_e content_mode = g_scene_browsing->filelist->content_mode;
    print_dbg("signal:%d", signal);
    switch (signal)
    {
        case WIDGET_SIG_DIALOG_OK:
        {
            if ((content_mode == CONTENT_MOVE) || (content_mode == CONTENT_COPY))
            {
                dialog_init_t dialog_init;
                bool result;
                _com_delete_dialog();

                dialog_init.head = DIALOG_HEAD_INFO;
                if(content_mode == CONTENT_MOVE)
                {
                    dialog_init.str_id = g_scene_browsing->str_moving->str_id;
                }
                else
                {
                    dialog_init.str_id = g_scene_browsing->str_coping->str_id;
                }
                result = com_create_noop_dialog_widget(&dialog_init);

                if (true == result)
                {
                    dialog_paint(g_scene_browsing->dialog);
                    gui_screen_update();
                }
                sys_forbid_screensaver(true);
                sys_forbid_auto_standby(true);
                sys_forbid_timed_shutdown(true);
                sys_enable_sys_evt_detect(false);

                create_thread(browser_widget_filelist_exec_fileop, NULL);
            }
            else
            {
                print_err();
            }
        }
        break;

        case WIDGET_SIG_DIALOG_CANCAL:
        {
            if (((content_mode == CONTENT_MOVE) || (content_mode == CONTENT_COPY)))
            {
                if ((g_scene_browsing->filelist->in_file_op) == false)
                {
                    _com_delete_dialog();
                    browser_widget_filelist_exit_fileop_mode();
                    g_scene_browsing->filelist->content_mode = CONTENT_NORMAL;
                }
            }
            else
            {
                print_err();
            }
        }
        break;

        default:
        break;
    }
}

static bool _com_create_dialog(dialog_init_t * dialog_init)
{
    if (g_scene_browsing->dialog == NULL)
    {
        g_scene_browsing->dialog = dialog_create(dialog_init, _com_dialog_cb);
        if (g_scene_browsing->dialog == NULL)
        {
            print_err("dialog_create failed\n\n");
            return false;
        }
    }
    return true;
}

static bool _com_delete_dialog(void)
{
    if (g_scene_browsing->dialog != NULL)
    {
        dialog_delete(g_scene_browsing->dialog);
        g_scene_browsing->dialog = NULL;
    }
    return true;
}

static void _com_filelist_cb(filelist_t * filelist, filelist_sig_e sig)
{
    switch (sig)
    {
        case WIDGET_SIG_FILELIST_QUIT:
        {
            if (g_scene_browsing->filelist->content_mode == CONTENT_NORMAL)
            {
                _scene_browsing_quit();
            }
            else
            {
                g_scene_browsing->filelist->content_mode = CONTENT_NORMAL;
                browser_widget_filelist_exit_fileop_mode();
            }
        }
        break;
        case WIDGET_SIG_FILELIST_OK:
        {
            app_ext_t* ext = find_open_app(filelist->file_name);
            print_dbg(" filepath=%s filename = %s", filelist->path_name, filelist->file_name);
            if ((ext != NULL) && (ext->app_name != NULL))
            {
                bool result;
                dialog_init_t dialog_init;

                //msg_apps_t msg_invoke_app;
                memset(&s_msg_invoke_app, 0, sizeof(s_msg_invoke_app));
                strcpy(s_msg_invoke_app.content, ext->app_name);
                strcat(s_msg_invoke_app.content, " ");
                strcat(s_msg_invoke_app.content, ext->scene);
                strcat(s_msg_invoke_app.content, " ");
                strcat(s_msg_invoke_app.content, "\"");
                strcat(s_msg_invoke_app.content, filelist->path_name);
                strcat(s_msg_invoke_app.content, filelist->file_name);
                strcat(s_msg_invoke_app.content, "\"");
                print_dbg("t.content=%s", s_msg_invoke_app.content);

                /*
                 send_async_msg("manager.app", &msg_invoke_app);
                 _com_delete_headbar();
                 browser_widget_filelist_enterbackgroud(g_scene_browsing->filelist);
                 */
                dialog_init.head = DIALOG_HEAD_INFO;
                dialog_init.str_id = g_scene_browsing->str_file_opening->str_id;
                result = _com_create_dialog(&dialog_init);

                if (true == result)
                {
                    dialog_paint(g_scene_browsing->dialog);
                    gui_screen_update();
                }

                set_single_shot_timer(1, (timer_proc) _scene_browsing_invoke_app, NULL);
                //enter_background_mode();
            }
            else
            {
                dialog_init_t init;
                init.head = DIALOG_HEAD_WARN;
                init.str_id = g_scene_browsing->str_file_notsup->str_id;
                dialog_modal(&init);
            }
        }
        break;
        case WIDGET_SIG_FILELIST_SHORT_MENU:
        {
            _com_filelist_shortmenu_cb(filelist);
        }
        break;
        case WIDGET_SIG_FILELIST_DELETE:
        {
            if (filelist->list_inuse == CONTENT)
            {
                if (filelist->content_mode == CONTENT_NORMAL)
                {
                    opt_delete_process(NULL,WIDGET_SIG_MENUVIEW_OK);
                }
            }
        }
        break;
        case WIDGET_SIG_FILELIST_CONTENT_RELOAD:
        {
            browser_browsing_widget_option_delete(g_scene_browsing->opt);

            _com_delete_dialog();

            if (g_scene_browsing->softkey != NULL)
            {
                softkeyboard_delete(g_scene_browsing->softkey);
                g_scene_browsing->softkey = NULL;
            }

            if ((filelist->content_mode == CONTENT_COPY) || (filelist->content_mode == CONTENT_MOVE))
            {
                browser_widget_filelist_exit_fileop_mode();
                filelist->content_mode = CONTENT_NORMAL;
            }
            if(filelist->select_mode == true)
            {
                browser_widget_filelist_toggle_select_mode();
            }
        }
        break;
        default:
        break;
    }
}

static void _com_filelist_shortmenu_cb(filelist_t * filelist)
{

    if (filelist->list_inuse == CONTENT)
    {
        if (filelist->content_mode == CONTENT_NORMAL)
        {
            if (g_scene_browsing->opt == NULL)
            {
                g_scene_browsing->opt = browser_browsing_widget_option_create(NULL);

                if (g_scene_browsing->opt != NULL)
                {
                    fusion_effect_t eff;
                    fusion_get_config(FUSION_TYPE_OPTION_RGB, FUSION_FORWARD, &eff);
                    fusion_set_effect(&eff);
                    _option_fusion_set_first_frame(g_scene_browsing->opt);
                    browser_browsing_widget_option_paint(g_scene_browsing->opt);
                    _option_fusion_set_last_frame(g_scene_browsing->opt);
                    fusion_display( NULL);
                }
                else
                {
                    print_err();
                }
            }
        }
        else if (filelist->content_mode == CONTENT_MOVE)
        {
            dialog_init_t dialog_init;
            dialog_init.head = DIALOG_HEAD_QUERY;
            dialog_init.str_id = g_scene_browsing->str_move_confirm->str_id;
            dialog_init.def_sel=WIDGET_SIG_DIALOG_CANCAL;
            if (_com_create_dialog(&dialog_init) == true)
            {
                dialog_paint(g_scene_browsing->dialog);
            }
        }
        else if (filelist->content_mode == CONTENT_COPY)
        {
            dialog_init_t dialog_init;
            dialog_init.head = DIALOG_HEAD_QUERY;
            dialog_init.str_id = g_scene_browsing->str_copy_confirm->str_id;
            dialog_init.def_sel=WIDGET_SIG_DIALOG_CANCAL;
            if (_com_create_dialog(&dialog_init) == true)
            {
                dialog_paint(g_scene_browsing->dialog);
            }
        }
        else
        {
            print_err();
        }
    }
    else //filelist->list_inuse == ENTRY
    {
        if (filelist->content_mode == CONTENT_MOVE)
        {
            dialog_init_t dialog_init;
            dialog_init.head = DIALOG_HEAD_QUERY;
            dialog_init.str_id = g_scene_browsing->str_move_confirm->str_id;
            dialog_init.def_sel=WIDGET_SIG_DIALOG_CANCAL;
            if (_com_create_dialog(&dialog_init) == true)
            {
                dialog_paint(g_scene_browsing->dialog);
            }
        }
        else if (filelist->content_mode == CONTENT_COPY)
        {
            dialog_init_t dialog_init;
            dialog_init.head = DIALOG_HEAD_QUERY;
            dialog_init.str_id = g_scene_browsing->str_copy_confirm->str_id;
            dialog_init.def_sel=WIDGET_SIG_DIALOG_CANCAL;
            if (_com_create_dialog(&dialog_init) == true)
            {
                dialog_paint(g_scene_browsing->dialog);
            }
        }
        else
        {
            print_err();
        }
    }
}

static bool _com_create_filelist(void)
{
    g_scene_browsing->filelist = browser_widget_filelist_create(_com_filelist_cb);
    if (NULL == g_scene_browsing->filelist)
    {
        print_err("filelist_create failed\n\n");
        return false;
    }
    return true;
}


void _scene_fusion_set_first_frame(scene_t *scene)
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

void _scene_fusion_set_last_frame(scene_t *scene)
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

static void _option_fusion_set_first_frame(menuview_t *mv)
{
    fusion_frame_t frame;
    canvas_info_t info;

    canvas_get_info(mv->canvas_id, &info);
    frame.rect.x = info.rect.x;
    frame.rect.y = info.rect.y;
    frame.rect.width = info.rect.width;
    frame.rect.height = info.rect.height;
    frame.pdata = NULL;
    frame.alpha = false;
    fusion_set_first_frame(&frame);
}

static void _option_fusion_set_last_frame(menuview_t *mv)
{
    fusion_frame_t frame;
    canvas_info_t info;

    canvas_get_info(mv->canvas_id, &info);
    frame.rect.x = info.rect.x;
    frame.rect.y = info.rect.y;
    frame.rect.width = info.rect.width;
    frame.rect.height = info.rect.height;
    frame.pdata = (unsigned char *) info.pdata;
    frame.alpha = true;
    fusion_set_last_frame(&frame);
}


static bool _scene_browsing_open_theme(void)
{
    rotation_e angle;
    char file[PATH_MAX];

    // 取视图角度
    angle = sty_get_view_rotation();

    // 设置屏幕方向
    sty_set_screen_rotation(angle);

    // 载入commonui的 style 场景
    if ((angle == ROTATION_0) || (angle == ROTATION_180))
    {
        g_scene_browsing->common_scene = load_scene(g_app_browser->commonui_style, COMMONUI_SIDELONG_SCENE);

        g_scene_browsing->scene = load_scene(g_app_browser->browser_style, BROWSING_SIDELONG_SCENE);

    }
    else // if ((angle == ROTATION_90 )|| (angle == ROTATION_270))
    {
        g_scene_browsing->common_scene = load_scene(g_app_browser->commonui_style, COMMONUI_UPRIGHT_SCENE);

        g_scene_browsing->scene = load_scene(g_app_browser->browser_style, BROWSING_UPRIGHT_SCENE);
    }

    if ((g_scene_browsing->common_scene == NULL) || (g_scene_browsing->scene == NULL))
    {
        print_err("load commonui and application scene failed");
        return false;
    }
    else
    {
        sty_set_common_scene(g_scene_browsing->common_scene);
        sty_set_app_scene(g_scene_browsing->scene);
    }

    // 获取commonui 的图片资源文件名
    sprintf(file, "%s/%s.res", sys_get_common_theme_res_dir(), "commonui");

    // 打开 commonui 图片资源文件
    if (sty_open_common_pic_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }

    // 获取browser 的图片资源文件名
    sprintf(file, "%s/%s.res", sys_get_app_theme_res_dir(), "browser");

    // 打开应用的图片资源文件
    if (sty_open_app_pic_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }
    return true;
}

static bool _scene_browsing_close_theme(void)
{
    free_scene(g_scene_browsing->common_scene);
    g_scene_browsing->common_scene = NULL;

    free_scene(g_scene_browsing->scene);
    g_scene_browsing->scene = NULL;

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

static bool _scene_browsing_open_str_res(void)
{
    char file[PATH_MAX];

    // 获取commonui 字符串资源文件名
    sprintf(file, "%s/%s_%s.str", sys_get_common_string_res_dir(), "commonui", sys_get_cur_language_codes());
    print_dbg("load %s", file);
    // 打开 commonui 字符串资源文件
    if (sty_open_common_str_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }

    // 获取setting应用字符串资源文件名
    sprintf(file, "%s/%s_%s.str", sys_get_app_string_res_dir(), "browser", sys_get_cur_language_codes());
    print_dbg("load %s", file);
    // 打开应用的字符串资源文件
    if (sty_open_app_str_res(file) == false)
    {
        print_err("open %s failed!", file);
        return false;
    }

    // 设置语言编码
    sty_set_text_encoding(GUI_UTF8_ENC);
    gui_set_language_mode(sys_get_cur_language_id());
    return true;
}

static bool _scene_browsing_close_str_res(void)
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

static bool _scene_browsing_set_default_font(void)
{
    /* crate font */
    if (g_scene_browsing->fontdes > 0)
    {
        return false;
    }

    g_scene_browsing->fontdes = gui_create_font(sys_get_default_font_file(), 16);
    if (g_scene_browsing->fontdes < 0)
    {
        print_err("call gui_create_font failed");
        return false;
    }

    if (gui_dc_set_default_fontface(g_scene_browsing->fontdes) < 0)
    {
        print_err("gui_dc_set_default_fontface failed");
        return false;
    }

    return true;
}

static bool _scene_browsing_unset_default_font(void)
{
    /* crate font */
    if (g_scene_browsing->fontdes <= 0)
    {
        return false;
    }

    gui_destroy_font(g_scene_browsing->fontdes);
    g_scene_browsing->fontdes = 0;

    return true;
}

static void _scene_browsing_load_resources(void)
{
    resgroup_resource_t *option_group;

    //load resource
    g_scene_browsing->pic_bg = (picture_resource_t *) get_scene_child(g_scene_browsing->scene, PIC_BG);
    g_scene_browsing->pic_title = (picture_resource_t *) get_scene_child(g_scene_browsing->scene, PIC_TITLE);
    g_scene_browsing->str_app_name = (string_resource_t *) get_scene_child(g_scene_browsing->scene, STR_APP_NAME);
    g_scene_browsing->str_low_power = (string_resource_t *) get_scene_child(g_scene_browsing->scene, STR_LOW_POWER);
    g_scene_browsing->str_no_space = (string_resource_t *) get_scene_child(g_scene_browsing->scene, STR_NO_SPACE);

    option_group = (resgroup_resource_t *) get_scene_child(g_scene_browsing->scene, OPTION_RESGROUP);

    g_scene_browsing->str_move_confirm = (string_resource_t *) get_resgroup_child(option_group, STR_MOVE_CONFIRM);
    g_scene_browsing->str_moving = (string_resource_t *) get_resgroup_child(option_group, STR_MOVING);
    g_scene_browsing->str_move_succeed = (string_resource_t *) get_resgroup_child(option_group, STR_MOVE_SUCCEED);
    g_scene_browsing->str_move_failed = (string_resource_t *) get_resgroup_child(option_group, STR_MOVE_FAILED);

    g_scene_browsing->str_copy_confirm = (string_resource_t *) get_resgroup_child(option_group, STR_COPY_CONFIRM);
    g_scene_browsing->str_coping = (string_resource_t *) get_resgroup_child(option_group, STR_COPING);
    g_scene_browsing->str_copy_succeed = (string_resource_t *) get_resgroup_child(option_group, STR_COPY_SUCCEED);
    g_scene_browsing->str_copy_failed = (string_resource_t *) get_resgroup_child(option_group, STR_COPY_FAILED);

    g_scene_browsing->str_delete_confirm = (string_resource_t *) get_resgroup_child(option_group, STR_DELETE_CONFIRM);
    g_scene_browsing->str_deleting = (string_resource_t *) get_resgroup_child(option_group, STR_DELETING);
    g_scene_browsing->str_delete_succeed = (string_resource_t *) get_resgroup_child(option_group, STR_DELETE_SUCCEED);
    g_scene_browsing->str_delete_failed = (string_resource_t *) get_resgroup_child(option_group, STR_DELETE_FAILED);

    g_scene_browsing->str_replace_comfirm = (string_resource_t *) get_resgroup_child(option_group, STR_REPLACE_CONFIRM);
    g_scene_browsing->str_subdir_err = (string_resource_t *) get_resgroup_child(option_group, STR_SUBDIR_ERR);

    g_scene_browsing->str_file_opening = (string_resource_t *) get_resgroup_child(option_group, STR_FILE_OPENING);

    g_scene_browsing->str_file_notsup = (string_resource_t *) get_resgroup_child(option_group, STR_FILE_NOTSUP);

}

static void _scene_browsing_rotate(void)
{

    fusion_effect_t eff;
    fusion_get_config(FUSION_TYPE_ROTATE, FUSION_FORWARD, &eff);
    fusion_set_effect(&eff);
    fusion_set_first_frame(NULL);

    _scene_browsing_close_theme();

    _scene_browsing_open_theme();

    _scene_browsing_close_str_res();

    _scene_browsing_open_str_res();

    _scene_browsing_load_resources();

    gui_wm_set_size(g_scene_browsing->win_id, g_scene_browsing->scene->width, g_scene_browsing->scene->height);

    if (NULL != g_scene_browsing->bg_bitmap_info)
    {
        sty_delete_bitmap(g_scene_browsing->bg_bitmap_info);
        g_scene_browsing->bg_bitmap_info = NULL;
    }
    g_scene_browsing->bg_bitmap_info = get_user_bg_pic(BG_PHOTO);
    if (NULL == g_scene_browsing->bg_bitmap_info)
    {
        g_scene_browsing->bg_bitmap_info = sty_create_bitmap((int) g_scene_browsing->pic_bg->pic_id);
    }
    if (NULL == g_scene_browsing->bg_bitmap_info)
    {
        print_err();
    }

    _scene_browsing_paint();

    if (g_scene_browsing->filelist != NULL)
    {
        browser_widget_filelist_load_style(g_scene_browsing->filelist);
        browser_widget_filelist_paint(g_scene_browsing->filelist);
    }

    if (g_scene_browsing->headbar != NULL)
    {
        headbar_load_style(g_scene_browsing->headbar);
        headbar_paint(g_scene_browsing->headbar);
    }

    if (g_scene_browsing->softkey != NULL)
    {
        softkeyboard_load_style(g_scene_browsing->softkey);
        softkeyboard_paint(g_scene_browsing->softkey);
    }

    if (g_scene_browsing->dialog != NULL)
    {
        dialog_load_style(g_scene_browsing->dialog);
        dialog_paint(g_scene_browsing->dialog);
    }

    if (g_scene_browsing->vbar != NULL)
    {
        volumebar_load_style(g_scene_browsing->vbar);
        volumebar_paint(g_scene_browsing->vbar);
    }

    if (g_scene_browsing->msgbox != NULL)
    {
        msgbox_load_style(g_scene_browsing->msgbox);
        msgbox_paint(g_scene_browsing->msgbox);
    }

    if (g_scene_browsing->opt != NULL)
    {
        menuview_load_style(g_scene_browsing->opt, MENUVIEW_OPTION);
        menuview_paint(g_scene_browsing->opt);
    }

    gui_framebuffer_update();

    fusion_set_last_frame(NULL);


    fusion_display(NULL);

}



static int _scene_browsing_init(void)
{
    fusion_effect_t eff;
    fusion_get_config(FUSION_TYPE_APP_CHANGE, s_enter_type, &eff);
    fusion_set_effect(&eff);

    sem_init(&g_sem_modal.sem, PTHREAD_PROCESS_PRIVATE, 0);

    if (g_scene_browsing == NULL)
    {
        g_scene_browsing = malloc(sizeof(scene_browsing_t));

        if (g_scene_browsing == NULL)
        {
            print_err();
            return -1;
        }
        else
        {
            memset(g_scene_browsing, 0, sizeof(scene_browsing_t)); //clear
        }
    }

    /* load theme 资源文件 */
    if (_scene_browsing_open_theme() == false)
    {
        return -1;
    }

    /* load 字符串资源文件 */
    if (_scene_browsing_open_str_res() == false)
    {
        return -1;
    }

    /* 创建字体 */
    /*
     if (gui_create_font("attfv1.ttf", 16) < 0)
     {
     print_err("call gui_create_font failed");
     return false;
     }
     */
    _scene_browsing_set_default_font();

    /* load 场景资源 */
    _scene_browsing_load_resources();

    g_scene_browsing->win_id = gui_wm_create_window(g_scene_browsing->scene->x, g_scene_browsing->scene->y,
            g_scene_browsing->scene->width, g_scene_browsing->scene->height, WM_CREATE_SHOW,
            _scene_browsing_cb, 0);
    if (g_scene_browsing->win_id == 0)
    {
        print_err("");
        return -1;
    }
    gui_wm_set_focus(g_scene_browsing->win_id);

    g_scene_browsing->bg_bitmap_info = get_user_bg_pic(BG_PHOTO);

    if (NULL == g_scene_browsing->bg_bitmap_info)
    {
        /*获取用户自定义背景图失败，使用ui-editor上添加的默认图*/
        g_scene_browsing->bg_bitmap_info = sty_create_bitmap((int) g_scene_browsing->pic_bg->pic_id);
    }

    if (NULL == g_scene_browsing->bg_bitmap_info)
    {
        print_err();
        return -1;
    }

    _scene_fusion_set_first_frame(g_scene_browsing->scene);

    _com_create_msgbox();

    _com_create_headbar();

    _com_create_filelist();

    headbar_paint(g_scene_browsing->headbar);
    //browser_widget_filelist_paint(g_scene_browsing->filelist);
    _scene_browsing_paint(); //draw static window bg

    _scene_fusion_set_last_frame(g_scene_browsing->scene);
    fusion_display(NULL);

    clear_key_msg_queue(-1,-1);

    return 0;
}

static int _scene_browsing_deinit(void)
{
    _scene_browsing_unset_default_font();

    _scene_browsing_close_theme();
    _scene_browsing_close_str_res();

    if (g_scene_browsing == NULL)
    {
        return -1;
    }

    if (g_scene_browsing->win_id != 0)
    {
        gui_wm_delete_window(g_scene_browsing->win_id);
    }

    if (g_scene_browsing->bg_bitmap_info != NULL)
    {
        sty_delete_bitmap(g_scene_browsing->bg_bitmap_info);
        g_scene_browsing->bg_bitmap_info = NULL;
    }

    _com_delete_headbar();

    _com_delete_msgbox();

    browser_widget_filelist_delete(g_scene_browsing->filelist);

    browser_browsing_widget_option_delete(g_scene_browsing->opt);

    _com_delete_dialog();

    if(g_scene_browsing->vbar != NULL)
    {
        volumebar_delete(g_scene_browsing->vbar);
        g_scene_browsing->vbar = NULL;
    }

    if (g_scene_browsing->softkey != NULL)
    {
        softkeyboard_delete(g_scene_browsing->softkey);
        g_scene_browsing->softkey = NULL;
    }

    free(g_scene_browsing);
    g_scene_browsing = NULL;

    sem_destroy(&g_sem_modal.sem);

    return 0;
}

static void _scene_browsing_quit(void)
{
    exit_msg_loop();
    g_scene_browsing->exit_type = SCENE_EXIT;
}


static bool _scene_browsing_paint(void)
{
    //获取dc
    int hdc = gui_dc_get((unsigned short)g_scene_browsing->win_id);

    //draw static main window bg
    //sty_draw_picture_res(hdc, g_scene_browsing->pic_bg);

    sty_draw_bitmap(hdc, g_scene_browsing->bg_bitmap_info, 0, 0);

    return true;
}

static bool _scene_proc_gui(scene_browsing_t * data, gui_wm_message_t * pmsg)
{
    key_event_t *kmsg;
    //menu_t *cur_menu;


    bool result = false;

    switch (pmsg->msgid)
    {
        case WM_MSG_CREATE:
        {
            print_dbg();
        }
        break;

        case WM_MSG_PAINT:
        {
            print_dbg();
        }
        break;

        case WM_MSG_KEY:
        {
            kmsg = (key_event_t *) (pmsg->data.p);
            switch (kmsg->val | kmsg->type)
            {
                case KEY_OK | KEY_TYPE_DOWN:
                {
                }
                break;
                case KEY_RETURN | KEY_TYPE_DOWN:
                {
                    //                if(g_scene_browsing->filelist == NULL);
                    //                scene_browsing_quit();
                }
                break;
                case KEY_VOL | KEY_TYPE_DOWN:
                case KEY_VSUB | KEY_TYPE_DOWN:
                case KEY_VADD | KEY_TYPE_DOWN:
                {
                    if (data->vbar == NULL)
                    {
                        data->vbar = volumebar_create(0, volumebar_cb);
                        if (data->vbar == NULL)
                        {
                            print_err();
                        }
                        else
                        {
                            volumebar_paint(data->vbar);
                        }
                        result = true;
                    }
                }
                break;
                case KEY_MENU | KEY_TYPE_DOWN:
                {
                }
                break;
                default:
                {
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
    return result;
}

static int _scene_proc_sys_msg(msg_apps_t * msg)
{
    msg_reply_t reply;

    //    print_dbg("msgcontent:%s", msg->content); //注意打印content内容超长越界的情况
    print_info("msgtype:%d, content[0]:0x%x", msg->type, msg->content[0]);
    memset(&reply, 0x00, sizeof(reply));

    switch (msg->type)
    {
        case MSG_ACTIVE_SYNC:
        {
            reply.type = MSG_REPLY_SUCCESS;
            reply_sync_msg((const msg_reply_t *) &reply);
        }
        break;
        case MSG_SET_APP_FOREGROUND:
        {
            /*
            _com_create_headbar();
            browser_widget_filelist_enterforegroud(g_scene_browsing->filelist);
            clear_key_msg_queue(-1,-1);
            enter_foreground_mode();
            gui_wm_set_focus(g_scene_browsing->win_id);
            _scene_browsing_paint();
            headbar_paint(g_scene_browsing->headbar);
            browser_widget_filelist_paint(g_scene_browsing->filelist);
            */
        }
        break;

        case MSG_GSENSOR_ROTATION_CHANGED:
        {
            _scene_browsing_rotate();
        }
        break;

        case MSG_BACKGROUND_PIC_CHANGE:
        {
            if (g_scene_browsing->bg_bitmap_info != NULL)
            {
                sty_delete_bitmap(g_scene_browsing->bg_bitmap_info);
                g_scene_browsing->bg_bitmap_info = NULL;
            }
            g_scene_browsing->bg_bitmap_info = get_user_bg_pic(BG_PHOTO);

            if (NULL == g_scene_browsing->bg_bitmap_info)
            {
                /*获取用户自定义背景图失败，使用ui-editor上添加的默认图*/
                g_scene_browsing->bg_bitmap_info = sty_create_bitmap((int) g_scene_browsing->pic_bg->pic_id);
            }
        }
        break;

        case MSG_APP_QUIT:
        {
            msg_app_quit_received = true;
            if(g_thread_active == false)
            {
                _scene_browsing_quit();
            }
        }
        break;

        default:
        {
            if ((msg->type == MSG_LOW_VOLTAGE) && (g_low_power_dialog_visible == true))
            {
                break;
            }

            if (g_scene_browsing->filelist != NULL)
            {
                browser_widget_filelist_proc_sys(g_scene_browsing->filelist, msg);
            }

            if (g_scene_browsing->msgbox != NULL)
            {
                /* 控件msgbox用来处理默认系统消息(低电提示、卡插入和拔出提示、USB插入功能选择提示等) */
                msgbox_proc_sys(g_scene_browsing->msgbox, msg);
            }
            break;
        }

    }
    return true;
}


static void _scene_browsing_cb(gui_wm_message_t * pmsg)
{

    //top canvas process
    int top = canvas_get_focus(pmsg);

    //dispatch msg
    if (_scene_proc_gui(g_scene_browsing, pmsg) == true)
    {
        return;
    }

    if ((NULL != g_scene_browsing->filelist) && (top == g_scene_browsing->filelist->canvas_id))
    {
        browser_widget_filelist_proc_gui(g_scene_browsing->filelist, pmsg);
        return;
    }
    if ((g_scene_browsing->dialog != NULL) && (top == g_scene_browsing->dialog->canvas_id))
    {
        dialog_proc_gui(g_scene_browsing->dialog, pmsg);
        return;
    }
    if ((g_scene_browsing->softkey != NULL) && (top == g_scene_browsing->softkey->canvas_id))
    {
        softkeyboard_proc_gui(g_scene_browsing->softkey, pmsg);
        return;
    }
    if ((g_scene_browsing->opt != NULL) && (top == g_scene_browsing->opt->canvas_id))
    {
        browser_browsing_widget_option_proc_gui(g_scene_browsing->opt, pmsg);
        return;
    }
    if ((g_scene_browsing->vbar != NULL) && (top == g_scene_browsing->vbar->canvas_id))
    {
        volumebar_proc_gui(g_scene_browsing->vbar, pmsg);
        return;
    }
    if ((g_scene_browsing->loadingbox != NULL) && (top == g_scene_browsing->loadingbox->canvas_id))
    {
        loadingbox_proc_gui(g_scene_browsing->loadingbox, pmsg);
        return;
    }
    if ((g_scene_browsing->msgbox != NULL) && (top == g_scene_browsing->msgbox->canvas_id))
    {
        msgbox_proc_gui(g_scene_browsing->msgbox, pmsg);
        return;
    }
    if ((NULL != g_scene_browsing->headbar) && (top == g_scene_browsing->headbar->canvas_id))
    {
        headbar_proc_gui(g_scene_browsing->headbar, pmsg);
        return;
    }
}


static void _scene_browsing_invoke_app(void* param)
{
    browser_widget_filelist_enterbackgroud(g_scene_browsing->filelist);

    s_msg_invoke_app.type = MSG_CREAT_APP_RESUME;
    send_async_msg("manager.app", &s_msg_invoke_app);
}


scene_status_t browser_scene_browsing(fusion_enter_e enter_type)
{
    int ret;
    msg_apps_t msg;

    s_enter_type = enter_type;

    ret = _scene_browsing_init();
    if (ret < 0)
    {
        print_dbg("scene init error!\n");
        return SCENE_EXIT;
    }
    else
    {
        print_dbg("browser scene init success!\n");
    }

    register_sys_dispatcher(_scene_proc_sys_msg);
    while (get_msg(&msg) == true)
    {
        dispatch_msg(&msg);
    }
    unregister_sys_dispatcher();

    ret = _scene_browsing_deinit();
    if (ret < 0)
    {
        print_dbg("scene exit error! \n");
        return SCENE_EXIT;
    }
    return SCENE_EXIT;
}
