/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 * file : setting_list_menu_font_select.c
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

typedef enum
{
    FONT_TYPE_SYSTEM = 0,
    FONT_TYPE_USER,
}font_type_e;

typedef struct font_file_s
{
    font_type_e type;
    char * file_name;
    struct font_file_s * next;
}font_file_t;

static font_file_t * _get_font_file_list(void);
static void _delete_font_file_list( font_file_t * plist);

/*********************************************************************************
 * internal static struct and data definition
 *********************************************************************************/

static menu_t * s_menu = NULL;
static int s_cur_select_item = 0;
static font_file_t * s_font_file_list = NULL;
static int s_select_item_pic_id = 0;
static char s_cur_select_text[128] = {0};
static int s_menu_id = -1;
static char s_sys_def_font_show_text[128] = {0};

/*********************************************************************************
 * interface implementaiton
 *********************************************************************************/
menu_t* setting_list_menu_font_select_enter( menuview_t *menuview, fusion_enter_e fusion_enter_type)
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

const char * setting_list_menu_font_select_get_cur_selected_text(void)
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
    string_resource_t * str_res;

    font_file_t * font_node;
    static menuitem_t* item;
    int cnt = 0;
    const char * text = NULL;

    //initial menu
    if( s_menu == NULL )
    {
        scene = sty_get_app_scene();
        if ( NULL == scene )
        {
            print_err("get app scene failed!");
            return false;
        }

        //load font select menu resource group
        res_group = (resgroup_resource_t *)get_scene_child(scene, REG_FONT);
        if(NULL == res_group)
        {
            print_err("get resource group REG_FONT");
            return false;
        }

        pic_res = ( picture_resource_t * )get_resgroup_child(res_group, PIC_SELECT);
        if(pic_res != NULL)
        {
            s_select_item_pic_id = pic_res->pic_id;
        }

        str_res = ( string_resource_t * )get_resgroup_child(res_group, STR_SYS_DEFAULT_FONT);
        if(str_res != NULL)
        {
            text = sty_get_string(str_res->str_id);
        }

        if(text != NULL)
        {
            strncpy(s_sys_def_font_show_text, text, sizeof(s_sys_def_font_show_text));
        }

        //create menu
        s_menu = menu_create();
        if(NULL == s_menu)
        {
            print_err("create menu failed!");
            return false;
        }

        s_font_file_list = _get_font_file_list();

        //init menu items
        for( font_node = s_font_file_list, cnt = 0; font_node != NULL; font_node = font_node->next, cnt++ )
        {
            item = menuitem_create();
            if(item == NULL)
            {
                print_warning("create menu item failed!");
                continue;
            }

            item->callback = (item_cb_t)_item_cb;

            if(font_node->type == FONT_TYPE_SYSTEM)
            {
                item->text = s_sys_def_font_show_text;
                if(g_setting_cfgs[SET_CFG_TYPE_USER_FONT_VALID].value == 0)
                {
                    item->unsel_icon_id = s_select_item_pic_id;
                    item->sel_icon_id = s_select_item_pic_id;
                    s_cur_select_item = cnt;

                    strncpy(s_cur_select_text, s_sys_def_font_show_text, sizeof(s_cur_select_text));
                }
            }
            else
            {
                item->text = font_node->file_name;
                if( (g_setting_cfgs[SET_CFG_TYPE_USER_FONT_VALID].value != 0)
                       && (font_node->file_name != NULL) 
                        && (strcmp(g_setting_cfgs[SET_CFG_TYPE_USER_FONT].text, font_node->file_name) == 0) )
                {
                    item->unsel_icon_id = s_select_item_pic_id;
                    item->sel_icon_id = s_select_item_pic_id;
                    s_cur_select_item = cnt;

                    strncpy(s_cur_select_text, font_node->file_name, sizeof(s_cur_select_text));
                }
            }

            menu_append_item( s_menu, item);

            menuitem_delete(item);
        }

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

    _delete_font_file_list(s_font_file_list);
    s_font_file_list =  NULL;

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
    menuitem_t *cur_item;
    menuitem_t *last_item;
    msg_apps_t msg = {0};
    char user_font_path[1024] = {0};
    int fontdes = -1; 

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

            if(cur_menu->current > 0)
            {
                sprintf(user_font_path, "%s%s", \
                        sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_SYSTEM_FONT), cur_item->text);
                fontdes = gui_create_font(user_font_path, 16);

                if(fontdes < 0)
                {
                    print_err(" unsupport font!");
                    return;
                }
                else
                {
                    gui_destroy_font(fontdes);
                }
   
                strncpy(g_setting_cfgs[SET_CFG_TYPE_USER_FONT].text, cur_item->text, CFG_TEXT_LEN);
                g_setting_cfgs[SET_CFG_TYPE_USER_FONT_VALID].value = 1;
                strncpy(s_cur_select_text, cur_item->text, sizeof(s_cur_select_text));
            }
            else
            {
                strncpy(g_setting_cfgs[SET_CFG_TYPE_USER_FONT].text, "" , CFG_TEXT_LEN);
                g_setting_cfgs[SET_CFG_TYPE_USER_FONT_VALID].value = 0;

                strncpy(s_cur_select_text, s_sys_def_font_show_text, sizeof(s_cur_select_text));
            }

            g_setting_cfgs[SET_CFG_TYPE_USER_FONT_VALID].changed = true;
            setting_cfg_write(&g_setting_cfgs[SET_CFG_TYPE_USER_FONT_VALID]);

            g_setting_cfgs[SET_CFG_TYPE_USER_FONT].changed = true;
            setting_cfg_write(&g_setting_cfgs[SET_CFG_TYPE_USER_FONT]);

            // emit font changed widget signal
            setting_list_widget_menu_emit_signal(menuview, WIDGET_SIG_MENU_FONT_CHANGED);

            // brodcast font changed msg
            msg.type = MSG_FONT_CHANGED;
            broadcast_msg(&msg);

            last_item = menu_get_item(cur_menu, s_cur_select_item);
            if(last_item!= NULL)
            {
                last_item->unsel_icon_id = 0;
                last_item->sel_icon_id = 0;
            }

            s_cur_select_item = cur_menu->current;
            cur_item->unsel_icon_id = s_select_item_pic_id;
            cur_item->sel_icon_id = s_select_item_pic_id;

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

