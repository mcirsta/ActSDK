/*******************************************************************************
 *                              USDK(1100)
 *                            Module: camera_dv
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *        2009-5-26 16:31   1.0             build this file
 *******************************************************************************/
/*!
 * \file     camera_dv_option_free_space.c
 * \brief    概述
 * \author
 * \par      GENERAL DESCRIPTION:
 *               菜单项-剩余空间
 * \par      EXTERNALIZED FUNCTIONS:
 *               用到外面的模块
 * \version 1.0
 * \date  2009-5-26
 *******************************************************************************/
#include "camera_dv_option_free_space.h"
#include "camera_dv_option.h"
#include "camera_dv_control.h"
#include "camera_scene_dv.h"
#include "camera_dv_config.h"

#include "camera_sty.h"

/* listmenu entry */

static char s_str_free[64] = "FREE SPACE:XXXXMB";

static int _proc_items_do_free_space(menuview_t *menuview, menuview_sig_e signal);

/* menu list data define */
menu_t *g_camera_dv_option_free_space = NULL;

/* menu max items */
#define CAMERA_DV_OPTION_FREE_SPACE_MAX_ITEMS   1

/* 进入主菜单 */
bool camera_dv_option_free_space_enter(menuview_t *menuview)
{
    menu_t *cur_menu;

    cur_menu = camera_dv_option_free_space_init();
    menuview_set_menu(menuview, cur_menu);
    menuview_paint(menuview);

    return true;
}

/* 退出主菜单 */
bool camera_dv_option_free_space_exit(menuview_t *menuview)
{
    //delete free_space menu
    camera_dv_option_free_space_deinit();

    //return to option
    camera_dv_option_enter(menuview, FUSION_BACKWARD);

    return true;
}

menu_t* camera_dv_option_free_space_init(void)
{
    scene_t* scene; //!< app scene
    resgroup_resource_t *group;
    string_resource_t *str_res[CAMERA_DV_OPTION_FREE_SPACE_MAX_ITEMS] =
    { 0 };
    const char* str_idtemp;
    menuitem_t item =
    { 0 };
    mmm_disk_info_t freeinfo;

    //initial menu
    if (g_camera_dv_option_free_space == NULL)
    {
        //get scene
        scene = sty_get_app_scene();
        if (scene == NULL)
        {
            print_err("cann't find app scene!");
            return false;
        }

        //load resource group
        memset(s_str_free, 0, sizeof(s_str_free));
        group = (resgroup_resource_t *) get_scene_child(scene, DV_STRGROUP_OPTION_MAIN);
        str_res[0] = (string_resource_t *) get_resgroup_child(group, STR_FREE_SPACE);

        /*
        // 可以不用mmm直接用之前读取的
        //freeinfo.space_left = g_camera_scene_dv->dv_config.cfg_freeinfo.space_left;
        */
        //create menu
        g_camera_dv_option_free_space = menu_create();
        if (g_camera_dv_option_free_space == NULL)
        {
            print_err("cann't create menu!");
            return false;
        }
        else
        {
            g_camera_dv_option_free_space->callback = (menu_cb_t) _proc_items_do_free_space;
            if (g_camera_dv_cfgs[CAM_DV_CFG_TYPE_SAVE_PATH].value == CAM_DV_STORAGE_NONE)
            {
                camera_dv_dialog_create(CAM_DIALOG_NODISK);
            }
            else if(g_setpath == false)
            {
                camera_dv_dialog_create(CAM_DIALOG_SETPATH_FAIL);
            }
            else
            {
                camera_dv_ctr_get_disk_info(&freeinfo);
                if(str_res[0] != NULL)
                {
                    str_idtemp = sty_get_string((int) str_res[0]->str_id);
                    snprintf(s_str_free, sizeof(s_str_free), "%s:%dMB", str_idtemp, freeinfo.space_left / 1024);
                    //qac nest >5 print_info("menu show free space=%s\n", s_str_free);
                }
                //init items
                memset(&item, 0, sizeof(item));
                // append items
                item.has_child = false;
                item.callback = (item_cb_t) _proc_items_do_free_space;
                item.sel_icon_id = 0;
                item.unsel_icon_id = 0;
                item.text = s_str_free;
                menu_append_item(g_camera_dv_option_free_space, &item);
            }
        }
        //init current index
        menu_set_cur_index(g_camera_dv_option_free_space, 0);
    }

    return g_camera_dv_option_free_space;
}

int camera_dv_option_free_space_deinit(void)
{
    //delete menu
    if (g_camera_dv_option_free_space != NULL)
    {
        //create menu
        menu_delete(g_camera_dv_option_free_space);
        g_camera_dv_option_free_space = NULL;
    }

    return true;
}
static int _proc_items_do_free_space(menuview_t *menuview, menuview_sig_e signal)
{
    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:

        break;

        case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        camera_dv_option_free_space_exit(menuview);
        break;

        default:
        break;
    }
    return 0;
}

bool camera_dv_option_free_space_refresh(void)
{
    if (g_camera_dv_option_free_space != NULL)
    {
        menuview_t *menuview = g_camera_scene_dv->opt;
        menu_t *cur_menu;
        camera_dv_option_free_space_exit(menuview);
        cur_menu = (menu_t *) camera_dv_option_free_space_init();
        menuview_set_menu(menuview, cur_menu);
        menuview_paint(menuview);
    }
    return true;
}

