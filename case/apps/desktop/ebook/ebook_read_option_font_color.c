

#include "ebook.h"
#include "ebook_read_option_font_color.h"

static bool read_option_font_color_enter( menuview_t* menuview, resgroup_resource_t* resgroup );
static bool _read_option_font_color_exit(void);
static bool _option_font_color_init( resgroup_resource_t* resgroup );
static void read_option_font_color_deinit(void);

static int _proc_item( menuview_t *data, menuview_sig_e signal );

static menuview_t* s_menuview = NULL;
static menu_t* s_menu = NULL;
static scene_read_option_t* s_read_option = NULL;

static read_option_font_color_t s_read_option_font_color_api =
{
    read_option_font_color_enter,
    read_option_font_color_deinit
};

static bool read_option_font_color_enter( menuview_t* menuview, resgroup_resource_t* resgroup )
{
//      print_dbg("_read_option_font_size_enter.......");

    s_menuview = menuview;

    _option_font_color_init( resgroup );

    menuview_set_menu( menuview, s_menu );
    menuview_paint( menuview );

    return true;
}

static bool _option_font_color_init( resgroup_resource_t* resgroup )
{
    menuitem_t menu_item;
    menuitem_t* menu_item_p;

    unsigned int color_num;
    picture_resource_t* picture;
    string_resource_t* string;

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

    color_num = g_scene_read->get_font_color_num();
    picture = g_ebook_main->get_dot_picture();

    /* load strings */
    /* default */
    string = ( string_resource_t* )g_style_api->get_resgroup_child( resgroup, STR_DEFAULT );
    if( string == NULL )
    {
        print_err(" scene read : load STR_EB_L_FON failure \n" ); 
        return false;
    }
    memset( &menu_item, 0, sizeof( menuitem_t ) );
    menu_item.text_id = string->str_id;
    menu_item.callback = (item_cb_t)_proc_item;
    menu_append_item( s_menu, &menu_item );

    /* color1 */
    string = ( string_resource_t* )g_style_api->get_resgroup_child( resgroup, STR_COLOR1 );
    if( string == NULL )
    {
        print_err(" scene read : load STR_EB_L_FON failure \n" ); 
        return false;
    }
    memset( &menu_item, 0, sizeof( menuitem_t ) );
    menu_item.text_id = string->str_id;
    menu_item.callback = (item_cb_t)_proc_item;
    menu_append_item( s_menu, &menu_item );

    /* color2 */
    string = ( string_resource_t* )g_style_api->get_resgroup_child( resgroup, STR_COLOR2 );
    if( string == NULL )
    {
        print_err(" scene read : load STR_EB_L_FON failure \n" ); 
        return false;
    }
    memset( &menu_item, 0, sizeof( menuitem_t ) );
    menu_item.text_id = string->str_id;
    menu_item.callback = (item_cb_t)_proc_item;
    menu_append_item( s_menu, &menu_item );

    /* color3 */
    string = ( string_resource_t* )g_style_api->get_resgroup_child( resgroup, STR_COLOR3 );
    if( string == NULL )
    {
        print_err(" scene read : load STR_EB_L_FON failure \n" ); 
        return false;
    }
    memset( &menu_item, 0, sizeof( menuitem_t ) );
    menu_item.text_id = string->str_id;
    menu_item.callback = (item_cb_t)_proc_item;
    menu_append_item( s_menu, &menu_item );

    /* color4 */
    string = ( string_resource_t* )g_style_api->get_resgroup_child( resgroup, STR_COLOR4 );
    if( string == NULL )
    {
        print_err(" scene read : load STR_EB_L_FON failure \n" ); 
        return false;
    }
    memset( &menu_item, 0, sizeof( menuitem_t ) );
    menu_item.text_id = string->str_id;
    menu_item.callback = (item_cb_t)_proc_item;
    menu_append_item( s_menu, &menu_item );

//	    for ( i = 0; i < color_num; i++ )
//	    {
//	        string = (char*)malloc( 10 );
//	        if ( string == NULL )
//	        {
//	            continue;
//	        }
//	        
//	        memset( string ,0, 10 );        
//	        strcpy( string, "color" );
//	        snprintf( string, 10, "%s%d", string, ( i + 1 ) );
//	
//	        memset( &menu_item, 0, sizeof( menuitem_t ) );
//	        menu_item.text = string;
//	        menu_item.callback = (item_cb_t)_proc_item;
//	        menu_append_item( s_menu, &menu_item );       
//	    }

    /* 设置当前选中的图标 */
    picture = g_ebook_main->get_dot_picture();
    menu_item_p = menu_get_item( s_menu, (int)g_scene_read->get_font_color() );
    if ( menu_item_p != NULL )
    {
        menu_item_p->unsel_icon_id = picture->pic_id;
        menu_item_p->sel_icon_id = picture->pic_id;
    }
    
    return true;
}


static int _proc_item( menuview_t *data, menuview_sig_e signal )
{
    switch ( signal ) 
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            unsigned int last; 
            unsigned int m_index;
            picture_resource_t* picture;
            menuitem_t* menu_item_p;
            
            m_index = ( unsigned int )menu_get_cur_index( s_menu );

            last = g_scene_read->get_font_color();
            g_scene_read->change_font_color( m_index );

            picture = g_ebook_main->get_dot_picture();

            /* unset last item */
            menu_item_p = menu_get_item( s_menu, (int)last );
            if ( menu_item_p != NULL )
            {
                menu_item_p->unsel_icon_id = 0;
                menu_item_p->sel_icon_id = 0;
            }

            /* set current item */
            menu_item_p = menu_get_item( s_menu, (int)m_index );
            if ( menu_item_p != NULL )
            {
                menu_item_p->unsel_icon_id = picture->pic_id;
                menu_item_p->sel_icon_id = picture->pic_id;
            }
            menuview_paint( s_menuview );
            
            break;
        }

        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            _read_option_font_color_exit();
            break;
        }
             
        default:
        {
            break;
        }
    }   

    return 0;
}

static void read_option_font_color_deinit(void)
{
    int sum;
    int i;
    menuitem_t* item;

    if ( s_menu != NULL )
    {
        sum = s_menu->total;
        for ( i = 1; i < sum; i++ )
        {
            item = menu_get_item( s_menu, i );
            if ( item != NULL )
            {
                free( item->text );
                item->text = NULL;
            }
        }

        menu_delete( s_menu );
        s_menu = NULL;
    }

    s_menuview = NULL;
}

static bool _read_option_font_color_exit(void)
{
    read_option_font_color_deinit();

    s_read_option->scene_read_option_enter();

    return true;
}

read_option_font_color_t* create_read_option_font_color( scene_read_option_t* read_option )
{
    s_read_option = read_option;

    return &s_read_option_font_color_api;
}

