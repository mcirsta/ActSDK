/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 * file : setting_list_menu_advanced.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-5-6             v1.0             build this file
 ********************************************************************************/

#include "setting_list_widget_menu.h"

/*********************************************************************************
 * internal static functions declaration
 *********************************************************************************/
static bool _menu_init(void);
static bool _menu_deinit(void);
static void _menu_return( menuview_t *menuview );
static void _menu_update_minor_info(void);
static void _check_menu_visible( void );

/* menu  item callbacks */
static void _player_info_cb( menuview_t *menuview, menuview_sig_e signal );
static void _connect_type_cb( menuview_t *menuview, menuview_sig_e signal );
static void _select_font_cb( menuview_t *menuview, menuview_sig_e signal );
static void _set_key_tone_cb( menuview_t *menuview, menuview_sig_e signal );
static void _show_effect_cb( menuview_t *menuview, menuview_sig_e signal );
static void _search_forlder_cb( menuview_t *menuview, menuview_sig_e signal );
static void _gsensor_support_cb( menuview_t *menuview, menuview_sig_e signal );
static void _format_internal_cb( menuview_t *menuview, menuview_sig_e signal );
static void _format_card_cb( menuview_t *menuview, menuview_sig_e signal );
static void _format_external_cb( menuview_t *menuview, menuview_sig_e signal );
static void _set_username_cb( menuview_t *menuview, menuview_sig_e signal );
static void _clear_drm_cb( menuview_t *menuview, menuview_sig_e signal );
static void _check_system_cb( menuview_t *menuview, menuview_sig_e signal );
static void _reset_cb( menuview_t *menuview, menuview_sig_e signal );

static bool _create_softkeyboard_widget(void);
static void _softkeyboard_widget_cb(softkeyboard_t *keyboard,  softkeyboard_sig_e signal);
static bool _delete_sofkeyboard_widget(void);

static bool _create_info_dialog_widget(unsigned short str_id);
static void _info_dialog_widget_cb(dialog_t *dialog,  widget_sig_dialog_e signal);
static bool _delete_info_dialog_widget(void);

static bool _create_loadingbox_widget( void );
static void _loadingbox_widget_cb(loadingbox_t *loadingbox, loadingbox_sig_e signal);
static bool _delete_loadingbox_widget( void );

static bool _create_search_folder_adjust_widget(void);
static void _search_forlder_widget_cb(adjust_t *adjust, adjust_signal_e signal);
static bool _delete_search_forlder_adjust_widget(void);

static bool _create_gsensor_support_adjust_widget(void);
static void _gsensor_support_widget_cb(adjust_t *adjust, adjust_signal_e signal);
static bool _delete_gsensor_support_adjust_widget(void);

static bool _create_progress_box_widget(void);
static void _progress_box_widget_cb( progress_box_t * progress_box, progress_box_sig_e signal);
static bool _delete_progress_box_widget(void);

static void * _thread_sys_check(void * data);
static int _monitor_sys_check_status_timer(void *param);
static int s_monitor_sys_check_timer_id = -1;
static int _reboot_system(void *param);

static void * _thread_format_internal(void * data);
static void * _thread_format_card(void * data);
static void * _thread_format_external(void * data);
static int _format_done(void * param);

static bool _create_thread( void *proc, void *arg );

static bool _popup_info_dialog(unsigned short str_id, unsigned int stay_time);
static int _info_dialog_timeout(void * param);
static int s_info_dialog_timer_id = -1;

/*********************************************************************************
 * internal static struct and data definition
 *********************************************************************************/

enum menuitem_name
{
    MENU_INFO = 0,
    MENU_CONNECT_TYPE,
    MENU_FONT,
    MENU_KEY_TONE,
    MENU_SHOW_EFFECT,
    MENU_SEARCH_FOLDER,
    MENU_GSENSOR_SHAKE_SUPPORT,
    MENU_FORMT_INTERNAL,
    MENU_FORMT_CARD,
    MENU_FORMT_EXTERNAL,
    MENU_SET_USERNAME,
    MENU_CLEAR_DRM,
    MENU_CHECK_SYS,
    MENU_RESET,
};

/* menu items style data define */
static menuitem_style_t s_style_items[] =
{
    { 0, 0, STR_INFO,           true, (item_cb_t)_player_info_cb,       NULL, true, 0 },
    { 0, 0, STR_CONNECT_TYPE,   true, (item_cb_t)_connect_type_cb,      NULL, true, 0 },
    { 0, 0, STR_FONT,           true, (item_cb_t)_select_font_cb,       NULL, true, 0 },
    { 0, 0, STR_KEY_TONE,       true, (item_cb_t)_set_key_tone_cb,      NULL, true, 0 },
    { 0, 0, STR_SHOW_EFFECT,    true, (item_cb_t)_show_effect_cb,       NULL, true, 0 },
    { 0, 0, STR_SEARCH_FOLDER,  false, (item_cb_t)_search_forlder_cb,   NULL, true, 0 },
    { 0, 0, STR_GSENSOR_SHAKE_SUPPORT, false, (item_cb_t)_gsensor_support_cb, NULL, true, 0 },
    { 0, 0, STR_FORMAT_INTERNAL, false, (item_cb_t)_format_internal_cb,  NULL, true, 0 },
    { 0, 0, STR_FORMAT_CARD,     false, (item_cb_t)_format_card_cb,      NULL, true, 0 },
    { 0, 0, STR_FORMAT_EXTERNAL, false, (item_cb_t)_format_external_cb,  NULL, true, 0 },
    { 0, 0, STR_SET_USERNAME,   false, (item_cb_t)_set_username_cb,     NULL, true, 0 },
    { 0, 0, STR_CLEAR_DRM,      false, (item_cb_t)_clear_drm_cb,        NULL, true, 0 },
    { 0, 0, STR_CHECK_SYS,      false, (item_cb_t)_check_system_cb,     NULL, true, 0 },
    { 0, 0, STR_RESET,          false, (item_cb_t)_reset_cb,            NULL, true, 0 },
};

