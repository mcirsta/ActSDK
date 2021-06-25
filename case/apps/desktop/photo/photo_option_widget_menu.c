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

#include "photo_scene_playing.h"
#include "photo_option_widget_menu.h"
#include "photo_option_rotate.h"
#include "photo_dec_ctrl.h"
#include "photo_option.h"

static menuview_cb_t s_widget_menu_cb = NULL;
static menu_entry_t s_menu_entry = NULL;

option_state_e g_cur_opt_state = OPT_MAIN;

menuview_t *photo_option_widget_menu_create(menuview_cb_t callback, fusion_enter_e fenter)
{
    menuview_t *mv = menuview_create(MENUVIEW_OPTION, callback);
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
            photo_option_enter(mv, fenter);
        }

        /*save callback*/
        s_widget_menu_cb = callback;
    }
    return mv;
}

bool photo_option_widget_menu_delete(menuview_t *mv)
{
    if (NULL != mv)
    {
        menuview_delete(mv);
    }
    return true;
}

bool photo_option_widget_menu_proc_gui(menuview_t *mv, gui_wm_message_t *pmsg)
{
    key_event_t * kmsg = (key_event_t *) (pmsg->data.p);

    if(pmsg->msgid == WM_MSG_KEY)
    {
        kmsg = (key_event_t *) (pmsg->data.p);
        if((KEY_MENU == kmsg->val) && (KEY_TYPE_DOWN == kmsg->type))
        {
            /* 某些菜单截获menu按键 */
            switch(g_cur_opt_state)
            {
            case OPT_ROTATE:
            case OPT_ZOOM:
                scene_playing_normal_display(g_playing);
                break;
            default:
                menuview_proc_gui(mv, pmsg);
                break;
            }
        }
        else
        {
            menuview_proc_gui(mv, pmsg);
        }
    }
    else
    {
        menuview_proc_gui(mv, pmsg);
    }

    return true;
}

bool photo_option_widget_menu_exit(menuview_t *mv, menu_entry_t entry)
{
    s_menu_entry = entry;

    if (s_widget_menu_cb != NULL)
    {
        /*restore callback*/
        (s_widget_menu_cb) (mv, WIDGET_SIG_MENUVIEW_QUIT);
    }

    return true;
}

