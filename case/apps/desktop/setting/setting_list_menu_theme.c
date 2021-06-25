/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 * file : setting_list_menu_theme.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-5-5             v1.0             build this file
 ********************************************************************************/

#include "setting_list_widget_menu.h"

typedef struct theme_s
{
    char  theme[128];
    int  name_id;
    struct theme_s * next;
}theme_t;

static theme_t * _get_theme_list(void);
static void _delete_theme_list(theme_t * plist);
static void _strip_white_space(char * str); 

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

static menu_t *s_menu = NULL;
static int s_cur_select_item = 0;
static int s_select_item_pic_id = 0;
static char s_cur_select_text[128] = {0};
static int s_menu_id = -1;
static theme_t * s_theme_list = NULL;

typedef struct
{
    unsigned int style_id;
    unsigned short str_id;
}str_res_t;

typedef enum
{
    STR_NAME_THEME_1 = 0,
    STR_NAME_THEME_2,
    STR_NAME_THEME_3,
}str_name_e;

static str_res_t s_str_res_table[] =
{
    {STR_THEME1, 0},
    {STR_THEME2, 0},
    {STR_THEME3, 0},
};

static void _load_str_res(resgroup_resource_t * res_group, str_res_t str_res_table[], int count);

/*********************************************************************************
 * interface implementation
 *********************************************************************************/

menu_t* setting_list_menu_theme_enter( menuview_t *menuview, fusion_enter_e fusion_enter_type)
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

const char * setting_list_menu_theme_get_cur_selected_text(void)
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
 * internal static  implementation
 *********************************************************************************/

static bool _menu_init( void )
{
    scene_t * scene;
    resgroup_resource_t * res_group;
    picture_resource_t * pic_res;

    theme_t * theme_node;
    static menuitem_t* item;
    const char * text = NULL;
    int cnt = 0;

    /* initial menu */
    if( s_menu == NULL )
    {
        scene = sty_get_app_scene();
        if ( NULL == scene )
        {
            print_err("get app scene failed!");
            return false;
        }

        /* load theme menu resource group */
        res_group = (resgroup_resource_t *)get_scene_child(scene, REG_THEME);
        if(NULL == res_group)
        {
            print_err("get resource group REG_THEME");
            return false;
        }

        pic_res = ( picture_resource_t * )get_resgroup_child(res_group, PIC_SELECT);
        if(pic_res != NULL)
        {
            s_select_item_pic_id = pic_res->pic_id;
        }

        _load_str_res(res_group, s_str_res_table, ARRAY_SIZE(s_str_res_table));

        //create menu
        s_menu = menu_create();
        if(NULL == s_menu)
        {
            print_err("create menu failed!");
            return false;
        }

        s_theme_list = _get_theme_list();

        //init menu items
        for( theme_node = s_theme_list, cnt = 0; theme_node != NULL; theme_node = theme_node->next, cnt++ )
        {
            item = menuitem_create();
            if(item == NULL)
            {
                print_warning("create menu item failed!");
                continue;
            }

            item->info1 = theme_node->theme;
            item->callback = (item_cb_t)_item_cb;

            if(strrchr(theme_node->theme, '1') != NULL)
            {
                theme_node->name_id = s_str_res_table[STR_NAME_THEME_1].str_id; 
                item->text_id = theme_node->name_id;
            }
            else if(strrchr(theme_node->theme, '2') != NULL)
            {
                theme_node->name_id = s_str_res_table[STR_NAME_THEME_2].str_id; 
                item->text_id = theme_node->name_id;
            }
            else
            {
                theme_node->name_id = s_str_res_table[STR_NAME_THEME_3].str_id; 
                item->text_id = theme_node->name_id;
            }

            if( (item->info1 != NULL) 
                    && (strcmp(g_setting_cfgs[SET_CFG_TYPE_THEME].text, item->info1) == 0) )
            {
                item->unsel_icon_id = s_select_item_pic_id;
                item->sel_icon_id = s_select_item_pic_id;
                s_cur_select_item = cnt;

                text = sty_get_string(item->text_id);

                if(text != NULL)
                {
                    strncpy(s_cur_select_text, text, sizeof(s_cur_select_text));
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

    _delete_theme_list(s_theme_list);
    s_theme_list = NULL;

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

static void _item_cb( menuview_t *menuview, menuview_sig_e signal )
{
    menu_t *cur_menu;
    menuitem_t *cur_item;
    menuitem_t *menu_item;
    const char * text = NULL;
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

            strcpy(g_setting_cfgs[SET_CFG_TYPE_THEME].text, cur_item->info1);
            g_setting_cfgs[SET_CFG_TYPE_THEME].changed = true;
            setting_cfg_write(&g_setting_cfgs[SET_CFG_TYPE_THEME]);

            // emit theme changed widget signal
            setting_list_widget_menu_emit_signal(menuview, WIDGET_SIG_MENU_THEME_CHANGED);

            // brodcast theme changed msg
            msg.type = MSG_THEME_CHANGED;
            broadcast_msg(&msg);

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

static theme_t * _get_theme_list(void)
{
    theme_t * head = NULL;
    theme_t * tail = NULL;
    theme_t * node = NULL;

    char themes[CFG_TEXT_LEN] = {0};
    char * next = NULL;
    char * p = NULL;

    if(get_config(CFG_THEMES, (char *)themes, CFG_TEXT_LEN) < 0)
    {
        print_err("get_config(CFG_THEMES) failed!");
        return NULL;
    }
    
    next = themes;

    while( (*next) != '\0' ) 
    {
        /* Get a theme name from a sequence of themes separated by ',' */
        p = next;

        while ( (*p != '\0') && (isspace(*p) != 0))
        {
            p++;
        }

        while ( (*p) != '\0')
        {
            if(*p == ',')
            {
                break;
            }
            p++;
        }

        node =  (theme_t *)malloc(sizeof(theme_t));
        if(node == NULL)
        {
            printf("malloc failed!\n");
            continue;
        }

        memset(node, 0, sizeof(theme_t));

        memcpy(node->theme, next, (size_t)(p - next));
        _strip_white_space(node->theme);

        node->next =  NULL;

        if(head == NULL)
        {
            head =  node;
            tail = head;
        }
        else
        {
            tail->next = node;
            tail = tail->next;
        }

        if(*p != 0)
        {
            next = p + 1;
        }
        else
        {
            next = p;
        }
    }  

    return head;
}


static void _delete_theme_list( theme_t * plist)
{
    theme_t * node = plist; 
    while(plist != NULL) 
    {
        node = plist;
        plist = plist->next;
        free(node);
    }
}

static void _strip_white_space(char * str) 
{
    char * p = NULL;
    char * q = NULL;

    for(p = &str[(strlen(str) - 1)]; p != (str - 1) ; p--)
    {
        if(isspace(*p) == 0)
        {
            *(p+1) = '\0';
            break;
        }
    }

    for(p = str; *p != '\0'; p++)
    {
        if(isspace(*p) == 0)
        {
            break;
        }
    }

    if(p != str)
    {
        for(q = str; *q != '\0'; q++)
        {
            *q = *p;
            p++;
        }

        *q = *p;
    }
}

static void _load_str_res(resgroup_resource_t * res_group, str_res_t str_res_table[], int count)
{
    int i ;
    string_resource_t  * str_res;

    for(i = 0; i < count; i++)
    {
        str_res = ( string_resource_t * )get_resgroup_child(res_group, str_res_table[i].style_id);
        if(str_res != NULL)
        {
            str_res_table[i].str_id = str_res->str_id;
        }
    }
}

