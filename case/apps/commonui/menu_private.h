/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * File : menu_private.h
 * History:         
 *      <author>    <time>           <version >             <desc>
 *    songzhining    2009-04-21         v1.0               build this file 
 ********************************************************************************/
/*!
* \file     menu_private.h
* \brief    menu data operation
* \author   songzhining
*               
* \version 1.0
* \date  2009/04/21
*******************************************************************************/

#ifndef __MENU_PRIVATE_H__
#define __MENU_PRIVATE_H__


#include "include_case.h"



/* define inner menu item */
typedef struct menuitem_inner_s
{
    menuitem_t data;    //!< item content
    struct menuitem_inner_s *next;    //!< next item
    
}menuitem_inner_t;


#endif

