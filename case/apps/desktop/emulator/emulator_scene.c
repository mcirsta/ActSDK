#include "emulator_scene.h"
#include "emulator_play.h"
#include "em_first_level_menu.h"
#include "em_second_level_menu.h"
#include "em_fourth_level_menu.h"

int sys_msg_type = 0;
fusion_enter_e enter_type = FUSION_FORWARD;
scene_emulator_t* g_scene_emulator = NULL;
bool msgbox_exist = false; //msg_box生命期的标志
extern emu_state_t emu_state; /*模拟器当前状态:EMU_STAT_STOP,EMU_STAT_RUNNING,EMU_STAT_PAUSE*/
extern int start_byFileNavi;
extern style_t* app_style;
extern style_t* commonui_style;
extern int deviceSel;


static int _proc_sys_msg(msg_apps_t *msg);
static bool _emulator_scene_init(void);
static bool _emulator_scene_deinit(void);

static bool _scene_proc_gui(gui_wm_message_t* pmsg);
static bool _scene_proc_key(key_event_t *key_event);

static bool _emulator_scene_init_paint(scene_emulator_t *my_scene);
static bool _scene_emulator_load_resources(void);

static void _headbar_cb(headbar_t *headbar, int signal);

static bool _create_msgbox(void);
static void _msgbox_cb(msgbox_t * msgbox, msgbox_sig_e signal);
static bool _delete_msgbox(void);

static bool _create_volumebar(void);
static void _volumebar_cb(volumebar_t *volumebar, volumebar_sig_e signal);
static bool _delete_volumebar(void);

static void _emulator_scene_callback(gui_wm_message_t *pmsg);

static bool _scene_emulator_open_theme(void);
static bool _scene_emulator_close_theme(void);
static bool _scene_emulator_open_str_res(void);
static bool _scene_emulator_close_str_res(void);

static bool _scene_emulator_rotate(void);
static bool _scene_set_default_font(void);
static bool _scene_unset_default_font(void);

static void _menuview_callback(menuview_t *menuview, menuview_sig_e signal);
static bool _set_keymap_menuview_proc_gui(gui_wm_message_t* pmsg);





static bool _scene_set_default_font(void)
{
    /* crate font */
    if (g_scene_emulator->fontdes > 0)
    {
        return false;
    }

    g_scene_emulator->fontdes = gui_create_font(sys_get_default_font_file(), 16);
    if (g_scene_emulator->fontdes < 0)
    {
        return false;
    }

    if (gui_dc_set_default_fontface(g_scene_emulator->fontdes) < 0)
    {
        return false;
    }

    return true;
}

static bool _scene_unset_default_font(void)
{
    /* crate font */
    if (g_scene_emulator->fontdes <= 0)
    {
        return false;
    }

    gui_destroy_font(g_scene_emulator->fontdes);
    g_scene_emulator->fontdes = 0;

    return true;
}

