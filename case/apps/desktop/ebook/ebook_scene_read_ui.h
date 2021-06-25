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


#ifndef __EBOOK_SCENE_READ_UI_H__
#define __EBOOK_SCENE_READ_UI_H__

#include "include_case.h"

#include "ebook.h"

typedef struct
{
    bool (*const load_style_resource)( scene_t* scene );
    void (*const deinit)(void);
    
    bool (*const set_hdc)( int hdc );
    void (*const set_dc_font_size)( int size );
    void (*const set_ui_font_color)( unsigned int color );
    
    bool (*const paint_initial_ui )( control_mode_e control_mode, unsigned char* filename );
    bool (*const paint_text_background )(void);
    bool (*const paint_text ) ( char* show_buffer, font_layout_t layout, unsigned int show_lines, 
                    file_type_e type, unsigned int language );
    bool (*const paint_control_mode )( control_mode_e control_mode );
    void (*const paint_cur_total_page )( unsigned char* page_string );

    void (*const set_font_colors)( unsigned int index, int color, int bg_color );
    void (*const set_use_color_background)( bool use );
    bool (*const use_color_background)(void);
    
}scene_read_ui_t;

scene_read_ui_t* create_scene_read_ui(void);

#endif /* __EBOOK_SCENE_READ_UI_H__ */

