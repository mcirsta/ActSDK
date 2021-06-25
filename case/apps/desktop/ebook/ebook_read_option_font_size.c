

#include "ebook.h"
#include "ebook_read_option_font_size.h"

static bool read_option_font_size_enter( menuview_t* menuview, resgroup_resource_t* resgroup );
static bool _read_option_font_size_exit(void);
static bool _option_font_size_init( resgroup_resource_t* resgroup );
static void read_option_font_size_deinit(void);

static int _proc_item_large( menuview_t *data, menuview_sig_e signal );
static int _proc_item_medium( menuview_t *data, menuview_sig_e signal );
static int _proc_item_small( menuview_t *data, menuview_sig_e signal );

static void _set_current_item( font_type_e last );

static menuview_t* s_menuview = NULL;
static menu_t* s_menu = NULL;
static scene_read_option_t* s_read_option = NULL;

static read_option_font_size_t s_read_option_font_size_api =
{
    read_option_font_size_enter,
    read_option_font_size_deinit
};

static bool read_option_font_size_enter( menuview_t* menuview, resgroup_resource_t* resgroup )
{
//      print_dbg("_read_option_font_size_enter.......");

    s_menuview = menuview;

    _option_font_size_init( resgroup );

    menuview_set_menu( menuview, s_menu );
    menuview_paint( menuview );

    return true;
}

static bool _option_font_size_init( resgroup_resource_t* resgroup )
{
    string_resource_t* temp;
    picture_resource_t* picture;
    menuitem_t menu_item;
    menuitem_t* menu_item_p;
  
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

    /* load strings */    
    /* small font */
    temp = ( string_resource_t* )g_style_api->get_resgroup_child( resgroup, STR_EB_S_FON );
    if( temp == NULL )
    {
        print_err(" scene read : load STR_EB_L_FON failure \n" ); 
        return false;
    }
    memset( &menu_item, 0, sizeof( menuitem_t ) );
    menu_item.text_id = temp->str_id;
    menu_item.callback = (item_cb_t)_proc_item_small;
    menu_append_item( s_menu, &menu_item );

    /* medium font */
    temp = ( string_resource_t* )g_style_api->get_resgroup_child( resgroup, STR_EB_M_FON );
    if( temp == NULL )
    {
        print_err(" scene read : load STR_EB_L_FON failure \n" ); 
        return false;
    }
    memset( &menu_item, 0, sizeof( menuitem_t ) );
    menu_item.text_id = temp->str_id;
    menu_item.callback = (item_cb_t)_proc_item_medium;
    menu_append_item( s_menu, &menu_item );

    /* large font */
    temp = ( string_resource_t* )g_style_api->get_resgroup_child( resgroup, STR_EB_L_FON );
    if( temp == NULL )
    {
        print_err(" scene read : load STR_EB_L_FON failure \n" ); 
        return false;
    }
    memset( &menu_item, 0, sizeof( menuitem_t ) );
    menu_item.text_id = temp->str_id;
    menu_item.callback = (item_cb_t)_proc_item_large;
    menu_append_item( s_menu, &menu_item );


    /* 设置当前选中的图标 */
    picture = g_ebook_main->get_dot_picture();
    menu_item_p = menu_get_item( s_menu, g_scene_read->get_font_type() );
    if ( menu_item_p != NULL )
    {
        menu_item_p->unsel_icon_id = picture->pic_id;
        menu_item_p->sel_icon_id = picture->pic_id;
    }
    
    return true;
}


static int _proc_item_large( menuview_t *data, menuview_sig_e signal )
{
    switch ( signal ) 
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            font_type_e last;
            
//              s_read_option->scene_read_option_exit();

            last = g_scene_read->get_font_type();
            g_scene_read->change_font_type( LARGE_FONT );

            _set_current_item( last );
            break;
        }

        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            _read_option_font_size_exit();
            break;
        }
             
        default:
        {
            break;
        }
    }   

    return 0;
}

static int _proc_item_medium( menuview_t *data, menuview_sig_e signal )
{
    switch ( signal ) 
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            font_type_e last;
            
//              s_read_option->scene_read_option_exit();

            last = g_scene_read->get_font_type();
            g_scene_read->change_font_type( MEDIUM_FONT );

            _set_current_item( last );            
            break;
        }

        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            _read_option_font_size_exit();
            break;
        }
             
        default:
        {
            break;
        }
    }   

    return 0;
}

static int _proc_item_small( menuview_t *data, menuview_sig_e signal )
{
    switch ( signal ) 
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            font_type_e last;
            
//              s_read_option->scene_read_option_exit();

            last = g_scene_read->get_font_type();
            g_scene_read->change_font_type( SMALL_FONT );

            _set_current_item( last );
            break;
        }

        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            _read_option_font_size_exit();
            break;
        }
             
        default:
        {
            break;
        }
    }   

    return 0;
}

static void _set_current_item( font_type_e last )
{
    picture_resource_t* picture;
    menuitem_t* menu_item_p;

    picture = g_ebook_main->get_dot_picture();

    /* unset last */
    menu_item_p = menu_get_item( s_menu, last );
    if ( menu_item_p != NULL )
    {
        menu_item_p->unsel_icon_id = 0;
        menu_item_p->sel_icon_id = 0;
    }

    /* set current */
    menu_item_p = menu_get_item( s_menu, g_scene_read->get_font_type() );
    if ( menu_item_p != NULL )
    {
        menu_item_p->unsel_icon_id = picture->pic_id;
        menu_item_p->sel_icon_id = picture->pic_id;
    }

    menuview_paint( s_menuview );
}

static void read_option_font_size_deinit(void)
{
    menu_delete( s_menu );
    s_menu = NULL;

    s_menuview = NULL;
}

static bool _read_option_font_size_exit(void)
{
    read_option_font_size_deinit();

    s_read_option->scene_read_option_enter();

    return true;
}

read_option_font_size_t* create_read_option_font_size( scene_read_option_t* read_option )
{
    s_read_option = read_option;

    return &s_read_option_font_size_api;
}