static font_file_t * _get_font_file_list(void)
{
    DIR      *dir;
    struct dirent   *file;
    struct stat statbuf;
    char abs_file_path[512] = {0};
    font_file_t * head = NULL;
    font_file_t * tail = NULL;
    font_file_t * node = NULL;
    char * root_path = NULL;

    node =  (font_file_t *)malloc(sizeof(font_file_t));
    if(node == NULL)
    {
        print_err("malloc failed!");
        return NULL;
    }

    node->file_name = (char *)malloc(strlen(g_setting_cfgs[SET_CFG_TYPE_DEFAULT_FONT].text) + 1);
    if(node->file_name == NULL)
    {
        print_err("malloc failed!");
        free(node);
        return NULL;
    }

    node->type =  FONT_TYPE_SYSTEM;
    strcpy(node->file_name, g_setting_cfgs[SET_CFG_TYPE_DEFAULT_FONT].text);
    node->next =  NULL;

    head =  node;
    tail = head;

    /* find user font file */
    root_path = sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_SYSTEM_FONT);
    sys_check_media_dir(sys_get_media_dir(HOTPLUGIN_DEV_TYPE_LOCAL,DIR_SYSTEM_FONT), UDISK_ROOT);
    dir = opendir(root_path);
    if ( dir == NULL ) 
    {
        print_err( "opendir(%s) failed!", root_path);
        free(node->file_name);
        free(node);
        return NULL;
    }

    for( file = readdir(dir); file != NULL; file = readdir(dir) )
    {
        sprintf(abs_file_path, "%s/%s", root_path, file->d_name);
        if( (stat(abs_file_path, &statbuf) == 0) && (!S_ISDIR(statbuf.st_mode)) )
        {
            char * p;
            p = strrchr(file->d_name, '.');
            if( (p != NULL) && (strcasecmp(p, ".ttf") == 0) )
            {
                /* find a file with a '.ttf' extention name */
                node =  (font_file_t *)malloc(sizeof(font_file_t));

                if(node == NULL)
                {
                    print_err("malloc failed!");
                    continue;
                }

                node->file_name = (char *)malloc(strlen(file->d_name) + 1);

                if(node->file_name == NULL)
                {
                    print_err("malloc failed!");
                    free(node);
                    continue;
                }

                node->type =  FONT_TYPE_USER;
                strcpy(node->file_name, file->d_name);
                node->next =  NULL;

                tail->next = node;
                tail = tail->next;
            }
        }
    }

    if ( closedir(dir) != 0 )
    {
        print_err( "close directory: %s  failed!", root_path);
    }

    return head;
}

static void _delete_font_file_list( font_file_t * plist)
{
    font_file_t * node = plist; 
    while(plist != NULL) 
    {
        node = plist;
        plist = plist->next;
        free(node->file_name);
        free(node);
    }

}
