/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : menuview_private.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     menuview_private.h
* \brief    menuview widget private interface
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#ifndef __MENUVIEW_PRIVATE_H__
#define __MENUVIEW_PRIVATE_H__


#include "include_case.h"


/* define text scroll interval */
#define SCROLL_INTERVAL 100

/* define menuview private data */
typedef struct menuview_private_s
{
    /*! »æÍ¼DC */
    int hdc;

    /*! slider¿Ø¼þÖ¸Õë */
    slider_t *slider;
    slider_init_t slider_param;

    /* secure flag */
    bool is_hidden;  //!<ÊÇ·ñÒþ²Ø
    
    /* scroll text */
    int scroll_timer;
    gui_scroll_info_t *scroll_info;
    gui_bitmap_info_t *scroll_bg;
    int scroll_bg_x;
    int scroll_bg_y;
    
    /* attr for painting */
    int lines;          //!< the number of lines in one screen    
    int sel_height;     //!< the height of selected line   
    int unsel_height;   //!< the height of unselected line   
    int xstart;         //!< the x of first line   
    int ystart;         //!< the y of first line  
    bool bg_inited;     //!< bg init flag  
    int prefetch;       //!< prefetch line number  
    
    /* resource for painting */
    resgroup_resource_t *resgroup;
    picture_resource_t *pic_bg;
    picture_resource_t *pic_sel_folder;
    picture_resource_t *pic_unsel_folder;
    
    /* selected line */
    picture_resource_t *pic_sel_line;
    picture_resource_t *pic_sel_icon_bg;
    picture_resource_t *pic_sel_icon;
    string_resource_t *str_sel_name;
    string_resource_t *str_sel_str1;
    string_resource_t *str_sel_str2;
    picture_resource_t *pic_sel_son;    
    
    /* unselected line */
    picture_resource_t *pic_unsel_line;
    picture_resource_t *pic_unsel_icon;
    picture_resource_t *pic_unsel_icon_bg;
    string_resource_t *str_unsel_name;
    string_resource_t *str_unsel_str1;
    string_resource_t *str_unsel_str2;
    picture_resource_t *pic_unsel_son;
    
    gui_bitmap_info_t *bmp_bg;  //bitmap cache
    gui_bitmap_info_t *bmp_sel_line;  //bitmap cache
    gui_bitmap_info_t *bmp_sel_icon_bg;  //bitmap cache
    gui_bitmap_info_t *bmp_sel_son;  //bitmap cache
    gui_bitmap_info_t *bmp_unsel_line;  //bitmap cache
    gui_bitmap_info_t *bmp_unsel_icon_bg;  //bitmap cache
    gui_bitmap_info_t *bmp_unsel_son;  //bitmap cache
    gui_bitmap_info_t *bmp_sel_folder;
    gui_bitmap_info_t *bmp_unsel_folder;
    
}menuview_private_t;


#endif

