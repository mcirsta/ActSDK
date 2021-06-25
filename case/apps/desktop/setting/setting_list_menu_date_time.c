/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 * file : setting_list_menu_date_time.c
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
static void _adjust_date_cb( menuview_t *menuview, menuview_sig_e signal );
static void _adjust_time_cb( menuview_t *menuview, menuview_sig_e signal );
static void _drm_time_cb( menuview_t *menuview, menuview_sig_e signal );

static bool _create_date_adjust_widget(void);
static void _date_adjust_widget_cb(adjust_t *adjust, adjust_signal_e signal);
static bool _delete_date_adjust_widget(void);

static bool _create_time_adjust_widget(void);
static void _time_adjust_widget_cb(adjust_t *adjust, adjust_signal_e signal);
static bool _delete_time_adjust_widget(void);

static int s_update_date_timer_id  = 0;
static int _update_date_timer_cb(void *param);

static int s_update_drm_timer_id  = 0;
static int _update_drm_timer_cb(void *param);

static bool _set_cur_date(rtc_date_t * date);
static bool _set_cur_time(rtc_time_t * time);

static inline void _format_date_str( rtc_date_t * date, char * date_str);
static inline void _format_time_str( rtc_time_t * time, char * time_str);

/*********************************************************************************
 * internal static struct and data definition
 *********************************************************************************/

enum menuitem_name
{
    MENU_ADJUST_DATE = 0,
    MENU_ADJUST_TIME,
    MENU_DRM_TIME,
};

/* menu items style data define */
static menuitem_style_t s_style_items[] =
{
    { 0, 0, STR_ADJUST_DATE,   false, (item_cb_t)_adjust_date_cb, NULL, true, 0},
    { 0, 0, STR_ADJUST_TIME,  false, (item_cb_t)_adjust_time_cb , NULL, true, 0},
    { 0, 0, STR_DRM_TIME,  false, (item_cb_t)_drm_time_cb       , NULL, true, 0},
};


static menu_t *s_menu = NULL;
static menuview_t *s_menuview = NULL;
static int s_menu_id = -1;

static rtc_time_t  s_cur_time;
static rtc_date_t  s_cur_date;
static char s_cur_time_str[64];
static char s_cur_date_str[64];

static rtc_time_t  s_cur_drm_time;
static rtc_date_t  s_cur_drm_date;
static char s_cur_drm_time_str[64];
static char s_cur_drm_date_str[64];
static char s_cur_drm_date_time_str[128];

static time_format_e s_time_format = TIME_FORMAT_24;

/*********************************************************************************
 * interface implementaiton
 *********************************************************************************/
menu_t* setting_list_menu_date_time_enter( menuview_t *menuview, fusion_enter_e fusion_enter_type)
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
    scene_t * scene;
    resgroup_resource_t * res_group;
    int value = 0;

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

        //load date and time adjust menu resource group
        res_group = (resgroup_resource_t *)get_scene_child(scene, REG_DATE_TIME);
        if(NULL == res_group)
        {
            print_err("get resource group REG_DATE_TIME");
            return false;
        }

        _check_menu_visible();
        real_items = menu_load_item_resource(res_group, s_style_items, menu_items, ARRAY_SIZE(s_style_items));

        _update_date_timer_cb(NULL);
        _update_drm_timer_cb(NULL);

        if(s_style_items[MENU_ADJUST_DATE].visible == true)
        {
            menu_items[s_style_items[MENU_ADJUST_DATE].index].info1 = s_cur_date_str;
        }

        if(s_style_items[MENU_ADJUST_TIME].visible == true)
        {
            menu_items[s_style_items[MENU_ADJUST_TIME].index].info1 = s_cur_time_str;
        }

        if(s_style_items[MENU_DRM_TIME].visible == true)
        {
            menu_items[s_style_items[MENU_DRM_TIME].index].info1 = s_cur_drm_date_time_str;
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

    if((s_style_items[MENU_DRM_TIME].visible == true) || (s_style_items[MENU_DRM_TIME].visible == true))
    {
        /* update current date and time every 2 seconds */
        s_update_date_timer_id = set_timer(2000, _update_date_timer_cb, NULL);
    }

    if(s_style_items[MENU_DRM_TIME].visible == true)
    {
        /* update current DRM time every 2 seconds */
        s_update_drm_timer_id = set_timer(2000, _update_drm_timer_cb, NULL);
    }

    if(get_config(CFG_TIME_FORMAT, (char *)&value, sizeof(value)) < 0)
    {
        print_err("get_config(CFG_TIME_FORMAT) failed!");
    }

    if(value == 12)
    {
        s_time_format = TIME_FORMAT_12;
    }
    else
    {
        s_time_format = TIME_FORMAT_24;
    }

    return true;
}