static menu_t * s_menu = NULL;
static menuview_t *s_menuview = NULL;
static int s_menu_id = -1;
bool need_reset_font = false;

typedef struct
{
    unsigned int style_id;
    unsigned short str_id;
}str_res_t;

typedef enum
{
    STR_NAME_INFO_FMT_INTERNAL = 0,
    STR_NAME_INFO_FMT_COMPLETE,
    STR_NAME_INFO_DEV_NOT_EXIST,
    STR_NAME_INFO_FMT_CARD,
    STR_NAME_INFO_FMT_EXTERNAL,
    STR_NAME_INFO_CLR_DRM_HDS,
    STR_NAME_INFO_CHECK_SYS,
    STR_NAME_INFO_IN_SYS_CHECKING,
    STR_NAME_INFO_SYS_CHECK_COMPLETE,
    STR_NAME_INFO_RESET,
}str_name_e;

/* 鎻愮ず淇℃伅璧勬簮 */
static str_res_t s_str_res_table[] =
{
    {STR_INFO_FORMAT_INTERNAL, 0},
    {STR_INFO_FORMAT_COMPLETE, 0},
    {STR_INFO_DEV_NOT_EXIST, 0},
    {STR_INFO_FORMAT_CARD, 0},
    {STR_INFO_FORMAT_EXTERNAL, 0},
    {STR_INFO_CLEAR_DRM_HDS, 0},
    {STR_INFO_CHECK_SYS, 0},
    {STR_INFO_IN_SYS_CHECKING, 0},
    {STR_INFO_SYS_CHECK_COMPLETE, 0},
    {STR_INFO_RESET, 0},
};

static void _load_str_res(resgroup_resource_t * res_group, str_res_t str_res_table[], int count);

/*********************************************************************************
 * interface implementaiton
 *********************************************************************************/

menu_t* setting_list_menu_advanced_enter( menuview_t *menuview, fusion_enter_e fusion_enter_type)
{
    sub_menu_t sub_menu = {0};

    if(_menu_init() == false)
    {
        print_err("init menu data failed!");
        return NULL;
    }
    menuview_set_menu( menuview, s_menu );
    menuview_load_style( menuview, MENUVIEW_FILELIST_ICON );

    if(s_menu_id == -1)
    {
        sub_menu.deinit_cb = _menu_deinit;
        sub_menu.update_minor_info_cb = (menu_update_minor_info_cb)_menu_update_minor_info;
        s_menu_id = setting_list_widget_menu_register_sub_menu(menuview, &sub_menu);
    }

    _menu_update_minor_info();

    setting_list_widget_menu_show_fusion(menuview, fusion_enter_type);

    s_menuview = menuview;

    return s_menu;
}


/*********************************************************************************
 * internal static  implementaiton
 *********************************************************************************/

static bool _menu_init(void)
{
    scene_t * scene;
    resgroup_resource_t * res_group;

#pragma __PRQA_IGNORE_START__
    /* 浣跨敤 0 鍒濆鍖栨暟缁?*/
    menuitem_t menu_items[ARRAY_SIZE(s_style_items)] = {{0}};
#pragma __PRQA_IGNORE_END__

    int real_items = 0;

    //initial menu
    if( s_menu == NULL )
    {
        scene = sty_get_app_scene();
        if ( NULL == scene )
        {
            print_err("get app scene failed!");
            return false;
        }

        //load advanced menu resource group
        res_group = (resgroup_resource_t *)get_scene_child(scene, REG_ADVANCED);
        if(NULL == res_group)
        {
            print_err("get resource group REG_ADVANCED");
            return false;
        }

        _check_menu_visible();

        /* load menu item string and pictrure resource */
        real_items = menu_load_item_resource(res_group, s_style_items, menu_items, ARRAY_SIZE(s_style_items));

        /* load other string resourc */
        _load_str_res(res_group, s_str_res_table, ARRAY_SIZE(s_str_res_table));

        //create menu
        s_menu = menu_create();
        if(NULL == s_menu)
        {
            print_err("create menu failed!");
            return false;
        }

        //init menu items
        menu_append_items( s_menu, menu_items, real_items);

        menu_set_cur_index( s_menu, 0 );
    }

    return true;
}

static void _check_menu_visible( void )
{
    setting_cfg_init(g_menu_advanced_cfgs, CFG_MENU_ADVANCED_MAX); 
    setting_cfg_read_all(g_menu_advanced_cfgs, CFG_MENU_ADVANCED_MAX); 

    if(g_menu_advanced_cfgs[CFG_MENU_ADVANCED_INFO].value == 0)
    {
        s_style_items[MENU_INFO].visible = false;
    }

    if(g_menu_advanced_cfgs[CFG_MENU_ADVANCED_CONNECT_TYPE].value == 0)
    {
        s_style_items[MENU_CONNECT_TYPE].visible = false;
    }

    if(g_menu_advanced_cfgs[CFG_MENU_ADVANCED_FONT].value == 0)
    {
        s_style_items[MENU_FONT].visible = false;
    }

    if(g_menu_advanced_cfgs[CFG_MENU_ADVANCED_KEY_TONE].value == 0)
    {
        s_style_items[MENU_KEY_TONE].visible = false;
    }

    if(g_menu_advanced_cfgs[CFG_MENU_ADVANCED_SHOW_EFFECT].value == 0)
    {
        s_style_items[MENU_SHOW_EFFECT].visible = false;
    }

    if(g_menu_advanced_cfgs[CFG_MENU_ADVANCED_SEARCH_FOLDER].value == 0)
    {
        s_style_items[MENU_SEARCH_FOLDER].visible = false;
    }

    if(g_menu_advanced_cfgs[CFG_MENU_ADVANCED_GSENSOR_SHAKE_SUPPORT].value == 0)
    {
        s_style_items[MENU_GSENSOR_SHAKE_SUPPORT].visible = false;
    }

    if(g_menu_advanced_cfgs[CFG_MENU_ADVANCED_FORMAT_INTERNAL].value == 0)
    {
        s_style_items[MENU_FORMT_INTERNAL].visible = false;
    }

    if(g_menu_advanced_cfgs[CFG_MENU_ADVANCED_FORMAT_CARD].value == 0)
    {
        s_style_items[MENU_FORMT_CARD].visible = false;
    }

    if(g_menu_advanced_cfgs[CFG_MENU_ADVANCED_FORMAT_EXTERNAL].value == 0)
    {
        s_style_items[MENU_FORMT_EXTERNAL].visible = false;
    }

    if(g_menu_advanced_cfgs[CFG_MENU_ADVANCED_SET_USERNAME].value == 0)
    {
        s_style_items[MENU_SET_USERNAME].visible = false;
    }

    if(g_menu_advanced_cfgs[CFG_MENU_ADVANCED_CLEAR_DRM].value == 0)
    {
        s_style_items[MENU_CLEAR_DRM].visible = false;
    }

    if(g_menu_advanced_cfgs[CFG_MENU_ADVANCED_CHECK_SYS].value == 0)
    {
        s_style_items[MENU_CHECK_SYS].visible = false;
    }

    if(g_menu_advanced_cfgs[CFG_MENU_ADVANCED_RESET].value == 0)
    {
        s_style_items[MENU_RESET].visible = false;
    }

    setting_cfg_deinit(g_menu_advanced_cfgs, CFG_MENU_ADVANCED_MAX); 
}

