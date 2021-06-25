/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 * file : setting_list_menu_connect_type.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-5-8             v1.0             build this file
 ********************************************************************************/

#include "setting_list_widget_menu.h"


/*********************************************************************************
 * internal static functions declaration
 *********************************************************************************/
static bool _menu_init( void );
static bool _menu_deinit( void );
static void _menu_return( menuview_t *menuview );

/* menu  item callbacks */
static void _udisk_cb( menuview_t *menuview, menuview_sig_e signal );
static void _mtp_cb( menuview_t *menuview, menuview_sig_e signal );

/*********************************************************************************
 * internal static struct and data definition
 *********************************************************************************/
enum menuitem_name
{
    MENU_UDISK = 0,
    MENU_MTP,
};

/* menu items style data define */
static menuitem_style_t s_style_items[] =
{
    { 0, 0, STR_CONNECT_UDISK, false, (item_cb_t)_udisk_cb, NULL, true, 0},
    { 0, 0, STR_CONNECT_MTP, false, (item_cb_t)_mtp_cb, NULL, true, 0},
};

static menu_t * s_menu = NULL;
static enum menuitem_name s_cur_select_item = MENU_UDISK;
static int s_select_item_pic_id = 0;
static char s_cur_select_text[128] = {0};
static int s_menu_id = -1;

/*********************************************************************************
 * interface implementaiton
 *********************************************************************************/

menu_t* setting_list_menu_connect_type_enter( menuview_t *menuview, fusion_enter_e fusion_enter_type)
{
    sub_menu_t sub_menu = {0};

    if(_menu_init() == false)
    {
        print_err("init menu data failed!");
        return NULL;
    }

    menuview_set_menu( menuview, s_menu );
    menuview_load_style( menuview, MENUVIEW_MENULIST_ICON );

    if(s_menu_id == -1)
    {
        sub_menu.deinit_cb = _menu_deinit;
        sub_menu.update_minor_info_cb = NULL;
        s_menu_id = setting_list_widget_menu_register_sub_menu(menuview, &sub_menu);
    }

    setting_list_widget_menu_show_fusion(menuview, fusion_enter_type);

    return s_menu;
}

const char * setting_list_menu_connect_type_get_cur_selected_text(void)
{
    if( s_menu != NULL )
    {
        _menu_deinit();
    }

    _menu_init();
    _menu_deinit();
    return s_cur_select_text;
}

/*********************************************************************************
 * internal static  implementaiton
 *********************************************************************************/
static bool _menu_init( void )
{
    scene_t * scene;
    resgroup_resource_t * res_group;
    picture_resource_t * pic_res;
    const char * text = NULL;

#pragma __PRQA_IGNORE_START__
    /* 使用 0 初始化数组 */
    menuitem_t menu_items[ARRAY_SIZE(s_style_items)] = {{0}};
#pragma __PRQA_IGNORE_END__

    int real_items = 0;

    //initial menu
    if( s_menu == NULL )
    {
        scene = sty_get_app_scene();
        if ( NULL == scene  )
        {
            print_err("get app scene failed!");
            return false;
        }

        //load connect type menu resource group
        res_group = (resgroup_resource_t *)get_scene_child(scene, REG_CONNECT_TYPE);
        if(NULL == res_group)
        {
            print_err("get resource group REG_CONNECT_TYPE");
            return false;
        }

        real_items = menu_load_item_resource(res_group, s_style_items, menu_items, ARRAY_SIZE(s_style_items));

        if(strcmp(g_setting_cfgs[SET_CFG_TYPE_CONNECT].text, "MTP") == 0)
        {
            s_cur_select_item = MENU_MTP;
        }
        else
        {
            s_cur_select_item = MENU_UDISK;
        }


        pic_res = ( picture_resource_t * )get_resgroup_child(res_group, PIC_SELECT);

        if(pic_res != NULL)
        {
            s_select_item_pic_id = pic_res->pic_id;
        }

        menu_items[s_cur_select_item].unsel_icon_id = s_select_item_pic_id;
        menu_items[s_cur_select_item].sel_icon_id = s_select_item_pic_id;

        text = sty_get_string(menu_items[s_cur_select_item].text_id);
        if(text != NULL)
        {
            strncpy(s_cur_select_text, text, sizeof(s_cur_select_text));
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

        menu_set_cur_index( s_menu, s_cur_select_item);
    }
    return true;
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
    _menu_deinit();

    //return to parent menu
    setting_list_menu_advanced_enter(menuview, FUSION_BACKWARD);
}

static void _udisk_cb( menuview_t *menuview, menuview_sig_e signal )
{
    menu_t *cur_menu;
    menuitem_t *cur_item;
    menuitem_t *last_item;
    const char * text = NULL;

    switch ( signal )
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            cur_menu = menuview_get_menu(menuview);

            if(cur_menu->current == s_cur_select_item)
            {
                /* current item no changed */
                return;
            }

            cur_item = menu_get_cur_item(cur_menu);

            if(cur_item == NULL)
            {
                return;
            }

            strcpy(g_setting_cfgs[SET_CFG_TYPE_CONNECT].text, "UDISK");
            g_setting_cfgs[SET_CFG_TYPE_CONNECT].changed = true;
            setting_cfg_write(&g_setting_cfgs[SET_CFG_TYPE_CONNECT]);

            last_item = menu_get_item(cur_menu, s_cur_select_item);
            if(last_item != NULL)
            {
                last_item->unsel_icon_id = 0;
                last_item->sel_icon_id = 0;
            }

            s_cur_select_item = cur_menu->current;
            cur_item->unsel_icon_id = s_select_item_pic_id;
            cur_item->sel_icon_id = s_select_item_pic_id;

            text = sty_get_string(cur_item->text_id);
            if(text != NULL)
            {
                strncpy(s_cur_select_text, text, sizeof(s_cur_select_text));
            }

            menuview_paint(menuview);

            // return to parent menu
            //_menu_return( menuview );

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

static void _mtp_cb( menuview_t *menuview, menuview_sig_e signal )
{
    menu_t *cur_menu;
    menuitem_t *cur_item;
    menuitem_t *last_item;
    const char * text = NULL;

    switch ( signal )
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            cur_menu = menuview_get_menu(menuview);

            if(cur_menu->current == s_cur_select_item)
            {
                /* current item no changed */
                return;
            }

            cur_item = menu_get_cur_item(cur_menu);

            if(cur_item == NULL)
            {
                return;
            }

            strcpy(g_setting_cfgs[SET_CFG_TYPE_CONNECT].text, "MTP");
            g_setting_cfgs[SET_CFG_TYPE_CONNECT].changed = true;
            setting_cfg_write(&g_setting_cfgs[SET_CFG_TYPE_CONNECT]);

            last_item = menu_get_item(cur_menu, s_cur_select_item);
            if(last_item != NULL)
            {
                last_item->unsel_icon_id = 0;
                last_item->sel_icon_id = 0;
            }

            s_cur_select_item = cur_menu->current;
            cur_item->unsel_icon_id = s_select_item_pic_id;
            cur_item->sel_icon_id = s_select_item_pic_id;

            text = sty_get_string(cur_item->text_id);
            if(text != NULL)
            {
                strncpy(s_cur_select_text, text, sizeof(s_cur_select_text));
            }

            menuview_paint(menuview);

            // return to parent menu
            //_menu_return( menuview );
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