/* scene paint function */
static bool _emulator_scene_init_paint(scene_emulator_t *my_scene)
{
    /* draw static main window bg */
    //sty_draw_picture_res( my_scene->scene_common.handle_gui_dc, my_scene->bg_pic);
    sty_draw_bitmap(my_scene->scene_common.handle_gui_dc, my_scene->bg_bitmap_info, 0, 0);

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

bool em_create_headbar(void)
{
    headbar_init_t headbar_init;

    headbar_init.type = HEADBAR_TYPE_NORMAL;
    headbar_init.str_id_title = g_scene_emulator->title_str->str_id;
    headbar_init.pic_id_head = g_scene_emulator->icon_pic->pic_id;

    /* init type and callback function */
    g_scene_emulator->headbar = headbar_create(&headbar_init, _headbar_cb);
    if (NULL == g_scene_emulator->headbar)
    {
        return false;
    }

    headbar_paint(g_scene_emulator->headbar);

    return true;

}

bool em_delete_headbar(void)
{
    bool result = true;

    if (g_scene_emulator->headbar != NULL)
    {
        result = headbar_delete(g_scene_emulator->headbar);
        g_scene_emulator->headbar = NULL;
    }
    return result;
}

static bool _create_msgbox(void)
{
    g_scene_emulator->msgbox = msgbox_create(_msgbox_cb);
    if (NULL == g_scene_emulator->msgbox)
    {
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
        //如果正在游戏时收到 msg_box 的要求，就暂停游戏，等msg_box退出后再继续游戏
        if ((g_scene_emulator->thread_exist == 1) && (emu_state == EMU_STAT_PAUSE) && (msgbox_exist == true))
        {
            msgbox_exist = false;
            emu_continue();
            msg_key_server_enable(0);
        }  
        break;
        
        default:
        break;
    }
}

static bool _delete_msgbox(void)
{
    bool result = true;

    if (g_scene_emulator->msgbox != NULL)
    {
        result = msgbox_delete(g_scene_emulator->msgbox);
        g_scene_emulator->msgbox = NULL;
    }
    return result;
}

static bool _create_volumebar(void)
{
    if (g_scene_emulator->volumebar != NULL)
    {
        return false;
    }

    g_scene_emulator->volumebar = volumebar_create(0, (volumebar_cb_t) _volumebar_cb);
    if (g_scene_emulator->volumebar == NULL)
    {
        //print_err("create volumebar error");
    }
    else
    {
        volumebar_paint(g_scene_emulator->volumebar);
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
    if (g_scene_emulator->volumebar != NULL)
    {
        volumebar_delete(g_scene_emulator->volumebar);
        g_scene_emulator->volumebar = NULL;
    }

    return true;
}

bool em_create_menuview(void)
{
    if (g_scene_emulator->menuview == NULL)
    {//MENUVIEW_MENULIST_TEXT
        g_scene_emulator->menuview = menuview_create(MENUVIEW_FILELIST_TEXT, _menuview_callback);
    }
    else
    {
        return false;
    }
    if (g_scene_emulator->menuview == NULL)
    {
        return false;
    }
    return true;
}

bool em_delete_menuview(void)
{
    bool result = true;

    if (g_scene_emulator->menuview != NULL)
    {
        result = menuview_delete(g_scene_emulator->menuview);
    }
    if (false == result)
    {
        //print_warning("delete menuview failed!");
    }
    g_scene_emulator->menuview = NULL;

    return result;
}

static void _menuview_callback(menuview_t *menuview, menuview_sig_e signal)
{
    //print_info("======== _menuview_callback =============\n");
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

        default:
        break;
    }
    return result;
}

static bool _scene_proc_gui(gui_wm_message_t* pmsg)
{
    bool result = false;

    switch (pmsg->msgid)
    {
        case WM_MSG_CREATE:
        break;

        case WM_MSG_KEY:
        if (g_scene_emulator->thread_exist == 1)
        {
            //deal_key_when_playing((key_event_t *) (pmsg->data.p));
        }
        else
        {
            _scene_proc_key((key_event_t *) (pmsg->data.p));
        }
        result = true;
        break;

        default:
        gui_wm_default_callback(pmsg);
        break;
    }
    return result;
}

static bool _set_keymap_menuview_proc_gui(gui_wm_message_t* pmsg)
{
    bool result = false;

    switch (pmsg->msgid)
    {
        case WM_MSG_KEY:
        set_keymap_proc_key((key_event_t *) (pmsg->data.p));
        result = true;
        break;

        default:
        gui_wm_default_callback(pmsg);
        break;
    }
    return result;
}

/* scene callback function */
static void _emulator_scene_callback(gui_wm_message_t *pmsg)
{
    int top;
    
    /*但在第四级菜单（设置按键）时，不调用menuvie的回调，自己处理nemu的动作*/
    if (g_scene_emulator->cur_menu == 4)
    {
        top = canvas_get_focus(pmsg); 
        //处理 "确定修改按键" 对话框的dialog公共回调函数
        if ((NULL != g_scene_emulator->dlg) && (top == g_scene_emulator->dlg->canvas_id))
        {
            dialog_proc_gui(g_scene_emulator->dlg, pmsg);
        } 
        else
        {      	
            _set_keymap_menuview_proc_gui(pmsg);
        }
        goto clear_key;
    }
    
    /* 如果在UI界面，就响应Vol键 */    
    if ((pmsg->msgid == WM_MSG_KEY) && (emu_state != EMU_STAT_RUNNING) )
    {
        key_event_t* key_event = (key_event_t*)(pmsg->data.p);
        if (key_event->type == KEY_TYPE_DOWN)
        {
            if ((key_event->val == KEY_VSUB) || (key_event->val == KEY_VADD) || (key_event->val == KEY_VOL))
            {
                _create_volumebar();	
            }
        }	 
    }

    /* top canvas process */
    top = canvas_get_focus(pmsg);


    
    /* dispatch msg */
    if ((NULL != g_scene_emulator->adjust) && (top == g_scene_emulator->adjust->canvas_id))
    {
        adjust_proc_gui(g_scene_emulator->adjust, pmsg);
        goto clear_key;
    }    
    if ((NULL != g_scene_emulator->dlg) && (top == g_scene_emulator->dlg->canvas_id))
    {
        dialog_proc_gui(g_scene_emulator->dlg, pmsg);
        goto clear_key;
    }
    if ((NULL != g_scene_emulator->volumebar) && (top == g_scene_emulator->volumebar->canvas_id))
    {
        volumebar_proc_gui(g_scene_emulator->volumebar, pmsg);
        goto clear_key;
    }     
    if ((g_scene_emulator->menuview != NULL) && (top == g_scene_emulator->menuview->canvas_id))
    {
        menuview_proc_gui(g_scene_emulator->menuview, pmsg);
        goto clear_key;
    }   
    if ((NULL != g_scene_emulator->dirlist) && (top == g_scene_emulator->dirlist->canvas_id))
    {
        dirlist_proc_gui(g_scene_emulator->dirlist, pmsg);
        goto clear_key;
    }    
    if ((g_scene_emulator->msgbox != NULL) && (top == g_scene_emulator->msgbox->canvas_id))
    {
        /* 处理默认系统消息(低电提示、卡插入和拔出提示、USB插入功能选择提示等)的msgbox控件需要接受GUI消息 */
        msgbox_proc_gui(g_scene_emulator->msgbox, pmsg);
        goto clear_key;
    }
    
    _scene_proc_gui(pmsg);  
    
    clear_key: 
    print_dbg("---------\n");
    //clear_key_msg_queue(-1, -1);
}

static bool _scene_emulator_load_resources(void)
{
    scene_emulator_t *my_scene = g_scene_emulator;

    my_scene->bg_pic = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PIC_BG );
    my_scene->icon_pic = (picture_resource_t *) get_scene_child(my_scene->scene_common.scene, PIC_ICON);
    my_scene->title_str = (string_resource_t *) get_scene_child(my_scene->scene_common.scene, STR_TITLE);

    my_scene->restart_str = (string_resource_t *) get_scene_child(my_scene->scene_common.scene, STR_RESTART);
    my_scene->load_str = (string_resource_t *) get_scene_child(my_scene->scene_common.scene, STR_LOAD);
    my_scene->save_str = (string_resource_t *) get_scene_child(my_scene->scene_common.scene, STR_SAVE);
    my_scene->save_err_str = (string_resource_t *) get_scene_child(my_scene->scene_common.scene, STR_SAVE_ERR);    
    my_scene->set_str = (string_resource_t *) get_scene_child(my_scene->scene_common.scene, STR_SETTING);
    my_scene->quit_str = (string_resource_t *) get_scene_child(my_scene->scene_common.scene, STR_QUIT);

    my_scene->sound_str = (string_resource_t *) get_scene_child(my_scene->scene_common.scene, STR_SOUND);
    my_scene->key_str = (string_resource_t *) get_scene_child(my_scene->scene_common.scene, STR_KEY);
    my_scene->disp_str = (string_resource_t *) get_scene_child(my_scene->scene_common.scene, STR_DISP);
    my_scene->no_file_str = (string_resource_t *) get_scene_child(my_scene->scene_common.scene, STR_NO_FILE);
    my_scene->full_screen_str = (string_resource_t *) get_scene_child(my_scene->scene_common.scene, STR_FULL_SCREEN);
    my_scene->saved_str = (string_resource_t *) get_scene_child(my_scene->scene_common.scene, STR_HAS_SAVED);
    my_scene->modify_str = (string_resource_t *) get_scene_child(my_scene->scene_common.scene, STR_SURE_MODIFY);
    my_scene->failopen_str = (string_resource_t *) get_scene_child(my_scene->scene_common.scene, STR_FAIL_OPEN);
    my_scene->nosupp_str = (string_resource_t *) get_scene_child(my_scene->scene_common.scene, STR_NOSUPP);  
    
    my_scene->nodisk_str = (string_resource_t *) get_scene_child(my_scene->scene_common.scene, STR_NODISK);
      
    return true;
}

