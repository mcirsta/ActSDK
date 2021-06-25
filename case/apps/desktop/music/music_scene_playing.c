/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_scene_playing.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_scene_playing.c
* \brief    playing scene
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#include "music.h"


/* global variable define */
scene_play_t* g_scene_playing = NULL;

#define        UPDATE_TIME        100//250        //ms


/* internal static callback function declaration */
static void _headerbar_callback( headbar_t *headbar, widget_sig_headbar_e signal );
static void _volumebar_callback( volumebar_t *volumebar, volumebar_sig_e signal );
static void _widget_option_callback( menuview_t *menuview, menuview_sig_e signal );
static void _msgbox_callback( msgbox_t * msgbox, msgbox_sig_e signal);

static int _modal_info_widget( unsigned short str_id );

/* internal static scene function declaration */
static bool _scene_playing_open_theme( void );
static bool _scene_playing_close_theme( void );
static bool _scene_playing_open_str_res( void );
static bool _scene_playing_close_str_res( void );
static bool _scene_playing_set_default_font(void);
static bool _scene_playing_unset_default_font(void);

static bool _scene_playing_init( fusion_enter_e fenter );
static bool _scene_playing_deinit( void );
static bool _scene_playing_load_style( void );
static bool _scene_playing_paint( void );
static bool _scene_playing_proc_gui( gui_wm_message_t *pmsg );
static bool _scene_playing_proc_sys( msg_apps_t *msg );
static void _scene_playing_callback( gui_wm_message_t *pmsg );
static bool _scene_playing_rotation( void );

/* internal static function declaration */
static bool _playing_start_timer( void );
static bool _playing_stop_timer( void );
static bool _playing_init_status( void );
static bool _playing_init_play( void );
static bool _playing_cancel_ffb( void );
static void _playing_timer_event( void *param );    /* timer function */

static void _playing_update_button( play_status_e play_status );

static void _album_init_buffer( void );
static void _album_deinit_buffer( void );
static void _album_start_decode( void );
static void _album_decode_callback( void );


static void _headerbar_callback(headbar_t *headbar, widget_sig_headbar_e signal)
{
}

static void _volumebar_callback(volumebar_t *volumebar, volumebar_sig_e signal)
{
    switch ( signal ) 
    {
    case WIDGET_SIG_VOLUMEBAR_CHANGED:
        print_dbg("SIG_VOL_CHANGED: %s---%d---\n",__FILE__,__LINE__);
        break;
         
    case WIDGET_SIG_VOLUMEBAR_QUIT:
        print_dbg("SIG_VOL_DONE: %s---%d---\n",__FILE__,__LINE__);   
        volumebar_delete( volumebar );
        g_scene_playing->vbar = NULL;
        break;
         
    default:
        break;
    }
}