static bool _menu_deinit(void)
{
    if( NULL == s_menu )
    {
        print_warning("no menu data to delete!");
        return false;
    }

    menu_delete( s_menu );
    s_menu = NULL;

    if(s_menu_id != -1)
    {
        setting_list_widget_menu_unregister_sub_menu(s_menu_id);
        s_menu_id = -1;
    }

    s_menuview = NULL;

    _delete_sofkeyboard_widget();
    _delete_info_dialog_widget();
    _delete_search_forlder_adjust_widget();
    _delete_progress_box_widget();

    if(s_info_dialog_timer_id > 0)
    {
        kill_timer(s_info_dialog_timer_id);
    }

    return true;
}

static void _menu_return( menuview_t *menuview )
{
    _menu_deinit();

    //return to parent menu
    setting_list_menu_main_enter(menuview, FUSION_BACKWARD);
}

static void _menu_update_minor_info(void)
{
    menuitem_t* menu_item;

    if(s_style_items[MENU_CONNECT_TYPE].visible == true)
    {
        menu_item = menu_get_item( s_menu, s_style_items[MENU_CONNECT_TYPE].index);
        if(menu_item != NULL)
        {
            menu_item->info1 = (char *)setting_list_menu_connect_type_get_cur_selected_text();
        }
    }

    if(s_style_items[MENU_FONT].visible == true)
    {
        menu_item = menu_get_item( s_menu, s_style_items[MENU_FONT].index);
        if(menu_item != NULL)
        {
            menu_item->info1 = (char *)setting_list_menu_font_select_get_cur_selected_text();
        }
    }

    if(s_style_items[MENU_KEY_TONE].visible == true)
    {
        menu_item = menu_get_item( s_menu, s_style_items[MENU_KEY_TONE].index);
        if(menu_item != NULL)
        {
            menu_item->info1 = (char *)setting_list_menu_key_tone_get_cur_selected_text();
        }
    }

    if(s_style_items[MENU_SHOW_EFFECT].visible == true)
    {
        menu_item = menu_get_item( s_menu, s_style_items[MENU_SHOW_EFFECT].index);
        if(menu_item != NULL)
        {
            menu_item->info1 = (char *)setting_list_menu_show_effect_get_cur_selected_text();
        }
    }

    if(s_style_items[MENU_SET_USERNAME].visible == true)
    {
        menu_item = menu_get_item( s_menu, s_style_items[MENU_SET_USERNAME].index);
        if(menu_item != NULL)
        {
            menu_item->info1 = g_setting_cfgs[SET_CFG_TYPE_USERNAME].text;
        }
    }

    if(s_style_items[MENU_SEARCH_FOLDER].visible == true)
    {
        menu_item = menu_get_item( s_menu, s_style_items[MENU_SEARCH_FOLDER].index);
        if(menu_item != NULL)
        {
            menu_item->info1 = g_setting_cfgs[SET_CFG_TYPE_SEARCH_FOLDER].text;
        }
    }

    if(s_style_items[MENU_GSENSOR_SHAKE_SUPPORT].visible == true)
    {
        menu_item = menu_get_item( s_menu, s_style_items[MENU_GSENSOR_SHAKE_SUPPORT].index);
        if(menu_item != NULL)
        {
            menu_item->info1 = g_setting_cfgs[SET_CFG_TYPE_GSENSOR_SHAKE_SUPPORT].text;
        }
    }

}

static void _player_info_cb( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal )
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            // enter player informaition menu
            setting_list_menu_info_enter(menuview, FUSION_FORWARD);
            break;
        }

        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            // return to parent menu
            _menu_return( menuview );
            break;
        }

        default:
        {
            break;
        }
    }
}

static void _connect_type_cb( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal )
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            // enter connection type select menu
            setting_list_menu_connect_type_enter(menuview, FUSION_FORWARD);
            break;
        }

        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            // return to parent menu
            _menu_return( menuview );
            break;
        }

        default:
        {
            break;
        }
    }
}

static void _select_font_cb( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal )
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            // enter font type select menu
            setting_list_menu_font_select_enter(menuview, FUSION_FORWARD);
            break;
        }

        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            // return to parent menu
            _menu_return( menuview );
            break;
        }

        default:
        {
            break;
        }
    }
}

static void _set_key_tone_cb( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal )
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            // enter key tone select menu
            setting_list_menu_key_tone_enter(menuview, FUSION_FORWARD);
            break;
        }

        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            // return to parent menu
            _menu_return( menuview );
            break;
        }

        default:
        {
            break;
        }
    }
}

