/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_list_mainmenu.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason    2009-06-14         v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_list_mainmenu.h
 * \brief    mainmenu of photo
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/06/14
 *******************************************************************************/

#ifndef __PHOTO_LIST_MAINMENU_H__
#define __PHOTO_LIST_MAINMENU_H__

#include "photo.h"

bool photo_list_mainmenu_enter(menuview_t *mv, fusion_enter_e fenter);
bool photo_list_mainmenu_exit(menuview_t *mv);
bool photo_list_mainmenu_remove_nowplaying(menuview_t *mv);

#endif

