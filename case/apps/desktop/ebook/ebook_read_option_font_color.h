/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : ebook_read_option_font_color.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    zhaowenjie    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     ebook_read_option_font_color.h
* \brief    listmenu scene
* \author   zhaowenjie
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/


#ifndef __EBOOK_READ_OPTION_FONT_COLOR_H__
#define __EBOOK_READ_OPTION_FONT_COLOR_H__

#include "include_case.h"

#include "ebook_read_option.h"

typedef struct
{
    bool (*const read_option_font_color_enter )( menuview_t* menuview, resgroup_resource_t* resgroup );
    void (*const read_option_font_color_deinit)(void);
}read_option_font_color_t;

read_option_font_color_t* create_read_option_font_color( scene_read_option_t* read_option );

#endif /* __EBOOK_READ_OPTION_FONT_COLOR_H__ */

