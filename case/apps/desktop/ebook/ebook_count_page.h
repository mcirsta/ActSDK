/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : ebook_count_page.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    zhaowenjie    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     ebook_count_page.h
* \brief    listmenu scene
* \author   zhaowenjie
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/


#ifndef __EBOOK_COUNT_PAGE_H__
#define __EBOOK_COUNT_PAGE_H__

#include "include_case.h"
#include "ebook.h"


typedef struct
{
    unsigned int skip_point;      /* 快速跳转点 */
    unsigned int skip_point_page; /* 跳转点所在的页数 */
    unsigned int page_distance;   /* 还差的页数 */
    
}skip_point_t;

typedef struct
{
    void (*const set_hdc)( int hdc );
    void (*const set_font_layout)( font_layout_t* font_layout );

    bool (*const start_count_page)( unsigned char* ascii_width, unsigned char* font_width_buffer );
    bool (*const is_running)();
    void (*const stop_count_page)(void);

    int (*const get_total_page)(void);

    void (*const get_skip_point)( unsigned int percent, skip_point_t* point );
}count_page_t;


count_page_t* create_count_page(void);

#endif /* __EBOOK_COUNT_PAGE_H__ */

