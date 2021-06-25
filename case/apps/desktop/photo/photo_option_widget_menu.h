/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_option_widget_menu.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason    2009-06-14         v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_option_widget_menu.c
 * \brief    photo playing option widget
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/06/14
 *******************************************************************************/

#ifndef __PHOTO_OPTION_WIDGET_MENU_H__
#define __PHOTO_OPTION_WIDGET_MENU_H__

#include "photo.h"

/* option²Ëµ¥×´Ì¬ */
typedef enum
{
    OPT_MAIN = 0,
    OPT_ROTATE,
    OPT_ZOOM,
    OPT_SLIDE_EFFECT,
    OPT_SET_BACKGROUND,
    OPT_INFO,
}option_state_e;

menuview_t *photo_option_widget_menu_create(menuview_cb_t callback, fusion_enter_e fenter);
bool photo_option_widget_menu_delete(menuview_t *mv);
bool photo_option_widget_menu_proc_gui(menuview_t *menuview, gui_wm_message_t *pmsg);
bool photo_option_widget_menu_exit(menuview_t *menuview, menu_entry_t entry);

extern option_state_e g_cur_opt_state;

#endif

