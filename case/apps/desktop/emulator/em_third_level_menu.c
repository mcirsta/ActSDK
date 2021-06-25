#include "emulator_scene.h"
#include "emulator_play.h"
#include "em_second_level_menu.h"
#include "em_third_level_menu.h"
#include "em_fourth_level_menu.h"


extern scene_emulator_t* g_scene_emulator;
extern emu_config_info_t emu_cfg_info;
extern fusion_enter_e enter_type;


static int _item_sound_proc(menuview_t *list, menuview_sig_e signal);
static int _item_keymap_proc(menuview_t *list, menuview_sig_e signal);
static int _item_disp_mode_proc(menuview_t *list, menuview_sig_e signal);
static bool _em_third_level_menu_exit(void);

static menu_t* _em_third_level_menu_init(void);
static bool _em_third_level_menu_deinit(void);

static bool _create_sound_output_adjust_widget(void);
static void _sound_output_adjust_widget_cb(adjust_t *adjust, adjust_signal_e signal);
static bool _delete_adjust_widget(void);

static bool _create_disp_mode_adjust_widget(void);
static void _disp_mode_adjust_widget_cb(adjust_t *adjust, adjust_signal_e signal);

menu_t *thirdLevelMenu = NULL;

/* third level menu item data define */
menuitem_t third_level_items[3];

/*
 ** 第三级菜单入口
 */
bool em_third_level_menu_enter(scene_emulator_t* my_scene)
{
    menu_t *cur_menu;
    fusion_effect_t effect;

    fusion_get_config(FUSION_TYPE_APP_CHANGE, enter_type, &effect); // FUSION_TYPE_COMMON
    fusion_set_effect(&effect);

    scene_fusion_set_first_frame(g_scene_emulator->scene_common.scene);

    my_scene->cur_menu = 3;
    cur_menu = _em_third_level_menu_init();
    menuview_set_menu(my_scene->menuview, cur_menu);
    //menuview_load_style(my_scene->menuview, MENUVIEW_MENULIST_TEXT);
    menuview_load_style(my_scene->menuview, MENUVIEW_FILELIST_TEXT );
    menuview_show(my_scene->menuview);
    menuview_paint(my_scene->menuview);

    scene_fusion_set_last_frame(g_scene_emulator->scene_common.scene);
    fusion_display( NULL);

    return true;
}

/*
 ** 退出第三级菜单
 */
static bool _em_third_level_menu_exit(void)
{
    g_scene_emulator->cur_item03 = thirdLevelMenu->current;

    /* delete seleq menu */
    menuview_hide(g_scene_emulator->menuview);
    _em_third_level_menu_deinit();

    menuview_set_menu(g_scene_emulator->menuview, NULL); //将menuview的menu指针置空，避免野指针
    
    return true;
}

/*
 ** 初始化第三级菜单
 */
static menu_t* _em_third_level_menu_init(void)
{
    scene_t* scene;
    int num;

    /* initial menu	*/
    if (thirdLevelMenu == NULL)
    {
        /* get scene */
        scene = sty_get_app_scene();
        if (scene == NULL)
        {
            return false;
        }
        /* init items */
        memset(&third_level_items, 0, sizeof(third_level_items));

        third_level_items[0].text_id = (int)g_scene_emulator->sound_str->str_id;
        third_level_items[0].has_child = false;
        third_level_items[0].callback = (item_cb_t) _item_sound_proc;
        if (emu_cfg_info.audio_enable == 0)
        {
            third_level_items[0].info1 = "off";
        }
        else
        {
            third_level_items[0].info1 = "on";
        }        

        third_level_items[1].text_id = (int)g_scene_emulator->key_str->str_id;
        third_level_items[1].has_child = false;
        third_level_items[1].callback = (item_cb_t) _item_keymap_proc;

        third_level_items[2].text_id = (int)g_scene_emulator->disp_str->str_id;
        third_level_items[2].has_child = false;
        third_level_items[2].callback = (item_cb_t) _item_disp_mode_proc;
        third_level_items[2].info1 = "info2";
        if (emu_cfg_info.video_mode == EMU_MODE_FULLSCR)
        {
            third_level_items[2].info1 = "full";
        }
        else
        {
            third_level_items[2].info1 = "normal";
        } 
        
        /* create menu */
        thirdLevelMenu = menu_create();
        if (thirdLevelMenu == NULL)
        {
            return false;
        }
        else
        {
            /* append items */
            for (num = 0; num < 3; num++)
            {
                menu_append_item(thirdLevelMenu, &third_level_items[num]);
            }
        }

        /* init current index */
        thirdLevelMenu->total = 3;
        thirdLevelMenu->offset = 0;
        menu_set_cur_index(thirdLevelMenu, g_scene_emulator->cur_item03);
    }
    return thirdLevelMenu;
}

/*
 ** 删除第三级菜单
 */
static bool _em_third_level_menu_deinit(void)
{
    /* delete menu */
    if (thirdLevelMenu != NULL)
    {
        /* create menu */
        menu_delete(thirdLevelMenu);
        thirdLevelMenu = NULL;
    }

    return true;
}

/*
 ** 以下是第三级菜单的各个item 的回调函数
 */
static int _item_sound_proc(menuview_t *list, menuview_sig_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
        /* 显示 sound_output adjust控件  */
        _create_sound_output_adjust_widget();
        break;

        /* 退回到第二级菜单 */
        case WIDGET_SIG_MENUVIEW_QUIT:
        _em_third_level_menu_exit();
        em_second_level_menu_enter(g_scene_emulator);
        break;

        default:
        break;
    }
    return 0;
}


