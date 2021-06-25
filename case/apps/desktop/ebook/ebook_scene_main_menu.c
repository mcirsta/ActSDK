/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : scene_listmenu.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *    zhaowenjie    2009-2-25          v1.0               build this file
 ********************************************************************************/
/*!
* \file     scene_listmenu.c
* \brief    listmenu scene
* \author   zhaowenjie
*
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#include "ebook_scene_main_menu.h"

#include "ebook.h"

#include "ebook_sty.h"


static int s_window = 0;
static headbar_t* s_headbar = NULL;
static menuview_t* s_menuview = NULL;
static unsigned short s_current_index = 0;
static volumebar_t* s_volumebar = NULL;
static msgbox_t* s_msgbox = NULL;

static scene_t* s_scene = NULL;

static string_resource_t* s_reading_string;
static string_resource_t* s_dir_list_string;
static string_resource_t* s_bookmark_list_string;


void enter_scene_mainmenu(void);
static bool _scene_mainmenu_init(void);
static bool _scene_mainmenu_deinit(void);
static bool _load_style_resources(void);
static bool _show_scene_mainmenu(void);
static bool _create_widgets(void);
static void _proc_gui_msg(gui_wm_message_t *pmsg);

static void _paint_background(void);
static int _proc_sys_msg( msg_apps_t *msg );
static void _proc_key( key_event_t *msg );

static int _proc_item_now_reading( menuview_t* list, menuview_sig_e signal );
static int _proc_item_dir_list( menuview_t* list, menuview_sig_e signal );
static int _proc_item_bookmark_list( menuview_t* list, menuview_sig_e signal );

static void _scene_rotate(void);

static void _delete_now_reading_item( void );

static void _headbar_callback(headbar_t * headbar, widget_sig_headbar_e signal );
static void _menuview_callback( menuview_t *list, menuview_sig_e signal );
static void _volumebar_callback(volumebar_t *data, volumebar_sig_e signal);
static void _msgbox_callback(msgbox_t * msgbox, msgbox_sig_e signal );


static scene_mainmenu_t s_scene_mainmenu_api =
{
    enter_scene_mainmenu
};

/* scene entry function */
void enter_scene_mainmenu(void)
{
    bool ret;
    msg_apps_t  msg;
    hotplugin_dev_type_e main_disk ; 
    dialog_init_t dialog_init;

    print_dbg(" ************   enter  scene_mainmenu  ************* ");

    /* 场景初始化 */
    ret = _scene_mainmenu_init( );
    if(ret == false )
    {
        print_err("mainmenu scene init error! \n");
        g_ebook_main->set_current_scene( SCENE_EXIT );
        
        goto ERROR_EXIT;
    }

    main_disk = sys_get_main_disk();
    if( main_disk == HOTPLUGIN_DEV_TYPE_NONE )
    {
        /* 模态提示框 : 没有任何存储器 */
        dialog_init.head = DIALOG_HEAD_WARN;
        dialog_init.str_id = g_ebook_main->get_no_disk_string()->str_id;
        dialog_modal( &dialog_init );

        g_ebook_main->set_current_scene( SCENE_EXIT );
        goto ERROR_EXIT;
    }

    /* 场景消息循环 */
    register_sys_dispatcher( _proc_sys_msg );

    while(get_msg(&msg) == true)
    {
        dispatch_msg(&msg);
    }

    unregister_sys_dispatcher();


    ERROR_EXIT:

    /* 场景退出 */
    ret = _scene_mainmenu_deinit( );
    if ( ret == false )
    {
        print_err(" mainmenu scene exit error! \n");
        g_ebook_main->set_current_scene( SCENE_EXIT );
    }

    print_info(" ************  exit  scene_mainmenu  ************* ");
}


/* scene initial function */
static bool _scene_mainmenu_init(void)
{
    bool ret;
    
   
    ret = _load_style_resources();
    if ( ret == false)
    {
        print_err("%s----%d-----\n",__FILE__,__LINE__);
        return false;
    }

    ret = _show_scene_mainmenu();
    if ( ret == false)
    {
        print_err("%s----%d-----\n",__FILE__,__LINE__);

        /* 释放fusion的内存，否则会内存泄漏 */
        fusion_display( NULL);
        return false;
    }

    fusion_set_last_frame( NULL );
    fusion_display( NULL);

    clear_key_msg_queue( -1, -1 );
    
    return true;
}

