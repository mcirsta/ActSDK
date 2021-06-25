/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : recorder_recording_widget_option.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *    wangsheng    2009-2-25          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     recorder_recording_widget_option.c
 * \brief    recorder_recording widget
 * \author   wangsheng
 *
 * \version 1.0
 * \date  2009/02/25
 *******************************************************************************/
#include "recorder_recording_widget_option.h"
#include "recorder_scene_recording.h"

static option_entry_t g_option_entry = NULL;
static menuview_cb_t g_widget_option_cb = NULL;

static void _menuview_callback(menuview_t *menuview, menuview_sig_e signal)
{
}

menuview_t* recorder_recording_widget_option_create(menuview_cb_t callback)
{
    menuview_t *menuview;
    fusion_type_e fenter;

    fenter = FUSION_FORWARD;

    /*create list*/
    menuview = menuview_create(MENUVIEW_OPTION, _menuview_callback);
    if (menuview == NULL)
    {
        print_err("menuview create fail!");
        return NULL;
    }
    else
    {
        /*restore last menu*/
        if (g_option_entry != NULL)
        {
            (g_option_entry) (menuview);
        }
        else
        {
            recorder_recording_option_enter(menuview, fenter);
        }

        /*save callback*/
        g_widget_option_cb = callback;
    }
    return menuview;
}

bool recorder_recording_widget_option_delete(menuview_t *menuview)
{
    if (menuview == NULL)
    {
        print_err("menuview delete fail!");
        return false;
    }
    else
    {
        /*delete menuview*/
        menuview_delete(menuview);

        return true;
    }
}
#if 0
bool recorder_recording_widget_option_load_style( menuview_t *menuview )
{
    if ( menuview != NULL )
    {
        return menuview_load_style( menuview, menuview->type );
    }
    else
    {
        return false;
    }
}

bool recorder_recording_widget_option_paint( menuview_t *menuview )
{
    return menuview_paint( menuview );
}

bool recorder_recording_widget_option_proc_gui( menuview_t *menuview, gui_wm_message_t *pmsg )
{
    return menuview_proc_gui( menuview, pmsg );
}
#endif
bool recorder_recording_widget_option_exit(menuview_t *menuview, option_entry_t entry)
{
    /*save last menu entry*/
    g_option_entry = entry;

    /*callback*/
    if (g_widget_option_cb != NULL)
    {
        (g_widget_option_cb) (menuview, WIDGET_SIG_MENUVIEW_QUIT);
        return true;
    }
    else
    {
        return false;
    }
}
