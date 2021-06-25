/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 * file : setting_list_menu.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-5-5             v1.0             build this file
 ********************************************************************************/

#include "setting_list_widget_menu.h"

typedef struct menu_mode_s
{
    int id;
    sub_menu_t sub_menu;
    struct menu_node_s * next;
}menu_node_t;

static menu_node_t * s_menu_node_list = NULL;

static menu_entry_t g_menu_entry  = NULL;
static menuview_cb_t g_widget_menu_cg = NULL;

static void _menuview_callback(menuview_t *menuview, menuview_sig_e signal)
{

}

menuview_t* setting_list_widget_menu_create( menu_cb_t callback )
{
    menuview_t *menuview;

    //create a menu viewer
    menuview = menuview_create( MENUVIEW_FILELIST_ICON, _menuview_callback );
    if( menuview == NULL )
    {
        print_err("menuview create failed!");
        return NULL;
    }
    else
    {
        //restore last menu
        if ( g_menu_entry != NULL )
        {
            (g_menu_entry)( menuview );
        }

        //save callback
        g_widget_menu_cg = (menuview_cb_t)callback;
    }

    return menuview;
}

bool setting_list_widget_menu_delete( menuview_t *menuview )
{
    menu_node_t * node = s_menu_node_list;

    if( menuview == NULL )
    {
        print_err("menuview delete fail!");
        return false;
    }
    else
    {
        while ( s_menu_node_list != NULL )
        {
            node = s_menu_node_list;
            s_menu_node_list = (menu_node_t *)(s_menu_node_list->next);

            if(node->sub_menu.deinit_cb != NULL)
            {
                node->sub_menu.deinit_cb();
            }

            free(node);
        }

        menuview_delete( menuview );
    }

    return true;
}

bool setting_list_widget_menu_load_style( menuview_t *menuview )
{
    if ( menuview != NULL )
    {
        return menuview_load_style( menuview, menuview->type );
    }
    else
    {
        return false;
    }
}

bool setting_list_widget_menu_paint( menuview_t *menuview )
{
    return menuview_paint( menuview );
}

bool setting_list_widget_menu_hide( menuview_t *menuview )
{
    return menuview_hide( menuview );
}

bool setting_list_widget_menu_show( menuview_t *menuview )
{
    menuview_show( menuview );
    return menuview_paint(menuview);
}

bool setting_list_widget_menu_proc_gui( menuview_t *menuview, gui_wm_message_t *pmsg )
{
    return menuview_proc_gui( menuview, pmsg );
}

bool setting_list_widget_menu_exit( menuview_t *menuview,  menu_entry_t entry )
{
    //save last menu entry
    g_menu_entry = entry;
    return setting_list_widget_menu_emit_signal(menuview, WIDGET_SIG_MENU_QUIT);
}

inline bool setting_list_widget_menu_emit_signal(menuview_t *menuview, menu_sig_e sig)
{
    if ( g_widget_menu_cg != NULL )
    {
        (g_widget_menu_cg) (menuview, sig);
        return true;
    }
    else
    {
        return false;
    }
}

int setting_list_widget_menu_register_sub_menu( menuview_t *menuview , sub_menu_t* sub_menu)
{
    static int id = 0;
    if( (menuview != NULL) && (sub_menu != NULL) )
    {
        menu_node_t * m = (menu_node_t *) malloc ( sizeof(menu_node_t));

        if( m == NULL )
        {
            print_err("malloc error!");
            return -1;
        }

        id++;
        m->id = id;
        m->sub_menu.deinit_cb = sub_menu->deinit_cb;
        m->sub_menu.update_minor_info_cb = sub_menu->update_minor_info_cb;
        m->next = (struct menu_node_s *)s_menu_node_list;
        s_menu_node_list = m;

        return m->id;
    }

    return -1;

}