static bool _load_style_resources(void)
{
    s_scene = g_style_api->load_scene( g_ebook_main->get_style(), MAINMENU_UPRIGHT_SCENE );
    if( s_scene == NULL)
    {
        print_err(" mainmenu load scene failure %s---%d---\n",__FILE__,__LINE__);
        return false;
    }
    else
    {        
        /* 设置应用style场景资源 */
        g_sty_dc->sty_set_app_scene( s_scene );
        
        /* load resource */
        s_reading_string = ( string_resource_t* )g_style_api->get_scene_child( s_scene, STR_READING );
        s_dir_list_string = ( string_resource_t* )g_style_api->get_scene_child( s_scene, STR_DIRECTORY_LIST );
        s_bookmark_list_string = ( string_resource_t* )g_style_api->get_scene_child( s_scene, STR_BOOKMARK_LIST );

        

        if( ( s_reading_string == NULL ) || ( s_dir_list_string == NULL ) 
            || ( s_bookmark_list_string == NULL ) )
        {
            print_err(" mainmenu load style resources failure %s---%d---\n",__FILE__,__LINE__);
            return false;
        }
    }

    return true;
}

static bool _show_scene_mainmenu(void)
{
    bool ret = true;
    rotation_e angle;
    fusion_enter_e fenter;
    fusion_type_e ftype = FUSION_TYPE_COMMON;
    fusion_effect_t eff;

    angle = g_sty_dc->sty_get_view_rotation();
    ret = g_sty_dc->sty_set_screen_rotation( angle );
    if ( ret == false )
    {
        print_err( "sty_set_screen_rotation failure \n" );
        return false;
    }

    /* 要在设置gui宽高，角度之后调用 */
    if ( g_ebook_main->get_last_scene() == SCENE_EXIT )
    {
        fenter = FUSION_FORWARD;
        ftype = FUSION_TYPE_APP_CHANGE;
    }
    else
    {
        fenter = FUSION_BACKWARD;
    }

    fusion_get_config( ftype, fenter, &eff );
    fusion_set_effect( &eff );
    fusion_set_first_frame( NULL );

    /* 创建窗口 */
    if ( ( angle == ROTATION_0 ) ||  ( angle == ROTATION_180 ) )
    {
        s_window = gui_wm_create_window( s_scene->x, s_scene->y, s_scene->height, s_scene->width,
                                    WM_CREATE_SHOW, _proc_gui_msg, 0);
    }
    else
    {
        s_window = gui_wm_create_window( s_scene->x, s_scene->y, s_scene->width, s_scene->height,
                                    WM_CREATE_SHOW, _proc_gui_msg, 0);
    }
    
    ret = _create_widgets();
    if ( ret == false )
    {
        return false;
    }

    gui_wm_set_focus( s_window );

    /* paint all */
    _paint_background();
    headbar_paint( s_headbar );

    menuview_paint( s_menuview );

    return true;
}

static bool _create_widgets(void)
{
    menuitem_t menu_item;

    headbar_init_t init;
    init.type = HEADBAR_TYPE_NORMAL;
    init.str_id_title = g_ebook_main->get_ebook_string()->str_id;
    init.pic_id_head = g_ebook_main->get_headbar_pid();

    s_headbar = headbar_create( &init, _headbar_callback );
    if( s_headbar == NULL )
    {
        print_err("--: %s---%d---\n",__FILE__,__LINE__);
        return false;
    }

    s_menuview = menuview_create( MENUVIEW_MENULIST_TEXT, _menuview_callback );
    if( s_menuview == NULL )
    {
        print_err("");
        return false;
    }
    else
    {
        s_menuview->menu = menu_create();

        if ( g_scene_read->is_reading() == true )
        {
            memset( &menu_item, 0, sizeof( menuitem_t ) );
            menu_item.text_id = s_reading_string->str_id;
            menu_item.callback = (item_cb_t)_proc_item_now_reading;
            menu_append_item( s_menuview->menu, &menu_item );
        }

        memset( &menu_item, 0, sizeof( menuitem_t ) );
        menu_item.text_id = s_dir_list_string->str_id;
        menu_item.callback = (item_cb_t)_proc_item_dir_list;
        menu_append_item( s_menuview->menu, &menu_item );

        memset( &menu_item, 0, sizeof( menuitem_t ) );
        menu_item.text_id = s_bookmark_list_string->str_id;
        menu_item.callback = (item_cb_t)_proc_item_bookmark_list;
        menu_append_item( s_menuview->menu, &menu_item );

        menu_set_cur_index( s_menuview->menu, s_current_index );
    }

    s_msgbox = msgbox_create( _msgbox_callback );

    return true;
}

