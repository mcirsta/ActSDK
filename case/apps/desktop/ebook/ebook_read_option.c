
#include "ebook.h"
#include "ebook_read_option.h"
#include "ebook_read_option_font_size.h"
#include "ebook_read_option_font_color.h"
#include "ebook_read_option_select_coding.h"
#include "ebook_read_option_page_effect.h"

static bool s_has_change_size_item = false;

void set_has_change_size_item(void);

void scene_read_option_set_scene(scene_t* scene);
menuview_t* scene_read_option_enter(void);
bool scene_read_option_exit(void);

static bool _load_style_resources( scene_t* scene );
static bool _create_widgets(void);

static void _menuview_callback( menuview_t* list, menuview_sig_e signal );

static int _proc_item_skip_to( menuview_t *data, menuview_sig_e signal );
static int _proc_item_play_gap( menuview_t *data, menuview_sig_e signal );
static int _proc_item_add_bookmark( menuview_t *data, menuview_sig_e signal );
static int _proc_item_select_coding( menuview_t *data, menuview_sig_e signal );
static int _proc_item_font_size( menuview_t *data, menuview_sig_e signal );
static int _proc_item_font_color( menuview_t *data, menuview_sig_e signal );
static int _proc_item_page_effect( menuview_t *data, menuview_sig_e signal );
static int _proc_item_delete_file( menuview_t *data, menuview_sig_e signal );

void _option_fusion_set_first_frame(menuview_t *mv);
void _option_fusion_set_last_frame(menuview_t *mv);

static scene_t* s_scene = NULL;

static menuview_t* s_menuview = NULL;
static menu_t* s_menu = NULL;

static resgroup_resource_t* s_option_resgroup = NULL;

static read_option_font_size_t* s_read_option_font_size = NULL;
static read_option_font_color_t* s_read_option_font_color = NULL;
static read_option_select_coding_t* s_read_option_select_coding = NULL;
static read_option_page_effect_t* s_read_option_page_effect = NULL;

static scene_read_option_t s_scene_read_option_api =
{
    set_has_change_size_item,
        
    scene_read_option_set_scene,
    scene_read_option_enter,
    scene_read_option_exit
};


void scene_read_option_set_scene(scene_t* scene)
{
    s_scene = scene;
}

menuview_t* scene_read_option_enter(void)
{
    bool ret;
    bool need_fusion = false;

    /* 从子菜单返回时，可能没有 "跳转至" 一项，所以每次都重新生成菜单 */
    
    if ( s_menuview == NULL )
    {
        /* 第一次进入才有特效 */
        need_fusion = true;             
    }
    else
    {
        if ( s_menu != NULL )
        {
            menu_delete( s_menu );
            s_menu = NULL;
        }

        if ( s_menuview != NULL )
        {
            menuview_delete( s_menuview );
            s_menuview = NULL;
        }    
    }

    ret = _create_widgets();
    if ( ret == false )
    {
        return NULL;
    }
    
    ret = _load_style_resources( s_scene );
    if ( ret == false )
    {
        return NULL;
    }

    if ( need_fusion == true )
    {
        _option_fusion_set_first_frame( s_menuview );
    }
    
    menuview_set_menu( s_menuview, s_menu );
    menuview_paint( s_menuview );

    if ( need_fusion == true )
    {
        _option_fusion_set_last_frame( s_menuview );
        
        fusion_display( NULL);
    }

    return s_menuview;
}

static bool _create_widgets(void)
{
    s_menuview = menuview_create( MENUVIEW_OPTION, _menuview_callback );
    if ( s_menuview == NULL )
    {
        print_err("menuview_create(); fail !");
        return false;
    }

    s_menu = menu_create();
    if ( s_menu == NULL )
    {
        print_err("menu_create(); fail !");
        return false;
    }

    return true;
}

