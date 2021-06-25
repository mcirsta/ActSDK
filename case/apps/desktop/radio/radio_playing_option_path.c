/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : radio_option_path.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      cheason    2009-04-23          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     radio_option_path.c
 * \brief    FM播放界面场景下的option菜单保存路径相关操作
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/04/23
 *******************************************************************************/

#include "radio_scene_playing.h"
#include "radio_playing_option.h"
#include "radio_playing_option_path.h"

static res_option_path_t s_res_option_path =
{ NULL };
static menu_t *s_menu_option_path = NULL;
static bool s_cardin =false;

static void _option_path_cb(menuview_t *menuview, menuview_sig_e signal);
static void _option_path_locol_cb(menuview_t *menuview, menuview_sig_e signal);
static void _option_path_nolocol_cb(menuview_t *menuview, menuview_sig_e signal);
static void _add_item(menu_t *menu_data, menuitem_t *item, string_resource_t *str, bool sub_menu_flag);
static bool _fill_items(menu_t *menu_data, radio_scene_playing_t *playing);
static bool _fill_items_local(menu_t *menu_data, radio_scene_playing_t *playing);
static bool _fill_items_nolocal(menu_t *menu_data, radio_scene_playing_t *playing);

/******************************************************************************/
/*!                    
 * \par  Description:     option path callback
*******************************************************************************/
static void _option_path_cb(menuview_t *menuview, menuview_sig_e signal)
{
    if(HOTPLUGIN_DEV_TYPE_LOCAL == app_data.main_disk)
    {
        _option_path_locol_cb(menuview,signal);
    }
    else
    {
        _option_path_nolocol_cb(menuview,signal);
    }
    return;
}

/******************************************************************************/
/*!                    
 * \par  Description:     nor flash option path callback
*******************************************************************************/
static void _option_path_nolocol_cb(menuview_t *menuview, menuview_sig_e signal)
{
    menu_t *cur_menu = menuview->menu;
    menuitem_t *item;
    int lo_index;
    int old_path;
    if(NULL == cur_menu)
    {
        return;
    }

    lo_index = cur_menu->current;
    old_path = g_scene_playing->rec_path;

    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
        print_dbg("%d   now %d\n",g_scene_playing->rec_path,lo_index);

        /*无卡,选中u盘*/
        if((s_cardin == false) && (lo_index == 0))
        {
            g_scene_playing->rec_path = 2;
           
        }
        
        /*有卡，并且选中卡*/
        if((s_cardin == true) && (lo_index == 0))
        {
            g_scene_playing->rec_path = 1;
            if((old_path == 2)&&(cur_menu->total == 2))
            {
                item = menu_get_item(cur_menu, 1);/*菜单第二项,u盘*/
                if (NULL == item)
                {
                    print_dbg("menu_get_item return NULL 1\n");
                    break;
                }
                print_dbg();

                item->sel_icon_id = 0;
                item->unsel_icon_id = 0;
            }
            
        }
        else if((s_cardin == true) && (lo_index == 1))/*有卡,有u盘,选中u盘*/
        {
            g_scene_playing->rec_path = 2;
            if(((old_path) == 1)&&(cur_menu->total == 2))
            {
                item = menu_get_item(cur_menu, old_path-1);/*菜单第1项,卡*/
                if (NULL == item)
                {
                    print_dbg("menu_get_item return NULL 1\n");
                    break;
                }

                item->sel_icon_id = 0;
                item->unsel_icon_id = 0;               
            }
            
        }
        else
        {
            break;
        }

        item = menu_get_item(cur_menu, lo_index);
        if (NULL == item)
        {
            print_dbg("menu_get_item return NULL\n");
            break;
        }
        item->sel_icon_id = g_scene_playing->pic_select->pic_id;
        item->unsel_icon_id = g_scene_playing->pic_select->pic_id;

        menuview_paint(menuview);
      
       
        break;

        case WIDGET_SIG_MENUVIEW_QUIT:
        radio_playing_option_path_exit(menuview);
        break;

        case WIDGET_SIG_MENUVIEW_DOWN:
        break;

        case WIDGET_SIG_MENUVIEW_UP:
        break;

        case WIDGET_SIG_MENUVIEW_EXCEED_FIRST:
        {
            int last = menu_get_global_size(menuview->menu) - 1;
            if (last >= 0)
            {
                menu_set_cur_index(menuview->menu, last);
            }
        }
        break;

        case WIDGET_SIG_MENUVIEW_EXCEED_LAST:
        {
            int num = menu_get_global_size(menuview->menu);
            if (num > 0)
            {
                menu_set_cur_index(menuview->menu, 0);
            }
        }
        break;

        default:
        break;
    }
}

