/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : browser_browsing_widget_option.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *    binannan    2009-05-25          v1.0               build this file
 ********************************************************************************/
#include "browser_scene_browsing.h"
#include "browser_browsing_widget_option.h"
#include "browser_browsing_option.h"

/*
 * 局部静态函数列表
 */
/*********************************************************************/

static void _menuview_callback(menuview_t *menuview, menuview_sig_e signal);

/*********************************************************************/

static option_entry_t s_option_entry = NULL;

static menuview_cb_t s_widget_option_cb = NULL;

/*********************************************************************/


static void _menuview_callback(menuview_t *menuview, menuview_sig_e signal)
{
    print_dbg();
}

menuview_t* browser_browsing_widget_option_create(menuview_cb_t callback)
{
    menuview_t *menuview;

    //create list
    menuview = menuview_create(MENUVIEW_OPTION, _menuview_callback);
    if (menuview == NULL)
    {
        print_err("menuview create fail!");
        return NULL;
    }
    else
    {
        //restore last menu
        if (s_option_entry != NULL)
        {
            (s_option_entry)(menuview);
        }
        else
        {
            browser_browsing_option_enter(menuview);
        }

        //save callback
        s_widget_option_cb = callback;
    }

    return menuview;
}

bool browser_browsing_widget_option_delete(menuview_t *menuview)
{
    if (menuview == NULL)
    {
        print_dbg("the menuview is null.");
        return false;
    }
    else
    {
        //delete menuview
        menuview_delete(menuview);
        g_scene_browsing->opt = NULL;

        //delete menu
        browser_browsing_option_deinit();
        return true;
    }
}

bool browser_browsing_widget_option_load_style(menuview_t *menuview)
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

bool browser_browsing_widget_option_paint(menuview_t *menuview)
{
    return menuview_paint(menuview);
}

bool browser_browsing_widget_option_proc_gui(menuview_t *menuview, gui_wm_message_t *pmsg)
{
    return menuview_proc_gui(menuview, pmsg);
}

bool browser_browsing_widget_option_exit(menuview_t *menuview, option_entry_t entry)
{
    //save last menu entry
    s_option_entry = entry;

    //callback
    if (s_widget_option_cb != NULL)
    {
        (s_widget_option_cb)(menuview, WIDGET_SIG_MENUVIEW_QUIT);
        return true;
    }
    else
    {
        return false;
    }
}

