

#include "ebook.h"
#include "ebook_read_option_page_effect.h"

static bool read_option_page_effect_enter( menuview_t* menuview, resgroup_resource_t* resgroup );
static bool _read_option_page_effect_exit(void);
static bool _option_page_effect_init( resgroup_resource_t* resgroup );
static void read_option_page_effect_deinit(void);

static int _proc_item_off( menuview_t *data, menuview_sig_e signal );
static int _proc_item_move_page( menuview_t *data, menuview_sig_e signal );
static int _proc_item_turn_page( menuview_t *data, menuview_sig_e signal );

static menu_t* s_menu = NULL;
static scene_read_option_t* s_read_option = NULL;

static string_resource_t* s_off_string = NULL;
static string_resource_t* s_move_page_string = NULL;
static string_resource_t* s_turn_page_string = NULL;

static read_option_page_effect_t s_read_option_page_effect_api =
{
    read_option_page_effect_enter,
    read_option_page_effect_deinit
};

static bool read_option_page_effect_enter( menuview_t* menuview, resgroup_resource_t* resgroup )
{
//      print_dbg("_read_option_page_effect_enter.......");

    _option_page_effect_init( resgroup );

    menuview_set_menu( menuview, s_menu );
    menuview_paint( menuview );

    return true;
}

static bool _option_page_effect_init( resgroup_resource_t* resgroup )
{
    string_resource_t* temp;
    picture_resource_t* picture;
    menuitem_t menu_item;
    menuitem_t* menu_item_p;
    int idx = 0;
    fusion_mode_e mode;
  
    if ( resgroup == NULL )
    {
        print_err(" resgroup == NULL "); 
        return false;
    }

    s_menu = menu_create();
    if ( s_menu == NULL )
    {
        print_err("menu_create(); fail !");
        return false;
    }

    mode = g_scene_read->get_fusion_mode();

    /* load strings */    
    
    /* off */
    temp = ( string_resource_t* )g_style_api->get_resgroup_child( resgroup, STR_OFF );
    if( temp == NULL )
    {
        print_err(" scene read : load STR_OFF failure \n" ); 
        return false;
    }
    s_off_string = temp;
    memset( &menu_item, 0, sizeof( menuitem_t ) );
    menu_item.text_id = temp->str_id;
    menu_item.callback = (item_cb_t)_proc_item_off;
    menu_append_item( s_menu, &menu_item );

    if ( temp->pixel == (int)mode )
    {
        idx = 0;
    }
    

    /* move page */
    temp = ( string_resource_t* )g_style_api->get_resgroup_child( resgroup, STR_MOVE_PAGE );
    if( temp == NULL )
    {
        print_err(" scene read : load STR_MOVE_PAGE failure \n" ); 
        return false;
    }
    s_move_page_string = temp;
    memset( &menu_item, 0, sizeof( menuitem_t ) );
    menu_item.text_id = temp->str_id;
    menu_item.callback = (item_cb_t)_proc_item_move_page;
    menu_append_item( s_menu, &menu_item );

    if ( temp->pixel == (int)mode )
    {
        idx = 1;
    }

    /* turn page */
    temp = ( string_resource_t* )g_style_api->get_resgroup_child( resgroup, STR_TURN_PAGE );
    if( temp == NULL )
    {
        print_err(" scene read : load STR_MOVE_PAGE failure \n" ); 
        return false;
    }
    s_turn_page_string = temp;
    memset( &menu_item, 0, sizeof( menuitem_t ) );
    menu_item.text_id = temp->str_id;
    menu_item.callback = (item_cb_t)_proc_item_turn_page;
    menu_append_item( s_menu, &menu_item );

    if ( temp->pixel == (int)mode )
    {
        idx = 2;
    }

    /* 设置当前选中的图标 */
    picture = g_ebook_main->get_dot_picture();
    menu_item_p = menu_get_item( s_menu, idx );
    if ( menu_item_p != NULL )
    {
        menu_item_p->unsel_icon_id = picture->pic_id;
        menu_item_p->sel_icon_id = picture->pic_id;
    }

    return true;
}


static int _proc_item_off( menuview_t *data, menuview_sig_e signal )
{
    switch ( signal ) 
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            /* 使用字符串的pixel属性存储实际的特效模式值 */
            g_scene_read->set_fusion_mode( (fusion_mode_e)s_off_string->pixel );
            
            s_read_option->scene_read_option_exit();
            break;
        }

        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            _read_option_page_effect_exit();
            break;
        }
             
        default:
        {
            break;
        }
    }

    return 0;
}

static int _proc_item_move_page( menuview_t *data, menuview_sig_e signal )
{
    switch ( signal ) 
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            g_scene_read->set_fusion_mode( (fusion_mode_e)s_move_page_string->pixel );
            
            s_read_option->scene_read_option_exit();
            break;
        }

        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            _read_option_page_effect_exit();
            break;
        }
             
        default:
        {
            break;
        }
    }   

    return 0;
}

static int _proc_item_turn_page( menuview_t *data, menuview_sig_e signal )
{
    switch ( signal ) 
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            g_scene_read->set_fusion_mode( (fusion_mode_e)s_turn_page_string->pixel );
            
            s_read_option->scene_read_option_exit();
            break;
        }

        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            _read_option_page_effect_exit();
            break;
        }
             
        default:
        {
            break;
        }
    }   

    return 0;
}

static void read_option_page_effect_deinit(void)
{
    menu_delete( s_menu );
    s_menu = NULL;
}

static bool _read_option_page_effect_exit(void)
{
    read_option_page_effect_deinit();

    s_read_option->scene_read_option_enter();

    return true;
}

read_option_page_effect_t* create_read_option_page_effect( scene_read_option_t* read_option )
{
    s_read_option = read_option;

    return &s_read_option_page_effect_api;
}

