/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 * file : setting_list_menu_power.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-5-5             v1.0             build this file
 ********************************************************************************/

#include "setting_list_widget_menu.h"

/*********************************************************************************
 * internal static functions declaration
 *********************************************************************************/
static bool _menu_init( void );
static bool _menu_deinit( void );
static void _menu_return( menuview_t *menuview );
static void _check_menu_visible( void );

/* menu  item callbacks */
static void _idle_time_cb( menuview_t *menuview, menuview_sig_e signal );
static void _sleep_time_cb( menuview_t *menuview, menuview_sig_e signal );

static bool _create_idle_time_adjust_widget(void);
static void _idle_time_adjust_widget_cb(adjust_t *adjust, adjust_signal_e signal);
static bool _delete_idle_time_adjust_widget(void);

static bool _create_sleep_time_adjust_widget(void);
static void _sleep_time_adjust_widget_cb(adjust_t *adjust, adjust_signal_e signal);
static bool _delete_sleep_time_adjust_widget(void);

/*********************************************************************************
 * internal static struct and data definition
 *********************************************************************************/

enum menuitem_name
{
    MENU_IDLE_TIME = 0,
    MENU_SLEEP_TIME,
};

/* menu items style data define */
static menuitem_style_t s_style_items[] =
{
    { 0, 0, STR_IDLE_TIME,   false, (item_cb_t)_idle_time_cb , NULL, true, 0},
    { 0, 0, STR_SLEEP_TIME,  false, (item_cb_t)_sleep_time_cb, NULL, true, 0},
};

enum idle_time_value
{
    IDLE_OFF,
    IDLE_10_SEC,
    IDLE_30_SEC,
    IDLE_1_MIN,
    IDLE_3_MIN,
    IDLE_5_MIN,
    IDLE_10_MIN,
};

static value_map_t idle_time_maps[] =
{
    {0,    "Off"},
    {10,   "10 Second"},
    {30,   "30 Second"},
    {60,   "1 Minute"},
    {180,  "3 Minute"},
    {300,  "5 Minute"},
    {600,  "10 Minute"},
};

enum sleep_time_value
{
    SLEEP_OFF,
    SLEEP_10_MIN,
    SLEEP_30_MIN,
    SLEEP_1_HR,
    SLEEP_2_HR,
    SLEEP_3_HR,
    SLEEP_4_HR,
    SLEEP_5_HR,
};

static value_map_t sleep_time_maps[] =
{
    {0,      "Off"},
    {600,    "10 Minute"},
    {1800,   "30 Minute"},
    {3600,   "1 Hour"},
    {7200,   "2 Hour"},
    {10800,  "3 Hour"},
    {14400,  "4 Hour"},
    {18000,  "5 Hour"},
};

static menu_t *s_menu = NULL;
static menuview_t *s_menuview = NULL;
static enum idle_time_value cur_idle_time = IDLE_OFF;
static enum sleep_time_value cur_sleep_time = SLEEP_OFF;
static int s_menu_id = -1;

static char s_idle_time_str[64] = {0};
static char s_sleep_time_str[64] = {0};


/*********************************************************************************
 * interface implementaiton
 *********************************************************************************/
menu_t* setting_list_menu_power_enter( menuview_t *menuview, fusion_enter_e fusion_enter_type)
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
        sub_menu.update_minor_info_cb = NULL;
        s_menu_id = setting_list_widget_menu_register_sub_menu(menuview, &sub_menu);
    }

    setting_list_widget_menu_show_fusion(menuview, fusion_enter_type);

    s_menuview = menuview;
    return s_menu;
}


/*********************************************************************************
 * internal static  implementaiton
 *********************************************************************************/
static bool _menu_init( void )
{
    int value;
    scene_t * scene;
    resgroup_resource_t * res_group;

#pragma __PRQA_IGNORE_START__
    /* 使用 0 初始化数组 */
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

        //load power menu resource group
        res_group = (resgroup_resource_t *)get_scene_child(scene, REG_POWER);
        if(NULL == res_group)
        {
            print_err("get resource group REG_POWER");
            return false;
        }

        _check_menu_visible();
        real_items = menu_load_item_resource(res_group, s_style_items, menu_items, ARRAY_SIZE(s_style_items));

        if(s_style_items[MENU_IDLE_TIME].visible == true)
        {
            value = g_setting_cfgs[SET_CFG_TYPE_IDLE_TIME].value;
            cur_idle_time = maps_find(idle_time_maps, ARRAY_SIZE(idle_time_maps), value);
            strncpy(s_idle_time_str, idle_time_maps[cur_idle_time].show_text, sizeof(s_idle_time_str));
            menu_items[s_style_items[MENU_IDLE_TIME].index].info1 = s_idle_time_str;
        }

        if(s_style_items[MENU_SLEEP_TIME].visible == true)
        {
            value = g_setting_cfgs[SET_CFG_TYPE_SLEEP_TIME].value;
            cur_sleep_time = maps_find(sleep_time_maps, ARRAY_SIZE(sleep_time_maps), value);
            strncpy(s_sleep_time_str, sleep_time_maps[cur_sleep_time].show_text, sizeof(s_sleep_time_str));
            menu_items[s_style_items[MENU_SLEEP_TIME].index].info1 = s_sleep_time_str;
        }

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
    setting_cfg_init(g_menu_power_cfgs, CFG_MENU_POWER_MAX); 
    setting_cfg_read_all(g_menu_power_cfgs, CFG_MENU_POWER_MAX); 

    if(g_menu_power_cfgs[CFG_MENU_POWER_IDLE_TIME].value == 0)
    {
        s_style_items[MENU_IDLE_TIME].visible = false;
    }

    if(g_menu_power_cfgs[CFG_MENU_POWER_SLEEP_TIME].value == 0)
    {
        s_style_items[MENU_SLEEP_TIME].visible = false;
    }

    setting_cfg_deinit(g_menu_power_cfgs, CFG_MENU_POWER_MAX); 
}