/* scene initial function */
static bool _emulator_scene_init(void)
{
    scene_emulator_t *my_scene;
    hotplugin_dev_type_e main_disk ; 
    dialog_init_t dialog_init;
    
    /* 分配场景数据空间 */
    if (g_scene_emulator == NULL)
    {
        g_scene_emulator = malloc(sizeof(scene_emulator_t));

        if (g_scene_emulator == NULL)
        {
            //print_err();
            return false;
        }
        else
        {
            memset(g_scene_emulator, 0, sizeof(scene_emulator_t)); //clear
            my_scene = g_scene_emulator;
        }
    }
    else
    {
        my_scene = g_scene_emulator;
    }

    /* load theme 资源文件 */
    if (_scene_emulator_open_theme() == false)
    {
        return false;
    }

    /* load 字符串资源文件 */
    if (_scene_emulator_open_str_res() == false)
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
    _scene_emulator_load_resources();

    //判断主盘是否存在
    main_disk = sys_get_main_disk();
    if(main_disk == HOTPLUGIN_DEV_TYPE_NONE)
    {
        //模态提示框
        if (my_scene->nodisk_str != NULL)
        {
            dialog_init.head = DIALOG_HEAD_WARN;
            dialog_init.str_id = my_scene->nodisk_str->str_id;
            dialog_modal( &dialog_init );
        }
        return false;
    }
    
    g_scene_emulator->scene_common.win_id = gui_wm_create_window(g_scene_emulator->scene_common.scene->x,
            g_scene_emulator->scene_common.scene->y, g_scene_emulator->scene_common.scene->width,
            g_scene_emulator->scene_common.scene->height, WM_CREATE_SHOW, _emulator_scene_callback, 0);
    if (g_scene_emulator->scene_common.win_id == 0)
    {
        //print_err("call gui_wm_create_window failed");
        return false;
    }

    /* 设置焦点 */
    gui_wm_set_focus(g_scene_emulator->scene_common.win_id);
    g_scene_emulator->scene_common.handle_gui_dc = gui_dc_get((unsigned short)g_scene_emulator->scene_common.win_id);

    g_scene_emulator->bg_bitmap_info = get_user_bg_pic(BG_EMULATOR);
    if (NULL == g_scene_emulator->bg_bitmap_info)
    {
        g_scene_emulator->bg_bitmap_info = sty_create_bitmap((int) g_scene_emulator->bg_pic->pic_id);
    }
    if (NULL == g_scene_emulator->bg_bitmap_info)
    {
        //print_err("bg_bitmap_info is NULL. \n");
        return false;
    }

    _emulator_scene_init_paint(g_scene_emulator);

    if (em_create_headbar() == false)
    {
        //print_err("create headbar failed!");
        return false;
    }

    /* create menu */
    if (em_create_menuview() == false)
    {
        //print_err("menuview create fail!");
        return false;
    }

    if (_create_msgbox() == false)
    {
        //print_err("create msgbox failed!");
        return false;
    }

    /* 控制变量初始化 */
    my_scene->cur_item01 = 0;
    my_scene->cur_item02 = 0;
    my_scene->cur_item03 = 0;
    my_scene->thread_exist = 0;
    my_scene->cur_menu = 1;
    emu_state = EMU_STAT_STOP;
    enter_type = FUSION_FORWARD;
    msgbox_exist = false;

    sys_set_key_tone_enable(0); //按键音使能禁止接口
    
    if (start_byFileNavi == 0)
    {
        em_dirlist_enter(g_scene_emulator, 0);
    }
    else
    {
        em_second_level_menu_enter(g_scene_emulator);
    }
    return true;
}

