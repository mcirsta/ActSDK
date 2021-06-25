/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 * file : setting_list_menu_key_tone.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      Herb Hu    2009-5-8             v1.0             build this file
 ********************************************************************************/

#include "setting_list_widget_menu.h"

#define KEY_TONE_FILE_PATH    "/mnt/diska/apps"

typedef struct key_tone_s
{
    char  key_tone[128];
    int  name_id;
    struct key_tone_s * next;
}key_tone_t;

static key_tone_t * _get_key_tone_list(void);
static void _delete_key_tone_list(key_tone_t * plist);
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

static menu_t * s_menu = NULL;
static int s_cur_select_item = 0;
static int s_select_item_pic_id = 0;
static char s_cur_select_text[128] = {0};
static int s_menu_id = -1;
static key_tone_t * s_key_tone_list = NULL;

typedef struct
{
    unsigned int style_id;
    unsigned short str_id;
}str_res_t;

typedef enum
{
    STR_NAME_KEY_TONE_OFF = 0,
    STR_NAME_KEY_TONE_1,
    STR_NAME_KEY_TONE_2,
    STR_NAME_KEY_TONE_3,
    STR_NAME_KEY_TONE_4,
}str_name_e;

static str_res_t s_str_res_table[] =
{
    {STR_KEY_TONE_OFF, 0},
    {STR_KEY_TONE_1, 0},
    {STR_KEY_TONE_2, 0},
    {STR_KEY_TONE_3, 0},
    {STR_KEY_TONE_4, 0},
};

static void _load_str_res(resgroup_resource_t * res_group, str_res_t str_res_table[], int count);

/*********************************************************************************
 * interface implementaiton
 *********************************************************************************/
menu_t* setting_list_menu_key_tone_enter( menuview_t *menuview, fusion_enter_e fusion_enter_type)
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

