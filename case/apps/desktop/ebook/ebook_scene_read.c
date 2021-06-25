/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : ebook_scene_read.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *    zhaowenjie    2009-2-25          v1.0               build this file
 ********************************************************************************/
/*!
* \file     ebook_scene_read.c
* \brief    read scene
* \author   zhaowenjie
*
* \version 1.0
* \date  2009/02/25
*******************************************************************************/


#include "ebook_book_buffer.h"
#include "ebook_scene_read_ui.h"
#include "ebook_read_option.h"
#include "ebook_count_page.h"
#include "ebook_scene_bookmark.h"
#include "fusion.h"

//#define SHOW_ALL_CHAR

#ifdef SHOW_ASCII_CHAR

#define PRINTF printf

#else

#define  PRINTF(...) do{}while(0)

#endif

#define TOTAL_PAGE_UPDATE_SPEED 300

#define GUI_TTF_FONT "attfv1.ttf"

#define SMALL_FONT_SIZE 16
#define MEDIUM_FONT_SIZE 20
#define LARGE_FONT_SIZE 24

#define CUR_TOTAL_PAGE_BUFFER_LENGTH 50

#define SKIP_TO_STEP 5

#define AUTO_PLAY_MAX_GAP 30
#define AUTO_PLAY_MIN_GAP 5
#define AUTO_PLAY_STEP 5

#define FONT_WIDTH_BUFFER_WIDTH 64 * 1024 /* 64k */

static unsigned int s_auto_play_gap = ( 5 * 1000 ); /* 5 sec */
static unsigned int s_quick_move_gap = 100; /* 100 ms */

static int s_update_page_timer = NULL_TIMER;
static int s_auto_read_timer = NULL_TIMER;
static int s_quick_move_timer = NULL_TIMER;
static bool s_quick_move_next = true;

static font_layout_t s_font_layout[] =
{
    { 0, 0, 0, SMALL_FONT_SIZE, SMALL_FONT_LINE_HEIGHT, SMALL_FONT_MAX_LINE },
    { 0, 0, 0, MEDIUM_FONT_SIZE, MEDIUM_FONT_LINE_HEIGHT, MEDIUM_FONT_MAX_LINE },
    { 0, 0, 0, LARGE_FONT_SIZE, LARGE_FONT_LINE_HEIGHT, LARGE_FONT_MAX_LINE }
};

/* 字体的类型，config 中 ebook_default_font_color 可配置 */
static font_type_e s_font_type = SMALL_FONT;
static font_layout_t s_current_font_layout;

static book_buffer_t* s_book_buffer;
static scene_read_ui_t* s_scene_read_ui;
static count_page_t* s_count_page;

static int s_window = 0;
static int s_hdc;
static int s_ttf_fontdesc;
static headbar_t* s_headbar = NULL;
static menuview_t* s_menuview = NULL;
static volumebar_t* s_volumebar = NULL;
static adjust_t* s_adjust = NULL;
static dialog_t* s_dialog = NULL;
static msgbox_t* s_msgbox = NULL;

static string_resource_t* s_skip_to_string = NULL;
static unsigned int s_skip_to_step = SKIP_TO_STEP;
static unsigned int s_skip_percent = SKIP_TO_STEP;
static string_resource_t* s_bookmark_full_string = NULL;
static string_resource_t* s_add_bookmark_fail_string = NULL;

static unsigned int s_auto_play_max_gap = AUTO_PLAY_MAX_GAP;
static unsigned int s_auto_play_min_gap = AUTO_PLAY_MIN_GAP;
static unsigned int s_auto_play_step = AUTO_PLAY_STEP;
static string_resource_t* s_auto_play_gap_string = NULL;
static string_resource_t* s_second_string = NULL;

static char s_file_name[ PATH_MAX ];
static char s_file_full_path[ PATH_MAX ];

static unsigned char s_cur_total_page[ CUR_TOTAL_PAGE_BUFFER_LENGTH ];
static bool s_new_book = true;

static scene_t* s_scene;
static control_mode_e s_control_mode = HAND_READING;

/* 临时存放要显示的字符 */
static unsigned char s_show_buffer[ SHOW_BUFFER_SIZE ];

static int s_current_page_start_point = 0;
static unsigned int s_current_page = 0;

static unsigned char s_ascii_width[256];
static char s_font_width_buffer[ FONT_WIDTH_BUFFER_WIDTH ]; /* 64k */

/* 字体的颜色值，config 中 ebook_default_font 可配置 */
static unsigned int s_font_color = 0;

/* 语言的模式，config 中 ebook_language_mode 可配置 */
static unsigned int s_language_mode = MLANG_EN_GB; /* 当前内码语言 */

static unsigned int s_show_lines = 0; /* 需要显示的行数 */

static scene_read_option_t* s_read_option = NULL;

/* 字体颜色的总数 */
static unsigned int s_font_color_num;

static fusion_mode_e s_fusion_mode = FUSION_NORMAL; /* 翻页的特效 */
static trans_direct_t s_fusion_direction;
static sty_rect_t s_fusion_rect;

void enter_scene_read(void);
static bool _scene_read_init(void);
static bool _load_style_resources(void);
static bool _show_scene_read(void);
static void _paint_background(void);
static bool _create_widgets(void);

bool _start_count_page(void);
void _stop_count_page(void);

static void _proc_gui_msg(gui_wm_message_t *pmsg);
static void _proc_key( key_event_t *msg );
static bool _scene_read_deinit(void);

static bool _show_book(void);

static bool _show_current_page(void);
static bool _repaint_current_page(void);

static bool _try_show_next_page(void);
static bool _show_next_page( bool fill_buffer, bool paint, bool need_fusion );
static bool _try_show_prev_page(void);
static bool _show_prev_page( bool fill_buffer, bool paint );
static bool _find_prev_page_point(void);

static bool _paint_current_page(void);

static void _show_next_multi_lines( bool fill_buffer );
static bool _get_a_line( bool fill_buffer );
static bool _get_a_ascii_line( bool fill_buffer );
static bool _get_a_utf8_line( bool fill_buffer );
static bool _get_a_unicode_line( bool fill_buffer );
static bool _end_of_file( unsigned int show_offset );

bool is_reading(void);
void set_file_path( const char* path, const char* name );
void set_file_name( const char* name );
void set_file_full_path( const char* full_path );
char* get_file_path(void);
char* get_file_name(void);
file_type_e get_file_type(void);

void set_font_layout( unsigned short findex, int x, int y, int width, int size, int line_height, int max_line );

void set_font_type( font_type_e type );
font_type_e get_font_type(void);
void change_font_type( font_type_e type );

void set_ui_font_colors( unsigned int findex, int color, int bg_color );
void set_font_color( unsigned int color );
unsigned int get_font_color(void);
void change_font_color( unsigned int color );

static void _get_ascii_width(void);
static char _get_char_width_ext( unsigned short char_add, int encoding );


void set_control_mode( control_mode_e mode );
static void _change_control_mode(void);

void set_auto_play_gap( unsigned int gap );
unsigned int get_auto_play_gap( void );
void _change_auto_play_gap( unsigned int gap );
void creat_skip_to_adjust(void);
void creat_auto_play_gap_adjust(void);
static void _skip_to_percent( unsigned int percent );

void try_delete_current_file(void);
static void _try_delete_file_dialog_callback( dialog_t* dialog, widget_sig_dialog_e signal );
static bool _deleting_current_file(void);
static void _delete_file_dialog_callback( dialog_t* dialog, widget_sig_dialog_e signal );
static bool _delete_current_file(void);
void clear_current_reading_file(void);

void update_cur_total_page(void);
static int _proc_update_page_timer( void * param );
static int _proc_auto_read_timer( void * param );
static int _proc_quick_move_timer( void * param );

void kill_update_page_timer(void);

void set_current_page_num( unsigned int page );
int get_current_page_start_point(void);
void set_current_page_start_point( int point );
void revise_current_page_start_point( int point );

void add_current_page_to_bookmark(void);
void set_bookmark( plist_detail_info_t* bookmark );

static bool _check_file_same_length( char* first, char* second );

void set_language_mode( unsigned int language );
void change_language_mode( unsigned int language );
unsigned int get_language_mode(void);

void clear_option_menuview( void );
static bool count_page_finished(void);
static bool close_reading_book(void);

void restart_auto_reading_timer(void);

