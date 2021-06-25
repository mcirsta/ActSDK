/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_list_mem.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason    2009-06-14          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_list_mem.c
 * \brief    photo list memory selection
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/06/14
 *******************************************************************************/

#include "photo_scene_list.h"
#include "photo_list_mem.h"
#include "photo_dec_ctrl.h"
#include "photo_list_mainmenu.h"
#include "photo_list_widget_menu.h"
#include "photo_playlist.h"

static void _proc_item_int_mem(menuview_t *menuview, menuview_sig_e signal);
static void _proc_item_car_mem(menuview_t *menuview, menuview_sig_e signal);
static void _proc_item_ext_mem(menuview_t *menuview, menuview_sig_e signal);
static bool _mem_init(void);
static bool _mem_deinit(void);

static menuview_t *s_menuview = NULL;
static menu_t *s_memlist_menu = NULL;
static bool s_int_exist = false;
static bool s_card_exist = false;
static bool s_ext_exist = false;

static void _proc_item_int_mem(menuview_t *menuview, menuview_sig_e signal)
{
    int type;
    switch (signal)
    {
    case WIDGET_SIG_MENUVIEW_OK:
        type = PATH_INTERNAL;
        set_config(CFG_PHOTO_PATH_TYPE, (const char *) &type, sizeof(type));
        g_scene_list->exit_type = SCENE_LIST_DIRLIST_NEXT;
        photo_list_widget_menu_exit(menuview, photo_list_mem_enter);
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        photo_list_mem_exit(menuview);
        break;

    default:
        break;
    }
}

static void _proc_item_car_mem(menuview_t *menuview, menuview_sig_e signal)
{
    int type;
    switch (signal)
    {
    case WIDGET_SIG_MENUVIEW_OK:
        type = PATH_CARD;
        set_config(CFG_PHOTO_PATH_TYPE, (const char *) &type, sizeof(type));
        g_scene_list->exit_type = SCENE_LIST_DIRLIST_NEXT;
        photo_list_widget_menu_exit(menuview, photo_list_mem_enter);
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        photo_list_mem_exit(menuview);
        break;

    default:
        break;
    }
}

static void _proc_item_ext_mem(menuview_t *menuview, menuview_sig_e signal)
{
    int type;
    switch (signal)
    {
    case WIDGET_SIG_MENUVIEW_OK:
        type = PATH_EXTERNAL;
        set_config(CFG_PHOTO_PATH_TYPE, (const char *) &type, sizeof(type));
        g_scene_list->exit_type = SCENE_LIST_DIRLIST_NEXT;
        photo_list_widget_menu_exit(menuview, photo_list_mem_enter);
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        photo_list_mem_exit(menuview);
        break;

    default:
        break;
    }
}

static bool _check_mem_exist(bool *int_exist, bool *card_exist, bool *ext_exist)
{
    hotplugin_dev_info_t dev_info;

    assert((NULL != int_exist) && (NULL != card_exist) && (NULL != ext_exist));

    *int_exist = false;
    *card_exist = false;
    *ext_exist = false;

    sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_LOCAL, &dev_info);
    if(dev_info.access == HOTPLUGIN_DEV_ACCESS_OK)
    {
        *int_exist = true;
    }

    sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_CARD, &dev_info);
    if(dev_info.access == HOTPLUGIN_DEV_ACCESS_OK)
    {
        *card_exist = true;
    }

    sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_UHOST, &dev_info);
    if(dev_info.access == HOTPLUGIN_DEV_ACCESS_OK)
    {
        *ext_exist = true;
    }
    return true;
}