static void _show_effect_cb( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal )
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            // enter show effect select menu
            setting_list_menu_show_effect_enter(menuview, FUSION_FORWARD);
            break;
        }

        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            // return to parent menu
            _menu_return( menuview );
            break;
        }

        default:
        {
            break;
        }
    }
}

static void _search_forlder_cb( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal )
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            _create_search_folder_adjust_widget();
            break;
        }

        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            // return to parent menu
            _menu_return( menuview );
            break;
        }

        default:
        {
            break;
        }
    }

}

static void _gsensor_support_cb( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal )
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            _create_gsensor_support_adjust_widget();
            break;
        }

        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            // return to parent menu
            _menu_return( menuview );
            break;
        }

        default:
        {
            break;
        }
    }

}

static void _format_internal_cb( menuview_t *menuview, menuview_sig_e signal )
{
    dialog_init_t init_data = {0};
    msg_apps_t msg = {0};
    msg_reply_t reply = {0};

    init_data.head = DIALOG_HEAD_QUERY;
    init_data.str_id = s_str_res_table[STR_NAME_INFO_FMT_INTERNAL].str_id;
    init_data.def_sel =WIDGET_SIG_DIALOG_CANCAL;

    switch ( signal )
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            if(dialog_modal(&init_data) == WIDGET_SIG_DIALOG_OK)
            {
                sys_enable_sys_evt_detect(false);
                sys_forbid_timed_shutdown(false);


                _create_loadingbox_widget();

                msg.type = MSG_KILL_ALL_USER_APPS_SYNC;
                send_sync_msg(APP_NAME_MANAGER, &msg, &reply, NULL);

                _create_thread(_thread_format_internal, NULL);
            }
            break;
        }

        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            // return to parent menu
            _menu_return( menuview );
            break;
        }

        default:
        {
            break;
        }
    }
}

static void _format_card_cb( menuview_t *menuview, menuview_sig_e signal )
{
    dialog_init_t init_data = {0};
    hotplugin_dev_info_t card_info;
    msg_apps_t msg = {0};
    msg_reply_t reply = {0};

    init_data.head = DIALOG_HEAD_QUERY;
    init_data.str_id = s_str_res_table[STR_NAME_INFO_FMT_CARD].str_id;
    init_data.def_sel =WIDGET_SIG_DIALOG_CANCAL;

    switch ( signal )
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_CARD, &card_info);
            if(card_info.insert != HOTPLUGIN_DEV_READY)
            {
                _popup_info_dialog(s_str_res_table[STR_NAME_INFO_DEV_NOT_EXIST].str_id, 3000);
                return;
            }

            if(dialog_modal(&init_data) == WIDGET_SIG_DIALOG_OK)
            {
                sys_enable_sys_evt_detect(false);
                sys_forbid_timed_shutdown(false);

                _create_loadingbox_widget();

                msg.type = MSG_KILL_ALL_USER_APPS_SYNC;
                send_sync_msg(APP_NAME_MANAGER, &msg, &reply, NULL);

                _create_thread(_thread_format_card, NULL); 
            }

            break;
        }

        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            // return to parent menu
            _menu_return( menuview );
            break;
        }

        default:
        {
            break;
        }
    }
}

static void _format_external_cb( menuview_t *menuview, menuview_sig_e signal )
{
    dialog_init_t init_data = {0};
    hotplugin_dev_info_t uhost_info;
    msg_apps_t msg = {0};
    msg_reply_t reply = {0};

    init_data.head = DIALOG_HEAD_QUERY;
    init_data.str_id = s_str_res_table[STR_NAME_INFO_FMT_EXTERNAL].str_id;
    init_data.def_sel =WIDGET_SIG_DIALOG_CANCAL;

    switch ( signal )
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_UHOST, &uhost_info);
            if(uhost_info.insert != HOTPLUGIN_DEV_READY)
            {
                _popup_info_dialog(s_str_res_table[STR_NAME_INFO_DEV_NOT_EXIST].str_id, 3000);
                return;
            }

            if(dialog_modal(&init_data) == WIDGET_SIG_DIALOG_OK)
            {
                sys_enable_sys_evt_detect(false);
                sys_forbid_timed_shutdown(false);

                _create_loadingbox_widget();

                msg.type = MSG_KILL_ALL_USER_APPS_SYNC;
                send_sync_msg(APP_NAME_MANAGER, &msg, &reply, NULL);

                _create_thread(_thread_format_external, NULL);
            }

            break;
        }

        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            // return to parent menu
            _menu_return( menuview );
            break;
        }

        default:
        {
            break;
        }
    }
}

static void _set_username_cb( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal )
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            _create_softkeyboard_widget();
            break;
        }

        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            // return to parent menu
            _menu_return( menuview );
            break;
        }

        default:
        {
            break;
        }
    }
}

static void _clear_drm_cb( menuview_t *menuview, menuview_sig_e signal )
{
    dialog_init_t init_data = {0};

    init_data.head = DIALOG_HEAD_QUERY;
    init_data.str_id = s_str_res_table[STR_NAME_INFO_CLR_DRM_HDS].str_id;
    init_data.def_sel =WIDGET_SIG_DIALOG_CANCAL;

    switch ( signal )
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            if(dialog_modal(&init_data) == WIDGET_SIG_DIALOG_OK)
            {
                sys_forbid_auto_standby(true);
                sys_forbid_timed_shutdown(true);
                gui_screen_update();

                if(clear_hds(DEVICE_VRAM, DRM_HDS_FILE_PATH) < 0)
                {
                    print_err("Clear HDS region failed!");
                }

                sys_forbid_auto_standby(false);
                sys_forbid_timed_shutdown(false);
            }
            break;
        }

        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            // return to parent menu
            _menu_return( menuview );
            break;
        }

        default:
        {
            break;
        }
    }
}