/******************************************************************************/
/*!                    
 * \par  Description:     nand flash path callback
*******************************************************************************/
static void _option_path_locol_cb(menuview_t *menuview, menuview_sig_e signal)
{
    menu_t *cur_menu = menuview->menu;
    menuitem_t *item;
    int lo_index = cur_menu->current;

    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
        print_dbg("%d   now %d\n",g_scene_playing->rec_path,lo_index);
        /*没有卡,选择菜单第二项,即u盘*/
        if((s_cardin == false) && (lo_index == 1) )
        {

            /*如果原来保存路径为主盘,将主盘上的选择图标去掉*/
            if((g_scene_playing->rec_path) == 0)
            {
                item = menu_get_item(cur_menu, g_scene_playing->rec_path);
                if (NULL == item)
                {
                    print_dbg("menu_get_item return NULL 1\n");
                    break;
                }

                item->sel_icon_id = 0;
                item->unsel_icon_id = 0;
               
            }
            /*显示第二项上的选择图标*/
            item = menu_get_item(cur_menu, 1);
            if (NULL == item)
            {
                print_dbg("menu_get_item return NULL 2\n");
                break;
            }
            item->sel_icon_id = g_scene_playing->pic_select->pic_id;
            item->unsel_icon_id = g_scene_playing->pic_select->pic_id;

            g_scene_playing->rec_path = 2;
            menuview_paint(menuview);
            break;
        }
        /*没有卡,选择菜单第一项,即主盘*/
        if((s_cardin == false) && (lo_index == 0) )
        {

            
            if((g_scene_playing->rec_path) != 0)
            {
                item = menu_get_item(cur_menu, 1);
                if (NULL == item)
                {
                    print_dbg("menu_get_item return NULL 1\n");
                    break;
                }

                item->sel_icon_id = 0;
                item->unsel_icon_id = 0;
               
            }

            item = menu_get_item(cur_menu, 0);
            if (NULL == item)
            {
                print_dbg("menu_get_item return NULL 2\n");
                break;
            }
            item->sel_icon_id = g_scene_playing->pic_select->pic_id;
            item->unsel_icon_id = g_scene_playing->pic_select->pic_id;

            g_scene_playing->rec_path = 0;
            menuview_paint(menuview);
            break;
        }
        
        if (g_scene_playing->rec_path != lo_index)
        {
            item = menu_get_item(cur_menu, g_scene_playing->rec_path);
            if (NULL == item)
            {
                print_dbg("menu_get_item return NULL 3\n");
                break;
            }

            item->sel_icon_id = 0;
            item->unsel_icon_id = 0;

            g_scene_playing->rec_path = lo_index;

            item = menu_get_item(cur_menu, lo_index);
            if (NULL == item)
            {
                print_dbg("menu_get_item return NULL\n");
                break;
            }
            item->sel_icon_id = g_scene_playing->pic_select->pic_id;
            item->unsel_icon_id = g_scene_playing->pic_select->pic_id;

            menuview_paint(menuview);
        }
        break;

        case WIDGET_SIG_MENUVIEW_QUIT:
        radio_playing_option_path_exit(menuview);
        break;

        case WIDGET_SIG_MENUVIEW_DOWN:
        break;

        case WIDGET_SIG_MENUVIEW_UP:
        break;

        case WIDGET_SIG_MENUVIEW_EXCEED_FIRST:
        {
            int last = menu_get_global_size(menuview->menu) - 1;
            if (last >= 0)
            {
                menu_set_cur_index(menuview->menu, last);
            }
        }
        break;

        case WIDGET_SIG_MENUVIEW_EXCEED_LAST:
        {
            int num = menu_get_global_size(menuview->menu);
            if (num > 0)
            {
                menu_set_cur_index(menuview->menu, 0);
            }
        }
        break;

        default:
        break;
    }
}

/******************************************************************************/
/*!                    
 * \par  Description:     add one item
*******************************************************************************/
static void _add_item(menu_t *menu_data, menuitem_t *item, string_resource_t *str, bool sub_menu_flag)
{
    item->callback = (item_cb_t) _option_path_cb;
    item->text_id = str->str_id;
    item->has_child = sub_menu_flag;
    menu_append_item(menu_data, item);
}


/******************************************************************************/
/*!                    
 * \par  Description:     fill all items
*******************************************************************************/
static bool _fill_items(menu_t *menu_data, radio_scene_playing_t *playing)
{
    
    if(HOTPLUGIN_DEV_TYPE_LOCAL == app_data.main_disk)
    {
        return _fill_items_local(menu_data, playing);
    }
    else
    {
        return _fill_items_nolocal(menu_data, playing);
    }
}

