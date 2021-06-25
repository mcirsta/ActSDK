/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_scene_list.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_scene_list.c
* \brief    list scene
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#include "music.h"


/* global variable define */
scene_list_t* g_scene_list = NULL;


/* internal static callback function declaration */
static void _headerbar_callback( headbar_t *headbar, widget_sig_headbar_e signal );
static void _volumebar_callback( volumebar_t *volumebar, volumebar_sig_e signal );
static void _widget_menu_callback( menuview_t *menuview, menuview_sig_e signal );
static void _msgbox_callback( msgbox_t * msgbox, msgbox_sig_e signal);

/* internal static scene function declaration */
static bool _scene_list_open_theme( void );
static bool _scene_list_close_theme( void );
static bool _scene_list_open_str_res( void );
static bool _scene_list_close_str_res( void );
static bool _scene_list_set_default_font(void);
static bool _scene_list_unset_default_font(void);

static bool _scene_list_init( fusion_enter_e fenter );
static bool _scene_list_deinit( void );
static bool _scene_list_load_style( void );
static bool _scene_list_proc_gui( gui_wm_message_t *pmsg );
static bool _scene_list_proc_sys( msg_apps_t *msg );
static void _scene_list_callback( gui_wm_message_t *pmsg );
static bool _scene_list_rotation( void );


static void _headerbar_callback( headbar_t *headbar, widget_sig_headbar_e signal )
{
    print_dbg();
}

static void _volumebar_callback( volumebar_t *volumebar, volumebar_sig_e signal )
{
    switch ( signal ) 
    {
    case WIDGET_SIG_VOLUMEBAR_CHANGED:
        break;
         
    case WIDGET_SIG_VOLUMEBAR_QUIT:        
        volumebar_delete( volumebar );
        g_scene_list->vbar = NULL;
        break;
         
    default:
        break;
    }
}


static void _widget_menu_callback( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_QUIT:
        scene_list_quit();
        break;
         
    default:
        break;
    }    
}

static void _msgbox_callback( msgbox_t * msgbox, msgbox_sig_e signal)
{
    switch (signal)
    {
    case WIDGET_SIG_MSGBOX_SHOW:
        print_dbg("show message box");
        break;
    case WIDGET_SIG_MSGBOX_QUIT:
        print_dbg("quit message box");
        break;
    default:
        break;
    }
}

static bool _scene_list_open_theme( void )
{
    rotation_e angle;
    char file[PATH_MAX];
    bool is_upright;

    // 取视图角度
    angle = sty_get_view_rotation();

    // 载入commonui的 style 场景 
    if ( g_coverflow_mode == true )
    {
        is_upright = false;
        
        if ( angle == ROTATION_90 )
        {
            angle = ROTATION_0;
        }
        else if ( angle == ROTATION_270 )
        {
            angle = ROTATION_180;
        }
        else
        {
        }
    }
    else if ( ( angle == ROTATION_0 ) || ( angle == ROTATION_180 ) )
    {
        is_upright = false;
    }
    else //if ( ( angle == ROTATION_90 ) || ( angle == ROTATION_270 ) )
    {
        is_upright = true;
    } 
    
    // 设置屏幕方向 
    sty_set_screen_rotation(angle);

    if ( true == is_upright )
    {        
        // 获取commonui 的 style 资源文件名 
        g_scene_list->common_scene = load_scene( g_common_style, COMMONUI_UPRIGHT_SCENE );        
      
        // 获取应用的 style 资源文件名
        g_scene_list->scene = load_scene( g_music_style, LIST_UPRIGHT_SCENE );
    }
    else
    {
        // 获取commonui 的 style 资源文件名 
        g_scene_list->common_scene = load_scene( g_common_style, COMMONUI_SIDELONG_SCENE );

        // 获取应用的 style 资源文件名
        g_scene_list->scene = load_scene( g_music_style, LIST_SIDELONG_SCENE );
    }

    if ( g_scene_list->common_scene == NULL )
    {
        print_err("load commonui scene failed");
        return false;
    }
    else
    {
        // 设置 commonui 的 style 场景
        sty_set_common_scene(g_scene_list->common_scene);
    }

    if ( g_scene_list->scene == NULL )
    {
        print_err("load scene LIST_SCENE failed");
        return false;
    }
    else
    {
        // 设置应用的 style 场景 
        sty_set_app_scene(g_scene_list->scene);
    }

    // 获取commonui 的图片资源文件名 
    sprintf(file, "%s/%s.res", sys_get_common_theme_res_dir(), "commonui");

    // 打开 commonui 图片资源文件 
    if( sty_open_common_pic_res(file) == false )
    {
        print_err("open %s failed!", file);
        return false;
    }

    // 获取commonui 的图片资源文件名
    sprintf(file, "%s/%s.res", sys_get_app_theme_res_dir(), "music");

    // 打开应用的图片资源文件
    if( sty_open_app_pic_res(file) == false )
    {
        print_err("open %s failed!", file);
        return false;
    }

    return true;
}