static void _paint_background(void)
{
    int hdc = gui_dc_get( (unsigned short)s_window );

    g_sty_dc->sty_draw_bitmap( hdc, g_ebook_main->get_background_picture(), 0, 0 );
}

static void _proc_gui_msg(gui_wm_message_t *pmsg)
{
//      print_info( "mainmenu : gui_callback() ...... \n" );

    int top;
    key_event_t *kmsg;

    top = canvas_get_focus(pmsg);

    if ( s_headbar != NULL )
    {
        headbar_proc_gui( s_headbar, pmsg );
    }

    if ( ( s_menuview != NULL )
            && ( s_menuview->canvas_id == top ) )
    {
        menuview_proc_gui( s_menuview, pmsg );
    }

    if ( ( s_volumebar != NULL )
            && ( s_volumebar->canvas_id == top ) )
    {
        volumebar_proc_gui( s_volumebar, pmsg );
    }

    if ( ( s_msgbox != NULL )
            && ( s_msgbox->canvas_id == top ) )
    {
        msgbox_proc_gui( s_msgbox, pmsg );
    }

    if ( pmsg->msgid == WM_MSG_KEY )
    {
        kmsg = (key_event_t *)(pmsg->data.p); /*get key msg*/
        _proc_key( kmsg );
    }

    clear_key_msg_queue(-1, -1);
}

static void _proc_key( key_event_t *msg )
{
    key_event_t *kmsg = msg;
    switch ( kmsg->val | kmsg->type )
    {
        case KEY_VSUB | KEY_TYPE_DOWN:
        case KEY_VADD | KEY_TYPE_DOWN:
        {
            if ( s_volumebar == NULL)
            {
                s_volumebar = volumebar_create(0, _volumebar_callback );
                if ( s_volumebar  == NULL )
                {
                    print_err("volumeber create err!");
                }
                else
                {
                    volumebar_paint( s_volumebar ) ;
                }
            }
            break;
        }

        default:
        {
            break;
        }
    }
}

static int _proc_sys_msg( msg_apps_t *msg )
{
    msg_reply_t t;
    switch(msg->type)
    {
        case MSG_ACTIVE_SYNC:
        {
            /* 回复 process manager 应用创建成功。 */
            t.type = MSG_REPLY_SUCCESS;
            strncpy( t.content, EBOOK_APP ,MSG_CONTENT_SIZE );
            reply_sync_msg(&t);
            break;
        }

        case MSG_GSENSOR_ROTATION_CHANGED:
        {
            print_dbg(" receive  MSG_GSENSOR_ROTATION_CHANGED");

            _scene_rotate();
            break;
        }

        case MSG_MAIN_DISK_CHANGED:
        {
            print_dbg(" receive  MSG_MAIN_DISK_CHANGED");

            exit_msg_loop();
            g_ebook_main->set_current_scene( SCENE_EXIT );
            break;
        }

        case MSG_APP_QUIT:
        {
            print_dbg(" receive  MSG_APP_QUIT");

            exit_msg_loop();
            g_ebook_main->set_current_scene( SCENE_EXIT );
            break;
        }

        default :
        {
            if ( s_msgbox != NULL )
            {
                msgbox_proc_sys( s_msgbox, msg );
            }
        }
    }

    return 0;
}

/* scene release function */
static bool _scene_mainmenu_deinit(void)
{
    headbar_delete(s_headbar);
    s_headbar = NULL;

    if ( s_menuview != NULL )
    {
        if ( s_menuview->menu != NULL )
        {
            menu_delete( s_menuview->menu );
            s_menuview->menu = NULL;
        }

        menuview_delete( s_menuview );
        s_menuview = NULL;
    }

    if ( s_volumebar != NULL )
    {
        volumebar_delete( s_volumebar );
        s_volumebar = NULL;
    }

    if ( s_msgbox != NULL )
    {
        msgbox_delete( s_msgbox );
        s_msgbox = NULL;
    }

    /* 删除窗体 */
    if ( s_window != 0 )
    {
        gui_wm_delete_window( s_window );
    }

    /* 退出场景 */
    g_style_api->free_scene( s_scene );

    return true;
}