/******************************************************************************/
/*!                    
 * \par  Description:     fill all items for nand flash 
*******************************************************************************/
static bool _fill_items_local(menu_t *menu_data, radio_scene_playing_t *playing)
{
    menuitem_t *item;
    hotplugin_dev_info_t card_info;
    hotplugin_dev_info_t external_info;
    

    /* 填充的顺序必须跟 option_path_item_e 定义的一样 */
    item = menuitem_create();
    if (item == NULL)
    {
        return false;
    }


    sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_CARD, &card_info);
    sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_UHOST, &external_info);
    _add_item(menu_data, item, s_res_option_path.str_internal, false);
    if (card_info.access == HOTPLUGIN_DEV_ACCESS_OK)
    {
        s_cardin = true;
        _add_item(menu_data, item, s_res_option_path.str_card, false);
    }
    else
    {
        s_cardin = false;
    }
    if (external_info.access == HOTPLUGIN_DEV_ACCESS_OK)
    {
        _add_item(menu_data, item, s_res_option_path.str_external, false);
    }

    menuitem_delete(item);

    print_dbg("red %d ,s_cardin %d",playing->rec_path,(s_cardin ==true)?1:0);

    if (playing->rec_path == 0)
    {
        item = menu_get_item(menu_data, 0);
        if ((NULL != item) && (NULL != playing->pic_select))
        {
            item->sel_icon_id = playing->pic_select->pic_id;
            item->unsel_icon_id = playing->pic_select->pic_id;
        }
    }
    else if( (playing->rec_path == 1) )
    {
        if (card_info.access == HOTPLUGIN_DEV_ACCESS_OK)
        {
            item = menu_get_item(menu_data, 1);
            if ((NULL != item) && (NULL != playing->pic_select))
            {
                item->sel_icon_id = playing->pic_select->pic_id;
                item->unsel_icon_id = playing->pic_select->pic_id;
            }
        }
        else
        {
            item = menu_get_item(menu_data, 0);
            if ((NULL != item) && (NULL != playing->pic_select))
            {
                item->sel_icon_id = playing->pic_select->pic_id;
                item->unsel_icon_id = playing->pic_select->pic_id;
            }

            playing->rec_path = 0;
        }
    }
    else if((playing->rec_path == 2) && (s_cardin == true))
    {
        if (external_info.access == HOTPLUGIN_DEV_ACCESS_OK)
        {
            item = menu_get_item(menu_data, 2);
            if ((NULL != item) && (NULL != playing->pic_select))
            {
                item->sel_icon_id = playing->pic_select->pic_id;
                item->unsel_icon_id = playing->pic_select->pic_id;
            }
        }
        else
        {
            item = menu_get_item(menu_data, 0);
            if ((NULL != item) && (NULL != playing->pic_select))
            {
                item->sel_icon_id = playing->pic_select->pic_id;
                item->unsel_icon_id = playing->pic_select->pic_id;
            }
            playing->rec_path = 0;
        }
    }
    else/* if(playing->rec_path == 2) && (s_cardin == false))*/
    {
        if (external_info.access == HOTPLUGIN_DEV_ACCESS_OK)
        {
            item = menu_get_item(menu_data, 1);
            if ((NULL != item) && (NULL != playing->pic_select))
            {
                item->sel_icon_id = playing->pic_select->pic_id;
                item->unsel_icon_id = playing->pic_select->pic_id;
            }
        }
        else
        {
            item = menu_get_item(menu_data, 0);
            if ((NULL != item) && (NULL != playing->pic_select))
            {
                item->sel_icon_id = playing->pic_select->pic_id;
                item->unsel_icon_id = playing->pic_select->pic_id;
            }
            playing->rec_path = 0;
        }        
    }
    return true;
}
/******************************************************************************/
/*!                    
 * \par  Description:     fill all items for nor flash
*******************************************************************************/
static bool _fill_items_nolocal(menu_t *menu_data, radio_scene_playing_t *playing)
{
    menuitem_t *item;
    hotplugin_dev_info_t card_info;
    hotplugin_dev_info_t external_info;
    dialog_init_t dialog_init;

    /* 填充的顺序必须跟 option_path_item_e 定义的一样 */
    item = menuitem_create();
    if (item == NULL)
    {
        return false;
    }


    sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_CARD, &card_info);
    sys_get_hotplugin_dev_info(HOTPLUGIN_DEV_TYPE_UHOST, &external_info);
    if (card_info.access == HOTPLUGIN_DEV_ACCESS_OK)
    {
        s_cardin = true;
        _add_item(menu_data, item, s_res_option_path.str_card, false);
    }
    else
    {
        s_cardin = false;
    }
    if (external_info.access == HOTPLUGIN_DEV_ACCESS_OK)
    {
        _add_item(menu_data, item, s_res_option_path.str_external, false);
    }

    menuitem_delete(item);

    print_dbg("red %d ,s_cardin %d",playing->rec_path,(s_cardin ==true)?1:0);

    if(playing->rec_path == 0)
    {
        playing->rec_path = 1;
    }
    
    if ((s_cardin == false)&&(external_info.access != HOTPLUGIN_DEV_ACCESS_OK))
    {
        /*提示无磁盘*/
        if( NULL != playing->str_nodisk)
        {
            /* initial dialog data*/
            dialog_init.head = DIALOG_HEAD_WARN;
            dialog_init.str_id = playing->str_nodisk->str_id;

            dialog_modal(&dialog_init);
        }
    }
    else if((s_cardin == true)&&(external_info.access == HOTPLUGIN_DEV_ACCESS_OK))
    {
        if(playing->rec_path == 1)
        {
            item = menu_get_item(menu_data, 0);
            if ((NULL != item) && (NULL != playing->pic_select))
            {
                item->sel_icon_id = playing->pic_select->pic_id;
                item->unsel_icon_id = playing->pic_select->pic_id;
            }
        }
        else
        {            
            item = menu_get_item(menu_data, 1);
            if ((NULL != item) && (NULL != playing->pic_select))
            {
                item->sel_icon_id = playing->pic_select->pic_id;
                item->unsel_icon_id = playing->pic_select->pic_id;
            }
        }
    }
    else if(s_cardin == true)
    {
        item = menu_get_item(menu_data, 0);
        if ((NULL != item) && (NULL != playing->pic_select))
        {
            item->sel_icon_id = playing->pic_select->pic_id;
            item->unsel_icon_id = playing->pic_select->pic_id;
        }
        playing->rec_path = 1;
    }
    else
    {
        item = menu_get_item(menu_data, 0);
        if ((NULL != item) && (NULL != playing->pic_select))
        {
            item->sel_icon_id = playing->pic_select->pic_id;
            item->unsel_icon_id = playing->pic_select->pic_id;
        }
        playing->rec_path = 2;
    }
    return true;
}

