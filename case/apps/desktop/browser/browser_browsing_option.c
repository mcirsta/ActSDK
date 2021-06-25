/********************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : browser_browsing_option.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      binannan    2009-05-09          v1.0               build this file
 ********************************************************************************/
#include "include_case.h"
#include "include_psp.h"
#include "browser_scene_browsing.h"
#include "browser_browsing_option.h"
#include "browser_browsing_widget_option.h"

/*
 * 局部静态函数列表
 */
/*********************************************************************/
static int _opt_search_file_process(menuview_t *data, menuview_sig_e signal);
static int _opt_select_all_process(menuview_t *data, menuview_sig_e signal);
static int _opt_move_process(menuview_t *data, menuview_sig_e signal);
static int _opt_copy_process(menuview_t *data, menuview_sig_e signal);

static void _com_softkey_cb(softkeyboard_t * keyboard, softkeyboard_sig_e signal);
static void _deal_searching(void);
static void _searching_done(void);
static void _com_dialog_cb(dialog_t *dialog, widget_sig_dialog_e signal);
/*********************************************************************/

/* menu list data define */
menu_t *g_menu_option = NULL;

bool g_low_power_dialog_visible = false;

/*********************************************************************/

/* option menu process */
static int _opt_search_file_process(menuview_t *data, menuview_sig_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            browser_browsing_widget_option_delete(data);
            if (g_scene_browsing->filelist->content_mode == CONTENT_NORMAL)
            {
                if(g_scene_browsing->filelist->select_mode == true)
                {
                    browser_widget_filelist_toggle_select_mode();
                }
                g_scene_browsing->softkey = softkeyboard_create(IME_TYPE_PINYIN, _com_softkey_cb);
                if (g_scene_browsing->softkey == NULL)
                {
                    print_err("create soft keyboard widget failed!");
                    return -1;
                }
                browser_widget_filelist_hide(g_scene_browsing->filelist);

                softkeyboard_paint(g_scene_browsing->softkey);
            }
        }
        break;
        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            print_dbg();
            browser_browsing_widget_option_delete(data);
        }
        break;

        default:
        break;
    }
    return 0;
}

static int _opt_select_all_process(menuview_t *data, menuview_sig_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            browser_browsing_widget_option_delete(data);

            if (sys_power_threshold_check(POWER_THRESHOLD_FILE_OP) == false)
            {
                break;
            }

            browser_widget_filelist_toggle_select_mode();
        }
        break;
        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            print_dbg();
            browser_browsing_widget_option_delete(data);
        }
        break;

        default:
        break;
    }
    return 0;
}

static int _opt_move_process(menuview_t *data, menuview_sig_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            browser_browsing_widget_option_delete(data);
            if (sys_power_threshold_check(POWER_THRESHOLD_FILE_OP) == false)
            {
                dialog_init_t init;
                init.head = DIALOG_HEAD_LOWBAT;
                init.str_id = g_scene_browsing->str_low_power->str_id;

                g_low_power_dialog_visible = true;
                dialog_modal(&init);
                g_low_power_dialog_visible = false;
                break;
            }
            g_scene_browsing->filelist->content_mode = CONTENT_MOVE;
            browser_widget_filelist_enter_move_mode();
        }
        break;
        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            print_dbg();
            browser_browsing_widget_option_delete(data);
        }
        break;

        default:
        break;
    }
    return 0;
}
static int _opt_copy_process(menuview_t *data, menuview_sig_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            browser_browsing_widget_option_delete(data);
            if (sys_power_threshold_check(POWER_THRESHOLD_FILE_OP) == false)
            {
                dialog_init_t init;
                init.head = DIALOG_HEAD_LOWBAT;
                init.str_id = g_scene_browsing->str_low_power->str_id;
                g_low_power_dialog_visible = true;
                dialog_modal(&init);
                g_low_power_dialog_visible = false;
                break;
            }
            g_scene_browsing->filelist->content_mode = CONTENT_COPY;
            browser_widget_filelist_enter_copy_mode();
        }
        break;
        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            print_dbg();
            browser_browsing_widget_option_delete(data);
        }
        break;

        default:
        break;
    }
    return 0;
}

