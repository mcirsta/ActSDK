/*******************************************************************************
 *                              USDK(1100)
 *                            Module: camera
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *        2009-5-26 16:30   1.0             build this file
 *******************************************************************************/
/*!
 * \file     camera_dv_option_exposure_mode.c
 * \brief    概述
 * \author
 * \par      GENERAL DESCRIPTION:
 *               菜单项-曝光模式
 * \par      EXTERNALIZED FUNCTIONS:
 *               用到外面的模块
 * \version 1.0
 * \date  2009-5-26
 *******************************************************************************/
#include "camera_dv_option_exposure_mode.h"

#include "camera_dv_config.h"
#include "camera_sty.h"
#include "camera_dv_option.h"
#include "camera_dv_ui.h"
#include "camera_dv_control.h"
#include "camera_define.h"

/* listmenu entry */
static int _proc_items_do_exposure_mode(menuview_t *menuview, menuview_sig_e signal);

/* menu list data define */
menu_t *g_camera_dv_option_exposure_mode = NULL;

/* menu max items */
#define CAMERA_DV_OPTION_EXPOSURE_MODE_MAX_ITEMS   3

/* 进入主菜单 */
bool camera_dv_option_exposure_mode_enter(menuview_t *menuview)
{
    menu_t *cur_menu;

    cur_menu = camera_dv_option_exposure_mode_init();
    menuview_set_menu(menuview, cur_menu);
    menuview_paint(menuview);

    return true;
}

/* 退出主菜单 */
bool camera_dv_option_exposure_mode_exit(menuview_t *menuview)
{
    //delete exposure_mode menu
    camera_dv_option_exposure_mode_deinit();

    //return to option
    camera_dv_option_enter(menuview, FUSION_BACKWARD);

    return true;
}

menu_t* camera_dv_option_exposure_mode_init(void)
{
    scene_t* scene; //!< app scene
    resgroup_resource_t *group;
    string_resource_t *str_res[CAMERA_DV_OPTION_EXPOSURE_MODE_MAX_ITEMS] =
    { 0 };
    //picture_resource_t *g_cam_dv_opt_dot;

    menuitem_t item =
    { 0 };
    int num;

    //initial menu
    if (g_camera_dv_option_exposure_mode == NULL)
    {
        //get scene
        scene = sty_get_app_scene();
        if (scene == NULL)
        {
            print_err("cann't find app scene!");
            return false;
        }

        //load resource group
        group = (resgroup_resource_t *) get_scene_child(scene, DV_STRGROUP_EXPOSURE_MODE);
        str_res[0] = (string_resource_t *) get_resgroup_child(group, STR_AUTO);
        str_res[1] = (string_resource_t *) get_resgroup_child(group, STR_INDOOR);
        str_res[2] = (string_resource_t *) get_resgroup_child(group, STR_OUTDOOR);

        //init items
        memset(&item, 0, sizeof(item));

        //create menu
        g_camera_dv_option_exposure_mode = menu_create();
        if (g_camera_dv_option_exposure_mode == NULL)
        {
            print_err("cann't create menu!");
            return false;
        }
        else
        {
            // append items
            for (num = 0; num < CAMERA_DV_OPTION_EXPOSURE_MODE_MAX_ITEMS; num++)
            {
                if(str_res[num] != NULL)
                {
                    item.text_id = (int) str_res[num]->str_id;
                }
                item.has_child = false;
                item.callback = (item_cb_t) _proc_items_do_exposure_mode;
                if (num == g_camera_dv_cfgs[CAM_DV_CFG_TYPE_EXPOSURE_MODE].value)
                {
                    item.sel_icon_id = (int) g_cam_dv_opt_dot->pic_id;
                    item.unsel_icon_id = (int) g_cam_dv_opt_dot->pic_id;
                }
                else
                {
                    item.sel_icon_id = 0;
                    item.unsel_icon_id = 0;
                }
                menu_append_item(g_camera_dv_option_exposure_mode, &item);
            }
        }

        //init current index
        menu_set_cur_index(g_camera_dv_option_exposure_mode, 0);
    }

    return g_camera_dv_option_exposure_mode;
}

int camera_dv_option_exposure_mode_deinit(void)
{
    //delete menu
    if (g_camera_dv_option_exposure_mode != NULL)
    {
        //create menu
        menu_delete(g_camera_dv_option_exposure_mode);
        g_camera_dv_option_exposure_mode = NULL;
    }

    return true;
}
static int _proc_items_do_exposure_mode(menuview_t *menuview, menuview_sig_e signal)
{
    menu_t *cur_menu = menuview->menu;
    menuitem_t *cur_item = menu_get_cur_item(cur_menu);
    switch (signal)
    {
        case WIDGET_SIG_MENUVIEW_OK:
        {
            int cur_item_pos = menu_get_cur_index(cur_menu);//MMM_AUTO_BALANCE = 0?
            menuitem_t *save_opt_item = menu_get_item(cur_menu, g_camera_dv_cfgs[CAM_DV_CFG_TYPE_EXPOSURE_MODE].value);

            if (save_opt_item != NULL)
            {
                save_opt_item->sel_icon_id = 0;
                save_opt_item->unsel_icon_id = 0;
            }
            if (cur_item != NULL)
            {
                cur_item->sel_icon_id = (int) g_cam_dv_opt_dot->pic_id;
                cur_item->unsel_icon_id = (int) g_cam_dv_opt_dot->pic_id;
                print_dbg(">>>> menu_list: %s clicked", cur_item->text);                
            }
#if (DBG_MMM == 1)
            if (cur_item_pos> 2)//mmm_exposure_mode_t

            {
                cur_item_pos = 2;
            }
#endif
            //exit and save entry
            camera_dv_ctr_set_exposure_mode((unsigned int) cur_item_pos);
            /*! 与dv中的闪光灯样子差不多 */
            camera_dv_change_icon(CAM_ICON_EXPOSURE_MODE, (unsigned int)cur_item_pos);
            camera_dv_save_parameter(CAM_DV_PARA_EXPOSURE_MODE, cur_item_pos);
#if (MENU_SELECT_OK_HIDE == 1)
            camera_dv_widget_option_exit(menuview, camera_dv_option_exposure_mode_enter);
#else
            menuview_set_menu(menuview, cur_menu);
            menuview_paint(menuview);
#endif /* MENU_SELECT_OK_HIDE */
        }
        break;

        case WIDGET_SIG_MENUVIEW_QUIT:
        // return to parent menu
        camera_dv_option_exposure_mode_exit(menuview);
        break;

        default:
        break;
    }
    return 0;
}
