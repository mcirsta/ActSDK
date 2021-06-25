/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_list_dirlist.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_list_dirlist.c
* \brief    music_list_dirlist widget
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#include "music_list_dirlist.h"

/* 退出菜单 */
bool music_list_dirlist_exit( menuview_t *menuview );

/* listmenu entry */
static bool _create_dirlist( void );
static bool _delete_dirlist( void );
static void _dirlist_callback( dirlist_t* dirlist, dirlist_sig_e sig );

static bool _create_softkeyboard_widget( void );
static bool _delete_softkeyboard_widget( void );
static void _softkeyboard_callback(softkeyboard_t *keyboard, softkeyboard_sig_e signal);

static bool _create_loadingbox_widget( void );
static bool _delete_loadingbox_widget( void );
static void _loadingbox_widget_cb(loadingbox_t *loadingbox, loadingbox_sig_e signal);

static void _album_init_buffer( dirlist_t* dirlist );
static void _album_deinit_buffer( void );
static void _album_start_decode( dirlist_t* dirlist );
static void _album_decode_callback( void );

/* mainmenu */
static menuview_t *s_menuview;

/* album image decode */
static gui_bitmap_info_t *s_album_bmp;
static int s_album_index = 0;
static int s_search_num = 0;


/* 进入主菜单 */
bool music_list_dirlist_enter( menuview_t *menuview, fusion_enter_e fenter )
{   
    fusion_effect_t eff;
    
    //save mainmenu
    s_menuview = menuview;
    
    //set first
    if( fenter < FUSION_MAX )
    {
        fusion_get_config(FUSION_TYPE_COMMON, fenter, &eff);
        fusion_set_effect(&eff);
        scene_fusion_set_first_frame(g_scene_list->scene); 
    }
        
    /* create dirlist */
    _create_dirlist();
    
    //hide mainmenu
    menuview_hide( menuview );    
    
    //set last and display
    if( fenter < FUSION_MAX )
    {
        scene_fusion_set_last_frame(g_scene_list->scene);
        fusion_display(NULL);
    }

    return true;
}

/* 退出主菜单 */
bool music_list_dirlist_exit( menuview_t *menuview )
{
    //clear path
    memset( g_dirlist_path, 0, sizeof(g_dirlist_path) );
        
    //delete dirlist
    _delete_dirlist();

    //show dirlist
    menuview_show( menuview );  

    //return
    if ( g_dirlist_top[0] == '\0' )
    {
        music_list_mainmenu_enter( menuview, FUSION_BACKWARD ); 
    }   
    else    
    {
        music_list_recfile_enter( menuview, FUSION_BACKWARD ); 
    }  

    return true;
}

static bool _create_dirlist( void )
{
    int result;
    char ext_music[PATH_MAX];
    dirlist_init_t dirlist_init;
    
    /* read ext config */
    result = get_config( CFG_APP_SUPPORT_EXT_MUSIC, (char*)ext_music, sizeof(ext_music) );
    if( result < 0 )
    {
        print_err("get music ext failed!");
        return false;
    }
    
    /* init type and callback function */
    dirlist_init.type = DIRLIST_MUSIC;
    dirlist_init.filter_string = ext_music;
    dirlist_init.init_path_name = g_dirlist_path;
    dirlist_init.top_path_name = g_dirlist_top;
    
    if ( NULL == g_dirlist )
    {
        g_dirlist = dirlist_create( &dirlist_init, _dirlist_callback );
    }
    else
    {
        dirlist_show( g_dirlist );  
    }
    
    if ( NULL == g_dirlist )
    {
        print_err("dirlist_create failed\n\n");
        return false;
    }
    else
    {
        //paint dirlist
        dirlist_paint( g_dirlist );
    }   
    
    return true;
}

static bool _delete_dirlist( void )
{
    if ( (int)NULL != g_dirlist )
    {        
        //abort album decode
        album_abort_decode();

        //free album
        _album_deinit_buffer();
        
        dirlist_delete( g_dirlist );   
        g_dirlist = NULL;
        
        //kill timer
        kill_timer( g_scene_list->single_timer );
        g_scene_list->single_timer = -1;    
    }
    
    return true;
}