static bool _load_style_resources( scene_t* scene )
{
    string_resource_t* temp;
    menuitem_t menu_item;
    
    if ( scene == NULL )
    {
        print_err(" read scene load scene failure\n"); 
        return false;
    }
    
            
    s_option_resgroup = (resgroup_resource_t*)g_style_api->get_scene_child( scene, OPTION_RESGROUP );
    if( s_option_resgroup == NULL)
    {
        print_err(" read scene load OPTION_RESGROUP failure "); 
        return false;
    }
    
    /* load strings */  
    
    /* skip to */
    if ( g_scene_read->count_page_finished() == true )
    {
        temp = ( string_resource_t* )g_style_api->get_resgroup_child( s_option_resgroup, STR_EB_SKIP );
        if( temp == NULL )
        {
            print_err(" scene read : load STR_EB_SKIP failure \n" ); 
            return false;
        }
        memset( &menu_item, 0, sizeof( menuitem_t ) );
        menu_item.text_id = temp->str_id;
        menu_item.callback = (item_cb_t)_proc_item_skip_to;
        menu_append_item( s_menu, &menu_item );
    }
    

    /* play gap */
    temp = ( string_resource_t* )g_style_api->get_resgroup_child( s_option_resgroup, STR_EB_AUTOT );
    if( temp == NULL )
    {
        print_err(" scene read : load STR_EB_AUTOT failure \n" ); 
        return false;
    }
    memset( &menu_item, 0, sizeof( menuitem_t ) );
    menu_item.text_id = temp->str_id;
    menu_item.callback = (item_cb_t)_proc_item_play_gap;
    menu_append_item( s_menu, &menu_item );

    /* add bookmark */
    temp = ( string_resource_t* )g_style_api->get_resgroup_child( s_option_resgroup, STR_EBK_MRK );
    if( temp == NULL )
    {
        print_err(" scene read : load STR_EBK_MRK failure \n" ); 
        return false;
    }
    memset( &menu_item, 0, sizeof( menuitem_t ) );
    menu_item.text_id = temp->str_id;
    menu_item.callback = (item_cb_t)_proc_item_add_bookmark;
    menu_append_item( s_menu, &menu_item );

    
    /* select coding */
    temp = ( string_resource_t* )g_style_api->get_resgroup_child( s_option_resgroup, STR_EB_SEL_CODE );
    if ( g_scene_read->get_file_type() == TXT_ASCII_MBCS )
    {
        memset( &menu_item, 0, sizeof( menuitem_t ) );
        menu_item.text_id = temp->str_id;
        menu_item.has_child = true;
        menu_item.callback = (item_cb_t)_proc_item_select_coding;
        menu_append_item( s_menu, &menu_item );
    }

    /* font size */
    if ( s_has_change_size_item == true )
    {
        temp = ( string_resource_t* )g_style_api->get_resgroup_child( s_option_resgroup, STR_EB_FONT );
        if( temp == NULL )
        {
            print_err(" scene read : load STR_EB_FONT failure \n" ); 
            return false;
        }
        memset( &menu_item, 0, sizeof( menuitem_t ) );
        menu_item.text_id = temp->str_id;
        menu_item.has_child = true;
        menu_item.callback = (item_cb_t)_proc_item_font_size;
        menu_append_item( s_menu, &menu_item );
    }
    
    /* font color */
    temp = ( string_resource_t* )g_style_api->get_resgroup_child( s_option_resgroup, STR_EB_FONT_COLOR );
    memset( &menu_item, 0, sizeof( menuitem_t ) );
    menu_item.text_id = temp->str_id;
    menu_item.has_child = true;
    menu_item.callback = (item_cb_t)_proc_item_font_color;
    menu_append_item( s_menu, &menu_item );


    /* page effect */
    temp = ( string_resource_t* )g_style_api->get_resgroup_child( s_option_resgroup, STR_PAGE_EF );
    memset( &menu_item, 0, sizeof( menuitem_t ) );
    menu_item.text_id = temp->str_id;
    menu_item.has_child = true;
    menu_item.callback = (item_cb_t)_proc_item_page_effect;
    menu_append_item( s_menu, &menu_item );

    
    /* delete file */
    temp = ( string_resource_t* )g_style_api->get_resgroup_child( s_option_resgroup, STR_MDELFILE );
    if( temp == NULL )
    {
        print_err(" scene read : load STR_MDELFILE failure \n" ); 
        return false;
    }
    memset( &menu_item, 0, sizeof( menuitem_t ) );
    menu_item.text_id = temp->str_id;
    menu_item.callback = (item_cb_t)_proc_item_delete_file;
    menu_append_item( s_menu, &menu_item );

    return true;
}

static int _proc_item_skip_to( menuview_t *data, menuview_sig_e signal )
{       
    switch ( signal ) 
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            g_scene_read->creat_skip_to_adjust();
            break;
        }
        
        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            scene_read_option_exit( );
            break;
        }
        
        default:
        {
            break;
        }
    }   

    return 0;
}

static int _proc_item_play_gap( menuview_t *data, menuview_sig_e signal )
{
    switch ( signal ) 
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            g_scene_read->creat_auto_play_gap_adjust();
            break;
        }
        
        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            scene_read_option_exit( );
            break;
        }
        
        default:
        {
            break;
        }
    }

    return 0;
}

