

#include "ebook.h"
#include "ebook_read_option_select_coding.h"

bool read_option_select_coding_enter( menuview_t* menuview, resgroup_resource_t* resgroup );
static bool _read_option_select_coding_exit(void);
static bool _option_select_coding_init( resgroup_resource_t* resgroup );
static void read_option_select_coding_deinit(void);

static int _proc_items( menuview_t *data, menuview_sig_e signal );

static menuview_t* s_menuview = NULL;
static menu_t* s_menu = NULL;
static scene_read_option_t* s_read_option = NULL;

/* -1 表示未进入过该菜单 */
static unsigned int s_current_lang_index = -1;

static read_option_select_coding_t s_read_option_select_coding_api =
{
    read_option_select_coding_enter,
    read_option_select_coding_deinit
};

bool read_option_select_coding_enter( menuview_t* menuview, resgroup_resource_t* resgroup )
{
//      print_dbg("_read_option_font_size_enter.......");

    s_menuview = menuview;

    _option_select_coding_init( resgroup );

    menuview_set_menu( menuview, s_menu );
    menuview_paint( menuview );

    return true;
}

static bool _option_select_coding_init( resgroup_resource_t* resgroup )
{
    picture_resource_t* picture;
    menuitem_t menu_item;
    support_language_list_t* languages = NULL;
    unsigned int language;
    int id;
   
    unsigned int i = 0;
    
    s_menu = menu_create();
    if ( s_menu == NULL )
    {
        print_err("menu_create(); fail !");
        return false;
    }

    picture = g_ebook_main->get_dot_picture();

    language = g_scene_read->get_language_mode();
    languages = sys_get_cur_support_language_list();
    while( languages != NULL )
    {
        memset( &menu_item, 0, sizeof( menuitem_t ) );
        menu_item.callback = (item_cb_t)_proc_items;
        menu_item.text = languages->name;
        menu_item.info1 = languages->codes;

        id = sys_get_language_id( languages->codes );

        if ( (unsigned int)id == language )
        {
            menu_item.unsel_icon_id = picture->pic_id;
            menu_item.sel_icon_id = picture->pic_id;

//	            print_dbg(" current language id = %d ;;;;; s_current_lang_index = %d ", 
//	                                languages->id, s_current_lang_index );
            
            /* 跳到当前语言环境项 */
            s_current_lang_index = i;
        }
        
        menu_append_item( s_menu, &menu_item );
    
//          print_dbg("language name = %s, id = %d", languages->name, languages->id);
        languages = languages->next;

        i++;
    }

    /* 跳到当前语言环境项 */
    menu_set_cur_index( s_menu, (int)s_current_lang_index );
      
    return true;
}


static int _proc_items( menuview_t *data, menuview_sig_e signal )
{
    menuitem_t* menu_item_p;
    picture_resource_t* picture;
    char* code;
    int id;
    unsigned int item_index;

    switch ( signal ) 
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {            
            item_index = (unsigned int)menu_get_cur_index( s_menu );
//	            print_dbg(" WIDGET_SIG_MENUVIEW_OK......... item_index = %d ", item_index );
            
            if ( item_index == s_current_lang_index )
            {
                break;
            }
            
            menu_item_p = menu_get_cur_item( s_menu );
            if ( menu_item_p != NULL )
            {
                code = menu_item_p->info1;
                id = sys_get_language_id( code );
                
//	                print_dbg(" select code = %s , id = %d ", code, id );
                
                g_scene_read->change_language_mode( (unsigned int)id );

//                  s_read_option->scene_read_option_exit(); 

                picture = g_ebook_main->get_dot_picture();

                /* set current item */
                menu_item_p->unsel_icon_id = picture->pic_id;
                menu_item_p->sel_icon_id = picture->pic_id;

                /* unset the last */
//	                print_dbg(" unset last index ===  %d ", (int)s_current_lang_index );
                
                menu_item_p = menu_get_item( s_menu, (int)s_current_lang_index );
                if ( menu_item_p != NULL )
                {
                    menu_item_p->unsel_icon_id = 0;
                    menu_item_p->sel_icon_id = 0;
                }
                s_current_lang_index = item_index;

                menuview_paint( s_menuview );
            }

            break;
        }

        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            _read_option_select_coding_exit();
            break;
        }
             
        default:
        {
            break;
        }
    }   

    return 0;
}


static void read_option_select_coding_deinit(void)
{
    menu_delete( s_menu );
    s_menu = NULL;
}

static bool _read_option_select_coding_exit(void)
{
    read_option_select_coding_deinit();

    s_read_option->scene_read_option_enter();

    return true;
}

read_option_select_coding_t* create_read_option_select_coding( scene_read_option_t* read_option )
{
    s_read_option = read_option;

    return &s_read_option_select_coding_api;
}

