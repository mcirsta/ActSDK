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
 * \file     radio_playing_widget_option.c
 * \brief    radio_playing widget
 * \author   wangsheng
 *
 * \version 1.0
 * \date  2009/02/25
 *******************************************************************************/
#include "radio_playing_widget_option.h"

static option_entry_t g_option_entry = NULL;
static menuview_cb_t g_widget_option_cb = NULL;

/******************************************************************************/
/*!                    
 * \par  Description:     menu view callback
*******************************************************************************/
static void _menuview_callback(menuview_t *menuview, menuview_sig_e signal)
{
}

/******************************************************************************/
/*!                    
 * \par  Description:     widget option create
*******************************************************************************/
menuview_t* radio_playing_widget_option_create(menuview_cb_t callback)
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
        if (g_option_entry != NULL)
        {
            (g_option_entry)(menuview);
        }
        else
        {
            radio_playing_option_enter(menuview, fenter);
        }

        g_widget_option_cb = callback;
    }
    return menuview;
}

/******************************************************************************/
/*!                    
 * \par  Description:     widget option delete
*******************************************************************************/
bool radio_playing_widget_option_delete(menuview_t *menuview)
{
    if (menuview == NULL)
    {
        print_err("menuview delete fail!");
        return false;
    }
    else
    {
        menuview_delete(menuview);

        return true;
    }
}
#if 0
bool radio_playing_widget_option_load_style(menuview_t *menuview)
{
    if (menuview != NULL)
    {
        return menuview_load_style(menuview, menuview->type);
    }
    else
    {
        return false;
    }
}


bool radio_playing_widget_option_paint(menuview_t *menuview)
{
    return menuview_paint(menuview);
}

bool radio_playing_widget_option_proc_gui(menuview_t *menuview, gui_wm_message_t *pmsg)
{
    return menuview_proc_gui(menuview, pmsg);
}
#endif
/******************************************************************************/
/*!                    
 * \par  Description:     widget option exit
*******************************************************************************/
bool radio_playing_widget_option_exit(menuview_t *menuview, option_entry_t entry)
{
    /*save last menu entry*/
    g_option_entry = entry;

//    radio_playing_option_deinit();
    radio_playing_option_level_deinit();
    radio_playing_option_band_deinit();
    radio_playing_option_path_deinit();
    radio_playing_option_channellist_deinit();
    radio_playing_option_userchannellist_deinit();
    radio_playing_option_userfilelist_deinit();

    /*callback*/
    if (g_widget_option_cb != NULL)
    {
        (g_widget_option_cb)(menuview, WIDGET_SIG_MENUVIEW_QUIT);
        return true;
    }
    else
    {
        return false;
    }
}

