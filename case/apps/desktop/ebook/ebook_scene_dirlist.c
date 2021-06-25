/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : ebook_scene_dirlist.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    zhaowenjie    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     ebook_scene_dirlist.c
* \brief    dirlist scene
* \author   zhaowenjie
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#include "ebook_scene_dirlist.h"

#include "ebook.h"

static scene_t* s_scene = NULL;
static int s_window = 0;
static headbar_t* s_headbar = NULL;

static dirlist_t* s_dirlist = NULL; 
static volumebar_t* s_volumebar = NULL;
static msgbox_t* s_msgbox = NULL;
static softkeyboard_t* s_softkeyboard = NULL;
static loadingbox_t* s_loadingbox = NULL;
static int s_search_num = 0;

void enter_scene_dirlist(void);
static bool _scene_dirlist_init(void);
static bool _show_scene_dirlist(void);
static void _paint_background(void);
static bool _create_widgets(void);

static int _proc_sys_msg( msg_apps_t *msg );
static void _proc_gui_msg(gui_wm_message_t *pmsg);
static void _proc_key( key_event_t *msg );
static void _scene_rotate(void);
    
static bool _scene_dirlist_deinit(void);

FSEL_HANDLE get_fsel_handle( void );

void remove_dirlist( void );

static bool _create_softkeyboard_widget( void );
static bool _delete_softkeyboard_widget( void );
static void _softkeyboard_callback(softkeyboard_t *keyboard, softkeyboard_sig_e signal);
static bool _create_search_thread(void);
static void* _deal_searching( void* arg );
static void _searching_done( void );

static bool _create_loadingbox_widget( void );
static bool _delete_loadingbox_widget( void );
static void _loadingbox_widget_cb(loadingbox_t *loadingbox, loadingbox_sig_e signal);

static void _volumebar_callback(volumebar_t *data, volumebar_sig_e signal);
static void _headbar_callback(headbar_t * headbar, widget_sig_headbar_e signal );
static void _dirlist_callback( dirlist_t* dirlist, dirlist_sig_e signal );
static void _msgbox_callback(msgbox_t * msgbox, msgbox_sig_e signal );

static scene_dirlist_t s_scene_dirlist_api = 
{   
    enter_scene_dirlist,

    remove_dirlist,
    
    get_fsel_handle
};

