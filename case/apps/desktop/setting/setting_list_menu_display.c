/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 * file : setting_list_menu_display.c
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
static void _brightness_cb( menuview_t *menuview, menuview_sig_e signal );
static void _backlight_cb( menuview_t *menuview, menuview_sig_e signal );

static bool _create_brightness_adjust_widget(void);
static void _brightness_adjust_widget_cb(adjust_t *adjust, adjust_signal_e signal);
static bool _delete_brightness_adjust_widget(void);

static bool _create_backlight_adjust_widget(void);
static void _backlight_adjust_widget_cb(adjust_t *adjust, adjust_signal_e signal);
static bool _delete_backlight_adjust_widget(void);


/*********************************************************************************
 * internal static struct and data definition
 *********************************************************************************/

enum menuitem_name
{
    MENU_BRIGHTNESS = 0,
    MENU_BACKLIGHT,
};

/* menu items style data define */
static menuitem_style_t s_style_items[] =
{
    { 0, 0, STR_BRIGHTNESS, false, (item_cb_t)_brightness_cb, NULL, true, 0 },
    { 0, 0, STR_BACKLIGHT,  false, (item_cb_t)_backlight_cb , NULL, true, 0 },
};

enum backlight_time_value
{
    BACKLIGHT_OFF,
    BACKLIGHT_5_SEC,
    BACKLIGHT_10_SEC,
    BACKLIGHT_20_SEC,
    BACKLIGHT_30_SEC,
    BACKLIGHT_1_MIN,
    BACKLIGHT_5_MIN,
};

static value_map_t backlight_time_maps[] =
{
    {0,    "Off"},
    {5,    "5 Second"},
    {10,   "10 Second"},
    {20,   "20 Second"},
    {30,   "30 Second"},
    {60,   "1 Minute"},
    {300,  "5 Minute"},
};


static menu_t *s_menu = NULL;
static menuview_t *s_menuview = NULL;
static enum backlight_time_value cur_bcklight_time = BACKLIGHT_OFF;
static int s_menu_id = -1;

static char s_brightness_str[64] = {0};
static char s_backlight_str[64] = {0};


/*********************************************************************************
 * interface implementaiton
 *********************************************************************************/

menu_t* setting_list_menu_display_enter( menuview_t *menuview, fusion_enter_e fusion_enter_type)
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
        if ( scene == NULL )
        {
            print_err("get app scene failed!");
            return false;
        }

        //load display menu resource group
        res_group = (resgroup_resource_t *)get_scene_child(scene, REG_DISPLAY);
        if(NULL == res_group)
        {
            print_err("get resource group REG_DISPLAY");
            return false;
        }

        _check_menu_visible();
        real_items = menu_load_item_resource(res_group, s_style_items, menu_items, ARRAY_SIZE(s_style_items));

        if(s_style_items[MENU_BRIGHTNESS].visible == true)
        {
            value = g_setting_cfgs[SET_CFG_TYPE_BRIGHTNESS].value;
            sprintf(s_brightness_str, "%d", value);
            menu_items[s_style_items[MENU_BRIGHTNESS].index].info1 = s_brightness_str;
        }


        if(s_style_items[MENU_BACKLIGHT].visible == true)
        {
            value = g_setting_cfgs[SET_CFG_TYPE_BACKLIGHT].value;
            cur_bcklight_time = maps_find(backlight_time_maps, ARRAY_SIZE(backlight_time_maps), value);
            strncpy(s_backlight_str, backlight_time_maps[cur_bcklight_time].show_text, sizeof(s_backlight_str));
            menu_items[s_style_items[MENU_BACKLIGHT].index].info1 = s_backlight_str;
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
    setting_cfg_init(g_menu_display_cfgs, CFG_MENU_DISPLAY_MAX); 
    setting_cfg_read_all(g_menu_display_cfgs, CFG_MENU_DISPLAY_MAX); 

    if(g_menu_display_cfgs[CFG_MENU_DISPLAY_BRIGHTNESS].value == 0)
    {
        s_style_items[MENU_BRIGHTNESS].visible = false;
    }

    if(g_menu_display_cfgs[CFG_MENU_DISPLAY_BACKLIGHT].value == 0)
    {
        s_style_items[MENU_BACKLIGHT].visible = false;
    }

    setting_cfg_deinit(g_menu_display_cfgs, CFG_MENU_DISPLAY_MAX); 
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

    _delete_brightness_adjust_widget();
    _delete_backlight_adjust_widget();

    return true;
}

static void _menu_return( menuview_t *menuview )
{
    _menu_deinit();

    //return to parent menu
    setting_list_menu_main_enter(menuview, FUSION_BACKWARD);
}

static void _brightness_cb( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal )
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            _create_brightness_adjust_widget();
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