static int _proc_sys_msg( msg_apps_t* msg );
static void _auto_play_gap_adjust_cb( adjust_t *pAdjust, adjust_signal_e signal );
static void _skip_to_adjust_cb( adjust_t *pAdjust, adjust_signal_e signal );
static void _headbar_callback(headbar_t * headbar, widget_sig_headbar_e signal );
static void _volumebar_callback(volumebar_t *data, volumebar_sig_e signal);
static void _msgbox_callback(msgbox_t * msgbox, msgbox_sig_e signal );

void set_font_color_num( unsigned int num );
unsigned int get_font_color_num(void);
void set_ui_use_color_background( bool use );
void set_fusion_mode( fusion_mode_e mode );
fusion_mode_e get_fusion_mode(void);
void set_fusion_rect( short x, short y, short width, short height );
static void _scene_rotate(void);

void set_option_has_change_size_item(void);

static scene_read_t s_scene_read_api =
{
    enter_scene_read,
    clear_current_reading_file,
    close_reading_book,

    is_reading,
    set_file_path,
    set_file_name,
    set_file_full_path,
    get_file_path,
    get_file_name,
    get_file_type,

    set_font_layout,
    set_font_type,
    get_font_type,
    change_font_type,

    set_font_color_num,
    get_font_color_num,
    set_ui_font_colors,
    set_ui_use_color_background,

    set_font_color,
    get_font_color,
    change_font_color,

    set_control_mode,
    set_auto_play_gap,
    get_auto_play_gap,
    set_language_mode,
    change_language_mode,
    get_language_mode,

    creat_skip_to_adjust,
    creat_auto_play_gap_adjust,
    try_delete_current_file,

    set_current_page_num,
    get_current_page_start_point,
    set_current_page_start_point,
    revise_current_page_start_point,

    clear_option_menuview,
    restart_auto_reading_timer,

    add_current_page_to_bookmark,
    set_bookmark,

    update_cur_total_page,
    kill_update_page_timer,
    count_page_finished,

    set_fusion_mode,
    get_fusion_mode,
    set_fusion_rect,
    set_option_has_change_size_item
};

/* scene entry function */
void enter_scene_read(void)
{
    bool ret;
    msg_apps_t  msg;
    fusion_enter_e fenter;
    fusion_type_e ftype = FUSION_TYPE_COMMON;
    fusion_effect_t eff;
    hotplugin_dev_type_e main_disk ;
    dialog_init_t dialog_init;

    main_disk = sys_get_main_disk();
    if( main_disk == HOTPLUGIN_DEV_TYPE_NONE )
    {
        /* 模态提示框 : 没有任何存储器 */
        dialog_init.head = DIALOG_HEAD_WARN;
        dialog_init.str_id = g_ebook_main->get_no_disk_string()->str_id;
        dialog_modal( &dialog_init );
    
        return;
    }

    print_dbg(" ************   enter  scene_read  ************* ");

    /* 场景初始化 */

    if ( g_ebook_main->get_last_scene() == SCENE_EXIT )
    {
        /* 从别的应用直接进入阅读场景 */
        ftype = FUSION_TYPE_APP_CHANGE;
    }

    fenter = FUSION_FORWARD;       
    fusion_get_config( ftype, fenter, &eff );
    fusion_set_effect(&eff);
    fusion_set_first_frame( NULL );
    
    ret = _scene_read_init();
    if( ret == false )
    {
        print_err("scene read init error! \n");
        
        /* 释放fusion的内存，否则会内存泄漏 */
        fusion_display( NULL);

        g_ebook_main->go_back_last_scene();
        
        goto ERROR_EXIT;
    }

    
    /* 重新打开书 */
    ret = _show_book();
    if ( ret == false )
    {                       
        /* 释放fusion的内存，否则会内存泄漏 */
        fusion_display( NULL);
    
        g_ebook_main->go_back_last_scene();
        goto ERROR_EXIT;
    }

    ret = _start_count_page();
    if ( ret == false )
    {
        print_err(" start_count_page failed ! \n");
     
        /* 释放fusion的内存，否则会内存泄漏 */
        fusion_display( NULL);
        
        g_ebook_main->go_back_last_scene();
        goto ERROR_EXIT;
    }
    
    fusion_set_last_frame( NULL );    
    fusion_display( NULL);

    clear_key_msg_queue( -1, -1 );

    /* 场景消息循环 */
    register_sys_dispatcher( _proc_sys_msg );

    while( get_msg(&msg) == true )
    {
        dispatch_msg(&msg);
    }

    unregister_sys_dispatcher();

    ERROR_EXIT:

    /* 场景退出 */
    ret = _scene_read_deinit();
    if ( ret == false )
    {
        print_err("scene read exit error! \n");
        g_ebook_main->set_current_scene( SCENE_EXIT );
        return;
    }

    print_dbg(" ************  exit  scene_read  ************* ");
}

static bool _scene_read_init(void)
{
    bool ret = true;

    ret = _load_style_resources();
    if ( ret == false)
    {
        print_err("%s----%d-----\n",__FILE__,__LINE__);
        return false;
    }

    ret = _show_scene_read();
    if ( ret == false)
    {
        print_err("%s----%d-----\n",__FILE__,__LINE__);
        return false;
    }

    set_control_mode( s_control_mode );
    _get_ascii_width();

    /* 禁止屏保和省电关机 */
    sys_forbid_screensaver( true );
    sys_forbid_auto_standby( true );

    if ( s_update_page_timer == NULL_TIMER )
    {
        s_update_page_timer = set_timer( TOTAL_PAGE_UPDATE_SPEED, _proc_update_page_timer, NULL );
    }
    
    return true;
}

static bool _load_style_resources(void)
{
    bool ret;
    resgroup_resource_t* read_resgroup;
    rotation_e angle;

    angle = g_sty_dc->sty_get_view_rotation();
    ret = g_sty_dc->sty_set_screen_rotation( angle );
    if ( ret == false )
    {
        print_err( "sty_set_screen_rotation failure \n" );
        return false;
    }

    if ( ( angle == ROTATION_0 ) ||  ( angle == ROTATION_180 ) )
    {
        s_scene = g_style_api->load_scene( g_ebook_main->get_style(), READ_SIDELONG_SCENE );    
    }
    else
    {
        s_scene = g_style_api->load_scene( g_ebook_main->get_style(), READ_UPRIGHT_SCENE );
    }    
    if( s_scene == NULL)
    {
        print_err(" read scene load scene failure\n");
        return false;
    }

    read_resgroup = (resgroup_resource_t*)g_style_api->get_scene_child( s_scene, OPTION_RESGROUP );
    if( read_resgroup == NULL)
    {
        print_err(" read scene load option resgroup failure ");
        return false;
    }

    s_skip_to_string = ( string_resource_t* )g_style_api->get_resgroup_child( read_resgroup, STR_EB_SKIP );
    s_auto_play_gap_string = ( string_resource_t* )g_style_api->get_resgroup_child( read_resgroup, STR_EB_AUTOT );
    s_second_string = ( string_resource_t* )g_style_api->get_resgroup_child( read_resgroup, STR_SEC );
    s_bookmark_full_string = ( string_resource_t* )g_style_api->get_scene_child( s_scene, STR_BOOKMARK_FULL );
    s_add_bookmark_fail_string = ( string_resource_t* )g_style_api->get_scene_child( s_scene, STR_ADD_BOOKMARK_FAIL );
    if( ( s_skip_to_string == NULL ) || ( s_auto_play_gap_string == NULL ) ||
        ( s_second_string == NULL ) || ( s_bookmark_full_string == NULL ) ||
        ( s_add_bookmark_fail_string == NULL ) )
    {
        print_err(" read scene load strings error ");
        return false;
    }

    /* 设置应用style场景资源 */
    g_sty_dc->sty_set_app_scene( s_scene );

    ret = s_scene_read_ui->load_style_resource( s_scene );
    if ( ret == false )
    {
        print_err( "s_scene_read_ui->load_style_resource( s_scene ); failure \n" );
        return false;
    }

    return true;
}

