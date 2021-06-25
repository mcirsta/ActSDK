/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : ebook_scene_bookmark.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    zhaowenjie    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     ebook_scene_bookmark.c
* \brief    bookmark list scene
* \author   zhaowenjie
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#include "ebook_scene_bookmark.h"
#include "ebook.h"
#include "ebook_sty.h"

#define EBOOK_BOOKMARK "ebook_bookmark"
#define MAX_BOOKMARK_NUM 32 /* spec */

scene_bookmark_t* g_scene_bookmark;

static void* s_bookmark_list = NULL;

/* config 可配置 */
static unsigned int s_max_bookmark_num = MAX_BOOKMARK_NUM;

static scene_t* s_scene;

static int s_window = 0;
static headbar_t* s_headbar = NULL;
static menuview_t* s_menuview = NULL;
static int s_current_offset = 0;
static unsigned short s_current_index = 0;
static volumebar_t* s_volumebar = NULL;
static msgbox_t* s_msgbox = NULL;

static dialog_t* s_dialog = NULL;
static string_resource_t* s_no_bookmark_string = NULL;


bool open_bookmark(void);
add_bookmark_result_e add_a_bookmark( char* path, char* name, unsigned int page, 
                                        unsigned int point, unsigned int font );
static bool _delete_a_bookmark( int current_index );
bool quit_bookmark(void);

void enter_scene_bookmark(void);
static bool _scene_bookmark_init(void);
static bool _load_style_resources(void);
static bool _show_scene_bookmark(void);
static void _paint_background(void);
static bool _create_widgets(void);

static int _proc_menu_item( menuview_t *data, menuview_sig_e signal );
static void _proc_key( key_event_t *msg );

static void _proc_gui_msg(gui_wm_message_t *pmsg);
static int _proc_sys_msg( msg_apps_t *msg );

static void _try_delete_bookmark(void);
static void _deleting_bookmark(void);
static void _delete_bookmark_item(void);

static void _try_delete_dialog_callback( dialog_t* dialog, widget_sig_dialog_e signal );
static void _deleting_dialog_callback( dialog_t* dialog, widget_sig_dialog_e signal );

static void _create_no_bookmark_dialog(void);

static void _no_bookmark_dialog_callback( dialog_t* dialog, widget_sig_dialog_e signal );

static void _scene_rotate(void);

static void _volumebar_callback(volumebar_t *data, volumebar_sig_e signal);
static void _headbar_callback(headbar_t * headbar, widget_sig_headbar_e signal );
static void _menuview_callback( menuview_t *list, menuview_sig_e signal );
static void _msgbox_callback(msgbox_t * msgbox, msgbox_sig_e signal );

static bool _scene_bookmark_deinit(void);

static scene_bookmark_t s_scene_bookmark_api = 
{
    open_bookmark,
    add_a_bookmark,
    quit_bookmark,
    
    enter_scene_bookmark
};