static void _com_softkey_cb(softkeyboard_t * keyboard, softkeyboard_sig_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_SKB_OK:
        {
            print_dbg("WIDGET_SIG_SKB_OK");
            com_create_loadingbox_widget();
            create_thread(_deal_searching, NULL);
        }
        break;

        case WIDGET_SIG_SKB_CANCEL:
        {
            print_dbg("WIDGET_SIG_SKB_CANCEL");
            softkeyboard_delete(g_scene_browsing->softkey);
            g_scene_browsing->softkey = NULL;
            browser_widget_filelist_show(g_scene_browsing->filelist);
        }
        break;

        default:
        break;
    }
}

static void _deal_searching(void)
{
    char text[128];

    //get text
    softkeyboard_get_text(g_scene_browsing->softkey, text, sizeof(text));

    //set filter string
    browser_widget_filelist_enter_search_mode(text);

    //process after searching
    set_single_shot_timer(1, (timer_proc) _searching_done, NULL);
}

static void _searching_done(void)
{
    com_delete_loadingbox_widget();

    if ((g_scene_browsing->filelist->data->content->content_list->menu->total) == 0)
    {
        browser_widget_filelist_show_no_matching_items();

        browser_widget_filelist_enter_search_mode("");
    }
    else
    {
        softkeyboard_delete(g_scene_browsing->softkey);
        g_scene_browsing->softkey = NULL;

        browser_widget_filelist_show(g_scene_browsing->filelist);
    }
    g_thread_active = false;
}

static void _com_dialog_cb(dialog_t *dialog, widget_sig_dialog_e signal)
{
    dialog_init_t dialog_init;
    print_dbg("signal:%d", signal);
    switch (signal)
    {
        case WIDGET_SIG_DIALOG_OK:
        {
            bool result;

            dialog_delete(g_scene_browsing->dialog);
            g_scene_browsing->dialog = NULL;

            dialog_init.head = DIALOG_HEAD_INFO;
            dialog_init.str_id = g_scene_browsing->str_deleting->str_id;
            result = com_create_noop_dialog_widget(&dialog_init);
            if (true == result)
            {
                dialog_paint(g_scene_browsing->dialog);
                gui_screen_update();
            }

            sys_forbid_screensaver(true);
            sys_forbid_auto_standby(true);
            sys_forbid_timed_shutdown(true);
            sys_enable_sys_evt_detect(false);
            g_scene_browsing->filelist->content_mode = CONTENT_DELETE;
            create_thread(browser_widget_filelist_exec_delete_fileop, NULL);
        }
        break;

        case WIDGET_SIG_DIALOG_CANCAL:
        {
            dialog_delete(g_scene_browsing->dialog);
            g_scene_browsing->dialog = NULL;
        }
        break;

        default:
        break;
    }
}