static bool _mem_init(void)
{
    menuitem_t item;
    int sel_id = 0, num1 = 0, num2 = 0;    
    
    if(NULL != s_memlist_menu)
    {
        sel_id = menu_get_cur_index(s_memlist_menu);
        num1 = menu_get_total(s_memlist_menu);
        photo_menu_delete(s_memlist_menu);
        s_memlist_menu = NULL;
    }

    _check_mem_exist(&s_int_exist, &s_card_exist, &s_ext_exist);

    if((false == s_int_exist) && (false == s_card_exist) && (false == s_ext_exist))
    {
        /* 无任何磁盘 */
        return false;
    }

    if((true == s_int_exist) && (false == s_card_exist) && (false == s_ext_exist))
    {
        int type = PATH_INTERNAL;
        set_config(CFG_PHOTO_PATH_TYPE, (const char *) &type, sizeof(type));
        g_scene_list->exit_type = SCENE_LIST_DIRLIST_NEXT;
        photo_list_widget_menu_exit(s_menuview, photo_list_mainmenu_enter);
        return false;
    }
    
    if((false == s_int_exist) && (true == s_card_exist) && (false == s_ext_exist))
    {
        int type = PATH_CARD;
        set_config(CFG_PHOTO_PATH_TYPE, (const char *) &type, sizeof(type));
        g_scene_list->exit_type = SCENE_LIST_DIRLIST_NEXT;
        photo_list_widget_menu_exit(s_menuview, photo_list_mainmenu_enter);
        return false;
    }
    
    if((false == s_int_exist) && (false == s_card_exist) && (true == s_ext_exist))
    {
        int type = PATH_EXTERNAL;
        set_config(CFG_PHOTO_PATH_TYPE, (const char *) &type, sizeof(type));
        g_scene_list->exit_type = SCENE_LIST_DIRLIST_NEXT;
        photo_list_widget_menu_exit(s_menuview, photo_list_mainmenu_enter);
        return false;
    }
    

    if (NULL == s_memlist_menu)
    {
        s_memlist_menu = photo_menu_create();
        memset(&item, 0, sizeof(menuitem_t));

        if(true == s_int_exist)
        {
            _add_item(s_memlist_menu, &item, g_scene_list->resg_dirlist.str_int_mem, true,
                (item_cb_t) _proc_item_int_mem);
        }
        
        if(true == s_ext_exist)
        {
            _add_item(s_memlist_menu, &item, g_scene_list->resg_dirlist.str_ext_mem, true,
                    (item_cb_t) _proc_item_ext_mem);
        }
        
        if(true == s_card_exist)
        {
            _add_item(s_memlist_menu, &item, g_scene_list->resg_dirlist.str_car_mem, true,
                    (item_cb_t) _proc_item_car_mem);
        }
    }
    
    num2 = menu_get_total(s_memlist_menu);
    if((num1 != 0) && (num2 != 0))
    {
        if((num1 != num2) || (sel_id >= num2))
        {
            sel_id = 0;
        }
    }
    else
    {
        sel_id = 0;
    }
    
    menu_set_cur_index(s_memlist_menu, sel_id);

    return true;
}

static bool _mem_deinit(void)
{
    if (NULL != s_memlist_menu)
    {
        photo_menu_delete(s_memlist_menu);
        s_memlist_menu = NULL;
    }

    s_int_exist = false;
    s_card_exist = false;
    s_ext_exist = false;

    return true;
}

bool photo_list_mem_refresh(menuview_t *mv)
{
    bool int_tmp, card_tmp, ext_tmp;

    if(NULL == s_memlist_menu)
    {
        return false;
    }

    _check_mem_exist(&int_tmp, &card_tmp, &ext_tmp);
    if((int_tmp != s_int_exist) || (card_tmp != s_card_exist)
        || (ext_tmp != s_ext_exist))
    {
        _mem_init();
        menuview_paint(s_menuview);
    }

    return true;
}

bool photo_list_mem_enter(menuview_t *mv, fusion_enter_e fenter)
{
    fusion_effect_t eff;
    
    /* 从dirlist返回不显示mem菜单（因为dirlist已经显示了） */
    if((FUSION_BACKWARD == fenter) && (g_scene_list->preview_mode == PREV_LIST))
    {
        _mem_deinit();
        return photo_list_mainmenu_enter(mv, fenter);
    }

    /* 从缩略图返回时检查是否有卡或者U盘改动，如果有则调整菜单  */
    if((FUSION_BACKWARD == fenter) && (NULL != s_memlist_menu))
    {
        bool int_tmp, card_tmp, ext_tmp;
        _check_mem_exist(&int_tmp, &card_tmp, &ext_tmp);
        if((int_tmp + card_tmp + ext_tmp) < 2)
        {
            _mem_deinit();
            return photo_list_mainmenu_enter(mv, fenter);
        }
    }

    s_menuview = mv;
    if(!_mem_init())
    {
        return false;
    }

    fusion_get_config(FUSION_TYPE_COMMON, fenter, &eff);
    fusion_set_effect(&eff);
    scene_fusion_set_first_frame(g_scene_list->scene);

    scene_list_paint(g_scene_list);

    if(s_menuview->type != MENUVIEW_MENULIST_TEXT)
    {
        menuview_load_style(s_menuview, MENUVIEW_MENULIST_TEXT);
    }
    menuview_set_menu(s_menuview, s_memlist_menu);
    menuview_paint(s_menuview);

    scene_fusion_set_last_frame(g_scene_list->scene);
    fusion_display(NULL);

    return true;
}

bool photo_list_mem_exit(menuview_t *mv)
{
    _mem_deinit();
    photo_list_mainmenu_enter(mv, FUSION_BACKWARD);

    return true;
}