static bool _show_scene_read(void)
{
    bool ret = true;
    
    /* 创建窗口 */
    s_window = gui_wm_create_window( s_scene->x, s_scene->y, s_scene->width, s_scene->height,
                                    WM_CREATE_SHOW, _proc_gui_msg, 0);
    
    gui_wm_set_focus( s_window );

    s_hdc = gui_dc_get( (unsigned short)s_window );

    s_scene_read_ui->set_hdc( s_hdc );
    s_count_page->set_hdc( s_hdc );

    gui_dc_set_text_mode( s_hdc, GUI_DRAW_MODE_TRANS );

    set_font_type( s_font_type );
    set_font_color( s_font_color );

    ret = _create_widgets();
    if ( ret == false )
    {
        return false;
    }

    _paint_background();
    s_scene_read_ui->paint_initial_ui( s_control_mode , s_file_name );

    headbar_paint( s_headbar );

    return true;
}

static void _paint_background(void)
{
    int hdc = gui_dc_get( (unsigned short)s_window );

    g_sty_dc->sty_draw_bitmap( hdc, g_ebook_main->get_background_picture(), 0, 0 );
}

static bool _create_widgets(void)
{
    headbar_init_t init;
    init.type = HEADBAR_TYPE_NORMAL;
    init.str_id_title = g_ebook_main->get_ebook_string()->str_id;
    init.pic_id_head = g_ebook_main->get_headbar_pid();

    //create widget
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

    return true;
}

static void _proc_gui_msg( gui_wm_message_t *pmsg)
{
    int top;
    key_event_t *kmsg;

//	      if ( pmsg->msgid == WM_MSG_KEY )
//	      {
//	          kmsg = (key_event_t *)(pmsg->data.p); /*get key msg*/
//	      
//	          print_dbg(" key value = 0x%x, type = 0x%x ", kmsg->val , kmsg->type );
//	      }

    top = canvas_get_focus(pmsg);

    if ( s_headbar != NULL )
    {
        headbar_proc_gui( s_headbar, pmsg );
    }

    if ( ( s_msgbox != NULL )
            && ( s_msgbox->canvas_id == top ) )
    {
        msgbox_proc_gui( s_msgbox, pmsg );
        return;
    }

    if ( ( s_menuview != NULL ) 
            && ( s_menuview->canvas_id == top ) )
    {
        menuview_proc_gui( s_menuview, pmsg );
        return;
    }

    if ( ( s_adjust != NULL )
            && ( s_adjust->canvas_id == top ) )
    {
        adjust_proc_gui( s_adjust, pmsg);
        return;
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
        return;
    }
    
    if ( pmsg->msgid == WM_MSG_KEY )
    {
        kmsg = (key_event_t *)(pmsg->data.p); /*get key msg*/
  
        _proc_key( kmsg );
    }
    
}