static void _widget_option_callback(menuview_t *menuview, menuview_sig_e signal)
{
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_QUIT:
        //delete option
        music_playing_widget_option_delete( menuview );
        g_scene_playing->opt = NULL;
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

static int _modal_info_widget( unsigned short str_id )
{
    int result = 0;
    dialog_init_t dialog_init;        
    dialog_init.head = DIALOG_HEAD_WARN;
    dialog_init.str_id = str_id;

    result = dialog_modal( &dialog_init );
    
    return result;
}

static bool _scene_playing_open_theme( void )
{
    rotation_e angle;
    char file[PATH_MAX];

    // 取视图角度
    angle = sty_get_view_rotation();

    // 设置屏幕方向 
    sty_set_screen_rotation(angle);

    // 载入commonui的 style 场景 
    if ( ( angle == ROTATION_0 ) || ( angle == ROTATION_180 ) )
    {
        // 获取commonui 的 style 资源文件名 
        g_scene_playing->common_scene = load_scene( g_common_style, COMMONUI_SIDELONG_SCENE );
        
        // 获取应用的 style 资源文件名
        g_scene_playing->scene = load_scene(g_music_style, PLAYING_SIDELONG_SCENE);
    }
    else //if ( ( angle == ROTATION_90 ) || ( angle == ROTATION_270 ) )
    {
        // 获取commonui 的 style 资源文件名 
        g_scene_playing->common_scene = load_scene( g_common_style, COMMONUI_UPRIGHT_SCENE );
        
        // 获取应用的 style 资源文件名
        g_scene_playing->scene = load_scene(g_music_style, PLAYING_UPRIGHT_SCENE);
    }

    if (g_scene_playing->common_scene == NULL)
    {
        print_err("load commonui scene failed");
        return false;
    }
    else
    {
        // 设置 commonui 的 style 场景
        sty_set_common_scene(g_scene_playing->common_scene);
    }

    if (g_scene_playing->scene == NULL)
    {
        print_err("load scene LIST_SCENE failed");
        return false;
    }
    else
    {
        // 设置应用的 style 场景 
        sty_set_app_scene(g_scene_playing->scene);
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

static bool _scene_playing_close_theme( void )
{
    free_scene(g_scene_playing->common_scene);
    g_scene_playing->common_scene = NULL;

    free_scene(g_scene_playing->scene);
    g_scene_playing->scene = NULL;

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

static bool _scene_playing_open_str_res( void )
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
    sty_set_text_encoding( GUI_UTF8_ENC );

    // 设置语言模式
    gui_set_language_mode( sys_get_cur_language_id() );

    return true;
}

static bool _scene_playing_close_str_res( void )
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

static bool _scene_playing_set_default_font(void)
{
    /* crate font */
    if(g_scene_playing->fontdes > 0)
    {
        return false;
    }

    g_scene_playing->fontdes = gui_create_font((unsigned char*)sys_get_default_font_file(), 16);
    if(g_scene_playing->fontdes < 0)
    {
        print_err("call gui_create_font failed");
        return false;
    }

    if(gui_dc_set_default_fontface(g_scene_playing->fontdes) < 0) 
    {
        print_err("gui_dc_set_default_fontface failed");
        return false;
    }

    return true;
}

static bool _scene_playing_unset_default_font(void)
{
    /* crate font */
    if(g_scene_playing->fontdes <= 0)
    {
        return false;
    }

    gui_destroy_font(g_scene_playing->fontdes);
    g_scene_playing->fontdes = 0; 
   
    return true;
}

/* scene initial function */
static bool _scene_playing_init( fusion_enter_e fenter )
{
    bool bret; 
    fusion_effect_t eff;
    headbar_init_t headbar_init;   
//    dialog_init_t dialog_init; 

    //分配场景数据空间
    if( g_scene_playing == NULL )
    {
        g_scene_playing = malloc( sizeof(scene_play_t) );
        
        if( g_scene_playing == NULL )
        {
            print_err();
            goto malloc_err;
        }
        else
        {
            memset( g_scene_playing, 0, sizeof(scene_play_t) );    //clear
        }
    }

    //保存场景数据空间指针    
    g_scene_playing->exit_type = SCENE_PLAY_RETURN;
    g_scene_playing->win_id = -1;

    /* load theme 资源文件 */
    if( _scene_playing_open_theme() == false )
    {
        goto theme_err;
    }

    /* load 字符串资源文件 */
    if( _scene_playing_open_str_res() == false )
    {
        goto str_err;
    }

    /* 创建字体 */
    if( _scene_playing_set_default_font() == false )
    {
        goto font_err;
    }

    //load scene style resource
    _scene_playing_load_style(); 

    fusion_get_config( FUSION_TYPE_COMMON, fenter, &eff );
    fusion_set_effect(&eff);

    //fusion--set first
    scene_fusion_set_first_frame( g_scene_playing->scene );        

     //创建窗口
    g_scene_playing->win_id = gui_wm_create_window(g_scene_playing->scene->x, g_scene_playing->scene->y, 
                                        g_scene_playing->scene->width, g_scene_playing->scene->height,
                                        WM_CREATE_SHOW, _scene_playing_callback, 0 );
    
    if( g_scene_playing->win_id == -1 )
    {
        print_err(); 
        goto window_err;
    }
    
    //设置焦点
    gui_wm_set_focus( g_scene_playing->win_id );    
    
    //获取dc
    g_scene_playing->hdc = gui_dc_get( g_scene_playing->win_id );

    //create headbar 
    headbar_init.type = HEADBAR_TYPE_NORMAL;
    headbar_init.pic_id_head = g_scene_playing->pic_title->pic_id;
    headbar_init.str_id_title = g_scene_playing->str_app_name->str_id;
    
    g_scene_playing->hbar = headbar_create( &headbar_init, _headerbar_callback );
    if( g_scene_playing->hbar == NULL )
    {
        print_err(); 
        goto headbar_err;
    }

    //run music engine
    music_open_engine( true );

    //album image engine
//    album_open_engine();

    //init play status
    bret = _playing_init_status(); 
    if ( bret == false )
    {
        print_err("playing init failed!");
        goto msgbox_err;
    }

    //create msgbox
    g_scene_playing->msgbox = msgbox_create( _msgbox_callback );
    if ( NULL == g_scene_playing->msgbox )
    {
        print_err("create msgbox failed!");
        goto msgbox_err;
    }

    //paint all    
    _scene_playing_paint();

    headbar_paint( g_scene_playing->hbar );

    //fusion--set last and display
    scene_fusion_set_last_frame( g_scene_playing->scene ); 
    fusion_display( NULL );

    //update 
    gui_screen_update();

    //clear key
    clear_key_msg_queue( -1, -1 );    

    //play music
    bret = _playing_init_play();
    if ( bret == false )
    {
        print_err("play file fail!");
        goto play_err;
    }

    //set nowplaying
    music_set_nowplaying( true );

    //backlight control
    if ( g_scene_playing->engine.config.display_mode == EG_LYRIC )
    {
        sys_forbid_screensaver( true );
    }

    //set timer for updating status
    _playing_start_timer();   

    return true;

    msgbox_err:
    play_err:
    headbar_delete(g_scene_playing->hbar);
    
    headbar_err:
    gui_wm_delete_window(g_scene_playing->win_id);
    
    window_err:
    _scene_playing_unset_default_font();
    
    font_err:
    _scene_playing_close_str_res();
    
    str_err:
    _scene_playing_close_theme();
    
    theme_err:
    free( g_scene_playing );
    g_scene_playing = NULL;
    
    malloc_err:

    return false;
}

/* scene exit function */
static bool _scene_playing_deinit( void )
{
    bool bret = true;
    
    //close effect 
    if ( g_scene_playing->ssf_handle != NULL )
    {
        se_ssf_close( g_scene_playing->ssf_handle );
        g_scene_playing->ssf_handle = NULL;
    }
    
    if ( g_scene_playing->wav_handle != NULL )
    {
        se_wav_close( g_scene_playing->wav_handle );
        g_scene_playing->wav_handle = NULL;
    }

    //kill album timer
    kill_timer( g_scene_playing->single_timer );
    g_scene_playing->single_timer = -1;
    
    //kill timer for updating status
    _playing_stop_timer();
    
    //try to kill scroll timer
    kill_timer( g_scene_playing->scroll_timer );
    g_scene_playing->scroll_timer = -1;

    //delete text scroll
    sty_delete_scroll_string( g_scene_playing->scroll_info );
            
    //free bitmap
    sty_delete_bitmap( g_scene_playing->bmp_bg );
    g_scene_playing->bmp_bg = NULL;
    sty_delete_bitmap( g_scene_playing->bmp_pb_bg );
    g_scene_playing->bmp_pb_bg = NULL;
    sty_delete_bitmap( g_scene_playing->bmp_pb_full );
    g_scene_playing->bmp_pb_full = NULL;
    sty_delete_bitmap( g_scene_playing->bmp_effect_bg );
    g_scene_playing->bmp_effect_bg = NULL;
    sty_delete_bitmap( g_scene_playing->bmp_effect_dot );
    g_scene_playing->bmp_effect_dot = NULL;

    //deinit album
    album_abort_decode();
    _album_deinit_buffer();
    
    //删除控件
    msgbox_delete( g_scene_playing->msgbox );
    g_scene_playing->msgbox = NULL;
    headbar_delete( g_scene_playing->hbar );
    g_scene_playing->hbar = NULL;
    volumebar_delete( g_scene_playing->vbar );
    g_scene_playing->vbar = NULL;
    adjust_delete( g_scene_playing->adjust );
    g_scene_playing->adjust = NULL;
    dialog_delete( g_scene_playing->dialog );
    g_scene_playing->dialog = NULL;
    
    usereq_widget_delete( g_scene_playing->usereq );
    g_scene_playing->usereq = NULL;
    srsuser_widget_delete( g_scene_playing->srsuser );
    g_scene_playing->srsuser = NULL;
    
    //delete option
    music_playing_widget_option_delete( g_scene_playing->opt );
    g_scene_playing->opt = NULL;    

    //删除窗体
    gui_wm_delete_window( g_scene_playing->win_id );
    g_scene_playing->win_id = 0;

    /* destroy font */
    _scene_playing_unset_default_font();

    /* close theme resource file */
    if( _scene_playing_close_theme() == false )
    {
        bret = false;
    }

    /* close string resource file */
    if( _scene_playing_close_str_res() == false )
    {
        bret = false;
    }

    //backlight control
    if ( g_scene_playing->engine.config.display_mode == EG_LYRIC )
    {
        sys_forbid_screensaver( false );
    }    

    free(g_scene_playing);
    g_scene_playing = NULL;

    return bret;
}

static bool _scene_playing_load_style( void )
{
    scene_t *scene;
    resgroup_resource_t *group;
    int cur_index;
    
    unsigned int eq_id[EQ_MAX] =
    {
        STR_NORMAL,
        STR_POP,
        STR_CLASSIC,
        STR_SOFT,
        STR_JAZZ,
        STR_ROCK,
        STR_DBB,
        STR_CUSTOM,
        STR_SRSWOW,
        STR_SRSWOWHD,
        STR_SRSUSER,
    };
    
    //load control group
    scene = g_scene_playing->scene;
    g_scene_playing->resgroup = ( resgroup_resource_t * )get_scene_child( scene, PLAYING_RESGROUP );
    group = g_scene_playing->resgroup;
    
    //load bg
    g_scene_playing->pic_bg = ( picture_resource_t * )get_scene_child( scene, PIC_BG );  
    g_scene_playing->pic_title = (picture_resource_t *) get_scene_child( scene, PIC_TITLE );
    g_scene_playing->str_app_name = (string_resource_t*) get_scene_child( scene, STR_APP_NAME );

    g_scene_playing->pic_disk = ( picture_resource_t * )get_resgroup_child( group, PIC_DISK );   

    g_scene_playing->pic_album_bg = ( picture_resource_t * )get_resgroup_child( group, PIC_ALBUM_BG );   
    g_scene_playing->pic_def_album = ( picture_resource_t * )get_resgroup_child( group, PIC_DEF_ALBUM );  
    g_scene_playing->str_index = ( string_resource_t * )get_resgroup_child( group, STR_INDEX ); 
    g_scene_playing->str_title = ( string_resource_t * )get_resgroup_child( group, STR_TITLE ); 
  
    g_scene_playing->pic_artist = ( picture_resource_t * )get_resgroup_child( group, PIC_ARTIST );  
    g_scene_playing->str_artist = ( string_resource_t * )get_resgroup_child( group, STR_ARTIST ); 
    g_scene_playing->pic_album = ( picture_resource_t * )get_resgroup_child( group, PIC_ALBUM );  
    g_scene_playing->str_album = ( string_resource_t * )get_resgroup_child( group, STR_ALBUM ); 
    g_scene_playing->pic_genre = ( picture_resource_t * )get_resgroup_child( group, PIC_GENRE );  
    g_scene_playing->str_genre = ( string_resource_t * )get_resgroup_child( group, STR_GENRE ); 

    g_scene_playing->pic_effect_bg = ( picture_resource_t * )get_resgroup_child( group, PIC_EFFECT_BG );
    g_scene_playing->pic_effect_dot = ( picture_resource_t * )get_resgroup_child( group, PIC_EFFECT_DOT );
//    g_scene_playing->pic_effect_top = ( picture_resource_t * )get_resgroup_child( group, PIC_EFFECT_TOP );

    g_scene_playing->str_lyric1 = ( string_resource_t * )get_resgroup_child( group, STR_LYRIC1 ); 
    g_scene_playing->str_lyric2 = ( string_resource_t * )get_resgroup_child( group, STR_LYRIC2 ); 
    g_scene_playing->str_lyric3 = ( string_resource_t * )get_resgroup_child( group, STR_LYRIC3 ); 

    g_scene_playing->str_effect = ( string_resource_t * )get_resgroup_child( group, STR_EFFECT ); 
    g_scene_playing->reg_ab = ( picregion_resource_t * )get_resgroup_child( group, REG_AB ); 
    g_scene_playing->reg_playmode = ( picregion_resource_t * )get_resgroup_child( group, REG_PLAYMODE ); 
    g_scene_playing->reg_star = ( picregion_resource_t * )get_resgroup_child( group, REG_STAR ); 
    g_scene_playing->reg_fast = ( picregion_resource_t * )get_resgroup_child( group, REG_FAST ); 
    g_scene_playing->reg_lyric = ( picregion_resource_t * )get_resgroup_child( group, REG_LYRIC ); 
    
    g_scene_playing->pic_pb_bg = ( picture_resource_t * )get_resgroup_child( group, PIC_PB_BG );
    g_scene_playing->pic_pb_full = ( picture_resource_t * )get_resgroup_child( group, PIC_PB_FULL );
    g_scene_playing->pic_pb_dot = ( picture_resource_t * )get_resgroup_child( group, PIC_PB_DOT );

    g_scene_playing->str_cur_time = ( string_resource_t * )get_resgroup_child( group, STR_CUR_TIME );
    g_scene_playing->str_total_time = ( string_resource_t * )get_resgroup_child( group, STR_TOTAL_TIME );  

    g_scene_playing->reg_state = ( picregion_resource_t * )get_resgroup_child( group, REG_STATE ); 
    g_scene_playing->reg_prev = ( picregion_resource_t * )get_resgroup_child( group, REG_PREV ); 
    g_scene_playing->reg_next = ( picregion_resource_t * )get_resgroup_child( group, REG_NEXT ); 

    group = ( resgroup_resource_t * )get_scene_child( scene, SELEQ_MENU_RESGROUP );
    for ( cur_index=0; cur_index < EQ_MAX; cur_index++ )
    {
        g_scene_playing->str_eq[cur_index] = ( string_resource_t * )get_resgroup_child( group, eq_id[cur_index] );
    }
         
    g_scene_playing->pic_tick = ( picture_resource_t * )get_scene_child( scene, PIC_TICK );  

    group = ( resgroup_resource_t * )get_scene_child( scene, WIDGET_DIALOG_RESGROUP );
    g_scene_playing->str_del_file = ( string_resource_t * )get_resgroup_child( group, STR_DEL_FILE );   
    g_scene_playing->str_del_tag = ( string_resource_t * )get_resgroup_child( group, STR_DEL_TAG );   
    g_scene_playing->str_deleting = ( string_resource_t * )get_resgroup_child( group, STR_DELETING );   
    g_scene_playing->str_searching = ( string_resource_t * )get_resgroup_child( group, STR_SEARCHING ); 
    g_scene_playing->str_del_fail = ( string_resource_t * )get_resgroup_child( group, STR_DEL_FAIL );
    g_scene_playing->str_nospace = ( string_resource_t * )get_resgroup_child( group, STR_NOSPACE );
    g_scene_playing->str_add_fail = ( string_resource_t * )get_resgroup_child( group, STR_ADD_FAIL );

    g_scene_playing->str_err_openfile = ( string_resource_t * )get_resgroup_child( group, STR_ERR_OPENFILE ); 
    g_scene_playing->str_err_notsupport = ( string_resource_t * )get_resgroup_child( group, STR_ERR_NOTSUPPORT ); 
    g_scene_playing->str_err_decode = ( string_resource_t * )get_resgroup_child( group, STR_ERR_DECODE ); 
    g_scene_playing->str_err_nolicense = ( string_resource_t * )get_resgroup_child( group, STR_ERR_NOLICENSE ); 
    g_scene_playing->str_err_clock = ( string_resource_t * )get_resgroup_child( group, STR_ERR_CLOCK ); 
    g_scene_playing->str_err_drminfo = ( string_resource_t * )get_resgroup_child( group, STR_ERR_DRMINFO ); 

    //free bitmap
    sty_delete_bitmap( g_scene_playing->bmp_bg );
    g_scene_playing->bmp_bg = NULL;
    sty_delete_bitmap( g_scene_playing->bmp_effect_bg );
    g_scene_playing->bmp_effect_bg = NULL;
    sty_delete_bitmap( g_scene_playing->bmp_effect_dot );
    g_scene_playing->bmp_effect_dot = NULL;
    sty_delete_bitmap( g_scene_playing->bmp_effect_top );
    g_scene_playing->bmp_effect_top = NULL;
    sty_delete_bitmap( g_scene_playing->bmp_pb_bg );
    g_scene_playing->bmp_pb_bg = NULL;
    sty_delete_bitmap( g_scene_playing->bmp_pb_full );
    g_scene_playing->bmp_pb_full = NULL;
    sty_delete_bitmap( g_scene_playing->bmp_pb_dot );
    g_scene_playing->bmp_pb_dot = NULL;
     
    //cache bitmap
    g_scene_playing->bmp_bg = get_user_bg_pic(BG_MUSIC);
    
    if ( ( g_scene_playing->pic_bg != NULL ) && ( g_scene_playing->bmp_bg == NULL ) )
    {
        g_scene_playing->bmp_bg = sty_create_bitmap( (int)g_scene_playing->pic_bg->pic_id );
    }
    
    if ( g_scene_playing->pic_effect_bg != NULL )
    {
        g_scene_playing->bmp_effect_bg = sty_create_bitmap( (int)g_scene_playing->pic_effect_bg->pic_id );
    }
    
    if ( g_scene_playing->pic_effect_dot != NULL )
    {
        g_scene_playing->bmp_effect_dot = sty_create_bitmap( (int)g_scene_playing->pic_effect_dot->pic_id );
    }
    
    if ( g_scene_playing->pic_effect_top != NULL )
    {
        g_scene_playing->bmp_effect_top = sty_create_bitmap( (int)g_scene_playing->pic_effect_top->pic_id );
    }
    
    if ( g_scene_playing->pic_pb_bg != NULL )
    {
        g_scene_playing->bmp_pb_bg = sty_create_bitmap( (int)g_scene_playing->pic_pb_bg->pic_id );
    }
    
    if ( g_scene_playing->pic_pb_full != NULL )
    {
        g_scene_playing->bmp_pb_full = sty_create_bitmap( (int)g_scene_playing->pic_pb_full->pic_id );
    }
    
    if ( g_scene_playing->pic_pb_dot != NULL )
    {
        g_scene_playing->bmp_pb_dot = sty_create_bitmap( (int)g_scene_playing->pic_pb_dot->pic_id );
    }
     
    //resize window
    if ( g_scene_playing->win_id != -1 )
    {    
        gui_wm_set_size( g_scene_playing->win_id, g_scene_playing->scene->width, g_scene_playing->scene->height );
    }
    
    //bg init flag
    g_scene_playing->bg_inited = false;
    
    return true;
}


/* scene paint function */
static bool _scene_playing_paint( void )
{
    //paint bg only once
    if ( g_scene_playing->bg_inited == false )
    {
        //paint bg
        playing_paint_bg( g_scene_playing );  
    }

    //paint file info
    playing_paint_file_info( g_scene_playing );    

    //paint config info
    playing_paint_config( g_scene_playing );  

    //paint effect
    playing_paint_effect( g_scene_playing );

    //paint progress
    playing_paint_progress( g_scene_playing );

    //paint button
    playing_paint_button( g_scene_playing ); 

    if ( g_scene_playing->bg_inited == false )
    {
        g_scene_playing->bg_inited = true;
    }

    return true;
}

/* scene gui msg process */
static bool _scene_playing_proc_gui( gui_wm_message_t *pmsg )
{
    key_event_t *kmsg;
    bool bret = true;
    mengine_status_t *eg_status = &g_scene_playing->engine.eg_status;
    
    //main window process
    switch (pmsg->msgid) 
    {
    case WM_MSG_CREATE:
        break;
         
    case WM_MSG_PAINT:
        print_dbg();
        break;
         
    case WM_MSG_KEY:
        kmsg = (key_event_t *)(pmsg->data.p);
            
        switch ( kmsg->val | kmsg->type ) 
        {
        case KEY_VSUB | KEY_TYPE_DOWN:
        case KEY_VADD | KEY_TYPE_DOWN:
            {
                if ( g_scene_playing->vbar == NULL )
                {
                    g_scene_playing->vbar = volumebar_create( 0, _volumebar_callback );                    
                    if( g_scene_playing->vbar == NULL )
                    {
                        print_err(); 
                        return false;
                    }
                    else
                    {
                        volumebar_paint( g_scene_playing->vbar );
                    }                    
                }
            }
            break;
            
            
        case KEY_MENU | KEY_TYPE_DOWN:
            {
                if ( g_scene_playing->opt == NULL )
                {
                    //create option
                    g_scene_playing->opt = music_playing_widget_option_create( _widget_option_callback );
                    if( g_scene_playing->opt == NULL )
                    {
                        print_err("menuview create fail!"); 
                        return false;
                    }
                    else
                    {
                    }
                }    
            }
            break;

            
        case KEY_RETURN | KEY_TYPE_SHORT_UP:
        case KEY_RETURN | KEY_TYPE_LONG_UP:
        case KEY_RETURN | KEY_TYPE_HOLD_UP:
            {
                //exit scene
                g_scene_playing->exit_type = SCENE_PLAY_RETURN;
                scene_playing_quit();    
            }
            break;

                
        case KEY_OK | KEY_TYPE_SHORT_UP:
        case KEY_OK | KEY_TYPE_LONG_UP:
        case KEY_OK | KEY_TYPE_HOLD_UP:
            {         
                g_scene_playing->press_state = true;
                
                switch ( eg_status->play_status )
                {
                case EG_PLAY:
                    _playing_update_button( EG_PAUSE );
                    music_pause();
                    //kill timer for updating status
                    _playing_stop_timer();
                    break;
                    
                case EG_PAUSE:
                    _playing_update_button( EG_PLAY );
                    music_resume();            
                    //set timer for updating status
                    _playing_start_timer();        
                    break;
                    
                case EG_STOP:
                    _playing_update_button( EG_PLAY );   
                    music_play();    
                    //set timer for updating status
                    _playing_start_timer();  
                    break;
                    
                case EG_PLAY_AB:
                    music_set_ab_next();
                    break;
                    
                case EG_PLAY_FAST:
                    break;
                    
                default:
                    break;
                }
                
                g_scene_playing->press_state = false;
                
                //get music status    
                playing_update_status();

                //repaint
                _scene_playing_paint();
                
                //clear key
                clear_key_msg_queue(-1,-1);
            }
            break;

            
        case KEY_PREV | KEY_TYPE_LONG:
            {
                switch ( eg_status->play_status )
                {
                case EG_PLAY:
                case EG_PLAY_AB:
                    g_scene_playing->press_prev = true;
                    _playing_update_button( EG_PLAY_FAST );
                    //start fast backward
                    bret = music_fast_backward();
                    break;
                    
                case EG_PAUSE:                    
                case EG_STOP: 
                    g_scene_playing->press_prev = true;
                    _playing_update_button( eg_status->play_status );
                    //play prev file
                    bret = music_play_prev();    
                    g_scene_playing->press_prev = false;                       
                    break;

                case EG_PLAY_FAST:
                    break;
                    
                default:
                    break;
                }
                
                if(bret == false)
                {
                    print_err("fast backward error\n");
                }            

                //get music status    
                playing_update_status();
                
                //repaint
                _scene_playing_paint();
                
                //set timer for updating status
                _playing_start_timer(); 
                    
                //clear key
                clear_key_msg_queue(-1,KEY_TYPE_HOLD);
            }
            break;

                        
        case KEY_NEXT | KEY_TYPE_LONG:
            {                
                switch ( eg_status->play_status )
                {
                case EG_PLAY:
                case EG_PLAY_AB:
                    g_scene_playing->press_next = true;
                    _playing_update_button( EG_PLAY_FAST );
                    //start fast forward
                    bret = music_fast_forward();
                    break;
                    
                case EG_PAUSE:                    
                case EG_STOP: 
                    g_scene_playing->press_next = true;
                    _playing_update_button( eg_status->play_status );
                    //play next file
                    bret = music_play_next();   
                    g_scene_playing->press_next = false;         
                    break;
                                     
                case EG_PLAY_FAST:
                    break;
                    
                default:
                    break;
                }
                
                if(bret == false)
                {
                    print_err("fast forward error\n");
                }            

                //get music status    
                playing_update_status();                

                //repaint
                _scene_playing_paint();
                
                //set timer for updating status
                _playing_start_timer(); 
                
                //clear key
                clear_key_msg_queue(-1,KEY_TYPE_HOLD);
            }
            break;
            

        case KEY_PREV | KEY_TYPE_HOLD:
        case KEY_NEXT | KEY_TYPE_HOLD:
            {
                //clear key
                clear_key_msg_queue(-1,KEY_TYPE_HOLD);
            }
            break;

            
        case KEY_PREV | KEY_TYPE_SHORT_UP:
            {       
                g_scene_playing->press_prev = true;                 
                _playing_update_button( eg_status->play_status );
                //play prev file
                bret = music_play_prev();                
                g_scene_playing->press_prev = false;
                
                if(bret == false)
                {
                    print_err("play prev file error\n");
                }
                
                //get music status    
                playing_update_status();                

                //repaint
                _scene_playing_paint();
                
                //set timer for updating status
                _playing_start_timer();   
                
                //clear key
                clear_key_msg_queue(-1,-1);
            }
            break;


        case KEY_NEXT | KEY_TYPE_SHORT_UP:
            {
                g_scene_playing->press_next = true;                
                _playing_update_button( eg_status->play_status );
                //play next file
                bret = music_play_next(); 
                g_scene_playing->press_next = false;
                
                if(bret == false)
                {
                    print_err("play next file error\n");
                }
                
                //get music status    
                playing_update_status();
                
                //repaint
                _scene_playing_paint();
                
                //set timer for updating status
                _playing_start_timer();   
                
                //clear key
                clear_key_msg_queue(-1,-1);
            }
            break;

            
        case KEY_DELETE | KEY_TYPE_DOWN:
            {
                playing_delete_process();
                
                //clear key
                clear_key_msg_queue(-1,-1);
            }
            break;


        case KEY_PREV | KEY_TYPE_LONG_UP:
        case KEY_PREV | KEY_TYPE_HOLD_UP:
        case KEY_NEXT | KEY_TYPE_LONG_UP:
        case KEY_NEXT | KEY_TYPE_HOLD_UP:
            {       
                _playing_cancel_ffb();           
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

    return true;
}

static bool _scene_playing_proc_sys( msg_apps_t *msg )
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
        g_scene_playing->exit_type = SCENE_PLAY_QUIT;
        scene_playing_quit();
        break;

    case MSG_GSENSOR_ROTATION_CHANGED:
        print_info("received msg: MSG_GSENSOR_ROTATION_CHANGED");
        _scene_playing_rotation();
        break;
#if 0  
    case MSG_GSENSOR_SHAKED:
        print_info("received msg: MSG_GSENSOR_SHAKED");
        music_play_next();
        break;
#endif

    case MSG_KEYLOCK:
        _playing_cancel_ffb();

    default:
        if(g_dirlist != NULL)
        {
            dirlist_proc_sys(g_dirlist, msg);
        }
        
        if(g_scene_playing->msgbox != NULL)
        {
            /* 控件msgbox用来处理默认系统消息(低电提示、卡插入和拔出提示、USB插入功能选择提示等) */
            msgbox_proc_sys(g_scene_playing->msgbox, msg);
        }
        break;
    }

    return true;
}


/* scene callback function */
static void _scene_playing_callback( gui_wm_message_t *pmsg )
{ 
    int top;

    //top canvas process
    top = canvas_get_focus(pmsg);

    //dispatch msg
    if ( (g_scene_playing->vbar != NULL) && (top == g_scene_playing->vbar->canvas_id) )
    {
        volumebar_proc_gui( g_scene_playing->vbar, pmsg );
        return;
    }

    if ( (g_scene_playing->opt != NULL) && (top == g_scene_playing->opt->canvas_id) ) 
    {
        music_playing_widget_option_proc_gui( g_scene_playing->opt, pmsg );
        return;
    }

    if( (g_scene_playing->adjust != NULL) && (top == g_scene_playing->adjust->canvas_id))
    {
        adjust_proc_gui(g_scene_playing->adjust, pmsg);
        return;
    }

    if( (g_scene_playing->dialog != NULL) && (top == g_scene_playing->dialog->canvas_id))
    {
        dialog_proc_gui(g_scene_playing->dialog, pmsg);
        return;
    }

    if ( (g_scene_playing->usereq != NULL) && (top == g_scene_playing->usereq->canvas_id) )
    {
        usereq_widget_proc_gui( g_scene_playing->usereq, pmsg );
        return;
    }

    if ( (g_scene_playing->srsuser != NULL) && (top == g_scene_playing->srsuser->canvas_id) )   
    {
        srsuser_widget_proc_gui( g_scene_playing->srsuser, pmsg );
        return;
    }    

    if( (g_scene_playing->msgbox != NULL) && (top == g_scene_playing->msgbox->canvas_id) )
    {
        /* 处理默认系统消息(低电提示、卡插入和拔出提示、USB插入功能选择提示等)的msgbox控件需要接受GUI消息 */
        msgbox_proc_gui( g_scene_playing->msgbox, pmsg );
        return;
    }    
    else    //scene msg process
    {
        _scene_playing_proc_gui( pmsg );
        return;
    }    
}

/* scene rotation */
static bool _scene_playing_rotation( void )
{
    fusion_effect_t eff;
    
    fusion_get_config(FUSION_TYPE_ROTATE, FUSION_FORWARD, &eff);
    fusion_set_effect(&eff);
    fusion_set_first_frame(NULL);    

    //try to kill scroll timer
    kill_timer( g_scene_playing->scroll_timer );
    g_scene_playing->scroll_timer = -1;

    //reload theme
    _scene_playing_close_theme();
    _scene_playing_open_theme();
    
    //reload style
    _scene_playing_load_style();
    _scene_playing_paint();

    if( g_scene_playing->vbar != NULL )
    {
        volumebar_load_style( g_scene_playing->vbar );
        volumebar_paint( g_scene_playing->vbar );
    }

    if( g_scene_playing->hbar != NULL )
    {
        headbar_load_style( g_scene_playing->hbar );
        headbar_paint( g_scene_playing->hbar );
    }

    if( g_scene_playing->opt != NULL )
    {
        menuview_load_style( g_scene_playing->opt, g_scene_playing->opt->type );
        menuview_paint( g_scene_playing->opt );
    }

    if( g_scene_playing->adjust != NULL )
    {
        adjust_load_style( g_scene_playing->adjust );
        adjust_paint( g_scene_playing->adjust );
    }

    if( g_scene_playing->dialog != NULL )
    {
        dialog_load_style( g_scene_playing->dialog );
        dialog_paint( g_scene_playing->dialog );
    }

    if( g_scene_playing->msgbox != NULL )
    {
        msgbox_load_style( g_scene_playing->msgbox );
        msgbox_paint( g_scene_playing->msgbox );
    }
      
    if( g_scene_playing->usereq != NULL )
    {
        usereq_widget_load_style( g_scene_playing->usereq );
        usereq_widget_paint( g_scene_playing->usereq );
    }
       
    if( g_scene_playing->srsuser != NULL )
    {
        srsuser_widget_load_style( g_scene_playing->srsuser );
        srsuser_widget_paint( g_scene_playing->srsuser );
    }
        
    dialog_modal_load_style();
    dialog_modal_paint();

    fusion_set_last_frame(NULL);
    fusion_display(NULL);

    return true;
}

static bool _playing_start_timer( void )
{
    _playing_stop_timer();
    
    //set timer for updating status
    g_scene_playing->timer_id = set_timer( UPDATE_TIME, 
                                    (timer_proc)_playing_timer_event, g_scene_playing ); 

    return true;
}

static bool _playing_stop_timer( void )
{
    //kill timer for updating status
    kill_timer( g_scene_playing->timer_id );
    g_scene_playing->timer_id = -1;

    return true;
}

static bool _playing_init_status( void )
{
    int result;
    
    //clear index
    g_file_index = -1;
    
    //get music engine info
    result = playing_update_status();    
    if ( result < 0 )
    {
        return false;
    }

    //init status
    switch(g_scene_playing->engine.eg_status.play_status)
    {
    case EG_PAUSE:
    case EG_STOP:
        g_scene_playing->engine.eg_status.play_status = EG_PLAY;
        break;
        
    default:
        break;
    }
    
    if ( g_scene_playing->engine.music_status.cur_time == 0 )
    {
        g_scene_playing->engine.music_status.cur_time = (int)g_scene_playing->engine.config.audio_tag.time;
    }

    //check error
    if ( g_scene_playing->engine.eg_status.err_status != EG_ERR_NONE )
    {
        g_scene_playing->engine.eg_status.play_status = EG_STOP;
    }
    
    return true;
}

static bool _playing_init_play( void )
{
    bool bret = true;
    unsigned short str_id;
    
    //check error
    if ( g_scene_playing->engine.eg_status.err_status == EG_ERR_NONE )
    {
        //play music
        bret = music_play();
    }
    else
    {
        switch( g_scene_playing->engine.eg_status.err_status )
        {
        case EG_ERR_OPEN_FILE:
            str_id = g_scene_playing->str_err_openfile->str_id;
            break;
            
        case EG_ERR_NOT_SUPPORT:
            str_id = g_scene_playing->str_err_notsupport->str_id;
            break;
            
        case EG_ERR_DECODER_ERROR:
            str_id = g_scene_playing->str_err_decode->str_id;
            break;
            
        case EG_ERR_NO_LICENSE:
            str_id = g_scene_playing->str_err_nolicense->str_id;
            break;
            
        case EG_ERR_SECURE_CLOCK:
            str_id = g_scene_playing->str_err_clock->str_id;
            break;
            
        case EG_ERR_DRM_INFO:
            str_id = g_scene_playing->str_err_drminfo->str_id;
            break;
            
        default:
            str_id = g_scene_playing->str_err_notsupport->str_id;
            break;
        }
        
        _modal_info_widget( str_id );
        
        bret = false;
    }

    return bret;
}

static bool _playing_cancel_ffb( void )
{
    bool bret = true;

    //check last ffb
    if ( ( g_scene_playing->press_prev == true )
        || ( g_scene_playing->press_next == true ) )
    {
        //cancel fast      
        bret = music_cancel_ffb();
        if(bret == false)
        {
            print_err("cancel fast forward error\n");
        }

        //set timer for updating status
        _playing_start_timer(); 
    } 

    g_scene_playing->press_prev = false;
    g_scene_playing->press_next = false;  

    //get music status    
    playing_update_status();                

    //repaint
    _scene_playing_paint();
    
    return bret;
}

/* one second timer function */
static void _playing_timer_event( void *param )
{
    scene_play_t* scene_play = ( scene_play_t* )param;
    int cur_time = scene_play->engine.music_status.cur_time;
    play_status_e last_status = scene_play->engine.eg_status.play_status;
    play_status_e cur_status;
    int result;

    //get music engine info
    result = playing_update_status();
    cur_status = scene_play->engine.eg_status.play_status;

    //if file changed
    if ( result > 0 )
    {
        //repaint file info
        playing_paint_file_info( scene_play );
        playing_paint_config( scene_play );
    }
    else
    {
        //if ab repeat stop
        if ( ( last_status == EG_PLAY_AB ) && ( cur_status != EG_PLAY_AB ) )
        {
            //repaint
            playing_paint_config( scene_play ); 
        }    
    }
    
    //if current time changed
    if ( cur_time != scene_play->engine.music_status.cur_time )
    {
        //paint effect
        playing_paint_effect( scene_play );
    }
    
    //if second time changed
    if ( ( cur_time/1000 ) != ( scene_play->engine.music_status.cur_time/1000 ) )
    {
        //repaint
        playing_paint_progress( scene_play );  
    }
    
    //if status changed
    if ( last_status != cur_status )
    {
        //repaint
        playing_paint_button( scene_play ); 
    }
    
    if ( ( cur_status == EG_PAUSE ) || ( cur_status == EG_STOP ) )
    {
        //kill timer for updating status
        _playing_stop_timer();
    }    
}

static void _playing_update_button( play_status_e play_status )
{
    g_scene_playing->engine.eg_status.play_status = play_status;    
    playing_paint_button( g_scene_playing );
    gui_screen_update();  
}

static void _album_init_buffer( void )
{
    int width = g_scene_playing->pic_def_album->width;
    int height = g_scene_playing->pic_def_album->height;
    int bpp = 2;

    if ( g_scene_playing->album_bmp == NULL )
    {        
        g_scene_playing->album_bmp = malloc( sizeof(gui_bitmap_info_t) );
        if ( g_scene_playing->album_bmp == NULL )
        {
            print_err();
            return;
        }
        
        g_scene_playing->album_bmp->pdata = malloc( (unsigned int)(width * height * bpp) );
        if ( g_scene_playing->album_bmp->pdata == NULL )
        {
            print_err();
            return;
        }
    }
    
    g_scene_playing->album_bmp->bytesperpixel = (unsigned short)bpp;
    g_scene_playing->album_bmp->xsize = (unsigned short)width;
    g_scene_playing->album_bmp->ysize = (unsigned short)height;
}

static void _album_deinit_buffer( void )
{
    //free bitmap
    if ( g_scene_playing->album_bmp != NULL )
    {
        free( (char*)g_scene_playing->album_bmp->pdata );
        free( g_scene_playing->album_bmp );
        g_scene_playing->album_bmp = NULL;
    }
}

static void _album_start_decode( void )
{
    bool bret;
    plist_detail_info_t *id3_info = g_scene_playing->id3_info;
    
    //check album info
    bret = album_check_file( id3_info );
    if ( bret == true )
    {
        //init album bmp                
        _album_init_buffer();
                
        //start decode
        g_scene_playing->album_flag = false;
        album_abort_decode();
        album_start_decode( id3_info, g_scene_playing->album_bmp, _album_decode_callback );
    }
}

static void _album_decode_callback( void )
{
    mmm_img_info_t img_info;

    //change bitmap size
    album_get_image_info( &img_info );
    g_scene_playing->album_bmp->xsize = (unsigned short)img_info.img_width;
    g_scene_playing->album_bmp->ysize = (unsigned short)img_info.img_height;

    g_scene_playing->album_flag = true;
    playing_paint_album( g_scene_playing );
}

/* update status */
/* return: -1--error, 0--normal, 1-file change */
int playing_update_status( void )
{
    bool bret = true;
    mengine_config_t *cfg_ptr = &g_scene_playing->engine.config;
    static int s_file_total = 0;
    hotplugin_dev_type_e main_disk ; 

    //get music engine     info
    bret = music_get_engine_info( &g_scene_playing->engine );
    if(bret == false)
    {
        print_err("get music engine info error\n");
        return -1; //error
    }

    //no file in nplist
    if( cfg_ptr->file_total <= 0 )
    {
        //delete nowplaying
        music_set_nowplaying( false );
        
        //判断主盘是否存在
        main_disk = sys_get_main_disk();
        if(main_disk == HOTPLUGIN_DEV_TYPE_NONE)
        {
            //退出应用
            g_scene_playing->exit_type = SCENE_PLAY_QUIT;
        }
        else
        {
            //退出场景
            g_scene_playing->exit_type = SCENE_PLAY_RETURN;
        }
        scene_playing_quit();
                
        return 0; //normal
    }

    if( ( g_file_index != cfg_ptr->file_index ) || ( s_file_total != cfg_ptr->file_total ) )        
    {
        g_file_index = cfg_ptr->file_index;
        s_file_total = cfg_ptr->file_total;
        g_scene_playing->lyric_flag = false;
        g_scene_playing->album_flag = false;

        //kill scroll timer
        kill_timer( g_scene_playing->scroll_timer );
        g_scene_playing->scroll_timer = -1;
        
        //get id3 info
        g_scene_playing->id3_info = plistclass_get_id3_by_index( g_file_index );
        if( g_scene_playing->id3_info == NULL )
        {
            print_err("plistclass_get_id3_by_fullpath fail!");
        }
        else
        {        
            //parse lyric
            g_scene_playing->lyric_flag = lyric_init( g_scene_playing->id3_info->fullpath );
            
            //decode alubm
            kill_timer( g_scene_playing->single_timer );
            g_scene_playing->single_timer = set_single_shot_timer( 1, 
                                                    (timer_proc)_album_start_decode, g_scene_playing );
        }
        
        return 1;  //file changed
    }
    else
    {
        return 0; //normal
    }    
}


/* scene quit operation function */
bool scene_playing_quit( void )
{
    exit_msg_loop();
    
    return true;
}

/* scene entry function */
int music_scene_playing( fusion_enter_e fenter )
{
    bool ret;
    msg_apps_t  msg;
    scene_play_ret_t exit_type;
    
    /* 场景初始化 */
    ret = _scene_playing_init( fenter );
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
    register_sys_dispatcher( (sys_msg_proc)_scene_playing_proc_sys );

    while( true == get_msg(&msg) )
    {
        dispatch_msg(&msg);
    }
    
    unregister_sys_dispatcher();

    exit_type = g_scene_playing->exit_type;
    
    /* 场景退出 */
    ret = _scene_playing_deinit();
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

