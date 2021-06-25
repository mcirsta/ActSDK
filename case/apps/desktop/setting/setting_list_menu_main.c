/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 * file : setting_list_menu_main.c
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
static void _menu_update_minor_info(void);
static void _check_menu_visible( void );

/* menu  item callbacks */
static void _theme_cb( menuview_t *menuview, menuview_sig_e signal );
static void _screen_mode_cb( menuview_t *menuview, menuview_sig_e signal );
static void _display_cb( menuview_t *menuview, menuview_sig_e signal );
static void _power_cb( menuview_t *menuview, menuview_sig_e signal );
static void _date_time_cb( menuview_t *menuview, menuview_sig_e signal );
static void _language_cb( menuview_t *menuview, menuview_sig_e signal );
static void _tvout_cb( menuview_t *menuview, menuview_sig_e signal );
static void _sync_plist_cb( menuview_t *menuview, menuview_sig_e signal );
static void _advanced_cb( menuview_t *menuview, menuview_sig_e signal );

/*********************************************************************************
 * internal static struct and data definition
 *********************************************************************************/

enum menuitem_name
{
    MENU_THEME = 0,
    MENU_SCR_MODE,
    MENU_DISPLAY,
    MENU_POWER,
    MENU_DATE_TIME,
    MENU_LANGUAGE,
    MENU_TVOUT,
    MENU_SYNC_PLIST,
    MENU_ADVANCED,
};

/* menu items style data define */
static menuitem_style_t s_style_items[] =
{
    { PIC_THEME_UNSEL,     PIC_THEME_SEL,     STR_THEME,      true, (item_cb_t)_theme_cb       , NULL, true, 0 },
    { PIC_SCR_MODE_UNSEL,  PIC_SCR_MODE_SEL,  STR_SCR_MODE,   true, (item_cb_t)_screen_mode_cb , NULL, true, 0 },
    { PIC_DISPLAY_UNSEL,   PIC_DISPLAY_SEL,   STR_DISPLAY,    true, (item_cb_t)_display_cb     , NULL, true, 0 },
    { PIC_POWER_UNSEL,     PIC_POWER_SEL,     STR_POWER,      true, (item_cb_t)_power_cb       , NULL, true, 0 },
    { PIC_DATE_TIME_UNSEL, PIC_DATE_TIME_SEL, STR_DATE_TIME,  true, (item_cb_t)_date_time_cb   , NULL, true, 0 },
    { PIC_LANG_UNSEL,      PIC_LANG_SEL,      STR_LANG,       true, (item_cb_t)_language_cb    , NULL, true, 0 },
    { PIC_TVOUT_UNSEL,     PIC_TVOUT_SEL,     STR_TVOUT,      true, (item_cb_t)_tvout_cb       , NULL, true, 0 },
    { PIC_PLIST_UNSEL,     PIC_PLIST_SEL,     STR_SYNC_PLIST, true, (item_cb_t)_sync_plist_cb  , NULL, true, 0 },
    { PIC_ADVANCED_UNSEL,  PIC_ADVANCED_SEL,  STR_ADVANCED,   true, (item_cb_t)_advanced_cb    , NULL, true, 0 },
};

static menu_t *s_menu = NULL;
static int s_menu_id = -1;


/*********************************************************************************
 * interface implementaiton
 *********************************************************************************/

menu_t* setting_list_menu_main_enter( menuview_t *menuview, fusion_enter_e fusion_enter_type)
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

    if(fusion_enter_type == FUSION_FORWARD)
    {
        menuview_paint(menuview);
    }
    else
    {
        setting_list_widget_menu_show_fusion(menuview, fusion_enter_type);
    }

    return s_menu;
}


/*********************************************************************************
 * internal static  implementaiton
 *********************************************************************************/