static bool _scene_emulator_open_theme(void)
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
        g_scene_emulator->scene_common.common_scene = load_scene(commonui_style, COMMONUI_SIDELONG_SCENE);

        g_scene_emulator->scene_common.scene = load_scene(app_style, EMULATOR_SIDELONG_SCENE);
    }
    else //if (angle == ROTATION_90 || angle == ROTATION_270)
    {
        g_scene_emulator->scene_common.common_scene = load_scene(commonui_style, COMMONUI_UPRIGHT_SCENE);

        g_scene_emulator->scene_common.scene = load_scene(app_style, EMULATOR_UPRIGHT_SCENE);
    }

    if ((g_scene_emulator->scene_common.common_scene == NULL) || (g_scene_emulator->scene_common.scene == NULL))
    {
        //print_err("load commonui scene failed");
        return false;
    }
    else
    {
        sty_set_common_scene(g_scene_emulator->scene_common.common_scene);
        sty_set_app_scene(g_scene_emulator->scene_common.scene);
    }

    /* 获取commonui 的图片资源文件名 */
    sprintf(file, "%s/%s.res", sys_get_common_theme_res_dir(), "commonui");
    if (sty_open_common_pic_res(file) == false)
    {
        //print_err("open %s failed!", file);
        return false;
    }

    /* 获取commonui 的图片资源文件名 */
    sprintf(file, "%s/%s.res", sys_get_app_theme_res_dir(), "emulator");
    if (sty_open_app_pic_res(file) == false)
    {
        //print_err("open %s failed!", file);
        return false;
    }

    return true;
}