static void _proc_key( key_event_t *msg )
{
    bool ret;
    unsigned int total_page;

    key_event_t *kmsg = msg;
    switch ( kmsg->val | kmsg->type )
    {
        /* prev key 相关 */
        case KEY_PREV | KEY_TYPE_SHORT_UP :
        {
            ret = _try_show_prev_page();
            if ( ret == true )
            {
                restart_auto_reading_timer();
            }

            clear_key_msg_queue( KEY_PREV, KEY_TYPE_SHORT_UP );
            
            break;
        }

        case KEY_PREV | KEY_TYPE_HOLD :
        {
            if ( s_current_page == 1 )
            {
                break;
            }

            if ( s_quick_move_timer == NULL_TIMER )
            {
                s_quick_move_next = false; /* 方向是向前的 */
                s_quick_move_timer = set_timer( s_quick_move_gap, _proc_quick_move_timer, NULL );

                if ( s_auto_read_timer != NULL_TIMER )
                {
                    stop_timer( s_auto_read_timer );
                }
            }
            break;
        }

        case KEY_PREV | KEY_TYPE_HOLD_UP :
        {
//	            print_dbg(" KEY_PREV | KEY_TYPE_HOLD_UP................ ");
            
            if ( s_quick_move_timer != NULL_TIMER )
            {
                kill_timer( s_quick_move_timer );
                s_quick_move_timer = NULL_TIMER;

                _try_show_prev_page();

                restart_auto_reading_timer();
            }

//	            clear_key_msg_queue( -1, KEY_TYPE_HOLD );

            break;
        }

        /* next key 相关 */
        case KEY_NEXT | KEY_TYPE_SHORT_UP :
        {
            ret = _try_show_next_page();
            if ( ret == true )
            {
                restart_auto_reading_timer();
            }

            clear_key_msg_queue( KEY_NEXT, KEY_TYPE_SHORT_UP );
            
            break;
        }

        case KEY_NEXT | KEY_TYPE_HOLD :
        {
            total_page = (unsigned int)s_count_page->get_total_page();
            if ( s_current_page >= total_page )
            {
                break;
            }

            if ( s_quick_move_timer == NULL_TIMER )
            {
                s_quick_move_next = true;/* 方向是向后的 */
                s_quick_move_timer = set_timer( s_quick_move_gap, _proc_quick_move_timer, NULL );

                if ( s_auto_read_timer != NULL_TIMER )
                {
                    stop_timer( s_auto_read_timer );
                }
            }

//	            clear_key_msg_queue( -1, KEY_TYPE_HOLD );

            break;
        }

        case KEY_NEXT | KEY_TYPE_HOLD_UP:
        {
            print_dbg(" KEY_NEXT | KEY_TYPE_HOLD_UP................ ");
            
            if ( s_quick_move_timer != NULL_TIMER )
            {
                kill_timer( s_quick_move_timer );
                s_quick_move_timer = NULL_TIMER;

                _try_show_next_page();

                restart_auto_reading_timer();
            }
            break;
        }

        case KEY_MENU | KEY_TYPE_SHORT_UP:
        {
            s_read_option->scene_read_option_set_scene(s_scene);
            s_menuview = s_read_option->scene_read_option_enter();

            /* 暂停自动阅读的定时器 */
            if ( s_control_mode == AUTO_READING )
            {
                if ( s_auto_read_timer != NULL_TIMER )
                {
                    stop_timer( s_auto_read_timer );
                }
            }

            break;
        }

        case KEY_OK | KEY_TYPE_SHORT_UP:
        {
            _change_control_mode();
            break;
        }

        case KEY_RETURN | KEY_TYPE_SHORT_UP:
        {
            exit_msg_loop();
            g_ebook_main->go_back_last_scene();
            break;
        }

        case KEY_VSUB | KEY_TYPE_DOWN:
        case KEY_VADD | KEY_TYPE_DOWN:
        {
            if ( s_volumebar == NULL)
            {
                /* 构造音量条 */
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
            try_delete_current_file();
            break;
        }

        default:
        {
            break;
        }

    }
}

static bool _show_book(void)
{
    open_file_result_e result;

    result = s_book_buffer->open_book_file();

    if ( result != OPEN_SUCCESS )
    {
        dialog_init_t init;
        string_resource_t* string;

        if ( result == TOO_LARGE )
        {
            string = (string_resource_t*)g_style_api->get_scene_child( s_scene, STR_TOO_LARGE );
        }
        else
        {
            string = g_ebook_main->get_file_open_fail_string();
        }
        
        init.head = DIALOG_HEAD_FAIL;
        init.str_id = string->str_id;

        dialog_modal( &init );

        clear_current_reading_file();
    
        return false;  /* 为了显示打开错误对话框 */
    }

    _show_current_page();
            
    return true;
}


static bool _show_current_page(void)
{
    bool ret;
    
    ret = s_book_buffer->set_file_current_point( s_current_page_start_point );
    if ( ret == false )
    {
        s_current_page_start_point = 0;
    }

    ret = _show_next_page( true, true , false );

    /* 从标签场景进入的情况，已经设置好了 s_current_page */
    if ( ( s_new_book == true ) 
        && ( g_ebook_main->get_last_scene() != SCENE_BOOKMARK_LIST ) )
    {
        if ( ret == true )
        {
            /* 处理"空文件" */
            s_current_page++;
        }
    }

    return ret;
}

static bool _repaint_current_page(void)
{
    bool ret;

    s_book_buffer->set_file_current_point( s_current_page_start_point );

    ret = _show_next_page( true, true , false );

    return ret;
}

static bool _try_show_next_page(void)
{
    bool ret;
    unsigned int total_page;

    total_page = (unsigned int)s_count_page->get_total_page();
    if ( s_current_page >= total_page )
    {
        return false;
    }

    s_fusion_direction = TRANS_LEFT;

    ret = _show_next_page( true, true, true );
    if ( ret == true )
    {
        s_current_page++;
        update_cur_total_page();
    }

    return ret;
}

static bool _show_next_page( bool fill_buffer, bool paint, bool need_fusion )
{
    int new_page_point;

    s_show_lines = 0;
    memset( s_show_buffer, 0, SHOW_BUFFER_SIZE );

    new_page_point = s_book_buffer->get_current_reading_point() + 1;

    _show_next_multi_lines( fill_buffer );

    PRINTF(" \n\n============================ \n\n");

    if ( s_show_lines > 0 )
    {
        s_current_page_start_point = new_page_point;

        if ( paint == true )
        {
            fusion_frame_t frame;
            fusion_effect_t effect;

            if ( need_fusion == true )
            {
                /* 刚打开第一页时，不需要 */
                frame.rect.x = s_fusion_rect.x;
                frame.rect.y = s_fusion_rect.y;
                frame.rect.width = s_fusion_rect.width;
                frame.rect.height = s_fusion_rect.height;
                frame.pdata = NULL;
                frame.alpha = false;

                effect.mode = s_fusion_mode;
                effect.direct = s_fusion_direction;
                fusion_set_effect( &effect );
                fusion_set_first_frame( &frame );
            }

            _paint_current_page();

            if ( need_fusion == true )
            {
                fusion_set_last_frame( &frame );
                fusion_display( NULL );
            }

        }

        return true;
    }

    return false;
}

static bool _try_show_prev_page(void)
{
    bool ret;

    s_fusion_direction = TRANS_RIGHT;

    ret = _show_prev_page( true, true );
    if ( ret == true )
    {
        s_current_page--;
        if ( s_current_page == 0 )
        {
            s_current_page = 1;
        }

        update_cur_total_page();
        return true;
    }

    return false;
}

static bool _show_prev_page( bool fill_buffer, bool paint )
{
    bool ret;

    s_show_lines = 0;

//      _clear_all_left_char();

    ret = _find_prev_page_point();

    if ( ret == false )
    {
        if ( s_show_lines == 0 )
        {
            /* 已经到文件的开始了，不能再往前了 */
            paint = false;
        }
    }

    _show_next_page( fill_buffer, paint, true );

    return ret;
}

static bool _find_prev_page_point(void)
{
    int current_point;  /* 当前页，正在阅读点 */
    int save_point;     /* 保存往回走的文件点 */
    unsigned short back_read_num = 1;

    current_point = s_current_page_start_point;
    while( 1 )
    {
        int point = 0;

        /* 找到一个换行点 */
        point = s_book_buffer->find_prev_newline( back_read_num );
        point = point + 1; /* 到换行点的下一点 */
        save_point = point;

        /* 看该点距离当前点有多少行 */
        s_show_lines = 0;
        while( point < current_point )
        {
            _get_a_line( false );

            point = s_book_buffer->get_current_reading_point();
        }

        if ( s_show_lines > s_current_font_layout.max_line )    /* 距离要超过一页 */
        {
            /* 距离超过一页，成功 */
            int i;
            int distance;

            s_book_buffer->set_file_current_point( save_point );

            /* 跳过多余的行 */
            distance = (int)( s_show_lines - s_current_font_layout.max_line - 1 );
            for ( i = 0; i < distance; i++ )
            {
                _get_a_line( false );
            }

            return true;
        }

        if ( save_point == 0 )
        {
            /* 已经到了文件的开头 */

            if ( s_show_lines > 0 )
            {
                /* 改变字体大小，造成的错乱 */
                s_book_buffer->set_file_current_point( 0 );

                return true;
            }
            else
            {
                /* 没有前一页了，回到原来的点 */
                s_book_buffer->set_file_current_point( current_point );
            }

            return false;
        }

        /* 到回车符的前面，继续向前找 */
        back_read_num++; /* 文件指针再往前 512 个字节 */
    }
}

static bool _paint_current_page(void)
{
    s_scene_read_ui->paint_text_background();

    s_scene_read_ui->paint_text( s_show_buffer, s_current_font_layout, s_show_lines,
                                 s_book_buffer->get_file_type(), s_language_mode );

    return true;
}


static void _show_next_multi_lines( bool fill_buffer )
{
    unsigned int line;

    for ( line = 0; line < s_current_font_layout.max_line; line++ )
    {
        if ( _get_a_line( fill_buffer ) == false )
        {
            return;
        }

        PRINTF("\n");
    }
}


static bool _get_a_line( bool fill_buffer )
{
    bool result = true;
    file_type_e type = s_book_buffer->get_file_type();

    if ( type == TXT_ASCII_MBCS)
    {
        result = _get_a_ascii_line( fill_buffer );
    }
    else if ( type == TXT_UTF8)
    {
        result = _get_a_utf8_line( fill_buffer );
    }
    else
    {
        result = _get_a_unicode_line( fill_buffer );
    }

    return result;
}




/*  fill_buffer == true 表示是真的读取显示字符。
    fill_buffer == false 用于在向前翻页时，统计行数，所以不能填 s_buffer(越界)。

    返回 true 表示需要继续读下一行；
    返回 false 表示文件已经结束，不能再读了。
*/
static bool _get_a_ascii_line( bool fill_buffer )
{
    unsigned char cur_char = 0;
    unsigned char second_char = 0;
    unsigned char* char_address = NULL;
    unsigned int show_offset = 0;
    unsigned char* show_buffer = NULL;

    unsigned int word_length;
    unsigned int chars_width;


    chars_width = 0;
    show_offset = 0;

    show_buffer = s_show_buffer + ( s_show_lines * LINE_MAX_CHARS );

    for ( ; ; )
    {
        cur_char = s_book_buffer->get_current_char();   /* 取出当前字节 */

        if ( _end_of_file( show_offset ) == true )
        {
//                  print_dbg(" show_buffer = %s ", show_buffer );
            return false;
        }

        char_address = s_book_buffer->get_current_char_address(); /* 取出当前字节的地址 */
       
        word_length = (unsigned int)gui_get_word_length( char_address, (int)s_language_mode );

//	         print_info( " char = %c , value = 0x%x, word length = %d \n", cur_char, cur_char, word_length );
        PRINTF(" 0x%x ", cur_char );

        /* 处理换行符 */
        if ( ( word_length == 1 ) && ( cur_char == 0x0D ) )
        {
            continue;
        }
        if ( ( word_length == 1 ) && ( cur_char == 0x0A ) ) /* 换行*/
        {
            s_show_lines++;

//              print_dbg(" show_buffer = %s ", show_buffer );
            return true;
        }


        /* 是否超出屏幕宽度 */
        if ( word_length == 1 )
        {
            chars_width += s_ascii_width[ cur_char ];
        }
        else
        {
            chars_width += s_current_font_layout.font_size;
        }

        if ( chars_width >= s_current_font_layout.width )
        {
            /* 字节已经从buffer 取出来了，缓存起来 */
//              s_last_left_char = cur_char;
//              s_last_left_char_address = char_address;

            s_book_buffer->backward_n_char( 1 );

            s_show_lines++;

//              print_dbg(" show_buffer = %s ", show_buffer );
            return true;
        }


        if ( word_length == 2 )
        {
            /* 取出第二个字节 */
            second_char = s_book_buffer->get_current_char();
       
            if ( _end_of_file( show_offset ) == true )
            {
                return false;
            }

            PRINTF(" , 0x%x ", second_char );
        }


        if ( fill_buffer == true )
        {
            /* 存放到显示buffer */
            if ( word_length == 1 )
            {
                /* 存放显示的字符 */
                show_buffer[ show_offset ] = cur_char;
                show_offset++;
            }
            else
            {
                /* 存放显示的字符 */
                show_buffer[ show_offset ] = cur_char;
                show_offset++;
                show_buffer[ show_offset ] = second_char;
                show_offset++;
            }
        }

        PRINTF("   /   ");

    } /* for ( ; ; ) */

}

/* 把所有字符都存成两个字节的 unicode 字符来显示。 */
static bool _get_a_utf8_line( bool fill_buffer )
{
    unsigned int show_offset = 0;
    unsigned char* show_buffer = NULL;

    unsigned int chars_width;

    unsigned short first_char = NULL_CHAR;
    unsigned char second_char = NULL_CHAR;
    unsigned char third_char = NULL_CHAR;

    chars_width = 0;
    show_offset = 0;

    show_buffer = s_show_buffer + ( s_show_lines * LINE_MAX_CHARS );

    for ( ; ; )
    {
        unsigned short word_chars_num;
        unsigned short word = NULL_CHAR;

        first_char = (unsigned short)s_book_buffer->get_current_char(); /* 取出当前字节 */
        if ( _end_of_file( show_offset ) == true )
        {
            return false;
        }

        /* 跳过文件头 bom */
        if ( ( first_char == 0xEF ) && ( s_book_buffer->get_index_char(1) == 0XBB )
                && ( s_book_buffer->get_index_char(2) == 0xBF ))
        {
            s_book_buffer->get_current_char();
            s_book_buffer->get_current_char();
            if ( _end_of_file( show_offset ) == true )
            {
                return false;
            }

            continue;
        }

        /* 对不同字节数的字符进行不同的处理，得到字符的宽度 */
        if ( first_char < 0x80 ) /* ASCII 码 */
        {
            word_chars_num = 1;

            /* 处理换行符 */
            if ( first_char == 0x0D )
            {
                continue;
            }
            if ( first_char == 0x0A ) /* 换行*/
            {
                s_show_lines++;
                return true;
            }

            /* 先填到 buffer 中 ，也保存成两个字节的 unicode 字符 */
            if ( fill_buffer == true )
            {
                show_buffer[ show_offset ] = (unsigned char)first_char;
                show_offset++;
                show_buffer[ show_offset ] = 0;
                show_offset++;
            }

            chars_width += s_ascii_width[ (unsigned char)first_char ];
        }
        else if (( first_char & 0xe0) == 0xc0) /* 双字节的字符 */
        {
            word_chars_num = 2;

            /* 取出第二个字节 */
            second_char = s_book_buffer->get_current_char();
            if ( _end_of_file( show_offset ) == true )
            {
                return false;
            }

            /* 得到实际的 unicode 字符 */
            word = first_char << 6;
            word |= second_char & 0x3F;
            word &= 0x7FF;


            /* 先填到 buffer 中 */
            if ( fill_buffer == true )
            {
                show_buffer[show_offset ] = (unsigned char) (word & 0xFF);
                show_offset++;
                show_buffer[show_offset ] = (unsigned char) (word >> 8);
                show_offset++;
            }

            chars_width += (unsigned int)_get_char_width_ext( word, GUI_UNICODE_ENCODE );
        }
        else if (( first_char & 0xf0) == 0xe0) /* 三字节的字符 */
        {
            word_chars_num = 3;

            /* 取出第二，三个字节 */
            second_char = s_book_buffer->get_current_char();
            if ( _end_of_file( show_offset ) == true )
            {
                return false;
            }

            third_char = s_book_buffer->get_current_char();
            if ( _end_of_file( show_offset ) == true )
            {
                return false;
            }


            /* 将三字节的 utf8 转到两个字节的 unicode 字符*/
            word = ( first_char & 0x0F ) << 12;
            word |= ( ( second_char & 0x3F ) << 6);
            word |= ( third_char & 0x3F );


            /* 先填到 buffer 中 */
            if ( fill_buffer == true )
            {
                show_buffer[ show_offset ] = (unsigned char) (word & 0xFF);
                show_offset++;
                show_buffer[ show_offset ] = (unsigned char) (word >> 8);
                show_offset++;
            }

            chars_width += (unsigned int)_get_char_width_ext( word, GUI_UNICODE_ENCODE );
        }
        else  /* 其他情况 */
        {
            word_chars_num = 1;

            assert( first_char < 256);
            chars_width += s_ascii_width[ (unsigned char)first_char ];
        }

        if ( chars_width >= s_current_font_layout.width )
        {
            /* 清空已经填进去的字符 */
            if ( fill_buffer == true )
            {
                show_buffer[ show_offset - 2 ] = 0;
                show_buffer[ show_offset - 1 ] = 0;
            }

            s_book_buffer->backward_n_char( word_chars_num );

            s_show_lines++;
            return true;
        }

    } /* for ( ; ; ) */
}


static bool _get_a_unicode_line( bool fill_buffer )
{
    unsigned char first_char = 0;
    unsigned char second_char = 0;
    unsigned short word = 0;

    unsigned int show_offset = 0;
    unsigned char* show_buffer = NULL;

    unsigned int chars_width;

    chars_width = 0;
    show_offset = 0;


    show_buffer = s_show_buffer + ( s_show_lines * LINE_MAX_CHARS );

    for ( ; ; )
    {
        first_char = s_book_buffer->get_current_char(); /* 取出当前字节 */
        if ( _end_of_file( show_offset ) == true )
        {
            return false;
        }

        second_char = s_book_buffer->get_current_char();    /* 取出当前字节 */
        if ( _end_of_file( show_offset ) == true )
        {
            return false;
        }
//          print_info( " first char = 0x%x , second char = 0x%x \n", first_char, second_char );

        /* 跳过文件头 bom */
        if ( ( ( first_char == 0xFF) && ( second_char == 0xFE ) )
                    || ( ( first_char == 0xFE ) && ( second_char == 0xFF ) ) )
        {
            continue;
        }

        /* 处理换行符 */
        if ( ( ( first_char == 0x0D) && ( second_char == 0x00 ) )
                    || ( ( first_char == 0x00) && ( second_char == 0x0D ) ) )
        {
            continue;
        }
        else if ( ( ( first_char == 0x0A ) && ( second_char == 0x00 ) )
                    || ( ( first_char == 0x00) && ( second_char == 0x0A ) ) )
        {
            if ( fill_buffer == true )
            {
                show_buffer[ show_offset ] = 0;
                show_offset++;
                
                show_buffer[ show_offset ] = 0;
            }

            s_show_lines++;
            return true;
        }
        else
        {
        }

        /* 是否超出屏幕宽度 */
        /* 得到真正的 unicode 值 */
        if ( s_book_buffer->get_file_type() == TXT_UNICODE_LITTLE )
        {
            word = (unsigned short)first_char | ( ( (unsigned short)second_char ) << 8 );
        }
        else /* if ( s_book_buffer->get_file_type() == TXT_UNICODE_BIG ) */
        {
            word = ( ( (unsigned short)first_char ) << 8 ) | (unsigned short)second_char  ;
        }

        chars_width += (unsigned int)_get_char_width_ext( word, GUI_UNICODE_ENCODE );

        if ( chars_width >= s_current_font_layout.width )
        {
            s_book_buffer->backward_n_char( 2 );

            s_show_lines++;
            return true;
        }


        if ( fill_buffer == true )
        {
            /* 存放到显示buffer */
            if ( s_book_buffer->get_file_type() == TXT_UNICODE_LITTLE )
            {
                show_buffer[ show_offset ] = first_char;
                show_offset++;
                show_buffer[ show_offset ] = second_char;
                show_offset++;
            }
            else /* if ( s_book_buffer->get_file_type() == TXT_UNICODE_BIG ) */
            {
                show_buffer[ show_offset ] = second_char;
                show_offset++;
                show_buffer[ show_offset ] = first_char;
                show_offset++;
            }
        }

    } /* for ( ; ; ) */
}

static bool _end_of_file( unsigned int show_offset )
{
    if ( s_book_buffer->end_of_file() == true ) /* 是否已经到达文件结尾 */
    {
        if ( show_offset > 0 )
        {
            s_show_lines++;
        }

        return true;
    }

    return false;
}

void set_file_path( const char* path, const char* name )
{
    char new_full_path[ PATH_MAX ];
    memset( new_full_path, 0, PATH_MAX );
    
    strcpy( new_full_path, path );
    strcat( new_full_path, name );
    
    /* 要得到完整的路径才能比较 */
    if ( ( strcmp( s_file_full_path, new_full_path ) == 0 )
        && ( _check_file_same_length( s_file_full_path, new_full_path ) == true ) )
    {
        /* 还是旧书 */                
        return;
    }

    memset( s_file_full_path, 0, PATH_MAX );
    memset( s_file_name, 0, PATH_MAX );

    strcpy( s_file_full_path, new_full_path );
    strcpy( s_file_name, name );

    s_current_page = 0;
    s_current_page_start_point = 0;
    s_new_book = true;
}

void set_file_name( const char* name )
{
    if ( name != NULL )
    {    
        memset( s_file_name, 0, PATH_MAX );
        strcpy( s_file_name, name );
    }
    
}

void set_file_full_path( const char* full_path )
{
    if ( full_path != NULL )
    {
//	        if ( strcmp( s_file_full_path, full_path ) == 0 )
//	        {
//	            return;
//	        }
    
        memset( s_file_full_path, 0, PATH_MAX );
        strcpy( s_file_full_path, full_path );

        s_current_page = 0;
        s_current_page_start_point = 0;
        s_new_book = true;
    }    
}

char* get_file_name(void)
{
    if ( strlen( (char*)s_file_name ) == 0 )
    {
        return NULL;
    }

    return s_file_name;
}

char* get_file_path(void)
{
    if ( strlen( (char*)s_file_full_path ) == 0 )
    {
        return NULL;
    }

    return s_file_full_path;
}

file_type_e get_file_type(void)
{
    return s_book_buffer->get_file_type();
}

void set_control_mode( control_mode_e mode )
{
    s_control_mode = mode;

    if ( s_window != 0 )
    {
        if ( s_control_mode == AUTO_READING )
        {
            if ( s_auto_read_timer == NULL_TIMER )
            {
                s_auto_read_timer = set_timer( s_auto_play_gap, _proc_auto_read_timer, NULL );
            }
        }
    }
}

static void _change_control_mode(void)
{
    /* 开关定时器 */
    if ( s_control_mode == HAND_READING )
    {
        s_control_mode = AUTO_READING;

        if ( s_auto_read_timer == NULL_TIMER )
        {
            s_auto_read_timer = set_timer( s_auto_play_gap, _proc_auto_read_timer, NULL );
        }
        else
        {
            restart_timer( s_auto_read_timer );
        }
    }
    else
    {
        s_control_mode = HAND_READING;

        if ( s_auto_read_timer != NULL_TIMER )
        {
            stop_timer( s_auto_read_timer );
        }
    }

    s_scene_read_ui->paint_control_mode( s_control_mode );
}

void set_auto_play_gap( unsigned int gap )
{
    s_auto_play_gap = gap;
}

unsigned int get_auto_play_gap(void)
{
    return s_auto_play_gap;
}

void _change_auto_play_gap( unsigned int gap )
{
    if ( gap == s_auto_play_gap )
    {
        return;
    }

    s_auto_play_gap = gap;

    if ( s_auto_read_timer != NULL_TIMER )
    {
        modify_timer( s_auto_read_timer, s_auto_play_gap );
    }
}


void creat_skip_to_adjust(void)
{
    adjust_textdigit_init_t init;
    memset( &init, 0 , sizeof(adjust_textdigit_init_t) );

    init.str_title_id = s_skip_to_string->str_id;
    strcpy( init.str_unit , "%" );
    init.max_val = 100;
    init.min_val = 0;
    init.step = (int)s_skip_to_step;
    init.cur_val = (int)s_skip_percent;

    s_adjust = adjust_create( &init, ADJ_STYLE_TEXTDIGIT, _skip_to_adjust_cb );
}

static void _skip_to_adjust_cb( adjust_t *pAdjust, adjust_signal_e signal )
{
    switch( signal )
    {
        case WIDGET_SIG_ADJUST_OK:
        {
            print_dbg("skip to %d", pAdjust->adjust_digit.cur_val );

            s_skip_percent = (unsigned int)pAdjust->adjust_digit.cur_val;
            
            adjust_delete( s_adjust );
            s_adjust = NULL;

            s_read_option->scene_read_option_exit();

            _skip_to_percent( s_skip_percent );
            
            break;
        }

        case WIDGET_SIG_ADJUST_CANCEL:
        {
            adjust_delete( s_adjust );
            s_adjust = NULL;
            break;
        }

        default :
        {
        }
    }
}

void creat_auto_play_gap_adjust(void)
{
    adjust_textdigit_init_t init;
    memset( &init, 0 , sizeof(adjust_textdigit_init_t) );

    init.str_title_id = s_auto_play_gap_string->str_id;
    init.str_unit_id = s_second_string->str_id;
    init.max_val = (int)s_auto_play_max_gap;
    init.min_val = (int)s_auto_play_min_gap;
    init.step = (int)s_auto_play_step;
    init.cur_val = (int)(s_auto_play_gap / 1000);

    s_adjust = adjust_create( &init, ADJ_STYLE_TEXTDIGIT, _auto_play_gap_adjust_cb );
}

static void _auto_play_gap_adjust_cb( adjust_t *pAdjust, adjust_signal_e signal )
{
    switch( signal )
    {
        case WIDGET_SIG_ADJUST_OK:
        {
            _change_auto_play_gap( (unsigned int)( pAdjust->adjust_digit.cur_val * 1000 ) );
            print_dbg(" s_auto_play_gap = %d ", s_auto_play_gap );

            adjust_delete( s_adjust );
            s_adjust = NULL;
            break;
        }

        case WIDGET_SIG_ADJUST_CANCEL:
        {
            adjust_delete( s_adjust );
            s_adjust = NULL;
            break;
        }

        default :
        {
        }
    }
}

static void _skip_to_percent( unsigned int percent )
{
    bool ret;
    unsigned int page;
    skip_point_t point;
    bool need_fusion = true;

    if ( s_count_page->get_total_page() == 0 )
    {
        return;
    }

    s_count_page->get_skip_point( percent, &point );

    s_book_buffer->set_file_current_point( (int)point.skip_point );

    if ( ( point.skip_point_page + point.page_distance + 1 ) > s_current_page )
    {
        s_fusion_direction = TRANS_LEFT;
    }
    else if ( ( point.skip_point_page + point.page_distance + 1 ) < s_current_page )
    {
        s_fusion_direction = TRANS_RIGHT;
    }
    else
    {
        need_fusion = false;
    }

    s_current_page = point.skip_point_page;

    /* 跳过 skip point 到当前页的页 */
    for ( page = 0; page < ( point.page_distance - 1 ); page++ )
    {
        ret = _show_next_page( false, false, false );
        if ( ret == true )
        {
            s_current_page++;
        }
    }

    ret = _show_next_page( true, true, need_fusion );
    if ( ret == true )
    {
        s_current_page++;
    }

    update_cur_total_page();
}

void try_delete_current_file(void)
{
    dialog_init_t init;
    init.head = DIALOG_HEAD_QUERY;
    init.str_id = g_ebook_main->get_sure_delete_string()->str_id;
    init.def_sel = WIDGET_SIG_DIALOG_CANCAL;

    s_dialog = dialog_create( &init, _try_delete_file_dialog_callback );
    if ( s_dialog == NULL )
    {
        print_err(" create dialog error ! ");
        return;
    }

    dialog_paint( s_dialog );
}

static void _try_delete_file_dialog_callback( dialog_t* dialog, widget_sig_dialog_e signal )
{
    switch( signal )
    {
        case WIDGET_SIG_DIALOG_OK :
        {
            dialog_delete( s_dialog );
            s_dialog = NULL;

            _deleting_current_file();
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

static bool _deleting_current_file(void)
{
    dialog_init_t init;
    init.head = DIALOG_HEAD_INFO;
    init.str_id = g_ebook_main->get_deleting_string()->str_id;

    s_dialog = dialog_create( &init, _delete_file_dialog_callback );
    if ( s_dialog == NULL )
    {
        print_err(" create dialog error ! ");
        return false;
    }

    dialog_paint( s_dialog );

    _delete_current_file();

    return true;
}

static void _delete_file_dialog_callback( dialog_t* dialog, widget_sig_dialog_e signal )
{
    switch( signal )
    {
        case WIDGET_SIG_DIALOG_OK :
        {
            dialog_delete( s_dialog );
            s_dialog = NULL;

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

    /* 回到前一个场景 */
    exit_msg_loop();
    g_ebook_main->go_back_last_scene();
}


static bool _delete_current_file(void)
{
    bool ret;

    _stop_count_page();
    s_book_buffer->close_old_book();

    ret = g_ebook_main->remove_file( s_file_full_path );
    if ( ret == false )
    {
        print_info( "remove ebook file %s failed !!! ", s_file_full_path );
        return false;
    }
    print_info( "remove ebook file %s", s_file_full_path );

    clear_current_reading_file();

    return true;
}

void clear_current_reading_file(void)
{
    s_book_buffer->close_old_book();

    memset( s_file_full_path, 0, PATH_MAX );
    memset( s_file_name, 0, PATH_MAX );    

    s_new_book = true;
    s_current_page = 0;
    s_current_page_start_point = 0;
}

void update_cur_total_page(void)
{
    int total_page;

    if ( g_ebook_main->get_current_scene() != SCENE_READ )
    {
        return;
    }
    
    total_page = s_count_page->get_total_page();

    memset( s_cur_total_page, 0, CUR_TOTAL_PAGE_BUFFER_LENGTH );
    snprintf( s_cur_total_page, CUR_TOTAL_PAGE_BUFFER_LENGTH,"%d/%d", s_current_page, total_page );
    print_dbg(" s_cur_total_page = %s", s_cur_total_page );

    s_scene_read_ui->paint_cur_total_page( s_cur_total_page );
}

static int _proc_auto_read_timer( void * param )
{
//      print_dbg("");

    _try_show_next_page();

    return 0;
}

static int _proc_quick_move_timer( void * param )
{
    bool ret;
    unsigned int total_page;
    total_page = (unsigned int)s_count_page->get_total_page();

//      print_dbg("_proc_quick_move_timer ");
    if ( s_quick_move_next == true )   /* 向后翻页 */
    {
        if ( s_current_page >= total_page )
        {
            return 0;
        }

        if ( s_current_page == ( total_page - 1 ) )
        {
            /* 需要画出最后一页 */
            ret = _show_next_page( true, true, true );
        }
        else
        {
            ret = _show_next_page( false, false, false );
        }

        if ( ret == true )
        {
            s_current_page++;
            update_cur_total_page();
        }
    }
    else    /* 向前翻页 */
    {
        if ( s_current_page <= 1 )
        {
            return 0;
        }

        if ( s_current_page == 2 )
        {
            /* 需要画出第一页 */
            ret = _show_prev_page( true, true );
        }
        else
        {
            ret = _show_prev_page( false, false );
        }

        if ( ret == true )
        {
//              print_dbg("-=-*****************************---");
        
            s_current_page--;
            update_cur_total_page();
        }
    }

    return 0;
}

bool is_reading(void)
{
    char* path;
    path = get_file_path();
    if ( path != NULL )
    {
        return true;
    }

    return false;
}

void set_font_layout( unsigned short findex, int x, int y, int width, int size, int line_height, int max_line )
{
    if ( findex > 2  )
    {
        return;
    }
    
    s_font_layout[ findex ].x = x;
    s_font_layout[ findex ].y = y;
    s_font_layout[ findex ].width = (unsigned int)width;
    s_font_layout[ findex ].font_size = (unsigned int)size;
    s_font_layout[ findex ].line_height = (unsigned int)line_height;
    s_font_layout[ findex ].max_line = (unsigned int)max_line;
}

void set_font_type( font_type_e type )
{
    s_font_type = type;

    s_current_font_layout = s_font_layout[ type ];
    print_dbg("current font size = %d, line_height = %d,max_line = %d ", s_current_font_layout.font_size,
                s_current_font_layout.line_height, s_current_font_layout.max_line );

    if ( s_scene_read_ui != NULL )
    {
        s_scene_read_ui->set_dc_font_size( (int)s_current_font_layout.font_size );
    }

    if ( s_count_page != NULL )
    {
        s_count_page->set_font_layout( &s_current_font_layout );
    }
}

font_type_e get_font_type(void)
{
    return s_font_type;
}

void change_font_type( font_type_e type )
{
    if ( type == s_font_type )
    {
        return;
    }

    set_font_type( type );

    _get_ascii_width();
    memset( s_font_width_buffer, 0 , FONT_WIDTH_BUFFER_WIDTH );

    _repaint_current_page();

    /* 页数已经错误，需要重数 */
    _stop_count_page();
    _start_count_page();
}

void set_ui_font_colors( unsigned int findex, int color, int bg_color )
{
    s_scene_read_ui->set_font_colors( findex, color, bg_color );
}

void set_font_color_num( unsigned int num )
{
    s_font_color_num = num;
}

unsigned int get_font_color_num(void)
{
    return s_font_color_num;
}

void set_ui_use_color_background( bool use )
{
    s_scene_read_ui->set_use_color_background( use );
}

void set_font_color( unsigned int color )
{

    s_font_color = color;

    if ( s_scene_read_ui != NULL )
    {
        s_scene_read_ui->set_ui_font_color( color );
    }
}


unsigned int get_font_color(void)
{
    return s_font_color;
}

void change_font_color( unsigned int color )
{

    if ( color == s_font_color )
    {
        return;
    }

    s_font_color = color;

    set_font_color( color );

//	    if ( color == DEFAULT_COLOR_INDEX )
//	    {
//	        _paint_background();
//	    }

//	    if ( s_scene_read_ui->use_color_background() == true )
//	    {
//	        /* 重画阅读画面 */
//	        s_scene_read_ui->paint_initial_ui( s_control_mode, s_file_name );
//	        s_scene_read_ui->paint_cur_total_page( s_cur_total_page );
//	    }

//	    if ( color == DEFAULT_COLOR_INDEX )
//	    {
//	        headbar_paint( s_headbar );
//	    }

    /* 重画当前页 */
    _paint_current_page();
}


/* scene release function */
static bool _scene_read_deinit(void)
{
    if ( s_read_option != NULL)
    {
        s_read_option->scene_read_option_exit();
        s_menuview = NULL;
        //s_read_option = NULL;
    }

    if ( s_auto_read_timer != NULL_TIMER )
    {
        kill_timer( s_auto_read_timer );
        s_auto_read_timer = NULL_TIMER;
    }
    
    if ( s_quick_move_timer != NULL_TIMER )
    {
        kill_timer( s_quick_move_timer );
        s_quick_move_timer = NULL_TIMER;
    }

    if ( s_headbar != NULL )
    {
        headbar_delete(s_headbar);
        s_headbar = NULL;
    }

    if ( s_adjust != NULL )
    {
        adjust_delete( s_adjust );
        s_adjust = NULL;
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

    s_scene_read_ui->deinit();

    if ( s_scene != NULL )
    {
        g_style_api->free_scene( s_scene );
        s_scene = NULL;
    }

    gui_destroy_font( s_ttf_fontdesc );

    /* 删除窗体 */
    if ( s_window != 0)
    {
        gui_wm_delete_window( s_window );
    }

    s_book_buffer->close_old_book();
    /* 每次退出都要停止数页，因为阅读 jar 文件的时候，会生成临时文件，
     * 如果不退出，不删除这个临时文件，则 dirlist 会看到这个文件。
     */
    _stop_count_page(); 

    /* 解禁 */
    sys_forbid_screensaver( false );
    sys_forbid_auto_standby( false );

    return true;
}

static void _get_ascii_width(void)
{
    unsigned int i;

    memset( s_ascii_width, (int)s_current_font_layout.font_size, 32 );
    /* 修改绘图字体大小(计算字体宽度需要)*/
//      gui_select_font_size( s_current_font_layout.font_size );

    if ( s_book_buffer->get_file_type() != TXT_ASCII_MBCS )
    {
        for (i = 32; i < 256; i++)
        {
            s_ascii_width[i] = (unsigned char)gui_dc_get_char_dist_x( s_hdc, i, GUI_UNICODE_ENCODE );
//              print_dbg(" s_ascii_width[ %d ] ==== %d ", i, s_ascii_width[i] );
        }
    }
    else
    {
        for (i = 32; i < 256; i++)
        {
            s_ascii_width[i] = (unsigned char)gui_dc_get_char_dist_x( s_hdc, i, GUI_MBCS_ENCODE );
//              print_dbg(" s_ascii_width[ %d ] ==== %d ", i, s_ascii_width[i] );  
        }
    }
}


static char _get_char_width_ext( unsigned short char_add, int encoding )
{
    char* temp_add;

    if ( char_add <= 0x00FF )
    {
        return s_ascii_width[ char_add ];
    }

    if ((char_add >= 0x3400) && (char_add <= 0x9FBF))
    {
        /* 中文字等宽高 */
        return s_current_font_layout.font_size;
    }

    temp_add = s_font_width_buffer + char_add;

    if ( *temp_add != 0)
    {
        return *temp_add;

    }
    else
    {
        /* 修改绘图字体大小(计算字体宽度需要)*/
//          gui_select_font_size( s_current_font_layout.font_size );

        *temp_add = (char)gui_dc_get_char_dist_x( s_hdc, char_add, encoding );
        return *temp_add;
    }

}

static int _proc_sys_msg( msg_apps_t* msg )
{
    switch( msg->type )
    {
        case MSG_EBOOK_UPDATE_TOTAL_PAGE :
        {
            update_cur_total_page();
            break;
        }

        case MSG_GSENSOR_ROTATION_CHANGED:
        {
            print_dbg(" receive  MSG_GSENSOR_ROTATION_CHANGED");

            _scene_rotate();
            break;
        }

        case MSG_CARD_OUT:
        {
            char* temp = NULL; 
            
            print_dbg(" receive  MSG_CARD_OUT");

            if ( s_msgbox != NULL )
            {
                msgbox_proc_sys( s_msgbox, msg );
            }
	
            temp = strstr( s_file_full_path, CARD_ROOT );
            if ( temp != NULL )
            {
                clear_current_reading_file();

                exit_msg_loop();
                g_ebook_main->go_back_last_scene();             
            }
                          
            break;
        }

        case MSG_USB_HOST_DISK_OUT:
        {
            char* temp = NULL;
            
            print_dbg(" receive  MSG_USB_HOST_DISK_OUT");

            if ( s_msgbox != NULL )
            {
                msgbox_proc_sys( s_msgbox, msg );
            }

            temp = strstr( s_file_full_path, EXTERNAL_ROOT );
            if ( temp != NULL )
            {
                clear_current_reading_file();

                exit_msg_loop();
                g_ebook_main->go_back_last_scene();
            }
                          
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

void add_current_page_to_bookmark(void)
{
    add_bookmark_result_e ret;
    dialog_init_t init;
    
    ret = g_scene_bookmark->add_a_bookmark( s_file_full_path, s_file_name, s_current_page,
                                        (unsigned int)s_current_page_start_point, (unsigned int)s_font_type );
    if ( ret == ADD_BOOKMARK_SUCCESS )
    {
        return;
    }

    if ( ret == BOOKMARK_FULL )
    {
        init.str_id = s_bookmark_full_string->str_id;
    }
    else
    {
        init.str_id = s_add_bookmark_fail_string->str_id;
    }
    
    init.head = DIALOG_HEAD_WARN;
    init.def_sel = 0;

    dialog_modal( &init );

    
}

void set_bookmark( plist_detail_info_t* bookmark )
{
    unsigned int current_page;
    int current_point;
    font_type_e type;

    current_page = (unsigned int)bookmark->channels;
    current_point = (int)bookmark->rating;
    type = (font_type_e)bookmark->markinfo.time;

    if ( ( strcmp( s_file_full_path, bookmark->fullpath ) == 0 )
        && ( _check_file_same_length( s_file_full_path, bookmark->fullpath ) == true ) 
        && ( s_current_page == current_page ) 
        && ( s_current_page_start_point == current_point )
        && ( s_font_type == type ) )
    {
        /* 还是旧书 */
        
        return;
    }


    memset( s_file_full_path, 0, PATH_MAX );
    memset( s_file_name, 0, PATH_MAX );
    strcpy( s_file_full_path, bookmark->fullpath );
    strcpy( s_file_name, bookmark->title );

    s_current_page = current_page; /* 因为显示当前页时，会加 1 */
    s_current_page_start_point = current_point;
    s_font_type = type;
    s_new_book = true;
}

static bool _check_file_same_length( char* first, char* second )
{
    int len1;
    int len2;

    struct stat state;

    stat( first, &state );
    len1 = (int)state.st_size;

    memset( &state, 0 , sizeof( struct stat ) );
    stat( second, &state );
    len2 = (int)state.st_size;

    if ( len1 == len2 )
    {
        return true;
    }

    return false;
}

void set_language_mode( unsigned int language )
{
    s_language_mode = language;

    gui_set_language_mode( (int)language );
}

unsigned int get_language_mode(void)
{
    return s_language_mode;
}

void change_language_mode( unsigned int language )
{
    if ( language == s_language_mode )
    {
        return;
    }

    s_language_mode = language;
    gui_set_language_mode( (int)language );

    _get_ascii_width();
    memset( s_font_width_buffer, 0 , FONT_WIDTH_BUFFER_WIDTH );

    /* 重画当前页 */
    _repaint_current_page();

    /* 页数已经错误，需要重数 */
    _stop_count_page();
    _start_count_page();
}



static void _scene_rotate(void)
{
    bool ret;
    fusion_effect_t effect;

    fusion_get_config( FUSION_TYPE_ROTATE, FUSION_FORWARD, &effect );
    fusion_set_effect( &effect );
    fusion_set_first_frame( NULL );
    
    if ( s_scene != NULL )
    {
        g_style_api->free_scene( s_scene );
        s_scene = NULL;    
    }
    s_scene_read_ui->deinit();


    /* 重新装载画面资源 */
    ret = _load_style_resources();
    if ( ret == false )
    {
        print_err(" _load_style_resources() failed ");
        return;
    }
    
    
    g_ebook_main->change_common_ui_scene();
    
    /* 改变窗口大小 */
    gui_wm_set_size( s_window, s_scene->width, s_scene->height );

    g_ebook_main->update_background_picture();
    _paint_background();

    /* 重画 text 画面 */
    s_current_font_layout = s_font_layout[ s_font_type ];
    s_scene_read_ui->paint_initial_ui( s_control_mode , s_file_name );
    _repaint_current_page();

    /* 页数已经错误，需要重数 */
    _stop_count_page();
    _start_count_page();

    if ( s_headbar != NULL )
    {
        headbar_load_style( s_headbar );
        headbar_paint( s_headbar );
    }

    if ( s_menuview != NULL )
    {
        menuview_load_style( s_menuview, MENUVIEW_OPTION );
        menuview_paint( s_menuview );
    }

    if ( s_dialog != NULL )
    {
        dialog_load_style( s_dialog );
        dialog_paint( s_dialog );
    }

    fusion_set_last_frame( NULL );
    fusion_display( NULL );
}




bool close_reading_book(void)
{
    _stop_count_page();

    kill_update_page_timer();

    if ( is_reading() == false )
    {
        return true;
    }

    s_book_buffer->close_old_book();

    return true;
}

bool _start_count_page(void)
{
    bool ret;

    ret = s_count_page->start_count_page( s_ascii_width, s_font_width_buffer );
    
    return ret;
}

void _stop_count_page(void)
{
    s_count_page->stop_count_page();
}

static int _proc_update_page_timer( void * param )
{
    update_cur_total_page();
    return 0;
}

void kill_update_page_timer(void)
{    
    if ( s_update_page_timer != NULL_TIMER )
    {
        usleep( TOTAL_PAGE_UPDATE_SPEED * 1000 );
    
        kill_timer( s_update_page_timer );
        s_update_page_timer = NULL_TIMER;
    }

}


bool count_page_finished(void)
{
    if ( s_count_page->is_running() == true )
    {
        return false;
    }

    return true;
}

void restart_auto_reading_timer(void)
{
    if ( s_control_mode == AUTO_READING )
    {
        if ( s_auto_read_timer != NULL_TIMER )
        {
            restart_timer( s_auto_read_timer );
        }
    }
}

void clear_option_menuview( void )
{
    if ( s_menuview != NULL )
    {
        s_menuview = NULL;
    }
}

int get_current_page_start_point(void)
{
    return s_current_page_start_point;
}

void set_current_page_start_point( int point )
{
    s_current_page_start_point = point;
}

void revise_current_page_start_point( int point )
{
    s_current_page_start_point = point;

    /* 重画当前页 */
    _repaint_current_page();
}



void set_current_page_num( unsigned int page )
{
    print_info(" point === %d  ,,,,  s_current_page  ======= %d  ", s_current_page_start_point, page );
    s_current_page = page;
}

void set_fusion_mode( fusion_mode_e mode )
{
    s_fusion_mode = mode;
}

fusion_mode_e get_fusion_mode(void)
{
    return s_fusion_mode;
}

void set_fusion_rect( short x, short y, short width, short height )
{
    s_fusion_rect.x = x;
    s_fusion_rect.y = y;
    s_fusion_rect.width = width;
    s_fusion_rect.height = height;
}

void set_option_has_change_size_item(void)
{
    s_read_option->set_has_change_size_item();
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

static void _msgbox_callback(msgbox_t * msgbox, msgbox_sig_e signal )
{
}

scene_read_t* create_scene_read(void)
{
    s_book_buffer = create_book_buffer();
    s_scene_read_ui = create_scene_read_ui();
    s_count_page = create_count_page();
    s_read_option = create_scene_read_option();

    return &s_scene_read_api;
}