menu_t* browser_browsing_option_init(void)
{
    scene_t* scene;
    resgroup_resource_t* option_group;

    //initial menu
    if (g_menu_option == NULL)
    {
        menuitem_t  i_search_file;
        string_resource_t* str_search_file = NULL;

        menuitem_t i_select_all;
        string_resource_t* str_select_all = NULL;

        menuitem_t i_move ;
        string_resource_t* str_move = NULL;

        menuitem_t i_copy;
        string_resource_t* str_copy = NULL;

        menuitem_t i_delete;
        string_resource_t* str_delete = NULL;

        //create menu
        g_menu_option = menu_create();
        if (g_menu_option == NULL)
        {
            print_err("menu_create failed.");
            return NULL;
        }

        scene = sty_get_app_scene();
        if (scene == NULL)
        {
            print_err("call sty_get_common_scene failed.\n");
            menu_delete(g_menu_option);
            g_menu_option = NULL;
            return NULL;
        }

        option_group = (resgroup_resource_t*) get_scene_child(scene, OPTION_RESGROUP);
        if (option_group != NULL)
        {
            //menu item 'search file'
            memset(&i_search_file, 0, sizeof(menuitem_t));

            str_search_file = (string_resource_t*) get_resgroup_child(option_group, STR_SEARCH_FILE);
            if (str_search_file != NULL)
            {
                i_search_file.text_id = str_search_file->str_id;
            }
            i_search_file.has_child = true;
            i_search_file.callback = (item_cb_t) _opt_search_file_process;

            menu_append_item(g_menu_option, &i_search_file);

            //menu item 'select/cancel all'
            memset(&i_select_all, 0, sizeof(menuitem_t));

            str_select_all = (string_resource_t*) get_resgroup_child(option_group, STR_SELECT_ALL);
            if (str_select_all != NULL)
            {
                i_select_all.text_id = str_select_all->str_id;
            }
            i_select_all.has_child = true;
            i_select_all.callback = (item_cb_t) _opt_select_all_process;

            menu_append_item(g_menu_option, &i_select_all);

            //menu item 'move'
            memset(&i_move, 0, sizeof(menuitem_t));

            str_move = (string_resource_t*) get_resgroup_child(option_group, STR_MOVE);
            if (str_move != NULL)
            {
                i_move.text_id = str_move->str_id;
            }
            i_move.has_child = true;
            i_move.callback = (item_cb_t) _opt_move_process;

            menu_append_item(g_menu_option, &i_move);

            //menu item 'copy'
            memset(&i_copy, 0, sizeof(menuitem_t));

            str_copy = (string_resource_t*) get_resgroup_child(option_group, STR_COPY);
            if (str_copy != NULL)
            {
                i_copy.text_id = str_copy->str_id;
            }
            i_copy.has_child = true;
            i_copy.callback = (item_cb_t) _opt_copy_process;

            menu_append_item(g_menu_option, &i_copy);

            //menu item 'delete'
            memset(&i_delete, 0, sizeof(menuitem_t));

            str_delete = (string_resource_t*) get_resgroup_child(option_group, STR_DELETE);
            if (str_delete != NULL)
            {
                i_delete.text_id = str_delete->str_id;
            }
            i_delete.has_child = true;
            i_delete.callback = (item_cb_t) opt_delete_process;
            menu_append_item(g_menu_option, &i_delete);

            menu_set_cur_index(g_menu_option, 0);
            return g_menu_option;
        }
        else
        {
            menu_delete(g_menu_option);
            g_menu_option = NULL;
            return NULL;
        }
    }
    else
    {
        return NULL;
    }
}



int browser_browsing_option_deinit(void)
{
    if (g_menu_option != NULL)
    {
        menu_delete(g_menu_option);
        g_menu_option = NULL;
    }
    return true;
}

/* 进入主菜单 */
bool browser_browsing_option_enter(menuview_t *menuview)
{
    menu_t *cur_menu;

    cur_menu = browser_browsing_option_init();
    menuview_set_menu(menuview, cur_menu);
    return true;
}

/* 退出主菜单 */
bool browser_browsing_option_exit(menuview_t *menuview)
{
    //delete main menu
    browser_browsing_option_deinit();

    //exit option_ui
    browser_browsing_widget_option_exit(menuview, NULL);

    return true;
}

int opt_delete_process(menuview_t *data, menuview_sig_e signal)
{
    dialog_init_t dialog_init;
    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            if(data != NULL)
            {
                browser_browsing_widget_option_delete(data);
            }

            if (sys_power_threshold_check(POWER_THRESHOLD_FILE_OP) == false)
            {
                dialog_init.head = DIALOG_HEAD_LOWBAT;
                dialog_init.str_id = g_scene_browsing->str_low_power->str_id;
                g_low_power_dialog_visible = true;
                dialog_modal(&dialog_init);
                g_low_power_dialog_visible = false;
                break;
            }

            dialog_init.head = DIALOG_HEAD_QUERY;
            dialog_init.str_id = g_scene_browsing->str_delete_confirm->str_id;
            dialog_init.def_sel=WIDGET_SIG_DIALOG_CANCAL;

            g_scene_browsing->dialog = dialog_create(&dialog_init, (dialog_cb_t) _com_dialog_cb);

            if (NULL == g_scene_browsing->dialog)
            {
                print_err("dialog_create failed\n\n");
                return -1;
            }

            dialog_paint(g_scene_browsing->dialog);
        }
        break;
        case WIDGET_SIG_MENUVIEW_QUIT:
        {
            print_dbg();
            if(data != NULL)
            {
                browser_browsing_widget_option_delete(data);
            }
        }
        break;

        default:
        break;
    }
    return 0;
}

