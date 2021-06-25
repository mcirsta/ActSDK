/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : music_list_coverflow.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
 * \file     music_list_coverflow.h
 * \brief    music_list_coverflow widget interface
 * \author   songzhining
 *               
 * \version 1.0
 * \date  2009/02/25
 *******************************************************************************/

/*! 
 * \defgroup   music    music_list_coverflow控件接口函数
 */

#ifndef __MUSIC_LIST_COVERFLOW_H__
#define __MUSIC_LIST_COVERFLOW_H__


#include "music.h"


#define SIDE_SLIDE_NUM   3
#define SLIDE_INTERVAL   10

/*!
 *  \brief 
 *  coverflow_widget 控件句柄结构定义
 */
typedef struct coverflow_view_s
{
    /*! Coverflow操作数据 */
    void *lib_handle;
    RImage_t *image_list;
    int image_total;
    RImage_t image_default;
    RImage_t image_bg;
    bool is_hidden; //hidden flag
    
    /* resource for painting */
    picture_resource_t *pic_bg;
    picture_resource_t *pic_coverflow;
    string_resource_t *str_coverflow;
    
    gui_bitmap_info_t *bmp_bg;
    gui_bitmap_info_t *bmp_coverflow;
    gui_bitmap_info_t *bmp_album;

    int pic_width;
    int pic_height;
    sty_rect_t rect;
    
}coverflow_view_t;


bool coverflow_init( menu_t *menu );
bool coverflow_deinit( menu_t *menu );
bool coverflow_set_slide( menu_t *menu, int cur_index );
bool coverflow_next( menu_t *menu );
bool coverflow_prev( menu_t *menu );
bool coverflow_paint( int hdc, menu_t *menu );
bool coverflow_animation( int hdc, menu_t *menu );
int coverflow_get_slide_width( void );
int coverflow_get_slide_height( void );
bool coverflow_show( void );
bool coverflow_hide( void );

#endif