static void _dirlist_callback(dirlist_t* dirlist, dirlist_sig_e sig)
{
    void *handle;
    
    switch(sig)
    {
    case WIDGET_SIG_DIRLIST_QUIT:
        music_list_dirlist_exit( s_menuview ); 
        break;
        
    case WIDGET_SIG_DIRLIST_OK:
        //add dir to nplist
        handle = dirlist_get_fsel_handle( dirlist );
        
        //save dirlist path
        sprintf( g_dirlist_path, "%s%s", dirlist->path_name, dirlist->file_name); 
        g_file_index = -1;

        //check file
        if ( music_list_check_file( g_dirlist_path, g_file_index ) == false )
        {
            break;
        }
        
        //abort album decode
        album_abort_decode();

        //free album
        _album_deinit_buffer();
        
        //kill timer
        kill_timer( g_scene_list->single_timer );
        g_scene_list->single_timer = -1;    

        //exit menu
        g_scene_list->exit_type = SCENE_LIST_NEXT;     
        music_list_widget_menu_exit( s_menuview, music_list_dirlist_enter );
        break;
        
    case WIDGET_SIG_DIRLIST_DECODE:
        //abort album decode
        album_abort_decode();
        
        //start album decode
        s_album_index = dirlist_get_offset( dirlist );
        kill_timer( g_scene_list->single_timer );
        g_scene_list->single_timer = set_single_shot_timer( 1, (timer_proc)_album_start_decode, \
                                                            dirlist );
        break;
        
    case WIDGET_SIG_DIRLIST_SHORT_MENU:
        _create_softkeyboard_widget();            
        break;
        
    default:
        break;
    }
}


static bool _create_softkeyboard_widget( void )
{
    //create widget
    g_scene_list->softkeyboard = softkeyboard_create(IME_TYPE_PINYIN, _softkeyboard_callback );
    if( g_scene_list->softkeyboard == NULL )
    {
        print_err("create softkeyboard widget failed!");
        return false;
    }
    else
    {
        //hide dirlist
        dirlist_hide(g_dirlist);
        
        softkeyboard_paint( g_scene_list->softkeyboard );
    }

    return true;
}

static bool _delete_softkeyboard_widget( void )
{
    softkeyboard_delete( g_scene_list->softkeyboard );
    g_scene_list->softkeyboard = NULL;

    return true;
}

static void _searching_done( void )
{ 
    dialog_init_t dialog_init;

    //delete searching dialog
    _delete_loadingbox_widget();
    
    if ( s_search_num <= 0 )
    {
        //clear filter string
        dirlist_set_search_string( g_dirlist, "" );
    
        //show cann't found dialog
        dialog_init.head = DIALOG_HEAD_WARN;
        dialog_init.str_id = g_scene_list->str_notfound->str_id;
        dialog_modal( &dialog_init );
    }
    else
    {     
        //delete softkeyboard
        _delete_softkeyboard_widget();

        //show dirlist
        dirlist_show(g_dirlist);

        //update dirlist
        dirlist_paint( g_dirlist );       
         
        //abort album decode
        album_abort_decode();
        
        //start album decode
        s_album_index = dirlist_get_offset( g_dirlist );
        kill_timer( g_scene_list->single_timer );
        g_scene_list->single_timer = set_single_shot_timer( 1, (timer_proc)_album_start_decode, \
                                                            g_dirlist );  
    }
}

static void _deal_searching( void )
{
    char text[128];
     
    //get text
    softkeyboard_get_text( g_scene_list->softkeyboard, text, sizeof(text) );    
    
    //set filter string
    s_search_num = dirlist_set_search_string( g_dirlist, text );
    
    //process after searching
    kill_timer( g_scene_list->single_timer );
    g_scene_list->single_timer = set_single_shot_timer( 1, (timer_proc)_searching_done, NULL );
}

static void _softkeyboard_callback(softkeyboard_t *keyboard, softkeyboard_sig_e signal)
{
    switch ( signal ) 
    {
    case WIDGET_SIG_SKB_OK:
        //create loadingbox
        _create_loadingbox_widget();

        //create thread to search
        music_create_thread( _deal_searching, NULL );
        break;

    case WIDGET_SIG_SKB_CANCEL:
        //delete softkeyboard
        _delete_softkeyboard_widget();
        
        //show dirlist
        dirlist_show(g_dirlist);
        
        //update dirlist
        dirlist_paint( g_dirlist );
        break;
        
    default:
        break;
    }
}