static bool _scene_emulator_close_theme(void)
{
    /* 释放场景资源 */
    free_scene(g_scene_emulator->scene_common.scene);
    g_scene_emulator->scene_common.scene = NULL;

    /* 释放commonui的场景资源*/
    free_scene(g_scene_emulator->scene_common.common_scene);
    g_scene_emulator->scene_common.common_scene = NULL;

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

static bool _scene_emulator_open_str_res(void)
{
    char file[512];

    /* 获取commonui 字符串资源文件名 */
    sprintf(file, "%s/%s_%s.str", sys_get_common_string_res_dir(), "commonui", sys_get_cur_language_codes());
    //print_dbg("load %s", file);
    /* 打开 commonui 字符串资源文件 */
    if (sty_open_common_str_res(file) == false)
    {
        //print_err("open %s failed!", file);
        return false;
    }

    /* 获取setting应用字符串资源文件名 */
    sprintf(file, "%s/%s_%s.str", sys_get_app_string_res_dir(), "emulator", sys_get_cur_language_codes());
    //print_dbg("load %s", file);
    /* 打开应用的字符串资源文件 */
    if (sty_open_app_str_res(file) == false)
    {
        //print_err("open %s failed!", file);
        return false;
    }

    /* 设置语言编码 */
    sty_set_text_encoding(GUI_UTF8_ENC);
    gui_set_language_mode(sys_get_cur_language_id());
    
    return true;
}

static bool _scene_emulator_close_str_res(void)
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
static bool _emulator_scene_deinit(void)
{
    bool ret = true;
    
    if (g_scene_emulator->thread_exist == 1)
    {
        emu_exit();	
    }

    if (NULL != g_scene_emulator->bg_bitmap_info)
    {
        sty_delete_bitmap(g_scene_emulator->bg_bitmap_info);
        g_scene_emulator->bg_bitmap_info = NULL;
    }

    if (em_delete_headbar() == false)
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

    if (_scene_emulator_close_theme() == false)
    {
        ret = false;
    }

    if (_scene_emulator_close_str_res() == false)
    {
        ret = false;
    }

    if (false == em_delete_menuview())
    {
        //print_warning("delete menuview failed!");
        ret = false;
    }
    
    em_dirlist_exit(g_scene_emulator->dirlist);

    /* 删除窗体 */
    if (gui_wm_delete_window(g_scene_emulator->scene_common.win_id) < 0)
    {
        //print_warning("delete window failed!");
        ret = false;
    }
    g_scene_emulator->scene_common.win_id = 0;

    /* destroy font */
    if (false == _scene_unset_default_font())
    {
        //print_warning("unset font failed!");
        ret = false;
    }
    
    free(g_scene_emulator);
    sys_set_key_tone_enable(1); //打开按键音
    return ret;
}

void emulator_scene_quit(void)
{
    msg_key_server_enable(1);
    exit_msg_loop();
    g_scene_emulator->scene_common.exit_type = EMULATOR_SCENE_RETURN;
}

static bool _scene_emulator_rotate(void)
{
    fusion_effect_t fusion_effect =
    { 0 };

    /*正在游戏时，不进行横竖屏切换*/
    if (emu_state == EMU_STAT_RUNNING)
    {
        return true;
    }

    fusion_get_config(FUSION_TYPE_ROTATE, FUSION_FORWARD, &fusion_effect);
    fusion_set_effect(&fusion_effect);
    fusion_set_first_frame(NULL);

    _scene_emulator_close_theme();
    _scene_emulator_open_theme();
    _scene_emulator_load_resources();

    gui_wm_set_size(g_scene_emulator->scene_common.win_id, g_scene_emulator->scene_common.scene->width,
            g_scene_emulator->scene_common.scene->height);

    if (NULL != g_scene_emulator->bg_bitmap_info)
    {
        sty_delete_bitmap(g_scene_emulator->bg_bitmap_info);
        g_scene_emulator->bg_bitmap_info = NULL;
    }
    g_scene_emulator->bg_bitmap_info = get_user_bg_pic(BG_EMULATOR);
    if (NULL == g_scene_emulator->bg_bitmap_info)
    {
        g_scene_emulator->bg_bitmap_info = sty_create_bitmap((int) g_scene_emulator->bg_pic->pic_id);
    }
    if (NULL == g_scene_emulator->bg_bitmap_info)
    {
        //print_err("bg_bitmap_info is NULL. \n");
        return false;
    }

    _emulator_scene_init_paint(g_scene_emulator);

    if (g_scene_emulator->headbar != NULL)
    {
        headbar_load_style(g_scene_emulator->headbar);
        headbar_paint(g_scene_emulator->headbar);
    }

    if (g_scene_emulator->volumebar != NULL)
    {
        volumebar_load_style(g_scene_emulator->volumebar);
        volumebar_paint(g_scene_emulator->volumebar);
    }

    if (g_scene_emulator->dirlist != NULL)
    {
        dirlist_load_style(g_scene_emulator->dirlist);
        dirlist_paint(g_scene_emulator->dirlist);
    }
    
    if (g_scene_emulator->adjust != NULL)
    {
        adjust_load_style(g_scene_emulator->adjust);
        adjust_paint(g_scene_emulator->adjust);
    }
    
    if (g_scene_emulator->dlg != NULL)
    {
        dialog_load_style(g_scene_emulator->dlg);
        dialog_paint(g_scene_emulator->dlg);
    }

    if (g_scene_emulator->menuview != NULL)
    {
        menuview_load_style(g_scene_emulator->menuview, MENUVIEW_MENULIST_TEXT);
        menuview_paint(g_scene_emulator->menuview);
    }

    if (g_scene_emulator->msgbox != NULL)
    {
        msgbox_load_style(g_scene_emulator->msgbox);
        msgbox_paint(g_scene_emulator->msgbox);
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
        if (g_scene_emulator->thread_exist == 1)
        {
            emu_exit();
            usleep(200000);
        }
        
        msg_key_server_enable(1);
        exit_msg_loop();
        g_scene_emulator->scene_common.exit_type = EMULATOR_SCENE_RETURN;
        break;

        case MSG_GSENSOR_ROTATION_CHANGED:
        //print_info("received msg: MSG_GSENSOR_ROTATION_CHANGED");
        _scene_emulator_rotate();
        break;

        default:
        //更新目录列表
        if(g_scene_emulator->dirlist != NULL)
        {
            dirlist_proc_sys(g_scene_emulator->dirlist, msg);
        }

        if (g_scene_emulator->msgbox != NULL)
        {
            sys_msg_type = msg->type;
            msg_key_server_enable(1);

            //如果正在游戏时收到 msg_box 的要求，就暂停游戏，等msg_box退出后再继续游戏
            if ((g_scene_emulator->thread_exist == 1) && (emu_state == EMU_STAT_RUNNING))
            {
                emu_pause();
                msgbox_exist = true;
                //卡拔出
                if(sys_msg_type == MSG_CARD_OUT)
                {
                    //如果正在播放的游戏文件在卡上，那么就退出游戏
                    if(deviceSel == 1 )
                    {
                        emu_exit();
                        return_to_dirlist();
                    }
                    else
                    {
                        msgbox_exist = false;
                        emu_continue();
                        msg_key_server_enable(0);    
                    }
                }
                
                //host拔出，
                if(sys_msg_type == MSG_USB_HOST_DISK_OUT)
                {
                    //如果正在播放的游戏文件在host上，那么就退出游戏
                    if(deviceSel == 2 )
                    {
                        emu_exit();
                        return_to_dirlist();
                    }
                    else
                    {
                        msgbox_exist = false;
                        emu_continue();
                        msg_key_server_enable(0);    
                    }
                }
            }
            
            if((g_scene_emulator->thread_exist==1)&&(emu_state==EMU_STAT_PAUSE)&&(msg->type==MSG_USB_B_OUT))
            {
                msgbox_exist = false;
                emu_continue();
                msg_key_server_enable(0);
            }
            
            /* 控件msgbox用来处理默认系统消息(低电提示、卡插入和拔出提示、USB插入功能选择提示等) */
            msgbox_proc_sys(g_scene_emulator->msgbox, msg);
        }
        break;
    }
    return 0;
}

/*
 * 设置当前场景全部区域作为特效的第一帧
 */
void scene_fusion_set_first_frame(scene_t *scene)
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

/*
 * 设置当前场景全部区域作为特效的最后一帧
 */
void scene_fusion_set_last_frame(scene_t *scene)
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

int emulator_scene(void)
{
    bool ret;
    msg_apps_t msg;

    /* 场景初始化 */
    ret = _emulator_scene_init();
    if (ret == false)
    {
        goto ERROR_EXIT;	
    }
    
//    if (ret < 0)
//    {
//        //print_err("scene_emulator  init error! \n");
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
    
    ret = _emulator_scene_deinit();
    if (ret == false)
    {
        //print_err("scene exit error! \n");
        return -1;
    }
    return 0;
}