static void _check_system_cb( menuview_t *menuview, menuview_sig_e signal )
{
    dialog_init_t init_data = {0};

    init_data.head = DIALOG_HEAD_QUERY;
    init_data.str_id = s_str_res_table[STR_NAME_INFO_CHECK_SYS].str_id;
    init_data.def_sel =WIDGET_SIG_DIALOG_CANCAL;

    switch ( signal )
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            if(dialog_modal(&init_data) == WIDGET_SIG_DIALOG_OK)
            {
                gui_screen_update();
                _create_progress_box_widget();
                _create_thread(_thread_sys_check, NULL);
            }
            break;
        }

        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            // return to parent menu
            _menu_return( menuview );
            break;
        }

        default:
        {
            break;
        }
    }
}

static void _reset_cb( menuview_t *menuview, menuview_sig_e signal )
{
    msg_apps_t msg = {0};
    msg_reply_t reply = {0};
    hotplugin_dev_type_e main_disk = HOTPLUGIN_DEV_TYPE_NONE;

    dialog_init_t init_data = {0};

    init_data.head = DIALOG_HEAD_QUERY;
    init_data.str_id = s_str_res_table[STR_NAME_INFO_RESET].str_id;
    init_data.def_sel =WIDGET_SIG_DIALOG_CANCAL;

    switch ( signal )
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            if(dialog_modal(&init_data) == WIDGET_SIG_DIALOG_OK)
            {
                sys_forbid_auto_standby(true);
                sys_forbid_timed_shutdown(true);
                sys_forbid_screensaver(true);

                gui_screen_update();

                msg.type = MSG_KILL_ALL_USER_APPS_SYNC;
                send_sync_msg(APP_NAME_MANAGER, &msg, &reply, NULL);

                main_disk = sys_get_main_disk();

                if(reset_all_config() < 0)
                {
                    print_err("reset all config failed!");
                }

                sys_set_main_disk(main_disk);

                if(restore_default_bg_pic(BG_ALL) == false)
                {
                    print_err("reset all backgroud pic failed!");
                }

                if(restore_default_all_animation() == false)
                {
                    print_err("reset power on and power off animation failed!");
                }

                msg.type = MSG_RESET_DEFAULT_CFG;
                broadcast_msg(&msg);

                // brodcast language changed msg
                msg.type = MSG_LANGUAGE_CHANGED;
                broadcast_msg(&msg);

                sys_forbid_screensaver(false);
                sys_forbid_auto_standby(false);
                sys_forbid_timed_shutdown(false);
            }
            break;
        }

        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            // return to parent menu
            _menu_return( menuview );
            break;
        }

        default:
        {
            break;
        }
    }
}


static bool _create_softkeyboard_widget(void)
{
    g_scene_list->softkeyboard = softkeyboard_create(IME_TYPE_PINYIN, _softkeyboard_widget_cb);
    if( NULL == g_scene_list->softkeyboard )
    {
        print_err("create softkeyboard widget failed!");
        return false;
    }

    setting_list_widget_menu_hide(s_menuview);
    softkeyboard_set_text(g_scene_list->softkeyboard, g_setting_cfgs[SET_CFG_TYPE_USERNAME].text);
    softkeyboard_paint(g_scene_list->softkeyboard);

    return true;
}

static void _softkeyboard_widget_cb(softkeyboard_t *keyboard,  softkeyboard_sig_e signal)
{
    switch ( signal )
    {
        case WIDGET_SIG_SKB_OK:
        {
            print_dbg("WIDGET_SIG_SKB_OK");
            softkeyboard_get_text(g_scene_list->softkeyboard, g_setting_cfgs[SET_CFG_TYPE_USERNAME].text, CFG_TEXT_LEN);
            _delete_sofkeyboard_widget();
            g_setting_cfgs[SET_CFG_TYPE_USERNAME].changed = true;
            setting_cfg_write(&g_setting_cfgs[SET_CFG_TYPE_USERNAME]);
            break;
        }

        case WIDGET_SIG_SKB_CANCEL:
        {
            print_dbg("WIDGET_SIG_SKB_CANCEL");
            _delete_sofkeyboard_widget();
            break;
        }
        default:
        {
            break;
        }
    }
}

static bool _delete_sofkeyboard_widget(void)
{
    if(g_scene_list->softkeyboard != NULL)
    {
        softkeyboard_delete(g_scene_list->softkeyboard);
        g_scene_list->softkeyboard = NULL;
        setting_list_widget_menu_show(s_menuview);
    }

    return true;
}

static void _load_str_res(resgroup_resource_t * res_group, str_res_t str_res_table[], int count)
{
    int i ;
    string_resource_t  * str_res;

    for(i = 0; i < count; i++)
    {
        str_res = ( string_resource_t * )get_resgroup_child(res_group, str_res_table[i].style_id);
        if(str_res != NULL)
        {
            str_res_table[i].str_id = str_res->str_id;
        }
    }
}

static bool _create_info_dialog_widget(unsigned short str_id)
{
    dialog_init_t init_data = {0};

    init_data.head = DIALOG_HEAD_INFO;
    init_data.str_id = str_id;

    g_scene_list->dialog = dialog_create( &init_data,  (dialog_cb_t)_info_dialog_widget_cb);
    if( NULL == g_scene_list->dialog )
    {
        print_err("create info dialog widget failed!");
        return false;
    }

    dialog_paint(g_scene_list->dialog);

    return true;
}

static void _info_dialog_widget_cb(dialog_t *dialog,  widget_sig_dialog_e signal)
{
    switch(signal)
    {
        case WIDGET_SIG_DIALOG_OK:
        case WIDGET_SIG_DIALOG_CANCAL:
        {
            _delete_info_dialog_widget();
            break;
        }
        default :
        {
            break;
        }
    }
}

static bool _delete_info_dialog_widget(void)
{
    if(g_scene_list->dialog != NULL)
    {
        dialog_delete(g_scene_list->dialog);
        g_scene_list->dialog = NULL;
    }

    return true;
}