static int _proc_item_add_bookmark( menuview_t *data, menuview_sig_e signal )
{
    switch ( signal ) 
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            g_scene_read->add_current_page_to_bookmark();
            scene_read_option_exit( );
            break;
        }
        
        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            scene_read_option_exit( );
            break;
        }
        
        default:
        {
            break;
        }
    }

    return 0;
}

static int _proc_item_select_coding( menuview_t *data, menuview_sig_e signal )
{
    switch ( signal ) 
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            s_read_option_select_coding->read_option_select_coding_enter( s_menuview, s_option_resgroup );
            break;
        }
        
        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            scene_read_option_exit( );
            break;
        }

        default:
        {
            break;
        }
    }

    return 0;
}

static int _proc_item_font_size( menuview_t *data, menuview_sig_e signal )
{
    switch ( signal ) 
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            s_read_option_font_size->read_option_font_size_enter( s_menuview, s_option_resgroup );
            break;
        }

        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            scene_read_option_exit( );
            break;
        }
    
        default:
        {
            break;
        }
    }

    return 0;
}

static int _proc_item_font_color( menuview_t *data, menuview_sig_e signal )
{
    switch ( signal ) 
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            s_read_option_font_color->read_option_font_color_enter( s_menuview, s_option_resgroup );
            break;
        }
        
        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            scene_read_option_exit( );       
            break;
        }
    
        default:
        {
            break;
        }
    }

    return 0;
}

static int _proc_item_page_effect( menuview_t *data, menuview_sig_e signal )
{
    switch ( signal ) 
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            s_read_option_page_effect->read_option_page_effect_enter( s_menuview, s_option_resgroup );
            break;
        }

        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            scene_read_option_exit( );    
            break;
        }
    
        default:
        {
            break;
        }
    }

    return 0;
}

static int _proc_item_delete_file( menuview_t *data, menuview_sig_e signal )
{
    switch ( signal ) 
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            g_scene_read->try_delete_current_file();
            break;
        }

        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            scene_read_option_exit( );    
            break;
        }
    
        default:
        {
            break;
        }
    }

    return 0;
}





static void _menuview_callback( menuview_t* list, menuview_sig_e signal )
{
}

bool scene_read_option_exit(void)
{
    s_read_option_font_size->read_option_font_size_deinit();
    s_read_option_font_color->read_option_font_color_deinit();
    s_read_option_select_coding->read_option_select_coding_deinit();
    s_read_option_page_effect->read_option_page_effect_deinit();

    if ( s_menu != NULL )
    {
        menu_delete( s_menu );
        s_menu = NULL;
    }

    if ( s_menuview != NULL )
    {
        menuview_delete( s_menuview );
        s_menuview = NULL;
    }

    g_scene_read->clear_option_menuview();
    /* 重启自动阅读的定时器。 */
    g_scene_read->restart_auto_reading_timer();

    return true;
}

/*
 * Option菜单特效第一帧 */
void _option_fusion_set_first_frame(menuview_t *mv)
{
    fusion_effect_t eff;
    fusion_frame_t frame;
    canvas_info_t info;

    canvas_get_info(mv->canvas_id, &info);

    frame.rect.x = info.rect.x;
    frame.rect.y = info.rect.y;
    frame.rect.width = info.rect.width;
    frame.rect.height = info.rect.height;
    frame.pdata = NULL;
    frame.alpha = false;

    fusion_get_config( FUSION_TYPE_OPTION_RGB, FUSION_FORWARD, &eff);
    fusion_set_effect(&eff);

    fusion_set_first_frame(&frame);
}

/*
 * Option菜单特效最后一帧
 */
void _option_fusion_set_last_frame(menuview_t *mv)
{
    fusion_frame_t frame;
    canvas_info_t info;

    canvas_get_info(mv->canvas_id, &info);

    frame.rect.x = info.rect.x;
    frame.rect.y = info.rect.y;
    frame.rect.width = info.rect.width;
    frame.rect.height = info.rect.height;
    frame.pdata = (unsigned char *)info.pdata;
    frame.alpha = true;
    fusion_set_last_frame(&frame);
}

void set_has_change_size_item(void)
{
    s_has_change_size_item = true;
}

scene_read_option_t* create_scene_read_option(void)
{
    s_read_option_font_size = create_read_option_font_size( &s_scene_read_option_api );
    s_read_option_font_color = create_read_option_font_color( &s_scene_read_option_api );
    s_read_option_select_coding = create_read_option_select_coding( &s_scene_read_option_api );
    s_read_option_page_effect = create_read_option_page_effect( &s_scene_read_option_api );

    return &s_scene_read_option_api;
}
