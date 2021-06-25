/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : ebook_read_option_page_effect.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    zhaowenjie    2009-5-23          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     ebook_read_option_page_effect.h
* \brief    listmenu scene
* \author   zhaowenjie
*               
* \version 1.0
* \date  2009/05/23
*******************************************************************************/


#ifndef __EBOOK_READ_OPTION_PAGE_EFFECT_H__
#define __EBOOK_READ_OPTION_PAGE_EFFECT_H__

#include "include_case.h"

#include "ebook_read_option.h"

typedef struct
{
    bool (*const read_option_page_effect_enter )( menuview_t* menuview, resgroup_resource_t* resgroup );
    void (*const read_option_page_effect_deinit)(void);
}read_option_page_effect_t;

read_option_page_effect_t* create_read_option_page_effect( scene_read_option_t* read_option );

#endif /* __EBOOK_READ_OPTION_PAGE_EFFECT_H__ */