static bool _scene_list_close_theme(void)
{
    free_scene( g_scene_list->common_scene );
    g_scene_list->common_scene = NULL;

    free_scene( g_scene_list->scene );
    g_scene_list->scene = NULL;

    if( sty_close_common_pic_res() == false )
    {
        print_err("close common picture resource error!");
        return false;
    }

    if( sty_close_app_pic_res() == false )
    {
        print_err("close app picture resource error!");
        return false;
    } 

    return true;
}

static bool _scene_list_open_str_res( void )
{
    char file[PATH_MAX];  

    // 获取commonui 字符串资源文件名 
    sprintf(file, "%s/%s_%s.str", sys_get_common_string_res_dir(), "commonui", sys_get_cur_language_codes());

    // 打开 commonui 字符串资源文件 
    if( sty_open_common_str_res(file) == false )
    {
        print_err("open %s failed!", file);
        return false;
    }

    // 获取setting应用字符串资源文件名
    sprintf(file, "%s/%s_%s.str", sys_get_app_string_res_dir(), "music", sys_get_cur_language_codes());

    // 打开应用的字符串资源文件 
    if( sty_open_app_str_res(file) == false )
    {
        print_err("open %s failed!", file);
        return false;
    }

    // 设置语言编码
    sty_set_text_encoding(GUI_UTF8_ENC);

    // 设置语言模式
    gui_set_language_mode( sys_get_cur_language_id() );

    return true;
}

static bool _scene_list_close_str_res( void )
{
    if( sty_close_common_str_res() == false )
    {
        print_err("close common string resource error!");
        return false;
    }

    if( sty_close_app_str_res() == false )
    {
        print_err("close app string resource error!");
        return false;
    } 

    return true;
}

static bool _scene_list_set_default_font(void)
{
    /* crate font */
    if(g_scene_list->fontdes > 0)
    {
        return false;
    }

    g_scene_list->fontdes = gui_create_font((unsigned char*)sys_get_default_font_file(), 16);
    if(g_scene_list->fontdes < 0)
    {
        print_err("call gui_create_font failed");
        return false;
    }

    if(gui_dc_set_default_fontface(g_scene_list->fontdes) < 0) 
    {
        print_err("gui_dc_set_default_fontface failed");
        return false;
    }

    return true;
}

static bool _scene_list_unset_default_font(void)
{
    /* crate font */
    if(g_scene_list->fontdes <= 0)
    {
        return false;
    }

    gui_destroy_font(g_scene_list->fontdes);
    g_scene_list->fontdes = 0; 
   
    return true;
}