static bool _menu_init( void )
{
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

        //load main menu resource group
        res_group = (resgroup_resource_t *)get_scene_child(scene, REG_MAIN_MENU);
        if(NULL == res_group)
        {
            print_err("get resource group REG_MAIN_MENU");
            return false;
        }

        _check_menu_visible();

        real_items = menu_load_item_resource(res_group, s_style_items, menu_items, ARRAY_SIZE(s_style_items));

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
    setting_cfg_init(g_menu_main_cfgs, CFG_MENU_MAIN_MAX); 
    setting_cfg_read_all(g_menu_main_cfgs, CFG_MENU_MAIN_MAX); 

    if(g_menu_main_cfgs[CFG_MENU_MAIN_THEME].value == 0)
    {
        s_style_items[MENU_THEME].visible = false;
    }

    if(g_menu_main_cfgs[CFG_MENU_MAIN_SCR_MODE].value == 0)
    {
        s_style_items[MENU_SCR_MODE].visible = false;
    }

    if(g_menu_main_cfgs[CFG_MENU_MAIN_DISPLAY].value == 0)
    {
        s_style_items[MENU_DISPLAY].visible = false;
    }

    if(g_menu_main_cfgs[CFG_MENU_MAIN_POWER].value == 0)
    {
        s_style_items[MENU_POWER].visible = false;
    }

    if(g_menu_main_cfgs[CFG_MENU_MAIN_DATE_TIME].value == 0)
    {
        s_style_items[MENU_DATE_TIME].visible = false;
    }

    if(g_menu_main_cfgs[CFG_MENU_MAIN_LANGUAGE].value == 0)
    {
        s_style_items[MENU_LANGUAGE].visible = false;
    }

    if(g_menu_main_cfgs[CFG_MENU_MAIN_TVOUT].value == 0)
    {
        s_style_items[MENU_TVOUT].visible = false;
    }

    if(g_menu_main_cfgs[CFG_MENU_MAIN_SYNC_PLIST].value == 0)
    {
        s_style_items[MENU_SYNC_PLIST].visible = false;
    }

    if(g_menu_main_cfgs[CFG_MENU_MAIN_ADVANCED].value == 0)
    {
        s_style_items[MENU_ADVANCED].visible = false;
    }

    setting_cfg_deinit(g_menu_main_cfgs, CFG_MENU_MAIN_MAX); 
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

    return true;
}

static void _menu_return( menuview_t *menuview )
{
    /* delete menu data */
    _menu_deinit();

    /* exit mutilevel menu widget */
    setting_list_widget_menu_exit( menuview, NULL );
}

static void _menu_update_minor_info(void)
{
    menuitem_t* menu_item;

    if(s_style_items[MENU_THEME].visible == true)
    {
        menu_item = menu_get_item( s_menu, s_style_items[MENU_THEME].index);
        if(menu_item != NULL)
        {
            menu_item->info1 = (char *)setting_list_menu_theme_get_cur_selected_text();
        }
    }

    if(s_style_items[MENU_SCR_MODE].visible == true)
    {
        menu_item = menu_get_item( s_menu, s_style_items[MENU_SCR_MODE].index);
        if(menu_item != NULL)
        {
            menu_item->info1 = (char *)setting_list_menu_screen_mode_get_cur_selected_text();
        }
    }

    if(s_style_items[MENU_LANGUAGE].visible == true)
    {
        menu_item = menu_get_item( s_menu, s_style_items[MENU_LANGUAGE].index);
        if(menu_item != NULL)
        {
            menu_item->info1 = (char *)setting_list_menu_language_get_cur_selected_text();
        }
    }

    if(s_style_items[MENU_TVOUT].visible == true)
    {
        menu_item = menu_get_item( s_menu, s_style_items[MENU_TVOUT].index);
        if(menu_item != NULL)
        {
            menu_item->info1 = (char *)setting_list_menu_tvout_get_cur_selected_text();
        }
    }
}

static void _theme_cb( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal )
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            // enter theme select menu
            setting_list_menu_theme_enter( menuview, FUSION_FORWARD);
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

static void _screen_mode_cb( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal )
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            // enter sceen mode config menu
            setting_list_menu_screen_mode_enter(menuview, FUSION_FORWARD);
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

static void _display_cb( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal )
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            // enter display config menu
            setting_list_menu_display_enter(menuview, FUSION_FORWARD);
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

static void _power_cb( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal )
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            // enter power config menu
            setting_list_menu_power_enter(menuview, FUSION_FORWARD);
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

static void _date_time_cb( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal )
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            // enter date and time config menu
            setting_list_menu_date_time_enter(menuview, FUSION_FORWARD);
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

static void _language_cb( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal )
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            // enter language config menu
            setting_list_menu_language_enter(menuview, FUSION_FORWARD);
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

static void _tvout_cb( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal )
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            // enter tv out config menu
            setting_list_menu_tvout_enter(menuview, FUSION_FORWARD);
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

static void _sync_plist_cb( menuview_t *menuview, menuview_sig_e signal )
{
    msg_apps_t msg = {0};
    switch ( signal )
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            enter_background_mode();
            msg.type = MSG_CREAT_APP;
            strcpy(msg.content, APP_NAME_PL_GEN);
            send_async_msg(APP_NAME_MANAGER, &msg);

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

static void _advanced_cb( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal )
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            // enter advanced config menu
            setting_list_menu_advanced_enter(menuview, FUSION_FORWARD);
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