static int _proc_item_now_reading( menuview_t* list, menuview_sig_e signal )
{
    switch ( signal )
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            bool ret;
            ret = g_ebook_main->check_file_exists( g_scene_read->get_file_path() );
            if ( ret == false )
            {
                _delete_now_reading_item();
                break;
            }
            
            exit_msg_loop();
            g_ebook_main->set_current_scene( SCENE_READ );

            s_current_index = (unsigned short)menu_get_cur_index( s_menuview->menu );
            break;
        }

        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            exit_msg_loop();
            g_ebook_main->set_current_scene( SCENE_EXIT );
            break;
        }

        default:
        {
            break;
        }
    }

    return 0;
}

static int _proc_item_dir_list( menuview_t* list, menuview_sig_e signal )
{
    switch ( signal )
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            exit_msg_loop();
            g_ebook_main->set_current_scene( SCENE_DIRECTORY_LIST );

            s_current_index = (unsigned short)menu_get_cur_index( s_menuview->menu );
            break;
        }

        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            exit_msg_loop();
            g_ebook_main->set_current_scene( SCENE_EXIT );
            break;
        }

        default:
        {
            break;
        }
    }

    return 0;
}

static int _proc_item_bookmark_list( menuview_t* list, menuview_sig_e signal )
{
    switch ( signal )
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            exit_msg_loop();
            g_ebook_main->set_current_scene( SCENE_BOOKMARK_LIST );

            s_current_index = (unsigned short)menu_get_cur_index( s_menuview->menu );
            break;
        }

        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            exit_msg_loop();
            g_ebook_main->set_current_scene( SCENE_EXIT );
            break;
        }

        default:
        {
            break;
        }
    }

    return 0;
}

static void _scene_rotate(void)
{
    bool ret;
    rotation_e angle;
    fusion_effect_t effect;

    fusion_set_first_frame( NULL );

    angle = g_sty_dc->sty_get_view_rotation();
    ret = g_sty_dc->sty_set_screen_rotation( angle );
    if ( ret == false )
    {
        print_err( "sty_set_screen_rotation failure \n" );
    }

    g_ebook_main->change_common_ui_scene();

    /* 改变窗口大小 */
    if ( ( angle == ROTATION_0 ) ||  ( angle == ROTATION_180 ) )
    {
        gui_wm_set_size( s_window, s_scene->height, s_scene->width );
    }
    else
    {
        gui_wm_set_size( s_window, s_scene->width, s_scene->height );
    }

    g_ebook_main->update_background_picture();
    _paint_background();

    if ( s_headbar != NULL )
    {
        headbar_load_style( s_headbar );
        headbar_paint( s_headbar );
    }

    if ( s_menuview != NULL )
    {
        menuview_load_style( s_menuview, MENUVIEW_MENULIST_TEXT );
        menuview_paint( s_menuview );
    }

    fusion_set_last_frame( NULL );
    fusion_get_config( FUSION_TYPE_ROTATE, FUSION_FORWARD, &effect );
    fusion_display( NULL );
}

static void _delete_now_reading_item( void )
{
    g_scene_read->clear_current_reading_file();

    menu_remove_item( s_menuview->menu, 0 );
    menuview_paint( s_menuview );
}

static void _headbar_callback(headbar_t * headbar, widget_sig_headbar_e signal )
{
}


static void _menuview_callback( menuview_t* list, menuview_sig_e signal )
{
}

static void _volumebar_callback(volumebar_t *data, volumebar_sig_e signal)
{
    switch( signal )
    {
        case WIDGET_SIG_VOLUMEBAR_QUIT :
        {
            if ( s_volumebar != NULL )
            {
                volumebar_delete( s_volumebar );
                s_volumebar = NULL;
            }

            break;
        }

        default :
        {
            break;
        }
    }
}

static void _msgbox_callback(msgbox_t * msgbox, msgbox_sig_e signal )
{
}

scene_mainmenu_t* create_scene_mainmenu(void)
{
    return &s_scene_mainmenu_api;
}

