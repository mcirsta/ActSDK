/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 * file : setting_list_menu_language.c
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

/* menu  item callbacks */
static void _item_cb( menuview_t *menuview, menuview_sig_e signal );

/*********************************************************************************
 * internal static struct and data definition
 *********************************************************************************/
/* menu list data define */
static menu_t *s_menu = NULL;
static int s_cur_select_item = 0;
static support_language_list_t * s_support_lang_list = NULL;
static int s_select_item_pic_id = 0;
static char s_cur_select_text[128] = {0};
static int s_menu_id = -1;

/*********************************************************************************
 * interface implementaiton
 *********************************************************************************/
menu_t* setting_list_menu_language_enter( menuview_t *menuview, fusion_enter_e fusion_enter_type)
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

const char * setting_list_menu_language_get_cur_selected_text(void)
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
    picture_resource_t * pic_select_res;

    support_language_list_t * lang_node;
    static menuitem_t* item;
    int cnt;

    //initial menu
    if( s_menu == NULL )
    {
        scene = sty_get_app_scene();
        if ( NULL == scene )
        {
            print_err("get app scene failed!");
            return false;
        }

        //load language menu resource group
        res_group = (resgroup_resource_t *)get_scene_child(scene, REG_LANG);
        if(NULL == res_group)
        {
            print_err("get resource group REG_LANG");
            return false;
        }

        pic_select_res = ( picture_resource_t * )get_resgroup_child(res_group, PIC_SELECT);
        if(pic_select_res != NULL)
        {
            s_select_item_pic_id = pic_select_res->pic_id;
        }

        //create menu
        s_menu = menu_create();
        if(NULL == s_menu)
        {
            print_err("create menu failed!");
            return false;
        }

        s_support_lang_list = sys_get_cur_support_language_list();

        //init menu items
        for( lang_node = s_support_lang_list, cnt = 0; lang_node != NULL; lang_node = lang_node->next, cnt++ )
        {
            item = menuitem_create();
            if(item == NULL)
            {
                print_warning("create menu item failed!");
                continue;
            }

            item->callback = (item_cb_t)_item_cb;
            item->text = lang_node->name;
            item->info1 = lang_node->codes;

            if(strcmp(g_setting_cfgs[SET_CFG_TYPE_LANG_CODES].text, lang_node->codes) == 0)
            {
                item->unsel_icon_id = s_select_item_pic_id;
                item->sel_icon_id = s_select_item_pic_id;
                s_cur_select_item = cnt;

                if(lang_node->name != NULL)
                {
                    strncpy(s_cur_select_text, lang_node->name, sizeof(s_cur_select_text));
                }
            }

            menu_append_item( s_menu, item);

            menuitem_delete(item);
        }

        menu_set_cur_index( s_menu, s_cur_select_item );
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

    sys_delete_cur_support_language_list(s_support_lang_list);
    s_support_lang_list = NULL;

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
    setting_list_menu_main_enter(menuview, FUSION_BACKWARD);
}

#define CFG_KEY_LANG_CODES   "LANG_CODES"

static void _item_cb( menuview_t *menuview, menuview_sig_e signal )
{
    menu_t *cur_menu;
    menuitem_t *cur_item;
    menuitem_t *last_item;
    msg_apps_t msg = {0};

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

            strncpy(g_setting_cfgs[SET_CFG_TYPE_LANG_CODES].text, cur_item->info1, CFG_TEXT_LEN);
            g_setting_cfgs[SET_CFG_TYPE_LANG_CODES].changed = true;
            setting_cfg_write(&g_setting_cfgs[SET_CFG_TYPE_LANG_CODES]);

            // emit language changed widget signal
            setting_list_widget_menu_emit_signal(menuview, WIDGET_SIG_MENU_LANGUAGE_CHANGED);

            // brodcast language changed msg
            msg.type = MSG_LANGUAGE_CHANGED;
            broadcast_msg(&msg);

            last_item = menu_get_item(cur_menu, s_cur_select_item);
            if(last_item != NULL)
            {
                last_item->unsel_icon_id = 0;
                last_item->sel_icon_id = 0;
            }

            s_cur_select_item = cur_menu->current;
            cur_item->unsel_icon_id = s_select_item_pic_id;
            cur_item->sel_icon_id = s_select_item_pic_id;

            strncpy(s_cur_select_text, cur_item->text, sizeof(s_cur_select_text));

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


