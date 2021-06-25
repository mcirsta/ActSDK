/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_list_widget_menu.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason   2009-06-14          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_list_widget_menu.h
 * \brief    photo list widget
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/06/14
 *******************************************************************************/

#ifndef __PHOTO_LIST_WIDGET_MENU_H__
#define __PHOTO_LIST_WIDGET_MENU_H__

#include "photo.h"

menuview_t *photo_list_widget_menu_create(menuview_cb_t callback, fusion_enter_e fenter);
bool photo_list_widget_menu_delete(menuview_t *mv);
bool photo_list_widget_menu_proc_gui(menuview_t *menuview, gui_wm_message_t *pmsg);
bool photo_list_widget_menu_exit(menuview_t *menuview, menu_entry_t entry);

#endif

