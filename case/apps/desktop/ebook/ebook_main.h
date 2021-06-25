/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : ebook_main.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    zhaowenjie    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     ebook_main.h
* \brief    ebook main head file
* \author   zhaowenjie
*               
* \version 1.0
* \date  2009/06/01
*******************************************************************************/


#ifndef __EBOOK_MAIN_H__
#define __EBOOK_MAIN_H__

#include "include_case.h"
#include "ebook.h"

typedef struct
{
    style_t* (*const get_style )(void);
    bool (*const change_common_ui_scene)(void);
    
    void (*const set_current_scene)( scene_e scene );
    scene_e (*const get_current_scene)(void);
    void (*const go_back_last_scene)(void);
    scene_e (*const get_last_scene)(void);
        
    unsigned short (*const get_headbar_pid)(void);
    gui_bitmap_info_t* (*const get_background_picture)(void);
    void (*const update_background_picture)(void);

    string_resource_t* (*const get_no_disk_string)(void);
    picture_resource_t* (*const get_dot_picture)(void);
    string_resource_t* (*const get_ebook_string)(void);
    string_resource_t* (*const get_sure_delete_string)(void);
    string_resource_t* (*const get_deleting_string)(void);
    string_resource_t* (*const get_not_found_string)(void);
    string_resource_t* (*const get_file_open_fail_string)(void);

    bool (*const check_file_exists)( char* file_path );
    bool (*const remove_file)( char* file );
    
}ebook_main_t;

#endif /* __EBOOK_MAIN_H__ */