static int _item_keymap_proc(menuview_t *list, menuview_sig_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
        /* 进入第四级菜单（option,更改按键设置）*/
        _em_third_level_menu_exit();
        em_fourth_level_menu_enter(g_scene_emulator);
        break;

        /* 退回到第二级菜单 */
        case WIDGET_SIG_MENUVIEW_QUIT:
        _em_third_level_menu_exit();
        em_second_level_menu_enter(g_scene_emulator);
        break;

        default:
        break;
    }
    return 0;
}

static int _item_disp_mode_proc(menuview_t *list, menuview_sig_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
        /* 显示 disp mode 对话框  */
        _create_disp_mode_adjust_widget();
        break;

        /* 退回到第二级菜单 */
        case WIDGET_SIG_MENUVIEW_QUIT:
        _em_third_level_menu_exit();
        em_second_level_menu_enter(g_scene_emulator);
        break;

        default:
        break;
    }
    return 0;
}



/*
 ** 声音输出模式adjust控件(无声|有声)
 */
static bool _create_sound_output_adjust_widget(void)
{
    adjust_switch_init_t adjust_data = {0};

    if (NULL == g_scene_emulator->adjust)
    {
        adjust_data.str_title_id = g_scene_emulator->sound_str->str_id;

        if(emu_cfg_info.audio_enable == 1)
        {
            adjust_data.on_off_status = ADJUST_ON;
        }
        else
        {
            adjust_data.on_off_status = ADJUST_OFF;
        }

        g_scene_emulator->adjust = adjust_create( &adjust_data, ADJ_STYLE_SWITCH, _sound_output_adjust_widget_cb);
        if(NULL == g_scene_emulator->adjust)
        {
            print_err("create adjust failed!");
            return false;
        }

        adjust_paint(g_scene_emulator->adjust);
    }
    return true;
}

static void _sound_output_adjust_widget_cb(adjust_t *adjust, adjust_signal_e signal)
{
    adj_switch_mode_e switch_val;

    switch(signal)
    {
        case WIDGET_SIG_ADJUST_OK:
        adjust_get_switch_status(&switch_val);
        if(ADJUST_ON == switch_val)
        {
            if (emu_cfg_info.audio_enable == 0)  //0无声； 1有声
            {
                emu_set_sound(1);
                emu_cfg_info.audio_enable = 1;
                third_level_items[0].info1 = "on";
            } 
        }
        else
        {
            if (emu_cfg_info.audio_enable == 1)  //0无声； 1有声
            {
                emu_set_sound(0);
                emu_cfg_info.audio_enable = 0;
                third_level_items[0].info1 = "off";
            }
        }
        menu_set_item(thirdLevelMenu, 0, &third_level_items[0] );
        menuview_paint(g_scene_emulator->menuview);

        _delete_adjust_widget();
        break;

        case WIDGET_SIG_ADJUST_CANCEL:
        _delete_adjust_widget();
        break;
                
        case WIDGET_SIG_ADJUST_UP:
        case WIDGET_SIG_ADJUST_DOWN:
        default:
        break;
    }
}

static bool _delete_adjust_widget(void)
{
    if(g_scene_emulator->adjust != NULL)
    {
        adjust_delete(g_scene_emulator->adjust);
        g_scene_emulator->adjust = NULL;
    }

    return true;
}


/*
 ** 显示模式(全屏|非全屏) adjust控件
 */
static bool _create_disp_mode_adjust_widget(void)
{
    adjust_switch_init_t adjust_data = {0};

    if (NULL == g_scene_emulator->adjust)
    {
        adjust_data.str_title_id = g_scene_emulator->full_screen_str->str_id;

        if(emu_cfg_info.video_mode == EMU_MODE_FULLSCR)
        {
            adjust_data.on_off_status = ADJUST_ON;
        }
        else
        {
            adjust_data.on_off_status = ADJUST_OFF;
        }

        g_scene_emulator->adjust = adjust_create( &adjust_data, ADJ_STYLE_SWITCH, _disp_mode_adjust_widget_cb);
        if(NULL == g_scene_emulator->adjust)
        {
            print_err("create adjust failed!");
            return false;
        }

        adjust_paint(g_scene_emulator->adjust);
    }
    return true;
}

static void _disp_mode_adjust_widget_cb(adjust_t *adjust, adjust_signal_e signal)
{
    adj_switch_mode_e switch_val;

    switch(signal)
    {
        case WIDGET_SIG_ADJUST_OK:
        adjust_get_switch_status(&switch_val);
        if(ADJUST_ON == switch_val)
        { 
            if (emu_cfg_info.video_mode == EMU_MODE_NORMAL)
            {
                emu_set_disp_mode(EMU_MODE_FULLSCR);
                emu_cfg_info.video_mode = EMU_MODE_FULLSCR;
                third_level_items[2].info1 = "full";
            }             
        }
        else  //off
        {
            if (emu_cfg_info.video_mode == EMU_MODE_FULLSCR)
            {
                emu_set_disp_mode(EMU_MODE_NORMAL);
                emu_cfg_info.video_mode = EMU_MODE_NORMAL;
                third_level_items[2].info1 = "normal";
            } 
        }
        menu_set_item(thirdLevelMenu, 2, &third_level_items[2] );
        menuview_paint(g_scene_emulator->menuview);
        
        _delete_adjust_widget();
        break;

        case WIDGET_SIG_ADJUST_CANCEL:
        _delete_adjust_widget();
        break;
                
        case WIDGET_SIG_ADJUST_UP:
        case WIDGET_SIG_ADJUST_DOWN:
        default:
        break;
    }
}


