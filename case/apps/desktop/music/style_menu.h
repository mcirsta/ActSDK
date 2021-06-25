/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : style_menu.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-2-25          v1.0               build this file 
 ********************************************************************************/
/*!
* \file     style_menu.h
* \brief    style_menu operation
* \author   songzhining
*               
* \version 1.0
* \date  2009/02/25
*******************************************************************************/

#ifndef __STYLE_MENU_H__
#define __STYLE_MENU_H__

#include "include_case.h"


/* 定义style相关菜单数据 */
typedef struct
{
    int sty_str;    //style string resource
    bool has_child;
    item_cb_t callback;
    int sty_pic_sel;    //style picture resource
    int sty_pic_unsel;    //style picture resource

    int str_id;
    int pic_sel_id;
    int pic_unsel_id;
    
} style_menuitem_t;


/* 从style载入菜单数据 */
bool menu_load_style( int resgroup_id, style_menuitem_t *items, int total );

#endif