static bool _create_search_folder_adjust_widget(void)
{
    adjust_switch_init_t adjust_data = {0};
    menuitem_t * menuitem;

    menuitem = menu_get_item(s_menu, s_style_items[MENU_SEARCH_FOLDER].index);
    if(menuitem != NULL)
    {
        adjust_data.str_title_id = menuitem->text_id;
    }

    if(strcmp(g_setting_cfgs[SET_CFG_TYPE_SEARCH_FOLDER].text, "on") == 0)
    {
        adjust_data.on_off_status = ADJUST_ON;
    }
    else
    {
        adjust_data.on_off_status = ADJUST_OFF;
    }

    g_scene_list->adjust = adjust_create( &adjust_data, ADJ_STYLE_SWITCH, _search_forlder_widget_cb);
    if(NULL == g_scene_list->adjust)
    {
        print_err("create adjust failed!");
        return false;
    }

    adjust_paint(g_scene_list->adjust);

    return true;
}

static void _search_forlder_widget_cb(adjust_t *adjust, adjust_signal_e signal)
{
    adj_switch_mode_e switch_val;

    switch(signal)
    {
        case WIDGET_SIG_ADJUST_OK:
        {
            adjust_get_switch_status(&switch_val);
            if(ADJUST_ON == switch_val)
            {
                strncpy(g_setting_cfgs[SET_CFG_TYPE_SEARCH_FOLDER].text, "on", CFG_TEXT_LEN);
            }
            else
            {
                strncpy(g_setting_cfgs[SET_CFG_TYPE_SEARCH_FOLDER].text, "off", CFG_TEXT_LEN);
            }

            g_setting_cfgs[SET_CFG_TYPE_SEARCH_FOLDER].changed = true;
            setting_cfg_write(&g_setting_cfgs[SET_CFG_TYPE_SEARCH_FOLDER]);

            _delete_search_forlder_adjust_widget();
            menuview_paint( s_menuview );
            break;
        }

        case WIDGET_SIG_ADJUST_CANCEL:
        {
            _delete_search_forlder_adjust_widget();
            break;
        }

        case WIDGET_SIG_ADJUST_UP:
        case WIDGET_SIG_ADJUST_DOWN:
        {
            break;
        }

        default:
        {
            break;
        }
    }
}

static bool _delete_search_forlder_adjust_widget(void)
{
    if(g_scene_list->adjust != NULL)
    {
        adjust_delete(g_scene_list->adjust);
        g_scene_list->adjust = NULL;
    }

    return true;
}

static bool _create_gsensor_support_adjust_widget(void)
{
    adjust_switch_init_t adjust_data = {0};
    menuitem_t * menuitem;

    menuitem = menu_get_item(s_menu, s_style_items[MENU_GSENSOR_SHAKE_SUPPORT].index);
    if(menuitem != NULL)
    {
        adjust_data.str_title_id = menuitem->text_id;
    }

    if(strcmp(g_setting_cfgs[SET_CFG_TYPE_SEARCH_FOLDER].text, "on") == 0)
    {
        adjust_data.on_off_status = ADJUST_ON;
    }
    else
    {
        adjust_data.on_off_status = ADJUST_OFF;
    }

    g_scene_list->adjust = adjust_create( &adjust_data, ADJ_STYLE_SWITCH, _gsensor_support_widget_cb);
    if(NULL == g_scene_list->adjust)
    {
        print_err("create adjust failed!");
        return false;
    }

    adjust_paint(g_scene_list->adjust);

    return true;
}

static void _gsensor_support_widget_cb(adjust_t *adjust, adjust_signal_e signal)
{
    adj_switch_mode_e switch_val;
    msg_apps_t msg = {0};

    switch(signal)
    {
        case WIDGET_SIG_ADJUST_OK:
        {
            adjust_get_switch_status(&switch_val);
            if(ADJUST_ON == switch_val)
            {
                strncpy(g_setting_cfgs[SET_CFG_TYPE_GSENSOR_SHAKE_SUPPORT].text, "on", CFG_TEXT_LEN);
            }
            else
            {
                strncpy(g_setting_cfgs[SET_CFG_TYPE_GSENSOR_SHAKE_SUPPORT].text, "off", CFG_TEXT_LEN);
            }

            g_setting_cfgs[SET_CFG_TYPE_GSENSOR_SHAKE_SUPPORT].changed = true;
            setting_cfg_write(&g_setting_cfgs[SET_CFG_TYPE_GSENSOR_SHAKE_SUPPORT]);


            msg.type = MSG_RESET_GSENSOR_SHAKE;
            send_async_msg(APP_NAME_MSG_SERV, &msg);
            _delete_gsensor_support_adjust_widget();
            menuview_paint( s_menuview );
            break;
        }

        case WIDGET_SIG_ADJUST_CANCEL:
        {
            _delete_gsensor_support_adjust_widget();
            break;
        }

        case WIDGET_SIG_ADJUST_UP:
        case WIDGET_SIG_ADJUST_DOWN:
        {
            break;
        }

        default:
        {
            break;
        }
    }
}

static bool _delete_gsensor_support_adjust_widget(void)
{
    if(g_scene_list->adjust != NULL)
    {
        adjust_delete(g_scene_list->adjust);
        g_scene_list->adjust = NULL;
    }

    return true;
}

static bool _create_progress_box_widget(void)
{
    progress_box_init_t init = {0};
    unsigned int total_cap, cur_cap;

    system_codecheck(GET_TOAL_LEN, &total_cap);
    cur_cap = 0;

    init.text_str_id = s_str_res_table[STR_NAME_INFO_IN_SYS_CHECKING].str_id;
    init.complete_text_str_id = s_str_res_table[STR_NAME_INFO_SYS_CHECK_COMPLETE].str_id;
    init.cur_step = cur_cap;
    init.total_step = total_cap;

    g_scene_list->progress_box = progress_box_create( &init,  (progress_box_cb_t)_progress_box_widget_cb);
    if(NULL == g_scene_list->progress_box)
    {
        print_err("create progress box failed!");
        return false;
    }

    progress_box_paint(g_scene_list->progress_box);
    s_monitor_sys_check_timer_id = set_timer(200, _monitor_sys_check_status_timer, NULL);

    return true;
}

