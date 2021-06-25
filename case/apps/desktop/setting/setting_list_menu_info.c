/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 * file : setting_list_menu_info.c
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
static void _check_menu_visible( void );

/* menu  item callbacks */
static void _item_cb( menuview_t *menuview, menuview_sig_e signal );

/*********************************************************************************
 * internal static struct and data definition
 *********************************************************************************/

enum menuitem_name
{
    MENU_USERNAME = 0,
    MENU_VERSION ,
    MENU_INTERNAL_SPACE,
    MENU_INTERNAL_FREE,
    MENU_CARD_SPACE,
    MENU_CARD_FREE,
    MENU_EXTERNAL_SPACE,
    MENU_EXTERNAL_FREE,
};

/* menu items style data define */
static menuitem_style_t s_style_items[] =
{
    { PIC_USERNAME,       PIC_USERNAME,       STR_USERNAME,       false, (item_cb_t)_item_cb, NULL, true, 0 },
    { PIC_VERSION,        PIC_VERSION,        STR_VERSION,        false, (item_cb_t)_item_cb, NULL, true, 0 },
    { PIC_INTERNAL_SPACE, PIC_INTERNAL_SPACE, STR_INTERNAL_SPACE, false, (item_cb_t)_item_cb, NULL, true, 0 },
    { PIC_INTERNAL_FREE,  PIC_INTERNAL_FREE,  STR_INTERNAL_FREE,  false, (item_cb_t)_item_cb, NULL, true, 0 },
    { PIC_CARD_SPACE,     PIC_CARD_SPACE,     STR_CARD_SPACE,     false, (item_cb_t)_item_cb, NULL, true, 0 },
    { PIC_CARD_FREE,      PIC_CARD_FREE,      STR_CARD_FREE,      false, (item_cb_t)_item_cb, NULL, true, 0 },
    { PIC_EXTERNAL_SPACE, PIC_EXTERNAL_SPACE, STR_EXTERNAL_SPACE, false, (item_cb_t)_item_cb, NULL, true, 0 },
    { PIC_EXTERNAL_FREE,  PIC_EXTERNAL_FREE,  STR_EXTERNAL_FREE,  false, (item_cb_t)_item_cb, NULL, true, 0 },
};

static menu_t * s_menu = NULL;
static int s_menu_id = -1;

struct storage_info_s
{
    unsigned long long blk_size;
    unsigned long long total_blks;
    unsigned long long avail_blks;
    char total_str[32];
    char free_str[32];
};

static struct storage_info_s s_storage_info_internal =  {0};
static struct storage_info_s s_storage_info_card =  {0};
static struct storage_info_s s_storage_info_external =  {0};

static bool _get_storage_info(const char * file_path, struct storage_info_s * storage);


/*********************************************************************************
 * interface implementaiton
 *********************************************************************************/

menu_t* setting_list_menu_info_enter( menuview_t *menuview, fusion_enter_e fusion_enter_type)
{
    sub_menu_t sub_menu = {0};

    if(_menu_init() == false)
    {
        print_err("init menu data failed!");
        return NULL;
    }
    menuview_set_menu( menuview, s_menu );
    menuview_load_style( menuview, MENUVIEW_PICLIST_INFO );

    if(s_menu_id == -1)
    {
        sub_menu.deinit_cb = _menu_deinit;
        sub_menu.update_minor_info_cb = NULL;
        s_menu_id = setting_list_widget_menu_register_sub_menu(menuview, &sub_menu);
    }

    setting_list_widget_menu_show_fusion(menuview, fusion_enter_type);

    return s_menu;
}

/*********************************************************************************
 * internal static  implementaiton
 *********************************************************************************/