const char * setting_list_menu_key_tone_get_cur_selected_text(void)
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

    key_tone_t * key_tone_node;
    static menuitem_t* item;
    const char * text = NULL;
    int cnt = 0;

    //initial menu
    if( s_menu != NULL )
    {
        return true;
    }

    scene = sty_get_app_scene();
    if ( NULL ==scene )
    {
        print_err("get app scene failed!");
        return false;
    }

    //load key tone select menu resource group
    res_group = (resgroup_resource_t *)get_scene_child(scene, REG_KEY_TONE);
    if(NULL == res_group)
    {
        print_err("get resource group REG_KEY_TONE");
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

    s_key_tone_list = _get_key_tone_list();

    item = menuitem_create();
    if(item != NULL)
    {
        item->text_id = s_str_res_table[STR_NAME_KEY_TONE_OFF].str_id;
        item->callback = (item_cb_t)_item_cb;

        if((s_key_tone_list == NULL) 
                || (g_setting_cfgs[SET_CFG_TYPE_KEY_TONE_ON].value == 0))
        {
            item->unsel_icon_id = s_select_item_pic_id;
            item->sel_icon_id = s_select_item_pic_id;
            s_cur_select_item = 0;

            text = sty_get_string(item->text_id);
            if(text != NULL)
            {
                strncpy(s_cur_select_text, text, sizeof(s_cur_select_text));
            }
        }

        menu_append_item( s_menu, item);
        menuitem_delete(item);
    }


    //init menu items
    for(key_tone_node = s_key_tone_list, cnt = 1; key_tone_node != NULL; key_tone_node = key_tone_node->next, cnt++)
    {
        item = menuitem_create();
        if(item == NULL)
        {
            print_warning("create menu item failed!");
            continue;
        }

        item->info1 = key_tone_node->key_tone;
        item->callback = (item_cb_t)_item_cb;

        if(strrchr(key_tone_node->key_tone, '1') != NULL)
        {
            key_tone_node->name_id = s_str_res_table[STR_NAME_KEY_TONE_1].str_id; 
            item->text_id = key_tone_node->name_id;
        }
        else if(strrchr(key_tone_node->key_tone, '2') != NULL)
        {
            key_tone_node->name_id = s_str_res_table[STR_NAME_KEY_TONE_2].str_id; 
            item->text_id = key_tone_node->name_id;
        }
        else if(strrchr(key_tone_node->key_tone, '3') != NULL)
        {
            key_tone_node->name_id = s_str_res_table[STR_NAME_KEY_TONE_3].str_id; 
            item->text_id = key_tone_node->name_id;
        }
        else if(strrchr(key_tone_node->key_tone, '4') != NULL)
        {
            key_tone_node->name_id = s_str_res_table[STR_NAME_KEY_TONE_4].str_id; 
            item->text_id = key_tone_node->name_id;
        }
        else
        {
            menuitem_delete(item);
            continue;
        }

        if( (item->info1 != NULL) 
                && (strcmp(g_setting_cfgs[SET_CFG_TYPE_KEY_TONE_CUR].text, item->info1) == 0) 
                && (g_setting_cfgs[SET_CFG_TYPE_KEY_TONE_ON].value != 0))
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

    _delete_key_tone_list(s_key_tone_list);
    s_key_tone_list = NULL;

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
    menuitem_t *menu_item;
    msg_apps_t msg = {0};
    bool on;
    char key_tone_path[512] = {0};
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
            
            if(cur_menu->current == 0)
            {
                g_setting_cfgs[SET_CFG_TYPE_KEY_TONE_ON].value = 0;
                g_setting_cfgs[SET_CFG_TYPE_KEY_TONE_ON].changed = true;
                setting_cfg_write(&g_setting_cfgs[SET_CFG_TYPE_KEY_TONE_ON]);
                msg.type = MSG_SET_KEY_TONE_ON;
                on = false;
                memcpy(msg.content, &on, sizeof(on));
                send_async_msg(APP_NAME_MSG_SERV, &msg);

            }
            else
            {
                sprintf(g_setting_cfgs[SET_CFG_TYPE_KEY_TONE_CUR].text, cur_item->info1);
                g_setting_cfgs[SET_CFG_TYPE_KEY_TONE_CUR].changed = true;
                setting_cfg_write(&g_setting_cfgs[SET_CFG_TYPE_KEY_TONE_CUR]);

                if(get_config(g_setting_cfgs[SET_CFG_TYPE_KEY_TONE_CUR].text, key_tone_path, 512) < 0)
                {
                    print_err("get config: %s failed!", g_setting_cfgs[SET_CFG_TYPE_KEY_TONE_CUR].text);
                }

                if(set_config(CFG_KEY_TONE_FULLPATH, key_tone_path, (int)strlen(key_tone_path)) < 0)
                {
                    print_err("set config: %s failed!", CFG_KEY_TONE_FULLPATH);
                }

                g_setting_cfgs[SET_CFG_TYPE_KEY_TONE_ON].value = 1;
                g_setting_cfgs[SET_CFG_TYPE_KEY_TONE_ON].changed = true;
                setting_cfg_write(&g_setting_cfgs[SET_CFG_TYPE_KEY_TONE_ON]);
                msg.type = MSG_SET_KEY_TONE_ON;
                on = true;
                memcpy(msg.content, &on, sizeof(on));
                send_async_msg(APP_NAME_MSG_SERV, &msg);

            }

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

static key_tone_t * _get_key_tone_list(void)
{
    key_tone_t * head = NULL;
    key_tone_t * tail = NULL;
    key_tone_t * node = NULL;

    char key_tones[CFG_TEXT_LEN] = {0};
    char * next = NULL;
    char * p = NULL;

    if(get_config(CFG_KEY_TONES, (char *)key_tones, CFG_TEXT_LEN) < 0)
    {
        print_err("get_config(CFG_THEMES) failed!");
        return NULL;
    }
    
    next = key_tones;

    while( (*next) != '\0' ) 
    {
        /* Get a key_tone name from a sequence of key_tones separated by ',' */
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

        node =  (key_tone_t *)malloc(sizeof(key_tone_t));
        if(node == NULL)
        {
            printf("malloc failed!\n");
            continue;
        }

        memset(node, 0, sizeof(key_tone_t));

        memcpy(node->key_tone, next, (size_t)(p - next));
        _strip_white_space(node->key_tone);

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


static void _delete_key_tone_list( key_tone_t * plist)
{
    key_tone_t * node = plist; 
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