static void _progress_box_widget_cb(progress_box_t *progress_box, progress_box_sig_e signal)
{
    switch(signal)
    {
        case WIDGET_SIG_PROGRESS_BOX_COMPLETE:
        {
            kill_timer(s_monitor_sys_check_timer_id);
            s_monitor_sys_check_timer_id = -1;

            if(sync_config() < 0)
            {
                print_err("invoke sync_config() failed!");
            }

            set_single_shot_timer(2000, _reboot_system, NULL);

            break;
        }
        default:
        {
            break;
        }
    }
}

static bool _delete_progress_box_widget(void)
{
    if(g_scene_list->progress_box!= NULL)
    {
        progress_box_delete(g_scene_list->progress_box);
        g_scene_list->progress_box = NULL;
    }

    if(s_monitor_sys_check_timer_id > 0)
    {
        kill_timer(s_monitor_sys_check_timer_id);
        s_monitor_sys_check_timer_id = -1;
    }

    return true;
}

static int _monitor_sys_check_status_timer(void *param)
{
    unsigned int cur_cap;

    system_codecheck(GET_CUR_LEN, &cur_cap);

    if(g_scene_list->progress_box != NULL)
    {
        progress_box_set_cur_step(g_scene_list->progress_box, cur_cap);
    }

    return 0;
}

#pragma __PRQA_IGNORE_START__
/* 鐗规剰鍒堕�犳寰幆锛屽疄鐜皐atchdog閲嶅惎绯荤粺鐨勭洰鐨? 灞忚斀QAC妫�鏌?*/

static int _reboot_system(void *param)
{
    int wd_fd = -1;
    int ret;
    int time_out = 1000; 
    bool loop = true;

    wd_fd = open( DEVICE_WD, O_RDWR, 0);
    if( wd_fd < 0 )
    {
        print_err("Error: open watchdog device\n\n");
    }
    
    ret = ioctl(wd_fd, WDD_CMD_SET_TIME, time_out);

    while(loop == true)
    {
        loop = true;
    }

    close(wd_fd);

    return 0;
}

#pragma __PRQA_IGNORE_END__

static bool _create_loadingbox_widget( void )
{
    if(g_scene_list->loadingbox == NULL)
    {
        g_scene_list->loadingbox = loadingbox_create( LOADINGBOX_WAITING, _loadingbox_widget_cb );
        if ( NULL == g_scene_list->loadingbox )
        {
            print_err("loadingbox_create failed\n\n");
            return false;
        }
        else
        {
            //paint
            loadingbox_paint( g_scene_list->loadingbox );    
        }
    }

    return true;
}

static bool _delete_loadingbox_widget( void )
{    
    if(g_scene_list->loadingbox != NULL)
    {
        loadingbox_delete( g_scene_list->loadingbox );
        g_scene_list->loadingbox = NULL;
    }

    return true;
}

static void _loadingbox_widget_cb(loadingbox_t *loadingbox, loadingbox_sig_e signal)
{
}

static void * _thread_sys_check(void * data)
{
    sys_enable_sys_evt_detect(false);

    sys_forbid_auto_standby(true);
    sys_forbid_timed_shutdown(true);
    sys_forbid_screensaver(true);

    system_codecheck(START_CHECK, NULL);

    sys_enable_sys_evt_detect(true);
    sys_forbid_auto_standby(false);
    sys_forbid_timed_shutdown(false);
    sys_forbid_screensaver(false);

    return NULL;
}

static void * _thread_format_internal(void * data)
{
    hotplugin_dev_type_e main_disk;

    sys_forbid_auto_standby(true);
    sys_forbid_screensaver(true);

    if(sys_umount(UDISK_ROOT,0) < 0)
    {
        print_err("sys_umount(%s) error!", UDISK_ROOT);
    }

    if(format(DEVICE_FLASH"u", "fat32") != 0)
    {
        print_err("format %s failed!", DEVICE_FLASH"u");
    }

    if(sys_mount(DEVICE_FLASH"u", UDISK_ROOT, "fat", (0 * 8) + 0, NULL) < 0)
    {
        print_err("sys_mount(%s, %s) error!", DEVICE_FLASH"u", UDISK_ROOT);
        main_disk = sys_get_main_disk();

        if(main_disk == HOTPLUGIN_DEV_TYPE_LOCAL)
        {
            sys_set_main_disk(HOTPLUGIN_DEV_TYPE_NONE);
        }
    }
    else
    {
        main_disk = sys_get_main_disk();
        if(main_disk == HOTPLUGIN_DEV_TYPE_NONE)
        {
            sys_set_main_disk(HOTPLUGIN_DEV_TYPE_LOCAL);
        }

        main_disk = sys_get_main_disk();
        if(main_disk == HOTPLUGIN_DEV_TYPE_LOCAL)
        {
            sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL, DIR_ALL), UDISK_ROOT);
        }
    }

    if(g_setting_cfgs[SET_CFG_TYPE_USER_FONT_VALID].value != 0)
    {
        strncpy(g_setting_cfgs[SET_CFG_TYPE_USER_FONT].text, "" , CFG_TEXT_LEN);
        g_setting_cfgs[SET_CFG_TYPE_USER_FONT_VALID].value = 0;

        g_setting_cfgs[SET_CFG_TYPE_USER_FONT_VALID].changed = true;
        setting_cfg_write(&g_setting_cfgs[SET_CFG_TYPE_USER_FONT_VALID]);

        g_setting_cfgs[SET_CFG_TYPE_USER_FONT].changed = true;
        setting_cfg_write(&g_setting_cfgs[SET_CFG_TYPE_USER_FONT]);

        need_reset_font = true;
    }

    sys_forbid_auto_standby(false);
    sys_forbid_screensaver(false);

    set_single_shot_timer(1, _format_done, NULL);

    return NULL;
}

