/*******************************************************************************
 *                              NOYA1100
 *                            Module: video
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-05-15          1.0             build this file 
*******************************************************************************/
/*!
 * \file     video_playing_option_info.c
 * \brief    
 * \author   shaofeng
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2009/06/03
*******************************************************************************/


#include "include_case.h"

/* include file of this application, 本应用的头文件 */
#include "include_video.h"


/* extern function, 函数声明 */
static menu_t * _option_info_init( void );
static bool _option_info_deinit( void );
static int _proc_item_info_select( menuview_t *menuview, menuview_sig_e signal );



/*
Information:
     File Name
     Resolution
     Frame Rate
     Bit Rate
     Total Time
     File size ( KB)
*/
typedef struct
{
    char file_name[PATH_MAX];
    char resolution[16];
    char frame_rate[16];
    char bit_rate[16];
    char total_time[16];
    char file_size[16];
}video_option_info_content_t;

static video_option_info_content_t s_info_content 
#pragma __PRQA_IGNORE_START__
= 
{ { 0 } }
#pragma __PRQA_IGNORE_END__

;


/* define variable, 变量定义 */
static menu_t *s_option_info = NULL;
const static video_menu_info_t s_option_menu_info[] = 
{
    { STR_FILE_NAME,    PIC_FILE_NAME,  PIC_FILE_NAME },
    { STR_RESOLUTION,   PIC_RESOLUTION, PIC_RESOLUTION },
    { STR_FRAME_RATE,   PIC_FRAME_RATE, PIC_FRAME_RATE },
    { STR_BIT_RATE,     PIC_BIT_RATE,   PIC_BIT_RATE },
    { STR_TOTAL_TIME,   PIC_TOTAL_TIME, PIC_TOTAL_TIME },
    { STR_FILE_SIZE,    PIC_FILE_SIZE,  PIC_FILE_SIZE },
};
/* menu items */
static menuitem_t s_option_info_items[] =
{   
    { 0, 0, 0, false,  (item_cb_t)_proc_item_info_select, NULL, s_info_content.file_name,   NULL, NULL },
    { 0, 0, 0, false,  (item_cb_t)_proc_item_info_select, NULL, s_info_content.resolution,  NULL, NULL },
    { 0, 0, 0, false,  (item_cb_t)_proc_item_info_select, NULL, s_info_content.frame_rate,  NULL, NULL },
    { 0, 0, 0, false,  (item_cb_t)_proc_item_info_select, NULL, s_info_content.bit_rate,    NULL, NULL },
    { 0, 0, 0, false,  (item_cb_t)_proc_item_info_select, NULL, s_info_content.total_time,  NULL, NULL },
    { 0, 0, 0, false,  (item_cb_t)_proc_item_info_select, NULL, s_info_content.file_size,   NULL, NULL },
};


#if 0
#endif
/* 进入菜单 */
bool video_playing_option_info_enter( menuview_t *menuview )
{
    menu_t *cur_menu;

    cur_menu = _option_info_init();
    if ( NULL == cur_menu )
    {
        print_err("_option_info_init err");
        return false;
    }
    menuview_set_menu( menuview, cur_menu );
    //更换为option_info 风格
    menuview_load_style( menuview, MENUVIEW_OPTION_INFO);
    menuview_paint( menuview );

    return true;
}


/* 退出菜单 */
bool video_playing_option_info_exit( void )
{
    //delete main menu
    _option_info_deinit();
    
    return true;
}


static menu_t * _option_info_init( void )
{
    bool result = true;
    char filename[PATH_MAX];
    mmm_video_file_info_t file_info;
    struct stat file_stat;
    
    //initial menu  
    if( s_option_info == NULL )
    {
        /* 获取filename信息 */
        memset( filename, 0x00, sizeof(filename) );
        result = video_playlist_get_filename_by_index( video_vars.nplist, video_vars.plist.np_index, filename );
        if ( false == result )
        {
            return NULL;
        }
        
        /* 获取info信息 */
        result = video_control(V_CMD_GET_MEDIA_INFO, &file_info);
        if ( false == result )
        {
            return NULL;
        }
        
        /* 将info信息填入menu */
        memset( &s_info_content, 0x00, sizeof(s_info_content) );
        
        strcpy( s_info_content.file_name, filename );
        
        if ( (0 == file_info.width) || (0 == file_info.height) )
        {
            memcpy( s_info_content.resolution, "-", 1 );
        }
        else
        {
            snprintf( s_info_content.resolution, sizeof(s_info_content.resolution), \
                    "%d*%d", file_info.width, file_info.height );
        }
        
        if ( 0 == file_info.frame_rate )
        {
            memcpy( s_info_content.frame_rate, "-", 1 );
        }
        else
        {
            sprintf( s_info_content.frame_rate, "%d", file_info.frame_rate );
        }
        
        if ( 0 == file_info.video_bitrate )
        {
            memcpy( s_info_content.bit_rate, "-", 1 );
        }
        else
        {
            sprintf( s_info_content.bit_rate, "%d kbps", file_info.video_bitrate/1024 );
        }
        
        video_playing_time_to_str( file_info.total_time, s_info_content.total_time );
        stat( video_vars.plist.np_fullpath, &file_stat );
        sprintf( s_info_content.file_size, "%d KB", file_stat.st_size/1024 );
        
        //create menu
        s_option_info = menu_create();
        video_load_menuitem( OPTION_INFO_RESGROUP, s_option_menu_info, \
                    sizeof(s_option_info_items)/sizeof(menuitem_t), s_option_info_items );
        
        //init items
        menu_append_items( s_option_info, s_option_info_items, sizeof(s_option_info_items)/sizeof(menuitem_t) );
    }

    return s_option_info;
}


static bool _option_info_deinit( void )
{    
    //delete menu   
    if( NULL == s_option_info )
    {
        return false;
    }
    
    menu_delete( s_option_info );
    s_option_info = NULL;

    return true;
}


static int _proc_item_info_select( menuview_t *menuview, menuview_sig_e signal )
{
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        video_playing_option_info_exit();
        video_playing_option_enter( menuview, FUSION_BACKWARD );
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        video_playing_option_info_exit();
        video_playing_option_enter( menuview, FUSION_BACKWARD );
        break;

    default:
        break;
    }   
    return 1;
}