bool setting_list_widget_menu_unregister_sub_menu(int sub_menu_id)
{
    menu_node_t *mn = s_menu_node_list;
    menu_node_t *ltm = NULL;

    if(sub_menu_id == -1)
    {
        return false;
    }

    while ( mn != NULL )
    {
        if( mn->id == sub_menu_id)
        {
            if( ltm != NULL )
            {
                ltm->next = mn->next;
            }
            else
            {
                s_menu_node_list = (menu_node_t *)(mn->next);
            }

            free ( mn );
            return true;
        }
        ltm = mn;
        mn = (menu_node_t *)(mn->next);
    }

    return false;

}

void setting_list_widget_menu_show_fusion( menuview_t *menuview, fusion_enter_e fusion_enter_type )
{
    fusion_effect_t fusion_effect = {0};
    fusion_frame_t frame;

    if(menuview == NULL)
    {
        return;
    }

    fusion_get_config(FUSION_TYPE_COMMON, fusion_enter_type, &fusion_effect);
    fusion_set_effect(&fusion_effect);

    frame.rect.x = g_scene_list->scene->x;
    frame.rect.y = g_scene_list->scene->y;
    frame.rect.width = g_scene_list->scene->width;
    frame.rect.height = g_scene_list->scene->height;
    frame.pdata = NULL;
    frame.alpha = false;
    fusion_set_first_frame(&frame);

    menuview_paint( menuview );

    frame.rect.x = g_scene_list->scene->x;
    frame.rect.y = g_scene_list->scene->y;
    frame.rect.width = g_scene_list->scene->width;
    frame.rect.height = g_scene_list->scene->height;
    frame.pdata = NULL;
    frame.alpha = false;
    fusion_set_last_frame(&frame);

    fusion_display( NULL);
}

bool setting_list_widget_menu_update_minor_info( menuview_t *menuview )
{
    menu_node_t * node = s_menu_node_list;

    if( menuview == NULL )
    {
        print_err("menuview delete fail!");
        return false;
    }
    else
    {
        while ( s_menu_node_list != NULL )
        {
            node = s_menu_node_list;
            s_menu_node_list = (menu_node_t *)(s_menu_node_list->next);

            if(node->sub_menu.update_minor_info_cb != NULL)
            {
                node->sub_menu.update_minor_info_cb();
            }
        }
    }

    return true;
}

int menu_load_item_resource(resgroup_resource_t * res_group, menuitem_style_t * menuitem_style, \
        menuitem_t * menuitem,  int count)
{
    int i = 0 ;
    int j = 0;
    picture_resource_t * pic_res;
    string_resource_t  * str_res;

    for(i = 0; i < count; i++)
    {
        if(menuitem_style[i].visible == false)
        {
            continue; 
        }

        if(menuitem_style[i].style_unsel_icon_id != 0)
        {
            pic_res = ( picture_resource_t * )get_resgroup_child(res_group, menuitem_style[i].style_unsel_icon_id);
            if(pic_res != NULL)
            {
                menuitem[j].unsel_icon_id = pic_res->pic_id;
            }
        }
        else
        {
            menuitem[j].unsel_icon_id = 0;
        }

        if(menuitem_style[i].style_sel_icon_id != 0)
        {
            pic_res = ( picture_resource_t * )get_resgroup_child(res_group, menuitem_style[i].style_sel_icon_id);
            if(pic_res != NULL)
            {
                menuitem[j].sel_icon_id = pic_res->pic_id;
            }
        }
        else
        {
            menuitem[j].sel_icon_id = 0;
        }

        if(menuitem_style[i].style_text_id != 0)
        {
            str_res = ( string_resource_t * )get_resgroup_child(res_group, menuitem_style[i].style_text_id);
            if(str_res != NULL)
            {
                menuitem[j].text_id = str_res->str_id;
            }
        }
        else
        {
            menuitem[j].text_id = 0;
        }

        menuitem[j].has_child = menuitem_style[i].has_child;
        menuitem[j].callback = menuitem_style[i].callback;
        menuitem[j].text = menuitem_style[i].text;
        menuitem_style[i].index = j;
        j ++;
    }

    return j;
}


int maps_find(value_map_t * maps, int count, int value)
{
    int i;
    for(i = 0; i < count; i++)
    {
        if(maps[i].value == value)
        {
            return i;
        }
    }

    print_warning("%d is not in maps!", value);
    return 0;
}