/* scene entry function */
void enter_scene_bookmark(void)
{
    bool ret;
    msg_apps_t  msg;
    hotplugin_dev_type_e main_disk ; 
    dialog_init_t dialog_init;

    print_info(" ************   enter  scene_bookmark  ************* "); 
    
    /* 场景初始化 */
    ret = _scene_bookmark_init( );
    if( ret == false )
    {
        print_err("bookmark scene init error! \n");
        g_ebook_main->go_back_last_scene();

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

    while( get_msg(&msg) == true )
    {
        dispatch_msg(&msg);
    }

    unregister_sys_dispatcher();

    ERROR_EXIT:

    /* 场景退出 */
    ret = _scene_bookmark_deinit( );
    if ( ret == false )
    {
        printf(" mainmenu scene exit error! \n");
        g_ebook_main->go_back_last_scene();
    }

    print_info(" ************  exit  scene_bookmark  ************* ");
}


/* scene initial function */
static bool _scene_bookmark_init(void)
{
    bool ret;
    fusion_enter_e fenter;
    fusion_effect_t eff;

    ret = open_bookmark();
    if ( ret == false )
    {
        return false;
    }

    if ( g_ebook_main->get_last_scene() == SCENE_MAIN_MENU )
    {
        fenter = FUSION_FORWARD;
    }
    else
    {
        fenter = FUSION_BACKWARD;
    }

    fusion_get_config( FUSION_TYPE_COMMON, fenter, &eff );
    fusion_set_effect(&eff);
    fusion_set_first_frame( NULL );

    ret = _load_style_resources();
    if ( ret == false)
    {
        print_err("load_style_resources fail !"); 
        return false;
    }
    
    ret = _show_scene_bookmark();
    if ( ret == false)
    {
        print_err(" scene_bookmark_init \n" ); 
        return false;
    }

    fusion_set_last_frame( NULL );
    fusion_display( NULL);

    clear_key_msg_queue( -1, -1 );
        
    return true;
}

static bool _load_style_resources(void)
{
    s_scene = g_style_api->load_scene( g_ebook_main->get_style(), BOOKMARK_UPRIGHT_SCENE );
    if( s_scene == NULL)
    {
        print_err(" bookmark load scene failure %s---%d---\n",__FILE__,__LINE__); 
        return false;
    }
    else
    {
        /* 设置应用style场景资源 */
        g_sty_dc->sty_set_app_scene( s_scene );
    
        /* load resource */
        s_no_bookmark_string = ( string_resource_t* )g_style_api->get_scene_child( s_scene, STR_NO_BOOKMARK );        
        if( s_no_bookmark_string == NULL )
        {
            print_err(" scene bookmark load style resources failure %s---%d---\n",__FILE__,__LINE__); 
            return false;
        }
    }
    
    return true;
}

bool open_bookmark(void)
{
    int ret;
    plist_trivefield_t trive_field;

    if ( s_bookmark_list != NULL )
    {
        return true;
    }
            
    trive_field.num = 5; 
    trive_field.nfield[0] = FULLPATH;   /* 文件全路径 */
    trive_field.nfield[1] = TITLE;   /* 文件全路径 */
    //trive_field.nfield[2] = BITRATE;      /* 实际存放 "页码" */
    trive_field.nfield[2] = CHANNEL;      /* 实际存放 "页码" */
    trive_field.nfield[3] = RATING;     /* 实际存放 "页的起始点" */
    trive_field.nfield[4] = MARK;     /* 实际存放 "字体大小" */

    /*设置数据库路径*/
    plist_set_rootdir(sys_get_main_disk_dir(DIR_SYSTEM_PLAYLIST));

    ret = plist_init();
    if ( ret != 0 )
    {
        print_err("\n plist_init : errno is %d",errno);
        return false;
    }
    
    s_bookmark_list = plist_open( EBOOK_BOOKMARK, TXTRESERVE1, &trive_field );
    if( s_bookmark_list == NULL )
    {
        print_err("\n plist_open : errno is %d",errno);
        return false;
    }

    return true;
}

static bool _show_scene_bookmark(void)
{   
    bool ret = true;
    rotation_e angle;

    angle = g_sty_dc->sty_get_view_rotation();
    ret = g_sty_dc->sty_set_screen_rotation( angle );
    if ( ret == false )
    {
        print_err( "sty_set_screen_rotation failure \n" );
        return false;
    }

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
            
    gui_wm_set_focus( s_window );
    
    ret = _create_widgets();
    if ( ret == false )
    {
        return false;
    }

    /* paint all */
    _paint_background();
    headbar_paint( s_headbar );
    menuview_paint( s_menuview );
    
    return true;
}

static void _paint_background(void)
{
    int hdc = gui_dc_get( (unsigned short)s_window );

    g_sty_dc->sty_draw_bitmap( hdc, g_ebook_main->get_background_picture(), 0, 0 );
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

    s_msgbox = msgbox_create( _msgbox_callback );
    if ( s_msgbox == NULL )
    {
        print_err(" msgbox_create NULL .......");
        return false;
    }
    
    s_menuview = menuview_create( MENUVIEW_PICLIST_EBOOK, _menuview_callback );
    if( s_menuview == NULL )
    {
        print_err("--: %s---%d---\n",__FILE__,__LINE__); 
        return false;
    }
    else
    {
        int sum;
                
        int i;
        plist_detail_info_t bookmark;
        char* string1;
        char* string2;

        sum = plist_get_item_num( s_bookmark_list );
        if ( sum == 0 )
        {
            _create_no_bookmark_dialog();
            return true;
        }
        
        s_menuview->menu = menu_create();
        memset( &menu_item, 0 , sizeof( menuitem_t ) );
        menu_item.callback = (item_cb_t)_proc_menu_item;

        /* 添加所有的标签 */
        for ( i = 0 ; i < sum; i++ )        
        {            
            string1 = (char*)malloc( PATH_MAX );
            string2 = (char*)malloc( PATH_MAX );

            if ( ( string1 == NULL ) || ( string2 == NULL ) )
            {
                continue;
            }
            
            memset( string1, 0, PATH_MAX );
            memset( string2, 0, PATH_MAX );
        
            plist_get_items_to_mallocbuf( s_bookmark_list, &bookmark,
                                i, 1, false );

            //print_info("bookmark: path = %s, name = %s, page = %d, point = %d",
            //            bookmark.fullpath, bookmark.title, bookmark.bitrate, bookmark.rating );
            print_info("bookmark: path = %s, name = %s, page = %d, point = %d",
                        bookmark.fullpath, bookmark.title, bookmark.channels, bookmark.rating );

            strcpy( string1, bookmark.title );            
            sprintf( string2, "page:%d", bookmark.channels );

            menu_item.text = string1;
            menu_item.info2 = string2;
            menu_append_item( s_menuview->menu, &menu_item );

            if ( g_ebook_main->get_last_scene() == SCENE_MAIN_MENU )
            {
                s_current_offset = 0;
                s_current_index = 0;                
            }
            menu_set_offset( s_menuview->menu, s_current_offset );
            menu_set_cur_index( s_menuview->menu, (int)s_current_index );
        }
    }

    return true;
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

    if ( ( s_dialog != NULL )
            && ( s_dialog->canvas_id == top ) )
    {
        dialog_proc_gui( s_dialog, pmsg );
        return;
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

static int _proc_sys_msg( msg_apps_t *msg )
{
    print_dbg( " receive msg type = %d ", msg->type );

    switch(msg->type)
    {
        case MSG_GSENSOR_ROTATION_CHANGED:
        {
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

static void _create_no_bookmark_dialog(void)
{
    dialog_init_t init;

    init.head = DIALOG_HEAD_WARN;
    init.str_id = s_no_bookmark_string->str_id;

    s_dialog = dialog_create( &init, _no_bookmark_dialog_callback );
    if ( s_dialog == NULL )
    {
        print_err(" create dialog error ! ");
        return;
    }
                
    dialog_paint( s_dialog );
}

/* scene release function */
static bool _scene_bookmark_deinit(void)
{
    int i;
    int sum = 0;
    menuitem_t* item = NULL;

    headbar_delete( s_headbar );
    s_headbar = NULL;

    if ( s_menuview != NULL )
    {
        if ( s_menuview->menu != NULL )
        {
            sum = s_menuview->menu->total;
            for ( i = 0; i < sum; i++ )
            {
                item = menu_get_item( s_menuview->menu, i );
                if ( item != NULL )
                {
                    free( item->text );
                    free( item->info2 );
                    item->text = NULL;
                    item->info2 = NULL;
                }
                
            }
        }
    }
        
    if ( ( s_menuview != NULL ) && ( s_menuview->menu != NULL ) )
    {
        menu_delete( s_menuview->menu );
        s_menuview->menu = NULL;
    }

    if ( s_menuview != NULL )
    {
        menuview_delete( s_menuview );
        s_menuview = NULL;
    }

    if ( s_dialog != NULL )
    {
        dialog_delete( s_dialog );
        s_dialog = NULL;
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

    //删除窗体
    gui_wm_delete_window( s_window );

    /* 释放 scene 资源 */
    g_style_api->free_scene( s_scene );

//      print_info(" scene_bookmark : gui_wm_delete_window( s_window ); ");

    return true;
}


add_bookmark_result_e add_a_bookmark( char* path, char* name, unsigned int page, unsigned int point, unsigned int font )
{
    int ret;
    unsigned int bookmark_sum;
    plist_detail_info_t info;

    if ( s_bookmark_list == NULL )
    {
        bool result;
        result = open_bookmark();
        if ( result == false )
        {
            return false;
        }
    }

    bookmark_sum = (unsigned int)plist_get_item_num( s_bookmark_list );
    if ( bookmark_sum >= s_max_bookmark_num )
    {
        return BOOKMARK_FULL;
    }

    memset( &info, 0, sizeof( plist_detail_info_t ) );
    info.fullpath = path;
    info.title = name;

    info.channels = (int)page;
    info.rating = (int)point;
    info.markinfo.time = font;
    
    ret = plist_add_item( s_bookmark_list, &info );

    if ( ret == -1 )
    {
        print_err("plist_add_item fail : errno = %d \n", errno );
        return ADD_BOOKMARK_FAIL;
    }

    print_info( "add a bookmark : path = %s, name = %s, page = %d, point = %d, font = %d"
                , path, name, page, point, font );

    return ADD_BOOKMARK_SUCCESS;
}

static bool _delete_a_bookmark( int bindex )
{
    int ret;
    ret = plist_del_items( s_bookmark_list, bindex, 1 );
    if ( ret == -1 )
    {
        print_err("plist_del_items fail : errno = %s \n", strerror(errno) );
        return false;
    }

    return true;
}

bool quit_bookmark(void)
{
    int ret;

    if ( s_bookmark_list == NULL )
    {
        return true;
    }

    plist_close( s_bookmark_list );
    
    ret = plist_quit( );
    if ( ret == -1 )
    {
        print_err("plist_quit fail : errno = %d \n", errno );
        return false;
    }

    s_bookmark_list = NULL;

    return true;
}

static int _proc_menu_item( menuview_t* data, menuview_sig_e signal )
{       
    int current_index;
    plist_detail_info_t bookmark;
    
    switch ( signal ) 
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            bool ret;
            
            current_index = menu_get_cur_index( s_menuview->menu );
    
            plist_get_items_to_mallocbuf( s_bookmark_list, &bookmark,
                                current_index, 1, false );
    
            print_info("bookmark: path = %s, name = %s, page = %d, point = %d, font = %d",
                        bookmark.fullpath, bookmark.title, 
                        bookmark.channels, bookmark.rating, bookmark.markinfo.time );

            ret = g_ebook_main->check_file_exists( bookmark.fullpath );
            if ( ret == false )
            {
                _delete_bookmark_item();
                break;
            }
    
            g_scene_read->set_bookmark( &bookmark );
            exit_msg_loop(); 
            g_ebook_main->set_current_scene( SCENE_READ );

            s_current_offset = menu_get_offset( s_menuview->menu );
            s_current_index = (unsigned short)menu_get_cur_index( s_menuview->menu );
                        
            break;
        }

        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            exit_msg_loop(); 
            g_ebook_main->set_current_scene( SCENE_MAIN_MENU );
            break;
        }
    
        default:
        {
            break;
        }
    }

    return 0;
}

static void _proc_key( key_event_t *msg )
{
    key_event_t *kmsg;
    kmsg = msg;
    switch ( kmsg->val | kmsg->type )
    {
        case KEY_MENU | KEY_TYPE_DOWN:
        {
            _try_delete_bookmark();
            break;
        }

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

        case KEY_DELETE | KEY_TYPE_SHORT_UP:
        {
            _try_delete_bookmark();
            break;
        }

        default:
        {
            break;
        }
    }
}

static void _try_delete_bookmark(void)
{
    int sum = 0;
    dialog_init_t init;

    sum = plist_get_item_num( s_bookmark_list );
    if ( sum == 0 )
    {
        return;
    }
    
    init.head = DIALOG_HEAD_QUERY;
    init.str_id = g_ebook_main->get_sure_delete_string()->str_id;
    init.def_sel=WIDGET_SIG_DIALOG_CANCAL;

    s_dialog = dialog_create( &init, _try_delete_dialog_callback );
    if ( s_dialog == NULL )
    {
        print_err(" create dialog error ! ");
        return;
    }
    
    dialog_paint( s_dialog );
}

static void _deleting_bookmark(void)
{
    dialog_init_t init;
    init.head = DIALOG_HEAD_INFO;
    init.str_id = g_ebook_main->get_deleting_string()->str_id;

    s_dialog = dialog_create( &init, _deleting_dialog_callback );
    if ( s_dialog == NULL )
    {
        print_err(" create dialog error ! ");
        return;
    }
    
    dialog_paint( s_dialog );
    gui_screen_update(); /* 马上画出dialog */

    _delete_bookmark_item();
}

static void _delete_bookmark_item(void)
{
    int current_index;
    menuitem_t* item;

    current_index = menu_get_cur_index( s_menuview->menu );
    
    item = menu_get_item( s_menuview->menu, current_index );
    if ( item != NULL )
    {
        free( item->text );
        free( item->info2 );
        item->text = NULL;
        item->info2 = NULL;
    }
    
    menu_remove_item( s_menuview->menu, current_index );
    menuview_paint( s_menuview );
    
    _delete_a_bookmark( current_index );
}

static void _no_bookmark_dialog_callback( dialog_t* dialog, widget_sig_dialog_e signal )
{
    switch( signal )
    {
        case WIDGET_SIG_DIALOG_CANCAL :
        {
            dialog_delete( s_dialog );
            s_dialog = NULL;

            /* back to main menu scene */
//              exit_msg_loop(); 
//              g_ebook_main->set_current_scene( SCENE_MAIN_MENU );
            break; 
        }
        
        default :
        {
            break;
        }
    }
}

static void _try_delete_dialog_callback( dialog_t* dialog, widget_sig_dialog_e signal )
{
    switch( signal )
    {
        case WIDGET_SIG_DIALOG_OK :
        {
            dialog_delete( s_dialog );
            s_dialog = NULL;
            
            _deleting_bookmark();
            break;
        }

        case WIDGET_SIG_DIALOG_CANCAL :
        {
            dialog_delete( s_dialog );
            s_dialog = NULL;
            break; 
        }

        default :
        {
            break;
        }
    }
}

static void _deleting_dialog_callback( dialog_t* dialog, widget_sig_dialog_e signal )
{
    int sum;

    switch( signal )
    {
        case WIDGET_SIG_DIALOG_CANCAL :
        {
            dialog_delete( s_dialog );
            s_dialog = NULL;

            sum = s_menuview->menu->total;
            if ( sum == 0 )
            {
                _create_no_bookmark_dialog();
            }
            break; 
        }

        default :
        {
            break;
        }
    }
}

static void _scene_rotate(void)
{
    bool ret;
    rotation_e angle;
    fusion_effect_t effect;

    fusion_get_config( FUSION_TYPE_COMMON, FUSION_FORWARD, &effect );
    fusion_set_effect( &effect );
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
        menuview_load_style( s_menuview, MENUVIEW_PICLIST_EBOOK );
        menuview_paint( s_menuview );
    }

    fusion_set_last_frame( NULL );
    fusion_display( NULL );
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

static void _headbar_callback(headbar_t * headbar, widget_sig_headbar_e signal )
{
}


static void _menuview_callback( menuview_t *list, menuview_sig_e signal )
{
    switch ( signal ) 
    {
        
        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            exit_msg_loop(); 
            g_ebook_main->set_current_scene( SCENE_MAIN_MENU );
            break;
        }
    
        default:
        {
            break;
        }
    }
}

static void _msgbox_callback(msgbox_t * msgbox, msgbox_sig_e signal )
{    
}

scene_bookmark_t* create_scene_bookmark(void)
{
    return &s_scene_bookmark_api;
}