/******************************************************************************/
/*!                    
 * \par  Description:     load style
*******************************************************************************/

void radio_option_path_load_style(resgroup_resource_t *resgroup)
{
    if (NULL == resgroup)
    {
        return;
    }
    /* resgroup option */
    s_res_option_path.resgroup = resgroup;
    s_res_option_path.str_internal = (string_resource_t *) get_resgroup_child(resgroup, STR_INTERNAL);
    s_res_option_path.str_card = (string_resource_t *) get_resgroup_child(resgroup, STR_CARD);
    s_res_option_path.str_external = (string_resource_t *) get_resgroup_child(resgroup, STR_EXTERNAL);
}

/******************************************************************************/
/*!                    
 * \par  Description:     path init
*******************************************************************************/
menu_t* radio_playing_option_path_init(void)
{
    if ((NULL == g_scene_playing) || (NULL == s_res_option_path.resgroup))
    {
        return NULL;
    }

    if (NULL == s_menu_option_path)
    {
        s_menu_option_path = menu_create();
        
        if(NULL == s_menu_option_path)
        {
            return false;
        }
        if(false == _fill_items(s_menu_option_path, g_scene_playing))
        {
            return false;
        }
        menu_set_cur_index(s_menu_option_path, 0);
    }

    s_menu_option_path->callback = (menu_cb_t)_option_path_cb;
    return s_menu_option_path;
}

/******************************************************************************/
/*!                    
 * \par  Description:     path deinit
*******************************************************************************/
int radio_playing_option_path_deinit(void)
{
    if (NULL != s_menu_option_path)
    {
        menu_delete(s_menu_option_path);
        s_menu_option_path = NULL;
    }

    return 0;
}

/******************************************************************************/
/*!                    
 * \par  Description:     path enter
*******************************************************************************/
bool radio_playing_option_path_enter(menuview_t *menuview)
{
    menu_t *cur_menu;

    cur_menu = radio_playing_option_path_init();
    if(false == cur_menu)
    {
        return false;
    }
    menuview_set_menu(menuview, cur_menu);
    menuview_paint(menuview);

    return true;
}

/******************************************************************************/
/*!                    
 * \par  Description:     path exit
*******************************************************************************/
bool radio_playing_option_path_exit(menuview_t *menuview)
{
    fusion_type_e fenter;
    fenter = FUSION_BACKWARD;

    radio_playing_option_path_deinit();

    radio_playing_option_enter(menuview, fenter);

    return true;
}

/******************************************************************************/
/*!                    
 * \par  Description:   update path menu item
*******************************************************************************/
void update_savepath(void)
{
    if(NULL != s_menu_option_path)
    {
        radio_playing_option_path_deinit();
        radio_playing_option_path_enter(g_scene_playing->option_menu);
    }
}