/* scene initial function */
static bool _scene_list_init( fusion_enter_e fenter )
{ 
    hotplugin_dev_type_e main_disk ; 
    dialog_init_t dialog_init;
    headbar_init_t headbar_init;   
    
    //分配场景数据空间
    if( g_scene_list == NULL )
    {
        g_scene_list = malloc( sizeof(scene_list_t) );
        
        if( g_scene_list == NULL )
        {
            print_err();
            goto malloc_err;
        }
        else
        {
            memset( g_scene_list, 0, sizeof(scene_list_t) );    //clear
        }
    }

    //保存场景数据空间指针
    g_scene_list->exit_type = SCENE_LIST_RETURN;
    g_scene_list->win_id = -1;

    /* load theme 资源文件 */
    if( _scene_list_open_theme() == false )
    {
        goto theme_err;
    }

    /* load 字符串资源文件 */
    if( _scene_list_open_str_res() == false )
    {
        goto str_err;
    }

    /* 创建字体 */
    if( _scene_list_set_default_font() == false )
    {
        goto font_err;
    }

    //load style
    _scene_list_load_style();

    //判断主盘是否存在
    main_disk = sys_get_main_disk();
    if(main_disk == HOTPLUGIN_DEV_TYPE_NONE)
    {
        //模态提示框
        if (g_scene_list->str_nodisk != NULL)
        {
            dialog_init.head = DIALOG_HEAD_WARN;
            dialog_init.str_id = g_scene_list->str_nodisk->str_id;
            dialog_modal( &dialog_init );
        }
        goto maindisk_err;
    }

     //创建窗口
    g_scene_list->win_id = gui_wm_create_window( g_scene_list->scene->x, g_scene_list->scene->y, 
                                        g_scene_list->scene->width, g_scene_list->scene->height,
                                        WM_CREATE_SHOW, _scene_list_callback, 0 );
    if( g_scene_list->win_id == -1 )
    {
        print_err(); 
        goto window_err;
    }

    //设置焦点
    gui_wm_set_focus( g_scene_list->win_id );

    //获取dc
    g_scene_list->hdc = gui_dc_get( g_scene_list->win_id );

    //create headbar
    headbar_init.type = HEADBAR_TYPE_NORMAL;
    headbar_init.pic_id_head = g_scene_list->pic_title->pic_id;
    headbar_init.str_id_title = g_scene_list->str_app_name->str_id;

    g_scene_list->hbar = headbar_create( &headbar_init, _headerbar_callback );
    if( g_scene_list->hbar == NULL )
    {
        print_err(); 
        goto headbar_err;
    }

    //create msgbox
    g_scene_list->msgbox = msgbox_create( _msgbox_callback );
    if ( NULL == g_scene_list->msgbox )
    {
        print_err("create msgbox failed!");
        goto msgbox_err;
    }

    //paint all
    scene_list_paint();     //draw static window bg        

    headbar_paint( g_scene_list->hbar );

    //create menu
    g_scene_list->list = music_list_widget_menu_create( _widget_menu_callback, fenter );
    if( g_scene_list->list == NULL )
    {
        print_err("menuview create fail!"); 
        goto list_widget_err;
    }

    //update 
    gui_screen_update();

    //clear key
    clear_key_msg_queue( -1, -1 );
    
    //extra init
    if( app_extra_init(g_app_music) == false )
    {
        //模态提示框
        if (g_scene_list->str_nospace != NULL)
        {
            dialog_init.head = DIALOG_HEAD_WARN;
            dialog_init.str_id = g_scene_list->str_nospace->str_id;
            dialog_modal( &dialog_init );
        }
        print_err("app_extra_init error! \n");
        goto list_widget_err;
    }

    //run music engine
    music_open_engine( true );    
    
    //album image engine
//    album_open_engine();    
    
    return true;

    list_widget_err:
    msgbox_delete(g_scene_list->msgbox);
    
    msgbox_err:
    headbar_delete(g_scene_list->hbar);
    
    headbar_err:
    gui_wm_delete_window(g_scene_list->win_id);
    
    window_err:
    maindisk_err:  //释放load_style中申请的资源
    sty_delete_bitmap( g_scene_list->bmp_bg );
    g_scene_list->bmp_bg = NULL;
    _scene_list_unset_default_font();
    
    font_err:
    _scene_list_close_str_res();
    
    str_err:
    _scene_list_close_theme();
    
    theme_err:
    free( g_scene_list );
    g_scene_list = NULL;
    
    malloc_err:

    return false;
}


