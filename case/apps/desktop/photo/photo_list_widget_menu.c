/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_list_widget_menu.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *       cheason    2009-06-14         v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_list_widget_menu.c
 * \brief    photo list widget
 * \author   cheason
 *
 * \version 1.0
 * \date  2009/06/14
 *******************************************************************************/

#include "photo_scene_list.h"
#include "photo_list_widget_menu.h"
#include "photo_list_mainmenu.h"

static menuview_cb_t s_widget_menu_cb = NULL;
static menu_entry_t s_menu_entry = NULL;

menuview_t *photo_list_widget_menu_create(menuview_cb_t callback, fusion_enter_e fenter)
{
    menuview_t *mv = menuview_create(MENUVIEW_MENULIST_ICON, callback);
    if (NULL == mv)
    {
        print_err("menuview create fail!");
        return NULL;
    }
    else
    {
        /*restore last menu*/
        if (s_menu_entry != NULL)
        {
            (s_menu_entry) (mv, fenter);
        }
        else
        {
            photo_list_mainmenu_enter(mv, fenter);
        }

        /*save callback*/
        s_widget_menu_cb = callback;
    }
    return mv;
}

bool photo_list_widget_menu_delete(menuview_t *mv)
{
    if (NULL != mv)
    {
        menuview_delete(mv);
    }
    return true;
}

bool photo_list_widget_menu_proc_gui(menuview_t *mv, gui_wm_message_t *pmsg)
{
    return menuview_proc_gui(mv, pmsg);
}

bool photo_list_widget_menu_exit(menuview_t *mv, menu_entry_t entry)
{
    s_menu_entry = entry;

    if (s_widget_menu_cb != NULL)
    {
        /* restore callback */
        (s_widget_menu_cb) (mv, WIDGET_SIG_MENUVIEW_QUIT);
    }

    return true;
}