static bool _menu_init( void )
{
    scene_t * scene;
    resgroup_resource_t * res_group;

#pragma __PRQA_IGNORE_START__
    /* 浣跨敤 0 鍒濆鍖栨暟缁� */
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

        //load informaition menu resource group
        res_group = (resgroup_resource_t *)get_scene_child(scene, REG_INFO);
        if(NULL == res_group)
        {
            print_err("get resource group REG_INFO");
            return false;
        }

        _check_menu_visible();
        real_items = menu_load_item_resource(res_group, s_style_items, menu_items, ARRAY_SIZE(s_style_items));


        menu_items[MENU_USERNAME].info1 = g_setting_cfgs[SET_CFG_TYPE_USERNAME].text;
        menu_items[MENU_VERSION].info1 = g_setting_cfgs[SET_CFG_TYPE_VERSION].text;

        if((s_style_items[MENU_INTERNAL_SPACE].visible == true)
                || (s_style_items[MENU_INTERNAL_FREE].visible == true))
        {
            _get_storage_info(UDISK_ROOT, &s_storage_info_internal);
            menu_items[s_style_items[MENU_INTERNAL_SPACE].index].info1 = s_storage_info_internal.total_str;
            menu_items[s_style_items[MENU_INTERNAL_FREE].index].info1 = s_storage_info_internal.free_str;
        }

        if((s_style_items[MENU_CARD_SPACE].visible == true)
                || (s_style_items[MENU_CARD_FREE].visible == true))
        {
            _get_storage_info(CARD_ROOT, &s_storage_info_card);
            menu_items[s_style_items[MENU_CARD_SPACE].index].info1 = s_storage_info_card.total_str;
            menu_items[s_style_items[MENU_CARD_FREE].index].info1 = s_storage_info_card.free_str;
        }

        if((s_style_items[MENU_EXTERNAL_SPACE].visible == true)
                || (s_style_items[MENU_EXTERNAL_FREE].visible == true))
        {
            _get_storage_info(EXTERNAL_ROOT, &s_storage_info_external);
            menu_items[s_style_items[MENU_EXTERNAL_SPACE].index].info1 = s_storage_info_external.total_str;
            menu_items[s_style_items[MENU_EXTERNAL_FREE].index].info1 = s_storage_info_external.free_str;
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

    return true;
}

static void _check_menu_visible( void )
{
    setting_cfg_init(g_menu_info_cfgs, CFG_MENU_INFO_MAX); 
    setting_cfg_read_all(g_menu_info_cfgs, CFG_MENU_INFO_MAX); 

    if(g_menu_info_cfgs[CFG_MENU_INFO_USERNAME].value == 0)
    {
        s_style_items[MENU_USERNAME].visible = false;
    }

    if(g_menu_info_cfgs[CFG_MENU_INFO_VERSION].value == 0)
    {
        s_style_items[MENU_VERSION].visible = false;
    }

    if(g_menu_info_cfgs[CFG_MENU_INFO_INTERNAL_SPACE].value == 0)
    {
        s_style_items[MENU_INTERNAL_SPACE].visible = false;
    }

    if(g_menu_info_cfgs[CFG_MENU_INFO_INTERNAL_FREE].value == 0)
    {
        s_style_items[MENU_INTERNAL_FREE].visible = false;
    }

    if(g_menu_info_cfgs[CFG_MENU_INFO_CARD_SPACE].value == 0)
    {
        s_style_items[MENU_CARD_SPACE].visible = false;
    }

    if(g_menu_info_cfgs[CFG_MENU_INFO_CARD_FREE].value == 0)
    {
        s_style_items[MENU_CARD_FREE].visible = false;
    }

    if(g_menu_info_cfgs[CFG_MENU_INFO_EXTERNAL_SPACE].value == 0)
    {
        s_style_items[MENU_EXTERNAL_SPACE].visible = false;
    }

    if(g_menu_info_cfgs[CFG_MENU_INFO_EXTERNAL_FREE].value == 0)
    {
        s_style_items[MENU_EXTERNAL_FREE].visible = false;
    }

    setting_cfg_deinit(g_menu_info_cfgs, CFG_MENU_INFO_MAX); 
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

static bool _get_storage_info(const char * file_path, struct storage_info_s * storage)
{
    struct statfs fs;
    unsigned long long bytes = 0;
    unsigned long mega_bytes = 0;
    unsigned long kilo_bytes = 0;
    bool result = true;

    if(NULL == storage)
    {
        print_err("paramter storage is a NULL pointer!");
        return false;
    }

    if ( statfs( file_path, &fs ) < 0 )
    {
        print_err("invoke statfs(%s) error!", file_path);

        fs.f_bsize = 0;
        fs.f_blocks = 0;
        fs.f_bavail = 0;

        result = false;
    }

    print_dbg("%s blk_size:%ld, total_blks:%ld, avail_blks:%ld", \
            file_path, fs.f_bsize, fs.f_blocks, fs.f_bavail);

    storage->blk_size = (unsigned long long)fs.f_bsize;
    storage->total_blks = (unsigned long long)fs.f_blocks;
    storage->avail_blks = (unsigned long long)fs.f_bavail;

    print_dbg("%s blk_size:%ld, total_blks:%ld, avail_blks:%ld", \
            file_path, storage->blk_size, storage->total_blks, storage->avail_blks);

    /* format storage info show string */
    bytes = (storage->total_blks * storage->blk_size);

    if(bytes >= (1 << 20))
    {
        mega_bytes = (unsigned long)(bytes >> 20); // convert to mega bytes
        sprintf(storage->total_str, "%ld MB", mega_bytes);
    }
    else if( bytes >= (1 << 10) )
    {
        kilo_bytes = (unsigned long)(bytes >> 10); // convert to kilo bytes
        sprintf(storage->total_str, "%ld KB", kilo_bytes);
    }
    else
    {
        sprintf(storage->total_str, "%ld B", bytes);
    }
        

    bytes = (storage->avail_blks * storage->blk_size); 
    if(bytes >= (1 << 20))
    {
        mega_bytes = (unsigned long)(bytes >> 20); // convert to mega bytes
        sprintf(storage->free_str, "%ld MB", mega_bytes);
    }
    else if( bytes >= (1 << 10) )
    {
        kilo_bytes = (unsigned long)(bytes >> 10); // convert to kilo bytes
        sprintf(storage->free_str, "%ld KB", kilo_bytes);
    }
    else
    {
        sprintf(storage->free_str, "%ld B", bytes);
    }

    return result;
}