/* scene exit function */
static bool _scene_list_deinit( void )
{
    bool bret = true;
  
    //kill timer
    kill_timer( g_scene_list->single_timer );
    g_scene_list->single_timer = -1;    

    sty_delete_bitmap( g_scene_list->bmp_bg );
    g_scene_list->bmp_bg = NULL;

    //hide
    dirlist_hide( g_dirlist );

    //删除控件
    msgbox_delete( g_scene_list->msgbox );
    g_scene_list->msgbox = NULL;
    music_list_widget_menu_delete( g_scene_list->list );
    g_scene_list->list = NULL;
    headbar_delete( g_scene_list->hbar );
    g_scene_list->hbar = NULL;
    volumebar_delete( g_scene_list->vbar );
    g_scene_list->vbar = NULL;
    dialog_delete( g_scene_list->dialog );
    g_scene_list->dialog = NULL;
    softkeyboard_delete( g_scene_list->softkeyboard );
    g_scene_list->softkeyboard = NULL;
    loadingbox_delete( g_scene_list->loadingbox );
    g_scene_list->loadingbox = NULL;

    //删除窗体
    gui_wm_delete_window( g_scene_list->win_id );
    g_scene_list->win_id = 0;    

    /* destroy font */
    _scene_list_unset_default_font();
    
    /* close theme resource file */
    if( _scene_list_close_theme() == false )
    {
        bret = false;
    }

    /* close string resource file */
    if( _scene_list_close_str_res() == false )
    {
        bret = false;
    }

    free(g_scene_list);
    g_scene_list = NULL;

    return bret;
}

static bool _scene_list_load_style( void )
{
    resgroup_resource_t *group;
    
    //load bg
    g_scene_list->pic_bg = ( picture_resource_t * )get_scene_child( g_scene_list->scene, PIC_BG );  
    g_scene_list->pic_title = (picture_resource_t *) get_scene_child( g_scene_list->scene, PIC_TITLE );
    g_scene_list->str_app_name = (string_resource_t*) get_scene_child( g_scene_list->scene, STR_APP_NAME );
    
    group = ( resgroup_resource_t * )get_scene_child( g_scene_list->scene, WIDGET_DIALOG_RESGROUP );
    g_scene_list->str_del_file = ( string_resource_t * )get_resgroup_child( group, STR_DEL_FILE );   
    g_scene_list->str_del_tag = ( string_resource_t * )get_resgroup_child( group, STR_DEL_TAG );   
    g_scene_list->str_deleting = ( string_resource_t * )get_resgroup_child( group, STR_DELETING );   
    g_scene_list->str_searching = ( string_resource_t * )get_resgroup_child( group, STR_SEARCHING );  
    g_scene_list->str_notfound = ( string_resource_t * )get_resgroup_child( group, STR_NOTFOUND ); 
    g_scene_list->str_nofile = ( string_resource_t * )get_resgroup_child( group, STR_NOFILE );   
    g_scene_list->str_notag = ( string_resource_t * )get_resgroup_child( group, STR_NOTAG );    
    g_scene_list->str_del_fail = ( string_resource_t * )get_resgroup_child( group, STR_DEL_FAIL );    
    g_scene_list->str_nodisk = ( string_resource_t * )get_resgroup_child( group, STR_NODISK );  
    g_scene_list->str_nospace = ( string_resource_t * )get_resgroup_child( group, STR_NOSPACE );
    
    g_scene_list->str_err_openfile = ( string_resource_t * )get_resgroup_child( group, STR_ERR_OPENFILE ); 
    g_scene_list->str_err_notsupport = ( string_resource_t * )get_resgroup_child( group, STR_ERR_NOTSUPPORT ); 
    g_scene_list->str_err_decode = ( string_resource_t * )get_resgroup_child( group, STR_ERR_DECODE ); 
    g_scene_list->str_err_nolicense = ( string_resource_t * )get_resgroup_child( group, STR_ERR_NOLICENSE ); 
    g_scene_list->str_err_clock = ( string_resource_t * )get_resgroup_child( group, STR_ERR_CLOCK ); 
    g_scene_list->str_err_drminfo = ( string_resource_t * )get_resgroup_child( group, STR_ERR_DRMINFO ); 
    
    //resize window
    if ( g_scene_list->win_id != -1 )
    {    
        gui_wm_set_size( g_scene_list->win_id, g_scene_list->scene->width, g_scene_list->scene->height );
    }
        
    //cache bitmap
    sty_delete_bitmap( g_scene_list->bmp_bg );

    g_scene_list->bmp_bg = get_user_bg_pic(BG_MUSIC);
      
    if ( ( g_scene_list->pic_bg != NULL ) && ( g_scene_list->bmp_bg == NULL ) )
    {
        g_scene_list->bmp_bg = sty_create_bitmap( (int)g_scene_list->pic_bg->pic_id );
    }
    
    return true;
}