static void _backlight_cb( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal )
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            _create_backlight_adjust_widget();
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

static bool _create_brightness_adjust_widget(void)
{
    adjust_textdigit_init_t adjust_data = {0};
    menuitem_t * menuitem;

    menuitem = menu_get_item(s_menu, s_style_items[MENU_BRIGHTNESS].index);
    if(menuitem != NULL)
    {
        adjust_data.str_title_id = menuitem->text_id;
    }

    adjust_data.max_val = 5;
    adjust_data.min_val = 1;
    adjust_data.cur_val = g_setting_cfgs[SET_CFG_TYPE_BRIGHTNESS].value;
    adjust_data.step = 1;
    g_scene_list->adjust = adjust_create( &adjust_data, ADJ_STYLE_TEXTDIGIT, _brightness_adjust_widget_cb);
    if(NULL == g_scene_list->adjust)
    {
        print_err("create adjust failed!");
        return false;
    }

    adjust_paint(g_scene_list->adjust);
    return true;
}

static bool _delete_brightness_adjust_widget(void)
{
    if(g_scene_list->adjust != NULL)
    {
        adjust_delete(g_scene_list->adjust);
        g_scene_list->adjust = NULL;
    }

    return true;
}

static void _brightness_adjust_widget_cb(adjust_t *adjust, adjust_signal_e signal)
{
    int value;

    switch(signal)
    {
        case WIDGET_SIG_ADJUST_OK:
        case WIDGET_SIG_ADJUST_CANCEL:
        {
            _delete_brightness_adjust_widget();
            menuview_paint( s_menuview );
            break;
        }

        case WIDGET_SIG_ADJUST_UP:
        case WIDGET_SIG_ADJUST_DOWN:
        {
            adjust_get_current_val(&value);
            g_setting_cfgs[SET_CFG_TYPE_BRIGHTNESS].value = value;
            g_setting_cfgs[SET_CFG_TYPE_BRIGHTNESS].changed = true;
            setting_cfg_write(&g_setting_cfgs[SET_CFG_TYPE_BRIGHTNESS]);

            sprintf(s_brightness_str, "%d", value);
            sys_set_lcd_backlight((unsigned int)value);

            break;
        }

        default:
        {
            break;
        }
    }
}

static bool _create_backlight_adjust_widget(void)
{
    unsigned int i;
    adjust_textdigit_init_t adjust_data = {0};
    menuitem_t * menuitem;

    menuitem = menu_get_item(s_menu, s_style_items[MENU_BACKLIGHT].index);
    if(menuitem != NULL)
    {
        adjust_data.str_title_id = menuitem->text_id;
    }

    adjust_data.max_val = BACKLIGHT_5_MIN;
    adjust_data.min_val = BACKLIGHT_OFF;
    adjust_data.cur_val = cur_bcklight_time;
    adjust_data.step = 1;

    for(i = 0; i < ARRAY_SIZE(backlight_time_maps); i++)
    {
        adjust_data.str_disp[i] = backlight_time_maps[i].show_text;
    }

    g_scene_list->adjust = adjust_create( &adjust_data, ADJ_STYLE_TEXTDIGIT, _backlight_adjust_widget_cb);
    if(NULL == g_scene_list->adjust)
    {
        print_err("create adjust failed!");
        return false;
    }

    adjust_paint(g_scene_list->adjust);

    return true;
}

static bool _delete_backlight_adjust_widget(void)
{
    if(g_scene_list->adjust != NULL)
    {
        adjust_delete(g_scene_list->adjust);
        g_scene_list->adjust = NULL;
    }

    return true;
}

static void _backlight_adjust_widget_cb(adjust_t *adjust, adjust_signal_e signal)
{
    int i;

    switch(signal)
    {
        case WIDGET_SIG_ADJUST_OK:
        {
            adjust_get_current_val(&i);
            cur_bcklight_time = i;

            g_setting_cfgs[SET_CFG_TYPE_BACKLIGHT].value = backlight_time_maps[cur_bcklight_time].value;
            strncpy(s_backlight_str, backlight_time_maps[cur_bcklight_time].show_text, sizeof(s_backlight_str));
            g_setting_cfgs[SET_CFG_TYPE_BACKLIGHT].changed = true;
            setting_cfg_write(&g_setting_cfgs[SET_CFG_TYPE_BACKLIGHT]);

            _delete_backlight_adjust_widget();
            menuview_paint( s_menuview );
            sys_set_screensaver_idle_time((unsigned int)g_setting_cfgs[SET_CFG_TYPE_BACKLIGHT].value);

            break;
        }
        case WIDGET_SIG_ADJUST_CANCEL:
        {
            _delete_backlight_adjust_widget();
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
