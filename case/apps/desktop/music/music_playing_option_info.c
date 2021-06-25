/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_playing_option_info.c
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     music_playing_option_info.c
* \brief    music_playing_option_info widget
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#include "music_playing_option_info.h"

/* 退出主菜单 */
bool music_playing_option_info_exit( menuview_t *menuview );

/* 初始化菜单数据 */
menu_t* music_playing_option_info_init( void );

/* 删除菜单数据 */
int music_playing_option_info_deinit( void );

/* listmenu entry */
static int _proc_items( menuview_t *menuview, menuview_sig_e signal );

/* menu list data define */
static menu_t *s_info = NULL;
static int s_info_offset = 0;
static int s_info_current = 0;

/* menu items data define */
static style_menuitem_t s_info_items[] =
{    
    // name, has_child, callback
    { STR_TITLE,   false,   (item_cb_t)_proc_items,  PIC_TITLE, PIC_TITLE, 0, 0, 0 },
    { STR_ALBUM,   false,   (item_cb_t)_proc_items,  PIC_ALBUM, PIC_ALBUM, 0, 0, 0 },
    { STR_ARTIST,   false,   (item_cb_t)_proc_items,  PIC_ARTIST, PIC_ARTIST, 0, 0, 0 },
    { STR_GENRE,   false,   (item_cb_t)_proc_items,  PIC_GENRE, PIC_GENRE, 0, 0, 0 },
    { STR_YEAR,   false,   (item_cb_t)_proc_items,  PIC_YEAR, PIC_YEAR, 0, 0, 0 },
    { STR_TIME,   false,   (item_cb_t)_proc_items,  PIC_TIME, PIC_TIME, 0, 0, 0 },
    { STR_BITRATE,   false,   (item_cb_t)_proc_items,  PIC_BITRATE, PIC_BITRATE, 0, 0, 0 },
    { STR_SAMPLE,   false,   (item_cb_t)_proc_items,  PIC_SAMPLE, PIC_SAMPLE, 0, 0, 0 },
    { STR_FORMAT,   false,   (item_cb_t)_proc_items,  PIC_FORMAT, PIC_FORMAT, 0, 0, 0 },
};

static char s_char_time[16];
static char s_char_bitrate[32];
static char s_char_sample[32];

/* menu list max items */
#define MAX_INFO_ITEMS  ( sizeof(s_info_items)/sizeof(style_menuitem_t) )



/* 进入主菜单 */
bool music_playing_option_info_enter( menuview_t *menuview )
{
    menu_t *cur_menu;

    cur_menu = music_playing_option_info_init();
    menuview_set_menu( menuview, cur_menu );
    
    if ( menuview->type != MENUVIEW_OPTION_INFO )
    {
        menuview_load_style( menuview, MENUVIEW_OPTION_INFO );
    }
    menuview_paint( menuview );

    return true;
}

/* 退出主菜单 */
bool music_playing_option_info_exit( menuview_t *menuview )
{
    //delete main menu
    music_playing_option_info_deinit();

    s_info_offset = 0;
    s_info_current = 0;

    //return
    music_playing_option_enter( menuview );    
    
    return true;
}

menu_t* music_playing_option_info_init( void )
{
    menuitem_t item;
    unsigned int num;
    plist_detail_info_t *id3_info;
    mmm_music_file_info_t *file_info;
    char *text_info[MAX_INFO_ITEMS];
    int hour, minute, second;

    //init text_info
    memset( text_info, 0, sizeof(text_info) );
    
    //initial menu    
    if( s_info == NULL )
    {
        //create menu
        s_info = menu_create();

        //get id3 info
        //id3_info = plistclass_get_id3_by_fullpath( g_fullpath );   
        id3_info = g_scene_playing->id3_info;   
        if( id3_info == NULL )
        {
            print_err("plistclass_get_id3_by_fullpath fail!");
        }
        else
        {
            text_info[0] = id3_info->title;
            text_info[1] = id3_info->album;
            text_info[2] = id3_info->artist;
            text_info[3] = id3_info->genre;
            text_info[4] = id3_info->age;
        }
        
        file_info = &g_scene_playing->engine.file_info;        

        //total time
        hour = (int)(file_info->total_time / 3600);
        minute = (int)((file_info->total_time % 3600) / 60);
        second = (int)(file_info->total_time % 60);    
        snprintf( s_char_time, sizeof(s_char_time), "%02d:%02d:%02d", hour, minute, second );    
        text_info[5] = s_char_time;

        //bit rate
        snprintf( s_char_bitrate, sizeof(s_char_bitrate), "%d Kbps", file_info->bitrate );                 
        text_info[6] = s_char_bitrate;

        //sample rate
        snprintf( s_char_sample, sizeof(s_char_sample), "%d Khz", file_info->sample_rate );    
        text_info[7] = s_char_sample;

        //file format
        text_info[8] = file_info->file_format;

        //load style
        menu_load_style( INFO_MENU_RESGROUP, s_info_items, MAX_INFO_ITEMS );
        
        //init items
        memset( &item, 0, sizeof(menuitem_t) );
        
        for ( num=0; num<MAX_INFO_ITEMS; num++ )
        {
            item.unsel_icon_id = s_info_items[num].pic_unsel_id;
            item.sel_icon_id = s_info_items[num].pic_sel_id;
            item.text_id = s_info_items[num].str_id;
            item.has_child = s_info_items[num].has_child;
            item.callback = s_info_items[num].callback;

            //to do
            item.info1 = text_info[num];//"test info";
            
            menu_append_item( s_info, &item );
        }

        //init current index
        menu_set_offset( s_info, s_info_offset );
        menu_set_cur_index( s_info, s_info_current );
    }

    return s_info;
}

int music_playing_option_info_deinit( void )
{    
    //delete menu    
    if( s_info != NULL )
    {
        //save index
        s_info_offset = s_info->offset;
        s_info_current = s_info->current;
        
        //create menu
        menu_delete( s_info );
        s_info = NULL;
    }

    return true;
}

static int _proc_items( menuview_t *menuview, menuview_sig_e signal )
{    
    switch ( signal ) 
    {
    case WIDGET_SIG_MENUVIEW_OK:
        //toDo------------
        break;

    case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        music_playing_option_info_exit( menuview );
        break;
         
    default:
        break;
    }    

    return true;
}