/* scene gui msg process */
static bool _scene_list_proc_gui( gui_wm_message_t *pmsg )
{
    key_event_t *kmsg;
    bool result = false;
    
    switch ( pmsg->msgid ) 
    {
    case WM_MSG_CREATE:
        break;
         
    case WM_MSG_PAINT:
        print_dbg("WM_MSG_PAINT");
        break;

    case WM_MSG_KEY:
        kmsg = (key_event_t *)(pmsg->data.p);    //get key msg
        
        switch ( kmsg->val | kmsg->type ) 
        {
        case KEY_VSUB | KEY_TYPE_DOWN:
        case KEY_VADD | KEY_TYPE_DOWN:
            {
                if ( g_scene_list->vbar == NULL )
                {
                    g_scene_list->vbar = volumebar_create( 0, _volumebar_callback );                    
                    if( g_scene_list->vbar == NULL )
                    {
                        print_err(); 
                    }
                    else
                    {
                        volumebar_paint( g_scene_list->vbar );
                    }                    
                    result = true;
                }
            }
            break;
             
        default:
            break;
        } 
        break;
         
    default:
        gui_wm_default_callback(pmsg);
        break;
    }

    return result;
}

static bool _scene_list_proc_sys( msg_apps_t *msg )
{
    msg_reply_t reply;

    print_dbg("msg.type:%d\n", msg->type); 

    switch (msg->type)
    {
    case MSG_ACTIVE_SYNC:
        reply.type = MSG_REPLY_SUCCESS;
        reply_sync_msg(&reply);            
        break;
        
    case MSG_MAIN_DISK_CHANGED:
    case MSG_APP_QUIT:
        g_scene_list->exit_type = SCENE_LIST_QUIT;
        scene_list_quit();
        break;

    case MSG_GSENSOR_ROTATION_CHANGED:
        print_info("received msg: MSG_GSENSOR_ROTATION_CHANGED");
        _scene_list_rotation();
        break;
#if 0
    case MSG_GSENSOR_SHAKED:
        print_info("received msg: MSG_GSENSOR_SHAKED");
        music_play_next();
        break;
#endif        
    default:
        if(g_dirlist != NULL)
        {
            dirlist_proc_sys(g_dirlist, msg);
        }
        
        if(g_scene_list->msgbox != NULL)
        {
            /* 控件msgbox用来处理默认系统消息(低电提示、卡插入和拔出提示、USB插入功能选择提示等) */
            msgbox_proc_sys(g_scene_list->msgbox, msg);
        }
        break;
    }

    return true;
}

/* scene callback function */
static void _scene_list_callback( gui_wm_message_t *pmsg )
{
    int top;
    
    //scene process
    if ( _scene_list_proc_gui( pmsg ) == true )
    {
        return;
    }

    //top canvas process
    top = canvas_get_focus(pmsg);

    //dispatch msg
    if ( (g_scene_list->vbar != NULL) && (top == g_scene_list->vbar->canvas_id) )    //volumebar process
    {
        volumebar_proc_gui( g_scene_list->vbar, pmsg );
        return;
    }    

    if ( (g_dirlist != NULL) && (top == g_dirlist->canvas_id) )    //dirlist process
    {
        dirlist_proc_gui( g_dirlist, pmsg );
        return;
    }

    if ( (g_scene_list->dialog != NULL) && (top == g_scene_list->dialog->canvas_id) )    //dialog process
    {
        dialog_proc_gui( g_scene_list->dialog, pmsg );
        return;
    }    

    if ( (g_scene_list->softkeyboard != NULL) && (top == g_scene_list->softkeyboard->canvas_id) )
    {
        softkeyboard_proc_gui( g_scene_list->softkeyboard, pmsg );
        return;
    }    

    if ( (g_scene_list->loadingbox != NULL) && (top == g_scene_list->loadingbox->canvas_id) )
    {
        loadingbox_proc_gui( g_scene_list->loadingbox, pmsg );
        return;
    }        

    if( (g_scene_list->msgbox != NULL) && (top == g_scene_list->msgbox->canvas_id) )
    {
        /* 处理默认系统消息(低电提示、卡插入和拔出提示、USB插入功能选择提示等)的msgbox控件需要接受GUI消息 */
        msgbox_proc_gui( g_scene_list->msgbox, pmsg );
        return;
    }
    else //if ( (g_scene_list->list != NULL) && (top == g_scene_list->list->canvas_id) )    //listmenu process
    {
        music_list_widget_menu_proc_gui( g_scene_list->list, pmsg );
        return;
    }    
}