static bool _create_loadingbox_widget( void )
{
    if(g_scene_list->loadingbox == NULL)
    {
        g_scene_list->loadingbox = loadingbox_create( LOADINGBOX_SEARCHING, _loadingbox_widget_cb );
        if ( NULL == g_scene_list->loadingbox )
        {
            print_err("loadingbox_create failed\n\n");
            return false;
        }
        else
        {
            //paint
            loadingbox_paint( g_scene_list->loadingbox );    
        }
    }

    return true;
}

static bool _delete_loadingbox_widget( void )
{    
    if(g_scene_list->loadingbox != NULL)
    {
        loadingbox_delete( g_scene_list->loadingbox );
        g_scene_list->loadingbox = NULL;
    }
    
    return true;
}

static void _loadingbox_widget_cb(loadingbox_t *loadingbox, loadingbox_sig_e signal)
{
}

static void _album_init_buffer( dirlist_t* dirlist )
{
    int width;
    int height;
    int bpp = 2;
    
    width = dirlist_get_bitmap_width( dirlist );
    height = dirlist_get_bitmap_height( dirlist );    

    if ( s_album_bmp == NULL )
    {        
        s_album_bmp = malloc( sizeof(gui_bitmap_info_t) );
        if ( s_album_bmp == NULL )
        {
            print_err();
            return;
        }
        
        s_album_bmp->pdata = malloc( (unsigned int)(width * height * bpp) );
        if ( s_album_bmp->pdata == NULL )
        {
            print_err();
            return;
        }
    }
    
    s_album_bmp->bytesperpixel = (unsigned short)bpp;
    s_album_bmp->xsize = (unsigned short)width;
    s_album_bmp->ysize = (unsigned short)height;
}

static void _album_deinit_buffer( void )
{
    //free bitmap
    if ( s_album_bmp != NULL )
    {
        free( (char*)s_album_bmp->pdata );
        free( s_album_bmp );
        s_album_bmp = NULL;
    }
}

static void _album_start_decode( dirlist_t* dirlist )
{
    bool bret;
    gui_bitmap_info_t *bmp;
    int total;
    plist_detail_info_t *id3_info;
    char fullpath[PATH_MAX];
    char *filename;
    bool is_dir;

    total = dirlist_get_total( dirlist );
    if ( total <= 0 )
    {
        return;
    }

    for( ; s_album_index < total; s_album_index ++ )
    {
        bmp = dirlist_get_item_bitmap( dirlist, s_album_index );
        
        if( bmp != NULL )
        {
        }
        else
        {
            is_dir = (bool)dirlist_is_item_dir( dirlist, s_album_index );
            if( is_dir == true )
            {
                continue;
            }
            
            //get fullpath
            filename = dirlist_get_item_name( dirlist, s_album_index );
            sprintf( fullpath, "%s%s", dirlist->path_name, filename );
            
            //get detail id3
            id3_info = plistclass_get_id3_by_fullpath( fullpath );            
            if( id3_info == NULL )
            {
                print_err("plistclass_get_id3_by_fullpath fail!");
                continue;
            }
            
            //check album info
            bret = album_check_file( id3_info );
            if ( bret == true )
            {
                //init album bmp                
                _album_init_buffer( dirlist );
                
                //start decode
                album_abort_decode();
                album_start_decode( id3_info, s_album_bmp, _album_decode_callback );
                break;
            }
        }
    }

}

static void _album_decode_callback( void )
{
    mmm_img_info_t img_info;

    //change bitmap size
    album_get_image_info( &img_info );
    s_album_bmp->xsize = (unsigned short)img_info.img_width;
    s_album_bmp->ysize = (unsigned short)img_info.img_height;
    
    //set album bmp to item
    dirlist_set_item_bitmap( g_dirlist, s_album_index, s_album_bmp );
    
    //draw bitmap
    dirlist_paint_bitmap( g_dirlist, s_album_index );   

    /* 向场景发送预览解码完毕消息，解码下一幅 */
    s_album_index ++;
    kill_timer( g_scene_list->single_timer );
    g_scene_list->single_timer = set_single_shot_timer( 1, (timer_proc)_album_start_decode, g_dirlist );
}