static bool _menu_deinit( void )
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

    _delete_idle_time_adjust_widget();
    _delete_sleep_time_adjust_widget();

    return true;
}

static void _menu_return( menuview_t *menuview )
{
    _menu_deinit();

    //return to parent menu
    setting_list_menu_main_enter(menuview, FUSION_BACKWARD);
}

static void _idle_time_cb( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal )
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            _create_idle_time_adjust_widget();
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

static void _sleep_time_cb( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal )
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            _create_sleep_time_adjust_widget();
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

static bool _create_idle_time_adjust_widget(void)
{
    unsigned int i;
    adjust_textdigit_init_t adjust_data = {0};
    menuitem_t * menuitem;

    menuitem = menu_get_item(s_menu, s_style_items[MENU_IDLE_TIME].index);
    if(menuitem != NULL)
    {
        adjust_data.str_title_id = menuitem->text_id;
    }

    adjust_data.max_val = IDLE_10_MIN;
    adjust_data.min_val = IDLE_OFF;
    adjust_data.cur_val = cur_idle_time;
    adjust_data.step = 1;

    for(i = 0; i < ARRAY_SIZE(idle_time_maps); i++)
    {
        adjust_data.str_disp[i] = idle_time_maps[i].show_text;
    }

    g_scene_list->adjust = adjust_create( &adjust_data, ADJ_STYLE_TEXTDIGIT, _idle_time_adjust_widget_cb);
    if(NULL == g_scene_list->adjust)
    {
        print_err("create adjust failed!");
        return false;
    }

    adjust_paint(g_scene_list->adjust);

    return true;
}

static bool _delete_idle_time_adjust_widget(void)
{
    if(g_scene_list->adjust != NULL)
    {
        adjust_delete(g_scene_list->adjust);
        g_scene_list->adjust = NULL;
    }

    return true;
}

static void _idle_time_adjust_widget_cb(adjust_t *adjust, adjust_signal_e signal)
{
    int i;

    switch(signal)
    {
        case WIDGET_SIG_ADJUST_OK:
        {
            adjust_get_current_val(&i);
            cur_idle_time = i;

            g_setting_cfgs[SET_CFG_TYPE_IDLE_TIME].value = idle_time_maps[cur_idle_time].value;
            g_setting_cfgs[SET_CFG_TYPE_IDLE_TIME].changed = true;
            setting_cfg_write(&g_setting_cfgs[SET_CFG_TYPE_IDLE_TIME]); 

            strncpy(s_idle_time_str, idle_time_maps[cur_idle_time].show_text, sizeof(s_idle_time_str));

            _delete_idle_time_adjust_widget();
            menuview_paint( s_menuview );

            sys_set_auto_standby_idle_time((unsigned int)g_setting_cfgs[SET_CFG_TYPE_IDLE_TIME].value);
            break;
        }

        case WIDGET_SIG_ADJUST_CANCEL:
        {
            _delete_idle_time_adjust_widget();
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

static bool _create_sleep_time_adjust_widget(void)
{
    unsigned int i;
    adjust_textdigit_init_t adjust_data = {0};
    menuitem_t * menuitem;

    menuitem = menu_get_item(s_menu, s_style_items[MENU_SLEEP_TIME].index);
    if(menuitem != NULL)
    {
        adjust_data.str_title_id = menuitem->text_id;
    }

    adjust_data.max_val = SLEEP_5_HR;
    adjust_data.min_val = SLEEP_OFF;
    adjust_data.cur_val = cur_sleep_time;
    adjust_data.step = 1;

    for(i = 0; i < ARRAY_SIZE(sleep_time_maps); i++)
    {
        adjust_data.str_disp[i] = sleep_time_maps[i].show_text;
    }

    g_scene_list->adjust = adjust_create( &adjust_data, ADJ_STYLE_TEXTDIGIT, _sleep_time_adjust_widget_cb);
    if(NULL == g_scene_list->adjust)
    {
        print_err("create adjust failed!");
        return false;
    }

    adjust_paint(g_scene_list->adjust);

    return true;
}

static bool _delete_sleep_time_adjust_widget(void)
{
    if(g_scene_list->adjust != NULL)
    {
        adjust_delete(g_scene_list->adjust);
        g_scene_list->adjust = NULL;
    }

    return true;
}

static void _sleep_time_adjust_widget_cb(adjust_t *adjust, adjust_signal_e signal)
{
    int i;

    switch(signal)
    {
        case WIDGET_SIG_ADJUST_OK:
        {
            adjust_get_current_val(&i);
            cur_sleep_time = i;

            g_setting_cfgs[SET_CFG_TYPE_SLEEP_TIME].value = sleep_time_maps[cur_sleep_time].value;
            g_setting_cfgs[SET_CFG_TYPE_SLEEP_TIME].changed = true;
            setting_cfg_write(&g_setting_cfgs[SET_CFG_TYPE_SLEEP_TIME]); 

            strncpy(s_sleep_time_str, sleep_time_maps[cur_sleep_time].show_text, sizeof(s_sleep_time_str));

            _delete_sleep_time_adjust_widget();
            menuview_paint( s_menuview );

            sys_set_timed_shutdown_time((unsigned int)g_setting_cfgs[SET_CFG_TYPE_SLEEP_TIME].value);
            break;
        }
        case WIDGET_SIG_ADJUST_CANCEL:
        {
            _delete_sleep_time_adjust_widget();
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
