/********************************************************************************
 *                              USDK(1100)
 *                             Module: app 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      shaofeng    2009-05-16          v1.0               build this file 
********************************************************************************/
/*!
 * \file     load_menuitem.h
 * \brief    
 * \author   shaofeng
 *               
 * \version 1.0
 * \date  2009/05/16
*******************************************************************************/

#ifndef __LOAD_MENUITEM_H__
#define __LOAD_MENUITEM_H__

#include "include_case.h"


/*本应用的头文件*/
#include "video.h"


/************************ extern function, 函数声明 ***************************/
extern bool video_load_menuitem( unsigned int resgroup_id, const video_menu_info_t *video_menuitem, int num, menuitem_t *items );
extern bool video_load_taglist_menuitem( int num, video_tag_info_t *tag_info, menuitem_t *menuitem );

#endif /* __LOAD_MENUITEM_H__ */

