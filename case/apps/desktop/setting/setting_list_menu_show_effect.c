/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 * file : setting_list_menu_show_effect.c
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
static void _item_cb( menuview_t *menuview, menuview_sig_e signal );

/*********************************************************************************
 * internal static struct and data definition
 *********************************************************************************/
enum menuitem_name
{
    MENU_EFFECT_FLY_IN = 0,
    MENU_EFFECT_CUBE,
    MENU_EFFECT_ROTATION,
    MENU_EFFECT_OFF,
};

/* menu items style data define */
static menuitem_style_t s_style_items[] =
{
    { 0, 0, STR_FLY_IN,   false, (item_cb_t)_item_cb, NULL, true, 0},
    { 0, 0, STR_CUBE,     false, (item_cb_t)_item_cb, NULL, true, 0},
    { 0, 0, STR_ROTATION, false, (item_cb_t)_item_cb, NULL, true, 0},
    { 0, 0, STR_OFF, false, (item_cb_t)_item_cb, NULL, true, 0},
};

static menu_t * s_menu = NULL;
static enum menuitem_name s_cur_select_item = MENU_EFFECT_OFF;
static int s_select_item_pic_id = 0;
static char s_cur_select_text[128] = {0};
static int s_menu_id = -1;


/*********************************************************************************
 * interface implementaiton
 *********************************************************************************/
menu_t* setting_list_menu_show_effect_enter( menuview_t *menuview, fusion_enter_e fusion_enter_type)
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

const char * setting_list_menu_show_effect_get_cur_selected_text(void)
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
    fusion_effect_t fusion_effect = {0};

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

        //load key tone select menu resource group
        res_group = (resgroup_resource_t *)get_scene_child(scene, REG_SHOW_EFFECT);
        if(NULL == res_group)
        {
            print_err("get resource group REG_SHOW_EFFECT");
            return false;
        }

        real_items = menu_load_item_resource(res_group, s_style_items, menu_items, ARRAY_SIZE(s_style_items));

        fusion_get_config(FUSION_TYPE_COMMON, FUSION_FORWARD, &fusion_effect);

        if(fusion_effect.mode == FUSION_PUSH)
        {
            s_cur_select_item = MENU_EFFECT_FLY_IN;
        }
        else if(fusion_effect.mode == FUSION_CUBE)
        {
            s_cur_select_item = MENU_EFFECT_CUBE;
        }
        else if(fusion_effect.mode == FUSION_ROTATE)
        {
            s_cur_select_item = MENU_EFFECT_ROTATION;
        }
        else
        {
            s_cur_select_item = MENU_EFFECT_OFF;
        }

        pic_res = ( picture_resource_t * )get_resgroup_child(res_group, PIC_SELECT);
        if(pic_res != NULL)
        {
            s_select_item_pic_id = pic_res->pic_id;
            menu_items[s_cur_select_item].unsel_icon_id = s_select_item_pic_id;
            menu_items[s_cur_select_item].sel_icon_id = s_select_item_pic_id;
        }

        text = sty_get_string(menu_items[s_cur_select_item].text_id);
        if(text != NULL)
        {
            strncpy(s_cur_select_text, text, sizeof(s_cur_select_text));
        }

        s_menu = menu_create();
        if(NULL == s_menu)
        {
            print_err("create menu failed!");
            return false;
        }
        //init menu items
        menu_append_items( s_menu, menu_items, real_items);

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

static void _item_cb( menuview_t *menuview, menuview_sig_e signal )
{
    menu_t *cur_menu;
    menuitem_t *menu_item;
    fusion_effect_t fusion_effect = {0};
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

            if(cur_menu->current == MENU_EFFECT_FLY_IN)
            {
                fusion_effect.mode = FUSION_PUSH;
            }
            else if(cur_menu->current == MENU_EFFECT_CUBE)
            {
                fusion_effect.mode = FUSION_CUBE;
            }
            else if(cur_menu->current == MENU_EFFECT_ROTATION)
            {
                fusion_effect.mode = FUSION_ROTATE;
            }
            else
            {
                fusion_effect.mode = FUSION_NORMAL;
            }

            fusion_effect.direct = TRANS_RIGHT; 
            fusion_set_config(FUSION_TYPE_COMMON, FUSION_FORWARD, &fusion_effect);
            fusion_set_config(FUSION_TYPE_APP_CHANGE, FUSION_FORWARD, &fusion_effect);
            fusion_effect.direct = TRANS_LEFT; 
            fusion_set_config(FUSION_TYPE_COMMON, FUSION_BACKWARD, &fusion_effect);
            fusion_set_config(FUSION_TYPE_APP_CHANGE, FUSION_BACKWARD, &fusion_effect);

            menu_item = menu_get_item(cur_menu, s_cur_select_item);
            if(menu_item != NULL)
            {
                menu_item->unsel_icon_id = 0;
                menu_item->sel_icon_id = 0;
            }

            s_cur_select_item = cur_menu->current;
            menu_item = menu_get_item(cur_menu, s_cur_select_item);
            if(menu_item != NULL)
            {
                menu_item->unsel_icon_id = s_select_item_pic_id;
                menu_item->sel_icon_id = s_select_item_pic_id;

                text = sty_get_string(menu_item->text_id);
                if(text != NULL)
                {
                    strncpy(s_cur_select_text, text, sizeof(s_cur_select_text));
                }
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
