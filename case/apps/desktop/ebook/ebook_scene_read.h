/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : scene_listmenu.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    zhaowenjie    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     scene_listmenu.h
* \brief    listmenu scene
* \author   zhaowenjie
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/


#ifndef __EBOOK_SCENE_READ_H__
#define __EBOOK_SCENE_READ_H__

#include "include_case.h"

#include "ebook.h"

typedef enum
{
    HAND_READING = 0,
    AUTO_READING
    
}control_mode_e;

typedef struct
{
    void (*const enter_scene_read)(void);
    void (*const clear_current_reading_file)(void);
    bool (*const close_reading_book)(void);

    bool (*const is_reading)(void);
    void (*const set_file_path)( const char* path, const char* name );
    void (*const set_file_name)( const char* name );
    void (*const set_file_full_path)( const char* full_path );
    char* (*const get_file_path)(void);
    char* (*const get_file_name)(void);
    file_type_e (*const get_file_type)(void);

    void (*const set_font_layout)( unsigned short index, int x, int y, int width, int size, int line_height, int max_line );
    void (*const set_font_type)( font_type_e type );
    font_type_e (*const get_font_type)(void);
    void (*const change_font_type)( font_type_e type );

    void (*const set_font_color_num)( unsigned int num );
    unsigned int (*const get_font_color_num)();
    void (*const set_ui_font_colors)( unsigned int index, int color, int bg_color );
    void (*const set_ui_use_color_background)( bool use );
    
    void (*const set_font_color)( unsigned int color );
    unsigned int (*const get_font_color)(void);
    void (*const change_font_color)( unsigned int color );

    void (*const set_control_mode)( control_mode_e mode );
    void (*const set_auto_play_gap)( unsigned int gap );
    unsigned int (*const get_auto_play_gap)(void);
    void (*const set_language_mode)( unsigned int language );
    void (*const change_language_mode)( unsigned int language );
    unsigned int (*const get_language_mode)(void);
    
    void (*const creat_skip_to_adjust)(void);
    void (*const creat_auto_play_gap_adjust)(void);
    void (*const try_delete_current_file)(void);

    void (*const set_current_page_num)( unsigned int page );
    int (*const get_current_page_start_point)(void);
    void (*const set_current_page_start_point)( int point );
    void (*const revise_current_page_start_point)( int point );

    void (*const clear_option_menuview)( void );
    void (*const restart_auto_reading_timer)(void);
    
    void (*const add_current_page_to_bookmark)(void);
    void (*const set_bookmark)( plist_detail_info_t* bookmark );

    void (*const update_cur_total_page)(void);
    void (*const kill_update_page_timer)(void);
    bool (*const count_page_finished)(void);

    void (*const set_fusion_mode)( fusion_mode_e mode );
    fusion_mode_e (*const get_fusion_mode)(void);
    void (*const set_fusion_rect)( short x, short y, short width, short height );

    void ( *const set_option_has_change_size_item )(void);

}scene_read_t;

scene_read_t* create_scene_read();

#endif /* __EBOOK_SCENE_READ_H__ */