static void * _thread_format_card(void * data)
{
    hotplugin_dev_type_e main_disk;
    hotplugin_dev_info_t card_info;

    sys_forbid_auto_standby(true);
    sys_forbid_screensaver(true);

    sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_CARD, &card_info);

    if (card_info.access == HOTPLUGIN_DEV_ACCESS_OK)
    {
        if (sys_umount(CARD_ROOT, 0) < 0)
        {
            print_err("sys_umount(%s) error!", CARD_ROOT);
        }
    }

    if (format(DEVICE_CARD, "fat32") != 0)
    {
        /* try fat16 */
        if (format(DEVICE_CARD, "fat16") != 0)
        {
            print_err("format %s failed!", DEVICE_CARD);
        }
    }

    if (sys_mount(DEVICE_CARD, CARD_ROOT, "fat", 0, NULL) < 0)
    {
        print_err("sys_mount(%s, %s) error!", DEVICE_CARD, CARD_ROOT);

        main_disk = sys_get_main_disk();

        if (main_disk == HOTPLUGIN_DEV_TYPE_CARD)
        {
            sys_set_main_disk(HOTPLUGIN_DEV_TYPE_NONE);
        }
    }
    else
    {
        main_disk = sys_get_main_disk();

        if (main_disk == HOTPLUGIN_DEV_TYPE_NONE)
        {
            sys_set_main_disk(HOTPLUGIN_DEV_TYPE_CARD);
        }

        main_disk = sys_get_main_disk();
        if (main_disk == HOTPLUGIN_DEV_TYPE_CARD)
        {
            sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_CARD, DIR_ALL), CARD_ROOT);
        }
    }

    sys_forbid_auto_standby(false);
    sys_forbid_screensaver(false);

    set_single_shot_timer(1, _format_done, NULL);

    return NULL;
}

static void * _thread_format_external(void * data)
{
    hotplugin_dev_type_e main_disk;
    hotplugin_dev_info_t uhost_info;

    sys_forbid_auto_standby(true);
    sys_forbid_screensaver(true);

    sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_UHOST, &uhost_info);

    if(uhost_info.access == HOTPLUGIN_DEV_ACCESS_OK)
    {
        if(sys_umount(EXTERNAL_ROOT,0) <  0)
        {
            print_err("sys_umount(%s) error!", EXTERNAL_ROOT);
        }
    }

    if(format(DEVICE_USTOR, "fat32") != 0)
    {
        print_err("format %s failed!", DEVICE_USTOR);
    }

    if(sys_mount(DEVICE_USTOR, EXTERNAL_ROOT, "fat", 0, NULL) < 0 )
    {
        print_err("sys_mount(%s, %s) error!", DEVICE_USTOR, EXTERNAL_ROOT);
        main_disk = sys_get_main_disk();

        if(main_disk == HOTPLUGIN_DEV_TYPE_UHOST)
        {
            sys_set_main_disk(HOTPLUGIN_DEV_TYPE_NONE);
        }
    }
    else
    {
        main_disk = sys_get_main_disk();

        if(main_disk == HOTPLUGIN_DEV_TYPE_NONE)
        {
            sys_set_main_disk(HOTPLUGIN_DEV_TYPE_UHOST);
        }

        main_disk = sys_get_main_disk();
        if(main_disk == HOTPLUGIN_DEV_TYPE_UHOST)
        {
            sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_UHOST, DIR_ALL), EXTERNAL_ROOT);
        }
    }

    sys_forbid_auto_standby(false);
    sys_forbid_screensaver(false);

    set_single_shot_timer(1, _format_done, NULL);

    return NULL;
}

static int _format_done(void * param)
{
    msg_apps_t msg = {0};
    menuitem_t* menu_item;

    if(need_reset_font == true)
    {
        // emit font changed widget signal
        setting_list_widget_menu_emit_signal(s_menuview, WIDGET_SIG_MENU_FONT_CHANGED);

        // brodcast font changed msg
        msg.type = MSG_FONT_CHANGED;
        broadcast_msg(&msg);

        if(s_style_items[MENU_FONT].visible == true)
        {
            menu_item = menu_get_item( s_menu, s_style_items[MENU_FONT].index);
            if(menu_item != NULL)
            {
                menu_item->info1 = (char *)setting_list_menu_font_select_get_cur_selected_text();
            }
        }

        need_reset_font = false;
    }

    _delete_loadingbox_widget();
    _popup_info_dialog(s_str_res_table[STR_NAME_INFO_FMT_COMPLETE].str_id, 3000);

    sys_enable_sys_evt_detect(true);
    sys_forbid_timed_shutdown(false);

    return 0;
}

static bool _create_thread( void *proc, void * arg )
{
    pthread_attr_t attr;
    struct sched_param param;
    pthread_t tid;
    int policy;

    pthread_getschedparam( pthread_self(), &policy, &param );
    param.sched_priority = param.sched_priority - 2;

    pthread_attr_init(&attr);
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_PROCESS);
    pthread_attr_setschedparam(&attr, &param);

    if(pthread_create(&tid, &attr, proc, arg ) != 0)
    {
        print_err("create thread failed!");

        pthread_attr_destroy(&attr);
        return false;
    }

    pthread_attr_destroy(&attr);
    return true;
}

static bool _popup_info_dialog(unsigned short str_id, unsigned int stay_time)
{
    _create_info_dialog_widget(str_id);

    if(s_info_dialog_timer_id == -1)
    {
        set_single_shot_timer(stay_time, _info_dialog_timeout, NULL);
    }
    else
    {
        modify_timer(s_info_dialog_timer_id, stay_time);
        restart_timer(s_info_dialog_timer_id);
    }

    return true;
}

static int _info_dialog_timeout(void * param)
{
    _delete_info_dialog_widget();
    return 0;
}