static void _check_menu_visible( void )
{
    setting_cfg_init(g_menu_date_time_cfgs, CFG_MENU_DATE_TIME_MAX); 
    setting_cfg_read_all(g_menu_date_time_cfgs, CFG_MENU_DATE_TIME_MAX); 

    if(g_menu_date_time_cfgs[CFG_MENU_DATE_TIME_ADJUST_DATE].value == 0)
    {
        s_style_items[MENU_ADJUST_DATE].visible = false;
    }

    if(g_menu_date_time_cfgs[CFG_MENU_DATE_TIME_ADJUST_TIME].value == 0)
    {
        s_style_items[MENU_ADJUST_TIME].visible = false;
    }

    if(g_menu_date_time_cfgs[CFG_MENU_DATE_TIME_DRM_TIME].value == 0)
    {
        s_style_items[MENU_DRM_TIME].visible = false;
    }

    setting_cfg_deinit(g_menu_date_time_cfgs, CFG_MENU_DATE_TIME_MAX); 
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

    if(s_update_date_timer_id != 0)
    {
        kill_timer(s_update_date_timer_id);
        s_update_date_timer_id = 0;
    }

    if(s_menu_id != -1)
    {
        setting_list_widget_menu_unregister_sub_menu(s_menu_id);
        s_menu_id = -1;
    }

    s_menuview = NULL;

    _delete_date_adjust_widget();
    _delete_time_adjust_widget();

    return true;
}

static void _menu_return( menuview_t *menuview )
{
    _menu_deinit();

    //return to parent menu
    setting_list_menu_main_enter(menuview, FUSION_BACKWARD);
}

static void _adjust_date_cb( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal )
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            _create_date_adjust_widget();
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

static void _adjust_time_cb( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal )
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            _create_time_adjust_widget();
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

static void _drm_time_cb( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal )
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
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

static bool _create_date_adjust_widget(void)
{
    adjust_date_init_t adjust_data = {0};
    menuitem_t * menuitem;

    menuitem = menu_get_item(s_menu, s_style_items[MENU_ADJUST_DATE].index);
    if(menuitem != NULL)
    {
        adjust_data.str_title_id = menuitem->text_id;
    }

    adjust_data.sys_date = s_cur_date;
    g_scene_list->adjust = adjust_create( &adjust_data, ADJ_STYLE_DATE, _date_adjust_widget_cb);

    if(NULL == g_scene_list->adjust)
    {
        print_err("create adjust failed!");
        return false;
    }

    adjust_paint(g_scene_list->adjust);

    return true;
}

static bool _delete_date_adjust_widget(void)
{
    if(g_scene_list->adjust != NULL)
    {
        adjust_delete(g_scene_list->adjust);
        g_scene_list->adjust = NULL;
    }

    return true;
}