/* scene entry function */
void enter_scene_dirlist(void )
{
    bool ret;
    msg_apps_t  msg;
    hotplugin_dev_type_e main_disk ; 
    dialog_init_t dialog_init;

    print_info(" ************   enter  scene_dirlist  ************* "); 
    
    /* 场景初始化 */
    ret = _scene_dirlist_init( );
    if(ret == false )
    {
        print_err("dirlist scene init error! \n");
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

    while( get_msg(&msg) == true )
    {
        dispatch_msg(&msg);
    }

    unregister_sys_dispatcher();

    ERROR_EXIT:

    /* 场景退出 */
    ret = _scene_dirlist_deinit( );
    if ( ret == false )
    {
        printf(" dirlist scene exit error! \n");
        g_ebook_main->set_current_scene( SCENE_EXIT );
    }

    print_info(" ************  exit  scene_dirlist  ************* ");
}


/* scene initial function */
static bool _scene_dirlist_init(void)
{
    bool ret;
    fusion_enter_e fenter;
    fusion_effect_t eff;

    if ( g_ebook_main->get_last_scene() == SCENE_MAIN_MENU )
    {
        fenter = FUSION_FORWARD;
    }
    else
    {
        fenter = FUSION_BACKWARD;
    }

    fusion_get_config( FUSION_TYPE_COMMON, fenter, &eff );
    fusion_set_effect( &eff );
    fusion_set_first_frame( NULL );
    
    ret = _show_scene_dirlist();
    if ( ret == false)
    {
        print_err(" scene_dirlist_init \n" ); 
        return false;
    }

    fusion_set_last_frame( NULL );
    fusion_display( NULL);

    clear_key_msg_queue( -1, -1 );
        
    return true;
}


static bool _show_scene_dirlist(void)
{   
    bool ret = true;
    rotation_e angle;
    
    angle = g_sty_dc->sty_get_view_rotation();
    ret = g_sty_dc->sty_set_screen_rotation( angle );
    if ( ret == false )
    {
        print_err( "sty_set_screen_rotation failure \n" );
    }

    s_scene = g_style_api->load_scene( g_ebook_main->get_style(), DIRLIST_UPRIGHT_SCENE );
    if( s_scene == NULL)
    {
        print_err(" dirlist load scene failure %s---%d---\n",__FILE__,__LINE__); 
        return false;
    }
            
    /* 创建窗口 */
    if ( ( angle == ROTATION_0 ) ||  ( angle == ROTATION_180 ) )
    {
        s_window = gui_wm_create_window( 0, 0, s_scene->height, s_scene->width,
                                    WM_CREATE_SHOW, _proc_gui_msg, 0);
    }
    else
    {
        s_window = gui_wm_create_window( 0, 0, s_scene->width, s_scene->height,
                                    WM_CREATE_SHOW, _proc_gui_msg, 0);
    }
    
    ret = _create_widgets();
    if ( ret == false )
    {
        return false;
    }

    gui_wm_set_focus( s_window );

    _paint_background();
    headbar_paint( s_headbar );
    dirlist_paint( s_dirlist );
    
    return true;
}

static bool _create_widgets(void)
{
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

    if ( s_dirlist == NULL )
    {
        int result;
        char ext_ebook[PATH_MAX];
        dirlist_init_t dir_init;

        memset( &dir_init, 0, sizeof( dirlist_init_t ) );
        dir_init.type = DIRLIST_EBOOK;

        /* read ext config */
        result = get_config(CFG_APP_SUPPORT_EXT_EBOOK, (char*) ext_ebook, sizeof(ext_ebook));
        dir_init.filter_string = ext_ebook;
        
        s_dirlist = dirlist_create( &dir_init, _dirlist_callback );
        if ( s_dirlist == NULL )
        {
            print_err("dirlist_create NULL .......");
            return false;
        }
    }
    else
    {    
        dirlist_show( s_dirlist );
    }
    
    s_msgbox = msgbox_create( _msgbox_callback );
    if ( s_msgbox == NULL )
    {
        print_err(" msgbox_create NULL .......");
        return false;
    }

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

    if ( pmsg->msgid == WM_MSG_KEY )
    {
        kmsg = (key_event_t *)(pmsg->data.p); /*get key msg*/
        _proc_key( kmsg );
    }

    #pragma __PRQA_IGNORE_START__

    if ( ( s_dirlist != NULL ) 
            && ( s_dirlist->canvas_id == top ) )
    {
        dirlist_proc_gui( s_dirlist, pmsg );
    }
    else if ( ( s_volumebar != NULL ) 
            && ( s_volumebar->canvas_id == top ) )
    {
        volumebar_proc_gui( s_volumebar, pmsg );
    }
    else if ( ( s_msgbox != NULL ) 
            && ( s_msgbox->canvas_id == top ) )
    {
        msgbox_proc_gui( s_msgbox, pmsg );
    }
    else if ( ( s_softkeyboard != NULL) && ( top == s_softkeyboard->canvas_id) )
    {
        softkeyboard_proc_gui( s_softkeyboard, pmsg );
    }    
    else if ( ( s_loadingbox != NULL) && (top == s_loadingbox->canvas_id) )
    {
        loadingbox_proc_gui( s_loadingbox, pmsg );
    }

    #pragma __PRQA_IGNORE_END__

    clear_key_msg_queue(-1, -1);
}

static int _proc_sys_msg( msg_apps_t *msg )
{
    switch(msg->type)
    {
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
            if( s_dirlist != NULL )
            {
                dirlist_proc_sys( s_dirlist, msg ); /*内部会处理卡，U盘拔插消息. */
            }
            
            if ( s_msgbox != NULL )
            {
                msgbox_proc_sys( s_msgbox, msg );
            }
        }
    }
    
    return 0;
}