/* scene rotation and fusion effect */
static bool _scene_list_rotation( void )
{
    fusion_effect_t eff;
    
    fusion_get_config(FUSION_TYPE_ROTATE, FUSION_FORWARD, &eff);
    fusion_set_effect(&eff);
    fusion_set_first_frame(NULL);
    
    //scene rotate
    scene_list_rotation();

    fusion_set_last_frame(NULL);
    fusion_display(NULL);

    return true;
}

/* scene paint function */
bool scene_list_paint( void )
{
    //draw static main window bg
    int x, y;

    if ( g_scene_list->pic_bg != NULL )
    {
        x = g_scene_list->pic_bg->x;
        y = g_scene_list->pic_bg->y;
        sty_draw_bitmap( g_scene_list->hdc, g_scene_list->bmp_bg, x, y );
    }

    return true;
}

/* scene quit */
bool scene_list_quit( void )
{
    exit_msg_loop();
    
    return true;
}

/* scene rotation */
bool scene_list_rotation( void )
{
    //reload theme
    _scene_list_close_theme();
    _scene_list_open_theme();
    
    //reload style
    _scene_list_load_style();
    scene_list_paint();

    if( g_scene_list->vbar != NULL )
    {
        volumebar_load_style( g_scene_list->vbar );
        volumebar_paint( g_scene_list->vbar );
    }

    if( g_scene_list->hbar != NULL )
    {
        headbar_load_style( g_scene_list->hbar );
        headbar_paint( g_scene_list->hbar );
    }

    if( g_scene_list->list != NULL )
    {
        menuview_load_style( g_scene_list->list, g_scene_list->list->type );
        menuview_paint( g_scene_list->list );
    }

    if( g_dirlist != NULL )
    {
        dirlist_load_style( g_dirlist );
        dirlist_paint( g_dirlist );
    }

    if( g_scene_list->dialog != NULL )
    {
        dialog_load_style( g_scene_list->dialog );
        dialog_paint( g_scene_list->dialog );
    }

    if( g_scene_list->softkeyboard != NULL )
    {
        softkeyboard_load_style( g_scene_list->softkeyboard );
        softkeyboard_paint( g_scene_list->softkeyboard );
    }

    if( g_scene_list->loadingbox != NULL )
    {
        loadingbox_load_style( g_scene_list->loadingbox );
        loadingbox_paint( g_scene_list->loadingbox );
    }

    if( g_scene_list->msgbox != NULL )
    {
        msgbox_load_style( g_scene_list->msgbox );
        msgbox_paint( g_scene_list->msgbox );
    }
 
    if ( ( g_scene_list->list != NULL ) && ( true == g_coverflow_mode ) )
    {
        //init coverflow
        coverflow_init( g_scene_list->list->menu );
        coverflow_paint( g_scene_list->hdc, g_scene_list->list->menu );
    }
    
    dialog_modal_load_style();
    dialog_modal_paint();

    return true;
}

/* scene entry function */
int music_scene_list( fusion_enter_e fenter )
{
    bool ret;
    msg_apps_t  msg;    
    scene_list_ret_t exit_type;
    
    /* 场景初始化 */
    ret = _scene_list_init( fenter );
    if( ret == false )
    {
        print_err("scene init error! \n");
        return -1;
    }
    else
    {
        print_dbg(); 
    }
        
//    _print_relative_time(__func__, __LINE__);

    /* 场景消息循环 */ 
    register_sys_dispatcher( (sys_msg_proc)_scene_list_proc_sys );

    while( true == get_msg(&msg) )
    {
        dispatch_msg(&msg);
    }

    unregister_sys_dispatcher();

    exit_type = g_scene_list->exit_type;
    
    /* 场景退出 */
    ret = _scene_list_deinit();
    if( ret == false )
    {
        print_err("scene exit error! \n");
        return -1;
    }
    else
    {
    }
    
    return exit_type;
}