static void _date_adjust_widget_cb(adjust_t *adjust, adjust_signal_e signal)
{
    rtc_date_t date;

    switch(signal)
    {
        case WIDGET_SIG_ADJUST_OK:
        {
            adjust_get_setting_date(&date);
            _delete_date_adjust_widget();

            _set_cur_date(&date);
            menuview_paint( s_menuview );
            break;
        }
        case WIDGET_SIG_ADJUST_CANCEL:
        {
            _delete_date_adjust_widget();
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

static bool _create_time_adjust_widget(void)
{
    adjust_time_init_t adjust_data = {0};
    menuitem_t * menuitem;

    menuitem = menu_get_item(s_menu, s_style_items[MENU_ADJUST_TIME].index);
    if(menuitem != NULL)
    {
        adjust_data.str_title_id = menuitem->text_id;
    }

    adjust_data.sys_time = s_cur_time;
    g_scene_list->adjust = adjust_create( &adjust_data, ADJ_STYLE_TIME, _time_adjust_widget_cb);
    if(NULL == g_scene_list->adjust)
    {
        print_err("create adjust failed!");
        return false;
    }

    adjust_paint(g_scene_list->adjust);

    return true;
}

static bool _delete_time_adjust_widget(void)
{
    if(g_scene_list->adjust != NULL)
    {
        adjust_delete(g_scene_list->adjust);
        g_scene_list->adjust = NULL;
    }

    return true;

}

static void _time_adjust_widget_cb(adjust_t *adjust, adjust_signal_e signal)
{
    rtc_time_t time;

    switch(signal)
    {
        case WIDGET_SIG_ADJUST_OK:
        {
            adjust_get_setting_time(&time);
            _delete_time_adjust_widget();

            time.second = 0;
            _set_cur_time(&time);
            menuview_paint( s_menuview );
            break;
        }

        case WIDGET_SIG_ADJUST_CANCEL:
        {
            _delete_time_adjust_widget();
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

static int _update_date_timer_cb(void *param)
{

    sys_get_rtc_date_time(&s_cur_time, &s_cur_date);

    //print_dbg("s_cur_date:%d-%d-%d",s_cur_date.year, s_cur_date.month, s_cur_date.day);
    //print_dbg("s_cur_time:%d:%d:%d",s_cur_time.hour, s_cur_time.minute, s_cur_time.second);
    _format_date_str(&s_cur_date, s_cur_date_str);
    _format_time_str(&s_cur_time, s_cur_time_str);

    menuview_paint(s_menuview );
    return 0;
}

static int _update_drm_timer_cb(void *param)
{

    sys_get_rtc_drm_date_time(&s_cur_drm_time, &s_cur_drm_date);

    //print_dbg("s_cur_date:%d-%d-%d",s_cur_drm_date.year, s_cur_drm_date.month, s_cur_drm_date.day);
    //print_dbg("s_cur_time:%d:%d:%d",s_cur_drm_time.hour, s_cur_drm_time.minute, s_cur_drm_time.second);
    _format_date_str(&s_cur_drm_date, s_cur_drm_date_str);
    _format_time_str(&s_cur_drm_time, s_cur_drm_time_str);
    sprintf(s_cur_drm_date_time_str, "%s/%s", s_cur_drm_date_str, s_cur_drm_time_str);

    menuview_paint(s_menuview );
    return 0;
}

static bool _set_cur_date(rtc_date_t * date)
{
    rtc_time_t cur_drm_time;
    rtc_date_t cur_drm_date;

    sys_get_rtc_drm_date_time(&cur_drm_time, &cur_drm_date);
    sys_set_rtc_date_time(NULL, date);
    sys_set_rtc_drm_date_time(&cur_drm_time, &cur_drm_date);

    s_cur_date = *date;
    _format_date_str(&s_cur_date, s_cur_date_str);

    return true;
}

static bool _set_cur_time(rtc_time_t * time)
{
    rtc_time_t cur_drm_time;
    rtc_date_t cur_drm_date;

    sys_get_rtc_drm_date_time(&cur_drm_time, &cur_drm_date);
    sys_set_rtc_date_time(time, NULL);
    sys_set_rtc_drm_date_time(&cur_drm_time, &cur_drm_date);

    s_cur_time = *time;
    _format_time_str(&s_cur_time, s_cur_time_str);

    return true;
}

static inline void _format_date_str( rtc_date_t * date, char * date_str)
{
    sprintf(date_str, "%4d-%02d-%02d", date->year, date->month, date->day);
}

static inline void _format_time_str( rtc_time_t * time, char * time_str)
{
    if(s_time_format == TIME_FORMAT_12)
    {
        if(time->hour > 12)
        {
            sprintf(time_str, "%02d:%02d PM", time->hour - 12, time->minute);
        }
        else if(time->hour == 0)
        {
            sprintf(time_str, "%02d:%02d AM", time->hour + 12, time->minute);
        }
        else if(time->hour == 12)
        {
            sprintf(time_str, "%02d:%02d PM", time->hour, time->minute);
        }
        else
        {
            sprintf(time_str, "%02d:%02d AM", time->hour, time->minute);
        }
    }
    else
    {
        sprintf(time_str, "%02d:%02d", time->hour, time->minute);
    }
}