/* scene release function */
static bool _scene_dirlist_deinit(void)
{
    headbar_delete(s_headbar);
    s_headbar = NULL;
    
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

    if ( g_ebook_main->get_current_scene() == SCENE_READ )
    {        
        if ( s_dirlist != NULL )
        {
            /* 从阅读界面回来时，需要显示原来一样的 */
            dirlist_hide( s_dirlist );
        }
    }
    else
    {
        remove_dirlist();
    }

    /* 删除窗体 */
    gui_wm_delete_window( s_window );

    return true;
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

static void _dirlist_callback( dirlist_t* dirlist, dirlist_sig_e signal )
{
    switch( signal )
    {
        case WIDGET_SIG_DIRLIST_QUIT:
        {
            exit_msg_loop(); 
            g_ebook_main->set_current_scene( SCENE_MAIN_MENU );

            break;
        }
            
        case WIDGET_SIG_DIRLIST_OK:
        {    
            g_scene_read->set_file_path( dirlist->path_name, dirlist->file_name );
                
            exit_msg_loop(); 
            g_ebook_main->set_current_scene( SCENE_READ );
            
            break;
        }

        /* search */
        case WIDGET_SIG_DIRLIST_SHORT_MENU:
        {    
            _create_softkeyboard_widget();            
            break;
        }
            
        default:
        {
            break;
        }
    }
}

void remove_dirlist( void )
{
    if ( s_dirlist != NULL )
    {
        dirlist_delete( s_dirlist );
        s_dirlist = NULL;
    }
}

static void _scene_rotate(void)
{
    bool ret;
    rotation_e angle;
    fusion_effect_t effect;

    fusion_get_config( FUSION_TYPE_ROTATE, FUSION_FORWARD, &effect );
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

    if ( s_dirlist != NULL )
    {
        dirlist_load_style( s_dirlist );
        dirlist_paint( s_dirlist );
    }

    fusion_set_last_frame( NULL );
    fusion_display( NULL );
}

static bool _create_softkeyboard_widget( void )
{
    //create widget
    s_softkeyboard = softkeyboard_create(IME_TYPE_PINYIN, _softkeyboard_callback );
    if( s_softkeyboard == NULL )
    {
        print_err("create softkeyboard widget failed!");
        return false;
    }
    else
    {
        //hide dirlist
        dirlist_hide( s_dirlist );
       
        softkeyboard_paint( s_softkeyboard );
    }

    return true;
}

static bool _delete_softkeyboard_widget( void )
{
    softkeyboard_delete( s_softkeyboard );
    s_softkeyboard = NULL;

    return true;
}

static void _softkeyboard_callback(softkeyboard_t *keyboard, softkeyboard_sig_e signal)
{
    switch ( signal ) 
    {
        case WIDGET_SIG_SKB_OK:
        {
            bool ret;
            
            /* create loadingbox */
            _create_loadingbox_widget();

            /* create thread to search */
            ret = _create_search_thread();
            if ( ret == true )
            {
                break;
            }
            else
            {
                _delete_loadingbox_widget();
                /* 不用 break */
            }
        }

        case WIDGET_SIG_SKB_CANCEL:
        {
            //delete softkeyboard
            _delete_softkeyboard_widget();
            
            //show dirlist
            dirlist_show( s_dirlist);
            
            //update dirlist
            dirlist_paint( s_dirlist );
            break;
        }
            
        default:
        {
            break;
        }
    }
}

static bool _create_loadingbox_widget( void )
{
    if( s_loadingbox == NULL)
    {
        s_loadingbox = loadingbox_create( LOADINGBOX_SEARCHING, _loadingbox_widget_cb );
        if ( NULL == s_loadingbox )
        {
            print_err("loadingbox_create failed\n\n");
            return false;
        }
        else
        {
            //paint
            loadingbox_paint( s_loadingbox );    
        }
    }

    return true;
}

static bool _create_search_thread(void)
{
    pthread_t tid;
    pthread_attr_t pthread_attr;
    struct sched_param self_param;
    int policy;
    int result = -1;

    /*
     * process manger's msg loop should be activated as long as there is
     * a msg in the process manager's msg queue
     */
    pthread_attr_init(&pthread_attr);
    pthread_attr_setinheritsched(&pthread_attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setdetachstate(&pthread_attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setscope(&pthread_attr, PTHREAD_SCOPE_PROCESS);
    
    //创建比主进程低一级的线程, 保证主进程先运行
    pthread_getschedparam( pthread_self(), &policy, &self_param );
    self_param.sched_priority = self_param.sched_priority - 2;
    pthread_attr_setschedparam(&pthread_attr, &self_param);
    
    print_dbg(" manager_msg_loop  thread's priority is: %d\n", self_param.sched_priority );

    result = pthread_create( &tid, &pthread_attr, _deal_searching, NULL );
    pthread_attr_destroy( &pthread_attr );
    if( result != 0 )
    {
        print_err("can’t create thread, result:%d\n", result);
        return false;
    }
    
    return true;
}

static void* _deal_searching( void* arg )
{
    char text[128];
     
    /* get text */
    softkeyboard_get_text( s_softkeyboard, text, sizeof(text) );    
    
    /* set filter string */
    s_search_num = dirlist_set_search_string( s_dirlist, text );
    
    /* process after searching */
    set_single_shot_timer( 1, (timer_proc)_searching_done, NULL );

    return NULL;
}

static void _searching_done( void )
{ 
    dialog_init_t dialog_init;

    /* delete searching dialog */
    _delete_loadingbox_widget();
    
    if ( s_search_num <= 0 )
    {
        //clear filter string
        dirlist_set_search_string( s_dirlist, "" );
    
        /* show cann't found dialog */
        dialog_init.head = DIALOG_HEAD_WARN;
        dialog_init.str_id = g_ebook_main->get_not_found_string()->str_id;
        dialog_modal( &dialog_init );
    }
    else
    {     
        /* delete softkeyboard */
        _delete_softkeyboard_widget();

        /* show dirlist */
        dirlist_show( s_dirlist );

        /* update dirlist */
        dirlist_paint( s_dirlist );  
    }
}

static bool _delete_loadingbox_widget( void )
{    
    if( s_loadingbox != NULL)
    {
        loadingbox_delete( s_loadingbox );
        s_loadingbox = NULL;
    }
    
    return true;
}

FSEL_HANDLE get_fsel_handle( void )
{
    FSEL_HANDLE fsel;
    
    if ( s_dirlist == NULL )
    {
        return NULL;
    }

    fsel = dirlist_get_fsel_handle( s_dirlist );

    return fsel;
}

static void _loadingbox_widget_cb(loadingbox_t *loadingbox, loadingbox_sig_e signal)
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

static void _headbar_callback(headbar_t * headbar, widget_sig_headbar_e signal )
{
}

static void _msgbox_callback(msgbox_t * msgbox, msgbox_sig_e signal )
{    
}

scene_dirlist_t* create_scene_dirlist(void)
{
    return &s_scene_dirlist_api;
}

